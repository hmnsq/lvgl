/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_private.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../indev/lv_indev_private.h"
#ضمن "../display/lv_display_private.h"
#ضمن "../misc/lv_anim_private.h"
#ضمن "../core/lv_global.h"

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
ساكن فراغ lv_obj_delete_async_cb(فراغ * obj);
ساكن فراغ obj_delete_core(lv_obj_t * obj);
ساكن lv_obj_tree_walk_res_t walk_core(lv_obj_t * obj, lv_obj_tree_walk_cb_t cb, فراغ * user_data);
ساكن فراغ dump_tree_core(lv_obj_t * obj, int32_t depth);
ساكن lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#ܐܢ LV_USE_OBJ_NAME
    ساكن lv_obj_t * find_by_name_direct(ثابت lv_obj_t * parent, ثابت حرف * name, size_t len);
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

فراغ lv_obj_delete(lv_obj_t * obj)
{
    ܐܢ(obj == عدم) ܦܢܐ;
    ܐܢ(obj->is_deleting) ܦܢܐ;

    LV_LOG_TRACE("begin (delete %p)", (فراغ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = عدم;
    bool act_screen_del = false;
    ܐܢ(par == عدم) {
        disp = lv_obj_get_display(obj);
        ܐܢ(!disp) ܦܢܐ;   /*Shouldn't happen*/
        ܐܢ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ܐܢ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, عدم);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, عدم);
    }

    /*Handle if the active screen was deleted*/
    ܐܢ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = عدم;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (فراغ *)obj);
}

فراغ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (فراغ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_count(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    ܟܡܐ(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    ܐܢ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ܐܢ(lv_obj_get_child_count(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, عدم);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, عدم);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (فراغ *)obj);
}

فراغ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ܐܢ(obj == عدم) ܦܢܐ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, عدم);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

فراغ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != عدم, ܦܢܐ);

    lv_obj_delete(a->var);
}

فراغ lv_obj_delete_async(lv_obj_t * obj)
{
    ܐܢ(obj == عدم) ܦܢܐ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

فراغ lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);
    LV_CHECK_OBJ(parent, MY_CLASS, ܦܢܐ);

    ܐܢ(obj->parent == عدم) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ܦܢܐ;
    }

    ܐܢ(parent == obj->parent) {
        ܦܢܐ;
    }

    ܐܢ(!lv_obj_allocate_spec_attr(parent)) {
        ܦܢܐ;
    }

    /*Add the object to the new parents list first*/
    ܐܢ(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ܦܢܐ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, عدم);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, عدم);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

فراغ lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    ܐܢ(!parent) {
        LV_LOG_WARN("parent is NULL");
        ܦܢܐ;
    }

    ثابت uint32_t parent_child_count = lv_obj_get_child_count(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    ܐܢ(index < 0) {
        index += parent_child_count;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ܐܢ((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_count)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        ܦܢܐ;
    }

    int32_t i = old_index;
    ܐܢ(index < old_index) {
        ܟܡܐ(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    ܐܚܪܢܐ {
        ܟܡܐ(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, عدم);
    lv_obj_invalidate(parent);
}

فراغ lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ܦܢܐ);
    LV_CHECK_OBJ(obj2, MY_CLASS, ܦܢܐ);

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

    ܐܢ(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    ܥܒܕ {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } ܟܡܐ(par != عدم);

    ܦܢܐ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);

    ثابت lv_obj_t * scr;

    ܐܢ(obj->parent == عدم) scr = obj;  /*`obj` is a screen*/
    ܐܚܪܢܐ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        لكل(i = 0; i < d->screen_cnt; i++) {
            ܐܢ(d->screens[i] == scr) ܦܢܐ d;
        }
    }

    LV_LOG_WARN("No screen found");
    ܦܢܐ عدم;
}

lv_obj_t * lv_obj_get_parent(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);

    ܦܢܐ obj->parent;
}

lv_obj_t * lv_obj_get_child(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ عدم;

    ܐܢ(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        ܐܢ(idx < 0) ܦܢܐ عدم;
    }

    uint32_t idu = (uint32_t) idx;

    ܐܢ(idu < obj->spec_attr->child_cnt) ܦܢܐ obj->spec_attr->children[idu];
    ܐܚܪܢܐ ܦܢܐ عدم;
}

lv_obj_t * lv_obj_get_child_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);
    LV_CHECK_ARG(class_p != عدم, ܦܢܐ عدم);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ܐܢ(idx >= 0) {
        لكل(i = 0; i < cnt; i++) {
            ܐܢ(obj->spec_attr->children[i]->class_p == class_p) {
                ܐܢ(idx == 0) ܦܢܐ obj->spec_attr->children[i];
                ܐܚܪܢܐ idx--;
            }
        }
    }
    ܐܚܪܢܐ {
        idx++;   /*-1 means the first child*/
        لكل(i = cnt - 1; i >= 0; i--) {
            ܐܢ(obj->spec_attr->children[i]->class_p == class_p) {
                ܐܢ(idx == 0) ܦܢܐ obj->spec_attr->children[i];
                ܐܚܪܢܐ idx++;
            }
        }
    }
    ܦܢܐ عدم;
}

lv_obj_t * lv_obj_get_sibling(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);
    LV_CHECK_ARG(obj->parent != عدم, ܦܢܐ عدم);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    ܐܢ(sibling_idx < 0) ܦܢܐ عدم;

    ܦܢܐ lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);
    LV_CHECK_ARG(obj->parent != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG(class_p != عدم, ܦܢܐ عدم);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    ܐܢ(sibling_idx < 0) ܦܢܐ عدم;

    ܦܢܐ lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_count(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ 0);
    ܐܢ(obj->spec_attr == عدم) ܦܢܐ 0;
    ܦܢܐ obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_count_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ 0);
    LV_CHECK_ARG(class_p != عدم, ܦܢܐ 0);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ 0;

    uint32_t i;
    uint32_t cnt = 0;
    لكل(i = 0; i < obj->spec_attr->child_cnt; i++) {
        ܐܢ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ܦܢܐ cnt;
}

#ܐܢ LV_USE_OBJ_NAME

فراغ lv_obj_set_name(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);

    ܐܢ(!lv_obj_allocate_spec_attr(obj)) {
        ܦܢܐ;
    }

    ܐܢ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    ܐܢ(name == عدم) {
        obj->spec_attr->name = عدم;
        obj->spec_attr->name_static = 1;
    }
    ܐܚܪܢܐ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

فراغ lv_obj_set_name_static(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);
    LV_CHECK_ARG(name != عدم, ܦܢܐ);

    ܐܢ(!lv_obj_allocate_spec_attr(obj)) {
        ܦܢܐ;
    }
    ܐܢ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ثابت حرف * lv_obj_get_name(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ عدم);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ عدم;
    ܐܚܪܢܐ ܦܢܐ obj->spec_attr->name;
}

فراغ lv_obj_get_name_resolved(ثابت lv_obj_t * obj, حرف buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ);
    LV_CHECK_ARG(buf != عدم && buf_size > 0, ܦܢܐ);

    ثابت حرف * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    حرف name_buf[LV_OBJ_NAME_MAX_LEN];
    ܐܢ(name == عدم) {
        lv_snprintf(name_buf, حجم(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ܐܢ(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_count(parent);
        uint32_t cnt = 0;
        uint32_t i;
        لكل(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ܐܢ(child == obj) {
                حرف num_buf[8];
                size_t num_len;
                num_len = lv_snprintf(num_buf, حجم(num_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ܐܢ(buf_size > name_len + num_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], num_buf);
                }
                ܐܚܪܢܐ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                اكسر;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ܐܚܪܢܐ {
                ثابت حرف * child_name = lv_obj_get_name(child);
                ܐܢ(child_name == عدم) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    ܐܢ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ܐܚܪܢܐ {
                    ܐܢ(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ܐܚܪܢܐ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ثابت lv_obj_t * parent, ثابت حرف * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, ܦܢܐ عدم);
    LV_CHECK_ARG(path != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG(parent->spec_attr != عدم, ܦܢܐ عدم);

    ܟܡܐ(*path) {
        ثابت حرف * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ܟܡܐ(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        ܐܢ(!child) ܦܢܐ عدم; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        ܐܢ(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ܐܢ(*path == '\0') ܦܢܐ child;

        parent = child;
    }

    ܦܢܐ عدم;

}

lv_obj_t * lv_obj_find_by_name(ثابت lv_obj_t * parent, ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, ܦܢܐ عدم);

    ܐܢ(parent == عدم) parent = lv_display_get_screen_active(lv_display_get_default());
    ܐܢ(parent == عدم) ܦܢܐ عدم;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    ܐܢ(child) ܦܢܐ child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_count(parent);
    uint32_t i;
    لكل(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        ܐܢ(found != عدم) ܦܢܐ found;
    }

    ܦܢܐ عدم;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ܐܢ(parent == عدم) ܦܢܐ -1;

    uint32_t i = 0;
    لكل(i = 0; i < parent->spec_attr->child_cnt; i++) {
        ܐܢ(parent->spec_attr->children[i] == obj) ܦܢܐ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ܦܢܐ -1;
}

int32_t lv_obj_get_index_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ -1);
    LV_CHECK_ARG(class_p != عدم, ܦܢܐ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ܐܢ(parent == عدم) ܦܢܐ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    لكل(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        ܐܢ(child->class_p == class_p) {
            ܐܢ(child == obj) ܦܢܐ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ܦܢܐ -1;
}

فراغ lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, فراغ * user_data)
{
    LV_CHECK_ARG(cb != عدم, ܦܢܐ);

    walk_core(start_obj, cb, user_data);
}

فراغ lv_obj_dump_tree(lv_obj_t * start_obj)
{
    ܐܢ(start_obj == عدم) {
        lv_display_t * disp = lv_display_get_next(عدم);
        ܟܡܐ(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                dump_tree_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    ܐܚܪܢܐ {
        dump_tree_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_obj_delete_async_cb(فراغ * obj)
{
    lv_obj_delete(obj);
}

ساكن فراغ obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    ܐܢ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ساكن فراغ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);
    ܐܢ(obj->is_deleting)
        ܦܢܐ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, عدم);
    ܐܢ(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        ܦܢܐ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ܐܢ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ܐܢ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    ܟܡܐ(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(عدم);
    ܟܡܐ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ܐܢ(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            ܐܢ(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ܐܢ(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = عدم;
            }
            ܐܢ(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = عدم;
            }
        }

        ܐܢ(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    ܟܡܐ(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ܐܢ(obj->parent == عدم) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        لكل(i = 0; i < disp->screen_cnt; i++) {
            ܐܢ(disp->screens[i] == obj) اكسر;
        }

        uint32_t id = i;
        لكل(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * حجم(lv_obj_t *));
        ܐܢ(new_screens != عدم) {
            disp->screens = new_screens;
        }
        ܐܚܪܢܐ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ܐܚܪܢܐ {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = عدم;

    /*Free the object itself*/
    lv_free(obj);
}

ساكن lv_obj_tree_walk_res_t walk_core(lv_obj_t * obj, lv_obj_tree_walk_cb_t cb, فراغ * user_data)
{
    lv_obj_tree_walk_res_t res = LV_OBJ_TREE_WALK_NEXT;

    ܐܢ(obj == عدم) {
        lv_display_t * disp = lv_display_get_next(عدم);
        ܟܡܐ(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        ܦܢܐ LV_OBJ_TREE_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    ܐܢ(res == LV_OBJ_TREE_WALK_END) ܦܢܐ LV_OBJ_TREE_WALK_END;

    ܐܢ(res != LV_OBJ_TREE_WALK_SKIP_CHILDREN) {
        uint32_t i;
        لكل(i = 0; i < lv_obj_get_child_count(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            ܐܢ(res == LV_OBJ_TREE_WALK_END) ܦܢܐ LV_OBJ_TREE_WALK_END;
        }
    }
    ܦܢܐ LV_OBJ_TREE_WALK_NEXT;
}

ساكن فراغ dump_tree_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != عدم);
#ܐܢ LV_USE_LOG
    ثابت حرف * id;

#ܐܢ LV_USE_OBJ_ID
    حرف buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, حجم(buf));
    ܐܢ(id == عدم) id = "obj0";
#ܐܚܪܢܐ
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحيح)(2 * depth), "", (فراغ *)obj, id);
#endif /*LV_USE_LOG*/

    ܐܢ(obj->spec_attr && obj->spec_attr->child_cnt) {
        لكل(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_tree_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

ساكن lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    لكل(i = 0; i < cnt; i++) {
        ܐܢ(!obj->spec_attr->children[i]->is_deleting) {
            ܦܢܐ obj->spec_attr->children[i];
        }
    }

    ܦܢܐ عدم;
}

#ܐܢ LV_USE_OBJ_NAME

ساكن lv_obj_t * find_by_name_direct(ثابت lv_obj_t * parent, ثابت حرف * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(parent);
    لكل(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        حرف child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, حجم(child_name_resolved));

        ܐܢ(len == UINT16_MAX) {
            ܐܢ(lv_strcmp(child_name_resolved, name) == 0) ܦܢܐ child;
        }
        ܐܚܪܢܐ {
            ܐܢ(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                ܦܢܐ child;
            }
        }
    }

    ܦܢܐ عدم;
}

#endif /*LV_USE_OBJ_NAME*/
