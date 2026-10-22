/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "../misc/lv_event_private.h"
#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "lv_obj_event_private.h"
#അന്തര്ഭാവ "lv_obj_class_private.h"
#അന്തര്ഭാവ "lv_obj_private.h"
#അന്തര്ഭാവ "../indev/lv_indev_private.h"

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
സ്ഥിതം lv_result_t event_send_core(lv_event_t * e);
സ്ഥിതം bool event_is_bubbled(lv_event_t * e);
സ്ഥിതം bool event_is_trickled(lv_event_t * e);
#എങ്കിൽ LV_USE_CHECK_ARG
    സ്ഥിതം bool event_code_in_array(lv_event_code_t code, സ്ഥിരം lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#എങ്കിൽ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#അല്ലെങ്കിൽ
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, ശൂന്യം * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = ശൂന്യ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_തള്ളുക(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_എടുക്കുക(&e);

    തിരികെ res;
}

lv_result_t lv_obj_event_base(സ്ഥിരം lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != ശൂന്യ, തിരികെ LV_RESULT_INVALID);

    സ്ഥിരം lv_obj_class_t * base;
    എങ്കിൽ(class_p == ശൂന്യ) base = ((lv_obj_t *)e->current_target)->class_p;
    അല്ലെങ്കിൽ base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ആകുമ്പോൾ(base && base->event_cb == ശൂന്യ) base = base->base_class;

    എങ്കിൽ(base == ശൂന്യ) തിരികെ LV_RESULT_OK;
    എങ്കിൽ(base->event_cb == ശൂന്യ) തിരികെ LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = ശൂന്യ;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    എങ്കിൽ(e->deleted) res = LV_RESULT_INVALID;

    തിരികെ res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(event_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ 0);

    എങ്കിൽ(!lv_obj_allocate_spec_attr(obj)) {
        തിരികെ ശൂന്യ;
    }

    തിരികെ lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_എണ്ണം(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ 0);
    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ 0;
    തിരികെ lv_event_get_എണ്ണം(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ ശൂന്യ;
    തിരികെ lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ false);
    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ false;
    തിരികെ lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ false);
    LV_CHECK_ARG(dsc != ശൂന്യ, തിരികെ false);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ false;
    തിരികെ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ 0);

    uint32_t event_cnt = lv_obj_get_event_എണ്ണം(obj);
    uint32_t removed_എണ്ണം = 0;
    int32_t i;

    എങ്കിൽ(event_cnt == 0) തിരികെ 0;

    വേണ്ടി(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        എങ്കിൽ(dsc && (event_cb == ശൂന്യ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_എണ്ണം++;
        }
    }

    തിരികെ removed_എണ്ണം;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ 0);

    uint32_t event_cnt = lv_obj_get_event_എണ്ണം(obj);
    uint32_t removed_എണ്ണം = 0;
    int32_t i;

    എങ്കിൽ(event_cnt == 0) തിരികെ 0;

    വേണ്ടി(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        എങ്കിൽ(dsc && (event_cb == ശൂന്യ || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_എണ്ണം ++;
        }
    }

    തിരികെ removed_എണ്ണം;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    തിരികെ lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    തിരികെ lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
#എങ്കിൽ LV_USE_CHECK_ARG
    സ്ഥിതം സ്ഥിരം lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_ഇടത്,      LV_EVENT_GESTURE_വലത്,
        LV_EVENT_താക്കോൽ,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_സന്ദേശം(
        event_code_in_array(e->code, indev_codes, വലുപ്പം(indev_codes) / വലുപ്പം(indev_codes[0])),
        തിരികെ ശൂന്യ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
#എങ്കിൽ LV_USE_CHECK_ARG
    സ്ഥിതം സ്ഥിരം lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_സന്ദേശം(
        event_code_in_array(e->code, draw_codes, വലുപ്പം(draw_codes) / വലുപ്പം(draw_codes[0])),
        തിരികെ ശൂന്യ,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

സ്ഥിരം lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_SIZE_CHANGED,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

uint32_t lv_event_get_താക്കോൽ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ 0);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_താക്കോൽ,
                            തിരികെ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    തിരികെ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ 0);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_ROTARY,
                            തിരികെ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    തിരികെ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_SCROLL_BEGIN,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

ശൂന്യം lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            തിരികെ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_GET_SELF_SIZE,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_HIT_TEST,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

സ്ഥിരം lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_COVER_CHECK,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    തിരികെ p->area;
}

ശൂന്യം lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_COVER_CHECK,
                            തിരികെ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    എങ്കിൽ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            തിരികെ ശൂന്യ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    തിരികെ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ശൂന്യ, തിരികെ 0);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(e->code == LV_EVENT_STATE_CHANGED,
                            തിരികെ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    തിരികെ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

സ്ഥിതം lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (ശൂന്യം *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    എങ്കിൽ(indev_act) {
        എങ്കിൽ(e->stop_processing) തിരികെ LV_RESULT_OK;
        എങ്കിൽ(e->deleted) തിരികെ LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : ശൂന്യ;

    res = lv_event_send(list, e, true);
    എങ്കിൽ(res != LV_RESULT_OK || e->stop_processing) തിരികെ res;

    res = lv_obj_event_base(ശൂന്യ, e);
    എങ്കിൽ(res != LV_RESULT_OK || e->stop_processing) തിരികെ res;

    res = lv_event_send(list, e, false);
    എങ്കിൽ(res != LV_RESULT_OK || e->stop_processing) തിരികെ res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    എങ്കിൽ(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    എങ്കിൽ(res != LV_RESULT_OK) തിരികെ res;

    /*Trickle down to children if enabled*/
    എങ്കിൽ(event_is_trickled(e)) {
        uint32_t child_എണ്ണം = lv_obj_get_child_എണ്ണം(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        വേണ്ടി(uint32_t i = 0; i < child_എണ്ണം && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            എങ്കിൽ(child) {
                e->current_target = child;
                res = event_send_core(e);
                എങ്കിൽ(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (ശൂന്യം *)child);
                    നിർത്തുക;
                }
            }
        }
    }

    തിരികെ res;
}

സ്ഥിതം bool event_is_bubbled(lv_event_t * e)
{
    എങ്കിൽ(e->stop_bubbling) തിരികെ false;

    /*Event codes that always bubble*/
    തിരഞ്ഞെടുപ്പ്(e->code) {
        അവസ്ഥ LV_EVENT_CHILD_CREATED:
        അവസ്ഥ LV_EVENT_CHILD_DELETED:
            തിരികെ true;
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
    }

    /*Check other codes only if bubbling is enabled*/
    എങ്കിൽ(lv_obj_is_event_bubble(e->current_target) == false) തിരികെ false;

    തിരഞ്ഞെടുപ്പ്(e->code) {
        അവസ്ഥ LV_EVENT_HIT_TEST:
        അവസ്ഥ LV_EVENT_COVER_CHECK:
        അവസ്ഥ LV_EVENT_REFR_EXT_DRAW_SIZE:
        അവസ്ഥ LV_EVENT_DRAW_MAIN_BEGIN:
        അവസ്ഥ LV_EVENT_DRAW_MAIN:
        അവസ്ഥ LV_EVENT_DRAW_MAIN_END:
        അവസ്ഥ LV_EVENT_DRAW_POST_BEGIN:
        അവസ്ഥ LV_EVENT_DRAW_POST:
        അവസ്ഥ LV_EVENT_DRAW_POST_END:
        അവസ്ഥ LV_EVENT_DRAW_TASK_ADDED:
        അവസ്ഥ LV_EVENT_REFRESH:
        അവസ്ഥ LV_EVENT_DELETE:
        അവസ്ഥ LV_EVENT_CHILD_CREATED:
        അവസ്ഥ LV_EVENT_CHILD_DELETED:
        അവസ്ഥ LV_EVENT_CHILD_CHANGED:
        അവസ്ഥ LV_EVENT_SIZE_CHANGED:
        അവസ്ഥ LV_EVENT_STYLE_CHANGED:
        അവസ്ഥ LV_EVENT_GET_SELF_SIZE:
            തിരികെ false;
        സ്ഥിരസ്ഥിതി:
            തിരികെ true;
    }
}

സ്ഥിതം bool event_is_trickled(lv_event_t * e)
{
    എങ്കിൽ(e->stop_trickling) തിരികെ false;

    /*Check other codes only if trickle is enabled*/
    എങ്കിൽ(lv_obj_is_event_trickle(e->current_target) == false) തിരികെ false;

    തിരഞ്ഞെടുപ്പ്(e->code) {
        അവസ്ഥ LV_EVENT_HIT_TEST:
        അവസ്ഥ LV_EVENT_COVER_CHECK:
        അവസ്ഥ LV_EVENT_REFR_EXT_DRAW_SIZE:
        അവസ്ഥ LV_EVENT_DRAW_MAIN_BEGIN:
        അവസ്ഥ LV_EVENT_DRAW_MAIN:
        അവസ്ഥ LV_EVENT_DRAW_MAIN_END:
        അവസ്ഥ LV_EVENT_DRAW_POST_BEGIN:
        അവസ്ഥ LV_EVENT_DRAW_POST:
        അവസ്ഥ LV_EVENT_DRAW_POST_END:
        അവസ്ഥ LV_EVENT_DRAW_TASK_ADDED:
        അവസ്ഥ LV_EVENT_REFRESH:
        അവസ്ഥ LV_EVENT_DELETE:
        അവസ്ഥ LV_EVENT_CHILD_CREATED:
        അവസ്ഥ LV_EVENT_CHILD_DELETED:
        അവസ്ഥ LV_EVENT_CHILD_CHANGED:
        അവസ്ഥ LV_EVENT_SIZE_CHANGED:
        അവസ്ഥ LV_EVENT_STYLE_CHANGED:
        അവസ്ഥ LV_EVENT_GET_SELF_SIZE:
            തിരികെ false;
        സ്ഥിരസ്ഥിതി:
            തിരികെ true;
    }
}

#എങ്കിൽ LV_USE_CHECK_ARG
സ്ഥിതം bool event_code_in_array(lv_event_code_t code, സ്ഥിരം lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    വേണ്ടി(i = 0; i < len; i++) {
        എങ്കിൽ(code == arr[i]) തിരികെ true;
    }
    തിരികെ false;
}
#endif
