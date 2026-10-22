/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "lv_obj_private.h"
#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "lv_obj_class_private.h"
#অন্তর্ভাব "../indev/lv_indev_private.h"
#অন্তর্ভাব "../display/lv_display_private.h"
#অন্তর্ভাব "../misc/lv_anim_private.h"
#অন্তর্ভাব "../core/lv_global.h"

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
স্থির শূন্য lv_obj_delete_async_cb(শূন্য * obj);
স্থির শূন্য obj_delete_core(lv_obj_t * obj);
স্থির lv_obj_বৃক্ষ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_data);
স্থির শূন্য dump_বৃক্ষ_core(lv_obj_t * obj, int32_t depth);
স্থির lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#যদি LV_USE_OBJ_NAME
    স্থির lv_obj_t * find_by_name_direct(ধ্রুব lv_obj_t * parent, ধ্রুব বর্ণ * name, size_t len);
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

শূন্য lv_obj_delete(lv_obj_t * obj)
{
    যদি(obj == রিক্ত) প্রত্যাগম;
    যদি(obj->is_deleting) প্রত্যাগম;

    LV_LOG_TRACE("begin (delete %p)", (শূন্য *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = রিক্ত;
    bool act_screen_del = false;
    যদি(par == রিক্ত) {
        disp = lv_obj_get_display(obj);
        যদি(!disp) প্রত্যাগম;   /*Shouldn't happen*/
        যদি(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    যদি(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, রিক্ত);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, রিক্ত);
    }

    /*Handle if the active screen was deleted*/
    যদি(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = রিক্ত;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (শূন্য *)obj);
}

শূন্য lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (শূন্য *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_গুনতি(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    যাবত্(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    যদি(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    যদি(lv_obj_get_child_গুনতি(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, রিক্ত);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, রিক্ত);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (শূন্য *)obj);
}

শূন্য lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    যদি(obj == রিক্ত) প্রত্যাগম;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, রিক্ত);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

শূন্য lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != রিক্ত, প্রত্যাগম);

    lv_obj_delete(a->var);
}

শূন্য lv_obj_delete_async(lv_obj_t * obj)
{
    যদি(obj == রিক্ত) প্রত্যাগম;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

শূন্য lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);
    LV_CHECK_OBJ(parent, MY_CLASS, প্রত্যাগম);

    যদি(obj->parent == রিক্ত) {
        LV_LOG_WARN("Can't set the parent of a screen");
        প্রত্যাগম;
    }

    যদি(parent == obj->parent) {
        প্রত্যাগম;
    }

    যদি(!lv_obj_allocate_spec_attr(parent)) {
        প্রত্যাগম;
    }

    /*Add the object to the new parents list first*/
    যদি(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        প্রত্যাগম;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, রিক্ত);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, রিক্ত);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

শূন্য lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(!parent) {
        LV_LOG_WARN("parent is NULL");
        প্রত্যাগম;
    }

    ধ্রুব uint32_t parent_child_গুনতি = lv_obj_get_child_গুনতি(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ধ্রুব int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    যদি(index < 0) {
        index += parent_child_গুনতি;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    যদি((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_গুনতি)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        প্রত্যাগম;
    }

    int32_t i = old_index;
    যদি(index < old_index) {
        যাবত্(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    অন্যথা {
        যাবত্(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, রিক্ত);
    lv_obj_invalidate(parent);
}

শূন্য lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, প্রত্যাগম);
    LV_CHECK_OBJ(obj2, MY_CLASS, প্রত্যাগম);

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

    যদি(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);

    ধ্রুব lv_obj_t * par = obj;
    ধ্রুব lv_obj_t * act_par;

    কুরু {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } যাবত্(par != রিক্ত);

    প্রত্যাগম (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);

    ধ্রুব lv_obj_t * scr;

    যদি(obj->parent == রিক্ত) scr = obj;  /*`obj` is a screen*/
    অন্যথা scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        প্রতি(i = 0; i < d->screen_cnt; i++) {
            যদি(d->screens[i] == scr) প্রত্যাগম d;
        }
    }

    LV_LOG_WARN("No screen found");
    প্রত্যাগম রিক্ত;
}

lv_obj_t * lv_obj_get_parent(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);

    প্রত্যাগম obj->parent;
}

lv_obj_t * lv_obj_get_child(ধ্রুব lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);

    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম রিক্ত;

    যদি(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        যদি(idx < 0) প্রত্যাগম রিক্ত;
    }

    uint32_t idu = (uint32_t) idx;

    যদি(idu < obj->spec_attr->child_cnt) প্রত্যাগম obj->spec_attr->children[idu];
    অন্যথা প্রত্যাগম রিক্ত;
}

lv_obj_t * lv_obj_get_child_by_type(ধ্রুব lv_obj_t * obj, int32_t idx, ধ্রুব lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(class_p != রিক্ত, প্রত্যাগম রিক্ত);

    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম রিক্ত;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    যদি(idx >= 0) {
        প্রতি(i = 0; i < cnt; i++) {
            যদি(obj->spec_attr->children[i]->class_p == class_p) {
                যদি(idx == 0) প্রত্যাগম obj->spec_attr->children[i];
                অন্যথা idx--;
            }
        }
    }
    অন্যথা {
        idx++;   /*-1 means the first child*/
        প্রতি(i = cnt - 1; i >= 0; i--) {
            যদি(obj->spec_attr->children[i]->class_p == class_p) {
                যদি(idx == 0) প্রত্যাগম obj->spec_attr->children[i];
                অন্যথা idx++;
            }
        }
    }
    প্রত্যাগম রিক্ত;
}

lv_obj_t * lv_obj_get_sibling(ধ্রুব lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(obj->parent != রিক্ত, প্রত্যাগম রিক্ত);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    যদি(sibling_idx < 0) প্রত্যাগম রিক্ত;

    প্রত্যাগম lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ধ্রুব lv_obj_t * obj, int32_t idx, ধ্রুব lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(obj->parent != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(class_p != রিক্ত, প্রত্যাগম রিক্ত);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    যদি(sibling_idx < 0) প্রত্যাগম রিক্ত;

    প্রত্যাগম lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_গুনতি(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম 0);
    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম 0;
    প্রত্যাগম obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_গুনতি_by_type(ধ্রুব lv_obj_t * obj, ধ্রুব lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম 0);
    LV_CHECK_ARG(class_p != রিক্ত, প্রত্যাগম 0);

    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম 0;

    uint32_t i;
    uint32_t cnt = 0;
    প্রতি(i = 0; i < obj->spec_attr->child_cnt; i++) {
        যদি(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    প্রত্যাগম cnt;
}

#যদি LV_USE_OBJ_NAME

শূন্য lv_obj_set_name(lv_obj_t * obj, ধ্রুব বর্ণ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        প্রত্যাগম;
    }

    যদি(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((শূন্য *)obj->spec_attr->name);

    যদি(name == রিক্ত) {
        obj->spec_attr->name = রিক্ত;
        obj->spec_attr->name_static = 1;
    }
    অন্যথা {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

শূন্য lv_obj_set_name_static(lv_obj_t * obj, ধ্রুব বর্ণ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);
    LV_CHECK_ARG(name != রিক্ত, প্রত্যাগম);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        প্রত্যাগম;
    }
    যদি(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((শূন্য *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ধ্রুব বর্ণ * lv_obj_get_name(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম রিক্ত);

    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম রিক্ত;
    অন্যথা প্রত্যাগম obj->spec_attr->name;
}

শূন্য lv_obj_get_name_resolved(ধ্রুব lv_obj_t * obj, বর্ণ buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);
    LV_CHECK_ARG(buf != রিক্ত && buf_size > 0, প্রত্যাগম);

    ধ্রুব বর্ণ * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    বর্ণ name_buf[LV_OBJ_NAME_MAX_LEN];
    যদি(name == রিক্ত) {
        lv_snprintf(name_buf, পরিমাণ(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    যদি(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
        uint32_t cnt = 0;
        uint32_t i;
        প্রতি(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            যদি(child == obj) {
                বর্ণ অঙ্ক_buf[8];
                size_t অঙ্ক_len;
                অঙ্ক_len = lv_snprintf(অঙ্ক_buf, পরিমাণ(অঙ্ক_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                যদি(buf_size > name_len + অঙ্ক_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], অঙ্ক_buf);
                }
                অন্যথা {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                ভঙ্গ;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            অন্যথা {
                ধ্রুব বর্ণ * child_name = lv_obj_get_name(child);
                যদি(child_name == রিক্ত) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    যদি(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                অন্যথা {
                    যদি(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    অন্যথা {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ধ্রুব lv_obj_t * parent, ধ্রুব বর্ণ * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(path != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(parent->spec_attr != রিক্ত, প্রত্যাগম রিক্ত);

    যাবত্(*path) {
        ধ্রুব বর্ণ * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        যাবত্(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        যদি(!child) প্রত্যাগম রিক্ত; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        যদি(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        যদি(*path == '\0') প্রত্যাগম child;

        parent = child;
    }

    প্রত্যাগম রিক্ত;

}

lv_obj_t * lv_obj_find_by_name(ধ্রুব lv_obj_t * parent, ধ্রুব বর্ণ * name)
{
    LV_CHECK_ARG(name != রিক্ত, প্রত্যাগম রিক্ত);

    যদি(parent == রিক্ত) parent = lv_display_get_screen_active(lv_display_get_default());
    যদি(parent == রিক্ত) প্রত্যাগম রিক্ত;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    যদি(child) প্রত্যাগম child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
    uint32_t i;
    প্রতি(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        যদি(found != রিক্ত) প্রত্যাগম found;
    }

    প্রত্যাগম রিক্ত;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ধ্রুব lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(parent == রিক্ত) প্রত্যাগম -1;

    uint32_t i = 0;
    প্রতি(i = 0; i < parent->spec_attr->child_cnt; i++) {
        যদি(parent->spec_attr->children[i] == obj) প্রত্যাগম (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    প্রত্যাগম -1;
}

int32_t lv_obj_get_index_by_type(ধ্রুব lv_obj_t * obj, ধ্রুব lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম -1);
    LV_CHECK_ARG(class_p != রিক্ত, প্রত্যাগম -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(parent == রিক্ত) প্রত্যাগম 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    প্রতি(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        যদি(child->class_p == class_p) {
            যদি(child == obj) প্রত্যাগম idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    প্রত্যাগম -1;
}

শূন্য lv_obj_বৃক্ষ_walk(lv_obj_t * start_obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_data)
{
    LV_CHECK_ARG(cb != রিক্ত, প্রত্যাগম);

    walk_core(start_obj, cb, user_data);
}

শূন্য lv_obj_dump_বৃক্ষ(lv_obj_t * start_obj)
{
    যদি(start_obj == রিক্ত) {
        lv_display_t * disp = lv_display_get_next(রিক্ত);
        যাবত্(disp) {
            uint32_t i;
            প্রতি(i = 0; i < disp->screen_cnt; i++) {
                dump_বৃক্ষ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    অন্যথা {
        dump_বৃক্ষ_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির শূন্য lv_obj_delete_async_cb(শূন্য * obj)
{
    lv_obj_delete(obj);
}

স্থির শূন্য obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    যদি(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

স্থির শূন্য obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != রিক্ত);
    যদি(obj->is_deleting)
        প্রত্যাগম;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, রিক্ত);
    যদি(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        প্রত্যাগম;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    যদি(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    যদি(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    যাবত্(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(রিক্ত);
    যাবত্(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        যদি(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            যদি(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            যদি(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = রিক্ত;
            }
            যদি(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = রিক্ত;
            }
        }

        যদি(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    যাবত্(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    যদি(obj->parent == রিক্ত) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        প্রতি(i = 0; i < disp->screen_cnt; i++) {
            যদি(disp->screens[i] == obj) ভঙ্গ;
        }

        uint32_t id = i;
        প্রতি(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * পরিমাণ(lv_obj_t *));
        যদি(new_screens != রিক্ত) {
            disp->screens = new_screens;
        }
        অন্যথা {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    অন্যথা {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = রিক্ত;

    /*Free the object itself*/
    lv_free(obj);
}

স্থির lv_obj_বৃক্ষ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_data)
{
    lv_obj_বৃক্ষ_walk_res_t res = LV_OBJ_বৃক্ষ_WALK_NEXT;

    যদি(obj == রিক্ত) {
        lv_display_t * disp = lv_display_get_next(রিক্ত);
        যাবত্(disp) {
            uint32_t i;
            প্রতি(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        প্রত্যাগম LV_OBJ_বৃক্ষ_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    যদি(res == LV_OBJ_বৃক্ষ_WALK_END) প্রত্যাগম LV_OBJ_বৃক্ষ_WALK_END;

    যদি(res != LV_OBJ_বৃক্ষ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        প্রতি(i = 0; i < lv_obj_get_child_গুনতি(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            যদি(res == LV_OBJ_বৃক্ষ_WALK_END) প্রত্যাগম LV_OBJ_বৃক্ষ_WALK_END;
        }
    }
    প্রত্যাগম LV_OBJ_বৃক্ষ_WALK_NEXT;
}

স্থির শূন্য dump_বৃক্ষ_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != রিক্ত);
#যদি LV_USE_LOG
    ধ্রুব বর্ণ * id;

#যদি LV_USE_OBJ_ID
    বর্ণ buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, পরিমাণ(buf));
    যদি(id == রিক্ত) id = "obj0";
#অন্যথা
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (পূর্ণাঙ্ক)(2 * depth), "", (শূন্য *)obj, id);
#endif /*LV_USE_LOG*/

    যদি(obj->spec_attr && obj->spec_attr->child_cnt) {
        প্রতি(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_বৃক্ষ_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

স্থির lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != রিক্ত);

    যদি(obj->spec_attr == রিক্ত) প্রত্যাগম রিক্ত;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    প্রতি(i = 0; i < cnt; i++) {
        যদি(!obj->spec_attr->children[i]->is_deleting) {
            প্রত্যাগম obj->spec_attr->children[i];
        }
    }

    প্রত্যাগম রিক্ত;
}

#যদি LV_USE_OBJ_NAME

স্থির lv_obj_t * find_by_name_direct(ধ্রুব lv_obj_t * parent, ধ্রুব বর্ণ * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
    প্রতি(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        বর্ণ child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, পরিমাণ(child_name_resolved));

        যদি(len == UINT16_MAX) {
            যদি(lv_strcmp(child_name_resolved, name) == 0) প্রত্যাগম child;
        }
        অন্যথা {
            যদি(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                প্রত্যাগম child;
            }
        }
    }

    প্রত্যাগম রিক্ত;
}

#endif /*LV_USE_OBJ_NAME*/
