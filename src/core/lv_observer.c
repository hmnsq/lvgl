/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "lv_observer_private.h"
#यदि LV_USE_OBSERVER

#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "../core/lv_obj_private.h"
#अन्तर्भाव "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#परिभाषय subject_list LV_GLOBAL_DEFAULT()->subject_ll

#यदि_अपरिभाषितम् FLT_MAX
    #परिभाषय FLT_MAX 3.402823466e+38F
#यदि_अन्तः

/**********************
 *      TYPEDEFS
 **********************/
प्रकारनिर्देश संरचना {
    lv_state_t state;
} bound_state_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    int32_t मानम्;
} subject_समुच्चयः_int_user_दत्त_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    भिन्न मानम्;
} subject_समुच्चयः_float_user_दत्त_t;

प्रकारनिर्देश संरचना {
    lv_subject_t * subject;
    ध्रुव वर्ण * मानम्;
} subject_समुच्चयः_string_user_दत्त_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर शून्य remove_observers_from_गणः_subject(lv_subject_t * subject);
स्थिर शून्य सङ्कलय_observers_गन्तव्यम्_गणः_subject(lv_subject_t * subject, lv_subject_t ** list, आकार_t आकार);
स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_दत्त);
स्थिर शून्य subject_toggle_cb(lv_event_t * e);
स्थिर शून्य subject_समुच्चयः_int_cb(lv_event_t * e);
#यदि LV_USE_FLOAT
    स्थिर शून्य subject_समुच्चयः_float_cb(lv_event_t * e);
#यदि_अन्तः

स्थिर शून्य subject_समुच्चयः_string_cb(lv_event_t * e);
स्थिर शून्य subject_increment_cb(lv_event_t * e);

स्थिर शून्य unsubscribe_on_delete_cb(lv_event_t * e);
स्थिर शून्य गणः_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य obj_मानम्_परिवर्तितम्_event_cb(lv_event_t * e);

स्थिर शून्य lv_subject_notify_if_परिवर्तितम्(lv_subject_t * subject);

स्थिर शून्य subject_समुच्चयः_string_free_user_दत्त_event_cb(lv_event_t * e);

स्थिर शून्य समुच्चयः_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य समुच्चयः_int_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य समुच्चयः_string_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य समुच्चयः_color_observer(lv_observer_t * observer, lv_subject_t * subject);
स्थिर शून्य समुच्चयः_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#यदि LV_USE_FLOAT
    स्थिर शून्य समुच्चयः_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    स्थिर शून्य init_float(lv_subject_t * subject, भिन्न मानम्);
#यदि_अन्तः /*LV_USE_FLOAT*/

स्थिर शून्य init_int(lv_subject_t * subject, int32_t मानम्);
स्थिर शून्य init_string(lv_subject_t * subject, वर्ण * buf, वर्ण * पूर्व_buf, आकार_t आकार, ध्रुव वर्ण * मानम्);
स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मानम्);
स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color);
स्थिर शून्य init_गणः(lv_subject_t * subject, lv_subject_t ** list, आकार_t आकार);
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
        यदि(curr->type == LV_SUBJECT_TYPE_गणः) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        यदि(curr->type != LV_SUBJECT_TYPE_गणः) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, प्रत्यागम रिक्त);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);
    lv_subject_t * subject = lv_ll_ins_पुच्छ(&subject_list);
    LV_ASSERT_MALLOC(subject);
    यदि(!subject) {
        प्रत्यागम रिक्त;
    }

    init_common(subject);

    विकल्प(type) {
        पक्ष LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_FLOAT:
#यदि LV_USE_FLOAT
            init_float(subject, 0.f);
#यदि_अन्तः /*LV_USE_FLOAT*/
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, रिक्त);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_गणः:
            init_गणः(subject, रिक्त, 0);
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
    यदि(!subject) {
        प्रत्यागम;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#यदि LV_USE_EXT_दत्त
शून्य lv_subject_समुच्चयः_external_दत्त(lv_subject_t * subject, शून्य * दत्त, शून्य (* free_cb)(शून्य * दत्त))
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);

    subject->ext_दत्त.दत्त = दत्त;
    subject->ext_दत्त.free_cb = free_cb;
}
#यदि_अन्तः

शून्य lv_subject_समुच्चयः_int(lv_subject_t * subject, int32_t मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    मानम् = LV_CLAMP(subject->min_मानम्.अङ्कः, मानम्, subject->max_मानम्.अङ्कः);

    subject->पूर्व_मानम्.अङ्कः = subject->मानम्.अङ्कः;
    subject->मानम्.अङ्कः = मानम्;
    lv_subject_notify_if_परिवर्तितम्(subject);
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

    प्रत्यागम subject->पूर्व_मानम्.अङ्कः;
}

शून्य lv_subject_समुच्चयः_min_मानम्_int(lv_subject_t * subject, int32_t min_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject->min_मानम्.अङ्कः = min_मानम्;
}

शून्य lv_subject_समुच्चयः_max_मानम्_int(lv_subject_t * subject, int32_t max_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject->max_मानम्.अङ्कः = max_मानम्;
}

#यदि LV_USE_FLOAT

शून्य lv_subject_समुच्चयः_float(lv_subject_t * subject, भिन्न मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    मानम् = LV_CLAMP(subject->min_मानम्.float_v, मानम्, subject->max_मानम्.float_v);

    subject->पूर्व_मानम्.float_v = subject->मानम्.float_v;
    subject->मानम्.float_v = मानम्;
    lv_subject_notify_if_परिवर्तितम्(subject);
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

    प्रत्यागम subject->पूर्व_मानम्.float_v;
}

शून्य lv_subject_समुच्चयः_min_मानम्_float(lv_subject_t * subject, भिन्न min_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject->min_मानम्.float_v = min_मानम्;
}

शून्य lv_subject_समुच्चयः_max_मानम्_float(lv_subject_t * subject, भिन्न max_मानम्)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject->max_मानम्.float_v = max_मानम्;
}


#यदि_अन्तः /*LV_USE_FLOAT*/

शून्य lv_subject_समुच्चयः_string_बफर_static(lv_subject_t * subject, वर्ण * buf, वर्ण * पूर्व_buf, आकार_t आकार)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(buf != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(आकार > 0, प्रत्यागम);

    buf[0] = '\0';
    यदि(पूर्व_buf) पूर्व_buf[0] = '\0';

    subject->आकार = (uint32_t)आकार;
    subject->मानम्.pointer = buf;
    subject->पूर्व_मानम्.pointer = पूर्व_buf;
}

शून्य lv_subject_समुच्चयः_string(lv_subject_t * subject, ध्रुव वर्ण * buf)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(buf != रिक्त, प्रत्यागम);

    यदि(subject->आकार == 0) प्रत्यागम;
    यदि(subject->पूर्व_मानम्.pointer) {
        lv_strlcpy((वर्ण *)subject->पूर्व_मानम्.pointer, subject->मानम्.pointer, subject->आकार);
    }

    lv_strlcpy((वर्ण *)subject->मानम्.pointer, buf, subject->आकार);

    lv_subject_notify_if_परिवर्तितम्(subject);
}

शून्य lv_subject_snprintf(lv_subject_t * subject, ध्रुव वर्ण * format, ...)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(format != रिक्त, प्रत्यागम);

    यदि(subject->आकार < 1U) प्रत्यागम;

    यदि(subject->पूर्व_मानम्.pointer) {
        lv_strlcpy((वर्ण *)subject->पूर्व_मानम्.pointer, subject->मानम्.pointer, subject->आकार);
    }

    va_list va;
    va_start(va, format);
    ध्रुव पूर्णाङ्क ret = lv_vsnprintf((वर्ण *)subject->मानम्.pointer, subject->आकार, format, va);
    LV_अप्रयुक्तम्(ret);
    va_अन्तः(va);

    lv_subject_notify_if_परिवर्तितम्(subject);
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

    प्रत्यागम subject->पूर्व_मानम्.pointer;
}

शून्य lv_subject_समुच्चयः_pointer(lv_subject_t * subject, शून्य * ptr)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम);

    subject->पूर्व_मानम्.pointer = subject->मानम्.pointer;
    subject->मानम्.pointer = ptr;
    lv_subject_notify_if_परिवर्तितम्(subject);
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

    प्रत्यागम subject->पूर्व_मानम्.pointer;
}

शून्य lv_subject_समुच्चयः_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम);

    subject->पूर्व_मानम्.color = subject->मानम्.color;
    subject->मानम्.color = color;
    lv_subject_notify_if_परिवर्तितम्(subject);
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

    प्रत्यागम subject->पूर्व_मानम्.color;
}

शून्य lv_subject_समुच्चयः_गणः_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(list != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_गणः, प्रत्यागम);

    प्रति(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_सन्देशः(list[i] != रिक्त, प्रत्यागम, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_गणः_subject(subject);
    सङ्कलय_observers_गन्तव्यम्_गणः_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_गणः_element(lv_subject_t * subject, int32_t सूचकांक)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_गणः, प्रत्यागम रिक्त);
    LV_CHECK_ARG(सूचकांक >= 0 && सूचकांक < subject->आकार, प्रत्यागम रिक्त);

    प्रत्यागम ((lv_subject_t **)(subject->मानम्.pointer))[सूचकांक];
}

lv_observer_t * lv_subject_सङ्कलय_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * user_दत्त)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_subject_सङ्कलय_observer_obj(subject, observer_cb, रिक्त, user_दत्त);
    यदि(observer == रिक्त) प्रत्यागम रिक्त;

    observer->for_obj = 0;
    प्रत्यागम observer;
}

lv_observer_t * lv_subject_सङ्कलय_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            शून्य * user_दत्त)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_ll_ins_पुच्छ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    यदि(observer == रिक्त) प्रत्यागम रिक्त;

    lv_memzero(observer, परिमाण(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_दत्त = user_दत्त;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    यदि(obj != रिक्त) {
        lv_obj_सङ्कलय_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    प्रत्यागम observer;
}

lv_observer_t * lv_subject_सङ्कलय_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, शून्य * target,
                                                    शून्य * user_दत्त)
{
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(observer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, प्रत्यागम रिक्त);

    lv_observer_t * observer = lv_ll_ins_पुच्छ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    यदि(observer == रिक्त) प्रत्यागम रिक्त;

    lv_memzero(observer, परिमाण(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_दत्त = user_दत्त;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    प्रत्यागम observer;
}


शून्य lv_observer_delete(lv_observer_t * observer)
{
    यदि(observer == रिक्त) प्रत्यागम;

    यदि(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_दत्त(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_दत्त(observer->target, रिक्त, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    यदि(observer->auto_free_user_दत्त) {
        lv_free(observer->user_दत्त);
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
        यदि(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_दत्त;
            यदि(subject == रिक्त || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_दत्त(obj, रिक्त, subject);

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

    कुरु {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            यदि(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                यदि(subject->notify_restart_query) भङ्ग;
                observer->notified = 1;
            }
        }
    } यावत्(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_सङ्कलय_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_संहिता_t trigger, int32_t चरणः)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);

    lv_subject_increment_dsc_t * user_दत्त = lv_malloc(परिमाण(lv_subject_increment_dsc_t));
    यदि(user_दत्त == रिक्त) {
        LV_ASSERT_MALLOC(user_दत्त);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        प्रत्यागम रिक्त;
    }

    user_दत्त->चरणः = चरणः;
    user_दत्त->subject = subject;
    user_दत्त->rollover = false;
    user_दत्त->min_मानम् = INT32_MIN;
    user_दत्त->max_मानम् = INT32_MAX;
    lv_obj_सङ्कलय_event_cb(obj, subject_increment_cb, trigger, user_दत्त);
    lv_obj_सङ्कलय_event_cb(obj, lv_event_free_user_दत्त_cb, LV_EVENT_DELETE, user_दत्त);

    प्रत्यागम user_दत्त;
}

शून्य lv_obj_समुच्चयः_subject_increment_event_min_मानम्(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_मानम्)
{
    LV_अप्रयुक्तम्(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->min_मानम् = min_मानम्;
    यदि(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        यदि(dsc->subject->मानम्.अङ्कः < min_मानम्) {
            lv_subject_समुच्चयः_int(dsc->subject, min_मानम्);
        }
    }
#यदि LV_USE_FLOAT
    अन्यथा यदि(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        यदि(dsc->subject->मानम्.float_v < (भिन्न)min_मानम्) {
            lv_subject_समुच्चयः_float(dsc->subject, (भिन्न)min_मानम्);
        }
    }
#यदि_अन्तः
}

शून्य lv_obj_समुच्चयः_subject_increment_event_max_मानम्(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_मानम्)
{
    LV_अप्रयुक्तम्(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->max_मानम् = max_मानम्;
    यदि(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        यदि(dsc->subject->मानम्.अङ्कः > max_मानम्) {
            lv_subject_समुच्चयः_int(dsc->subject, max_मानम्);
        }
    }
#यदि LV_USE_FLOAT
    अन्यथा यदि(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        यदि(dsc->subject->मानम्.float_v > (भिन्न)max_मानम्) {
            lv_subject_समुच्चयः_float(dsc->subject, (भिन्न)max_मानम्);
        }
    }
#यदि_अन्तः
}

शून्य lv_obj_समुच्चयः_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_अप्रयुक्तम्(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम);

    dsc->rollover = rollover;
}

शून्य lv_obj_सङ्कलय_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_संहिता_t trigger)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    lv_obj_सङ्कलय_event_cb(obj, subject_toggle_cb, trigger, subject);
}

शून्य lv_obj_सङ्कलय_subject_समुच्चयः_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_संहिता_t trigger, int32_t मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम);

    subject_समुच्चयः_int_user_दत्त_t * user_दत्त = lv_malloc(परिमाण(subject_समुच्चयः_int_user_दत्त_t));
    यदि(user_दत्त == रिक्त) {
        LV_ASSERT_MALLOC(user_दत्त);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_दत्त->subject = subject;
    user_दत्त->मानम् = मानम्;

    lv_obj_सङ्कलय_event_cb(obj, subject_समुच्चयः_int_cb, trigger, user_दत्त);
    lv_obj_सङ्कलय_event_cb(obj, lv_event_free_user_दत्त_cb, LV_EVENT_DELETE, user_दत्त);
}

#यदि LV_USE_FLOAT
शून्य lv_obj_सङ्कलय_subject_समुच्चयः_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_संहिता_t trigger, भिन्न मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम);

    subject_समुच्चयः_float_user_दत्त_t * user_दत्त = lv_malloc(परिमाण(subject_समुच्चयः_float_user_दत्त_t));
    यदि(user_दत्त == रिक्त) {
        LV_ASSERT_MALLOC(user_दत्त);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_दत्त->subject = subject;
    user_दत्त->मानम् = मानम्;

    lv_obj_सङ्कलय_event_cb(obj, subject_समुच्चयः_float_cb, trigger, user_दत्त);
    lv_obj_सङ्कलय_event_cb(obj, lv_event_free_user_दत्त_cb, LV_EVENT_DELETE, user_दत्त);
}
#यदि_अन्तः /*LV_USE_FLOAT*/

शून्य lv_obj_सङ्कलय_subject_समुच्चयः_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_संहिता_t trigger,
                                         ध्रुव वर्ण * मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम);
    LV_CHECK_ARG(मानम् != रिक्त, प्रत्यागम);

    subject_समुच्चयः_string_user_दत्त_t * user_दत्त = lv_malloc(परिमाण(subject_समुच्चयः_string_user_दत्त_t));
    यदि(user_दत्त == रिक्त) {
        LV_ASSERT_MALLOC(user_दत्त);
        LV_LOG_WARN("Couldn't allocate user_data");
        प्रत्यागम;
    }

    user_दत्त->subject = subject;
    user_दत्त->मानम् = lv_strdup(मानम्);
    यदि(user_दत्त->मानम् == रिक्त) {
        LV_ASSERT_MALLOC(user_दत्त->मानम्);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_दत्त);
        प्रत्यागम;
    }

    lv_obj_सङ्कलय_event_cb(obj, subject_समुच्चयः_string_cb, trigger, user_दत्त);
    lv_obj_सङ्कलय_event_cb(obj, subject_समुच्चयः_string_free_user_दत्त_event_cb, LV_EVENT_DELETE, user_दत्त);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_bool_t समुच्चयः_bool_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_bool_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_bool_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_bool_cb;
    समुच्चयः_bool_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_int_t समुच्चयः_int_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_int_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त);
    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_int_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_int_cb;
    समुच्चयः_int_observer(observable, subject);
    प्रत्यागम observable;
}

#यदि LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_float_t समुच्चयः_float_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_float_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_float_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_float_cb;
    समुच्चयः_float_observer(observable, subject);
    प्रत्यागम observable;
}
#यदि_अन्तः

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_string_t समुच्चयः_string_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_string_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_string_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_string_cb;
    समुच्चयः_string_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_color_t समुच्चयः_color_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_color_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_color_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_color_cb;
    समुच्चयः_color_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_समुच्चयः_pointer_t समुच्चयः_pointer_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(समुच्चयः_pointer_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, प्रत्यागम रिक्त);

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, समुच्चयः_pointer_observer, obj, रिक्त);
    यदि(observable == रिक्त) {
        LV_LOG_WARN("Couldn't add observer to subject");
        प्रत्यागम रिक्त;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (शून्य (*)(शून्य))समुच्चयः_pointer_cb;
    समुच्चयः_pointer_observer(observable, subject);
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(subject->type == LV_SUBJECT_TYPE_INT, प्रत्यागम रिक्त, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_सङ्कलय_event_cb(obj, obj_मानम्_परिवर्तितम्_event_cb, LV_EVENT_मानम्_परिवर्तितम्, subject);

    प्रत्यागम observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम रिक्त);

    प्रत्यागम (lv_obj_t *)lv_observer_get_target(observer);
}

शून्य * lv_observer_get_user_दत्त(ध्रुव lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम रिक्त);

    प्रत्यागम observer->user_दत्त;
}

शून्य lv_observer_समुच्चयः_user_दत्त(lv_observer_t * observer, शून्य * user_दत्त)
{
    LV_CHECK_ARG(observer != रिक्त, प्रत्यागम);

    /* Release internally-owned data only when replacing it. */
    यदि(observer->auto_free_user_दत्त && observer->user_दत्त != user_दत्त) {
        lv_free(observer->user_दत्त);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_दत्त = user_दत्त;
    observer->auto_free_user_दत्त = 0;
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
    subject->पूर्व_मानम्.अङ्कः = मानम्;
    subject->min_मानम्.अङ्कः = INT32_MIN;
    subject->max_मानम्.अङ्कः = INT32_MAX;
}

#यदि LV_USE_FLOAT

स्थिर शून्य init_float(lv_subject_t * subject, भिन्न मानम्)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->मानम्.float_v = मानम्;
    subject->पूर्व_मानम्.float_v = मानम्;
    subject->min_मानम्.float_v = -FLT_MAX;
    subject->max_मानम्.float_v = FLT_MAX;
}

#यदि_अन्तः /*LV_USE_FLOAT*/

स्थिर शून्य init_pointer(lv_subject_t * subject, शून्य * मानम्)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->मानम्.pointer = मानम्;
    subject->पूर्व_मानम्.pointer = मानम्;
}

स्थिर शून्य init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != रिक्त);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->मानम्.color = color;
    subject->पूर्व_मानम्.color = color;
}

स्थिर शून्य init_गणः(lv_subject_t * subject, lv_subject_t ** list, आकार_t आकार)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || आकार == 0);
    subject->type = LV_SUBJECT_TYPE_गणः;
    सङ्कलय_observers_गन्तव्यम्_गणः_subject(subject, list, आकार);
}

स्थिर शून्य सङ्कलय_observers_गन्तव्यम्_गणः_subject(lv_subject_t * subject, lv_subject_t ** list, आकार_t आकार)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(list != रिक्त || आकार == 0);
    subject->मानम्.pointer = list;
    subject->आकार = (uint32_t)आकार;

    प्रति(uint32_t i = 0; i < आकार; i++) {
        lv_subject_सङ्कलय_observer(list[i], गणः_notify_cb, subject);
    }
}

स्थिर शून्य init_string(lv_subject_t * subject, वर्ण * buf, वर्ण * पूर्व_buf, आकार_t आकार, ध्रुव वर्ण * मानम्)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(buf != रिक्त || आकार == 0);

    यदि(आकार > 0) {
        lv_strlcpy(buf, मानम्, आकार);
        यदि(पूर्व_buf) lv_strlcpy(पूर्व_buf, मानम्, आकार);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->आकार = (uint32_t)आकार;
    subject->मानम्.pointer = buf;
    subject->पूर्व_मानम्.pointer = पूर्व_buf;
}

स्थिर शून्य deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    यदि(subject->type == LV_SUBJECT_TYPE_गणः) {
        remove_observers_from_गणः_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_शीर्ष(&subject->subs_ll);
    यावत्(observer) {
        lv_observer_t * observer_अग्र = lv_ll_get_अग्र(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_अग्र;
    }

#यदि LV_USE_EXT_दत्त
    यदि(subject->ext_दत्त.free_cb) {
        subject->ext_दत्त.free_cb(subject->ext_दत्त.दत्त);
        subject->ext_दत्त.दत्त = रिक्त;
    }
#यदि_अन्तः

    lv_ll_clear(&subject->subs_ll);
}

स्थिर शून्य subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_subject_t * subject = lv_event_get_user_दत्त(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_समुच्चयः_int(subject, v);
}

स्थिर शून्य subject_समुच्चयः_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_समुच्चयः_int_user_दत्त_t * user_दत्त = lv_event_get_user_दत्त(e);
    lv_subject_समुच्चयः_int(user_दत्त->subject, user_दत्त->मानम्);
}


#यदि LV_USE_FLOAT
स्थिर शून्य subject_समुच्चयः_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_समुच्चयः_float_user_दत्त_t * user_दत्त = lv_event_get_user_दत्त(e);
    lv_subject_समुच्चयः_float(user_दत्त->subject, user_दत्त->मानम्);
}
#यदि_अन्तः

स्थिर शून्य subject_समुच्चयः_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_समुच्चयः_string_user_दत्त_t * user_दत्त = lv_event_get_user_दत्त(e);
    lv_subject_समुच्चयः_string(user_दत्त->subject, user_दत्त->मानम्);
}

स्थिर शून्य subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_subject_increment_dsc_t * user_दत्त = lv_event_get_user_दत्त(e);

    यदि(user_दत्त->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_मानम् = LV_MIN(user_दत्त->max_मानम्, user_दत्त->subject->max_मानम्.अङ्कः);
        int32_t min_मानम् = LV_MAX(user_दत्त->min_मानम्, user_दत्त->subject->min_मानम्.अङ्कः);

        int32_t मानम् = lv_subject_get_int(user_दत्त->subject);
        मानम् += user_दत्त->चरणः;

        यदि(user_दत्त->rollover) {
            यदि(मानम् > max_मानम्) {
                मानम् = min_मानम्;
            }
            अन्यथा यदि(मानम् < min_मानम्) {
                मानम् = max_मानम्;
            }
        }
        अन्यथा {
            मानम् = LV_CLAMP(min_मानम्, मानम्, max_मानम्);
        }

        lv_subject_समुच्चयः_int(user_दत्त->subject, मानम्);
    }
#यदि LV_USE_FLOAT
    अन्यथा यदि(user_दत्त->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        भिन्न max_मानम् = LV_MIN((भिन्न)user_दत्त->max_मानम्, user_दत्त->subject->max_मानम्.float_v);
        भिन्न min_मानम् = LV_MAX((भिन्न)user_दत्त->min_मानम्, user_दत्त->subject->min_मानम्.float_v);


        भिन्न मानम् = lv_subject_get_float(user_दत्त->subject);
        मानम् += (भिन्न)user_दत्त->चरणः;

        यदि(user_दत्त->rollover) {
            यदि(मानम् > max_मानम्) {
                मानम् = min_मानम्;
            }
            अन्यथा यदि(मानम् < min_मानम्) {
                मानम् = max_मानम्;
            }
        }
        अन्यथा {
            मानम् = LV_CLAMP(min_मानम्, मानम्, max_मानम्);
        }

        lv_subject_समुच्चयः_float(user_दत्त->subject, मानम्);
    }
#यदि_अन्तः
}

स्थिर शून्य गणः_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != रिक्त);
    LV_अप्रयुक्तम्(subject);
    lv_subject_t * subject_गणः = observer->user_दत्त;
    lv_subject_notify(subject_गणः);
}

स्थिर शून्य unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_observer_t * observer = lv_event_get_user_दत्त(e);
    lv_observer_delete(observer);
}

स्थिर lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(obj != रिक्त);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(परिमाण(bound_state_t));
    यदि(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        प्रत्यागम रिक्त;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_सङ्कलय_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_दत्त = 1;

    प्रत्यागम observable;
}

स्थिर शून्य obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != रिक्त);
    LV_ASSERT(subject != रिक्त);
    bound_state_t * p = observer->user_दत्त;
    LV_ASSERT(p != रिक्त);

    यदि(subject->मानम्.अङ्कः) {
        lv_obj_सङ्कलय_state(observer->target, p->state);
    }
    अन्यथा {
        lv_obj_remove_state(observer->target, p->state);
    }
}

स्थिर शून्य obj_मानम्_परिवर्तितम्_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_दत्त(e);
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(obj != रिक्त);

    lv_subject_समुच्चयः_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

स्थिर शून्य lv_subject_notify_if_परिवर्तितम्(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    विकल्प(subject->type) {
        पक्ष LV_SUBJECT_TYPE_INT :
            यदि(subject->मानम्.अङ्कः != subject->पूर्व_मानम्.अङ्कः) {
                lv_subject_notify(subject);
            }
            भङ्ग;
#यदि LV_USE_FLOAT
        पक्ष LV_SUBJECT_TYPE_FLOAT :
            यदि(subject->मानम्.float_v != subject->पूर्व_मानम्.float_v) {
                lv_subject_notify(subject);
            }
            भङ्ग;
#यदि_अन्तः
        पक्ष LV_SUBJECT_TYPE_गणः :
        पक्ष LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_COLOR  :
            यदि(!lv_color_eq(subject->मानम्.color, subject->पूर्व_मानम्.color)) {
                lv_subject_notify(subject);
            }
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_STRING:
            यदि(!subject->पूर्व_मानम्.pointer ||
               lv_strcmp(subject->मानम्.pointer, subject->पूर्व_मानम्.pointer)) {
                lv_subject_notify(subject);
            }
            भङ्ग;
        पक्ष LV_SUBJECT_TYPE_INVALID :
        पक्ष LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

स्थिर शून्य subject_समुच्चयः_string_free_user_दत्त_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != रिक्त);
    subject_समुच्चयः_string_user_दत्त_t * user_दत्त = lv_event_get_user_दत्त(e);
    LV_ASSERT(user_दत्त != रिक्त);
    lv_free((शून्य *)user_दत्त->मानम्);
    lv_free(user_दत्त);
}

स्थिर शून्य समुच्चयः_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_bool_t समुच्चयः_bool_cb = (lv_obj_समुच्चयः_bool_t)observer->user_cb;
    यदि(समुच्चयः_bool_cb) समुच्चयः_bool_cb(obj, subject->मानम्.अङ्कः);
}

स्थिर शून्य समुच्चयः_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_int_t समुच्चयः_int_cb = (lv_obj_समुच्चयः_int_t)observer->user_cb;
    यदि(समुच्चयः_int_cb) समुच्चयः_int_cb(obj, subject->मानम्.अङ्कः);
}

#यदि LV_USE_FLOAT
स्थिर शून्य समुच्चयः_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_float_t समुच्चयः_float_cb = (lv_obj_समुच्चयः_float_t)observer->user_cb;
    यदि(समुच्चयः_float_cb) समुच्चयः_float_cb(obj, subject->मानम्.float_v);
}
#यदि_अन्तः /*LV_USE_FLOAT*/

स्थिर शून्य समुच्चयः_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_string_t समुच्चयः_string_cb = (lv_obj_समुच्चयः_string_t)observer->user_cb;
    यदि(समुच्चयः_string_cb) समुच्चयः_string_cb(obj, subject->मानम्.pointer);
}


स्थिर शून्य समुच्चयः_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_color_t समुच्चयः_color_cb = (lv_obj_समुच्चयः_color_t)observer->user_cb;
    यदि(समुच्चयः_color_cb) समुच्चयः_color_cb(obj, subject->मानम्.color);
}

स्थिर शून्य समुच्चयः_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_समुच्चयः_pointer_t समुच्चयः_pointer_cb = (lv_obj_समुच्चयः_pointer_t)observer->user_cb;
    यदि(समुच्चयः_pointer_cb) समुच्चयः_pointer_cb(obj, subject->मानम्.pointer);
}

स्थिर शून्य remove_observers_from_गणः_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_गणः);

    प्रति(uint32_t i = 0; i < subject->आकार; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->मानम्.pointer))[i];
        subject_remove_observer(sub, गणः_notify_cb, subject);
    }
}

स्थिर शून्य subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, शून्य * user_दत्त)
{
    LV_ASSERT(subject != रिक्त);
    LV_ASSERT(cb != रिक्त);

    lv_observer_t * obs = lv_ll_get_शीर्ष(&subject->subs_ll);
    यावत्(obs) {
        lv_observer_t * obs_अग्र = lv_ll_get_अग्र(&subject->subs_ll, obs);
        यदि(obs->cb == cb && obs->user_दत्त == user_दत्त) {
            lv_observer_delete(obs);
        }
        obs = obs_अग्र;
    }
}

#यदि_अन्तः /*LV_USE_OBSERVER*/
