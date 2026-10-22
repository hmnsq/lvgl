/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_observer_private.h"
#если LV_USE_OBSERVER

#include "../lvgl_public.h"
#include "../core/lv_global.h"
#include "../core/lv_obj_private.h"
#include "../misc/lv_event_private.h"
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
typedef struct {
    lv_state_t state;
} bound_state_t;

typedef struct {
    lv_subject_t * subject;
    int32_t value;
} subject_set_int_user_data_t;

typedef struct {
    lv_subject_t * subject;
    float value;
} subject_set_float_user_data_t;

typedef struct {
    lv_subject_t * subject;
    const char * value;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void remove_observers_from_group_subject(lv_subject_t * subject);
static void add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
static void subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, void * user_data);
static void subject_toggle_cb(lv_event_t * e);
static void subject_set_int_cb(lv_event_t * e);
#если LV_USE_FLOAT
    static void subject_set_float_cb(lv_event_t * e);
#endif

static void subject_set_string_cb(lv_event_t * e);
static void subject_increment_cb(lv_event_t * e);

static void unsubscribe_on_delete_cb(lv_event_t * e);
static void group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
static lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

static void obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
static void obj_value_changed_event_cb(lv_event_t * e);

static void lv_subject_notify_if_changed(lv_subject_t * subject);

static void subject_set_string_free_user_data_event_cb(lv_event_t * e);

static void set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
static void set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
static void set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
static void set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
static void set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#если LV_USE_FLOAT
    static void set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    static void init_float(lv_subject_t * subject, float value);
#endif /*LV_USE_FLOAT*/

static void init_int(lv_subject_t * subject, int32_t value);
static void init_string(lv_subject_t * subject, char * buf, char * prev_buf, size_t size, const char * value);
static void init_pointer(lv_subject_t * subject, void * value);
static void init_color(lv_subject_t * subject, lv_color_t color);
static void init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
static void init_common(lv_subject_t * subject);

static void deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_subject_global_init(void)
{
    lv_ll_init(&subject_list, sizeof(lv_subject_t));
}
void lv_subject_global_deinit(void)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        если(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        если(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, вернуть NULL);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, вернуть NULL);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    если(!subject) {
        вернуть NULL;
    }

    init_common(subject);

    switch(type) {
        case LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            break;
        case LV_SUBJECT_TYPE_FLOAT:
#если LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            break;
        case LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, NULL);
            break;
        case LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            break;
        case LV_SUBJECT_TYPE_GROUP:
            init_group(subject, NULL, 0);
            break;
        case LV_SUBJECT_TYPE_STRING:
            init_string(subject, NULL, NULL, 0, "");
            break;
        case LV_SUBJECT_TYPE_INVALID:
        case LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    вернуть subject;
}

void lv_subject_delete(lv_subject_t * subject)
{
    если(!subject) {
        вернуть;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#если LV_USE_EXT_DATA
void lv_subject_set_external_data(lv_subject_t * subject, void * data, void (* free_cb)(void * data))
{
    LV_CHECK_ARG(subject != NULL, вернуть);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

void lv_subject_set_int(lv_subject_t * subject, int32_t value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть);

    value = LV_CLAMP(subject->min_value.num, value, subject->max_value.num);

    subject->prev_value.num = subject->value.num;
    subject->value.num = value;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть 0);

    вернуть subject->value.num;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть 0);

    вернуть subject->prev_value.num;
}

void lv_subject_set_min_value_int(lv_subject_t * subject, int32_t min_value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть);

    subject->min_value.num = min_value;
}

void lv_subject_set_max_value_int(lv_subject_t * subject, int32_t max_value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть);

    subject->max_value.num = max_value;
}

#если LV_USE_FLOAT

void lv_subject_set_float(lv_subject_t * subject, float value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть);

    value = LV_CLAMP(subject->min_value.float_v, value, subject->max_value.float_v);

    subject->prev_value.float_v = subject->value.float_v;
    subject->value.float_v = value;
    lv_subject_notify_if_changed(subject);
}

float lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть 0.0);

    вернуть subject->value.float_v;
}

float lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть 0.0);

    вернуть subject->prev_value.float_v;
}

void lv_subject_set_min_value_float(lv_subject_t * subject, float min_value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть);

    subject->min_value.float_v = min_value;
}

void lv_subject_set_max_value_float(lv_subject_t * subject, float max_value)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть);

    subject->max_value.float_v = max_value;
}


#endif /*LV_USE_FLOAT*/

void lv_subject_set_string_buffer_static(lv_subject_t * subject, char * buf, char * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть);
    LV_CHECK_ARG(buf != NULL, вернуть);
    LV_CHECK_ARG(size > 0, вернуть);

    buf[0] = '\0';
    если(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->value.pointer = buf;
    subject->prev_value.pointer = prev_buf;
}

void lv_subject_set_string(lv_subject_t * subject, const char * buf)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть);
    LV_CHECK_ARG(buf != NULL, вернуть);

    если(subject->size == 0) вернуть;
    если(subject->prev_value.pointer) {
        lv_strlcpy((char *)subject->prev_value.pointer, subject->value.pointer, subject->size);
    }

    lv_strlcpy((char *)subject->value.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

void lv_subject_snprintf(lv_subject_t * subject, const char * format, ...)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть);
    LV_CHECK_ARG(format != NULL, вернуть);

    если(subject->size < 1U) вернуть;

    если(subject->prev_value.pointer) {
        lv_strlcpy((char *)subject->prev_value.pointer, subject->value.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    const целое ret = lv_vsnprintf((char *)subject->value.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

const char * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть NULL);

    вернуть subject->value.pointer;
}

const char * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть NULL);

    вернуть subject->prev_value.pointer;
}

void lv_subject_set_pointer(lv_subject_t * subject, void * ptr)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, вернуть);

    subject->prev_value.pointer = subject->value.pointer;
    subject->value.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

const void * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, вернуть NULL);


    вернуть subject->value.pointer;
}

const void * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, вернуть NULL);

    вернуть subject->prev_value.pointer;
}

void lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, вернуть);

    subject->prev_value.color = subject->value.color;
    subject->value.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, вернуть lv_color_black());

    вернуть subject->value.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, вернуть lv_color_black());

    вернуть subject->prev_value.color;
}

void lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(list != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, вернуть);

    для(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_MSG(list[i] != NULL, вернуть, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, вернуть NULL);
    LV_CHECK_ARG(index >= 0 && index < subject->size, вернуть NULL);

    вернуть ((lv_subject_t **)(subject->value.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, void * user_data)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(observer_cb != NULL, вернуть NULL);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, NULL, user_data);
    если(observer == NULL) вернуть NULL;

    observer->for_obj = 0;
    вернуть observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            void * user_data)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(observer_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, вернуть NULL);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    если(observer == NULL) вернуть NULL;

    lv_memzero(observer, sizeof(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    если(obj != NULL) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    вернуть observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, void * target,
                                                    void * user_data)
{
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(observer_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, вернуть NULL);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    если(observer == NULL) вернуть NULL;

    lv_memzero(observer, sizeof(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    вернуть observer;
}


void lv_observer_delete(lv_observer_t * observer)
{
    если(observer == NULL) вернуть;

    если(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, NULL, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    если(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

void lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != NULL, вернуть);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_count(&obj->spec_attr->event_list) : 0);
    для(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        если(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            если(subject == NULL || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, NULL, subject);

}

void * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != NULL, вернуть NULL);

    вернуть observer->target;
}

void lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != NULL, вернуть);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    do {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            если(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                если(subject->notify_restart_query) break;
                observer->notified = 1;
            }
        }
    } пока(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть NULL);

    lv_subject_increment_dsc_t * user_data = lv_malloc(sizeof(lv_subject_increment_dsc_t));
    если(user_data == NULL) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        вернуть NULL;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_value = INT32_MIN;
    user_data->max_value = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    вернуть user_data;
}

void lv_obj_set_subject_increment_event_min_value(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_value)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, вернуть);
    LV_CHECK_ARG(dsc != NULL, вернуть);

    dsc->min_value = min_value;
    если(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        если(dsc->subject->value.num < min_value) {
            lv_subject_set_int(dsc->subject, min_value);
        }
    }
#если LV_USE_FLOAT
    иначе если(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        если(dsc->subject->value.float_v < (float)min_value) {
            lv_subject_set_float(dsc->subject, (float)min_value);
        }
    }
#endif
}

void lv_obj_set_subject_increment_event_max_value(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_value)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, вернуть);
    LV_CHECK_ARG(dsc != NULL, вернуть);

    dsc->max_value = max_value;
    если(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        если(dsc->subject->value.num > max_value) {
            lv_subject_set_int(dsc->subject, max_value);
        }
    }
#если LV_USE_FLOAT
    иначе если(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        если(dsc->subject->value.float_v > (float)max_value) {
            lv_subject_set_float(dsc->subject, (float)max_value);
        }
    }
#endif
}

void lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, вернуть);
    LV_CHECK_ARG(dsc != NULL, вернуть);

    dsc->rollover = rollover;
}

void lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != NULL, вернуть);
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

void lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t value)
{
    LV_CHECK_ARG(obj != NULL, вернуть);
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть);

    subject_set_int_user_data_t * user_data = lv_malloc(sizeof(subject_set_int_user_data_t));
    если(user_data == NULL) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        вернуть;
    }

    user_data->subject = subject;
    user_data->value = value;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#если LV_USE_FLOAT
void lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, float value)
{
    LV_CHECK_ARG(obj != NULL, вернуть);
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть);

    subject_set_float_user_data_t * user_data = lv_malloc(sizeof(subject_set_float_user_data_t));
    если(user_data == NULL) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        вернуть;
    }

    user_data->subject = subject;
    user_data->value = value;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

void lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         const char * value)
{
    LV_CHECK_ARG(obj != NULL, вернуть);
    LV_CHECK_ARG(subject != NULL, вернуть);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть);
    LV_CHECK_ARG(value != NULL, вернуть);

    subject_set_string_user_data_t * user_data = lv_malloc(sizeof(subject_set_string_user_data_t));
    если(user_data == NULL) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        вернуть;
    }

    user_data->subject = subject;
    user_data->value = lv_strdup(value);
    если(user_data->value == NULL) {
        LV_ASSERT_MALLOC(user_data->value);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        вернуть;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_bool_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть NULL);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_bool_cb;
    set_bool_observer(observable, subject);
    вернуть observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_int_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, вернуть NULL);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_int_cb;
    set_int_observer(observable, subject);
    вернуть observable;
}

#если LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_float_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, вернуть NULL);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_float_cb;
    set_float_observer(observable, subject);
    вернуть observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_string_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, вернуть NULL);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_string_cb;
    set_string_observer(observable, subject);
    вернуть observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_color_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, вернуть NULL);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_color_cb;
    set_color_observer(observable, subject);
    вернуть observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG(set_pointer_cb != NULL, вернуть NULL);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, вернуть NULL);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, NULL);
    если(observable == NULL) {
        LV_LOG_WARN("Couldn't add observer to subject");
        вернуть NULL;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (void (*)(void))set_pointer_cb;
    set_pointer_observer(observable, subject);
    вернуть observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != NULL, вернуть NULL);
    LV_CHECK_ARG(subject != NULL, вернуть NULL);
    LV_CHECK_ARG_FORMAT_MSG(subject->type == LV_SUBJECT_TYPE_INT, вернуть NULL, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);

    вернуть observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != NULL, вернуть NULL);

    вернуть (lv_obj_t *)lv_observer_get_target(observer);
}

void * lv_observer_get_user_data(const lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != NULL, вернуть NULL);

    вернуть observer->user_data;
}

void lv_observer_set_user_data(lv_observer_t * observer, void * user_data)
{
    LV_CHECK_ARG(observer != NULL, вернуть);

    /* Release internally-owned data only when replacing it. */
    если(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != NULL);
    lv_memzero(subject, sizeof(*subject));
    lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}
static void init_int(lv_subject_t * subject, int32_t value)
{
    LV_ASSERT(subject != NULL);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->value.num = value;
    subject->prev_value.num = value;
    subject->min_value.num = INT32_MIN;
    subject->max_value.num = INT32_MAX;
}

#если LV_USE_FLOAT

static void init_float(lv_subject_t * subject, float value)
{
    LV_ASSERT(subject != NULL);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->value.float_v = value;
    subject->prev_value.float_v = value;
    subject->min_value.float_v = -FLT_MAX;
    subject->max_value.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

static void init_pointer(lv_subject_t * subject, void * value)
{
    LV_ASSERT(subject != NULL);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->value.pointer = value;
    subject->prev_value.pointer = value;
}

static void init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != NULL);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->value.color = color;
    subject->prev_value.color = color;
}

static void init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(list != NULL || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

static void add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(list != NULL || size == 0);
    subject->value.pointer = list;
    subject->size = (uint32_t)size;

    для(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

static void init_string(lv_subject_t * subject, char * buf, char * prev_buf, size_t size, const char * value)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(buf != NULL || size == 0);

    если(size > 0) {
        lv_strlcpy(buf, value, size);
        если(prev_buf) lv_strlcpy(prev_buf, value, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->value.pointer = buf;
    subject->prev_value.pointer = prev_buf;
}

static void deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != NULL);
    если(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    пока(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#если LV_USE_EXT_DATA
    если(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = NULL;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

static void subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

static void subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->value);
}


#если LV_USE_FLOAT
static void subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->value);
}
#endif

static void subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->value);
}

static void subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    если(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_value = LV_MIN(user_data->max_value, user_data->subject->max_value.num);
        int32_t min_value = LV_MAX(user_data->min_value, user_data->subject->min_value.num);

        int32_t value = lv_subject_get_int(user_data->subject);
        value += user_data->step;

        если(user_data->rollover) {
            если(value > max_value) {
                value = min_value;
            }
            иначе если(value < min_value) {
                value = max_value;
            }
        }
        иначе {
            value = LV_CLAMP(min_value, value, max_value);
        }

        lv_subject_set_int(user_data->subject, value);
    }
#если LV_USE_FLOAT
    иначе если(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        float max_value = LV_MIN((float)user_data->max_value, user_data->subject->max_value.float_v);
        float min_value = LV_MAX((float)user_data->min_value, user_data->subject->min_value.float_v);


        float value = lv_subject_get_float(user_data->subject);
        value += (float)user_data->step;

        если(user_data->rollover) {
            если(value > max_value) {
                value = min_value;
            }
            иначе если(value < min_value) {
                value = max_value;
            }
        }
        иначе {
            value = LV_CLAMP(min_value, value, max_value);
        }

        lv_subject_set_float(user_data->subject, value);
    }
#endif
}

static void group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != NULL);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

static void unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

static lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(obj != NULL);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(sizeof(bound_state_t));
    если(p == NULL) {
        LV_LOG_WARN("Out of memory");
        вернуть NULL;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    вернуть observable;
}

static void obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != NULL);
    LV_ASSERT(subject != NULL);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != NULL);

    если(subject->value.num) {
        lv_obj_add_state(observer->target, p->state);
    }
    иначе {
        lv_obj_remove_state(observer->target, p->state);
    }
}

static void obj_value_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != NULL);
    LV_ASSERT(obj != NULL);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

static void lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    switch(subject->type) {
        case LV_SUBJECT_TYPE_INT :
            если(subject->value.num != subject->prev_value.num) {
                lv_subject_notify(subject);
            }
            break;
#если LV_USE_FLOAT
        case LV_SUBJECT_TYPE_FLOAT :
            если(subject->value.float_v != subject->prev_value.float_v) {
                lv_subject_notify(subject);
            }
            break;
#endif
        case LV_SUBJECT_TYPE_GROUP :
        case LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            break;
        case LV_SUBJECT_TYPE_COLOR  :
            если(!lv_color_eq(subject->value.color, subject->prev_value.color)) {
                lv_subject_notify(subject);
            }
            break;
        case LV_SUBJECT_TYPE_STRING:
            если(!subject->prev_value.pointer ||
               lv_strcmp(subject->value.pointer, subject->prev_value.pointer)) {
                lv_subject_notify(subject);
            }
            break;
        case LV_SUBJECT_TYPE_INVALID :
        case LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

static void subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != NULL);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != NULL);
    lv_free((void *)user_data->value);
    lv_free(user_data);
}

static void set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    если(set_bool_cb) set_bool_cb(obj, subject->value.num);
}

static void set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    если(set_int_cb) set_int_cb(obj, subject->value.num);
}

#если LV_USE_FLOAT
static void set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    если(set_float_cb) set_float_cb(obj, subject->value.float_v);
}
#endif /*LV_USE_FLOAT*/

static void set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    если(set_string_cb) set_string_cb(obj, subject->value.pointer);
}


static void set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    если(set_color_cb) set_color_cb(obj, subject->value.color);
}

static void set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    если(set_pointer_cb) set_pointer_cb(obj, subject->value.pointer);
}

static void remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    для(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->value.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

static void subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, void * user_data)
{
    LV_ASSERT(subject != NULL);
    LV_ASSERT(cb != NULL);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    пока(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        если(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
