/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#અન્તર્ભાવ "lv_observer_private.h"
#જો LV_USE_OBSERVER

#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "../core/lv_global.h"
#અન્તર્ભાવ "../core/lv_obj_private.h"
#અન્તર્ભાવ "../misc/lv_event_private.h"
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
પ્રકારવ્યાખ્યા સંરચના {
    lv_state_t state;
} bound_state_t;

પ્રકારવ્યાખ્યા સંરચના {
    lv_subject_t * subject;
    int32_t કિંમત;
} subject_set_int_user_data_t;

પ્રકારવ્યાખ્યા સંરચના {
    lv_subject_t * subject;
    દશાંશ કિંમત;
} subject_set_float_user_data_t;

પ્રકારવ્યાખ્યા સંરચના {
    lv_subject_t * subject;
    અચલ અક્ષર * કિંમત;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

સ્થિર શૂન્ય remove_observers_from_group_subject(lv_subject_t * subject);
સ્થિર શૂન્ય add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
સ્થિર શૂન્ય subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, શૂન્ય * user_data);
સ્થિર શૂન્ય subject_toggle_cb(lv_event_t * e);
સ્થિર શૂન્ય subject_set_int_cb(lv_event_t * e);
#જો LV_USE_FLOAT
    સ્થિર શૂન્ય subject_set_float_cb(lv_event_t * e);
#endif

સ્થિર શૂન્ય subject_set_string_cb(lv_event_t * e);
સ્થિર શૂન્ય subject_increment_cb(lv_event_t * e);

સ્થિર શૂન્ય unsubscribe_on_delete_cb(lv_event_t * e);
સ્થિર શૂન્ય group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

સ્થિર શૂન્ય obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર શૂન્ય obj_કિંમત_changed_event_cb(lv_event_t * e);

સ્થિર શૂન્ય lv_subject_notify_if_changed(lv_subject_t * subject);

સ્થિર શૂન્ય subject_set_string_free_user_data_event_cb(lv_event_t * e);

સ્થિર શૂન્ય set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર શૂન્ય set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર શૂન્ય set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર શૂન્ય set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
સ્થિર શૂન્ય set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#જો LV_USE_FLOAT
    સ્થિર શૂન્ય set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    સ્થિર શૂન્ય init_float(lv_subject_t * subject, દશાંશ કિંમત);
#endif /*LV_USE_FLOAT*/

સ્થિર શૂન્ય init_int(lv_subject_t * subject, int32_t કિંમત);
સ્થિર શૂન્ય init_string(lv_subject_t * subject, અક્ષર * buf, અક્ષર * prev_buf, size_t size, અચલ અક્ષર * કિંમત);
સ્થિર શૂન્ય init_pointer(lv_subject_t * subject, શૂન્ય * કિંમત);
સ્થિર શૂન્ય init_color(lv_subject_t * subject, lv_color_t color);
સ્થિર શૂન્ય init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
સ્થિર શૂન્ય init_common(lv_subject_t * subject);

સ્થિર શૂન્ય deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

શૂન્ય lv_subject_global_init(શૂન્ય)
{
    lv_ll_init(&subject_list, કદ(lv_subject_t));
}
શૂન્ય lv_subject_global_deinit(શૂન્ય)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        જો(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        જો(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, પાછા ખાલી);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, પાછા ખાલી);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    જો(!subject) {
        પાછા ખાલી;
    }

    init_common(subject);

    પસંદગી(type) {
        સ્થિતિ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_FLOAT:
#જો LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, ખાલી);
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_GROUP:
            init_group(subject, ખાલી, 0);
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_STRING:
            init_string(subject, ખાલી, ખાલી, 0, "");
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_INVALID:
        સ્થિતિ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    પાછા subject;
}

શૂન્ય lv_subject_delete(lv_subject_t * subject)
{
    જો(!subject) {
        પાછા;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#જો LV_USE_EXT_DATA
શૂન્ય lv_subject_set_external_data(lv_subject_t * subject, શૂન્ય * data, શૂન્ય (* free_cb)(શૂન્ય * data))
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

શૂન્ય lv_subject_set_int(lv_subject_t * subject, int32_t કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા);

    કિંમત = LV_CLAMP(subject->min_કિંમત.અંક, કિંમત, subject->max_કિંમત.અંક);

    subject->prev_કિંમત.અંક = subject->કિંમત.અંક;
    subject->કિંમત.અંક = કિંમત;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા 0);

    પાછા subject->કિંમત.અંક;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા 0);

    પાછા subject->prev_કિંમત.અંક;
}

શૂન્ય lv_subject_set_min_કિંમત_int(lv_subject_t * subject, int32_t min_કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા);

    subject->min_કિંમત.અંક = min_કિંમત;
}

શૂન્ય lv_subject_set_max_કિંમત_int(lv_subject_t * subject, int32_t max_કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા);

    subject->max_કિંમત.અંક = max_કિંમત;
}

#જો LV_USE_FLOAT

શૂન્ય lv_subject_set_float(lv_subject_t * subject, દશાંશ કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા);

    કિંમત = LV_CLAMP(subject->min_કિંમત.float_v, કિંમત, subject->max_કિંમત.float_v);

    subject->prev_કિંમત.float_v = subject->કિંમત.float_v;
    subject->કિંમત.float_v = કિંમત;
    lv_subject_notify_if_changed(subject);
}

દશાંશ lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા 0.0);

    પાછા subject->કિંમત.float_v;
}

દશાંશ lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા 0.0);

    પાછા subject->prev_કિંમત.float_v;
}

શૂન્ય lv_subject_set_min_કિંમત_float(lv_subject_t * subject, દશાંશ min_કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા);

    subject->min_કિંમત.float_v = min_કિંમત;
}

શૂન્ય lv_subject_set_max_કિંમત_float(lv_subject_t * subject, દશાંશ max_કિંમત)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા);

    subject->max_કિંમત.float_v = max_કિંમત;
}


#endif /*LV_USE_FLOAT*/

શૂન્ય lv_subject_set_string_buffer_static(lv_subject_t * subject, અક્ષર * buf, અક્ષર * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા);
    LV_CHECK_ARG(buf != ખાલી, પાછા);
    LV_CHECK_ARG(size > 0, પાછા);

    buf[0] = '\0';
    જો(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->કિંમત.pointer = buf;
    subject->prev_કિંમત.pointer = prev_buf;
}

શૂન્ય lv_subject_set_string(lv_subject_t * subject, અચલ અક્ષર * buf)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા);
    LV_CHECK_ARG(buf != ખાલી, પાછા);

    જો(subject->size == 0) પાછા;
    જો(subject->prev_કિંમત.pointer) {
        lv_strlcpy((અક્ષર *)subject->prev_કિંમત.pointer, subject->કિંમત.pointer, subject->size);
    }

    lv_strlcpy((અક્ષર *)subject->કિંમત.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

શૂન્ય lv_subject_snprintf(lv_subject_t * subject, અચલ અક્ષર * format, ...)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા);
    LV_CHECK_ARG(format != ખાલી, પાછા);

    જો(subject->size < 1U) પાછા;

    જો(subject->prev_કિંમત.pointer) {
        lv_strlcpy((અક્ષર *)subject->prev_કિંમત.pointer, subject->કિંમત.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    અચલ પૂર્ણાંક ret = lv_vsnprintf((અક્ષર *)subject->કિંમત.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

અચલ અક્ષર * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા ખાલી);

    પાછા subject->કિંમત.pointer;
}

અચલ અક્ષર * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા ખાલી);

    પાછા subject->prev_કિંમત.pointer;
}

શૂન્ય lv_subject_set_pointer(lv_subject_t * subject, શૂન્ય * ptr)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, પાછા);

    subject->prev_કિંમત.pointer = subject->કિંમત.pointer;
    subject->કિંમત.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

અચલ શૂન્ય * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, પાછા ખાલી);


    પાછા subject->કિંમત.pointer;
}

અચલ શૂન્ય * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, પાછા ખાલી);

    પાછા subject->prev_કિંમત.pointer;
}

શૂન્ય lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, પાછા);

    subject->prev_કિંમત.color = subject->કિંમત.color;
    subject->કિંમત.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, પાછા lv_color_black());

    પાછા subject->કિંમત.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, પાછા lv_color_black());

    પાછા subject->prev_કિંમત.color;
}

શૂન્ય lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(list != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, પાછા);

    માટે(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_સંદેશ(list[i] != ખાલી, પાછા, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, પાછા ખાલી);
    LV_CHECK_ARG(index >= 0 && index < subject->size, પાછા ખાલી);

    પાછા ((lv_subject_t **)(subject->કિંમત.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, શૂન્ય * user_data)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(observer_cb != ખાલી, પાછા ખાલી);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, ખાલી, user_data);
    જો(observer == ખાલી) પાછા ખાલી;

    observer->for_obj = 0;
    પાછા observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            શૂન્ય * user_data)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(observer_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, પાછા ખાલી);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    જો(observer == ખાલી) પાછા ખાલી;

    lv_memzero(observer, કદ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    જો(obj != ખાલી) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    પાછા observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, શૂન્ય * target,
                                                    શૂન્ય * user_data)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(observer_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, પાછા ખાલી);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    જો(observer == ખાલી) પાછા ખાલી;

    lv_memzero(observer, કદ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    પાછા observer;
}


શૂન્ય lv_observer_delete(lv_observer_t * observer)
{
    જો(observer == ખાલી) પાછા;

    જો(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, ખાલી, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    જો(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

શૂન્ય lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_ગણતરી(&obj->spec_attr->event_list) : 0);
    માટે(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        જો(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            જો(subject == ખાલી || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, ખાલી, subject);

}

શૂન્ય * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ખાલી, પાછા ખાલી);

    પાછા observer->target;
}

શૂન્ય lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ખાલી, પાછા);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    કરો {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            જો(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                જો(subject->notify_restart_query) તોડો;
                observer->notified = 1;
            }
        }
    } જ્યારે(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા ખાલી);

    lv_subject_increment_dsc_t * user_data = lv_malloc(કદ(lv_subject_increment_dsc_t));
    જો(user_data == ખાલી) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        પાછા ખાલી;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_કિંમત = INT32_MIN;
    user_data->max_કિંમત = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    પાછા user_data;
}

શૂન્ય lv_obj_set_subject_increment_event_min_કિંમત(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_કિંમત)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    LV_CHECK_ARG(dsc != ખાલી, પાછા);

    dsc->min_કિંમત = min_કિંમત;
    જો(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        જો(dsc->subject->કિંમત.અંક < min_કિંમત) {
            lv_subject_set_int(dsc->subject, min_કિંમત);
        }
    }
#જો LV_USE_FLOAT
    નહીંતર જો(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        જો(dsc->subject->કિંમત.float_v < (દશાંશ)min_કિંમત) {
            lv_subject_set_float(dsc->subject, (દશાંશ)min_કિંમત);
        }
    }
#endif
}

શૂન્ય lv_obj_set_subject_increment_event_max_કિંમત(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_કિંમત)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    LV_CHECK_ARG(dsc != ખાલી, પાછા);

    dsc->max_કિંમત = max_કિંમત;
    જો(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        જો(dsc->subject->કિંમત.અંક > max_કિંમત) {
            lv_subject_set_int(dsc->subject, max_કિંમત);
        }
    }
#જો LV_USE_FLOAT
    નહીંતર જો(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        જો(dsc->subject->કિંમત.float_v > (દશાંશ)max_કિંમત) {
            lv_subject_set_float(dsc->subject, (દશાંશ)max_કિંમત);
        }
    }
#endif
}

શૂન્ય lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    LV_CHECK_ARG(dsc != ખાલી, પાછા);

    dsc->rollover = rollover;
}

શૂન્ય lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

શૂન્ય lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t કિંમત)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા);

    subject_set_int_user_data_t * user_data = lv_malloc(કદ(subject_set_int_user_data_t));
    જો(user_data == ખાલી) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        પાછા;
    }

    user_data->subject = subject;
    user_data->કિંમત = કિંમત;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#જો LV_USE_FLOAT
શૂન્ય lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, દશાંશ કિંમત)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા);

    subject_set_float_user_data_t * user_data = lv_malloc(કદ(subject_set_float_user_data_t));
    જો(user_data == ખાલી) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        પાછા;
    }

    user_data->subject = subject;
    user_data->કિંમત = કિંમત;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

શૂન્ય lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         અચલ અક્ષર * કિંમત)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(subject != ખાલી, પાછા);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા);
    LV_CHECK_ARG(કિંમત != ખાલી, પાછા);

    subject_set_string_user_data_t * user_data = lv_malloc(કદ(subject_set_string_user_data_t));
    જો(user_data == ખાલી) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        પાછા;
    }

    user_data->subject = subject;
    user_data->કિંમત = lv_strdup(કિંમત);
    જો(user_data->કિંમત == ખાલી) {
        LV_ASSERT_MALLOC(user_data->કિંમત);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        પાછા;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_bool_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા ખાલી);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_bool_cb;
    set_bool_observer(observable, subject);
    પાછા observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_int_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, પાછા ખાલી);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_int_cb;
    set_int_observer(observable, subject);
    પાછા observable;
}

#જો LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_float_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, પાછા ખાલી);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_float_cb;
    set_float_observer(observable, subject);
    પાછા observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_string_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, પાછા ખાલી);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_string_cb;
    set_string_observer(observable, subject);
    પાછા observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_color_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, પાછા ખાલી);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_color_cb;
    set_color_observer(observable, subject);
    પાછા observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(set_pointer_cb != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, પાછા ખાલી);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, ખાલી);
    જો(observable == ખાલી) {
        LV_LOG_WARN("Couldn't add observer to subject");
        પાછા ખાલી;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (શૂન્ય (*)(શૂન્ય))set_pointer_cb;
    set_pointer_observer(observable, subject);
    પાછા observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG_FORMAT_સંદેશ(subject->type == LV_SUBJECT_TYPE_INT, પાછા ખાલી, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_કિંમત_changed_event_cb, LV_EVENT_કિંમત_CHANGED, subject);

    પાછા observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ખાલી, પાછા ખાલી);

    પાછા (lv_obj_t *)lv_observer_get_target(observer);
}

શૂન્ય * lv_observer_get_user_data(અચલ lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ખાલી, પાછા ખાલી);

    પાછા observer->user_data;
}

શૂન્ય lv_observer_set_user_data(lv_observer_t * observer, શૂન્ય * user_data)
{
    LV_CHECK_ARG(observer != ખાલી, પાછા);

    /* Release internally-owned data only when replacing it. */
    જો(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

સ્થિર શૂન્ય init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ખાલી);
    lv_memzero(subject, કદ(*subject));
    lv_ll_init(&(subject->subs_ll), કદ(lv_observer_t));
}
સ્થિર શૂન્ય init_int(lv_subject_t * subject, int32_t કિંમત)
{
    LV_ASSERT(subject != ખાલી);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->કિંમત.અંક = કિંમત;
    subject->prev_કિંમત.અંક = કિંમત;
    subject->min_કિંમત.અંક = INT32_MIN;
    subject->max_કિંમત.અંક = INT32_MAX;
}

#જો LV_USE_FLOAT

સ્થિર શૂન્ય init_float(lv_subject_t * subject, દશાંશ કિંમત)
{
    LV_ASSERT(subject != ખાલી);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->કિંમત.float_v = કિંમત;
    subject->prev_કિંમત.float_v = કિંમત;
    subject->min_કિંમત.float_v = -FLT_MAX;
    subject->max_કિંમત.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

સ્થિર શૂન્ય init_pointer(lv_subject_t * subject, શૂન્ય * કિંમત)
{
    LV_ASSERT(subject != ખાલી);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->કિંમત.pointer = કિંમત;
    subject->prev_કિંમત.pointer = કિંમત;
}

સ્થિર શૂન્ય init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ખાલી);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->કિંમત.color = color;
    subject->prev_કિંમત.color = color;
}

સ્થિર શૂન્ય init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(list != ખાલી || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

સ્થિર શૂન્ય add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(list != ખાલી || size == 0);
    subject->કિંમત.pointer = list;
    subject->size = (uint32_t)size;

    માટે(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

સ્થિર શૂન્ય init_string(lv_subject_t * subject, અક્ષર * buf, અક્ષર * prev_buf, size_t size, અચલ અક્ષર * કિંમત)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(buf != ખાલી || size == 0);

    જો(size > 0) {
        lv_strlcpy(buf, કિંમત, size);
        જો(prev_buf) lv_strlcpy(prev_buf, કિંમત, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->કિંમત.pointer = buf;
    subject->prev_કિંમત.pointer = prev_buf;
}

સ્થિર શૂન્ય deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ખાલી);
    જો(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    જ્યારે(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#જો LV_USE_EXT_DATA
    જો(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = ખાલી;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

સ્થિર શૂન્ય subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

સ્થિર શૂન્ય subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->કિંમત);
}


#જો LV_USE_FLOAT
સ્થિર શૂન્ય subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->કિંમત);
}
#endif

સ્થિર શૂન્ય subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->કિંમત);
}

સ્થિર શૂન્ય subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    જો(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_કિંમત = LV_MIN(user_data->max_કિંમત, user_data->subject->max_કિંમત.અંક);
        int32_t min_કિંમત = LV_MAX(user_data->min_કિંમત, user_data->subject->min_કિંમત.અંક);

        int32_t કિંમત = lv_subject_get_int(user_data->subject);
        કિંમત += user_data->step;

        જો(user_data->rollover) {
            જો(કિંમત > max_કિંમત) {
                કિંમત = min_કિંમત;
            }
            નહીંતર જો(કિંમત < min_કિંમત) {
                કિંમત = max_કિંમત;
            }
        }
        નહીંતર {
            કિંમત = LV_CLAMP(min_કિંમત, કિંમત, max_કિંમત);
        }

        lv_subject_set_int(user_data->subject, કિંમત);
    }
#જો LV_USE_FLOAT
    નહીંતર જો(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        દશાંશ max_કિંમત = LV_MIN((દશાંશ)user_data->max_કિંમત, user_data->subject->max_કિંમત.float_v);
        દશાંશ min_કિંમત = LV_MAX((દશાંશ)user_data->min_કિંમત, user_data->subject->min_કિંમત.float_v);


        દશાંશ કિંમત = lv_subject_get_float(user_data->subject);
        કિંમત += (દશાંશ)user_data->step;

        જો(user_data->rollover) {
            જો(કિંમત > max_કિંમત) {
                કિંમત = min_કિંમત;
            }
            નહીંતર જો(કિંમત < min_કિંમત) {
                કિંમત = max_કિંમત;
            }
        }
        નહીંતર {
            કિંમત = LV_CLAMP(min_કિંમત, કિંમત, max_કિંમત);
        }

        lv_subject_set_float(user_data->subject, કિંમત);
    }
#endif
}

સ્થિર શૂન્ય group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ખાલી);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

સ્થિર શૂન્ય unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

સ્થિર lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(obj != ખાલી);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(કદ(bound_state_t));
    જો(p == ખાલી) {
        LV_LOG_WARN("Out of memory");
        પાછા ખાલી;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    પાછા observable;
}

સ્થિર શૂન્ય obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ખાલી);
    LV_ASSERT(subject != ખાલી);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != ખાલી);

    જો(subject->કિંમત.અંક) {
        lv_obj_add_state(observer->target, p->state);
    }
    નહીંતર {
        lv_obj_remove_state(observer->target, p->state);
    }
}

સ્થિર શૂન્ય obj_કિંમત_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(obj != ખાલી);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

સ્થિર શૂન્ય lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    પસંદગી(subject->type) {
        સ્થિતિ LV_SUBJECT_TYPE_INT :
            જો(subject->કિંમત.અંક != subject->prev_કિંમત.અંક) {
                lv_subject_notify(subject);
            }
            તોડો;
#જો LV_USE_FLOAT
        સ્થિતિ LV_SUBJECT_TYPE_FLOAT :
            જો(subject->કિંમત.float_v != subject->prev_કિંમત.float_v) {
                lv_subject_notify(subject);
            }
            તોડો;
#endif
        સ્થિતિ LV_SUBJECT_TYPE_GROUP :
        સ્થિતિ LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_COLOR  :
            જો(!lv_color_eq(subject->કિંમત.color, subject->prev_કિંમત.color)) {
                lv_subject_notify(subject);
            }
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_STRING:
            જો(!subject->prev_કિંમત.pointer ||
               lv_strcmp(subject->કિંમત.pointer, subject->prev_કિંમત.pointer)) {
                lv_subject_notify(subject);
            }
            તોડો;
        સ્થિતિ LV_SUBJECT_TYPE_INVALID :
        સ્થિતિ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

સ્થિર શૂન્ય subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != ખાલી);
    lv_free((શૂન્ય *)user_data->કિંમત);
    lv_free(user_data);
}

સ્થિર શૂન્ય set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    જો(set_bool_cb) set_bool_cb(obj, subject->કિંમત.અંક);
}

સ્થિર શૂન્ય set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    જો(set_int_cb) set_int_cb(obj, subject->કિંમત.અંક);
}

#જો LV_USE_FLOAT
સ્થિર શૂન્ય set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    જો(set_float_cb) set_float_cb(obj, subject->કિંમત.float_v);
}
#endif /*LV_USE_FLOAT*/

સ્થિર શૂન્ય set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    જો(set_string_cb) set_string_cb(obj, subject->કિંમત.pointer);
}


સ્થિર શૂન્ય set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    જો(set_color_cb) set_color_cb(obj, subject->કિંમત.color);
}

સ્થિર શૂન્ય set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    જો(set_pointer_cb) set_pointer_cb(obj, subject->કિંમત.pointer);
}

સ્થિર શૂન્ય remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    માટે(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->કિંમત.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

સ્થિર શૂન્ય subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, શૂન્ય * user_data)
{
    LV_ASSERT(subject != ખાલી);
    LV_ASSERT(cb != ખાલી);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    જ્યારે(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        જો(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
