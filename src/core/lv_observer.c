/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#അന്തര്ഭാവ "lv_observer_private.h"
#എങ്കിൽ LV_USE_OBSERVER

#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "../core/lv_global.h"
#അന്തര്ഭാവ "../core/lv_obj_private.h"
#അന്തര്ഭാവ "../misc/lv_event_private.h"
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
തരനിർവചനം ഘടന {
    lv_state_t state;
} bound_state_t;

തരനിർവചനം ഘടന {
    lv_subject_t * subject;
    int32_t മൂല്യം;
} subject_set_int_user_data_t;

തരനിർവചനം ഘടന {
    lv_subject_t * subject;
    ദശാംശം മൂല്യം;
} subject_set_float_user_data_t;

തരനിർവചനം ഘടന {
    lv_subject_t * subject;
    സ്ഥിരം അക്ഷരം * മൂല്യം;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

സ്ഥിതം ശൂന്യം remove_observers_from_group_subject(lv_subject_t * subject);
സ്ഥിതം ശൂന്യം add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
സ്ഥിതം ശൂന്യം subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ശൂന്യം * user_data);
സ്ഥിതം ശൂന്യം subject_toggle_cb(lv_event_t * e);
സ്ഥിതം ശൂന്യം subject_set_int_cb(lv_event_t * e);
#എങ്കിൽ LV_USE_FLOAT
    സ്ഥിതം ശൂന്യം subject_set_float_cb(lv_event_t * e);
#endif

സ്ഥിതം ശൂന്യം subject_set_string_cb(lv_event_t * e);
സ്ഥിതം ശൂന്യം subject_increment_cb(lv_event_t * e);

സ്ഥിതം ശൂന്യം unsubscribe_on_delete_cb(lv_event_t * e);
സ്ഥിതം ശൂന്യം group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

സ്ഥിതം ശൂന്യം obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം ശൂന്യം obj_മൂല്യം_changed_event_cb(lv_event_t * e);

സ്ഥിതം ശൂന്യം lv_subject_notify_if_changed(lv_subject_t * subject);

സ്ഥിതം ശൂന്യം subject_set_string_free_user_data_event_cb(lv_event_t * e);

സ്ഥിതം ശൂന്യം set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം ശൂന്യം set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം ശൂന്യം set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം ശൂന്യം set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
സ്ഥിതം ശൂന്യം set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#എങ്കിൽ LV_USE_FLOAT
    സ്ഥിതം ശൂന്യം set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    സ്ഥിതം ശൂന്യം init_float(lv_subject_t * subject, ദശാംശം മൂല്യം);
#endif /*LV_USE_FLOAT*/

സ്ഥിതം ശൂന്യം init_int(lv_subject_t * subject, int32_t മൂല്യം);
സ്ഥിതം ശൂന്യം init_string(lv_subject_t * subject, അക്ഷരം * buf, അക്ഷരം * prev_buf, size_t size, സ്ഥിരം അക്ഷരം * മൂല്യം);
സ്ഥിതം ശൂന്യം init_pointer(lv_subject_t * subject, ശൂന്യം * മൂല്യം);
സ്ഥിതം ശൂന്യം init_color(lv_subject_t * subject, lv_color_t color);
സ്ഥിതം ശൂന്യം init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
സ്ഥിതം ശൂന്യം init_common(lv_subject_t * subject);

സ്ഥിതം ശൂന്യം deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ശൂന്യം lv_subject_global_init(ശൂന്യം)
{
    lv_ll_init(&subject_list, വലുപ്പം(lv_subject_t));
}
ശൂന്യം lv_subject_global_deinit(ശൂന്യം)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        എങ്കിൽ(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        എങ്കിൽ(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, തിരികെ ശൂന്യ);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    എങ്കിൽ(!subject) {
        തിരികെ ശൂന്യ;
    }

    init_common(subject);

    തിരഞ്ഞെടുപ്പ്(type) {
        അവസ്ഥ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_FLOAT:
#എങ്കിൽ LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, ശൂന്യ);
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_GROUP:
            init_group(subject, ശൂന്യ, 0);
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_STRING:
            init_string(subject, ശൂന്യ, ശൂന്യ, 0, "");
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_INVALID:
        അവസ്ഥ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    തിരികെ subject;
}

ശൂന്യം lv_subject_delete(lv_subject_t * subject)
{
    എങ്കിൽ(!subject) {
        തിരികെ;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#എങ്കിൽ LV_USE_EXT_DATA
ശൂന്യം lv_subject_set_external_data(lv_subject_t * subject, ശൂന്യം * data, ശൂന്യം (* free_cb)(ശൂന്യം * data))
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

ശൂന്യം lv_subject_set_int(lv_subject_t * subject, int32_t മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ);

    മൂല്യം = LV_CLAMP(subject->min_മൂല്യം.അക്കം, മൂല്യം, subject->max_മൂല്യം.അക്കം);

    subject->prev_മൂല്യം.അക്കം = subject->മൂല്യം.അക്കം;
    subject->മൂല്യം.അക്കം = മൂല്യം;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ 0);

    തിരികെ subject->മൂല്യം.അക്കം;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ 0);

    തിരികെ subject->prev_മൂല്യം.അക്കം;
}

ശൂന്യം lv_subject_set_min_മൂല്യം_int(lv_subject_t * subject, int32_t min_മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ);

    subject->min_മൂല്യം.അക്കം = min_മൂല്യം;
}

ശൂന്യം lv_subject_set_max_മൂല്യം_int(lv_subject_t * subject, int32_t max_മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ);

    subject->max_മൂല്യം.അക്കം = max_മൂല്യം;
}

#എങ്കിൽ LV_USE_FLOAT

ശൂന്യം lv_subject_set_float(lv_subject_t * subject, ദശാംശം മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ);

    മൂല്യം = LV_CLAMP(subject->min_മൂല്യം.float_v, മൂല്യം, subject->max_മൂല്യം.float_v);

    subject->prev_മൂല്യം.float_v = subject->മൂല്യം.float_v;
    subject->മൂല്യം.float_v = മൂല്യം;
    lv_subject_notify_if_changed(subject);
}

ദശാംശം lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ 0.0);

    തിരികെ subject->മൂല്യം.float_v;
}

ദശാംശം lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ 0.0);

    തിരികെ subject->prev_മൂല്യം.float_v;
}

ശൂന്യം lv_subject_set_min_മൂല്യം_float(lv_subject_t * subject, ദശാംശം min_മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ);

    subject->min_മൂല്യം.float_v = min_മൂല്യം;
}

ശൂന്യം lv_subject_set_max_മൂല്യം_float(lv_subject_t * subject, ദശാംശം max_മൂല്യം)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ);

    subject->max_മൂല്യം.float_v = max_മൂല്യം;
}


#endif /*LV_USE_FLOAT*/

ശൂന്യം lv_subject_set_string_buffer_static(lv_subject_t * subject, അക്ഷരം * buf, അക്ഷരം * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ);
    LV_CHECK_ARG(buf != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(size > 0, തിരികെ);

    buf[0] = '\0';
    എങ്കിൽ(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->മൂല്യം.pointer = buf;
    subject->prev_മൂല്യം.pointer = prev_buf;
}

ശൂന്യം lv_subject_set_string(lv_subject_t * subject, സ്ഥിരം അക്ഷരം * buf)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ);
    LV_CHECK_ARG(buf != ശൂന്യ, തിരികെ);

    എങ്കിൽ(subject->size == 0) തിരികെ;
    എങ്കിൽ(subject->prev_മൂല്യം.pointer) {
        lv_strlcpy((അക്ഷരം *)subject->prev_മൂല്യം.pointer, subject->മൂല്യം.pointer, subject->size);
    }

    lv_strlcpy((അക്ഷരം *)subject->മൂല്യം.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

ശൂന്യം lv_subject_snprintf(lv_subject_t * subject, സ്ഥിരം അക്ഷരം * format, ...)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ);
    LV_CHECK_ARG(format != ശൂന്യ, തിരികെ);

    എങ്കിൽ(subject->size < 1U) തിരികെ;

    എങ്കിൽ(subject->prev_മൂല്യം.pointer) {
        lv_strlcpy((അക്ഷരം *)subject->prev_മൂല്യം.pointer, subject->മൂല്യം.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    സ്ഥിരം പൂർണ്ണസംഖ്യ ret = lv_vsnprintf((അക്ഷരം *)subject->മൂല്യം.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

സ്ഥിരം അക്ഷരം * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ ശൂന്യ);

    തിരികെ subject->മൂല്യം.pointer;
}

സ്ഥിരം അക്ഷരം * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ ശൂന്യ);

    തിരികെ subject->prev_മൂല്യം.pointer;
}

ശൂന്യം lv_subject_set_pointer(lv_subject_t * subject, ശൂന്യം * ptr)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, തിരികെ);

    subject->prev_മൂല്യം.pointer = subject->മൂല്യം.pointer;
    subject->മൂല്യം.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

സ്ഥിരം ശൂന്യം * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, തിരികെ ശൂന്യ);


    തിരികെ subject->മൂല്യം.pointer;
}

സ്ഥിരം ശൂന്യം * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, തിരികെ ശൂന്യ);

    തിരികെ subject->prev_മൂല്യം.pointer;
}

ശൂന്യം lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, തിരികെ);

    subject->prev_മൂല്യം.color = subject->മൂല്യം.color;
    subject->മൂല്യം.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, തിരികെ lv_color_black());

    തിരികെ subject->മൂല്യം.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, തിരികെ lv_color_black());

    തിരികെ subject->prev_മൂല്യം.color;
}

ശൂന്യം lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(list != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, തിരികെ);

    വേണ്ടി(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_സന്ദേശം(list[i] != ശൂന്യ, തിരികെ, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(index >= 0 && index < subject->size, തിരികെ ശൂന്യ);

    തിരികെ ((lv_subject_t **)(subject->മൂല്യം.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(observer_cb != ശൂന്യ, തിരികെ ശൂന്യ);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, ശൂന്യ, user_data);
    എങ്കിൽ(observer == ശൂന്യ) തിരികെ ശൂന്യ;

    observer->for_obj = 0;
    തിരികെ observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            ശൂന്യം * user_data)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(observer_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, തിരികെ ശൂന്യ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    എങ്കിൽ(observer == ശൂന്യ) തിരികെ ശൂന്യ;

    lv_memzero(observer, വലുപ്പം(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    എങ്കിൽ(obj != ശൂന്യ) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    തിരികെ observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, ശൂന്യം * target,
                                                    ശൂന്യം * user_data)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(observer_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, തിരികെ ശൂന്യ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    എങ്കിൽ(observer == ശൂന്യ) തിരികെ ശൂന്യ;

    lv_memzero(observer, വലുപ്പം(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    തിരികെ observer;
}


ശൂന്യം lv_observer_delete(lv_observer_t * observer)
{
    എങ്കിൽ(observer == ശൂന്യ) തിരികെ;

    എങ്കിൽ(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, ശൂന്യ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    എങ്കിൽ(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

ശൂന്യം lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_എണ്ണം(&obj->spec_attr->event_list) : 0);
    വേണ്ടി(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        എങ്കിൽ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            എങ്കിൽ(subject == ശൂന്യ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, ശൂന്യ, subject);

}

ശൂന്യം * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ശൂന്യ, തിരികെ ശൂന്യ);

    തിരികെ observer->target;
}

ശൂന്യം lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    ചെയ്യുക {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            എങ്കിൽ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                എങ്കിൽ(subject->notify_restart_query) നിർത്തുക;
                observer->notified = 1;
            }
        }
    } ആകുമ്പോൾ(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ ശൂന്യ);

    lv_subject_increment_dsc_t * user_data = lv_malloc(വലുപ്പം(lv_subject_increment_dsc_t));
    എങ്കിൽ(user_data == ശൂന്യ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        തിരികെ ശൂന്യ;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_മൂല്യം = INT32_MIN;
    user_data->max_മൂല്യം = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    തിരികെ user_data;
}

ശൂന്യം lv_obj_set_subject_increment_event_min_മൂല്യം(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_മൂല്യം)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, തിരികെ);
    LV_CHECK_ARG(dsc != ശൂന്യ, തിരികെ);

    dsc->min_മൂല്യം = min_മൂല്യം;
    എങ്കിൽ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        എങ്കിൽ(dsc->subject->മൂല്യം.അക്കം < min_മൂല്യം) {
            lv_subject_set_int(dsc->subject, min_മൂല്യം);
        }
    }
#എങ്കിൽ LV_USE_FLOAT
    അല്ലെങ്കിൽ എങ്കിൽ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        എങ്കിൽ(dsc->subject->മൂല്യം.float_v < (ദശാംശം)min_മൂല്യം) {
            lv_subject_set_float(dsc->subject, (ദശാംശം)min_മൂല്യം);
        }
    }
#endif
}

ശൂന്യം lv_obj_set_subject_increment_event_max_മൂല്യം(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_മൂല്യം)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, തിരികെ);
    LV_CHECK_ARG(dsc != ശൂന്യ, തിരികെ);

    dsc->max_മൂല്യം = max_മൂല്യം;
    എങ്കിൽ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        എങ്കിൽ(dsc->subject->മൂല്യം.അക്കം > max_മൂല്യം) {
            lv_subject_set_int(dsc->subject, max_മൂല്യം);
        }
    }
#എങ്കിൽ LV_USE_FLOAT
    അല്ലെങ്കിൽ എങ്കിൽ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        എങ്കിൽ(dsc->subject->മൂല്യം.float_v > (ദശാംശം)max_മൂല്യം) {
            lv_subject_set_float(dsc->subject, (ദശാംശം)max_മൂല്യം);
        }
    }
#endif
}

ശൂന്യം lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, തിരികെ);
    LV_CHECK_ARG(dsc != ശൂന്യ, തിരികെ);

    dsc->rollover = rollover;
}

ശൂന്യം lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

ശൂന്യം lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t മൂല്യം)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ);

    subject_set_int_user_data_t * user_data = lv_malloc(വലുപ്പം(subject_set_int_user_data_t));
    എങ്കിൽ(user_data == ശൂന്യ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        തിരികെ;
    }

    user_data->subject = subject;
    user_data->മൂല്യം = മൂല്യം;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#എങ്കിൽ LV_USE_FLOAT
ശൂന്യം lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, ദശാംശം മൂല്യം)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ);

    subject_set_float_user_data_t * user_data = lv_malloc(വലുപ്പം(subject_set_float_user_data_t));
    എങ്കിൽ(user_data == ശൂന്യ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        തിരികെ;
    }

    user_data->subject = subject;
    user_data->മൂല്യം = മൂല്യം;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

ശൂന്യം lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         സ്ഥിരം അക്ഷരം * മൂല്യം)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ);
    LV_CHECK_ARG(മൂല്യം != ശൂന്യ, തിരികെ);

    subject_set_string_user_data_t * user_data = lv_malloc(വലുപ്പം(subject_set_string_user_data_t));
    എങ്കിൽ(user_data == ശൂന്യ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        തിരികെ;
    }

    user_data->subject = subject;
    user_data->മൂല്യം = lv_strdup(മൂല്യം);
    എങ്കിൽ(user_data->മൂല്യം == ശൂന്യ) {
        LV_ASSERT_MALLOC(user_data->മൂല്യം);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        തിരികെ;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_bool_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ ശൂന്യ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_bool_cb;
    set_bool_observer(observable, subject);
    തിരികെ observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_int_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ ശൂന്യ);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_int_cb;
    set_int_observer(observable, subject);
    തിരികെ observable;
}

#എങ്കിൽ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_float_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, തിരികെ ശൂന്യ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_float_cb;
    set_float_observer(observable, subject);
    തിരികെ observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_string_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, തിരികെ ശൂന്യ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_string_cb;
    set_string_observer(observable, subject);
    തിരികെ observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_color_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, തിരികെ ശൂന്യ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_color_cb;
    set_color_observer(observable, subject);
    തിരികെ observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(set_pointer_cb != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, തിരികെ ശൂന്യ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, ശൂന്യ);
    എങ്കിൽ(observable == ശൂന്യ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        തിരികെ ശൂന്യ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ശൂന്യം (*)(ശൂന്യം))set_pointer_cb;
    set_pointer_observer(observable, subject);
    തിരികെ observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(subject != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG_FORMAT_സന്ദേശം(subject->type == LV_SUBJECT_TYPE_INT, തിരികെ ശൂന്യ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_മൂല്യം_changed_event_cb, LV_EVENT_മൂല്യം_CHANGED, subject);

    തിരികെ observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ശൂന്യ, തിരികെ ശൂന്യ);

    തിരികെ (lv_obj_t *)lv_observer_get_target(observer);
}

ശൂന്യം * lv_observer_get_user_data(സ്ഥിരം lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ശൂന്യ, തിരികെ ശൂന്യ);

    തിരികെ observer->user_data;
}

ശൂന്യം lv_observer_set_user_data(lv_observer_t * observer, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(observer != ശൂന്യ, തിരികെ);

    /* Release internally-owned data only when replacing it. */
    എങ്കിൽ(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

സ്ഥിതം ശൂന്യം init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ശൂന്യ);
    lv_memzero(subject, വലുപ്പം(*subject));
    lv_ll_init(&(subject->subs_ll), വലുപ്പം(lv_observer_t));
}
സ്ഥിതം ശൂന്യം init_int(lv_subject_t * subject, int32_t മൂല്യം)
{
    LV_ASSERT(subject != ശൂന്യ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->മൂല്യം.അക്കം = മൂല്യം;
    subject->prev_മൂല്യം.അക്കം = മൂല്യം;
    subject->min_മൂല്യം.അക്കം = INT32_MIN;
    subject->max_മൂല്യം.അക്കം = INT32_MAX;
}

#എങ്കിൽ LV_USE_FLOAT

സ്ഥിതം ശൂന്യം init_float(lv_subject_t * subject, ദശാംശം മൂല്യം)
{
    LV_ASSERT(subject != ശൂന്യ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->മൂല്യം.float_v = മൂല്യം;
    subject->prev_മൂല്യം.float_v = മൂല്യം;
    subject->min_മൂല്യം.float_v = -FLT_MAX;
    subject->max_മൂല്യം.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

സ്ഥിതം ശൂന്യം init_pointer(lv_subject_t * subject, ശൂന്യം * മൂല്യം)
{
    LV_ASSERT(subject != ശൂന്യ);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->മൂല്യം.pointer = മൂല്യം;
    subject->prev_മൂല്യം.pointer = മൂല്യം;
}

സ്ഥിതം ശൂന്യം init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ശൂന്യ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->മൂല്യം.color = color;
    subject->prev_മൂല്യം.color = color;
}

സ്ഥിതം ശൂന്യം init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(list != ശൂന്യ || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

സ്ഥിതം ശൂന്യം add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(list != ശൂന്യ || size == 0);
    subject->മൂല്യം.pointer = list;
    subject->size = (uint32_t)size;

    വേണ്ടി(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

സ്ഥിതം ശൂന്യം init_string(lv_subject_t * subject, അക്ഷരം * buf, അക്ഷരം * prev_buf, size_t size, സ്ഥിരം അക്ഷരം * മൂല്യം)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(buf != ശൂന്യ || size == 0);

    എങ്കിൽ(size > 0) {
        lv_strlcpy(buf, മൂല്യം, size);
        എങ്കിൽ(prev_buf) lv_strlcpy(prev_buf, മൂല്യം, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->മൂല്യം.pointer = buf;
    subject->prev_മൂല്യം.pointer = prev_buf;
}

സ്ഥിതം ശൂന്യം deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ശൂന്യ);
    എങ്കിൽ(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    ആകുമ്പോൾ(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#എങ്കിൽ LV_USE_EXT_DATA
    എങ്കിൽ(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = ശൂന്യ;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

സ്ഥിതം ശൂന്യം subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

സ്ഥിതം ശൂന്യം subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->മൂല്യം);
}


#എങ്കിൽ LV_USE_FLOAT
സ്ഥിതം ശൂന്യം subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->മൂല്യം);
}
#endif

സ്ഥിതം ശൂന്യം subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->മൂല്യം);
}

സ്ഥിതം ശൂന്യം subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    എങ്കിൽ(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_മൂല്യം = LV_MIN(user_data->max_മൂല്യം, user_data->subject->max_മൂല്യം.അക്കം);
        int32_t min_മൂല്യം = LV_MAX(user_data->min_മൂല്യം, user_data->subject->min_മൂല്യം.അക്കം);

        int32_t മൂല്യം = lv_subject_get_int(user_data->subject);
        മൂല്യം += user_data->step;

        എങ്കിൽ(user_data->rollover) {
            എങ്കിൽ(മൂല്യം > max_മൂല്യം) {
                മൂല്യം = min_മൂല്യം;
            }
            അല്ലെങ്കിൽ എങ്കിൽ(മൂല്യം < min_മൂല്യം) {
                മൂല്യം = max_മൂല്യം;
            }
        }
        അല്ലെങ്കിൽ {
            മൂല്യം = LV_CLAMP(min_മൂല്യം, മൂല്യം, max_മൂല്യം);
        }

        lv_subject_set_int(user_data->subject, മൂല്യം);
    }
#എങ്കിൽ LV_USE_FLOAT
    അല്ലെങ്കിൽ എങ്കിൽ(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ദശാംശം max_മൂല്യം = LV_MIN((ദശാംശം)user_data->max_മൂല്യം, user_data->subject->max_മൂല്യം.float_v);
        ദശാംശം min_മൂല്യം = LV_MAX((ദശാംശം)user_data->min_മൂല്യം, user_data->subject->min_മൂല്യം.float_v);


        ദശാംശം മൂല്യം = lv_subject_get_float(user_data->subject);
        മൂല്യം += (ദശാംശം)user_data->step;

        എങ്കിൽ(user_data->rollover) {
            എങ്കിൽ(മൂല്യം > max_മൂല്യം) {
                മൂല്യം = min_മൂല്യം;
            }
            അല്ലെങ്കിൽ എങ്കിൽ(മൂല്യം < min_മൂല്യം) {
                മൂല്യം = max_മൂല്യം;
            }
        }
        അല്ലെങ്കിൽ {
            മൂല്യം = LV_CLAMP(min_മൂല്യം, മൂല്യം, max_മൂല്യം);
        }

        lv_subject_set_float(user_data->subject, മൂല്യം);
    }
#endif
}

സ്ഥിതം ശൂന്യം group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ശൂന്യ);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

സ്ഥിതം ശൂന്യം unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

സ്ഥിതം lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(obj != ശൂന്യ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(വലുപ്പം(bound_state_t));
    എങ്കിൽ(p == ശൂന്യ) {
        LV_LOG_WARN("Out of memory");
        തിരികെ ശൂന്യ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    തിരികെ observable;
}

സ്ഥിതം ശൂന്യം obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ശൂന്യ);
    LV_ASSERT(subject != ശൂന്യ);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != ശൂന്യ);

    എങ്കിൽ(subject->മൂല്യം.അക്കം) {
        lv_obj_add_state(observer->target, p->state);
    }
    അല്ലെങ്കിൽ {
        lv_obj_remove_state(observer->target, p->state);
    }
}

സ്ഥിതം ശൂന്യം obj_മൂല്യം_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(obj != ശൂന്യ);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

സ്ഥിതം ശൂന്യം lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    തിരഞ്ഞെടുപ്പ്(subject->type) {
        അവസ്ഥ LV_SUBJECT_TYPE_INT :
            എങ്കിൽ(subject->മൂല്യം.അക്കം != subject->prev_മൂല്യം.അക്കം) {
                lv_subject_notify(subject);
            }
            നിർത്തുക;
#എങ്കിൽ LV_USE_FLOAT
        അവസ്ഥ LV_SUBJECT_TYPE_FLOAT :
            എങ്കിൽ(subject->മൂല്യം.float_v != subject->prev_മൂല്യം.float_v) {
                lv_subject_notify(subject);
            }
            നിർത്തുക;
#endif
        അവസ്ഥ LV_SUBJECT_TYPE_GROUP :
        അവസ്ഥ LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_COLOR  :
            എങ്കിൽ(!lv_color_eq(subject->മൂല്യം.color, subject->prev_മൂല്യം.color)) {
                lv_subject_notify(subject);
            }
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_STRING:
            എങ്കിൽ(!subject->prev_മൂല്യം.pointer ||
               lv_strcmp(subject->മൂല്യം.pointer, subject->prev_മൂല്യം.pointer)) {
                lv_subject_notify(subject);
            }
            നിർത്തുക;
        അവസ്ഥ LV_SUBJECT_TYPE_INVALID :
        അവസ്ഥ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

സ്ഥിതം ശൂന്യം subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ശൂന്യ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != ശൂന്യ);
    lv_free((ശൂന്യം *)user_data->മൂല്യം);
    lv_free(user_data);
}

സ്ഥിതം ശൂന്യം set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    എങ്കിൽ(set_bool_cb) set_bool_cb(obj, subject->മൂല്യം.അക്കം);
}

സ്ഥിതം ശൂന്യം set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    എങ്കിൽ(set_int_cb) set_int_cb(obj, subject->മൂല്യം.അക്കം);
}

#എങ്കിൽ LV_USE_FLOAT
സ്ഥിതം ശൂന്യം set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    എങ്കിൽ(set_float_cb) set_float_cb(obj, subject->മൂല്യം.float_v);
}
#endif /*LV_USE_FLOAT*/

സ്ഥിതം ശൂന്യം set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    എങ്കിൽ(set_string_cb) set_string_cb(obj, subject->മൂല്യം.pointer);
}


സ്ഥിതം ശൂന്യം set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    എങ്കിൽ(set_color_cb) set_color_cb(obj, subject->മൂല്യം.color);
}

സ്ഥിതം ശൂന്യം set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    എങ്കിൽ(set_pointer_cb) set_pointer_cb(obj, subject->മൂല്യം.pointer);
}

സ്ഥിതം ശൂന്യം remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    വേണ്ടി(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->മൂല്യം.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

സ്ഥിതം ശൂന്യം subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ശൂന്യം * user_data)
{
    LV_ASSERT(subject != ശൂന്യ);
    LV_ASSERT(cb != ശൂന്യ);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    ആകുമ്പോൾ(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        എങ്കിൽ(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
