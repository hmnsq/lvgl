/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_group_private.h"
#شامل "../core/lv_obj_private.h"
#شامل "../core/lv_global.h"
#شامل "../lvgl_public.h"

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
ساکن bool focus_اگلا_core(lv_گروہ_t * گروہ, خالی * (*begin)(ثابت lv_ll_t *),
                            خالی * (*move)(ثابت lv_ll_t *, ثابت خالی *));
ساکن خالی lv_گروہ_refocus(lv_گروہ_t * g);
ساکن lv_indev_t * get_indev(ثابت lv_گروہ_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

خالی lv_گروہ_init(خالی)
{
    lv_ll_init(گروہ_ll_p, حجم(lv_گروہ_t));
}

خالی lv_گروہ_deinit(خالی)
{
    lv_ll_clear(گروہ_ll_p);
}

lv_گروہ_t * lv_گروہ_create(خالی)
{
    lv_گروہ_t * گروہ = lv_ll_ins_سر(گروہ_ll_p);
    LV_ASSERT_MALLOC(گروہ);
    اگر(گروہ == تہی) واپس تہی;
    lv_ll_init(&گروہ->obj_ll, حجم(lv_obj_t *));

    گروہ->obj_focus      = تہی;
    گروہ->frozen         = 0;
    گروہ->focus_cb       = تہی;
    گروہ->کنارہ_cb        = تہی;
    گروہ->editing        = 0;
    گروہ->refocus_policy = LV_گروہ_REFOCUS_POLICY_پچھلا;
    گروہ->wrap           = 1;
    گروہ->user_ڈیٹا      = تہی;
#اگر LV_USE_EXT_ڈیٹا
    گروہ->ext_ڈیٹا.free_cb = تہی;
    گروہ->ext_ڈیٹا.ڈیٹا = تہی;
#اگر_ختم

    واپس گروہ;
}

خالی lv_گروہ_delete(lv_گروہ_t * گروہ)
{
    اگر(گروہ == تہی) واپس;

    /*Defocus the currently focused object*/
    اگر(گروہ->obj_focus != تہی) {
        lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        lv_obj_invalidate(*گروہ->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروہ->obj_ll, obj) {
        اگر((*obj)->spec_attr)(*obj)->spec_attr->گروہ_p = تہی;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_اگلا(تہی);
    جب(indev) {
        اگر(lv_indev_get_گروہ(indev) == گروہ) {
            lv_indev_سیٹ_گروہ(indev, تہی);
        }
        indev = lv_indev_get_اگلا(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    اگر(گروہ == lv_گروہ_get_default()) lv_گروہ_سیٹ_default(تہی);

    lv_ll_clear(&(گروہ->obj_ll));
    lv_ll_remove(گروہ_ll_p, گروہ);
#اگر LV_USE_EXT_ڈیٹا
    اگر(گروہ->ext_ڈیٹا.free_cb) {
        گروہ->ext_ڈیٹا.free_cb(گروہ->ext_ڈیٹا.ڈیٹا);
        گروہ->ext_ڈیٹا.ڈیٹا = تہی;
    }
#اگر_ختم
    lv_free(گروہ);
}

خالی lv_گروہ_سیٹ_default(lv_گروہ_t * گروہ)
{
    default_گروہ = گروہ;
}

lv_گروہ_t * lv_گروہ_get_default(خالی)
{
    واپس default_گروہ;
}

خالی lv_گروہ_جوڑو_obj(lv_گروہ_t * گروہ, lv_obj_t * obj)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);
    LV_CHECK_ARG(obj != تہی, واپس);

    LV_LOG_TRACE("begin");

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }

    LV_ASSERT(obj->spec_attr != تہی);

    /*Be sure the object is removed from its current group*/
    lv_گروہ_remove_obj(obj);
    obj->spec_attr->گروہ_p = گروہ;

    lv_obj_t ** اگلا = lv_ll_ins_دم(&گروہ->obj_ll);
    LV_ASSERT_MALLOC(اگلا);
    اگر(اگلا == تہی) واپس;
    *اگلا = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    اگر(lv_ll_get_سر(&گروہ->obj_ll) == اگلا) {
        lv_گروہ_refocus(گروہ);
    }

    LV_LOG_TRACE("finished");
}

خالی lv_گروہ_تبادلہ_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != تہی, واپس);
    LV_CHECK_ARG(obj2 != تہی, واپس);

    lv_گروہ_t * g1 = lv_obj_get_گروہ(obj1);
    lv_گروہ_t * g2 = lv_obj_get_گروہ(obj2);

    اگر(g1 != g2) واپس;
    اگر(g1 == تہی) واپس;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        اگر((*obj_i) == obj1)(*obj_i) = obj2;
        ورنہ اگر((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_گروہ_get_focused(g1);
    اگر(focused == obj1) lv_گروہ_focus_obj(obj2);
    ورنہ اگر(focused == obj2) lv_گروہ_focus_obj(obj1);
}

خالی lv_گروہ_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);

    lv_گروہ_t * g = lv_obj_get_گروہ(obj);
    اگر(g == تہی) واپس;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    اگر(g->obj_focus && *g->obj_focus == obj) {
        اگر(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        اگر(lv_ll_get_سر(&g->obj_ll) != g->obj_focus || lv_ll_get_دم(&g->obj_ll) != g->obj_focus) {
            lv_گروہ_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    اگر(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = تہی;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        اگر(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            اگر(obj->spec_attr) obj->spec_attr->گروہ_p = تہی;
            توڑو;
        }
    }
    LV_LOG_TRACE("finished");
}

خالی lv_گروہ_remove_all_objs(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    /*Defocus the currently focused object*/
    اگر(گروہ->obj_focus != تہی) {
        lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        lv_obj_invalidate(*گروہ->obj_focus);
        گروہ->obj_focus = تہی;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&گروہ->obj_ll, obj) {
        اگر((*obj)->spec_attr)(*obj)->spec_attr->گروہ_p = تہی;
    }

    lv_ll_clear(&(گروہ->obj_ll));
}

خالی lv_گروہ_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);

    lv_گروہ_t * g = lv_obj_get_گروہ(obj);

    اگر(g == تہی) واپس;
    اگر(g->frozen) واپس;

    /*On defocus edit mode must be leaved*/
    lv_گروہ_سیٹ_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        اگر(*i == obj) {
            اگر(g->obj_focus != تہی) {
                اگر(*g->obj_focus == obj) توڑو;  /*Refocusing same object*/
                lv_نتیجہ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                اگر(res != LV_نتیجہ_OK) واپس;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            اگر(g->focus_cb) g->focus_cb(g);
            lv_نتیجہ_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            اگر(res != LV_نتیجہ_OK) واپس;
            lv_obj_invalidate(*g->obj_focus);
            توڑو;
        }
    }
}

خالی lv_گروہ_focus_اگلا(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    bool focus_بدلا = focus_اگلا_core(گروہ, lv_ll_get_سر, lv_ll_get_اگلا);

    اگر(!focus_بدلا && گروہ->کنارہ_cb) {
        گروہ->کنارہ_cb(گروہ, true);
    }
}

خالی lv_گروہ_focus_پچھلا(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    bool focus_بدلا = focus_اگلا_core(گروہ, lv_ll_get_دم, lv_ll_get_پچھلا);

    اگر(!focus_بدلا && گروہ->کنارہ_cb) {
        گروہ->کنارہ_cb(گروہ, false);
    }
}

خالی lv_گروہ_focus_freeze(lv_گروہ_t * گروہ, bool en)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    اگر(en == false) گروہ->frozen = 0;
    ورنہ گروہ->frozen = 1;
}

lv_نتیجہ_t lv_گروہ_send_ڈیٹا(lv_گروہ_t * گروہ, uint32_t c)
{
    LV_CHECK_ARG(گروہ != تہی, واپس LV_نتیجہ_INVALID);

    lv_obj_t * act = lv_گروہ_get_focused(گروہ);
    اگر(act == تہی) واپس LV_نتیجہ_OK;

    اگر(lv_obj_has_state(act, LV_STATE_DISABLED)) واپس LV_نتیجہ_OK;

    واپس lv_obj_send_event(act, LV_EVENT_کلید, &c);
}

خالی lv_گروہ_سیٹ_focus_cb(lv_گروہ_t * گروہ, lv_گروہ_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    گروہ->focus_cb = focus_cb;
}

خالی lv_گروہ_سیٹ_کنارہ_cb(lv_گروہ_t * گروہ, lv_گروہ_کنارہ_cb_t کنارہ_cb)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    گروہ->کنارہ_cb = کنارہ_cb;
}

خالی lv_گروہ_سیٹ_editing(lv_گروہ_t * گروہ, bool edit)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);
    uint8_t en_val = edit ? 1 : 0;

    اگر(en_val == گروہ->editing) واپس; /*Do not set the same mode again*/

    گروہ->editing     = en_val;
    lv_obj_t * focused = lv_گروہ_get_focused(گروہ);

    اگر(focused) {
        lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_FOCUSED, get_indev(گروہ));
        اگر(res != LV_نتیجہ_OK) واپس;

        lv_obj_invalidate(focused);
    }
}

خالی lv_گروہ_سیٹ_refocus_policy(lv_گروہ_t * گروہ, lv_گروہ_refocus_policy_t policy)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);
    گروہ->refocus_policy = policy & 0x01;
}

خالی lv_گروہ_سیٹ_wrap(lv_گروہ_t * گروہ, bool en)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);
    گروہ->wrap = en ? 1 : 0;
}

lv_obj_t * lv_گروہ_get_focused(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس تہی);
    اگر(گروہ->obj_focus == تہی) واپس تہی;

    واپس *گروہ->obj_focus;
}

lv_گروہ_focus_cb_t lv_گروہ_get_focus_cb(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس تہی);
    واپس گروہ->focus_cb;
}

lv_گروہ_کنارہ_cb_t lv_گروہ_get_کنارہ_cb(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس تہی);
    واپس گروہ->کنارہ_cb;
}

bool lv_گروہ_get_editing(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس false);
    واپس گروہ->editing;
}

bool lv_گروہ_get_wrap(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس false);
    واپس گروہ->wrap;
}

uint32_t lv_گروہ_get_obj_گنتی(lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس 0);
    واپس lv_ll_get_len(&گروہ->obj_ll);
}

lv_obj_t * lv_گروہ_get_obj_by_اشاریہ(lv_گروہ_t * گروہ, uint32_t اشاریہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس تہی);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&گروہ->obj_ll, obj) {
        اگر(len == اشاریہ) {
            واپس *obj;
        }
        len++;
    }
    واپس تہی;
}

uint32_t lv_گروہ_get_گنتی(خالی)
{
    واپس lv_ll_get_len(گروہ_ll_p);
}

lv_گروہ_t  * lv_گروہ_by_اشاریہ(uint32_t اشاریہ)
{
    uint32_t len = 0;
    lv_گروہ_t * گروہ;

    LV_LL_READ_عقب(گروہ_ll_p, گروہ) {
        اگر(len == اشاریہ) {
            واپس گروہ;
        }
        len++;
    }

    واپس تہی;
}

#اگر LV_USE_EXT_ڈیٹا
خالی lv_گروہ_سیٹ_external_ڈیٹا(lv_گروہ_t * گروہ, خالی * ڈیٹا, خالی (* free_cb)(خالی * ڈیٹا))
{
    LV_CHECK_ARG(گروہ != تہی, واپس);

    گروہ->ext_ڈیٹا.ڈیٹا = ڈیٹا;
    گروہ->ext_ڈیٹا.free_cb = free_cb;
}
#اگر_ختم

خالی lv_گروہ_سیٹ_user_ڈیٹا(lv_گروہ_t * گروہ, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(گروہ != تہی, واپس);
    گروہ->user_ڈیٹا = user_ڈیٹا;
}

خالی * lv_گروہ_get_user_ڈیٹا(ثابت lv_گروہ_t * گروہ)
{
    LV_CHECK_ARG(گروہ != تہی, واپس تہی);
    واپس گروہ->user_ڈیٹا;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن خالی lv_گروہ_refocus(lv_گروہ_t * g)
{
    LV_ASSERT(g != تہی);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t عارضی_wrap = g->wrap;
    g->wrap           = 1;

    اگر(g->refocus_policy == LV_گروہ_REFOCUS_POLICY_اگلا)
        lv_گروہ_focus_اگلا(g);
    ورنہ اگر(g->refocus_policy == LV_گروہ_REFOCUS_POLICY_پچھلا)
        lv_گروہ_focus_پچھلا(g);
    /*Restore wrap property*/
    g->wrap = عارضی_wrap;
}

ساکن bool focus_اگلا_core(lv_گروہ_t * گروہ, خالی * (*begin)(ثابت lv_ll_t *),
                            خالی * (*move)(ثابت lv_ll_t *, ثابت خالی *))
{
    LV_ASSERT(گروہ != تہی);
    LV_ASSERT(begin != تہی);
    LV_ASSERT(move != تہی);
    اگر(گروہ->frozen) واپس false;

    bool focus_بدلا = false;

    lv_obj_t ** obj_اگلا     = گروہ->obj_focus;
    lv_obj_t ** obj_sentinel = تہی;
    bool can_move            = true;
    bool can_begin           = true;

    برائے(;;) {
        اگر(obj_اگلا == تہی) {
            اگر(گروہ->wrap || obj_sentinel == تہی) {
                اگر(!can_begin) واپس focus_بدلا;
                obj_اگلا  = begin(&گروہ->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ورنہ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                واپس focus_بدلا;
            }
        }

        اگر(obj_sentinel == تہی) {
            obj_sentinel = obj_اگلا;
            اگر(obj_sentinel == تہی) واپس focus_بدلا; /*Group is empty*/
        }

        اگر(can_move) {
            obj_اگلا = move(&گروہ->obj_ll, obj_اگلا);

            /*Give up if we walked the entire list and haven't found another visible object*/
            اگر(obj_اگلا == obj_sentinel) واپس focus_بدلا;
        }

        can_move = true;

        اگر(obj_اگلا == تہی) جاری;
        اگر(lv_obj_get_state(*obj_اگلا) & LV_STATE_DISABLED) جاری;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * والد = *obj_اگلا;
        جب(والد) {
            اگر(lv_obj_is_hidden(والد)) توڑو;
            والد = lv_obj_get_والد(والد);
        }

        اگر(والد && lv_obj_is_hidden(والد)) جاری;

        /*If we got her a good candidate is found*/
        توڑو;
    }

    اگر(obj_اگلا == گروہ->obj_focus) واپس focus_بدلا; /*There's only one visible object and it's already focused*/

    اگر(گروہ->obj_focus) {
        lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_DEFOCUSED, get_indev(گروہ));
        اگر(res != LV_نتیجہ_OK) واپس focus_بدلا;
        lv_obj_invalidate(*گروہ->obj_focus);
    }

    گروہ->obj_focus = obj_اگلا;

    lv_نتیجہ_t res = lv_obj_send_event(*گروہ->obj_focus, LV_EVENT_FOCUSED, get_indev(گروہ));
    اگر(res != LV_نتیجہ_OK) واپس focus_بدلا;

    lv_obj_invalidate(*گروہ->obj_focus);

    اگر(گروہ->focus_cb) گروہ->focus_cb(گروہ);
    focus_بدلا = true;
    واپس focus_بدلا;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساکن lv_indev_t * get_indev(ثابت lv_گروہ_t * g)
{
    LV_ASSERT(g != تہی);
    lv_indev_t * indev_guess = تہی;
    lv_indev_t * indev = lv_indev_get_اگلا(تہی);

    جب(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        اگر(indev_type == LV_INDEV_TYPE_اشارہ_گر) واپس indev;
        اگر(lv_indev_get_گروہ(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_اگلا(indev);
    }

    واپس indev_guess;
}
