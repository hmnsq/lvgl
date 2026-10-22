/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_group_private.h"
#ضمن "../core/lv_obj_private.h"
#ضمن "../core/lv_global.h"
#ضمن "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#وصف_ڪريو default_گروهه LV_GLOBAL_DEFAULT()->گروهه_default
#وصف_ڪريو گروهه_ll_p &(LV_GLOBAL_DEFAULT()->گروهه_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن bool focus_ايندڙ_core(lv_گروهه_t * گروهه, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *));
ساكن فراغ lv_گروهه_refocus(lv_گروهه_t * g);
ساكن lv_indev_t * get_indev(ثابت lv_گروهه_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_گروهه_init(فراغ)
{
    lv_ll_init(گروهه_ll_p, حجم(lv_گروهه_t));
}

فراغ lv_گروهه_deinit(فراغ)
{
    lv_ll_clear(گروهه_ll_p);
}

lv_گروهه_t * lv_گروهه_create(فراغ)
{
    lv_گروهه_t * گروهه = lv_ll_ins_سر(گروهه_ll_p);
    LV_ASSERT_MALLOC(گروهه);
    إذا(گروهه == عدم) أرجع عدم;
    lv_ll_init(&گروهه->obj_ll, حجم(lv_obj_t *));

    گروهه->obj_focus      = عدم;
    گروهه->frozen         = 0;
    گروهه->focus_cb       = عدم;
    گروهه->ڪنارو_cb        = عدم;
    گروهه->editing        = 0;
    گروهه->refocus_policy = LV_گروهه_REFOCUS_POLICY_PREV;
    گروهه->wrap           = 1;
    گروهه->user_معلومات      = عدم;
#إذا LV_USE_EXT_معلومات
    گروهه->ext_معلومات.free_cb = عدم;
    گروهه->ext_معلومات.معلومات = عدم;
#جيڪڏهن_پورو

    أرجع گروهه;
}

فراغ lv_گروهه_delete(lv_گروهه_t * گروهه)
{
    إذا(گروهه == عدم) أرجع;

    /*Defocus the currently focused object*/
    إذا(گروهه->obj_focus != عدم) {
        lv_obj_send_event(*گروهه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروهه));
        lv_obj_invalidate(*گروهه->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروهه->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروهه_p = عدم;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_ايندڙ(عدم);
    طالما(indev) {
        إذا(lv_indev_get_گروهه(indev) == گروهه) {
            lv_indev_سيٽ_گروهه(indev, عدم);
        }
        indev = lv_indev_get_ايندڙ(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    إذا(گروهه == lv_گروهه_get_default()) lv_گروهه_سيٽ_default(عدم);

    lv_ll_clear(&(گروهه->obj_ll));
    lv_ll_remove(گروهه_ll_p, گروهه);
#إذا LV_USE_EXT_معلومات
    إذا(گروهه->ext_معلومات.free_cb) {
        گروهه->ext_معلومات.free_cb(گروهه->ext_معلومات.معلومات);
        گروهه->ext_معلومات.معلومات = عدم;
    }
#جيڪڏهن_پورو
    lv_free(گروهه);
}

فراغ lv_گروهه_سيٽ_default(lv_گروهه_t * گروهه)
{
    default_گروهه = گروهه;
}

lv_گروهه_t * lv_گروهه_get_default(فراغ)
{
    أرجع default_گروهه;
}

فراغ lv_گروهه_جوڙيو_obj(lv_گروهه_t * گروهه, lv_obj_t * obj)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_LOG_TRACE("begin");

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    LV_ASSERT(obj->spec_attr != عدم);

    /*Be sure the object is removed from its current group*/
    lv_گروهه_remove_obj(obj);
    obj->spec_attr->گروهه_p = گروهه;

    lv_obj_t ** ايندڙ = lv_ll_ins_پڇ(&گروهه->obj_ll);
    LV_ASSERT_MALLOC(ايندڙ);
    إذا(ايندڙ == عدم) أرجع;
    *ايندڙ = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    إذا(lv_ll_get_سر(&گروهه->obj_ll) == ايندڙ) {
        lv_گروهه_refocus(گروهه);
    }

    LV_LOG_TRACE("finished");
}

فراغ lv_گروهه_مٽا_سٽا_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != عدم, أرجع);
    LV_CHECK_ARG(obj2 != عدم, أرجع);

    lv_گروهه_t * g1 = lv_obj_get_گروهه(obj1);
    lv_گروهه_t * g2 = lv_obj_get_گروهه(obj2);

    إذا(g1 != g2) أرجع;
    إذا(g1 == عدم) أرجع;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        إذا((*obj_i) == obj1)(*obj_i) = obj2;
        وإلا إذا((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_گروهه_get_focused(g1);
    إذا(focused == obj1) lv_گروهه_focus_obj(obj2);
    وإلا إذا(focused == obj2) lv_گروهه_focus_obj(obj1);
}

فراغ lv_گروهه_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروهه_t * g = lv_obj_get_گروهه(obj);
    إذا(g == عدم) أرجع;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    إذا(g->obj_focus && *g->obj_focus == obj) {
        إذا(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        إذا(lv_ll_get_سر(&g->obj_ll) != g->obj_focus || lv_ll_get_پڇ(&g->obj_ll) != g->obj_focus) {
            lv_گروهه_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    إذا(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = عدم;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        إذا(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            إذا(obj->spec_attr) obj->spec_attr->گروهه_p = عدم;
            اكسر;
        }
    }
    LV_LOG_TRACE("finished");
}

فراغ lv_گروهه_remove_all_objs(lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    /*Defocus the currently focused object*/
    إذا(گروهه->obj_focus != عدم) {
        lv_obj_send_event(*گروهه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروهه));
        lv_obj_invalidate(*گروهه->obj_focus);
        گروهه->obj_focus = عدم;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروهه->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروهه_p = عدم;
    }

    lv_ll_clear(&(گروهه->obj_ll));
}

فراغ lv_گروهه_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروهه_t * g = lv_obj_get_گروهه(obj);

    إذا(g == عدم) أرجع;
    إذا(g->frozen) أرجع;

    /*On defocus edit mode must be leaved*/
    lv_گروهه_سيٽ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        إذا(*i == obj) {
            إذا(g->obj_focus != عدم) {
                إذا(*g->obj_focus == obj) اكسر;  /*Refocusing same object*/
                lv_نتيجو_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                إذا(res != LV_نتيجو_OK) أرجع;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            إذا(g->focus_cb) g->focus_cb(g);
            lv_نتيجو_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            إذا(res != LV_نتيجو_OK) أرجع;
            lv_obj_invalidate(*g->obj_focus);
            اكسر;
        }
    }
}

فراغ lv_گروهه_focus_ايندڙ(lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    bool focus_بدليو = focus_ايندڙ_core(گروهه, lv_ll_get_سر, lv_ll_get_ايندڙ);

    إذا(!focus_بدليو && گروهه->ڪنارو_cb) {
        گروهه->ڪنارو_cb(گروهه, true);
    }
}

فراغ lv_گروهه_focus_prev(lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    bool focus_بدليو = focus_ايندڙ_core(گروهه, lv_ll_get_پڇ, lv_ll_get_prev);

    إذا(!focus_بدليو && گروهه->ڪنارو_cb) {
        گروهه->ڪنارو_cb(گروهه, false);
    }
}

فراغ lv_گروهه_focus_freeze(lv_گروهه_t * گروهه, bool en)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    إذا(en == false) گروهه->frozen = 0;
    وإلا گروهه->frozen = 1;
}

lv_نتيجو_t lv_گروهه_send_معلومات(lv_گروهه_t * گروهه, uint32_t c)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع LV_نتيجو_INVALID);

    lv_obj_t * act = lv_گروهه_get_focused(گروهه);
    إذا(act == عدم) أرجع LV_نتيجو_OK;

    إذا(lv_obj_has_state(act, LV_STATE_DISABLED)) أرجع LV_نتيجو_OK;

    أرجع lv_obj_send_event(act, LV_EVENT_ڪنجي, &c);
}

فراغ lv_گروهه_سيٽ_focus_cb(lv_گروهه_t * گروهه, lv_گروهه_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    گروهه->focus_cb = focus_cb;
}

فراغ lv_گروهه_سيٽ_ڪنارو_cb(lv_گروهه_t * گروهه, lv_گروهه_ڪنارو_cb_t ڪنارو_cb)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    گروهه->ڪنارو_cb = ڪنارو_cb;
}

فراغ lv_گروهه_سيٽ_editing(lv_گروهه_t * گروهه, bool edit)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);
    uint8_t en_val = edit ? 1 : 0;

    إذا(en_val == گروهه->editing) أرجع; /*Do not set the same mode again*/

    گروهه->editing     = en_val;
    lv_obj_t * focused = lv_گروهه_get_focused(گروهه);

    إذا(focused) {
        lv_نتيجو_t res = lv_obj_send_event(*گروهه->obj_focus, LV_EVENT_FOCUSED, get_indev(گروهه));
        إذا(res != LV_نتيجو_OK) أرجع;

        lv_obj_invalidate(focused);
    }
}

فراغ lv_گروهه_سيٽ_refocus_policy(lv_گروهه_t * گروهه, lv_گروهه_refocus_policy_t policy)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);
    گروهه->refocus_policy = policy & 0x01;
}

فراغ lv_گروهه_سيٽ_wrap(lv_گروهه_t * گروهه, bool en)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);
    گروهه->wrap = en ? 1 : 0;
}

lv_obj_t * lv_گروهه_get_focused(ثابت lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع عدم);
    إذا(گروهه->obj_focus == عدم) أرجع عدم;

    أرجع *گروهه->obj_focus;
}

lv_گروهه_focus_cb_t lv_گروهه_get_focus_cb(ثابت lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع عدم);
    أرجع گروهه->focus_cb;
}

lv_گروهه_ڪنارو_cb_t lv_گروهه_get_ڪنارو_cb(ثابت lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع عدم);
    أرجع گروهه->ڪنارو_cb;
}

bool lv_گروهه_get_editing(ثابت lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع false);
    أرجع گروهه->editing;
}

bool lv_گروهه_get_wrap(lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع false);
    أرجع گروهه->wrap;
}

uint32_t lv_گروهه_get_obj_ڳڻپ(lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع 0);
    أرجع lv_ll_get_len(&گروهه->obj_ll);
}

lv_obj_t * lv_گروهه_get_obj_by_اشاريو(lv_گروهه_t * گروهه, uint32_t اشاريو)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع عدم);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&گروهه->obj_ll, obj) {
        إذا(len == اشاريو) {
            أرجع *obj;
        }
        len++;
    }
    أرجع عدم;
}

uint32_t lv_گروهه_get_ڳڻپ(فراغ)
{
    أرجع lv_ll_get_len(گروهه_ll_p);
}

lv_گروهه_t  * lv_گروهه_by_اشاريو(uint32_t اشاريو)
{
    uint32_t len = 0;
    lv_گروهه_t * گروهه;

    LV_LL_READ_پٺ(گروهه_ll_p, گروهه) {
        إذا(len == اشاريو) {
            أرجع گروهه;
        }
        len++;
    }

    أرجع عدم;
}

#إذا LV_USE_EXT_معلومات
فراغ lv_گروهه_سيٽ_external_معلومات(lv_گروهه_t * گروهه, فراغ * معلومات, فراغ (* free_cb)(فراغ * معلومات))
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);

    گروهه->ext_معلومات.معلومات = معلومات;
    گروهه->ext_معلومات.free_cb = free_cb;
}
#جيڪڏهن_پورو

فراغ lv_گروهه_سيٽ_user_معلومات(lv_گروهه_t * گروهه, فراغ * user_معلومات)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع);
    گروهه->user_معلومات = user_معلومات;
}

فراغ * lv_گروهه_get_user_معلومات(ثابت lv_گروهه_t * گروهه)
{
    LV_CHECK_ARG(گروهه != عدم, أرجع عدم);
    أرجع گروهه->user_معلومات;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_گروهه_refocus(lv_گروهه_t * g)
{
    LV_ASSERT(g != عدم);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t عارضي_wrap = g->wrap;
    g->wrap           = 1;

    إذا(g->refocus_policy == LV_گروهه_REFOCUS_POLICY_ايندڙ)
        lv_گروهه_focus_ايندڙ(g);
    وإلا إذا(g->refocus_policy == LV_گروهه_REFOCUS_POLICY_PREV)
        lv_گروهه_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = عارضي_wrap;
}

ساكن bool focus_ايندڙ_core(lv_گروهه_t * گروهه, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *))
{
    LV_ASSERT(گروهه != عدم);
    LV_ASSERT(begin != عدم);
    LV_ASSERT(move != عدم);
    إذا(گروهه->frozen) أرجع false;

    bool focus_بدليو = false;

    lv_obj_t ** obj_ايندڙ     = گروهه->obj_focus;
    lv_obj_t ** obj_sentinel = عدم;
    bool can_move            = true;
    bool can_begin           = true;

    لكل(;;) {
        إذا(obj_ايندڙ == عدم) {
            إذا(گروهه->wrap || obj_sentinel == عدم) {
                إذا(!can_begin) أرجع focus_بدليو;
                obj_ايندڙ  = begin(&گروهه->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وإلا {
                /*Currently focused object is the last/first in the group, keep it that way*/
                أرجع focus_بدليو;
            }
        }

        إذا(obj_sentinel == عدم) {
            obj_sentinel = obj_ايندڙ;
            إذا(obj_sentinel == عدم) أرجع focus_بدليو; /*Group is empty*/
        }

        إذا(can_move) {
            obj_ايندڙ = move(&گروهه->obj_ll, obj_ايندڙ);

            /*Give up if we walked the entire list and haven't found another visible object*/
            إذا(obj_ايندڙ == obj_sentinel) أرجع focus_بدليو;
        }

        can_move = true;

        إذا(obj_ايندڙ == عدم) استمر;
        إذا(lv_obj_get_state(*obj_ايندڙ) & LV_STATE_DISABLED) استمر;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * والد = *obj_ايندڙ;
        طالما(والد) {
            إذا(lv_obj_is_hidden(والد)) اكسر;
            والد = lv_obj_get_والد(والد);
        }

        إذا(والد && lv_obj_is_hidden(والد)) استمر;

        /*If we got her a good candidate is found*/
        اكسر;
    }

    إذا(obj_ايندڙ == گروهه->obj_focus) أرجع focus_بدليو; /*There's only one visible object and it's already focused*/

    إذا(گروهه->obj_focus) {
        lv_نتيجو_t res = lv_obj_send_event(*گروهه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروهه));
        إذا(res != LV_نتيجو_OK) أرجع focus_بدليو;
        lv_obj_invalidate(*گروهه->obj_focus);
    }

    گروهه->obj_focus = obj_ايندڙ;

    lv_نتيجو_t res = lv_obj_send_event(*گروهه->obj_focus, LV_EVENT_FOCUSED, get_indev(گروهه));
    إذا(res != LV_نتيجو_OK) أرجع focus_بدليو;

    lv_obj_invalidate(*گروهه->obj_focus);

    إذا(گروهه->focus_cb) گروهه->focus_cb(گروهه);
    focus_بدليو = true;
    أرجع focus_بدليو;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساكن lv_indev_t * get_indev(ثابت lv_گروهه_t * g)
{
    LV_ASSERT(g != عدم);
    lv_indev_t * indev_guess = عدم;
    lv_indev_t * indev = lv_indev_get_ايندڙ(عدم);

    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        إذا(indev_type == LV_INDEV_TYPE_اشارو) أرجع indev;
        إذا(lv_indev_get_گروهه(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_ايندڙ(indev);
    }

    أرجع indev_guess;
}
