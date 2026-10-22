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
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)
#ವ್ಯಾಖ್ಯಾನಿಸು disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#ವ್ಯಾಖ್ಯಾನಿಸು OBJ_DUMP_STRING_LEN 128
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_delete_async_cb(ಶೂನ್ಯ * obj);
ಸ್ಥಿರ ಶೂನ್ಯ obj_delete_core(lv_obj_t * obj);
ಸ್ಥಿರ lv_obj_ವೃಕ್ಷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ dump_ವೃಕ್ಷ_core(lv_obj_t * obj, int32_t ಆಳ);
ಸ್ಥಿರ lv_obj_t * lv_obj_get_ಮೊದಲ_not_deleting_ಮಗು(lv_obj_t * obj);
#ಆದರೆ LV_USE_OBJ_NAME
    ಸ್ಥಿರ lv_obj_t * ಕಂಡುಹಿಡಿ_by_name_ನೇರ(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name, ಗಾತ್ರ_t len);
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_NAME*/

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
    ಆದರೆ(obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(obj->is_deleting) ಹಿಂತಿರುಗಿ;

    LV_LOG_TRACE("begin (delete %p)", (ಶೂನ್ಯ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_ಪೋಷಕ(obj);

    lv_display_t * disp = ಖಾಲಿ;
    bool act_screen_del = false;
    ಆದರೆ(par == ಖಾಲಿ) {
        disp = lv_obj_get_display(obj);
        ಆದರೆ(!disp) ಹಿಂತಿರುಗಿ;   /*Shouldn't happen*/
        ಆದರೆ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ಆದರೆ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, ಖಾಲಿ);
        lv_obj_send_event(par, LV_EVENT_ಮಗು_DELETED, ಖಾಲಿ);
    }

    /*Handle if the active screen was deleted*/
    ಆದರೆ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ಖಾಲಿ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (ಶೂನ್ಯ *)obj);
}

ಶೂನ್ಯ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (ಶೂನ್ಯ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    lv_obj_t * ಮಗು = lv_obj_get_ಮೊದಲ_not_deleting_ಮಗು(obj);
    ಆಗ(ಮಗು) {
        obj_delete_core(ಮಗು);
        ಮಗು = lv_obj_get_ಮೊದಲ_not_deleting_ಮಗು(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_ಗಮ್ಯ(obj, 0, 0, LV_ANIM_OFF);
    ಆದರೆ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ಆದರೆ(lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, ಖಾಲಿ);
        lv_obj_send_event(obj, LV_EVENT_ಮಗು_DELETED, ಖಾಲಿ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (ಶೂನ್ಯ *)obj);
}

ಶೂನ್ಯ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ಆದರೆ(obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ಗಣ_var(&a, obj);
    lv_anim_ಗಣ_exec_cb(&a, ಖಾಲಿ);
    lv_anim_ಗಣ_duration(&a, 1);
    lv_anim_ಗಣ_delay(&a, delay_ms);
    lv_anim_ಗಣ_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_ಆರಂಭ(&a);
}

ಶೂನ್ಯ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_obj_delete(a->var);
}

ಶೂನ್ಯ lv_obj_delete_async(lv_obj_t * obj)
{
    ಆದರೆ(obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

ಶೂನ್ಯ lv_obj_ಗಣ_ಪೋಷಕ(lv_obj_t * obj, lv_obj_t * ಪೋಷಕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(ಪೋಷಕ, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(obj->ಪೋಷಕ == ಖಾಲಿ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(ಪೋಷಕ == obj->ಪೋಷಕ) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(!lv_obj_allocate_spec_attr(ಪೋಷಕ)) {
        ಹಿಂತಿರುಗಿ;
    }

    /*Add the object to the new parents list first*/
    ಆದರೆ(lv_obj_ಕೂಡಿಸು_ಮಗು(ಪೋಷಕ, obj) != LV_ಫಲಿತಾಂಶ_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ಹಿಂತಿರುಗಿ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_ಮಗು(obj->ಪೋಷಕ, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->ಪೋಷಕ);
    lv_obj_send_event(obj->ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj);
    lv_obj_send_event(obj->ಪೋಷಕ, LV_EVENT_ಮಗು_DELETED, ಖಾಲಿ);
    obj->ಪೋಷಕ = ಪೋಷಕ;

    /*Notify the new parent about the child*/
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj);
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_CREATED, ಖಾಲಿ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

ಶೂನ್ಯ lv_obj_move_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(lv_obj_t * obj, int32_t ಸೂಚ್ಯಂಕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /* Check parent validity */
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(!ಪೋಷಕ) {
        LV_LOG_WARN("parent is NULL");
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ uint32_t ಪೋಷಕ_ಮಗು_ಎಣಿಕೆ = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ಸ್ಥಿರಾಂಕ int32_t old_ಸೂಚ್ಯಂಕ = lv_obj_get_ಸೂಚ್ಯಂಕ(obj);
    LV_ASSERT(0 <= old_ಸೂಚ್ಯಂಕ);

    ಆದರೆ(ಸೂಚ್ಯಂಕ < 0) {
        ಸೂಚ್ಯಂಕ += ಪೋಷಕ_ಮಗು_ಎಣಿಕೆ;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ಆದರೆ((ಸೂಚ್ಯಂಕ < 0)
       /* Index is same or bigger than parent child count */
       || (ಸೂಚ್ಯಂಕ >= (int32_t) ಪೋಷಕ_ಮಗು_ಎಣಿಕೆ)
       /* If both previous and new index are the same */
       || (ಸೂಚ್ಯಂಕ == old_ಸೂಚ್ಯಂಕ)) {

        ಹಿಂತಿರುಗಿ;
    }

    int32_t i = old_ಸೂಚ್ಯಂಕ;
    ಆದರೆ(ಸೂಚ್ಯಂಕ < old_ಸೂಚ್ಯಂಕ) {
        ಆಗ(i > ಸೂಚ್ಯಂಕ)  {
            ಪೋಷಕ->spec_attr->children[i] = ಪೋಷಕ->spec_attr->children[i - 1];
            i--;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆಗ(i < ಸೂಚ್ಯಂಕ) {
            ಪೋಷಕ->spec_attr->children[i] = ಪೋಷಕ->spec_attr->children[i + 1];
            i++;
        }
    }

    ಪೋಷಕ->spec_attr->children[ಸೂಚ್ಯಂಕ] = obj;
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, ಖಾಲಿ);
    lv_obj_invalidate(ಪೋಷಕ);
}

ಶೂನ್ಯ lv_obj_ಅದಲುಬದಲು(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(obj2, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj1);
    lv_obj_t * parent2 = lv_obj_get_ಪೋಷಕ(obj2);

    uint_fast32_t index1 = lv_obj_get_ಸೂಚ್ಯಂಕ(obj1);
    uint_fast32_t index2 = lv_obj_get_ಸೂಚ್ಯಂಕ(obj2);

    lv_obj_send_event(parent2, LV_EVENT_ಮಗು_DELETED, obj2);
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_DELETED, obj1);

    ಪೋಷಕ->spec_attr->children[index1] = obj2;
    obj2->ಪೋಷಕ = ಪೋಷಕ;

    parent2->spec_attr->children[index2] = obj1;
    obj1->ಪೋಷಕ = parent2;

    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj2);
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj1);
    lv_obj_send_event(parent2, LV_EVENT_ಮಗು_CREATED, obj1);

    lv_obj_invalidate(ಪೋಷಕ);

    ಆದರೆ(ಪೋಷಕ != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_ಗುಂಪು_ಅದಲುಬದಲು_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಸ್ಥಿರಾಂಕ lv_obj_t * par = obj;
    ಸ್ಥಿರಾಂಕ lv_obj_t * act_par;

    ಮಾಡು {
        act_par = par;
        par = lv_obj_get_ಪೋಷಕ(act_par);
    } ಆಗ(par != ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಸ್ಥಿರಾಂಕ lv_obj_t * scr;

    ಆದರೆ(obj->ಪೋಷಕ == ಖಾಲಿ) scr = obj;  /*`obj` is a screen*/
    ಇಲ್ಲದಿದ್ದರೆ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_ತಲೆ = disp_ll_p;
    LV_LL_READ(disp_ತಲೆ, d) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < d->screen_cnt; i++) {
            ಆದರೆ(d->screens[i] == scr) ಹಿಂತಿರುಗಿ d;
        }
    }

    LV_LOG_WARN("No screen found");
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

lv_obj_t * lv_obj_get_ಪೋಷಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ obj->ಪೋಷಕ;
}

lv_obj_t * lv_obj_get_ಮಗು(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ(idx < 0) {
        idx = obj->spec_attr->ಮಗು_cnt + idx;
        ಆದರೆ(idx < 0) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    uint32_t idu = (uint32_t) idx;

    ಆದರೆ(idu < obj->spec_attr->ಮಗು_cnt) ಹಿಂತಿರುಗಿ obj->spec_attr->children[idu];
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

lv_obj_t * lv_obj_get_ಮಗು_by_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, int32_t idx, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ಮಗು_cnt;
    ಆದರೆ(idx >= 0) {
        ಗಾಗಿ(i = 0; i < cnt; i++) {
            ಆದರೆ(obj->spec_attr->children[i]->class_p == class_p) {
                ಆದರೆ(idx == 0) ಹಿಂತಿರುಗಿ obj->spec_attr->children[i];
                ಇಲ್ಲದಿದ್ದರೆ idx--;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        idx++;   /*-1 means the first child*/
        ಗಾಗಿ(i = cnt - 1; i >= 0; i--) {
            ಆದರೆ(obj->spec_attr->children[i]->class_p == class_p) {
                ಆದರೆ(idx == 0) ಹಿಂತಿರುಗಿ obj->spec_attr->children[i];
                ಇಲ್ಲದಿದ್ದರೆ idx++;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

lv_obj_t * lv_obj_get_sibling(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(obj->ಪೋಷಕ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_ಸೂಚ್ಯಂಕ(obj) + idx;
    ಆದರೆ(sibling_idx < 0) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ lv_obj_get_ಮಗು(ಪೋಷಕ, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, int32_t idx, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(obj->ಪೋಷಕ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_ಸೂಚ್ಯಂಕ_by_type(obj, class_p) + idx;
    ಆದರೆ(sibling_idx < 0) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ lv_obj_get_ಮಗು_by_type(ಪೋಷಕ, sibling_idx, class_p);
}

uint32_t lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);
    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ obj->spec_attr->ಮಗು_cnt;
}

uint32_t lv_obj_get_ಮಗು_ಎಣಿಕೆ_by_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;

    uint32_t i;
    uint32_t cnt = 0;
    ಗಾಗಿ(i = 0; i < obj->spec_attr->ಮಗು_cnt; i++) {
        ಆದರೆ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ಹಿಂತಿರುಗಿ cnt;
}

#ಆದರೆ LV_USE_OBJ_NAME

ಶೂನ್ಯ lv_obj_ಗಣ_name(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ಶೂನ್ಯ *)obj->spec_attr->name);

    ಆದರೆ(name == ಖಾಲಿ) {
        obj->spec_attr->name = ಖಾಲಿ;
        obj->spec_attr->name_static = 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_name_static(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(name != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ಶೂನ್ಯ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * lv_obj_get_name(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ obj->spec_attr->name;
}

ಶೂನ್ಯ lv_obj_get_name_resolved(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಅಕ್ಷರ buf[], ಗಾತ್ರ_t buf_ಗಾತ್ರ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(buf != ಖಾಲಿ && buf_ಗಾತ್ರ > 0, ಹಿಂತಿರುಗಿ);

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    ಅಕ್ಷರ name_buf[LV_OBJ_NAME_MAX_LEN];
    ಆದರೆ(name == ಖಾಲಿ) {
        lv_snprintf(name_buf, ಗಾತ್ರ(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    ಗಾತ್ರ_t name_len = lv_strlen(name);
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ಆದರೆ(ಪೋಷಕ && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
        uint32_t cnt = 0;
        uint32_t i;
        ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
            lv_obj_t * ಮಗು = lv_obj_get_ಮಗು(ಪೋಷಕ, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ಆದರೆ(ಮಗು == obj) {
                ಅಕ್ಷರ ಅಂಕೆ_buf[8];
                ಗಾತ್ರ_t ಅಂಕೆ_len;
                ಅಂಕೆ_len = lv_snprintf(ಅಂಕೆ_buf, ಗಾತ್ರ(ಅಂಕೆ_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ಆದರೆ(buf_ಗಾತ್ರ > name_len + ಅಂಕೆ_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], ಅಂಕೆ_buf);
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_ಗಾತ್ರ);
                }
                ನಿಲ್ಲಿಸು;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ಇಲ್ಲದಿದ್ದರೆ {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮಗು_name = lv_obj_get_name(ಮಗು);
                ಆದರೆ(ಮಗು_name == ಖಾಲಿ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    ಗಾತ್ರ_t class_name_len = lv_strlen(ಮಗು->class_p->name);
                    ಆದರೆ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(ಮಗು->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ಇಲ್ಲದಿದ್ದರೆ {
                    ಆದರೆ(lv_strcmp(ಮಗು->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_ಗಾತ್ರ);
    }
}

lv_obj_t * lv_obj_get_ಮಗು_by_name(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮಾರ್ಗ)
{
    LV_CHECK_OBJ(ಪೋಷಕ, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಮಾರ್ಗ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಪೋಷಕ->spec_attr != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆಗ(*ಮಾರ್ಗ) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * segment = ಮಾರ್ಗ;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ಆಗ(ಮಾರ್ಗ[len] && ಮಾರ್ಗ[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * ಮಗು = ಕಂಡುಹಿಡಿ_by_name_ನೇರ(ಪೋಷಕ, segment, len);
        ಆದರೆ(!ಮಗು) ಹಿಂತಿರುಗಿ ಖಾಲಿ; /*Segment not found*/

        /* Advance to the next segment */
        ಮಾರ್ಗ += len;
        ಆದರೆ(*ಮಾರ್ಗ == '/') ಮಾರ್ಗ++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ಆದರೆ(*ಮಾರ್ಗ == '\0') ಹಿಂತಿರುಗಿ ಮಗು;

        ಪೋಷಕ = ಮಗು;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

}

lv_obj_t * lv_obj_ಕಂಡುಹಿಡಿ_by_name(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_ARG(name != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಪೋಷಕ = lv_display_get_screen_ಸಕ್ರಿಯ(lv_display_get_default());
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    lv_obj_t * ಮಗು = ಕಂಡುಹಿಡಿ_by_name_ನೇರ(ಪೋಷಕ, name, UINT16_MAX);
    ಆದರೆ(ಮಗು) ಹಿಂತಿರುಗಿ ಮಗು;

    /*Search children recursively*/
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
    uint32_t i;
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        ಮಗು = ಪೋಷಕ->spec_attr->children[i];
        lv_obj_t * ಸಿಕ್ಕಿತು = lv_obj_ಕಂಡುಹಿಡಿ_by_name(ಮಗು, name);
        ಆದರೆ(ಸಿಕ್ಕಿತು != ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ -1);

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ -1;

    uint32_t i = 0;
    ಗಾಗಿ(i = 0; i < ಪೋಷಕ->spec_attr->ಮಗು_cnt; i++) {
        ಆದರೆ(ಪೋಷಕ->spec_attr->children[i] == obj) ಹಿಂತಿರುಗಿ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ಹಿಂತಿರುಗಿ -1;
}

int32_t lv_obj_get_ಸೂಚ್ಯಂಕ_by_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ -1);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ -1);

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    ಗಾಗಿ(i = 0; i < ಪೋಷಕ->spec_attr->ಮಗು_cnt; i++) {
        lv_obj_t * ಮಗು = ಪೋಷಕ->spec_attr->children[i];
        ಆದರೆ(ಮಗು->class_p == class_p) {
            ಆದರೆ(ಮಗು == obj) ಹಿಂತಿರುಗಿ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ಹಿಂತಿರುಗಿ -1;
}

ಶೂನ್ಯ lv_obj_ವೃಕ್ಷ_walk(lv_obj_t * ಆರಂಭ_obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    walk_core(ಆರಂಭ_obj, cb, user_ದತ್ತಾಂಶ);
}

ಶೂನ್ಯ lv_obj_dump_ವೃಕ್ಷ(lv_obj_t * ಆರಂಭ_obj)
{
    ಆದರೆ(ಆರಂಭ_obj == ಖಾಲಿ) {
        lv_display_t * disp = lv_display_get_ಮುಂದಿನ(ಖಾಲಿ);
        ಆಗ(disp) {
            uint32_t i;
            ಗಾಗಿ(i = 0; i < disp->screen_cnt; i++) {
                dump_ವೃಕ್ಷ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_ಮುಂದಿನ(disp);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        dump_ವೃಕ್ಷ_core(ಆರಂಭ_obj, 0);
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
    ಆದರೆ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    ಆದರೆ(obj->is_deleting)
        ಹಿಂತಿರುಗಿ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ಖಾಲಿ);
    ಆದರೆ(res == LV_ಫಲಿತಾಂಶ_INVALID) {
        obj->is_deleting = false;
        ಹಿಂತಿರುಗಿ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ಆದರೆ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ಆದರೆ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * ಮಗು = lv_obj_get_ಮಗು(obj, 0);
    ಆಗ(ಮಗು) {
        obj_delete_core(ಮಗು);
        ಮಗು = lv_obj_get_ಮಗು(obj, 0);
    }

    lv_ಗುಂಪು_t * ಗುಂಪು = lv_obj_get_ಗುಂಪು(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_ಮುಂದಿನ(ಖಾಲಿ);
    ಆಗ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ಆದರೆ(indev_type == LV_INDEV_TYPE_ಸೂಚಕ || indev_type == LV_INDEV_TYPE_BUTTON) {
            ಆದರೆ(indev->ಸೂಚಕ.act_obj == obj || indev->ಸೂಚಕ.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ಆದರೆ(indev->ಸೂಚಕ.ಕೊನೆಯ_pressed == obj) {
                indev->ಸೂಚಕ.ಕೊನೆಯ_pressed = ಖಾಲಿ;
            }
            ಆದರೆ(indev->ಸೂಚಕ.ಕೊನೆಯ_hovered == obj) {
                indev->ಸೂಚಕ.ಕೊನೆಯ_hovered = ಖಾಲಿ;
            }
        }

        ಆದರೆ(indev->ಗುಂಪು == ಗುಂಪು && obj == lv_indev_get_ಸಕ್ರಿಯ_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_ಮುಂದಿನ(indev);
    }

    /*Delete all pending async del-s*/
    lv_ಫಲಿತಾಂಶ_t async_cancel_res = LV_ಫಲಿತಾಂಶ_OK;
    ಆಗ(async_cancel_res == LV_ಫಲಿತಾಂಶ_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ಆದರೆ(obj->ಪೋಷಕ == ಖಾಲಿ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        ಗಾಗಿ(i = 0; i < disp->screen_cnt; i++) {
            ಆದರೆ(disp->screens[i] == obj) ನಿಲ್ಲಿಸು;
        }

        uint32_t id = i;
        ಗಾಗಿ(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * ಗಾತ್ರ(lv_obj_t *));
        ಆದರೆ(new_screens != ಖಾಲಿ) {
            disp->screens = new_screens;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_obj_remove_ಮಗು(obj->ಪೋಷಕ, obj);
    }

    obj->ಪೋಷಕ = ಖಾಲಿ;

    /*Free the object itself*/
    lv_free(obj);
}

ಸ್ಥಿರ lv_obj_ವೃಕ್ಷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ವೃಕ್ಷ_walk_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    lv_obj_ವೃಕ್ಷ_walk_res_t res = LV_OBJ_ವೃಕ್ಷ_WALK_ಮುಂದಿನ;

    ಆದರೆ(obj == ಖಾಲಿ) {
        lv_display_t * disp = lv_display_get_ಮುಂದಿನ(ಖಾಲಿ);
        ಆಗ(disp) {
            uint32_t i;
            ಗಾಗಿ(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_ದತ್ತಾಂಶ);
            }
            disp = lv_display_get_ಮುಂದಿನ(disp);
        }
        ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಕೊನೆ;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_ದತ್ತಾಂಶ);

    ಆದರೆ(res == LV_OBJ_ವೃಕ್ಷ_WALK_ಕೊನೆ) ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಕೊನೆ;

    ಆದರೆ(res != LV_OBJ_ವೃಕ್ಷ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj); i++) {
            res = walk_core(lv_obj_get_ಮಗು(obj, i), cb, user_ದತ್ತಾಂಶ);
            ಆದರೆ(res == LV_OBJ_ವೃಕ್ಷ_WALK_ಕೊನೆ) ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಕೊನೆ;
        }
    }
    ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಮುಂದಿನ;
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_ವೃಕ್ಷ_core(lv_obj_t * obj, int32_t ಆಳ)
{
    LV_ASSERT(obj != ಖಾಲಿ);
#ಆದರೆ LV_USE_LOG
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * id;

#ಆದರೆ LV_USE_OBJ_ID
    ಅಕ್ಷರ buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, ಗಾತ್ರ(buf));
    ಆದರೆ(id == ಖಾಲಿ) id = "obj0";
#ಇಲ್ಲದಿದ್ದರೆ
    id = "obj0";
#ಆದರೆ_ಅಂತ್ಯ

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (ಪೂರ್ಣಾಂಕ)(2 * ಆಳ), "", (ಶೂನ್ಯ *)obj, id);
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_LOG*/

    ಆದರೆ(obj->spec_attr && obj->spec_attr->ಮಗು_cnt) {
        ಗಾಗಿ(uint32_t i = 0; i < obj->spec_attr->ಮಗು_cnt; i++) {
            dump_ವೃಕ್ಷ_core(lv_obj_get_ಮಗು(obj, i), ಆಳ + 1);
        }
    }
}

ಸ್ಥಿರ lv_obj_t * lv_obj_get_ಮೊದಲ_not_deleting_ಮಗು(lv_obj_t * obj)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ಮಗು_cnt;
    ಗಾಗಿ(i = 0; i < cnt; i++) {
        ಆದರೆ(!obj->spec_attr->children[i]->is_deleting) {
            ಹಿಂತಿರುಗಿ obj->spec_attr->children[i];
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ LV_USE_OBJ_NAME

ಸ್ಥಿರ lv_obj_t * ಕಂಡುಹಿಡಿ_by_name_ನೇರ(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name, ಗಾತ್ರ_t len)
{
    uint32_t i;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        lv_obj_t * ಮಗು = ಪೋಷಕ->spec_attr->children[i];

        ಅಕ್ಷರ ಮಗು_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(ಮಗು, ಮಗು_name_resolved, ಗಾತ್ರ(ಮಗು_name_resolved));

        ಆದರೆ(len == UINT16_MAX) {
            ಆದರೆ(lv_strcmp(ಮಗು_name_resolved, name) == 0) ಹಿಂತಿರುಗಿ ಮಗು;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ(lv_strncmp(ಮಗು_name_resolved, name, len) == 0 &&
               ಮಗು_name_resolved[len] == '\0') {
                ಹಿಂತಿರುಗಿ ಮಗು;
            }
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_NAME*/
