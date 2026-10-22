/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#شامل "lv_observer_private.h"
#اگر LV_USE_OBSERVER

#شامل "../lvgl_public.h"
#شامل "../core/lv_global.h"
#شامل "../core/lv_obj_private.h"
#شامل "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#تعریف_کرو subject_list LV_GLOBAL_DEFAULT()->subject_ll

#اگر_غیر_تعریف_شدہ FLT_MAX
    #تعریف_کرو FLT_MAX 3.402823466e+38F
#اگر_ختم

/**********************
 *      TYPEDEFS
 **********************/
قسمتعریف ساخت {
    lv_state_t state;
} bound_state_t;

قسمتعریف ساخت {
    lv_subject_t * subject;
    int32_t قدر;
} subject_سیٹ_int_user_ڈیٹا_t;

قسمتعریف ساخت {
    lv_subject_t * subject;
    اعشاری قدر;
} subject_سیٹ_float_user_ڈیٹا_t;

قسمتعریف ساخت {
    lv_subject_t * subject;
    ثابت حرف * قدر;
} subject_سیٹ_string_user_ڈیٹا_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساکن خالی remove_observers_from_گروہ_subject(lv_subject_t * subject);
ساکن خالی جوڑو_observers_مقصد_گروہ_subject(lv_subject_t * subject, lv_subject_t ** list, سائز_t سائز);
ساکن خالی subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, خالی * user_ڈیٹا);
ساکن خالی subject_toggle_cb(lv_event_t * e);
ساکن خالی subject_سیٹ_int_cb(lv_event_t * e);
#اگر LV_USE_FLOAT
    ساکن خالی subject_سیٹ_float_cb(lv_event_t * e);
#اگر_ختم

ساکن خالی subject_سیٹ_string_cb(lv_event_t * e);
ساکن خالی subject_increment_cb(lv_event_t * e);

ساکن خالی unsubscribe_on_delete_cb(lv_event_t * e);
ساکن خالی گروہ_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ساکن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ساکن خالی obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ساکن خالی obj_قدر_بدلا_event_cb(lv_event_t * e);

ساکن خالی lv_subject_notify_if_بدلا(lv_subject_t * subject);

ساکن خالی subject_سیٹ_string_free_user_ڈیٹا_event_cb(lv_event_t * e);

ساکن خالی سیٹ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ساکن خالی سیٹ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ساکن خالی سیٹ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ساکن خالی سیٹ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ساکن خالی سیٹ_اشارہ_گر_observer(lv_observer_t * observer, lv_subject_t * subject);

#اگر LV_USE_FLOAT
    ساکن خالی سیٹ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ساکن خالی init_float(lv_subject_t * subject, اعشاری قدر);
#اگر_ختم /*LV_USE_FLOAT*/

ساکن خالی init_int(lv_subject_t * subject, int32_t قدر);
ساکن خالی init_string(lv_subject_t * subject, حرف * buf, حرف * پچھلا_buf, سائز_t سائز, ثابت حرف * قدر);
ساکن خالی init_اشارہ_گر(lv_subject_t * subject, خالی * قدر);
ساکن خالی init_color(lv_subject_t * subject, lv_color_t color);
ساکن خالی init_گروہ(lv_subject_t * subject, lv_subject_t ** list, سائز_t سائز);
ساکن خالی init_common(lv_subject_t * subject);

ساکن خالی deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

خالی lv_subject_global_init(خالی)
{
    lv_ll_init(&subject_list, حجم(lv_subject_t));
}
خالی lv_subject_global_deinit(خالی)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        اگر(curr->type == LV_SUBJECT_TYPE_گروہ) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        اگر(curr->type != LV_SUBJECT_TYPE_گروہ) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, واپس تہی);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, واپس تہی);
    lv_subject_t * subject = lv_ll_ins_دم(&subject_list);
    LV_ASSERT_MALLOC(subject);
    اگر(!subject) {
        واپس تہی;
    }

    init_common(subject);

    انتخاب(type) {
        حالت LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            توڑو;
        حالت LV_SUBJECT_TYPE_FLOAT:
#اگر LV_USE_FLOAT
            init_float(subject, 0.f);
#اگر_ختم /*LV_USE_FLOAT*/
            توڑو;
        حالت LV_SUBJECT_TYPE_اشارہ_گر:
            init_اشارہ_گر(subject, تہی);
            توڑو;
        حالت LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            توڑو;
        حالت LV_SUBJECT_TYPE_گروہ:
            init_گروہ(subject, تہی, 0);
            توڑو;
        حالت LV_SUBJECT_TYPE_STRING:
            init_string(subject, تہی, تہی, 0, "");
            توڑو;
        حالت LV_SUBJECT_TYPE_INVALID:
        حالت LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    واپس subject;
}

خالی lv_subject_delete(lv_subject_t * subject)
{
    اگر(!subject) {
        واپس;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#اگر LV_USE_EXT_ڈیٹا
خالی lv_subject_سیٹ_external_ڈیٹا(lv_subject_t * subject, خالی * ڈیٹا, خالی (* free_cb)(خالی * ڈیٹا))
{
    LV_CHECK_ARG(subject != تہی, واپس);

    subject->ext_ڈیٹا.ڈیٹا = ڈیٹا;
    subject->ext_ڈیٹا.free_cb = free_cb;
}
#اگر_ختم

خالی lv_subject_سیٹ_int(lv_subject_t * subject, int32_t قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس);

    قدر = LV_CLAMP(subject->min_قدر.عدد, قدر, subject->max_قدر.عدد);

    subject->پچھلا_قدر.عدد = subject->قدر.عدد;
    subject->قدر.عدد = قدر;
    lv_subject_notify_if_بدلا(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس 0);

    واپس subject->قدر.عدد;
}

int32_t lv_subject_get_پچھلا_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس 0);

    واپس subject->پچھلا_قدر.عدد;
}

خالی lv_subject_سیٹ_min_قدر_int(lv_subject_t * subject, int32_t min_قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس);

    subject->min_قدر.عدد = min_قدر;
}

خالی lv_subject_سیٹ_max_قدر_int(lv_subject_t * subject, int32_t max_قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس);

    subject->max_قدر.عدد = max_قدر;
}

#اگر LV_USE_FLOAT

خالی lv_subject_سیٹ_float(lv_subject_t * subject, اعشاری قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس);

    قدر = LV_CLAMP(subject->min_قدر.float_v, قدر, subject->max_قدر.float_v);

    subject->پچھلا_قدر.float_v = subject->قدر.float_v;
    subject->قدر.float_v = قدر;
    lv_subject_notify_if_بدلا(subject);
}

اعشاری lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس 0.0);

    واپس subject->قدر.float_v;
}

اعشاری lv_subject_get_پچھلا_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس 0.0);

    واپس subject->پچھلا_قدر.float_v;
}

خالی lv_subject_سیٹ_min_قدر_float(lv_subject_t * subject, اعشاری min_قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس);

    subject->min_قدر.float_v = min_قدر;
}

خالی lv_subject_سیٹ_max_قدر_float(lv_subject_t * subject, اعشاری max_قدر)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس);

    subject->max_قدر.float_v = max_قدر;
}


#اگر_ختم /*LV_USE_FLOAT*/

خالی lv_subject_سیٹ_string_بفر_static(lv_subject_t * subject, حرف * buf, حرف * پچھلا_buf, سائز_t سائز)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس);
    LV_CHECK_ARG(buf != تہی, واپس);
    LV_CHECK_ARG(سائز > 0, واپس);

    buf[0] = '\0';
    اگر(پچھلا_buf) پچھلا_buf[0] = '\0';

    subject->سائز = (uint32_t)سائز;
    subject->قدر.اشارہ_گر = buf;
    subject->پچھلا_قدر.اشارہ_گر = پچھلا_buf;
}

خالی lv_subject_سیٹ_string(lv_subject_t * subject, ثابت حرف * buf)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس);
    LV_CHECK_ARG(buf != تہی, واپس);

    اگر(subject->سائز == 0) واپس;
    اگر(subject->پچھلا_قدر.اشارہ_گر) {
        lv_strlcpy((حرف *)subject->پچھلا_قدر.اشارہ_گر, subject->قدر.اشارہ_گر, subject->سائز);
    }

    lv_strlcpy((حرف *)subject->قدر.اشارہ_گر, buf, subject->سائز);

    lv_subject_notify_if_بدلا(subject);
}

خالی lv_subject_snprintf(lv_subject_t * subject, ثابت حرف * format, ...)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس);
    LV_CHECK_ARG(format != تہی, واپس);

    اگر(subject->سائز < 1U) واپس;

    اگر(subject->پچھلا_قدر.اشارہ_گر) {
        lv_strlcpy((حرف *)subject->پچھلا_قدر.اشارہ_گر, subject->قدر.اشارہ_گر, subject->سائز);
    }

    va_list va;
    va_آغاز(va, format);
    ثابت صحیح ret = lv_vsnprintf((حرف *)subject->قدر.اشارہ_گر, subject->سائز, format, va);
    LV_غیر_مستعمل(ret);
    va_انتہا(va);

    lv_subject_notify_if_بدلا(subject);
}

ثابت حرف * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس تہی);

    واپس subject->قدر.اشارہ_گر;
}

ثابت حرف * lv_subject_get_پچھلا_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس تہی);

    واپس subject->پچھلا_قدر.اشارہ_گر;
}

خالی lv_subject_سیٹ_اشارہ_گر(lv_subject_t * subject, خالی * ptr)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, واپس);

    subject->پچھلا_قدر.اشارہ_گر = subject->قدر.اشارہ_گر;
    subject->قدر.اشارہ_گر = ptr;
    lv_subject_notify_if_بدلا(subject);
}

ثابت خالی * lv_subject_get_اشارہ_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, واپس تہی);


    واپس subject->قدر.اشارہ_گر;
}

ثابت خالی * lv_subject_get_پچھلا_اشارہ_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, واپس تہی);

    واپس subject->پچھلا_قدر.اشارہ_گر;
}

خالی lv_subject_سیٹ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, واپس);

    subject->پچھلا_قدر.color = subject->قدر.color;
    subject->قدر.color = color;
    lv_subject_notify_if_بدلا(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, واپس lv_color_black());

    واپس subject->قدر.color;
}

lv_color_t lv_subject_get_پچھلا_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, واپس lv_color_black());

    واپس subject->پچھلا_قدر.color;
}

خالی lv_subject_سیٹ_گروہ_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(list != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروہ, واپس);

    برائے(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_پیغام(list[i] != تہی, واپس, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_گروہ_subject(subject);
    جوڑو_observers_مقصد_گروہ_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_گروہ_element(lv_subject_t * subject, int32_t اشاریہ)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروہ, واپس تہی);
    LV_CHECK_ARG(اشاریہ >= 0 && اشاریہ < subject->سائز, واپس تہی);

    واپس ((lv_subject_t **)(subject->قدر.اشارہ_گر))[اشاریہ];
}

lv_observer_t * lv_subject_جوڑو_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(observer_cb != تہی, واپس تہی);

    lv_observer_t * observer = lv_subject_جوڑو_observer_obj(subject, observer_cb, تہی, user_ڈیٹا);
    اگر(observer == تہی) واپس تہی;

    observer->for_obj = 0;
    واپس observer;
}

lv_observer_t * lv_subject_جوڑو_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(observer_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, واپس تہی);

    lv_observer_t * observer = lv_ll_ins_دم(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    اگر(observer == تہی) واپس تہی;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ڈیٹا = user_ڈیٹا;
    observer->ہدف = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    اگر(obj != تہی) {
        lv_obj_جوڑو_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    واپس observer;
}

lv_observer_t * lv_subject_جوڑو_observer_with_ہدف(lv_subject_t * subject, lv_observer_cb_t observer_cb, خالی * ہدف,
                                                    خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(observer_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, واپس تہی);

    lv_observer_t * observer = lv_ll_ins_دم(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    اگر(observer == تہی) واپس تہی;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ڈیٹا = user_ڈیٹا;
    observer->ہدف = ہدف;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    واپس observer;
}


خالی lv_observer_delete(lv_observer_t * observer)
{
    اگر(observer == تہی) واپس;

    اگر(observer->for_obj && observer->ہدف) {
        lv_obj_remove_event_cb_with_user_ڈیٹا(observer->ہدف, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_ڈیٹا(observer->ہدف, تہی, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    اگر(observer->auto_free_user_ڈیٹا) {
        lv_free(observer->user_ڈیٹا);
    }
    lv_free(observer);
}

خالی lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_گنتی(&obj->spec_attr->event_list) : 0);
    برائے(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        اگر(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_ڈیٹا;
            اگر(subject == تہی || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_ڈیٹا(obj, تہی, subject);

}

خالی * lv_observer_get_ہدف(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != تہی, واپس تہی);

    واپس observer->ہدف;
}

خالی lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != تہی, واپس);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    کرو {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            اگر(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                اگر(subject->notify_restart_query) توڑو;
                observer->notified = 1;
            }
        }
    } جب(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_جوڑو_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_رمز_t trigger, int32_t قدم)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, واپس تہی);

    lv_subject_increment_dsc_t * user_ڈیٹا = lv_malloc(حجم(lv_subject_increment_dsc_t));
    اگر(user_ڈیٹا == تہی) {
        LV_ASSERT_MALLOC(user_ڈیٹا);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        واپس تہی;
    }

    user_ڈیٹا->قدم = قدم;
    user_ڈیٹا->subject = subject;
    user_ڈیٹا->rollover = false;
    user_ڈیٹا->min_قدر = INT32_MIN;
    user_ڈیٹا->max_قدر = INT32_MAX;
    lv_obj_جوڑو_event_cb(obj, subject_increment_cb, trigger, user_ڈیٹا);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, user_ڈیٹا);

    واپس user_ڈیٹا;
}

خالی lv_obj_سیٹ_subject_increment_event_min_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_قدر)
{
    LV_غیر_مستعمل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, واپس);
    LV_CHECK_ARG(dsc != تہی, واپس);

    dsc->min_قدر = min_قدر;
    اگر(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        اگر(dsc->subject->قدر.عدد < min_قدر) {
            lv_subject_سیٹ_int(dsc->subject, min_قدر);
        }
    }
#اگر LV_USE_FLOAT
    ورنہ اگر(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        اگر(dsc->subject->قدر.float_v < (اعشاری)min_قدر) {
            lv_subject_سیٹ_float(dsc->subject, (اعشاری)min_قدر);
        }
    }
#اگر_ختم
}

خالی lv_obj_سیٹ_subject_increment_event_max_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_قدر)
{
    LV_غیر_مستعمل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, واپس);
    LV_CHECK_ARG(dsc != تہی, واپس);

    dsc->max_قدر = max_قدر;
    اگر(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        اگر(dsc->subject->قدر.عدد > max_قدر) {
            lv_subject_سیٹ_int(dsc->subject, max_قدر);
        }
    }
#اگر LV_USE_FLOAT
    ورنہ اگر(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        اگر(dsc->subject->قدر.float_v > (اعشاری)max_قدر) {
            lv_subject_سیٹ_float(dsc->subject, (اعشاری)max_قدر);
        }
    }
#اگر_ختم
}

خالی lv_obj_سیٹ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_غیر_مستعمل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, واپس);
    LV_CHECK_ARG(dsc != تہی, واپس);

    dsc->rollover = rollover;
}

خالی lv_obj_جوڑو_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_رمز_t trigger)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس);

    lv_obj_جوڑو_event_cb(obj, subject_toggle_cb, trigger, subject);
}

خالی lv_obj_جوڑو_subject_سیٹ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_رمز_t trigger, int32_t قدر)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس);

    subject_سیٹ_int_user_ڈیٹا_t * user_ڈیٹا = lv_malloc(حجم(subject_سیٹ_int_user_ڈیٹا_t));
    اگر(user_ڈیٹا == تہی) {
        LV_ASSERT_MALLOC(user_ڈیٹا);
        LV_LOG_WARN("Couldn't allocate user_data");
        واپس;
    }

    user_ڈیٹا->subject = subject;
    user_ڈیٹا->قدر = قدر;

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_int_cb, trigger, user_ڈیٹا);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, user_ڈیٹا);
}

#اگر LV_USE_FLOAT
خالی lv_obj_جوڑو_subject_سیٹ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_رمز_t trigger, اعشاری قدر)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس);

    subject_سیٹ_float_user_ڈیٹا_t * user_ڈیٹا = lv_malloc(حجم(subject_سیٹ_float_user_ڈیٹا_t));
    اگر(user_ڈیٹا == تہی) {
        LV_ASSERT_MALLOC(user_ڈیٹا);
        LV_LOG_WARN("Couldn't allocate user_data");
        واپس;
    }

    user_ڈیٹا->subject = subject;
    user_ڈیٹا->قدر = قدر;

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_float_cb, trigger, user_ڈیٹا);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, user_ڈیٹا);
}
#اگر_ختم /*LV_USE_FLOAT*/

خالی lv_obj_جوڑو_subject_سیٹ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_رمز_t trigger,
                                         ثابت حرف * قدر)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(subject != تہی, واپس);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس);
    LV_CHECK_ARG(قدر != تہی, واپس);

    subject_سیٹ_string_user_ڈیٹا_t * user_ڈیٹا = lv_malloc(حجم(subject_سیٹ_string_user_ڈیٹا_t));
    اگر(user_ڈیٹا == تہی) {
        LV_ASSERT_MALLOC(user_ڈیٹا);
        LV_LOG_WARN("Couldn't allocate user_data");
        واپس;
    }

    user_ڈیٹا->subject = subject;
    user_ڈیٹا->قدر = lv_strdup(قدر);
    اگر(user_ڈیٹا->قدر == تہی) {
        LV_ASSERT_MALLOC(user_ڈیٹا->قدر);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_ڈیٹا);
        واپس;
    }

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_string_cb, trigger, user_ڈیٹا);
    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_string_free_user_ڈیٹا_event_cb, LV_EVENT_DELETE, user_ڈیٹا);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_bool_t سیٹ_bool_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_bool_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس تہی);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_bool_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_bool_cb;
    سیٹ_bool_observer(observable, subject);
    واپس observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_int_t سیٹ_int_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_int_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, واپس تہی);
    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_int_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_int_cb;
    سیٹ_int_observer(observable, subject);
    واپس observable;
}

#اگر LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_float_t سیٹ_float_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_float_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, واپس تہی);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_float_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_float_cb;
    سیٹ_float_observer(observable, subject);
    واپس observable;
}
#اگر_ختم

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_string_t سیٹ_string_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_string_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, واپس تہی);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_string_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_string_cb;
    سیٹ_string_observer(observable, subject);
    واپس observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_color_t سیٹ_color_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_color_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, واپس تہی);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_color_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_color_cb;
    سیٹ_color_observer(observable, subject);
    واپس observable;
}

lv_observer_t * lv_obj_bind_اشارہ_گر(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_اشارہ_گر_t سیٹ_اشارہ_گر_cb)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG(سیٹ_اشارہ_گر_cb != تہی, واپس تہی);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, واپس تہی);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_اشارہ_گر_observer, obj, تہی);
    اگر(observable == تہی) {
        LV_LOG_WARN("Couldn't add observer to subject");
        واپس تہی;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (خالی (*)(خالی))سیٹ_اشارہ_گر_cb;
    سیٹ_اشارہ_گر_observer(observable, subject);
    واپس observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(subject != تہی, واپس تہی);
    LV_CHECK_ARG_FORMAT_پیغام(subject->type == LV_SUBJECT_TYPE_INT, واپس تہی, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_جوڑو_event_cb(obj, obj_قدر_بدلا_event_cb, LV_EVENT_قدر_بدلا, subject);

    واپس observable;
}


lv_obj_t * lv_observer_get_ہدف_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != تہی, واپس تہی);

    واپس (lv_obj_t *)lv_observer_get_ہدف(observer);
}

خالی * lv_observer_get_user_ڈیٹا(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != تہی, واپس تہی);

    واپس observer->user_ڈیٹا;
}

خالی lv_observer_سیٹ_user_ڈیٹا(lv_observer_t * observer, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(observer != تہی, واپس);

    /* Release internally-owned data only when replacing it. */
    اگر(observer->auto_free_user_ڈیٹا && observer->user_ڈیٹا != user_ڈیٹا) {
        lv_free(observer->user_ڈیٹا);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_ڈیٹا = user_ڈیٹا;
    observer->auto_free_user_ڈیٹا = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن خالی init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != تہی);
    lv_memzero(subject, حجم(*subject));
    lv_ll_init(&(subject->subs_ll), حجم(lv_observer_t));
}
ساکن خالی init_int(lv_subject_t * subject, int32_t قدر)
{
    LV_ASSERT(subject != تہی);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->قدر.عدد = قدر;
    subject->پچھلا_قدر.عدد = قدر;
    subject->min_قدر.عدد = INT32_MIN;
    subject->max_قدر.عدد = INT32_MAX;
}

#اگر LV_USE_FLOAT

ساکن خالی init_float(lv_subject_t * subject, اعشاری قدر)
{
    LV_ASSERT(subject != تہی);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->قدر.float_v = قدر;
    subject->پچھلا_قدر.float_v = قدر;
    subject->min_قدر.float_v = -FLT_MAX;
    subject->max_قدر.float_v = FLT_MAX;
}

#اگر_ختم /*LV_USE_FLOAT*/

ساکن خالی init_اشارہ_گر(lv_subject_t * subject, خالی * قدر)
{
    LV_ASSERT(subject != تہی);
    subject->type = LV_SUBJECT_TYPE_اشارہ_گر;
    subject->قدر.اشارہ_گر = قدر;
    subject->پچھلا_قدر.اشارہ_گر = قدر;
}

ساکن خالی init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != تہی);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->قدر.color = color;
    subject->پچھلا_قدر.color = color;
}

ساکن خالی init_گروہ(lv_subject_t * subject, lv_subject_t ** list, سائز_t سائز)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(list != تہی || سائز == 0);
    subject->type = LV_SUBJECT_TYPE_گروہ;
    جوڑو_observers_مقصد_گروہ_subject(subject, list, سائز);
}

ساکن خالی جوڑو_observers_مقصد_گروہ_subject(lv_subject_t * subject, lv_subject_t ** list, سائز_t سائز)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(list != تہی || سائز == 0);
    subject->قدر.اشارہ_گر = list;
    subject->سائز = (uint32_t)سائز;

    برائے(uint32_t i = 0; i < سائز; i++) {
        lv_subject_جوڑو_observer(list[i], گروہ_notify_cb, subject);
    }
}

ساکن خالی init_string(lv_subject_t * subject, حرف * buf, حرف * پچھلا_buf, سائز_t سائز, ثابت حرف * قدر)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(buf != تہی || سائز == 0);

    اگر(سائز > 0) {
        lv_strlcpy(buf, قدر, سائز);
        اگر(پچھلا_buf) lv_strlcpy(پچھلا_buf, قدر, سائز);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->سائز = (uint32_t)سائز;
    subject->قدر.اشارہ_گر = buf;
    subject->پچھلا_قدر.اشارہ_گر = پچھلا_buf;
}

ساکن خالی deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != تہی);
    اگر(subject->type == LV_SUBJECT_TYPE_گروہ) {
        remove_observers_from_گروہ_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_سر(&subject->subs_ll);
    جب(observer) {
        lv_observer_t * observer_اگلا = lv_ll_get_اگلا(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_اگلا;
    }

#اگر LV_USE_EXT_ڈیٹا
    اگر(subject->ext_ڈیٹا.free_cb) {
        subject->ext_ڈیٹا.free_cb(subject->ext_ڈیٹا.ڈیٹا);
        subject->ext_ڈیٹا.ڈیٹا = تہی;
    }
#اگر_ختم

    lv_ll_clear(&subject->subs_ll);
}

ساکن خالی subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    lv_subject_t * subject = lv_event_get_user_ڈیٹا(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_سیٹ_int(subject, v);
}

ساکن خالی subject_سیٹ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    subject_سیٹ_int_user_ڈیٹا_t * user_ڈیٹا = lv_event_get_user_ڈیٹا(e);
    lv_subject_سیٹ_int(user_ڈیٹا->subject, user_ڈیٹا->قدر);
}


#اگر LV_USE_FLOAT
ساکن خالی subject_سیٹ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    subject_سیٹ_float_user_ڈیٹا_t * user_ڈیٹا = lv_event_get_user_ڈیٹا(e);
    lv_subject_سیٹ_float(user_ڈیٹا->subject, user_ڈیٹا->قدر);
}
#اگر_ختم

ساکن خالی subject_سیٹ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    subject_سیٹ_string_user_ڈیٹا_t * user_ڈیٹا = lv_event_get_user_ڈیٹا(e);
    lv_subject_سیٹ_string(user_ڈیٹا->subject, user_ڈیٹا->قدر);
}

ساکن خالی subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    lv_subject_increment_dsc_t * user_ڈیٹا = lv_event_get_user_ڈیٹا(e);

    اگر(user_ڈیٹا->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_قدر = LV_MIN(user_ڈیٹا->max_قدر, user_ڈیٹا->subject->max_قدر.عدد);
        int32_t min_قدر = LV_MAX(user_ڈیٹا->min_قدر, user_ڈیٹا->subject->min_قدر.عدد);

        int32_t قدر = lv_subject_get_int(user_ڈیٹا->subject);
        قدر += user_ڈیٹا->قدم;

        اگر(user_ڈیٹا->rollover) {
            اگر(قدر > max_قدر) {
                قدر = min_قدر;
            }
            ورنہ اگر(قدر < min_قدر) {
                قدر = max_قدر;
            }
        }
        ورنہ {
            قدر = LV_CLAMP(min_قدر, قدر, max_قدر);
        }

        lv_subject_سیٹ_int(user_ڈیٹا->subject, قدر);
    }
#اگر LV_USE_FLOAT
    ورنہ اگر(user_ڈیٹا->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        اعشاری max_قدر = LV_MIN((اعشاری)user_ڈیٹا->max_قدر, user_ڈیٹا->subject->max_قدر.float_v);
        اعشاری min_قدر = LV_MAX((اعشاری)user_ڈیٹا->min_قدر, user_ڈیٹا->subject->min_قدر.float_v);


        اعشاری قدر = lv_subject_get_float(user_ڈیٹا->subject);
        قدر += (اعشاری)user_ڈیٹا->قدم;

        اگر(user_ڈیٹا->rollover) {
            اگر(قدر > max_قدر) {
                قدر = min_قدر;
            }
            ورنہ اگر(قدر < min_قدر) {
                قدر = max_قدر;
            }
        }
        ورنہ {
            قدر = LV_CLAMP(min_قدر, قدر, max_قدر);
        }

        lv_subject_سیٹ_float(user_ڈیٹا->subject, قدر);
    }
#اگر_ختم
}

ساکن خالی گروہ_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != تہی);
    LV_غیر_مستعمل(subject);
    lv_subject_t * subject_گروہ = observer->user_ڈیٹا;
    lv_subject_notify(subject_گروہ);
}

ساکن خالی unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    lv_observer_t * observer = lv_event_get_user_ڈیٹا(e);
    lv_observer_delete(observer);
}

ساکن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(obj != تہی);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(حجم(bound_state_t));
    اگر(p == تہی) {
        LV_LOG_WARN("Out of memory");
        واپس تہی;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_ڈیٹا = 1;

    واپس observable;
}

ساکن خالی obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != تہی);
    LV_ASSERT(subject != تہی);
    bound_state_t * p = observer->user_ڈیٹا;
    LV_ASSERT(p != تہی);

    اگر(subject->قدر.عدد) {
        lv_obj_جوڑو_state(observer->ہدف, p->state);
    }
    ورنہ {
        lv_obj_remove_state(observer->ہدف, p->state);
    }
}

ساکن خالی obj_قدر_بدلا_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    lv_obj_t * obj = lv_event_get_current_ہدف(e);
    lv_subject_t * subject = lv_event_get_user_ڈیٹا(e);
    LV_ASSERT(subject != تہی);
    LV_ASSERT(obj != تہی);

    lv_subject_سیٹ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ساکن خالی lv_subject_notify_if_بدلا(lv_subject_t * subject)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    انتخاب(subject->type) {
        حالت LV_SUBJECT_TYPE_INT :
            اگر(subject->قدر.عدد != subject->پچھلا_قدر.عدد) {
                lv_subject_notify(subject);
            }
            توڑو;
#اگر LV_USE_FLOAT
        حالت LV_SUBJECT_TYPE_FLOAT :
            اگر(subject->قدر.float_v != subject->پچھلا_قدر.float_v) {
                lv_subject_notify(subject);
            }
            توڑو;
#اگر_ختم
        حالت LV_SUBJECT_TYPE_گروہ :
        حالت LV_SUBJECT_TYPE_اشارہ_گر :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            توڑو;
        حالت LV_SUBJECT_TYPE_COLOR  :
            اگر(!lv_color_eq(subject->قدر.color, subject->پچھلا_قدر.color)) {
                lv_subject_notify(subject);
            }
            توڑو;
        حالت LV_SUBJECT_TYPE_STRING:
            اگر(!subject->پچھلا_قدر.اشارہ_گر ||
               lv_strcmp(subject->قدر.اشارہ_گر, subject->پچھلا_قدر.اشارہ_گر)) {
                lv_subject_notify(subject);
            }
            توڑو;
        حالت LV_SUBJECT_TYPE_INVALID :
        حالت LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ساکن خالی subject_سیٹ_string_free_user_ڈیٹا_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    subject_سیٹ_string_user_ڈیٹا_t * user_ڈیٹا = lv_event_get_user_ڈیٹا(e);
    LV_ASSERT(user_ڈیٹا != تہی);
    lv_free((خالی *)user_ڈیٹا->قدر);
    lv_free(user_ڈیٹا);
}

ساکن خالی سیٹ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_bool_t سیٹ_bool_cb = (lv_obj_سیٹ_bool_t)observer->user_cb;
    اگر(سیٹ_bool_cb) سیٹ_bool_cb(obj, subject->قدر.عدد);
}

ساکن خالی سیٹ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_int_t سیٹ_int_cb = (lv_obj_سیٹ_int_t)observer->user_cb;
    اگر(سیٹ_int_cb) سیٹ_int_cb(obj, subject->قدر.عدد);
}

#اگر LV_USE_FLOAT
ساکن خالی سیٹ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_float_t سیٹ_float_cb = (lv_obj_سیٹ_float_t)observer->user_cb;
    اگر(سیٹ_float_cb) سیٹ_float_cb(obj, subject->قدر.float_v);
}
#اگر_ختم /*LV_USE_FLOAT*/

ساکن خالی سیٹ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_string_t سیٹ_string_cb = (lv_obj_سیٹ_string_t)observer->user_cb;
    اگر(سیٹ_string_cb) سیٹ_string_cb(obj, subject->قدر.اشارہ_گر);
}


ساکن خالی سیٹ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_color_t سیٹ_color_cb = (lv_obj_سیٹ_color_t)observer->user_cb;
    اگر(سیٹ_color_cb) سیٹ_color_cb(obj, subject->قدر.color);
}

ساکن خالی سیٹ_اشارہ_گر_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ہدف;
    lv_obj_سیٹ_اشارہ_گر_t سیٹ_اشارہ_گر_cb = (lv_obj_سیٹ_اشارہ_گر_t)observer->user_cb;
    اگر(سیٹ_اشارہ_گر_cb) سیٹ_اشارہ_گر_cb(obj, subject->قدر.اشارہ_گر);
}

ساکن خالی remove_observers_from_گروہ_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_گروہ);

    برائے(uint32_t i = 0; i < subject->سائز; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->قدر.اشارہ_گر))[i];
        subject_remove_observer(sub, گروہ_notify_cb, subject);
    }
}

ساکن خالی subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, خالی * user_ڈیٹا)
{
    LV_ASSERT(subject != تہی);
    LV_ASSERT(cb != تہی);

    lv_observer_t * obs = lv_ll_get_سر(&subject->subs_ll);
    جب(obs) {
        lv_observer_t * obs_اگلا = lv_ll_get_اگلا(&subject->subs_ll, obs);
        اگر(obs->cb == cb && obs->user_ڈیٹا == user_ڈیٹا) {
            lv_observer_delete(obs);
        }
        obs = obs_اگلا;
    }
}

#اگر_ختم /*LV_USE_OBSERVER*/
