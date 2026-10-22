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


#define subject_list LV_GLOBAL_DEFAULT()->subject_ll

#ifndef FLT_MAX
    #define FLT_MAX 3.402823466e+38F
#endif

/**********************
 *      TYPEDEFS
 **********************/
تعریفنوع ساختار {
    lv_state_t state;
} bound_state_t;

تعریفنوع ساختار {
    lv_subject_t * subject;
    int32_t مقدار;
} subject_set_int_user_data_t;

تعریفنوع ساختار {
    lv_subject_t * subject;
    اعشاری مقدار;
} subject_set_float_user_data_t;

تعریفنوع ساختار {
    lv_subject_t * subject;
    ثابت نویسه * مقدار;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ایستا تهی remove_observers_from_group_subject(lv_subject_t * subject);
ایستا تهی add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ایستا تهی subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, تهی * user_data);
ایستا تهی subject_toggle_cb(lv_event_t * e);
ایستا تهی subject_set_int_cb(lv_event_t * e);
#اگر LV_USE_FLOAT
    ایستا تهی subject_set_float_cb(lv_event_t * e);
#endif

ایستا تهی subject_set_string_cb(lv_event_t * e);
ایستا تهی subject_increment_cb(lv_event_t * e);

ایستا تهی unsubscribe_on_delete_cb(lv_event_t * e);
ایستا تهی group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ایستا lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ایستا تهی obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ایستا تهی obj_مقدار_changed_event_cb(lv_event_t * e);

ایستا تهی lv_subject_notify_if_changed(lv_subject_t * subject);

ایستا تهی subject_set_string_free_user_data_event_cb(lv_event_t * e);

ایستا تهی set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ایستا تهی set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ایستا تهی set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ایستا تهی set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ایستا تهی set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#اگر LV_USE_FLOAT
    ایستا تهی set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ایستا تهی init_float(lv_subject_t * subject, اعشاری مقدار);
#endif /*LV_USE_FLOAT*/

ایستا تهی init_int(lv_subject_t * subject, int32_t مقدار);
ایستا تهی init_string(lv_subject_t * subject, نویسه * buf, نویسه * prev_buf, size_t size, ثابت نویسه * مقدار);
ایستا تهی init_pointer(lv_subject_t * subject, تهی * مقدار);
ایستا تهی init_color(lv_subject_t * subject, lv_color_t color);
ایستا تهی init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ایستا تهی init_common(lv_subject_t * subject);

ایستا تهی deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

تهی lv_subject_global_init(تهی)
{
    lv_ll_init(&subject_list, اندازه(lv_subject_t));
}
تهی lv_subject_global_deinit(تهی)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        اگر(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        اگر(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, بازگشت پوچ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, بازگشت پوچ);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    اگر(!subject) {
        بازگشت پوچ;
    }

    init_common(subject);

    انتخاب(type) {
        حالت LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            بشکن;
        حالت LV_SUBJECT_TYPE_FLOAT:
#اگر LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            بشکن;
        حالت LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, پوچ);
            بشکن;
        حالت LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            بشکن;
        حالت LV_SUBJECT_TYPE_GROUP:
            init_group(subject, پوچ, 0);
            بشکن;
        حالت LV_SUBJECT_TYPE_STRING:
            init_string(subject, پوچ, پوچ, 0, "");
            بشکن;
        حالت LV_SUBJECT_TYPE_INVALID:
        حالت LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    بازگشت subject;
}

تهی lv_subject_delete(lv_subject_t * subject)
{
    اگر(!subject) {
        بازگشت;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#اگر LV_USE_EXT_DATA
تهی lv_subject_set_external_data(lv_subject_t * subject, تهی * data, تهی (* free_cb)(تهی * data))
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

تهی lv_subject_set_int(lv_subject_t * subject, int32_t مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت);

    مقدار = LV_CLAMP(subject->min_مقدار.رقم, مقدار, subject->max_مقدار.رقم);

    subject->prev_مقدار.رقم = subject->مقدار.رقم;
    subject->مقدار.رقم = مقدار;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت 0);

    بازگشت subject->مقدار.رقم;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت 0);

    بازگشت subject->prev_مقدار.رقم;
}

تهی lv_subject_set_min_مقدار_int(lv_subject_t * subject, int32_t min_مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت);

    subject->min_مقدار.رقم = min_مقدار;
}

تهی lv_subject_set_max_مقدار_int(lv_subject_t * subject, int32_t max_مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت);

    subject->max_مقدار.رقم = max_مقدار;
}

#اگر LV_USE_FLOAT

تهی lv_subject_set_float(lv_subject_t * subject, اعشاری مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت);

    مقدار = LV_CLAMP(subject->min_مقدار.float_v, مقدار, subject->max_مقدار.float_v);

    subject->prev_مقدار.float_v = subject->مقدار.float_v;
    subject->مقدار.float_v = مقدار;
    lv_subject_notify_if_changed(subject);
}

اعشاری lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت 0.0);

    بازگشت subject->مقدار.float_v;
}

اعشاری lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت 0.0);

    بازگشت subject->prev_مقدار.float_v;
}

تهی lv_subject_set_min_مقدار_float(lv_subject_t * subject, اعشاری min_مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت);

    subject->min_مقدار.float_v = min_مقدار;
}

تهی lv_subject_set_max_مقدار_float(lv_subject_t * subject, اعشاری max_مقدار)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت);

    subject->max_مقدار.float_v = max_مقدار;
}


#endif /*LV_USE_FLOAT*/

تهی lv_subject_set_string_buffer_static(lv_subject_t * subject, نویسه * buf, نویسه * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت);
    LV_CHECK_ARG(buf != پوچ, بازگشت);
    LV_CHECK_ARG(size > 0, بازگشت);

    buf[0] = '\0';
    اگر(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->مقدار.pointer = buf;
    subject->prev_مقدار.pointer = prev_buf;
}

تهی lv_subject_set_string(lv_subject_t * subject, ثابت نویسه * buf)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت);
    LV_CHECK_ARG(buf != پوچ, بازگشت);

    اگر(subject->size == 0) بازگشت;
    اگر(subject->prev_مقدار.pointer) {
        lv_strlcpy((نویسه *)subject->prev_مقدار.pointer, subject->مقدار.pointer, subject->size);
    }

    lv_strlcpy((نویسه *)subject->مقدار.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

تهی lv_subject_snprintf(lv_subject_t * subject, ثابت نویسه * format, ...)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت);
    LV_CHECK_ARG(format != پوچ, بازگشت);

    اگر(subject->size < 1U) بازگشت;

    اگر(subject->prev_مقدار.pointer) {
        lv_strlcpy((نویسه *)subject->prev_مقدار.pointer, subject->مقدار.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    ثابت صحیح ret = lv_vsnprintf((نویسه *)subject->مقدار.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

ثابت نویسه * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت پوچ);

    بازگشت subject->مقدار.pointer;
}

ثابت نویسه * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت پوچ);

    بازگشت subject->prev_مقدار.pointer;
}

تهی lv_subject_set_pointer(lv_subject_t * subject, تهی * ptr)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, بازگشت);

    subject->prev_مقدار.pointer = subject->مقدار.pointer;
    subject->مقدار.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

ثابت تهی * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, بازگشت پوچ);


    بازگشت subject->مقدار.pointer;
}

ثابت تهی * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, بازگشت پوچ);

    بازگشت subject->prev_مقدار.pointer;
}

تهی lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, بازگشت);

    subject->prev_مقدار.color = subject->مقدار.color;
    subject->مقدار.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, بازگشت lv_color_black());

    بازگشت subject->مقدار.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, بازگشت lv_color_black());

    بازگشت subject->prev_مقدار.color;
}

تهی lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(list != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, بازگشت);

    برای(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_پیام(list[i] != پوچ, بازگشت, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, بازگشت پوچ);
    LV_CHECK_ARG(index >= 0 && index < subject->size, بازگشت پوچ);

    بازگشت ((lv_subject_t **)(subject->مقدار.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, تهی * user_data)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(observer_cb != پوچ, بازگشت پوچ);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, پوچ, user_data);
    اگر(observer == پوچ) بازگشت پوچ;

    observer->for_obj = 0;
    بازگشت observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            تهی * user_data)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(observer_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, بازگشت پوچ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    اگر(observer == پوچ) بازگشت پوچ;

    lv_memzero(observer, اندازه(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    اگر(obj != پوچ) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    بازگشت observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, تهی * target,
                                                    تهی * user_data)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(observer_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, بازگشت پوچ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    اگر(observer == پوچ) بازگشت پوچ;

    lv_memzero(observer, اندازه(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    بازگشت observer;
}


تهی lv_observer_delete(lv_observer_t * observer)
{
    اگر(observer == پوچ) بازگشت;

    اگر(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, پوچ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    اگر(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

تهی lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
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
    برای(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        اگر(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            اگر(subject == پوچ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, پوچ, subject);

}

تهی * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != پوچ, بازگشت پوچ);

    بازگشت observer->target;
}

تهی lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != پوچ, بازگشت);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    انجامبده {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            اگر(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                اگر(subject->notify_restart_query) بشکن;
                observer->notified = 1;
            }
        }
    } وقتی(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت پوچ);

    lv_subject_increment_dsc_t * user_data = lv_malloc(اندازه(lv_subject_increment_dsc_t));
    اگر(user_data == پوچ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        بازگشت پوچ;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_مقدار = INT32_MIN;
    user_data->max_مقدار = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    بازگشت user_data;
}

تهی lv_obj_set_subject_increment_event_min_مقدار(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_مقدار)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, بازگشت);
    LV_CHECK_ARG(dsc != پوچ, بازگشت);

    dsc->min_مقدار = min_مقدار;
    اگر(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        اگر(dsc->subject->مقدار.رقم < min_مقدار) {
            lv_subject_set_int(dsc->subject, min_مقدار);
        }
    }
#اگر LV_USE_FLOAT
    وگرنه اگر(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        اگر(dsc->subject->مقدار.float_v < (اعشاری)min_مقدار) {
            lv_subject_set_float(dsc->subject, (اعشاری)min_مقدار);
        }
    }
#endif
}

تهی lv_obj_set_subject_increment_event_max_مقدار(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_مقدار)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, بازگشت);
    LV_CHECK_ARG(dsc != پوچ, بازگشت);

    dsc->max_مقدار = max_مقدار;
    اگر(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        اگر(dsc->subject->مقدار.رقم > max_مقدار) {
            lv_subject_set_int(dsc->subject, max_مقدار);
        }
    }
#اگر LV_USE_FLOAT
    وگرنه اگر(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        اگر(dsc->subject->مقدار.float_v > (اعشاری)max_مقدار) {
            lv_subject_set_float(dsc->subject, (اعشاری)max_مقدار);
        }
    }
#endif
}

تهی lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, بازگشت);
    LV_CHECK_ARG(dsc != پوچ, بازگشت);

    dsc->rollover = rollover;
}

تهی lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

تهی lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t مقدار)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت);

    subject_set_int_user_data_t * user_data = lv_malloc(اندازه(subject_set_int_user_data_t));
    اگر(user_data == پوچ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        بازگشت;
    }

    user_data->subject = subject;
    user_data->مقدار = مقدار;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#اگر LV_USE_FLOAT
تهی lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, اعشاری مقدار)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت);

    subject_set_float_user_data_t * user_data = lv_malloc(اندازه(subject_set_float_user_data_t));
    اگر(user_data == پوچ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        بازگشت;
    }

    user_data->subject = subject;
    user_data->مقدار = مقدار;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

تهی lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         ثابت نویسه * مقدار)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    LV_CHECK_ARG(subject != پوچ, بازگشت);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت);
    LV_CHECK_ARG(مقدار != پوچ, بازگشت);

    subject_set_string_user_data_t * user_data = lv_malloc(اندازه(subject_set_string_user_data_t));
    اگر(user_data == پوچ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        بازگشت;
    }

    user_data->subject = subject;
    user_data->مقدار = lv_strdup(مقدار);
    اگر(user_data->مقدار == پوچ) {
        LV_ASSERT_MALLOC(user_data->مقدار);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        بازگشت;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_bool_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت پوچ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_bool_cb;
    set_bool_observer(observable, subject);
    بازگشت observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_int_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, بازگشت پوچ);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_int_cb;
    set_int_observer(observable, subject);
    بازگشت observable;
}

#اگر LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_float_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, بازگشت پوچ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_float_cb;
    set_float_observer(observable, subject);
    بازگشت observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_string_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, بازگشت پوچ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_string_cb;
    set_string_observer(observable, subject);
    بازگشت observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_color_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, بازگشت پوچ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_color_cb;
    set_color_observer(observable, subject);
    بازگشت observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(set_pointer_cb != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, بازگشت پوچ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, پوچ);
    اگر(observable == پوچ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        بازگشت پوچ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (تهی (*)(تهی))set_pointer_cb;
    set_pointer_observer(observable, subject);
    بازگشت observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(subject != پوچ, بازگشت پوچ);
    LV_CHECK_ARG_FORMAT_پیام(subject->type == LV_SUBJECT_TYPE_INT, بازگشت پوچ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_مقدار_changed_event_cb, LV_EVENT_مقدار_CHANGED, subject);

    بازگشت observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != پوچ, بازگشت پوچ);

    بازگشت (lv_obj_t *)lv_observer_get_target(observer);
}

تهی * lv_observer_get_user_data(ثابت lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != پوچ, بازگشت پوچ);

    بازگشت observer->user_data;
}

تهی lv_observer_set_user_data(lv_observer_t * observer, تهی * user_data)
{
    LV_CHECK_ARG(observer != پوچ, بازگشت);

    /* Release internally-owned data only when replacing it. */
    اگر(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ایستا تهی init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != پوچ);
    lv_memzero(subject, اندازه(*subject));
    lv_ll_init(&(subject->subs_ll), اندازه(lv_observer_t));
}
ایستا تهی init_int(lv_subject_t * subject, int32_t مقدار)
{
    LV_ASSERT(subject != پوچ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->مقدار.رقم = مقدار;
    subject->prev_مقدار.رقم = مقدار;
    subject->min_مقدار.رقم = INT32_MIN;
    subject->max_مقدار.رقم = INT32_MAX;
}

#اگر LV_USE_FLOAT

ایستا تهی init_float(lv_subject_t * subject, اعشاری مقدار)
{
    LV_ASSERT(subject != پوچ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->مقدار.float_v = مقدار;
    subject->prev_مقدار.float_v = مقدار;
    subject->min_مقدار.float_v = -FLT_MAX;
    subject->max_مقدار.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

ایستا تهی init_pointer(lv_subject_t * subject, تهی * مقدار)
{
    LV_ASSERT(subject != پوچ);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->مقدار.pointer = مقدار;
    subject->prev_مقدار.pointer = مقدار;
}

ایستا تهی init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != پوچ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->مقدار.color = color;
    subject->prev_مقدار.color = color;
}

ایستا تهی init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(list != پوچ || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

ایستا تهی add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(list != پوچ || size == 0);
    subject->مقدار.pointer = list;
    subject->size = (uint32_t)size;

    برای(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

ایستا تهی init_string(lv_subject_t * subject, نویسه * buf, نویسه * prev_buf, size_t size, ثابت نویسه * مقدار)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(buf != پوچ || size == 0);

    اگر(size > 0) {
        lv_strlcpy(buf, مقدار, size);
        اگر(prev_buf) lv_strlcpy(prev_buf, مقدار, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->مقدار.pointer = buf;
    subject->prev_مقدار.pointer = prev_buf;
}

ایستا تهی deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != پوچ);
    اگر(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    وقتی(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#اگر LV_USE_EXT_DATA
    اگر(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = پوچ;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

ایستا تهی subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

ایستا تهی subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->مقدار);
}


#اگر LV_USE_FLOAT
ایستا تهی subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->مقدار);
}
#endif

ایستا تهی subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->مقدار);
}

ایستا تهی subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    اگر(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_مقدار = LV_MIN(user_data->max_مقدار, user_data->subject->max_مقدار.رقم);
        int32_t min_مقدار = LV_MAX(user_data->min_مقدار, user_data->subject->min_مقدار.رقم);

        int32_t مقدار = lv_subject_get_int(user_data->subject);
        مقدار += user_data->step;

        اگر(user_data->rollover) {
            اگر(مقدار > max_مقدار) {
                مقدار = min_مقدار;
            }
            وگرنه اگر(مقدار < min_مقدار) {
                مقدار = max_مقدار;
            }
        }
        وگرنه {
            مقدار = LV_CLAMP(min_مقدار, مقدار, max_مقدار);
        }

        lv_subject_set_int(user_data->subject, مقدار);
    }
#اگر LV_USE_FLOAT
    وگرنه اگر(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        اعشاری max_مقدار = LV_MIN((اعشاری)user_data->max_مقدار, user_data->subject->max_مقدار.float_v);
        اعشاری min_مقدار = LV_MAX((اعشاری)user_data->min_مقدار, user_data->subject->min_مقدار.float_v);


        اعشاری مقدار = lv_subject_get_float(user_data->subject);
        مقدار += (اعشاری)user_data->step;

        اگر(user_data->rollover) {
            اگر(مقدار > max_مقدار) {
                مقدار = min_مقدار;
            }
            وگرنه اگر(مقدار < min_مقدار) {
                مقدار = max_مقدار;
            }
        }
        وگرنه {
            مقدار = LV_CLAMP(min_مقدار, مقدار, max_مقدار);
        }

        lv_subject_set_float(user_data->subject, مقدار);
    }
#endif
}

ایستا تهی group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != پوچ);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

ایستا تهی unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

ایستا lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(obj != پوچ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(اندازه(bound_state_t));
    اگر(p == پوچ) {
        LV_LOG_WARN("Out of memory");
        بازگشت پوچ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    بازگشت observable;
}

ایستا تهی obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != پوچ);
    LV_ASSERT(subject != پوچ);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != پوچ);

    اگر(subject->مقدار.رقم) {
        lv_obj_add_state(observer->target, p->state);
    }
    وگرنه {
        lv_obj_remove_state(observer->target, p->state);
    }
}

ایستا تهی obj_مقدار_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(obj != پوچ);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ایستا تهی lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    انتخاب(subject->type) {
        حالت LV_SUBJECT_TYPE_INT :
            اگر(subject->مقدار.رقم != subject->prev_مقدار.رقم) {
                lv_subject_notify(subject);
            }
            بشکن;
#اگر LV_USE_FLOAT
        حالت LV_SUBJECT_TYPE_FLOAT :
            اگر(subject->مقدار.float_v != subject->prev_مقدار.float_v) {
                lv_subject_notify(subject);
            }
            بشکن;
#endif
        حالت LV_SUBJECT_TYPE_GROUP :
        حالت LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            بشکن;
        حالت LV_SUBJECT_TYPE_COLOR  :
            اگر(!lv_color_eq(subject->مقدار.color, subject->prev_مقدار.color)) {
                lv_subject_notify(subject);
            }
            بشکن;
        حالت LV_SUBJECT_TYPE_STRING:
            اگر(!subject->prev_مقدار.pointer ||
               lv_strcmp(subject->مقدار.pointer, subject->prev_مقدار.pointer)) {
                lv_subject_notify(subject);
            }
            بشکن;
        حالت LV_SUBJECT_TYPE_INVALID :
        حالت LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ایستا تهی subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != پوچ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != پوچ);
    lv_free((تهی *)user_data->مقدار);
    lv_free(user_data);
}

ایستا تهی set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    اگر(set_bool_cb) set_bool_cb(obj, subject->مقدار.رقم);
}

ایستا تهی set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    اگر(set_int_cb) set_int_cb(obj, subject->مقدار.رقم);
}

#اگر LV_USE_FLOAT
ایستا تهی set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    اگر(set_float_cb) set_float_cb(obj, subject->مقدار.float_v);
}
#endif /*LV_USE_FLOAT*/

ایستا تهی set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    اگر(set_string_cb) set_string_cb(obj, subject->مقدار.pointer);
}


ایستا تهی set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    اگر(set_color_cb) set_color_cb(obj, subject->مقدار.color);
}

ایستا تهی set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    اگر(set_pointer_cb) set_pointer_cb(obj, subject->مقدار.pointer);
}

ایستا تهی remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    برای(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->مقدار.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

ایستا تهی subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, تهی * user_data)
{
    LV_ASSERT(subject != پوچ);
    LV_ASSERT(cb != پوچ);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    وقتی(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        اگر(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
