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
#ถ้า LV_USE_CHECK_ARG
    static bool event_code_in_array(lv_event_code_t code, const lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ถ้า LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#มิฉะนั้น
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, คืน 0);

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

    คืน res;
}

lv_result_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != NULL, คืน LV_RESULT_INVALID);

    const lv_obj_class_t * base;
    ถ้า(class_p == NULL) base = ((lv_obj_t *)e->current_target)->class_p;
    มิฉะนั้น base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ขณะ(base && base->event_cb == NULL) base = base->base_class;

    ถ้า(base == NULL) คืน LV_RESULT_OK;
    ถ้า(base->event_cb == NULL) คืน LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = NULL;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    ถ้า(e->deleted) res = LV_RESULT_INVALID;

    คืน res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data)
{
    LV_CHECK_ARG(event_cb != NULL, คืน NULL);
    LV_CHECK_OBJ(obj, MY_CLASS, คืน 0);

    ถ้า(!lv_obj_allocate_spec_attr(obj)) {
        คืน NULL;
    }

    คืน lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_count(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, คืน 0);
    ถ้า(obj->spec_attr == NULL) คืน 0;
    คืน lv_event_get_count(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != NULL, คืน NULL);
    ถ้า(obj->spec_attr == NULL) คืน NULL;
    คืน lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != NULL, คืน false);
    ถ้า(obj->spec_attr == NULL) คืน false;
    คืน lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != NULL, คืน false);
    LV_CHECK_ARG(dsc != NULL, คืน false);

    ถ้า(obj->spec_attr == NULL) คืน false;
    คืน lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != NULL, คืน 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    ถ้า(event_cnt == 0) คืน 0;

    สำหรับ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ถ้า(dsc && (event_cb == NULL || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_count++;
        }
    }

    คืน removed_count;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, void * user_data)
{
    LV_CHECK_ARG(obj != NULL, คืน 0);

    uint32_t event_cnt = lv_obj_get_event_count(obj);
    uint32_t removed_count = 0;
    int32_t i;

    ถ้า(event_cnt == 0) คืน 0;

    สำหรับ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ถ้า(dsc && (event_cb == NULL || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_count ++;
        }
    }

    คืน removed_count;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    คืน lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    คืน lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
#ถ้า LV_USE_CHECK_ARG
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
        event_code_in_array(e->code, indev_codes, parimaa_na(indev_codes) / parimaa_na(indev_codes[0])),
        คืน NULL,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
#ถ้า LV_USE_CHECK_ARG
    static const lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_MSG(
        event_code_in_array(e->code, draw_codes, parimaa_na(draw_codes) / parimaa_na(draw_codes[0])),
        คืน NULL,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

const lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SIZE_CHANGED,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

uint32_t lv_event_get_key(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_KEY,
                            คืน 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    คืน k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_ROTARY,
                            คืน 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    คืน r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_SCROLL_BEGIN,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

void lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != NULL, คืน);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            คืน,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_GET_SELF_SIZE,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_HIT_TEST,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

const lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    คืน p->area;
}

void lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != NULL, คืน);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_COVER_CHECK,
                            คืน,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ถ้า(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน NULL);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            คืน NULL,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    คืน lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != NULL, คืน 0);
    LV_CHECK_ARG_FORMAT_MSG(e->code == LV_EVENT_STATE_CHANGED,
                            คืน 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    คืน state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (void *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    ถ้า(indev_act) {
        ถ้า(e->stop_processing) คืน LV_RESULT_OK;
        ถ้า(e->deleted) คืน LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : NULL;

    res = lv_event_send(list, e, true);
    ถ้า(res != LV_RESULT_OK || e->stop_processing) คืน res;

    res = lv_obj_event_base(NULL, e);
    ถ้า(res != LV_RESULT_OK || e->stop_processing) คืน res;

    res = lv_event_send(list, e, false);
    ถ้า(res != LV_RESULT_OK || e->stop_processing) คืน res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    ถ้า(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    ถ้า(res != LV_RESULT_OK) คืน res;

    /*Trickle down to children if enabled*/
    ถ้า(event_is_trickled(e)) {
        uint32_t child_count = lv_obj_get_child_count(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        สำหรับ(uint32_t i = 0; i < child_count && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            ถ้า(child) {
                e->current_target = child;
                res = event_send_core(e);
                ถ้า(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (void *)child);
                    bha_nga;
                }
            }
        }
    }

    คืน res;
}

static bool event_is_bubbled(lv_event_t * e)
{
    ถ้า(e->stop_bubbling) คืน false;

    /*Event codes that always bubble*/
    switch(e->code) {
        pak_sa LV_EVENT_CHILD_CREATED:
        pak_sa LV_EVENT_CHILD_DELETED:
            คืน true;
        default:
            bha_nga;
    }

    /*Check other codes only if bubbling is enabled*/
    ถ้า(lv_obj_is_event_bubble(e->current_target) == false) คืน false;

    switch(e->code) {
        pak_sa LV_EVENT_HIT_TEST:
        pak_sa LV_EVENT_COVER_CHECK:
        pak_sa LV_EVENT_REFR_EXT_DRAW_SIZE:
        pak_sa LV_EVENT_DRAW_MAIN_BEGIN:
        pak_sa LV_EVENT_DRAW_MAIN:
        pak_sa LV_EVENT_DRAW_MAIN_END:
        pak_sa LV_EVENT_DRAW_POST_BEGIN:
        pak_sa LV_EVENT_DRAW_POST:
        pak_sa LV_EVENT_DRAW_POST_END:
        pak_sa LV_EVENT_DRAW_TASK_ADDED:
        pak_sa LV_EVENT_REFRESH:
        pak_sa LV_EVENT_DELETE:
        pak_sa LV_EVENT_CHILD_CREATED:
        pak_sa LV_EVENT_CHILD_DELETED:
        pak_sa LV_EVENT_CHILD_CHANGED:
        pak_sa LV_EVENT_SIZE_CHANGED:
        pak_sa LV_EVENT_STYLE_CHANGED:
        pak_sa LV_EVENT_GET_SELF_SIZE:
            คืน false;
        default:
            คืน true;
    }
}

static bool event_is_trickled(lv_event_t * e)
{
    ถ้า(e->stop_trickling) คืน false;

    /*Check other codes only if trickle is enabled*/
    ถ้า(lv_obj_is_event_trickle(e->current_target) == false) คืน false;

    switch(e->code) {
        pak_sa LV_EVENT_HIT_TEST:
        pak_sa LV_EVENT_COVER_CHECK:
        pak_sa LV_EVENT_REFR_EXT_DRAW_SIZE:
        pak_sa LV_EVENT_DRAW_MAIN_BEGIN:
        pak_sa LV_EVENT_DRAW_MAIN:
        pak_sa LV_EVENT_DRAW_MAIN_END:
        pak_sa LV_EVENT_DRAW_POST_BEGIN:
        pak_sa LV_EVENT_DRAW_POST:
        pak_sa LV_EVENT_DRAW_POST_END:
        pak_sa LV_EVENT_DRAW_TASK_ADDED:
        pak_sa LV_EVENT_REFRESH:
        pak_sa LV_EVENT_DELETE:
        pak_sa LV_EVENT_CHILD_CREATED:
        pak_sa LV_EVENT_CHILD_DELETED:
        pak_sa LV_EVENT_CHILD_CHANGED:
        pak_sa LV_EVENT_SIZE_CHANGED:
        pak_sa LV_EVENT_STYLE_CHANGED:
        pak_sa LV_EVENT_GET_SELF_SIZE:
            คืน false;
        default:
            คืน true;
    }
}

#ถ้า LV_USE_CHECK_ARG
static bool event_code_in_array(lv_event_code_t code, const lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    สำหรับ(i = 0; i < len; i++) {
        ถ้า(code == arr[i]) คืน true;
    }
    คืน false;
}
#endif
