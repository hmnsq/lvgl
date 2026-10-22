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
#إذا LV_USE_CHECK_ARG
    ساكن bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع 0);

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

    lv_event_ادفع(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_اسحب(&e);

    إرجاع res;
}

lv_result_t lv_obj_event_base(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != عدم, إرجاع LV_RESULT_INVALID);

    ثابت lv_obj_class_t * base;
    إذا(class_p == عدم) base = ((lv_obj_t *)e->current_target)->class_p;
    وإلا base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    بينما(base && base->event_cb == عدم) base = base->base_class;

    إذا(base == عدم) إرجاع LV_RESULT_OK;
    إذا(base->event_cb == عدم) إرجاع LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = عدم;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    إذا(e->deleted) res = LV_RESULT_INVALID;

    إرجاع res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, فراغ * user_data)
{
    LV_CHECK_ARG(event_cb != عدم, إرجاع عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع 0);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        إرجاع عدم;
    }

    إرجاع lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_تعداد(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, إرجاع 0);
    إذا(obj->spec_attr == عدم) إرجاع 0;
    إرجاع lv_event_get_تعداد(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != عدم, إرجاع عدم);
    إذا(obj->spec_attr == عدم) إرجاع عدم;
    إرجاع lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != عدم, إرجاع false);
    إذا(obj->spec_attr == عدم) إرجاع false;
    إرجاع lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != عدم, إرجاع false);
    LV_CHECK_ARG(dsc != عدم, إرجاع false);

    إذا(obj->spec_attr == عدم) إرجاع false;
    إرجاع lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != عدم, إرجاع 0);

    uint32_t event_cnt = lv_obj_get_event_تعداد(obj);
    uint32_t removed_تعداد = 0;
    int32_t i;

    إذا(event_cnt == 0) إرجاع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_تعداد++;
        }
    }

    إرجاع removed_تعداد;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_data)
{
    LV_CHECK_ARG(obj != عدم, إرجاع 0);

    uint32_t event_cnt = lv_obj_get_event_تعداد(obj);
    uint32_t removed_تعداد = 0;
    int32_t i;

    إذا(event_cnt == 0) إرجاع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_تعداد ++;
        }
    }

    إرجاع removed_تعداد;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    إرجاع lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    إرجاع lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_يسار,      LV_EVENT_GESTURE_يمين,
        LV_EVENT_مفتاح,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_رسالة(
        event_code_in_array(e->code, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        إرجاع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_رسالة(
        event_code_in_array(e->code, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        إرجاع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_SIZE_CHANGED,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

uint32_t lv_event_get_مفتاح(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع 0);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_مفتاح,
                            إرجاع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    إرجاع k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع 0);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_ROTARY,
                            إرجاع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    إرجاع r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_SCROLL_BEGIN,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

فراغ lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != عدم, إرجاع);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            إرجاع,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_GET_SELF_SIZE,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_HIT_TEST,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_COVER_CHECK,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إرجاع p->area;
}

فراغ lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, إرجاع);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_COVER_CHECK,
                            إرجاع,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إذا(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع عدم);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            إرجاع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    إرجاع lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, إرجاع 0);
    LV_CHECK_ARG_FORMAT_رسالة(e->code == LV_EVENT_STATE_CHANGED,
                            إرجاع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    إرجاع state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (فراغ *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    إذا(indev_act) {
        إذا(e->stop_processing) إرجاع LV_RESULT_OK;
        إذا(e->deleted) إرجاع LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    إذا(res != LV_RESULT_OK || e->stop_processing) إرجاع res;

    res = lv_obj_event_base(عدم, e);
    إذا(res != LV_RESULT_OK || e->stop_processing) إرجاع res;

    res = lv_event_send(list, e, false);
    إذا(res != LV_RESULT_OK || e->stop_processing) إرجاع res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    إذا(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    إذا(res != LV_RESULT_OK) إرجاع res;

    /*Trickle down to children if enabled*/
    إذا(event_is_trickled(e)) {
        uint32_t child_تعداد = lv_obj_get_child_تعداد(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < child_تعداد && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            إذا(child) {
                e->current_target = child;
                res = event_send_core(e);
                إذا(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (فراغ *)child);
                    اكسر;
                }
            }
        }
    }

    إرجاع res;
}

ساكن bool event_is_bubbled(lv_event_t * e)
{
    إذا(e->stop_bubbling) إرجاع false;

    /*Event codes that always bubble*/
    بدل(e->code) {
        حالة LV_EVENT_CHILD_CREATED:
        حالة LV_EVENT_CHILD_DELETED:
            إرجاع true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    إذا(lv_obj_is_event_bubble(e->current_target) == false) إرجاع false;

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
            إرجاع false;
        افتراضي:
            إرجاع true;
    }
}

ساكن bool event_is_trickled(lv_event_t * e)
{
    إذا(e->stop_trickling) إرجاع false;

    /*Check other codes only if trickle is enabled*/
    إذا(lv_obj_is_event_trickle(e->current_target) == false) إرجاع false;

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
            إرجاع false;
        افتراضي:
            إرجاع true;
    }
}

#إذا LV_USE_CHECK_ARG
ساكن bool event_code_in_array(lv_event_code_t code, ثابت lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        إذا(code == arr[i]) إرجاع true;
    }
    إرجاع false;
}
#endif
