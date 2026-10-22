/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ಅನ್ತರ್ಭಾವ "lv_observer_private.h"
#ಆದರೆ LV_USE_OBSERVER

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#ವ್ಯಾಖ್ಯಾನಿಸು subject_list LV_GLOBAL_DEFAULT()->subject_ll

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FLT_MAX
    #ವ್ಯಾಖ್ಯಾನಿಸು FLT_MAX 3.402823466e+38F
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *      TYPEDEFS
 **********************/
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_state_t state;
} bound_state_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_subject_t * subject;
    int32_t ಮೌಲ್ಯ;
} subject_ಗಣ_int_user_ದತ್ತಾಂಶ_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_subject_t * subject;
    ದಶಮಾಂಶ ಮೌಲ್ಯ;
} subject_ಗಣ_float_user_ದತ್ತಾಂಶ_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_subject_t * subject;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮೌಲ್ಯ;
} subject_ಗಣ_string_user_ದತ್ತಾಂಶ_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ remove_observers_from_ಗುಂಪು_subject(lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_observers_ಗಮ್ಯ_ಗುಂಪು_subject(lv_subject_t * subject, lv_subject_t ** list, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಶೂನ್ಯ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಶೂನ್ಯ subject_toggle_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_int_cb(lv_event_t * e);
#ಆದರೆ LV_USE_FLOAT
    ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_float_cb(lv_event_t * e);
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_string_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ subject_increment_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ unsubscribe_on_delete_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ ಗುಂಪು_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ಸ್ಥಿರ ಶೂನ್ಯ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ obj_ಮೌಲ್ಯ_ಬದಲಾಯಿತು_event_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ lv_subject_notify_if_ಬದಲಾಯಿತು(lv_subject_t * subject);

ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_string_free_user_ದತ್ತಾಂಶ_event_cb(lv_event_t * e);

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಸೂಚಕ_observer(lv_observer_t * observer, lv_subject_t * subject);

#ಆದರೆ LV_USE_FLOAT
    ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ಸ್ಥಿರ ಶೂನ್ಯ init_float(lv_subject_t * subject, ದಶಮಾಂಶ ಮೌಲ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ init_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_string(lv_subject_t * subject, ಅಕ್ಷರ * buf, ಅಕ್ಷರ * prev_buf, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_ಸೂಚಕ(lv_subject_t * subject, ಶೂನ್ಯ * ಮೌಲ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ init_color(lv_subject_t * subject, lv_color_t color);
ಸ್ಥಿರ ಶೂನ್ಯ init_ಗುಂಪು(lv_subject_t * subject, lv_subject_t ** list, ಗಾತ್ರ_t ಗಾತ್ರ_2);
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
    lv_ll_init(&subject_list, ಗಾತ್ರ(lv_subject_t));
}
ಶೂನ್ಯ lv_subject_global_deinit(ಶೂನ್ಯ)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        ಆದರೆ(curr->type == LV_SUBJECT_TYPE_ಗುಂಪು) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        ಆದರೆ(curr->type != LV_SUBJECT_TYPE_ಗುಂಪು) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    lv_subject_t * subject = lv_ll_ins_ಬಾಲ(&subject_list);
    LV_ASSERT_MALLOC(subject);
    ಆದರೆ(!subject) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    init_common(subject);

    ಆಯ್ಕೆ(type) {
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_FLOAT:
#ಆದರೆ LV_USE_FLOAT
            init_float(subject, 0.f);
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_ಸೂಚಕ:
            init_ಸೂಚಕ(subject, ಖಾಲಿ);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_ಗುಂಪು:
            init_ಗುಂಪು(subject, ಖಾಲಿ, 0);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_STRING:
            init_string(subject, ಖಾಲಿ, ಖಾಲಿ, 0, "");
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_INVALID:
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    ಹಿಂತಿರುಗಿ subject;
}

ಶೂನ್ಯ lv_subject_delete(lv_subject_t * subject)
{
    ಆದರೆ(!subject) {
        ಹಿಂತಿರುಗಿ;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
ಶೂನ್ಯ lv_subject_ಗಣ_external_ದತ್ತಾಂಶ(lv_subject_t * subject, ಶೂನ್ಯ * ದತ್ತಾಂಶ, ಶೂನ್ಯ (* free_cb)(ಶೂನ್ಯ * ದತ್ತಾಂಶ))
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    subject->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    subject->ext_ದತ್ತಾಂಶ.free_cb = free_cb;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ lv_subject_ಗಣ_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ);

    ಮೌಲ್ಯ = LV_CLAMP(subject->min_ಮೌಲ್ಯ.ಅಂಕೆ, ಮೌಲ್ಯ, subject->max_ಮೌಲ್ಯ.ಅಂಕೆ);

    subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ = subject->ಮೌಲ್ಯ.ಅಂಕೆ;
    subject->ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ subject->ಮೌಲ್ಯ.ಅಂಕೆ;
}

int32_t lv_subject_get_ಹಿಂದಿನ_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ;
}

ಶೂನ್ಯ lv_subject_ಗಣ_min_ಮೌಲ್ಯ_int(lv_subject_t * subject, int32_t min_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ);

    subject->min_ಮೌಲ್ಯ.ಅಂಕೆ = min_ಮೌಲ್ಯ;
}

ಶೂನ್ಯ lv_subject_ಗಣ_max_ಮೌಲ್ಯ_int(lv_subject_t * subject, int32_t max_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ);

    subject->max_ಮೌಲ್ಯ.ಅಂಕೆ = max_ಮೌಲ್ಯ;
}

#ಆದರೆ LV_USE_FLOAT

ಶೂನ್ಯ lv_subject_ಗಣ_float(lv_subject_t * subject, ದಶಮಾಂಶ ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ);

    ಮೌಲ್ಯ = LV_CLAMP(subject->min_ಮೌಲ್ಯ.float_v, ಮೌಲ್ಯ, subject->max_ಮೌಲ್ಯ.float_v);

    subject->prev_ಮೌಲ್ಯ.float_v = subject->ಮೌಲ್ಯ.float_v;
    subject->ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

ದಶಮಾಂಶ lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ 0.0);

    ಹಿಂತಿರುಗಿ subject->ಮೌಲ್ಯ.float_v;
}

ದಶಮಾಂಶ lv_subject_get_ಹಿಂದಿನ_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ 0.0);

    ಹಿಂತಿರುಗಿ subject->prev_ಮೌಲ್ಯ.float_v;
}

ಶೂನ್ಯ lv_subject_ಗಣ_min_ಮೌಲ್ಯ_float(lv_subject_t * subject, ದಶಮಾಂಶ min_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ);

    subject->min_ಮೌಲ್ಯ.float_v = min_ಮೌಲ್ಯ;
}

ಶೂನ್ಯ lv_subject_ಗಣ_max_ಮೌಲ್ಯ_float(lv_subject_t * subject, ದಶಮಾಂಶ max_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ);

    subject->max_ಮೌಲ್ಯ.float_v = max_ಮೌಲ್ಯ;
}


#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/

ಶೂನ್ಯ lv_subject_ಗಣ_string_ಬಫರ್_static(lv_subject_t * subject, ಅಕ್ಷರ * buf, ಅಕ್ಷರ * prev_buf, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(buf != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(ಗಾತ್ರ_2 > 0, ಹಿಂತಿರುಗಿ);

    buf[0] = '\0';
    ಆದರೆ(prev_buf) prev_buf[0] = '\0';

    subject->ಗಾತ್ರ_2 = (uint32_t)ಗಾತ್ರ_2;
    subject->ಮೌಲ್ಯ.ಸೂಚಕ = buf;
    subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ = prev_buf;
}

ಶೂನ್ಯ lv_subject_ಗಣ_string(lv_subject_t * subject, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * buf)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(buf != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(subject->ಗಾತ್ರ_2 == 0) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ) {
        lv_strlcpy((ಅಕ್ಷರ *)subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ, subject->ಮೌಲ್ಯ.ಸೂಚಕ, subject->ಗಾತ್ರ_2);
    }

    lv_strlcpy((ಅಕ್ಷರ *)subject->ಮೌಲ್ಯ.ಸೂಚಕ, buf, subject->ಗಾತ್ರ_2);

    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

ಶೂನ್ಯ lv_subject_snprintf(lv_subject_t * subject, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * format, ...)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(format != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(subject->ಗಾತ್ರ_2 < 1U) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ) {
        lv_strlcpy((ಅಕ್ಷರ *)subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ, subject->ಮೌಲ್ಯ.ಸೂಚಕ, subject->ಗಾತ್ರ_2);
    }

    va_list va;
    va_ಆರಂಭ(va, format);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ret = lv_vsnprintf((ಅಕ್ಷರ *)subject->ಮೌಲ್ಯ.ಸೂಚಕ, subject->ಗಾತ್ರ_2, format, va);
    LV_ಬಳಸದ(ret);
    va_ಕೊನೆ(va);

    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ subject->ಮೌಲ್ಯ.ಸೂಚಕ;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * lv_subject_get_ಹಿಂದಿನ_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ;
}

ಶೂನ್ಯ lv_subject_ಗಣ_ಸೂಚಕ(lv_subject_t * subject, ಶೂನ್ಯ * ptr)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಸೂಚಕ, ಹಿಂತಿರುಗಿ);

    subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ = subject->ಮೌಲ್ಯ.ಸೂಚಕ;
    subject->ಮೌಲ್ಯ.ಸೂಚಕ = ptr;
    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * lv_subject_get_ಸೂಚಕ(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಸೂಚಕ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);


    ಹಿಂತಿರುಗಿ subject->ಮೌಲ್ಯ.ಸೂಚಕ;
}

ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * lv_subject_get_ಹಿಂದಿನ_ಸೂಚಕ(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಸೂಚಕ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ;
}

ಶೂನ್ಯ lv_subject_ಗಣ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಹಿಂತಿರುಗಿ);

    subject->prev_ಮೌಲ್ಯ.color = subject->ಮೌಲ್ಯ.color;
    subject->ಮೌಲ್ಯ.color = color;
    lv_subject_notify_if_ಬದಲಾಯಿತು(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಹಿಂತಿರುಗಿ lv_color_black());

    ಹಿಂತಿರುಗಿ subject->ಮೌಲ್ಯ.color;
}

lv_color_t lv_subject_get_ಹಿಂದಿನ_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಹಿಂತಿರುಗಿ lv_color_black());

    ಹಿಂತಿರುಗಿ subject->prev_ಮೌಲ್ಯ.color;
}

ಶೂನ್ಯ lv_subject_ಗಣ_ಗುಂಪು_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(list != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಗುಂಪು, ಹಿಂತಿರುಗಿ);

    ಗಾಗಿ(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(list[i] != ಖಾಲಿ, ಹಿಂತಿರುಗಿ, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_ಗುಂಪು_subject(subject);
    ಕೂಡಿಸು_observers_ಗಮ್ಯ_ಗುಂಪು_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_ಗುಂಪು_element(lv_subject_t * subject, int32_t ಸೂಚ್ಯಂಕ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಗುಂಪು, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಸೂಚ್ಯಂಕ >= 0 && ಸೂಚ್ಯಂಕ < subject->ಗಾತ್ರ_2, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ ((lv_subject_t **)(subject->ಮೌಲ್ಯ.ಸೂಚಕ))[ಸೂಚ್ಯಂಕ];
}

lv_observer_t * lv_subject_ಕೂಡಿಸು_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(observer_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observer = lv_subject_ಕೂಡಿಸು_observer_obj(subject, observer_cb, ಖಾಲಿ, user_ದತ್ತಾಂಶ);
    ಆದರೆ(observer == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    observer->for_obj = 0;
    ಹಿಂತಿರುಗಿ observer;
}

lv_observer_t * lv_subject_ಕೂಡಿಸು_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(observer_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observer = lv_ll_ins_ಬಾಲ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ಆದರೆ(observer == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    lv_memzero(observer, ಗಾತ್ರ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
    observer->ಗುರಿ = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    ಆದರೆ(obj != ಖಾಲಿ) {
        lv_obj_ಕೂಡಿಸು_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ಹಿಂತಿರುಗಿ observer;
}

lv_observer_t * lv_subject_ಕೂಡಿಸು_observer_with_ಗುರಿ(lv_subject_t * subject, lv_observer_cb_t observer_cb, ಶೂನ್ಯ * ಗುರಿ,
                                                    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(observer_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observer = lv_ll_ins_ಬಾಲ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ಆದರೆ(observer == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    lv_memzero(observer, ಗಾತ್ರ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
    observer->ಗುರಿ = ಗುರಿ;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ಹಿಂತಿರುಗಿ observer;
}


ಶೂನ್ಯ lv_observer_delete(lv_observer_t * observer)
{
    ಆದರೆ(observer == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(observer->for_obj && observer->ಗುರಿ) {
        lv_obj_remove_event_cb_with_user_ದತ್ತಾಂಶ(observer->ಗುರಿ, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_ದತ್ತಾಂಶ(observer->ಗುರಿ, ಖಾಲಿ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    ಆದರೆ(observer->auto_free_user_ದತ್ತಾಂಶ) {
        lv_free(observer->user_ದತ್ತಾಂಶ);
    }
    lv_free(observer);
}

ಶೂನ್ಯ lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
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
    ಗಾಗಿ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        ಆದರೆ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_ದತ್ತಾಂಶ;
            ಆದರೆ(subject == ಖಾಲಿ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_ದತ್ತಾಂಶ(obj, ಖಾಲಿ, subject);

}

ಶೂನ್ಯ * lv_observer_get_ಗುರಿ(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ observer->ಗುರಿ;
}

ಶೂನ್ಯ lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    ಮಾಡು {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            ಆದರೆ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                ಆದರೆ(subject->notify_restart_query) ನಿಲ್ಲಿಸು;
                observer->notified = 1;
            }
        }
    } ಆಗ(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_ಕೂಡಿಸು_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_ಸಂಕೇತ_t trigger, int32_t ಹೆಜ್ಜೆ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_subject_increment_dsc_t * user_ದತ್ತಾಂಶ = lv_malloc(ಗಾತ್ರ(lv_subject_increment_dsc_t));
    ಆದರೆ(user_ದತ್ತಾಂಶ == ಖಾಲಿ) {
        LV_ASSERT_MALLOC(user_ದತ್ತಾಂಶ);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    user_ದತ್ತಾಂಶ->ಹೆಜ್ಜೆ = ಹೆಜ್ಜೆ;
    user_ದತ್ತಾಂಶ->subject = subject;
    user_ದತ್ತಾಂಶ->rollover = false;
    user_ದತ್ತಾಂಶ->min_ಮೌಲ್ಯ = INT32_MIN;
    user_ದತ್ತಾಂಶ->max_ಮೌಲ್ಯ = INT32_MAX;
    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_increment_cb, trigger, user_ದತ್ತಾಂಶ);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, user_ದತ್ತಾಂಶ);

    ಹಿಂತಿರುಗಿ user_ದತ್ತಾಂಶ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_subject_increment_event_min_ಮೌಲ್ಯ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_ಮೌಲ್ಯ)
{
    LV_ಬಳಸದ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    dsc->min_ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
    ಆದರೆ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ಆದರೆ(dsc->subject->ಮೌಲ್ಯ.ಅಂಕೆ < min_ಮೌಲ್ಯ) {
            lv_subject_ಗಣ_int(dsc->subject, min_ಮೌಲ್ಯ);
        }
    }
#ಆದರೆ LV_USE_FLOAT
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ಆದರೆ(dsc->subject->ಮೌಲ್ಯ.float_v < (ದಶಮಾಂಶ)min_ಮೌಲ್ಯ) {
            lv_subject_ಗಣ_float(dsc->subject, (ದಶಮಾಂಶ)min_ಮೌಲ್ಯ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ lv_obj_ಗಣ_subject_increment_event_max_ಮೌಲ್ಯ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_ಮೌಲ್ಯ)
{
    LV_ಬಳಸದ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    dsc->max_ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
    ಆದರೆ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ಆದರೆ(dsc->subject->ಮೌಲ್ಯ.ಅಂಕೆ > max_ಮೌಲ್ಯ) {
            lv_subject_ಗಣ_int(dsc->subject, max_ಮೌಲ್ಯ);
        }
    }
#ಆದರೆ LV_USE_FLOAT
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ಆದರೆ(dsc->subject->ಮೌಲ್ಯ.float_v > (ದಶಮಾಂಶ)max_ಮೌಲ್ಯ) {
            lv_subject_ಗಣ_float(dsc->subject, (ದಶಮಾಂಶ)max_ಮೌಲ್ಯ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ lv_obj_ಗಣ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_ಬಳಸದ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    dsc->rollover = rollover;
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ಸಂಕೇತ_t trigger)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ);

    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_toggle_cb, trigger, subject);
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_subject_ಗಣ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ಸಂಕೇತ_t trigger, int32_t ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ);

    subject_ಗಣ_int_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_malloc(ಗಾತ್ರ(subject_ಗಣ_int_user_ದತ್ತಾಂಶ_t));
    ಆದರೆ(user_ದತ್ತಾಂಶ == ಖಾಲಿ) {
        LV_ASSERT_MALLOC(user_ದತ್ತಾಂಶ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಹಿಂತಿರುಗಿ;
    }

    user_ದತ್ತಾಂಶ->subject = subject;
    user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_ಗಣ_int_cb, trigger, user_ದತ್ತಾಂಶ);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, user_ದತ್ತಾಂಶ);
}

#ಆದರೆ LV_USE_FLOAT
ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_subject_ಗಣ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ಸಂಕೇತ_t trigger, ದಶಮಾಂಶ ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ);

    subject_ಗಣ_float_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_malloc(ಗಾತ್ರ(subject_ಗಣ_float_user_ದತ್ತಾಂಶ_t));
    ಆದರೆ(user_ದತ್ತಾಂಶ == ಖಾಲಿ) {
        LV_ASSERT_MALLOC(user_ದತ್ತಾಂಶ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಹಿಂತಿರುಗಿ;
    }

    user_ದತ್ತಾಂಶ->subject = subject;
    user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ = ಮೌಲ್ಯ;

    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_ಗಣ_float_cb, trigger, user_ದತ್ತಾಂಶ);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, user_ದತ್ತಾಂಶ);
}
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_subject_ಗಣ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ಸಂಕೇತ_t trigger,
                                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    subject_ಗಣ_string_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_malloc(ಗಾತ್ರ(subject_ಗಣ_string_user_ದತ್ತಾಂಶ_t));
    ಆದರೆ(user_ದತ್ತಾಂಶ == ಖಾಲಿ) {
        LV_ASSERT_MALLOC(user_ದತ್ತಾಂಶ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ಹಿಂತಿರುಗಿ;
    }

    user_ದತ್ತಾಂಶ->subject = subject;
    user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ = lv_strdup(ಮೌಲ್ಯ);
    ಆದರೆ(user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ == ಖಾಲಿ) {
        LV_ASSERT_MALLOC(user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ;
    }

    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_ಗಣ_string_cb, trigger, user_ದತ್ತಾಂಶ);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, subject_ಗಣ_string_free_user_ದತ್ತಾಂಶ_event_cb, LV_EVENT_DELETE, user_ದತ್ತಾಂಶ);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_bool_t ಗಣ_bool_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_bool_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_bool_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_bool_cb;
    ಗಣ_bool_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_int_t ಗಣ_int_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_int_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_int_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_int_cb;
    ಗಣ_int_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}

#ಆದರೆ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_float_t ಗಣ_float_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_float_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_float_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_float_cb;
    ಗಣ_float_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}
#ಆದರೆ_ಅಂತ್ಯ

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_string_t ಗಣ_string_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_string_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_string_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_string_cb;
    ಗಣ_string_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_color_t ಗಣ_color_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_color_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_color_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_color_cb;
    ಗಣ_color_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}

lv_observer_t * lv_obj_bind_ಸೂಚಕ(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ಗಣ_ಸೂಚಕ_t ಗಣ_ಸೂಚಕ_cb)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(ಗಣ_ಸೂಚಕ_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ಸೂಚಕ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, ಗಣ_ಸೂಚಕ_observer, obj, ಖಾಲಿ);
    ಆದರೆ(observable == ಖಾಲಿ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ಶೂನ್ಯ (*)(ಶೂನ್ಯ))ಗಣ_ಸೂಚಕ_cb;
    ಗಣ_ಸೂಚಕ_observer(observable, subject);
    ಹಿಂತಿರುಗಿ observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG_FORMAT_ಸಂದೇಶ(subject->type == LV_SUBJECT_TYPE_INT, ಹಿಂತಿರುಗಿ ಖಾಲಿ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_ಕೂಡಿಸು_event_cb(obj, obj_ಮೌಲ್ಯ_ಬದಲಾಯಿತು_event_cb, LV_EVENT_ಮೌಲ್ಯ_ಬದಲಾಯಿತು, subject);

    ಹಿಂತಿರುಗಿ observable;
}


lv_obj_t * lv_observer_get_ಗುರಿ_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ (lv_obj_t *)lv_observer_get_ಗುರಿ(observer);
}

ಶೂನ್ಯ * lv_observer_get_user_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ observer->user_ದತ್ತಾಂಶ;
}

ಶೂನ್ಯ lv_observer_ಗಣ_user_ದತ್ತಾಂಶ(lv_observer_t * observer, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_ARG(observer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    /* Release internally-owned data only when replacing it. */
    ಆದರೆ(observer->auto_free_user_ದತ್ತಾಂಶ && observer->user_ದತ್ತಾಂಶ != user_ದತ್ತಾಂಶ) {
        lv_free(observer->user_ದತ್ತಾಂಶ);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
    observer->auto_free_user_ದತ್ತಾಂಶ = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    lv_memzero(subject, ಗಾತ್ರ(*subject));
    lv_ll_init(&(subject->subs_ll), ಗಾತ್ರ(lv_observer_t));
}
ಸ್ಥಿರ ಶೂನ್ಯ init_int(lv_subject_t * subject, int32_t ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ = ಮೌಲ್ಯ;
    subject->min_ಮೌಲ್ಯ.ಅಂಕೆ = INT32_MIN;
    subject->max_ಮೌಲ್ಯ.ಅಂಕೆ = INT32_MAX;
}

#ಆದರೆ LV_USE_FLOAT

ಸ್ಥಿರ ಶೂನ್ಯ init_float(lv_subject_t * subject, ದಶಮಾಂಶ ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.float_v = ಮೌಲ್ಯ;
    subject->min_ಮೌಲ್ಯ.float_v = -FLT_MAX;
    subject->max_ಮೌಲ್ಯ.float_v = FLT_MAX;
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ init_ಸೂಚಕ(lv_subject_t * subject, ಶೂನ್ಯ * ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    subject->type = LV_SUBJECT_TYPE_ಸೂಚಕ;
    subject->ಮೌಲ್ಯ.ಸೂಚಕ = ಮೌಲ್ಯ;
    subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ = ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->ಮೌಲ್ಯ.color = color;
    subject->prev_ಮೌಲ್ಯ.color = color;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_ಗುಂಪು(lv_subject_t * subject, lv_subject_t ** list, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(list != ಖಾಲಿ || ಗಾತ್ರ_2 == 0);
    subject->type = LV_SUBJECT_TYPE_ಗುಂಪು;
    ಕೂಡಿಸು_observers_ಗಮ್ಯ_ಗುಂಪು_subject(subject, list, ಗಾತ್ರ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_observers_ಗಮ್ಯ_ಗುಂಪು_subject(lv_subject_t * subject, lv_subject_t ** list, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(list != ಖಾಲಿ || ಗಾತ್ರ_2 == 0);
    subject->ಮೌಲ್ಯ.ಸೂಚಕ = list;
    subject->ಗಾತ್ರ_2 = (uint32_t)ಗಾತ್ರ_2;

    ಗಾಗಿ(uint32_t i = 0; i < ಗಾತ್ರ_2; i++) {
        lv_subject_ಕೂಡಿಸು_observer(list[i], ಗುಂಪು_notify_cb, subject);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ init_string(lv_subject_t * subject, ಅಕ್ಷರ * buf, ಅಕ್ಷರ * prev_buf, ಗಾತ್ರ_t ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಮೌಲ್ಯ)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(buf != ಖಾಲಿ || ಗಾತ್ರ_2 == 0);

    ಆದರೆ(ಗಾತ್ರ_2 > 0) {
        lv_strlcpy(buf, ಮೌಲ್ಯ, ಗಾತ್ರ_2);
        ಆದರೆ(prev_buf) lv_strlcpy(prev_buf, ಮೌಲ್ಯ, ಗಾತ್ರ_2);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->ಗಾತ್ರ_2 = (uint32_t)ಗಾತ್ರ_2;
    subject->ಮೌಲ್ಯ.ಸೂಚಕ = buf;
    subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ = prev_buf;
}

ಸ್ಥಿರ ಶೂನ್ಯ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    ಆದರೆ(subject->type == LV_SUBJECT_TYPE_ಗುಂಪು) {
        remove_observers_from_ಗುಂಪು_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_ತಲೆ(&subject->subs_ll);
    ಆಗ(observer) {
        lv_observer_t * observer_ಮುಂದಿನ = lv_ll_get_ಮುಂದಿನ(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_ಮುಂದಿನ;
    }

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    ಆದರೆ(subject->ext_ದತ್ತಾಂಶ.free_cb) {
        subject->ext_ದತ್ತಾಂಶ.free_cb(subject->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ);
        subject->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    lv_ll_clear(&subject->subs_ll);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    lv_subject_t * subject = lv_event_get_user_ದತ್ತಾಂಶ(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_ಗಣ_int(subject, v);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    subject_ಗಣ_int_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_event_get_user_ದತ್ತಾಂಶ(e);
    lv_subject_ಗಣ_int(user_ದತ್ತಾಂಶ->subject, user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ);
}


#ಆದರೆ LV_USE_FLOAT
ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    subject_ಗಣ_float_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_event_get_user_ದತ್ತಾಂಶ(e);
    lv_subject_ಗಣ_float(user_ದತ್ತಾಂಶ->subject, user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ);
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    subject_ಗಣ_string_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_event_get_user_ದತ್ತಾಂಶ(e);
    lv_subject_ಗಣ_string(user_ದತ್ತಾಂಶ->subject, user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    lv_subject_increment_dsc_t * user_ದತ್ತಾಂಶ = lv_event_get_user_ದತ್ತಾಂಶ(e);

    ಆದರೆ(user_ದತ್ತಾಂಶ->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_ಮೌಲ್ಯ = LV_MIN(user_ದತ್ತಾಂಶ->max_ಮೌಲ್ಯ, user_ದತ್ತಾಂಶ->subject->max_ಮೌಲ್ಯ.ಅಂಕೆ);
        int32_t min_ಮೌಲ್ಯ = LV_MAX(user_ದತ್ತಾಂಶ->min_ಮೌಲ್ಯ, user_ದತ್ತಾಂಶ->subject->min_ಮೌಲ್ಯ.ಅಂಕೆ);

        int32_t ಮೌಲ್ಯ = lv_subject_get_int(user_ದತ್ತಾಂಶ->subject);
        ಮೌಲ್ಯ += user_ದತ್ತಾಂಶ->ಹೆಜ್ಜೆ;

        ಆದರೆ(user_ದತ್ತಾಂಶ->rollover) {
            ಆದರೆ(ಮೌಲ್ಯ > max_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಮೌಲ್ಯ < min_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಮೌಲ್ಯ = LV_CLAMP(min_ಮೌಲ್ಯ, ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);
        }

        lv_subject_ಗಣ_int(user_ದತ್ತಾಂಶ->subject, ಮೌಲ್ಯ);
    }
#ಆದರೆ LV_USE_FLOAT
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(user_ದತ್ತಾಂಶ->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ದಶಮಾಂಶ max_ಮೌಲ್ಯ = LV_MIN((ದಶಮಾಂಶ)user_ದತ್ತಾಂಶ->max_ಮೌಲ್ಯ, user_ದತ್ತಾಂಶ->subject->max_ಮೌಲ್ಯ.float_v);
        ದಶಮಾಂಶ min_ಮೌಲ್ಯ = LV_MAX((ದಶಮಾಂಶ)user_ದತ್ತಾಂಶ->min_ಮೌಲ್ಯ, user_ದತ್ತಾಂಶ->subject->min_ಮೌಲ್ಯ.float_v);


        ದಶಮಾಂಶ ಮೌಲ್ಯ = lv_subject_get_float(user_ದತ್ತಾಂಶ->subject);
        ಮೌಲ್ಯ += (ದಶಮಾಂಶ)user_ದತ್ತಾಂಶ->ಹೆಜ್ಜೆ;

        ಆದರೆ(user_ದತ್ತಾಂಶ->rollover) {
            ಆದರೆ(ಮೌಲ್ಯ > max_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = min_ಮೌಲ್ಯ;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಮೌಲ್ಯ < min_ಮೌಲ್ಯ) {
                ಮೌಲ್ಯ = max_ಮೌಲ್ಯ;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಮೌಲ್ಯ = LV_CLAMP(min_ಮೌಲ್ಯ, ಮೌಲ್ಯ, max_ಮೌಲ್ಯ);
        }

        lv_subject_ಗಣ_float(user_ದತ್ತಾಂಶ->subject, ಮೌಲ್ಯ);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗುಂಪು_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ಖಾಲಿ);
    LV_ಬಳಸದ(subject);
    lv_subject_t * subject_ಗುಂಪು = observer->user_ದತ್ತಾಂಶ;
    lv_subject_notify(subject_ಗುಂಪು);
}

ಸ್ಥಿರ ಶೂನ್ಯ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    lv_observer_t * observer = lv_event_get_user_ದತ್ತಾಂಶ(e);
    lv_observer_delete(observer);
}

ಸ್ಥಿರ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(obj != ಖಾಲಿ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(ಗಾತ್ರ(bound_state_t));
    ಆದರೆ(p == ಖಾಲಿ) {
        LV_LOG_WARN("Out of memory");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_ದತ್ತಾಂಶ = 1;

    ಹಿಂತಿರುಗಿ observable;
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ಖಾಲಿ);
    LV_ASSERT(subject != ಖಾಲಿ);
    bound_state_t * p = observer->user_ದತ್ತಾಂಶ;
    LV_ASSERT(p != ಖಾಲಿ);

    ಆದರೆ(subject->ಮೌಲ್ಯ.ಅಂಕೆ) {
        lv_obj_ಕೂಡಿಸು_state(observer->ಗುರಿ, p->state);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_obj_remove_state(observer->ಗುರಿ, p->state);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_ಮೌಲ್ಯ_ಬದಲಾಯಿತು_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    lv_obj_t * obj = lv_event_get_current_ಗುರಿ(e);
    lv_subject_t * subject = lv_event_get_user_ದತ್ತಾಂಶ(e);
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(obj != ಖಾಲಿ);

    lv_subject_ಗಣ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ಸ್ಥಿರ ಶೂನ್ಯ lv_subject_notify_if_ಬದಲಾಯಿತು(lv_subject_t * subject)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    ಆಯ್ಕೆ(subject->type) {
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_INT :
            ಆದರೆ(subject->ಮೌಲ್ಯ.ಅಂಕೆ != subject->prev_ಮೌಲ್ಯ.ಅಂಕೆ) {
                lv_subject_notify(subject);
            }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ LV_USE_FLOAT
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_FLOAT :
            ಆದರೆ(subject->ಮೌಲ್ಯ.float_v != subject->prev_ಮೌಲ್ಯ.float_v) {
                lv_subject_notify(subject);
            }
            ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_ಗುಂಪು :
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_ಸೂಚಕ :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_COLOR  :
            ಆದರೆ(!lv_color_eq(subject->ಮೌಲ್ಯ.color, subject->prev_ಮೌಲ್ಯ.color)) {
                lv_subject_notify(subject);
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_STRING:
            ಆದರೆ(!subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ ||
               lv_strcmp(subject->ಮೌಲ್ಯ.ಸೂಚಕ, subject->prev_ಮೌಲ್ಯ.ಸೂಚಕ)) {
                lv_subject_notify(subject);
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_INVALID :
        ಸ್ಥಿತಿ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_ಗಣ_string_free_user_ದತ್ತಾಂಶ_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    subject_ಗಣ_string_user_ದತ್ತಾಂಶ_t * user_ದತ್ತಾಂಶ = lv_event_get_user_ದತ್ತಾಂಶ(e);
    LV_ASSERT(user_ದತ್ತಾಂಶ != ಖಾಲಿ);
    lv_free((ಶೂನ್ಯ *)user_ದತ್ತಾಂಶ->ಮೌಲ್ಯ);
    lv_free(user_ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_bool_t ಗಣ_bool_cb = (lv_obj_ಗಣ_bool_t)observer->user_cb;
    ಆದರೆ(ಗಣ_bool_cb) ಗಣ_bool_cb(obj, subject->ಮೌಲ್ಯ.ಅಂಕೆ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_int_t ಗಣ_int_cb = (lv_obj_ಗಣ_int_t)observer->user_cb;
    ಆದರೆ(ಗಣ_int_cb) ಗಣ_int_cb(obj, subject->ಮೌಲ್ಯ.ಅಂಕೆ);
}

#ಆದರೆ LV_USE_FLOAT
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_float_t ಗಣ_float_cb = (lv_obj_ಗಣ_float_t)observer->user_cb;
    ಆದರೆ(ಗಣ_float_cb) ಗಣ_float_cb(obj, subject->ಮೌಲ್ಯ.float_v);
}
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_FLOAT*/

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_string_t ಗಣ_string_cb = (lv_obj_ಗಣ_string_t)observer->user_cb;
    ಆದರೆ(ಗಣ_string_cb) ಗಣ_string_cb(obj, subject->ಮೌಲ್ಯ.ಸೂಚಕ);
}


ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_color_t ಗಣ_color_cb = (lv_obj_ಗಣ_color_t)observer->user_cb;
    ಆದರೆ(ಗಣ_color_cb) ಗಣ_color_cb(obj, subject->ಮೌಲ್ಯ.color);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಸೂಚಕ_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ಗುರಿ;
    lv_obj_ಗಣ_ಸೂಚಕ_t ಗಣ_ಸೂಚಕ_cb = (lv_obj_ಗಣ_ಸೂಚಕ_t)observer->user_cb;
    ಆದರೆ(ಗಣ_ಸೂಚಕ_cb) ಗಣ_ಸೂಚಕ_cb(obj, subject->ಮೌಲ್ಯ.ಸೂಚಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_observers_from_ಗುಂಪು_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_ಗುಂಪು);

    ಗಾಗಿ(uint32_t i = 0; i < subject->ಗಾತ್ರ_2; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->ಮೌಲ್ಯ.ಸೂಚಕ))[i];
        subject_remove_observer(sub, ಗುಂಪು_notify_cb, subject);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_ASSERT(subject != ಖಾಲಿ);
    LV_ASSERT(cb != ಖಾಲಿ);

    lv_observer_t * obs = lv_ll_get_ತಲೆ(&subject->subs_ll);
    ಆಗ(obs) {
        lv_observer_t * obs_ಮುಂದಿನ = lv_ll_get_ಮುಂದಿನ(&subject->subs_ll, obs);
        ಆದರೆ(obs->cb == cb && obs->user_ದತ್ತಾಂಶ == user_ದತ್ತಾಂಶ) {
            lv_observer_delete(obs);
        }
        obs = obs_ಮುಂದಿನ;
    }
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBSERVER*/
