/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../misc/lv_event_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_event_private.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#परिभाषय MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
स्थिर lv_परिणाम_t event_send_core(lv_event_t * e);
स्थिर bool event_is_bubbled(lv_event_t * e);
स्थिर bool event_is_trickled(lv_event_t * e);
#यदि LV_USE_CHECK_ARG
    स्थिर bool event_संहिता_अन्तर्_array(lv_event_संहिता_t संहिता, ध्रुव lv_event_संहिता_t * arr, uint32_t len);
#यदि_अन्तः

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#यदि LV_USE_LOG && LV_LOG_TRACE_EVENT
    #परिभाषय LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#अन्यथा
    #परिभाषय LV_TRACE_EVENT(...)
#यदि_अन्तः

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_परिणाम_t lv_obj_send_event(lv_obj_t * obj, lv_event_संहिता_t event_संहिता, शून्य * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.संहिता = event_संहिता;
    e.user_दत्त = रिक्त;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_प्रक्षिप(&e);

    /*Send the event*/
    lv_परिणाम_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_निष्कासय(&e);

    प्रत्यागम res;
}

lv_परिणाम_t lv_obj_event_आधारः(ध्रुव lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम LV_परिणाम_INVALID);
    LV_CHECK_ARG(e->current_target != रिक्त, प्रत्यागम LV_परिणाम_INVALID);

    ध्रुव lv_obj_class_t * आधारः;
    यदि(class_p == रिक्त) आधारः = ((lv_obj_t *)e->current_target)->class_p;
    अन्यथा आधारः = class_p->आधारः_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    यावत्(आधारः && आधारः->event_cb == रिक्त) आधारः = आधारः->आधारः_class;

    यदि(आधारः == रिक्त) प्रत्यागम LV_परिणाम_OK;
    यदि(आधारः->event_cb == रिक्त) प्रत्यागम LV_परिणाम_OK;

    /*Call the actual event callback*/
    e->user_दत्त = रिक्त;
    LV_PROFILER_EVENT_BEGIN_चिह्नकम्(lv_event_संहिता_get_name(e->संहिता));
    आधारः->event_cb(आधारः, e);
    LV_PROFILER_EVENT_अन्तः_चिह्नकम्(lv_event_संहिता_get_name(e->संहिता));

    lv_परिणाम_t res = LV_परिणाम_OK;
    /*Stop if the object is deleted*/
    यदि(e->deleted) res = LV_परिणाम_INVALID;

    प्रत्यागम res;
}

lv_event_dsc_t * lv_obj_सङ्कलय_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_संहिता_t filter, शून्य * user_दत्त)
{
    LV_CHECK_ARG(event_cb != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    यदि(!lv_obj_allocate_spec_attr(obj)) {
        प्रत्यागम रिक्त;
    }

    प्रत्यागम lv_event_सङ्कलय(&obj->spec_attr->event_list, event_cb, filter, user_दत्त);
}

uint32_t lv_obj_get_event_गणनम्(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम 0);
    यदि(obj->spec_attr == रिक्त) प्रत्यागम 0;
    प्रत्यागम lv_event_get_गणनम्(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t सूचकांक)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    यदि(obj->spec_attr == रिक्त) प्रत्यागम रिक्त;
    प्रत्यागम lv_event_get_dsc(&obj->spec_attr->event_list, सूचकांक);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t सूचकांक)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);
    यदि(obj->spec_attr == रिक्त) प्रत्यागम false;
    प्रत्यागम lv_event_remove(&obj->spec_attr->event_list, सूचकांक);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);
    LV_CHECK_ARG(dsc != रिक्त, प्रत्यागम false);

    यदि(obj->spec_attr == रिक्त) प्रत्यागम false;
    प्रत्यागम lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम 0);

    uint32_t event_cnt = lv_obj_get_event_गणनम्(obj);
    uint32_t removed_गणनम् = 0;
    int32_t i;

    यदि(event_cnt == 0) प्रत्यागम 0;

    प्रति(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        यदि(dsc && (event_cb == रिक्त || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_गणनम्++;
        }
    }

    प्रत्यागम removed_गणनम्;
}

uint32_t lv_obj_remove_event_cb_with_user_दत्त(lv_obj_t * obj, lv_event_cb_t event_cb, शून्य * user_दत्त)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम 0);

    uint32_t event_cnt = lv_obj_get_event_गणनम्(obj);
    uint32_t removed_गणनम् = 0;
    int32_t i;

    यदि(event_cnt == 0) प्रत्यागम 0;

    प्रति(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        यदि(dsc && (event_cb == रिक्त || dsc->cb == event_cb) && dsc->user_दत्त == user_दत्त) {
            lv_obj_remove_event(obj, i);
            removed_गणनम् ++;
        }
    }

    प्रत्यागम removed_गणनम्;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    प्रत्यागम lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    प्रत्यागम lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
#यदि LV_USE_CHECK_ARG
    स्थिर ध्रुव lv_event_संहिता_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_नष्टम्,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_अन्तः,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_ऊर्ध्वम्,        LV_EVENT_GESTURE_अधस्तात्,
        LV_EVENT_GESTURE_वामः,      LV_EVENT_GESTURE_दक्षिणः,
        LV_EVENT_कुञ्चिका,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#यदि_अन्तः
    LV_CHECK_ARG_FORMAT_सन्देशः(
        event_संहिता_अन्तर्_array(e->संहिता, indev_codes, परिमाण(indev_codes) / परिमाण(indev_codes[0])),
        प्रत्यागम रिक्त,
        "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
#यदि LV_USE_CHECK_ARG
    स्थिर ध्रुव lv_event_संहिता_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_अन्तः,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_अन्तः,
    };
#यदि_अन्तः
    LV_CHECK_ARG_FORMAT_सन्देशः(
        event_संहिता_अन्तर्_array(e->संहिता, draw_codes, परिमाण(draw_codes) / परिमाण(draw_codes[0])),
        प्रत्यागम रिक्त,
        "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

ध्रुव lv_area_t * lv_event_get_old_आकार(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_आकार_परिवर्तितम्,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

uint32_t lv_event_get_कुञ्चिका(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम 0);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_कुञ्चिका,
                            प्रत्यागम 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    uint32_t * k = lv_event_get_param(e);
    प्रत्यागम k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम 0);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_ROTARY,
                            प्रत्यागम 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    int32_t * r = lv_event_get_param(e);
    प्रत्यागम r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_SCROLL_BEGIN,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

शून्य lv_event_समुच्चयः_ext_draw_आकार(lv_event_t * e, int32_t आकार)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_REFR_EXT_DRAW_आकार,
                            प्रत्यागम,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    int32_t * cur_आकार = lv_event_get_param(e);
    *cur_आकार = LV_MAX(*cur_आकार, आकार);
}

lv_point_t * lv_event_get_self_आकार_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_GET_SELF_आकार,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

lv_लब्धम्_test_info_t * lv_event_get_लब्धम्_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_लब्धम्_TEST,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

ध्रुव lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_COVER_CHECK,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    प्रत्यागम p->area;
}

शून्य lv_event_समुच्चयः_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_COVER_CHECK,
                            प्रत्यागम,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    यदि(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_DRAW_TASK_ADDED,
                            प्रत्यागम रिक्त,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    प्रत्यागम lv_event_get_param(e);
}

lv_state_t lv_event_get_पूर्व_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != रिक्त, प्रत्यागम 0);
    LV_CHECK_ARG_FORMAT_सन्देशः(e->संहिता == LV_EVENT_STATE_परिवर्तितम्,
                            प्रत्यागम 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->संहिता);
    lv_state_t * state = lv_event_get_param(e);
    प्रत्यागम state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर lv_परिणाम_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->संहिता, (शून्य *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_सक्रिय();
    यदि(indev_act) {
        यदि(e->stop_processing) प्रत्यागम LV_परिणाम_OK;
        यदि(e->deleted) प्रत्यागम LV_परिणाम_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_परिणाम_t res = LV_परिणाम_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : रिक्त;

    res = lv_event_send(list, e, true);
    यदि(res != LV_परिणाम_OK || e->stop_processing) प्रत्यागम res;

    res = lv_obj_event_आधारः(रिक्त, e);
    यदि(res != LV_परिणाम_OK || e->stop_processing) प्रत्यागम res;

    res = lv_event_send(list, e, false);
    यदि(res != LV_परिणाम_OK || e->stop_processing) प्रत्यागम res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    यदि(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    यदि(res != LV_परिणाम_OK) प्रत्यागम res;

    /*Trickle down to children if enabled*/
    यदि(event_is_trickled(e)) {
        uint32_t child_गणनम् = lv_obj_get_child_गणनम्(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        प्रति(uint32_t i = 0; i < child_गणनम् && res == LV_परिणाम_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            यदि(child) {
                e->current_target = child;
                res = event_send_core(e);
                यदि(res != LV_परिणाम_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->संहिता, (शून्य *)child);
                    भङ्ग;
                }
            }
        }
    }

    प्रत्यागम res;
}

स्थिर bool event_is_bubbled(lv_event_t * e)
{
    यदि(e->stop_bubbling) प्रत्यागम false;

    /*Event codes that always bubble*/
    विकल्प(e->संहिता) {
        पक्ष LV_EVENT_CHILD_CREATED:
        पक्ष LV_EVENT_CHILD_DELETED:
            प्रत्यागम true;
        स्वभाव:
            भङ्ग;
    }

    /*Check other codes only if bubbling is enabled*/
    यदि(lv_obj_is_event_bubble(e->current_target) == false) प्रत्यागम false;

    विकल्प(e->संहिता) {
        पक्ष LV_EVENT_लब्धम्_TEST:
        पक्ष LV_EVENT_COVER_CHECK:
        पक्ष LV_EVENT_REFR_EXT_DRAW_आकार:
        पक्ष LV_EVENT_DRAW_MAIN_BEGIN:
        पक्ष LV_EVENT_DRAW_MAIN:
        पक्ष LV_EVENT_DRAW_MAIN_अन्तः:
        पक्ष LV_EVENT_DRAW_POST_BEGIN:
        पक्ष LV_EVENT_DRAW_POST:
        पक्ष LV_EVENT_DRAW_POST_अन्तः:
        पक्ष LV_EVENT_DRAW_TASK_ADDED:
        पक्ष LV_EVENT_REFRESH:
        पक्ष LV_EVENT_DELETE:
        पक्ष LV_EVENT_CHILD_CREATED:
        पक्ष LV_EVENT_CHILD_DELETED:
        पक्ष LV_EVENT_CHILD_परिवर्तितम्:
        पक्ष LV_EVENT_आकार_परिवर्तितम्:
        पक्ष LV_EVENT_STYLE_परिवर्तितम्:
        पक्ष LV_EVENT_GET_SELF_आकार:
            प्रत्यागम false;
        स्वभाव:
            प्रत्यागम true;
    }
}

स्थिर bool event_is_trickled(lv_event_t * e)
{
    यदि(e->stop_trickling) प्रत्यागम false;

    /*Check other codes only if trickle is enabled*/
    यदि(lv_obj_is_event_trickle(e->current_target) == false) प्रत्यागम false;

    विकल्प(e->संहिता) {
        पक्ष LV_EVENT_लब्धम्_TEST:
        पक्ष LV_EVENT_COVER_CHECK:
        पक्ष LV_EVENT_REFR_EXT_DRAW_आकार:
        पक्ष LV_EVENT_DRAW_MAIN_BEGIN:
        पक्ष LV_EVENT_DRAW_MAIN:
        पक्ष LV_EVENT_DRAW_MAIN_अन्तः:
        पक्ष LV_EVENT_DRAW_POST_BEGIN:
        पक्ष LV_EVENT_DRAW_POST:
        पक्ष LV_EVENT_DRAW_POST_अन्तः:
        पक्ष LV_EVENT_DRAW_TASK_ADDED:
        पक्ष LV_EVENT_REFRESH:
        पक्ष LV_EVENT_DELETE:
        पक्ष LV_EVENT_CHILD_CREATED:
        पक्ष LV_EVENT_CHILD_DELETED:
        पक्ष LV_EVENT_CHILD_परिवर्तितम्:
        पक्ष LV_EVENT_आकार_परिवर्तितम्:
        पक्ष LV_EVENT_STYLE_परिवर्तितम्:
        पक्ष LV_EVENT_GET_SELF_आकार:
            प्रत्यागम false;
        स्वभाव:
            प्रत्यागम true;
    }
}

#यदि LV_USE_CHECK_ARG
स्थिर bool event_संहिता_अन्तर्_array(lv_event_संहिता_t संहिता, ध्रुव lv_event_संहिता_t * arr, uint32_t len)
{
    uint32_t i;
    प्रति(i = 0; i < len; i++) {
        यदि(संहिता == arr[i]) प्रत्यागम true;
    }
    प्रत्यागम false;
}
#यदि_अन्तः
