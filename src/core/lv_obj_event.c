/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../misc/lv_event_private.h"
#include "../lvgl_public.h"
#include "lv_obj_event_private.h"
#include "lv_obj_class_private.h"
#include "lv_obj_private.h"
#include "../indev/lv_indev_private.h"

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
static lv_result_t event_send_core(lv_event_t * e);
static bool event_is_bubbled(lv_event_t * e);
static bool event_is_trickled(lv_event_t * e);
#если LV_USE_CHECK_ARG
    static bool event_code_in_array(lv_event_code_t code, const lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#если LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#иначе
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, вернуть 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = NULL;
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

    вернуть res;
}

lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != NULL, вернуть LV_RESULT_INVALID);

    const lv_obj_class_t * base;
    если(class_p == NULL) base = ((lv_obj_t *)e->current_target)->class_p;
    иначе base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    пока(base && base->event_cb == NULL) base = base->base_class;

    если(base == NULL) вернуть LV_RESULT_OK;
    если(base->event_cb == NULL) вернуть LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = NULL;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    если(e->deleted) res = LV_RESULT_INVALID;

    вернуть res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
{
    LV_CHECK_ARG(event_cb != NULL, вернуть NULL);
    LV_CHECK_OBJ(obj, MY_CLASS, вернуть 0);

    если(!lv_obj_allocate_spec_attr(obj)) {
        вернуть NULL;
    }

    вернуть lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_count(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, вернуть 0);
    если(obj->spec_attr == NULL) вернуть 0;
    вернуть lv_event_get_count(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    если(obj->spec_attr == NULL) вернуть NULL;
    вернуть lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != NULL, вернуть false);
    если(obj->spec_attr == NULL) вернуть false;
    вернуть lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != NULL, вернуть false);
    LV_CHECK_ARG(dsc != NULL, вернуть false);

    если(obj->spec_attr == NULL) вернуть false;
    вернуть lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    если(event_cnt == 0) вернуть 0;

    для(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        если(dsc && (event_cb == NULL || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_count++;
        }
    }

    вернуть removed_count;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)
{
    LV_CHECK_ARG(obj != NULL, вернуть 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    если(event_cnt == 0) вернуть 0;

    для(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        если(dsc && (event_cb == NULL || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_count ++;
        }
    }

    вернуть removed_count;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    вернуть lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    вернуть lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
#если LV_USE_CHECK_ARG
    static const lv_event_code_t indev_codes[] = {
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
        event_code_in_array(e->code, indev_codes, sizeof(indev_codes) / sizeof(indev_codes[0])),
        вернуть NULL,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
#если LV_USE_CHECK_ARG
    static const lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_MSG(
        event_code_in_array(e->code, draw_codes, sizeof(draw_codes) / sizeof(draw_codes[0])),
        вернуть NULL,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

const lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SIZE_CHANGED,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

uint32_t lv_event_get_key(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_KEY,
                            вернуть 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    вернуть k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_ROTARY,
                            вернуть 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    вернуть r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SCROLL_BEGIN,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

void lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != NULL, вернуть);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            вернуть,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_GET_SELF_SIZE,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_HIT_TEST,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

const lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    вернуть p->area;
}

void lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != NULL, вернуть);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            вернуть,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    если(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            вернуть NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    вернуть lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, вернуть 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_STATE_CHANGED,
                            вернуть 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    вернуть state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (void *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    если(indev_act) {
        если(e->stop_processing) вернуть LV_RESULT_OK;
        если(e->deleted) вернуть LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : NULL;

    res = lv_event_send(list, e, true);
    если(res != LV_RESULT_OK || e->stop_processing) вернуть res;

    res = lv_obj_event_base(NULL, e);
    если(res != LV_RESULT_OK || e->stop_processing) вернуть res;

    res = lv_event_send(list, e, false);
    если(res != LV_RESULT_OK || e->stop_processing) вернуть res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    если(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    если(res != LV_RESULT_OK) вернуть res;

    /*Trickle down to children if enabled*/
    если(event_is_trickled(e)) {
        uint32_t child_count = lv_obj_get_child_count(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        для(uint32_t i = 0; i < child_count && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            если(child) {
                e->current_target = child;
                res = event_send_core(e);
                если(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (void *)child);
                    break;
                }
            }
        }
    }

    вернуть res;
}

static bool event_is_bubbled(lv_event_t * e)
{
    если(e->stop_bubbling) вернуть false;

    /*Event codes that always bubble*/
    switch(e->code) {
        case LV_EVENT_CHILD_CREATED:
        case LV_EVENT_CHILD_DELETED:
            вернуть true;
        default:
            break;
    }

    /*Check other codes only if bubbling is enabled*/
    если(lv_obj_is_event_bubble(e->current_target) == false) вернуть false;

    switch(e->code) {
        case LV_EVENT_HIT_TEST:
        case LV_EVENT_COVER_CHECK:
        case LV_EVENT_REFR_EXT_DRAW_SIZE:
        case LV_EVENT_DRAW_MAIN_BEGIN:
        case LV_EVENT_DRAW_MAIN:
        case LV_EVENT_DRAW_MAIN_END:
        case LV_EVENT_DRAW_POST_BEGIN:
        case LV_EVENT_DRAW_POST:
        case LV_EVENT_DRAW_POST_END:
        case LV_EVENT_DRAW_TASK_ADDED:
        case LV_EVENT_REFRESH:
        case LV_EVENT_DELETE:
        case LV_EVENT_CHILD_CREATED:
        case LV_EVENT_CHILD_DELETED:
        case LV_EVENT_CHILD_CHANGED:
        case LV_EVENT_SIZE_CHANGED:
        case LV_EVENT_STYLE_CHANGED:
        case LV_EVENT_GET_SELF_SIZE:
            вернуть false;
        default:
            вернуть true;
    }
}

static bool event_is_trickled(lv_event_t * e)
{
    если(e->stop_trickling) вернуть false;

    /*Check other codes only if trickle is enabled*/
    если(lv_obj_is_event_trickle(e->current_target) == false) вернуть false;

    switch(e->code) {
        case LV_EVENT_HIT_TEST:
        case LV_EVENT_COVER_CHECK:
        case LV_EVENT_REFR_EXT_DRAW_SIZE:
        case LV_EVENT_DRAW_MAIN_BEGIN:
        case LV_EVENT_DRAW_MAIN:
        case LV_EVENT_DRAW_MAIN_END:
        case LV_EVENT_DRAW_POST_BEGIN:
        case LV_EVENT_DRAW_POST:
        case LV_EVENT_DRAW_POST_END:
        case LV_EVENT_DRAW_TASK_ADDED:
        case LV_EVENT_REFRESH:
        case LV_EVENT_DELETE:
        case LV_EVENT_CHILD_CREATED:
        case LV_EVENT_CHILD_DELETED:
        case LV_EVENT_CHILD_CHANGED:
        case LV_EVENT_SIZE_CHANGED:
        case LV_EVENT_STYLE_CHANGED:
        case LV_EVENT_GET_SELF_SIZE:
            вернуть false;
        default:
            вернуть true;
    }
}

#если LV_USE_CHECK_ARG
static bool event_code_in_array(lv_event_code_t code, const lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    для(i = 0; i < len; i++) {
        если(code == arr[i]) вернуть true;
    }
    вернуть false;
}
#endif
