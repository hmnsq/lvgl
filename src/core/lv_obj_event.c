/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "../misc/lv_event_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_event_private.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర lv_ఫలితం_t event_send_core(lv_event_t * e);
స్థిర bool event_is_bubbled(lv_event_t * e);
స్థిర bool event_is_trickled(lv_event_t * e);
#ఒకవేళ LV_USE_CHECK_ARG
    స్థిర bool event_సంకేతం_లోపల_array(lv_event_సంకేతం_t సంకేతం_2, స్థిరరాశి lv_event_సంకేతం_t * arr, uint32_t len);
#అయితే_ముగింపు

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ఒకవేళ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #నిర్వచించు LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#లేకపోతే
    #నిర్వచించు LV_TRACE_EVENT(...)
#అయితే_ముగింపు

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ఫలితం_t lv_obj_send_event(lv_obj_t * obj, lv_event_సంకేతం_t event_సంకేతం, శూన్య * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    lv_event_t e;
    e.current_లక్ష్యం = obj;
    e.original_లక్ష్యం = obj;
    e.సంకేతం_2 = event_సంకేతం;
    e.user_దత్తాంశం = ఖాళీ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_నెట్టు(&e);

    /*Send the event*/
    lv_ఫలితం_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_తీయి(&e);

    తిరిగి res;
}

lv_ఫలితం_t lv_obj_event_పునాది(స్థిరరాశి lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);
    LV_CHECK_ARG(e->current_లక్ష్యం != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);

    స్థిరరాశి lv_obj_class_t * పునాది;
    ఒకవేళ(class_p == ఖాళీ) పునాది = ((lv_obj_t *)e->current_లక్ష్యం)->class_p;
    లేకపోతే పునాది = class_p->పునాది_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    అయితే(పునాది && పునాది->event_cb == ఖాళీ) పునాది = పునాది->పునాది_class;

    ఒకవేళ(పునాది == ఖాళీ) తిరిగి LV_ఫలితం_OK;
    ఒకవేళ(పునాది->event_cb == ఖాళీ) తిరిగి LV_ఫలితం_OK;

    /*Call the actual event callback*/
    e->user_దత్తాంశం = ఖాళీ;
    LV_PROFILER_EVENT_BEGIN_ట్యాగ్(lv_event_సంకేతం_get_name(e->సంకేతం_2));
    పునాది->event_cb(పునాది, e);
    LV_PROFILER_EVENT_చివర_ట్యాగ్(lv_event_సంకేతం_get_name(e->సంకేతం_2));

    lv_ఫలితం_t res = LV_ఫలితం_OK;
    /*Stop if the object is deleted*/
    ఒకవేళ(e->deleted) res = LV_ఫలితం_INVALID;

    తిరిగి res;
}

lv_event_dsc_t * lv_obj_కూడు_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_సంకేతం_t filter, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(event_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి ఖాళీ;
    }

    తిరిగి lv_event_కూడు(&obj->spec_attr->event_list, event_cb, filter, user_దత్తాంశం);
}

uint32_t lv_obj_get_event_లెక్క(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి 0);
    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
    తిరిగి lv_event_get_లెక్క(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t సూచీ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి ఖాళీ;
    తిరిగి lv_event_get_dsc(&obj->spec_attr->event_list, సూచీ);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t సూచీ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి false;
    తిరిగి lv_event_remove(&obj->spec_attr->event_list, సూచీ);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(dsc != ఖాళీ, తిరిగి false);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి false;
    తిరిగి lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి 0);

    uint32_t event_cnt = lv_obj_get_event_లెక్క(obj);
    uint32_t removed_లెక్క = 0;
    int32_t i;

    ఒకవేళ(event_cnt == 0) తిరిగి 0;

    కొరకు(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ఒకవేళ(dsc && (event_cb == ఖాళీ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_లెక్క++;
        }
    }

    తిరిగి removed_లెక్క;
}

uint32_t lv_obj_remove_event_cb_with_user_దత్తాంశం(lv_obj_t * obj, lv_event_cb_t event_cb, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి 0);

    uint32_t event_cnt = lv_obj_get_event_లెక్క(obj);
    uint32_t removed_లెక్క = 0;
    int32_t i;

    ఒకవేళ(event_cnt == 0) తిరిగి 0;

    కొరకు(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ఒకవేళ(dsc && (event_cb == ఖాళీ || dsc->cb == event_cb) && dsc->user_దత్తాంశం == user_దత్తాంశం) {
            lv_obj_remove_event(obj, i);
            removed_లెక్క ++;
        }
    }

    తిరిగి removed_లెక్క;
}

lv_obj_t * lv_event_get_current_లక్ష్యం_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి lv_event_get_current_లక్ష్యం(e);
}

lv_obj_t * lv_event_get_లక్ష్యం_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి lv_event_get_లక్ష్యం(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
#ఒకవేళ LV_USE_CHECK_ARG
    స్థిర స్థిరరాశి lv_event_సంకేతం_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_కోల్పోయిన,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_చివర,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_పైకి,        LV_EVENT_GESTURE_కిందికి,
        LV_EVENT_GESTURE_ఎడమ,      LV_EVENT_GESTURE_కుడి,
        LV_EVENT_కీలకం,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#అయితే_ముగింపు
    LV_CHECK_ARG_FORMAT_సందేశం(
        event_సంకేతం_లోపల_array(e->సంకేతం_2, indev_codes, పరిమాణం(indev_codes) / పరిమాణం(indev_codes[0])),
        తిరిగి ఖాళీ,
        "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
#ఒకవేళ LV_USE_CHECK_ARG
    స్థిర స్థిరరాశి lv_event_సంకేతం_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_చివర,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_చివర,
    };
#అయితే_ముగింపు
    LV_CHECK_ARG_FORMAT_సందేశం(
        event_సంకేతం_లోపల_array(e->సంకేతం_2, draw_codes, పరిమాణం(draw_codes) / పరిమాణం(draw_codes[0])),
        తిరిగి ఖాళీ,
        "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

స్థిరరాశి lv_area_t * lv_event_get_old_కొలత(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_కొలత_మారింది,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

uint32_t lv_event_get_కీలకం(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి 0);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_కీలకం,
                            తిరిగి 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    uint32_t * k = lv_event_get_param(e);
    తిరిగి k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి 0);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_ROTARY,
                            తిరిగి 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    int32_t * r = lv_event_get_param(e);
    తిరిగి r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_SCROLL_BEGIN,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

శూన్య lv_event_సమితి_ext_draw_కొలత(lv_event_t * e, int32_t కొలత)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_REFR_EXT_DRAW_కొలత,
                            తిరిగి,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    int32_t * cur_కొలత = lv_event_get_param(e);
    *cur_కొలత = LV_MAX(*cur_కొలత, కొలత);
}

lv_బిందువు_t * lv_event_get_self_కొలత_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_GET_SELF_కొలత,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

lv_తగిలింది_test_info_t * lv_event_get_తగిలింది_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_తగిలింది_TEST,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

స్థిరరాశి lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_COVER_CHECK,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    తిరిగి p->area;
}

శూన్య lv_event_సమితి_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_COVER_CHECK,
                            తిరిగి,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ఒకవేళ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_DRAW_TASK_ADDED,
                            తిరిగి ఖాళీ,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    తిరిగి lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ఖాళీ, తిరిగి 0);
    LV_CHECK_ARG_FORMAT_సందేశం(e->సంకేతం_2 == LV_EVENT_STATE_మారింది,
                            తిరిగి 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->సంకేతం_2);
    lv_state_t * state = lv_event_get_param(e);
    తిరిగి state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర lv_ఫలితం_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->సంకేతం_2, (శూన్య *)e->original_లక్ష్యం, e->param);

    lv_indev_t * indev_act = lv_indev_చురుకైన();
    ఒకవేళ(indev_act) {
        ఒకవేళ(e->stop_processing) తిరిగి LV_ఫలితం_OK;
        ఒకవేళ(e->deleted) తిరిగి LV_ఫలితం_INVALID;
    }

    lv_obj_t * లక్ష్యం = e->current_లక్ష్యం;
    lv_ఫలితం_t res = LV_ఫలితం_OK;
    lv_event_list_t * list = లక్ష్యం->spec_attr ?  &లక్ష్యం->spec_attr->event_list : ఖాళీ;

    res = lv_event_send(list, e, true);
    ఒకవేళ(res != LV_ఫలితం_OK || e->stop_processing) తిరిగి res;

    res = lv_obj_event_పునాది(ఖాళీ, e);
    ఒకవేళ(res != LV_ఫలితం_OK || e->stop_processing) తిరిగి res;

    res = lv_event_send(list, e, false);
    ఒకవేళ(res != LV_ఫలితం_OK || e->stop_processing) తిరిగి res;

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(e->current_లక్ష్యం);
    ఒకవేళ(మాతృ && event_is_bubbled(e)) {
        e->current_లక్ష్యం = మాతృ;
        res = event_send_core(e);
    }
    ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి res;

    /*Trickle down to children if enabled*/
    ఒకవేళ(event_is_trickled(e)) {
        uint32_t బిడ్డ_లెక్క = lv_obj_get_బిడ్డ_లెక్క(లక్ష్యం);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        కొరకు(uint32_t i = 0; i < బిడ్డ_లెక్క && res == LV_ఫలితం_OK && !e->stop_processing; i++) {
            lv_obj_t * బిడ్డ = lv_obj_get_బిడ్డ(లక్ష్యం, i);
            ఒకవేళ(బిడ్డ) {
                e->current_లక్ష్యం = బిడ్డ;
                res = event_send_core(e);
                ఒకవేళ(res != LV_ఫలితం_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->సంకేతం_2, (శూన్య *)బిడ్డ);
                    ఆపు;
                }
            }
        }
    }

    తిరిగి res;
}

స్థిర bool event_is_bubbled(lv_event_t * e)
{
    ఒకవేళ(e->stop_bubbling) తిరిగి false;

    /*Event codes that always bubble*/
    ఎంపిక(e->సంకేతం_2) {
        స్థితి LV_EVENT_బిడ్డ_CREATED:
        స్థితి LV_EVENT_బిడ్డ_DELETED:
            తిరిగి true;
        అప్రమేయ:
            ఆపు;
    }

    /*Check other codes only if bubbling is enabled*/
    ఒకవేళ(lv_obj_is_event_bubble(e->current_లక్ష్యం) == false) తిరిగి false;

    ఎంపిక(e->సంకేతం_2) {
        స్థితి LV_EVENT_తగిలింది_TEST:
        స్థితి LV_EVENT_COVER_CHECK:
        స్థితి LV_EVENT_REFR_EXT_DRAW_కొలత:
        స్థితి LV_EVENT_DRAW_MAIN_BEGIN:
        స్థితి LV_EVENT_DRAW_MAIN:
        స్థితి LV_EVENT_DRAW_MAIN_చివర:
        స్థితి LV_EVENT_DRAW_POST_BEGIN:
        స్థితి LV_EVENT_DRAW_POST:
        స్థితి LV_EVENT_DRAW_POST_చివర:
        స్థితి LV_EVENT_DRAW_TASK_ADDED:
        స్థితి LV_EVENT_REFRESH:
        స్థితి LV_EVENT_DELETE:
        స్థితి LV_EVENT_బిడ్డ_CREATED:
        స్థితి LV_EVENT_బిడ్డ_DELETED:
        స్థితి LV_EVENT_బిడ్డ_మారింది:
        స్థితి LV_EVENT_కొలత_మారింది:
        స్థితి LV_EVENT_STYLE_మారింది:
        స్థితి LV_EVENT_GET_SELF_కొలత:
            తిరిగి false;
        అప్రమేయ:
            తిరిగి true;
    }
}

స్థిర bool event_is_trickled(lv_event_t * e)
{
    ఒకవేళ(e->stop_trickling) తిరిగి false;

    /*Check other codes only if trickle is enabled*/
    ఒకవేళ(lv_obj_is_event_trickle(e->current_లక్ష్యం) == false) తిరిగి false;

    ఎంపిక(e->సంకేతం_2) {
        స్థితి LV_EVENT_తగిలింది_TEST:
        స్థితి LV_EVENT_COVER_CHECK:
        స్థితి LV_EVENT_REFR_EXT_DRAW_కొలత:
        స్థితి LV_EVENT_DRAW_MAIN_BEGIN:
        స్థితి LV_EVENT_DRAW_MAIN:
        స్థితి LV_EVENT_DRAW_MAIN_చివర:
        స్థితి LV_EVENT_DRAW_POST_BEGIN:
        స్థితి LV_EVENT_DRAW_POST:
        స్థితి LV_EVENT_DRAW_POST_చివర:
        స్థితి LV_EVENT_DRAW_TASK_ADDED:
        స్థితి LV_EVENT_REFRESH:
        స్థితి LV_EVENT_DELETE:
        స్థితి LV_EVENT_బిడ్డ_CREATED:
        స్థితి LV_EVENT_బిడ్డ_DELETED:
        స్థితి LV_EVENT_బిడ్డ_మారింది:
        స్థితి LV_EVENT_కొలత_మారింది:
        స్థితి LV_EVENT_STYLE_మారింది:
        స్థితి LV_EVENT_GET_SELF_కొలత:
            తిరిగి false;
        అప్రమేయ:
            తిరిగి true;
    }
}

#ఒకవేళ LV_USE_CHECK_ARG
స్థిర bool event_సంకేతం_లోపల_array(lv_event_సంకేతం_t సంకేతం_2, స్థిరరాశి lv_event_సంకేతం_t * arr, uint32_t len)
{
    uint32_t i;
    కొరకు(i = 0; i < len; i++) {
        ఒకవేళ(సంకేతం_2 == arr[i]) తిరిగి true;
    }
    తిరిగి false;
}
#అయితే_ముగింపు
