/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_group_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ default_ਸਮੂਹ LV_GLOBAL_DEFAULT()->ਸਮੂਹ_default
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਮੂਹ_ll_p &(LV_GLOBAL_DEFAULT()->ਸਮੂਹ_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ bool focus_ਅਗਲਾ_core(lv_ਸਮੂਹ_t * ਸਮੂਹ, ਖਾਲੀ * (*begin)(ਅਚਲ lv_ll_t *),
                            ਖਾਲੀ * (*move)(ਅਚਲ lv_ll_t *, ਅਚਲ ਖਾਲੀ *));
ਸਥਿਰ ਖਾਲੀ lv_ਸਮੂਹ_refocus(lv_ਸਮੂਹ_t * g);
ਸਥਿਰ lv_indev_t * get_indev(ਅਚਲ lv_ਸਮੂਹ_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ਖਾਲੀ lv_ਸਮੂਹ_init(ਖਾਲੀ)
{
    lv_ll_init(ਸਮੂਹ_ll_p, ਆਕਾਰ(lv_ਸਮੂਹ_t));
}

ਖਾਲੀ lv_ਸਮੂਹ_deinit(ਖਾਲੀ)
{
    lv_ll_clear(ਸਮੂਹ_ll_p);
}

lv_ਸਮੂਹ_t * lv_ਸਮੂਹ_create(ਖਾਲੀ)
{
    lv_ਸਮੂਹ_t * ਸਮੂਹ = lv_ll_ins_ਸਿਰ(ਸਮੂਹ_ll_p);
    LV_ASSERT_MALLOC(ਸਮੂਹ);
    ਜੇ(ਸਮੂਹ == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
    lv_ll_init(&ਸਮੂਹ->obj_ll, ਆਕਾਰ(lv_obj_t *));

    ਸਮੂਹ->obj_focus      = ਨਿਰਾ;
    ਸਮੂਹ->frozen         = 0;
    ਸਮੂਹ->focus_cb       = ਨਿਰਾ;
    ਸਮੂਹ->ਕਿਨਾਰਾ_cb        = ਨਿਰਾ;
    ਸਮੂਹ->editing        = 0;
    ਸਮੂਹ->refocus_policy = LV_ਸਮੂਹ_REFOCUS_POLICY_PREV;
    ਸਮੂਹ->wrap           = 1;
    ਸਮੂਹ->user_ਅੰਕੜੇ      = ਨਿਰਾ;
#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    ਸਮੂਹ->ext_ਅੰਕੜੇ.free_cb = ਨਿਰਾ;
    ਸਮੂਹ->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ਸਮੂਹ;
}

ਖਾਲੀ lv_ਸਮੂਹ_delete(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    ਜੇ(ਸਮੂਹ == ਨਿਰਾ) ਵਾਪਸ;

    /*Defocus the currently focused object*/
    ਜੇ(ਸਮੂਹ->obj_focus != ਨਿਰਾ) {
        lv_obj_send_event(*ਸਮੂਹ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ਸਮੂਹ));
        lv_obj_invalidate(*ਸਮੂਹ->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ਸਮੂਹ->obj_ll, obj) {
        ਜੇ((*obj)->spec_attr)(*obj)->spec_attr->ਸਮੂਹ_p = ਨਿਰਾ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_ਅਗਲਾ(ਨਿਰਾ);
    ਜਦੋਂ(indev) {
        ਜੇ(lv_indev_get_ਸਮੂਹ(indev) == ਸਮੂਹ) {
            lv_indev_ਸੈੱਟ_ਸਮੂਹ(indev, ਨਿਰਾ);
        }
        indev = lv_indev_get_ਅਗਲਾ(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ਜੇ(ਸਮੂਹ == lv_ਸਮੂਹ_get_default()) lv_ਸਮੂਹ_ਸੈੱਟ_default(ਨਿਰਾ);

    lv_ll_clear(&(ਸਮੂਹ->obj_ll));
    lv_ll_remove(ਸਮੂਹ_ll_p, ਸਮੂਹ);
#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    ਜੇ(ਸਮੂਹ->ext_ਅੰਕੜੇ.free_cb) {
        ਸਮੂਹ->ext_ਅੰਕੜੇ.free_cb(ਸਮੂਹ->ext_ਅੰਕੜੇ.ਅੰਕੜੇ);
        ਸਮੂਹ->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਨਿਰਾ;
    }
#ਜੇ_ਅੰਤ
    lv_free(ਸਮੂਹ);
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_default(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    default_ਸਮੂਹ = ਸਮੂਹ;
}

lv_ਸਮੂਹ_t * lv_ਸਮੂਹ_get_default(ਖਾਲੀ)
{
    ਵਾਪਸ default_ਸਮੂਹ;
}

ਖਾਲੀ lv_ਸਮੂਹ_ਜੋੜੋ_obj(lv_ਸਮੂਹ_t * ਸਮੂਹ, lv_obj_t * obj)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    LV_LOG_TRACE("begin");

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }

    LV_ASSERT(obj->spec_attr != ਨਿਰਾ);

    /*Be sure the object is removed from its current group*/
    lv_ਸਮੂਹ_remove_obj(obj);
    obj->spec_attr->ਸਮੂਹ_p = ਸਮੂਹ;

    lv_obj_t ** ਅਗਲਾ = lv_ll_ins_ਪੂਛ(&ਸਮੂਹ->obj_ll);
    LV_ASSERT_MALLOC(ਅਗਲਾ);
    ਜੇ(ਅਗਲਾ == ਨਿਰਾ) ਵਾਪਸ;
    *ਅਗਲਾ = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ਜੇ(lv_ll_get_ਸਿਰ(&ਸਮੂਹ->obj_ll) == ਅਗਲਾ) {
        lv_ਸਮੂਹ_refocus(ਸਮੂਹ);
    }

    LV_LOG_TRACE("finished");
}

ਖਾਲੀ lv_ਸਮੂਹ_ਅਦਲਾ_ਬਦਲੀ_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(obj2 != ਨਿਰਾ, ਵਾਪਸ);

    lv_ਸਮੂਹ_t * g1 = lv_obj_get_ਸਮੂਹ(obj1);
    lv_ਸਮੂਹ_t * g2 = lv_obj_get_ਸਮੂਹ(obj2);

    ਜੇ(g1 != g2) ਵਾਪਸ;
    ਜੇ(g1 == ਨਿਰਾ) ਵਾਪਸ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ਜੇ((*obj_i) == obj1)(*obj_i) = obj2;
        ਨਹੀਂਤਾਂ ਜੇ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_ਸਮੂਹ_get_focused(g1);
    ਜੇ(focused == obj1) lv_ਸਮੂਹ_focus_obj(obj2);
    ਨਹੀਂਤਾਂ ਜੇ(focused == obj2) lv_ਸਮੂਹ_focus_obj(obj1);
}

ਖਾਲੀ lv_ਸਮੂਹ_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_ਸਮੂਹ_t * g = lv_obj_get_ਸਮੂਹ(obj);
    ਜੇ(g == ਨਿਰਾ) ਵਾਪਸ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ਜੇ(g->obj_focus && *g->obj_focus == obj) {
        ਜੇ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ਜੇ(lv_ll_get_ਸਿਰ(&g->obj_ll) != g->obj_focus || lv_ll_get_ਪੂਛ(&g->obj_ll) != g->obj_focus) {
            lv_ਸਮੂਹ_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ਜੇ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ਨਿਰਾ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ਜੇ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ਜੇ(obj->spec_attr) obj->spec_attr->ਸਮੂਹ_p = ਨਿਰਾ;
            ਰੋਕੋ;
        }
    }
    LV_LOG_TRACE("finished");
}

ਖਾਲੀ lv_ਸਮੂਹ_remove_all_objs(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    /*Defocus the currently focused object*/
    ਜੇ(ਸਮੂਹ->obj_focus != ਨਿਰਾ) {
        lv_obj_send_event(*ਸਮੂਹ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ਸਮੂਹ));
        lv_obj_invalidate(*ਸਮੂਹ->obj_focus);
        ਸਮੂਹ->obj_focus = ਨਿਰਾ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ਸਮੂਹ->obj_ll, obj) {
        ਜੇ((*obj)->spec_attr)(*obj)->spec_attr->ਸਮੂਹ_p = ਨਿਰਾ;
    }

    lv_ll_clear(&(ਸਮੂਹ->obj_ll));
}

ਖਾਲੀ lv_ਸਮੂਹ_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_ਸਮੂਹ_t * g = lv_obj_get_ਸਮੂਹ(obj);

    ਜੇ(g == ਨਿਰਾ) ਵਾਪਸ;
    ਜੇ(g->frozen) ਵਾਪਸ;

    /*On defocus edit mode must be leaved*/
    lv_ਸਮੂਹ_ਸੈੱਟ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ਜੇ(*i == obj) {
            ਜੇ(g->obj_focus != ਨਿਰਾ) {
                ਜੇ(*g->obj_focus == obj) ਰੋਕੋ;  /*Refocusing same object*/
                lv_ਨਤੀਜਾ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ਜੇ(g->focus_cb) g->focus_cb(g);
            lv_ਨਤੀਜਾ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
            lv_obj_invalidate(*g->obj_focus);
            ਰੋਕੋ;
        }
    }
}

ਖਾਲੀ lv_ਸਮੂਹ_focus_ਅਗਲਾ(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    bool focus_ਬਦਲਿਆ = focus_ਅਗਲਾ_core(ਸਮੂਹ, lv_ll_get_ਸਿਰ, lv_ll_get_ਅਗਲਾ);

    ਜੇ(!focus_ਬਦਲਿਆ && ਸਮੂਹ->ਕਿਨਾਰਾ_cb) {
        ਸਮੂਹ->ਕਿਨਾਰਾ_cb(ਸਮੂਹ, true);
    }
}

ਖਾਲੀ lv_ਸਮੂਹ_focus_prev(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    bool focus_ਬਦਲਿਆ = focus_ਅਗਲਾ_core(ਸਮੂਹ, lv_ll_get_ਪੂਛ, lv_ll_get_prev);

    ਜੇ(!focus_ਬਦਲਿਆ && ਸਮੂਹ->ਕਿਨਾਰਾ_cb) {
        ਸਮੂਹ->ਕਿਨਾਰਾ_cb(ਸਮੂਹ, false);
    }
}

ਖਾਲੀ lv_ਸਮੂਹ_focus_freeze(lv_ਸਮੂਹ_t * ਸਮੂਹ, bool en)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    ਜੇ(en == false) ਸਮੂਹ->frozen = 0;
    ਨਹੀਂਤਾਂ ਸਮੂਹ->frozen = 1;
}

lv_ਨਤੀਜਾ_t lv_ਸਮੂਹ_send_ਅੰਕੜੇ(lv_ਸਮੂਹ_t * ਸਮੂਹ, uint32_t c)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    lv_obj_t * act = lv_ਸਮੂਹ_get_focused(ਸਮੂਹ);
    ਜੇ(act == ਨਿਰਾ) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;

    ਜੇ(lv_obj_has_state(act, LV_STATE_DISABLED)) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;

    ਵਾਪਸ lv_obj_send_event(act, LV_EVENT_ਕੁੰਜੀ, &c);
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_focus_cb(lv_ਸਮੂਹ_t * ਸਮੂਹ, lv_ਸਮੂਹ_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    ਸਮੂਹ->focus_cb = focus_cb;
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_ਕਿਨਾਰਾ_cb(lv_ਸਮੂਹ_t * ਸਮੂਹ, lv_ਸਮੂਹ_ਕਿਨਾਰਾ_cb_t ਕਿਨਾਰਾ_cb)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    ਸਮੂਹ->ਕਿਨਾਰਾ_cb = ਕਿਨਾਰਾ_cb;
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_editing(lv_ਸਮੂਹ_t * ਸਮੂਹ, bool edit)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);
    uint8_t en_val = edit ? 1 : 0;

    ਜੇ(en_val == ਸਮੂਹ->editing) ਵਾਪਸ; /*Do not set the same mode again*/

    ਸਮੂਹ->editing     = en_val;
    lv_obj_t * focused = lv_ਸਮੂਹ_get_focused(ਸਮੂਹ);

    ਜੇ(focused) {
        lv_ਨਤੀਜਾ_t res = lv_obj_send_event(*ਸਮੂਹ->obj_focus, LV_EVENT_FOCUSED, get_indev(ਸਮੂਹ));
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

        lv_obj_invalidate(focused);
    }
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_refocus_policy(lv_ਸਮੂਹ_t * ਸਮੂਹ, lv_ਸਮੂਹ_refocus_policy_t policy)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);
    ਸਮੂਹ->refocus_policy = policy & 0x01;
}

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_wrap(lv_ਸਮੂਹ_t * ਸਮੂਹ, bool en)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);
    ਸਮੂਹ->wrap = en ? 1 : 0;
}

lv_obj_t * lv_ਸਮੂਹ_get_focused(ਅਚਲ lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਜੇ(ਸਮੂਹ->obj_focus == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ *ਸਮੂਹ->obj_focus;
}

lv_ਸਮੂਹ_focus_cb_t lv_ਸਮੂਹ_get_focus_cb(ਅਚਲ lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ ਸਮੂਹ->focus_cb;
}

lv_ਸਮੂਹ_ਕਿਨਾਰਾ_cb_t lv_ਸਮੂਹ_get_ਕਿਨਾਰਾ_cb(ਅਚਲ lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ ਸਮੂਹ->ਕਿਨਾਰਾ_cb;
}

bool lv_ਸਮੂਹ_get_editing(ਅਚਲ lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ false);
    ਵਾਪਸ ਸਮੂਹ->editing;
}

bool lv_ਸਮੂਹ_get_wrap(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ false);
    ਵਾਪਸ ਸਮੂਹ->wrap;
}

uint32_t lv_ਸਮੂਹ_get_obj_ਗਿਣਤੀ(lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ 0);
    ਵਾਪਸ lv_ll_get_len(&ਸਮੂਹ->obj_ll);
}

lv_obj_t * lv_ਸਮੂਹ_get_obj_by_ਸੂਚਕਾਂਕ(lv_ਸਮੂਹ_t * ਸਮੂਹ, uint32_t ਸੂਚਕਾਂਕ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&ਸਮੂਹ->obj_ll, obj) {
        ਜੇ(len == ਸੂਚਕਾਂਕ) {
            ਵਾਪਸ *obj;
        }
        len++;
    }
    ਵਾਪਸ ਨਿਰਾ;
}

uint32_t lv_ਸਮੂਹ_get_ਗਿਣਤੀ(ਖਾਲੀ)
{
    ਵਾਪਸ lv_ll_get_len(ਸਮੂਹ_ll_p);
}

lv_ਸਮੂਹ_t  * lv_ਸਮੂਹ_by_ਸੂਚਕਾਂਕ(uint32_t ਸੂਚਕਾਂਕ)
{
    uint32_t len = 0;
    lv_ਸਮੂਹ_t * ਸਮੂਹ;

    LV_LL_READ_ਪਿੱਠ(ਸਮੂਹ_ll_p, ਸਮੂਹ) {
        ਜੇ(len == ਸੂਚਕਾਂਕ) {
            ਵਾਪਸ ਸਮੂਹ;
        }
        len++;
    }

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_external_ਅੰਕੜੇ(lv_ਸਮੂਹ_t * ਸਮੂਹ, ਖਾਲੀ * ਅੰਕੜੇ, ਖਾਲੀ (* free_cb)(ਖਾਲੀ * ਅੰਕੜੇ))
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);

    ਸਮੂਹ->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਅੰਕੜੇ;
    ਸਮੂਹ->ext_ਅੰਕੜੇ.free_cb = free_cb;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ lv_ਸਮੂਹ_ਸੈੱਟ_user_ਅੰਕੜੇ(lv_ਸਮੂਹ_t * ਸਮੂਹ, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ);
    ਸਮੂਹ->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
}

ਖਾਲੀ * lv_ਸਮੂਹ_get_user_ਅੰਕੜੇ(ਅਚਲ lv_ਸਮੂਹ_t * ਸਮੂਹ)
{
    LV_CHECK_ARG(ਸਮੂਹ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ ਸਮੂਹ->user_ਅੰਕੜੇ;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਖਾਲੀ lv_ਸਮੂਹ_refocus(lv_ਸਮੂਹ_t * g)
{
    LV_ASSERT(g != ਨਿਰਾ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t ਆਰਜ਼ੀ_wrap = g->wrap;
    g->wrap           = 1;

    ਜੇ(g->refocus_policy == LV_ਸਮੂਹ_REFOCUS_POLICY_ਅਗਲਾ)
        lv_ਸਮੂਹ_focus_ਅਗਲਾ(g);
    ਨਹੀਂਤਾਂ ਜੇ(g->refocus_policy == LV_ਸਮੂਹ_REFOCUS_POLICY_PREV)
        lv_ਸਮੂਹ_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = ਆਰਜ਼ੀ_wrap;
}

ਸਥਿਰ bool focus_ਅਗਲਾ_core(lv_ਸਮੂਹ_t * ਸਮੂਹ, ਖਾਲੀ * (*begin)(ਅਚਲ lv_ll_t *),
                            ਖਾਲੀ * (*move)(ਅਚਲ lv_ll_t *, ਅਚਲ ਖਾਲੀ *))
{
    LV_ASSERT(ਸਮੂਹ != ਨਿਰਾ);
    LV_ASSERT(begin != ਨਿਰਾ);
    LV_ASSERT(move != ਨਿਰਾ);
    ਜੇ(ਸਮੂਹ->frozen) ਵਾਪਸ false;

    bool focus_ਬਦਲਿਆ = false;

    lv_obj_t ** obj_ਅਗਲਾ     = ਸਮੂਹ->obj_focus;
    lv_obj_t ** obj_sentinel = ਨਿਰਾ;
    bool can_move            = true;
    bool can_begin           = true;

    ਲਈ(;;) {
        ਜੇ(obj_ਅਗਲਾ == ਨਿਰਾ) {
            ਜੇ(ਸਮੂਹ->wrap || obj_sentinel == ਨਿਰਾ) {
                ਜੇ(!can_begin) ਵਾਪਸ focus_ਬਦਲਿਆ;
                obj_ਅਗਲਾ  = begin(&ਸਮੂਹ->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ਨਹੀਂਤਾਂ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ਵਾਪਸ focus_ਬਦਲਿਆ;
            }
        }

        ਜੇ(obj_sentinel == ਨਿਰਾ) {
            obj_sentinel = obj_ਅਗਲਾ;
            ਜੇ(obj_sentinel == ਨਿਰਾ) ਵਾਪਸ focus_ਬਦਲਿਆ; /*Group is empty*/
        }

        ਜੇ(can_move) {
            obj_ਅਗਲਾ = move(&ਸਮੂਹ->obj_ll, obj_ਅਗਲਾ);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ਜੇ(obj_ਅਗਲਾ == obj_sentinel) ਵਾਪਸ focus_ਬਦਲਿਆ;
        }

        can_move = true;

        ਜੇ(obj_ਅਗਲਾ == ਨਿਰਾ) ਜਾਰੀ;
        ਜੇ(lv_obj_get_state(*obj_ਅਗਲਾ) & LV_STATE_DISABLED) ਜਾਰੀ;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * ਮਾਪਾ = *obj_ਅਗਲਾ;
        ਜਦੋਂ(ਮਾਪਾ) {
            ਜੇ(lv_obj_is_hidden(ਮਾਪਾ)) ਰੋਕੋ;
            ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਮਾਪਾ);
        }

        ਜੇ(ਮਾਪਾ && lv_obj_is_hidden(ਮਾਪਾ)) ਜਾਰੀ;

        /*If we got her a good candidate is found*/
        ਰੋਕੋ;
    }

    ਜੇ(obj_ਅਗਲਾ == ਸਮੂਹ->obj_focus) ਵਾਪਸ focus_ਬਦਲਿਆ; /*There's only one visible object and it's already focused*/

    ਜੇ(ਸਮੂਹ->obj_focus) {
        lv_ਨਤੀਜਾ_t res = lv_obj_send_event(*ਸਮੂਹ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ਸਮੂਹ));
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ focus_ਬਦਲਿਆ;
        lv_obj_invalidate(*ਸਮੂਹ->obj_focus);
    }

    ਸਮੂਹ->obj_focus = obj_ਅਗਲਾ;

    lv_ਨਤੀਜਾ_t res = lv_obj_send_event(*ਸਮੂਹ->obj_focus, LV_EVENT_FOCUSED, get_indev(ਸਮੂਹ));
    ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ focus_ਬਦਲਿਆ;

    lv_obj_invalidate(*ਸਮੂਹ->obj_focus);

    ਜੇ(ਸਮੂਹ->focus_cb) ਸਮੂਹ->focus_cb(ਸਮੂਹ);
    focus_ਬਦਲਿਆ = true;
    ਵਾਪਸ focus_ਬਦਲਿਆ;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ਸਥਿਰ lv_indev_t * get_indev(ਅਚਲ lv_ਸਮੂਹ_t * g)
{
    LV_ASSERT(g != ਨਿਰਾ);
    lv_indev_t * indev_guess = ਨਿਰਾ;
    lv_indev_t * indev = lv_indev_get_ਅਗਲਾ(ਨਿਰਾ);

    ਜਦੋਂ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ਜੇ(indev_type == LV_INDEV_TYPE_ਸੂਚਕ) ਵਾਪਸ indev;
        ਜੇ(lv_indev_get_ਸਮੂਹ(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_ਅਗਲਾ(indev);
    }

    ਵਾਪਸ indev_guess;
}
