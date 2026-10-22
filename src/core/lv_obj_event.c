/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "../misc/lv_event_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_event_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../indev/lv_indev_private.h"

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
ಸ್ಥಿರ lv_result_t event_send_core(lv_event_t * e);
ಸ್ಥಿರ bool event_is_bubbled(lv_event_t * e);
ಸ್ಥಿರ bool event_is_trickled(lv_event_t * e);
#ಯದಿ LV_USE_CHECK_ARG
    ಸ್ಥಿರ bool event_code_in_array(lv_event_code_t code, ಧ್ರುವ lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ಯದಿ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ಅನ್ಯಥಾ
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, ಶೂನ್ಯ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = ರಿಕ್ತ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ತಳ್ಳು(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_ತೆಗೆ(&e);

    ಪ್ರತ್ಯಾಗಮ res;
}

lv_result_t lv_obj_event_base(ಧ್ರುವ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);

    ಧ್ರುವ lv_obj_class_t * base;
    ಯದಿ(class_p == ರಿಕ್ತ) base = ((lv_obj_t *)e->current_target)->class_p;
    ಅನ್ಯಥಾ base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ಯಾವತ್(base && base->event_cb == ರಿಕ್ತ) base = base->base_class;

    ಯದಿ(base == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;
    ಯದಿ(base->event_cb == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = ರಿಕ್ತ;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    ಯದಿ(e->deleted) res = LV_RESULT_INVALID;

    ಪ್ರತ್ಯಾಗಮ res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(event_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);

    ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_ಎಣಿಕೆ(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ lv_event_get_ಎಣಿಕೆ(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ false;
    ಪ್ರತ್ಯಾಗಮ lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    LV_CHECK_ARG(dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);

    ಯದಿ(obj->spec_attr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ false;
    ಪ್ರತ್ಯಾಗಮ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);

    uint32_t event_cnt = lv_obj_get_event_ಎಣಿಕೆ(obj);
    uint32_t removed_ಎಣಿಕೆ = 0;
    int32_t i;

    ಯದಿ(event_cnt == 0) ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ಯದಿ(dsc && (event_cb == ರಿಕ್ತ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_ಎಣಿಕೆ++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ removed_ಎಣಿಕೆ;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);

    uint32_t event_cnt = lv_obj_get_event_ಎಣಿಕೆ(obj);
    uint32_t removed_ಎಣಿಕೆ = 0;
    int32_t i;

    ಯದಿ(event_cnt == 0) ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ಯದಿ(dsc && (event_cb == ರಿಕ್ತ || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_ಎಣಿಕೆ ++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ removed_ಎಣಿಕೆ;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
#ಯದಿ LV_USE_CHECK_ARG
    ಸ್ಥಿರ ಧ್ರುವ lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_ಎಡ,      LV_EVENT_GESTURE_ಬಲ,
        LV_EVENT_ಕೀಲಿ,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(
        event_code_in_array(e->code, indev_codes, ಪರಿಮಾಣ(indev_codes) / ಪರಿಮಾಣ(indev_codes[0])),
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
#ಯದಿ LV_USE_CHECK_ARG
    ಸ್ಥಿರ ಧ್ರುವ lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(
        event_code_in_array(e->code, draw_codes, ಪರಿಮಾಣ(draw_codes) / ಪರಿಮಾಣ(draw_codes[0])),
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

ಧ್ರುವ lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_SIZE_CHANGED,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

uint32_t lv_event_get_ಕೀಲಿ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_ಕೀಲಿ,
                            ಪ್ರತ್ಯಾಗಮ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    ಪ್ರತ್ಯಾಗಮ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_ROTARY,
                            ಪ್ರತ್ಯಾಗಮ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    ಪ್ರತ್ಯಾಗಮ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_SCROLL_BEGIN,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

ಶೂನ್ಯ lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            ಪ್ರತ್ಯಾಗಮ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_GET_SELF_SIZE,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_HIT_TEST,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

ಧ್ರುವ lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_COVER_CHECK,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ಪ್ರತ್ಯಾಗಮ p->area;
}

ಶೂನ್ಯ lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_COVER_CHECK,
                            ಪ್ರತ್ಯಾಗಮ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ಯದಿ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ಪ್ರತ್ಯಾಗಮ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->code == LV_EVENT_STATE_CHANGED,
                            ಪ್ರತ್ಯಾಗಮ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    ಪ್ರತ್ಯಾಗಮ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (ಶೂನ್ಯ *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    ಯದಿ(indev_act) {
        ಯದಿ(e->stop_processing) ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;
        ಯದಿ(e->deleted) ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : ರಿಕ್ತ;

    res = lv_event_send(list, e, true);
    ಯದಿ(res != LV_RESULT_OK || e->stop_processing) ಪ್ರತ್ಯಾಗಮ res;

    res = lv_obj_event_base(ರಿಕ್ತ, e);
    ಯದಿ(res != LV_RESULT_OK || e->stop_processing) ಪ್ರತ್ಯಾಗಮ res;

    res = lv_event_send(list, e, false);
    ಯದಿ(res != LV_RESULT_OK || e->stop_processing) ಪ್ರತ್ಯಾಗಮ res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    ಯದಿ(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ res;

    /*Trickle down to children if enabled*/
    ಯದಿ(event_is_trickled(e)) {
        uint32_t child_ಎಣಿಕೆ = lv_obj_get_child_ಎಣಿಕೆ(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        ಪ್ರತಿ(uint32_t i = 0; i < child_ಎಣಿಕೆ && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            ಯದಿ(child) {
                e->current_target = child;
                res = event_send_core(e);
                ಯದಿ(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (ಶೂನ್ಯ *)child);
                    ಭಙ್ಗ;
                }
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ bool event_is_bubbled(lv_event_t * e)
{
    ಯದಿ(e->stop_bubbling) ಪ್ರತ್ಯಾಗಮ false;

    /*Event codes that always bubble*/
    ವಿಕಲ್ಪ(e->code) {
        ಪಕ್ಷ LV_EVENT_CHILD_CREATED:
        ಪಕ್ಷ LV_EVENT_CHILD_DELETED:
            ಪ್ರತ್ಯಾಗಮ true;
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
    }

    /*Check other codes only if bubbling is enabled*/
    ಯದಿ(lv_obj_is_event_bubble(e->current_target) == false) ಪ್ರತ್ಯಾಗಮ false;

    ವಿಕಲ್ಪ(e->code) {
        ಪಕ್ಷ LV_EVENT_HIT_TEST:
        ಪಕ್ಷ LV_EVENT_COVER_CHECK:
        ಪಕ್ಷ LV_EVENT_REFR_EXT_DRAW_SIZE:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN_BEGIN:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN_END:
        ಪಕ್ಷ LV_EVENT_DRAW_POST_BEGIN:
        ಪಕ್ಷ LV_EVENT_DRAW_POST:
        ಪಕ್ಷ LV_EVENT_DRAW_POST_END:
        ಪಕ್ಷ LV_EVENT_DRAW_TASK_ADDED:
        ಪಕ್ಷ LV_EVENT_REFRESH:
        ಪಕ್ಷ LV_EVENT_DELETE:
        ಪಕ್ಷ LV_EVENT_CHILD_CREATED:
        ಪಕ್ಷ LV_EVENT_CHILD_DELETED:
        ಪಕ್ಷ LV_EVENT_CHILD_CHANGED:
        ಪಕ್ಷ LV_EVENT_SIZE_CHANGED:
        ಪಕ್ಷ LV_EVENT_STYLE_CHANGED:
        ಪಕ್ಷ LV_EVENT_GET_SELF_SIZE:
            ಪ್ರತ್ಯಾಗಮ false;
        ಸ್ವಭಾವ:
            ಪ್ರತ್ಯಾಗಮ true;
    }
}

ಸ್ಥಿರ bool event_is_trickled(lv_event_t * e)
{
    ಯದಿ(e->stop_trickling) ಪ್ರತ್ಯಾಗಮ false;

    /*Check other codes only if trickle is enabled*/
    ಯದಿ(lv_obj_is_event_trickle(e->current_target) == false) ಪ್ರತ್ಯಾಗಮ false;

    ವಿಕಲ್ಪ(e->code) {
        ಪಕ್ಷ LV_EVENT_HIT_TEST:
        ಪಕ್ಷ LV_EVENT_COVER_CHECK:
        ಪಕ್ಷ LV_EVENT_REFR_EXT_DRAW_SIZE:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN_BEGIN:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN:
        ಪಕ್ಷ LV_EVENT_DRAW_MAIN_END:
        ಪಕ್ಷ LV_EVENT_DRAW_POST_BEGIN:
        ಪಕ್ಷ LV_EVENT_DRAW_POST:
        ಪಕ್ಷ LV_EVENT_DRAW_POST_END:
        ಪಕ್ಷ LV_EVENT_DRAW_TASK_ADDED:
        ಪಕ್ಷ LV_EVENT_REFRESH:
        ಪಕ್ಷ LV_EVENT_DELETE:
        ಪಕ್ಷ LV_EVENT_CHILD_CREATED:
        ಪಕ್ಷ LV_EVENT_CHILD_DELETED:
        ಪಕ್ಷ LV_EVENT_CHILD_CHANGED:
        ಪಕ್ಷ LV_EVENT_SIZE_CHANGED:
        ಪಕ್ಷ LV_EVENT_STYLE_CHANGED:
        ಪಕ್ಷ LV_EVENT_GET_SELF_SIZE:
            ಪ್ರತ್ಯಾಗಮ false;
        ಸ್ವಭಾವ:
            ಪ್ರತ್ಯಾಗಮ true;
    }
}

#ಯದಿ LV_USE_CHECK_ARG
ಸ್ಥಿರ bool event_code_in_array(lv_event_code_t code, ಧ್ರುವ lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < len; i++) {
        ಯದಿ(code == arr[i]) ಪ್ರತ್ಯಾಗಮ true;
    }
    ಪ್ರತ್ಯಾಗಮ false;
}
#endif
