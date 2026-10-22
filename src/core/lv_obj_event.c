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
#تعریف_کړه MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_پایله_t event_send_core(lv_event_t * e);
ساكن bool event_is_bubbled(lv_event_t * e);
ساكن bool event_is_trickled(lv_event_t * e);
#إذا LV_USE_CHECK_ARG
    ساكن bool event_کوډ_دننه_array(lv_event_کوډ_t کوډ, ثابت lv_event_کوډ_t * arr, uint32_t len);
#د_که_پای

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_EVENT
    #تعریف_کړه LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #تعریف_کړه LV_TRACE_EVENT(...)
#د_که_پای

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_پایله_t lv_obj_send_event(lv_obj_t * obj, lv_event_کوډ_t event_کوډ, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    lv_event_t e;
    e.current_موخه = obj;
    e.original_موخه = obj;
    e.کوډ = event_کوډ;
    e.user_معلومات = عدم;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ټېل(&e);

    /*Send the event*/
    lv_پایله_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_راوباسه(&e);

    أرجع res;
}

lv_پایله_t lv_obj_event_بنسټ(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع LV_پایله_INVALID);
    LV_CHECK_ARG(e->current_موخه != عدم, أرجع LV_پایله_INVALID);

    ثابت lv_obj_class_t * بنسټ;
    إذا(class_p == عدم) بنسټ = ((lv_obj_t *)e->current_موخه)->class_p;
    وإلا بنسټ = class_p->بنسټ_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    طالما(بنسټ && بنسټ->event_cb == عدم) بنسټ = بنسټ->بنسټ_class;

    إذا(بنسټ == عدم) أرجع LV_پایله_OK;
    إذا(بنسټ->event_cb == عدم) أرجع LV_پایله_OK;

    /*Call the actual event callback*/
    e->user_معلومات = عدم;
    LV_PROFILER_EVENT_BEGIN_نښان(lv_event_کوډ_get_name(e->کوډ));
    بنسټ->event_cb(بنسټ, e);
    LV_PROFILER_EVENT_پای_ټکی_نښان(lv_event_کوډ_get_name(e->کوډ));

    lv_پایله_t res = LV_پایله_OK;
    /*Stop if the object is deleted*/
    إذا(e->deleted) res = LV_پایله_INVALID;

    أرجع res;
}

lv_event_dsc_t * lv_obj_جمع_کړه_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_کوډ_t filter, فراغ * user_معلومات)
{
    LV_CHECK_ARG(event_cb != عدم, أرجع عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع عدم;
    }

    أرجع lv_event_جمع_کړه(&obj->spec_attr->event_list, event_cb, filter, user_معلومات);
}

uint32_t lv_obj_get_event_شمېر(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع lv_event_get_شمېر(&obj->spec_attr->event_list);
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

    uint32_t event_cnt = lv_obj_get_event_شمېر(obj);
    uint32_t removed_شمېر = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_شمېر++;
        }
    }

    أرجع removed_شمېر;
}

uint32_t lv_obj_remove_event_cb_with_user_معلومات(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_معلومات)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_شمېر(obj);
    uint32_t removed_شمېر = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_معلومات == user_معلومات) {
            lv_obj_remove_event(obj, i);
            removed_شمېر ++;
        }
    }

    أرجع removed_شمېر;
}

lv_obj_t * lv_event_get_current_موخه_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_current_موخه(e);
}

lv_obj_t * lv_event_get_موخه_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_موخه(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کوډ_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_ورک,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_پای_ټکی,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_پورته,        LV_EVENT_GESTURE_ښکته,
        LV_EVENT_GESTURE_کیڼ,      LV_EVENT_GESTURE_ښی,
        LV_EVENT_کلی,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#د_که_پای
    LV_CHECK_ARG_FORMAT_پیغام(
        event_کوډ_دننه_array(e->کوډ, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کوډ_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_پای_ټکی,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_پای_ټکی,
    };
#د_که_پای
    LV_CHECK_ARG_FORMAT_پیغام(
        event_کوډ_دننه_array(e->کوډ, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_اندازه(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_اندازه_بدل_شوی,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

uint32_t lv_event_get_کلی(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_کلی,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    uint32_t * k = lv_event_get_param(e);
    أرجع k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_ROTARY,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    int32_t * r = lv_event_get_param(e);
    أرجع r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_SCROLL_BEGIN,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

فراغ lv_event_سېټ_ext_draw_اندازه(lv_event_t * e, int32_t اندازه)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_REFR_EXT_DRAW_اندازه,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    int32_t * cur_اندازه = lv_event_get_param(e);
    *cur_اندازه = LV_MAX(*cur_اندازه, اندازه);
}

lv_ټکی_t * lv_event_get_self_اندازه_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_GET_SELF_اندازه,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

lv_موندنه_test_info_t * lv_event_get_موندنه_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_موندنه_TEST,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_COVER_CHECK,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    أرجع p->area;
}

فراغ lv_event_سېټ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_COVER_CHECK,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إذا(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_DRAW_TASK_ADDED,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    أرجع lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوډ == LV_EVENT_STATE_بدل_شوی,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوډ);
    lv_state_t * state = lv_event_get_param(e);
    أرجع state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_پایله_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->کوډ, (فراغ *)e->original_موخه, e->param);

    lv_indev_t * indev_act = lv_indev_فعال();
    إذا(indev_act) {
        إذا(e->stop_processing) أرجع LV_پایله_OK;
        إذا(e->deleted) أرجع LV_پایله_INVALID;
    }

    lv_obj_t * موخه = e->current_موخه;
    lv_پایله_t res = LV_پایله_OK;
    lv_event_list_t * list = موخه->spec_attr ?  &موخه->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    إذا(res != LV_پایله_OK || e->stop_processing) أرجع res;

    res = lv_obj_event_بنسټ(عدم, e);
    إذا(res != LV_پایله_OK || e->stop_processing) أرجع res;

    res = lv_event_send(list, e, false);
    إذا(res != LV_پایله_OK || e->stop_processing) أرجع res;

    lv_obj_t * پلار = lv_obj_get_پلار(e->current_موخه);
    إذا(پلار && event_is_bubbled(e)) {
        e->current_موخه = پلار;
        res = event_send_core(e);
    }
    إذا(res != LV_پایله_OK) أرجع res;

    /*Trickle down to children if enabled*/
    إذا(event_is_trickled(e)) {
        uint32_t ماشوم_شمېر = lv_obj_get_ماشوم_شمېر(موخه);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < ماشوم_شمېر && res == LV_پایله_OK && !e->stop_processing; i++) {
            lv_obj_t * ماشوم = lv_obj_get_ماشوم(موخه, i);
            إذا(ماشوم) {
                e->current_موخه = ماشوم;
                res = event_send_core(e);
                إذا(res != LV_پایله_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->کوډ, (فراغ *)ماشوم);
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
    بدل(e->کوډ) {
        حالة LV_EVENT_ماشوم_CREATED:
        حالة LV_EVENT_ماشوم_DELETED:
            أرجع true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    إذا(lv_obj_is_event_bubble(e->current_موخه) == false) أرجع false;

    بدل(e->کوډ) {
        حالة LV_EVENT_موندنه_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_اندازه:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_پای_ټکی:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_پای_ټکی:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_ماشوم_CREATED:
        حالة LV_EVENT_ماشوم_DELETED:
        حالة LV_EVENT_ماشوم_بدل_شوی:
        حالة LV_EVENT_اندازه_بدل_شوی:
        حالة LV_EVENT_STYLE_بدل_شوی:
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
    إذا(lv_obj_is_event_trickle(e->current_موخه) == false) أرجع false;

    بدل(e->کوډ) {
        حالة LV_EVENT_موندنه_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_اندازه:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_پای_ټکی:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_پای_ټکی:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_ماشوم_CREATED:
        حالة LV_EVENT_ماشوم_DELETED:
        حالة LV_EVENT_ماشوم_بدل_شوی:
        حالة LV_EVENT_اندازه_بدل_شوی:
        حالة LV_EVENT_STYLE_بدل_شوی:
        حالة LV_EVENT_GET_SELF_اندازه:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

#إذا LV_USE_CHECK_ARG
ساكن bool event_کوډ_دننه_array(lv_event_کوډ_t کوډ, ثابت lv_event_کوډ_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        إذا(کوډ == arr[i]) أرجع true;
    }
    أرجع false;
}
#د_که_پای
