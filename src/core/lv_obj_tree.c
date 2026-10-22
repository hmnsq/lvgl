/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "../indev/lv_indev_private.h"
#अन्तर्भाव "../display/lv_display_private.h"
#अन्तर्भाव "../misc/lv_anim_private.h"
#अन्तर्भाव "../core/lv_global.h"

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
स्थिर शून्य lv_obj_delete_async_cb(शून्य * obj);
स्थिर शून्य obj_delete_core(lv_obj_t * obj);
स्थिर lv_obj_वृक्ष_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्ष_walk_cb_t cb, शून्य * user_data);
स्थिर शून्य dump_वृक्ष_core(lv_obj_t * obj, int32_t depth);
स्थिर lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#जर LV_USE_OBJ_NAME
    स्थिर lv_obj_t * find_by_name_direct(स्थिरांक lv_obj_t * parent, स्थिरांक अक्षर * name, माप_प्रकार len);
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

शून्य lv_obj_delete(lv_obj_t * obj)
{
    जर(obj == रिक्त) परत;
    जर(obj->is_deleting) परत;

    LV_LOG_TRACE("begin (delete %p)", (शून्य *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = रिक्त;
    bool act_screen_del = false;
    जर(par == रिक्त) {
        disp = lv_obj_get_display(obj);
        जर(!disp) परत;   /*Shouldn't happen*/
        जर(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    जर(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, रिक्त);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, रिक्त);
    }

    /*Handle if the active screen was deleted*/
    जर(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = रिक्त;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (शून्य *)obj);
}

शून्य lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (शून्य *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, परत);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_मोजणी(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    पर्यंत(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    जर(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    जर(lv_obj_get_child_मोजणी(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, रिक्त);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, रिक्त);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (शून्य *)obj);
}

शून्य lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    जर(obj == रिक्त) परत;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, रिक्त);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

शून्य lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != रिक्त, परत);

    lv_obj_delete(a->var);
}

शून्य lv_obj_delete_async(lv_obj_t * obj)
{
    जर(obj == रिक्त) परत;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

शून्य lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);
    LV_CHECK_OBJ(parent, MY_CLASS, परत);

    जर(obj->parent == रिक्त) {
        LV_LOG_WARN("Can't set the parent of a screen");
        परत;
    }

    जर(parent == obj->parent) {
        परत;
    }

    जर(!lv_obj_allocate_spec_attr(parent)) {
        परत;
    }

    /*Add the object to the new parents list first*/
    जर(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        परत;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, रिक्त);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, रिक्त);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

शून्य lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    जर(!parent) {
        LV_LOG_WARN("parent is NULL");
        परत;
    }

    स्थिरांक uint32_t parent_child_मोजणी = lv_obj_get_child_मोजणी(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    स्थिरांक int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    जर(index < 0) {
        index += parent_child_मोजणी;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    जर((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_मोजणी)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        परत;
    }

    int32_t i = old_index;
    जर(index < old_index) {
        पर्यंत(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    अन्यथा {
        पर्यंत(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, रिक्त);
    lv_obj_invalidate(parent);
}

शून्य lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, परत);
    LV_CHECK_OBJ(obj2, MY_CLASS, परत);

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

    जर(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);

    स्थिरांक lv_obj_t * par = obj;
    स्थिरांक lv_obj_t * act_par;

    करा {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } पर्यंत(par != रिक्त);

    परत (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);

    स्थिरांक lv_obj_t * scr;

    जर(obj->parent == रिक्त) scr = obj;  /*`obj` is a screen*/
    अन्यथा scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        साठी(i = 0; i < d->screen_cnt; i++) {
            जर(d->screens[i] == scr) परत d;
        }
    }

    LV_LOG_WARN("No screen found");
    परत रिक्त;
}

lv_obj_t * lv_obj_get_parent(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);

    परत obj->parent;
}

lv_obj_t * lv_obj_get_child(स्थिरांक lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);

    जर(obj->spec_attr == रिक्त) परत रिक्त;

    जर(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        जर(idx < 0) परत रिक्त;
    }

    uint32_t idu = (uint32_t) idx;

    जर(idu < obj->spec_attr->child_cnt) परत obj->spec_attr->children[idu];
    अन्यथा परत रिक्त;
}

lv_obj_t * lv_obj_get_child_by_type(स्थिरांक lv_obj_t * obj, int32_t idx, स्थिरांक lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);
    LV_CHECK_ARG(class_p != रिक्त, परत रिक्त);

    जर(obj->spec_attr == रिक्त) परत रिक्त;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    जर(idx >= 0) {
        साठी(i = 0; i < cnt; i++) {
            जर(obj->spec_attr->children[i]->class_p == class_p) {
                जर(idx == 0) परत obj->spec_attr->children[i];
                अन्यथा idx--;
            }
        }
    }
    अन्यथा {
        idx++;   /*-1 means the first child*/
        साठी(i = cnt - 1; i >= 0; i--) {
            जर(obj->spec_attr->children[i]->class_p == class_p) {
                जर(idx == 0) परत obj->spec_attr->children[i];
                अन्यथा idx++;
            }
        }
    }
    परत रिक्त;
}

lv_obj_t * lv_obj_get_sibling(स्थिरांक lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);
    LV_CHECK_ARG(obj->parent != रिक्त, परत रिक्त);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    जर(sibling_idx < 0) परत रिक्त;

    परत lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(स्थिरांक lv_obj_t * obj, int32_t idx, स्थिरांक lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);
    LV_CHECK_ARG(obj->parent != रिक्त, परत रिक्त);
    LV_CHECK_ARG(class_p != रिक्त, परत रिक्त);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    जर(sibling_idx < 0) परत रिक्त;

    परत lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_मोजणी(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत 0);
    जर(obj->spec_attr == रिक्त) परत 0;
    परत obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_मोजणी_by_type(स्थिरांक lv_obj_t * obj, स्थिरांक lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत 0);
    LV_CHECK_ARG(class_p != रिक्त, परत 0);

    जर(obj->spec_attr == रिक्त) परत 0;

    uint32_t i;
    uint32_t cnt = 0;
    साठी(i = 0; i < obj->spec_attr->child_cnt; i++) {
        जर(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    परत cnt;
}

#जर LV_USE_OBJ_NAME

शून्य lv_obj_set_name(lv_obj_t * obj, स्थिरांक अक्षर * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);

    जर(!lv_obj_allocate_spec_attr(obj)) {
        परत;
    }

    जर(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((शून्य *)obj->spec_attr->name);

    जर(name == रिक्त) {
        obj->spec_attr->name = रिक्त;
        obj->spec_attr->name_static = 1;
    }
    अन्यथा {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

शून्य lv_obj_set_name_static(lv_obj_t * obj, स्थिरांक अक्षर * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);
    LV_CHECK_ARG(name != रिक्त, परत);

    जर(!lv_obj_allocate_spec_attr(obj)) {
        परत;
    }
    जर(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((शून्य *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

स्थिरांक अक्षर * lv_obj_get_name(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत रिक्त);

    जर(obj->spec_attr == रिक्त) परत रिक्त;
    अन्यथा परत obj->spec_attr->name;
}

शून्य lv_obj_get_name_resolved(स्थिरांक lv_obj_t * obj, अक्षर buf[], माप_प्रकार buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);
    LV_CHECK_ARG(buf != रिक्त && buf_size > 0, परत);

    स्थिरांक अक्षर * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    अक्षर name_buf[LV_OBJ_NAME_MAX_LEN];
    जर(name == रिक्त) {
        lv_snprintf(name_buf, आकार(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    माप_प्रकार name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    जर(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_मोजणी(parent);
        uint32_t cnt = 0;
        uint32_t i;
        साठी(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            जर(child == obj) {
                अक्षर अंक_buf[8];
                माप_प्रकार अंक_len;
                अंक_len = lv_snprintf(अंक_buf, आकार(अंक_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                जर(buf_size > name_len + अंक_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], अंक_buf);
                }
                अन्यथा {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                थांबा;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            अन्यथा {
                स्थिरांक अक्षर * child_name = lv_obj_get_name(child);
                जर(child_name == रिक्त) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    माप_प्रकार class_name_len = lv_strlen(child->class_p->name);
                    जर(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                अन्यथा {
                    जर(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    अन्यथा {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(स्थिरांक lv_obj_t * parent, स्थिरांक अक्षर * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, परत रिक्त);
    LV_CHECK_ARG(path != रिक्त, परत रिक्त);
    LV_CHECK_ARG(parent->spec_attr != रिक्त, परत रिक्त);

    पर्यंत(*path) {
        स्थिरांक अक्षर * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        पर्यंत(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        जर(!child) परत रिक्त; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        जर(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        जर(*path == '\0') परत child;

        parent = child;
    }

    परत रिक्त;

}

lv_obj_t * lv_obj_find_by_name(स्थिरांक lv_obj_t * parent, स्थिरांक अक्षर * name)
{
    LV_CHECK_ARG(name != रिक्त, परत रिक्त);

    जर(parent == रिक्त) parent = lv_display_get_screen_active(lv_display_get_default());
    जर(parent == रिक्त) परत रिक्त;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    जर(child) परत child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_मोजणी(parent);
    uint32_t i;
    साठी(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        जर(found != रिक्त) परत found;
    }

    परत रिक्त;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(स्थिरांक lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    जर(parent == रिक्त) परत -1;

    uint32_t i = 0;
    साठी(i = 0; i < parent->spec_attr->child_cnt; i++) {
        जर(parent->spec_attr->children[i] == obj) परत (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    परत -1;
}

int32_t lv_obj_get_index_by_type(स्थिरांक lv_obj_t * obj, स्थिरांक lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत -1);
    LV_CHECK_ARG(class_p != रिक्त, परत -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    जर(parent == रिक्त) परत 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    साठी(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        जर(child->class_p == class_p) {
            जर(child == obj) परत idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    परत -1;
}

शून्य lv_obj_वृक्ष_walk(lv_obj_t * start_obj, lv_obj_वृक्ष_walk_cb_t cb, शून्य * user_data)
{
    LV_CHECK_ARG(cb != रिक्त, परत);

    walk_core(start_obj, cb, user_data);
}

शून्य lv_obj_dump_वृक्ष(lv_obj_t * start_obj)
{
    जर(start_obj == रिक्त) {
        lv_display_t * disp = lv_display_get_next(रिक्त);
        पर्यंत(disp) {
            uint32_t i;
            साठी(i = 0; i < disp->screen_cnt; i++) {
                dump_वृक्ष_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    अन्यथा {
        dump_वृक्ष_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर शून्य lv_obj_delete_async_cb(शून्य * obj)
{
    lv_obj_delete(obj);
}

स्थिर शून्य obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    जर(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

स्थिर शून्य obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != रिक्त);
    जर(obj->is_deleting)
        परत;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, रिक्त);
    जर(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        परत;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    जर(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    जर(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    पर्यंत(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(रिक्त);
    पर्यंत(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        जर(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            जर(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            जर(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = रिक्त;
            }
            जर(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = रिक्त;
            }
        }

        जर(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    पर्यंत(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    जर(obj->parent == रिक्त) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        साठी(i = 0; i < disp->screen_cnt; i++) {
            जर(disp->screens[i] == obj) थांबा;
        }

        uint32_t id = i;
        साठी(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * आकार(lv_obj_t *));
        जर(new_screens != रिक्त) {
            disp->screens = new_screens;
        }
        अन्यथा {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    अन्यथा {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = रिक्त;

    /*Free the object itself*/
    lv_free(obj);
}

स्थिर lv_obj_वृक्ष_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्ष_walk_cb_t cb, शून्य * user_data)
{
    lv_obj_वृक्ष_walk_res_t res = LV_OBJ_वृक्ष_WALK_NEXT;

    जर(obj == रिक्त) {
        lv_display_t * disp = lv_display_get_next(रिक्त);
        पर्यंत(disp) {
            uint32_t i;
            साठी(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        परत LV_OBJ_वृक्ष_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    जर(res == LV_OBJ_वृक्ष_WALK_END) परत LV_OBJ_वृक्ष_WALK_END;

    जर(res != LV_OBJ_वृक्ष_WALK_SKIP_CHILDREN) {
        uint32_t i;
        साठी(i = 0; i < lv_obj_get_child_मोजणी(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            जर(res == LV_OBJ_वृक्ष_WALK_END) परत LV_OBJ_वृक्ष_WALK_END;
        }
    }
    परत LV_OBJ_वृक्ष_WALK_NEXT;
}

स्थिर शून्य dump_वृक्ष_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != रिक्त);
#जर LV_USE_LOG
    स्थिरांक अक्षर * id;

#जर LV_USE_OBJ_ID
    अक्षर buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, आकार(buf));
    जर(id == रिक्त) id = "obj0";
#अन्यथा
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (पूर्णांक)(2 * depth), "", (शून्य *)obj, id);
#endif /*LV_USE_LOG*/

    जर(obj->spec_attr && obj->spec_attr->child_cnt) {
        साठी(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_वृक्ष_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

स्थिर lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != रिक्त);

    जर(obj->spec_attr == रिक्त) परत रिक्त;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    साठी(i = 0; i < cnt; i++) {
        जर(!obj->spec_attr->children[i]->is_deleting) {
            परत obj->spec_attr->children[i];
        }
    }

    परत रिक्त;
}

#जर LV_USE_OBJ_NAME

स्थिर lv_obj_t * find_by_name_direct(स्थिरांक lv_obj_t * parent, स्थिरांक अक्षर * name, माप_प्रकार len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_मोजणी(parent);
    साठी(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        अक्षर child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, आकार(child_name_resolved));

        जर(len == UINT16_MAX) {
            जर(lv_strcmp(child_name_resolved, name) == 0) परत child;
        }
        अन्यथा {
            जर(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                परत child;
            }
        }
    }

    परत रिक्त;
}

#endif /*LV_USE_OBJ_NAME*/
