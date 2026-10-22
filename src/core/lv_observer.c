/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ಅನ್ತರ್ಭಾವ "lv_observer_private.h"
#ಯದಿ LV_USE_OBSERVER

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_event_private.h"
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
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_state_t state;
} bound_state_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_subject_t * subject;
    int32_t ಮೌಲ್ಯ;
} subject_set_int_user_data_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_subject_t * subject;
    ಭಿನ್ನ ಮೌಲ್ಯ;
} subject_set_float_user_data_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_subject_t * subject;
    ಧ್ರುವ ವರ್ಣ * ಮೌಲ್ಯ;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ remove_observers_from_group_subject(lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ಶೂನ್ಯ * user_data);
ಸ್ಥಿರ ಶೂನ್ಯ subject_toggle_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ subject_set_int_cb(lv_event_t * e);
#ಯದಿ LV_USE_FLOAT
    ಸ್ಥಿರ ಶೂನ್ಯ subject_set_float_cb(lv_event_t * e);
#endif

ಸ್ಥಿರ ಶೂನ್ಯ subject_set_string_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ subject_increment_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ unsubscribe_on_delete_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ಸ್ಥಿರ ಶೂನ್ಯ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ obj_ಮೌಲ್ಯ_changed_event_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ lv_subject_notify_if_changed(lv_subject_t * subject);

ಸ್ಥಿರ ಶೂನ್ಯ subject_set_string_free_user_data_event_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#ಯದಿ LV_USE_FLOAT
    ಸ್ಥಿರ ಶೂನ್ಯ set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ಸ್ಥಿರ ಶೂನ್ಯ init_float(lv_subject_t * subject, ಭಿನ್ನ ಮೌಲ್ಯ);
#endif /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ init_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_string(lv_subject_t * subject, ವರ್ಣ * buf, ವರ್ಣ * prev_buf, size_t size, ಧ್ರುವ ವರ್ಣ * ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_pointer(lv_subject_t * subject, ಶೂನ್ಯ * ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_color(lv_subject_t * subject, lv_color_t color);
ಸ್ಥಿರ ಶೂನ್ಯ init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ಸ್ಥಿರ ಶೂನ್ಯ init_common(lv_subject_t * subject);

ಸ್ಥಿರ ಶೂನ್ಯ deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_subject_global_init(ಶೂನ್ಯ)
{
    lv_ll_init(&subject_list, ಪರಿಮಾಣ(lv_subject_t));
}
ಶೂನ್ಯ lv_subject_global_deinit(ಶೂನ್ಯ)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        ಯದಿ(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        ಯದಿ(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    ಯದಿ(!subject) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    init_common(subject);

    ವಿಕಲ್ಪ(type) {
        ಪಕ್ಷ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_FLOAT:
#ಯದಿ LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, ರಿಕ್ತ);
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_GROUP:
            init_group(subject, ರಿಕ್ತ, 0);
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_STRING:
            init_string(subject, ರಿಕ್ತ, ರಿಕ್ತ, 0, "");
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_INVALID:
        ಪಕ್ಷ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    ಪ್ರತ್ಯಾಗಮ subject;
}

ಶೂನ್ಯ lv_subject_delete(lv_subject_t * subject)
{
    ಯದಿ(!subject) {
        ಪ್ರತ್ಯಾಗಮ;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#ಯದಿ LV_USE_EXT_DATA
ಶೂನ್ಯ lv_subject_set_external_data(lv_subject_t * subject, ಶೂನ್ಯ * data, ಶೂನ್ಯ (* free_cb)(ಶೂನ್ಯ * data))
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

ಶೂನ್ಯ lv_subject_set_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ);

    ಮೌಲ್ಯ = LV_CLAMP(subject->min_ಮೌಲ್ಯ.ಅಂಕೆ, ಮೌಲ್ಯ, subject->max_ಮೌಲ್ಯ.ಅಂಕೆ);

    subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ = subject->ಮೌಲ್ಯ.ಅಂಕೆ;
    subject->ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ 0);

    ಪ್ರತ್ಯಾಗಮ subject->ಮೌಲ್ಯ.ಅಂಕೆ;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ 0);

    ಪ್ರತ್ಯಾಗಮ subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ;
}

ಶೂನ್ಯ lv_subject_set_min_ಮೌಲ್ಯ_int(lv_subject_t * subject, int32_t min_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ);

    subject->min_ಮೌಲ್ಯ.ಅಂಕೆ = min_ಮೌಲ್ಯ;
}

ಶೂನ್ಯ lv_subject_set_max_ಮೌಲ್ಯ_int(lv_subject_t * subject, int32_t max_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ);

    subject->max_ಮೌಲ್ಯ.ಅಂಕೆ = max_ಮೌಲ್ಯ;
}

#ಯದಿ LV_USE_FLOAT

ಶೂನ್ಯ lv_subject_set_float(lv_subject_t * subject, ಭಿನ್ನ ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ);

    ಮೌಲ್ಯ = LV_CLAMP(subject->min_ಮೌಲ್ಯ.float_v, ಮೌಲ್ಯ, subject->max_ಮೌಲ್ಯ.float_v);

    subject->prev_ಮೌಲ್ಯ.float_v = subject->ಮೌಲ್ಯ.float_v;
    subject->ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    lv_subject_notify_if_changed(subject);
}

ಭಿನ್ನ lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ 0.0);

    ಪ್ರತ್ಯಾಗಮ subject->ಮೌಲ್ಯ.float_v;
}

ಭಿನ್ನ lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ 0.0);

    ಪ್ರತ್ಯಾಗಮ subject->prev_ಮೌಲ್ಯ.float_v;
}

ಶೂನ್ಯ lv_subject_set_min_ಮೌಲ್ಯ_float(lv_subject_t * subject, ಭಿನ್ನ min_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ);

    subject->min_ಮೌಲ್ಯ.float_v = min_ಮೌಲ್ಯ;
}

ಶೂನ್ಯ lv_subject_set_max_ಮೌಲ್ಯ_float(lv_subject_t * subject, ಭಿನ್ನ max_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ);

    subject->max_ಮೌಲ್ಯ.float_v = max_ಮೌಲ್ಯ;
}


#endif /*LV_USE_FLOAT*/

ಶೂನ್ಯ lv_subject_set_string_buffer_static(lv_subject_t * subject, ವರ್ಣ * buf, ವರ್ಣ * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(buf != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(size > 0, ಪ್ರತ್ಯಾಗಮ);

    buf[0] = '\0';
    ಯದಿ(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->ಮೌಲ್ಯ.pointer = buf;
    subject->prev_ಮೌಲ್ಯ.pointer = prev_buf;
}

ಶೂನ್ಯ lv_subject_set_string(lv_subject_t * subject, ಧ್ರುವ ವರ್ಣ * buf)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(buf != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(subject->size == 0) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ(subject->prev_ಮೌಲ್ಯ.pointer) {
        lv_strlcpy((ವರ್ಣ *)subject->prev_ಮೌಲ್ಯ.pointer, subject->ಮೌಲ್ಯ.pointer, subject->size);
    }

    lv_strlcpy((ವರ್ಣ *)subject->ಮೌಲ್ಯ.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

ಶೂನ್ಯ lv_subject_snprintf(lv_subject_t * subject, ಧ್ರುವ ವರ್ಣ * format, ...)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(format != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(subject->size < 1U) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ(subject->prev_ಮೌಲ್ಯ.pointer) {
        lv_strlcpy((ವರ್ಣ *)subject->prev_ಮೌಲ್ಯ.pointer, subject->ಮೌಲ್ಯ.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ret = lv_vsnprintf((ವರ್ಣ *)subject->ಮೌಲ್ಯ.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

ಧ್ರುವ ವರ್ಣ * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ subject->ಮೌಲ್ಯ.pointer;
}

ಧ್ರುವ ವರ್ಣ * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ subject->prev_ಮೌಲ್ಯ.pointer;
}

ಶೂನ್ಯ lv_subject_set_pointer(lv_subject_t * subject, ಶೂನ್ಯ * ptr)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ಪ್ರತ್ಯಾಗಮ);

    subject->prev_ಮೌಲ್ಯ.pointer = subject->ಮೌಲ್ಯ.pointer;
    subject->ಮೌಲ್ಯ.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

ಧ್ರುವ ಶೂನ್ಯ * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);


    ಪ್ರತ್ಯಾಗಮ subject->ಮೌಲ್ಯ.pointer;
}

ಧ್ರುವ ಶೂನ್ಯ * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ subject->prev_ಮೌಲ್ಯ.pointer;
}

ಶೂನ್ಯ lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಪ್ರತ್ಯಾಗಮ);

    subject->prev_ಮೌಲ್ಯ.color = subject->ಮೌಲ್ಯ.color;
    subject->ಮೌಲ್ಯ.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಪ್ರತ್ಯಾಗಮ lv_color_black());

    ಪ್ರತ್ಯಾಗಮ subject->ಮೌಲ್ಯ.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಪ್ರತ್ಯಾಗಮ lv_color_black());

    ಪ್ರತ್ಯಾಗಮ subject->prev_ಮೌಲ್ಯ.color;
}

ಶೂನ್ಯ lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(list != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, ಪ್ರತ್ಯಾಗಮ);

    ಪ್ರತಿ(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(list[i] != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(index >= 0 && index < subject->size, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ ((lv_subject_t **)(subject->ಮೌಲ್ಯ.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(observer_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, ರಿಕ್ತ, user_data);
    ಯದಿ(observer == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    observer->for_obj = 0;
    ಪ್ರತ್ಯಾಗಮ observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(observer_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ಯದಿ(observer == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    lv_memzero(observer, ಪರಿಮಾಣ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    ಯದಿ(obj != ರಿಕ್ತ) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ಪ್ರತ್ಯಾಗಮ observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, ಶೂನ್ಯ * target,
                                                    ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(observer_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ಯದಿ(observer == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    lv_memzero(observer, ಪರಿಮಾಣ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ಪ್ರತ್ಯಾಗಮ observer;
}


ಶೂನ್ಯ lv_observer_delete(lv_observer_t * observer)
{
    ಯದಿ(observer == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, ರಿಕ್ತ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    ಯದಿ(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

ಶೂನ್ಯ lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_ಎಣಿಕೆ(&obj->spec_attr->event_list) : 0);
    ಪ್ರತಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        ಯದಿ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            ಯದಿ(subject == ರಿಕ್ತ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, ರಿಕ್ತ, subject);

}

ಶೂನ್ಯ * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ observer->target;
}

ಶೂನ್ಯ lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    ಕುರು {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            ಯದಿ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                ಯದಿ(subject->notify_restart_query) ಭಙ್ಗ;
                observer->notified = 1;
            }
        }
    } ಯಾವತ್(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_subject_increment_dsc_t * user_data = lv_malloc(ಪರಿಮಾಣ(lv_subject_increment_dsc_t));
    ಯದಿ(user_data == ರಿಕ್ತ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_ಮೌಲ್ಯ = INT32_MIN;
    user_data->max_ಮೌಲ್ಯ = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    ಪ್ರತ್ಯಾಗಮ user_data;
}

ಶೂನ್ಯ lv_obj_set_subject_increment_event_min_ಮೌಲ್ಯ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_ಮೌಲ್ಯ)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    dsc->min_ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
    ಯದಿ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ಯದಿ(dsc->subject->ಮೌಲ್ಯ.ಅಂಕೆ < min_ಮೌಲ್ಯ) {
            lv_subject_set_int(dsc->subject, min_ಮೌಲ್ಯ);
        }
    }
#ಯದಿ LV_USE_FLOAT
    ಅನ್ಯಥಾ ಯದಿ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ಯದಿ(dsc->subject->ಮೌಲ್ಯ.float_v < (ಭಿನ್ನ)min_ಮೌಲ್ಯ) {
            lv_subject_set_float(dsc->subject, (ಭಿನ್ನ)min_ಮೌಲ್ಯ);
        }
    }
#endif
}

ಶೂನ್ಯ lv_obj_set_subject_increment_event_max_ಮೌಲ್ಯ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_ಮೌಲ್ಯ)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    dsc->max_ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
    ಯದಿ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ಯದಿ(dsc->subject->ಮೌಲ್ಯ.ಅಂಕೆ > max_ಮೌಲ್ಯ) {
            lv_subject_set_int(dsc->subject, max_ಮೌಲ್ಯ);
        }
    }
#ಯದಿ LV_USE_FLOAT
    ಅನ್ಯಥಾ ಯದಿ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ಯದಿ(dsc->subject->ಮೌಲ್ಯ.float_v > (ಭಿನ್ನ)max_ಮೌಲ್ಯ) {
            lv_subject_set_float(dsc->subject, (ಭಿನ್ನ)max_ಮೌಲ್ಯ);
        }
    }
#endif
}

ಶೂನ್ಯ lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    dsc->rollover = rollover;
}

ಶೂನ್ಯ lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

ಶೂನ್ಯ lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ);

    subject_set_int_user_data_t * user_data = lv_malloc(ಪರಿಮಾಣ(subject_set_int_user_data_t));
    ಯದಿ(user_data == ರಿಕ್ತ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಪ್ರತ್ಯಾಗಮ;
    }

    user_data->subject = subject;
    user_data->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#ಯದಿ LV_USE_FLOAT
ಶೂನ್ಯ lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, ಭಿನ್ನ ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ);

    subject_set_float_user_data_t * user_data = lv_malloc(ಪರಿಮಾಣ(subject_set_float_user_data_t));
    ಯದಿ(user_data == ರಿಕ್ತ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಪ್ರತ್ಯಾಗಮ;
    }

    user_data->subject = subject;
    user_data->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

ಶೂನ್ಯ lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         ಧ್ರುವ ವರ್ಣ * ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    subject_set_string_user_data_t * user_data = lv_malloc(ಪರಿಮಾಣ(subject_set_string_user_data_t));
    ಯದಿ(user_data == ರಿಕ್ತ) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಪ್ರತ್ಯಾಗಮ;
    }

    user_data->subject = subject;
    user_data->ಮೌಲ್ಯ = lv_strdup(ಮೌಲ್ಯ);
    ಯದಿ(user_data->ಮೌಲ್ಯ == ರಿಕ್ತ) {
        LV_ASSERT_MALLOC(user_data->ಮೌಲ್ಯ);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_bool_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_bool_cb;
    set_bool_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_int_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_int_cb;
    set_int_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}

#ಯದಿ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_float_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_float_cb;
    set_float_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_string_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_string_cb;
    set_string_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_color_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_color_cb;
    set_color_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(set_pointer_cb != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, ರಿಕ್ತ);
    ಯದಿ(observable == ರಿಕ್ತ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))set_pointer_cb;
    set_pointer_observer(observable, subject);
    ಪ್ರತ್ಯಾಗಮ observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(subject->type == LV_SUBJECT_TYPE_INT, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_ಮೌಲ್ಯ_changed_event_cb, LV_EVENT_ಮೌಲ್ಯ_CHANGED, subject);

    ಪ್ರತ್ಯಾಗಮ observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ (lv_obj_t *)lv_observer_get_target(observer);
}

ಶೂನ್ಯ * lv_observer_get_user_data(ಧ್ರುವ lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ observer->user_data;
}

ಶೂನ್ಯ lv_observer_set_user_data(lv_observer_t * observer, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(observer != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    /* Release internally-owned data only when replacing it. */
    ಯದಿ(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    lv_memzero(subject, ಪರಿಮಾಣ(*subject));
    lv_ll_init(&(subject->subs_ll), ಪರಿಮಾಣ(lv_observer_t));
}
ಸ್ಥಿರ ಶೂನ್ಯ init_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    subject->min_ಮೌಲ್ಯ.ಅಂಕೆ = INT32_MIN;
    subject->max_ಮೌಲ್ಯ.ಅಂಕೆ = INT32_MAX;
}

#ಯದಿ LV_USE_FLOAT

ಸ್ಥಿರ ಶೂನ್ಯ init_float(lv_subject_t * subject, ಭಿನ್ನ ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    subject->min_ಮೌಲ್ಯ.float_v = -FLT_MAX;
    subject->max_ಮೌಲ್ಯ.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ init_pointer(lv_subject_t * subject, ಶೂನ್ಯ * ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->ಮೌಲ್ಯ.pointer = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.pointer = ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->ಮೌಲ್ಯ.color = color;
    subject->prev_ಮೌಲ್ಯ.color = color;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(list != ರಿಕ್ತ || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

ಸ್ಥಿರ ಶೂನ್ಯ add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(list != ರಿಕ್ತ || size == 0);
    subject->ಮೌಲ್ಯ.pointer = list;
    subject->size = (uint32_t)size;

    ಪ್ರತಿ(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ init_string(lv_subject_t * subject, ವರ್ಣ * buf, ವರ್ಣ * prev_buf, size_t size, ಧ್ರುವ ವರ್ಣ * ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(buf != ರಿಕ್ತ || size == 0);

    ಯದಿ(size > 0) {
        lv_strlcpy(buf, ಮೌಲ್ಯ, size);
        ಯದಿ(prev_buf) lv_strlcpy(prev_buf, ಮೌಲ್ಯ, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->ಮೌಲ್ಯ.pointer = buf;
    subject->prev_ಮೌಲ್ಯ.pointer = prev_buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    ಯದಿ(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    ಯಾವತ್(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#ಯದಿ LV_USE_EXT_DATA
    ಯದಿ(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = ರಿಕ್ತ;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->ಮೌಲ್ಯ);
}


#ಯದಿ LV_USE_FLOAT
ಸ್ಥಿರ ಶೂನ್ಯ subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->ಮೌಲ್ಯ);
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    ಯದಿ(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_ಮೌಲ್ಯ = LV_MIN(user_data->max_ಮೌಲ್ಯ, user_data->subject->max_ಮೌಲ್ಯ.ಅಂಕೆ);
        int32_t min_ಮೌಲ್ಯ = LV_MAX(user_data->min_ಮೌಲ್ಯ, user_data->subject->min_ಮೌಲ್ಯ.ಅಂಕೆ);

        int32_t ಮೌಲ್ಯ = lv_subject_get_int(user_data->subject);
        ಮೌಲ್ಯ += user_data->step;

        ಯದಿ(user_data->rollover) {
            ಯದಿ(ಮೌಲ್ಯ > max_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
            }
            ಅನ್ಯಥಾ ಯದಿ(ಮೌಲ್ಯ < min_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
            }
        }
        ಅನ್ಯಥಾ {
            ಮೌಲ್ಯ = LV_CLAMP(min_ಮೌಲ್ಯ, ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);
        }

        lv_subject_set_int(user_data->subject, ಮೌಲ್ಯ);
    }
#ಯದಿ LV_USE_FLOAT
    ಅನ್ಯಥಾ ಯದಿ(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ಭಿನ್ನ max_ಮೌಲ್ಯ = LV_MIN((ಭಿನ್ನ)user_data->max_ಮೌಲ್ಯ, user_data->subject->max_ಮೌಲ್ಯ.float_v);
        ಭಿನ್ನ min_ಮೌಲ್ಯ = LV_MAX((ಭಿನ್ನ)user_data->min_ಮೌಲ್ಯ, user_data->subject->min_ಮೌಲ್ಯ.float_v);


        ಭಿನ್ನ ಮೌಲ್ಯ = lv_subject_get_float(user_data->subject);
        ಮೌಲ್ಯ += (ಭಿನ್ನ)user_data->step;

        ಯದಿ(user_data->rollover) {
            ಯದಿ(ಮೌಲ್ಯ > max_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
            }
            ಅನ್ಯಥಾ ಯದಿ(ಮೌಲ್ಯ < min_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
            }
        }
        ಅನ್ಯಥಾ {
            ಮೌಲ್ಯ = LV_CLAMP(min_ಮೌಲ್ಯ, ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);
        }

        lv_subject_set_float(user_data->subject, ಮೌಲ್ಯ);
    }
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ರಿಕ್ತ);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

ಸ್ಥಿರ ಶೂನ್ಯ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

ಸ್ಥಿರ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(obj != ರಿಕ್ತ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(ಪರಿಮಾಣ(bound_state_t));
    ಯದಿ(p == ರಿಕ್ತ) {
        LV_LOG_WARN("Out of memory");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    ಪ್ರತ್ಯಾಗಮ observable;
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ರಿಕ್ತ);
    LV_ASSERT(subject != ರಿಕ್ತ);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != ರಿಕ್ತ);

    ಯದಿ(subject->ಮೌಲ್ಯ.ಅಂಕೆ) {
        lv_obj_add_state(observer->target, p->state);
    }
    ಅನ್ಯಥಾ {
        lv_obj_remove_state(observer->target, p->state);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_ಮೌಲ್ಯ_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(obj != ರಿಕ್ತ);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ಸ್ಥಿರ ಶೂನ್ಯ lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    ವಿಕಲ್ಪ(subject->type) {
        ಪಕ್ಷ LV_SUBJECT_TYPE_INT :
            ಯದಿ(subject->ಮೌಲ್ಯ.ಅಂಕೆ != subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ) {
                lv_subject_notify(subject);
            }
            ಭಙ್ಗ;
#ಯದಿ LV_USE_FLOAT
        ಪಕ್ಷ LV_SUBJECT_TYPE_FLOAT :
            ಯದಿ(subject->ಮೌಲ್ಯ.float_v != subject->prev_ಮೌಲ್ಯ.float_v) {
                lv_subject_notify(subject);
            }
            ಭಙ್ಗ;
#endif
        ಪಕ್ಷ LV_SUBJECT_TYPE_GROUP :
        ಪಕ್ಷ LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_COLOR  :
            ಯದಿ(!lv_color_eq(subject->ಮೌಲ್ಯ.color, subject->prev_ಮೌಲ್ಯ.color)) {
                lv_subject_notify(subject);
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_STRING:
            ಯದಿ(!subject->prev_ಮೌಲ್ಯ.pointer ||
               lv_strcmp(subject->ಮೌಲ್ಯ.pointer, subject->prev_ಮೌಲ್ಯ.pointer)) {
                lv_subject_notify(subject);
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ LV_SUBJECT_TYPE_INVALID :
        ಪಕ್ಷ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ರಿಕ್ತ);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != ರಿಕ್ತ);
    lv_free((ಶೂನ್ಯ *)user_data->ಮೌಲ್ಯ);
    lv_free(user_data);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    ಯದಿ(set_bool_cb) set_bool_cb(obj, subject->ಮೌಲ್ಯ.ಅಂಕೆ);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    ಯದಿ(set_int_cb) set_int_cb(obj, subject->ಮೌಲ್ಯ.ಅಂಕೆ);
}

#ಯದಿ LV_USE_FLOAT
ಸ್ಥಿರ ಶೂನ್ಯ set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    ಯದಿ(set_float_cb) set_float_cb(obj, subject->ಮೌಲ್ಯ.float_v);
}
#endif /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    ಯದಿ(set_string_cb) set_string_cb(obj, subject->ಮೌಲ್ಯ.pointer);
}


ಸ್ಥಿರ ಶೂನ್ಯ set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    ಯದಿ(set_color_cb) set_color_cb(obj, subject->ಮೌಲ್ಯ.color);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    ಯದಿ(set_pointer_cb) set_pointer_cb(obj, subject->ಮೌಲ್ಯ.pointer);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    ಪ್ರತಿ(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->ಮೌಲ್ಯ.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ಶೂನ್ಯ * user_data)
{
    LV_ASSERT(subject != ರಿಕ್ತ);
    LV_ASSERT(cb != ರಿಕ್ತ);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    ಯಾವತ್(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        ಯದಿ(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
