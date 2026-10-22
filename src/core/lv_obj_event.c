/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাৱ "../misc/lv_event_private.h"
#অন্তর্ভাৱ "../lvgl_public.h"
#অন্তর্ভাৱ "lv_obj_event_private.h"
#অন্তর্ভাৱ "lv_obj_class_private.h"
#অন্তর্ভাৱ "lv_obj_private.h"
#অন্তর্ভাৱ "../indev/lv_indev_private.h"

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
স্থির lv_result_t event_send_core(lv_event_t * e);
স্থির bool event_is_bubbled(lv_event_t * e);
স্থির bool event_is_trickled(lv_event_t * e);
#যদি LV_USE_CHECK_ARG
    স্থির bool event_code_in_array(lv_event_code_t code, ধ্রুৱ lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#যদি LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#অন্যথা
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, শূন্য * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ঘূৰাই 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = রিক্ত;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ঠেলো(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_বেৰ_কৰো(&e);

    ঘূৰাই res;
}

lv_result_t lv_obj_event_base(ধ্রুৱ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != রিক্ত, ঘূৰাই LV_RESULT_INVALID);

    ধ্রুৱ lv_obj_class_t * base;
    যদি(class_p == রিক্ত) base = ((lv_obj_t *)e->current_target)->class_p;
    অন্যথা base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    যেতিয়ালৈ(base && base->event_cb == রিক্ত) base = base->base_class;

    যদি(base == রিক্ত) ঘূৰাই LV_RESULT_OK;
    যদি(base->event_cb == রিক্ত) ঘূৰাই LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = রিক্ত;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    যদি(e->deleted) res = LV_RESULT_INVALID;

    ঘূৰাই res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, শূন্য * user_data)
{
    LV_CHECK_ARG(event_cb != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_OBJ(obj, MY_CLASS, ঘূৰাই 0);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_গুনতি(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই 0);
    যদি(obj->spec_attr == রিক্ত) ঘূৰাই 0;
    ঘূৰাই lv_event_get_গুনতি(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই রিক্ত);
    যদি(obj->spec_attr == রিক্ত) ঘূৰাই রিক্ত;
    ঘূৰাই lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই false);
    যদি(obj->spec_attr == রিক্ত) ঘূৰাই false;
    ঘূৰাই lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই false);
    LV_CHECK_ARG(dsc != রিক্ত, ঘূৰাই false);

    যদি(obj->spec_attr == রিক্ত) ঘূৰাই false;
    ঘূৰাই lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই 0);

    uint32_t event_cnt = lv_obj_get_event_গুনতি(obj);
    uint32_t removed_গুনতি = 0;
    int32_t i;

    যদি(event_cnt == 0) ঘূৰাই 0;

    বাবে(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        যদি(dsc && (event_cb == রিক্ত || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_গুনতি++;
        }
    }

    ঘূৰাই removed_গুনতি;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, শূন্য * user_data)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই 0);

    uint32_t event_cnt = lv_obj_get_event_গুনতি(obj);
    uint32_t removed_গুনতি = 0;
    int32_t i;

    যদি(event_cnt == 0) ঘূৰাই 0;

    বাবে(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        যদি(dsc && (event_cb == রিক্ত || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_গুনতি ++;
        }
    }

    ঘূৰাই removed_গুনতি;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    ঘূৰাই lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    ঘূৰাই lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
#যদি LV_USE_CHECK_ARG
    স্থির ধ্রুৱ lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_বাম,      LV_EVENT_GESTURE_ডান,
        LV_EVENT_চাবি,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_বাৰ্তা(
        event_code_in_array(e->code, indev_codes, পরিমাণ(indev_codes) / পরিমাণ(indev_codes[0])),
        ঘূৰাই রিক্ত,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
#যদি LV_USE_CHECK_ARG
    স্থির ধ্রুৱ lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_বাৰ্তা(
        event_code_in_array(e->code, draw_codes, পরিমাণ(draw_codes) / পরিমাণ(draw_codes[0])),
        ঘূৰাই রিক্ত,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

ধ্রুৱ lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_SIZE_CHANGED,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

uint32_t lv_event_get_চাবি(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই 0);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_চাবি,
                            ঘূৰাই 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    ঘূৰাই k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই 0);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_ROTARY,
                            ঘূৰাই 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    ঘূৰাই r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_SCROLL_BEGIN,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

শূন্য lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            ঘূৰাই,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_GET_SELF_SIZE,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_HIT_TEST,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

ধ্রুৱ lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_COVER_CHECK,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ঘূৰাই p->area;
}

শূন্য lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_COVER_CHECK,
                            ঘূৰাই,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    যদি(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই রিক্ত);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            ঘূৰাই রিক্ত,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ঘূৰাই lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != রিক্ত, ঘূৰাই 0);
    LV_CHECK_ARG_FORMAT_বাৰ্তা(e->code == LV_EVENT_STATE_CHANGED,
                            ঘূৰাই 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    ঘূৰাই state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (শূন্য *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    যদি(indev_act) {
        যদি(e->stop_processing) ঘূৰাই LV_RESULT_OK;
        যদি(e->deleted) ঘূৰাই LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : রিক্ত;

    res = lv_event_send(list, e, true);
    যদি(res != LV_RESULT_OK || e->stop_processing) ঘূৰাই res;

    res = lv_obj_event_base(রিক্ত, e);
    যদি(res != LV_RESULT_OK || e->stop_processing) ঘূৰাই res;

    res = lv_event_send(list, e, false);
    যদি(res != LV_RESULT_OK || e->stop_processing) ঘূৰাই res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    যদি(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    যদি(res != LV_RESULT_OK) ঘূৰাই res;

    /*Trickle down to children if enabled*/
    যদি(event_is_trickled(e)) {
        uint32_t child_গুনতি = lv_obj_get_child_গুনতি(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        বাবে(uint32_t i = 0; i < child_গুনতি && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            যদি(child) {
                e->current_target = child;
                res = event_send_core(e);
                যদি(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (শূন্য *)child);
                    ভঙ্গ;
                }
            }
        }
    }

    ঘূৰাই res;
}

স্থির bool event_is_bubbled(lv_event_t * e)
{
    যদি(e->stop_bubbling) ঘূৰাই false;

    /*Event codes that always bubble*/
    ৱিকল্প(e->code) {
        পক্ষ LV_EVENT_CHILD_CREATED:
        পক্ষ LV_EVENT_CHILD_DELETED:
            ঘূৰাই true;
        স্ৱভাৱ:
            ভঙ্গ;
    }

    /*Check other codes only if bubbling is enabled*/
    যদি(lv_obj_is_event_bubble(e->current_target) == false) ঘূৰাই false;

    ৱিকল্প(e->code) {
        পক্ষ LV_EVENT_HIT_TEST:
        পক্ষ LV_EVENT_COVER_CHECK:
        পক্ষ LV_EVENT_REFR_EXT_DRAW_SIZE:
        পক্ষ LV_EVENT_DRAW_MAIN_BEGIN:
        পক্ষ LV_EVENT_DRAW_MAIN:
        পক্ষ LV_EVENT_DRAW_MAIN_END:
        পক্ষ LV_EVENT_DRAW_POST_BEGIN:
        পক্ষ LV_EVENT_DRAW_POST:
        পক্ষ LV_EVENT_DRAW_POST_END:
        পক্ষ LV_EVENT_DRAW_TASK_ADDED:
        পক্ষ LV_EVENT_REFRESH:
        পক্ষ LV_EVENT_DELETE:
        পক্ষ LV_EVENT_CHILD_CREATED:
        পক্ষ LV_EVENT_CHILD_DELETED:
        পক্ষ LV_EVENT_CHILD_CHANGED:
        পক্ষ LV_EVENT_SIZE_CHANGED:
        পক্ষ LV_EVENT_STYLE_CHANGED:
        পক্ষ LV_EVENT_GET_SELF_SIZE:
            ঘূৰাই false;
        স্ৱভাৱ:
            ঘূৰাই true;
    }
}

স্থির bool event_is_trickled(lv_event_t * e)
{
    যদি(e->stop_trickling) ঘূৰাই false;

    /*Check other codes only if trickle is enabled*/
    যদি(lv_obj_is_event_trickle(e->current_target) == false) ঘূৰাই false;

    ৱিকল্প(e->code) {
        পক্ষ LV_EVENT_HIT_TEST:
        পক্ষ LV_EVENT_COVER_CHECK:
        পক্ষ LV_EVENT_REFR_EXT_DRAW_SIZE:
        পক্ষ LV_EVENT_DRAW_MAIN_BEGIN:
        পক্ষ LV_EVENT_DRAW_MAIN:
        পক্ষ LV_EVENT_DRAW_MAIN_END:
        পক্ষ LV_EVENT_DRAW_POST_BEGIN:
        পক্ষ LV_EVENT_DRAW_POST:
        পক্ষ LV_EVENT_DRAW_POST_END:
        পক্ষ LV_EVENT_DRAW_TASK_ADDED:
        পক্ষ LV_EVENT_REFRESH:
        পক্ষ LV_EVENT_DELETE:
        পক্ষ LV_EVENT_CHILD_CREATED:
        পক্ষ LV_EVENT_CHILD_DELETED:
        পক্ষ LV_EVENT_CHILD_CHANGED:
        পক্ষ LV_EVENT_SIZE_CHANGED:
        পক্ষ LV_EVENT_STYLE_CHANGED:
        পক্ষ LV_EVENT_GET_SELF_SIZE:
            ঘূৰাই false;
        স্ৱভাৱ:
            ঘূৰাই true;
    }
}

#যদি LV_USE_CHECK_ARG
স্থির bool event_code_in_array(lv_event_code_t code, ধ্রুৱ lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    বাবে(i = 0; i < len; i++) {
        যদি(code == arr[i]) ঘূৰাই true;
    }
    ঘূৰাই false;
}
#endif
