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
#تعریف_کړه default_ډله LV_GLOBAL_DEFAULT()->ډله_default
#تعریف_کړه ډله_ll_p &(LV_GLOBAL_DEFAULT()->ډله_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن bool focus_بل_core(lv_ډله_t * ډله, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *));
ساكن فراغ lv_ډله_refocus(lv_ډله_t * g);
ساكن lv_indev_t * get_indev(ثابت lv_ډله_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_ډله_init(فراغ)
{
    lv_ll_init(ډله_ll_p, حجم(lv_ډله_t));
}

فراغ lv_ډله_deinit(فراغ)
{
    lv_ll_clear(ډله_ll_p);
}

lv_ډله_t * lv_ډله_create(فراغ)
{
    lv_ډله_t * ډله = lv_ll_ins_سر(ډله_ll_p);
    LV_ASSERT_MALLOC(ډله);
    إذا(ډله == عدم) أرجع عدم;
    lv_ll_init(&ډله->obj_ll, حجم(lv_obj_t *));

    ډله->obj_focus      = عدم;
    ډله->frozen         = 0;
    ډله->focus_cb       = عدم;
    ډله->څنډه_cb        = عدم;
    ډله->editing        = 0;
    ډله->refocus_policy = LV_ډله_REFOCUS_POLICY_PREV;
    ډله->wrap           = 1;
    ډله->user_معلومات      = عدم;
#إذا LV_USE_EXT_معلومات
    ډله->ext_معلومات.free_cb = عدم;
    ډله->ext_معلومات.معلومات = عدم;
#د_که_پای

    أرجع ډله;
}

فراغ lv_ډله_delete(lv_ډله_t * ډله)
{
    إذا(ډله == عدم) أرجع;

    /*Defocus the currently focused object*/
    إذا(ډله->obj_focus != عدم) {
        lv_obj_send_event(*ډله->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ډله));
        lv_obj_invalidate(*ډله->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ډله->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->ډله_p = عدم;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_بل(عدم);
    طالما(indev) {
        إذا(lv_indev_get_ډله(indev) == ډله) {
            lv_indev_سېټ_ډله(indev, عدم);
        }
        indev = lv_indev_get_بل(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    إذا(ډله == lv_ډله_get_default()) lv_ډله_سېټ_default(عدم);

    lv_ll_clear(&(ډله->obj_ll));
    lv_ll_remove(ډله_ll_p, ډله);
#إذا LV_USE_EXT_معلومات
    إذا(ډله->ext_معلومات.free_cb) {
        ډله->ext_معلومات.free_cb(ډله->ext_معلومات.معلومات);
        ډله->ext_معلومات.معلومات = عدم;
    }
#د_که_پای
    lv_free(ډله);
}

فراغ lv_ډله_سېټ_default(lv_ډله_t * ډله)
{
    default_ډله = ډله;
}

lv_ډله_t * lv_ډله_get_default(فراغ)
{
    أرجع default_ډله;
}

فراغ lv_ډله_جمع_کړه_obj(lv_ډله_t * ډله, lv_obj_t * obj)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_LOG_TRACE("begin");

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    LV_ASSERT(obj->spec_attr != عدم);

    /*Be sure the object is removed from its current group*/
    lv_ډله_remove_obj(obj);
    obj->spec_attr->ډله_p = ډله;

    lv_obj_t ** بل = lv_ll_ins_لکۍ(&ډله->obj_ll);
    LV_ASSERT_MALLOC(بل);
    إذا(بل == عدم) أرجع;
    *بل = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    إذا(lv_ll_get_سر(&ډله->obj_ll) == بل) {
        lv_ډله_refocus(ډله);
    }

    LV_LOG_TRACE("finished");
}

فراغ lv_ډله_بدلون_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != عدم, أرجع);
    LV_CHECK_ARG(obj2 != عدم, أرجع);

    lv_ډله_t * g1 = lv_obj_get_ډله(obj1);
    lv_ډله_t * g2 = lv_obj_get_ډله(obj2);

    إذا(g1 != g2) أرجع;
    إذا(g1 == عدم) أرجع;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        إذا((*obj_i) == obj1)(*obj_i) = obj2;
        وإلا إذا((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_ډله_get_focused(g1);
    إذا(focused == obj1) lv_ډله_focus_obj(obj2);
    وإلا إذا(focused == obj2) lv_ډله_focus_obj(obj1);
}

فراغ lv_ډله_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_ډله_t * g = lv_obj_get_ډله(obj);
    إذا(g == عدم) أرجع;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    إذا(g->obj_focus && *g->obj_focus == obj) {
        إذا(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        إذا(lv_ll_get_سر(&g->obj_ll) != g->obj_focus || lv_ll_get_لکۍ(&g->obj_ll) != g->obj_focus) {
            lv_ډله_refocus(g);
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
            إذا(obj->spec_attr) obj->spec_attr->ډله_p = عدم;
            اكسر;
        }
    }
    LV_LOG_TRACE("finished");
}

فراغ lv_ډله_remove_all_objs(lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    /*Defocus the currently focused object*/
    إذا(ډله->obj_focus != عدم) {
        lv_obj_send_event(*ډله->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ډله));
        lv_obj_invalidate(*ډله->obj_focus);
        ډله->obj_focus = عدم;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&ډله->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->ډله_p = عدم;
    }

    lv_ll_clear(&(ډله->obj_ll));
}

فراغ lv_ډله_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_ډله_t * g = lv_obj_get_ډله(obj);

    إذا(g == عدم) أرجع;
    إذا(g->frozen) أرجع;

    /*On defocus edit mode must be leaved*/
    lv_ډله_سېټ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        إذا(*i == obj) {
            إذا(g->obj_focus != عدم) {
                إذا(*g->obj_focus == obj) اكسر;  /*Refocusing same object*/
                lv_پایله_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                إذا(res != LV_پایله_OK) أرجع;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            إذا(g->focus_cb) g->focus_cb(g);
            lv_پایله_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            إذا(res != LV_پایله_OK) أرجع;
            lv_obj_invalidate(*g->obj_focus);
            اكسر;
        }
    }
}

فراغ lv_ډله_focus_بل(lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    bool focus_بدل_شوی = focus_بل_core(ډله, lv_ll_get_سر, lv_ll_get_بل);

    إذا(!focus_بدل_شوی && ډله->څنډه_cb) {
        ډله->څنډه_cb(ډله, true);
    }
}

فراغ lv_ډله_focus_prev(lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    bool focus_بدل_شوی = focus_بل_core(ډله, lv_ll_get_لکۍ, lv_ll_get_prev);

    إذا(!focus_بدل_شوی && ډله->څنډه_cb) {
        ډله->څنډه_cb(ډله, false);
    }
}

فراغ lv_ډله_focus_freeze(lv_ډله_t * ډله, bool en)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    إذا(en == false) ډله->frozen = 0;
    وإلا ډله->frozen = 1;
}

lv_پایله_t lv_ډله_send_معلومات(lv_ډله_t * ډله, uint32_t c)
{
    LV_CHECK_ARG(ډله != عدم, أرجع LV_پایله_INVALID);

    lv_obj_t * act = lv_ډله_get_focused(ډله);
    إذا(act == عدم) أرجع LV_پایله_OK;

    إذا(lv_obj_has_state(act, LV_STATE_DISABLED)) أرجع LV_پایله_OK;

    أرجع lv_obj_send_event(act, LV_EVENT_کلی, &c);
}

فراغ lv_ډله_سېټ_focus_cb(lv_ډله_t * ډله, lv_ډله_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    ډله->focus_cb = focus_cb;
}

فراغ lv_ډله_سېټ_څنډه_cb(lv_ډله_t * ډله, lv_ډله_څنډه_cb_t څنډه_cb)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    ډله->څنډه_cb = څنډه_cb;
}

فراغ lv_ډله_سېټ_editing(lv_ډله_t * ډله, bool edit)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);
    uint8_t en_val = edit ? 1 : 0;

    إذا(en_val == ډله->editing) أرجع; /*Do not set the same mode again*/

    ډله->editing     = en_val;
    lv_obj_t * focused = lv_ډله_get_focused(ډله);

    إذا(focused) {
        lv_پایله_t res = lv_obj_send_event(*ډله->obj_focus, LV_EVENT_FOCUSED, get_indev(ډله));
        إذا(res != LV_پایله_OK) أرجع;

        lv_obj_invalidate(focused);
    }
}

فراغ lv_ډله_سېټ_refocus_policy(lv_ډله_t * ډله, lv_ډله_refocus_policy_t policy)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);
    ډله->refocus_policy = policy & 0x01;
}

فراغ lv_ډله_سېټ_wrap(lv_ډله_t * ډله, bool en)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);
    ډله->wrap = en ? 1 : 0;
}

lv_obj_t * lv_ډله_get_focused(ثابت lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع عدم);
    إذا(ډله->obj_focus == عدم) أرجع عدم;

    أرجع *ډله->obj_focus;
}

lv_ډله_focus_cb_t lv_ډله_get_focus_cb(ثابت lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع عدم);
    أرجع ډله->focus_cb;
}

lv_ډله_څنډه_cb_t lv_ډله_get_څنډه_cb(ثابت lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع عدم);
    أرجع ډله->څنډه_cb;
}

bool lv_ډله_get_editing(ثابت lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع false);
    أرجع ډله->editing;
}

bool lv_ډله_get_wrap(lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع false);
    أرجع ډله->wrap;
}

uint32_t lv_ډله_get_obj_شمېر(lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع 0);
    أرجع lv_ll_get_len(&ډله->obj_ll);
}

lv_obj_t * lv_ډله_get_obj_by_شاخص(lv_ډله_t * ډله, uint32_t شاخص)
{
    LV_CHECK_ARG(ډله != عدم, أرجع عدم);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&ډله->obj_ll, obj) {
        إذا(len == شاخص) {
            أرجع *obj;
        }
        len++;
    }
    أرجع عدم;
}

uint32_t lv_ډله_get_شمېر(فراغ)
{
    أرجع lv_ll_get_len(ډله_ll_p);
}

lv_ډله_t  * lv_ډله_by_شاخص(uint32_t شاخص)
{
    uint32_t len = 0;
    lv_ډله_t * ډله;

    LV_LL_READ_شا(ډله_ll_p, ډله) {
        إذا(len == شاخص) {
            أرجع ډله;
        }
        len++;
    }

    أرجع عدم;
}

#إذا LV_USE_EXT_معلومات
فراغ lv_ډله_سېټ_external_معلومات(lv_ډله_t * ډله, فراغ * معلومات, فراغ (* free_cb)(فراغ * معلومات))
{
    LV_CHECK_ARG(ډله != عدم, أرجع);

    ډله->ext_معلومات.معلومات = معلومات;
    ډله->ext_معلومات.free_cb = free_cb;
}
#د_که_پای

فراغ lv_ډله_سېټ_user_معلومات(lv_ډله_t * ډله, فراغ * user_معلومات)
{
    LV_CHECK_ARG(ډله != عدم, أرجع);
    ډله->user_معلومات = user_معلومات;
}

فراغ * lv_ډله_get_user_معلومات(ثابت lv_ډله_t * ډله)
{
    LV_CHECK_ARG(ډله != عدم, أرجع عدم);
    أرجع ډله->user_معلومات;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_ډله_refocus(lv_ډله_t * g)
{
    LV_ASSERT(g != عدم);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t لنډمهاله_wrap = g->wrap;
    g->wrap           = 1;

    إذا(g->refocus_policy == LV_ډله_REFOCUS_POLICY_بل)
        lv_ډله_focus_بل(g);
    وإلا إذا(g->refocus_policy == LV_ډله_REFOCUS_POLICY_PREV)
        lv_ډله_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = لنډمهاله_wrap;
}

ساكن bool focus_بل_core(lv_ډله_t * ډله, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *))
{
    LV_ASSERT(ډله != عدم);
    LV_ASSERT(begin != عدم);
    LV_ASSERT(move != عدم);
    إذا(ډله->frozen) أرجع false;

    bool focus_بدل_شوی = false;

    lv_obj_t ** obj_بل     = ډله->obj_focus;
    lv_obj_t ** obj_sentinel = عدم;
    bool can_move            = true;
    bool can_begin           = true;

    لكل(;;) {
        إذا(obj_بل == عدم) {
            إذا(ډله->wrap || obj_sentinel == عدم) {
                إذا(!can_begin) أرجع focus_بدل_شوی;
                obj_بل  = begin(&ډله->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وإلا {
                /*Currently focused object is the last/first in the group, keep it that way*/
                أرجع focus_بدل_شوی;
            }
        }

        إذا(obj_sentinel == عدم) {
            obj_sentinel = obj_بل;
            إذا(obj_sentinel == عدم) أرجع focus_بدل_شوی; /*Group is empty*/
        }

        إذا(can_move) {
            obj_بل = move(&ډله->obj_ll, obj_بل);

            /*Give up if we walked the entire list and haven't found another visible object*/
            إذا(obj_بل == obj_sentinel) أرجع focus_بدل_شوی;
        }

        can_move = true;

        إذا(obj_بل == عدم) استمر;
        إذا(lv_obj_get_state(*obj_بل) & LV_STATE_DISABLED) استمر;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * پلار = *obj_بل;
        طالما(پلار) {
            إذا(lv_obj_is_hidden(پلار)) اكسر;
            پلار = lv_obj_get_پلار(پلار);
        }

        إذا(پلار && lv_obj_is_hidden(پلار)) استمر;

        /*If we got her a good candidate is found*/
        اكسر;
    }

    إذا(obj_بل == ډله->obj_focus) أرجع focus_بدل_شوی; /*There's only one visible object and it's already focused*/

    إذا(ډله->obj_focus) {
        lv_پایله_t res = lv_obj_send_event(*ډله->obj_focus, LV_EVENT_DEFOCUSED, get_indev(ډله));
        إذا(res != LV_پایله_OK) أرجع focus_بدل_شوی;
        lv_obj_invalidate(*ډله->obj_focus);
    }

    ډله->obj_focus = obj_بل;

    lv_پایله_t res = lv_obj_send_event(*ډله->obj_focus, LV_EVENT_FOCUSED, get_indev(ډله));
    إذا(res != LV_پایله_OK) أرجع focus_بدل_شوی;

    lv_obj_invalidate(*ډله->obj_focus);

    إذا(ډله->focus_cb) ډله->focus_cb(ډله);
    focus_بدل_شوی = true;
    أرجع focus_بدل_شوی;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساكن lv_indev_t * get_indev(ثابت lv_ډله_t * g)
{
    LV_ASSERT(g != عدم);
    lv_indev_t * indev_guess = عدم;
    lv_indev_t * indev = lv_indev_get_بل(عدم);

    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        إذا(indev_type == LV_INDEV_TYPE_اشاره_ګر) أرجع indev;
        إذا(lv_indev_get_ډله(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_بل(indev);
    }

    أرجع indev_guess;
}
