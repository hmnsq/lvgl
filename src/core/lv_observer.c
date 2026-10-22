/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "lv_observer_private.h"
#जर LV_USE_OBSERVER

#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "../core/lv_obj_private.h"
#अन्तर्भाव "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#define subject_list LV_GLOBAL_DEFAULT()->subject_ll

#ifndef भग्न_उच्च
    #define भग्न_उच्च 3.402823466e+38F
#endif

/**********************
 *      TYPEDEFS
 **********************/
प्रकारव्याख्या संरचना {
    lv_state_t state;
} bound_state_t;

प्रकारव्याख्या संरचना {
    lv_subject_t * subject;
    int32_t मूल्य;
} subject_set_int_user_data_t;

प्रकारव्याख्या संरचना {
    lv_subject_t * subject;
    दशांश मूल्य;
} subject_set_float_user_data_t;

प्रकारव्याख्या संरचना {
    lv_subject_t * subject;
    स्थिरांक अक्षर * मूल्य;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर शून्य remove_observers_from_group_subject(lv_subject_t * subject);
स्थिर शून्य add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार size);
स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_data);
स्थिर शून्य subject_toggle_cb(lv_event_t * e);
स्थिर शून्य subject_set_int_cb(lv_event_t * e);
#जर LV_USE_FLOAT
    स्थिर शून्य subject_set_float_cb(lv_event_t * e);
#endif

स्थिर शून्य subject_set_string_cb(lv_event_t * e);
स्थिर शून्य subject_increment_cb(lv_event_t * e);

स्थिर शून्य unsubscribe_on_delete_cb(lv_event_t * e);
स्थिर शून्य group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य obj_मूल्य_changed_event_cb(lv_event_t * e);

स्थिर शून्य lv_subject_notify_if_changed(lv_subject_t * subject);

स्थिर शून्य subject_set_string_free_user_data_event_cb(lv_event_t * e);

स्थिर शून्य set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#जर LV_USE_FLOAT
    स्थिर शून्य set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    स्थिर शून्य init_float(lv_subject_t * subject, दशांश मूल्य);
#endif /*LV_USE_FLOAT*/

स्थिर शून्य init_int(lv_subject_t * subject, int32_t मूल्य);
स्थिर शून्य init_string(lv_subject_t * subject, अक्षर * buf, अक्षर * prev_buf, माप_प्रकार size, स्थिरांक अक्षर * मूल्य);
स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मूल्य);
स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color);
स्थिर शून्य init_group(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार size);
स्थिर शून्य init_common(lv_subject_t * subject);

स्थिर शून्य deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

शून्य lv_subject_global_init(शून्य)
{
    lv_ll_init(&subject_list, आकार(lv_subject_t));
}
शून्य lv_subject_global_deinit(शून्य)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        जर(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        जर(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, परत रिक्त);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, परत रिक्त);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    जर(!subject) {
        परत रिक्त;
    }

    init_common(subject);

    निवड(type) {
        स्थिती LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            थांबा;
        स्थिती LV_SUBJECT_TYPE_FLOAT:
#जर LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            थांबा;
        स्थिती LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, रिक्त);
            थांबा;
        स्थिती LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            थांबा;
        स्थिती LV_SUBJECT_TYPE_GROUP:
            init_group(subject, रिक्त, 0);
            थांबा;
        स्थिती LV_SUBJECT_TYPE_STRING:
            init_string(subject, रिक्त, रिक्त, 0, "");
            थांबा;
        स्थिती LV_SUBJECT_TYPE_INVALID:
        स्थिती LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    परत subject;
}

शून्य lv_subject_delete(lv_subject_t * subject)
{
    जर(!subject) {
        परत;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#जर LV_USE_EXT_DATA
शून्य lv_subject_set_external_data(lv_subject_t * subject, शून्य * data, शून्य (* free_cb)(शून्य * data))
{
    LV_CHECK_ARG(subject != रिक्त, परत);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

शून्य lv_subject_set_int(lv_subject_t * subject, int32_t मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत);

    मूल्य = LV_CLAMP(subject->min_मूल्य.अंक, मूल्य, subject->max_मूल्य.अंक);

    subject->prev_मूल्य.अंक = subject->मूल्य.अंक;
    subject->मूल्य.अंक = मूल्य;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत 0);

    परत subject->मूल्य.अंक;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत 0);

    परत subject->prev_मूल्य.अंक;
}

शून्य lv_subject_set_min_मूल्य_int(lv_subject_t * subject, int32_t min_मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत);

    subject->min_मूल्य.अंक = min_मूल्य;
}

शून्य lv_subject_set_max_मूल्य_int(lv_subject_t * subject, int32_t max_मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत);

    subject->max_मूल्य.अंक = max_मूल्य;
}

#जर LV_USE_FLOAT

शून्य lv_subject_set_float(lv_subject_t * subject, दशांश मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत);

    मूल्य = LV_CLAMP(subject->min_मूल्य.float_v, मूल्य, subject->max_मूल्य.float_v);

    subject->prev_मूल्य.float_v = subject->मूल्य.float_v;
    subject->मूल्य.float_v = मूल्य;
    lv_subject_notify_if_changed(subject);
}

दशांश lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत 0.0);

    परत subject->मूल्य.float_v;
}

दशांश lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत 0.0);

    परत subject->prev_मूल्य.float_v;
}

शून्य lv_subject_set_min_मूल्य_float(lv_subject_t * subject, दशांश min_मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत);

    subject->min_मूल्य.float_v = min_मूल्य;
}

शून्य lv_subject_set_max_मूल्य_float(lv_subject_t * subject, दशांश max_मूल्य)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत);

    subject->max_मूल्य.float_v = max_मूल्य;
}


#endif /*LV_USE_FLOAT*/

शून्य lv_subject_set_string_buffer_static(lv_subject_t * subject, अक्षर * buf, अक्षर * prev_buf, माप_प्रकार size)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत);
    LV_CHECK_ARG(buf != रिक्त, परत);
    LV_CHECK_ARG(size > 0, परत);

    buf[0] = '\0';
    जर(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->मूल्य.pointer = buf;
    subject->prev_मूल्य.pointer = prev_buf;
}

शून्य lv_subject_set_string(lv_subject_t * subject, स्थिरांक अक्षर * buf)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत);
    LV_CHECK_ARG(buf != रिक्त, परत);

    जर(subject->size == 0) परत;
    जर(subject->prev_मूल्य.pointer) {
        lv_strlcpy((अक्षर *)subject->prev_मूल्य.pointer, subject->मूल्य.pointer, subject->size);
    }

    lv_strlcpy((अक्षर *)subject->मूल्य.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

शून्य lv_subject_snprintf(lv_subject_t * subject, स्थिरांक अक्षर * format, ...)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत);
    LV_CHECK_ARG(format != रिक्त, परत);

    जर(subject->size < 1U) परत;

    जर(subject->prev_मूल्य.pointer) {
        lv_strlcpy((अक्षर *)subject->prev_मूल्य.pointer, subject->मूल्य.pointer, subject->size);
    }

    बहु_सूची va;
    बहु_शुरू(va, format);
    स्थिरांक पूर्णांक ret = lv_vsnprintf((अक्षर *)subject->मूल्य.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    बहु_पूर्ण(va);

    lv_subject_notify_if_changed(subject);
}

स्थिरांक अक्षर * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत रिक्त);

    परत subject->मूल्य.pointer;
}

स्थिरांक अक्षर * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत रिक्त);

    परत subject->prev_मूल्य.pointer;
}

शून्य lv_subject_set_pointer(lv_subject_t * subject, शून्य * ptr)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, परत);

    subject->prev_मूल्य.pointer = subject->मूल्य.pointer;
    subject->मूल्य.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

स्थिरांक शून्य * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, परत रिक्त);


    परत subject->मूल्य.pointer;
}

स्थिरांक शून्य * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, परत रिक्त);

    परत subject->prev_मूल्य.pointer;
}

शून्य lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, परत);

    subject->prev_मूल्य.color = subject->मूल्य.color;
    subject->मूल्य.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, परत lv_color_black());

    परत subject->मूल्य.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, परत lv_color_black());

    परत subject->prev_मूल्य.color;
}

शून्य lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(list != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, परत);

    साठी(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_संदेश(list[i] != रिक्त, परत, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, परत रिक्त);
    LV_CHECK_ARG(index >= 0 && index < subject->size, परत रिक्त);

    परत ((lv_subject_t **)(subject->मूल्य.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, परत रिक्त);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, रिक्त, user_data);
    जर(observer == रिक्त) परत रिक्त;

    observer->for_obj = 0;
    परत observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, परत रिक्त);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    जर(observer == रिक्त) परत रिक्त;

    lv_memzero(observer, आकार(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    जर(obj != रिक्त) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    परत observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * target,
                                                    शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, परत रिक्त);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    जर(observer == रिक्त) परत रिक्त;

    lv_memzero(observer, आकार(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    परत observer;
}


शून्य lv_observer_delete(lv_observer_t * observer)
{
    जर(observer == रिक्त) परत;

    जर(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, रिक्त, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    जर(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

शून्य lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_मोजणी(&obj->spec_attr->event_list) : 0);
    साठी(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        जर(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            जर(subject == रिक्त || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, रिक्त, subject);

}

शून्य * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, परत रिक्त);

    परत observer->target;
}

शून्य lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, परत);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    करा {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            जर(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                जर(subject->notify_restart_query) थांबा;
                observer->notified = 1;
            }
        }
    } पर्यंत(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, परत रिक्त);

    lv_subject_increment_dsc_t * user_data = lv_malloc(आकार(lv_subject_increment_dsc_t));
    जर(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        परत रिक्त;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_मूल्य = INT32_MIN;
    user_data->max_मूल्य = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    परत user_data;
}

शून्य lv_obj_set_subject_increment_event_min_मूल्य(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_मूल्य)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    LV_CHECK_ARG(dsc != रिक्त, परत);

    dsc->min_मूल्य = min_मूल्य;
    जर(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        जर(dsc->subject->मूल्य.अंक < min_मूल्य) {
            lv_subject_set_int(dsc->subject, min_मूल्य);
        }
    }
#जर LV_USE_FLOAT
    अन्यथा जर(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        जर(dsc->subject->मूल्य.float_v < (दशांश)min_मूल्य) {
            lv_subject_set_float(dsc->subject, (दशांश)min_मूल्य);
        }
    }
#endif
}

शून्य lv_obj_set_subject_increment_event_max_मूल्य(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_मूल्य)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    LV_CHECK_ARG(dsc != रिक्त, परत);

    dsc->max_मूल्य = max_मूल्य;
    जर(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        जर(dsc->subject->मूल्य.अंक > max_मूल्य) {
            lv_subject_set_int(dsc->subject, max_मूल्य);
        }
    }
#जर LV_USE_FLOAT
    अन्यथा जर(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        जर(dsc->subject->मूल्य.float_v > (दशांश)max_मूल्य) {
            lv_subject_set_float(dsc->subject, (दशांश)max_मूल्य);
        }
    }
#endif
}

शून्य lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    LV_CHECK_ARG(dsc != रिक्त, परत);

    dsc->rollover = rollover;
}

शून्य lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

शून्य lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत);

    subject_set_int_user_data_t * user_data = lv_malloc(आकार(subject_set_int_user_data_t));
    जर(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        परत;
    }

    user_data->subject = subject;
    user_data->मूल्य = मूल्य;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#जर LV_USE_FLOAT
शून्य lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, दशांश मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत);

    subject_set_float_user_data_t * user_data = lv_malloc(आकार(subject_set_float_user_data_t));
    जर(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        परत;
    }

    user_data->subject = subject;
    user_data->मूल्य = मूल्य;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

शून्य lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         स्थिरांक अक्षर * मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(subject != रिक्त, परत);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत);
    LV_CHECK_ARG(मूल्य != रिक्त, परत);

    subject_set_string_user_data_t * user_data = lv_malloc(आकार(subject_set_string_user_data_t));
    जर(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        परत;
    }

    user_data->subject = subject;
    user_data->मूल्य = lv_strdup(मूल्य);
    जर(user_data->मूल्य == रिक्त) {
        LV_ASSERT_MALLOC(user_data->मूल्य);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        परत;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_bool_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_bool_cb;
    set_bool_observer(observable, subject);
    परत observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_int_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, परत रिक्त);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_int_cb;
    set_int_observer(observable, subject);
    परत observable;
}

#जर LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_float_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, परत रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_float_cb;
    set_float_observer(observable, subject);
    परत observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_string_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, परत रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_string_cb;
    set_string_observer(observable, subject);
    परत observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_color_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, परत रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_color_cb;
    set_color_observer(observable, subject);
    परत observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG(set_pointer_cb != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, परत रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, रिक्त);
    जर(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        परत रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_pointer_cb;
    set_pointer_observer(observable, subject);
    परत observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);
    LV_CHECK_ARG_FORMAT_संदेश(subject->type == LV_SUBJECT_TYPE_INT, परत रिक्त, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_मूल्य_changed_event_cb, LV_EVENT_मूल्य_CHANGED, subject);

    परत observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, परत रिक्त);

    परत (lv_obj_t *)lv_observer_get_target(observer);
}

शून्य * lv_observer_get_user_data(स्थिरांक lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, परत रिक्त);

    परत observer->user_data;
}

शून्य lv_observer_set_user_data(lv_observer_t * observer, शून्य * user_data)
{
    LV_CHECK_ARG(observer != रिक्त, परत);

    /* Release internally-owned data only when replacing it. */
    जर(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर शून्य init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    lv_memzero(subject, आकार(*subject));
    lv_ll_init(&(subject->subs_ll), आकार(lv_observer_t));
}
स्थिर शून्य init_int(lv_subject_t * subject, int32_t मूल्य)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->मूल्य.अंक = मूल्य;
    subject->prev_मूल्य.अंक = मूल्य;
    subject->min_मूल्य.अंक = INT32_MIN;
    subject->max_मूल्य.अंक = INT32_MAX;
}

#जर LV_USE_FLOAT

स्थिर शून्य init_float(lv_subject_t * subject, दशांश मूल्य)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->मूल्य.float_v = मूल्य;
    subject->prev_मूल्य.float_v = मूल्य;
    subject->min_मूल्य.float_v = -भग्न_उच्च;
    subject->max_मूल्य.float_v = भग्न_उच्च;
}

#endif /*LV_USE_FLOAT*/

स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मूल्य)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->मूल्य.pointer = मूल्य;
    subject->prev_मूल्य.pointer = मूल्य;
}

स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->मूल्य.color = color;
    subject->prev_मूल्य.color = color;
}

स्थिर शून्य init_group(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार size)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

स्थिर शून्य add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार size)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || size == 0);
    subject->मूल्य.pointer = list;
    subject->size = (uint32_t)size;

    साठी(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

स्थिर शून्य init_string(lv_subject_t * subject, अक्षर * buf, अक्षर * prev_buf, माप_प्रकार size, स्थिरांक अक्षर * मूल्य)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(buf != रिक्त || size == 0);

    जर(size > 0) {
        lv_strlcpy(buf, मूल्य, size);
        जर(prev_buf) lv_strlcpy(prev_buf, मूल्य, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->मूल्य.pointer = buf;
    subject->prev_मूल्य.pointer = prev_buf;
}

स्थिर शून्य deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    जर(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    पर्यंत(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#जर LV_USE_EXT_DATA
    जर(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = रिक्त;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

स्थिर शून्य subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

स्थिर शून्य subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->मूल्य);
}


#जर LV_USE_FLOAT
स्थिर शून्य subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->मूल्य);
}
#endif

स्थिर शून्य subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->मूल्य);
}

स्थिर शून्य subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    जर(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_मूल्य = LV_MIN(user_data->max_मूल्य, user_data->subject->max_मूल्य.अंक);
        int32_t min_मूल्य = LV_MAX(user_data->min_मूल्य, user_data->subject->min_मूल्य.अंक);

        int32_t मूल्य = lv_subject_get_int(user_data->subject);
        मूल्य += user_data->step;

        जर(user_data->rollover) {
            जर(मूल्य > max_मूल्य) {
                मूल्य = min_मूल्य;
            }
            अन्यथा जर(मूल्य < min_मूल्य) {
                मूल्य = max_मूल्य;
            }
        }
        अन्यथा {
            मूल्य = LV_CLAMP(min_मूल्य, मूल्य, max_मूल्य);
        }

        lv_subject_set_int(user_data->subject, मूल्य);
    }
#जर LV_USE_FLOAT
    अन्यथा जर(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        दशांश max_मूल्य = LV_MIN((दशांश)user_data->max_मूल्य, user_data->subject->max_मूल्य.float_v);
        दशांश min_मूल्य = LV_MAX((दशांश)user_data->min_मूल्य, user_data->subject->min_मूल्य.float_v);


        दशांश मूल्य = lv_subject_get_float(user_data->subject);
        मूल्य += (दशांश)user_data->step;

        जर(user_data->rollover) {
            जर(मूल्य > max_मूल्य) {
                मूल्य = min_मूल्य;
            }
            अन्यथा जर(मूल्य < min_मूल्य) {
                मूल्य = max_मूल्य;
            }
        }
        अन्यथा {
            मूल्य = LV_CLAMP(min_मूल्य, मूल्य, max_मूल्य);
        }

        lv_subject_set_float(user_data->subject, मूल्य);
    }
#endif
}

स्थिर शून्य group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != रिक्त);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

स्थिर शून्य unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

स्थिर lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(obj != रिक्त);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(आकार(bound_state_t));
    जर(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        परत रिक्त;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    परत observable;
}

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != रिक्त);
    LV_ASSERT(subject != रिक्त);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != रिक्त);

    जर(subject->मूल्य.अंक) {
        lv_obj_add_state(observer->target, p->state);
    }
    अन्यथा {
        lv_obj_remove_state(observer->target, p->state);
    }
}

स्थिर शून्य obj_मूल्य_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(obj != रिक्त);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

स्थिर शून्य lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    निवड(subject->type) {
        स्थिती LV_SUBJECT_TYPE_INT :
            जर(subject->मूल्य.अंक != subject->prev_मूल्य.अंक) {
                lv_subject_notify(subject);
            }
            थांबा;
#जर LV_USE_FLOAT
        स्थिती LV_SUBJECT_TYPE_FLOAT :
            जर(subject->मूल्य.float_v != subject->prev_मूल्य.float_v) {
                lv_subject_notify(subject);
            }
            थांबा;
#endif
        स्थिती LV_SUBJECT_TYPE_GROUP :
        स्थिती LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            थांबा;
        स्थिती LV_SUBJECT_TYPE_COLOR  :
            जर(!lv_color_eq(subject->मूल्य.color, subject->prev_मूल्य.color)) {
                lv_subject_notify(subject);
            }
            थांबा;
        स्थिती LV_SUBJECT_TYPE_STRING:
            जर(!subject->prev_मूल्य.pointer ||
               lv_strcmp(subject->मूल्य.pointer, subject->prev_मूल्य.pointer)) {
                lv_subject_notify(subject);
            }
            थांबा;
        स्थिती LV_SUBJECT_TYPE_INVALID :
        स्थिती LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

स्थिर शून्य subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != रिक्त);
    lv_free((शून्य *)user_data->मूल्य);
    lv_free(user_data);
}

स्थिर शून्य set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    जर(set_bool_cb) set_bool_cb(obj, subject->मूल्य.अंक);
}

स्थिर शून्य set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    जर(set_int_cb) set_int_cb(obj, subject->मूल्य.अंक);
}

#जर LV_USE_FLOAT
स्थिर शून्य set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    जर(set_float_cb) set_float_cb(obj, subject->मूल्य.float_v);
}
#endif /*LV_USE_FLOAT*/

स्थिर शून्य set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    जर(set_string_cb) set_string_cb(obj, subject->मूल्य.pointer);
}


स्थिर शून्य set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    जर(set_color_cb) set_color_cb(obj, subject->मूल्य.color);
}

स्थिर शून्य set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    जर(set_pointer_cb) set_pointer_cb(obj, subject->मूल्य.pointer);
}

स्थिर शून्य remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    साठी(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->मूल्य.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_data)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(cb != रिक्त);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    पर्यंत(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        जर(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
