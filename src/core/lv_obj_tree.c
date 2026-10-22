/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#அந்தர்பாவ "lv_obj_private.h"
#அந்தர்பாவ "../lvgl_public.h"
#அந்தர்பாவ "lv_obj_class_private.h"
#அந்தர்பாவ "../indev/lv_indev_private.h"
#அந்தர்பாவ "../display/lv_display_private.h"
#அந்தர்பாவ "../misc/lv_anim_private.h"
#அந்தர்பாவ "../core/lv_global.h"

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
நிலையான வெறுமை lv_obj_delete_async_cb(வெறுமை * obj);
நிலையான வெறுமை obj_delete_core(lv_obj_t * obj);
நிலையான lv_obj_மரம்_walk_res_t walk_core(lv_obj_t * obj, lv_obj_மரம்_walk_cb_t cb, வெறுமை * user_data);
நிலையான வெறுமை dump_மரம்_core(lv_obj_t * obj, int32_t depth);
நிலையான lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#எனில் LV_USE_OBJ_NAME
    நிலையான lv_obj_t * find_by_name_direct(மாறிலி lv_obj_t * parent, மாறிலி எழுத்து * name, size_t len);
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

வெறுமை lv_obj_delete(lv_obj_t * obj)
{
    எனில்(obj == வெற்று) திரும்பு;
    எனில்(obj->is_deleting) திரும்பு;

    LV_LOG_TRACE("begin (delete %p)", (வெறுமை *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = வெற்று;
    bool act_screen_del = false;
    எனில்(par == வெற்று) {
        disp = lv_obj_get_display(obj);
        எனில்(!disp) திரும்பு;   /*Shouldn't happen*/
        எனில்(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    எனில்(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, வெற்று);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, வெற்று);
    }

    /*Handle if the active screen was deleted*/
    எனில்(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = வெற்று;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (வெறுமை *)obj);
}

வெறுமை lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (வெறுமை *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_மொத்தம்(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    போது(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    எனில்(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    எனில்(lv_obj_get_child_மொத்தம்(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, வெற்று);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, வெற்று);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (வெறுமை *)obj);
}

வெறுமை lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    எனில்(obj == வெற்று) திரும்பு;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, வெற்று);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

வெறுமை lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != வெற்று, திரும்பு);

    lv_obj_delete(a->var);
}

வெறுமை lv_obj_delete_async(lv_obj_t * obj)
{
    எனில்(obj == வெற்று) திரும்பு;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

வெறுமை lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);
    LV_CHECK_OBJ(parent, MY_CLASS, திரும்பு);

    எனில்(obj->parent == வெற்று) {
        LV_LOG_WARN("Can't set the parent of a screen");
        திரும்பு;
    }

    எனில்(parent == obj->parent) {
        திரும்பு;
    }

    எனில்(!lv_obj_allocate_spec_attr(parent)) {
        திரும்பு;
    }

    /*Add the object to the new parents list first*/
    எனில்(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        திரும்பு;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, வெற்று);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, வெற்று);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

வெறுமை lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    எனில்(!parent) {
        LV_LOG_WARN("parent is NULL");
        திரும்பு;
    }

    மாறிலி uint32_t parent_child_மொத்தம் = lv_obj_get_child_மொத்தம்(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    மாறிலி int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    எனில்(index < 0) {
        index += parent_child_மொத்தம்;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    எனில்((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_மொத்தம்)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        திரும்பு;
    }

    int32_t i = old_index;
    எனில்(index < old_index) {
        போது(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    இல்லையெனில் {
        போது(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, வெற்று);
    lv_obj_invalidate(parent);
}

வெறுமை lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, திரும்பு);
    LV_CHECK_OBJ(obj2, MY_CLASS, திரும்பு);

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

    எனில்(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);

    மாறிலி lv_obj_t * par = obj;
    மாறிலி lv_obj_t * act_par;

    செய் {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } போது(par != வெற்று);

    திரும்பு (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);

    மாறிலி lv_obj_t * scr;

    எனில்(obj->parent == வெற்று) scr = obj;  /*`obj` is a screen*/
    இல்லையெனில் scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        க்கு(i = 0; i < d->screen_cnt; i++) {
            எனில்(d->screens[i] == scr) திரும்பு d;
        }
    }

    LV_LOG_WARN("No screen found");
    திரும்பு வெற்று;
}

lv_obj_t * lv_obj_get_parent(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);

    திரும்பு obj->parent;
}

lv_obj_t * lv_obj_get_child(மாறிலி lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);

    எனில்(obj->spec_attr == வெற்று) திரும்பு வெற்று;

    எனில்(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        எனில்(idx < 0) திரும்பு வெற்று;
    }

    uint32_t idu = (uint32_t) idx;

    எனில்(idu < obj->spec_attr->child_cnt) திரும்பு obj->spec_attr->children[idu];
    இல்லையெனில் திரும்பு வெற்று;
}

lv_obj_t * lv_obj_get_child_by_type(மாறிலி lv_obj_t * obj, int32_t idx, மாறிலி lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);
    LV_CHECK_ARG(class_p != வெற்று, திரும்பு வெற்று);

    எனில்(obj->spec_attr == வெற்று) திரும்பு வெற்று;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    எனில்(idx >= 0) {
        க்கு(i = 0; i < cnt; i++) {
            எனில்(obj->spec_attr->children[i]->class_p == class_p) {
                எனில்(idx == 0) திரும்பு obj->spec_attr->children[i];
                இல்லையெனில் idx--;
            }
        }
    }
    இல்லையெனில் {
        idx++;   /*-1 means the first child*/
        க்கு(i = cnt - 1; i >= 0; i--) {
            எனில்(obj->spec_attr->children[i]->class_p == class_p) {
                எனில்(idx == 0) திரும்பு obj->spec_attr->children[i];
                இல்லையெனில் idx++;
            }
        }
    }
    திரும்பு வெற்று;
}

lv_obj_t * lv_obj_get_sibling(மாறிலி lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);
    LV_CHECK_ARG(obj->parent != வெற்று, திரும்பு வெற்று);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    எனில்(sibling_idx < 0) திரும்பு வெற்று;

    திரும்பு lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(மாறிலி lv_obj_t * obj, int32_t idx, மாறிலி lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);
    LV_CHECK_ARG(obj->parent != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(class_p != வெற்று, திரும்பு வெற்று);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    எனில்(sibling_idx < 0) திரும்பு வெற்று;

    திரும்பு lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_மொத்தம்(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு 0);
    எனில்(obj->spec_attr == வெற்று) திரும்பு 0;
    திரும்பு obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_மொத்தம்_by_type(மாறிலி lv_obj_t * obj, மாறிலி lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு 0);
    LV_CHECK_ARG(class_p != வெற்று, திரும்பு 0);

    எனில்(obj->spec_attr == வெற்று) திரும்பு 0;

    uint32_t i;
    uint32_t cnt = 0;
    க்கு(i = 0; i < obj->spec_attr->child_cnt; i++) {
        எனில்(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    திரும்பு cnt;
}

#எனில் LV_USE_OBJ_NAME

வெறுமை lv_obj_set_name(lv_obj_t * obj, மாறிலி எழுத்து * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);

    எனில்(!lv_obj_allocate_spec_attr(obj)) {
        திரும்பு;
    }

    எனில்(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((வெறுமை *)obj->spec_attr->name);

    எனில்(name == வெற்று) {
        obj->spec_attr->name = வெற்று;
        obj->spec_attr->name_static = 1;
    }
    இல்லையெனில் {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

வெறுமை lv_obj_set_name_static(lv_obj_t * obj, மாறிலி எழுத்து * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);
    LV_CHECK_ARG(name != வெற்று, திரும்பு);

    எனில்(!lv_obj_allocate_spec_attr(obj)) {
        திரும்பு;
    }
    எனில்(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((வெறுமை *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

மாறிலி எழுத்து * lv_obj_get_name(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு வெற்று);

    எனில்(obj->spec_attr == வெற்று) திரும்பு வெற்று;
    இல்லையெனில் திரும்பு obj->spec_attr->name;
}

வெறுமை lv_obj_get_name_resolved(மாறிலி lv_obj_t * obj, எழுத்து buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு);
    LV_CHECK_ARG(buf != வெற்று && buf_size > 0, திரும்பு);

    மாறிலி எழுத்து * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    எழுத்து name_buf[LV_OBJ_NAME_MAX_LEN];
    எனில்(name == வெற்று) {
        lv_snprintf(name_buf, அளவு(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    எனில்(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_மொத்தம்(parent);
        uint32_t cnt = 0;
        uint32_t i;
        க்கு(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            எனில்(child == obj) {
                எழுத்து இலக்கம்_buf[8];
                size_t இலக்கம்_len;
                இலக்கம்_len = lv_snprintf(இலக்கம்_buf, அளவு(இலக்கம்_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                எனில்(buf_size > name_len + இலக்கம்_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], இலக்கம்_buf);
                }
                இல்லையெனில் {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                நிறுத்து;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            இல்லையெனில் {
                மாறிலி எழுத்து * child_name = lv_obj_get_name(child);
                எனில்(child_name == வெற்று) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    எனில்(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                இல்லையெனில் {
                    எனில்(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    இல்லையெனில் {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(மாறிலி lv_obj_t * parent, மாறிலி எழுத்து * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, திரும்பு வெற்று);
    LV_CHECK_ARG(path != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(parent->spec_attr != வெற்று, திரும்பு வெற்று);

    போது(*path) {
        மாறிலி எழுத்து * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        போது(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        எனில்(!child) திரும்பு வெற்று; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        எனில்(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        எனில்(*path == '\0') திரும்பு child;

        parent = child;
    }

    திரும்பு வெற்று;

}

lv_obj_t * lv_obj_find_by_name(மாறிலி lv_obj_t * parent, மாறிலி எழுத்து * name)
{
    LV_CHECK_ARG(name != வெற்று, திரும்பு வெற்று);

    எனில்(parent == வெற்று) parent = lv_display_get_screen_active(lv_display_get_default());
    எனில்(parent == வெற்று) திரும்பு வெற்று;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    எனில்(child) திரும்பு child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_மொத்தம்(parent);
    uint32_t i;
    க்கு(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        எனில்(found != வெற்று) திரும்பு found;
    }

    திரும்பு வெற்று;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(மாறிலி lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    எனில்(parent == வெற்று) திரும்பு -1;

    uint32_t i = 0;
    க்கு(i = 0; i < parent->spec_attr->child_cnt; i++) {
        எனில்(parent->spec_attr->children[i] == obj) திரும்பு (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    திரும்பு -1;
}

int32_t lv_obj_get_index_by_type(மாறிலி lv_obj_t * obj, மாறிலி lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு -1);
    LV_CHECK_ARG(class_p != வெற்று, திரும்பு -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    எனில்(parent == வெற்று) திரும்பு 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    க்கு(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        எனில்(child->class_p == class_p) {
            எனில்(child == obj) திரும்பு idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    திரும்பு -1;
}

வெறுமை lv_obj_மரம்_walk(lv_obj_t * start_obj, lv_obj_மரம்_walk_cb_t cb, வெறுமை * user_data)
{
    LV_CHECK_ARG(cb != வெற்று, திரும்பு);

    walk_core(start_obj, cb, user_data);
}

வெறுமை lv_obj_dump_மரம்(lv_obj_t * start_obj)
{
    எனில்(start_obj == வெற்று) {
        lv_display_t * disp = lv_display_get_next(வெற்று);
        போது(disp) {
            uint32_t i;
            க்கு(i = 0; i < disp->screen_cnt; i++) {
                dump_மரம்_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    இல்லையெனில் {
        dump_மரம்_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

நிலையான வெறுமை lv_obj_delete_async_cb(வெறுமை * obj)
{
    lv_obj_delete(obj);
}

நிலையான வெறுமை obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    எனில்(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

நிலையான வெறுமை obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != வெற்று);
    எனில்(obj->is_deleting)
        திரும்பு;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, வெற்று);
    எனில்(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        திரும்பு;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    எனில்(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    எனில்(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    போது(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(வெற்று);
    போது(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        எனில்(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            எனில்(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            எனில்(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = வெற்று;
            }
            எனில்(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = வெற்று;
            }
        }

        எனில்(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    போது(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    எனில்(obj->parent == வெற்று) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        க்கு(i = 0; i < disp->screen_cnt; i++) {
            எனில்(disp->screens[i] == obj) நிறுத்து;
        }

        uint32_t id = i;
        க்கு(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * அளவு(lv_obj_t *));
        எனில்(new_screens != வெற்று) {
            disp->screens = new_screens;
        }
        இல்லையெனில் {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    இல்லையெனில் {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = வெற்று;

    /*Free the object itself*/
    lv_free(obj);
}

நிலையான lv_obj_மரம்_walk_res_t walk_core(lv_obj_t * obj, lv_obj_மரம்_walk_cb_t cb, வெறுமை * user_data)
{
    lv_obj_மரம்_walk_res_t res = LV_OBJ_மரம்_WALK_NEXT;

    எனில்(obj == வெற்று) {
        lv_display_t * disp = lv_display_get_next(வெற்று);
        போது(disp) {
            uint32_t i;
            க்கு(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        திரும்பு LV_OBJ_மரம்_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    எனில்(res == LV_OBJ_மரம்_WALK_END) திரும்பு LV_OBJ_மரம்_WALK_END;

    எனில்(res != LV_OBJ_மரம்_WALK_SKIP_CHILDREN) {
        uint32_t i;
        க்கு(i = 0; i < lv_obj_get_child_மொத்தம்(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            எனில்(res == LV_OBJ_மரம்_WALK_END) திரும்பு LV_OBJ_மரம்_WALK_END;
        }
    }
    திரும்பு LV_OBJ_மரம்_WALK_NEXT;
}

நிலையான வெறுமை dump_மரம்_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != வெற்று);
#எனில் LV_USE_LOG
    மாறிலி எழுத்து * id;

#எனில் LV_USE_OBJ_ID
    எழுத்து buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, அளவு(buf));
    எனில்(id == வெற்று) id = "obj0";
#இல்லையெனில்
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (முழு)(2 * depth), "", (வெறுமை *)obj, id);
#endif /*LV_USE_LOG*/

    எனில்(obj->spec_attr && obj->spec_attr->child_cnt) {
        க்கு(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_மரம்_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

நிலையான lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != வெற்று);

    எனில்(obj->spec_attr == வெற்று) திரும்பு வெற்று;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    க்கு(i = 0; i < cnt; i++) {
        எனில்(!obj->spec_attr->children[i]->is_deleting) {
            திரும்பு obj->spec_attr->children[i];
        }
    }

    திரும்பு வெற்று;
}

#எனில் LV_USE_OBJ_NAME

நிலையான lv_obj_t * find_by_name_direct(மாறிலி lv_obj_t * parent, மாறிலி எழுத்து * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_மொத்தம்(parent);
    க்கு(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        எழுத்து child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, அளவு(child_name_resolved));

        எனில்(len == UINT16_MAX) {
            எனில்(lv_strcmp(child_name_resolved, name) == 0) திரும்பு child;
        }
        இல்லையெனில் {
            எனில்(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                திரும்பு child;
            }
        }
    }

    திரும்பு வெற்று;
}

#endif /*LV_USE_OBJ_NAME*/
