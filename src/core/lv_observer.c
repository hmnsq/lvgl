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


#تعریف_کرو subject_list LV_GLOBAL_DEFAULT()->subject_ll

#جے_بنا_تعریف FLT_MAX
    #تعریف_کرو FLT_MAX 3.402823466e+38F
#جے_انت

/**********************
 *      TYPEDEFS
 **********************/
تعريفنوع بنية {
    lv_state_t state;
} bound_state_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    int32_t قدر;
} subject_سیٹ_int_user_انکڑے_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    عائم قدر;
} subject_سیٹ_float_user_انکڑے_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    ثابت حرف * قدر;
} subject_سیٹ_string_user_انکڑے_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن فراغ remove_observers_from_گروہ_subject(lv_subject_t * subject);
ساكن فراغ جوڑو_observers_منزل_گروہ_subject(lv_subject_t * subject, lv_subject_t ** list, ناپ_t ناپ);
ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_انکڑے);
ساكن فراغ subject_toggle_cb(lv_event_t * e);
ساكن فراغ subject_سیٹ_int_cb(lv_event_t * e);
#إذا LV_USE_FLOAT
    ساكن فراغ subject_سیٹ_float_cb(lv_event_t * e);
#جے_انت

ساكن فراغ subject_سیٹ_string_cb(lv_event_t * e);
ساكن فراغ subject_increment_cb(lv_event_t * e);

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e);
ساكن فراغ گروہ_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ obj_قدر_بدلیا_event_cb(lv_event_t * e);

ساكن فراغ lv_subject_notify_if_بدلیا(lv_subject_t * subject);

ساكن فراغ subject_سیٹ_string_free_user_انکڑے_event_cb(lv_event_t * e);

ساكن فراغ سیٹ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سیٹ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سیٹ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سیٹ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سیٹ_اشارہ_گر_observer(lv_observer_t * observer, lv_subject_t * subject);

#إذا LV_USE_FLOAT
    ساكن فراغ سیٹ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ساكن فراغ init_float(lv_subject_t * subject, عائم قدر);
#جے_انت /*LV_USE_FLOAT*/

ساكن فراغ init_int(lv_subject_t * subject, int32_t قدر);
ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, ناپ_t ناپ, ثابت حرف * قدر);
ساكن فراغ init_اشارہ_گر(lv_subject_t * subject, فراغ * قدر);
ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color);
ساكن فراغ init_گروہ(lv_subject_t * subject, lv_subject_t ** list, ناپ_t ناپ);
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
        إذا(curr->type == LV_SUBJECT_TYPE_گروہ) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        إذا(curr->type != LV_SUBJECT_TYPE_گروہ) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, أرجع عدم);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, أرجع عدم);
    lv_subject_t * subject = lv_ll_ins_پوچھ(&subject_list);
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
#جے_انت /*LV_USE_FLOAT*/
            اكسر;
        حالة LV_SUBJECT_TYPE_اشارہ_گر:
            init_اشارہ_گر(subject, عدم);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            اكسر;
        حالة LV_SUBJECT_TYPE_گروہ:
            init_گروہ(subject, عدم, 0);
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

#إذا LV_USE_EXT_انکڑے
فراغ lv_subject_سیٹ_external_انکڑے(lv_subject_t * subject, فراغ * انکڑے, فراغ (* free_cb)(فراغ * انکڑے))
{
    LV_CHECK_ARG(subject != عدم, أرجع);

    subject->ext_انکڑے.انکڑے = انکڑے;
    subject->ext_انکڑے.free_cb = free_cb;
}
#جے_انت

فراغ lv_subject_سیٹ_int(lv_subject_t * subject, int32_t قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    قدر = LV_CLAMP(subject->min_قدر.عدد, قدر, subject->max_قدر.عدد);

    subject->prev_قدر.عدد = subject->قدر.عدد;
    subject->قدر.عدد = قدر;
    lv_subject_notify_if_بدلیا(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->قدر.عدد;
}

int32_t lv_subject_get_پچھلا_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->prev_قدر.عدد;
}

فراغ lv_subject_سیٹ_min_قدر_int(lv_subject_t * subject, int32_t min_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->min_قدر.عدد = min_قدر;
}

فراغ lv_subject_سیٹ_max_قدر_int(lv_subject_t * subject, int32_t max_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->max_قدر.عدد = max_قدر;
}

#إذا LV_USE_FLOAT

فراغ lv_subject_سیٹ_float(lv_subject_t * subject, عائم قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    قدر = LV_CLAMP(subject->min_قدر.float_v, قدر, subject->max_قدر.float_v);

    subject->prev_قدر.float_v = subject->قدر.float_v;
    subject->قدر.float_v = قدر;
    lv_subject_notify_if_بدلیا(subject);
}

عائم lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->قدر.float_v;
}

عائم lv_subject_get_پچھلا_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->prev_قدر.float_v;
}

فراغ lv_subject_سیٹ_min_قدر_float(lv_subject_t * subject, عائم min_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->min_قدر.float_v = min_قدر;
}

فراغ lv_subject_سیٹ_max_قدر_float(lv_subject_t * subject, عائم max_قدر)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->max_قدر.float_v = max_قدر;
}


#جے_انت /*LV_USE_FLOAT*/

فراغ lv_subject_سیٹ_string_بفر_static(lv_subject_t * subject, حرف * buf, حرف * prev_buf, ناپ_t ناپ)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);
    LV_CHECK_ARG(ناپ > 0, أرجع);

    buf[0] = '\0';
    إذا(prev_buf) prev_buf[0] = '\0';

    subject->ناپ = (uint32_t)ناپ;
    subject->قدر.اشارہ_گر = buf;
    subject->prev_قدر.اشارہ_گر = prev_buf;
}

فراغ lv_subject_سیٹ_string(lv_subject_t * subject, ثابت حرف * buf)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);

    إذا(subject->ناپ == 0) أرجع;
    إذا(subject->prev_قدر.اشارہ_گر) {
        lv_strlcpy((حرف *)subject->prev_قدر.اشارہ_گر, subject->قدر.اشارہ_گر, subject->ناپ);
    }

    lv_strlcpy((حرف *)subject->قدر.اشارہ_گر, buf, subject->ناپ);

    lv_subject_notify_if_بدلیا(subject);
}

فراغ lv_subject_snprintf(lv_subject_t * subject, ثابت حرف * format, ...)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(format != عدم, أرجع);

    إذا(subject->ناپ < 1U) أرجع;

    إذا(subject->prev_قدر.اشارہ_گر) {
        lv_strlcpy((حرف *)subject->prev_قدر.اشارہ_گر, subject->قدر.اشارہ_گر, subject->ناپ);
    }

    va_list va;
    va_شروع(va, format);
    ثابت صحيح ret = lv_vsnprintf((حرف *)subject->قدر.اشارہ_گر, subject->ناپ, format, va);
    LV_بن_ورتیا(ret);
    va_سرا(va);

    lv_subject_notify_if_بدلیا(subject);
}

ثابت حرف * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->قدر.اشارہ_گر;
}

ثابت حرف * lv_subject_get_پچھلا_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->prev_قدر.اشارہ_گر;
}

فراغ lv_subject_سیٹ_اشارہ_گر(lv_subject_t * subject, فراغ * ptr)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, أرجع);

    subject->prev_قدر.اشارہ_گر = subject->قدر.اشارہ_گر;
    subject->قدر.اشارہ_گر = ptr;
    lv_subject_notify_if_بدلیا(subject);
}

ثابت فراغ * lv_subject_get_اشارہ_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, أرجع عدم);


    أرجع subject->قدر.اشارہ_گر;
}

ثابت فراغ * lv_subject_get_پچھلا_اشارہ_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, أرجع عدم);

    أرجع subject->prev_قدر.اشارہ_گر;
}

فراغ lv_subject_سیٹ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع);

    subject->prev_قدر.color = subject->قدر.color;
    subject->قدر.color = color;
    lv_subject_notify_if_بدلیا(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->قدر.color;
}

lv_color_t lv_subject_get_پچھلا_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->prev_قدر.color;
}

فراغ lv_subject_سیٹ_گروہ_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(list != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروہ, أرجع);

    لكل(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_پیغام(list[i] != عدم, أرجع, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_گروہ_subject(subject);
    جوڑو_observers_منزل_گروہ_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_گروہ_element(lv_subject_t * subject, int32_t اشاریہ)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروہ, أرجع عدم);
    LV_CHECK_ARG(اشاریہ >= 0 && اشاریہ < subject->ناپ, أرجع عدم);

    أرجع ((lv_subject_t **)(subject->قدر.اشارہ_گر))[اشاریہ];
}

lv_observer_t * lv_subject_جوڑو_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);

    lv_observer_t * observer = lv_subject_جوڑو_observer_obj(subject, observer_cb, عدم, user_انکڑے);
    إذا(observer == عدم) أرجع عدم;

    observer->for_obj = 0;
    أرجع observer;
}

lv_observer_t * lv_subject_جوڑو_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            فراغ * user_انکڑے)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_پوچھ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_انکڑے = user_انکڑے;
    observer->نشانہ = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    إذا(obj != عدم) {
        lv_obj_جوڑو_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}

lv_observer_t * lv_subject_جوڑو_observer_with_نشانہ(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * نشانہ,
                                                    فراغ * user_انکڑے)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_پوچھ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_انکڑے = user_انکڑے;
    observer->نشانہ = نشانہ;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}


فراغ lv_observer_delete(lv_observer_t * observer)
{
    إذا(observer == عدم) أرجع;

    إذا(observer->for_obj && observer->نشانہ) {
        lv_obj_remove_event_cb_with_user_انکڑے(observer->نشانہ, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_انکڑے(observer->نشانہ, عدم, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    إذا(observer->auto_free_user_انکڑے) {
        lv_free(observer->user_انکڑے);
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
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_گنتی(&obj->spec_attr->event_list) : 0);
    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        إذا(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_انکڑے;
            إذا(subject == عدم || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_انکڑے(obj, عدم, subject);

}

فراغ * lv_observer_get_نشانہ(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->نشانہ;
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

lv_subject_increment_dsc_t * lv_obj_جوڑو_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_کوڈ_t trigger, int32_t قدم)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_subject_increment_dsc_t * user_انکڑے = lv_malloc(حجم(lv_subject_increment_dsc_t));
    إذا(user_انکڑے == عدم) {
        LV_ASSERT_MALLOC(user_انکڑے);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        أرجع عدم;
    }

    user_انکڑے->قدم = قدم;
    user_انکڑے->subject = subject;
    user_انکڑے->rollover = false;
    user_انکڑے->min_قدر = INT32_MIN;
    user_انکڑے->max_قدر = INT32_MAX;
    lv_obj_جوڑو_event_cb(obj, subject_increment_cb, trigger, user_انکڑے);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, user_انکڑے);

    أرجع user_انکڑے;
}

فراغ lv_obj_سیٹ_subject_increment_event_min_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_قدر)
{
    LV_بن_ورتیا(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->min_قدر = min_قدر;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->قدر.عدد < min_قدر) {
            lv_subject_سیٹ_int(dsc->subject, min_قدر);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->قدر.float_v < (عائم)min_قدر) {
            lv_subject_سیٹ_float(dsc->subject, (عائم)min_قدر);
        }
    }
#جے_انت
}

فراغ lv_obj_سیٹ_subject_increment_event_max_قدر(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_قدر)
{
    LV_بن_ورتیا(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->max_قدر = max_قدر;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->قدر.عدد > max_قدر) {
            lv_subject_سیٹ_int(dsc->subject, max_قدر);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->قدر.float_v > (عائم)max_قدر) {
            lv_subject_سیٹ_float(dsc->subject, (عائم)max_قدر);
        }
    }
#جے_انت
}

فراغ lv_obj_سیٹ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_بن_ورتیا(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->rollover = rollover;
}

فراغ lv_obj_جوڑو_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوڈ_t trigger)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    lv_obj_جوڑو_event_cb(obj, subject_toggle_cb, trigger, subject);
}

فراغ lv_obj_جوڑو_subject_سیٹ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوڈ_t trigger, int32_t قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject_سیٹ_int_user_انکڑے_t * user_انکڑے = lv_malloc(حجم(subject_سیٹ_int_user_انکڑے_t));
    إذا(user_انکڑے == عدم) {
        LV_ASSERT_MALLOC(user_انکڑے);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_انکڑے->subject = subject;
    user_انکڑے->قدر = قدر;

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_int_cb, trigger, user_انکڑے);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, user_انکڑے);
}

#إذا LV_USE_FLOAT
فراغ lv_obj_جوڑو_subject_سیٹ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوڈ_t trigger, عائم قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject_سیٹ_float_user_انکڑے_t * user_انکڑے = lv_malloc(حجم(subject_سیٹ_float_user_انکڑے_t));
    إذا(user_انکڑے == عدم) {
        LV_ASSERT_MALLOC(user_انکڑے);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_انکڑے->subject = subject;
    user_انکڑے->قدر = قدر;

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_float_cb, trigger, user_انکڑے);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, user_انکڑے);
}
#جے_انت /*LV_USE_FLOAT*/

فراغ lv_obj_جوڑو_subject_سیٹ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوڈ_t trigger,
                                         ثابت حرف * قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(قدر != عدم, أرجع);

    subject_سیٹ_string_user_انکڑے_t * user_انکڑے = lv_malloc(حجم(subject_سیٹ_string_user_انکڑے_t));
    إذا(user_انکڑے == عدم) {
        LV_ASSERT_MALLOC(user_انکڑے);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_انکڑے->subject = subject;
    user_انکڑے->قدر = lv_strdup(قدر);
    إذا(user_انکڑے->قدر == عدم) {
        LV_ASSERT_MALLOC(user_انکڑے->قدر);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_انکڑے);
        أرجع;
    }

    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_string_cb, trigger, user_انکڑے);
    lv_obj_جوڑو_event_cb(obj, subject_سیٹ_string_free_user_انکڑے_event_cb, LV_EVENT_DELETE, user_انکڑے);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_bool_t سیٹ_bool_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_bool_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_bool_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_bool_cb;
    سیٹ_bool_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_int_t سیٹ_int_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_int_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);
    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_int_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_int_cb;
    سیٹ_int_observer(observable, subject);
    أرجع observable;
}

#إذا LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_float_t سیٹ_float_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_float_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_float_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_float_cb;
    سیٹ_float_observer(observable, subject);
    أرجع observable;
}
#جے_انت

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_string_t سیٹ_string_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_string_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_string_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_string_cb;
    سیٹ_string_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_color_t سیٹ_color_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_color_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_color_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_color_cb;
    سیٹ_color_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_اشارہ_گر(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سیٹ_اشارہ_گر_t سیٹ_اشارہ_گر_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سیٹ_اشارہ_گر_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشارہ_گر, أرجع عدم);

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, سیٹ_اشارہ_گر_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سیٹ_اشارہ_گر_cb;
    سیٹ_اشارہ_گر_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_جوڑو_event_cb(obj, obj_قدر_بدلیا_event_cb, LV_EVENT_قدر_بدلیا, subject);

    أرجع observable;
}


lv_obj_t * lv_observer_get_نشانہ_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع (lv_obj_t *)lv_observer_get_نشانہ(observer);
}

فراغ * lv_observer_get_user_انکڑے(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->user_انکڑے;
}

فراغ lv_observer_سیٹ_user_انکڑے(lv_observer_t * observer, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(observer != عدم, أرجع);

    /* Release internally-owned data only when replacing it. */
    إذا(observer->auto_free_user_انکڑے && observer->user_انکڑے != user_انکڑے) {
        lv_free(observer->user_انکڑے);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_انکڑے = user_انکڑے;
    observer->auto_free_user_انکڑے = 0;
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
    subject->قدر.عدد = قدر;
    subject->prev_قدر.عدد = قدر;
    subject->min_قدر.عدد = INT32_MIN;
    subject->max_قدر.عدد = INT32_MAX;
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

#جے_انت /*LV_USE_FLOAT*/

ساكن فراغ init_اشارہ_گر(lv_subject_t * subject, فراغ * قدر)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_اشارہ_گر;
    subject->قدر.اشارہ_گر = قدر;
    subject->prev_قدر.اشارہ_گر = قدر;
}

ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->قدر.color = color;
    subject->prev_قدر.color = color;
}

ساكن فراغ init_گروہ(lv_subject_t * subject, lv_subject_t ** list, ناپ_t ناپ)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || ناپ == 0);
    subject->type = LV_SUBJECT_TYPE_گروہ;
    جوڑو_observers_منزل_گروہ_subject(subject, list, ناپ);
}

ساكن فراغ جوڑو_observers_منزل_گروہ_subject(lv_subject_t * subject, lv_subject_t ** list, ناپ_t ناپ)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || ناپ == 0);
    subject->قدر.اشارہ_گر = list;
    subject->ناپ = (uint32_t)ناپ;

    لكل(uint32_t i = 0; i < ناپ; i++) {
        lv_subject_جوڑو_observer(list[i], گروہ_notify_cb, subject);
    }
}

ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, ناپ_t ناپ, ثابت حرف * قدر)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(buf != عدم || ناپ == 0);

    إذا(ناپ > 0) {
        lv_strlcpy(buf, قدر, ناپ);
        إذا(prev_buf) lv_strlcpy(prev_buf, قدر, ناپ);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->ناپ = (uint32_t)ناپ;
    subject->قدر.اشارہ_گر = buf;
    subject->prev_قدر.اشارہ_گر = prev_buf;
}

ساكن فراغ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    إذا(subject->type == LV_SUBJECT_TYPE_گروہ) {
        remove_observers_from_گروہ_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_سر(&subject->subs_ll);
    طالما(observer) {
        lv_observer_t * observer_اگلا = lv_ll_get_اگلا(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_اگلا;
    }

#إذا LV_USE_EXT_انکڑے
    إذا(subject->ext_انکڑے.free_cb) {
        subject->ext_انکڑے.free_cb(subject->ext_انکڑے.انکڑے);
        subject->ext_انکڑے.انکڑے = عدم;
    }
#جے_انت

    lv_ll_clear(&subject->subs_ll);
}

ساكن فراغ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_t * subject = lv_event_get_user_انکڑے(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_سیٹ_int(subject, v);
}

ساكن فراغ subject_سیٹ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سیٹ_int_user_انکڑے_t * user_انکڑے = lv_event_get_user_انکڑے(e);
    lv_subject_سیٹ_int(user_انکڑے->subject, user_انکڑے->قدر);
}


#إذا LV_USE_FLOAT
ساكن فراغ subject_سیٹ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سیٹ_float_user_انکڑے_t * user_انکڑے = lv_event_get_user_انکڑے(e);
    lv_subject_سیٹ_float(user_انکڑے->subject, user_انکڑے->قدر);
}
#جے_انت

ساكن فراغ subject_سیٹ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سیٹ_string_user_انکڑے_t * user_انکڑے = lv_event_get_user_انکڑے(e);
    lv_subject_سیٹ_string(user_انکڑے->subject, user_انکڑے->قدر);
}

ساكن فراغ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_increment_dsc_t * user_انکڑے = lv_event_get_user_انکڑے(e);

    إذا(user_انکڑے->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_قدر = LV_MIN(user_انکڑے->max_قدر, user_انکڑے->subject->max_قدر.عدد);
        int32_t min_قدر = LV_MAX(user_انکڑے->min_قدر, user_انکڑے->subject->min_قدر.عدد);

        int32_t قدر = lv_subject_get_int(user_انکڑے->subject);
        قدر += user_انکڑے->قدم;

        إذا(user_انکڑے->rollover) {
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

        lv_subject_سیٹ_int(user_انکڑے->subject, قدر);
    }
#إذا LV_USE_FLOAT
    وإلا إذا(user_انکڑے->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        عائم max_قدر = LV_MIN((عائم)user_انکڑے->max_قدر, user_انکڑے->subject->max_قدر.float_v);
        عائم min_قدر = LV_MAX((عائم)user_انکڑے->min_قدر, user_انکڑے->subject->min_قدر.float_v);


        عائم قدر = lv_subject_get_float(user_انکڑے->subject);
        قدر += (عائم)user_انکڑے->قدم;

        إذا(user_انکڑے->rollover) {
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

        lv_subject_سیٹ_float(user_انکڑے->subject, قدر);
    }
#جے_انت
}

ساكن فراغ گروہ_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_بن_ورتیا(subject);
    lv_subject_t * subject_گروہ = observer->user_انکڑے;
    lv_subject_notify(subject_گروہ);
}

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_observer_t * observer = lv_event_get_user_انکڑے(e);
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

    lv_observer_t * observable = lv_subject_جوڑو_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_انکڑے = 1;

    أرجع observable;
}

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_ASSERT(subject != عدم);
    bound_state_t * p = observer->user_انکڑے;
    LV_ASSERT(p != عدم);

    إذا(subject->قدر.عدد) {
        lv_obj_جوڑو_state(observer->نشانہ, p->state);
    }
    وإلا {
        lv_obj_remove_state(observer->نشانہ, p->state);
    }
}

ساكن فراغ obj_قدر_بدلیا_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_current_نشانہ(e);
    lv_subject_t * subject = lv_event_get_user_انکڑے(e);
    LV_ASSERT(subject != عدم);
    LV_ASSERT(obj != عدم);

    lv_subject_سیٹ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ساكن فراغ lv_subject_notify_if_بدلیا(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    بدل(subject->type) {
        حالة LV_SUBJECT_TYPE_INT :
            إذا(subject->قدر.عدد != subject->prev_قدر.عدد) {
                lv_subject_notify(subject);
            }
            اكسر;
#إذا LV_USE_FLOAT
        حالة LV_SUBJECT_TYPE_FLOAT :
            إذا(subject->قدر.float_v != subject->prev_قدر.float_v) {
                lv_subject_notify(subject);
            }
            اكسر;
#جے_انت
        حالة LV_SUBJECT_TYPE_گروہ :
        حالة LV_SUBJECT_TYPE_اشارہ_گر :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR  :
            إذا(!lv_color_eq(subject->قدر.color, subject->prev_قدر.color)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_STRING:
            إذا(!subject->prev_قدر.اشارہ_گر ||
               lv_strcmp(subject->قدر.اشارہ_گر, subject->prev_قدر.اشارہ_گر)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_INVALID :
        حالة LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ساكن فراغ subject_سیٹ_string_free_user_انکڑے_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سیٹ_string_user_انکڑے_t * user_انکڑے = lv_event_get_user_انکڑے(e);
    LV_ASSERT(user_انکڑے != عدم);
    lv_free((فراغ *)user_انکڑے->قدر);
    lv_free(user_انکڑے);
}

ساكن فراغ سیٹ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_bool_t سیٹ_bool_cb = (lv_obj_سیٹ_bool_t)observer->user_cb;
    إذا(سیٹ_bool_cb) سیٹ_bool_cb(obj, subject->قدر.عدد);
}

ساكن فراغ سیٹ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_int_t سیٹ_int_cb = (lv_obj_سیٹ_int_t)observer->user_cb;
    إذا(سیٹ_int_cb) سیٹ_int_cb(obj, subject->قدر.عدد);
}

#إذا LV_USE_FLOAT
ساكن فراغ سیٹ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_float_t سیٹ_float_cb = (lv_obj_سیٹ_float_t)observer->user_cb;
    إذا(سیٹ_float_cb) سیٹ_float_cb(obj, subject->قدر.float_v);
}
#جے_انت /*LV_USE_FLOAT*/

ساكن فراغ سیٹ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_string_t سیٹ_string_cb = (lv_obj_سیٹ_string_t)observer->user_cb;
    إذا(سیٹ_string_cb) سیٹ_string_cb(obj, subject->قدر.اشارہ_گر);
}


ساكن فراغ سیٹ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_color_t سیٹ_color_cb = (lv_obj_سیٹ_color_t)observer->user_cb;
    إذا(سیٹ_color_cb) سیٹ_color_cb(obj, subject->قدر.color);
}

ساكن فراغ سیٹ_اشارہ_گر_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->نشانہ;
    lv_obj_سیٹ_اشارہ_گر_t سیٹ_اشارہ_گر_cb = (lv_obj_سیٹ_اشارہ_گر_t)observer->user_cb;
    إذا(سیٹ_اشارہ_گر_cb) سیٹ_اشارہ_گر_cb(obj, subject->قدر.اشارہ_گر);
}

ساكن فراغ remove_observers_from_گروہ_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_گروہ);

    لكل(uint32_t i = 0; i < subject->ناپ; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->قدر.اشارہ_گر))[i];
        subject_remove_observer(sub, گروہ_notify_cb, subject);
    }
}

ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_انکڑے)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(cb != عدم);

    lv_observer_t * obs = lv_ll_get_سر(&subject->subs_ll);
    طالما(obs) {
        lv_observer_t * obs_اگلا = lv_ll_get_اگلا(&subject->subs_ll, obs);
        إذا(obs->cb == cb && obs->user_انکڑے == user_انکڑے) {
            lv_observer_delete(obs);
        }
        obs = obs_اگلا;
    }
}

#جے_انت /*LV_USE_OBSERVER*/
