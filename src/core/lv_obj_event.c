/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../misc/lv_event_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_event_private.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../indev/lv_indev_private.h"

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
स्थिर lv_result_t event_send_core(lv_event_t * e);
स्थिर bool event_is_bubbled(lv_event_t * e);
स्थिर bool event_is_trickled(lv_event_t * e);
#जर LV_USE_CHECK_ARG
    स्थिर bool event_code_in_array(lv_event_code_t code, स्थिरांक lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#जर LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#अन्यथा
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, शून्य * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = रिक्त;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ढकला(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_काढा(&e);

    परत res;
}

lv_result_t lv_obj_event_base(स्थिरांक lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != रिक्त, परत LV_RESULT_INVALID);

    स्थिरांक lv_obj_class_t * base;
    जर(class_p == रिक्त) base = ((lv_obj_t *)e->current_target)->class_p;
    अन्यथा base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    पर्यंत(base && base->event_cb == रिक्त) base = base->base_class;

    जर(base == रिक्त) परत LV_RESULT_OK;
    जर(base->event_cb == रिक्त) परत LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = रिक्त;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    जर(e->deleted) res = LV_RESULT_INVALID;

    परत res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, शून्य * user_data)
{
    LV_CHECK_ARG(event_cb != रिक्त, परत रिक्त);
    LV_CHECK_OBJ(obj, MY_CLASS, परत 0);

    जर(!lv_obj_allocate_spec_attr(obj)) {
        परत रिक्त;
    }

    परत lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_मोजणी(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, परत 0);
    जर(obj->spec_attr == रिक्त) परत 0;
    परत lv_event_get_मोजणी(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    जर(obj->spec_attr == रिक्त) परत रिक्त;
    परत lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != रिक्त, परत false);
    जर(obj->spec_attr == रिक्त) परत false;
    परत lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != रिक्त, परत false);
    LV_CHECK_ARG(dsc != रिक्त, परत false);

    जर(obj->spec_attr == रिक्त) परत false;
    परत lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत 0);

    uint32_t event_cnt = lv_obj_get_event_मोजणी(obj);
    uint32_t removed_मोजणी = 0;
    int32_t i;

    जर(event_cnt == 0) परत 0;

    साठी(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        जर(dsc && (event_cb == रिक्त || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_मोजणी++;
        }
    }

    परत removed_मोजणी;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, शून्य * user_data)
{
    LV_CHECK_ARG(obj != रिक्त, परत 0);

    uint32_t event_cnt = lv_obj_get_event_मोजणी(obj);
    uint32_t removed_मोजणी = 0;
    int32_t i;

    जर(event_cnt == 0) परत 0;

    साठी(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        जर(dsc && (event_cb == रिक्त || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_मोजणी ++;
        }
    }

    परत removed_मोजणी;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    परत lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    परत lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
#जर LV_USE_CHECK_ARG
    स्थिर स्थिरांक lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_डावा,      LV_EVENT_GESTURE_उजवा,
        LV_EVENT_किल्ली,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_संदेश(
        event_code_in_array(e->code, indev_codes, आकार(indev_codes) / आकार(indev_codes[0])),
        परत रिक्त,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
#जर LV_USE_CHECK_ARG
    स्थिर स्थिरांक lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_संदेश(
        event_code_in_array(e->code, draw_codes, आकार(draw_codes) / आकार(draw_codes[0])),
        परत रिक्त,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

स्थिरांक lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_SIZE_CHANGED,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

uint32_t lv_event_get_किल्ली(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_किल्ली,
                            परत 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    परत k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_ROTARY,
                            परत 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    परत r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_SCROLL_BEGIN,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

शून्य lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != रिक्त, परत);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            परत,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_GET_SELF_SIZE,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_HIT_TEST,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

स्थिरांक lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_COVER_CHECK,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    परत p->area;
}

शून्य lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != रिक्त, परत);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_COVER_CHECK,
                            परत,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    जर(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            परत रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    परत lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, परत 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_STATE_CHANGED,
                            परत 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    परत state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (शून्य *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    जर(indev_act) {
        जर(e->stop_processing) परत LV_RESULT_OK;
        जर(e->deleted) परत LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : रिक्त;

    res = lv_event_send(list, e, true);
    जर(res != LV_RESULT_OK || e->stop_processing) परत res;

    res = lv_obj_event_base(रिक्त, e);
    जर(res != LV_RESULT_OK || e->stop_processing) परत res;

    res = lv_event_send(list, e, false);
    जर(res != LV_RESULT_OK || e->stop_processing) परत res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    जर(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    जर(res != LV_RESULT_OK) परत res;

    /*Trickle down to children if enabled*/
    जर(event_is_trickled(e)) {
        uint32_t child_मोजणी = lv_obj_get_child_मोजणी(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        साठी(uint32_t i = 0; i < child_मोजणी && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            जर(child) {
                e->current_target = child;
                res = event_send_core(e);
                जर(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (शून्य *)child);
                    थांबा;
                }
            }
        }
    }

    परत res;
}

स्थिर bool event_is_bubbled(lv_event_t * e)
{
    जर(e->stop_bubbling) परत false;

    /*Event codes that always bubble*/
    निवड(e->code) {
        स्थिती LV_EVENT_CHILD_CREATED:
        स्थिती LV_EVENT_CHILD_DELETED:
            परत true;
        पूर्वनिर्धारित:
            थांबा;
    }

    /*Check other codes only if bubbling is enabled*/
    जर(lv_obj_is_event_bubble(e->current_target) == false) परत false;

    निवड(e->code) {
        स्थिती LV_EVENT_HIT_TEST:
        स्थिती LV_EVENT_COVER_CHECK:
        स्थिती LV_EVENT_REFR_EXT_DRAW_SIZE:
        स्थिती LV_EVENT_DRAW_MAIN_BEGIN:
        स्थिती LV_EVENT_DRAW_MAIN:
        स्थिती LV_EVENT_DRAW_MAIN_END:
        स्थिती LV_EVENT_DRAW_POST_BEGIN:
        स्थिती LV_EVENT_DRAW_POST:
        स्थिती LV_EVENT_DRAW_POST_END:
        स्थिती LV_EVENT_DRAW_TASK_ADDED:
        स्थिती LV_EVENT_REFRESH:
        स्थिती LV_EVENT_DELETE:
        स्थिती LV_EVENT_CHILD_CREATED:
        स्थिती LV_EVENT_CHILD_DELETED:
        स्थिती LV_EVENT_CHILD_CHANGED:
        स्थिती LV_EVENT_SIZE_CHANGED:
        स्थिती LV_EVENT_STYLE_CHANGED:
        स्थिती LV_EVENT_GET_SELF_SIZE:
            परत false;
        पूर्वनिर्धारित:
            परत true;
    }
}

स्थिर bool event_is_trickled(lv_event_t * e)
{
    जर(e->stop_trickling) परत false;

    /*Check other codes only if trickle is enabled*/
    जर(lv_obj_is_event_trickle(e->current_target) == false) परत false;

    निवड(e->code) {
        स्थिती LV_EVENT_HIT_TEST:
        स्थिती LV_EVENT_COVER_CHECK:
        स्थिती LV_EVENT_REFR_EXT_DRAW_SIZE:
        स्थिती LV_EVENT_DRAW_MAIN_BEGIN:
        स्थिती LV_EVENT_DRAW_MAIN:
        स्थिती LV_EVENT_DRAW_MAIN_END:
        स्थिती LV_EVENT_DRAW_POST_BEGIN:
        स्थिती LV_EVENT_DRAW_POST:
        स्थिती LV_EVENT_DRAW_POST_END:
        स्थिती LV_EVENT_DRAW_TASK_ADDED:
        स्थिती LV_EVENT_REFRESH:
        स्थिती LV_EVENT_DELETE:
        स्थिती LV_EVENT_CHILD_CREATED:
        स्थिती LV_EVENT_CHILD_DELETED:
        स्थिती LV_EVENT_CHILD_CHANGED:
        स्थिती LV_EVENT_SIZE_CHANGED:
        स्थिती LV_EVENT_STYLE_CHANGED:
        स्थिती LV_EVENT_GET_SELF_SIZE:
            परत false;
        पूर्वनिर्धारित:
            परत true;
    }
}

#जर LV_USE_CHECK_ARG
स्थिर bool event_code_in_array(lv_event_code_t code, स्थिरांक lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    साठी(i = 0; i < len; i++) {
        जर(code == arr[i]) परत true;
    }
    परत false;
}
#endif
