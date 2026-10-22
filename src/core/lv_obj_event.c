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
#وصف_ڪريو MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_نتيجو_t event_send_core(lv_event_t * e);
ساكن bool event_is_bubbled(lv_event_t * e);
ساكن bool event_is_trickled(lv_event_t * e);
#إذا LV_USE_CHECK_ARG
    ساكن bool event_ڪوڊ_اندريون_array(lv_event_ڪوڊ_t ڪوڊ, ثابت lv_event_ڪوڊ_t * arr, uint32_t len);
#جيڪڏهن_پورو

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_EVENT
    #وصف_ڪريو LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #وصف_ڪريو LV_TRACE_EVENT(...)
#جيڪڏهن_پورو

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_نتيجو_t lv_obj_send_event(lv_obj_t * obj, lv_event_ڪوڊ_t event_ڪوڊ, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    lv_event_t e;
    e.current_هدف = obj;
    e.original_هدف = obj;
    e.ڪوڊ = event_ڪوڊ;
    e.user_معلومات = عدم;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ڌڪو(&e);

    /*Send the event*/
    lv_نتيجو_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_ڪڍو(&e);

    أرجع res;
}

lv_نتيجو_t lv_obj_event_بنياد(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع LV_نتيجو_INVALID);
    LV_CHECK_ARG(e->current_هدف != عدم, أرجع LV_نتيجو_INVALID);

    ثابت lv_obj_class_t * بنياد;
    إذا(class_p == عدم) بنياد = ((lv_obj_t *)e->current_هدف)->class_p;
    وإلا بنياد = class_p->بنياد_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    طالما(بنياد && بنياد->event_cb == عدم) بنياد = بنياد->بنياد_class;

    إذا(بنياد == عدم) أرجع LV_نتيجو_OK;
    إذا(بنياد->event_cb == عدم) أرجع LV_نتيجو_OK;

    /*Call the actual event callback*/
    e->user_معلومات = عدم;
    LV_PROFILER_EVENT_BEGIN_ٽيگ(lv_event_ڪوڊ_get_name(e->ڪوڊ));
    بنياد->event_cb(بنياد, e);
    LV_PROFILER_EVENT_پڇاڙي_ٽيگ(lv_event_ڪوڊ_get_name(e->ڪوڊ));

    lv_نتيجو_t res = LV_نتيجو_OK;
    /*Stop if the object is deleted*/
    إذا(e->deleted) res = LV_نتيجو_INVALID;

    أرجع res;
}

lv_event_dsc_t * lv_obj_جوڙيو_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_ڪوڊ_t filter, فراغ * user_معلومات)
{
    LV_CHECK_ARG(event_cb != عدم, أرجع عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع عدم;
    }

    أرجع lv_event_جوڙيو(&obj->spec_attr->event_list, event_cb, filter, user_معلومات);
}

uint32_t lv_obj_get_event_ڳڻپ(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع lv_event_get_ڳڻپ(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t اشاريو)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    إذا(obj->spec_attr == عدم) أرجع عدم;
    أرجع lv_event_get_dsc(&obj->spec_attr->event_list, اشاريو);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t اشاريو)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    إذا(obj->spec_attr == عدم) أرجع false;
    أرجع lv_event_remove(&obj->spec_attr->event_list, اشاريو);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(dsc != عدم, أرجع false);

    إذا(obj->spec_attr == عدم) أرجع false;
    أرجع lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_ڳڻپ(obj);
    uint32_t removed_ڳڻپ = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_ڳڻپ++;
        }
    }

    أرجع removed_ڳڻپ;
}

uint32_t lv_obj_remove_event_cb_with_user_معلومات(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_معلومات)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_ڳڻپ(obj);
    uint32_t removed_ڳڻپ = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_معلومات == user_معلومات) {
            lv_obj_remove_event(obj, i);
            removed_ڳڻپ ++;
        }
    }

    أرجع removed_ڳڻپ;
}

lv_obj_t * lv_event_get_current_هدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_current_هدف(e);
}

lv_obj_t * lv_event_get_هدف_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_هدف(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_ڪوڊ_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_گم,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_پڇاڙي,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_مٿي,        LV_EVENT_GESTURE_هيٺ_ڏانهن,
        LV_EVENT_GESTURE_کاٻو,      LV_EVENT_GESTURE_ساڄو,
        LV_EVENT_ڪنجي,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#جيڪڏهن_پورو
    LV_CHECK_ARG_FORMAT_نياپو(
        event_ڪوڊ_اندريون_array(e->ڪوڊ, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_ڪوڊ_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_پڇاڙي,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_پڇاڙي,
    };
#جيڪڏهن_پورو
    LV_CHECK_ARG_FORMAT_نياپو(
        event_ڪوڊ_اندريون_array(e->ڪوڊ, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_سائيز(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_سائيز_بدليو,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

uint32_t lv_event_get_ڪنجي(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_ڪنجي,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    uint32_t * k = lv_event_get_param(e);
    أرجع k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_ROTARY,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    int32_t * r = lv_event_get_param(e);
    أرجع r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_SCROLL_BEGIN,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

فراغ lv_event_سيٽ_ext_draw_سائيز(lv_event_t * e, int32_t سائيز)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_REFR_EXT_DRAW_سائيز,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    int32_t * cur_سائيز = lv_event_get_param(e);
    *cur_سائيز = LV_MAX(*cur_سائيز, سائيز);
}

lv_نقطو_t * lv_event_get_self_سائيز_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_GET_SELF_سائيز,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

lv_ڪاميابي_test_info_t * lv_event_get_ڪاميابي_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_ڪاميابي_TEST,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_COVER_CHECK,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    أرجع p->area;
}

فراغ lv_event_سيٽ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_COVER_CHECK,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إذا(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_DRAW_TASK_ADDED,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    أرجع lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_نياپو(e->ڪوڊ == LV_EVENT_STATE_بدليو,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->ڪوڊ);
    lv_state_t * state = lv_event_get_param(e);
    أرجع state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_نتيجو_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->ڪوڊ, (فراغ *)e->original_هدف, e->param);

    lv_indev_t * indev_act = lv_indev_سرگرم();
    إذا(indev_act) {
        إذا(e->stop_processing) أرجع LV_نتيجو_OK;
        إذا(e->deleted) أرجع LV_نتيجو_INVALID;
    }

    lv_obj_t * هدف = e->current_هدف;
    lv_نتيجو_t res = LV_نتيجو_OK;
    lv_event_list_t * list = هدف->spec_attr ?  &هدف->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    إذا(res != LV_نتيجو_OK || e->stop_processing) أرجع res;

    res = lv_obj_event_بنياد(عدم, e);
    إذا(res != LV_نتيجو_OK || e->stop_processing) أرجع res;

    res = lv_event_send(list, e, false);
    إذا(res != LV_نتيجو_OK || e->stop_processing) أرجع res;

    lv_obj_t * والد = lv_obj_get_والد(e->current_هدف);
    إذا(والد && event_is_bubbled(e)) {
        e->current_هدف = والد;
        res = event_send_core(e);
    }
    إذا(res != LV_نتيجو_OK) أرجع res;

    /*Trickle down to children if enabled*/
    إذا(event_is_trickled(e)) {
        uint32_t ٻار_ڳڻپ = lv_obj_get_ٻار_ڳڻپ(هدف);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < ٻار_ڳڻپ && res == LV_نتيجو_OK && !e->stop_processing; i++) {
            lv_obj_t * ٻار = lv_obj_get_ٻار(هدف, i);
            إذا(ٻار) {
                e->current_هدف = ٻار;
                res = event_send_core(e);
                إذا(res != LV_نتيجو_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->ڪوڊ, (فراغ *)ٻار);
                    اكسر;
                }
            }
        }
    }

    أرجع res;
}

ساكن bool event_is_bubbled(lv_event_t * e)
{
    إذا(e->stop_bubbling) أرجع false;

    /*Event codes that always bubble*/
    بدل(e->ڪوڊ) {
        حالة LV_EVENT_ٻار_CREATED:
        حالة LV_EVENT_ٻار_DELETED:
            أرجع true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    إذا(lv_obj_is_event_bubble(e->current_هدف) == false) أرجع false;

    بدل(e->ڪوڊ) {
        حالة LV_EVENT_ڪاميابي_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_سائيز:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_پڇاڙي:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_پڇاڙي:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_ٻار_CREATED:
        حالة LV_EVENT_ٻار_DELETED:
        حالة LV_EVENT_ٻار_بدليو:
        حالة LV_EVENT_سائيز_بدليو:
        حالة LV_EVENT_STYLE_بدليو:
        حالة LV_EVENT_GET_SELF_سائيز:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

ساكن bool event_is_trickled(lv_event_t * e)
{
    إذا(e->stop_trickling) أرجع false;

    /*Check other codes only if trickle is enabled*/
    إذا(lv_obj_is_event_trickle(e->current_هدف) == false) أرجع false;

    بدل(e->ڪوڊ) {
        حالة LV_EVENT_ڪاميابي_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_سائيز:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_پڇاڙي:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_پڇاڙي:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_ٻار_CREATED:
        حالة LV_EVENT_ٻار_DELETED:
        حالة LV_EVENT_ٻار_بدليو:
        حالة LV_EVENT_سائيز_بدليو:
        حالة LV_EVENT_STYLE_بدليو:
        حالة LV_EVENT_GET_SELF_سائيز:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

#إذا LV_USE_CHECK_ARG
ساكن bool event_ڪوڊ_اندريون_array(lv_event_ڪوڊ_t ڪوڊ, ثابت lv_event_ڪوڊ_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        إذا(ڪوڊ == arr[i]) أرجع true;
    }
    أرجع false;
}
#جيڪڏهن_پورو
