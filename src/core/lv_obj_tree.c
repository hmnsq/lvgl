/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_private.h"
#شامل "../lvgl_public.h"
#شامل "lv_obj_class_private.h"
#شامل "../indev/lv_indev_private.h"
#شامل "../display/lv_display_private.h"
#شامل "../misc/lv_anim_private.h"
#شامل "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#define disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#define OBJ_DUMP_STRING_LEN 128
#define LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ایستا تهی lv_obj_delete_async_cb(تهی * obj);
ایستا تهی obj_delete_core(lv_obj_t * obj);
ایستا lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, تهی * user_data);
ایستا تهی dump_درخت_core(lv_obj_t * obj, int32_t depth);
ایستا lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#اگر LV_USE_OBJ_NAME
    ایستا lv_obj_t * find_by_name_direct(ثابت lv_obj_t * parent, ثابت نویسه * name, size_t len);
#endif /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

تهی lv_obj_delete(lv_obj_t * obj)
{
    اگر(obj == پوچ) بازگشت;
    اگر(obj->is_deleting) بازگشت;

    LV_LOG_TRACE("begin (delete %p)", (تهی *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = پوچ;
    bool act_screen_del = false;
    اگر(par == پوچ) {
        disp = lv_obj_get_display(obj);
        اگر(!disp) بازگشت;   /*Shouldn't happen*/
        اگر(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    اگر(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, پوچ);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, پوچ);
    }

    /*Handle if the active screen was deleted*/
    اگر(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = پوچ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (تهی *)obj);
}

تهی lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (تهی *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_شمار(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    وقتی(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    اگر(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    اگر(lv_obj_get_child_شمار(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, پوچ);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, پوچ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (تهی *)obj);
}

تهی lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    اگر(obj == پوچ) بازگشت;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, پوچ);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

تهی lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != پوچ, بازگشت);

    lv_obj_delete(a->var);
}

تهی lv_obj_delete_async(lv_obj_t * obj)
{
    اگر(obj == پوچ) بازگشت;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

تهی lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);
    LV_CHECK_OBJ(parent, MY_CLASS, بازگشت);

    اگر(obj->parent == پوچ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        بازگشت;
    }

    اگر(parent == obj->parent) {
        بازگشت;
    }

    اگر(!lv_obj_allocate_spec_attr(parent)) {
        بازگشت;
    }

    /*Add the object to the new parents list first*/
    اگر(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        بازگشت;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, پوچ);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, پوچ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

تهی lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    اگر(!parent) {
        LV_LOG_WARN("parent is NULL");
        بازگشت;
    }

    ثابت uint32_t parent_child_شمار = lv_obj_get_child_شمار(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    اگر(index < 0) {
        index += parent_child_شمار;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    اگر((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_شمار)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        بازگشت;
    }

    int32_t i = old_index;
    اگر(index < old_index) {
        وقتی(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    وگرنه {
        وقتی(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, پوچ);
    lv_obj_invalidate(parent);
}

تهی lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, بازگشت);
    LV_CHECK_OBJ(obj2, MY_CLASS, بازگشت);

    lv_obj_t * parent = lv_obj_get_parent(obj1);
    lv_obj_t * parent2 = lv_obj_get_parent(obj2);

    uint_fast32_t index1 = lv_obj_get_index(obj1);
    uint_fast32_t index2 = lv_obj_get_index(obj2);

    lv_obj_send_event(parent2, LV_EVENT_CHILD_DELETED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_DELETED, obj1);

    parent->spec_attr->children[index1] = obj2;
    obj2->parent = parent;

    parent2->spec_attr->children[index2] = obj1;
    obj1->parent = parent2;

    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CHANGED, obj1);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CREATED, obj1);

    lv_obj_invalidate(parent);

    اگر(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    انجامبده {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } وقتی(par != پوچ);

    بازگشت (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);

    ثابت lv_obj_t * scr;

    اگر(obj->parent == پوچ) scr = obj;  /*`obj` is a screen*/
    وگرنه scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        برای(i = 0; i < d->screen_cnt; i++) {
            اگر(d->screens[i] == scr) بازگشت d;
        }
    }

    LV_LOG_WARN("No screen found");
    بازگشت پوچ;
}

lv_obj_t * lv_obj_get_parent(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);

    بازگشت obj->parent;
}

lv_obj_t * lv_obj_get_child(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);

    اگر(obj->spec_attr == پوچ) بازگشت پوچ;

    اگر(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        اگر(idx < 0) بازگشت پوچ;
    }

    uint32_t idu = (uint32_t) idx;

    اگر(idu < obj->spec_attr->child_cnt) بازگشت obj->spec_attr->children[idu];
    وگرنه بازگشت پوچ;
}

lv_obj_t * lv_obj_get_child_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);
    LV_CHECK_ARG(class_p != پوچ, بازگشت پوچ);

    اگر(obj->spec_attr == پوچ) بازگشت پوچ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    اگر(idx >= 0) {
        برای(i = 0; i < cnt; i++) {
            اگر(obj->spec_attr->children[i]->class_p == class_p) {
                اگر(idx == 0) بازگشت obj->spec_attr->children[i];
                وگرنه idx--;
            }
        }
    }
    وگرنه {
        idx++;   /*-1 means the first child*/
        برای(i = cnt - 1; i >= 0; i--) {
            اگر(obj->spec_attr->children[i]->class_p == class_p) {
                اگر(idx == 0) بازگشت obj->spec_attr->children[i];
                وگرنه idx++;
            }
        }
    }
    بازگشت پوچ;
}

lv_obj_t * lv_obj_get_sibling(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);
    LV_CHECK_ARG(obj->parent != پوچ, بازگشت پوچ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    اگر(sibling_idx < 0) بازگشت پوچ;

    بازگشت lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);
    LV_CHECK_ARG(obj->parent != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(class_p != پوچ, بازگشت پوچ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    اگر(sibling_idx < 0) بازگشت پوچ;

    بازگشت lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_شمار(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);
    اگر(obj->spec_attr == پوچ) بازگشت 0;
    بازگشت obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_شمار_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);
    LV_CHECK_ARG(class_p != پوچ, بازگشت 0);

    اگر(obj->spec_attr == پوچ) بازگشت 0;

    uint32_t i;
    uint32_t cnt = 0;
    برای(i = 0; i < obj->spec_attr->child_cnt; i++) {
        اگر(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    بازگشت cnt;
}

#اگر LV_USE_OBJ_NAME

تهی lv_obj_set_name(lv_obj_t * obj, ثابت نویسه * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }

    اگر(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((تهی *)obj->spec_attr->name);

    اگر(name == پوچ) {
        obj->spec_attr->name = پوچ;
        obj->spec_attr->name_static = 1;
    }
    وگرنه {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

تهی lv_obj_set_name_static(lv_obj_t * obj, ثابت نویسه * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);
    LV_CHECK_ARG(name != پوچ, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }
    اگر(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((تهی *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ثابت نویسه * lv_obj_get_name(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت پوچ);

    اگر(obj->spec_attr == پوچ) بازگشت پوچ;
    وگرنه بازگشت obj->spec_attr->name;
}

تهی lv_obj_get_name_resolved(ثابت lv_obj_t * obj, نویسه buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);
    LV_CHECK_ARG(buf != پوچ && buf_size > 0, بازگشت);

    ثابت نویسه * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    نویسه name_buf[LV_OBJ_NAME_MAX_LEN];
    اگر(name == پوچ) {
        lv_snprintf(name_buf, اندازه(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    اگر(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_شمار(parent);
        uint32_t cnt = 0;
        uint32_t i;
        برای(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            اگر(child == obj) {
                نویسه رقم_buf[8];
                size_t رقم_len;
                رقم_len = lv_snprintf(رقم_buf, اندازه(رقم_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                اگر(buf_size > name_len + رقم_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], رقم_buf);
                }
                وگرنه {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                بشکن;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            وگرنه {
                ثابت نویسه * child_name = lv_obj_get_name(child);
                اگر(child_name == پوچ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    اگر(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                وگرنه {
                    اگر(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    وگرنه {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ثابت lv_obj_t * parent, ثابت نویسه * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, بازگشت پوچ);
    LV_CHECK_ARG(path != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(parent->spec_attr != پوچ, بازگشت پوچ);

    وقتی(*path) {
        ثابت نویسه * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        وقتی(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        اگر(!child) بازگشت پوچ; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        اگر(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        اگر(*path == '\0') بازگشت child;

        parent = child;
    }

    بازگشت پوچ;

}

lv_obj_t * lv_obj_find_by_name(ثابت lv_obj_t * parent, ثابت نویسه * name)
{
    LV_CHECK_ARG(name != پوچ, بازگشت پوچ);

    اگر(parent == پوچ) parent = lv_display_get_screen_active(lv_display_get_default());
    اگر(parent == پوچ) بازگشت پوچ;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    اگر(child) بازگشت child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_شمار(parent);
    uint32_t i;
    برای(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        اگر(found != پوچ) بازگشت found;
    }

    بازگشت پوچ;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    اگر(parent == پوچ) بازگشت -1;

    uint32_t i = 0;
    برای(i = 0; i < parent->spec_attr->child_cnt; i++) {
        اگر(parent->spec_attr->children[i] == obj) بازگشت (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    بازگشت -1;
}

int32_t lv_obj_get_index_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت -1);
    LV_CHECK_ARG(class_p != پوچ, بازگشت -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    اگر(parent == پوچ) بازگشت 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    برای(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        اگر(child->class_p == class_p) {
            اگر(child == obj) بازگشت idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    بازگشت -1;
}

تهی lv_obj_درخت_walk(lv_obj_t * start_obj, lv_obj_درخت_walk_cb_t cb, تهی * user_data)
{
    LV_CHECK_ARG(cb != پوچ, بازگشت);

    walk_core(start_obj, cb, user_data);
}

تهی lv_obj_dump_درخت(lv_obj_t * start_obj)
{
    اگر(start_obj == پوچ) {
        lv_display_t * disp = lv_display_get_next(پوچ);
        وقتی(disp) {
            uint32_t i;
            برای(i = 0; i < disp->screen_cnt; i++) {
                dump_درخت_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    وگرنه {
        dump_درخت_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ایستا تهی lv_obj_delete_async_cb(تهی * obj)
{
    lv_obj_delete(obj);
}

ایستا تهی obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    اگر(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ایستا تهی obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != پوچ);
    اگر(obj->is_deleting)
        بازگشت;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, پوچ);
    اگر(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        بازگشت;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    اگر(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    اگر(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    وقتی(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(پوچ);
    وقتی(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        اگر(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            اگر(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            اگر(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = پوچ;
            }
            اگر(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = پوچ;
            }
        }

        اگر(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    وقتی(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    اگر(obj->parent == پوچ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        برای(i = 0; i < disp->screen_cnt; i++) {
            اگر(disp->screens[i] == obj) بشکن;
        }

        uint32_t id = i;
        برای(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * اندازه(lv_obj_t *));
        اگر(new_screens != پوچ) {
            disp->screens = new_screens;
        }
        وگرنه {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    وگرنه {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = پوچ;

    /*Free the object itself*/
    lv_free(obj);
}

ایستا lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, تهی * user_data)
{
    lv_obj_درخت_walk_res_t res = LV_OBJ_درخت_WALK_NEXT;

    اگر(obj == پوچ) {
        lv_display_t * disp = lv_display_get_next(پوچ);
        وقتی(disp) {
            uint32_t i;
            برای(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        بازگشت LV_OBJ_درخت_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    اگر(res == LV_OBJ_درخت_WALK_END) بازگشت LV_OBJ_درخت_WALK_END;

    اگر(res != LV_OBJ_درخت_WALK_SKIP_CHILDREN) {
        uint32_t i;
        برای(i = 0; i < lv_obj_get_child_شمار(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            اگر(res == LV_OBJ_درخت_WALK_END) بازگشت LV_OBJ_درخت_WALK_END;
        }
    }
    بازگشت LV_OBJ_درخت_WALK_NEXT;
}

ایستا تهی dump_درخت_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != پوچ);
#اگر LV_USE_LOG
    ثابت نویسه * id;

#اگر LV_USE_OBJ_ID
    نویسه buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, اندازه(buf));
    اگر(id == پوچ) id = "obj0";
#وگرنه
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحیح)(2 * depth), "", (تهی *)obj, id);
#endif /*LV_USE_LOG*/

    اگر(obj->spec_attr && obj->spec_attr->child_cnt) {
        برای(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_درخت_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

ایستا lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != پوچ);

    اگر(obj->spec_attr == پوچ) بازگشت پوچ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    برای(i = 0; i < cnt; i++) {
        اگر(!obj->spec_attr->children[i]->is_deleting) {
            بازگشت obj->spec_attr->children[i];
        }
    }

    بازگشت پوچ;
}

#اگر LV_USE_OBJ_NAME

ایستا lv_obj_t * find_by_name_direct(ثابت lv_obj_t * parent, ثابت نویسه * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_شمار(parent);
    برای(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        نویسه child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, اندازه(child_name_resolved));

        اگر(len == UINT16_MAX) {
            اگر(lv_strcmp(child_name_resolved, name) == 0) بازگشت child;
        }
        وگرنه {
            اگر(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                بازگشت child;
            }
        }
    }

    بازگشت پوچ;
}

#endif /*LV_USE_OBJ_NAME*/
