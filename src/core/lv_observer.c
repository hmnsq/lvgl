/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ଅନ୍ତର୍ଭାଵ "lv_observer_private.h"
#ଯଦି LV_USE_OBSERVER

#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_global.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_event_private.h"
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
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_state_t state;
} bound_state_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_subject_t * subject;
    int32_t ମୂଲ୍ୟ;
} subject_set_int_user_data_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_subject_t * subject;
    ଦଶମିକ ମୂଲ୍ୟ;
} subject_set_float_user_data_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_subject_t * subject;
    ଅଚଳ ଅକ୍ଷର * ମୂଲ୍ୟ;
} subject_set_string_user_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

ସ୍ଥିର ଶୂନ୍ୟ remove_observers_from_group_subject(lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ସ୍ଥିର ଶୂନ୍ୟ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ଶୂନ୍ୟ * user_data);
ସ୍ଥିର ଶୂନ୍ୟ subject_toggle_cb(lv_event_t * e);
ସ୍ଥିର ଶୂନ୍ୟ subject_set_int_cb(lv_event_t * e);
#ଯଦି LV_USE_FLOAT
    ସ୍ଥିର ଶୂନ୍ୟ subject_set_float_cb(lv_event_t * e);
#endif

ସ୍ଥିର ଶୂନ୍ୟ subject_set_string_cb(lv_event_t * e);
ସ୍ଥିର ଶୂନ୍ୟ subject_increment_cb(lv_event_t * e);

ସ୍ଥିର ଶୂନ୍ୟ unsubscribe_on_delete_cb(lv_event_t * e);
ସ୍ଥିର ଶୂନ୍ୟ group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state);

ସ୍ଥିର ଶୂନ୍ୟ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ obj_ମୂଲ୍ୟ_changed_event_cb(lv_event_t * e);

ସ୍ଥିର ଶୂନ୍ୟ lv_subject_notify_if_changed(lv_subject_t * subject);

ସ୍ଥିର ଶୂନ୍ୟ subject_set_string_free_user_data_event_cb(lv_event_t * e);

ସ୍ଥିର ଶୂନ୍ୟ set_bool_observer(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ set_int_observer(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ set_string_observer(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ set_color_observer(lv_observer_t * observer, lv_subject_t * subject);
ସ୍ଥିର ଶୂନ୍ୟ set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject);

#ଯଦି LV_USE_FLOAT
    ସ୍ଥିର ଶୂନ୍ୟ set_float_observer(lv_observer_t * observer, lv_subject_t * subject);
    ସ୍ଥିର ଶୂନ୍ୟ init_float(lv_subject_t * subject, ଦଶମିକ ମୂଲ୍ୟ);
#endif /*LV_USE_FLOAT*/

ସ୍ଥିର ଶୂନ୍ୟ init_int(lv_subject_t * subject, int32_t ମୂଲ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ init_string(lv_subject_t * subject, ଅକ୍ଷର * buf, ଅକ୍ଷର * prev_buf, size_t size, ଅଚଳ ଅକ୍ଷର * ମୂଲ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ init_pointer(lv_subject_t * subject, ଶୂନ୍ୟ * ମୂଲ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ init_color(lv_subject_t * subject, lv_color_t color);
ସ୍ଥିର ଶୂନ୍ୟ init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size);
ସ୍ଥିର ଶୂନ୍ୟ init_common(lv_subject_t * subject);

ସ୍ଥିର ଶୂନ୍ୟ deinit(lv_subject_t * subject);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ଶୂନ୍ୟ lv_subject_global_init(ଶୂନ୍ୟ)
{
    lv_ll_init(&subject_list, ଆକାର(lv_subject_t));
}
ଶୂନ୍ୟ lv_subject_global_deinit(ଶୂନ୍ୟ)
{
    lv_subject_t * curr;

    /* always deinit group subjects first as they will
     * dereference other subjects during deinitialization */
    LV_LL_READ(&subject_list, curr) {
        ଯଦି(curr->type == LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }

    LV_LL_READ(&subject_list, curr) {
        ଯଦି(curr->type != LV_SUBJECT_TYPE_GROUP) {
            deinit(curr);
        }
    }
    lv_ll_clear(&subject_list);
}

lv_subject_t * lv_subject_create(lv_subject_type_t type)
{
    LV_CHECK_ARG(type != LV_SUBJECT_TYPE_INVALID && type != LV_SUBJECT_TYPE_NONE, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(LV_USE_FLOAT || type != LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ ଖାଲି);
    lv_subject_t * subject = lv_ll_ins_tail(&subject_list);
    LV_ASSERT_MALLOC(subject);
    ଯଦି(!subject) {
        ଫେରନ୍ତୁ ଖାଲି;
    }

    init_common(subject);

    ବାଛ(type) {
        ସ୍ଥିତି LV_SUBJECT_TYPE_INT:
            init_int(subject, 0);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_FLOAT:
#ଯଦି LV_USE_FLOAT
            init_float(subject, 0.f);
#endif /*LV_USE_FLOAT*/
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_POINTER:
            init_pointer(subject, ଖାଲି);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_COLOR:
            init_color(subject, lv_color_black());
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_GROUP:
            init_group(subject, ଖାଲି, 0);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_STRING:
            init_string(subject, ଖାଲି, ଖାଲି, 0, "");
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_INVALID:
        ସ୍ଥିତି LV_SUBJECT_TYPE_NONE:
            LV_UNREACHABLE();
    }
    ଫେରନ୍ତୁ subject;
}

ଶୂନ୍ୟ lv_subject_delete(lv_subject_t * subject)
{
    ଯଦି(!subject) {
        ଫେରନ୍ତୁ;
    }
    /* TODO: we can check here if this subject is
     * referenced by a group subject and remove it from the list*/

    deinit(subject);
    lv_ll_remove(&subject_list, subject);
    lv_free(subject);
}

#ଯଦି LV_USE_EXT_DATA
ଶୂନ୍ୟ lv_subject_set_external_data(lv_subject_t * subject, ଶୂନ୍ୟ * data, ଶୂନ୍ୟ (* free_cb)(ଶୂନ୍ୟ * data))
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);

    subject->ext_data.data = data;
    subject->ext_data.free_cb = free_cb;
}
#endif

ଶୂନ୍ୟ lv_subject_set_int(lv_subject_t * subject, int32_t ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ);

    ମୂଲ୍ୟ = LV_CLAMP(subject->min_ମୂଲ୍ୟ.ଅଙ୍କ, ମୂଲ୍ୟ, subject->max_ମୂଲ୍ୟ.ଅଙ୍କ);

    subject->prev_ମୂଲ୍ୟ.ଅଙ୍କ = subject->ମୂଲ୍ୟ.ଅଙ୍କ;
    subject->ମୂଲ୍ୟ.ଅଙ୍କ = ମୂଲ୍ୟ;
    lv_subject_notify_if_changed(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ 0);

    ଫେରନ୍ତୁ subject->ମୂଲ୍ୟ.ଅଙ୍କ;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ 0);

    ଫେରନ୍ତୁ subject->prev_ମୂଲ୍ୟ.ଅଙ୍କ;
}

ଶୂନ୍ୟ lv_subject_set_min_ମୂଲ୍ୟ_int(lv_subject_t * subject, int32_t min_ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ);

    subject->min_ମୂଲ୍ୟ.ଅଙ୍କ = min_ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ lv_subject_set_max_ମୂଲ୍ୟ_int(lv_subject_t * subject, int32_t max_ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ);

    subject->max_ମୂଲ୍ୟ.ଅଙ୍କ = max_ମୂଲ୍ୟ;
}

#ଯଦି LV_USE_FLOAT

ଶୂନ୍ୟ lv_subject_set_float(lv_subject_t * subject, ଦଶମିକ ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ);

    ମୂଲ୍ୟ = LV_CLAMP(subject->min_ମୂଲ୍ୟ.float_v, ମୂଲ୍ୟ, subject->max_ମୂଲ୍ୟ.float_v);

    subject->prev_ମୂଲ୍ୟ.float_v = subject->ମୂଲ୍ୟ.float_v;
    subject->ମୂଲ୍ୟ.float_v = ମୂଲ୍ୟ;
    lv_subject_notify_if_changed(subject);
}

ଦଶମିକ lv_subject_get_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ 0.0);

    ଫେରନ୍ତୁ subject->ମୂଲ୍ୟ.float_v;
}

ଦଶମିକ lv_subject_get_previous_float(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ 0.0);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ 0.0);

    ଫେରନ୍ତୁ subject->prev_ମୂଲ୍ୟ.float_v;
}

ଶୂନ୍ୟ lv_subject_set_min_ମୂଲ୍ୟ_float(lv_subject_t * subject, ଦଶମିକ min_ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ);

    subject->min_ମୂଲ୍ୟ.float_v = min_ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ lv_subject_set_max_ମୂଲ୍ୟ_float(lv_subject_t * subject, ଦଶମିକ max_ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ);

    subject->max_ମୂଲ୍ୟ.float_v = max_ମୂଲ୍ୟ;
}


#endif /*LV_USE_FLOAT*/

ଶୂନ୍ୟ lv_subject_set_string_buffer_static(lv_subject_t * subject, ଅକ୍ଷର * buf, ଅକ୍ଷର * prev_buf, size_t size)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(buf != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(size > 0, ଫେରନ୍ତୁ);

    buf[0] = '\0';
    ଯଦି(prev_buf) prev_buf[0] = '\0';

    subject->size = (uint32_t)size;
    subject->ମୂଲ୍ୟ.pointer = buf;
    subject->prev_ମୂଲ୍ୟ.pointer = prev_buf;
}

ଶୂନ୍ୟ lv_subject_set_string(lv_subject_t * subject, ଅଚଳ ଅକ୍ଷର * buf)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(buf != ଖାଲି, ଫେରନ୍ତୁ);

    ଯଦି(subject->size == 0) ଫେରନ୍ତୁ;
    ଯଦି(subject->prev_ମୂଲ୍ୟ.pointer) {
        lv_strlcpy((ଅକ୍ଷର *)subject->prev_ମୂଲ୍ୟ.pointer, subject->ମୂଲ୍ୟ.pointer, subject->size);
    }

    lv_strlcpy((ଅକ୍ଷର *)subject->ମୂଲ୍ୟ.pointer, buf, subject->size);

    lv_subject_notify_if_changed(subject);
}

ଶୂନ୍ୟ lv_subject_snprintf(lv_subject_t * subject, ଅଚଳ ଅକ୍ଷର * format, ...)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(format != ଖାଲି, ଫେରନ୍ତୁ);

    ଯଦି(subject->size < 1U) ଫେରନ୍ତୁ;

    ଯଦି(subject->prev_ମୂଲ୍ୟ.pointer) {
        lv_strlcpy((ଅକ୍ଷର *)subject->prev_ମୂଲ୍ୟ.pointer, subject->ମୂଲ୍ୟ.pointer, subject->size);
    }

    va_list va;
    va_start(va, format);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = lv_vsnprintf((ଅକ୍ଷର *)subject->ମୂଲ୍ୟ.pointer, subject->size, format, va);
    LV_UNUSED(ret);
    va_end(va);

    lv_subject_notify_if_changed(subject);
}

ଅଚଳ ଅକ୍ଷର * lv_subject_get_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ subject->ମୂଲ୍ୟ.pointer;
}

ଅଚଳ ଅକ୍ଷର * lv_subject_get_previous_string(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ subject->prev_ମୂଲ୍ୟ.pointer;
}

ଶୂନ୍ୟ lv_subject_set_pointer(lv_subject_t * subject, ଶୂନ୍ୟ * ptr)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ଫେରନ୍ତୁ);

    subject->prev_ମୂଲ୍ୟ.pointer = subject->ମୂଲ୍ୟ.pointer;
    subject->ମୂଲ୍ୟ.pointer = ptr;
    lv_subject_notify_if_changed(subject);
}

ଅଚଳ ଶୂନ୍ୟ * lv_subject_get_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ଫେରନ୍ତୁ ଖାଲି);


    ଫେରନ୍ତୁ subject->ମୂଲ୍ୟ.pointer;
}

ଅଚଳ ଶୂନ୍ୟ * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ subject->prev_ମୂଲ୍ୟ.pointer;
}

ଶୂନ୍ୟ lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ଫେରନ୍ତୁ);

    subject->prev_ମୂଲ୍ୟ.color = subject->ମୂଲ୍ୟ.color;
    subject->ମୂଲ୍ୟ.color = color;
    lv_subject_notify_if_changed(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ଫେରନ୍ତୁ lv_color_black());

    ଫେରନ୍ତୁ subject->ମୂଲ୍ୟ.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ lv_color_black());
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ଫେରନ୍ତୁ lv_color_black());

    ଫେରନ୍ତୁ subject->prev_ମୂଲ୍ୟ.color;
}

ଶୂନ୍ୟ lv_subject_set_group_list_static(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(list != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, ଫେରନ୍ତୁ);

    ପାଇଁ(uint32_t i = 0; i < list_len; i++) {
        LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(list[i] != ଖାଲି, ଫେରନ୍ତୁ, "Subject %" LV_PRIu32 " is NULL", i);
    }
    remove_observers_from_group_subject(subject);
    add_observers_to_group_subject(subject, list, list_len);
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_GROUP, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(index >= 0 && index < subject->size, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ ((lv_subject_t **)(subject->ମୂଲ୍ୟ.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(observer_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, observer_cb, ଖାଲି, user_data);
    ଯଦି(observer == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    observer->for_obj = 0;
    ଫେରନ୍ତୁ observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(observer_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ଯଦି(observer == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    lv_memzero(observer, ଆକାର(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = obj;
    observer->for_obj = 1;
    /* subscribe to delete event of the object */
    ଯଦି(obj != ଖାଲି) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ଫେରନ୍ତୁ observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t observer_cb, ଶୂନ୍ୟ * target,
                                                    ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(observer_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type != LV_SUBJECT_TYPE_INVALID, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observer = lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    ଯଦି(observer == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    lv_memzero(observer, ଆକାର(*observer));

    observer->subject = subject;
    observer->cb = observer_cb;
    observer->user_data = user_data;
    observer->target = target;

    /* Update Observer immediately. */
    observer->cb(observer, subject);

    ଫେରନ୍ତୁ observer;
}


ଶୂନ୍ୟ lv_observer_delete(lv_observer_t * observer)
{
    ଯଦି(observer == ଖାଲି) ଫେରନ୍ତୁ;

    ଯଦି(observer->for_obj && observer->target) {
        lv_obj_remove_event_cb_with_user_data(observer->target, unsubscribe_on_delete_cb, observer);
        lv_obj_remove_event_cb_with_user_data(observer->target, ଖାଲି, observer->subject);
    }

    observer->subject->notify_restart_query = 1;

    lv_ll_remove(&(observer->subject->subs_ll), observer);

    ଯଦି(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

ଶୂନ୍ୟ lv_obj_remove_from_subject(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    /* subject == NULL is documented as valid: remove from ALL subjects */

    /*
     * Look for the `observer` that connects `obj` and `subject`
     * Since the obj is associated with the subject,
     *  the `obj` will have an LV_EVENT_REMOVE event with the `unsubscribe_on_delete_cb` callback
     *  associated.
     * From the event we can then find the observer in the event's `user_data` field
     */
    int32_t i;
    int32_t event_cnt = (int32_t)(obj->spec_attr ? lv_event_get_ଗଣନା(&obj->spec_attr->event_list) : 0);
    ପାଇଁ(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * event_dsc = lv_obj_get_event_dsc(obj, i);
        ଯଦି(event_dsc->cb == unsubscribe_on_delete_cb) {
            lv_observer_t * observer = event_dsc->user_data;
            ଯଦି(subject == ଖାଲି || subject == observer->subject) {
                /* lv_observer_delete handles the deletion of all possible event callbacks */
                lv_observer_delete(observer);
            }
        }
    }
    /* Gracefully de-couple `subject` from Widget by deleting any existing
     * `LV_EVENT_VALUE_CHANGED` event associated with `subject` in case
     * one of the `..._bind_value()` functions was used. */
    lv_obj_remove_event_cb_with_user_data(obj, ଖାଲି, subject);

}

ଶୂନ୍ୟ * lv_observer_get_target(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ observer->target;
}

ଶୂନ୍ୟ lv_subject_notify(lv_subject_t * subject)
{
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);

    lv_observer_t * observer;
    LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    କର {
        subject->notify_restart_query = 0;
        LV_LL_READ(&(subject->subs_ll), observer) {
            ଯଦି(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                ଯଦି(subject->notify_restart_query) ଭାଙ୍ଗ;
                observer->notified = 1;
            }
        }
    } ଯେତେବେଳେ(subject->notify_restart_query);
}

lv_subject_increment_dsc_t * lv_obj_add_subject_increment_event(lv_obj_t * obj, lv_subject_t * subject,
                                                                lv_event_code_t trigger, int32_t step)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT || subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ ଖାଲି);

    lv_subject_increment_dsc_t * user_data = lv_malloc(ଆକାର(lv_subject_increment_dsc_t));
    ଯଦି(user_data == ଖାଲି) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data in in <lv_obj-subject_increment>");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    user_data->step = step;
    user_data->subject = subject;
    user_data->rollover = false;
    user_data->min_ମୂଲ୍ୟ = INT32_MIN;
    user_data->max_ମୂଲ୍ୟ = INT32_MAX;
    lv_obj_add_event_cb(obj, subject_increment_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);

    ଫେରନ୍ତୁ user_data;
}

ଶୂନ୍ୟ lv_obj_set_subject_increment_event_min_ମୂଲ୍ୟ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t min_ମୂଲ୍ୟ)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(dsc != ଖାଲି, ଫେରନ୍ତୁ);

    dsc->min_ମୂଲ୍ୟ = min_ମୂଲ୍ୟ;
    ଯଦି(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ଯଦି(dsc->subject->ମୂଲ୍ୟ.ଅଙ୍କ < min_ମୂଲ୍ୟ) {
            lv_subject_set_int(dsc->subject, min_ମୂଲ୍ୟ);
        }
    }
#ଯଦି LV_USE_FLOAT
    ଅନ୍ୟଥା ଯଦି(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ଯଦି(dsc->subject->ମୂଲ୍ୟ.float_v < (ଦଶମିକ)min_ମୂଲ୍ୟ) {
            lv_subject_set_float(dsc->subject, (ଦଶମିକ)min_ମୂଲ୍ୟ);
        }
    }
#endif
}

ଶୂନ୍ୟ lv_obj_set_subject_increment_event_max_ମୂଲ୍ୟ(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, int32_t max_ମୂଲ୍ୟ)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(dsc != ଖାଲି, ଫେରନ୍ତୁ);

    dsc->max_ମୂଲ୍ୟ = max_ମୂଲ୍ୟ;
    ଯଦି(dsc->subject->type == LV_SUBJECT_TYPE_INT) {
        ଯଦି(dsc->subject->ମୂଲ୍ୟ.ଅଙ୍କ > max_ମୂଲ୍ୟ) {
            lv_subject_set_int(dsc->subject, max_ମୂଲ୍ୟ);
        }
    }
#ଯଦି LV_USE_FLOAT
    ଅନ୍ୟଥା ଯଦି(dsc->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        ଯଦି(dsc->subject->ମୂଲ୍ୟ.float_v > (ଦଶମିକ)max_ମୂଲ୍ୟ) {
            lv_subject_set_float(dsc->subject, (ଦଶମିକ)max_ମୂଲ୍ୟ);
        }
    }
#endif
}

ଶୂନ୍ୟ lv_obj_set_subject_increment_event_rollover(lv_obj_t * obj, lv_subject_increment_dsc_t * dsc, bool rollover)
{
    LV_UNUSED(obj);
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(dsc != ଖାଲି, ଫେରନ୍ତୁ);

    dsc->rollover = rollover;
}

ଶୂନ୍ୟ lv_obj_add_subject_toggle_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ);

    lv_obj_add_event_cb(obj, subject_toggle_cb, trigger, subject);
}

ଶୂନ୍ୟ lv_obj_add_subject_set_int_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, int32_t ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ);

    subject_set_int_user_data_t * user_data = lv_malloc(ଆକାର(subject_set_int_user_data_t));
    ଯଦି(user_data == ଖାଲି) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ଫେରନ୍ତୁ;
    }

    user_data->subject = subject;
    user_data->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    lv_obj_add_event_cb(obj, subject_set_int_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}

#ଯଦି LV_USE_FLOAT
ଶୂନ୍ୟ lv_obj_add_subject_set_float_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger, ଦଶମିକ ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ);

    subject_set_float_user_data_t * user_data = lv_malloc(ଆକାର(subject_set_float_user_data_t));
    ଯଦି(user_data == ଖାଲି) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ଫେରନ୍ତୁ;
    }

    user_data->subject = subject;
    user_data->ମୂଲ୍ୟ = ମୂଲ୍ୟ;

    lv_obj_add_event_cb(obj, subject_set_float_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, user_data);
}
#endif /*LV_USE_FLOAT*/

ଶୂନ୍ୟ lv_obj_add_subject_set_string_event(lv_obj_t * obj, lv_subject_t * subject, lv_event_code_t trigger,
                                         ଅଚଳ ଅକ୍ଷର * ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(ମୂଲ୍ୟ != ଖାଲି, ଫେରନ୍ତୁ);

    subject_set_string_user_data_t * user_data = lv_malloc(ଆକାର(subject_set_string_user_data_t));
    ଯଦି(user_data == ଖାଲି) {
        LV_ASSERT_MALLOC(user_data);
        LV_LOG_WARN("Couldn't allocate user_data");
        ଫେରନ୍ତୁ;
    }

    user_data->subject = subject;
    user_data->ମୂଲ୍ୟ = lv_strdup(ମୂଲ୍ୟ);
    ଯଦି(user_data->ମୂଲ୍ୟ == ଖାଲି) {
        LV_ASSERT_MALLOC(user_data->ମୂଲ୍ୟ);
        LV_LOG_WARN("Couldn't allocate string value");
        lv_free(user_data);
        ଫେରନ୍ତୁ;
    }

    lv_obj_add_event_cb(obj, subject_set_string_cb, trigger, user_data);
    lv_obj_add_event_cb(obj, subject_set_string_free_user_data_event_cb, LV_EVENT_DELETE, user_data);
}

lv_observer_t * lv_obj_bind_bool(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_bool_t set_bool_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_bool_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_bool_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_bool_cb;
    set_bool_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}

lv_observer_t * lv_obj_bind_int(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_int_t set_int_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_int_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ ଖାଲି);
    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_int_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_int_cb;
    set_int_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}

#ଯଦି LV_USE_FLOAT
lv_observer_t * lv_obj_bind_float(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_float_t set_float_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_float_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_FLOAT, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_float_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_float_cb;
    set_float_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}
#endif

lv_observer_t * lv_obj_bind_string(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_string_t set_string_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_string_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_STRING, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_string_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_string_cb;
    set_string_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}

lv_observer_t * lv_obj_bind_color(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_color_t set_color_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_color_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_COLOR, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_color_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_color_cb;
    set_color_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}

lv_observer_t * lv_obj_bind_pointer(lv_obj_t * obj, lv_subject_t * subject, lv_obj_set_pointer_t set_pointer_cb)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(set_pointer_cb != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject->type == LV_SUBJECT_TYPE_POINTER, ଫେରନ୍ତୁ ଖାଲି);

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, set_pointer_observer, obj, ଖାଲି);
    ଯଦି(observable == ଖାଲି) {
        LV_LOG_WARN("Couldn't add observer to subject");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Passing a function pointer as void * user_data generates warning so set it here, and call the callback manually */
    observable->user_cb = (ଶୂନ୍ୟ (*)(ଶୂନ୍ୟ))set_pointer_cb;
    set_pointer_observer(observable, subject);
    ଫେରନ୍ତୁ observable;
}

lv_observer_t * lv_obj_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG_FORMAT_ସନ୍ଦେଶ(subject->type == LV_SUBJECT_TYPE_INT, ଫେରନ୍ତୁ ଖାଲି, "Incompatible subject type: %d",
                            subject->type);

    lv_observer_t * observable = bind_state(subject, obj, LV_STATE_CHECKED);

    lv_obj_add_event_cb(obj, obj_ମୂଲ୍ୟ_changed_event_cb, LV_EVENT_ମୂଲ୍ୟ_CHANGED, subject);

    ଫେରନ୍ତୁ observable;
}


lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ (lv_obj_t *)lv_observer_get_target(observer);
}

ଶୂନ୍ୟ * lv_observer_get_user_data(ଅଚଳ lv_observer_t * observer)
{
    LV_CHECK_ARG(observer != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ observer->user_data;
}

ଶୂନ୍ୟ lv_observer_set_user_data(lv_observer_t * observer, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(observer != ଖାଲି, ଫେରନ୍ତୁ);

    /* Release internally-owned data only when replacing it. */
    ଯଦି(observer->auto_free_user_data && observer->user_data != user_data) {
        lv_free(observer->user_data);
    }

    /* Data assigned through the public setter is always caller-owned. */
    observer->user_data = user_data;
    observer->auto_free_user_data = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ସ୍ଥିର ଶୂନ୍ୟ init_common(lv_subject_t * subject)
{
    LV_ASSERT(subject != ଖାଲି);
    lv_memzero(subject, ଆକାର(*subject));
    lv_ll_init(&(subject->subs_ll), ଆକାର(lv_observer_t));
}
ସ୍ଥିର ଶୂନ୍ୟ init_int(lv_subject_t * subject, int32_t ମୂଲ୍ୟ)
{
    LV_ASSERT(subject != ଖାଲି);
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->ମୂଲ୍ୟ.ଅଙ୍କ = ମୂଲ୍ୟ;
    subject->prev_ମୂଲ୍ୟ.ଅଙ୍କ = ମୂଲ୍ୟ;
    subject->min_ମୂଲ୍ୟ.ଅଙ୍କ = INT32_MIN;
    subject->max_ମୂଲ୍ୟ.ଅଙ୍କ = INT32_MAX;
}

#ଯଦି LV_USE_FLOAT

ସ୍ଥିର ଶୂନ୍ୟ init_float(lv_subject_t * subject, ଦଶମିକ ମୂଲ୍ୟ)
{
    LV_ASSERT(subject != ଖାଲି);
    subject->type = LV_SUBJECT_TYPE_FLOAT;
    subject->ମୂଲ୍ୟ.float_v = ମୂଲ୍ୟ;
    subject->prev_ମୂଲ୍ୟ.float_v = ମୂଲ୍ୟ;
    subject->min_ମୂଲ୍ୟ.float_v = -FLT_MAX;
    subject->max_ମୂଲ୍ୟ.float_v = FLT_MAX;
}

#endif /*LV_USE_FLOAT*/

ସ୍ଥିର ଶୂନ୍ୟ init_pointer(lv_subject_t * subject, ଶୂନ୍ୟ * ମୂଲ୍ୟ)
{
    LV_ASSERT(subject != ଖାଲି);
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->ମୂଲ୍ୟ.pointer = ମୂଲ୍ୟ;
    subject->prev_ମୂଲ୍ୟ.pointer = ମୂଲ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ init_color(lv_subject_t * subject, lv_color_t color)
{
    LV_ASSERT(subject != ଖାଲି);
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->ମୂଲ୍ୟ.color = color;
    subject->prev_ମୂଲ୍ୟ.color = color;
}

ସ୍ଥିର ଶୂନ୍ୟ init_group(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(list != ଖାଲି || size == 0);
    subject->type = LV_SUBJECT_TYPE_GROUP;
    add_observers_to_group_subject(subject, list, size);
}

ସ୍ଥିର ଶୂନ୍ୟ add_observers_to_group_subject(lv_subject_t * subject, lv_subject_t ** list, size_t size)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(list != ଖାଲି || size == 0);
    subject->ମୂଲ୍ୟ.pointer = list;
    subject->size = (uint32_t)size;

    ପାଇଁ(uint32_t i = 0; i < size; i++) {
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ init_string(lv_subject_t * subject, ଅକ୍ଷର * buf, ଅକ୍ଷର * prev_buf, size_t size, ଅଚଳ ଅକ୍ଷର * ମୂଲ୍ୟ)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(buf != ଖାଲି || size == 0);

    ଯଦି(size > 0) {
        lv_strlcpy(buf, ମୂଲ୍ୟ, size);
        ଯଦି(prev_buf) lv_strlcpy(prev_buf, ମୂଲ୍ୟ, size);
    }

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = (uint32_t)size;
    subject->ମୂଲ୍ୟ.pointer = buf;
    subject->prev_ମୂଲ୍ୟ.pointer = prev_buf;
}

ସ୍ଥିର ଶୂନ୍ୟ deinit(lv_subject_t * subject)
{
    LV_ASSERT(subject != ଖାଲି);
    ଯଦି(subject->type == LV_SUBJECT_TYPE_GROUP) {
        remove_observers_from_group_subject(subject);
    }

    lv_observer_t * observer = lv_ll_get_head(&subject->subs_ll);
    ଯେତେବେଳେ(observer) {
        lv_observer_t * observer_next = lv_ll_get_next(&subject->subs_ll, observer);

        lv_observer_delete(observer);
        observer = observer_next;
    }

#ଯଦି LV_USE_EXT_DATA
    ଯଦି(subject->ext_data.free_cb) {
        subject->ext_data.free_cb(subject->ext_data.data);
        subject->ext_data.data = ଖାଲି;
    }
#endif

    lv_ll_clear(&subject->subs_ll);
}

ସ୍ଥିର ଶୂନ୍ୟ subject_toggle_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    lv_subject_t * subject = lv_event_get_user_data(e);
    int32_t v = lv_subject_get_int(subject);
    v = !v;

    lv_subject_set_int(subject, v);
}

ସ୍ଥିର ଶୂନ୍ୟ subject_set_int_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    subject_set_int_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_int(user_data->subject, user_data->ମୂଲ୍ୟ);
}


#ଯଦି LV_USE_FLOAT
ସ୍ଥିର ଶୂନ୍ୟ subject_set_float_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    subject_set_float_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_float(user_data->subject, user_data->ମୂଲ୍ୟ);
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ subject_set_string_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    lv_subject_set_string(user_data->subject, user_data->ମୂଲ୍ୟ);
}

ସ୍ଥିର ଶୂନ୍ୟ subject_increment_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    lv_subject_increment_dsc_t * user_data = lv_event_get_user_data(e);

    ଯଦି(user_data->subject->type == LV_SUBJECT_TYPE_INT) {
        /*Use the smaller range*/
        int32_t max_ମୂଲ୍ୟ = LV_MIN(user_data->max_ମୂଲ୍ୟ, user_data->subject->max_ମୂଲ୍ୟ.ଅଙ୍କ);
        int32_t min_ମୂଲ୍ୟ = LV_MAX(user_data->min_ମୂଲ୍ୟ, user_data->subject->min_ମୂଲ୍ୟ.ଅଙ୍କ);

        int32_t ମୂଲ୍ୟ = lv_subject_get_int(user_data->subject);
        ମୂଲ୍ୟ += user_data->step;

        ଯଦି(user_data->rollover) {
            ଯଦି(ମୂଲ୍ୟ > max_ମୂଲ୍ୟ) {
                ମୂଲ୍ୟ = min_ମୂଲ୍ୟ;
            }
            ଅନ୍ୟଥା ଯଦି(ମୂଲ୍ୟ < min_ମୂଲ୍ୟ) {
                ମୂଲ୍ୟ = max_ମୂଲ୍ୟ;
            }
        }
        ଅନ୍ୟଥା {
            ମୂଲ୍ୟ = LV_CLAMP(min_ମୂଲ୍ୟ, ମୂଲ୍ୟ, max_ମୂଲ୍ୟ);
        }

        lv_subject_set_int(user_data->subject, ମୂଲ୍ୟ);
    }
#ଯଦି LV_USE_FLOAT
    ଅନ୍ୟଥା ଯଦି(user_data->subject->type == LV_SUBJECT_TYPE_FLOAT) {
        /*Use the smaller range*/
        ଦଶମିକ max_ମୂଲ୍ୟ = LV_MIN((ଦଶମିକ)user_data->max_ମୂଲ୍ୟ, user_data->subject->max_ମୂଲ୍ୟ.float_v);
        ଦଶମିକ min_ମୂଲ୍ୟ = LV_MAX((ଦଶମିକ)user_data->min_ମୂଲ୍ୟ, user_data->subject->min_ମୂଲ୍ୟ.float_v);


        ଦଶମିକ ମୂଲ୍ୟ = lv_subject_get_float(user_data->subject);
        ମୂଲ୍ୟ += (ଦଶମିକ)user_data->step;

        ଯଦି(user_data->rollover) {
            ଯଦି(ମୂଲ୍ୟ > max_ମୂଲ୍ୟ) {
                ମୂଲ୍ୟ = min_ମୂଲ୍ୟ;
            }
            ଅନ୍ୟଥା ଯଦି(ମୂଲ୍ୟ < min_ମୂଲ୍ୟ) {
                ମୂଲ୍ୟ = max_ମୂଲ୍ୟ;
            }
        }
        ଅନ୍ୟଥା {
            ମୂଲ୍ୟ = LV_CLAMP(min_ମୂଲ୍ୟ, ମୂଲ୍ୟ, max_ମୂଲ୍ୟ);
        }

        lv_subject_set_float(user_data->subject, ମୂଲ୍ୟ);
    }
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ଖାଲି);
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

ସ୍ଥିର ଶୂନ୍ୟ unsubscribe_on_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_delete(observer);
}

ସ୍ଥିର lv_observer_t * bind_state(lv_subject_t * subject, lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(obj != ଖାଲି);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_INT);

    bound_state_t * p = lv_malloc(ଆକାର(bound_state_t));
    ଯଦି(p == ଖାଲି) {
        LV_LOG_WARN("Out of memory");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    p->state = state;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, obj_state_observer_cb, obj, p);
    observable->auto_free_user_data = 1;

    ଫେରନ୍ତୁ observable;
}

ସ୍ଥିର ଶୂନ୍ୟ obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_ASSERT(observer != ଖାଲି);
    LV_ASSERT(subject != ଖାଲି);
    bound_state_t * p = observer->user_data;
    LV_ASSERT(p != ଖାଲି);

    ଯଦି(subject->ମୂଲ୍ୟ.ଅଙ୍କ) {
        lv_obj_add_state(observer->target, p->state);
    }
    ଅନ୍ୟଥା {
        lv_obj_remove_state(observer->target, p->state);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ obj_ମୂଲ୍ୟ_changed_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(obj != ଖାଲି);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

ସ୍ଥିର ଶୂନ୍ୟ lv_subject_notify_if_changed(lv_subject_t * subject)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(subject->type != LV_SUBJECT_TYPE_INVALID && subject->type != LV_SUBJECT_TYPE_NONE);

    ବାଛ(subject->type) {
        ସ୍ଥିତି LV_SUBJECT_TYPE_INT :
            ଯଦି(subject->ମୂଲ୍ୟ.ଅଙ୍କ != subject->prev_ମୂଲ୍ୟ.ଅଙ୍କ) {
                lv_subject_notify(subject);
            }
            ଭାଙ୍ଗ;
#ଯଦି LV_USE_FLOAT
        ସ୍ଥିତି LV_SUBJECT_TYPE_FLOAT :
            ଯଦି(subject->ମୂଲ୍ୟ.float_v != subject->prev_ମୂଲ୍ୟ.float_v) {
                lv_subject_notify(subject);
            }
            ଭାଙ୍ଗ;
#endif
        ସ୍ଥିତି LV_SUBJECT_TYPE_GROUP :
        ସ୍ଥିତି LV_SUBJECT_TYPE_POINTER :
            /* Always notify as we don't know how to compare this */
            lv_subject_notify(subject);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_COLOR  :
            ଯଦି(!lv_color_eq(subject->ମୂଲ୍ୟ.color, subject->prev_ମୂଲ୍ୟ.color)) {
                lv_subject_notify(subject);
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_STRING:
            ଯଦି(!subject->prev_ମୂଲ୍ୟ.pointer ||
               lv_strcmp(subject->ମୂଲ୍ୟ.pointer, subject->prev_ମୂଲ୍ୟ.pointer)) {
                lv_subject_notify(subject);
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LV_SUBJECT_TYPE_INVALID :
        ସ୍ଥିତି LV_SUBJECT_TYPE_NONE :
            LV_UNREACHABLE();
    }
}

ସ୍ଥିର ଶୂନ୍ୟ subject_set_string_free_user_data_event_cb(lv_event_t * e)
{
    LV_ASSERT(e != ଖାଲି);
    subject_set_string_user_data_t * user_data = lv_event_get_user_data(e);
    LV_ASSERT(user_data != ଖାଲି);
    lv_free((ଶୂନ୍ୟ *)user_data->ମୂଲ୍ୟ);
    lv_free(user_data);
}

ସ୍ଥିର ଶୂନ୍ୟ set_bool_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_bool_t set_bool_cb = (lv_obj_set_bool_t)observer->user_cb;
    ଯଦି(set_bool_cb) set_bool_cb(obj, subject->ମୂଲ୍ୟ.ଅଙ୍କ);
}

ସ୍ଥିର ଶୂନ୍ୟ set_int_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_int_t set_int_cb = (lv_obj_set_int_t)observer->user_cb;
    ଯଦି(set_int_cb) set_int_cb(obj, subject->ମୂଲ୍ୟ.ଅଙ୍କ);
}

#ଯଦି LV_USE_FLOAT
ସ୍ଥିର ଶୂନ୍ୟ set_float_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_float_t set_float_cb = (lv_obj_set_float_t)observer->user_cb;
    ଯଦି(set_float_cb) set_float_cb(obj, subject->ମୂଲ୍ୟ.float_v);
}
#endif /*LV_USE_FLOAT*/

ସ୍ଥିର ଶୂନ୍ୟ set_string_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_string_t set_string_cb = (lv_obj_set_string_t)observer->user_cb;
    ଯଦି(set_string_cb) set_string_cb(obj, subject->ମୂଲ୍ୟ.pointer);
}


ସ୍ଥିର ଶୂନ୍ୟ set_color_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_color_t set_color_cb = (lv_obj_set_color_t)observer->user_cb;
    ଯଦି(set_color_cb) set_color_cb(obj, subject->ମୂଲ୍ୟ.color);
}

ସ୍ଥିର ଶୂନ୍ୟ set_pointer_observer(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_obj_t * obj = (lv_obj_t *)observer->target;
    lv_obj_set_pointer_t set_pointer_cb = (lv_obj_set_pointer_t)observer->user_cb;
    ଯଦି(set_pointer_cb) set_pointer_cb(obj, subject->ମୂଲ୍ୟ.pointer);
}

ସ୍ଥିର ଶୂନ୍ୟ remove_observers_from_group_subject(lv_subject_t * subject)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(subject->type == LV_SUBJECT_TYPE_GROUP);

    ପାଇଁ(uint32_t i = 0; i < subject->size; ++i) {
        lv_subject_t * sub = ((lv_subject_t **)(subject->ମୂଲ୍ୟ.pointer))[i];
        subject_remove_observer(sub, group_notify_cb, subject);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ subject_remove_observer(lv_subject_t * subject, lv_observer_cb_t cb, ଶୂନ୍ୟ * user_data)
{
    LV_ASSERT(subject != ଖାଲି);
    LV_ASSERT(cb != ଖାଲି);

    lv_observer_t * obs = lv_ll_get_head(&subject->subs_ll);
    ଯେତେବେଳେ(obs) {
        lv_observer_t * obs_next = lv_ll_get_next(&subject->subs_ll, obs);
        ଯଦି(obs->cb == cb && obs->user_data == user_data) {
            lv_observer_delete(obs);
        }
        obs = obs_next;
    }
}

#endif /*LV_USE_OBSERVER*/
