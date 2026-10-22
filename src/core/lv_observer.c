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


#تعریف_کن subject_list LV_GLOBAL_DEFAULT()->subject_ll

#اگر_تعریف_نشده FLT_MAX
    #تعریف_کن FLT_MAX 3.402823466e+38F
#پایان_اگر

/**********************
 *      TYPEDEFS
 **********************/
تعريفنوع بنية {
    lv_state_t state;
} bound_state_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    int32_t مقدار;
} subject_مجموعه_int_user_داده_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    عائم مقدار;
} subject_مجموعه_float_user_داده_t;

تعريفنوع بنية {
    lv_subject_t * subject;
    ثابت حرف * مقدار;
} subject_مجموعه_string_user_داده_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن فراغ remove_observers_from_گروه_subject(lv_subject_t * subject);
ساكن فراغ جمع_کن_observers_مقصد_گروه_subject(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه);
ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_داده);
ساكن فراغ subject_toggle_cb(lv_event_t * e);
ساكن فراغ subject_مجموعه_int_cb(lv_event_t * e);
#إذا LV_USE_FLOAT
    ساكن فراغ subject_مجموعه_float_cb(lv_event_t * e);
#پایان_اگر

ساكن فراغ subject_مجموعه_string_cb(lv_event_t * e);
ساكن فراغ subject_increment_cb(lv_event_t * e);

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e);
ساكن فراغ گروه_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ obj_مقدار_تغییریافته_event_cb(lv_event_t * e);

ساكن فراغ lv_subject_notify_if_تغییریافته(lv_subject_t * subject);

ساكن فراغ subject_مجموعه_string_free_user_داده_event_cb(lv_event_t * e);

ساكن فراغ مجموعه_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ مجموعه_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ مجموعه_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ مجموعه_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ساكن فراغ مجموعه_اشاره_گر_observer(lv_observer_t * observer, lv_subject_t * subject);

#إذا LV_USE_FLOAT
    ساكن فراغ مجموعه_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ساكن فراغ init_float(lv_subject_t * subject, عائم مقدار);
#پایان_اگر /*LV_USE_FLOAT*/

ساكن فراغ init_int(lv_subject_t * subject, int32_t مقدار);
ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه, ثابت حرف * مقدار);
ساكن فراغ init_اشاره_گر(lv_subject_t * subject, فراغ * مقدار);
ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color);
ساكن فراغ init_گروه(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه);
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
        إذا(curr->type == LV_SUBJECT_TYPE_گروه) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        إذا(curr->type != LV_SUBJECT_TYPE_گروه) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, أرجع عدم);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, أرجع عدم);
    lv_subject_t * subject = lv_ll_ins_دم(&subject_list);
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
#پایان_اگر /*LV_USE_FLOAT*/
            اكسر;
        حالة LV_SUBJECT_TYPE_اشاره_گر:
            init_اشاره_گر(subject, عدم);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            اكسر;
        حالة LV_SUBJECT_TYPE_گروه:
            init_گروه(subject, عدم, 0);
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

#إذا LV_USE_EXT_داده
فراغ lv_subject_مجموعه_external_داده(lv_subject_t * subject, فراغ * داده, فراغ (* free_cb)(فراغ * داده))
{
    LV_CHECK_ARG(subject != عدم, أرجع);

    subject->ext_داده.داده = داده;
    subject->ext_داده.free_cb = free_cb;
}
#پایان_اگر

فراغ lv_subject_مجموعه_int(lv_subject_t * subject, int32_t مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    مقدار = LV_CLAMP(subject->min_مقدار.رقم, مقدار, subject->max_مقدار.رقم);

    subject->prev_مقدار.رقم = subject->مقدار.رقم;
    subject->مقدار.رقم = مقدار;
    lv_subject_notify_if_تغییریافته(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->مقدار.رقم;
}

int32_t lv_subject_get_قبلی_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع 0);

    أرجع subject->prev_مقدار.رقم;
}

فراغ lv_subject_مجموعه_min_مقدار_int(lv_subject_t * subject, int32_t min_مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->min_مقدار.رقم = min_مقدار;
}

فراغ lv_subject_مجموعه_max_مقدار_int(lv_subject_t * subject, int32_t max_مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject->max_مقدار.رقم = max_مقدار;
}

#إذا LV_USE_FLOAT

فراغ lv_subject_مجموعه_float(lv_subject_t * subject, عائم مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    مقدار = LV_CLAMP(subject->min_مقدار.float_v, مقدار, subject->max_مقدار.float_v);

    subject->prev_مقدار.float_v = subject->مقدار.float_v;
    subject->مقدار.float_v = مقدار;
    lv_subject_notify_if_تغییریافته(subject);
}

عائم lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->مقدار.float_v;
}

عائم lv_subject_get_قبلی_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع 0.0);

    أرجع subject->prev_مقدار.float_v;
}

فراغ lv_subject_مجموعه_min_مقدار_float(lv_subject_t * subject, عائم min_مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->min_مقدار.float_v = min_مقدار;
}

فراغ lv_subject_مجموعه_max_مقدار_float(lv_subject_t * subject, عائم max_مقدار)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject->max_مقدار.float_v = max_مقدار;
}


#پایان_اگر /*LV_USE_FLOAT*/

فراغ lv_subject_مجموعه_string_میانگیر_static(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);
    LV_CHECK_ARG(اندازه > 0, أرجع);

    buf[0] = '\0';
    إذا(prev_buf) prev_buf[0] = '\0';

    subject->اندازه = (uint32_t)اندازه;
    subject->مقدار.اشاره_گر = buf;
    subject->prev_مقدار.اشاره_گر = prev_buf;
}

فراغ lv_subject_مجموعه_string(lv_subject_t * subject, ثابت حرف * buf)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(buf != عدم, أرجع);

    إذا(subject->اندازه == 0) أرجع;
    إذا(subject->prev_مقدار.اشاره_گر) {
        lv_strlcpy((حرف *)subject->prev_مقدار.اشاره_گر, subject->مقدار.اشاره_گر, subject->اندازه);
    }

    lv_strlcpy((حرف *)subject->مقدار.اشاره_گر, buf, subject->اندازه);

    lv_subject_notify_if_تغییریافته(subject);
}

فراغ lv_subject_snprintf(lv_subject_t * subject, ثابت حرف * format, ...)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(format != عدم, أرجع);

    إذا(subject->اندازه < 1U) أرجع;

    إذا(subject->prev_مقدار.اشاره_گر) {
        lv_strlcpy((حرف *)subject->prev_مقدار.اشاره_گر, subject->مقدار.اشاره_گر, subject->اندازه);
    }

    va_list va;
    va_شروع(va, format);
    ثابت صحيح ret = lv_vsnprintf((حرف *)subject->مقدار.اشاره_گر, subject->اندازه, format, va);
    LV_استفادهنشده(ret);
    va_انتها(va);

    lv_subject_notify_if_تغییریافته(subject);
}

ثابت حرف * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->مقدار.اشاره_گر;
}

ثابت حرف * lv_subject_get_قبلی_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    أرجع subject->prev_مقدار.اشاره_گر;
}

فراغ lv_subject_مجموعه_اشاره_گر(lv_subject_t * subject, فراغ * ptr)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_گر, أرجع);

    subject->prev_مقدار.اشاره_گر = subject->مقدار.اشاره_گر;
    subject->مقدار.اشاره_گر = ptr;
    lv_subject_notify_if_تغییریافته(subject);
}

ثابت فراغ * lv_subject_get_اشاره_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_گر, أرجع عدم);


    أرجع subject->مقدار.اشاره_گر;
}

ثابت فراغ * lv_subject_get_قبلی_اشاره_گر(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_گر, أرجع عدم);

    أرجع subject->prev_مقدار.اشاره_گر;
}

فراغ lv_subject_مجموعه_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع);

    subject->prev_مقدار.color = subject->مقدار.color;
    subject->مقدار.color = color;
    lv_subject_notify_if_تغییریافته(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->مقدار.color;
}

lv_color_t lv_subject_get_قبلی_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != عدم, أرجع lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع lv_color_black());

    أرجع subject->prev_مقدار.color;
}

فراغ lv_subject_مجموعه_گروه_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(list != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروه, أرجع);

    لكل(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_پیام(list[i] != عدم, أرجع, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_گروه_subject(subject);
    جمع_کن_observers_مقصد_گروه_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_گروه_element(lv_subject_t * subject, int32_t شاخص)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_گروه, أرجع عدم);
    LV_CHECK_ARG(شاخص >= 0 && شاخص < subject->اندازه, أرجع عدم);

    أرجع ((lv_subject_t **)(subject->مقدار.اشاره_گر))[شاخص];
}

lv_observer_t * lv_subject_جمع_کن_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * user_داده)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);

    lv_observer_t * observer = lv_subject_جمع_کن_observer_obj(subject, observer_cb, عدم, user_داده);
    إذا(observer == عدم) أرجع عدم;

    observer->for_obj = 0;
    أرجع observer;
}

lv_observer_t * lv_subject_جمع_کن_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            فراغ * user_داده)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_دم(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_داده = user_داده;
    observer->هدف = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    إذا(obj != عدم) {
        lv_obj_جمع_کن_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}

lv_observer_t * lv_subject_جمع_کن_observer_with_هدف(lv_subject_t * subject, lv_observer_cb_t observer_cb, فراغ * هدف,
                                                    فراغ * user_داده)
{
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(observer_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, أرجع عدم);

    lv_observer_t * observer = lv_ll_ins_دم(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    إذا(observer == عدم) أرجع عدم;

    lv_memzero(observer, حجم(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_داده = user_داده;
    observer->هدف = هدف;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    أرجع observer;
}


فراغ lv_observer_delete(lv_observer_t * observer)
{
    إذا(observer == عدم) أرجع;

    إذا(observer->for_obj && observer->هدف) {
        lv_obj_remove_event_cb_with_user_داده(observer->هدف, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_داده(observer->هدف, عدم, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    إذا(observer->auto_free_user_داده) {
        lv_free(observer->user_داده);
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
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_شمار(&obj->spec_attr->event_list) : 0);
    لكل(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        إذا(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_داده;
            إذا(subject == عدم || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_داده(obj, عدم, subject);

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

lv_subject_increment_dsc_t * lv_obj_جمع_کن_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_کد_t trigger, int32_t گام)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_subject_increment_dsc_t * user_داده = lv_malloc(حجم(lv_subject_increment_dsc_t));
    إذا(user_داده == عدم) {
        LV_ASSERT_MALLOC(user_داده);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        أرجع عدم;
    }

    user_داده->گام = گام;
    user_داده->subject = subject;
    user_داده->rollover = false;
    user_داده->min_مقدار = INT32_MIN;
    user_داده->max_مقدار = INT32_MAX;
    lv_obj_جمع_کن_event_cb(obj, subject_increment_cb, trigger, user_داده);
    lv_obj_جمع_کن_event_cb(obj, lv_event_free_user_داده_cb, LV_EVENT_DELETE, user_داده);

    أرجع user_داده;
}

فراغ lv_obj_مجموعه_subject_increment_event_min_مقدار(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_مقدار)
{
    LV_استفادهنشده(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->min_مقدار = min_مقدار;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->مقدار.رقم < min_مقدار) {
            lv_subject_مجموعه_int(dsc->subject, min_مقدار);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->مقدار.float_v < (عائم)min_مقدار) {
            lv_subject_مجموعه_float(dsc->subject, (عائم)min_مقدار);
        }
    }
#پایان_اگر
}

فراغ lv_obj_مجموعه_subject_increment_event_max_مقدار(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_مقدار)
{
    LV_استفادهنشده(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->max_مقدار = max_مقدار;
    إذا(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        إذا(dsc->subject->مقدار.رقم > max_مقدار) {
            lv_subject_مجموعه_int(dsc->subject, max_مقدار);
        }
    }
#إذا LV_USE_FLOAT
    وإلا إذا(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        إذا(dsc->subject->مقدار.float_v > (عائم)max_مقدار) {
            lv_subject_مجموعه_float(dsc->subject, (عائم)max_مقدار);
        }
    }
#پایان_اگر
}

فراغ lv_obj_مجموعه_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_استفادهنشده(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    LV_CHECK_ARG(dsc != عدم, أرجع);

    dsc->rollover = rollover;
}

فراغ lv_obj_جمع_کن_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کد_t trigger)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    lv_obj_جمع_کن_event_cb(obj, subject_toggle_cb, trigger, subject);
}

فراغ lv_obj_جمع_کن_subject_مجموعه_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کد_t trigger, int32_t مقدار)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع);

    subject_مجموعه_int_user_داده_t * user_داده = lv_malloc(حجم(subject_مجموعه_int_user_داده_t));
    إذا(user_داده == عدم) {
        LV_ASSERT_MALLOC(user_داده);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_داده->subject = subject;
    user_داده->مقدار = مقدار;

    lv_obj_جمع_کن_event_cb(obj, subject_مجموعه_int_cb, trigger, user_داده);
    lv_obj_جمع_کن_event_cb(obj, lv_event_free_user_داده_cb, LV_EVENT_DELETE, user_داده);
}

#إذا LV_USE_FLOAT
فراغ lv_obj_جمع_کن_subject_مجموعه_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کد_t trigger, عائم مقدار)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع);

    subject_مجموعه_float_user_داده_t * user_داده = lv_malloc(حجم(subject_مجموعه_float_user_داده_t));
    إذا(user_داده == عدم) {
        LV_ASSERT_MALLOC(user_داده);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_داده->subject = subject;
    user_داده->مقدار = مقدار;

    lv_obj_جمع_کن_event_cb(obj, subject_مجموعه_float_cb, trigger, user_داده);
    lv_obj_جمع_کن_event_cb(obj, lv_event_free_user_داده_cb, LV_EVENT_DELETE, user_داده);
}
#پایان_اگر /*LV_USE_FLOAT*/

فراغ lv_obj_جمع_کن_subject_مجموعه_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_کد_t trigger,
                                         ثابت حرف * مقدار)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(subject != عدم, أرجع);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع);
    LV_CHECK_ARG(مقدار != عدم, أرجع);

    subject_مجموعه_string_user_داده_t * user_داده = lv_malloc(حجم(subject_مجموعه_string_user_داده_t));
    إذا(user_داده == عدم) {
        LV_ASSERT_MALLOC(user_داده);
        LV_LOG_WARN("Couldn't allocate user_data");
        أرجع;
    }

    user_داده->subject = subject;
    user_داده->مقدار = lv_strdup(مقدار);
    إذا(user_داده->مقدار == عدم) {
        LV_ASSERT_MALLOC(user_داده->مقدار);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_داده);
        أرجع;
    }

    lv_obj_جمع_کن_event_cb(obj, subject_مجموعه_string_cb, trigger, user_داده);
    lv_obj_جمع_کن_event_cb(obj, subject_مجموعه_string_free_user_داده_event_cb, LV_EVENT_DELETE, user_داده);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_bool_t مجموعه_bool_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_bool_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_bool_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_bool_cb;
    مجموعه_bool_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_int_t مجموعه_int_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_int_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم);
    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_int_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_int_cb;
    مجموعه_int_observer(observable, subject);
    أرجع observable;
}

#إذا LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_float_t مجموعه_float_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_float_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_float_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_float_cb;
    مجموعه_float_observer(observable, subject);
    أرجع observable;
}
#پایان_اگر

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_string_t مجموعه_string_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_string_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_string_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_string_cb;
    مجموعه_string_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_color_t مجموعه_color_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_color_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_color_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_color_cb;
    مجموعه_color_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_اشاره_گر(lv_obj_t * obj, lv_subject_t * subject, lv_obj_مجموعه_اشاره_گر_t مجموعه_اشاره_گر_cb)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG(مجموعه_اشاره_گر_cb != عدم, أرجع عدم);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_اشاره_گر, أرجع عدم);

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, مجموعه_اشاره_گر_observer, obj, عدم);
    إذا(observable == عدم) {
        LV_LOG_WARN("Couldn't add observer to subject");
        أرجع عدم;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (فراغ (*)(فراغ))مجموعه_اشاره_گر_cb;
    مجموعه_اشاره_گر_observer(observable, subject);
    أرجع observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);
    LV_CHECK_ARG_FORMAT_پیام(subject->type == LV_SUBJECT_TYPE_INT, أرجع عدم, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_جمع_کن_event_cb(obj, obj_مقدار_تغییریافته_event_cb, LV_EVENT_مقدار_تغییریافته, subject);

    أرجع observable;
}


lv_obj_t * lv_observer_get_هدف_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع (lv_obj_t *)lv_observer_get_هدف(observer);
}

فراغ * lv_observer_get_user_داده(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != عدم, أرجع عدم);

    أرجع observer->user_داده;
}

فراغ lv_observer_مجموعه_user_داده(lv_observer_t * observer, فراغ * user_داده)
{
    LV_CHECK_ARG(observer != عدم, أرجع);

    /* Release internally-owned data only when replacing it. */
    إذا(observer->auto_free_user_داده && observer->user_داده != user_داده) {
        lv_free(observer->user_داده);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_داده = user_داده;
    observer->auto_free_user_داده = 0;
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
ساكن فراغ init_int(lv_subject_t * subject, int32_t مقدار)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->مقدار.رقم = مقدار;
    subject->prev_مقدار.رقم = مقدار;
    subject->min_مقدار.رقم = INT32_MIN;
    subject->max_مقدار.رقم = INT32_MAX;
}

#إذا LV_USE_FLOAT

ساكن فراغ init_float(lv_subject_t * subject, عائم مقدار)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->مقدار.float_v = مقدار;
    subject->prev_مقدار.float_v = مقدار;
    subject->min_مقدار.float_v = -FLT_MAX;
    subject->max_مقدار.float_v = FLT_MAX;
}

#پایان_اگر /*LV_USE_FLOAT*/

ساكن فراغ init_اشاره_گر(lv_subject_t * subject, فراغ * مقدار)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_اشاره_گر;
    subject->مقدار.اشاره_گر = مقدار;
    subject->prev_مقدار.اشاره_گر = مقدار;
}

ساكن فراغ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != عدم);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->مقدار.color = color;
    subject->prev_مقدار.color = color;
}

ساكن فراغ init_گروه(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || اندازه == 0);
    subject->type = LV_SUBJECT_TYPE_گروه;
    جمع_کن_observers_مقصد_گروه_subject(subject, list, اندازه);
}

ساكن فراغ جمع_کن_observers_مقصد_گروه_subject(lv_subject_t * subject, lv_subject_t ** list, اندازه_t اندازه)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(list != عدم || اندازه == 0);
    subject->مقدار.اشاره_گر = list;
    subject->اندازه = (uint32_t)اندازه;

    لكل(uint32_t i = 0; i < اندازه; i++) {
        lv_subject_جمع_کن_observer(list[i], گروه_notify_cb, subject);
    }
}

ساكن فراغ init_string(lv_subject_t * subject, حرف * buf, حرف * prev_buf, اندازه_t اندازه, ثابت حرف * مقدار)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(buf != عدم || اندازه == 0);

    إذا(اندازه > 0) {
        lv_strlcpy(buf, مقدار, اندازه);
        إذا(prev_buf) lv_strlcpy(prev_buf, مقدار, اندازه);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->اندازه = (uint32_t)اندازه;
    subject->مقدار.اشاره_گر = buf;
    subject->prev_مقدار.اشاره_گر = prev_buf;
}

ساكن فراغ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    إذا(subject->type == LV_SUBJECT_TYPE_گروه) {
        remove_observers_from_گروه_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_سر(&subject->subs_ll);
    طالما(observer) {
        lv_observer_t * observer_بعدی = lv_ll_get_بعدی(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_بعدی;
    }

#إذا LV_USE_EXT_داده
    إذا(subject->ext_داده.free_cb) {
        subject->ext_داده.free_cb(subject->ext_داده.داده);
        subject->ext_داده.داده = عدم;
    }
#پایان_اگر

    lv_ll_clear(&subject->subs_ll);
}

ساكن فراغ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_t * subject = lv_event_get_user_داده(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_مجموعه_int(subject, v);
}

ساكن فراغ subject_مجموعه_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_مجموعه_int_user_داده_t * user_داده = lv_event_get_user_داده(e);
    lv_subject_مجموعه_int(user_داده->subject, user_داده->مقدار);
}


#إذا LV_USE_FLOAT
ساكن فراغ subject_مجموعه_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_مجموعه_float_user_داده_t * user_داده = lv_event_get_user_داده(e);
    lv_subject_مجموعه_float(user_داده->subject, user_داده->مقدار);
}
#پایان_اگر

ساكن فراغ subject_مجموعه_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_مجموعه_string_user_داده_t * user_داده = lv_event_get_user_داده(e);
    lv_subject_مجموعه_string(user_داده->subject, user_داده->مقدار);
}

ساكن فراغ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_subject_increment_dsc_t * user_داده = lv_event_get_user_داده(e);

    إذا(user_داده->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_مقدار = LV_MIN(user_داده->max_مقدار, user_داده->subject->max_مقدار.رقم);
        int32_t min_مقدار = LV_MAX(user_داده->min_مقدار, user_داده->subject->min_مقدار.رقم);

        int32_t مقدار = lv_subject_get_int(user_داده->subject);
        مقدار += user_داده->گام;

        إذا(user_داده->rollover) {
            إذا(مقدار > max_مقدار) {
                مقدار = min_مقدار;
            }
            وإلا إذا(مقدار < min_مقدار) {
                مقدار = max_مقدار;
            }
        }
        وإلا {
            مقدار = LV_CLAMP(min_مقدار, مقدار, max_مقدار);
        }

        lv_subject_مجموعه_int(user_داده->subject, مقدار);
    }
#إذا LV_USE_FLOAT
    وإلا إذا(user_داده->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        عائم max_مقدار = LV_MIN((عائم)user_داده->max_مقدار, user_داده->subject->max_مقدار.float_v);
        عائم min_مقدار = LV_MAX((عائم)user_داده->min_مقدار, user_داده->subject->min_مقدار.float_v);


        عائم مقدار = lv_subject_get_float(user_داده->subject);
        مقدار += (عائم)user_داده->گام;

        إذا(user_داده->rollover) {
            إذا(مقدار > max_مقدار) {
                مقدار = min_مقدار;
            }
            وإلا إذا(مقدار < min_مقدار) {
                مقدار = max_مقدار;
            }
        }
        وإلا {
            مقدار = LV_CLAMP(min_مقدار, مقدار, max_مقدار);
        }

        lv_subject_مجموعه_float(user_داده->subject, مقدار);
    }
#پایان_اگر
}

ساكن فراغ گروه_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_استفادهنشده(subject);
    lv_subject_t * subject_گروه = observer->user_داده;
    lv_subject_notify(subject_گروه);
}

ساكن فراغ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_observer_t * observer = lv_event_get_user_داده(e);
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

    lv_observer_t * observable = lv_subject_جمع_کن_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_داده = 1;

    أرجع observable;
}

ساكن فراغ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != عدم);
    LV_ASSERT(subject != عدم);
    bound_state_t * p = observer->user_داده;
    LV_ASSERT(p != عدم);

    إذا(subject->مقدار.رقم) {
        lv_obj_جمع_کن_state(observer->هدف, p->state);
    }
    وإلا {
        lv_obj_remove_state(observer->هدف, p->state);
    }
}

ساكن فراغ obj_مقدار_تغییریافته_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_current_هدف(e);
    lv_subject_t * subject = lv_event_get_user_داده(e);
    LV_ASSERT(subject != عدم);
    LV_ASSERT(obj != عدم);

    lv_subject_مجموعه_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ساكن فراغ lv_subject_notify_if_تغییریافته(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    بدل(subject->type) {
        حالة LV_SUBJECT_TYPE_INT :
            إذا(subject->مقدار.رقم != subject->prev_مقدار.رقم) {
                lv_subject_notify(subject);
            }
            اكسر;
#إذا LV_USE_FLOAT
        حالة LV_SUBJECT_TYPE_FLOAT :
            إذا(subject->مقدار.float_v != subject->prev_مقدار.float_v) {
                lv_subject_notify(subject);
            }
            اكسر;
#پایان_اگر
        حالة LV_SUBJECT_TYPE_گروه :
        حالة LV_SUBJECT_TYPE_اشاره_گر :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            اكسر;
        حالة LV_SUBJECT_TYPE_COLOR  :
            إذا(!lv_color_eq(subject->مقدار.color, subject->prev_مقدار.color)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_STRING:
            إذا(!subject->prev_مقدار.اشاره_گر ||
               lv_strcmp(subject->مقدار.اشاره_گر, subject->prev_مقدار.اشاره_گر)) {
                lv_subject_notify(subject);
            }
            اكسر;
        حالة LV_SUBJECT_TYPE_INVALID :
        حالة LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ساكن فراغ subject_مجموعه_string_free_user_داده_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    subject_مجموعه_string_user_داده_t * user_داده = lv_event_get_user_داده(e);
    LV_ASSERT(user_داده != عدم);
    lv_free((فراغ *)user_داده->مقدار);
    lv_free(user_داده);
}

ساكن فراغ مجموعه_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_bool_t مجموعه_bool_cb = (lv_obj_مجموعه_bool_t)observer->user_cb;
    إذا(مجموعه_bool_cb) مجموعه_bool_cb(obj, subject->مقدار.رقم);
}

ساكن فراغ مجموعه_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_int_t مجموعه_int_cb = (lv_obj_مجموعه_int_t)observer->user_cb;
    إذا(مجموعه_int_cb) مجموعه_int_cb(obj, subject->مقدار.رقم);
}

#إذا LV_USE_FLOAT
ساكن فراغ مجموعه_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_float_t مجموعه_float_cb = (lv_obj_مجموعه_float_t)observer->user_cb;
    إذا(مجموعه_float_cb) مجموعه_float_cb(obj, subject->مقدار.float_v);
}
#پایان_اگر /*LV_USE_FLOAT*/

ساكن فراغ مجموعه_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_string_t مجموعه_string_cb = (lv_obj_مجموعه_string_t)observer->user_cb;
    إذا(مجموعه_string_cb) مجموعه_string_cb(obj, subject->مقدار.اشاره_گر);
}


ساكن فراغ مجموعه_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_color_t مجموعه_color_cb = (lv_obj_مجموعه_color_t)observer->user_cb;
    إذا(مجموعه_color_cb) مجموعه_color_cb(obj, subject->مقدار.color);
}

ساكن فراغ مجموعه_اشاره_گر_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->هدف;
    lv_obj_مجموعه_اشاره_گر_t مجموعه_اشاره_گر_cb = (lv_obj_مجموعه_اشاره_گر_t)observer->user_cb;
    إذا(مجموعه_اشاره_گر_cb) مجموعه_اشاره_گر_cb(obj, subject->مقدار.اشاره_گر);
}

ساكن فراغ remove_observers_from_گروه_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_گروه);

    لكل(uint32_t i = 0; i < subject->اندازه; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->مقدار.اشاره_گر))[i];
        subject_remove_observer(sub, گروه_notify_cb, subject);
    }
}

ساكن فراغ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, فراغ * user_داده)
{
    LV_ASSERT(subject != عدم);
    LV_ASSERT(cb != عدم);

    lv_observer_t * obs = lv_ll_get_سر(&subject->subs_ll);
    طالما(obs) {
        lv_observer_t * obs_بعدی = lv_ll_get_بعدی(&subject->subs_ll, obs);
        إذا(obs->cb == cb && obs->user_داده == user_داده) {
            lv_observer_delete(obs);
        }
        obs = obs_بعدی;
    }
}

#پایان_اگر /*LV_USE_OBSERVER*/
