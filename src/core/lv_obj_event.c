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
#define MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_result_t event_send_core(lv_event_t * e);
ساكن bool event_is_bubbled(lv_event_t * e);
ساكن bool event_is_trickled(lv_event_t * e);
#ܐܢ LV_USE_CHECK_ARG
    ساكن bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ܐܢ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ܐܚܪܢܐ
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = عدم;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_push(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_pop(&e);

    ܦܢܐ res;
}

lv_result_t lv_obj_event_base(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != عدم, ܦܢܐ LV_RESULT_INVALID);

    ثابت lv_obj_class_t * base;
    ܐܢ(class_p == عدم) base = ((lv_obj_t *)e->current_target)->class_p;
    ܐܚܪܢܐ base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ܟܡܐ(base && base->event_cb == عدم) base = base->base_class;

    ܐܢ(base == عدم) ܦܢܐ LV_RESULT_OK;
    ܐܢ(base->event_cb == عدم) ܦܢܐ LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = عدم;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    ܐܢ(e->deleted) res = LV_RESULT_INVALID;

    ܦܢܐ res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, فراغ * user_data)
{
    LV_CHECK_ARG(event_cb != عدم, ܦܢܐ عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, ܦܢܐ 0);

    ܐܢ(!lv_obj_allocate_spec_attr(obj)) {
        ܦܢܐ عدم;
    }

    ܦܢܐ lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_count(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ 0);
    ܐܢ(obj->spec_attr == عدم) ܦܢܐ 0;
    ܦܢܐ lv_event_get_count(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ عدم);
    ܐܢ(obj->spec_attr == عدم) ܦܢܐ عدم;
    ܦܢܐ lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ false);
    ܐܢ(obj->spec_attr == عدم) ܦܢܐ false;
    ܦܢܐ lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ false);
    LV_CHECK_ARG(dsc != عدم, ܦܢܐ false);

    ܐܢ(obj->spec_attr == عدم) ܦܢܐ false;
    ܦܢܐ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    ܐܢ(event_cnt == 0) ܦܢܐ 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ܐܢ(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_count++;
        }
    }

    ܦܢܐ removed_count;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_data)
{
    LV_CHECK_ARG(obj != عدم, ܦܢܐ 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    ܐܢ(event_cnt == 0) ܦܢܐ 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ܐܢ(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_count ++;
        }
    }

    ܦܢܐ removed_count;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    ܦܢܐ lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    ܦܢܐ lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
#ܐܢ LV_USE_CHECK_ARG
    ساكن ثابت lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_LEFT,      LV_EVENT_GESTURE_RIGHT,
        LV_EVENT_KEY,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_MSG(
        event_code_in_array(e->code, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        ܦܢܐ عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
#ܐܢ LV_USE_CHECK_ARG
    ساكن ثابت lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_MSG(
        event_code_in_array(e->code, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        ܦܢܐ عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SIZE_CHANGED,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

uint32_t lv_event_get_key(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_KEY,
                            ܦܢܐ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    ܦܢܐ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_ROTARY,
                            ܦܢܐ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    ܦܢܐ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SCROLL_BEGIN,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

فراغ lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            ܦܢܐ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_GET_SELF_SIZE,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_HIT_TEST,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ܦܢܐ p->area;
}

فراغ lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            ܦܢܐ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ܐܢ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ عدم);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            ܦܢܐ عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ܦܢܐ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, ܦܢܐ 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_STATE_CHANGED,
                            ܦܢܐ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    ܦܢܐ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (فراغ *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    ܐܢ(indev_act) {
        ܐܢ(e->stop_processing) ܦܢܐ LV_RESULT_OK;
        ܐܢ(e->deleted) ܦܢܐ LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    ܐܢ(res != LV_RESULT_OK || e->stop_processing) ܦܢܐ res;

    res = lv_obj_event_base(عدم, e);
    ܐܢ(res != LV_RESULT_OK || e->stop_processing) ܦܢܐ res;

    res = lv_event_send(list, e, false);
    ܐܢ(res != LV_RESULT_OK || e->stop_processing) ܦܢܐ res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    ܐܢ(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    ܐܢ(res != LV_RESULT_OK) ܦܢܐ res;

    /*Trickle down to children if enabled*/
    ܐܢ(event_is_trickled(e)) {
        uint32_t child_count = lv_obj_get_child_count(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < child_count && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            ܐܢ(child) {
                e->current_target = child;
                res = event_send_core(e);
                ܐܢ(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (فراغ *)child);
                    اكسر;
                }
            }
        }
    }

    ܦܢܐ res;
}

ساكن bool event_is_bubbled(lv_event_t * e)
{
    ܐܢ(e->stop_bubbling) ܦܢܐ false;

    /*Event codes that always bubble*/
    بدل(e->code) {
        حالة LV_EVENT_CHILD_CREATED:
        حالة LV_EVENT_CHILD_DELETED:
            ܦܢܐ true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    ܐܢ(lv_obj_is_event_bubble(e->current_target) == false) ܦܢܐ false;

    بدل(e->code) {
        حالة LV_EVENT_HIT_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_SIZE:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_END:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_END:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_CHILD_CREATED:
        حالة LV_EVENT_CHILD_DELETED:
        حالة LV_EVENT_CHILD_CHANGED:
        حالة LV_EVENT_SIZE_CHANGED:
        حالة LV_EVENT_STYLE_CHANGED:
        حالة LV_EVENT_GET_SELF_SIZE:
            ܦܢܐ false;
        افتراضي:
            ܦܢܐ true;
    }
}

ساكن bool event_is_trickled(lv_event_t * e)
{
    ܐܢ(e->stop_trickling) ܦܢܐ false;

    /*Check other codes only if trickle is enabled*/
    ܐܢ(lv_obj_is_event_trickle(e->current_target) == false) ܦܢܐ false;

    بدل(e->code) {
        حالة LV_EVENT_HIT_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_SIZE:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_END:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_END:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_CHILD_CREATED:
        حالة LV_EVENT_CHILD_DELETED:
        حالة LV_EVENT_CHILD_CHANGED:
        حالة LV_EVENT_SIZE_CHANGED:
        حالة LV_EVENT_STYLE_CHANGED:
        حالة LV_EVENT_GET_SELF_SIZE:
            ܦܢܐ false;
        افتراضي:
            ܦܢܐ true;
    }
}

#ܐܢ LV_USE_CHECK_ARG
ساكن bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        ܐܢ(code == arr[i]) ܦܢܐ true;
    }
    ܦܢܐ false;
}
#endif
