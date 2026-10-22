/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "../misc/lv_event_private.h"
#شامل "../lvgl_public.h"
#شامل "lv_obj_event_private.h"
#شامل "lv_obj_class_private.h"
#شامل "lv_obj_private.h"
#شامل "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساکن lv_نتیجہ_t event_send_core(lv_event_t * e);
ساکن bool event_is_bubbled(lv_event_t * e);
ساکن bool event_is_trickled(lv_event_t * e);
#اگر LV_USE_CHECK_ARG
    ساکن bool event_رمز_اندر_array(lv_event_رمز_t رمز, ثابت lv_event_رمز_t * arr, uint32_t len);
#اگر_ختم

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#اگر LV_USE_LOG && LV_LOG_TRACE_EVENT
    #تعریف_کرو LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ورنہ
    #تعریف_کرو LV_TRACE_EVENT(...)
#اگر_ختم

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_نتیجہ_t lv_obj_send_event(lv_obj_t * obj, lv_event_رمز_t event_رمز, خالی * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    lv_event_t e;
    e.current_ہدف = obj;
    e.original_ہدف = obj;
    e.رمز = event_رمز;
    e.user_ڈیٹا = تہی;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ڈالو(&e);

    /*Send the event*/
    lv_نتیجہ_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_نکالو(&e);

    واپس res;
}

lv_نتیجہ_t lv_obj_event_بنیاد(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس LV_نتیجہ_INVALID);
    LV_CHECK_ARG(e->current_ہدف != تہی, واپس LV_نتیجہ_INVALID);

    ثابت lv_obj_class_t * بنیاد;
    اگر(class_p == تہی) بنیاد = ((lv_obj_t *)e->current_ہدف)->class_p;
    ورنہ بنیاد = class_p->بنیاد_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    جب(بنیاد && بنیاد->event_cb == تہی) بنیاد = بنیاد->بنیاد_class;

    اگر(بنیاد == تہی) واپس LV_نتیجہ_OK;
    اگر(بنیاد->event_cb == تہی) واپس LV_نتیجہ_OK;

    /*Call the actual event callback*/
    e->user_ڈیٹا = تہی;
    LV_PROFILER_EVENT_BEGIN_لیبل(lv_event_رمز_get_name(e->رمز));
    بنیاد->event_cb(بنیاد, e);
    LV_PROFILER_EVENT_انتہا_لیبل(lv_event_رمز_get_name(e->رمز));

    lv_نتیجہ_t res = LV_نتیجہ_OK;
    /*Stop if the object is deleted*/
    اگر(e->deleted) res = LV_نتیجہ_INVALID;

    واپس res;
}

lv_event_dsc_t * lv_obj_جوڑو_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_رمز_t filter, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(event_cb != تہی, واپس تہی);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس تہی;
    }

    واپس lv_event_جوڑو(&obj->spec_attr->event_list, event_cb, filter, user_ڈیٹا);
}

uint32_t lv_obj_get_event_گنتی(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس 0);
    اگر(obj->spec_attr == تہی) واپس 0;
    واپس lv_event_get_گنتی(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t اشاریہ)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    اگر(obj->spec_attr == تہی) واپس تہی;
    واپس lv_event_get_dsc(&obj->spec_attr->event_list, اشاریہ);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t اشاریہ)
{
    LV_CHECK_ARG(obj != تہی, واپس false);
    اگر(obj->spec_attr == تہی) واپس false;
    واپس lv_event_remove(&obj->spec_attr->event_list, اشاریہ);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != تہی, واپس false);
    LV_CHECK_ARG(dsc != تہی, واپس false);

    اگر(obj->spec_attr == تہی) واپس false;
    واپس lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس 0);

    uint32_t event_cnt = lv_obj_get_event_گنتی(obj);
    uint32_t removed_گنتی = 0;
    int32_t i;

    اگر(event_cnt == 0) واپس 0;

    برائے(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        اگر(dsc && (event_cb == تہی || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_گنتی++;
        }
    }

    واپس removed_گنتی;
}

uint32_t lv_obj_remove_event_cb_with_user_ڈیٹا(lv_obj_t * obj, lv_event_cb_t event_cb, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(obj != تہی, واپس 0);

    uint32_t event_cnt = lv_obj_get_event_گنتی(obj);
    uint32_t removed_گنتی = 0;
    int32_t i;

    اگر(event_cnt == 0) واپس 0;

    برائے(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        اگر(dsc && (event_cb == تہی || dsc->cb == event_cb) && dsc->user_ڈیٹا == user_ڈیٹا) {
            lv_obj_remove_event(obj, i);
            removed_گنتی ++;
        }
    }

    واپس removed_گنتی;
}

lv_obj_t * lv_event_get_current_ہدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    واپس lv_event_get_current_ہدف(e);
}

lv_obj_t * lv_event_get_ہدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    واپس lv_event_get_ہدف(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
#اگر LV_USE_CHECK_ARG
    ساکن ثابت lv_event_رمز_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_گمشدہ,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_انتہا,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_اوپر,        LV_EVENT_GESTURE_نیچے_کو,
        LV_EVENT_GESTURE_بایاں,      LV_EVENT_GESTURE_دایاں,
        LV_EVENT_کلید,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#اگر_ختم
    LV_CHECK_ARG_FORMAT_پیغام(
        event_رمز_اندر_array(e->رمز, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        واپس تہی,
        "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
#اگر LV_USE_CHECK_ARG
    ساکن ثابت lv_event_رمز_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_انتہا,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_انتہا,
    };
#اگر_ختم
    LV_CHECK_ARG_FORMAT_پیغام(
        event_رمز_اندر_array(e->رمز, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        واپس تہی,
        "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_سائز(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_سائز_بدلا,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

uint32_t lv_event_get_کلید(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_کلید,
                            واپس 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    uint32_t * k = lv_event_get_param(e);
    واپس k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_ROTARY,
                            واپس 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    int32_t * r = lv_event_get_param(e);
    واپس r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_SCROLL_BEGIN,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

خالی lv_event_سیٹ_ext_draw_سائز(lv_event_t * e, int32_t سائز)
{
    LV_CHECK_ARG(e != تہی, واپس);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_REFR_EXT_DRAW_سائز,
                            واپس,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    int32_t * cur_سائز = lv_event_get_param(e);
    *cur_سائز = LV_MAX(*cur_سائز, سائز);
}

lv_نقطہ_t * lv_event_get_self_سائز_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_GET_SELF_سائز,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

lv_کامیابی_test_info_t * lv_event_get_کامیابی_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_کامیابی_TEST,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_COVER_CHECK,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    واپس p->area;
}

خالی lv_event_سیٹ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != تہی, واپس);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_COVER_CHECK,
                            واپس,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    اگر(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_DRAW_TASK_ADDED,
                            واپس تہی,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    واپس lv_event_get_param(e);
}

lv_state_t lv_event_get_پچھلا_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != تہی, واپس 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->رمز == LV_EVENT_STATE_بدلا,
                            واپس 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->رمز);
    lv_state_t * state = lv_event_get_param(e);
    واپس state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن lv_نتیجہ_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->رمز, (خالی *)e->original_ہدف, e->param);

    lv_indev_t * indev_act = lv_indev_فعال();
    اگر(indev_act) {
        اگر(e->stop_processing) واپس LV_نتیجہ_OK;
        اگر(e->deleted) واپس LV_نتیجہ_INVALID;
    }

    lv_obj_t * ہدف = e->current_ہدف;
    lv_نتیجہ_t res = LV_نتیجہ_OK;
    lv_event_list_t * list = ہدف->spec_attr ?  &ہدف->spec_attr->event_list : تہی;

    res = lv_event_send(list, e, true);
    اگر(res != LV_نتیجہ_OK || e->stop_processing) واپس res;

    res = lv_obj_event_بنیاد(تہی, e);
    اگر(res != LV_نتیجہ_OK || e->stop_processing) واپس res;

    res = lv_event_send(list, e, false);
    اگر(res != LV_نتیجہ_OK || e->stop_processing) واپس res;

    lv_obj_t * والد = lv_obj_get_والد(e->current_ہدف);
    اگر(والد && event_is_bubbled(e)) {
        e->current_ہدف = والد;
        res = event_send_core(e);
    }
    اگر(res != LV_نتیجہ_OK) واپس res;

    /*Trickle down to children if enabled*/
    اگر(event_is_trickled(e)) {
        uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(ہدف);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        برائے(uint32_t i = 0; i < بچہ_گنتی && res == LV_نتیجہ_OK && !e->stop_processing; i++) {
            lv_obj_t * بچہ = lv_obj_get_بچہ(ہدف, i);
            اگر(بچہ) {
                e->current_ہدف = بچہ;
                res = event_send_core(e);
                اگر(res != LV_نتیجہ_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->رمز, (خالی *)بچہ);
                    توڑو;
                }
            }
        }
    }

    واپس res;
}

ساکن bool event_is_bubbled(lv_event_t * e)
{
    اگر(e->stop_bubbling) واپس false;

    /*Event codes that always bubble*/
    انتخاب(e->رمز) {
        حالت LV_EVENT_بچہ_CREATED:
        حالت LV_EVENT_بچہ_DELETED:
            واپس true;
        طےشدہ:
            توڑو;
    }

    /*Check other codes only if bubbling is enabled*/
    اگر(lv_obj_is_event_bubble(e->current_ہدف) == false) واپس false;

    انتخاب(e->رمز) {
        حالت LV_EVENT_کامیابی_TEST:
        حالت LV_EVENT_COVER_CHECK:
        حالت LV_EVENT_REFR_EXT_DRAW_سائز:
        حالت LV_EVENT_DRAW_MAIN_BEGIN:
        حالت LV_EVENT_DRAW_MAIN:
        حالت LV_EVENT_DRAW_MAIN_انتہا:
        حالت LV_EVENT_DRAW_POST_BEGIN:
        حالت LV_EVENT_DRAW_POST:
        حالت LV_EVENT_DRAW_POST_انتہا:
        حالت LV_EVENT_DRAW_TASK_ADDED:
        حالت LV_EVENT_REFRESH:
        حالت LV_EVENT_DELETE:
        حالت LV_EVENT_بچہ_CREATED:
        حالت LV_EVENT_بچہ_DELETED:
        حالت LV_EVENT_بچہ_بدلا:
        حالت LV_EVENT_سائز_بدلا:
        حالت LV_EVENT_STYLE_بدلا:
        حالت LV_EVENT_GET_SELF_سائز:
            واپس false;
        طےشدہ:
            واپس true;
    }
}

ساکن bool event_is_trickled(lv_event_t * e)
{
    اگر(e->stop_trickling) واپس false;

    /*Check other codes only if trickle is enabled*/
    اگر(lv_obj_is_event_trickle(e->current_ہدف) == false) واپس false;

    انتخاب(e->رمز) {
        حالت LV_EVENT_کامیابی_TEST:
        حالت LV_EVENT_COVER_CHECK:
        حالت LV_EVENT_REFR_EXT_DRAW_سائز:
        حالت LV_EVENT_DRAW_MAIN_BEGIN:
        حالت LV_EVENT_DRAW_MAIN:
        حالت LV_EVENT_DRAW_MAIN_انتہا:
        حالت LV_EVENT_DRAW_POST_BEGIN:
        حالت LV_EVENT_DRAW_POST:
        حالت LV_EVENT_DRAW_POST_انتہا:
        حالت LV_EVENT_DRAW_TASK_ADDED:
        حالت LV_EVENT_REFRESH:
        حالت LV_EVENT_DELETE:
        حالت LV_EVENT_بچہ_CREATED:
        حالت LV_EVENT_بچہ_DELETED:
        حالت LV_EVENT_بچہ_بدلا:
        حالت LV_EVENT_سائز_بدلا:
        حالت LV_EVENT_STYLE_بدلا:
        حالت LV_EVENT_GET_SELF_سائز:
            واپس false;
        طےشدہ:
            واپس true;
    }
}

#اگر LV_USE_CHECK_ARG
ساکن bool event_رمز_اندر_array(lv_event_رمز_t رمز, ثابت lv_event_رمز_t * arr, uint32_t len)
{
    uint32_t i;
    برائے(i = 0; i < len; i++) {
        اگر(رمز == arr[i]) واپس true;
    }
    واپس false;
}
#اگر_ختم
