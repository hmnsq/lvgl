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
ساكن lv_obj_شجرة_walk_res_t walk_core(lv_obj_t * obj, lv_obj_شجرة_walk_cb_t cb, فراغ * user_data);
ساكن فراغ dump_شجرة_core(lv_obj_t * obj, int32_t depth);
ساكن lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#إذا LV_USE_OBJ_NAME
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
    إذا(obj == عدم) أرجع;
    إذا(obj->is_deleting) أرجع;

    LV_LOG_TRACE("begin (delete %p)", (فراغ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = عدم;
    bool act_screen_del = false;
    إذا(par == عدم) {
        disp = lv_obj_get_display(obj);
        إذا(!disp) أرجع;   /*Shouldn't happen*/
        إذا(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    إذا(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, عدم);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, عدم);
    }

    /*Handle if the active screen was deleted*/
    إذا(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = عدم;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (فراغ *)obj);
}

فراغ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (فراغ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_عدد(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    طالما(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    إذا(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    إذا(lv_obj_get_child_عدد(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, عدم);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, عدم);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (فراغ *)obj);
}

فراغ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    إذا(obj == عدم) أرجع;

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
    LV_CHECK_ARG(a != عدم, أرجع);

    lv_obj_delete(a->var);
}

فراغ lv_obj_delete_async(lv_obj_t * obj)
{
    إذا(obj == عدم) أرجع;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

فراغ lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_OBJ(parent, MY_CLASS, أرجع);

    إذا(obj->parent == عدم) {
        LV_LOG_WARN("Can't set the parent of a screen");
        أرجع;
    }

    إذا(parent == obj->parent) {
        أرجع;
    }

    إذا(!lv_obj_allocate_spec_attr(parent)) {
        أرجع;
    }

    /*Add the object to the new parents list first*/
    إذا(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        أرجع;
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
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    إذا(!parent) {
        LV_LOG_WARN("parent is NULL");
        أرجع;
    }

    ثابت uint32_t parent_child_عدد = lv_obj_get_child_عدد(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    إذا(index < 0) {
        index += parent_child_عدد;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    إذا((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_عدد)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        أرجع;
    }

    int32_t i = old_index;
    إذا(index < old_index) {
        طالما(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    وإلا {
        طالما(i < index) {
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
    LV_CHECK_OBJ(obj1, MY_CLASS, أرجع);
    LV_CHECK_OBJ(obj2, MY_CLASS, أرجع);

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

    إذا(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    افعل {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } طالما(par != عدم);

    أرجع (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * scr;

    إذا(obj->parent == عدم) scr = obj;  /*`obj` is a screen*/
    وإلا scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        لكل(i = 0; i < d->screen_cnt; i++) {
            إذا(d->screens[i] == scr) أرجع d;
        }
    }

    LV_LOG_WARN("No screen found");
    أرجع عدم;
}

lv_obj_t * lv_obj_get_parent(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    أرجع obj->parent;
}

lv_obj_t * lv_obj_get_child(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    إذا(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        إذا(idx < 0) أرجع عدم;
    }

    uint32_t idu = (uint32_t) idx;

    إذا(idu < obj->spec_attr->child_cnt) أرجع obj->spec_attr->children[idu];
    وإلا أرجع عدم;
}

lv_obj_t * lv_obj_get_child_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    إذا(idx >= 0) {
        لكل(i = 0; i < cnt; i++) {
            إذا(obj->spec_attr->children[i]->class_p == class_p) {
                إذا(idx == 0) أرجع obj->spec_attr->children[i];
                وإلا idx--;
            }
        }
    }
    وإلا {
        idx++;   /*-1 means the first child*/
        لكل(i = cnt - 1; i >= 0; i--) {
            إذا(obj->spec_attr->children[i]->class_p == class_p) {
                إذا(idx == 0) أرجع obj->spec_attr->children[i];
                وإلا idx++;
            }
        }
    }
    أرجع عدم;
}

lv_obj_t * lv_obj_get_sibling(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(obj->parent != عدم, أرجع عدم);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(obj->parent != عدم, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_عدد(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_عدد_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    LV_CHECK_ARG(class_p != عدم, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;

    uint32_t i;
    uint32_t cnt = 0;
    لكل(i = 0; i < obj->spec_attr->child_cnt; i++) {
        إذا(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    أرجع cnt;
}

#إذا LV_USE_OBJ_NAME

فراغ lv_obj_set_name(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    إذا(name == عدم) {
        obj->spec_attr->name = عدم;
        obj->spec_attr->name_static = 1;
    }
    وإلا {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

فراغ lv_obj_set_name_static(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(name != عدم, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    إذا(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ثابت حرف * lv_obj_get_name(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;
    وإلا أرجع obj->spec_attr->name;
}

فراغ lv_obj_get_name_resolved(ثابت lv_obj_t * obj, حرف buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(buf != عدم && buf_size > 0, أرجع);

    ثابت حرف * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    حرف name_buf[LV_OBJ_NAME_MAX_LEN];
    إذا(name == عدم) {
        lv_snprintf(name_buf, حجم(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    إذا(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_عدد(parent);
        uint32_t cnt = 0;
        uint32_t i;
        لكل(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            إذا(child == obj) {
                حرف رقم_buf[8];
                size_t رقم_len;
                رقم_len = lv_snprintf(رقم_buf, حجم(رقم_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                إذا(buf_size > name_len + رقم_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], رقم_buf);
                }
                وإلا {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                اكسر;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            وإلا {
                ثابت حرف * child_name = lv_obj_get_name(child);
                إذا(child_name == عدم) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    إذا(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                وإلا {
                    إذا(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    وإلا {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ثابت lv_obj_t * parent, ثابت حرف * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(path != عدم, أرجع عدم);
    LV_CHECK_ARG(parent->spec_attr != عدم, أرجع عدم);

    طالما(*path) {
        ثابت حرف * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        طالما(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        إذا(!child) أرجع عدم; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        إذا(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        إذا(*path == '\0') أرجع child;

        parent = child;
    }

    أرجع عدم;

}

lv_obj_t * lv_obj_find_by_name(ثابت lv_obj_t * parent, ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع عدم);

    إذا(parent == عدم) parent = lv_display_get_screen_active(lv_display_get_default());
    إذا(parent == عدم) أرجع عدم;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    إذا(child) أرجع child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_عدد(parent);
    uint32_t i;
    لكل(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        إذا(found != عدم) أرجع found;
    }

    أرجع عدم;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    إذا(parent == عدم) أرجع -1;

    uint32_t i = 0;
    لكل(i = 0; i < parent->spec_attr->child_cnt; i++) {
        إذا(parent->spec_attr->children[i] == obj) أرجع (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    أرجع -1;
}

int32_t lv_obj_get_index_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);
    LV_CHECK_ARG(class_p != عدم, أرجع -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    إذا(parent == عدم) أرجع 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    لكل(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        إذا(child->class_p == class_p) {
            إذا(child == obj) أرجع idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    أرجع -1;
}

فراغ lv_obj_شجرة_walk(lv_obj_t * start_obj, lv_obj_شجرة_walk_cb_t cb, فراغ * user_data)
{
    LV_CHECK_ARG(cb != عدم, أرجع);

    walk_core(start_obj, cb, user_data);
}

فراغ lv_obj_dump_شجرة(lv_obj_t * start_obj)
{
    إذا(start_obj == عدم) {
        lv_display_t * disp = lv_display_get_next(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                dump_شجرة_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    وإلا {
        dump_شجرة_core(start_obj, 0);
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
    إذا(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ساكن فراغ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);
    إذا(obj->is_deleting)
        أرجع;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, عدم);
    إذا(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        أرجع;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    إذا(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    إذا(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    طالما(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(عدم);
    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        إذا(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            إذا(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            إذا(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = عدم;
            }
            إذا(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = عدم;
            }
        }

        إذا(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    طالما(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    إذا(obj->parent == عدم) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        لكل(i = 0; i < disp->screen_cnt; i++) {
            إذا(disp->screens[i] == obj) اكسر;
        }

        uint32_t id = i;
        لكل(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * حجم(lv_obj_t *));
        إذا(new_screens != عدم) {
            disp->screens = new_screens;
        }
        وإلا {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    وإلا {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = عدم;

    /*Free the object itself*/
    lv_free(obj);
}

ساكن lv_obj_شجرة_walk_res_t walk_core(lv_obj_t * obj, lv_obj_شجرة_walk_cb_t cb, فراغ * user_data)
{
    lv_obj_شجرة_walk_res_t res = LV_OBJ_شجرة_WALK_NEXT;

    إذا(obj == عدم) {
        lv_display_t * disp = lv_display_get_next(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        أرجع LV_OBJ_شجرة_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    إذا(res == LV_OBJ_شجرة_WALK_END) أرجع LV_OBJ_شجرة_WALK_END;

    إذا(res != LV_OBJ_شجرة_WALK_SKIP_CHILDREN) {
        uint32_t i;
        لكل(i = 0; i < lv_obj_get_child_عدد(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            إذا(res == LV_OBJ_شجرة_WALK_END) أرجع LV_OBJ_شجرة_WALK_END;
        }
    }
    أرجع LV_OBJ_شجرة_WALK_NEXT;
}

ساكن فراغ dump_شجرة_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != عدم);
#إذا LV_USE_LOG
    ثابت حرف * id;

#إذا LV_USE_OBJ_ID
    حرف buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, حجم(buf));
    إذا(id == عدم) id = "obj0";
#وإلا
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحيح)(2 * depth), "", (فراغ *)obj, id);
#endif /*LV_USE_LOG*/

    إذا(obj->spec_attr && obj->spec_attr->child_cnt) {
        لكل(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_شجرة_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

ساكن lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    لكل(i = 0; i < cnt; i++) {
        إذا(!obj->spec_attr->children[i]->is_deleting) {
            أرجع obj->spec_attr->children[i];
        }
    }

    أرجع عدم;
}

#إذا LV_USE_OBJ_NAME

ساكن lv_obj_t * find_by_name_direct(ثابت lv_obj_t * parent, ثابت حرف * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_عدد(parent);
    لكل(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        حرف child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, حجم(child_name_resolved));

        إذا(len == UINT16_MAX) {
            إذا(lv_strcmp(child_name_resolved, name) == 0) أرجع child;
        }
        وإلا {
            إذا(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                أرجع child;
            }
        }
    }

    أرجع عدم;
}

#endif /*LV_USE_OBJ_NAME*/
