/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_group_private.h"
#అన్తర్భావ "../core/lv_obj_private.h"
#అన్తర్భావ "../core/lv_global.h"
#అన్తర్భావ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు default_సమూహం LV_GLOBAL_DEFAULT()->సమూహం_default
#నిర్వచించు సమూహం_ll_p &(LV_GLOBAL_DEFAULT()->సమూహం_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర bool focus_తదుపరి_core(lv_సమూహం_t * సమూహం, శూన్య * (*begin)(స్థిరరాశి lv_ll_t *),
                            శూన్య * (*move)(స్థిరరాశి lv_ll_t *, స్థిరరాశి శూన్య *));
స్థిర శూన్య lv_సమూహం_refocus(lv_సమూహం_t * g);
స్థిర lv_indev_t * get_indev(స్థిరరాశి lv_సమూహం_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

శూన్య lv_సమూహం_init(శూన్య)
{
    lv_ll_init(సమూహం_ll_p, పరిమాణం(lv_సమూహం_t));
}

శూన్య lv_సమూహం_deinit(శూన్య)
{
    lv_ll_clear(సమూహం_ll_p);
}

lv_సమూహం_t * lv_సమూహం_create(శూన్య)
{
    lv_సమూహం_t * సమూహం = lv_ll_ins_తల(సమూహం_ll_p);
    LV_ASSERT_MALLOC(సమూహం);
    ఒకవేళ(సమూహం == ఖాళీ) తిరిగి ఖాళీ;
    lv_ll_init(&సమూహం->obj_ll, పరిమాణం(lv_obj_t *));

    సమూహం->obj_focus      = ఖాళీ;
    సమూహం->frozen         = 0;
    సమూహం->focus_cb       = ఖాళీ;
    సమూహం->అంచు_cb        = ఖాళీ;
    సమూహం->editing        = 0;
    సమూహం->refocus_policy = LV_సమూహం_REFOCUS_POLICY_PREV;
    సమూహం->wrap           = 1;
    సమూహం->user_దత్తాంశం      = ఖాళీ;
#ఒకవేళ LV_USE_EXT_దత్తాంశం
    సమూహం->ext_దత్తాంశం.free_cb = ఖాళీ;
    సమూహం->ext_దత్తాంశం.దత్తాంశం = ఖాళీ;
#అయితే_ముగింపు

    తిరిగి సమూహం;
}

శూన్య lv_సమూహం_delete(lv_సమూహం_t * సమూహం)
{
    ఒకవేళ(సమూహం == ఖాళీ) తిరిగి;

    /*Defocus the currently focused object*/
    ఒకవేళ(సమూహం->obj_focus != ఖాళీ) {
        lv_obj_send_event(*సమూహం->obj_focus, LV_EVENT_DEFOCUSED, get_indev(సమూహం));
        lv_obj_invalidate(*సమూహం->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&సమూహం->obj_ll, obj) {
        ఒకవేళ((*obj)->spec_attr)(*obj)->spec_attr->సమూహం_p = ఖాళీ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_తదుపరి(ఖాళీ);
    అయితే(indev) {
        ఒకవేళ(lv_indev_get_సమూహం(indev) == సమూహం) {
            lv_indev_సమితి_సమూహం(indev, ఖాళీ);
        }
        indev = lv_indev_get_తదుపరి(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ఒకవేళ(సమూహం == lv_సమూహం_get_default()) lv_సమూహం_సమితి_default(ఖాళీ);

    lv_ll_clear(&(సమూహం->obj_ll));
    lv_ll_remove(సమూహం_ll_p, సమూహం);
#ఒకవేళ LV_USE_EXT_దత్తాంశం
    ఒకవేళ(సమూహం->ext_దత్తాంశం.free_cb) {
        సమూహం->ext_దత్తాంశం.free_cb(సమూహం->ext_దత్తాంశం.దత్తాంశం);
        సమూహం->ext_దత్తాంశం.దత్తాంశం = ఖాళీ;
    }
#అయితే_ముగింపు
    lv_free(సమూహం);
}

శూన్య lv_సమూహం_సమితి_default(lv_సమూహం_t * సమూహం)
{
    default_సమూహం = సమూహం;
}

lv_సమూహం_t * lv_సమూహం_get_default(శూన్య)
{
    తిరిగి default_సమూహం;
}

శూన్య lv_సమూహం_కూడు_obj(lv_సమూహం_t * సమూహం, lv_obj_t * obj)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    LV_LOG_TRACE("begin");

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }

    LV_ASSERT(obj->spec_attr != ఖాళీ);

    /*Be sure the object is removed from its current group*/
    lv_సమూహం_remove_obj(obj);
    obj->spec_attr->సమూహం_p = సమూహం;

    lv_obj_t ** తదుపరి = lv_ll_ins_తోక(&సమూహం->obj_ll);
    LV_ASSERT_MALLOC(తదుపరి);
    ఒకవేళ(తదుపరి == ఖాళీ) తిరిగి;
    *తదుపరి = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ఒకవేళ(lv_ll_get_తల(&సమూహం->obj_ll) == తదుపరి) {
        lv_సమూహం_refocus(సమూహం);
    }

    LV_LOG_TRACE("finished");
}

శూన్య lv_సమూహం_మార్పిడి_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(obj2 != ఖాళీ, తిరిగి);

    lv_సమూహం_t * g1 = lv_obj_get_సమూహం(obj1);
    lv_సమూహం_t * g2 = lv_obj_get_సమూహం(obj2);

    ఒకవేళ(g1 != g2) తిరిగి;
    ఒకవేళ(g1 == ఖాళీ) తిరిగి;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ఒకవేళ((*obj_i) == obj1)(*obj_i) = obj2;
        లేకపోతే ఒకవేళ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_సమూహం_get_focused(g1);
    ఒకవేళ(focused == obj1) lv_సమూహం_focus_obj(obj2);
    లేకపోతే ఒకవేళ(focused == obj2) lv_సమూహం_focus_obj(obj1);
}

శూన్య lv_సమూహం_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_సమూహం_t * g = lv_obj_get_సమూహం(obj);
    ఒకవేళ(g == ఖాళీ) తిరిగి;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ఒకవేళ(g->obj_focus && *g->obj_focus == obj) {
        ఒకవేళ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ఒకవేళ(lv_ll_get_తల(&g->obj_ll) != g->obj_focus || lv_ll_get_తోక(&g->obj_ll) != g->obj_focus) {
            lv_సమూహం_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ఒకవేళ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ఖాళీ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ఒకవేళ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ఒకవేళ(obj->spec_attr) obj->spec_attr->సమూహం_p = ఖాళీ;
            ఆపు;
        }
    }
    LV_LOG_TRACE("finished");
}

శూన్య lv_సమూహం_remove_all_objs(lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    /*Defocus the currently focused object*/
    ఒకవేళ(సమూహం->obj_focus != ఖాళీ) {
        lv_obj_send_event(*సమూహం->obj_focus, LV_EVENT_DEFOCUSED, get_indev(సమూహం));
        lv_obj_invalidate(*సమూహం->obj_focus);
        సమూహం->obj_focus = ఖాళీ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&సమూహం->obj_ll, obj) {
        ఒకవేళ((*obj)->spec_attr)(*obj)->spec_attr->సమూహం_p = ఖాళీ;
    }

    lv_ll_clear(&(సమూహం->obj_ll));
}

శూన్య lv_సమూహం_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_సమూహం_t * g = lv_obj_get_సమూహం(obj);

    ఒకవేళ(g == ఖాళీ) తిరిగి;
    ఒకవేళ(g->frozen) తిరిగి;

    /*On defocus edit mode must be leaved*/
    lv_సమూహం_సమితి_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ఒకవేళ(*i == obj) {
            ఒకవేళ(g->obj_focus != ఖాళీ) {
                ఒకవేళ(*g->obj_focus == obj) ఆపు;  /*Refocusing same object*/
                lv_ఫలితం_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ఒకవేళ(g->focus_cb) g->focus_cb(g);
            lv_ఫలితం_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
            lv_obj_invalidate(*g->obj_focus);
            ఆపు;
        }
    }
}

శూన్య lv_సమూహం_focus_తదుపరి(lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    bool focus_మారింది = focus_తదుపరి_core(సమూహం, lv_ll_get_తల, lv_ll_get_తదుపరి);

    ఒకవేళ(!focus_మారింది && సమూహం->అంచు_cb) {
        సమూహం->అంచు_cb(సమూహం, true);
    }
}

శూన్య lv_సమూహం_focus_prev(lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    bool focus_మారింది = focus_తదుపరి_core(సమూహం, lv_ll_get_తోక, lv_ll_get_prev);

    ఒకవేళ(!focus_మారింది && సమూహం->అంచు_cb) {
        సమూహం->అంచు_cb(సమూహం, false);
    }
}

శూన్య lv_సమూహం_focus_freeze(lv_సమూహం_t * సమూహం, bool en)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    ఒకవేళ(en == false) సమూహం->frozen = 0;
    లేకపోతే సమూహం->frozen = 1;
}

lv_ఫలితం_t lv_సమూహం_send_దత్తాంశం(lv_సమూహం_t * సమూహం, uint32_t c)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);

    lv_obj_t * act = lv_సమూహం_get_focused(సమూహం);
    ఒకవేళ(act == ఖాళీ) తిరిగి LV_ఫలితం_OK;

    ఒకవేళ(lv_obj_has_state(act, LV_STATE_DISABLED)) తిరిగి LV_ఫలితం_OK;

    తిరిగి lv_obj_send_event(act, LV_EVENT_కీలకం, &c);
}

శూన్య lv_సమూహం_సమితి_focus_cb(lv_సమూహం_t * సమూహం, lv_సమూహం_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    సమూహం->focus_cb = focus_cb;
}

శూన్య lv_సమూహం_సమితి_అంచు_cb(lv_సమూహం_t * సమూహం, lv_సమూహం_అంచు_cb_t అంచు_cb)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    సమూహం->అంచు_cb = అంచు_cb;
}

శూన్య lv_సమూహం_సమితి_editing(lv_సమూహం_t * సమూహం, bool edit)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);
    uint8_t en_val = edit ? 1 : 0;

    ఒకవేళ(en_val == సమూహం->editing) తిరిగి; /*Do not set the same mode again*/

    సమూహం->editing     = en_val;
    lv_obj_t * focused = lv_సమూహం_get_focused(సమూహం);

    ఒకవేళ(focused) {
        lv_ఫలితం_t res = lv_obj_send_event(*సమూహం->obj_focus, LV_EVENT_FOCUSED, get_indev(సమూహం));
        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

        lv_obj_invalidate(focused);
    }
}

శూన్య lv_సమూహం_సమితి_refocus_policy(lv_సమూహం_t * సమూహం, lv_సమూహం_refocus_policy_t policy)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);
    సమూహం->refocus_policy = policy & 0x01;
}

శూన్య lv_సమూహం_సమితి_wrap(lv_సమూహం_t * సమూహం, bool en)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);
    సమూహం->wrap = en ? 1 : 0;
}

lv_obj_t * lv_సమూహం_get_focused(స్థిరరాశి lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి ఖాళీ);
    ఒకవేళ(సమూహం->obj_focus == ఖాళీ) తిరిగి ఖాళీ;

    తిరిగి *సమూహం->obj_focus;
}

lv_సమూహం_focus_cb_t lv_సమూహం_get_focus_cb(స్థిరరాశి lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి సమూహం->focus_cb;
}

lv_సమూహం_అంచు_cb_t lv_సమూహం_get_అంచు_cb(స్థిరరాశి lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి సమూహం->అంచు_cb;
}

bool lv_సమూహం_get_editing(స్థిరరాశి lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి false);
    తిరిగి సమూహం->editing;
}

bool lv_సమూహం_get_wrap(lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి false);
    తిరిగి సమూహం->wrap;
}

uint32_t lv_సమూహం_get_obj_లెక్క(lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి 0);
    తిరిగి lv_ll_get_len(&సమూహం->obj_ll);
}

lv_obj_t * lv_సమూహం_get_obj_by_సూచీ(lv_సమూహం_t * సమూహం, uint32_t సూచీ)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి ఖాళీ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&సమూహం->obj_ll, obj) {
        ఒకవేళ(len == సూచీ) {
            తిరిగి *obj;
        }
        len++;
    }
    తిరిగి ఖాళీ;
}

uint32_t lv_సమూహం_get_లెక్క(శూన్య)
{
    తిరిగి lv_ll_get_len(సమూహం_ll_p);
}

lv_సమూహం_t  * lv_సమూహం_by_సూచీ(uint32_t సూచీ)
{
    uint32_t len = 0;
    lv_సమూహం_t * సమూహం;

    LV_LL_READ_వెనుక(సమూహం_ll_p, సమూహం) {
        ఒకవేళ(len == సూచీ) {
            తిరిగి సమూహం;
        }
        len++;
    }

    తిరిగి ఖాళీ;
}

#ఒకవేళ LV_USE_EXT_దత్తాంశం
శూన్య lv_సమూహం_సమితి_external_దత్తాంశం(lv_సమూహం_t * సమూహం, శూన్య * దత్తాంశం, శూన్య (* free_cb)(శూన్య * దత్తాంశం))
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);

    సమూహం->ext_దత్తాంశం.దత్తాంశం = దత్తాంశం;
    సమూహం->ext_దత్తాంశం.free_cb = free_cb;
}
#అయితే_ముగింపు

శూన్య lv_సమూహం_సమితి_user_దత్తాంశం(lv_సమూహం_t * సమూహం, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి);
    సమూహం->user_దత్తాంశం = user_దత్తాంశం;
}

శూన్య * lv_సమూహం_get_user_దత్తాంశం(స్థిరరాశి lv_సమూహం_t * సమూహం)
{
    LV_CHECK_ARG(సమూహం != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి సమూహం->user_దత్తాంశం;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర శూన్య lv_సమూహం_refocus(lv_సమూహం_t * g)
{
    LV_ASSERT(g != ఖాళీ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t తాత్కాలిక_wrap = g->wrap;
    g->wrap           = 1;

    ఒకవేళ(g->refocus_policy == LV_సమూహం_REFOCUS_POLICY_తదుపరి)
        lv_సమూహం_focus_తదుపరి(g);
    లేకపోతే ఒకవేళ(g->refocus_policy == LV_సమూహం_REFOCUS_POLICY_PREV)
        lv_సమూహం_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = తాత్కాలిక_wrap;
}

స్థిర bool focus_తదుపరి_core(lv_సమూహం_t * సమూహం, శూన్య * (*begin)(స్థిరరాశి lv_ll_t *),
                            శూన్య * (*move)(స్థిరరాశి lv_ll_t *, స్థిరరాశి శూన్య *))
{
    LV_ASSERT(సమూహం != ఖాళీ);
    LV_ASSERT(begin != ఖాళీ);
    LV_ASSERT(move != ఖాళీ);
    ఒకవేళ(సమూహం->frozen) తిరిగి false;

    bool focus_మారింది = false;

    lv_obj_t ** obj_తదుపరి     = సమూహం->obj_focus;
    lv_obj_t ** obj_sentinel = ఖాళీ;
    bool can_move            = true;
    bool can_begin           = true;

    కొరకు(;;) {
        ఒకవేళ(obj_తదుపరి == ఖాళీ) {
            ఒకవేళ(సమూహం->wrap || obj_sentinel == ఖాళీ) {
                ఒకవేళ(!can_begin) తిరిగి focus_మారింది;
                obj_తదుపరి  = begin(&సమూహం->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            లేకపోతే {
                /*Currently focused object is the last/first in the group, keep it that way*/
                తిరిగి focus_మారింది;
            }
        }

        ఒకవేళ(obj_sentinel == ఖాళీ) {
            obj_sentinel = obj_తదుపరి;
            ఒకవేళ(obj_sentinel == ఖాళీ) తిరిగి focus_మారింది; /*Group is empty*/
        }

        ఒకవేళ(can_move) {
            obj_తదుపరి = move(&సమూహం->obj_ll, obj_తదుపరి);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ఒకవేళ(obj_తదుపరి == obj_sentinel) తిరిగి focus_మారింది;
        }

        can_move = true;

        ఒకవేళ(obj_తదుపరి == ఖాళీ) కొనసాగు;
        ఒకవేళ(lv_obj_get_state(*obj_తదుపరి) & LV_STATE_DISABLED) కొనసాగు;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * మాతృ = *obj_తదుపరి;
        అయితే(మాతృ) {
            ఒకవేళ(lv_obj_is_hidden(మాతృ)) ఆపు;
            మాతృ = lv_obj_get_మాతృ(మాతృ);
        }

        ఒకవేళ(మాతృ && lv_obj_is_hidden(మాతృ)) కొనసాగు;

        /*If we got her a good candidate is found*/
        ఆపు;
    }

    ఒకవేళ(obj_తదుపరి == సమూహం->obj_focus) తిరిగి focus_మారింది; /*There's only one visible object and it's already focused*/

    ఒకవేళ(సమూహం->obj_focus) {
        lv_ఫలితం_t res = lv_obj_send_event(*సమూహం->obj_focus, LV_EVENT_DEFOCUSED, get_indev(సమూహం));
        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి focus_మారింది;
        lv_obj_invalidate(*సమూహం->obj_focus);
    }

    సమూహం->obj_focus = obj_తదుపరి;

    lv_ఫలితం_t res = lv_obj_send_event(*సమూహం->obj_focus, LV_EVENT_FOCUSED, get_indev(సమూహం));
    ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి focus_మారింది;

    lv_obj_invalidate(*సమూహం->obj_focus);

    ఒకవేళ(సమూహం->focus_cb) సమూహం->focus_cb(సమూహం);
    focus_మారింది = true;
    తిరిగి focus_మారింది;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
స్థిర lv_indev_t * get_indev(స్థిరరాశి lv_సమూహం_t * g)
{
    LV_ASSERT(g != ఖాళీ);
    lv_indev_t * indev_guess = ఖాళీ;
    lv_indev_t * indev = lv_indev_get_తదుపరి(ఖాళీ);

    అయితే(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ఒకవేళ(indev_type == LV_INDEV_TYPE_సూచిక) తిరిగి indev;
        ఒకవేళ(lv_indev_get_సమూహం(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_తదుపరి(indev);
    }

    తిరిగి indev_guess;
}
