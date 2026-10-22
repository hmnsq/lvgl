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
#تعریف_کن default_گروه LV_GLOBAL_DEFAULT()->گروه_default
#تعریف_کن گروه_ll_p &(LV_GLOBAL_DEFAULT()->گروه_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن bool focus_بعدی_core(lv_گروه_t * گروه, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *));
ساكن فراغ lv_گروه_refocus(lv_گروه_t * g);
ساكن lv_indev_t * get_indev(ثابت lv_گروه_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_گروه_init(فراغ)
{
    lv_ll_init(گروه_ll_p, حجم(lv_گروه_t));
}

فراغ lv_گروه_deinit(فراغ)
{
    lv_ll_clear(گروه_ll_p);
}

lv_گروه_t * lv_گروه_create(فراغ)
{
    lv_گروه_t * گروه = lv_ll_ins_سر(گروه_ll_p);
    LV_ASSERT_MALLOC(گروه);
    إذا(گروه == عدم) أرجع عدم;
    lv_ll_init(&گروه->obj_ll, حجم(lv_obj_t *));

    گروه->obj_focus      = عدم;
    گروه->frozen         = 0;
    گروه->focus_cb       = عدم;
    گروه->یال_cb        = عدم;
    گروه->editing        = 0;
    گروه->refocus_policy = LV_گروه_REFOCUS_POLICY_PREV;
    گروه->wrap           = 1;
    گروه->user_داده      = عدم;
#إذا LV_USE_EXT_داده
    گروه->ext_داده.free_cb = عدم;
    گروه->ext_داده.داده = عدم;
#پایان_اگر

    أرجع گروه;
}

فراغ lv_گروه_delete(lv_گروه_t * گروه)
{
    إذا(گروه == عدم) أرجع;

    /*Defocus the currently focused object*/
    إذا(گروه->obj_focus != عدم) {
        lv_obj_send_event(*گروه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروه));
        lv_obj_invalidate(*گروه->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروه->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروه_p = عدم;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_بعدی(عدم);
    طالما(indev) {
        إذا(lv_indev_get_گروه(indev) == گروه) {
            lv_indev_مجموعه_گروه(indev, عدم);
        }
        indev = lv_indev_get_بعدی(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    إذا(گروه == lv_گروه_get_default()) lv_گروه_مجموعه_default(عدم);

    lv_ll_clear(&(گروه->obj_ll));
    lv_ll_remove(گروه_ll_p, گروه);
#إذا LV_USE_EXT_داده
    إذا(گروه->ext_داده.free_cb) {
        گروه->ext_داده.free_cb(گروه->ext_داده.داده);
        گروه->ext_داده.داده = عدم;
    }
#پایان_اگر
    lv_free(گروه);
}

فراغ lv_گروه_مجموعه_default(lv_گروه_t * گروه)
{
    default_گروه = گروه;
}

lv_گروه_t * lv_گروه_get_default(فراغ)
{
    أرجع default_گروه;
}

فراغ lv_گروه_جمع_کن_obj(lv_گروه_t * گروه, lv_obj_t * obj)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_LOG_TRACE("begin");

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    LV_ASSERT(obj->spec_attr != عدم);

    /*Be sure the object is removed from its current group*/
    lv_گروه_remove_obj(obj);
    obj->spec_attr->گروه_p = گروه;

    lv_obj_t ** بعدی = lv_ll_ins_دم(&گروه->obj_ll);
    LV_ASSERT_MALLOC(بعدی);
    إذا(بعدی == عدم) أرجع;
    *بعدی = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    إذا(lv_ll_get_سر(&گروه->obj_ll) == بعدی) {
        lv_گروه_refocus(گروه);
    }

    LV_LOG_TRACE("finished");
}

فراغ lv_گروه_جابجا_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != عدم, أرجع);
    LV_CHECK_ARG(obj2 != عدم, أرجع);

    lv_گروه_t * g1 = lv_obj_get_گروه(obj1);
    lv_گروه_t * g2 = lv_obj_get_گروه(obj2);

    إذا(g1 != g2) أرجع;
    إذا(g1 == عدم) أرجع;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        إذا((*obj_i) == obj1)(*obj_i) = obj2;
        وإلا إذا((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_گروه_get_focused(g1);
    إذا(focused == obj1) lv_گروه_focus_obj(obj2);
    وإلا إذا(focused == obj2) lv_گروه_focus_obj(obj1);
}

فراغ lv_گروه_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروه_t * g = lv_obj_get_گروه(obj);
    إذا(g == عدم) أرجع;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    إذا(g->obj_focus && *g->obj_focus == obj) {
        إذا(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        إذا(lv_ll_get_سر(&g->obj_ll) != g->obj_focus || lv_ll_get_دم(&g->obj_ll) != g->obj_focus) {
            lv_گروه_refocus(g);
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
            إذا(obj->spec_attr) obj->spec_attr->گروه_p = عدم;
            اكسر;
        }
    }
    LV_LOG_TRACE("finished");
}

فراغ lv_گروه_remove_all_objs(lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    /*Defocus the currently focused object*/
    إذا(گروه->obj_focus != عدم) {
        lv_obj_send_event(*گروه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروه));
        lv_obj_invalidate(*گروه->obj_focus);
        گروه->obj_focus = عدم;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروه->obj_ll, obj) {
        إذا((*obj)->spec_attr)(*obj)->spec_attr->گروه_p = عدم;
    }

    lv_ll_clear(&(گروه->obj_ll));
}

فراغ lv_گروه_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_گروه_t * g = lv_obj_get_گروه(obj);

    إذا(g == عدم) أرجع;
    إذا(g->frozen) أرجع;

    /*On defocus edit mode must be leaved*/
    lv_گروه_مجموعه_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        إذا(*i == obj) {
            إذا(g->obj_focus != عدم) {
                إذا(*g->obj_focus == obj) اكسر;  /*Refocusing same object*/
                lv_نتیجه_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                إذا(res != LV_نتیجه_OK) أرجع;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            إذا(g->focus_cb) g->focus_cb(g);
            lv_نتیجه_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            إذا(res != LV_نتیجه_OK) أرجع;
            lv_obj_invalidate(*g->obj_focus);
            اكسر;
        }
    }
}

فراغ lv_گروه_focus_بعدی(lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    bool focus_تغییریافته = focus_بعدی_core(گروه, lv_ll_get_سر, lv_ll_get_بعدی);

    إذا(!focus_تغییریافته && گروه->یال_cb) {
        گروه->یال_cb(گروه, true);
    }
}

فراغ lv_گروه_focus_prev(lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    bool focus_تغییریافته = focus_بعدی_core(گروه, lv_ll_get_دم, lv_ll_get_prev);

    إذا(!focus_تغییریافته && گروه->یال_cb) {
        گروه->یال_cb(گروه, false);
    }
}

فراغ lv_گروه_focus_freeze(lv_گروه_t * گروه, bool en)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    إذا(en == false) گروه->frozen = 0;
    وإلا گروه->frozen = 1;
}

lv_نتیجه_t lv_گروه_send_داده(lv_گروه_t * گروه, uint32_t c)
{
    LV_CHECK_ARG(گروه != عدم, أرجع LV_نتیجه_INVALID);

    lv_obj_t * act = lv_گروه_get_focused(گروه);
    إذا(act == عدم) أرجع LV_نتیجه_OK;

    إذا(lv_obj_has_state(act, LV_STATE_DISABLED)) أرجع LV_نتیجه_OK;

    أرجع lv_obj_send_event(act, LV_EVENT_کلید, &c);
}

فراغ lv_گروه_مجموعه_focus_cb(lv_گروه_t * گروه, lv_گروه_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    گروه->focus_cb = focus_cb;
}

فراغ lv_گروه_مجموعه_یال_cb(lv_گروه_t * گروه, lv_گروه_یال_cb_t یال_cb)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    گروه->یال_cb = یال_cb;
}

فراغ lv_گروه_مجموعه_editing(lv_گروه_t * گروه, bool edit)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);
    uint8_t en_val = edit ? 1 : 0;

    إذا(en_val == گروه->editing) أرجع; /*Do not set the same mode again*/

    گروه->editing     = en_val;
    lv_obj_t * focused = lv_گروه_get_focused(گروه);

    إذا(focused) {
        lv_نتیجه_t res = lv_obj_send_event(*گروه->obj_focus, LV_EVENT_FOCUSED, get_indev(گروه));
        إذا(res != LV_نتیجه_OK) أرجع;

        lv_obj_invalidate(focused);
    }
}

فراغ lv_گروه_مجموعه_refocus_policy(lv_گروه_t * گروه, lv_گروه_refocus_policy_t policy)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);
    گروه->refocus_policy = policy & 0x01;
}

فراغ lv_گروه_مجموعه_wrap(lv_گروه_t * گروه, bool en)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);
    گروه->wrap = en ? 1 : 0;
}

lv_obj_t * lv_گروه_get_focused(ثابت lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع عدم);
    إذا(گروه->obj_focus == عدم) أرجع عدم;

    أرجع *گروه->obj_focus;
}

lv_گروه_focus_cb_t lv_گروه_get_focus_cb(ثابت lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع عدم);
    أرجع گروه->focus_cb;
}

lv_گروه_یال_cb_t lv_گروه_get_یال_cb(ثابت lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع عدم);
    أرجع گروه->یال_cb;
}

bool lv_گروه_get_editing(ثابت lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع false);
    أرجع گروه->editing;
}

bool lv_گروه_get_wrap(lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع false);
    أرجع گروه->wrap;
}

uint32_t lv_گروه_get_obj_شمار(lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع 0);
    أرجع lv_ll_get_len(&گروه->obj_ll);
}

lv_obj_t * lv_گروه_get_obj_by_شاخص(lv_گروه_t * گروه, uint32_t شاخص)
{
    LV_CHECK_ARG(گروه != عدم, أرجع عدم);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&گروه->obj_ll, obj) {
        إذا(len == شاخص) {
            أرجع *obj;
        }
        len++;
    }
    أرجع عدم;
}

uint32_t lv_گروه_get_شمار(فراغ)
{
    أرجع lv_ll_get_len(گروه_ll_p);
}

lv_گروه_t  * lv_گروه_by_شاخص(uint32_t شاخص)
{
    uint32_t len = 0;
    lv_گروه_t * گروه;

    LV_LL_READ_پشت(گروه_ll_p, گروه) {
        إذا(len == شاخص) {
            أرجع گروه;
        }
        len++;
    }

    أرجع عدم;
}

#إذا LV_USE_EXT_داده
فراغ lv_گروه_مجموعه_external_داده(lv_گروه_t * گروه, فراغ * داده, فراغ (* free_cb)(فراغ * داده))
{
    LV_CHECK_ARG(گروه != عدم, أرجع);

    گروه->ext_داده.داده = داده;
    گروه->ext_داده.free_cb = free_cb;
}
#پایان_اگر

فراغ lv_گروه_مجموعه_user_داده(lv_گروه_t * گروه, فراغ * user_داده)
{
    LV_CHECK_ARG(گروه != عدم, أرجع);
    گروه->user_داده = user_داده;
}

فراغ * lv_گروه_get_user_داده(ثابت lv_گروه_t * گروه)
{
    LV_CHECK_ARG(گروه != عدم, أرجع عدم);
    أرجع گروه->user_داده;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_گروه_refocus(lv_گروه_t * g)
{
    LV_ASSERT(g != عدم);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t موقت_wrap = g->wrap;
    g->wrap           = 1;

    إذا(g->refocus_policy == LV_گروه_REFOCUS_POLICY_بعدی)
        lv_گروه_focus_بعدی(g);
    وإلا إذا(g->refocus_policy == LV_گروه_REFOCUS_POLICY_PREV)
        lv_گروه_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = موقت_wrap;
}

ساكن bool focus_بعدی_core(lv_گروه_t * گروه, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *))
{
    LV_ASSERT(گروه != عدم);
    LV_ASSERT(begin != عدم);
    LV_ASSERT(move != عدم);
    إذا(گروه->frozen) أرجع false;

    bool focus_تغییریافته = false;

    lv_obj_t ** obj_بعدی     = گروه->obj_focus;
    lv_obj_t ** obj_sentinel = عدم;
    bool can_move            = true;
    bool can_begin           = true;

    لكل(;;) {
        إذا(obj_بعدی == عدم) {
            إذا(گروه->wrap || obj_sentinel == عدم) {
                إذا(!can_begin) أرجع focus_تغییریافته;
                obj_بعدی  = begin(&گروه->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وإلا {
                /*Currently focused object is the last/first in the group, keep it that way*/
                أرجع focus_تغییریافته;
            }
        }

        إذا(obj_sentinel == عدم) {
            obj_sentinel = obj_بعدی;
            إذا(obj_sentinel == عدم) أرجع focus_تغییریافته; /*Group is empty*/
        }

        إذا(can_move) {
            obj_بعدی = move(&گروه->obj_ll, obj_بعدی);

            /*Give up if we walked the entire list and haven't found another visible object*/
            إذا(obj_بعدی == obj_sentinel) أرجع focus_تغییریافته;
        }

        can_move = true;

        إذا(obj_بعدی == عدم) استمر;
        إذا(lv_obj_get_state(*obj_بعدی) & LV_STATE_DISABLED) استمر;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * والد = *obj_بعدی;
        طالما(والد) {
            إذا(lv_obj_is_hidden(والد)) اكسر;
            والد = lv_obj_get_والد(والد);
        }

        إذا(والد && lv_obj_is_hidden(والد)) استمر;

        /*If we got her a good candidate is found*/
        اكسر;
    }

    إذا(obj_بعدی == گروه->obj_focus) أرجع focus_تغییریافته; /*There's only one visible object and it's already focused*/

    إذا(گروه->obj_focus) {
        lv_نتیجه_t res = lv_obj_send_event(*گروه->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروه));
        إذا(res != LV_نتیجه_OK) أرجع focus_تغییریافته;
        lv_obj_invalidate(*گروه->obj_focus);
    }

    گروه->obj_focus = obj_بعدی;

    lv_نتیجه_t res = lv_obj_send_event(*گروه->obj_focus, LV_EVENT_FOCUSED, get_indev(گروه));
    إذا(res != LV_نتیجه_OK) أرجع focus_تغییریافته;

    lv_obj_invalidate(*گروه->obj_focus);

    إذا(گروه->focus_cb) گروه->focus_cb(گروه);
    focus_تغییریافته = true;
    أرجع focus_تغییریافته;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساكن lv_indev_t * get_indev(ثابت lv_گروه_t * g)
{
    LV_ASSERT(g != عدم);
    lv_indev_t * indev_guess = عدم;
    lv_indev_t * indev = lv_indev_get_بعدی(عدم);

    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        إذا(indev_type == LV_INDEV_TYPE_اشاره_گر) أرجع indev;
        إذا(lv_indev_get_گروه(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_بعدی(indev);
    }

    أرجع indev_guess;
}
