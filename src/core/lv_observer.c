/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#समावेश "lv_observer_private.h"
#अगर LV_USE_OBSERVER

#समावेश "../lvgl_public.h"
#समावेश "../core/lv_global.h"
#समावेश "../core/lv_obj_private.h"
#समावेश "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#घोषणा subject_list LV_GLOBAL_DEFAULT()->subject_ll

#अगर_अघोषित भग्न_उच्च
    #घोषणा भग्न_उच्च 3.402823466e+38F
#पूर्ण_अगर

/**********************
 *      TYPEDEFS
 **********************/
प्रकार काष्ठा {
    lv_state_t state;
} bound_state_t;

प्रकार काष्ठा {
    lv_subject_t * subject;
    int32_t मूल्य;
} subject_set_int_user_तथ्य_t;

प्रकार काष्ठा {
    lv_subject_t * subject;
    भग्न मूल्य;
} subject_set_float_user_तथ्य_t;

प्रकार काष्ठा {
    lv_subject_t * subject;
    स्थिर अक्षर * मूल्य;
} subject_set_string_user_तथ्य_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

अटल व्योम remove_observers_from_group_subject(lv_subject_t * subject);
अटल व्योम add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार आकार);
अटल व्योम subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, व्योम * user_तथ्य);
अटल व्योम subject_toggle_cb(lv_event_t * e);
अटल व्योम subject_set_int_cb(lv_event_t * e);
#अगर LV_USE_FLOAT
    अटल व्योम subject_set_float_cb(lv_event_t * e);
#पूर्ण_अगर

अटल व्योम subject_set_string_cb(lv_event_t * e);
अटल व्योम subject_increment_cb(lv_event_t * e);

अटल व्योम unsubscribe_on_delete_cb(lv_event_t * e);
अटल व्योम group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
अटल lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

अटल व्योम obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
अटल व्योम obj_मूल्य_changed_event_cb(lv_event_t * e);

अटल व्योम lv_subject_notify_if_changed(lv_subject_t * subject);

अटल व्योम subject_set_string_free_user_तथ्य_event_cb(lv_event_t * e);

अटल व्योम set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
अटल व्योम set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
अटल व्योम set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
अटल व्योम set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
अटल व्योम set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#अगर LV_USE_FLOAT
    अटल व्योम set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    अटल व्योम init_float(lv_subject_t * subject, भग्न मूल्य);
#पूर्ण_अगर /*LV_USE_FLOAT*/

अटल व्योम init_int(lv_subject_t * subject, int32_t मूल्य);
अटल व्योम init_string(lv_subject_t * subject, अक्षर * buf, अक्षर * पूर्व_buf, माप_प्रकार आकार, स्थिर अक्षर * मूल्य);
अटल व्योम init_pointer(lv_subject_t * subject, व्योम * मूल्य);
अटल व्योम init_color(lv_subject_t * subject, lv_color_t color);
अटल व्योम init_group(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार आकार);
अटल व्योम init_common(lv_subject_t * subject);

अटल व्योम deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

व्योम lv_subject_global_init(व्योम)
{
    lv_ll_init(&subject_list, माप(lv_subject_t));
}
व्योम lv_subject_global_deinit(व्योम)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        अगर(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        अगर(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, वापस शून्य);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, वापस शून्य);
    lv_subject_t * subject = lv_ll_ins_पुच्छ(&subject_list);
    LV_ASSERT_MALLOC(subject);
    अगर(!subject) {
        वापस शून्य;
    }

    init_common(subject);

    चयन(type) {
        हाल LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            अवरोध;
        हाल LV_SUBJECT_TYPE_FLOAT:
#अगर LV_USE_FLOAT
            init_float(subject, 0.f);
#पूर्ण_अगर /*LV_USE_FLOAT*/
            अवरोध;
        हाल LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, शून्य);
            अवरोध;
        हाल LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            अवरोध;
        हाल LV_SUBJECT_TYPE_GROUP:
            init_group(subject, शून्य, 0);
            अवरोध;
        हाल LV_SUBJECT_TYPE_STRING:
            init_string(subject, शून्य, शून्य, 0, "");
            अवरोध;
        हाल LV_SUBJECT_TYPE_INVALID:
        हाल LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    वापस subject;
}

व्योम lv_subject_delete(lv_subject_t * subject)
{
    अगर(!subject) {
        वापस;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#अगर LV_USE_EXT_तथ्य
व्योम lv_subject_set_external_तथ्य(lv_subject_t * subject, व्योम * तथ्य, व्योम (* free_cb)(व्योम * तथ्य))
{
    LV_CHECK_ARG(subject != शून्य, वापस);

    subject->ext_तथ्य.तथ्य = तथ्य;
    subject->ext_तथ्य.free_cb = free_cb;
}
#पूर्ण_अगर

व्योम lv_subject_set_int(lv_subject_t * subject, int32_t मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस);

    मूल्य = LV_CLAMP(subject->min_मूल्य.अंक, मूल्य, subject->max_मूल्य.अंक);

    subject->पूर्व_मूल्य.अंक = subject->मूल्य.अंक;
    subject->मूल्य.अंक = मूल्य;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस 0);

    वापस subject->मूल्य.अंक;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस 0);

    वापस subject->पूर्व_मूल्य.अंक;
}

व्योम lv_subject_set_min_मूल्य_int(lv_subject_t * subject, int32_t min_मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस);

    subject->min_मूल्य.अंक = min_मूल्य;
}

व्योम lv_subject_set_max_मूल्य_int(lv_subject_t * subject, int32_t max_मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस);

    subject->max_मूल्य.अंक = max_मूल्य;
}

#अगर LV_USE_FLOAT

व्योम lv_subject_set_float(lv_subject_t * subject, भग्न मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस);

    मूल्य = LV_CLAMP(subject->min_मूल्य.float_v, मूल्य, subject->max_मूल्य.float_v);

    subject->पूर्व_मूल्य.float_v = subject->मूल्य.float_v;
    subject->मूल्य.float_v = मूल्य;
    lv_subject_notify_if_changed(subject);
}

भग्न lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस 0.0);

    वापस subject->मूल्य.float_v;
}

भग्न lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस 0.0);

    वापस subject->पूर्व_मूल्य.float_v;
}

व्योम lv_subject_set_min_मूल्य_float(lv_subject_t * subject, भग्न min_मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस);

    subject->min_मूल्य.float_v = min_मूल्य;
}

व्योम lv_subject_set_max_मूल्य_float(lv_subject_t * subject, भग्न max_मूल्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस);

    subject->max_मूल्य.float_v = max_मूल्य;
}


#पूर्ण_अगर /*LV_USE_FLOAT*/

व्योम lv_subject_set_string_बफर_static(lv_subject_t * subject, अक्षर * buf, अक्षर * पूर्व_buf, माप_प्रकार आकार)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस);
    LV_CHECK_ARG(buf != शून्य, वापस);
    LV_CHECK_ARG(आकार > 0, वापस);

    buf[0] = '\0';
    अगर(पूर्व_buf) पूर्व_buf[0] = '\0';

    subject->आकार = (uint32_t)आकार;
    subject->मूल्य.pointer = buf;
    subject->पूर्व_मूल्य.pointer = पूर्व_buf;
}

व्योम lv_subject_set_string(lv_subject_t * subject, स्थिर अक्षर * buf)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस);
    LV_CHECK_ARG(buf != शून्य, वापस);

    अगर(subject->आकार == 0) वापस;
    अगर(subject->पूर्व_मूल्य.pointer) {
        lv_strlcpy((अक्षर *)subject->पूर्व_मूल्य.pointer, subject->मूल्य.pointer, subject->आकार);
    }

    lv_strlcpy((अक्षर *)subject->मूल्य.pointer, buf, subject->आकार);

    lv_subject_notify_if_changed(subject);
}

व्योम lv_subject_snprintf(lv_subject_t * subject, स्थिर अक्षर * format, ...)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस);
    LV_CHECK_ARG(format != शून्य, वापस);

    अगर(subject->आकार < 1U) वापस;

    अगर(subject->पूर्व_मूल्य.pointer) {
        lv_strlcpy((अक्षर *)subject->पूर्व_मूल्य.pointer, subject->मूल्य.pointer, subject->आकार);
    }

    बहु_सूची va;
    बहु_शुरू(va, format);
    स्थिर पूर्णांक ret = lv_vsnprintf((अक्षर *)subject->मूल्य.pointer, subject->आकार, format, va);
    LV_UNUSED(ret);
    बहु_पूर्ण(va);

    lv_subject_notify_if_changed(subject);
}

स्थिर अक्षर * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस शून्य);

    वापस subject->मूल्य.pointer;
}

स्थिर अक्षर * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस शून्य);

    वापस subject->पूर्व_मूल्य.pointer;
}

व्योम lv_subject_set_pointer(lv_subject_t * subject, व्योम * ptr)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, वापस);

    subject->पूर्व_मूल्य.pointer = subject->मूल्य.pointer;
    subject->मूल्य.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

स्थिर व्योम * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, वापस शून्य);


    वापस subject->मूल्य.pointer;
}

स्थिर व्योम * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, वापस शून्य);

    वापस subject->पूर्व_मूल्य.pointer;
}

व्योम lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, वापस);

    subject->पूर्व_मूल्य.color = subject->मूल्य.color;
    subject->मूल्य.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, वापस lv_color_black());

    वापस subject->मूल्य.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, वापस lv_color_black());

    वापस subject->पूर्व_मूल्य.color;
}

व्योम lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(list != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, वापस);

    क्रम(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_संदेश(list[i] != शून्य, वापस, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t सूचकांक)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, वापस शून्य);
    LV_CHECK_ARG(सूचकांक >= 0 && सूचकांक < subject->आकार, वापस शून्य);

    वापस ((lv_subject_t **)(subject->मूल्य.pointer))[सूचकांक];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(observer_cb != शून्य, वापस शून्य);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, शून्य, user_तथ्य);
    अगर(observer == शून्य) वापस शून्य;

    observer->for_obj = 0;
    वापस observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            व्योम * user_तथ्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(observer_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, वापस शून्य);

    lv_observer_t * observer = lv_ll_ins_पुच्छ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    अगर(observer == शून्य) वापस शून्य;

    lv_memzero(observer, माप(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_तथ्य = user_तथ्य;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    अगर(obj != शून्य) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    वापस observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, व्योम * target,
                                                    व्योम * user_तथ्य)
{
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(observer_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, वापस शून्य);

    lv_observer_t * observer = lv_ll_ins_पुच्छ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    अगर(observer == शून्य) वापस शून्य;

    lv_memzero(observer, माप(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_तथ्य = user_तथ्य;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    वापस observer;
}


व्योम lv_observer_delete(lv_observer_t * observer)
{
    अगर(observer == शून्य) वापस;

    अगर(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_तथ्य(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_तथ्य(observer->target, शून्य, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    अगर(observer->auto_free_user_तथ्य) {
        lv_free(observer->user_तथ्य);
    }
    lv_free(observer);
}

व्योम lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_गिनती(&obj->spec_attr->event_list) : 0);
    क्रम(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        अगर(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_तथ्य;
            अगर(subject == शून्य || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_तथ्य(obj, शून्य, subject);

}

व्योम * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != शून्य, वापस शून्य);

    वापस observer->target;
}

व्योम lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != शून्य, वापस);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    करो {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            अगर(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                अगर(subject->notify_restart_query) अवरोध;
                observer->notified = 1;
            }
        }
    } जबतक(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, वापस शून्य);

    lv_subject_increment_dsc_t * user_तथ्य = lv_malloc(माप(lv_subject_increment_dsc_t));
    अगर(user_तथ्य == शून्य) {
        LV_ASSERT_MALLOC(user_तथ्य);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        वापस शून्य;
    }

    user_तथ्य->step = step;
    user_तथ्य->subject = subject;
    user_तथ्य->rollover = false;
    user_तथ्य->min_मूल्य = INT32_MIN;
    user_तथ्य->max_मूल्य = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_तथ्य);
    lv_obj_add_event_cb(obj, lv_event_free_user_तथ्य_cb, LV_EVENT_DELETE, user_तथ्य);

    वापस user_तथ्य;
}

व्योम lv_obj_set_subject_increment_event_min_मूल्य(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_मूल्य)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    LV_CHECK_ARG(dsc != शून्य, वापस);

    dsc->min_मूल्य = min_मूल्य;
    अगर(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        अगर(dsc->subject->मूल्य.अंक < min_मूल्य) {
            lv_subject_set_int(dsc->subject, min_मूल्य);
        }
    }
#अगर LV_USE_FLOAT
    अन्यथा अगर(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        अगर(dsc->subject->मूल्य.float_v < (भग्न)min_मूल्य) {
            lv_subject_set_float(dsc->subject, (भग्न)min_मूल्य);
        }
    }
#पूर्ण_अगर
}

व्योम lv_obj_set_subject_increment_event_max_मूल्य(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_मूल्य)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    LV_CHECK_ARG(dsc != शून्य, वापस);

    dsc->max_मूल्य = max_मूल्य;
    अगर(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        अगर(dsc->subject->मूल्य.अंक > max_मूल्य) {
            lv_subject_set_int(dsc->subject, max_मूल्य);
        }
    }
#अगर LV_USE_FLOAT
    अन्यथा अगर(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        अगर(dsc->subject->मूल्य.float_v > (भग्न)max_मूल्य) {
            lv_subject_set_float(dsc->subject, (भग्न)max_मूल्य);
        }
    }
#पूर्ण_अगर
}

व्योम lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    LV_CHECK_ARG(dsc != शून्य, वापस);

    dsc->rollover = rollover;
}

व्योम lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

व्योम lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t मूल्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस);

    subject_set_int_user_तथ्य_t * user_तथ्य = lv_malloc(माप(subject_set_int_user_तथ्य_t));
    अगर(user_तथ्य == शून्य) {
        LV_ASSERT_MALLOC(user_तथ्य);
        LV_LOG_WARN("Couldn't allocate user_data");
        वापस;
    }

    user_तथ्य->subject = subject;
    user_तथ्य->मूल्य = मूल्य;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_तथ्य);
    lv_obj_add_event_cb(obj, lv_event_free_user_तथ्य_cb, LV_EVENT_DELETE, user_तथ्य);
}

#अगर LV_USE_FLOAT
व्योम lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, भग्न मूल्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस);

    subject_set_float_user_तथ्य_t * user_तथ्य = lv_malloc(माप(subject_set_float_user_तथ्य_t));
    अगर(user_तथ्य == शून्य) {
        LV_ASSERT_MALLOC(user_तथ्य);
        LV_LOG_WARN("Couldn't allocate user_data");
        वापस;
    }

    user_तथ्य->subject = subject;
    user_तथ्य->मूल्य = मूल्य;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_तथ्य);
    lv_obj_add_event_cb(obj, lv_event_free_user_तथ्य_cb, LV_EVENT_DELETE, user_तथ्य);
}
#पूर्ण_अगर /*LV_USE_FLOAT*/

व्योम lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         स्थिर अक्षर * मूल्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(subject != शून्य, वापस);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस);
    LV_CHECK_ARG(मूल्य != शून्य, वापस);

    subject_set_string_user_तथ्य_t * user_तथ्य = lv_malloc(माप(subject_set_string_user_तथ्य_t));
    अगर(user_तथ्य == शून्य) {
        LV_ASSERT_MALLOC(user_तथ्य);
        LV_LOG_WARN("Couldn't allocate user_data");
        वापस;
    }

    user_तथ्य->subject = subject;
    user_तथ्य->मूल्य = lv_strdup(मूल्य);
    अगर(user_तथ्य->मूल्य == शून्य) {
        LV_ASSERT_MALLOC(user_तथ्य->मूल्य);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_तथ्य);
        वापस;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_तथ्य);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_तथ्य_event_cb, LV_EVENT_DELETE, user_तथ्य);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_bool_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस शून्य);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_bool_cb;
    set_bool_observer(observable, subject);
    वापस observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_int_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, वापस शून्य);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_int_cb;
    set_int_observer(observable, subject);
    वापस observable;
}

#अगर LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_float_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, वापस शून्य);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_float_cb;
    set_float_observer(observable, subject);
    वापस observable;
}
#पूर्ण_अगर

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_string_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, वापस शून्य);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_string_cb;
    set_string_observer(observable, subject);
    वापस observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_color_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, वापस शून्य);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_color_cb;
    set_color_observer(observable, subject);
    वापस observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG(set_pointer_cb != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, वापस शून्य);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, शून्य);
    अगर(observable == शून्य) {
        LV_LOG_WARN("Couldn't add observer to subject");
        वापस शून्य;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (व्योम (*)(व्योम))set_pointer_cb;
    set_pointer_observer(observable, subject);
    वापस observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(subject->type == LV_SUBJECT_TYPE_INT, वापस शून्य, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_मूल्य_changed_event_cb, LV_EVENT_मूल्य_CHANGED, subject);

    वापस observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != शून्य, वापस शून्य);

    वापस (lv_obj_t *)lv_observer_get_target(observer);
}

व्योम * lv_observer_get_user_तथ्य(स्थिर lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != शून्य, वापस शून्य);

    वापस observer->user_तथ्य;
}

व्योम lv_observer_set_user_तथ्य(lv_observer_t * observer, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(observer != शून्य, वापस);

    /* Release internally-owned data only when replacing it. */
    अगर(observer->auto_free_user_तथ्य && observer->user_तथ्य != user_तथ्य) {
        lv_free(observer->user_तथ्य);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_तथ्य = user_तथ्य;
    observer->auto_free_user_तथ्य = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल व्योम init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != शून्य);
    lv_memzero(subject, माप(*subject));
    lv_ll_init(&(subject->subs_ll), माप(lv_observer_t));
}
अटल व्योम init_int(lv_subject_t * subject, int32_t मूल्य)
{
    LV_ASSERT(subject != शून्य);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->मूल्य.अंक = मूल्य;
    subject->पूर्व_मूल्य.अंक = मूल्य;
    subject->min_मूल्य.अंक = INT32_MIN;
    subject->max_मूल्य.अंक = INT32_MAX;
}

#अगर LV_USE_FLOAT

अटल व्योम init_float(lv_subject_t * subject, भग्न मूल्य)
{
    LV_ASSERT(subject != शून्य);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->मूल्य.float_v = मूल्य;
    subject->पूर्व_मूल्य.float_v = मूल्य;
    subject->min_मूल्य.float_v = -भग्न_उच्च;
    subject->max_मूल्य.float_v = भग्न_उच्च;
}

#पूर्ण_अगर /*LV_USE_FLOAT*/

अटल व्योम init_pointer(lv_subject_t * subject, व्योम * मूल्य)
{
    LV_ASSERT(subject != शून्य);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->मूल्य.pointer = मूल्य;
    subject->पूर्व_मूल्य.pointer = मूल्य;
}

अटल व्योम init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != शून्य);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->मूल्य.color = color;
    subject->पूर्व_मूल्य.color = color;
}

अटल व्योम init_group(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार आकार)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(list != शून्य || आकार == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, आकार);
}

अटल व्योम add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, माप_प्रकार आकार)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(list != शून्य || आकार == 0);
    subject->मूल्य.pointer = list;
    subject->आकार = (uint32_t)आकार;

    क्रम(uint32_t i = 0; i < आकार; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

अटल व्योम init_string(lv_subject_t * subject, अक्षर * buf, अक्षर * पूर्व_buf, माप_प्रकार आकार, स्थिर अक्षर * मूल्य)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(buf != शून्य || आकार == 0);

    अगर(आकार > 0) {
        lv_strlcpy(buf, मूल्य, आकार);
        अगर(पूर्व_buf) lv_strlcpy(पूर्व_buf, मूल्य, आकार);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->आकार = (uint32_t)आकार;
    subject->मूल्य.pointer = buf;
    subject->पूर्व_मूल्य.pointer = पूर्व_buf;
}

अटल व्योम deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != शून्य);
    अगर(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_शीर्ष(&subject->subs_ll);
    जबतक(observer) {
        lv_observer_t * observer_अग्र = lv_ll_get_अग्र(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_अग्र;
    }

#अगर LV_USE_EXT_तथ्य
    अगर(subject->ext_तथ्य.free_cb) {
        subject->ext_तथ्य.free_cb(subject->ext_तथ्य.तथ्य);
        subject->ext_तथ्य.तथ्य = शून्य;
    }
#पूर्ण_अगर

    lv_ll_clear(&subject->subs_ll);
}

अटल व्योम subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    lv_subject_t * subject = lv_event_get_user_तथ्य(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

अटल व्योम subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    subject_set_int_user_तथ्य_t * user_तथ्य = lv_event_get_user_तथ्य(e);
    lv_subject_set_int(user_तथ्य->subject, user_तथ्य->मूल्य);
}


#अगर LV_USE_FLOAT
अटल व्योम subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    subject_set_float_user_तथ्य_t * user_तथ्य = lv_event_get_user_तथ्य(e);
    lv_subject_set_float(user_तथ्य->subject, user_तथ्य->मूल्य);
}
#पूर्ण_अगर

अटल व्योम subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    subject_set_string_user_तथ्य_t * user_तथ्य = lv_event_get_user_तथ्य(e);
    lv_subject_set_string(user_तथ्य->subject, user_तथ्य->मूल्य);
}

अटल व्योम subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    lv_subject_increment_dsc_t * user_तथ्य = lv_event_get_user_तथ्य(e);

    अगर(user_तथ्य->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_मूल्य = LV_MIN(user_तथ्य->max_मूल्य, user_तथ्य->subject->max_मूल्य.अंक);
        int32_t min_मूल्य = LV_MAX(user_तथ्य->min_मूल्य, user_तथ्य->subject->min_मूल्य.अंक);

        int32_t मूल्य = lv_subject_get_int(user_तथ्य->subject);
        मूल्य += user_तथ्य->step;

        अगर(user_तथ्य->rollover) {
            अगर(मूल्य > max_मूल्य) {
                मूल्य = min_मूल्य;
            }
            अन्यथा अगर(मूल्य < min_मूल्य) {
                मूल्य = max_मूल्य;
            }
        }
        अन्यथा {
            मूल्य = LV_CLAMP(min_मूल्य, मूल्य, max_मूल्य);
        }

        lv_subject_set_int(user_तथ्य->subject, मूल्य);
    }
#अगर LV_USE_FLOAT
    अन्यथा अगर(user_तथ्य->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        भग्न max_मूल्य = LV_MIN((भग्न)user_तथ्य->max_मूल्य, user_तथ्य->subject->max_मूल्य.float_v);
        भग्न min_मूल्य = LV_MAX((भग्न)user_तथ्य->min_मूल्य, user_तथ्य->subject->min_मूल्य.float_v);


        भग्न मूल्य = lv_subject_get_float(user_तथ्य->subject);
        मूल्य += (भग्न)user_तथ्य->step;

        अगर(user_तथ्य->rollover) {
            अगर(मूल्य > max_मूल्य) {
                मूल्य = min_मूल्य;
            }
            अन्यथा अगर(मूल्य < min_मूल्य) {
                मूल्य = max_मूल्य;
            }
        }
        अन्यथा {
            मूल्य = LV_CLAMP(min_मूल्य, मूल्य, max_मूल्य);
        }

        lv_subject_set_float(user_तथ्य->subject, मूल्य);
    }
#पूर्ण_अगर
}

अटल व्योम group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != शून्य);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_तथ्य;
    lv_subject_notify(subject_group);
}

अटल व्योम unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    lv_observer_t * observer = lv_event_get_user_तथ्य(e);
    lv_observer_delete(observer);
}

अटल lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(obj != शून्य);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(माप(bound_state_t));
    अगर(p == शून्य) {
        LV_LOG_WARN("Out of memory");
        वापस शून्य;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_तथ्य = 1;

    वापस observable;
}

अटल व्योम obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != शून्य);
    LV_ASSERT(subject != शून्य);
    bound_state_t * p = observer->user_तथ्य;
    LV_ASSERT(p != शून्य);

    अगर(subject->मूल्य.अंक) {
        lv_obj_add_state(observer->target, p->state);
    }
    अन्यथा {
        lv_obj_remove_state(observer->target, p->state);
    }
}

अटल व्योम obj_मूल्य_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_तथ्य(e);
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(obj != शून्य);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

अटल व्योम lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    चयन(subject->type) {
        हाल LV_SUBJECT_TYPE_INT :
            अगर(subject->मूल्य.अंक != subject->पूर्व_मूल्य.अंक) {
                lv_subject_notify(subject);
            }
            अवरोध;
#अगर LV_USE_FLOAT
        हाल LV_SUBJECT_TYPE_FLOAT :
            अगर(subject->मूल्य.float_v != subject->पूर्व_मूल्य.float_v) {
                lv_subject_notify(subject);
            }
            अवरोध;
#पूर्ण_अगर
        हाल LV_SUBJECT_TYPE_GROUP :
        हाल LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            अवरोध;
        हाल LV_SUBJECT_TYPE_COLOR  :
            अगर(!lv_color_eq(subject->मूल्य.color, subject->पूर्व_मूल्य.color)) {
                lv_subject_notify(subject);
            }
            अवरोध;
        हाल LV_SUBJECT_TYPE_STRING:
            अगर(!subject->पूर्व_मूल्य.pointer ||
               lv_strcmp(subject->मूल्य.pointer, subject->पूर्व_मूल्य.pointer)) {
                lv_subject_notify(subject);
            }
            अवरोध;
        हाल LV_SUBJECT_TYPE_INVALID :
        हाल LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

अटल व्योम subject_set_string_free_user_तथ्य_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != शून्य);
    subject_set_string_user_तथ्य_t * user_तथ्य = lv_event_get_user_तथ्य(e);
    LV_ASSERT(user_तथ्य != शून्य);
    lv_free((व्योम *)user_तथ्य->मूल्य);
    lv_free(user_तथ्य);
}

अटल व्योम set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    अगर(set_bool_cb) set_bool_cb(obj, subject->मूल्य.अंक);
}

अटल व्योम set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    अगर(set_int_cb) set_int_cb(obj, subject->मूल्य.अंक);
}

#अगर LV_USE_FLOAT
अटल व्योम set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    अगर(set_float_cb) set_float_cb(obj, subject->मूल्य.float_v);
}
#पूर्ण_अगर /*LV_USE_FLOAT*/

अटल व्योम set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    अगर(set_string_cb) set_string_cb(obj, subject->मूल्य.pointer);
}


अटल व्योम set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    अगर(set_color_cb) set_color_cb(obj, subject->मूल्य.color);
}

अटल व्योम set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    अगर(set_pointer_cb) set_pointer_cb(obj, subject->मूल्य.pointer);
}

अटल व्योम remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    क्रम(uint32_t i = 0; i < subject->आकार; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->मूल्य.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

अटल व्योम subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, व्योम * user_तथ्य)
{
    LV_ASSERT(subject != शून्य);
    LV_ASSERT(cb != शून्य);

    lv_observer_t * obs = lv_ll_get_शीर्ष(&subject->subs_ll);
    जबतक(obs) {
        lv_observer_t * obs_अग्र = lv_ll_get_अग्र(&subject->subs_ll, obs);
        अगर(obs->cb == cb && obs->user_तथ्य == user_तथ्य) {
            lv_observer_delete(obs);
        }
        obs = obs_अग्र;
    }
}

#पूर्ण_अगर /*LV_USE_OBSERVER*/
