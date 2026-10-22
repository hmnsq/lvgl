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
#define MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ایستا lv_result_t event_send_core(lv_event_t * e);
ایستا bool event_is_bubbled(lv_event_t * e);
ایستا bool event_is_trickled(lv_event_t * e);
#اگر LV_USE_CHECK_ARG
    ایستا bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#اگر LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وگرنه
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, تهی * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = پوچ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_افزودن(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_برداشت(&e);

    بازگشت res;
}

lv_result_t lv_obj_event_base(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != پوچ, بازگشت LV_RESULT_INVALID);

    ثابت lv_obj_class_t * base;
    اگر(class_p == پوچ) base = ((lv_obj_t *)e->current_target)->class_p;
    وگرنه base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    وقتی(base && base->event_cb == پوچ) base = base->base_class;

    اگر(base == پوچ) بازگشت LV_RESULT_OK;
    اگر(base->event_cb == پوچ) بازگشت LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = پوچ;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    اگر(e->deleted) res = LV_RESULT_INVALID;

    بازگشت res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, تهی * user_data)
{
    LV_CHECK_ARG(event_cb != پوچ, بازگشت پوچ);
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت پوچ;
    }

    بازگشت lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_شمار(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت 0);
    اگر(obj->spec_attr == پوچ) بازگشت 0;
    بازگشت lv_event_get_شمار(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    اگر(obj->spec_attr == پوچ) بازگشت پوچ;
    بازگشت lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت false);
    اگر(obj->spec_attr == پوچ) بازگشت false;
    بازگشت lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت false);
    LV_CHECK_ARG(dsc != پوچ, بازگشت false);

    اگر(obj->spec_attr == پوچ) بازگشت false;
    بازگشت lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت 0);

    uint32_t event_cnt = lv_obj_get_event_شمار(obj);
    uint32_t removed_شمار = 0;
    int32_t i;

    اگر(event_cnt == 0) بازگشت 0;

    برای(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        اگر(dsc && (event_cb == پوچ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_شمار++;
        }
    }

    بازگشت removed_شمار;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, تهی * user_data)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت 0);

    uint32_t event_cnt = lv_obj_get_event_شمار(obj);
    uint32_t removed_شمار = 0;
    int32_t i;

    اگر(event_cnt == 0) بازگشت 0;

    برای(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        اگر(dsc && (event_cb == پوچ || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_شمار ++;
        }
    }

    بازگشت removed_شمار;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    بازگشت lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    بازگشت lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
#اگر LV_USE_CHECK_ARG
    ایستا ثابت lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_چپ,      LV_EVENT_GESTURE_راست,
        LV_EVENT_کلید,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_پیام(
        event_code_in_array(e->code, indev_codes, اندازه(indev_codes) / اندازه(indev_codes[0])),
        بازگشت پوچ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
#اگر LV_USE_CHECK_ARG
    ایستا ثابت lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_پیام(
        event_code_in_array(e->code, draw_codes, اندازه(draw_codes) / اندازه(draw_codes[0])),
        بازگشت پوچ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_SIZE_CHANGED,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

uint32_t lv_event_get_کلید(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت 0);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_کلید,
                            بازگشت 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    بازگشت k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت 0);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_ROTARY,
                            بازگشت 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    بازگشت r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_SCROLL_BEGIN,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

تهی lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != پوچ, بازگشت);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            بازگشت,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_GET_SELF_SIZE,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_HIT_TEST,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_COVER_CHECK,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    بازگشت p->area;
}

تهی lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != پوچ, بازگشت);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_COVER_CHECK,
                            بازگشت,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    اگر(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            بازگشت پوچ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    بازگشت lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != پوچ, بازگشت 0);
    LV_CHECK_ARG_FORMAT_پیام(e->code == LV_EVENT_STATE_CHANGED,
                            بازگشت 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    بازگشت state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ایستا lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (تهی *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    اگر(indev_act) {
        اگر(e->stop_processing) بازگشت LV_RESULT_OK;
        اگر(e->deleted) بازگشت LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : پوچ;

    res = lv_event_send(list, e, true);
    اگر(res != LV_RESULT_OK || e->stop_processing) بازگشت res;

    res = lv_obj_event_base(پوچ, e);
    اگر(res != LV_RESULT_OK || e->stop_processing) بازگشت res;

    res = lv_event_send(list, e, false);
    اگر(res != LV_RESULT_OK || e->stop_processing) بازگشت res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    اگر(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    اگر(res != LV_RESULT_OK) بازگشت res;

    /*Trickle down to children if enabled*/
    اگر(event_is_trickled(e)) {
        uint32_t child_شمار = lv_obj_get_child_شمار(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        برای(uint32_t i = 0; i < child_شمار && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            اگر(child) {
                e->current_target = child;
                res = event_send_core(e);
                اگر(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (تهی *)child);
                    بشکن;
                }
            }
        }
    }

    بازگشت res;
}

ایستا bool event_is_bubbled(lv_event_t * e)
{
    اگر(e->stop_bubbling) بازگشت false;

    /*Event codes that always bubble*/
    انتخاب(e->code) {
        حالت LV_EVENT_CHILD_CREATED:
        حالت LV_EVENT_CHILD_DELETED:
            بازگشت true;
        پیشفرض:
            بشکن;
    }

    /*Check other codes only if bubbling is enabled*/
    اگر(lv_obj_is_event_bubble(e->current_target) == false) بازگشت false;

    انتخاب(e->code) {
        حالت LV_EVENT_HIT_TEST:
        حالت LV_EVENT_COVER_CHECK:
        حالت LV_EVENT_REFR_EXT_DRAW_SIZE:
        حالت LV_EVENT_DRAW_MAIN_BEGIN:
        حالت LV_EVENT_DRAW_MAIN:
        حالت LV_EVENT_DRAW_MAIN_END:
        حالت LV_EVENT_DRAW_POST_BEGIN:
        حالت LV_EVENT_DRAW_POST:
        حالت LV_EVENT_DRAW_POST_END:
        حالت LV_EVENT_DRAW_TASK_ADDED:
        حالت LV_EVENT_REFRESH:
        حالت LV_EVENT_DELETE:
        حالت LV_EVENT_CHILD_CREATED:
        حالت LV_EVENT_CHILD_DELETED:
        حالت LV_EVENT_CHILD_CHANGED:
        حالت LV_EVENT_SIZE_CHANGED:
        حالت LV_EVENT_STYLE_CHANGED:
        حالت LV_EVENT_GET_SELF_SIZE:
            بازگشت false;
        پیشفرض:
            بازگشت true;
    }
}

ایستا bool event_is_trickled(lv_event_t * e)
{
    اگر(e->stop_trickling) بازگشت false;

    /*Check other codes only if trickle is enabled*/
    اگر(lv_obj_is_event_trickle(e->current_target) == false) بازگشت false;

    انتخاب(e->code) {
        حالت LV_EVENT_HIT_TEST:
        حالت LV_EVENT_COVER_CHECK:
        حالت LV_EVENT_REFR_EXT_DRAW_SIZE:
        حالت LV_EVENT_DRAW_MAIN_BEGIN:
        حالت LV_EVENT_DRAW_MAIN:
        حالت LV_EVENT_DRAW_MAIN_END:
        حالت LV_EVENT_DRAW_POST_BEGIN:
        حالت LV_EVENT_DRAW_POST:
        حالت LV_EVENT_DRAW_POST_END:
        حالت LV_EVENT_DRAW_TASK_ADDED:
        حالت LV_EVENT_REFRESH:
        حالت LV_EVENT_DELETE:
        حالت LV_EVENT_CHILD_CREATED:
        حالت LV_EVENT_CHILD_DELETED:
        حالت LV_EVENT_CHILD_CHANGED:
        حالت LV_EVENT_SIZE_CHANGED:
        حالت LV_EVENT_STYLE_CHANGED:
        حالت LV_EVENT_GET_SELF_SIZE:
            بازگشت false;
        پیشفرض:
            بازگشت true;
    }
}

#اگر LV_USE_CHECK_ARG
ایستا bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    برای(i = 0; i < len; i++) {
        اگر(code == arr[i]) بازگشت true;
    }
    بازگشت false;
}
#endif
