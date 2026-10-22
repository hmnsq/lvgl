/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "../misc/lv_event_private.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_event_private.h"
#ضمن "lv_obj_class_private.h"
#ضمن "lv_obj_private.h"
#ضمن "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کن MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_نتیجه_t event_send_core(lv_event_t * e);
ساكن bool event_is_bubbled(lv_event_t * e);
ساكن bool event_is_trickled(lv_event_t * e);
#إذا LV_USE_CHECK_ARG
    ساكن bool event_کد_درون_array(lv_event_کد_t کد, ثابت lv_event_کد_t * arr, uint32_t len);
#پایان_اگر

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_EVENT
    #تعریف_کن LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #تعریف_کن LV_TRACE_EVENT(...)
#پایان_اگر

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_نتیجه_t lv_obj_send_event(lv_obj_t * obj, lv_event_کد_t event_کد, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    lv_event_t e;
    e.current_هدف = obj;
    e.original_هدف = obj;
    e.کد = event_کد;
    e.user_داده = عدم;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_افزودن(&e);

    /*Send the event*/
    lv_نتیجه_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_برداشت(&e);

    أرجع res;
}

lv_نتیجه_t lv_obj_event_پایه(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع LV_نتیجه_INVALID);
    LV_CHECK_ARG(e->current_هدف != عدم, أرجع LV_نتیجه_INVALID);

    ثابت lv_obj_class_t * پایه;
    إذا(class_p == عدم) پایه = ((lv_obj_t *)e->current_هدف)->class_p;
    وإلا پایه = class_p->پایه_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    طالما(پایه && پایه->event_cb == عدم) پایه = پایه->پایه_class;

    إذا(پایه == عدم) أرجع LV_نتیجه_OK;
    إذا(پایه->event_cb == عدم) أرجع LV_نتیجه_OK;

    /*Call the actual event callback*/
    e->user_داده = عدم;
    LV_PROFILER_EVENT_BEGIN_برچسب(lv_event_کد_get_name(e->کد));
    پایه->event_cb(پایه, e);
    LV_PROFILER_EVENT_انتها_برچسب(lv_event_کد_get_name(e->کد));

    lv_نتیجه_t res = LV_نتیجه_OK;
    /*Stop if the object is deleted*/
    إذا(e->deleted) res = LV_نتیجه_INVALID;

    أرجع res;
}

lv_event_dsc_t * lv_obj_جمع_کن_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_کد_t filter, فراغ * user_داده)
{
    LV_CHECK_ARG(event_cb != عدم, أرجع عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع عدم;
    }

    أرجع lv_event_جمع_کن(&obj->spec_attr->event_list, event_cb, filter, user_داده);
}

uint32_t lv_obj_get_event_شمار(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع lv_event_get_شمار(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t شاخص)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    إذا(obj->spec_attr == عدم) أرجع عدم;
    أرجع lv_event_get_dsc(&obj->spec_attr->event_list, شاخص);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t شاخص)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    إذا(obj->spec_attr == عدم) أرجع false;
    أرجع lv_event_remove(&obj->spec_attr->event_list, شاخص);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(dsc != عدم, أرجع false);

    إذا(obj->spec_attr == عدم) أرجع false;
    أرجع lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_شمار(obj);
    uint32_t removed_شمار = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_شمار++;
        }
    }

    أرجع removed_شمار;
}

uint32_t lv_obj_remove_event_cb_with_user_داده(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_داده)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_شمار(obj);
    uint32_t removed_شمار = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_داده == user_داده) {
            lv_obj_remove_event(obj, i);
            removed_شمار ++;
        }
    }

    أرجع removed_شمار;
}

lv_obj_t * lv_event_get_current_هدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_current_هدف(e);
}

lv_obj_t * lv_event_get_هدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_هدف(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کد_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_گمشده,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_انتها,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_به_بالا,        LV_EVENT_GESTURE_پایین,
        LV_EVENT_GESTURE_چپ,      LV_EVENT_GESTURE_راست,
        LV_EVENT_کلید,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#پایان_اگر
    LV_CHECK_ARG_FORMAT_پیام(
        event_کد_درون_array(e->کد, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کد_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_انتها,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_انتها,
    };
#پایان_اگر
    LV_CHECK_ARG_FORMAT_پیام(
        event_کد_درون_array(e->کد, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_اندازه(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_اندازه_تغییریافته,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

uint32_t lv_event_get_کلید(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_کلید,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    uint32_t * k = lv_event_get_param(e);
    أرجع k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_ROTARY,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    int32_t * r = lv_event_get_param(e);
    أرجع r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_SCROLL_BEGIN,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

فراغ lv_event_مجموعه_ext_draw_اندازه(lv_event_t * e, int32_t اندازه)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_REFR_EXT_DRAW_اندازه,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    int32_t * cur_اندازه = lv_event_get_param(e);
    *cur_اندازه = LV_MAX(*cur_اندازه, اندازه);
}

lv_نقطه_t * lv_event_get_self_اندازه_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_GET_SELF_اندازه,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

lv_اصابت_test_info_t * lv_event_get_اصابت_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_اصابت_TEST,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_COVER_CHECK,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    أرجع p->area;
}

فراغ lv_event_مجموعه_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_COVER_CHECK,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إذا(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_DRAW_TASK_ADDED,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    أرجع lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیام(e->کد == LV_EVENT_STATE_تغییریافته,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کد);
    lv_state_t * state = lv_event_get_param(e);
    أرجع state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_نتیجه_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->کد, (فراغ *)e->original_هدف, e->param);

    lv_indev_t * indev_act = lv_indev_فعال();
    إذا(indev_act) {
        إذا(e->stop_processing) أرجع LV_نتیجه_OK;
        إذا(e->deleted) أرجع LV_نتیجه_INVALID;
    }

    lv_obj_t * هدف = e->current_هدف;
    lv_نتیجه_t res = LV_نتیجه_OK;
    lv_event_list_t * list = هدف->spec_attr ?  &هدف->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    إذا(res != LV_نتیجه_OK || e->stop_processing) أرجع res;

    res = lv_obj_event_پایه(عدم, e);
    إذا(res != LV_نتیجه_OK || e->stop_processing) أرجع res;

    res = lv_event_send(list, e, false);
    إذا(res != LV_نتیجه_OK || e->stop_processing) أرجع res;

    lv_obj_t * والد = lv_obj_get_والد(e->current_هدف);
    إذا(والد && event_is_bubbled(e)) {
        e->current_هدف = والد;
        res = event_send_core(e);
    }
    إذا(res != LV_نتیجه_OK) أرجع res;

    /*Trickle down to children if enabled*/
    إذا(event_is_trickled(e)) {
        uint32_t فرزند_شمار = lv_obj_get_فرزند_شمار(هدف);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < فرزند_شمار && res == LV_نتیجه_OK && !e->stop_processing; i++) {
            lv_obj_t * فرزند = lv_obj_get_فرزند(هدف, i);
            إذا(فرزند) {
                e->current_هدف = فرزند;
                res = event_send_core(e);
                إذا(res != LV_نتیجه_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->کد, (فراغ *)فرزند);
                    اكسر;
                }
            }
        }
    }

    أرجع res;
}

ساكن bool event_is_bubbled(lv_event_t * e)
{
    إذا(e->stop_bubbling) أرجع false;

    /*Event codes that always bubble*/
    بدل(e->کد) {
        حالة LV_EVENT_فرزند_CREATED:
        حالة LV_EVENT_فرزند_DELETED:
            أرجع true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    إذا(lv_obj_is_event_bubble(e->current_هدف) == false) أرجع false;

    بدل(e->کد) {
        حالة LV_EVENT_اصابت_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_اندازه:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_انتها:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_انتها:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_فرزند_CREATED:
        حالة LV_EVENT_فرزند_DELETED:
        حالة LV_EVENT_فرزند_تغییریافته:
        حالة LV_EVENT_اندازه_تغییریافته:
        حالة LV_EVENT_STYLE_تغییریافته:
        حالة LV_EVENT_GET_SELF_اندازه:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

ساكن bool event_is_trickled(lv_event_t * e)
{
    إذا(e->stop_trickling) أرجع false;

    /*Check other codes only if trickle is enabled*/
    إذا(lv_obj_is_event_trickle(e->current_هدف) == false) أرجع false;

    بدل(e->کد) {
        حالة LV_EVENT_اصابت_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_اندازه:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_انتها:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_انتها:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_فرزند_CREATED:
        حالة LV_EVENT_فرزند_DELETED:
        حالة LV_EVENT_فرزند_تغییریافته:
        حالة LV_EVENT_اندازه_تغییریافته:
        حالة LV_EVENT_STYLE_تغییریافته:
        حالة LV_EVENT_GET_SELF_اندازه:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

#إذا LV_USE_CHECK_ARG
ساكن bool event_کد_درون_array(lv_event_کد_t کد, ثابت lv_event_کد_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        إذا(کد == arr[i]) أرجع true;
    }
    أرجع false;
}
#پایان_اگر
