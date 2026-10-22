/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "lv_observer_private.h"
#जइ LV_USE_OBSERVER

#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "../core/lv_obj_private.h"
#अन्तर्भाव "../misc/lv_event_private.h"
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
प्रकारनिर्देश संरचना {
    lv_state_t state;
} bound_state_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    int32_t मानम्;
} subject_set_int_user_data_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    भिन्न मानम्;
} subject_set_float_user_data_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    ध्रुव वर्ण * मानम्;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर शून्य remove_observers_from_group_subject(lv_subject_t * subject);
स्थिर शून्य add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_data);
स्थिर शून्य subject_toggle_cb(lv_event_t * e);
स्थिर शून्य subject_set_int_cb(lv_event_t * e);
#जइ LV_USE_FLOAT
    स्थिर शून्य subject_set_float_cb(lv_event_t * e);
#endif

स्थिर शून्य subject_set_string_cb(lv_event_t * e);
स्थिर शून्य subject_increment_cb(lv_event_t * e);

स्थिर शून्य unsubscribe_on_delete_cb(lv_event_t * e);
स्थिर शून्य group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य obj_मानम्_changed_event_cb(lv_event_t * e);

स्थिर शून्य lv_subject_notify_if_changed(lv_subject_t * subject);

स्थिर शून्य subject_set_string_free_user_data_event_cb(lv_event_t * e);

स्थिर शून्य set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#जइ LV_USE_FLOAT
    स्थिर शून्य set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    स्थिर शून्य init_float(lv_subject_t * subject, भिन्न मानम्);
#endif /*LV_USE_FLOAT*/

स्थिर शून्य init_int(lv_subject_t * subject, int32_t मानम्);
स्थिर शून्य init_string(lv_subject_t * subject, वर्ण * buf, वर्ण * prev_buf, size_t size, ध्रुव वर्ण * मानम्);
स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मानम्);
स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color);
स्थिर शून्य init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
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
    lv_ll_init(&subject_list, परिमाण(lv_subject_t));
}
शून्य lv_subject_global_deinit(शून्य)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        जइ(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        जइ(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, प्रत्यागम रिक्त);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    जइ(!subject) {
        प्रत्यागम रिक्त;
    }

    init_common(subject);

    विकल्प(type) {
        पक्ष LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_FLOAT:
#जइ LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, रिक्त);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_GROUP:
            init_group(subject, रिक्त, 0);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_STRING:
            init_string(subject, रिक्त, रिक्त, 0, "");
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_INVALID:
        पक्ष LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    प्रत्यागम subject;
}

शून्य lv_subject_delete(lv_subject_t * subject)
{
    जइ(!subject) {
        प्रत्यागम;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#जइ LV_USE_EXT_DATA
शून्य lv_subject_set_external_data(lv_subject_t * subject, शून्य * data, शून्य (* free_cb)(शून्य * data))
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

शून्य lv_subject_set_int(lv_subject_t * subject, int32_t मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    मानम् = LV_CLAMP(subject->min_मानम्.अङ्कः, मानम्, subject->max_मानम्.अङ्कः);

    subject->prev_मानम्.अङ्कः = subject->मानम्.अङ्कः;
    subject->मानम्.अङ्कः = मानम्;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम 0);

    प्रत्यागम subject->मानम्.अङ्कः;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम 0);

    प्रत्यागम subject->prev_मानम्.अङ्कः;
}

शून्य lv_subject_set_min_मानम्_int(lv_subject_t * subject, int32_t min_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject->min_मानम्.अङ्कः = min_मानम्;
}

शून्य lv_subject_set_max_मानम्_int(lv_subject_t * subject, int32_t max_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject->max_मानम्.अङ्कः = max_मानम्;
}

#जइ LV_USE_FLOAT

शून्य lv_subject_set_float(lv_subject_t * subject, भिन्न मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    मानम् = LV_CLAMP(subject->min_मानम्.float_v, मानम्, subject->max_मानम्.float_v);

    subject->prev_मानम्.float_v = subject->मानम्.float_v;
    subject->मानम्.float_v = मानम्;
    lv_subject_notify_if_changed(subject);
}

भिन्न lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम 0.0);

    प्रत्यागम subject->मानम्.float_v;
}

भिन्न lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम 0.0);

    प्रत्यागम subject->prev_मानम्.float_v;
}

शून्य lv_subject_set_min_मानम्_float(lv_subject_t * subject, भिन्न min_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject->min_मानम्.float_v = min_मानम्;
}

शून्य lv_subject_set_max_मानम्_float(lv_subject_t * subject, भिन्न max_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject->max_मानम्.float_v = max_मानम्;
}


#endif /*LV_USE_FLOAT*/

शून्य lv_subject_set_string_buffer_static(lv_subject_t * subject, वर्ण * buf, वर्ण * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(buf != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(size > 0, प्रत्यागम);

    buf[0] = '\0';
    जइ(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->मानम्.pointer = buf;
    subject->prev_मानम्.pointer = prev_buf;
}

शून्य lv_subject_set_string(lv_subject_t * subject, ध्रुव वर्ण * buf)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(buf != रिक्त, प्रत्यागम);

    जइ(subject->size == 0) प्रत्यागम;
    जइ(subject->prev_मानम्.pointer) {
        lv_strlcpy((वर्ण *)subject->prev_मानम्.pointer, subject->मानम्.pointer, subject->size);
    }

    lv_strlcpy((वर्ण *)subject->मानम्.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

शून्य lv_subject_snprintf(lv_subject_t * subject, ध्रुव वर्ण * format, ...)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(format != रिक्त, प्रत्यागम);

    जइ(subject->size < 1U) प्रत्यागम;

    जइ(subject->prev_मानम्.pointer) {
        lv_strlcpy((वर्ण *)subject->prev_मानम्.pointer, subject->मानम्.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    ध्रुव पूर्णाङ्क ret = lv_vsnprintf((वर्ण *)subject->मानम्.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

ध्रुव वर्ण * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम रिक्त);

    प्रत्यागम subject->मानम्.pointer;
}

ध्रुव वर्ण * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम रिक्त);

    प्रत्यागम subject->prev_मानम्.pointer;
}

शून्य lv_subject_set_pointer(lv_subject_t * subject, शून्य * ptr)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम);

    subject->prev_मानम्.pointer = subject->मानम्.pointer;
    subject->मानम्.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

ध्रुव शून्य * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम रिक्त);


    प्रत्यागम subject->मानम्.pointer;
}

ध्रुव शून्य * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम रिक्त);

    प्रत्यागम subject->prev_मानम्.pointer;
}

शून्य lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम);

    subject->prev_मानम्.color = subject->मानम्.color;
    subject->मानम्.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम lv_color_black());

    प्रत्यागम subject->मानम्.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम lv_color_black());

    प्रत्यागम subject->prev_मानम्.color;
}

शून्य lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(list != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, प्रत्यागम);

    प्रति(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_सन्देशः(list[i] != रिक्त, प्रत्यागम, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, प्रत्यागम रिक्त);
    LV_CHECK_ARG(index >= 0 && index < subject->size, प्रत्यागम रिक्त);

    प्रत्यागम ((lv_subject_t **)(subject->मानम्.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, रिक्त, user_data);
    जइ(observer == रिक्त) प्रत्यागम रिक्त;

    observer->for_obj = 0;
    प्रत्यागम observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    जइ(observer == रिक्त) प्रत्यागम रिक्त;

    lv_memzero(observer, परिमाण(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    जइ(obj != रिक्त) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    प्रत्यागम observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * target,
                                                    शून्य * user_data)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    जइ(observer == रिक्त) प्रत्यागम रिक्त;

    lv_memzero(observer, परिमाण(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    प्रत्यागम observer;
}


शून्य lv_observer_delete(lv_observer_t * observer)
{
    जइ(observer == रिक्त) प्रत्यागम;

    जइ(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, रिक्त, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    जइ(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

शून्य lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_गणनम्(&obj->spec_attr->event_list) : 0);
    प्रति(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        जइ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            जइ(subject == रिक्त || subject == observer->subject) {
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
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम रिक्त);

    प्रत्यागम observer->target;
}

शून्य lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    करेहि {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            जइ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                जइ(subject->notify_restart_query) भङ्ग;
                observer->notified = 1;
            }
        }
    } जाव(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);

    lv_subject_increment_dsc_t * user_data = lv_malloc(परिमाण(lv_subject_increment_dsc_t));
    जइ(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        प्रत्यागम रिक्त;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_मानम् = INT32_MIN;
    user_data->max_मानम् = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    प्रत्यागम user_data;
}

शून्य lv_obj_set_subject_increment_event_min_मानम्(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_मानम्)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->min_मानम् = min_मानम्;
    जइ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        जइ(dsc->subject->मानम्.अङ्कः < min_मानम्) {
            lv_subject_set_int(dsc->subject, min_मानम्);
        }
    }
#जइ LV_USE_FLOAT
    अण्णहा जइ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        जइ(dsc->subject->मानम्.float_v < (भिन्न)min_मानम्) {
            lv_subject_set_float(dsc->subject, (भिन्न)min_मानम्);
        }
    }
#endif
}

शून्य lv_obj_set_subject_increment_event_max_मानम्(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_मानम्)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->max_मानम् = max_मानम्;
    जइ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        जइ(dsc->subject->मानम्.अङ्कः > max_मानम्) {
            lv_subject_set_int(dsc->subject, max_मानम्);
        }
    }
#जइ LV_USE_FLOAT
    अण्णहा जइ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        जइ(dsc->subject->मानम्.float_v > (भिन्न)max_मानम्) {
            lv_subject_set_float(dsc->subject, (भिन्न)max_मानम्);
        }
    }
#endif
}

शून्य lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->rollover = rollover;
}

शून्य lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

शून्य lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject_set_int_user_data_t * user_data = lv_malloc(परिमाण(subject_set_int_user_data_t));
    जइ(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_data->subject = subject;
    user_data->मानम् = मानम्;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#जइ LV_USE_FLOAT
शून्य lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, भिन्न मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject_set_float_user_data_t * user_data = lv_malloc(परिमाण(subject_set_float_user_data_t));
    जइ(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_data->subject = subject;
    user_data->मानम् = मानम्;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

शून्य lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         ध्रुव वर्ण * मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(मानम् != रिक्त, प्रत्यागम);

    subject_set_string_user_data_t * user_data = lv_malloc(परिमाण(subject_set_string_user_data_t));
    जइ(user_data == रिक्त) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_data->subject = subject;
    user_data->मानम् = lv_strdup(मानम्);
    जइ(user_data->मानम् == रिक्त) {
        LV_ASSERT_MALLOC(user_data->मानम्);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        प्रत्यागम;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_bool_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_bool_cb;
    set_bool_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_int_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_int_cb;
    set_int_observer(observable, subject);
    प्रत्यागम observable;
}

#जइ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_float_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_float_cb;
    set_float_observer(observable, subject);
    प्रत्यागम observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_string_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_string_cb;
    set_string_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_color_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_color_cb;
    set_color_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(set_pointer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, रिक्त);
    जइ(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))set_pointer_cb;
    set_pointer_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_मानम्_changed_event_cb, LV_EVENT_मानम्_CHANGED, subject);

    प्रत्यागम observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम रिक्त);

    प्रत्यागम (lv_obj_t *)lv_observer_get_target(observer);
}

शून्य * lv_observer_get_user_data(ध्रुव lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम रिक्त);

    प्रत्यागम observer->user_data;
}

शून्य lv_observer_set_user_data(lv_observer_t * observer, शून्य * user_data)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम);

    /* Release internally-owned data only when replacing it. */
    जइ(observer->auto_free_user_data && observer->user_data != user_data) {
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
    lv_memzero(subject, परिमाण(*subject));
    lv_ll_init(&(subject->subs_ll), परिमाण(lv_observer_t));
}
स्थिर शून्य init_int(lv_subject_t * subject, int32_t मानम्)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->मानम्.अङ्कः = मानम्;
    subject->prev_मानम्.अङ्कः = मानम्;
    subject->min_मानम्.अङ्कः = INT32_MIN;
    subject->max_मानम्.अङ्कः = INT32_MAX;
}

#जइ LV_USE_FLOAT

स्थिर शून्य init_float(lv_subject_t * subject, भिन्न मानम्)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->मानम्.float_v = मानम्;
    subject->prev_मानम्.float_v = मानम्;
    subject->min_मानम्.float_v = -FLT_MAX;
    subject->max_मानम्.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मानम्)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->मानम्.pointer = मानम्;
    subject->prev_मानम्.pointer = मानम्;
}

स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->मानम्.color = color;
    subject->prev_मानम्.color = color;
}

स्थिर शून्य init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

स्थिर शून्य add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || size == 0);
    subject->मानम्.pointer = list;
    subject->size = (uint32_t)size;

    प्रति(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

स्थिर शून्य init_string(lv_subject_t * subject, वर्ण * buf, वर्ण * prev_buf, size_t size, ध्रुव वर्ण * मानम्)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(buf != रिक्त || size == 0);

    जइ(size > 0) {
        lv_strlcpy(buf, मानम्, size);
        जइ(prev_buf) lv_strlcpy(prev_buf, मानम्, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->मानम्.pointer = buf;
    subject->prev_मानम्.pointer = prev_buf;
}

स्थिर शून्य deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    जइ(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    जाव(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#जइ LV_USE_EXT_DATA
    जइ(subject->ext_data.free_cb) {
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
    lv_subject_set_int(user_data->subject, user_data->मानम्);
}


#जइ LV_USE_FLOAT
स्थिर शून्य subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->मानम्);
}
#endif

स्थिर शून्य subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->मानम्);
}

स्थिर शून्य subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    जइ(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_मानम् = LV_MIN(user_data->max_मानम्, user_data->subject->max_मानम्.अङ्कः);
        int32_t min_मानम् = LV_MAX(user_data->min_मानम्, user_data->subject->min_मानम्.अङ्कः);

        int32_t मानम् = lv_subject_get_int(user_data->subject);
        मानम् += user_data->step;

        जइ(user_data->rollover) {
            जइ(मानम् > max_मानम्) {
                मानम् = min_मानम्;
            }
            अण्णहा जइ(मानम् < min_मानम्) {
                मानम् = max_मानम्;
            }
        }
        अण्णहा {
            मानम् = LV_CLAMP(min_मानम्, मानम्, max_मानम्);
        }

        lv_subject_set_int(user_data->subject, मानम्);
    }
#जइ LV_USE_FLOAT
    अण्णहा जइ(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        भिन्न max_मानम् = LV_MIN((भिन्न)user_data->max_मानम्, user_data->subject->max_मानम्.float_v);
        भिन्न min_मानम् = LV_MAX((भिन्न)user_data->min_मानम्, user_data->subject->min_मानम्.float_v);


        भिन्न मानम् = lv_subject_get_float(user_data->subject);
        मानम् += (भिन्न)user_data->step;

        जइ(user_data->rollover) {
            जइ(मानम् > max_मानम्) {
                मानम् = min_मानम्;
            }
            अण्णहा जइ(मानम् < min_मानम्) {
                मानम् = max_मानम्;
            }
        }
        अण्णहा {
            मानम् = LV_CLAMP(min_मानम्, मानम्, max_मानम्);
        }

        lv_subject_set_float(user_data->subject, मानम्);
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

    bound_state_t * p = lv_malloc(परिमाण(bound_state_t));
    जइ(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        प्रत्यागम रिक्त;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    प्रत्यागम observable;
}

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != रिक्त);
    LV_ASSERT(subject != रिक्त);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != रिक्त);

    जइ(subject->मानम्.अङ्कः) {
        lv_obj_add_state(observer->target, p->state);
    }
    अण्णहा {
        lv_obj_remove_state(observer->target, p->state);
    }
}

स्थिर शून्य obj_मानम्_changed_event_cb(lv_event_t * e)
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

    विकल्प(subject->type) {
        पक्ष LV_SUBJECT_TYPE_INT :
            जइ(subject->मानम्.अङ्कः != subject->prev_मानम्.अङ्कः) {
                lv_subject_notify(subject);
            }
            भङ्ग;
#जइ LV_USE_FLOAT
        पक्ष LV_SUBJECT_TYPE_FLOAT :
            जइ(subject->मानम्.float_v != subject->prev_मानम्.float_v) {
                lv_subject_notify(subject);
            }
            भङ्ग;
#endif
        पक्ष LV_SUBJECT_TYPE_GROUP :
        पक्ष LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_COLOR  :
            जइ(!lv_color_eq(subject->मानम्.color, subject->prev_मानम्.color)) {
                lv_subject_notify(subject);
            }
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_STRING:
            जइ(!subject->prev_मानम्.pointer ||
               lv_strcmp(subject->मानम्.pointer, subject->prev_मानम्.pointer)) {
                lv_subject_notify(subject);
            }
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_INVALID :
        पक्ष LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

स्थिर शून्य subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != रिक्त);
    lv_free((शून्य *)user_data->मानम्);
    lv_free(user_data);
}

स्थिर शून्य set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    जइ(set_bool_cb) set_bool_cb(obj, subject->मानम्.अङ्कः);
}

स्थिर शून्य set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    जइ(set_int_cb) set_int_cb(obj, subject->मानम्.अङ्कः);
}

#जइ LV_USE_FLOAT
स्थिर शून्य set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    जइ(set_float_cb) set_float_cb(obj, subject->मानम्.float_v);
}
#endif /*LV_USE_FLOAT*/

स्थिर शून्य set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    जइ(set_string_cb) set_string_cb(obj, subject->मानम्.pointer);
}


स्थिर शून्य set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    जइ(set_color_cb) set_color_cb(obj, subject->मानम्.color);
}

स्थिर शून्य set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    जइ(set_pointer_cb) set_pointer_cb(obj, subject->मानम्.pointer);
}

स्थिर शून्य remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    प्रति(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->मानम्.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_data)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(cb != रिक्त);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    जाव(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        जइ(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
