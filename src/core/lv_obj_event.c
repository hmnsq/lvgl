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
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t event_send_core(lv_event_t * e);
ಸ್ಥಿರ bool event_is_bubbled(lv_event_t * e);
ಸ್ಥಿರ bool event_is_trickled(lv_event_t * e);
#ಆದರೆ LV_USE_CHECK_ARG
    ಸ್ಥಿರ bool event_ಸಂಕೇತ_ಒಳ_array(lv_event_ಸಂಕೇತ_t ಸಂಕೇತ_2, ಸ್ಥಿರಾಂಕ lv_event_ಸಂಕೇತ_t * arr, uint32_t len);
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ಆದರೆ LV_USE_LOG && LV_LOG_TRACE_EVENT
    #ವ್ಯಾಖ್ಯಾನಿಸು LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು LV_TRACE_EVENT(...)
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ಫಲಿತಾಂಶ_t lv_obj_send_event(lv_obj_t * obj, lv_event_ಸಂಕೇತ_t event_ಸಂಕೇತ, ಶೂನ್ಯ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    lv_event_t e;
    e.current_ಗುರಿ = obj;
    e.original_ಗುರಿ = obj;
    e.ಸಂಕೇತ_2 = event_ಸಂಕೇತ;
    e.user_ದತ್ತಾಂಶ = ಖಾಲಿ;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ತಳ್ಳು(&e);

    /*Send the event*/
    lv_ಫಲಿತಾಂಶ_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_ತೆಗೆ(&e);

    ಹಿಂತಿರುಗಿ res;
}

lv_ಫಲಿತಾಂಶ_t lv_obj_event_ತಳ(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);
    LV_CHECK_ARG(e->current_ಗುರಿ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * ತಳ;
    ಆದರೆ(class_p == ಖಾಲಿ) ತಳ = ((lv_obj_t *)e->current_ಗುರಿ)->class_p;
    ಇಲ್ಲದಿದ್ದರೆ ತಳ = class_p->ತಳ_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    ಆಗ(ತಳ && ತಳ->event_cb == ಖಾಲಿ) ತಳ = ತಳ->ತಳ_class;

    ಆದರೆ(ತಳ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    ಆದರೆ(ತಳ->event_cb == ಖಾಲಿ) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;

    /*Call the actual event callback*/
    e->user_ದತ್ತಾಂಶ = ಖಾಲಿ;
    LV_PROFILER_EVENT_BEGIN_ಟ್ಯಾಗ್(lv_event_ಸಂಕೇತ_get_name(e->ಸಂಕೇತ_2));
    ತಳ->event_cb(ತಳ, e);
    LV_PROFILER_EVENT_ಕೊನೆ_ಟ್ಯಾಗ್(lv_event_ಸಂಕೇತ_get_name(e->ಸಂಕೇತ_2));

    lv_ಫಲಿತಾಂಶ_t res = LV_ಫಲಿತಾಂಶ_OK;
    /*Stop if the object is deleted*/
    ಆದರೆ(e->deleted) res = LV_ಫಲಿತಾಂಶ_INVALID;

    ಹಿಂತಿರುಗಿ res;
}

lv_event_dsc_t * lv_obj_ಕೂಡಿಸು_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_ಸಂಕೇತ_t filter, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(event_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ lv_event_ಕೂಡಿಸು(&obj->spec_attr->event_list, event_cb, filter, user_ದತ್ತಾಂಶ);
}

uint32_t lv_obj_get_event_ಎಣಿಕೆ(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ lv_event_get_ಎಣಿಕೆ(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t ಸೂಚ್ಯಂಕ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ lv_event_get_dsc(&obj->spec_attr->event_list, ಸೂಚ್ಯಂಕ);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t ಸೂಚ್ಯಂಕ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;
    ಹಿಂತಿರುಗಿ lv_event_remove(&obj->spec_attr->event_list, ಸೂಚ್ಯಂಕ);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;
    ಹಿಂತಿರುಗಿ lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);

    uint32_t event_cnt = lv_obj_get_event_ಎಣಿಕೆ(obj);
    uint32_t removed_ಎಣಿಕೆ = 0;
    int32_t i;

    ಆದರೆ(event_cnt == 0) ಹಿಂತಿರುಗಿ 0;

    ಗಾಗಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ಆದರೆ(dsc && (event_cb == ಖಾಲಿ || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_ಎಣಿಕೆ++;
        }
    }

    ಹಿಂತಿರುಗಿ removed_ಎಣಿಕೆ;
}

uint32_t lv_obj_remove_event_cb_with_user_ದತ್ತಾಂಶ(lv_obj_t * obj, lv_event_cb_t event_cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);

    uint32_t event_cnt = lv_obj_get_event_ಎಣಿಕೆ(obj);
    uint32_t removed_ಎಣಿಕೆ = 0;
    int32_t i;

    ಆದರೆ(event_cnt == 0) ಹಿಂತಿರುಗಿ 0;

    ಗಾಗಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        ಆದರೆ(dsc && (event_cb == ಖಾಲಿ || dsc->cb == event_cb) && dsc->user_ದತ್ತಾಂಶ == user_ದತ್ತಾಂಶ) {
            lv_obj_remove_event(obj, i);
            removed_ಎಣಿಕೆ ++;
        }
    }

    ಹಿಂತಿರುಗಿ removed_ಎಣಿಕೆ;
}

lv_obj_t * lv_event_get_current_ಗುರಿ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ lv_event_get_current_ಗುರಿ(e);
}

lv_obj_t * lv_event_get_ಗುರಿ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ lv_event_get_ಗುರಿ(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
#ಆದರೆ LV_USE_CHECK_ARG
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ lv_event_ಸಂಕೇತ_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_ಕಳೆದುಹೋದ,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_ಕೊನೆ,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_ಮೇಲೆ,        LV_EVENT_GESTURE_ಕೆಳಕ್ಕೆ,
        LV_EVENT_GESTURE_ಎಡ,      LV_EVENT_GESTURE_ಬಲ,
        LV_EVENT_ಕೀಲಿ,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#ಆದರೆ_ಅಂತ್ಯ
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(
        event_ಸಂಕೇತ_ಒಳ_array(e->ಸಂಕೇತ_2, indev_codes, ಗಾತ್ರ(indev_codes) / ಗಾತ್ರ(indev_codes[0])),
        ಹಿಂತಿರುಗಿ ಖಾಲಿ,
        "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
#ಆದರೆ LV_USE_CHECK_ARG
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ lv_event_ಸಂಕೇತ_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_ಕೊನೆ,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_ಕೊನೆ,
    };
#ಆದರೆ_ಅಂತ್ಯ
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(
        event_ಸಂಕೇತ_ಒಳ_array(e->ಸಂಕೇತ_2, draw_codes, ಗಾತ್ರ(draw_codes) / ಗಾತ್ರ(draw_codes[0])),
        ಹಿಂತಿರುಗಿ ಖಾಲಿ,
        "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

ಸ್ಥಿರಾಂಕ lv_area_t * lv_event_get_old_ಗಾತ್ರ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_ಗಾತ್ರ_ಬದಲಾಯಿತು,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

uint32_t lv_event_get_ಕೀಲಿ(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_ಕೀಲಿ,
                            ಹಿಂತಿರುಗಿ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    uint32_t * k = lv_event_get_param(e);
    ಹಿಂತಿರುಗಿ k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_ROTARY,
                            ಹಿಂತಿರುಗಿ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    int32_t * r = lv_event_get_param(e);
    ಹಿಂತಿರುಗಿ r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_SCROLL_BEGIN,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

ಶೂನ್ಯ lv_event_ಗಣ_ext_draw_ಗಾತ್ರ(lv_event_t * e, int32_t ಗಾತ್ರ_2)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ,
                            ಹಿಂತಿರುಗಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    int32_t * cur_ಗಾತ್ರ = lv_event_get_param(e);
    *cur_ಗಾತ್ರ = LV_MAX(*cur_ಗಾತ್ರ, ಗಾತ್ರ_2);
}

lv_ಬಿಂದು_t * lv_event_get_self_ಗಾತ್ರ_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_GET_SELF_ಗಾತ್ರ,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

lv_ದೊರಕಿತು_test_info_t * lv_event_get_ದೊರಕಿತು_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_ದೊರಕಿತು_TEST,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

ಸ್ಥಿರಾಂಕ lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_COVER_CHECK,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ಹಿಂತಿರುಗಿ p->area;
}

ಶೂನ್ಯ lv_event_ಗಣ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_COVER_CHECK,
                            ಹಿಂತಿರುಗಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    ಆದರೆ(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_DRAW_TASK_ADDED,
                            ಹಿಂತಿರುಗಿ ಖಾಲಿ,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(e->ಸಂಕೇತ_2 == LV_EVENT_STATE_ಬದಲಾಯಿತು,
                            ಹಿಂತಿರುಗಿ 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ಸಂಕೇತ_2);
    lv_state_t * state = lv_event_get_param(e);
    ಹಿಂತಿರುಗಿ state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->ಸಂಕೇತ_2, (ಶೂನ್ಯ *)e->original_ಗುರಿ, e->param);

    lv_indev_t * indev_act = lv_indev_ಸಕ್ರಿಯ();
    ಆದರೆ(indev_act) {
        ಆದರೆ(e->stop_processing) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
        ಆದರೆ(e->deleted) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }

    lv_obj_t * ಗುರಿ = e->current_ಗುರಿ;
    lv_ಫಲಿತಾಂಶ_t res = LV_ಫಲಿತಾಂಶ_OK;
    lv_event_list_t * list = ಗುರಿ->spec_attr ?  &ಗುರಿ->spec_attr->event_list : ಖಾಲಿ;

    res = lv_event_send(list, e, true);
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK || e->stop_processing) ಹಿಂತಿರುಗಿ res;

    res = lv_obj_event_ತಳ(ಖಾಲಿ, e);
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK || e->stop_processing) ಹಿಂತಿರುಗಿ res;

    res = lv_event_send(list, e, false);
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK || e->stop_processing) ಹಿಂತಿರುಗಿ res;

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(e->current_ಗುರಿ);
    ಆದರೆ(ಪೋಷಕ && event_is_bubbled(e)) {
        e->current_ಗುರಿ = ಪೋಷಕ;
        res = event_send_core(e);
    }
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ res;

    /*Trickle down to children if enabled*/
    ಆದರೆ(event_is_trickled(e)) {
        uint32_t ಮಗು_ಎಣಿಕೆ = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಗುರಿ);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        ಗಾಗಿ(uint32_t i = 0; i < ಮಗು_ಎಣಿಕೆ && res == LV_ಫಲಿತಾಂಶ_OK && !e->stop_processing; i++) {
            lv_obj_t * ಮಗು = lv_obj_get_ಮಗು(ಗುರಿ, i);
            ಆದರೆ(ಮಗು) {
                e->current_ಗುರಿ = ಮಗು;
                res = event_send_core(e);
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->ಸಂಕೇತ_2, (ಶೂನ್ಯ *)ಮಗು);
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
    }

    ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ bool event_is_bubbled(lv_event_t * e)
{
    ಆದರೆ(e->stop_bubbling) ಹಿಂತಿರುಗಿ false;

    /*Event codes that always bubble*/
    ಆಯ್ಕೆ(e->ಸಂಕೇತ_2) {
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_CREATED:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_DELETED:
            ಹಿಂತಿರುಗಿ true;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }

    /*Check other codes only if bubbling is enabled*/
    ಆದರೆ(lv_obj_is_event_bubble(e->current_ಗುರಿ) == false) ಹಿಂತಿರುಗಿ false;

    ಆಯ್ಕೆ(e->ಸಂಕೇತ_2) {
        ಸ್ಥಿತಿ LV_EVENT_ದೊರಕಿತು_TEST:
        ಸ್ಥಿತಿ LV_EVENT_COVER_CHECK:
        ಸ್ಥಿತಿ LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN_BEGIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN_ಕೊನೆ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST_BEGIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST_ಕೊನೆ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_TASK_ADDED:
        ಸ್ಥಿತಿ LV_EVENT_REFRESH:
        ಸ್ಥಿತಿ LV_EVENT_DELETE:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_CREATED:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_DELETED:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_ಗಾತ್ರ_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_STYLE_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_GET_SELF_ಗಾತ್ರ:
            ಹಿಂತಿರುಗಿ false;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ಹಿಂತಿರುಗಿ true;
    }
}

ಸ್ಥಿರ bool event_is_trickled(lv_event_t * e)
{
    ಆದರೆ(e->stop_trickling) ಹಿಂತಿರುಗಿ false;

    /*Check other codes only if trickle is enabled*/
    ಆದರೆ(lv_obj_is_event_trickle(e->current_ಗುರಿ) == false) ಹಿಂತಿರುಗಿ false;

    ಆಯ್ಕೆ(e->ಸಂಕೇತ_2) {
        ಸ್ಥಿತಿ LV_EVENT_ದೊರಕಿತು_TEST:
        ಸ್ಥಿತಿ LV_EVENT_COVER_CHECK:
        ಸ್ಥಿತಿ LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN_BEGIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_MAIN_ಕೊನೆ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST_BEGIN:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_POST_ಕೊನೆ:
        ಸ್ಥಿತಿ LV_EVENT_DRAW_TASK_ADDED:
        ಸ್ಥಿತಿ LV_EVENT_REFRESH:
        ಸ್ಥಿತಿ LV_EVENT_DELETE:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_CREATED:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_DELETED:
        ಸ್ಥಿತಿ LV_EVENT_ಮಗು_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_ಗಾತ್ರ_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_STYLE_ಬದಲಾಯಿತು:
        ಸ್ಥಿತಿ LV_EVENT_GET_SELF_ಗಾತ್ರ:
            ಹಿಂತಿರುಗಿ false;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ಹಿಂತಿರುಗಿ true;
    }
}

#ಆದರೆ LV_USE_CHECK_ARG
ಸ್ಥಿರ bool event_ಸಂಕೇತ_ಒಳ_array(lv_event_ಸಂಕೇತ_t ಸಂಕೇತ_2, ಸ್ಥಿರಾಂಕ lv_event_ಸಂಕೇತ_t * arr, uint32_t len)
{
    uint32_t i;
    ಗಾಗಿ(i = 0; i < len; i++) {
        ಆದರೆ(ಸಂಕೇತ_2 == arr[i]) ಹಿಂತಿರುಗಿ true;
    }
    ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ
