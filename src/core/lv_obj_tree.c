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
স্থির lv_obj_বৃক্ষ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_তথ্য);
স্থির শূন্য dump_বৃক্ষ_core(lv_obj_t * obj, int32_t depth);
স্থির lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#যদি LV_USE_OBJ_NAME
    স্থির lv_obj_t * find_by_name_direct(ধ্রুবক lv_obj_t * parent, ধ্রুবক অক্ষর * name, আকার_t len);
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
    যদি(obj == ফাঁকা) ফেরত;
    যদি(obj->is_deleting) ফেরত;

    LV_LOG_TRACE("begin (delete %p)", (শূন্য *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = ফাঁকা;
    bool act_screen_del = false;
    যদি(par == ফাঁকা) {
        disp = lv_obj_get_display(obj);
        যদি(!disp) ফেরত;   /*Shouldn't happen*/
        যদি(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    যদি(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, ফাঁকা);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, ফাঁকা);
    }

    /*Handle if the active screen was deleted*/
    যদি(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ফাঁকা;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (শূন্য *)obj);
}

শূন্য lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (শূন্য *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_গুনতি(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    যখন(child) {
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
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, ফাঁকা);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, ফাঁকা);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (শূন্য *)obj);
}

শূন্য lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    যদি(obj == ফাঁকা) ফেরত;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, ফাঁকা);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

শূন্য lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ফাঁকা, ফেরত);

    lv_obj_delete(a->var);
}

শূন্য lv_obj_delete_async(lv_obj_t * obj)
{
    যদি(obj == ফাঁকা) ফেরত;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

শূন্য lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);
    LV_CHECK_OBJ(parent, MY_CLASS, ফেরত);

    যদি(obj->parent == ফাঁকা) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ফেরত;
    }

    যদি(parent == obj->parent) {
        ফেরত;
    }

    যদি(!lv_obj_allocate_spec_attr(parent)) {
        ফেরত;
    }

    /*Add the object to the new parents list first*/
    যদি(lv_obj_add_child(parent, obj) != LV_ফলাফল_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ফেরত;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, ফাঁকা);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, ফাঁকা);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

শূন্য lv_obj_move_to_সূচক(lv_obj_t * obj, int32_t সূচক)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(!parent) {
        LV_LOG_WARN("parent is NULL");
        ফেরত;
    }

    ধ্রুবক uint32_t parent_child_গুনতি = lv_obj_get_child_গুনতি(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ধ্রুবক int32_t old_সূচক = lv_obj_get_সূচক(obj);
    LV_ASSERT(0 <= old_সূচক);

    যদি(সূচক < 0) {
        সূচক += parent_child_গুনতি;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    যদি((সূচক < 0)
       /* Index is same or bigger than parent child count */
       || (সূচক >= (int32_t) parent_child_গুনতি)
       /* If both previous and new index are the same */
       || (সূচক == old_সূচক)) {

        ফেরত;
    }

    int32_t i = old_সূচক;
    যদি(সূচক < old_সূচক) {
        যখন(i > সূচক)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    অন্যথা {
        যখন(i < সূচক) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[সূচক] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, ফাঁকা);
    lv_obj_invalidate(parent);
}

শূন্য lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ফেরত);
    LV_CHECK_OBJ(obj2, MY_CLASS, ফেরত);

    lv_obj_t * parent = lv_obj_get_parent(obj1);
    lv_obj_t * parent2 = lv_obj_get_parent(obj2);

    uint_fast32_t index1 = lv_obj_get_সূচক(obj1);
    uint_fast32_t index2 = lv_obj_get_সূচক(obj2);

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

lv_obj_t * lv_obj_get_screen(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);

    ধ্রুবক lv_obj_t * par = obj;
    ধ্রুবক lv_obj_t * act_par;

    করো {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } যখন(par != ফাঁকা);

    ফেরত (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);

    ধ্রুবক lv_obj_t * scr;

    যদি(obj->parent == ফাঁকা) scr = obj;  /*`obj` is a screen*/
    অন্যথা scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_শীর্ষ = disp_ll_p;
    LV_LL_READ(disp_শীর্ষ, d) {
        uint32_t i;
        জন্য(i = 0; i < d->screen_cnt; i++) {
            যদি(d->screens[i] == scr) ফেরত d;
        }
    }

    LV_LOG_WARN("No screen found");
    ফেরত ফাঁকা;
}

lv_obj_t * lv_obj_get_parent(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);

    ফেরত obj->parent;
}

lv_obj_t * lv_obj_get_child(ধ্রুবক lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);

    যদি(obj->spec_attr == ফাঁকা) ফেরত ফাঁকা;

    যদি(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        যদি(idx < 0) ফেরত ফাঁকা;
    }

    uint32_t idu = (uint32_t) idx;

    যদি(idu < obj->spec_attr->child_cnt) ফেরত obj->spec_attr->children[idu];
    অন্যথা ফেরত ফাঁকা;
}

lv_obj_t * lv_obj_get_child_by_type(ধ্রুবক lv_obj_t * obj, int32_t idx, ধ্রুবক lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);
    LV_CHECK_ARG(class_p != ফাঁকা, ফেরত ফাঁকা);

    যদি(obj->spec_attr == ফাঁকা) ফেরত ফাঁকা;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    যদি(idx >= 0) {
        জন্য(i = 0; i < cnt; i++) {
            যদি(obj->spec_attr->children[i]->class_p == class_p) {
                যদি(idx == 0) ফেরত obj->spec_attr->children[i];
                অন্যথা idx--;
            }
        }
    }
    অন্যথা {
        idx++;   /*-1 means the first child*/
        জন্য(i = cnt - 1; i >= 0; i--) {
            যদি(obj->spec_attr->children[i]->class_p == class_p) {
                যদি(idx == 0) ফেরত obj->spec_attr->children[i];
                অন্যথা idx++;
            }
        }
    }
    ফেরত ফাঁকা;
}

lv_obj_t * lv_obj_get_sibling(ধ্রুবক lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);
    LV_CHECK_ARG(obj->parent != ফাঁকা, ফেরত ফাঁকা);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_সূচক(obj) + idx;
    যদি(sibling_idx < 0) ফেরত ফাঁকা;

    ফেরত lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ধ্রুবক lv_obj_t * obj, int32_t idx, ধ্রুবক lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);
    LV_CHECK_ARG(obj->parent != ফাঁকা, ফেরত ফাঁকা);
    LV_CHECK_ARG(class_p != ফাঁকা, ফেরত ফাঁকা);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_সূচক_by_type(obj, class_p) + idx;
    যদি(sibling_idx < 0) ফেরত ফাঁকা;

    ফেরত lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_গুনতি(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);
    যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
    ফেরত obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_গুনতি_by_type(ধ্রুবক lv_obj_t * obj, ধ্রুবক lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);
    LV_CHECK_ARG(class_p != ফাঁকা, ফেরত 0);

    যদি(obj->spec_attr == ফাঁকা) ফেরত 0;

    uint32_t i;
    uint32_t cnt = 0;
    জন্য(i = 0; i < obj->spec_attr->child_cnt; i++) {
        যদি(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ফেরত cnt;
}

#যদি LV_USE_OBJ_NAME

শূন্য lv_obj_set_name(lv_obj_t * obj, ধ্রুবক অক্ষর * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }

    যদি(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((শূন্য *)obj->spec_attr->name);

    যদি(name == ফাঁকা) {
        obj->spec_attr->name = ফাঁকা;
        obj->spec_attr->name_static = 1;
    }
    অন্যথা {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

শূন্য lv_obj_set_name_static(lv_obj_t * obj, ধ্রুবক অক্ষর * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);
    LV_CHECK_ARG(name != ফাঁকা, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }
    যদি(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((শূন্য *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ধ্রুবক অক্ষর * lv_obj_get_name(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত ফাঁকা);

    যদি(obj->spec_attr == ফাঁকা) ফেরত ফাঁকা;
    অন্যথা ফেরত obj->spec_attr->name;
}

শূন্য lv_obj_get_name_resolved(ধ্রুবক lv_obj_t * obj, অক্ষর buf[], আকার_t buf_আকার)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);
    LV_CHECK_ARG(buf != ফাঁকা && buf_আকার > 0, ফেরত);

    ধ্রুবক অক্ষর * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    অক্ষর name_buf[LV_OBJ_NAME_MAX_LEN];
    যদি(name == ফাঁকা) {
        lv_snprintf(name_buf, আকার(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    আকার_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    যদি(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
        uint32_t cnt = 0;
        uint32_t i;
        জন্য(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            যদি(child == obj) {
                অক্ষর অঙ্ক_buf[8];
                আকার_t অঙ্ক_len;
                অঙ্ক_len = lv_snprintf(অঙ্ক_buf, আকার(অঙ্ক_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                যদি(buf_আকার > name_len + অঙ্ক_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], অঙ্ক_buf);
                }
                অন্যথা {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_আকার);
                }
                বিরতি;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            অন্যথা {
                ধ্রুবক অক্ষর * child_name = lv_obj_get_name(child);
                যদি(child_name == ফাঁকা) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    আকার_t class_name_len = lv_strlen(child->class_p->name);
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
        lv_strlcpy(buf, name, buf_আকার);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ধ্রুবক lv_obj_t * parent, ধ্রুবক অক্ষর * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, ফেরত ফাঁকা);
    LV_CHECK_ARG(path != ফাঁকা, ফেরত ফাঁকা);
    LV_CHECK_ARG(parent->spec_attr != ফাঁকা, ফেরত ফাঁকা);

    যখন(*path) {
        ধ্রুবক অক্ষর * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        যখন(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        যদি(!child) ফেরত ফাঁকা; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        যদি(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        যদি(*path == '\0') ফেরত child;

        parent = child;
    }

    ফেরত ফাঁকা;

}

lv_obj_t * lv_obj_find_by_name(ধ্রুবক lv_obj_t * parent, ধ্রুবক অক্ষর * name)
{
    LV_CHECK_ARG(name != ফাঁকা, ফেরত ফাঁকা);

    যদি(parent == ফাঁকা) parent = lv_display_get_screen_active(lv_display_get_default());
    যদি(parent == ফাঁকা) ফেরত ফাঁকা;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    যদি(child) ফেরত child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
    uint32_t i;
    জন্য(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        যদি(found != ফাঁকা) ফেরত found;
    }

    ফেরত ফাঁকা;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_সূচক(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(parent == ফাঁকা) ফেরত -1;

    uint32_t i = 0;
    জন্য(i = 0; i < parent->spec_attr->child_cnt; i++) {
        যদি(parent->spec_attr->children[i] == obj) ফেরত (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ফেরত -1;
}

int32_t lv_obj_get_সূচক_by_type(ধ্রুবক lv_obj_t * obj, ধ্রুবক lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত -1);
    LV_CHECK_ARG(class_p != ফাঁকা, ফেরত -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(parent == ফাঁকা) ফেরত 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    জন্য(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        যদি(child->class_p == class_p) {
            যদি(child == obj) ফেরত idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ফেরত -1;
}

শূন্য lv_obj_বৃক্ষ_walk(lv_obj_t * start_obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_তথ্য)
{
    LV_CHECK_ARG(cb != ফাঁকা, ফেরত);

    walk_core(start_obj, cb, user_তথ্য);
}

শূন্য lv_obj_dump_বৃক্ষ(lv_obj_t * start_obj)
{
    যদি(start_obj == ফাঁকা) {
        lv_display_t * disp = lv_display_get_পরবর্তী(ফাঁকা);
        যখন(disp) {
            uint32_t i;
            জন্য(i = 0; i < disp->screen_cnt; i++) {
                dump_বৃক্ষ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_পরবর্তী(disp);
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
    LV_ASSERT(obj != ফাঁকা);
    যদি(obj->is_deleting)
        ফেরত;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_ফলাফল_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ফাঁকা);
    যদি(res == LV_ফলাফল_INVALID) {
        obj->is_deleting = false;
        ফেরত;
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
    যখন(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_পরবর্তী(ফাঁকা);
    যখন(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        যদি(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            যদি(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            যদি(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = ফাঁকা;
            }
            যদি(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = ফাঁকা;
            }
        }

        যদি(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_পরবর্তী(indev);
    }

    /*Delete all pending async del-s*/
    lv_ফলাফল_t async_cancel_res = LV_ফলাফল_OK;
    যখন(async_cancel_res == LV_ফলাফল_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    যদি(obj->parent == ফাঁকা) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        জন্য(i = 0; i < disp->screen_cnt; i++) {
            যদি(disp->screens[i] == obj) বিরতি;
        }

        uint32_t id = i;
        জন্য(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * আকার(lv_obj_t *));
        যদি(new_screens != ফাঁকা) {
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

    obj->parent = ফাঁকা;

    /*Free the object itself*/
    lv_free(obj);
}

স্থির lv_obj_বৃক্ষ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_বৃক্ষ_walk_cb_t cb, শূন্য * user_তথ্য)
{
    lv_obj_বৃক্ষ_walk_res_t res = LV_OBJ_বৃক্ষ_WALK_পরবর্তী;

    যদি(obj == ফাঁকা) {
        lv_display_t * disp = lv_display_get_পরবর্তী(ফাঁকা);
        যখন(disp) {
            uint32_t i;
            জন্য(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_তথ্য);
            }
            disp = lv_display_get_পরবর্তী(disp);
        }
        ফেরত LV_OBJ_বৃক্ষ_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_তথ্য);

    যদি(res == LV_OBJ_বৃক্ষ_WALK_END) ফেরত LV_OBJ_বৃক্ষ_WALK_END;

    যদি(res != LV_OBJ_বৃক্ষ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        জন্য(i = 0; i < lv_obj_get_child_গুনতি(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_তথ্য);
            যদি(res == LV_OBJ_বৃক্ষ_WALK_END) ফেরত LV_OBJ_বৃক্ষ_WALK_END;
        }
    }
    ফেরত LV_OBJ_বৃক্ষ_WALK_পরবর্তী;
}

স্থির শূন্য dump_বৃক্ষ_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != ফাঁকা);
#যদি LV_USE_LOG
    ধ্রুবক অক্ষর * id;

#যদি LV_USE_OBJ_ID
    অক্ষর buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, আকার(buf));
    যদি(id == ফাঁকা) id = "obj0";
#অন্যথা
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (পূর্ণসংখ্যা)(2 * depth), "", (শূন্য *)obj, id);
#endif /*LV_USE_LOG*/

    যদি(obj->spec_attr && obj->spec_attr->child_cnt) {
        জন্য(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_বৃক্ষ_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

স্থির lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != ফাঁকা);

    যদি(obj->spec_attr == ফাঁকা) ফেরত ফাঁকা;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    জন্য(i = 0; i < cnt; i++) {
        যদি(!obj->spec_attr->children[i]->is_deleting) {
            ফেরত obj->spec_attr->children[i];
        }
    }

    ফেরত ফাঁকা;
}

#যদি LV_USE_OBJ_NAME

স্থির lv_obj_t * find_by_name_direct(ধ্রুবক lv_obj_t * parent, ধ্রুবক অক্ষর * name, আকার_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(parent);
    জন্য(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        অক্ষর child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, আকার(child_name_resolved));

        যদি(len == UINT16_MAX) {
            যদি(lv_strcmp(child_name_resolved, name) == 0) ফেরত child;
        }
        অন্যথা {
            যদি(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                ফেরত child;
            }
        }
    }

    ফেরত ফাঁকা;
}

#endif /*LV_USE_OBJ_NAME*/
