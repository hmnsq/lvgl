/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#அந்தர்பாவ "lv_observer_private.h"
#எனில் LV_USE_OBSERVER

#அந்தர்பாவ "../lvgl_public.h"
#அந்தர்பாவ "../core/lv_global.h"
#அந்தர்பாவ "../core/lv_obj_private.h"
#அந்தர்பாவ "../misc/lv_event_private.h"
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
வகைவரை கட்டமைப்பு {
    lv_state_t state;
} bound_state_t;

வகைவரை கட்டமைப்பு {
    lv_subject_t * subject;
    int32_t மதிப்பு;
} subject_set_int_user_data_t;

வகைவரை கட்டமைப்பு {
    lv_subject_t * subject;
    தசம மதிப்பு;
} subject_set_float_user_data_t;

வகைவரை கட்டமைப்பு {
    lv_subject_t * subject;
    மாறிலி எழுத்து * மதிப்பு;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

நிலையான வெறுமை remove_observers_from_group_subject(lv_subject_t * subject);
நிலையான வெறுமை add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
நிலையான வெறுமை subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, வெறுமை * user_data);
நிலையான வெறுமை subject_toggle_cb(lv_event_t * e);
நிலையான வெறுமை subject_set_int_cb(lv_event_t * e);
#எனில் LV_USE_FLOAT
    நிலையான வெறுமை subject_set_float_cb(lv_event_t * e);
#endif

நிலையான வெறுமை subject_set_string_cb(lv_event_t * e);
நிலையான வெறுமை subject_increment_cb(lv_event_t * e);

நிலையான வெறுமை unsubscribe_on_delete_cb(lv_event_t * e);
நிலையான வெறுமை group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
நிலையான lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

நிலையான வெறுமை obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
நிலையான வெறுமை obj_மதிப்பு_changed_event_cb(lv_event_t * e);

நிலையான வெறுமை lv_subject_notify_if_changed(lv_subject_t * subject);

நிலையான வெறுமை subject_set_string_free_user_data_event_cb(lv_event_t * e);

நிலையான வெறுமை set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
நிலையான வெறுமை set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
நிலையான வெறுமை set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
நிலையான வெறுமை set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
நிலையான வெறுமை set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#எனில் LV_USE_FLOAT
    நிலையான வெறுமை set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    நிலையான வெறுமை init_float(lv_subject_t * subject, தசம மதிப்பு);
#endif /*LV_USE_FLOAT*/

நிலையான வெறுமை init_int(lv_subject_t * subject, int32_t மதிப்பு);
நிலையான வெறுமை init_string(lv_subject_t * subject, எழுத்து * buf, எழுத்து * prev_buf, size_t size, மாறிலி எழுத்து * மதிப்பு);
நிலையான வெறுமை init_pointer(lv_subject_t * subject, வெறுமை * மதிப்பு);
நிலையான வெறுமை init_color(lv_subject_t * subject, lv_color_t color);
நிலையான வெறுமை init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
நிலையான வெறுமை init_common(lv_subject_t * subject);

நிலையான வெறுமை deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

வெறுமை lv_subject_global_init(வெறுமை)
{
    lv_ll_init(&subject_list, அளவு(lv_subject_t));
}
வெறுமை lv_subject_global_deinit(வெறுமை)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        எனில்(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        எனில்(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, திரும்பு வெற்று);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, திரும்பு வெற்று);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    எனில்(!subject) {
        திரும்பு வெற்று;
    }

    init_common(subject);

    தேர்வு(type) {
        நிலை LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_FLOAT:
#எனில் LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, வெற்று);
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_GROUP:
            init_group(subject, வெற்று, 0);
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_STRING:
            init_string(subject, வெற்று, வெற்று, 0, "");
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_INVALID:
        நிலை LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    திரும்பு subject;
}

வெறுமை lv_subject_delete(lv_subject_t * subject)
{
    எனில்(!subject) {
        திரும்பு;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#எனில் LV_USE_EXT_DATA
வெறுமை lv_subject_set_external_data(lv_subject_t * subject, வெறுமை * data, வெறுமை (* free_cb)(வெறுமை * data))
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

வெறுமை lv_subject_set_int(lv_subject_t * subject, int32_t மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு);

    மதிப்பு = LV_CLAMP(subject->min_மதிப்பு.இலக்கம், மதிப்பு, subject->max_மதிப்பு.இலக்கம்);

    subject->prev_மதிப்பு.இலக்கம் = subject->மதிப்பு.இலக்கம்;
    subject->மதிப்பு.இலக்கம் = மதிப்பு;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு 0);

    திரும்பு subject->மதிப்பு.இலக்கம்;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு 0);

    திரும்பு subject->prev_மதிப்பு.இலக்கம்;
}

வெறுமை lv_subject_set_min_மதிப்பு_int(lv_subject_t * subject, int32_t min_மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு);

    subject->min_மதிப்பு.இலக்கம் = min_மதிப்பு;
}

வெறுமை lv_subject_set_max_மதிப்பு_int(lv_subject_t * subject, int32_t max_மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு);

    subject->max_மதிப்பு.இலக்கம் = max_மதிப்பு;
}

#எனில் LV_USE_FLOAT

வெறுமை lv_subject_set_float(lv_subject_t * subject, தசம மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு);

    மதிப்பு = LV_CLAMP(subject->min_மதிப்பு.float_v, மதிப்பு, subject->max_மதிப்பு.float_v);

    subject->prev_மதிப்பு.float_v = subject->மதிப்பு.float_v;
    subject->மதிப்பு.float_v = மதிப்பு;
    lv_subject_notify_if_changed(subject);
}

தசம lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு 0.0);

    திரும்பு subject->மதிப்பு.float_v;
}

தசம lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு 0.0);

    திரும்பு subject->prev_மதிப்பு.float_v;
}

வெறுமை lv_subject_set_min_மதிப்பு_float(lv_subject_t * subject, தசம min_மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு);

    subject->min_மதிப்பு.float_v = min_மதிப்பு;
}

வெறுமை lv_subject_set_max_மதிப்பு_float(lv_subject_t * subject, தசம max_மதிப்பு)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு);

    subject->max_மதிப்பு.float_v = max_மதிப்பு;
}


#endif /*LV_USE_FLOAT*/

வெறுமை lv_subject_set_string_buffer_static(lv_subject_t * subject, எழுத்து * buf, எழுத்து * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு);
    LV_CHECK_ARG(buf != வெற்று, திரும்பு);
    LV_CHECK_ARG(size > 0, திரும்பு);

    buf[0] = '\0';
    எனில்(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->மதிப்பு.pointer = buf;
    subject->prev_மதிப்பு.pointer = prev_buf;
}

வெறுமை lv_subject_set_string(lv_subject_t * subject, மாறிலி எழுத்து * buf)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு);
    LV_CHECK_ARG(buf != வெற்று, திரும்பு);

    எனில்(subject->size == 0) திரும்பு;
    எனில்(subject->prev_மதிப்பு.pointer) {
        lv_strlcpy((எழுத்து *)subject->prev_மதிப்பு.pointer, subject->மதிப்பு.pointer, subject->size);
    }

    lv_strlcpy((எழுத்து *)subject->மதிப்பு.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

வெறுமை lv_subject_snprintf(lv_subject_t * subject, மாறிலி எழுத்து * format, ...)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு);
    LV_CHECK_ARG(format != வெற்று, திரும்பு);

    எனில்(subject->size < 1U) திரும்பு;

    எனில்(subject->prev_மதிப்பு.pointer) {
        lv_strlcpy((எழுத்து *)subject->prev_மதிப்பு.pointer, subject->மதிப்பு.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    மாறிலி முழு ret = lv_vsnprintf((எழுத்து *)subject->மதிப்பு.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

மாறிலி எழுத்து * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு வெற்று);

    திரும்பு subject->மதிப்பு.pointer;
}

மாறிலி எழுத்து * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு வெற்று);

    திரும்பு subject->prev_மதிப்பு.pointer;
}

வெறுமை lv_subject_set_pointer(lv_subject_t * subject, வெறுமை * ptr)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, திரும்பு);

    subject->prev_மதிப்பு.pointer = subject->மதிப்பு.pointer;
    subject->மதிப்பு.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

மாறிலி வெறுமை * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, திரும்பு வெற்று);


    திரும்பு subject->மதிப்பு.pointer;
}

மாறிலி வெறுமை * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, திரும்பு வெற்று);

    திரும்பு subject->prev_மதிப்பு.pointer;
}

வெறுமை lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, திரும்பு);

    subject->prev_மதிப்பு.color = subject->மதிப்பு.color;
    subject->மதிப்பு.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, திரும்பு lv_color_black());

    திரும்பு subject->மதிப்பு.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, திரும்பு lv_color_black());

    திரும்பு subject->prev_மதிப்பு.color;
}

வெறுமை lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(list != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, திரும்பு);

    க்கு(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_செய்தி(list[i] != வெற்று, திரும்பு, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, திரும்பு வெற்று);
    LV_CHECK_ARG(index >= 0 && index < subject->size, திரும்பு வெற்று);

    திரும்பு ((lv_subject_t **)(subject->மதிப்பு.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, வெறுமை * user_data)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(observer_cb != வெற்று, திரும்பு வெற்று);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, வெற்று, user_data);
    எனில்(observer == வெற்று) திரும்பு வெற்று;

    observer->for_obj = 0;
    திரும்பு observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            வெறுமை * user_data)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(observer_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, திரும்பு வெற்று);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    எனில்(observer == வெற்று) திரும்பு வெற்று;

    lv_memzero(observer, அளவு(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    எனில்(obj != வெற்று) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    திரும்பு observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, வெறுமை * target,
                                                    வெறுமை * user_data)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(observer_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, திரும்பு வெற்று);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    எனில்(observer == வெற்று) திரும்பு வெற்று;

    lv_memzero(observer, அளவு(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    திரும்பு observer;
}


வெறுமை lv_observer_delete(lv_observer_t * observer)
{
    எனில்(observer == வெற்று) திரும்பு;

    எனில்(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, வெற்று, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    எனில்(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

வெறுமை lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_மொத்தம்(&obj->spec_attr->event_list) : 0);
    க்கு(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        எனில்(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            எனில்(subject == வெற்று || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, வெற்று, subject);

}

வெறுமை * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != வெற்று, திரும்பு வெற்று);

    திரும்பு observer->target;
}

வெறுமை lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    செய் {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            எனில்(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                எனில்(subject->notify_restart_query) நிறுத்து;
                observer->notified = 1;
            }
        }
    } போது(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு வெற்று);

    lv_subject_increment_dsc_t * user_data = lv_malloc(அளவு(lv_subject_increment_dsc_t));
    எனில்(user_data == வெற்று) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        திரும்பு வெற்று;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_மதிப்பு = INT32_MIN;
    user_data->max_மதிப்பு = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    திரும்பு user_data;
}

வெறுமை lv_obj_set_subject_increment_event_min_மதிப்பு(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_மதிப்பு)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, திரும்பு);
    LV_CHECK_ARG(dsc != வெற்று, திரும்பு);

    dsc->min_மதிப்பு = min_மதிப்பு;
    எனில்(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        எனில்(dsc->subject->மதிப்பு.இலக்கம் < min_மதிப்பு) {
            lv_subject_set_int(dsc->subject, min_மதிப்பு);
        }
    }
#எனில் LV_USE_FLOAT
    இல்லையெனில் எனில்(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        எனில்(dsc->subject->மதிப்பு.float_v < (தசம)min_மதிப்பு) {
            lv_subject_set_float(dsc->subject, (தசம)min_மதிப்பு);
        }
    }
#endif
}

வெறுமை lv_obj_set_subject_increment_event_max_மதிப்பு(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_மதிப்பு)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, திரும்பு);
    LV_CHECK_ARG(dsc != வெற்று, திரும்பு);

    dsc->max_மதிப்பு = max_மதிப்பு;
    எனில்(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        எனில்(dsc->subject->மதிப்பு.இலக்கம் > max_மதிப்பு) {
            lv_subject_set_int(dsc->subject, max_மதிப்பு);
        }
    }
#எனில் LV_USE_FLOAT
    இல்லையெனில் எனில்(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        எனில்(dsc->subject->மதிப்பு.float_v > (தசம)max_மதிப்பு) {
            lv_subject_set_float(dsc->subject, (தசம)max_மதிப்பு);
        }
    }
#endif
}

வெறுமை lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, திரும்பு);
    LV_CHECK_ARG(dsc != வெற்று, திரும்பு);

    dsc->rollover = rollover;
}

வெறுமை lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

வெறுமை lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t மதிப்பு)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு);

    subject_set_int_user_data_t * user_data = lv_malloc(அளவு(subject_set_int_user_data_t));
    எனில்(user_data == வெற்று) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        திரும்பு;
    }

    user_data->subject = subject;
    user_data->மதிப்பு = மதிப்பு;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#எனில் LV_USE_FLOAT
வெறுமை lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, தசம மதிப்பு)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு);

    subject_set_float_user_data_t * user_data = lv_malloc(அளவு(subject_set_float_user_data_t));
    எனில்(user_data == வெற்று) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        திரும்பு;
    }

    user_data->subject = subject;
    user_data->மதிப்பு = மதிப்பு;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

வெறுமை lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         மாறிலி எழுத்து * மதிப்பு)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு);
    LV_CHECK_ARG(மதிப்பு != வெற்று, திரும்பு);

    subject_set_string_user_data_t * user_data = lv_malloc(அளவு(subject_set_string_user_data_t));
    எனில்(user_data == வெற்று) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        திரும்பு;
    }

    user_data->subject = subject;
    user_data->மதிப்பு = lv_strdup(மதிப்பு);
    எனில்(user_data->மதிப்பு == வெற்று) {
        LV_ASSERT_MALLOC(user_data->மதிப்பு);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        திரும்பு;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_bool_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு வெற்று);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_bool_cb;
    set_bool_observer(observable, subject);
    திரும்பு observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_int_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு வெற்று);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_int_cb;
    set_int_observer(observable, subject);
    திரும்பு observable;
}

#எனில் LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_float_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, திரும்பு வெற்று);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_float_cb;
    set_float_observer(observable, subject);
    திரும்பு observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_string_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, திரும்பு வெற்று);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_string_cb;
    set_string_observer(observable, subject);
    திரும்பு observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_color_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, திரும்பு வெற்று);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_color_cb;
    set_color_observer(observable, subject);
    திரும்பு observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(set_pointer_cb != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, திரும்பு வெற்று);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, வெற்று);
    எனில்(observable == வெற்று) {
        LV_LOG_WARN("Couldn't add observer to subject");
        திரும்பு வெற்று;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (வெறுமை (*)(வெறுமை))set_pointer_cb;
    set_pointer_observer(observable, subject);
    திரும்பு observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(subject != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG_FORMAT_செய்தி(subject->type == LV_SUBJECT_TYPE_INT, திரும்பு வெற்று, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_மதிப்பு_changed_event_cb, LV_EVENT_மதிப்பு_CHANGED, subject);

    திரும்பு observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != வெற்று, திரும்பு வெற்று);

    திரும்பு (lv_obj_t *)lv_observer_get_target(observer);
}

வெறுமை * lv_observer_get_user_data(மாறிலி lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != வெற்று, திரும்பு வெற்று);

    திரும்பு observer->user_data;
}

வெறுமை lv_observer_set_user_data(lv_observer_t * observer, வெறுமை * user_data)
{
    LV_CHECK_ARG(observer != வெற்று, திரும்பு);

    /* Release internally-owned data only when replacing it. */
    எனில்(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

நிலையான வெறுமை init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != வெற்று);
    lv_memzero(subject, அளவு(*subject));
    lv_ll_init(&(subject->subs_ll), அளவு(lv_observer_t));
}
நிலையான வெறுமை init_int(lv_subject_t * subject, int32_t மதிப்பு)
{
    LV_ASSERT(subject != வெற்று);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->மதிப்பு.இலக்கம் = மதிப்பு;
    subject->prev_மதிப்பு.இலக்கம் = மதிப்பு;
    subject->min_மதிப்பு.இலக்கம் = INT32_MIN;
    subject->max_மதிப்பு.இலக்கம் = INT32_MAX;
}

#எனில் LV_USE_FLOAT

நிலையான வெறுமை init_float(lv_subject_t * subject, தசம மதிப்பு)
{
    LV_ASSERT(subject != வெற்று);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->மதிப்பு.float_v = மதிப்பு;
    subject->prev_மதிப்பு.float_v = மதிப்பு;
    subject->min_மதிப்பு.float_v = -FLT_MAX;
    subject->max_மதிப்பு.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

நிலையான வெறுமை init_pointer(lv_subject_t * subject, வெறுமை * மதிப்பு)
{
    LV_ASSERT(subject != வெற்று);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->மதிப்பு.pointer = மதிப்பு;
    subject->prev_மதிப்பு.pointer = மதிப்பு;
}

நிலையான வெறுமை init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != வெற்று);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->மதிப்பு.color = color;
    subject->prev_மதிப்பு.color = color;
}

நிலையான வெறுமை init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(list != வெற்று || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

நிலையான வெறுமை add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(list != வெற்று || size == 0);
    subject->மதிப்பு.pointer = list;
    subject->size = (uint32_t)size;

    க்கு(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

நிலையான வெறுமை init_string(lv_subject_t * subject, எழுத்து * buf, எழுத்து * prev_buf, size_t size, மாறிலி எழுத்து * மதிப்பு)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(buf != வெற்று || size == 0);

    எனில்(size > 0) {
        lv_strlcpy(buf, மதிப்பு, size);
        எனில்(prev_buf) lv_strlcpy(prev_buf, மதிப்பு, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->மதிப்பு.pointer = buf;
    subject->prev_மதிப்பு.pointer = prev_buf;
}

நிலையான வெறுமை deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != வெற்று);
    எனில்(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    போது(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#எனில் LV_USE_EXT_DATA
    எனில்(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = வெற்று;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

நிலையான வெறுமை subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

நிலையான வெறுமை subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->மதிப்பு);
}


#எனில் LV_USE_FLOAT
நிலையான வெறுமை subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->மதிப்பு);
}
#endif

நிலையான வெறுமை subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->மதிப்பு);
}

நிலையான வெறுமை subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    எனில்(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_மதிப்பு = LV_MIN(user_data->max_மதிப்பு, user_data->subject->max_மதிப்பு.இலக்கம்);
        int32_t min_மதிப்பு = LV_MAX(user_data->min_மதிப்பு, user_data->subject->min_மதிப்பு.இலக்கம்);

        int32_t மதிப்பு = lv_subject_get_int(user_data->subject);
        மதிப்பு += user_data->step;

        எனில்(user_data->rollover) {
            எனில்(மதிப்பு > max_மதிப்பு) {
                மதிப்பு = min_மதிப்பு;
            }
            இல்லையெனில் எனில்(மதிப்பு < min_மதிப்பு) {
                மதிப்பு = max_மதிப்பு;
            }
        }
        இல்லையெனில் {
            மதிப்பு = LV_CLAMP(min_மதிப்பு, மதிப்பு, max_மதிப்பு);
        }

        lv_subject_set_int(user_data->subject, மதிப்பு);
    }
#எனில் LV_USE_FLOAT
    இல்லையெனில் எனில்(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        தசம max_மதிப்பு = LV_MIN((தசம)user_data->max_மதிப்பு, user_data->subject->max_மதிப்பு.float_v);
        தசம min_மதிப்பு = LV_MAX((தசம)user_data->min_மதிப்பு, user_data->subject->min_மதிப்பு.float_v);


        தசம மதிப்பு = lv_subject_get_float(user_data->subject);
        மதிப்பு += (தசம)user_data->step;

        எனில்(user_data->rollover) {
            எனில்(மதிப்பு > max_மதிப்பு) {
                மதிப்பு = min_மதிப்பு;
            }
            இல்லையெனில் எனில்(மதிப்பு < min_மதிப்பு) {
                மதிப்பு = max_மதிப்பு;
            }
        }
        இல்லையெனில் {
            மதிப்பு = LV_CLAMP(min_மதிப்பு, மதிப்பு, max_மதிப்பு);
        }

        lv_subject_set_float(user_data->subject, மதிப்பு);
    }
#endif
}

நிலையான வெறுமை group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != வெற்று);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

நிலையான வெறுமை unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

நிலையான lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(obj != வெற்று);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(அளவு(bound_state_t));
    எனில்(p == வெற்று) {
        LV_LOG_WARN("Out of memory");
        திரும்பு வெற்று;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    திரும்பு observable;
}

நிலையான வெறுமை obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != வெற்று);
    LV_ASSERT(subject != வெற்று);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != வெற்று);

    எனில்(subject->மதிப்பு.இலக்கம்) {
        lv_obj_add_state(observer->target, p->state);
    }
    இல்லையெனில் {
        lv_obj_remove_state(observer->target, p->state);
    }
}

நிலையான வெறுமை obj_மதிப்பு_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(obj != வெற்று);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

நிலையான வெறுமை lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    தேர்வு(subject->type) {
        நிலை LV_SUBJECT_TYPE_INT :
            எனில்(subject->மதிப்பு.இலக்கம் != subject->prev_மதிப்பு.இலக்கம்) {
                lv_subject_notify(subject);
            }
            நிறுத்து;
#எனில் LV_USE_FLOAT
        நிலை LV_SUBJECT_TYPE_FLOAT :
            எனில்(subject->மதிப்பு.float_v != subject->prev_மதிப்பு.float_v) {
                lv_subject_notify(subject);
            }
            நிறுத்து;
#endif
        நிலை LV_SUBJECT_TYPE_GROUP :
        நிலை LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_COLOR  :
            எனில்(!lv_color_eq(subject->மதிப்பு.color, subject->prev_மதிப்பு.color)) {
                lv_subject_notify(subject);
            }
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_STRING:
            எனில்(!subject->prev_மதிப்பு.pointer ||
               lv_strcmp(subject->மதிப்பு.pointer, subject->prev_மதிப்பு.pointer)) {
                lv_subject_notify(subject);
            }
            நிறுத்து;
        நிலை LV_SUBJECT_TYPE_INVALID :
        நிலை LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

நிலையான வெறுமை subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != வெற்று);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != வெற்று);
    lv_free((வெறுமை *)user_data->மதிப்பு);
    lv_free(user_data);
}

நிலையான வெறுமை set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    எனில்(set_bool_cb) set_bool_cb(obj, subject->மதிப்பு.இலக்கம்);
}

நிலையான வெறுமை set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    எனில்(set_int_cb) set_int_cb(obj, subject->மதிப்பு.இலக்கம்);
}

#எனில் LV_USE_FLOAT
நிலையான வெறுமை set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    எனில்(set_float_cb) set_float_cb(obj, subject->மதிப்பு.float_v);
}
#endif /*LV_USE_FLOAT*/

நிலையான வெறுமை set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    எனில்(set_string_cb) set_string_cb(obj, subject->மதிப்பு.pointer);
}


நிலையான வெறுமை set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    எனில்(set_color_cb) set_color_cb(obj, subject->மதிப்பு.color);
}

நிலையான வெறுமை set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    எனில்(set_pointer_cb) set_pointer_cb(obj, subject->மதிப்பு.pointer);
}

நிலையான வெறுமை remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    க்கு(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->மதிப்பு.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

நிலையான வெறுமை subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, வெறுமை * user_data)
{
    LV_ASSERT(subject != வெற்று);
    LV_ASSERT(cb != வெற்று);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    போது(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        எனில்(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
