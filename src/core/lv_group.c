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
#تعریف_کرو default_گروہ LV_GLOBAL_DEFAULT()->گروہ_default
#تعریف_کرو گروہ_ll_p &(LV_GLOBAL_DEFAULT()->گروہ_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن bool focus_اگلا_core(lv_گروہ_t * گروہ, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *));
ساكن فراغ lv_گروہ_refocus(lv_گروہ_t * g);
ساكن lv_indev_t * get_indev(ثابت lv_گروہ_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_گروہ_init(فراغ)
{
    lv_ll_init(گروہ_ll_p, حجم(lv_گروہ_t));
}

فراغ lv_گروہ_deinit(فراغ)
{
    lv_ll_clear(گروہ_ll_p);
}

lv_گروہ_t * lv_گروہ_create(فراغ)
{
    lv_گروہ_t * گروہ = lv_ll_ins_سر(گروہ_ll_p);
    LV_ASSERT_MALLOC(گروہ);
    إذا(گروہ == عدم) أرجع عدم;
    lv_ll_init(&گروہ->obj_ll, حجم(lv_obj_t *));

    گروہ->obj_focus      = عدم;
    گروہ->frozen         = 0;
    گروہ->focus_cb       = عدم;
    گروہ->کنارا_cb        = عدم;
    گروہ->editing        = 0;
    گروہ->refocus_policy = LV_گروہ_REFOCUS_POLICY_PREV;
    گروہ->wrap           = 1;
    گروہ->user_انکڑے      = عدم;
#إذا LV_USE_EXT_انکڑے
    گروہ->ext_انکڑے.free_cb = عدم;
    گروہ->ext_انکڑے.انکڑے = عدم;
#جے_انت

    أرجع گروہ;
}

فراغ lv_گروہ_delete(lv_گروہ_t * گروہ)
{
    إذا(گروہ == عدم) أرجع;

    /*Defocus the currently focused object*/
    إذا(گروہ->obj_focus != عدم) {
        lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        lv_obj_invalidate(*گروہ->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروہ->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروہ_p = عدم;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_اگلا(عدم);
    طالما(indev) {
        إذا(lv_indev_get_گروہ(indev) == گروہ) {
            lv_indev_سیٹ_گروہ(indev, عدم);
        }
        indev = lv_indev_get_اگلا(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    إذا(گروہ == lv_گروہ_get_default()) lv_گروہ_سیٹ_default(عدم);

    lv_ll_clear(&(گروہ->obj_ll));
    lv_ll_remove(گروہ_ll_p, گروہ);
#إذا LV_USE_EXT_انکڑے
    إذا(گروہ->ext_انکڑے.free_cb) {
        گروہ->ext_انکڑے.free_cb(گروہ->ext_انکڑے.انکڑے);
        گروہ->ext_انکڑے.انکڑے = عدم;
    }
#جے_انت
    lv_free(گروہ);
}

فراغ lv_گروہ_سیٹ_default(lv_گروہ_t * گروہ)
{
    default_گروہ = گروہ;
}

lv_گروہ_t * lv_گروہ_get_default(فراغ)
{
    أرجع default_گروہ;
}

فراغ lv_گروہ_جوڑو_obj(lv_گروہ_t * گروہ, lv_obj_t * obj)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_LOG_TRACE("begin");

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    LV_ASSERT(obj->spec_attr != عدم);

    /*Be sure the object is removed from its current group*/
    lv_گروہ_remove_obj(obj);
    obj->spec_attr->گروہ_p = گروہ;

    lv_obj_t ** اگلا = lv_ll_ins_پوچھ(&گروہ->obj_ll);
    LV_ASSERT_MALLOC(اگلا);
    إذا(اگلا == عدم) أرجع;
    *اگلا = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    إذا(lv_ll_get_سر(&گروہ->obj_ll) == اگلا) {
        lv_گروہ_refocus(گروہ);
    }

    LV_LOG_TRACE("finished");
}

فراغ lv_گروہ_وٹاندرا_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != عدم, أرجع);
    LV_CHECK_ARG(obj2 != عدم, أرجع);

    lv_گروہ_t * g1 = lv_obj_get_گروہ(obj1);
    lv_گروہ_t * g2 = lv_obj_get_گروہ(obj2);

    إذا(g1 != g2) أرجع;
    إذا(g1 == عدم) أرجع;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        إذا((*obj_i) == obj1)(*obj_i) = obj2;
        وإلا إذا((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_گروہ_get_focused(g1);
    إذا(focused == obj1) lv_گروہ_focus_obj(obj2);
    وإلا إذا(focused == obj2) lv_گروہ_focus_obj(obj1);
}

فراغ lv_گروہ_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروہ_t * g = lv_obj_get_گروہ(obj);
    إذا(g == عدم) أرجع;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    إذا(g->obj_focus && *g->obj_focus == obj) {
        إذا(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        إذا(lv_ll_get_سر(&g->obj_ll) != g->obj_focus || lv_ll_get_پوچھ(&g->obj_ll) != g->obj_focus) {
            lv_گروہ_refocus(g);
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
            إذا(obj->spec_attr) obj->spec_attr->گروہ_p = عدم;
            اكسر;
        }
    }
    LV_LOG_TRACE("finished");
}

فراغ lv_گروہ_remove_all_objs(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    /*Defocus the currently focused object*/
    إذا(گروہ->obj_focus != عدم) {
        lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        lv_obj_invalidate(*گروہ->obj_focus);
        گروہ->obj_focus = عدم;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروہ->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروہ_p = عدم;
    }

    lv_ll_clear(&(گروہ->obj_ll));
}

فراغ lv_گروہ_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروہ_t * g = lv_obj_get_گروہ(obj);

    إذا(g == عدم) أرجع;
    إذا(g->frozen) أرجع;

    /*On defocus edit mode must be leaved*/
    lv_گروہ_سیٹ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        إذا(*i == obj) {
            إذا(g->obj_focus != عدم) {
                إذا(*g->obj_focus == obj) اكسر;  /*Refocusing same object*/
                lv_نتیجہ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                إذا(res != LV_نتیجہ_OK) أرجع;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            إذا(g->focus_cb) g->focus_cb(g);
            lv_نتیجہ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            إذا(res != LV_نتیجہ_OK) أرجع;
            lv_obj_invalidate(*g->obj_focus);
            اكسر;
        }
    }
}

فراغ lv_گروہ_focus_اگلا(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    bool focus_بدلیا = focus_اگلا_core(گروہ, lv_ll_get_سر, lv_ll_get_اگلا);

    إذا(!focus_بدلیا && گروہ->کنارا_cb) {
        گروہ->کنارا_cb(گروہ, true);
    }
}

فراغ lv_گروہ_focus_prev(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    bool focus_بدلیا = focus_اگلا_core(گروہ, lv_ll_get_پوچھ, lv_ll_get_prev);

    إذا(!focus_بدلیا && گروہ->کنارا_cb) {
        گروہ->کنارا_cb(گروہ, false);
    }
}

فراغ lv_گروہ_focus_freeze(lv_گروہ_t * گروہ, bool en)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    إذا(en == false) گروہ->frozen = 0;
    وإلا گروہ->frozen = 1;
}

lv_نتیجہ_t lv_گروہ_send_انکڑے(lv_گروہ_t * گروہ, uint32_t c)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع LV_نتیجہ_INVALID);

    lv_obj_t * act = lv_گروہ_get_focused(گروہ);
    إذا(act == عدم) أرجع LV_نتیجہ_OK;

    إذا(lv_obj_has_state(act, LV_STATE_DISABLED)) أرجع LV_نتیجہ_OK;

    أرجع lv_obj_send_event(act, LV_EVENT_کلید, &c);
}

فراغ lv_گروہ_سیٹ_focus_cb(lv_گروہ_t * گروہ, lv_گروہ_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    گروہ->focus_cb = focus_cb;
}

فراغ lv_گروہ_سیٹ_کنارا_cb(lv_گروہ_t * گروہ, lv_گروہ_کنارا_cb_t کنارا_cb)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    گروہ->کنارا_cb = کنارا_cb;
}

فراغ lv_گروہ_سیٹ_editing(lv_گروہ_t * گروہ, bool edit)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);
    uint8_t en_val = edit ? 1 : 0;

    إذا(en_val == گروہ->editing) أرجع; /*Do not set the same mode again*/

    گروہ->editing     = en_val;
    lv_obj_t * focused = lv_گروہ_get_focused(گروہ);

    إذا(focused) {
        lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_FOCUSED, get_indev(گروہ));
        إذا(res != LV_نتیجہ_OK) أرجع;

        lv_obj_invalidate(focused);
    }
}

فراغ lv_گروہ_سیٹ_refocus_policy(lv_گروہ_t * گروہ, lv_گروہ_refocus_policy_t policy)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);
    گروہ->refocus_policy = policy & 0x01;
}

فراغ lv_گروہ_سیٹ_wrap(lv_گروہ_t * گروہ, bool en)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);
    گروہ->wrap = en ? 1 : 0;
}

lv_obj_t * lv_گروہ_get_focused(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع عدم);
    إذا(گروہ->obj_focus == عدم) أرجع عدم;

    أرجع *گروہ->obj_focus;
}

lv_گروہ_focus_cb_t lv_گروہ_get_focus_cb(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع عدم);
    أرجع گروہ->focus_cb;
}

lv_گروہ_کنارا_cb_t lv_گروہ_get_کنارا_cb(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع عدم);
    أرجع گروہ->کنارا_cb;
}

bool lv_گروہ_get_editing(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع false);
    أرجع گروہ->editing;
}

bool lv_گروہ_get_wrap(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع false);
    أرجع گروہ->wrap;
}

uint32_t lv_گروہ_get_obj_گنتی(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع 0);
    أرجع lv_ll_get_len(&گروہ->obj_ll);
}

lv_obj_t * lv_گروہ_get_obj_by_اشاریہ(lv_گروہ_t * گروہ, uint32_t اشاریہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع عدم);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&گروہ->obj_ll, obj) {
        إذا(len == اشاریہ) {
            أرجع *obj;
        }
        len++;
    }
    أرجع عدم;
}

uint32_t lv_گروہ_get_گنتی(فراغ)
{
    أرجع lv_ll_get_len(گروہ_ll_p);
}

lv_گروہ_t  * lv_گروہ_by_اشاریہ(uint32_t اشاریہ)
{
    uint32_t len = 0;
    lv_گروہ_t * گروہ;

    LV_LL_READ_پٹھ(گروہ_ll_p, گروہ) {
        إذا(len == اشاریہ) {
            أرجع گروہ;
        }
        len++;
    }

    أرجع عدم;
}

#إذا LV_USE_EXT_انکڑے
فراغ lv_گروہ_سیٹ_external_انکڑے(lv_گروہ_t * گروہ, فراغ * انکڑے, فراغ (* free_cb)(فراغ * انکڑے))
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);

    گروہ->ext_انکڑے.انکڑے = انکڑے;
    گروہ->ext_انکڑے.free_cb = free_cb;
}
#جے_انت

فراغ lv_گروہ_سیٹ_user_انکڑے(lv_گروہ_t * گروہ, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع);
    گروہ->user_انکڑے = user_انکڑے;
}

فراغ * lv_گروہ_get_user_انکڑے(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != عدم, أرجع عدم);
    أرجع گروہ->user_انکڑے;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_گروہ_refocus(lv_گروہ_t * g)
{
    LV_ASSERT(g != عدم);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t عارضی_wrap = g->wrap;
    g->wrap           = 1;

    إذا(g->refocus_policy == LV_گروہ_REFOCUS_POLICY_اگلا)
        lv_گروہ_focus_اگلا(g);
    وإلا إذا(g->refocus_policy == LV_گروہ_REFOCUS_POLICY_PREV)
        lv_گروہ_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = عارضی_wrap;
}

ساكن bool focus_اگلا_core(lv_گروہ_t * گروہ, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *))
{
    LV_ASSERT(گروہ != عدم);
    LV_ASSERT(begin != عدم);
    LV_ASSERT(move != عدم);
    إذا(گروہ->frozen) أرجع false;

    bool focus_بدلیا = false;

    lv_obj_t ** obj_اگلا     = گروہ->obj_focus;
    lv_obj_t ** obj_sentinel = عدم;
    bool can_move            = true;
    bool can_begin           = true;

    لكل(;;) {
        إذا(obj_اگلا == عدم) {
            إذا(گروہ->wrap || obj_sentinel == عدم) {
                إذا(!can_begin) أرجع focus_بدلیا;
                obj_اگلا  = begin(&گروہ->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وإلا {
                /*Currently focused object is the last/first in the group, keep it that way*/
                أرجع focus_بدلیا;
            }
        }

        إذا(obj_sentinel == عدم) {
            obj_sentinel = obj_اگلا;
            إذا(obj_sentinel == عدم) أرجع focus_بدلیا; /*Group is empty*/
        }

        إذا(can_move) {
            obj_اگلا = move(&گروہ->obj_ll, obj_اگلا);

            /*Give up if we walked the entire list and haven't found another visible object*/
            إذا(obj_اگلا == obj_sentinel) أرجع focus_بدلیا;
        }

        can_move = true;

        إذا(obj_اگلا == عدم) استمر;
        إذا(lv_obj_get_state(*obj_اگلا) & LV_STATE_DISABLED) استمر;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * ماپا = *obj_اگلا;
        طالما(ماپا) {
            إذا(lv_obj_is_hidden(ماپا)) اكسر;
            ماپا = lv_obj_get_ماپا(ماپا);
        }

        إذا(ماپا && lv_obj_is_hidden(ماپا)) استمر;

        /*If we got her a good candidate is found*/
        اكسر;
    }

    إذا(obj_اگلا == گروہ->obj_focus) أرجع focus_بدلیا; /*There's only one visible object and it's already focused*/

    إذا(گروہ->obj_focus) {
        lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        إذا(res != LV_نتیجہ_OK) أرجع focus_بدلیا;
        lv_obj_invalidate(*گروہ->obj_focus);
    }

    گروہ->obj_focus = obj_اگلا;

    lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_FOCUSED, get_indev(گروہ));
    إذا(res != LV_نتیجہ_OK) أرجع focus_بدلیا;

    lv_obj_invalidate(*گروہ->obj_focus);

    إذا(گروہ->focus_cb) گروہ->focus_cb(گروہ);
    focus_بدلیا = true;
    أرجع focus_بدلیا;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساكن lv_indev_t * get_indev(ثابت lv_گروہ_t * g)
{
    LV_ASSERT(g != عدم);
    lv_indev_t * indev_guess = عدم;
    lv_indev_t * indev = lv_indev_get_اگلا(عدم);

    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        إذا(indev_type == LV_INDEV_TYPE_اشارہ_گر) أرجع indev;
        إذا(lv_indev_get_گروہ(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_اگلا(indev);
    }

    أرجع indev_guess;
}
