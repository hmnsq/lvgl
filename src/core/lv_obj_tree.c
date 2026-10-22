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
स्थिर lv_obj_वृक्षः_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्षः_walk_cb_t cb, शून्य * user_data);
स्थिर शून्य dump_वृक्षः_core(lv_obj_t * obj, int32_t depth);
स्थिर lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#सचे LV_USE_OBJ_NAME
    स्थिर lv_obj_t * find_by_name_direct(ध्रुव lv_obj_t * parent, ध्रुव वर्ण * name, size_t len);
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
    सचे(obj == रिक्त) पटिनिवत्त;
    सचे(obj->is_deleting) पटिनिवत्त;

    LV_LOG_TRACE("begin (delete %p)", (शून्य *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = रिक्त;
    bool act_screen_del = false;
    सचे(par == रिक्त) {
        disp = lv_obj_get_display(obj);
        सचे(!disp) पटिनिवत्त;   /*Shouldn't happen*/
        सचे(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    सचे(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, रिक्त);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, रिक्त);
    }

    /*Handle if the active screen was deleted*/
    सचे(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = रिक्त;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (शून्य *)obj);
}

शून्य lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (शून्य *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_गणनम्(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    याव(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    सचे(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    सचे(lv_obj_get_child_गणनम्(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, रिक्त);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, रिक्त);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (शून्य *)obj);
}

शून्य lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    सचे(obj == रिक्त) पटिनिवत्त;

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
    LV_CHECK_ARG(a != रिक्त, पटिनिवत्त);

    lv_obj_delete(a->var);
}

शून्य lv_obj_delete_async(lv_obj_t * obj)
{
    सचे(obj == रिक्त) पटिनिवत्त;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

शून्य lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);
    LV_CHECK_OBJ(parent, MY_CLASS, पटिनिवत्त);

    सचे(obj->parent == रिक्त) {
        LV_LOG_WARN("Can't set the parent of a screen");
        पटिनिवत्त;
    }

    सचे(parent == obj->parent) {
        पटिनिवत्त;
    }

    सचे(!lv_obj_allocate_spec_attr(parent)) {
        पटिनिवत्त;
    }

    /*Add the object to the new parents list first*/
    सचे(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        पटिनिवत्त;
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
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    सचे(!parent) {
        LV_LOG_WARN("parent is NULL");
        पटिनिवत्त;
    }

    ध्रुव uint32_t parent_child_गणनम् = lv_obj_get_child_गणनम्(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ध्रुव int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    सचे(index < 0) {
        index += parent_child_गणनम्;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    सचे((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_गणनम्)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        पटिनिवत्त;
    }

    int32_t i = old_index;
    सचे(index < old_index) {
        याव(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    अञ्ञथा {
        याव(i < index) {
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
    LV_CHECK_OBJ(obj1, MY_CLASS, पटिनिवत्त);
    LV_CHECK_OBJ(obj2, MY_CLASS, पटिनिवत्त);

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

    सचे(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);

    ध्रुव lv_obj_t * par = obj;
    ध्रुव lv_obj_t * act_par;

    करोहि {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } याव(par != रिक्त);

    पटिनिवत्त (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);

    ध्रुव lv_obj_t * scr;

    सचे(obj->parent == रिक्त) scr = obj;  /*`obj` is a screen*/
    अञ्ञथा scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        अत्थाय(i = 0; i < d->screen_cnt; i++) {
            सचे(d->screens[i] == scr) पटिनिवत्त d;
        }
    }

    LV_LOG_WARN("No screen found");
    पटिनिवत्त रिक्त;
}

lv_obj_t * lv_obj_get_parent(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);

    पटिनिवत्त obj->parent;
}

lv_obj_t * lv_obj_get_child(ध्रुव lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);

    सचे(obj->spec_attr == रिक्त) पटिनिवत्त रिक्त;

    सचे(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        सचे(idx < 0) पटिनिवत्त रिक्त;
    }

    uint32_t idu = (uint32_t) idx;

    सचे(idu < obj->spec_attr->child_cnt) पटिनिवत्त obj->spec_attr->children[idu];
    अञ्ञथा पटिनिवत्त रिक्त;
}

lv_obj_t * lv_obj_get_child_by_type(ध्रुव lv_obj_t * obj, int32_t idx, ध्रुव lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(class_p != रिक्त, पटिनिवत्त रिक्त);

    सचे(obj->spec_attr == रिक्त) पटिनिवत्त रिक्त;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    सचे(idx >= 0) {
        अत्थाय(i = 0; i < cnt; i++) {
            सचे(obj->spec_attr->children[i]->class_p == class_p) {
                सचे(idx == 0) पटिनिवत्त obj->spec_attr->children[i];
                अञ्ञथा idx--;
            }
        }
    }
    अञ्ञथा {
        idx++;   /*-1 means the first child*/
        अत्थाय(i = cnt - 1; i >= 0; i--) {
            सचे(obj->spec_attr->children[i]->class_p == class_p) {
                सचे(idx == 0) पटिनिवत्त obj->spec_attr->children[i];
                अञ्ञथा idx++;
            }
        }
    }
    पटिनिवत्त रिक्त;
}

lv_obj_t * lv_obj_get_sibling(ध्रुव lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(obj->parent != रिक्त, पटिनिवत्त रिक्त);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    सचे(sibling_idx < 0) पटिनिवत्त रिक्त;

    पटिनिवत्त lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ध्रुव lv_obj_t * obj, int32_t idx, ध्रुव lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(obj->parent != रिक्त, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(class_p != रिक्त, पटिनिवत्त रिक्त);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    सचे(sibling_idx < 0) पटिनिवत्त रिक्त;

    पटिनिवत्त lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_गणनम्(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त 0);
    सचे(obj->spec_attr == रिक्त) पटिनिवत्त 0;
    पटिनिवत्त obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_गणनम्_by_type(ध्रुव lv_obj_t * obj, ध्रुव lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त 0);
    LV_CHECK_ARG(class_p != रिक्त, पटिनिवत्त 0);

    सचे(obj->spec_attr == रिक्त) पटिनिवत्त 0;

    uint32_t i;
    uint32_t cnt = 0;
    अत्थाय(i = 0; i < obj->spec_attr->child_cnt; i++) {
        सचे(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    पटिनिवत्त cnt;
}

#सचे LV_USE_OBJ_NAME

शून्य lv_obj_set_name(lv_obj_t * obj, ध्रुव वर्ण * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);

    सचे(!lv_obj_allocate_spec_attr(obj)) {
        पटिनिवत्त;
    }

    सचे(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((शून्य *)obj->spec_attr->name);

    सचे(name == रिक्त) {
        obj->spec_attr->name = रिक्त;
        obj->spec_attr->name_static = 1;
    }
    अञ्ञथा {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

शून्य lv_obj_set_name_static(lv_obj_t * obj, ध्रुव वर्ण * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);
    LV_CHECK_ARG(name != रिक्त, पटिनिवत्त);

    सचे(!lv_obj_allocate_spec_attr(obj)) {
        पटिनिवत्त;
    }
    सचे(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((शून्य *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ध्रुव वर्ण * lv_obj_get_name(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त रिक्त);

    सचे(obj->spec_attr == रिक्त) पटिनिवत्त रिक्त;
    अञ्ञथा पटिनिवत्त obj->spec_attr->name;
}

शून्य lv_obj_get_name_resolved(ध्रुव lv_obj_t * obj, वर्ण buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त);
    LV_CHECK_ARG(buf != रिक्त && buf_size > 0, पटिनिवत्त);

    ध्रुव वर्ण * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    वर्ण name_buf[LV_OBJ_NAME_MAX_LEN];
    सचे(name == रिक्त) {
        lv_snprintf(name_buf, परिमाण(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    सचे(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_गणनम्(parent);
        uint32_t cnt = 0;
        uint32_t i;
        अत्थाय(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            सचे(child == obj) {
                वर्ण अङ्कः_buf[8];
                size_t अङ्कः_len;
                अङ्कः_len = lv_snprintf(अङ्कः_buf, परिमाण(अङ्कः_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                सचे(buf_size > name_len + अङ्कः_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], अङ्कः_buf);
                }
                अञ्ञथा {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                भङ्ग;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            अञ्ञथा {
                ध्रुव वर्ण * child_name = lv_obj_get_name(child);
                सचे(child_name == रिक्त) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    सचे(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                अञ्ञथा {
                    सचे(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    अञ्ञथा {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ध्रुव lv_obj_t * parent, ध्रुव वर्ण * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(path != रिक्त, पटिनिवत्त रिक्त);
    LV_CHECK_ARG(parent->spec_attr != रिक्त, पटिनिवत्त रिक्त);

    याव(*path) {
        ध्रुव वर्ण * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        याव(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        सचे(!child) पटिनिवत्त रिक्त; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        सचे(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        सचे(*path == '\0') पटिनिवत्त child;

        parent = child;
    }

    पटिनिवत्त रिक्त;

}

lv_obj_t * lv_obj_find_by_name(ध्रुव lv_obj_t * parent, ध्रुव वर्ण * name)
{
    LV_CHECK_ARG(name != रिक्त, पटिनिवत्त रिक्त);

    सचे(parent == रिक्त) parent = lv_display_get_screen_active(lv_display_get_default());
    सचे(parent == रिक्त) पटिनिवत्त रिक्त;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    सचे(child) पटिनिवत्त child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_गणनम्(parent);
    uint32_t i;
    अत्थाय(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        सचे(found != रिक्त) पटिनिवत्त found;
    }

    पटिनिवत्त रिक्त;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    सचे(parent == रिक्त) पटिनिवत्त -1;

    uint32_t i = 0;
    अत्थाय(i = 0; i < parent->spec_attr->child_cnt; i++) {
        सचे(parent->spec_attr->children[i] == obj) पटिनिवत्त (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    पटिनिवत्त -1;
}

int32_t lv_obj_get_index_by_type(ध्रुव lv_obj_t * obj, ध्रुव lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, पटिनिवत्त -1);
    LV_CHECK_ARG(class_p != रिक्त, पटिनिवत्त -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    सचे(parent == रिक्त) पटिनिवत्त 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    अत्थाय(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        सचे(child->class_p == class_p) {
            सचे(child == obj) पटिनिवत्त idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    पटिनिवत्त -1;
}

शून्य lv_obj_वृक्षः_walk(lv_obj_t * start_obj, lv_obj_वृक्षः_walk_cb_t cb, शून्य * user_data)
{
    LV_CHECK_ARG(cb != रिक्त, पटिनिवत्त);

    walk_core(start_obj, cb, user_data);
}

शून्य lv_obj_dump_वृक्षः(lv_obj_t * start_obj)
{
    सचे(start_obj == रिक्त) {
        lv_display_t * disp = lv_display_get_next(रिक्त);
        याव(disp) {
            uint32_t i;
            अत्थाय(i = 0; i < disp->screen_cnt; i++) {
                dump_वृक्षः_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    अञ्ञथा {
        dump_वृक्षः_core(start_obj, 0);
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
    सचे(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

स्थिर शून्य obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != रिक्त);
    सचे(obj->is_deleting)
        पटिनिवत्त;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, रिक्त);
    सचे(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        पटिनिवत्त;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    सचे(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    सचे(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    याव(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(रिक्त);
    याव(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        सचे(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            सचे(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            सचे(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = रिक्त;
            }
            सचे(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = रिक्त;
            }
        }

        सचे(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    याव(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    सचे(obj->parent == रिक्त) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        अत्थाय(i = 0; i < disp->screen_cnt; i++) {
            सचे(disp->screens[i] == obj) भङ्ग;
        }

        uint32_t id = i;
        अत्थाय(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * परिमाण(lv_obj_t *));
        सचे(new_screens != रिक्त) {
            disp->screens = new_screens;
        }
        अञ्ञथा {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    अञ्ञथा {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = रिक्त;

    /*Free the object itself*/
    lv_free(obj);
}

स्थिर lv_obj_वृक्षः_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्षः_walk_cb_t cb, शून्य * user_data)
{
    lv_obj_वृक्षः_walk_res_t res = LV_OBJ_वृक्षः_WALK_NEXT;

    सचे(obj == रिक्त) {
        lv_display_t * disp = lv_display_get_next(रिक्त);
        याव(disp) {
            uint32_t i;
            अत्थाय(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        पटिनिवत्त LV_OBJ_वृक्षः_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    सचे(res == LV_OBJ_वृक्षः_WALK_END) पटिनिवत्त LV_OBJ_वृक्षः_WALK_END;

    सचे(res != LV_OBJ_वृक्षः_WALK_SKIP_CHILDREN) {
        uint32_t i;
        अत्थाय(i = 0; i < lv_obj_get_child_गणनम्(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            सचे(res == LV_OBJ_वृक्षः_WALK_END) पटिनिवत्त LV_OBJ_वृक्षः_WALK_END;
        }
    }
    पटिनिवत्त LV_OBJ_वृक्षः_WALK_NEXT;
}

स्थिर शून्य dump_वृक्षः_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != रिक्त);
#सचे LV_USE_LOG
    ध्रुव वर्ण * id;

#सचे LV_USE_OBJ_ID
    वर्ण buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, परिमाण(buf));
    सचे(id == रिक्त) id = "obj0";
#अञ्ञथा
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (पूर्णाङ्क)(2 * depth), "", (शून्य *)obj, id);
#endif /*LV_USE_LOG*/

    सचे(obj->spec_attr && obj->spec_attr->child_cnt) {
        अत्थाय(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_वृक्षः_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

स्थिर lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != रिक्त);

    सचे(obj->spec_attr == रिक्त) पटिनिवत्त रिक्त;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    अत्थाय(i = 0; i < cnt; i++) {
        सचे(!obj->spec_attr->children[i]->is_deleting) {
            पटिनिवत्त obj->spec_attr->children[i];
        }
    }

    पटिनिवत्त रिक्त;
}

#सचे LV_USE_OBJ_NAME

स्थिर lv_obj_t * find_by_name_direct(ध्रुव lv_obj_t * parent, ध्रुव वर्ण * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गणनम्(parent);
    अत्थाय(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        वर्ण child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, परिमाण(child_name_resolved));

        सचे(len == UINT16_MAX) {
            सचे(lv_strcmp(child_name_resolved, name) == 0) पटिनिवत्त child;
        }
        अञ्ञथा {
            सचे(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                पटिनिवत्त child;
            }
        }
    }

    पटिनिवत्त रिक्त;
}

#endif /*LV_USE_OBJ_NAME*/
