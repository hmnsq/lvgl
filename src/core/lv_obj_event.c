/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ lv_ਨਤੀਜਾ_t event_send_core(lv_event_t * e);
ਸਥਿਰ bool event_is_bubbled(lv_event_t * e);
ਸਥਿਰ bool event_is_trickled(lv_event_t * e);
#ਜੇ LV_USE_CHECK_ARG
    ਸਥਿਰ bool event_ਕੋਡ_ਵਿੱਚ_array(lv_event_ਕੋਡ_t ਕੋਡ, ਅਚਲ lv_event_ਕੋਡ_t * arr, uint32_t len);
#ਜੇ_ਅੰਤ

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ਜੇ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_TRACE_EVENT(...)
#ਜੇ_ਅੰਤ

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ਨਤੀਜਾ_t lv_obj_send_event(lv_obj_t * obj, lv_event_ਕੋਡ_t event_ਕੋਡ, ਖਾਲੀ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    lv_event_t e;
    e.current_ਨਿਸ਼ਾਨਾ = obj;
    e.original_ਨਿਸ਼ਾਨਾ = obj;
    e.ਕੋਡ = event_ਕੋਡ;
    e.user_ਅੰਕੜੇ = ਨਿਰਾ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ਧੱਕੋ(&e);

    /*Send the event*/
    lv_ਨਤੀਜਾ_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_ਕੱਢੋ(&e);

    ਵਾਪਸ res;
}

lv_ਨਤੀਜਾ_t lv_obj_event_ਅਧਾਰ(ਅਚਲ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);
    LV_CHECK_ARG(e->current_ਨਿਸ਼ਾਨਾ != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    ਅਚਲ lv_obj_class_t * ਅਧਾਰ;
    ਜੇ(class_p == ਨਿਰਾ) ਅਧਾਰ = ((lv_obj_t *)e->current_ਨਿਸ਼ਾਨਾ)->class_p;
    ਨਹੀਂਤਾਂ ਅਧਾਰ = class_p->ਅਧਾਰ_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ਜਦੋਂ(ਅਧਾਰ && ਅਧਾਰ->event_cb == ਨਿਰਾ) ਅਧਾਰ = ਅਧਾਰ->ਅਧਾਰ_class;

    ਜੇ(ਅਧਾਰ == ਨਿਰਾ) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    ਜੇ(ਅਧਾਰ->event_cb == ਨਿਰਾ) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;

    /*Call the actual event callback*/
    e->user_ਅੰਕੜੇ = ਨਿਰਾ;
    LV_PROFILER_EVENT_BEGIN_ਟੈਗ(lv_event_ਕੋਡ_get_name(e->ਕੋਡ));
    ਅਧਾਰ->event_cb(ਅਧਾਰ, e);
    LV_PROFILER_EVENT_ਸਿਰਾ_ਟੈਗ(lv_event_ਕੋਡ_get_name(e->ਕੋਡ));

    lv_ਨਤੀਜਾ_t res = LV_ਨਤੀਜਾ_OK;
    /*Stop if the object is deleted*/
    ਜੇ(e->deleted) res = LV_ਨਤੀਜਾ_INVALID;

    ਵਾਪਸ res;
}

lv_event_dsc_t * lv_obj_ਜੋੜੋ_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_ਕੋਡ_t filter, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(event_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ lv_event_ਜੋੜੋ(&obj->spec_attr->event_list, event_cb, filter, user_ਅੰਕੜੇ);
}

uint32_t lv_obj_get_event_ਗਿਣਤੀ(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ 0);
    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
    ਵਾਪਸ lv_event_get_ਗਿਣਤੀ(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t ਸੂਚਕਾਂਕ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
    ਵਾਪਸ lv_event_get_dsc(&obj->spec_attr->event_list, ਸੂਚਕਾਂਕ);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t ਸੂਚਕਾਂਕ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ false;
    ਵਾਪਸ lv_event_remove(&obj->spec_attr->event_list, ਸੂਚਕਾਂਕ);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(dsc != ਨਿਰਾ, ਵਾਪਸ false);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ false;
    ਵਾਪਸ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ 0);

    uint32_t event_cnt = lv_obj_get_event_ਗਿਣਤੀ(obj);
    uint32_t removed_ਗਿਣਤੀ = 0;
    int32_t i;

    ਜੇ(event_cnt == 0) ਵਾਪਸ 0;

    ਲਈ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ਜੇ(dsc && (event_cb == ਨਿਰਾ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_ਗਿਣਤੀ++;
        }
    }

    ਵਾਪਸ removed_ਗਿਣਤੀ;
}

uint32_t lv_obj_remove_event_cb_with_user_ਅੰਕੜੇ(lv_obj_t * obj, lv_event_cb_t event_cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ 0);

    uint32_t event_cnt = lv_obj_get_event_ਗਿਣਤੀ(obj);
    uint32_t removed_ਗਿਣਤੀ = 0;
    int32_t i;

    ਜੇ(event_cnt == 0) ਵਾਪਸ 0;

    ਲਈ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ਜੇ(dsc && (event_cb == ਨਿਰਾ || dsc->cb == event_cb) && dsc->user_ਅੰਕੜੇ == user_ਅੰਕੜੇ) {
            lv_obj_remove_event(obj, i);
            removed_ਗਿਣਤੀ ++;
        }
    }

    ਵਾਪਸ removed_ਗਿਣਤੀ;
}

lv_obj_t * lv_event_get_current_ਨਿਸ਼ਾਨਾ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ lv_event_get_current_ਨਿਸ਼ਾਨਾ(e);
}

lv_obj_t * lv_event_get_ਨਿਸ਼ਾਨਾ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ lv_event_get_ਨਿਸ਼ਾਨਾ(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
#ਜੇ LV_USE_CHECK_ARG
    ਸਥਿਰ ਅਚਲ lv_event_ਕੋਡ_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_ਗੁਆਚਾ,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_ਸਿਰਾ,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_ਉੱਪਰ,        LV_EVENT_GESTURE_ਥੱਲੇ,
        LV_EVENT_GESTURE_ਖੱਬਾ,      LV_EVENT_GESTURE_ਸੱਜਾ,
        LV_EVENT_ਕੁੰਜੀ,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#ਜੇ_ਅੰਤ
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(
        event_ਕੋਡ_ਵਿੱਚ_array(e->ਕੋਡ, indev_codes, ਆਕਾਰ(indev_codes) / ਆਕਾਰ(indev_codes[0])),
        ਵਾਪਸ ਨਿਰਾ,
        "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
#ਜੇ LV_USE_CHECK_ARG
    ਸਥਿਰ ਅਚਲ lv_event_ਕੋਡ_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_ਸਿਰਾ,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_ਸਿਰਾ,
    };
#ਜੇ_ਅੰਤ
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(
        event_ਕੋਡ_ਵਿੱਚ_array(e->ਕੋਡ, draw_codes, ਆਕਾਰ(draw_codes) / ਆਕਾਰ(draw_codes[0])),
        ਵਾਪਸ ਨਿਰਾ,
        "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

ਅਚਲ lv_area_t * lv_event_get_old_ਮਾਪ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_ਮਾਪ_ਬਦਲਿਆ,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

uint32_t lv_event_get_ਕੁੰਜੀ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ 0);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_ਕੁੰਜੀ,
                            ਵਾਪਸ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    uint32_t * k = lv_event_get_param(e);
    ਵਾਪਸ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ 0);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_ROTARY,
                            ਵਾਪਸ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    int32_t * r = lv_event_get_param(e);
    ਵਾਪਸ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_SCROLL_BEGIN,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

ਖਾਲੀ lv_event_ਸੈੱਟ_ext_draw_ਮਾਪ(lv_event_t * e, int32_t ਮਾਪ)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_REFR_EXT_DRAW_ਮਾਪ,
                            ਵਾਪਸ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    int32_t * cur_ਮਾਪ = lv_event_get_param(e);
    *cur_ਮਾਪ = LV_MAX(*cur_ਮਾਪ, ਮਾਪ);
}

lv_ਬਿੰਦੂ_t * lv_event_get_self_ਮਾਪ_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_GET_SELF_ਮਾਪ,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

lv_ਪ੍ਰਾਪਤੀ_test_info_t * lv_event_get_ਪ੍ਰਾਪਤੀ_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_ਪ੍ਰਾਪਤੀ_TEST,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

ਅਚਲ lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_COVER_CHECK,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ਵਾਪਸ p->area;
}

ਖਾਲੀ lv_event_ਸੈੱਟ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_COVER_CHECK,
                            ਵਾਪਸ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ਜੇ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_DRAW_TASK_ADDED,
                            ਵਾਪਸ ਨਿਰਾ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    ਵਾਪਸ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ਨਿਰਾ, ਵਾਪਸ 0);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(e->ਕੋਡ == LV_EVENT_STATE_ਬਦਲਿਆ,
                            ਵਾਪਸ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ਕੋਡ);
    lv_state_t * state = lv_event_get_param(e);
    ਵਾਪਸ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ lv_ਨਤੀਜਾ_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->ਕੋਡ, (ਖਾਲੀ *)e->original_ਨਿਸ਼ਾਨਾ, e->param);

    lv_indev_t * indev_act = lv_indev_ਸਰਗਰਮ();
    ਜੇ(indev_act) {
        ਜੇ(e->stop_processing) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
        ਜੇ(e->deleted) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }

    lv_obj_t * ਨਿਸ਼ਾਨਾ = e->current_ਨਿਸ਼ਾਨਾ;
    lv_ਨਤੀਜਾ_t res = LV_ਨਤੀਜਾ_OK;
    lv_event_list_t * list = ਨਿਸ਼ਾਨਾ->spec_attr ?  &ਨਿਸ਼ਾਨਾ->spec_attr->event_list : ਨਿਰਾ;

    res = lv_event_send(list, e, true);
    ਜੇ(res != LV_ਨਤੀਜਾ_OK || e->stop_processing) ਵਾਪਸ res;

    res = lv_obj_event_ਅਧਾਰ(ਨਿਰਾ, e);
    ਜੇ(res != LV_ਨਤੀਜਾ_OK || e->stop_processing) ਵਾਪਸ res;

    res = lv_event_send(list, e, false);
    ਜੇ(res != LV_ਨਤੀਜਾ_OK || e->stop_processing) ਵਾਪਸ res;

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(e->current_ਨਿਸ਼ਾਨਾ);
    ਜੇ(ਮਾਪਾ && event_is_bubbled(e)) {
        e->current_ਨਿਸ਼ਾਨਾ = ਮਾਪਾ;
        res = event_send_core(e);
    }
    ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ res;

    /*Trickle down to children if enabled*/
    ਜੇ(event_is_trickled(e)) {
        uint32_t ਬੱਚਾ_ਗਿਣਤੀ = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਨਿਸ਼ਾਨਾ);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        ਲਈ(uint32_t i = 0; i < ਬੱਚਾ_ਗਿਣਤੀ && res == LV_ਨਤੀਜਾ_OK && !e->stop_processing; i++) {
            lv_obj_t * ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(ਨਿਸ਼ਾਨਾ, i);
            ਜੇ(ਬੱਚਾ) {
                e->current_ਨਿਸ਼ਾਨਾ = ਬੱਚਾ;
                res = event_send_core(e);
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->ਕੋਡ, (ਖਾਲੀ *)ਬੱਚਾ);
                    ਰੋਕੋ;
                }
            }
        }
    }

    ਵਾਪਸ res;
}

ਸਥਿਰ bool event_is_bubbled(lv_event_t * e)
{
    ਜੇ(e->stop_bubbling) ਵਾਪਸ false;

    /*Event codes that always bubble*/
    ਚੋਣ(e->ਕੋਡ) {
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_CREATED:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_DELETED:
            ਵਾਪਸ true;
        ਮੂਲ:
            ਰੋਕੋ;
    }

    /*Check other codes only if bubbling is enabled*/
    ਜੇ(lv_obj_is_event_bubble(e->current_ਨਿਸ਼ਾਨਾ) == false) ਵਾਪਸ false;

    ਚੋਣ(e->ਕੋਡ) {
        ਹਾਲਤ LV_EVENT_ਪ੍ਰਾਪਤੀ_TEST:
        ਹਾਲਤ LV_EVENT_COVER_CHECK:
        ਹਾਲਤ LV_EVENT_REFR_EXT_DRAW_ਮਾਪ:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN_BEGIN:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN_ਸਿਰਾ:
        ਹਾਲਤ LV_EVENT_DRAW_POST_BEGIN:
        ਹਾਲਤ LV_EVENT_DRAW_POST:
        ਹਾਲਤ LV_EVENT_DRAW_POST_ਸਿਰਾ:
        ਹਾਲਤ LV_EVENT_DRAW_TASK_ADDED:
        ਹਾਲਤ LV_EVENT_REFRESH:
        ਹਾਲਤ LV_EVENT_DELETE:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_CREATED:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_DELETED:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_ਮਾਪ_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_STYLE_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_GET_SELF_ਮਾਪ:
            ਵਾਪਸ false;
        ਮੂਲ:
            ਵਾਪਸ true;
    }
}

ਸਥਿਰ bool event_is_trickled(lv_event_t * e)
{
    ਜੇ(e->stop_trickling) ਵਾਪਸ false;

    /*Check other codes only if trickle is enabled*/
    ਜੇ(lv_obj_is_event_trickle(e->current_ਨਿਸ਼ਾਨਾ) == false) ਵਾਪਸ false;

    ਚੋਣ(e->ਕੋਡ) {
        ਹਾਲਤ LV_EVENT_ਪ੍ਰਾਪਤੀ_TEST:
        ਹਾਲਤ LV_EVENT_COVER_CHECK:
        ਹਾਲਤ LV_EVENT_REFR_EXT_DRAW_ਮਾਪ:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN_BEGIN:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN:
        ਹਾਲਤ LV_EVENT_DRAW_MAIN_ਸਿਰਾ:
        ਹਾਲਤ LV_EVENT_DRAW_POST_BEGIN:
        ਹਾਲਤ LV_EVENT_DRAW_POST:
        ਹਾਲਤ LV_EVENT_DRAW_POST_ਸਿਰਾ:
        ਹਾਲਤ LV_EVENT_DRAW_TASK_ADDED:
        ਹਾਲਤ LV_EVENT_REFRESH:
        ਹਾਲਤ LV_EVENT_DELETE:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_CREATED:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_DELETED:
        ਹਾਲਤ LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_ਮਾਪ_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_STYLE_ਬਦਲਿਆ:
        ਹਾਲਤ LV_EVENT_GET_SELF_ਮਾਪ:
            ਵਾਪਸ false;
        ਮੂਲ:
            ਵਾਪਸ true;
    }
}

#ਜੇ LV_USE_CHECK_ARG
ਸਥਿਰ bool event_ਕੋਡ_ਵਿੱਚ_array(lv_event_ਕੋਡ_t ਕੋਡ, ਅਚਲ lv_event_ਕੋਡ_t * arr, uint32_t len)
{
    uint32_t i;
    ਲਈ(i = 0; i < len; i++) {
        ਜੇ(ਕੋਡ == arr[i]) ਵਾਪਸ true;
    }
    ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ
