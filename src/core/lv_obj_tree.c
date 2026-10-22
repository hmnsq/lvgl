/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../indev/lv_indev_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_anim_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"

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
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_delete_async_cb(ಶೂನ್ಯ * obj);
ಸ್ಥಿರ ಶೂನ್ಯ obj_delete_core(lv_obj_t * obj);
ಸ್ಥಿರ lv_obj_ವೃಕ್ಷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_data);
ಸ್ಥಿರ ಶೂನ್ಯ dump_ವೃಕ್ಷ_core(lv_obj_t * obj, int32_t depth);
ಸ್ಥಿರ lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#ಯದಿ LV_USE_OBJ_NAME
    ಸ್ಥಿರ lv_obj_t * find_by_name_direct(ಧ್ರುವ lv_obj_t * parent, ಧ್ರುವ ವರ್ಣ * name, size_t len);
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

ಶೂನ್ಯ lv_obj_delete(lv_obj_t * obj)
{
    ಯದಿ(obj == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ(obj->is_deleting) ಪ್ರತ್ಯಾಗಮ;

    LV_LOG_TRACE("begin (delete %p)", (ಶೂನ್ಯ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = ರಿಕ್ತ;
    bool act_screen_del = false;
    ಯದಿ(par == ರಿಕ್ತ) {
        disp = lv_obj_get_display(obj);
        ಯದಿ(!disp) ಪ್ರತ್ಯಾಗಮ;   /*Shouldn't happen*/
        ಯದಿ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ಯದಿ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, ರಿಕ್ತ);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, ರಿಕ್ತ);
    }

    /*Handle if the active screen was deleted*/
    ಯದಿ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ರಿಕ್ತ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (ಶೂನ್ಯ *)obj);
}

ಶೂನ್ಯ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (ಶೂನ್ಯ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_ಎಣಿಕೆ(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    ಯಾವತ್(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    ಯದಿ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ಯದಿ(lv_obj_get_child_ಎಣಿಕೆ(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, ರಿಕ್ತ);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, ರಿಕ್ತ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (ಶೂನ್ಯ *)obj);
}

ಶೂನ್ಯ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ಯದಿ(obj == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, ರಿಕ್ತ);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

ಶೂನ್ಯ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_obj_delete(a->var);
}

ಶೂನ್ಯ lv_obj_delete_async(lv_obj_t * obj)
{
    ಯದಿ(obj == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

ಶೂನ್ಯ lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_OBJ(parent, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(obj->parent == ರಿಕ್ತ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ(parent == obj->parent) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ(!lv_obj_allocate_spec_attr(parent)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    /*Add the object to the new parents list first*/
    ಯದಿ(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ಪ್ರತ್ಯಾಗಮ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, ರಿಕ್ತ);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, ರಿಕ್ತ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

ಶೂನ್ಯ lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    ಯದಿ(!parent) {
        LV_LOG_WARN("parent is NULL");
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಧ್ರುವ uint32_t parent_child_ಎಣಿಕೆ = lv_obj_get_child_ಎಣಿಕೆ(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ಧ್ರುವ int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    ಯದಿ(index < 0) {
        index += parent_child_ಎಣಿಕೆ;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ಯದಿ((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_ಎಣಿಕೆ)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        ಪ್ರತ್ಯಾಗಮ;
    }

    int32_t i = old_index;
    ಯದಿ(index < old_index) {
        ಯಾವತ್(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    ಅನ್ಯಥಾ {
        ಯಾವತ್(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, ರಿಕ್ತ);
    lv_obj_invalidate(parent);
}

ಶೂನ್ಯ lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_OBJ(obj2, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

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

    ಯದಿ(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಧ್ರುವ lv_obj_t * par = obj;
    ಧ್ರುವ lv_obj_t * act_par;

    ಕುರು {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } ಯಾವತ್(par != ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಧ್ರುವ lv_obj_t * scr;

    ಯದಿ(obj->parent == ರಿಕ್ತ) scr = obj;  /*`obj` is a screen*/
    ಅನ್ಯಥಾ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < d->screen_cnt; i++) {
            ಯದಿ(d->screens[i] == scr) ಪ್ರತ್ಯಾಗಮ d;
        }
    }

    LV_LOG_WARN("No screen found");
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

lv_obj_t * lv_obj_get_parent(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ obj->parent;
}

lv_obj_t * lv_obj_get_child(ಧ್ರುವ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        ಯದಿ(idx < 0) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    uint32_t idu = (uint32_t) idx;

    ಯದಿ(idu < obj->spec_attr->child_cnt) ಪ್ರತ್ಯಾಗಮ obj->spec_attr->children[idu];
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

lv_obj_t * lv_obj_get_child_by_type(ಧ್ರುವ lv_obj_t * obj, int32_t idx, ಧ್ರುವ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ಯದಿ(idx >= 0) {
        ಪ್ರತಿ(i = 0; i < cnt; i++) {
            ಯದಿ(obj->spec_attr->children[i]->class_p == class_p) {
                ಯದಿ(idx == 0) ಪ್ರತ್ಯಾಗಮ obj->spec_attr->children[i];
                ಅನ್ಯಥಾ idx--;
            }
        }
    }
    ಅನ್ಯಥಾ {
        idx++;   /*-1 means the first child*/
        ಪ್ರತಿ(i = cnt - 1; i >= 0; i--) {
            ಯದಿ(obj->spec_attr->children[i]->class_p == class_p) {
                ಯದಿ(idx == 0) ಪ್ರತ್ಯಾಗಮ obj->spec_attr->children[i];
                ಅನ್ಯಥಾ idx++;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

lv_obj_t * lv_obj_get_sibling(ಧ್ರುವ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(obj->parent != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    ಯದಿ(sibling_idx < 0) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ಧ್ರುವ lv_obj_t * obj, int32_t idx, ಧ್ರುವ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(obj->parent != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    ಯದಿ(sibling_idx < 0) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_ಎಣಿಕೆ(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);
    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_ಎಣಿಕೆ_by_type(ಧ್ರುವ lv_obj_t * obj, ಧ್ರುವ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG(class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ 0;

    uint32_t i;
    uint32_t cnt = 0;
    ಪ್ರತಿ(i = 0; i < obj->spec_attr->child_cnt; i++) {
        ಯದಿ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ಪ್ರತ್ಯಾಗಮ cnt;
}

#ಯದಿ LV_USE_OBJ_NAME

ಶೂನ್ಯ lv_obj_set_name(lv_obj_t * obj, ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ಶೂನ್ಯ *)obj->spec_attr->name);

    ಯದಿ(name == ರಿಕ್ತ) {
        obj->spec_attr->name = ರಿಕ್ತ;
        obj->spec_attr->name_static = 1;
    }
    ಅನ್ಯಥಾ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

ಶೂನ್ಯ lv_obj_set_name_static(lv_obj_t * obj, ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(name != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ಶೂನ್ಯ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ಧ್ರುವ ವರ್ಣ * lv_obj_get_name(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ obj->spec_attr->name;
}

ಶೂನ್ಯ lv_obj_get_name_resolved(ಧ್ರುವ lv_obj_t * obj, ವರ್ಣ buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(buf != ರಿಕ್ತ && buf_size > 0, ಪ್ರತ್ಯಾಗಮ);

    ಧ್ರುವ ವರ್ಣ * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    ವರ್ಣ name_buf[LV_OBJ_NAME_MAX_LEN];
    ಯದಿ(name == ರಿಕ್ತ) {
        lv_snprintf(name_buf, ಪರಿಮಾಣ(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ಯದಿ(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_ಎಣಿಕೆ(parent);
        uint32_t cnt = 0;
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ಯದಿ(child == obj) {
                ವರ್ಣ ಅಂಕೆ_buf[8];
                size_t ಅಂಕೆ_len;
                ಅಂಕೆ_len = lv_snprintf(ಅಂಕೆ_buf, ಪರಿಮಾಣ(ಅಂಕೆ_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ಯದಿ(buf_size > name_len + ಅಂಕೆ_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], ಅಂಕೆ_buf);
                }
                ಅನ್ಯಥಾ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                ಭಙ್ಗ;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ಅನ್ಯಥಾ {
                ಧ್ರುವ ವರ್ಣ * child_name = lv_obj_get_name(child);
                ಯದಿ(child_name == ರಿಕ್ತ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    ಯದಿ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ಅನ್ಯಥಾ {
                    ಯದಿ(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ಅನ್ಯಥಾ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ಧ್ರುವ lv_obj_t * parent, ಧ್ರುವ ವರ್ಣ * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(path != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(parent->spec_attr != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯಾವತ್(*path) {
        ಧ್ರುವ ವರ್ಣ * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ಯಾವತ್(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        ಯದಿ(!child) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        ಯದಿ(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ಯದಿ(*path == '\0') ಪ್ರತ್ಯಾಗಮ child;

        parent = child;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

}

lv_obj_t * lv_obj_find_by_name(ಧ್ರುವ lv_obj_t * parent, ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_ARG(name != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(parent == ರಿಕ್ತ) parent = lv_display_get_screen_active(lv_display_get_default());
    ಯದಿ(parent == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    ಯದಿ(child) ಪ್ರತ್ಯಾಗಮ child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_ಎಣಿಕೆ(parent);
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        ಯದಿ(found != ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ found;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ಯದಿ(parent == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ -1;

    uint32_t i = 0;
    ಪ್ರತಿ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        ಯದಿ(parent->spec_attr->children[i] == obj) ಪ್ರತ್ಯಾಗಮ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ಪ್ರತ್ಯಾಗಮ -1;
}

int32_t lv_obj_get_index_by_type(ಧ್ರುವ lv_obj_t * obj, ಧ್ರುವ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ -1);
    LV_CHECK_ARG(class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ಯದಿ(parent == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    ಪ್ರತಿ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        ಯದಿ(child->class_p == class_p) {
            ಯದಿ(child == obj) ಪ್ರತ್ಯಾಗಮ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ಪ್ರತ್ಯಾಗಮ -1;
}

ಶೂನ್ಯ lv_obj_ವೃಕ್ಷ_walk(lv_obj_t * start_obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    walk_core(start_obj, cb, user_data);
}

ಶೂನ್ಯ lv_obj_dump_ವೃಕ್ಷ(lv_obj_t * start_obj)
{
    ಯದಿ(start_obj == ರಿಕ್ತ) {
        lv_display_t * disp = lv_display_get_next(ರಿಕ್ತ);
        ಯಾವತ್(disp) {
            uint32_t i;
            ಪ್ರತಿ(i = 0; i < disp->screen_cnt; i++) {
                dump_ವೃಕ್ಷ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    ಅನ್ಯಥಾ {
        dump_ವೃಕ್ಷ_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_delete_async_cb(ಶೂನ್ಯ * obj)
{
    lv_obj_delete(obj);
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    ಯದಿ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    ಯದಿ(obj->is_deleting)
        ಪ್ರತ್ಯಾಗಮ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ರಿಕ್ತ);
    ಯದಿ(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        ಪ್ರತ್ಯಾಗಮ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ಯದಿ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ಯದಿ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    ಯಾವತ್(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(ರಿಕ್ತ);
    ಯಾವತ್(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ಯದಿ(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            ಯದಿ(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ಯದಿ(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = ರಿಕ್ತ;
            }
            ಯದಿ(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = ರಿಕ್ತ;
            }
        }

        ಯದಿ(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    ಯಾವತ್(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ಯದಿ(obj->parent == ರಿಕ್ತ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        ಪ್ರತಿ(i = 0; i < disp->screen_cnt; i++) {
            ಯದಿ(disp->screens[i] == obj) ಭಙ್ಗ;
        }

        uint32_t id = i;
        ಪ್ರತಿ(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * ಪರಿಮಾಣ(lv_obj_t *));
        ಯದಿ(new_screens != ರಿಕ್ತ) {
            disp->screens = new_screens;
        }
        ಅನ್ಯಥಾ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ಅನ್ಯಥಾ {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = ರಿಕ್ತ;

    /*Free the object itself*/
    lv_free(obj);
}

ಸ್ಥಿರ lv_obj_ವೃಕ್ಷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_data)
{
    lv_obj_ವೃಕ್ಷ_walk_res_t res = LV_OBJ_ವೃಕ್ಷ_WALK_NEXT;

    ಯದಿ(obj == ರಿಕ್ತ) {
        lv_display_t * disp = lv_display_get_next(ರಿಕ್ತ);
        ಯಾವತ್(disp) {
            uint32_t i;
            ಪ್ರತಿ(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        ಪ್ರತ್ಯಾಗಮ LV_OBJ_ವೃಕ್ಷ_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    ಯದಿ(res == LV_OBJ_ವೃಕ್ಷ_WALK_END) ಪ್ರತ್ಯಾಗಮ LV_OBJ_ವೃಕ್ಷ_WALK_END;

    ಯದಿ(res != LV_OBJ_ವೃಕ್ಷ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < lv_obj_get_child_ಎಣಿಕೆ(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            ಯದಿ(res == LV_OBJ_ವೃಕ್ಷ_WALK_END) ಪ್ರತ್ಯಾಗಮ LV_OBJ_ವೃಕ್ಷ_WALK_END;
        }
    }
    ಪ್ರತ್ಯಾಗಮ LV_OBJ_ವೃಕ್ಷ_WALK_NEXT;
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_ವೃಕ್ಷ_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
#ಯದಿ LV_USE_LOG
    ಧ್ರುವ ವರ್ಣ * id;

#ಯದಿ LV_USE_OBJ_ID
    ವರ್ಣ buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, ಪರಿಮಾಣ(buf));
    ಯದಿ(id == ರಿಕ್ತ) id = "obj0";
#ಅನ್ಯಥಾ
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (ಪೂರ್ಣಾಙ್ಕ)(2 * depth), "", (ಶೂನ್ಯ *)obj, id);
#endif /*LV_USE_LOG*/

    ಯದಿ(obj->spec_attr && obj->spec_attr->child_cnt) {
        ಪ್ರತಿ(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_ವೃಕ್ಷ_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

ಸ್ಥಿರ lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != ರಿಕ್ತ);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ಪ್ರತಿ(i = 0; i < cnt; i++) {
        ಯದಿ(!obj->spec_attr->children[i]->is_deleting) {
            ಪ್ರತ್ಯಾಗಮ obj->spec_attr->children[i];
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#ಯದಿ LV_USE_OBJ_NAME

ಸ್ಥಿರ lv_obj_t * find_by_name_direct(ಧ್ರುವ lv_obj_t * parent, ಧ್ರುವ ವರ್ಣ * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ಎಣಿಕೆ(parent);
    ಪ್ರತಿ(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        ವರ್ಣ child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, ಪರಿಮಾಣ(child_name_resolved));

        ಯದಿ(len == UINT16_MAX) {
            ಯದಿ(lv_strcmp(child_name_resolved, name) == 0) ಪ್ರತ್ಯಾಗಮ child;
        }
        ಅನ್ಯಥಾ {
            ಯದಿ(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                ಪ್ರತ್ಯಾಗಮ child;
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif /*LV_USE_OBJ_NAME*/
