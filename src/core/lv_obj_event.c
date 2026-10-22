/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#அந்தர்பாவ "../misc/lv_event_private.h"
#அந்தர்பாவ "../lvgl_public.h"
#அந்தர்பாவ "lv_obj_event_private.h"
#அந்தர்பாவ "lv_obj_class_private.h"
#அந்தர்பாவ "lv_obj_private.h"
#அந்தர்பாவ "../indev/lv_indev_private.h"

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
நிலையான lv_result_t event_send_core(lv_event_t * e);
நிலையான bool event_is_bubbled(lv_event_t * e);
நிலையான bool event_is_trickled(lv_event_t * e);
#எனில் LV_USE_CHECK_ARG
    நிலையான bool event_code_in_array(lv_event_code_t code, மாறிலி lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#எனில் LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#இல்லையெனில்
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, வெறுமை * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = வெற்று;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_தள்ளு(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_எடு(&e);

    திரும்பு res;
}

lv_result_t lv_obj_event_base(மாறிலி lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != வெற்று, திரும்பு LV_RESULT_INVALID);

    மாறிலி lv_obj_class_t * base;
    எனில்(class_p == வெற்று) base = ((lv_obj_t *)e->current_target)->class_p;
    இல்லையெனில் base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    போது(base && base->event_cb == வெற்று) base = base->base_class;

    எனில்(base == வெற்று) திரும்பு LV_RESULT_OK;
    எனில்(base->event_cb == வெற்று) திரும்பு LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = வெற்று;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    எனில்(e->deleted) res = LV_RESULT_INVALID;

    திரும்பு res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, வெறுமை * user_data)
{
    LV_CHECK_ARG(event_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_OBJ(obj, MY_CLASS, திரும்பு 0);

    எனில்(!lv_obj_allocate_spec_attr(obj)) {
        திரும்பு வெற்று;
    }

    திரும்பு lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_மொத்தம்(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு 0);
    எனில்(obj->spec_attr == வெற்று) திரும்பு 0;
    திரும்பு lv_event_get_மொத்தம்(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    எனில்(obj->spec_attr == வெற்று) திரும்பு வெற்று;
    திரும்பு lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு false);
    எனில்(obj->spec_attr == வெற்று) திரும்பு false;
    திரும்பு lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு false);
    LV_CHECK_ARG(dsc != வெற்று, திரும்பு false);

    எனில்(obj->spec_attr == வெற்று) திரும்பு false;
    திரும்பு lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு 0);

    uint32_t event_cnt = lv_obj_get_event_மொத்தம்(obj);
    uint32_t removed_மொத்தம் = 0;
    int32_t i;

    எனில்(event_cnt == 0) திரும்பு 0;

    க்கு(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        எனில்(dsc && (event_cb == வெற்று || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_மொத்தம்++;
        }
    }

    திரும்பு removed_மொத்தம்;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, வெறுமை * user_data)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு 0);

    uint32_t event_cnt = lv_obj_get_event_மொத்தம்(obj);
    uint32_t removed_மொத்தம் = 0;
    int32_t i;

    எனில்(event_cnt == 0) திரும்பு 0;

    க்கு(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        எனில்(dsc && (event_cb == வெற்று || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_மொத்தம் ++;
        }
    }

    திரும்பு removed_மொத்தம்;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    திரும்பு lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    திரும்பு lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
#எனில் LV_USE_CHECK_ARG
    நிலையான மாறிலி lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_இடது,      LV_EVENT_GESTURE_வலது,
        LV_EVENT_சாவி,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_செய்தி(
        event_code_in_array(e->code, indev_codes, அளவு(indev_codes) / அளவு(indev_codes[0])),
        திரும்பு வெற்று,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
#எனில் LV_USE_CHECK_ARG
    நிலையான மாறிலி lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_செய்தி(
        event_code_in_array(e->code, draw_codes, அளவு(draw_codes) / அளவு(draw_codes[0])),
        திரும்பு வெற்று,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

மாறிலி lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_SIZE_CHANGED,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

uint32_t lv_event_get_சாவி(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு 0);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_சாவி,
                            திரும்பு 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    திரும்பு k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு 0);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_ROTARY,
                            திரும்பு 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    திரும்பு r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_SCROLL_BEGIN,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

வெறுமை lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            திரும்பு,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_GET_SELF_SIZE,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_HIT_TEST,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

மாறிலி lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_COVER_CHECK,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    திரும்பு p->area;
}

வெறுமை lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_COVER_CHECK,
                            திரும்பு,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    எனில்(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            திரும்பு வெற்று,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    திரும்பு lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != வெற்று, திரும்பு 0);
    LV_CHECK_ARG_FORMAT_செய்தி(e->code == LV_EVENT_STATE_CHANGED,
                            திரும்பு 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    திரும்பு state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

நிலையான lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (வெறுமை *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    எனில்(indev_act) {
        எனில்(e->stop_processing) திரும்பு LV_RESULT_OK;
        எனில்(e->deleted) திரும்பு LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : வெற்று;

    res = lv_event_send(list, e, true);
    எனில்(res != LV_RESULT_OK || e->stop_processing) திரும்பு res;

    res = lv_obj_event_base(வெற்று, e);
    எனில்(res != LV_RESULT_OK || e->stop_processing) திரும்பு res;

    res = lv_event_send(list, e, false);
    எனில்(res != LV_RESULT_OK || e->stop_processing) திரும்பு res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    எனில்(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    எனில்(res != LV_RESULT_OK) திரும்பு res;

    /*Trickle down to children if enabled*/
    எனில்(event_is_trickled(e)) {
        uint32_t child_மொத்தம் = lv_obj_get_child_மொத்தம்(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        க்கு(uint32_t i = 0; i < child_மொத்தம் && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            எனில்(child) {
                e->current_target = child;
                res = event_send_core(e);
                எனில்(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (வெறுமை *)child);
                    நிறுத்து;
                }
            }
        }
    }

    திரும்பு res;
}

நிலையான bool event_is_bubbled(lv_event_t * e)
{
    எனில்(e->stop_bubbling) திரும்பு false;

    /*Event codes that always bubble*/
    தேர்வு(e->code) {
        நிலை LV_EVENT_CHILD_CREATED:
        நிலை LV_EVENT_CHILD_DELETED:
            திரும்பு true;
        இயல்பு:
            நிறுத்து;
    }

    /*Check other codes only if bubbling is enabled*/
    எனில்(lv_obj_is_event_bubble(e->current_target) == false) திரும்பு false;

    தேர்வு(e->code) {
        நிலை LV_EVENT_HIT_TEST:
        நிலை LV_EVENT_COVER_CHECK:
        நிலை LV_EVENT_REFR_EXT_DRAW_SIZE:
        நிலை LV_EVENT_DRAW_MAIN_BEGIN:
        நிலை LV_EVENT_DRAW_MAIN:
        நிலை LV_EVENT_DRAW_MAIN_END:
        நிலை LV_EVENT_DRAW_POST_BEGIN:
        நிலை LV_EVENT_DRAW_POST:
        நிலை LV_EVENT_DRAW_POST_END:
        நிலை LV_EVENT_DRAW_TASK_ADDED:
        நிலை LV_EVENT_REFRESH:
        நிலை LV_EVENT_DELETE:
        நிலை LV_EVENT_CHILD_CREATED:
        நிலை LV_EVENT_CHILD_DELETED:
        நிலை LV_EVENT_CHILD_CHANGED:
        நிலை LV_EVENT_SIZE_CHANGED:
        நிலை LV_EVENT_STYLE_CHANGED:
        நிலை LV_EVENT_GET_SELF_SIZE:
            திரும்பு false;
        இயல்பு:
            திரும்பு true;
    }
}

நிலையான bool event_is_trickled(lv_event_t * e)
{
    எனில்(e->stop_trickling) திரும்பு false;

    /*Check other codes only if trickle is enabled*/
    எனில்(lv_obj_is_event_trickle(e->current_target) == false) திரும்பு false;

    தேர்வு(e->code) {
        நிலை LV_EVENT_HIT_TEST:
        நிலை LV_EVENT_COVER_CHECK:
        நிலை LV_EVENT_REFR_EXT_DRAW_SIZE:
        நிலை LV_EVENT_DRAW_MAIN_BEGIN:
        நிலை LV_EVENT_DRAW_MAIN:
        நிலை LV_EVENT_DRAW_MAIN_END:
        நிலை LV_EVENT_DRAW_POST_BEGIN:
        நிலை LV_EVENT_DRAW_POST:
        நிலை LV_EVENT_DRAW_POST_END:
        நிலை LV_EVENT_DRAW_TASK_ADDED:
        நிலை LV_EVENT_REFRESH:
        நிலை LV_EVENT_DELETE:
        நிலை LV_EVENT_CHILD_CREATED:
        நிலை LV_EVENT_CHILD_DELETED:
        நிலை LV_EVENT_CHILD_CHANGED:
        நிலை LV_EVENT_SIZE_CHANGED:
        நிலை LV_EVENT_STYLE_CHANGED:
        நிலை LV_EVENT_GET_SELF_SIZE:
            திரும்பு false;
        இயல்பு:
            திரும்பு true;
    }
}

#எனில் LV_USE_CHECK_ARG
நிலையான bool event_code_in_array(lv_event_code_t code, மாறிலி lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    க்கு(i = 0; i < len; i++) {
        எனில்(code == arr[i]) திரும்பு true;
    }
    திரும்பு false;
}
#endif
