/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ضمن "lv_observer_private.h"
#إذا LV_USE_OBSERVER

#ضمن "../lvgl_public.h"
#ضمن "../core/lv_global.h"
#ضمن "../core/lv_obj_private.h"
#ضمن "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#وصف_ڪريو subject_list LV_GLOBAL_DEFAULT()->subject_ll

#جيڪڏهن_وصف_نه_ٿيل FLT_MAX
    #وصف_ڪريو FLT_MAX 3.402823466e+38F
#جيڪڏهن_پورو

/**********************
 *      TYPEDEFS
 **********************/
تعريفنوع بنية {
    lv_state_t state;
} bound_state_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    int32_t قدر;
} subject_سيٽ_int_user_معلومات_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    عائم قدر;
} subject_سيٽ_float_user_معلومات_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    ثابت حرف * قدر;
} subject_سيٽ_string_user_معلومات_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن فراغ remove_observers_from_گروهه_subject(lv_subject_t * subject);
ساكن فراغ جوڙيو_observers_منزل_گروهه_subject(lv_subject_t * subject, lv_subject_t ** list, سائيز_t سائيز);
ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_معلومات);
ساكن فراغ subject_toggle_cb(lv_event_t * e);
ساكن فراغ subject_سيٽ_int_cb(lv_event_t * e);
#إذا LV_USE_FLOAT
    ساكن فراغ subject_سيٽ_float_cb(lv_event_t * e);
#جيڪڏهن_پورو

ساكن فراغ subject_سيٽ_string_cb(lv_event_t * e);
ساكن فراغ subject_increment_cb(lv_event_t * e);

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e);
ساكن فراغ گروهه_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ obj_قدر_بدليو_event_cb(lv_event_t * e);

ساكن فراغ lv_subject_notify_if_بدليو(lv_subject_t * subject);

ساكن فراغ subject_سيٽ_string_free_user_معلومات_event_cb(lv_event_t * e);

ساكن فراغ سيٽ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سيٽ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سيٽ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سيٽ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سيٽ_اشارو_observer(lv_observer_t * observer, lv_subject_t * subject);

#إذا LV_USE_FLOAT
    ساكن فراغ سيٽ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ساكن فراغ init_float(lv_subject_t * subject, عائم قدر);
#جيڪڏهن_پورو /*LV_USE_FLOAT*/

ساكن فراغ init_int(lv_subject_t * subject, int32_t قدر);
ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, سائيز_t سائيز, ثابت حرف * قدر);
ساكن فراغ init_اشارو(lv_subject_t * subject, فراغ * قدر);
ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color);
ساكن فراغ init_گروهه(lv_subject_t * subject, lv_subject_t ** list, سائيز_t سائيز);
ساكن فراغ init_common(lv_subject_t * subject);

ساكن فراغ deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_subject_global_init(فراغ)
{
    lv_ll_init(&subject_list, حجم(lv_subject_t));
}
فراغ lv_subject_global_deinit(فراغ)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        إذا(curr->type == LV_SUBJECT_TYPE_گروهه) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        إذا(curr->type != LV_SUBJECT_TYPE_گروهه) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, أرجع عدم);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, أرجع عدم);
    lv_subject_t * subject = lv_ll_ins_پڇ(&subject_list);
    LV_ASSERT_MALLOC(subject);
    إذا(!subject) {
        أرجع عدم;
    }

    init_common(subject);

    بدل(type) {
        حالة LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            اكسر;
        حالة LV_SUBJECT_TYPE_FLOAT:
#إذا LV_USE_FLOAT
            init_float(subject, 0.f);
#جيڪڏهن_پورو /*LV_USE_FLOAT*/
            اكسر;
        حالة LV_SUBJECT_TYPE_اشارو:
            init_اشارو(subject, عدم);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            اكسر;
        حالة LV_SUBJECT_TYPE_گروهه:
            init_گروهه(subject, عدم, 0);
            اكسر;
        حالة LV_SUBJECT_TYPE_STRING:
            init_string(subject, عدم, عدم, 0, "");
            اكسر;
        حالة LV_SUBJECT_TYPE_INVALID:
        حالة LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    أرجع subject;
}

فراغ lv_subject_delete(lv_subject_t * subject)
{
    إذا(!subject) {
        أرجع;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#إذا LV_USE_EXT_معلومات
فراغ lv_subject_سيٽ_external_معلومات(lv_subject_t * subject, فراغ * معلومات, فراغ (* free_cb)(فراغ * معلومات))
{
    LV_CHECK_ARG(subject != عدم, أرجع);

    subject->ext_معلومات.معلومات = معلومات;
    subject->ext_معلومات.free_cb = free_cb;
}
#جيڪڏهن_پورو

فراغ lv_subject_سيٽ_int(lv_subject_t * subject, int32_t قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    قدر = LV_CLAMP(subject->min_قدر.انگ, قدر, subject->max_قدر.انگ);

    subject->prev_قدر.انگ = subject->قدر.انگ;
    subject->قدر.انگ = قدر;
    lv_subject_notify_if_بدليو(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->قدر.انگ;
}

int32_t lv_subject_get_اڳيون_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->prev_قدر.انگ;
}

فراغ lv_subject_سيٽ_min_قدر_int(lv_subject_t * subject, int32_t min_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->min_قدر.انگ = min_قدر;
}

فراغ lv_subject_سيٽ_max_قدر_int(lv_subject_t * subject, int32_t max_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->max_قدر.انگ = max_قدر;
}

#إذا LV_USE_FLOAT

فراغ lv_subject_سيٽ_float(lv_subject_t * subject, عائم قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    قدر = LV_CLAMP(subject->min_قدر.float_v, قدر, subject->max_قدر.float_v);

    subject->prev_قدر.float_v = subject->قدر.float_v;
    subject->قدر.float_v = قدر;
    lv_subject_notify_if_بدليو(subject);
}

عائم lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->قدر.float_v;
}

عائم lv_subject_get_اڳيون_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->prev_قدر.float_v;
}

فراغ lv_subject_سيٽ_min_قدر_float(lv_subject_t * subject, عائم min_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->min_قدر.float_v = min_قدر;
}

فراغ lv_subject_سيٽ_max_قدر_float(lv_subject_t * subject, عائم max_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->max_قدر.float_v = max_قدر;
}


#جيڪڏهن_پورو /*LV_USE_FLOAT*/

فراغ lv_subject_سيٽ_string_بفر_static(lv_subject_t * subject, حرف * buf, حرف * prev_buf, سائيز_t سائيز)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);
    LV_CHECK_ARG(سائيز > 0, أرجع);

    buf[0] = '\0';
    إذا(prev_buf) prev_buf[0] = '\0';

    subject->سائيز = (uint32_t)سائيز;
    subject->قدر.اشارو = buf;
    subject->prev_قدر.اشارو = prev_buf;
}

فراغ lv_subject_سيٽ_string(lv_subject_t * subject, ثابت حرف * buf)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);

    إذا(subject->سائيز == 0) أرجع;
    إذا(subject->prev_قدر.اشارو) {
        lv_strlcpy((حرف *)subject->prev_قدر.اشارو, subject->قدر.اشارو, subject->سائيز);
    }

    lv_strlcpy((حرف *)subject->قدر.اشارو, buf, subject->سائيز);

    lv_subject_notify_if_بدليو(subject);
}

فراغ lv_subject_snprintf(lv_subject_t * subject, ثابت حرف * format, ...)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(format != عدم, أرجع);

    إذا(subject->سائيز < 1U) أرجع;

    إذا(subject->prev_قدر.اشارو) {
        lv_strlcpy((حرف *)subject->prev_قدر.اشارو, subject->قدر.اشارو, subject->سائيز);
    }

    va_list va;
    va_شروعات(va, format);
    ثابت صحيح ret = lv_vsnprintf((حرف *)subject->قدر.اشارو, subject->سائيز, format, va);
    LV_اڻ_استعمال_ٿيل(ret);
    va_پڇاڙي(va);

    lv_subject_notify_if_بدليو(subject);
}

ثابت حرف * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->قدر.اشارو;
}

ثابت حرف * lv_subject_get_اڳيون_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->prev_قدر.اشارو;
}

فراغ lv_subject_سيٽ_اشارو(lv_subject_t * subject, فراغ * ptr)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارو, أرجع);

    subject->prev_قدر.اشارو = subject->قدر.اشارو;
    subject->قدر.اشارو = ptr;
    lv_subject_notify_if_بدليو(subject);
}

ثابت فراغ * lv_subject_get_اشارو(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارو, أرجع عدم);


    أرجع subject->قدر.اشارو;
}

ثابت فراغ * lv_subject_get_اڳيون_اشارو(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارو, أرجع عدم);

    أرجع subject->prev_قدر.اشارو;
}

فراغ lv_subject_سيٽ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع);

    subject->prev_قدر.color = subject->قدر.color;
    subject->قدر.color = color;
    lv_subject_notify_if_بدليو(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->قدر.color;
}

lv_color_t lv_subject_get_اڳيون_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->prev_قدر.color;
}

فراغ lv_subject_سيٽ_گروهه_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(list != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروهه, أرجع);

    لكل(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_نياپو(list[i] != عدم, أرجع, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_گروهه_subject(subject);
    جوڙيو_observers_منزل_گروهه_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_گروهه_element(lv_subject_t * subject, int32_t اشاريو)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروهه, أرجع عدم);
    LV_CHECK_ARG(اشاريو >= 0 && اشاريو < subject->سائيز, أرجع عدم);

    أرجع ((lv_subject_t **)(subject->قدر.اشارو))[اشاريو];
}

lv_observer_t * lv_subject_جوڙيو_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);

    lv_observer_t * observer = lv_subject_جوڙيو_observer_obj(subject, observer_cb, عدم, user_معلومات);
    إذا(observer == عدم) أرجع عدم;

    observer->for_obj = 0;
    أرجع observer;
}

lv_observer_t * lv_subject_جوڙيو_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_پڇ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_معلومات = user_معلومات;
    observer->هدف = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    إذا(obj != عدم) {
        lv_obj_جوڙيو_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}

lv_observer_t * lv_subject_جوڙيو_observer_with_هدف(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * هدف,
                                                    فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_پڇ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_معلومات = user_معلومات;
    observer->هدف = هدف;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}


فراغ lv_observer_delete(lv_observer_t * observer)
{
    إذا(observer == عدم) أرجع;

    إذا(observer->for_obj && observer->هدف) {
        lv_obj_remove_event_cb_with_user_معلومات(observer->هدف, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_معلومات(observer->هدف, عدم, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    إذا(observer->auto_free_user_معلومات) {
        lv_free(observer->user_معلومات);
    }
    lv_free(observer);
}

فراغ lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_ڳڻپ(&obj->spec_attr->event_list) : 0);
    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        إذا(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_معلومات;
            إذا(subject == عدم || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_معلومات(obj, عدم, subject);

}

فراغ * lv_observer_get_هدف(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->هدف;
}

فراغ lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    افعل {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            إذا(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                إذا(subject->notify_restart_query) اكسر;
                observer->notified = 1;
            }
        }
    } طالما(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_جوڙيو_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_ڪوڊ_t trigger, int32_t قدم)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_subject_increment_dsc_t * user_معلومات = lv_malloc(حجم(lv_subject_increment_dsc_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        أرجع عدم;
    }

    user_معلومات->قدم = قدم;
    user_معلومات->subject = subject;
    user_معلومات->rollover = false;
    user_معلومات->min_قدر = INT32_MIN;
    user_معلومات->max_قدر = INT32_MAX;
    lv_obj_جوڙيو_event_cb(obj, subject_increment_cb, trigger, user_معلومات);
    lv_obj_جوڙيو_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);

    أرجع user_معلومات;
}

فراغ lv_obj_سيٽ_subject_increment_event_min_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_قدر)
{
    LV_اڻ_استعمال_ٿيل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->min_قدر = min_قدر;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->قدر.انگ < min_قدر) {
            lv_subject_سيٽ_int(dsc->subject, min_قدر);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->قدر.float_v < (عائم)min_قدر) {
            lv_subject_سيٽ_float(dsc->subject, (عائم)min_قدر);
        }
    }
#جيڪڏهن_پورو
}

فراغ lv_obj_سيٽ_subject_increment_event_max_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_قدر)
{
    LV_اڻ_استعمال_ٿيل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->max_قدر = max_قدر;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->قدر.انگ > max_قدر) {
            lv_subject_سيٽ_int(dsc->subject, max_قدر);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->قدر.float_v > (عائم)max_قدر) {
            lv_subject_سيٽ_float(dsc->subject, (عائم)max_قدر);
        }
    }
#جيڪڏهن_پورو
}

فراغ lv_obj_سيٽ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_اڻ_استعمال_ٿيل(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->rollover = rollover;
}

فراغ lv_obj_جوڙيو_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ڪوڊ_t trigger)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    lv_obj_جوڙيو_event_cb(obj, subject_toggle_cb, trigger, subject);
}

فراغ lv_obj_جوڙيو_subject_سيٽ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ڪوڊ_t trigger, int32_t قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject_سيٽ_int_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سيٽ_int_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->قدر = قدر;

    lv_obj_جوڙيو_event_cb(obj, subject_سيٽ_int_cb, trigger, user_معلومات);
    lv_obj_جوڙيو_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);
}

#إذا LV_USE_FLOAT
فراغ lv_obj_جوڙيو_subject_سيٽ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ڪوڊ_t trigger, عائم قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject_سيٽ_float_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سيٽ_float_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->قدر = قدر;

    lv_obj_جوڙيو_event_cb(obj, subject_سيٽ_float_cb, trigger, user_معلومات);
    lv_obj_جوڙيو_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);
}
#جيڪڏهن_پورو /*LV_USE_FLOAT*/

فراغ lv_obj_جوڙيو_subject_سيٽ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ڪوڊ_t trigger,
                                         ثابت حرف * قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(قدر != عدم, أرجع);

    subject_سيٽ_string_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سيٽ_string_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->قدر = lv_strdup(قدر);
    إذا(user_معلومات->قدر == عدم) {
        LV_ASSERT_MALLOC(user_معلومات->قدر);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_معلومات);
        أرجع;
    }

    lv_obj_جوڙيو_event_cb(obj, subject_سيٽ_string_cb, trigger, user_معلومات);
    lv_obj_جوڙيو_event_cb(obj, subject_سيٽ_string_free_user_معلومات_event_cb, LV_EVENT_DELETE, user_معلومات);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_bool_t سيٽ_bool_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_bool_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_bool_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_bool_cb;
    سيٽ_bool_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_int_t سيٽ_int_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_int_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);
    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_int_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_int_cb;
    سيٽ_int_observer(observable, subject);
    أرجع observable;
}

#إذا LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_float_t سيٽ_float_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_float_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_float_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_float_cb;
    سيٽ_float_observer(observable, subject);
    أرجع observable;
}
#جيڪڏهن_پورو

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_string_t سيٽ_string_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_string_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_string_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_string_cb;
    سيٽ_string_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_color_t سيٽ_color_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_color_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_color_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_color_cb;
    سيٽ_color_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_اشارو(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سيٽ_اشارو_t سيٽ_اشارو_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سيٽ_اشارو_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارو, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, سيٽ_اشارو_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سيٽ_اشارو_cb;
    سيٽ_اشارو_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_نياپو(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_جوڙيو_event_cb(obj, obj_قدر_بدليو_event_cb, LV_EVENT_قدر_بدليو, subject);

    أرجع observable;
}


lv_obj_t * lv_observer_get_هدف_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع (lv_obj_t *)lv_observer_get_هدف(observer);
}

فراغ * lv_observer_get_user_معلومات(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->user_معلومات;
}

فراغ lv_observer_سيٽ_user_معلومات(lv_observer_t * observer, فراغ * user_معلومات)
{
    LV_CHECK_ARG(observer != عدم, أرجع);

    /* Release internally-owned data only when replacing it. */
    إذا(observer->auto_free_user_معلومات && observer->user_معلومات != user_معلومات) {
        lv_free(observer->user_معلومات);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_معلومات = user_معلومات;
    observer->auto_free_user_معلومات = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    lv_memzero(subject, حجم(*subject));
    lv_ll_init(&(subject->subs_ll), حجم(lv_observer_t));
}
ساكن فراغ init_int(lv_subject_t * subject, int32_t قدر)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->قدر.انگ = قدر;
    subject->prev_قدر.انگ = قدر;
    subject->min_قدر.انگ = INT32_MIN;
    subject->max_قدر.انگ = INT32_MAX;
}

#إذا LV_USE_FLOAT

ساكن فراغ init_float(lv_subject_t * subject, عائم قدر)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->قدر.float_v = قدر;
    subject->prev_قدر.float_v = قدر;
    subject->min_قدر.float_v = -FLT_MAX;
    subject->max_قدر.float_v = FLT_MAX;
}

#جيڪڏهن_پورو /*LV_USE_FLOAT*/

ساكن فراغ init_اشارو(lv_subject_t * subject, فراغ * قدر)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_اشارو;
    subject->قدر.اشارو = قدر;
    subject->prev_قدر.اشارو = قدر;
}

ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->قدر.color = color;
    subject->prev_قدر.color = color;
}

ساكن فراغ init_گروهه(lv_subject_t * subject, lv_subject_t ** list, سائيز_t سائيز)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || سائيز == 0);
    subject->type = LV_SUBJECT_TYPE_گروهه;
    جوڙيو_observers_منزل_گروهه_subject(subject, list, سائيز);
}

ساكن فراغ جوڙيو_observers_منزل_گروهه_subject(lv_subject_t * subject, lv_subject_t ** list, سائيز_t سائيز)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || سائيز == 0);
    subject->قدر.اشارو = list;
    subject->سائيز = (uint32_t)سائيز;

    لكل(uint32_t i = 0; i < سائيز; i++) {
        lv_subject_جوڙيو_observer(list[i], گروهه_notify_cb, subject);
    }
}

ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, سائيز_t سائيز, ثابت حرف * قدر)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(buf != عدم || سائيز == 0);

    إذا(سائيز > 0) {
        lv_strlcpy(buf, قدر, سائيز);
        إذا(prev_buf) lv_strlcpy(prev_buf, قدر, سائيز);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->سائيز = (uint32_t)سائيز;
    subject->قدر.اشارو = buf;
    subject->prev_قدر.اشارو = prev_buf;
}

ساكن فراغ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    إذا(subject->type == LV_SUBJECT_TYPE_گروهه) {
        remove_observers_from_گروهه_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_سر(&subject->subs_ll);
    طالما(observer) {
        lv_observer_t * observer_ايندڙ = lv_ll_get_ايندڙ(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_ايندڙ;
    }

#إذا LV_USE_EXT_معلومات
    إذا(subject->ext_معلومات.free_cb) {
        subject->ext_معلومات.free_cb(subject->ext_معلومات.معلومات);
        subject->ext_معلومات.معلومات = عدم;
    }
#جيڪڏهن_پورو

    lv_ll_clear(&subject->subs_ll);
}

ساكن فراغ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_t * subject = lv_event_get_user_معلومات(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_سيٽ_int(subject, v);
}

ساكن فراغ subject_سيٽ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سيٽ_int_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سيٽ_int(user_معلومات->subject, user_معلومات->قدر);
}


#إذا LV_USE_FLOAT
ساكن فراغ subject_سيٽ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سيٽ_float_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سيٽ_float(user_معلومات->subject, user_معلومات->قدر);
}
#جيڪڏهن_پورو

ساكن فراغ subject_سيٽ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سيٽ_string_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سيٽ_string(user_معلومات->subject, user_معلومات->قدر);
}

ساكن فراغ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_increment_dsc_t * user_معلومات = lv_event_get_user_معلومات(e);

    إذا(user_معلومات->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_قدر = LV_MIN(user_معلومات->max_قدر, user_معلومات->subject->max_قدر.انگ);
        int32_t min_قدر = LV_MAX(user_معلومات->min_قدر, user_معلومات->subject->min_قدر.انگ);

        int32_t قدر = lv_subject_get_int(user_معلومات->subject);
        قدر += user_معلومات->قدم;

        إذا(user_معلومات->rollover) {
            إذا(قدر > max_قدر) {
                قدر = min_قدر;
            }
            وإلا إذا(قدر < min_قدر) {
                قدر = max_قدر;
            }
        }
        وإلا {
            قدر = LV_CLAMP(min_قدر, قدر, max_قدر);
        }

        lv_subject_سيٽ_int(user_معلومات->subject, قدر);
    }
#إذا LV_USE_FLOAT
    وإلا إذا(user_معلومات->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        عائم max_قدر = LV_MIN((عائم)user_معلومات->max_قدر, user_معلومات->subject->max_قدر.float_v);
        عائم min_قدر = LV_MAX((عائم)user_معلومات->min_قدر, user_معلومات->subject->min_قدر.float_v);


        عائم قدر = lv_subject_get_float(user_معلومات->subject);
        قدر += (عائم)user_معلومات->قدم;

        إذا(user_معلومات->rollover) {
            إذا(قدر > max_قدر) {
                قدر = min_قدر;
            }
            وإلا إذا(قدر < min_قدر) {
                قدر = max_قدر;
            }
        }
        وإلا {
            قدر = LV_CLAMP(min_قدر, قدر, max_قدر);
        }

        lv_subject_سيٽ_float(user_معلومات->subject, قدر);
    }
#جيڪڏهن_پورو
}

ساكن فراغ گروهه_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_اڻ_استعمال_ٿيل(subject);
    lv_subject_t * subject_گروهه = observer->user_معلومات;
    lv_subject_notify(subject_گروهه);
}

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_observer_t * observer = lv_event_get_user_معلومات(e);
    lv_observer_delete(observer);
}

ساكن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(obj != عدم);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(حجم(bound_state_t));
    إذا(p == عدم) {
        LV_LOG_WARN("Out of memory");
        أرجع عدم;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_معلومات = 1;

    أرجع observable;
}

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_ASSERT(subject != عدم);
    bound_state_t * p = observer->user_معلومات;
    LV_ASSERT(p != عدم);

    إذا(subject->قدر.انگ) {
        lv_obj_جوڙيو_state(observer->هدف, p->state);
    }
    وإلا {
        lv_obj_remove_state(observer->هدف, p->state);
    }
}

ساكن فراغ obj_قدر_بدليو_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_current_هدف(e);
    lv_subject_t * subject = lv_event_get_user_معلومات(e);
    LV_ASSERT(subject != عدم);
    LV_ASSERT(obj != عدم);

    lv_subject_سيٽ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ساكن فراغ lv_subject_notify_if_بدليو(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    بدل(subject->type) {
        حالة LV_SUBJECT_TYPE_INT :
            إذا(subject->قدر.انگ != subject->prev_قدر.انگ) {
                lv_subject_notify(subject);
            }
            اكسر;
#إذا LV_USE_FLOAT
        حالة LV_SUBJECT_TYPE_FLOAT :
            إذا(subject->قدر.float_v != subject->prev_قدر.float_v) {
                lv_subject_notify(subject);
            }
            اكسر;
#جيڪڏهن_پورو
        حالة LV_SUBJECT_TYPE_گروهه :
        حالة LV_SUBJECT_TYPE_اشارو :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR  :
            إذا(!lv_color_eq(subject->قدر.color, subject->prev_قدر.color)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_STRING:
            إذا(!subject->prev_قدر.اشارو ||
               lv_strcmp(subject->قدر.اشارو, subject->prev_قدر.اشارو)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_INVALID :
        حالة LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ساكن فراغ subject_سيٽ_string_free_user_معلومات_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سيٽ_string_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    LV_ASSERT(user_معلومات != عدم);
    lv_free((فراغ *)user_معلومات->قدر);
    lv_free(user_معلومات);
}

ساكن فراغ سيٽ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_bool_t سيٽ_bool_cb = (lv_obj_سيٽ_bool_t)observer->user_cb;
    إذا(سيٽ_bool_cb) سيٽ_bool_cb(obj, subject->قدر.انگ);
}

ساكن فراغ سيٽ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_int_t سيٽ_int_cb = (lv_obj_سيٽ_int_t)observer->user_cb;
    إذا(سيٽ_int_cb) سيٽ_int_cb(obj, subject->قدر.انگ);
}

#إذا LV_USE_FLOAT
ساكن فراغ سيٽ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_float_t سيٽ_float_cb = (lv_obj_سيٽ_float_t)observer->user_cb;
    إذا(سيٽ_float_cb) سيٽ_float_cb(obj, subject->قدر.float_v);
}
#جيڪڏهن_پورو /*LV_USE_FLOAT*/

ساكن فراغ سيٽ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_string_t سيٽ_string_cb = (lv_obj_سيٽ_string_t)observer->user_cb;
    إذا(سيٽ_string_cb) سيٽ_string_cb(obj, subject->قدر.اشارو);
}


ساكن فراغ سيٽ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_color_t سيٽ_color_cb = (lv_obj_سيٽ_color_t)observer->user_cb;
    إذا(سيٽ_color_cb) سيٽ_color_cb(obj, subject->قدر.color);
}

ساكن فراغ سيٽ_اشارو_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_سيٽ_اشارو_t سيٽ_اشارو_cb = (lv_obj_سيٽ_اشارو_t)observer->user_cb;
    إذا(سيٽ_اشارو_cb) سيٽ_اشارو_cb(obj, subject->قدر.اشارو);
}

ساكن فراغ remove_observers_from_گروهه_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_گروهه);

    لكل(uint32_t i = 0; i < subject->سائيز; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->قدر.اشارو))[i];
        subject_remove_observer(sub, گروهه_notify_cb, subject);
    }
}

ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_معلومات)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(cb != عدم);

    lv_observer_t * obs = lv_ll_get_سر(&subject->subs_ll);
    طالما(obs) {
        lv_observer_t * obs_ايندڙ = lv_ll_get_ايندڙ(&subject->subs_ll, obs);
        إذا(obs->cb == cb && obs->user_معلومات == user_معلومات) {
            lv_observer_delete(obs);
        }
        obs = obs_ايندڙ;
    }
}

#جيڪڏهن_پورو /*LV_USE_OBSERVER*/
