/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "../misc/lv_event_private.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_event_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_class_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../indev/lv_indev_private.h"

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
ସ୍ଥିର lv_result_t event_send_core(lv_event_t * e);
ସ୍ଥିର bool event_is_bubbled(lv_event_t * e);
ସ୍ଥିର bool event_is_trickled(lv_event_t * e);
#ଯଦି LV_USE_CHECK_ARG
    ସ୍ଥିର bool event_code_in_array(lv_event_code_t code, ଅଚଳ lv_event_code_t * arr, uint32_t len);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ଯଦି LV_USE_LOG && LV_LOG_TRACE_EVENT
    #define LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ଅନ୍ୟଥା
    #define LV_TRACE_EVENT(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, ଶୂନ୍ୟ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_data = ଖାଲି;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ଠେଲ(&e);

    /*Send the event*/
    lv_result_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_କାଢ଼(&e);

    ଫେରନ୍ତୁ res;
}

lv_result_t lv_obj_event_base(ଅଚଳ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);
    LV_CHECK_ARG(e->current_target != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);

    ଅଚଳ lv_obj_class_t * base;
    ଯଦି(class_p == ଖାଲି) base = ((lv_obj_t *)e->current_target)->class_p;
    ଅନ୍ୟଥା base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ଯେତେବେଳେ(base && base->event_cb == ଖାଲି) base = base->base_class;

    ଯଦି(base == ଖାଲି) ଫେରନ୍ତୁ LV_RESULT_OK;
    ଯଦି(base->event_cb == ଖାଲି) ଫେରନ୍ତୁ LV_RESULT_OK;

    /*Call the actual event callback*/
    e->user_data = ଖାଲି;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_result_t res = LV_RESULT_OK;
    /*Stop if the object is deleted*/
    ଯଦି(e->deleted) res = LV_RESULT_INVALID;

    ଫେରନ୍ତୁ res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(event_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ 0);

    ଯଦି(!lv_obj_allocate_spec_attr(obj)) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_data);
}

uint32_t lv_obj_get_event_ଗଣନା(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ 0);
    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ lv_event_get_ଗଣନା(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;
    ଫେରନ୍ତୁ lv_event_get_dsc(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t index)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ false);
    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ false;
    ଫେରନ୍ତୁ lv_event_remove(&obj->spec_attr->event_list, index);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ false);
    LV_CHECK_ARG(dsc != ଖାଲି, ଫେରନ୍ତୁ false);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ false;
    ଫେରନ୍ତୁ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ 0);

    uint32_t event_cnt = lv_obj_get_event_ଗଣନା(obj);
    uint32_t removed_ଗଣନା = 0;
    int32_t i;

    ଯଦି(event_cnt == 0) ଫେରନ୍ତୁ 0;

    ପାଇଁ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ଯଦି(dsc && (event_cb == ଖାଲି || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_ଗଣନା++;
        }
    }

    ଫେରନ୍ତୁ removed_ଗଣନା;
}

uint32_t lv_obj_remove_event_cb_with_user_data(lv_obj_t * obj, lv_event_cb_t event_cb, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ 0);

    uint32_t event_cnt = lv_obj_get_event_ଗଣନା(obj);
    uint32_t removed_ଗଣନା = 0;
    int32_t i;

    ଯଦି(event_cnt == 0) ଫେରନ୍ତୁ 0;

    ପାଇଁ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ଯଦି(dsc && (event_cb == ଖାଲି || dsc->cb == event_cb) && dsc->user_data == user_data) {
            lv_obj_remove_event(obj, i);
            removed_ଗଣନା ++;
        }
    }

    ଫେରନ୍ତୁ removed_ଗଣନା;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଫେରନ୍ତୁ lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଫେରନ୍ତୁ lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
#ଯଦି LV_USE_CHECK_ARG
    ସ୍ଥିର ଅଚଳ lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_ବାମ,      LV_EVENT_GESTURE_ଡାହାଣ,
        LV_EVENT_ଚାବି,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#endif
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(
        event_code_in_array(e->code, indev_codes, ଆକାର(indev_codes) / ଆକାର(indev_codes[0])),
        ଫେରନ୍ତୁ ଖାଲି,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
#ଯଦି LV_USE_CHECK_ARG
    ସ୍ଥିର ଅଚଳ lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#endif
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(
        event_code_in_array(e->code, draw_codes, ଆକାର(draw_codes) / ଆକାର(draw_codes[0])),
        ଫେରନ୍ତୁ ଖାଲି,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

ଅଚଳ lv_area_t * lv_event_get_old_size(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_SIZE_CHANGED,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

uint32_t lv_event_get_ଚାବି(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_ଚାବି,
                            ଫେରନ୍ତୁ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    ଫେରନ୍ତୁ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_ROTARY,
                            ଫେରନ୍ତୁ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    ଫେରନ୍ତୁ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_SCROLL_BEGIN,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

ଶୂନ୍ୟ lv_event_set_ext_draw_size(lv_event_t * e, int32_t size)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_REFR_EXT_DRAW_SIZE,
                            ଫେରନ୍ତୁ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_size = lv_event_get_param(e);
    *cur_size = LV_MAX(*cur_size, size);
}

lv_point_t * lv_event_get_self_size_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_GET_SELF_SIZE,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_HIT_TEST,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

ଅଚଳ lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_COVER_CHECK,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ଫେରନ୍ତୁ p->area;
}

ଶୂନ୍ୟ lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_COVER_CHECK,
                            ଫେରନ୍ତୁ,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ଯଦି(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            ଫେରନ୍ତୁ ଖାଲି,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    ଫେରନ୍ତୁ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ଖାଲି, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(e->code == LV_EVENT_STATE_CHANGED,
                            ଫେରନ୍ତୁ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    ଫେରନ୍ତୁ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ସ୍ଥିର lv_result_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (ଶୂନ୍ୟ *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    ଯଦି(indev_act) {
        ଯଦି(e->stop_processing) ଫେରନ୍ତୁ LV_RESULT_OK;
        ଯଦି(e->deleted) ଫେରନ୍ତୁ LV_RESULT_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_result_t res = LV_RESULT_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : ଖାଲି;

    res = lv_event_send(list, e, true);
    ଯଦି(res != LV_RESULT_OK || e->stop_processing) ଫେରନ୍ତୁ res;

    res = lv_obj_event_base(ଖାଲି, e);
    ଯଦି(res != LV_RESULT_OK || e->stop_processing) ଫେରନ୍ତୁ res;

    res = lv_event_send(list, e, false);
    ଯଦି(res != LV_RESULT_OK || e->stop_processing) ଫେରନ୍ତୁ res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    ଯଦି(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ res;

    /*Trickle down to children if enabled*/
    ଯଦି(event_is_trickled(e)) {
        uint32_t child_ଗଣନା = lv_obj_get_child_ଗଣନା(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        ପାଇଁ(uint32_t i = 0; i < child_ଗଣନା && res == LV_RESULT_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            ଯଦି(child) {
                e->current_target = child;
                res = event_send_core(e);
                ଯଦି(res != LV_RESULT_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (ଶୂନ୍ୟ *)child);
                    ଭାଙ୍ଗ;
                }
            }
        }
    }

    ଫେରନ୍ତୁ res;
}

ସ୍ଥିର bool event_is_bubbled(lv_event_t * e)
{
    ଯଦି(e->stop_bubbling) ଫେରନ୍ତୁ false;

    /*Event codes that always bubble*/
    ବାଛ(e->code) {
        ସ୍ଥିତି LV_EVENT_CHILD_CREATED:
        ସ୍ଥିତି LV_EVENT_CHILD_DELETED:
            ଫେରନ୍ତୁ true;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
    }

    /*Check other codes only if bubbling is enabled*/
    ଯଦି(lv_obj_is_event_bubble(e->current_target) == false) ଫେରନ୍ତୁ false;

    ବାଛ(e->code) {
        ସ୍ଥିତି LV_EVENT_HIT_TEST:
        ସ୍ଥିତି LV_EVENT_COVER_CHECK:
        ସ୍ଥିତି LV_EVENT_REFR_EXT_DRAW_SIZE:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN_BEGIN:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN_END:
        ସ୍ଥିତି LV_EVENT_DRAW_POST_BEGIN:
        ସ୍ଥିତି LV_EVENT_DRAW_POST:
        ସ୍ଥିତି LV_EVENT_DRAW_POST_END:
        ସ୍ଥିତି LV_EVENT_DRAW_TASK_ADDED:
        ସ୍ଥିତି LV_EVENT_REFRESH:
        ସ୍ଥିତି LV_EVENT_DELETE:
        ସ୍ଥିତି LV_EVENT_CHILD_CREATED:
        ସ୍ଥିତି LV_EVENT_CHILD_DELETED:
        ସ୍ଥିତି LV_EVENT_CHILD_CHANGED:
        ସ୍ଥିତି LV_EVENT_SIZE_CHANGED:
        ସ୍ଥିତି LV_EVENT_STYLE_CHANGED:
        ସ୍ଥିତି LV_EVENT_GET_SELF_SIZE:
            ଫେରନ୍ତୁ false;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଫେରନ୍ତୁ true;
    }
}

ସ୍ଥିର bool event_is_trickled(lv_event_t * e)
{
    ଯଦି(e->stop_trickling) ଫେରନ୍ତୁ false;

    /*Check other codes only if trickle is enabled*/
    ଯଦି(lv_obj_is_event_trickle(e->current_target) == false) ଫେରନ୍ତୁ false;

    ବାଛ(e->code) {
        ସ୍ଥିତି LV_EVENT_HIT_TEST:
        ସ୍ଥିତି LV_EVENT_COVER_CHECK:
        ସ୍ଥିତି LV_EVENT_REFR_EXT_DRAW_SIZE:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN_BEGIN:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN:
        ସ୍ଥିତି LV_EVENT_DRAW_MAIN_END:
        ସ୍ଥିତି LV_EVENT_DRAW_POST_BEGIN:
        ସ୍ଥିତି LV_EVENT_DRAW_POST:
        ସ୍ଥିତି LV_EVENT_DRAW_POST_END:
        ସ୍ଥିତି LV_EVENT_DRAW_TASK_ADDED:
        ସ୍ଥିତି LV_EVENT_REFRESH:
        ସ୍ଥିତି LV_EVENT_DELETE:
        ସ୍ଥିତି LV_EVENT_CHILD_CREATED:
        ସ୍ଥିତି LV_EVENT_CHILD_DELETED:
        ସ୍ଥିତି LV_EVENT_CHILD_CHANGED:
        ସ୍ଥିତି LV_EVENT_SIZE_CHANGED:
        ସ୍ଥିତି LV_EVENT_STYLE_CHANGED:
        ସ୍ଥିତି LV_EVENT_GET_SELF_SIZE:
            ଫେରନ୍ତୁ false;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଫେରନ୍ତୁ true;
    }
}

#ଯଦି LV_USE_CHECK_ARG
ସ୍ଥିର bool event_code_in_array(lv_event_code_t code, ଅଚଳ lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    ପାଇଁ(i = 0; i < len; i++) {
        ଯଦି(code == arr[i]) ଫେରନ୍ତୁ true;
    }
    ଫେରନ୍ତୁ false;
}
#endif
