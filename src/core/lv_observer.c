/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "lv_observer_private.h"
#ਜੇ LV_USE_OBSERVER

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ subject_list LV_GLOBAL_DEFAULT()->subject_ll

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FLT_MAX
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLT_MAX 3.402823466e+38F
#ਜੇ_ਅੰਤ

/**********************
 *      TYPEDEFS
 **********************/
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_state_t state;
} bound_state_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_subject_t * subject;
    int32_t ਮੁੱਲ;
} subject_ਸੈੱਟ_int_user_ਅੰਕੜੇ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_subject_t * subject;
    ਦਸ਼ਮਲਵ ਮੁੱਲ;
} subject_ਸੈੱਟ_float_user_ਅੰਕੜੇ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_subject_t * subject;
    ਅਚਲ ਅੱਖਰ * ਮੁੱਲ;
} subject_ਸੈੱਟ_string_user_ਅੰਕੜੇ_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ਸਥਿਰ ਖਾਲੀ remove_observers_from_ਸਮੂਹ_subject(lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_observers_ਮੰਜ਼ਿਲ_ਸਮੂਹ_subject(lv_subject_t * subject, lv_subject_t ** list, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ subject_toggle_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_int_cb(lv_event_t * e);
#ਜੇ LV_USE_FLOAT
    ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_float_cb(lv_event_t * e);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_string_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ subject_increment_cb(lv_event_t * e);

ਸਥਿਰ ਖਾਲੀ unsubscribe_on_delete_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ ਸਮੂਹ_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ਸਥਿਰ ਖਾਲੀ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ obj_ਮੁੱਲ_ਬਦਲਿਆ_event_cb(lv_event_t * e);

ਸਥਿਰ ਖਾਲੀ lv_subject_notify_if_ਬਦਲਿਆ(lv_subject_t * subject);

ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_string_free_user_ਅੰਕੜੇ_event_cb(lv_event_t * e);

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਸੂਚਕ_observer(lv_observer_t * observer, lv_subject_t * subject);

#ਜੇ LV_USE_FLOAT
    ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ਸਥਿਰ ਖਾਲੀ init_float(lv_subject_t * subject, ਦਸ਼ਮਲਵ ਮੁੱਲ);
#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/

ਸਥਿਰ ਖਾਲੀ init_int(lv_subject_t * subject, int32_t ਮੁੱਲ);
ਸਥਿਰ ਖਾਲੀ init_string(lv_subject_t * subject, ਅੱਖਰ * buf, ਅੱਖਰ * prev_buf, ਮਾਪ_t ਮਾਪ, ਅਚਲ ਅੱਖਰ * ਮੁੱਲ);
ਸਥਿਰ ਖਾਲੀ init_ਸੂਚਕ(lv_subject_t * subject, ਖਾਲੀ * ਮੁੱਲ);
ਸਥਿਰ ਖਾਲੀ init_color(lv_subject_t * subject, lv_color_t color);
ਸਥਿਰ ਖਾਲੀ init_ਸਮੂਹ(lv_subject_t * subject, lv_subject_t ** list, ਮਾਪ_t ਮਾਪ);
ਸਥਿਰ ਖਾਲੀ init_common(lv_subject_t * subject);

ਸਥਿਰ ਖਾਲੀ deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ਖਾਲੀ lv_subject_global_init(ਖਾਲੀ)
{
    lv_ll_init(&subject_list, ਆਕਾਰ(lv_subject_t));
}
ਖਾਲੀ lv_subject_global_deinit(ਖਾਲੀ)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        ਜੇ(curr->type == LV_SUBJECT_TYPE_ਸਮੂਹ) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        ਜੇ(curr->type != LV_SUBJECT_TYPE_ਸਮੂਹ) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ ਨਿਰਾ);
    lv_subject_t * subject = lv_ll_ins_ਪੂਛ(&subject_list);
    LV_ASSERT_MALLOC(subject);
    ਜੇ(!subject) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    init_common(subject);

    ਚੋਣ(type) {
        ਹਾਲਤ LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_FLOAT:
#ਜੇ LV_USE_FLOAT
            init_float(subject, 0.f);
#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_ਸੂਚਕ:
            init_ਸੂਚਕ(subject, ਨਿਰਾ);
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_ਸਮੂਹ:
            init_ਸਮੂਹ(subject, ਨਿਰਾ, 0);
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_STRING:
            init_string(subject, ਨਿਰਾ, ਨਿਰਾ, 0, "");
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_INVALID:
        ਹਾਲਤ LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    ਵਾਪਸ subject;
}

ਖਾਲੀ lv_subject_delete(lv_subject_t * subject)
{
    ਜੇ(!subject) {
        ਵਾਪਸ;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
ਖਾਲੀ lv_subject_ਸੈੱਟ_external_ਅੰਕੜੇ(lv_subject_t * subject, ਖਾਲੀ * ਅੰਕੜੇ, ਖਾਲੀ (* free_cb)(ਖਾਲੀ * ਅੰਕੜੇ))
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);

    subject->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਅੰਕੜੇ;
    subject->ext_ਅੰਕੜੇ.free_cb = free_cb;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ lv_subject_ਸੈੱਟ_int(lv_subject_t * subject, int32_t ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ);

    ਮੁੱਲ = LV_CLAMP(subject->min_ਮੁੱਲ.ਅੰਕ, ਮੁੱਲ, subject->max_ਮੁੱਲ.ਅੰਕ);

    subject->prev_ਮੁੱਲ.ਅੰਕ = subject->ਮੁੱਲ.ਅੰਕ;
    subject->ਮੁੱਲ.ਅੰਕ = ਮੁੱਲ;
    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ 0);

    ਵਾਪਸ subject->ਮੁੱਲ.ਅੰਕ;
}

int32_t lv_subject_get_ਪਿਛਲਾ_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ 0);

    ਵਾਪਸ subject->prev_ਮੁੱਲ.ਅੰਕ;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_min_ਮੁੱਲ_int(lv_subject_t * subject, int32_t min_ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ);

    subject->min_ਮੁੱਲ.ਅੰਕ = min_ਮੁੱਲ;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_max_ਮੁੱਲ_int(lv_subject_t * subject, int32_t max_ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ);

    subject->max_ਮੁੱਲ.ਅੰਕ = max_ਮੁੱਲ;
}

#ਜੇ LV_USE_FLOAT

ਖਾਲੀ lv_subject_ਸੈੱਟ_float(lv_subject_t * subject, ਦਸ਼ਮਲਵ ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ);

    ਮੁੱਲ = LV_CLAMP(subject->min_ਮੁੱਲ.float_v, ਮੁੱਲ, subject->max_ਮੁੱਲ.float_v);

    subject->prev_ਮੁੱਲ.float_v = subject->ਮੁੱਲ.float_v;
    subject->ਮੁੱਲ.float_v = ਮੁੱਲ;
    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

ਦਸ਼ਮਲਵ lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ 0.0);

    ਵਾਪਸ subject->ਮੁੱਲ.float_v;
}

ਦਸ਼ਮਲਵ lv_subject_get_ਪਿਛਲਾ_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ 0.0);

    ਵਾਪਸ subject->prev_ਮੁੱਲ.float_v;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_min_ਮੁੱਲ_float(lv_subject_t * subject, ਦਸ਼ਮਲਵ min_ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ);

    subject->min_ਮੁੱਲ.float_v = min_ਮੁੱਲ;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_max_ਮੁੱਲ_float(lv_subject_t * subject, ਦਸ਼ਮਲਵ max_ਮੁੱਲ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ);

    subject->max_ਮੁੱਲ.float_v = max_ਮੁੱਲ;
}


#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/

ਖਾਲੀ lv_subject_ਸੈੱਟ_string_ਬਫਰ_static(lv_subject_t * subject, ਅੱਖਰ * buf, ਅੱਖਰ * prev_buf, ਮਾਪ_t ਮਾਪ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ);
    LV_CHECK_ARG(buf != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(ਮਾਪ > 0, ਵਾਪਸ);

    buf[0] = '\0';
    ਜੇ(prev_buf) prev_buf[0] = '\0';

    subject->ਮਾਪ = (uint32_t)ਮਾਪ;
    subject->ਮੁੱਲ.ਸੂਚਕ = buf;
    subject->prev_ਮੁੱਲ.ਸੂਚਕ = prev_buf;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_string(lv_subject_t * subject, ਅਚਲ ਅੱਖਰ * buf)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ);
    LV_CHECK_ARG(buf != ਨਿਰਾ, ਵਾਪਸ);

    ਜੇ(subject->ਮਾਪ == 0) ਵਾਪਸ;
    ਜੇ(subject->prev_ਮੁੱਲ.ਸੂਚਕ) {
        lv_strlcpy((ਅੱਖਰ *)subject->prev_ਮੁੱਲ.ਸੂਚਕ, subject->ਮੁੱਲ.ਸੂਚਕ, subject->ਮਾਪ);
    }

    lv_strlcpy((ਅੱਖਰ *)subject->ਮੁੱਲ.ਸੂਚਕ, buf, subject->ਮਾਪ);

    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

ਖਾਲੀ lv_subject_snprintf(lv_subject_t * subject, ਅਚਲ ਅੱਖਰ * format, ...)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ);
    LV_CHECK_ARG(format != ਨਿਰਾ, ਵਾਪਸ);

    ਜੇ(subject->ਮਾਪ < 1U) ਵਾਪਸ;

    ਜੇ(subject->prev_ਮੁੱਲ.ਸੂਚਕ) {
        lv_strlcpy((ਅੱਖਰ *)subject->prev_ਮੁੱਲ.ਸੂਚਕ, subject->ਮੁੱਲ.ਸੂਚਕ, subject->ਮਾਪ);
    }

    va_list va;
    va_ਸ਼ੁਰੂ(va, format);
    ਅਚਲ ਪੂਰਨ ret = lv_vsnprintf((ਅੱਖਰ *)subject->ਮੁੱਲ.ਸੂਚਕ, subject->ਮਾਪ, format, va);
    LV_ਅਣਵਰਤਿਆ(ret);
    va_ਸਿਰਾ(va);

    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

ਅਚਲ ਅੱਖਰ * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ subject->ਮੁੱਲ.ਸੂਚਕ;
}

ਅਚਲ ਅੱਖਰ * lv_subject_get_ਪਿਛਲਾ_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ subject->prev_ਮੁੱਲ.ਸੂਚਕ;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_ਸੂਚਕ(lv_subject_t * subject, ਖਾਲੀ * ptr)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸੂਚਕ, ਵਾਪਸ);

    subject->prev_ਮੁੱਲ.ਸੂਚਕ = subject->ਮੁੱਲ.ਸੂਚਕ;
    subject->ਮੁੱਲ.ਸੂਚਕ = ptr;
    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

ਅਚਲ ਖਾਲੀ * lv_subject_get_ਸੂਚਕ(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸੂਚਕ, ਵਾਪਸ ਨਿਰਾ);


    ਵਾਪਸ subject->ਮੁੱਲ.ਸੂਚਕ;
}

ਅਚਲ ਖਾਲੀ * lv_subject_get_ਪਿਛਲਾ_ਸੂਚਕ(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸੂਚਕ, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ subject->prev_ਮੁੱਲ.ਸੂਚਕ;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ਵਾਪਸ);

    subject->prev_ਮੁੱਲ.color = subject->ਮੁੱਲ.color;
    subject->ਮੁੱਲ.color = color;
    lv_subject_notify_if_ਬਦਲਿਆ(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ਵਾਪਸ lv_color_black());

    ਵਾਪਸ subject->ਮੁੱਲ.color;
}

lv_color_t lv_subject_get_ਪਿਛਲਾ_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ਵਾਪਸ lv_color_black());

    ਵਾਪਸ subject->prev_ਮੁੱਲ.color;
}

ਖਾਲੀ lv_subject_ਸੈੱਟ_ਸਮੂਹ_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(list != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸਮੂਹ, ਵਾਪਸ);

    ਲਈ(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(list[i] != ਨਿਰਾ, ਵਾਪਸ, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_ਸਮੂਹ_subject(subject);
    ਜੋੜੋ_observers_ਮੰਜ਼ਿਲ_ਸਮੂਹ_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_ਸਮੂਹ_element(lv_subject_t * subject, int32_t ਸੂਚਕਾਂਕ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸਮੂਹ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੂਚਕਾਂਕ >= 0 && ਸੂਚਕਾਂਕ < subject->ਮਾਪ, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ ((lv_subject_t **)(subject->ਮੁੱਲ.ਸੂਚਕ))[ਸੂਚਕਾਂਕ];
}

lv_observer_t * lv_subject_ਜੋੜੋ_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(observer_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observer = lv_subject_ਜੋੜੋ_observer_obj(subject, observer_cb, ਨਿਰਾ, user_ਅੰਕੜੇ);
    ਜੇ(observer == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    observer->for_obj = 0;
    ਵਾਪਸ observer;
}

lv_observer_t * lv_subject_ਜੋੜੋ_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(observer_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observer = lv_ll_ins_ਪੂਛ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ਜੇ(observer == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    lv_memzero(observer, ਆਕਾਰ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
    observer->ਨਿਸ਼ਾਨਾ = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    ਜੇ(obj != ਨਿਰਾ) {
        lv_obj_ਜੋੜੋ_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ਵਾਪਸ observer;
}

lv_observer_t * lv_subject_ਜੋੜੋ_observer_with_ਨਿਸ਼ਾਨਾ(lv_subject_t * subject, lv_observer_cb_t observer_cb, ਖਾਲੀ * ਨਿਸ਼ਾਨਾ,
                                                    ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(observer_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observer = lv_ll_ins_ਪੂਛ(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ਜੇ(observer == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    lv_memzero(observer, ਆਕਾਰ(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
    observer->ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ਵਾਪਸ observer;
}


ਖਾਲੀ lv_observer_delete(lv_observer_t * observer)
{
    ਜੇ(observer == ਨਿਰਾ) ਵਾਪਸ;

    ਜੇ(observer->for_obj && observer->ਨਿਸ਼ਾਨਾ) {
        lv_obj_remove_event_cb_with_user_ਅੰਕੜੇ(observer->ਨਿਸ਼ਾਨਾ, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_ਅੰਕੜੇ(observer->ਨਿਸ਼ਾਨਾ, ਨਿਰਾ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    ਜੇ(observer->auto_free_user_ਅੰਕੜੇ) {
        lv_free(observer->user_ਅੰਕੜੇ);
    }
    lv_free(observer);
}

ਖਾਲੀ lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_ਗਿਣਤੀ(&obj->spec_attr->event_list) : 0);
    ਲਈ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        ਜੇ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_ਅੰਕੜੇ;
            ਜੇ(subject == ਨਿਰਾ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_ਅੰਕੜੇ(obj, ਨਿਰਾ, subject);

}

ਖਾਲੀ * lv_observer_get_ਨਿਸ਼ਾਨਾ(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ observer->ਨਿਸ਼ਾਨਾ;
}

ਖਾਲੀ lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    ਕਰੋ {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            ਜੇ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                ਜੇ(subject->notify_restart_query) ਰੋਕੋ;
                observer->notified = 1;
            }
        }
    } ਜਦੋਂ(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_ਜੋੜੋ_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_ਕੋਡ_t trigger, int32_t ਕਦਮ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ ਨਿਰਾ);

    lv_subject_increment_dsc_t * user_ਅੰਕੜੇ = lv_malloc(ਆਕਾਰ(lv_subject_increment_dsc_t));
    ਜੇ(user_ਅੰਕੜੇ == ਨਿਰਾ) {
        LV_ASSERT_MALLOC(user_ਅੰਕੜੇ);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        ਵਾਪਸ ਨਿਰਾ;
    }

    user_ਅੰਕੜੇ->ਕਦਮ = ਕਦਮ;
    user_ਅੰਕੜੇ->subject = subject;
    user_ਅੰਕੜੇ->rollover = false;
    user_ਅੰਕੜੇ->min_ਮੁੱਲ = INT32_MIN;
    user_ਅੰਕੜੇ->max_ਮੁੱਲ = INT32_MAX;
    lv_obj_ਜੋੜੋ_event_cb(obj, subject_increment_cb, trigger, user_ਅੰਕੜੇ);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, user_ਅੰਕੜੇ);

    ਵਾਪਸ user_ਅੰਕੜੇ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_subject_increment_event_min_ਮੁੱਲ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_ਮੁੱਲ)
{
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    LV_CHECK_ARG(dsc != ਨਿਰਾ, ਵਾਪਸ);

    dsc->min_ਮੁੱਲ = min_ਮੁੱਲ;
    ਜੇ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ਜੇ(dsc->subject->ਮੁੱਲ.ਅੰਕ < min_ਮੁੱਲ) {
            lv_subject_ਸੈੱਟ_int(dsc->subject, min_ਮੁੱਲ);
        }
    }
#ਜੇ LV_USE_FLOAT
    ਨਹੀਂਤਾਂ ਜੇ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ਜੇ(dsc->subject->ਮੁੱਲ.float_v < (ਦਸ਼ਮਲਵ)min_ਮੁੱਲ) {
            lv_subject_ਸੈੱਟ_float(dsc->subject, (ਦਸ਼ਮਲਵ)min_ਮੁੱਲ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_subject_increment_event_max_ਮੁੱਲ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_ਮੁੱਲ)
{
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    LV_CHECK_ARG(dsc != ਨਿਰਾ, ਵਾਪਸ);

    dsc->max_ਮੁੱਲ = max_ਮੁੱਲ;
    ਜੇ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ਜੇ(dsc->subject->ਮੁੱਲ.ਅੰਕ > max_ਮੁੱਲ) {
            lv_subject_ਸੈੱਟ_int(dsc->subject, max_ਮੁੱਲ);
        }
    }
#ਜੇ LV_USE_FLOAT
    ਨਹੀਂਤਾਂ ਜੇ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ਜੇ(dsc->subject->ਮੁੱਲ.float_v > (ਦਸ਼ਮਲਵ)max_ਮੁੱਲ) {
            lv_subject_ਸੈੱਟ_float(dsc->subject, (ਦਸ਼ਮਲਵ)max_ਮੁੱਲ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    LV_CHECK_ARG(dsc != ਨਿਰਾ, ਵਾਪਸ);

    dsc->rollover = rollover;
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ਕੋਡ_t trigger)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ);

    lv_obj_ਜੋੜੋ_event_cb(obj, subject_toggle_cb, trigger, subject);
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_subject_ਸੈੱਟ_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ਕੋਡ_t trigger, int32_t ਮੁੱਲ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ);

    subject_ਸੈੱਟ_int_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_malloc(ਆਕਾਰ(subject_ਸੈੱਟ_int_user_ਅੰਕੜੇ_t));
    ਜੇ(user_ਅੰਕੜੇ == ਨਿਰਾ) {
        LV_ASSERT_MALLOC(user_ਅੰਕੜੇ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ਵਾਪਸ;
    }

    user_ਅੰਕੜੇ->subject = subject;
    user_ਅੰਕੜੇ->ਮੁੱਲ = ਮੁੱਲ;

    lv_obj_ਜੋੜੋ_event_cb(obj, subject_ਸੈੱਟ_int_cb, trigger, user_ਅੰਕੜੇ);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, user_ਅੰਕੜੇ);
}

#ਜੇ LV_USE_FLOAT
ਖਾਲੀ lv_obj_ਜੋੜੋ_subject_ਸੈੱਟ_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ਕੋਡ_t trigger, ਦਸ਼ਮਲਵ ਮੁੱਲ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ);

    subject_ਸੈੱਟ_float_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_malloc(ਆਕਾਰ(subject_ਸੈੱਟ_float_user_ਅੰਕੜੇ_t));
    ਜੇ(user_ਅੰਕੜੇ == ਨਿਰਾ) {
        LV_ASSERT_MALLOC(user_ਅੰਕੜੇ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ਵਾਪਸ;
    }

    user_ਅੰਕੜੇ->subject = subject;
    user_ਅੰਕੜੇ->ਮੁੱਲ = ਮੁੱਲ;

    lv_obj_ਜੋੜੋ_event_cb(obj, subject_ਸੈੱਟ_float_cb, trigger, user_ਅੰਕੜੇ);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, user_ਅੰਕੜੇ);
}
#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/

ਖਾਲੀ lv_obj_ਜੋੜੋ_subject_ਸੈੱਟ_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_ਕੋਡ_t trigger,
                                         ਅਚਲ ਅੱਖਰ * ਮੁੱਲ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ);
    LV_CHECK_ARG(ਮੁੱਲ != ਨਿਰਾ, ਵਾਪਸ);

    subject_ਸੈੱਟ_string_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_malloc(ਆਕਾਰ(subject_ਸੈੱਟ_string_user_ਅੰਕੜੇ_t));
    ਜੇ(user_ਅੰਕੜੇ == ਨਿਰਾ) {
        LV_ASSERT_MALLOC(user_ਅੰਕੜੇ);
        LV_LOG_WARN("Couldn't allocate user_data");
        ਵਾਪਸ;
    }

    user_ਅੰਕੜੇ->subject = subject;
    user_ਅੰਕੜੇ->ਮੁੱਲ = lv_strdup(ਮੁੱਲ);
    ਜੇ(user_ਅੰਕੜੇ->ਮੁੱਲ == ਨਿਰਾ) {
        LV_ASSERT_MALLOC(user_ਅੰਕੜੇ->ਮੁੱਲ);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_ਅੰਕੜੇ);
        ਵਾਪਸ;
    }

    lv_obj_ਜੋੜੋ_event_cb(obj, subject_ਸੈੱਟ_string_cb, trigger, user_ਅੰਕੜੇ);
    lv_obj_ਜੋੜੋ_event_cb(obj, subject_ਸੈੱਟ_string_free_user_ਅੰਕੜੇ_event_cb, LV_EVENT_DELETE, user_ਅੰਕੜੇ);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_bool_t ਸੈੱਟ_bool_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_bool_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_bool_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_bool_cb;
    ਸੈੱਟ_bool_observer(observable, subject);
    ਵਾਪਸ observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_int_t ਸੈੱਟ_int_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_int_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ ਨਿਰਾ);
    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_int_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_int_cb;
    ਸੈੱਟ_int_observer(observable, subject);
    ਵਾਪਸ observable;
}

#ਜੇ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_float_t ਸੈੱਟ_float_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_float_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_float_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_float_cb;
    ਸੈੱਟ_float_observer(observable, subject);
    ਵਾਪਸ observable;
}
#ਜੇ_ਅੰਤ

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_string_t ਸੈੱਟ_string_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_string_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_string_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_string_cb;
    ਸੈੱਟ_string_observer(observable, subject);
    ਵਾਪਸ observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_color_t ਸੈੱਟ_color_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_color_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_color_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_color_cb;
    ਸੈੱਟ_color_observer(observable, subject);
    ਵਾਪਸ observable;
}

lv_observer_t * lv_obj_bind_ਸੂਚਕ(lv_obj_t * obj, lv_subject_t * subject, lv_obj_ਸੈੱਟ_ਸੂਚਕ_t ਸੈੱਟ_ਸੂਚਕ_cb)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਸੈੱਟ_ਸੂਚਕ_cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_ਸੂਚਕ, ਵਾਪਸ ਨਿਰਾ);

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, ਸੈੱਟ_ਸੂਚਕ_observer, obj, ਨਿਰਾ);
    ਜੇ(observable == ਨਿਰਾ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ਖਾਲੀ (*)(ਖਾਲੀ))ਸੈੱਟ_ਸੂਚਕ_cb;
    ਸੈੱਟ_ਸੂਚਕ_observer(observable, subject);
    ਵਾਪਸ observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG_FORMAT_ਸੁਨੇਹਾ(subject->type == LV_SUBJECT_TYPE_INT, ਵਾਪਸ ਨਿਰਾ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_ਜੋੜੋ_event_cb(obj, obj_ਮੁੱਲ_ਬਦਲਿਆ_event_cb, LV_EVENT_ਮੁੱਲ_ਬਦਲਿਆ, subject);

    ਵਾਪਸ observable;
}


lv_obj_t * lv_observer_get_ਨਿਸ਼ਾਨਾ_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ (lv_obj_t *)lv_observer_get_ਨਿਸ਼ਾਨਾ(observer);
}

ਖਾਲੀ * lv_observer_get_user_ਅੰਕੜੇ(ਅਚਲ lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ observer->user_ਅੰਕੜੇ;
}

ਖਾਲੀ lv_observer_ਸੈੱਟ_user_ਅੰਕੜੇ(lv_observer_t * observer, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(observer != ਨਿਰਾ, ਵਾਪਸ);

    /* Release internally-owned data only when replacing it. */
    ਜੇ(observer->auto_free_user_ਅੰਕੜੇ && observer->user_ਅੰਕੜੇ != user_ਅੰਕੜੇ) {
        lv_free(observer->user_ਅੰਕੜੇ);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
    observer->auto_free_user_ਅੰਕੜੇ = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਖਾਲੀ init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    lv_memzero(subject, ਆਕਾਰ(*subject));
    lv_ll_init(&(subject->subs_ll), ਆਕਾਰ(lv_observer_t));
}
ਸਥਿਰ ਖਾਲੀ init_int(lv_subject_t * subject, int32_t ਮੁੱਲ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->ਮੁੱਲ.ਅੰਕ = ਮੁੱਲ;
    subject->prev_ਮੁੱਲ.ਅੰਕ = ਮੁੱਲ;
    subject->min_ਮੁੱਲ.ਅੰਕ = INT32_MIN;
    subject->max_ਮੁੱਲ.ਅੰਕ = INT32_MAX;
}

#ਜੇ LV_USE_FLOAT

ਸਥਿਰ ਖਾਲੀ init_float(lv_subject_t * subject, ਦਸ਼ਮਲਵ ਮੁੱਲ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->ਮੁੱਲ.float_v = ਮੁੱਲ;
    subject->prev_ਮੁੱਲ.float_v = ਮੁੱਲ;
    subject->min_ਮੁੱਲ.float_v = -FLT_MAX;
    subject->max_ਮੁੱਲ.float_v = FLT_MAX;
}

#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/

ਸਥਿਰ ਖਾਲੀ init_ਸੂਚਕ(lv_subject_t * subject, ਖਾਲੀ * ਮੁੱਲ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    subject->type = LV_SUBJECT_TYPE_ਸੂਚਕ;
    subject->ਮੁੱਲ.ਸੂਚਕ = ਮੁੱਲ;
    subject->prev_ਮੁੱਲ.ਸੂਚਕ = ਮੁੱਲ;
}

ਸਥਿਰ ਖਾਲੀ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->ਮੁੱਲ.color = color;
    subject->prev_ਮੁੱਲ.color = color;
}

ਸਥਿਰ ਖਾਲੀ init_ਸਮੂਹ(lv_subject_t * subject, lv_subject_t ** list, ਮਾਪ_t ਮਾਪ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(list != ਨਿਰਾ || ਮਾਪ == 0);
    subject->type = LV_SUBJECT_TYPE_ਸਮੂਹ;
    ਜੋੜੋ_observers_ਮੰਜ਼ਿਲ_ਸਮੂਹ_subject(subject, list, ਮਾਪ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_observers_ਮੰਜ਼ਿਲ_ਸਮੂਹ_subject(lv_subject_t * subject, lv_subject_t ** list, ਮਾਪ_t ਮਾਪ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(list != ਨਿਰਾ || ਮਾਪ == 0);
    subject->ਮੁੱਲ.ਸੂਚਕ = list;
    subject->ਮਾਪ = (uint32_t)ਮਾਪ;

    ਲਈ(uint32_t i = 0; i < ਮਾਪ; i++) {
        lv_subject_ਜੋੜੋ_observer(list[i], ਸਮੂਹ_notify_cb, subject);
    }
}

ਸਥਿਰ ਖਾਲੀ init_string(lv_subject_t * subject, ਅੱਖਰ * buf, ਅੱਖਰ * prev_buf, ਮਾਪ_t ਮਾਪ, ਅਚਲ ਅੱਖਰ * ਮੁੱਲ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(buf != ਨਿਰਾ || ਮਾਪ == 0);

    ਜੇ(ਮਾਪ > 0) {
        lv_strlcpy(buf, ਮੁੱਲ, ਮਾਪ);
        ਜੇ(prev_buf) lv_strlcpy(prev_buf, ਮੁੱਲ, ਮਾਪ);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->ਮਾਪ = (uint32_t)ਮਾਪ;
    subject->ਮੁੱਲ.ਸੂਚਕ = buf;
    subject->prev_ਮੁੱਲ.ਸੂਚਕ = prev_buf;
}

ਸਥਿਰ ਖਾਲੀ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    ਜੇ(subject->type == LV_SUBJECT_TYPE_ਸਮੂਹ) {
        remove_observers_from_ਸਮੂਹ_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_ਸਿਰ(&subject->subs_ll);
    ਜਦੋਂ(observer) {
        lv_observer_t * observer_ਅਗਲਾ = lv_ll_get_ਅਗਲਾ(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_ਅਗਲਾ;
    }

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    ਜੇ(subject->ext_ਅੰਕੜੇ.free_cb) {
        subject->ext_ਅੰਕੜੇ.free_cb(subject->ext_ਅੰਕੜੇ.ਅੰਕੜੇ);
        subject->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਨਿਰਾ;
    }
#ਜੇ_ਅੰਤ

    lv_ll_clear(&subject->subs_ll);
}

ਸਥਿਰ ਖਾਲੀ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    lv_subject_t * subject = lv_event_get_user_ਅੰਕੜੇ(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_ਸੈੱਟ_int(subject, v);
}

ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    subject_ਸੈੱਟ_int_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_event_get_user_ਅੰਕੜੇ(e);
    lv_subject_ਸੈੱਟ_int(user_ਅੰਕੜੇ->subject, user_ਅੰਕੜੇ->ਮੁੱਲ);
}


#ਜੇ LV_USE_FLOAT
ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    subject_ਸੈੱਟ_float_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_event_get_user_ਅੰਕੜੇ(e);
    lv_subject_ਸੈੱਟ_float(user_ਅੰਕੜੇ->subject, user_ਅੰਕੜੇ->ਮੁੱਲ);
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    subject_ਸੈੱਟ_string_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_event_get_user_ਅੰਕੜੇ(e);
    lv_subject_ਸੈੱਟ_string(user_ਅੰਕੜੇ->subject, user_ਅੰਕੜੇ->ਮੁੱਲ);
}

ਸਥਿਰ ਖਾਲੀ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    lv_subject_increment_dsc_t * user_ਅੰਕੜੇ = lv_event_get_user_ਅੰਕੜੇ(e);

    ਜੇ(user_ਅੰਕੜੇ->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_ਮੁੱਲ = LV_MIN(user_ਅੰਕੜੇ->max_ਮੁੱਲ, user_ਅੰਕੜੇ->subject->max_ਮੁੱਲ.ਅੰਕ);
        int32_t min_ਮੁੱਲ = LV_MAX(user_ਅੰਕੜੇ->min_ਮੁੱਲ, user_ਅੰਕੜੇ->subject->min_ਮੁੱਲ.ਅੰਕ);

        int32_t ਮੁੱਲ = lv_subject_get_int(user_ਅੰਕੜੇ->subject);
        ਮੁੱਲ += user_ਅੰਕੜੇ->ਕਦਮ;

        ਜੇ(user_ਅੰਕੜੇ->rollover) {
            ਜੇ(ਮੁੱਲ > max_ਮੁੱਲ) {
                ਮੁੱਲ = min_ਮੁੱਲ;
            }
            ਨਹੀਂਤਾਂ ਜੇ(ਮੁੱਲ < min_ਮੁੱਲ) {
                ਮੁੱਲ = max_ਮੁੱਲ;
            }
        }
        ਨਹੀਂਤਾਂ {
            ਮੁੱਲ = LV_CLAMP(min_ਮੁੱਲ, ਮੁੱਲ, max_ਮੁੱਲ);
        }

        lv_subject_ਸੈੱਟ_int(user_ਅੰਕੜੇ->subject, ਮੁੱਲ);
    }
#ਜੇ LV_USE_FLOAT
    ਨਹੀਂਤਾਂ ਜੇ(user_ਅੰਕੜੇ->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ਦਸ਼ਮਲਵ max_ਮੁੱਲ = LV_MIN((ਦਸ਼ਮਲਵ)user_ਅੰਕੜੇ->max_ਮੁੱਲ, user_ਅੰਕੜੇ->subject->max_ਮੁੱਲ.float_v);
        ਦਸ਼ਮਲਵ min_ਮੁੱਲ = LV_MAX((ਦਸ਼ਮਲਵ)user_ਅੰਕੜੇ->min_ਮੁੱਲ, user_ਅੰਕੜੇ->subject->min_ਮੁੱਲ.float_v);


        ਦਸ਼ਮਲਵ ਮੁੱਲ = lv_subject_get_float(user_ਅੰਕੜੇ->subject);
        ਮੁੱਲ += (ਦਸ਼ਮਲਵ)user_ਅੰਕੜੇ->ਕਦਮ;

        ਜੇ(user_ਅੰਕੜੇ->rollover) {
            ਜੇ(ਮੁੱਲ > max_ਮੁੱਲ) {
                ਮੁੱਲ = min_ਮੁੱਲ;
            }
            ਨਹੀਂਤਾਂ ਜੇ(ਮੁੱਲ < min_ਮੁੱਲ) {
                ਮੁੱਲ = max_ਮੁੱਲ;
            }
        }
        ਨਹੀਂਤਾਂ {
            ਮੁੱਲ = LV_CLAMP(min_ਮੁੱਲ, ਮੁੱਲ, max_ਮੁੱਲ);
        }

        lv_subject_ਸੈੱਟ_float(user_ਅੰਕੜੇ->subject, ਮੁੱਲ);
    }
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ ਸਮੂਹ_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ਨਿਰਾ);
    LV_ਅਣਵਰਤਿਆ(subject);
    lv_subject_t * subject_ਸਮੂਹ = observer->user_ਅੰਕੜੇ;
    lv_subject_notify(subject_ਸਮੂਹ);
}

ਸਥਿਰ ਖਾਲੀ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    lv_observer_t * observer = lv_event_get_user_ਅੰਕੜੇ(e);
    lv_observer_delete(observer);
}

ਸਥਿਰ lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(obj != ਨਿਰਾ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(ਆਕਾਰ(bound_state_t));
    ਜੇ(p == ਨਿਰਾ) {
        LV_LOG_WARN("Out of memory");
        ਵਾਪਸ ਨਿਰਾ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_ਅੰਕੜੇ = 1;

    ਵਾਪਸ observable;
}

ਸਥਿਰ ਖਾਲੀ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ਨਿਰਾ);
    LV_ASSERT(subject != ਨਿਰਾ);
    bound_state_t * p = observer->user_ਅੰਕੜੇ;
    LV_ASSERT(p != ਨਿਰਾ);

    ਜੇ(subject->ਮੁੱਲ.ਅੰਕ) {
        lv_obj_ਜੋੜੋ_state(observer->ਨਿਸ਼ਾਨਾ, p->state);
    }
    ਨਹੀਂਤਾਂ {
        lv_obj_remove_state(observer->ਨਿਸ਼ਾਨਾ, p->state);
    }
}

ਸਥਿਰ ਖਾਲੀ obj_ਮੁੱਲ_ਬਦਲਿਆ_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    lv_obj_t * obj = lv_event_get_current_ਨਿਸ਼ਾਨਾ(e);
    lv_subject_t * subject = lv_event_get_user_ਅੰਕੜੇ(e);
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(obj != ਨਿਰਾ);

    lv_subject_ਸੈੱਟ_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ਸਥਿਰ ਖਾਲੀ lv_subject_notify_if_ਬਦਲਿਆ(lv_subject_t * subject)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    ਚੋਣ(subject->type) {
        ਹਾਲਤ LV_SUBJECT_TYPE_INT :
            ਜੇ(subject->ਮੁੱਲ.ਅੰਕ != subject->prev_ਮੁੱਲ.ਅੰਕ) {
                lv_subject_notify(subject);
            }
            ਰੋਕੋ;
#ਜੇ LV_USE_FLOAT
        ਹਾਲਤ LV_SUBJECT_TYPE_FLOAT :
            ਜੇ(subject->ਮੁੱਲ.float_v != subject->prev_ਮੁੱਲ.float_v) {
                lv_subject_notify(subject);
            }
            ਰੋਕੋ;
#ਜੇ_ਅੰਤ
        ਹਾਲਤ LV_SUBJECT_TYPE_ਸਮੂਹ :
        ਹਾਲਤ LV_SUBJECT_TYPE_ਸੂਚਕ :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_COLOR  :
            ਜੇ(!lv_color_eq(subject->ਮੁੱਲ.color, subject->prev_ਮੁੱਲ.color)) {
                lv_subject_notify(subject);
            }
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_STRING:
            ਜੇ(!subject->prev_ਮੁੱਲ.ਸੂਚਕ ||
               lv_strcmp(subject->ਮੁੱਲ.ਸੂਚਕ, subject->prev_ਮੁੱਲ.ਸੂਚਕ)) {
                lv_subject_notify(subject);
            }
            ਰੋਕੋ;
        ਹਾਲਤ LV_SUBJECT_TYPE_INVALID :
        ਹਾਲਤ LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ਸਥਿਰ ਖਾਲੀ subject_ਸੈੱਟ_string_free_user_ਅੰਕੜੇ_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    subject_ਸੈੱਟ_string_user_ਅੰਕੜੇ_t * user_ਅੰਕੜੇ = lv_event_get_user_ਅੰਕੜੇ(e);
    LV_ASSERT(user_ਅੰਕੜੇ != ਨਿਰਾ);
    lv_free((ਖਾਲੀ *)user_ਅੰਕੜੇ->ਮੁੱਲ);
    lv_free(user_ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_bool_t ਸੈੱਟ_bool_cb = (lv_obj_ਸੈੱਟ_bool_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_bool_cb) ਸੈੱਟ_bool_cb(obj, subject->ਮੁੱਲ.ਅੰਕ);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_int_t ਸੈੱਟ_int_cb = (lv_obj_ਸੈੱਟ_int_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_int_cb) ਸੈੱਟ_int_cb(obj, subject->ਮੁੱਲ.ਅੰਕ);
}

#ਜੇ LV_USE_FLOAT
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_float_t ਸੈੱਟ_float_cb = (lv_obj_ਸੈੱਟ_float_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_float_cb) ਸੈੱਟ_float_cb(obj, subject->ਮੁੱਲ.float_v);
}
#ਜੇ_ਅੰਤ /*LV_USE_FLOAT*/

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_string_t ਸੈੱਟ_string_cb = (lv_obj_ਸੈੱਟ_string_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_string_cb) ਸੈੱਟ_string_cb(obj, subject->ਮੁੱਲ.ਸੂਚਕ);
}


ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_color_t ਸੈੱਟ_color_cb = (lv_obj_ਸੈੱਟ_color_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_color_cb) ਸੈੱਟ_color_cb(obj, subject->ਮੁੱਲ.color);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਸੂਚਕ_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->ਨਿਸ਼ਾਨਾ;
    lv_obj_ਸੈੱਟ_ਸੂਚਕ_t ਸੈੱਟ_ਸੂਚਕ_cb = (lv_obj_ਸੈੱਟ_ਸੂਚਕ_t)observer->user_cb;
    ਜੇ(ਸੈੱਟ_ਸੂਚਕ_cb) ਸੈੱਟ_ਸੂਚਕ_cb(obj, subject->ਮੁੱਲ.ਸੂਚਕ);
}

ਸਥਿਰ ਖਾਲੀ remove_observers_from_ਸਮੂਹ_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_ਸਮੂਹ);

    ਲਈ(uint32_t i = 0; i < subject->ਮਾਪ; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->ਮੁੱਲ.ਸੂਚਕ))[i];
        subject_remove_observer(sub, ਸਮੂਹ_notify_cb, subject);
    }
}

ਸਥਿਰ ਖਾਲੀ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_ASSERT(subject != ਨਿਰਾ);
    LV_ASSERT(cb != ਨਿਰਾ);

    lv_observer_t * obs = lv_ll_get_ਸਿਰ(&subject->subs_ll);
    ਜਦੋਂ(obs) {
        lv_observer_t * obs_ਅਗਲਾ = lv_ll_get_ਅਗਲਾ(&subject->subs_ll, obs);
        ਜੇ(obs->cb == cb && obs->user_ਅੰਕੜੇ == user_ਅੰਕੜੇ) {
            lv_observer_delete(obs);
        }
        obs = obs_ਅਗਲਾ;
    }
}

#ਜੇ_ਅੰਤ /*LV_USE_OBSERVER*/
