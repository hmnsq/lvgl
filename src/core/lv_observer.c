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


#تعریف_کړه subject_list LV_GLOBAL_DEFAULT()->subject_ll

#که_نه_تعریف_شوی FLT_MAX
    #تعریف_کړه FLT_MAX 3.402823466e+38F
#د_که_پای

/**********************
 *      TYPEDEFS
 **********************/
تعريفنوع بنية {
    lv_state_t state;
} bound_state_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    int32_t ارزښت;
} subject_سېټ_int_user_معلومات_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    عائم ارزښت;
} subject_سېټ_float_user_معلومات_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    ثابت حرف * ارزښت;
} subject_سېټ_string_user_معلومات_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن فراغ remove_observers_from_ډله_subject(lv_subject_t * subject);
ساكن فراغ جمع_کړه_observers_منزل_ډله_subject(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه);
ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_معلومات);
ساكن فراغ subject_toggle_cb(lv_event_t * e);
ساكن فراغ subject_سېټ_int_cb(lv_event_t * e);
#إذا LV_USE_FLOAT
    ساكن فراغ subject_سېټ_float_cb(lv_event_t * e);
#د_که_پای

ساكن فراغ subject_سېټ_string_cb(lv_event_t * e);
ساكن فراغ subject_increment_cb(lv_event_t * e);

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e);
ساكن فراغ ډله_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ obj_ارزښت_بدل_شوی_event_cb(lv_event_t * e);

ساكن فراغ lv_subject_notify_if_بدل_شوی(lv_subject_t * subject);

ساكن فراغ subject_سېټ_string_free_user_معلومات_event_cb(lv_event_t * e);

ساكن فراغ سېټ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سېټ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سېټ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سېټ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ سېټ_اشاره_ګر_observer(lv_observer_t * observer, lv_subject_t * subject);

#إذا LV_USE_FLOAT
    ساكن فراغ سېټ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ساكن فراغ init_float(lv_subject_t * subject, عائم ارزښت);
#د_که_پای /*LV_USE_FLOAT*/

ساكن فراغ init_int(lv_subject_t * subject, int32_t ارزښت);
ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه, ثابت حرف * ارزښت);
ساكن فراغ init_اشاره_ګر(lv_subject_t * subject, فراغ * ارزښت);
ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color);
ساكن فراغ init_ډله(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه);
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
        إذا(curr->type == LV_SUBJECT_TYPE_ډله) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        إذا(curr->type != LV_SUBJECT_TYPE_ډله) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, أرجع عدم);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, أرجع عدم);
    lv_subject_t * subject = lv_ll_ins_لکۍ(&subject_list);
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
#د_که_پای /*LV_USE_FLOAT*/
            اكسر;
        حالة LV_SUBJECT_TYPE_اشاره_ګر:
            init_اشاره_ګر(subject, عدم);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            اكسر;
        حالة LV_SUBJECT_TYPE_ډله:
            init_ډله(subject, عدم, 0);
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
فراغ lv_subject_سېټ_external_معلومات(lv_subject_t * subject, فراغ * معلومات, فراغ (* free_cb)(فراغ * معلومات))
{
    LV_CHECK_ARG(subject != عدم, أرجع);

    subject->ext_معلومات.معلومات = معلومات;
    subject->ext_معلومات.free_cb = free_cb;
}
#د_که_پای

فراغ lv_subject_سېټ_int(lv_subject_t * subject, int32_t ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    ارزښت = LV_CLAMP(subject->min_ارزښت.عدد, ارزښت, subject->max_ارزښت.عدد);

    subject->prev_ارزښت.عدد = subject->ارزښت.عدد;
    subject->ارزښت.عدد = ارزښت;
    lv_subject_notify_if_بدل_شوی(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->ارزښت.عدد;
}

int32_t lv_subject_get_پخوانی_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->prev_ارزښت.عدد;
}

فراغ lv_subject_سېټ_min_ارزښت_int(lv_subject_t * subject, int32_t min_ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->min_ارزښت.عدد = min_ارزښت;
}

فراغ lv_subject_سېټ_max_ارزښت_int(lv_subject_t * subject, int32_t max_ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->max_ارزښت.عدد = max_ارزښت;
}

#إذا LV_USE_FLOAT

فراغ lv_subject_سېټ_float(lv_subject_t * subject, عائم ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    ارزښت = LV_CLAMP(subject->min_ارزښت.float_v, ارزښت, subject->max_ارزښت.float_v);

    subject->prev_ارزښت.float_v = subject->ارزښت.float_v;
    subject->ارزښت.float_v = ارزښت;
    lv_subject_notify_if_بدل_شوی(subject);
}

عائم lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->ارزښت.float_v;
}

عائم lv_subject_get_پخوانی_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->prev_ارزښت.float_v;
}

فراغ lv_subject_سېټ_min_ارزښت_float(lv_subject_t * subject, عائم min_ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->min_ارزښت.float_v = min_ارزښت;
}

فراغ lv_subject_سېټ_max_ارزښت_float(lv_subject_t * subject, عائم max_ارزښت)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->max_ارزښت.float_v = max_ارزښت;
}


#د_که_پای /*LV_USE_FLOAT*/

فراغ lv_subject_سېټ_string_بفر_static(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);
    LV_CHECK_ARG(اندازه > 0, أرجع);

    buf[0] = '\0';
    إذا(prev_buf) prev_buf[0] = '\0';

    subject->اندازه = (uint32_t)اندازه;
    subject->ارزښت.اشاره_ګر = buf;
    subject->prev_ارزښت.اشاره_ګر = prev_buf;
}

فراغ lv_subject_سېټ_string(lv_subject_t * subject, ثابت حرف * buf)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);

    إذا(subject->اندازه == 0) أرجع;
    إذا(subject->prev_ارزښت.اشاره_ګر) {
        lv_strlcpy((حرف *)subject->prev_ارزښت.اشاره_ګر, subject->ارزښت.اشاره_ګر, subject->اندازه);
    }

    lv_strlcpy((حرف *)subject->ارزښت.اشاره_ګر, buf, subject->اندازه);

    lv_subject_notify_if_بدل_شوی(subject);
}

فراغ lv_subject_snprintf(lv_subject_t * subject, ثابت حرف * format, ...)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(format != عدم, أرجع);

    إذا(subject->اندازه < 1U) أرجع;

    إذا(subject->prev_ارزښت.اشاره_ګر) {
        lv_strlcpy((حرف *)subject->prev_ارزښت.اشاره_ګر, subject->ارزښت.اشاره_ګر, subject->اندازه);
    }

    va_list va;
    va_پیل(va, format);
    ثابت صحيح ret = lv_vsnprintf((حرف *)subject->ارزښت.اشاره_ګر, subject->اندازه, format, va);
    LV_ناکارول_شوی(ret);
    va_پای_ټکی(va);

    lv_subject_notify_if_بدل_شوی(subject);
}

ثابت حرف * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->ارزښت.اشاره_ګر;
}

ثابت حرف * lv_subject_get_پخوانی_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->prev_ارزښت.اشاره_ګر;
}

فراغ lv_subject_سېټ_اشاره_ګر(lv_subject_t * subject, فراغ * ptr)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_ګر, أرجع);

    subject->prev_ارزښت.اشاره_ګر = subject->ارزښت.اشاره_ګر;
    subject->ارزښت.اشاره_ګر = ptr;
    lv_subject_notify_if_بدل_شوی(subject);
}

ثابت فراغ * lv_subject_get_اشاره_ګر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_ګر, أرجع عدم);


    أرجع subject->ارزښت.اشاره_ګر;
}

ثابت فراغ * lv_subject_get_پخوانی_اشاره_ګر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_ګر, أرجع عدم);

    أرجع subject->prev_ارزښت.اشاره_ګر;
}

فراغ lv_subject_سېټ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع);

    subject->prev_ارزښت.color = subject->ارزښت.color;
    subject->ارزښت.color = color;
    lv_subject_notify_if_بدل_شوی(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->ارزښت.color;
}

lv_color_t lv_subject_get_پخوانی_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->prev_ارزښت.color;
}

فراغ lv_subject_سېټ_ډله_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(list != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ډله, أرجع);

    لكل(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_پیغام(list[i] != عدم, أرجع, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_ډله_subject(subject);
    جمع_کړه_observers_منزل_ډله_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_ډله_element(lv_subject_t * subject, int32_t شاخص)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ډله, أرجع عدم);
    LV_CHECK_ARG(شاخص >= 0 && شاخص < subject->اندازه, أرجع عدم);

    أرجع ((lv_subject_t **)(subject->ارزښت.اشاره_ګر))[شاخص];
}

lv_observer_t * lv_subject_جمع_کړه_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);

    lv_observer_t * observer = lv_subject_جمع_کړه_observer_obj(subject, observer_cb, عدم, user_معلومات);
    إذا(observer == عدم) أرجع عدم;

    observer->for_obj = 0;
    أرجع observer;
}

lv_observer_t * lv_subject_جمع_کړه_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_لکۍ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_معلومات = user_معلومات;
    observer->موخه = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    إذا(obj != عدم) {
        lv_obj_جمع_کړه_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}

lv_observer_t * lv_subject_جمع_کړه_observer_with_موخه(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * موخه,
                                                    فراغ * user_معلومات)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_لکۍ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_معلومات = user_معلومات;
    observer->موخه = موخه;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}


فراغ lv_observer_delete(lv_observer_t * observer)
{
    إذا(observer == عدم) أرجع;

    إذا(observer->for_obj && observer->موخه) {
        lv_obj_remove_event_cb_with_user_معلومات(observer->موخه, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_معلومات(observer->موخه, عدم, observer->subject);
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
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_شمېر(&obj->spec_attr->event_list) : 0);
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

فراغ * lv_observer_get_موخه(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->موخه;
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

lv_subject_increment_dsc_t * lv_obj_جمع_کړه_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_کوډ_t trigger, int32_t ګام)
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

    user_معلومات->ګام = ګام;
    user_معلومات->subject = subject;
    user_معلومات->rollover = false;
    user_معلومات->min_ارزښت = INT32_MIN;
    user_معلومات->max_ارزښت = INT32_MAX;
    lv_obj_جمع_کړه_event_cb(obj, subject_increment_cb, trigger, user_معلومات);
    lv_obj_جمع_کړه_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);

    أرجع user_معلومات;
}

فراغ lv_obj_سېټ_subject_increment_event_min_ارزښت(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_ارزښت)
{
    LV_ناکارول_شوی(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->min_ارزښت = min_ارزښت;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->ارزښت.عدد < min_ارزښت) {
            lv_subject_سېټ_int(dsc->subject, min_ارزښت);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->ارزښت.float_v < (عائم)min_ارزښت) {
            lv_subject_سېټ_float(dsc->subject, (عائم)min_ارزښت);
        }
    }
#د_که_پای
}

فراغ lv_obj_سېټ_subject_increment_event_max_ارزښت(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_ارزښت)
{
    LV_ناکارول_شوی(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->max_ارزښت = max_ارزښت;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->ارزښت.عدد > max_ارزښت) {
            lv_subject_سېټ_int(dsc->subject, max_ارزښت);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->ارزښت.float_v > (عائم)max_ارزښت) {
            lv_subject_سېټ_float(dsc->subject, (عائم)max_ارزښت);
        }
    }
#د_که_پای
}

فراغ lv_obj_سېټ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_ناکارول_شوی(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->rollover = rollover;
}

فراغ lv_obj_جمع_کړه_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوډ_t trigger)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    lv_obj_جمع_کړه_event_cb(obj, subject_toggle_cb, trigger, subject);
}

فراغ lv_obj_جمع_کړه_subject_سېټ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوډ_t trigger, int32_t ارزښت)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject_سېټ_int_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سېټ_int_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->ارزښت = ارزښت;

    lv_obj_جمع_کړه_event_cb(obj, subject_سېټ_int_cb, trigger, user_معلومات);
    lv_obj_جمع_کړه_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);
}

#إذا LV_USE_FLOAT
فراغ lv_obj_جمع_کړه_subject_سېټ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوډ_t trigger, عائم ارزښت)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject_سېټ_float_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سېټ_float_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->ارزښت = ارزښت;

    lv_obj_جمع_کړه_event_cb(obj, subject_سېټ_float_cb, trigger, user_معلومات);
    lv_obj_جمع_کړه_event_cb(obj, lv_event_free_user_معلومات_cb, LV_EVENT_DELETE, user_معلومات);
}
#د_که_پای /*LV_USE_FLOAT*/

فراغ lv_obj_جمع_کړه_subject_سېټ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کوډ_t trigger,
                                         ثابت حرف * ارزښت)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(ارزښت != عدم, أرجع);

    subject_سېټ_string_user_معلومات_t * user_معلومات = lv_malloc(حجم(subject_سېټ_string_user_معلومات_t));
    إذا(user_معلومات == عدم) {
        LV_ASSERT_MALLOC(user_معلومات);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_معلومات->subject = subject;
    user_معلومات->ارزښت = lv_strdup(ارزښت);
    إذا(user_معلومات->ارزښت == عدم) {
        LV_ASSERT_MALLOC(user_معلومات->ارزښت);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_معلومات);
        أرجع;
    }

    lv_obj_جمع_کړه_event_cb(obj, subject_سېټ_string_cb, trigger, user_معلومات);
    lv_obj_جمع_کړه_event_cb(obj, subject_سېټ_string_free_user_معلومات_event_cb, LV_EVENT_DELETE, user_معلومات);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_bool_t سېټ_bool_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_bool_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_bool_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_bool_cb;
    سېټ_bool_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_int_t سېټ_int_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_int_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);
    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_int_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_int_cb;
    سېټ_int_observer(observable, subject);
    أرجع observable;
}

#إذا LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_float_t سېټ_float_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_float_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_float_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_float_cb;
    سېټ_float_observer(observable, subject);
    أرجع observable;
}
#د_که_پای

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_string_t سېټ_string_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_string_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_string_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_string_cb;
    سېټ_string_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_color_t سېټ_color_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_color_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_color_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_color_cb;
    سېټ_color_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_اشاره_ګر(lv_obj_t * obj, lv_subject_t * subject, lv_obj_سېټ_اشاره_ګر_t سېټ_اشاره_ګر_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(سېټ_اشاره_ګر_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_ګر, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, سېټ_اشاره_ګر_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))سېټ_اشاره_ګر_cb;
    سېټ_اشاره_ګر_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیغام(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_جمع_کړه_event_cb(obj, obj_ارزښت_بدل_شوی_event_cb, LV_EVENT_ارزښت_بدل_شوی, subject);

    أرجع observable;
}


lv_obj_t * lv_observer_get_موخه_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع (lv_obj_t *)lv_observer_get_موخه(observer);
}

فراغ * lv_observer_get_user_معلومات(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->user_معلومات;
}

فراغ lv_observer_سېټ_user_معلومات(lv_observer_t * observer, فراغ * user_معلومات)
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
ساكن فراغ init_int(lv_subject_t * subject, int32_t ارزښت)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->ارزښت.عدد = ارزښت;
    subject->prev_ارزښت.عدد = ارزښت;
    subject->min_ارزښت.عدد = INT32_MIN;
    subject->max_ارزښت.عدد = INT32_MAX;
}

#إذا LV_USE_FLOAT

ساكن فراغ init_float(lv_subject_t * subject, عائم ارزښت)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->ارزښت.float_v = ارزښت;
    subject->prev_ارزښت.float_v = ارزښت;
    subject->min_ارزښت.float_v = -FLT_MAX;
    subject->max_ارزښت.float_v = FLT_MAX;
}

#د_که_پای /*LV_USE_FLOAT*/

ساكن فراغ init_اشاره_ګر(lv_subject_t * subject, فراغ * ارزښت)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_اشاره_ګر;
    subject->ارزښت.اشاره_ګر = ارزښت;
    subject->prev_ارزښت.اشاره_ګر = ارزښت;
}

ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->ارزښت.color = color;
    subject->prev_ارزښت.color = color;
}

ساكن فراغ init_ډله(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || اندازه == 0);
    subject->type = LV_SUBJECT_TYPE_ډله;
    جمع_کړه_observers_منزل_ډله_subject(subject, list, اندازه);
}

ساكن فراغ جمع_کړه_observers_منزل_ډله_subject(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || اندازه == 0);
    subject->ارزښت.اشاره_ګر = list;
    subject->اندازه = (uint32_t)اندازه;

    لكل(uint32_t i = 0; i < اندازه; i++) {
        lv_subject_جمع_کړه_observer(list[i], ډله_notify_cb, subject);
    }
}

ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه, ثابت حرف * ارزښت)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(buf != عدم || اندازه == 0);

    إذا(اندازه > 0) {
        lv_strlcpy(buf, ارزښت, اندازه);
        إذا(prev_buf) lv_strlcpy(prev_buf, ارزښت, اندازه);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->اندازه = (uint32_t)اندازه;
    subject->ارزښت.اشاره_ګر = buf;
    subject->prev_ارزښت.اشاره_ګر = prev_buf;
}

ساكن فراغ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    إذا(subject->type == LV_SUBJECT_TYPE_ډله) {
        remove_observers_from_ډله_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_سر(&subject->subs_ll);
    طالما(observer) {
        lv_observer_t * observer_بل = lv_ll_get_بل(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_بل;
    }

#إذا LV_USE_EXT_معلومات
    إذا(subject->ext_معلومات.free_cb) {
        subject->ext_معلومات.free_cb(subject->ext_معلومات.معلومات);
        subject->ext_معلومات.معلومات = عدم;
    }
#د_که_پای

    lv_ll_clear(&subject->subs_ll);
}

ساكن فراغ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_t * subject = lv_event_get_user_معلومات(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_سېټ_int(subject, v);
}

ساكن فراغ subject_سېټ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سېټ_int_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سېټ_int(user_معلومات->subject, user_معلومات->ارزښت);
}


#إذا LV_USE_FLOAT
ساكن فراغ subject_سېټ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سېټ_float_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سېټ_float(user_معلومات->subject, user_معلومات->ارزښت);
}
#د_که_پای

ساكن فراغ subject_سېټ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سېټ_string_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    lv_subject_سېټ_string(user_معلومات->subject, user_معلومات->ارزښت);
}

ساكن فراغ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_increment_dsc_t * user_معلومات = lv_event_get_user_معلومات(e);

    إذا(user_معلومات->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_ارزښت = LV_MIN(user_معلومات->max_ارزښت, user_معلومات->subject->max_ارزښت.عدد);
        int32_t min_ارزښت = LV_MAX(user_معلومات->min_ارزښت, user_معلومات->subject->min_ارزښت.عدد);

        int32_t ارزښت = lv_subject_get_int(user_معلومات->subject);
        ارزښت += user_معلومات->ګام;

        إذا(user_معلومات->rollover) {
            إذا(ارزښت > max_ارزښت) {
                ارزښت = min_ارزښت;
            }
            وإلا إذا(ارزښت < min_ارزښت) {
                ارزښت = max_ارزښت;
            }
        }
        وإلا {
            ارزښت = LV_CLAMP(min_ارزښت, ارزښت, max_ارزښت);
        }

        lv_subject_سېټ_int(user_معلومات->subject, ارزښت);
    }
#إذا LV_USE_FLOAT
    وإلا إذا(user_معلومات->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        عائم max_ارزښت = LV_MIN((عائم)user_معلومات->max_ارزښت, user_معلومات->subject->max_ارزښت.float_v);
        عائم min_ارزښت = LV_MAX((عائم)user_معلومات->min_ارزښت, user_معلومات->subject->min_ارزښت.float_v);


        عائم ارزښت = lv_subject_get_float(user_معلومات->subject);
        ارزښت += (عائم)user_معلومات->ګام;

        إذا(user_معلومات->rollover) {
            إذا(ارزښت > max_ارزښت) {
                ارزښت = min_ارزښت;
            }
            وإلا إذا(ارزښت < min_ارزښت) {
                ارزښت = max_ارزښت;
            }
        }
        وإلا {
            ارزښت = LV_CLAMP(min_ارزښت, ارزښت, max_ارزښت);
        }

        lv_subject_سېټ_float(user_معلومات->subject, ارزښت);
    }
#د_که_پای
}

ساكن فراغ ډله_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_ناکارول_شوی(subject);
    lv_subject_t * subject_ډله = observer->user_معلومات;
    lv_subject_notify(subject_ډله);
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

    lv_observer_t * observable = lv_subject_جمع_کړه_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_معلومات = 1;

    أرجع observable;
}

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_ASSERT(subject != عدم);
    bound_state_t * p = observer->user_معلومات;
    LV_ASSERT(p != عدم);

    إذا(subject->ارزښت.عدد) {
        lv_obj_جمع_کړه_state(observer->موخه, p->state);
    }
    وإلا {
        lv_obj_remove_state(observer->موخه, p->state);
    }
}

ساكن فراغ obj_ارزښت_بدل_شوی_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_current_موخه(e);
    lv_subject_t * subject = lv_event_get_user_معلومات(e);
    LV_ASSERT(subject != عدم);
    LV_ASSERT(obj != عدم);

    lv_subject_سېټ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ساكن فراغ lv_subject_notify_if_بدل_شوی(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    بدل(subject->type) {
        حالة LV_SUBJECT_TYPE_INT :
            إذا(subject->ارزښت.عدد != subject->prev_ارزښت.عدد) {
                lv_subject_notify(subject);
            }
            اكسر;
#إذا LV_USE_FLOAT
        حالة LV_SUBJECT_TYPE_FLOAT :
            إذا(subject->ارزښت.float_v != subject->prev_ارزښت.float_v) {
                lv_subject_notify(subject);
            }
            اكسر;
#د_که_پای
        حالة LV_SUBJECT_TYPE_ډله :
        حالة LV_SUBJECT_TYPE_اشاره_ګر :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR  :
            إذا(!lv_color_eq(subject->ارزښت.color, subject->prev_ارزښت.color)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_STRING:
            إذا(!subject->prev_ارزښت.اشاره_ګر ||
               lv_strcmp(subject->ارزښت.اشاره_ګر, subject->prev_ارزښت.اشاره_ګر)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_INVALID :
        حالة LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ساكن فراغ subject_سېټ_string_free_user_معلومات_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_سېټ_string_user_معلومات_t * user_معلومات = lv_event_get_user_معلومات(e);
    LV_ASSERT(user_معلومات != عدم);
    lv_free((فراغ *)user_معلومات->ارزښت);
    lv_free(user_معلومات);
}

ساكن فراغ سېټ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_bool_t سېټ_bool_cb = (lv_obj_سېټ_bool_t)observer->user_cb;
    إذا(سېټ_bool_cb) سېټ_bool_cb(obj, subject->ارزښت.عدد);
}

ساكن فراغ سېټ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_int_t سېټ_int_cb = (lv_obj_سېټ_int_t)observer->user_cb;
    إذا(سېټ_int_cb) سېټ_int_cb(obj, subject->ارزښت.عدد);
}

#إذا LV_USE_FLOAT
ساكن فراغ سېټ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_float_t سېټ_float_cb = (lv_obj_سېټ_float_t)observer->user_cb;
    إذا(سېټ_float_cb) سېټ_float_cb(obj, subject->ارزښت.float_v);
}
#د_که_پای /*LV_USE_FLOAT*/

ساكن فراغ سېټ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_string_t سېټ_string_cb = (lv_obj_سېټ_string_t)observer->user_cb;
    إذا(سېټ_string_cb) سېټ_string_cb(obj, subject->ارزښت.اشاره_ګر);
}


ساكن فراغ سېټ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_color_t سېټ_color_cb = (lv_obj_سېټ_color_t)observer->user_cb;
    إذا(سېټ_color_cb) سېټ_color_cb(obj, subject->ارزښت.color);
}

ساكن فراغ سېټ_اشاره_ګر_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->موخه;
    lv_obj_سېټ_اشاره_ګر_t سېټ_اشاره_ګر_cb = (lv_obj_سېټ_اشاره_ګر_t)observer->user_cb;
    إذا(سېټ_اشاره_ګر_cb) سېټ_اشاره_ګر_cb(obj, subject->ارزښت.اشاره_ګر);
}

ساكن فراغ remove_observers_from_ډله_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_ډله);

    لكل(uint32_t i = 0; i < subject->اندازه; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->ارزښت.اشاره_ګر))[i];
        subject_remove_observer(sub, ډله_notify_cb, subject);
    }
}

ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_معلومات)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(cb != عدم);

    lv_observer_t * obs = lv_ll_get_سر(&subject->subs_ll);
    طالما(obs) {
        lv_observer_t * obs_بل = lv_ll_get_بل(&subject->subs_ll, obs);
        إذا(obs->cb == cb && obs->user_معلومات == user_معلومات) {
            lv_observer_delete(obs);
        }
        obs = obs_بل;
    }
}

#د_که_پای /*LV_USE_OBSERVER*/
