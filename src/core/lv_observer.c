/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "lv_observer_private.h"
#ఒకవేళ LV_USE_OBSERVER

#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../core/lv_global.h"
#అన్తర్భావ "../core/lv_obj_private.h"
#అన్తర్భావ "../misc/lv_event_private.h"
/*********************
 *      DEFINES
 *********************/


#నిర్వచించు subject_list LV_GLOBAL_DEFAULT()->subject_ll

#నిర్వచితం_కాకపోతే FLT_MAX
    #నిర్వచించు FLT_MAX 3.402823466e+38F
#అయితే_ముగింపు

/**********************
 *      TYPEDEFS
 **********************/
రకనిర్వచనం నిర్మాణం {
    lv_state_t state;
} bound_state_t;

రకనిర్వచనం నిర్మాణం {
    lv_subject_t * subject;
    int32_t విలువ;
} subject_సమితి_int_user_దత్తాంశం_t;

రకనిర్వచనం నిర్మాణం {
    lv_subject_t * subject;
    దశాంశం విలువ;
} subject_సమితి_float_user_దత్తాంశం_t;

రకనిర్వచనం నిర్మాణం {
    lv_subject_t * subject;
    స్థిరరాశి అక్షరం * విలువ;
} subject_సమితి_string_user_దత్తాంశం_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

స్థిర శూన్య remove_observers_from_సమూహం_subject(lv_subject_t * subject);
స్థిర శూన్య కూడు_observers_గమ్యం_సమూహం_subject(lv_subject_t * subject, lv_subject_t ** list, కొలత_t కొలత);
స్థిర శూన్య subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, శూన్య * user_దత్తాంశం);
స్థిర శూన్య subject_toggle_cb(lv_event_t * e);
స్థిర శూన్య subject_సమితి_int_cb(lv_event_t * e);
#ఒకవేళ LV_USE_FLOAT
    స్థిర శూన్య subject_సమితి_float_cb(lv_event_t * e);
#అయితే_ముగింపు

స్థిర శూన్య subject_సమితి_string_cb(lv_event_t * e);
స్థిర శూన్య subject_increment_cb(lv_event_t * e);

స్థిర శూన్య unsubscribe_on_delete_cb(lv_event_t * e);
స్థిర శూన్య సమూహం_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
స్థిర lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

స్థిర శూన్య obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
స్థిర శూన్య obj_విలువ_మారింది_event_cb(lv_event_t * e);

స్థిర శూన్య lv_subject_notify_if_మారింది(lv_subject_t * subject);

స్థిర శూన్య subject_సమితి_string_free_user_దత్తాంశం_event_cb(lv_event_t * e);

స్థిర శూన్య సమితి_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
స్థిర శూన్య సమితి_int_observer(lv_observer_t * observer, lv_subject_t * subject);
స్థిర శూన్య సమితి_string_observer(lv_observer_t * observer, lv_subject_t * subject);
స్థిర శూన్య సమితి_color_observer(lv_observer_t * observer, lv_subject_t * subject);
స్థిర శూన్య సమితి_సూచిక_observer(lv_observer_t * observer, lv_subject_t * subject);

#ఒకవేళ LV_USE_FLOAT
    స్థిర శూన్య సమితి_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    స్థిర శూన్య init_float(lv_subject_t * subject, దశాంశం విలువ);
#అయితే_ముగింపు /*LV_USE_FLOAT*/

స్థిర శూన్య init_int(lv_subject_t * subject, int32_t విలువ);
స్థిర శూన్య init_string(lv_subject_t * subject, అక్షరం * buf, అక్షరం * prev_buf, కొలత_t కొలత, స్థిరరాశి అక్షరం * విలువ);
స్థిర శూన్య init_సూచిక(lv_subject_t * subject, శూన్య * విలువ);
స్థిర శూన్య init_color(lv_subject_t * subject, lv_color_t color);
స్థిర శూన్య init_సమూహం(lv_subject_t * subject, lv_subject_t ** list, కొలత_t కొలత);
స్థిర శూన్య init_common(lv_subject_t * subject);

స్థిర శూన్య deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

శూన్య lv_subject_global_init(శూన్య)
{
    lv_ll_init(&subject_list, పరిమాణం(lv_subject_t));
}
శూన్య lv_subject_global_deinit(శూన్య)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        ఒకవేళ(curr->type == LV_SUBJECT_TYPE_సమూహం) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        ఒకవేళ(curr->type != LV_SUBJECT_TYPE_సమూహం) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, తిరిగి ఖాళీ);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, తిరిగి ఖాళీ);
    lv_subject_t * subject = lv_ll_ins_తోక(&subject_list);
    LV_ASSERT_MALLOC(subject);
    ఒకవేళ(!subject) {
        తిరిగి ఖాళీ;
    }

    init_common(subject);

    ఎంపిక(type) {
        స్థితి LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_FLOAT:
#ఒకవేళ LV_USE_FLOAT
            init_float(subject, 0.f);
#అయితే_ముగింపు /*LV_USE_FLOAT*/
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_సూచిక:
            init_సూచిక(subject, ఖాళీ);
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_సమూహం:
            init_సమూహం(subject, ఖాళీ, 0);
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_STRING:
            init_string(subject, ఖాళీ, ఖాళీ, 0, "");
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_INVALID:
        స్థితి LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    తిరిగి subject;
}

శూన్య lv_subject_delete(lv_subject_t * subject)
{
    ఒకవేళ(!subject) {
        తిరిగి;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#ఒకవేళ LV_USE_EXT_దత్తాంశం
శూన్య lv_subject_సమితి_external_దత్తాంశం(lv_subject_t * subject, శూన్య * దత్తాంశం, శూన్య (* free_cb)(శూన్య * దత్తాంశం))
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);

    subject->ext_దత్తాంశం.దత్తాంశం = దత్తాంశం;
    subject->ext_దత్తాంశం.free_cb = free_cb;
}
#అయితే_ముగింపు

శూన్య lv_subject_సమితి_int(lv_subject_t * subject, int32_t విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి);

    విలువ = LV_CLAMP(subject->min_విలువ.అంకె, విలువ, subject->max_విలువ.అంకె);

    subject->prev_విలువ.అంకె = subject->విలువ.అంకె;
    subject->విలువ.అంకె = విలువ;
    lv_subject_notify_if_మారింది(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి 0);

    తిరిగి subject->విలువ.అంకె;
}

int32_t lv_subject_get_మునుపటి_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి 0);

    తిరిగి subject->prev_విలువ.అంకె;
}

శూన్య lv_subject_సమితి_min_విలువ_int(lv_subject_t * subject, int32_t min_విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి);

    subject->min_విలువ.అంకె = min_విలువ;
}

శూన్య lv_subject_సమితి_max_విలువ_int(lv_subject_t * subject, int32_t max_విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి);

    subject->max_విలువ.అంకె = max_విలువ;
}

#ఒకవేళ LV_USE_FLOAT

శూన్య lv_subject_సమితి_float(lv_subject_t * subject, దశాంశం విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి);

    విలువ = LV_CLAMP(subject->min_విలువ.float_v, విలువ, subject->max_విలువ.float_v);

    subject->prev_విలువ.float_v = subject->విలువ.float_v;
    subject->విలువ.float_v = విలువ;
    lv_subject_notify_if_మారింది(subject);
}

దశాంశం lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి 0.0);

    తిరిగి subject->విలువ.float_v;
}

దశాంశం lv_subject_get_మునుపటి_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి 0.0);

    తిరిగి subject->prev_విలువ.float_v;
}

శూన్య lv_subject_సమితి_min_విలువ_float(lv_subject_t * subject, దశాంశం min_విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి);

    subject->min_విలువ.float_v = min_విలువ;
}

శూన్య lv_subject_సమితి_max_విలువ_float(lv_subject_t * subject, దశాంశం max_విలువ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి);

    subject->max_విలువ.float_v = max_విలువ;
}


#అయితే_ముగింపు /*LV_USE_FLOAT*/

శూన్య lv_subject_సమితి_string_బఫరు_static(lv_subject_t * subject, అక్షరం * buf, అక్షరం * prev_buf, కొలత_t కొలత)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి);
    LV_CHECK_ARG(buf != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(కొలత > 0, తిరిగి);

    buf[0] = '\0';
    ఒకవేళ(prev_buf) prev_buf[0] = '\0';

    subject->కొలత = (uint32_t)కొలత;
    subject->విలువ.సూచిక = buf;
    subject->prev_విలువ.సూచిక = prev_buf;
}

శూన్య lv_subject_సమితి_string(lv_subject_t * subject, స్థిరరాశి అక్షరం * buf)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి);
    LV_CHECK_ARG(buf != ఖాళీ, తిరిగి);

    ఒకవేళ(subject->కొలత == 0) తిరిగి;
    ఒకవేళ(subject->prev_విలువ.సూచిక) {
        lv_strlcpy((అక్షరం *)subject->prev_విలువ.సూచిక, subject->విలువ.సూచిక, subject->కొలత);
    }

    lv_strlcpy((అక్షరం *)subject->విలువ.సూచిక, buf, subject->కొలత);

    lv_subject_notify_if_మారింది(subject);
}

శూన్య lv_subject_snprintf(lv_subject_t * subject, స్థిరరాశి అక్షరం * format, ...)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి);
    LV_CHECK_ARG(format != ఖాళీ, తిరిగి);

    ఒకవేళ(subject->కొలత < 1U) తిరిగి;

    ఒకవేళ(subject->prev_విలువ.సూచిక) {
        lv_strlcpy((అక్షరం *)subject->prev_విలువ.సూచిక, subject->విలువ.సూచిక, subject->కొలత);
    }

    va_list va;
    va_ప్రారంభం(va, format);
    స్థిరరాశి పూర్ణాంకం ret = lv_vsnprintf((అక్షరం *)subject->విలువ.సూచిక, subject->కొలత, format, va);
    LV_వాడని(ret);
    va_చివర(va);

    lv_subject_notify_if_మారింది(subject);
}

స్థిరరాశి అక్షరం * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి ఖాళీ);

    తిరిగి subject->విలువ.సూచిక;
}

స్థిరరాశి అక్షరం * lv_subject_get_మునుపటి_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి ఖాళీ);

    తిరిగి subject->prev_విలువ.సూచిక;
}

శూన్య lv_subject_సమితి_సూచిక(lv_subject_t * subject, శూన్య * ptr)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సూచిక, తిరిగి);

    subject->prev_విలువ.సూచిక = subject->విలువ.సూచిక;
    subject->విలువ.సూచిక = ptr;
    lv_subject_notify_if_మారింది(subject);
}

స్థిరరాశి శూన్య * lv_subject_get_సూచిక(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సూచిక, తిరిగి ఖాళీ);


    తిరిగి subject->విలువ.సూచిక;
}

స్థిరరాశి శూన్య * lv_subject_get_మునుపటి_సూచిక(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సూచిక, తిరిగి ఖాళీ);

    తిరిగి subject->prev_విలువ.సూచిక;
}

శూన్య lv_subject_సమితి_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, తిరిగి);

    subject->prev_విలువ.color = subject->విలువ.color;
    subject->విలువ.color = color;
    lv_subject_notify_if_మారింది(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, తిరిగి lv_color_black());

    తిరిగి subject->విలువ.color;
}

lv_color_t lv_subject_get_మునుపటి_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, తిరిగి lv_color_black());

    తిరిగి subject->prev_విలువ.color;
}

శూన్య lv_subject_సమితి_సమూహం_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(list != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సమూహం, తిరిగి);

    కొరకు(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_సందేశం(list[i] != ఖాళీ, తిరిగి, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_సమూహం_subject(subject);
    కూడు_observers_గమ్యం_సమూహం_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_సమూహం_element(lv_subject_t * subject, int32_t సూచీ)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సమూహం, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సూచీ >= 0 && సూచీ < subject->కొలత, తిరిగి ఖాళీ);

    తిరిగి ((lv_subject_t **)(subject->విలువ.సూచిక))[సూచీ];
}

lv_observer_t * lv_subject_కూడు_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(observer_cb != ఖాళీ, తిరిగి ఖాళీ);

    lv_observer_t * observer = lv_subject_కూడు_observer_obj(subject, observer_cb, ఖాళీ, user_దత్తాంశం);
    ఒకవేళ(observer == ఖాళీ) తిరిగి ఖాళీ;

    observer->for_obj = 0;
    తిరిగి observer;
}

lv_observer_t * lv_subject_కూడు_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(observer_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, తిరిగి ఖాళీ);

    lv_observer_t * observer = lv_ll_ins_తోక(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ఒకవేళ(observer == ఖాళీ) తిరిగి ఖాళీ;

    lv_memzero(observer, పరిమాణం(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_దత్తాంశం = user_దత్తాంశం;
    observer->లక్ష్యం = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    ఒకవేళ(obj != ఖాళీ) {
        lv_obj_కూడు_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    తిరిగి observer;
}

lv_observer_t * lv_subject_కూడు_observer_with_లక్ష్యం(lv_subject_t * subject, lv_observer_cb_t observer_cb, శూన్య * లక్ష్యం,
                                                    శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(observer_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, తిరిగి ఖాళీ);

    lv_observer_t * observer = lv_ll_ins_తోక(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ఒకవేళ(observer == ఖాళీ) తిరిగి ఖాళీ;

    lv_memzero(observer, పరిమాణం(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_దత్తాంశం = user_దత్తాంశం;
    observer->లక్ష్యం = లక్ష్యం;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    తిరిగి observer;
}


శూన్య lv_observer_delete(lv_observer_t * observer)
{
    ఒకవేళ(observer == ఖాళీ) తిరిగి;

    ఒకవేళ(observer->for_obj && observer->లక్ష్యం) {
        lv_obj_remove_event_cb_with_user_దత్తాంశం(observer->లక్ష్యం, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_దత్తాంశం(observer->లక్ష్యం, ఖాళీ, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    ఒకవేళ(observer->auto_free_user_దత్తాంశం) {
        lv_free(observer->user_దత్తాంశం);
    }
    lv_free(observer);
}

శూన్య lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_లెక్క(&obj->spec_attr->event_list) : 0);
    కొరకు(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        ఒకవేళ(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_దత్తాంశం;
            ఒకవేళ(subject == ఖాళీ || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_దత్తాంశం(obj, ఖాళీ, subject);

}

శూన్య * lv_observer_get_లక్ష్యం(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ఖాళీ, తిరిగి ఖాళీ);

    తిరిగి observer->లక్ష్యం;
}

శూన్య lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    చేయి {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            ఒకవేళ(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                ఒకవేళ(subject->notify_restart_query) ఆపు;
                observer->notified = 1;
            }
        }
    } అయితే(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_కూడు_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_సంకేతం_t trigger, int32_t అడుగు)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి ఖాళీ);

    lv_subject_increment_dsc_t * user_దత్తాంశం = lv_malloc(పరిమాణం(lv_subject_increment_dsc_t));
    ఒకవేళ(user_దత్తాంశం == ఖాళీ) {
        LV_ASSERT_MALLOC(user_దత్తాంశం);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        తిరిగి ఖాళీ;
    }

    user_దత్తాంశం->అడుగు = అడుగు;
    user_దత్తాంశం->subject = subject;
    user_దత్తాంశం->rollover = false;
    user_దత్తాంశం->min_విలువ = INT32_MIN;
    user_దత్తాంశం->max_విలువ = INT32_MAX;
    lv_obj_కూడు_event_cb(obj, subject_increment_cb, trigger, user_దత్తాంశం);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, user_దత్తాంశం);

    తిరిగి user_దత్తాంశం;
}

శూన్య lv_obj_సమితి_subject_increment_event_min_విలువ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_విలువ)
{
    LV_వాడని(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    LV_CHECK_ARG(dsc != ఖాళీ, తిరిగి);

    dsc->min_విలువ = min_విలువ;
    ఒకవేళ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ఒకవేళ(dsc->subject->విలువ.అంకె < min_విలువ) {
            lv_subject_సమితి_int(dsc->subject, min_విలువ);
        }
    }
#ఒకవేళ LV_USE_FLOAT
    లేకపోతే ఒకవేళ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ఒకవేళ(dsc->subject->విలువ.float_v < (దశాంశం)min_విలువ) {
            lv_subject_సమితి_float(dsc->subject, (దశాంశం)min_విలువ);
        }
    }
#అయితే_ముగింపు
}

శూన్య lv_obj_సమితి_subject_increment_event_max_విలువ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_విలువ)
{
    LV_వాడని(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    LV_CHECK_ARG(dsc != ఖాళీ, తిరిగి);

    dsc->max_విలువ = max_విలువ;
    ఒకవేళ(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ఒకవేళ(dsc->subject->విలువ.అంకె > max_విలువ) {
            lv_subject_సమితి_int(dsc->subject, max_విలువ);
        }
    }
#ఒకవేళ LV_USE_FLOAT
    లేకపోతే ఒకవేళ(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ఒకవేళ(dsc->subject->విలువ.float_v > (దశాంశం)max_విలువ) {
            lv_subject_సమితి_float(dsc->subject, (దశాంశం)max_విలువ);
        }
    }
#అయితే_ముగింపు
}

శూన్య lv_obj_సమితి_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_వాడని(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    LV_CHECK_ARG(dsc != ఖాళీ, తిరిగి);

    dsc->rollover = rollover;
}

శూన్య lv_obj_కూడు_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_సంకేతం_t trigger)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి);

    lv_obj_కూడు_event_cb(obj, subject_toggle_cb, trigger, subject);
}

శూన్య lv_obj_కూడు_subject_సమితి_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_సంకేతం_t trigger, int32_t విలువ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి);

    subject_సమితి_int_user_దత్తాంశం_t * user_దత్తాంశం = lv_malloc(పరిమాణం(subject_సమితి_int_user_దత్తాంశం_t));
    ఒకవేళ(user_దత్తాంశం == ఖాళీ) {
        LV_ASSERT_MALLOC(user_దత్తాంశం);
        LV_LOG_WARN("Couldn't allocate user_data");
        తిరిగి;
    }

    user_దత్తాంశం->subject = subject;
    user_దత్తాంశం->విలువ = విలువ;

    lv_obj_కూడు_event_cb(obj, subject_సమితి_int_cb, trigger, user_దత్తాంశం);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, user_దత్తాంశం);
}

#ఒకవేళ LV_USE_FLOAT
శూన్య lv_obj_కూడు_subject_సమితి_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_సంకేతం_t trigger, దశాంశం విలువ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి);

    subject_సమితి_float_user_దత్తాంశం_t * user_దత్తాంశం = lv_malloc(పరిమాణం(subject_సమితి_float_user_దత్తాంశం_t));
    ఒకవేళ(user_దత్తాంశం == ఖాళీ) {
        LV_ASSERT_MALLOC(user_దత్తాంశం);
        LV_LOG_WARN("Couldn't allocate user_data");
        తిరిగి;
    }

    user_దత్తాంశం->subject = subject;
    user_దత్తాంశం->విలువ = విలువ;

    lv_obj_కూడు_event_cb(obj, subject_సమితి_float_cb, trigger, user_దత్తాంశం);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, user_దత్తాంశం);
}
#అయితే_ముగింపు /*LV_USE_FLOAT*/

శూన్య lv_obj_కూడు_subject_సమితి_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_సంకేతం_t trigger,
                                         స్థిరరాశి అక్షరం * విలువ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి);
    LV_CHECK_ARG(విలువ != ఖాళీ, తిరిగి);

    subject_సమితి_string_user_దత్తాంశం_t * user_దత్తాంశం = lv_malloc(పరిమాణం(subject_సమితి_string_user_దత్తాంశం_t));
    ఒకవేళ(user_దత్తాంశం == ఖాళీ) {
        LV_ASSERT_MALLOC(user_దత్తాంశం);
        LV_LOG_WARN("Couldn't allocate user_data");
        తిరిగి;
    }

    user_దత్తాంశం->subject = subject;
    user_దత్తాంశం->విలువ = lv_strdup(విలువ);
    ఒకవేళ(user_దత్తాంశం->విలువ == ఖాళీ) {
        LV_ASSERT_MALLOC(user_దత్తాంశం->విలువ);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_దత్తాంశం);
        తిరిగి;
    }

    lv_obj_కూడు_event_cb(obj, subject_సమితి_string_cb, trigger, user_దత్తాంశం);
    lv_obj_కూడు_event_cb(obj, subject_సమితి_string_free_user_దత్తాంశం_event_cb, LV_EVENT_DELETE, user_దత్తాంశం);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_bool_t సమితి_bool_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_bool_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి ఖాళీ);

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_bool_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_bool_cb;
    సమితి_bool_observer(observable, subject);
    తిరిగి observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_int_t సమితి_int_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_int_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి ఖాళీ);
    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_int_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_int_cb;
    సమితి_int_observer(observable, subject);
    తిరిగి observable;
}

#ఒకవేళ LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_float_t సమితి_float_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_float_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, తిరిగి ఖాళీ);

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_float_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_float_cb;
    సమితి_float_observer(observable, subject);
    తిరిగి observable;
}
#అయితే_ముగింపు

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_string_t సమితి_string_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_string_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, తిరిగి ఖాళీ);

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_string_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_string_cb;
    సమితి_string_observer(observable, subject);
    తిరిగి observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_color_t సమితి_color_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_color_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, తిరిగి ఖాళీ);

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_color_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_color_cb;
    సమితి_color_observer(observable, subject);
    తిరిగి observable;
}

lv_observer_t * lv_obj_bind_సూచిక(lv_obj_t * obj, lv_subject_t * subject, lv_obj_సమితి_సూచిక_t సమితి_సూచిక_cb)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(సమితి_సూచిక_cb != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_సూచిక, తిరిగి ఖాళీ);

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, సమితి_సూచిక_observer, obj, ఖాళీ);
    ఒకవేళ(observable == ఖాళీ) {
        LV_LOG_WARN("Couldn't add observer to subject");
        తిరిగి ఖాళీ;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (శూన్య (*)(శూన్య))సమితి_సూచిక_cb;
    సమితి_సూచిక_observer(observable, subject);
    తిరిగి observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG_FORMAT_సందేశం(subject->type == LV_SUBJECT_TYPE_INT, తిరిగి ఖాళీ, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_కూడు_event_cb(obj, obj_విలువ_మారింది_event_cb, LV_EVENT_విలువ_మారింది, subject);

    తిరిగి observable;
}


lv_obj_t * lv_observer_get_లక్ష్యం_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ఖాళీ, తిరిగి ఖాళీ);

    తిరిగి (lv_obj_t *)lv_observer_get_లక్ష్యం(observer);
}

శూన్య * lv_observer_get_user_దత్తాంశం(స్థిరరాశి lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ఖాళీ, తిరిగి ఖాళీ);

    తిరిగి observer->user_దత్తాంశం;
}

శూన్య lv_observer_సమితి_user_దత్తాంశం(lv_observer_t * observer, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(observer != ఖాళీ, తిరిగి);

    /* Release internally-owned data only when replacing it. */
    ఒకవేళ(observer->auto_free_user_దత్తాంశం && observer->user_దత్తాంశం != user_దత్తాంశం) {
        lv_free(observer->user_దత్తాంశం);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_దత్తాంశం = user_దత్తాంశం;
    observer->auto_free_user_దత్తాంశం = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర శూన్య init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ఖాళీ);
    lv_memzero(subject, పరిమాణం(*subject));
    lv_ll_init(&(subject->subs_ll), పరిమాణం(lv_observer_t));
}
స్థిర శూన్య init_int(lv_subject_t * subject, int32_t విలువ)
{
    LV_ASSERT(subject != ఖాళీ);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->విలువ.అంకె = విలువ;
    subject->prev_విలువ.అంకె = విలువ;
    subject->min_విలువ.అంకె = INT32_MIN;
    subject->max_విలువ.అంకె = INT32_MAX;
}

#ఒకవేళ LV_USE_FLOAT

స్థిర శూన్య init_float(lv_subject_t * subject, దశాంశం విలువ)
{
    LV_ASSERT(subject != ఖాళీ);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->విలువ.float_v = విలువ;
    subject->prev_విలువ.float_v = విలువ;
    subject->min_విలువ.float_v = -FLT_MAX;
    subject->max_విలువ.float_v = FLT_MAX;
}

#అయితే_ముగింపు /*LV_USE_FLOAT*/

స్థిర శూన్య init_సూచిక(lv_subject_t * subject, శూన్య * విలువ)
{
    LV_ASSERT(subject != ఖాళీ);
    subject->type = LV_SUBJECT_TYPE_సూచిక;
    subject->విలువ.సూచిక = విలువ;
    subject->prev_విలువ.సూచిక = విలువ;
}

స్థిర శూన్య init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ఖాళీ);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->విలువ.color = color;
    subject->prev_విలువ.color = color;
}

స్థిర శూన్య init_సమూహం(lv_subject_t * subject, lv_subject_t ** list, కొలత_t కొలత)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(list != ఖాళీ || కొలత == 0);
    subject->type = LV_SUBJECT_TYPE_సమూహం;
    కూడు_observers_గమ్యం_సమూహం_subject(subject, list, కొలత);
}

స్థిర శూన్య కూడు_observers_గమ్యం_సమూహం_subject(lv_subject_t * subject, lv_subject_t ** list, కొలత_t కొలత)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(list != ఖాళీ || కొలత == 0);
    subject->విలువ.సూచిక = list;
    subject->కొలత = (uint32_t)కొలత;

    కొరకు(uint32_t i = 0; i < కొలత; i++) {
        lv_subject_కూడు_observer(list[i], సమూహం_notify_cb, subject);
    }
}

స్థిర శూన్య init_string(lv_subject_t * subject, అక్షరం * buf, అక్షరం * prev_buf, కొలత_t కొలత, స్థిరరాశి అక్షరం * విలువ)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(buf != ఖాళీ || కొలత == 0);

    ఒకవేళ(కొలత > 0) {
        lv_strlcpy(buf, విలువ, కొలత);
        ఒకవేళ(prev_buf) lv_strlcpy(prev_buf, విలువ, కొలత);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->కొలత = (uint32_t)కొలత;
    subject->విలువ.సూచిక = buf;
    subject->prev_విలువ.సూచిక = prev_buf;
}

స్థిర శూన్య deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ఖాళీ);
    ఒకవేళ(subject->type == LV_SUBJECT_TYPE_సమూహం) {
        remove_observers_from_సమూహం_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_తల(&subject->subs_ll);
    అయితే(observer) {
        lv_observer_t * observer_తదుపరి = lv_ll_get_తదుపరి(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_తదుపరి;
    }

#ఒకవేళ LV_USE_EXT_దత్తాంశం
    ఒకవేళ(subject->ext_దత్తాంశం.free_cb) {
        subject->ext_దత్తాంశం.free_cb(subject->ext_దత్తాంశం.దత్తాంశం);
        subject->ext_దత్తాంశం.దత్తాంశం = ఖాళీ;
    }
#అయితే_ముగింపు

    lv_ll_clear(&subject->subs_ll);
}

స్థిర శూన్య subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    lv_subject_t * subject = lv_event_get_user_దత్తాంశం(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_సమితి_int(subject, v);
}

స్థిర శూన్య subject_సమితి_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    subject_సమితి_int_user_దత్తాంశం_t * user_దత్తాంశం = lv_event_get_user_దత్తాంశం(e);
    lv_subject_సమితి_int(user_దత్తాంశం->subject, user_దత్తాంశం->విలువ);
}


#ఒకవేళ LV_USE_FLOAT
స్థిర శూన్య subject_సమితి_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    subject_సమితి_float_user_దత్తాంశం_t * user_దత్తాంశం = lv_event_get_user_దత్తాంశం(e);
    lv_subject_సమితి_float(user_దత్తాంశం->subject, user_దత్తాంశం->విలువ);
}
#అయితే_ముగింపు

స్థిర శూన్య subject_సమితి_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    subject_సమితి_string_user_దత్తాంశం_t * user_దత్తాంశం = lv_event_get_user_దత్తాంశం(e);
    lv_subject_సమితి_string(user_దత్తాంశం->subject, user_దత్తాంశం->విలువ);
}

స్థిర శూన్య subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    lv_subject_increment_dsc_t * user_దత్తాంశం = lv_event_get_user_దత్తాంశం(e);

    ఒకవేళ(user_దత్తాంశం->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_విలువ = LV_MIN(user_దత్తాంశం->max_విలువ, user_దత్తాంశం->subject->max_విలువ.అంకె);
        int32_t min_విలువ = LV_MAX(user_దత్తాంశం->min_విలువ, user_దత్తాంశం->subject->min_విలువ.అంకె);

        int32_t విలువ = lv_subject_get_int(user_దత్తాంశం->subject);
        విలువ += user_దత్తాంశం->అడుగు;

        ఒకవేళ(user_దత్తాంశం->rollover) {
            ఒకవేళ(విలువ > max_విలువ) {
                విలువ = min_విలువ;
            }
            లేకపోతే ఒకవేళ(విలువ < min_విలువ) {
                విలువ = max_విలువ;
            }
        }
        లేకపోతే {
            విలువ = LV_CLAMP(min_విలువ, విలువ, max_విలువ);
        }

        lv_subject_సమితి_int(user_దత్తాంశం->subject, విలువ);
    }
#ఒకవేళ LV_USE_FLOAT
    లేకపోతే ఒకవేళ(user_దత్తాంశం->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        దశాంశం max_విలువ = LV_MIN((దశాంశం)user_దత్తాంశం->max_విలువ, user_దత్తాంశం->subject->max_విలువ.float_v);
        దశాంశం min_విలువ = LV_MAX((దశాంశం)user_దత్తాంశం->min_విలువ, user_దత్తాంశం->subject->min_విలువ.float_v);


        దశాంశం విలువ = lv_subject_get_float(user_దత్తాంశం->subject);
        విలువ += (దశాంశం)user_దత్తాంశం->అడుగు;

        ఒకవేళ(user_దత్తాంశం->rollover) {
            ఒకవేళ(విలువ > max_విలువ) {
                విలువ = min_విలువ;
            }
            లేకపోతే ఒకవేళ(విలువ < min_విలువ) {
                విలువ = max_విలువ;
            }
        }
        లేకపోతే {
            విలువ = LV_CLAMP(min_విలువ, విలువ, max_విలువ);
        }

        lv_subject_సమితి_float(user_దత్తాంశం->subject, విలువ);
    }
#అయితే_ముగింపు
}

స్థిర శూన్య సమూహం_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ఖాళీ);
    LV_వాడని(subject);
    lv_subject_t * subject_సమూహం = observer->user_దత్తాంశం;
    lv_subject_notify(subject_సమూహం);
}

స్థిర శూన్య unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    lv_observer_t * observer = lv_event_get_user_దత్తాంశం(e);
    lv_observer_delete(observer);
}

స్థిర lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(obj != ఖాళీ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(పరిమాణం(bound_state_t));
    ఒకవేళ(p == ఖాళీ) {
        LV_LOG_WARN("Out of memory");
        తిరిగి ఖాళీ;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_దత్తాంశం = 1;

    తిరిగి observable;
}

స్థిర శూన్య obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ఖాళీ);
    LV_ASSERT(subject != ఖాళీ);
    bound_state_t * p = observer->user_దత్తాంశం;
    LV_ASSERT(p != ఖాళీ);

    ఒకవేళ(subject->విలువ.అంకె) {
        lv_obj_కూడు_state(observer->లక్ష్యం, p->state);
    }
    లేకపోతే {
        lv_obj_remove_state(observer->లక్ష్యం, p->state);
    }
}

స్థిర శూన్య obj_విలువ_మారింది_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    lv_obj_t * obj = lv_event_get_current_లక్ష్యం(e);
    lv_subject_t * subject = lv_event_get_user_దత్తాంశం(e);
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(obj != ఖాళీ);

    lv_subject_సమితి_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

స్థిర శూన్య lv_subject_notify_if_మారింది(lv_subject_t * subject)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    ఎంపిక(subject->type) {
        స్థితి LV_SUBJECT_TYPE_INT :
            ఒకవేళ(subject->విలువ.అంకె != subject->prev_విలువ.అంకె) {
                lv_subject_notify(subject);
            }
            ఆపు;
#ఒకవేళ LV_USE_FLOAT
        స్థితి LV_SUBJECT_TYPE_FLOAT :
            ఒకవేళ(subject->విలువ.float_v != subject->prev_విలువ.float_v) {
                lv_subject_notify(subject);
            }
            ఆపు;
#అయితే_ముగింపు
        స్థితి LV_SUBJECT_TYPE_సమూహం :
        స్థితి LV_SUBJECT_TYPE_సూచిక :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_COLOR  :
            ఒకవేళ(!lv_color_eq(subject->విలువ.color, subject->prev_విలువ.color)) {
                lv_subject_notify(subject);
            }
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_STRING:
            ఒకవేళ(!subject->prev_విలువ.సూచిక ||
               lv_strcmp(subject->విలువ.సూచిక, subject->prev_విలువ.సూచిక)) {
                lv_subject_notify(subject);
            }
            ఆపు;
        స్థితి LV_SUBJECT_TYPE_INVALID :
        స్థితి LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

స్థిర శూన్య subject_సమితి_string_free_user_దత్తాంశం_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    subject_సమితి_string_user_దత్తాంశం_t * user_దత్తాంశం = lv_event_get_user_దత్తాంశం(e);
    LV_ASSERT(user_దత్తాంశం != ఖాళీ);
    lv_free((శూన్య *)user_దత్తాంశం->విలువ);
    lv_free(user_దత్తాంశం);
}

స్థిర శూన్య సమితి_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_bool_t సమితి_bool_cb = (lv_obj_సమితి_bool_t)observer->user_cb;
    ఒకవేళ(సమితి_bool_cb) సమితి_bool_cb(obj, subject->విలువ.అంకె);
}

స్థిర శూన్య సమితి_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_int_t సమితి_int_cb = (lv_obj_సమితి_int_t)observer->user_cb;
    ఒకవేళ(సమితి_int_cb) సమితి_int_cb(obj, subject->విలువ.అంకె);
}

#ఒకవేళ LV_USE_FLOAT
స్థిర శూన్య సమితి_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_float_t సమితి_float_cb = (lv_obj_సమితి_float_t)observer->user_cb;
    ఒకవేళ(సమితి_float_cb) సమితి_float_cb(obj, subject->విలువ.float_v);
}
#అయితే_ముగింపు /*LV_USE_FLOAT*/

స్థిర శూన్య సమితి_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_string_t సమితి_string_cb = (lv_obj_సమితి_string_t)observer->user_cb;
    ఒకవేళ(సమితి_string_cb) సమితి_string_cb(obj, subject->విలువ.సూచిక);
}


స్థిర శూన్య సమితి_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_color_t సమితి_color_cb = (lv_obj_సమితి_color_t)observer->user_cb;
    ఒకవేళ(సమితి_color_cb) సమితి_color_cb(obj, subject->విలువ.color);
}

స్థిర శూన్య సమితి_సూచిక_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->లక్ష్యం;
    lv_obj_సమితి_సూచిక_t సమితి_సూచిక_cb = (lv_obj_సమితి_సూచిక_t)observer->user_cb;
    ఒకవేళ(సమితి_సూచిక_cb) సమితి_సూచిక_cb(obj, subject->విలువ.సూచిక);
}

స్థిర శూన్య remove_observers_from_సమూహం_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_సమూహం);

    కొరకు(uint32_t i = 0; i < subject->కొలత; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->విలువ.సూచిక))[i];
        subject_remove_observer(sub, సమూహం_notify_cb, subject);
    }
}

స్థిర శూన్య subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, శూన్య * user_దత్తాంశం)
{
    LV_ASSERT(subject != ఖాళీ);
    LV_ASSERT(cb != ఖాళీ);

    lv_observer_t * obs = lv_ll_get_తల(&subject->subs_ll);
    అయితే(obs) {
        lv_observer_t * obs_తదుపరి = lv_ll_get_తదుపరి(&subject->subs_ll, obs);
        ఒకవేళ(obs->cb == cb && obs->user_దత్తాంశం == user_దత్తాంశం) {
            lv_observer_delete(obs);
        }
        obs = obs_తదుపరి;
    }
}

#అయితే_ముగింపు /*LV_USE_OBSERVER*/
