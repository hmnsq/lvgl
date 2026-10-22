/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#অন্তর্ভাব "lv_observer_private.h"
#যদি LV_USE_OBSERVER

#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "../core/lv_global.h"
#অন্তর্ভাব "../core/lv_obj_private.h"
#অন্তর্ভাব "../misc/lv_event_private.h"
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
প্রকারনির্দেশ সংরচনা {
    lv_state_t state;
} bound_state_t;

প্রকারনির্দেশ সংরচনা {
    lv_subject_t * subject;
    int32_t মান;
} subject_set_int_user_data_t;

প্রকারনির্দেশ সংরচনা {
    lv_subject_t * subject;
    ভিন্ন মান;
} subject_set_float_user_data_t;

প্রকারনির্দেশ সংরচনা {
    lv_subject_t * subject;
    ধ্রুব বর্ণ * মান;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

স্থির শূন্য remove_observers_from_group_subject(lv_subject_t * subject);
স্থির শূন্য add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
স্থির শূন্য subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, শূন্য * user_data);
স্থির শূন্য subject_toggle_cb(lv_event_t * e);
স্থির শূন্য subject_set_int_cb(lv_event_t * e);
#যদি LV_USE_FLOAT
    স্থির শূন্য subject_set_float_cb(lv_event_t * e);
#endif

স্থির শূন্য subject_set_string_cb(lv_event_t * e);
স্থির শূন্য subject_increment_cb(lv_event_t * e);

স্থির শূন্য unsubscribe_on_delete_cb(lv_event_t * e);
স্থির শূন্য group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
স্থির lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

স্থির শূন্য obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
স্থির শূন্য obj_মান_changed_event_cb(lv_event_t * e);

স্থির শূন্য lv_subject_notify_if_changed(lv_subject_t * subject);

স্থির শূন্য subject_set_string_free_user_data_event_cb(lv_event_t * e);

স্থির শূন্য set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
স্থির শূন্য set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
স্থির শূন্য set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
স্থির শূন্য set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
স্থির শূন্য set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#যদি LV_USE_FLOAT
    স্থির শূন্য set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    স্থির শূন্য init_float(lv_subject_t * subject, ভিন্ন মান);
#endif /*LV_USE_FLOAT*/

স্থির শূন্য init_int(lv_subject_t * subject, int32_t মান);
স্থির শূন্য init_string(lv_subject_t * subject, বর্ণ * buf, বর্ণ * prev_buf, size_t size, ধ্রুব বর্ণ * মান);
স্থির শূন্য init_pointer(lv_subject_t * subject, শূন্য * মান);
স্থির শূন্য init_color(lv_subject_t * subject, lv_color_t color);
স্থির শূন্য init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
স্থির শূন্য init_common(lv_subject_t * subject);

স্থির শূন্য deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

শূন্য lv_subject_global_init(শূন্য)
{
    lv_ll_init(&subject_list, পরিমাণ(lv_subject_t));
}
শূন্য lv_subject_global_deinit(শূন্য)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        যদি(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        যদি(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম রিক্ত);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    যদি(!subject) {
        প্রত্যাগম রিক্ত;
    }

    init_common(subject);

    বিকল্প(type) {
        পক্ষ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_FLOAT:
#যদি LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, রিক্ত);
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_GROUP:
            init_group(subject, রিক্ত, 0);
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_STRING:
            init_string(subject, রিক্ত, রিক্ত, 0, "");
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_INVALID:
        পক্ষ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    প্রত্যাগম subject;
}

শূন্য lv_subject_delete(lv_subject_t * subject)
{
    যদি(!subject) {
        প্রত্যাগম;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#যদি LV_USE_EXT_DATA
শূন্য lv_subject_set_external_data(lv_subject_t * subject, শূন্য * data, শূন্য (* free_cb)(শূন্য * data))
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

শূন্য lv_subject_set_int(lv_subject_t * subject, int32_t মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম);

    মান = LV_CLAMP(subject->min_মান.অঙ্ক, মান, subject->max_মান.অঙ্ক);

    subject->prev_মান.অঙ্ক = subject->মান.অঙ্ক;
    subject->মান.অঙ্ক = মান;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম 0);

    প্রত্যাগম subject->মান.অঙ্ক;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম 0);

    প্রত্যাগম subject->prev_মান.অঙ্ক;
}

শূন্য lv_subject_set_min_মান_int(lv_subject_t * subject, int32_t min_মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম);

    subject->min_মান.অঙ্ক = min_মান;
}

শূন্য lv_subject_set_max_মান_int(lv_subject_t * subject, int32_t max_মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম);

    subject->max_মান.অঙ্ক = max_মান;
}

#যদি LV_USE_FLOAT

শূন্য lv_subject_set_float(lv_subject_t * subject, ভিন্ন মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম);

    মান = LV_CLAMP(subject->min_মান.float_v, মান, subject->max_মান.float_v);

    subject->prev_মান.float_v = subject->মান.float_v;
    subject->মান.float_v = মান;
    lv_subject_notify_if_changed(subject);
}

ভিন্ন lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম 0.0);

    প্রত্যাগম subject->মান.float_v;
}

ভিন্ন lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম 0.0);

    প্রত্যাগম subject->prev_মান.float_v;
}

শূন্য lv_subject_set_min_মান_float(lv_subject_t * subject, ভিন্ন min_মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম);

    subject->min_মান.float_v = min_মান;
}

শূন্য lv_subject_set_max_মান_float(lv_subject_t * subject, ভিন্ন max_মান)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম);

    subject->max_মান.float_v = max_মান;
}


#endif /*LV_USE_FLOAT*/

শূন্য lv_subject_set_string_buffer_static(lv_subject_t * subject, বর্ণ * buf, বর্ণ * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম);
    LV_CHECK_ARG(buf != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(size > 0, প্রত্যাগম);

    buf[0] = '\0';
    যদি(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->মান.pointer = buf;
    subject->prev_মান.pointer = prev_buf;
}

শূন্য lv_subject_set_string(lv_subject_t * subject, ধ্রুব বর্ণ * buf)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম);
    LV_CHECK_ARG(buf != রিক্ত, প্রত্যাগম);

    যদি(subject->size == 0) প্রত্যাগম;
    যদি(subject->prev_মান.pointer) {
        lv_strlcpy((বর্ণ *)subject->prev_মান.pointer, subject->মান.pointer, subject->size);
    }

    lv_strlcpy((বর্ণ *)subject->মান.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

শূন্য lv_subject_snprintf(lv_subject_t * subject, ধ্রুব বর্ণ * format, ...)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম);
    LV_CHECK_ARG(format != রিক্ত, প্রত্যাগম);

    যদি(subject->size < 1U) প্রত্যাগম;

    যদি(subject->prev_মান.pointer) {
        lv_strlcpy((বর্ণ *)subject->prev_মান.pointer, subject->মান.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    ধ্রুব পূর্ণাঙ্ক ret = lv_vsnprintf((বর্ণ *)subject->মান.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

ধ্রুব বর্ণ * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম রিক্ত);

    প্রত্যাগম subject->মান.pointer;
}

ধ্রুব বর্ণ * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম রিক্ত);

    প্রত্যাগম subject->prev_মান.pointer;
}

শূন্য lv_subject_set_pointer(lv_subject_t * subject, শূন্য * ptr)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, প্রত্যাগম);

    subject->prev_মান.pointer = subject->মান.pointer;
    subject->মান.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

ধ্রুব শূন্য * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, প্রত্যাগম রিক্ত);


    প্রত্যাগম subject->মান.pointer;
}

ধ্রুব শূন্য * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, প্রত্যাগম রিক্ত);

    প্রত্যাগম subject->prev_মান.pointer;
}

শূন্য lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, প্রত্যাগম);

    subject->prev_মান.color = subject->মান.color;
    subject->মান.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, প্রত্যাগম lv_color_black());

    প্রত্যাগম subject->মান.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, প্রত্যাগম lv_color_black());

    প্রত্যাগম subject->prev_মান.color;
}

শূন্য lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(list != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, প্রত্যাগম);

    প্রতি(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_বার্তা(list[i] != রিক্ত, প্রত্যাগম, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(index >= 0 && index < subject->size, প্রত্যাগম রিক্ত);

    প্রত্যাগম ((lv_subject_t **)(subject->মান.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, শূন্য * user_data)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(observer_cb != রিক্ত, প্রত্যাগম রিক্ত);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, রিক্ত, user_data);
    যদি(observer == রিক্ত) প্রত্যাগম রিক্ত;

    observer->for_obj = 0;
    প্রত্যাগম observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            শূন্য * user_data)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(observer_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, প্রত্যাগম রিক্ত);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    যদি(observer == রিক্ত) প্রত্যাগম রিক্ত;

    lv_memzero(observer, পরিমাণ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    যদি(obj != রিক্ত) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    প্রত্যাগম observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, শূন্য * target,
                                                    শূন্য * user_data)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(observer_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, প্রত্যাগম রিক্ত);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    যদি(observer == রিক্ত) প্রত্যাগম রিক্ত;

    lv_memzero(observer, পরিমাণ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    প্রত্যাগম observer;
}


শূন্য lv_observer_delete(lv_observer_t * observer)
{
    যদি(observer == রিক্ত) প্রত্যাগম;

    যদি(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, রিক্ত, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    যদি(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

শূন্য lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_গুনতি(&obj->spec_attr->event_list) : 0);
    প্রতি(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        যদি(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            যদি(subject == রিক্ত || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, রিক্ত, subject);

}

শূন্য * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != রিক্ত, প্রত্যাগম রিক্ত);

    প্রত্যাগম observer->target;
}

শূন্য lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    কুরু {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            যদি(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                যদি(subject->notify_restart_query) ভঙ্গ;
                observer->notified = 1;
            }
        }
    } যাবত্(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম রিক্ত);

    lv_subject_increment_dsc_t * user_data = lv_malloc(পরিমাণ(lv_subject_increment_dsc_t));
    যদি(user_data == রিক্ত) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        প্রত্যাগম রিক্ত;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_মান = INT32_MIN;
    user_data->max_মান = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    প্রত্যাগম user_data;
}

শূন্য lv_obj_set_subject_increment_event_min_মান(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_মান)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    LV_CHECK_ARG(dsc != রিক্ত, প্রত্যাগম);

    dsc->min_মান = min_মান;
    যদি(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        যদি(dsc->subject->মান.অঙ্ক < min_মান) {
            lv_subject_set_int(dsc->subject, min_মান);
        }
    }
#যদি LV_USE_FLOAT
    অন্যথা যদি(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        যদি(dsc->subject->মান.float_v < (ভিন্ন)min_মান) {
            lv_subject_set_float(dsc->subject, (ভিন্ন)min_মান);
        }
    }
#endif
}

শূন্য lv_obj_set_subject_increment_event_max_মান(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_মান)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    LV_CHECK_ARG(dsc != রিক্ত, প্রত্যাগম);

    dsc->max_মান = max_মান;
    যদি(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        যদি(dsc->subject->মান.অঙ্ক > max_মান) {
            lv_subject_set_int(dsc->subject, max_মান);
        }
    }
#যদি LV_USE_FLOAT
    অন্যথা যদি(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        যদি(dsc->subject->মান.float_v > (ভিন্ন)max_মান) {
            lv_subject_set_float(dsc->subject, (ভিন্ন)max_মান);
        }
    }
#endif
}

শূন্য lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    LV_CHECK_ARG(dsc != রিক্ত, প্রত্যাগম);

    dsc->rollover = rollover;
}

শূন্য lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

শূন্য lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t মান)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম);

    subject_set_int_user_data_t * user_data = lv_malloc(পরিমাণ(subject_set_int_user_data_t));
    যদি(user_data == রিক্ত) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        প্রত্যাগম;
    }

    user_data->subject = subject;
    user_data->মান = মান;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#যদি LV_USE_FLOAT
শূন্য lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, ভিন্ন মান)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম);

    subject_set_float_user_data_t * user_data = lv_malloc(পরিমাণ(subject_set_float_user_data_t));
    যদি(user_data == রিক্ত) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        প্রত্যাগম;
    }

    user_data->subject = subject;
    user_data->মান = মান;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

শূন্য lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         ধ্রুব বর্ণ * মান)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম);
    LV_CHECK_ARG(মান != রিক্ত, প্রত্যাগম);

    subject_set_string_user_data_t * user_data = lv_malloc(পরিমাণ(subject_set_string_user_data_t));
    যদি(user_data == রিক্ত) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        প্রত্যাগম;
    }

    user_data->subject = subject;
    user_data->মান = lv_strdup(মান);
    যদি(user_data->মান == রিক্ত) {
        LV_ASSERT_MALLOC(user_data->মান);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        প্রত্যাগম;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_bool_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম রিক্ত);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_bool_cb;
    set_bool_observer(observable, subject);
    প্রত্যাগম observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_int_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম রিক্ত);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_int_cb;
    set_int_observer(observable, subject);
    প্রত্যাগম observable;
}

#যদি LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_float_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, প্রত্যাগম রিক্ত);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_float_cb;
    set_float_observer(observable, subject);
    প্রত্যাগম observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_string_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, প্রত্যাগম রিক্ত);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_string_cb;
    set_string_observer(observable, subject);
    প্রত্যাগম observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_color_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, প্রত্যাগম রিক্ত);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_color_cb;
    set_color_observer(observable, subject);
    প্রত্যাগম observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(set_pointer_cb != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, প্রত্যাগম রিক্ত);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, রিক্ত);
    যদি(observable == রিক্ত) {
        LV_LOG_WARN("Couldn't add observer to subject");
        প্রত্যাগম রিক্ত;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (শূন্য (*)(শূন্য))set_pointer_cb;
    set_pointer_observer(observable, subject);
    প্রত্যাগম observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG_FORMAT_বার্তা(subject->type == LV_SUBJECT_TYPE_INT, প্রত্যাগম রিক্ত, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_মান_changed_event_cb, LV_EVENT_মান_CHANGED, subject);

    প্রত্যাগম observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != রিক্ত, প্রত্যাগম রিক্ত);

    প্রত্যাগম (lv_obj_t *)lv_observer_get_target(observer);
}

শূন্য * lv_observer_get_user_data(ধ্রুব lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != রিক্ত, প্রত্যাগম রিক্ত);

    প্রত্যাগম observer->user_data;
}

শূন্য lv_observer_set_user_data(lv_observer_t * observer, শূন্য * user_data)
{
    LV_CHECK_ARG(observer != রিক্ত, প্রত্যাগম);

    /* Release internally-owned data only when replacing it. */
    যদি(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির শূন্য init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != রিক্ত);
    lv_memzero(subject, পরিমাণ(*subject));
    lv_ll_init(&(subject->subs_ll), পরিমাণ(lv_observer_t));
}
স্থির শূন্য init_int(lv_subject_t * subject, int32_t মান)
{
    LV_ASSERT(subject != রিক্ত);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->মান.অঙ্ক = মান;
    subject->prev_মান.অঙ্ক = মান;
    subject->min_মান.অঙ্ক = INT32_MIN;
    subject->max_মান.অঙ্ক = INT32_MAX;
}

#যদি LV_USE_FLOAT

স্থির শূন্য init_float(lv_subject_t * subject, ভিন্ন মান)
{
    LV_ASSERT(subject != রিক্ত);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->মান.float_v = মান;
    subject->prev_মান.float_v = মান;
    subject->min_মান.float_v = -FLT_MAX;
    subject->max_মান.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

স্থির শূন্য init_pointer(lv_subject_t * subject, শূন্য * মান)
{
    LV_ASSERT(subject != রিক্ত);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->মান.pointer = মান;
    subject->prev_মান.pointer = মান;
}

স্থির শূন্য init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != রিক্ত);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->মান.color = color;
    subject->prev_মান.color = color;
}

স্থির শূন্য init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(list != রিক্ত || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

স্থির শূন্য add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(list != রিক্ত || size == 0);
    subject->মান.pointer = list;
    subject->size = (uint32_t)size;

    প্রতি(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

স্থির শূন্য init_string(lv_subject_t * subject, বর্ণ * buf, বর্ণ * prev_buf, size_t size, ধ্রুব বর্ণ * মান)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(buf != রিক্ত || size == 0);

    যদি(size > 0) {
        lv_strlcpy(buf, মান, size);
        যদি(prev_buf) lv_strlcpy(prev_buf, মান, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->মান.pointer = buf;
    subject->prev_মান.pointer = prev_buf;
}

স্থির শূন্য deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != রিক্ত);
    যদি(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    যাবত্(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#যদি LV_USE_EXT_DATA
    যদি(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = রিক্ত;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

স্থির শূন্য subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

স্থির শূন্য subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->মান);
}


#যদি LV_USE_FLOAT
স্থির শূন্য subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->মান);
}
#endif

স্থির শূন্য subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->মান);
}

স্থির শূন্য subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    যদি(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_মান = LV_MIN(user_data->max_মান, user_data->subject->max_মান.অঙ্ক);
        int32_t min_মান = LV_MAX(user_data->min_মান, user_data->subject->min_মান.অঙ্ক);

        int32_t মান = lv_subject_get_int(user_data->subject);
        মান += user_data->step;

        যদি(user_data->rollover) {
            যদি(মান > max_মান) {
                মান = min_মান;
            }
            অন্যথা যদি(মান < min_মান) {
                মান = max_মান;
            }
        }
        অন্যথা {
            মান = LV_CLAMP(min_মান, মান, max_মান);
        }

        lv_subject_set_int(user_data->subject, মান);
    }
#যদি LV_USE_FLOAT
    অন্যথা যদি(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ভিন্ন max_মান = LV_MIN((ভিন্ন)user_data->max_মান, user_data->subject->max_মান.float_v);
        ভিন্ন min_মান = LV_MAX((ভিন্ন)user_data->min_মান, user_data->subject->min_মান.float_v);


        ভিন্ন মান = lv_subject_get_float(user_data->subject);
        মান += (ভিন্ন)user_data->step;

        যদি(user_data->rollover) {
            যদি(মান > max_মান) {
                মান = min_মান;
            }
            অন্যথা যদি(মান < min_মান) {
                মান = max_মান;
            }
        }
        অন্যথা {
            মান = LV_CLAMP(min_মান, মান, max_মান);
        }

        lv_subject_set_float(user_data->subject, মান);
    }
#endif
}

স্থির শূন্য group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != রিক্ত);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

স্থির শূন্য unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

স্থির lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(obj != রিক্ত);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(পরিমাণ(bound_state_t));
    যদি(p == রিক্ত) {
        LV_LOG_WARN("Out of memory");
        প্রত্যাগম রিক্ত;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    প্রত্যাগম observable;
}

স্থির শূন্য obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != রিক্ত);
    LV_ASSERT(subject != রিক্ত);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != রিক্ত);

    যদি(subject->মান.অঙ্ক) {
        lv_obj_add_state(observer->target, p->state);
    }
    অন্যথা {
        lv_obj_remove_state(observer->target, p->state);
    }
}

স্থির শূন্য obj_মান_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(obj != রিক্ত);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

স্থির শূন্য lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    বিকল্প(subject->type) {
        পক্ষ LV_SUBJECT_TYPE_INT :
            যদি(subject->মান.অঙ্ক != subject->prev_মান.অঙ্ক) {
                lv_subject_notify(subject);
            }
            ভঙ্গ;
#যদি LV_USE_FLOAT
        পক্ষ LV_SUBJECT_TYPE_FLOAT :
            যদি(subject->মান.float_v != subject->prev_মান.float_v) {
                lv_subject_notify(subject);
            }
            ভঙ্গ;
#endif
        পক্ষ LV_SUBJECT_TYPE_GROUP :
        পক্ষ LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_COLOR  :
            যদি(!lv_color_eq(subject->মান.color, subject->prev_মান.color)) {
                lv_subject_notify(subject);
            }
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_STRING:
            যদি(!subject->prev_মান.pointer ||
               lv_strcmp(subject->মান.pointer, subject->prev_মান.pointer)) {
                lv_subject_notify(subject);
            }
            ভঙ্গ;
        পক্ষ LV_SUBJECT_TYPE_INVALID :
        পক্ষ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

স্থির শূন্য subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != রিক্ত);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != রিক্ত);
    lv_free((শূন্য *)user_data->মান);
    lv_free(user_data);
}

স্থির শূন্য set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    যদি(set_bool_cb) set_bool_cb(obj, subject->মান.অঙ্ক);
}

স্থির শূন্য set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    যদি(set_int_cb) set_int_cb(obj, subject->মান.অঙ্ক);
}

#যদি LV_USE_FLOAT
স্থির শূন্য set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    যদি(set_float_cb) set_float_cb(obj, subject->মান.float_v);
}
#endif /*LV_USE_FLOAT*/

স্থির শূন্য set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    যদি(set_string_cb) set_string_cb(obj, subject->মান.pointer);
}


স্থির শূন্য set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    যদি(set_color_cb) set_color_cb(obj, subject->মান.color);
}

স্থির শূন্য set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    যদি(set_pointer_cb) set_pointer_cb(obj, subject->মান.pointer);
}

স্থির শূন্য remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    প্রতি(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->মান.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

স্থির শূন্য subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, শূন্য * user_data)
{
    LV_ASSERT(subject != রিক্ত);
    LV_ASSERT(cb != রিক্ত);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    যাবত্(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        যদি(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
