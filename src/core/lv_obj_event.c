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
#تعریف_کرو MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_نتیجہ_t event_send_core(lv_event_t * e);
ساكن bool event_is_bubbled(lv_event_t * e);
ساكن bool event_is_trickled(lv_event_t * e);
#إذا LV_USE_CHECK_ARG
    ساكن bool event_کوڈ_وچ_array(lv_event_کوڈ_t کوڈ, ثابت lv_event_کوڈ_t * arr, uint32_t len);
#جے_انت

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_EVENT
    #تعریف_کرو LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #تعریف_کرو LV_TRACE_EVENT(...)
#جے_انت

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_نتیجہ_t lv_obj_send_event(lv_obj_t * obj, lv_event_کوڈ_t event_کوڈ, فراغ * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    lv_event_t e;
    e.current_نشانہ = obj;
    e.original_نشانہ = obj;
    e.کوڈ = event_کوڈ;
    e.user_انکڑے = عدم;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_ڈالو(&e);

    /*Send the event*/
    lv_نتیجہ_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_نکالو(&e);

    أرجع res;
}

lv_نتیجہ_t lv_obj_event_ادھار(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع LV_نتیجہ_INVALID);
    LV_CHECK_ARG(e->current_نشانہ != عدم, أرجع LV_نتیجہ_INVALID);

    ثابت lv_obj_class_t * ادھار;
    إذا(class_p == عدم) ادھار = ((lv_obj_t *)e->current_نشانہ)->class_p;
    وإلا ادھار = class_p->ادھار_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    طالما(ادھار && ادھار->event_cb == عدم) ادھار = ادھار->ادھار_class;

    إذا(ادھار == عدم) أرجع LV_نتیجہ_OK;
    إذا(ادھار->event_cb == عدم) أرجع LV_نتیجہ_OK;

    /*Call the actual event callback*/
    e->user_انکڑے = عدم;
    LV_PROFILER_EVENT_BEGIN_ٹیگ(lv_event_کوڈ_get_name(e->کوڈ));
    ادھار->event_cb(ادھار, e);
    LV_PROFILER_EVENT_سرا_ٹیگ(lv_event_کوڈ_get_name(e->کوڈ));

    lv_نتیجہ_t res = LV_نتیجہ_OK;
    /*Stop if the object is deleted*/
    إذا(e->deleted) res = LV_نتیجہ_INVALID;

    أرجع res;
}

lv_event_dsc_t * lv_obj_جوڑو_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_کوڈ_t filter, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(event_cb != عدم, أرجع عدم);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع عدم;
    }

    أرجع lv_event_جوڑو(&obj->spec_attr->event_list, event_cb, filter, user_انکڑے);
}

uint32_t lv_obj_get_event_گنتی(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع lv_event_get_گنتی(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t اشاریہ)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    إذا(obj->spec_attr == عدم) أرجع عدم;
    أرجع lv_event_get_dsc(&obj->spec_attr->event_list, اشاریہ);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t اشاریہ)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    إذا(obj->spec_attr == عدم) أرجع false;
    أرجع lv_event_remove(&obj->spec_attr->event_list, اشاریہ);
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

    uint32_t event_cnt = lv_obj_get_event_گنتی(obj);
    uint32_t removed_گنتی = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_گنتی++;
        }
    }

    أرجع removed_گنتی;
}

uint32_t lv_obj_remove_event_cb_with_user_انکڑے(lv_obj_t * obj, lv_event_cb_t event_cb, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(obj != عدم, أرجع 0);

    uint32_t event_cnt = lv_obj_get_event_گنتی(obj);
    uint32_t removed_گنتی = 0;
    int32_t i;

    إذا(event_cnt == 0) أرجع 0;

    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        إذا(dsc && (event_cb == عدم || dsc->cb == event_cb) && dsc->user_انکڑے == user_انکڑے) {
            lv_obj_remove_event(obj, i);
            removed_گنتی ++;
        }
    }

    أرجع removed_گنتی;
}

lv_obj_t * lv_event_get_current_نشانہ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_current_نشانہ(e);
}

lv_obj_t * lv_event_get_نشانہ_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    أرجع lv_event_get_نشانہ(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کوڈ_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_گواچا,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_سرا,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_اتے,        LV_EVENT_GESTURE_تھلے,
        LV_EVENT_GESTURE_بایاں,      LV_EVENT_GESTURE_دایاں,
        LV_EVENT_کلید,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#جے_انت
    LV_CHECK_ARG_FORMAT_پیغام(
        event_کوڈ_وچ_array(e->کوڈ, indev_codes, حجم(indev_codes) / حجم(indev_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
#إذا LV_USE_CHECK_ARG
    ساكن ثابت lv_event_کوڈ_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_سرا,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_سرا,
    };
#جے_انت
    LV_CHECK_ARG_FORMAT_پیغام(
        event_کوڈ_وچ_array(e->کوڈ, draw_codes, حجم(draw_codes) / حجم(draw_codes[0])),
        أرجع عدم,
        "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_old_ناپ(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_ناپ_بدلیا,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

uint32_t lv_event_get_کلید(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_کلید,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    uint32_t * k = lv_event_get_param(e);
    أرجع k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_ROTARY,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    int32_t * r = lv_event_get_param(e);
    أرجع r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_SCROLL_BEGIN,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

فراغ lv_event_سیٹ_ext_draw_ناپ(lv_event_t * e, int32_t ناپ)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_REFR_EXT_DRAW_ناپ,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    int32_t * cur_ناپ = lv_event_get_param(e);
    *cur_ناپ = LV_MAX(*cur_ناپ, ناپ);
}

lv_نقطہ_t * lv_event_get_self_ناپ_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_GET_SELF_ناپ,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

lv_کامیابی_test_info_t * lv_event_get_کامیابی_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_کامیابی_TEST,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

ثابت lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_COVER_CHECK,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    أرجع p->area;
}

فراغ lv_event_سیٹ_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != عدم, أرجع);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_COVER_CHECK,
                            أرجع,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    إذا(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_DRAW_TASK_ADDED,
                            أرجع عدم,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    أرجع lv_event_get_param(e);
}

lv_state_t lv_event_get_prev_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != عدم, أرجع 0);
    LV_CHECK_ARG_FORMAT_پیغام(e->کوڈ == LV_EVENT_STATE_بدلیا,
                            أرجع 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->کوڈ);
    lv_state_t * state = lv_event_get_param(e);
    أرجع state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن lv_نتیجہ_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->کوڈ, (فراغ *)e->original_نشانہ, e->param);

    lv_indev_t * indev_act = lv_indev_سرگرم();
    إذا(indev_act) {
        إذا(e->stop_processing) أرجع LV_نتیجہ_OK;
        إذا(e->deleted) أرجع LV_نتیجہ_INVALID;
    }

    lv_obj_t * نشانہ = e->current_نشانہ;
    lv_نتیجہ_t res = LV_نتیجہ_OK;
    lv_event_list_t * list = نشانہ->spec_attr ?  &نشانہ->spec_attr->event_list : عدم;

    res = lv_event_send(list, e, true);
    إذا(res != LV_نتیجہ_OK || e->stop_processing) أرجع res;

    res = lv_obj_event_ادھار(عدم, e);
    إذا(res != LV_نتیجہ_OK || e->stop_processing) أرجع res;

    res = lv_event_send(list, e, false);
    إذا(res != LV_نتیجہ_OK || e->stop_processing) أرجع res;

    lv_obj_t * ماپا = lv_obj_get_ماپا(e->current_نشانہ);
    إذا(ماپا && event_is_bubbled(e)) {
        e->current_نشانہ = ماپا;
        res = event_send_core(e);
    }
    إذا(res != LV_نتیجہ_OK) أرجع res;

    /*Trickle down to children if enabled*/
    إذا(event_is_trickled(e)) {
        uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(نشانہ);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        لكل(uint32_t i = 0; i < بچہ_گنتی && res == LV_نتیجہ_OK && !e->stop_processing; i++) {
            lv_obj_t * بچہ = lv_obj_get_بچہ(نشانہ, i);
            إذا(بچہ) {
                e->current_نشانہ = بچہ;
                res = event_send_core(e);
                إذا(res != LV_نتیجہ_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->کوڈ, (فراغ *)بچہ);
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
    بدل(e->کوڈ) {
        حالة LV_EVENT_بچہ_CREATED:
        حالة LV_EVENT_بچہ_DELETED:
            أرجع true;
        افتراضي:
            اكسر;
    }

    /*Check other codes only if bubbling is enabled*/
    إذا(lv_obj_is_event_bubble(e->current_نشانہ) == false) أرجع false;

    بدل(e->کوڈ) {
        حالة LV_EVENT_کامیابی_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_ناپ:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_سرا:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_سرا:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_بچہ_CREATED:
        حالة LV_EVENT_بچہ_DELETED:
        حالة LV_EVENT_بچہ_بدلیا:
        حالة LV_EVENT_ناپ_بدلیا:
        حالة LV_EVENT_STYLE_بدلیا:
        حالة LV_EVENT_GET_SELF_ناپ:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

ساكن bool event_is_trickled(lv_event_t * e)
{
    إذا(e->stop_trickling) أرجع false;

    /*Check other codes only if trickle is enabled*/
    إذا(lv_obj_is_event_trickle(e->current_نشانہ) == false) أرجع false;

    بدل(e->کوڈ) {
        حالة LV_EVENT_کامیابی_TEST:
        حالة LV_EVENT_COVER_CHECK:
        حالة LV_EVENT_REFR_EXT_DRAW_ناپ:
        حالة LV_EVENT_DRAW_MAIN_BEGIN:
        حالة LV_EVENT_DRAW_MAIN:
        حالة LV_EVENT_DRAW_MAIN_سرا:
        حالة LV_EVENT_DRAW_POST_BEGIN:
        حالة LV_EVENT_DRAW_POST:
        حالة LV_EVENT_DRAW_POST_سرا:
        حالة LV_EVENT_DRAW_TASK_ADDED:
        حالة LV_EVENT_REFRESH:
        حالة LV_EVENT_DELETE:
        حالة LV_EVENT_بچہ_CREATED:
        حالة LV_EVENT_بچہ_DELETED:
        حالة LV_EVENT_بچہ_بدلیا:
        حالة LV_EVENT_ناپ_بدلیا:
        حالة LV_EVENT_STYLE_بدلیا:
        حالة LV_EVENT_GET_SELF_ناپ:
            أرجع false;
        افتراضي:
            أرجع true;
    }
}

#إذا LV_USE_CHECK_ARG
ساكن bool event_کوڈ_وچ_array(lv_event_کوڈ_t کوڈ, ثابت lv_event_کوڈ_t * arr, uint32_t len)
{
    uint32_t i;
    لكل(i = 0; i < len; i++) {
        إذا(کوڈ == arr[i]) أرجع true;
    }
    أرجع false;
}
#جے_انت
