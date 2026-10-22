/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_group_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು default_ಗುಂಪು LV_GLOBAL_DEFAULT()->ಗುಂಪು_default
#ವ್ಯಾಖ್ಯಾನಿಸು ಗುಂಪು_ll_p &(LV_GLOBAL_DEFAULT()->ಗುಂಪು_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ bool focus_ಮುಂದಿನ_core(lv_ಗುಂಪು_t * ಗುಂಪು, ಶೂನ್ಯ * (*begin)(ಸ್ಥಿರಾಂಕ lv_ll_t *),
                            ಶೂನ್ಯ * (*move)(ಸ್ಥಿರಾಂಕ lv_ll_t *, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *));
ಸ್ಥಿರ ಶೂನ್ಯ lv_ಗುಂಪು_refocus(lv_ಗುಂಪು_t * g);
ಸ್ಥಿರ lv_indev_t * get_indev(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_ಗುಂಪು_init(ಶೂನ್ಯ)
{
    lv_ll_init(ಗುಂಪು_ll_p, ಗಾತ್ರ(lv_ಗುಂಪು_t));
}

ಶೂನ್ಯ lv_ಗುಂಪು_deinit(ಶೂನ್ಯ)
{
    lv_ll_clear(ಗುಂಪು_ll_p);
}

lv_ಗುಂಪು_t * lv_ಗುಂಪು_create(ಶೂನ್ಯ)
{
    lv_ಗುಂಪು_t * ಗುಂಪು = lv_ll_ins_ತಲೆ(ಗುಂಪು_ll_p);
    LV_ASSERT_MALLOC(ಗುಂಪು);
    ಆದರೆ(ಗುಂಪು == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    lv_ll_init(&ಗುಂಪು->obj_ll, ಗಾತ್ರ(lv_obj_t *));

    ಗುಂಪು->obj_focus      = ಖಾಲಿ;
    ಗುಂಪು->frozen         = 0;
    ಗುಂಪು->focus_cb       = ಖಾಲಿ;
    ಗುಂಪು->ಅಂಚು_cb        = ಖಾಲಿ;
    ಗುಂಪು->editing        = 0;
    ಗುಂಪು->refocus_policy = LV_ಗುಂಪು_REFOCUS_POLICY_PREV;
    ಗುಂಪು->wrap           = 1;
    ಗುಂಪು->user_ದತ್ತಾಂಶ      = ಖಾಲಿ;
#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    ಗುಂಪು->ext_ದತ್ತಾಂಶ.free_cb = ಖಾಲಿ;
    ಗುಂಪು->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ಗುಂಪು;
}

ಶೂನ್ಯ lv_ಗುಂಪು_delete(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    ಆದರೆ(ಗುಂಪು == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    /*Defocus the currently focused object*/
    ಆದರೆ(ಗುಂಪು->obj_focus != ಖಾಲಿ) {
        lv_obj_send_event(*ಗುಂಪು->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ಗುಂಪು));
        lv_obj_invalidate(*ಗುಂಪು->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ಗುಂಪು->obj_ll, obj) {
        ಆದರೆ((*obj)->spec_attr)(*obj)->spec_attr->ಗುಂಪು_p = ಖಾಲಿ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_ಮುಂದಿನ(ಖಾಲಿ);
    ಆಗ(indev) {
        ಆದರೆ(lv_indev_get_ಗುಂಪು(indev) == ಗುಂಪು) {
            lv_indev_ಗಣ_ಗುಂಪು(indev, ಖಾಲಿ);
        }
        indev = lv_indev_get_ಮುಂದಿನ(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ಆದರೆ(ಗುಂಪು == lv_ಗುಂಪು_get_default()) lv_ಗುಂಪು_ಗಣ_default(ಖಾಲಿ);

    lv_ll_clear(&(ಗುಂಪು->obj_ll));
    lv_ll_remove(ಗುಂಪು_ll_p, ಗುಂಪು);
#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    ಆದರೆ(ಗುಂಪು->ext_ದತ್ತಾಂಶ.free_cb) {
        ಗುಂಪು->ext_ದತ್ತಾಂಶ.free_cb(ಗುಂಪು->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ);
        ಗುಂಪು->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    lv_free(ಗುಂಪು);
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_default(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    default_ಗುಂಪು = ಗುಂಪು;
}

lv_ಗುಂಪು_t * lv_ಗುಂಪು_get_default(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ default_ಗುಂಪು;
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಕೂಡಿಸು_obj(lv_ಗುಂಪು_t * ಗುಂಪು, lv_obj_t * obj)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_LOG_TRACE("begin");

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }

    LV_ASSERT(obj->spec_attr != ಖಾಲಿ);

    /*Be sure the object is removed from its current group*/
    lv_ಗುಂಪು_remove_obj(obj);
    obj->spec_attr->ಗುಂಪು_p = ಗುಂಪು;

    lv_obj_t ** ಮುಂದಿನ = lv_ll_ins_ಬಾಲ(&ಗುಂಪು->obj_ll);
    LV_ASSERT_MALLOC(ಮುಂದಿನ);
    ಆದರೆ(ಮುಂದಿನ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
    *ಮುಂದಿನ = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ಆದರೆ(lv_ll_get_ತಲೆ(&ಗುಂಪು->obj_ll) == ಮುಂದಿನ) {
        lv_ಗುಂಪು_refocus(ಗುಂಪು);
    }

    LV_LOG_TRACE("finished");
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಅದಲುಬದಲು_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(obj2 != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_ಗುಂಪು_t * g1 = lv_obj_get_ಗುಂಪು(obj1);
    lv_ಗುಂಪು_t * g2 = lv_obj_get_ಗುಂಪು(obj2);

    ಆದರೆ(g1 != g2) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(g1 == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ಆದರೆ((*obj_i) == obj1)(*obj_i) = obj2;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_ಗುಂಪು_get_focused(g1);
    ಆದರೆ(focused == obj1) lv_ಗುಂಪು_focus_obj(obj2);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(focused == obj2) lv_ಗುಂಪು_focus_obj(obj1);
}

ಶೂನ್ಯ lv_ಗುಂಪು_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_ಗುಂಪು_t * g = lv_obj_get_ಗುಂಪು(obj);
    ಆದರೆ(g == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ಆದರೆ(g->obj_focus && *g->obj_focus == obj) {
        ಆದರೆ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ಆದರೆ(lv_ll_get_ತಲೆ(&g->obj_ll) != g->obj_focus || lv_ll_get_ಬಾಲ(&g->obj_ll) != g->obj_focus) {
            lv_ಗುಂಪು_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ಆದರೆ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ಖಾಲಿ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ಆದರೆ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ಆದರೆ(obj->spec_attr) obj->spec_attr->ಗುಂಪು_p = ಖಾಲಿ;
            ನಿಲ್ಲಿಸು;
        }
    }
    LV_LOG_TRACE("finished");
}

ಶೂನ್ಯ lv_ಗುಂಪು_remove_all_objs(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    /*Defocus the currently focused object*/
    ಆದರೆ(ಗುಂಪು->obj_focus != ಖಾಲಿ) {
        lv_obj_send_event(*ಗುಂಪು->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ಗುಂಪು));
        lv_obj_invalidate(*ಗುಂಪು->obj_focus);
        ಗುಂಪು->obj_focus = ಖಾಲಿ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ಗುಂಪು->obj_ll, obj) {
        ಆದರೆ((*obj)->spec_attr)(*obj)->spec_attr->ಗುಂಪು_p = ಖಾಲಿ;
    }

    lv_ll_clear(&(ಗುಂಪು->obj_ll));
}

ಶೂನ್ಯ lv_ಗುಂಪು_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_ಗುಂಪು_t * g = lv_obj_get_ಗುಂಪು(obj);

    ಆದರೆ(g == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(g->frozen) ಹಿಂತಿರುಗಿ;

    /*On defocus edit mode must be leaved*/
    lv_ಗುಂಪು_ಗಣ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ಆದರೆ(*i == obj) {
            ಆದರೆ(g->obj_focus != ಖಾಲಿ) {
                ಆದರೆ(*g->obj_focus == obj) ನಿಲ್ಲಿಸು;  /*Refocusing same object*/
                lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ಆದರೆ(g->focus_cb) g->focus_cb(g);
            lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
            lv_obj_invalidate(*g->obj_focus);
            ನಿಲ್ಲಿಸು;
        }
    }
}

ಶೂನ್ಯ lv_ಗುಂಪು_focus_ಮುಂದಿನ(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    bool focus_ಬದಲಾಯಿತು = focus_ಮುಂದಿನ_core(ಗುಂಪು, lv_ll_get_ತಲೆ, lv_ll_get_ಮುಂದಿನ);

    ಆದರೆ(!focus_ಬದಲಾಯಿತು && ಗುಂಪು->ಅಂಚು_cb) {
        ಗುಂಪು->ಅಂಚು_cb(ಗುಂಪು, true);
    }
}

ಶೂನ್ಯ lv_ಗುಂಪು_focus_prev(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    bool focus_ಬದಲಾಯಿತು = focus_ಮುಂದಿನ_core(ಗುಂಪು, lv_ll_get_ಬಾಲ, lv_ll_get_prev);

    ಆದರೆ(!focus_ಬದಲಾಯಿತು && ಗುಂಪು->ಅಂಚು_cb) {
        ಗುಂಪು->ಅಂಚು_cb(ಗುಂಪು, false);
    }
}

ಶೂನ್ಯ lv_ಗುಂಪು_focus_freeze(lv_ಗುಂಪು_t * ಗುಂಪು, bool en)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(en == false) ಗುಂಪು->frozen = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಗುಂಪು->frozen = 1;
}

lv_ಫಲಿತಾಂಶ_t lv_ಗುಂಪು_send_ದತ್ತಾಂಶ(lv_ಗುಂಪು_t * ಗುಂಪು, uint32_t c)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    lv_obj_t * act = lv_ಗುಂಪು_get_focused(ಗುಂಪು);
    ಆದರೆ(act == ಖಾಲಿ) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;

    ಆದರೆ(lv_obj_has_state(act, LV_STATE_DISABLED)) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;

    ಹಿಂತಿರುಗಿ lv_obj_send_event(act, LV_EVENT_ಕೀಲಿ, &c);
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_focus_cb(lv_ಗುಂಪು_t * ಗುಂಪು, lv_ಗುಂಪು_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಗುಂಪು->focus_cb = focus_cb;
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_ಅಂಚು_cb(lv_ಗುಂಪು_t * ಗುಂಪು, lv_ಗುಂಪು_ಅಂಚು_cb_t ಅಂಚು_cb)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಗುಂಪು->ಅಂಚು_cb = ಅಂಚು_cb;
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_editing(lv_ಗುಂಪು_t * ಗುಂಪು, bool edit)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    uint8_t en_val = edit ? 1 : 0;

    ಆದರೆ(en_val == ಗುಂಪು->editing) ಹಿಂತಿರುಗಿ; /*Do not set the same mode again*/

    ಗುಂಪು->editing     = en_val;
    lv_obj_t * focused = lv_ಗುಂಪು_get_focused(ಗುಂಪು);

    ಆದರೆ(focused) {
        lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(*ಗುಂಪು->obj_focus, LV_EVENT_FOCUSED, get_indev(ಗುಂಪು));
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

        lv_obj_invalidate(focused);
    }
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_refocus_policy(lv_ಗುಂಪು_t * ಗುಂಪು, lv_ಗುಂಪು_refocus_policy_t policy)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    ಗುಂಪು->refocus_policy = policy & 0x01;
}

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_wrap(lv_ಗುಂಪು_t * ಗುಂಪು, bool en)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    ಗುಂಪು->wrap = en ? 1 : 0;
}

lv_obj_t * lv_ಗುಂಪು_get_focused(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಆದರೆ(ಗುಂಪು->obj_focus == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ *ಗುಂಪು->obj_focus;
}

lv_ಗುಂಪು_focus_cb_t lv_ಗುಂಪು_get_focus_cb(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ ಗುಂಪು->focus_cb;
}

lv_ಗುಂಪು_ಅಂಚು_cb_t lv_ಗುಂಪು_get_ಅಂಚು_cb(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ ಗುಂಪು->ಅಂಚು_cb;
}

bool lv_ಗುಂಪು_get_editing(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ ಗುಂಪು->editing;
}

bool lv_ಗುಂಪು_get_wrap(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ ಗುಂಪು->wrap;
}

uint32_t lv_ಗುಂಪು_get_obj_ಎಣಿಕೆ(lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    ಹಿಂತಿರುಗಿ lv_ll_get_len(&ಗುಂಪು->obj_ll);
}

lv_obj_t * lv_ಗುಂಪು_get_obj_by_ಸೂಚ್ಯಂಕ(lv_ಗುಂಪು_t * ಗುಂಪು, uint32_t ಸೂಚ್ಯಂಕ)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&ಗುಂಪು->obj_ll, obj) {
        ಆದರೆ(len == ಸೂಚ್ಯಂಕ) {
            ಹಿಂತಿರುಗಿ *obj;
        }
        len++;
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

uint32_t lv_ಗುಂಪು_get_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ lv_ll_get_len(ಗುಂಪು_ll_p);
}

lv_ಗುಂಪು_t  * lv_ಗುಂಪು_by_ಸೂಚ್ಯಂಕ(uint32_t ಸೂಚ್ಯಂಕ)
{
    uint32_t len = 0;
    lv_ಗುಂಪು_t * ಗುಂಪು;

    LV_LL_READ_ಹಿಂಭಾಗ(ಗುಂಪು_ll_p, ಗುಂಪು) {
        ಆದರೆ(len == ಸೂಚ್ಯಂಕ) {
            ಹಿಂತಿರುಗಿ ಗುಂಪು;
        }
        len++;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_external_ದತ್ತಾಂಶ(lv_ಗುಂಪು_t * ಗುಂಪು, ಶೂನ್ಯ * ದತ್ತಾಂಶ, ಶೂನ್ಯ (* free_cb)(ಶೂನ್ಯ * ದತ್ತಾಂಶ))
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಗುಂಪು->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    ಗುಂಪು->ext_ದತ್ತಾಂಶ.free_cb = free_cb;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ lv_ಗುಂಪು_ಗಣ_user_ದತ್ತಾಂಶ(lv_ಗುಂಪು_t * ಗುಂಪು, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    ಗುಂಪು->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
}

ಶೂನ್ಯ * lv_ಗುಂಪು_get_user_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * ಗುಂಪು)
{
    LV_CHECK_ARG(ಗುಂಪು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ ಗುಂಪು->user_ದತ್ತಾಂಶ;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ lv_ಗುಂಪು_refocus(lv_ಗುಂಪು_t * g)
{
    LV_ASSERT(g != ಖಾಲಿ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t ತಾತ್ಕಾಲಿಕ_wrap = g->wrap;
    g->wrap           = 1;

    ಆದರೆ(g->refocus_policy == LV_ಗುಂಪು_REFOCUS_POLICY_ಮುಂದಿನ)
        lv_ಗುಂಪು_focus_ಮುಂದಿನ(g);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(g->refocus_policy == LV_ಗುಂಪು_REFOCUS_POLICY_PREV)
        lv_ಗುಂಪು_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = ತಾತ್ಕಾಲಿಕ_wrap;
}

ಸ್ಥಿರ bool focus_ಮುಂದಿನ_core(lv_ಗುಂಪು_t * ಗುಂಪು, ಶೂನ್ಯ * (*begin)(ಸ್ಥಿರಾಂಕ lv_ll_t *),
                            ಶೂನ್ಯ * (*move)(ಸ್ಥಿರಾಂಕ lv_ll_t *, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *))
{
    LV_ASSERT(ಗುಂಪು != ಖಾಲಿ);
    LV_ASSERT(begin != ಖಾಲಿ);
    LV_ASSERT(move != ಖಾಲಿ);
    ಆದರೆ(ಗುಂಪು->frozen) ಹಿಂತಿರುಗಿ false;

    bool focus_ಬದಲಾಯಿತು = false;

    lv_obj_t ** obj_ಮುಂದಿನ     = ಗುಂಪು->obj_focus;
    lv_obj_t ** obj_sentinel = ಖಾಲಿ;
    bool can_move            = true;
    bool can_begin           = true;

    ಗಾಗಿ(;;) {
        ಆದರೆ(obj_ಮುಂದಿನ == ಖಾಲಿ) {
            ಆದರೆ(ಗುಂಪು->wrap || obj_sentinel == ಖಾಲಿ) {
                ಆದರೆ(!can_begin) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;
                obj_ಮುಂದಿನ  = begin(&ಗುಂಪು->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;
            }
        }

        ಆದರೆ(obj_sentinel == ಖಾಲಿ) {
            obj_sentinel = obj_ಮುಂದಿನ;
            ಆದರೆ(obj_sentinel == ಖಾಲಿ) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು; /*Group is empty*/
        }

        ಆದರೆ(can_move) {
            obj_ಮುಂದಿನ = move(&ಗುಂಪು->obj_ll, obj_ಮುಂದಿನ);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ಆದರೆ(obj_ಮುಂದಿನ == obj_sentinel) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;
        }

        can_move = true;

        ಆದರೆ(obj_ಮುಂದಿನ == ಖಾಲಿ) ಮುಂದುವರಿ;
        ಆದರೆ(lv_obj_get_state(*obj_ಮುಂದಿನ) & LV_STATE_DISABLED) ಮುಂದುವರಿ;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * ಪೋಷಕ = *obj_ಮುಂದಿನ;
        ಆಗ(ಪೋಷಕ) {
            ಆದರೆ(lv_obj_is_hidden(ಪೋಷಕ)) ನಿಲ್ಲಿಸು;
            ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಪೋಷಕ);
        }

        ಆದರೆ(ಪೋಷಕ && lv_obj_is_hidden(ಪೋಷಕ)) ಮುಂದುವರಿ;

        /*If we got her a good candidate is found*/
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ(obj_ಮುಂದಿನ == ಗುಂಪು->obj_focus) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು; /*There's only one visible object and it's already focused*/

    ಆದರೆ(ಗುಂಪು->obj_focus) {
        lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(*ಗುಂಪು->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ಗುಂಪು));
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;
        lv_obj_invalidate(*ಗುಂಪು->obj_focus);
    }

    ಗುಂಪು->obj_focus = obj_ಮುಂದಿನ;

    lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(*ಗುಂಪು->obj_focus, LV_EVENT_FOCUSED, get_indev(ಗುಂಪು));
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;

    lv_obj_invalidate(*ಗುಂಪು->obj_focus);

    ಆದರೆ(ಗುಂಪು->focus_cb) ಗುಂಪು->focus_cb(ಗುಂಪು);
    focus_ಬದಲಾಯಿತು = true;
    ಹಿಂತಿರುಗಿ focus_ಬದಲಾಯಿತು;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ಸ್ಥಿರ lv_indev_t * get_indev(ಸ್ಥಿರಾಂಕ lv_ಗುಂಪು_t * g)
{
    LV_ASSERT(g != ಖಾಲಿ);
    lv_indev_t * indev_guess = ಖಾಲಿ;
    lv_indev_t * indev = lv_indev_get_ಮುಂದಿನ(ಖಾಲಿ);

    ಆಗ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ಆದರೆ(indev_type == LV_INDEV_TYPE_ಸೂಚಕ) ಹಿಂತಿರುಗಿ indev;
        ಆದರೆ(lv_indev_get_ಗುಂಪು(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_ಮುಂದಿನ(indev);
    }

    ಹಿಂತಿರುಗಿ indev_guess;
}
