/**
 * @file lv_obj_event.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "../misc/lv_event_private.h"
#समावेश "../lvgl_public.h"
#समावेश "lv_obj_event_private.h"
#समावेश "lv_obj_class_private.h"
#समावेश "lv_obj_private.h"
#समावेश "../indev/lv_indev_private.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल lv_परिणाम_t event_send_core(lv_event_t * e);
अटल bool event_is_bubbled(lv_event_t * e);
अटल bool event_is_trickled(lv_event_t * e);
#अगर LV_USE_CHECK_ARG
    अटल bool event_code_in_array(lv_event_code_t code, स्थिर lv_event_code_t * arr, uint32_t len);
#पूर्ण_अगर

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#अगर LV_USE_LOG && LV_LOG_TRACE_EVENT
    #घोषणा LV_TRACE_EVENT(...) LV_LOG_TRACE(__VA_ARGS__)
#अन्यथा
    #घोषणा LV_TRACE_EVENT(...)
#पूर्ण_अगर

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_परिणाम_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, व्योम * param)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    lv_event_t e;
    e.current_target = obj;
    e.original_target = obj;
    e.code = event_code;
    e.user_तथ्य = शून्य;
    e.param = param;
    e.deleted = 0;
    e.stop_bubbling = 0;
    e.stop_processing = 0;
    e.stop_trickling = 0;

    lv_event_धकेलो(&e);

    /*Send the event*/
    lv_परिणाम_t res = event_send_core(&e);

    /*Remove this element from the list*/
    lv_event_निकालो(&e);

    वापस res;
}

lv_परिणाम_t lv_obj_event_base(स्थिर lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस LV_परिणाम_INVALID);
    LV_CHECK_ARG(e->current_target != शून्य, वापस LV_परिणाम_INVALID);

    स्थिर lv_obj_class_t * base;
    अगर(class_p == शून्य) base = ((lv_obj_t *)e->current_target)->class_p;
    अन्यथा base = class_p->base_class;

    /*Find a base in which call the ancestor's event handler_cb if set*/
    जबतक(base && base->event_cb == शून्य) base = base->base_class;

    अगर(base == शून्य) वापस LV_परिणाम_OK;
    अगर(base->event_cb == शून्य) वापस LV_परिणाम_OK;

    /*Call the actual event callback*/
    e->user_तथ्य = शून्य;
    LV_PROFILER_EVENT_BEGIN_TAG(lv_event_code_get_name(e->code));
    base->event_cb(base, e);
    LV_PROFILER_EVENT_END_TAG(lv_event_code_get_name(e->code));

    lv_परिणाम_t res = LV_परिणाम_OK;
    /*Stop if the object is deleted*/
    अगर(e->deleted) res = LV_परिणाम_INVALID;

    वापस res;
}

lv_event_dsc_t * lv_obj_add_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(event_cb != शून्य, वापस शून्य);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस शून्य;
    }

    वापस lv_event_add(&obj->spec_attr->event_list, event_cb, filter, user_तथ्य);
}

uint32_t lv_obj_get_event_गिनती(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस 0);
    अगर(obj->spec_attr == शून्य) वापस 0;
    वापस lv_event_get_गिनती(&obj->spec_attr->event_list);
}

lv_event_dsc_t * lv_obj_get_event_dsc(lv_obj_t * obj, uint32_t सूचकांक)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    अगर(obj->spec_attr == शून्य) वापस शून्य;
    वापस lv_event_get_dsc(&obj->spec_attr->event_list, सूचकांक);
}

bool lv_obj_remove_event(lv_obj_t * obj, uint32_t सूचकांक)
{
    LV_CHECK_ARG(obj != शून्य, वापस false);
    अगर(obj->spec_attr == शून्य) वापस false;
    वापस lv_event_remove(&obj->spec_attr->event_list, सूचकांक);
}

bool lv_obj_remove_event_dsc(lv_obj_t * obj, lv_event_dsc_t * dsc)
{
    LV_CHECK_ARG(obj != शून्य, वापस false);
    LV_CHECK_ARG(dsc != शून्य, वापस false);

    अगर(obj->spec_attr == शून्य) वापस false;
    वापस lv_event_remove_dsc(&obj->spec_attr->event_list, dsc);
}

uint32_t lv_obj_remove_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb)
{
    LV_CHECK_ARG(obj != शून्य, वापस 0);

    uint32_t event_cnt = lv_obj_get_event_गिनती(obj);
    uint32_t removed_गिनती = 0;
    int32_t i;

    अगर(event_cnt == 0) वापस 0;

    क्रम(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        अगर(dsc && (event_cb == शून्य || dsc->cb == event_cb)) {
            lv_obj_remove_event(obj, i);
            removed_गिनती++;
        }
    }

    वापस removed_गिनती;
}

uint32_t lv_obj_remove_event_cb_with_user_तथ्य(lv_obj_t * obj, lv_event_cb_t event_cb, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस 0);

    uint32_t event_cnt = lv_obj_get_event_गिनती(obj);
    uint32_t removed_गिनती = 0;
    int32_t i;

    अगर(event_cnt == 0) वापस 0;

    क्रम(i = event_cnt - 1; i >= 0; i--) {
        lv_event_dsc_t * dsc = lv_obj_get_event_dsc(obj, i);
        अगर(dsc && (event_cb == शून्य || dsc->cb == event_cb) && dsc->user_तथ्य == user_तथ्य) {
            lv_obj_remove_event(obj, i);
            removed_गिनती ++;
        }
    }

    वापस removed_गिनती;
}

lv_obj_t * lv_event_get_current_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    वापस lv_event_get_current_target(e);
}

lv_obj_t * lv_event_get_target_obj(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    वापस lv_event_get_target(e);
}

lv_indev_t * lv_event_get_indev(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
#अगर LV_USE_CHECK_ARG
    अटल स्थिर lv_event_code_t indev_codes[] = {
        LV_EVENT_PRESSED,           LV_EVENT_PRESSING,          LV_EVENT_PRESS_LOST,
        LV_EVENT_SHORT_CLICKED,     LV_EVENT_LONG_PRESSED,      LV_EVENT_LONG_PRESSED_REPEAT,
        LV_EVENT_CLICKED,           LV_EVENT_RELEASED,
        LV_EVENT_SCROLL_BEGIN,      LV_EVENT_SCROLL_END,        LV_EVENT_SCROLL,
        LV_EVENT_GESTURE,           LV_EVENT_GESTURE_UP,        LV_EVENT_GESTURE_DOWN,
        LV_EVENT_GESTURE_बायाँ,      LV_EVENT_GESTURE_दायाँ,
        LV_EVENT_कुंजी,
        LV_EVENT_FOCUSED,           LV_EVENT_DEFOCUSED,         LV_EVENT_LEAVE,
        LV_EVENT_HOVER_OVER,        LV_EVENT_HOVER_LEAVE,
    };
#पूर्ण_अगर
    LV_CHECK_ARG_FORMAT_संदेश(
        event_code_in_array(e->code, indev_codes, माप(indev_codes) / माप(indev_codes[0])),
        वापस शून्य,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

lv_layer_t * lv_event_get_layer(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
#अगर LV_USE_CHECK_ARG
    अटल स्थिर lv_event_code_t draw_codes[] = {
        LV_EVENT_DRAW_MAIN,     LV_EVENT_DRAW_MAIN_BEGIN,   LV_EVENT_DRAW_MAIN_END,
        LV_EVENT_DRAW_POST,     LV_EVENT_DRAW_POST_BEGIN,   LV_EVENT_DRAW_POST_END,
    };
#पूर्ण_अगर
    LV_CHECK_ARG_FORMAT_संदेश(
        event_code_in_array(e->code, draw_codes, माप(draw_codes) / माप(draw_codes[0])),
        वापस शून्य,
        "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

स्थिर lv_area_t * lv_event_get_old_आकार(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_आकार_CHANGED,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

uint32_t lv_event_get_कुंजी(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_कुंजी,
                            वापस 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    uint32_t * k = lv_event_get_param(e);
    वापस k ? *k : 0;
}

int32_t lv_event_get_rotary_diff(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_ROTARY,
                            वापस 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * r = lv_event_get_param(e);
    वापस r ? *r : 0;
}

lv_anim_t * lv_event_get_scroll_anim(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_SCROLL_BEGIN,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

व्योम lv_event_set_ext_draw_आकार(lv_event_t * e, int32_t आकार)
{
    LV_CHECK_ARG(e != शून्य, वापस);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_REFR_EXT_DRAW_आकार,
                            वापस,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    int32_t * cur_आकार = lv_event_get_param(e);
    *cur_आकार = LV_MAX(*cur_आकार, आकार);
}

lv_point_t * lv_event_get_self_आकार_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_GET_SELF_आकार,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

lv_hit_test_info_t * lv_event_get_hit_test_info(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_HIT_TEST,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

स्थिर lv_area_t * lv_event_get_cover_area(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_COVER_CHECK,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    वापस p->area;
}

व्योम lv_event_set_cover_res(lv_event_t * e, lv_cover_res_t res)
{
    LV_CHECK_ARG(e != शून्य, वापस);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_COVER_CHECK,
                            वापस,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_cover_check_info_t * p = lv_event_get_param(e);
    अगर(res > p->res) p->res = res;  /*Save only "stronger" results*/
}

lv_draw_task_t * lv_event_get_draw_task(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस शून्य);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_DRAW_TASK_ADDED,
                            वापस शून्य,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    वापस lv_event_get_param(e);
}

lv_state_t lv_event_get_पूर्व_state(lv_event_t * e)
{
    LV_CHECK_ARG(e != शून्य, वापस 0);
    LV_CHECK_ARG_FORMAT_संदेश(e->code == LV_EVENT_STATE_CHANGED,
                            वापस 0,
                            "invalid event code %" LV_PRId32, (int32_t)e->code);
    lv_state_t * state = lv_event_get_param(e);
    वापस state ? *state : 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल lv_परिणाम_t event_send_core(lv_event_t * e)
{
    LV_TRACE_EVENT("Sending event %d to %p with %p param", e->code, (व्योम *)e->original_target, e->param);

    lv_indev_t * indev_act = lv_indev_active();
    अगर(indev_act) {
        अगर(e->stop_processing) वापस LV_परिणाम_OK;
        अगर(e->deleted) वापस LV_परिणाम_INVALID;
    }

    lv_obj_t * target = e->current_target;
    lv_परिणाम_t res = LV_परिणाम_OK;
    lv_event_list_t * list = target->spec_attr ?  &target->spec_attr->event_list : शून्य;

    res = lv_event_send(list, e, true);
    अगर(res != LV_परिणाम_OK || e->stop_processing) वापस res;

    res = lv_obj_event_base(शून्य, e);
    अगर(res != LV_परिणाम_OK || e->stop_processing) वापस res;

    res = lv_event_send(list, e, false);
    अगर(res != LV_परिणाम_OK || e->stop_processing) वापस res;

    lv_obj_t * parent = lv_obj_get_parent(e->current_target);
    अगर(parent && event_is_bubbled(e)) {
        e->current_target = parent;
        res = event_send_core(e);
    }
    अगर(res != LV_परिणाम_OK) वापस res;

    /*Trickle down to children if enabled*/
    अगर(event_is_trickled(e)) {
        uint32_t child_गिनती = lv_obj_get_child_गिनती(target);

        /* we don't want the event to bubble up again when trickling down */
        e->stop_bubbling = 1;

        क्रम(uint32_t i = 0; i < child_गिनती && res == LV_परिणाम_OK && !e->stop_processing; i++) {
            lv_obj_t * child = lv_obj_get_child(target, i);
            अगर(child) {
                e->current_target = child;
                res = event_send_core(e);
                अगर(res != LV_परिणाम_OK) {
                    LV_LOG_WARN("Trickle down event %d to child %p failed", e->code, (व्योम *)child);
                    अवरोध;
                }
            }
        }
    }

    वापस res;
}

अटल bool event_is_bubbled(lv_event_t * e)
{
    अगर(e->stop_bubbling) वापस false;

    /*Event codes that always bubble*/
    चयन(e->code) {
        हाल LV_EVENT_CHILD_CREATED:
        हाल LV_EVENT_CHILD_DELETED:
            वापस true;
        शेष:
            अवरोध;
    }

    /*Check other codes only if bubbling is enabled*/
    अगर(lv_obj_is_event_bubble(e->current_target) == false) वापस false;

    चयन(e->code) {
        हाल LV_EVENT_HIT_TEST:
        हाल LV_EVENT_COVER_CHECK:
        हाल LV_EVENT_REFR_EXT_DRAW_आकार:
        हाल LV_EVENT_DRAW_MAIN_BEGIN:
        हाल LV_EVENT_DRAW_MAIN:
        हाल LV_EVENT_DRAW_MAIN_END:
        हाल LV_EVENT_DRAW_POST_BEGIN:
        हाल LV_EVENT_DRAW_POST:
        हाल LV_EVENT_DRAW_POST_END:
        हाल LV_EVENT_DRAW_TASK_ADDED:
        हाल LV_EVENT_REFRESH:
        हाल LV_EVENT_DELETE:
        हाल LV_EVENT_CHILD_CREATED:
        हाल LV_EVENT_CHILD_DELETED:
        हाल LV_EVENT_CHILD_CHANGED:
        हाल LV_EVENT_आकार_CHANGED:
        हाल LV_EVENT_STYLE_CHANGED:
        हाल LV_EVENT_GET_SELF_आकार:
            वापस false;
        शेष:
            वापस true;
    }
}

अटल bool event_is_trickled(lv_event_t * e)
{
    अगर(e->stop_trickling) वापस false;

    /*Check other codes only if trickle is enabled*/
    अगर(lv_obj_is_event_trickle(e->current_target) == false) वापस false;

    चयन(e->code) {
        हाल LV_EVENT_HIT_TEST:
        हाल LV_EVENT_COVER_CHECK:
        हाल LV_EVENT_REFR_EXT_DRAW_आकार:
        हाल LV_EVENT_DRAW_MAIN_BEGIN:
        हाल LV_EVENT_DRAW_MAIN:
        हाल LV_EVENT_DRAW_MAIN_END:
        हाल LV_EVENT_DRAW_POST_BEGIN:
        हाल LV_EVENT_DRAW_POST:
        हाल LV_EVENT_DRAW_POST_END:
        हाल LV_EVENT_DRAW_TASK_ADDED:
        हाल LV_EVENT_REFRESH:
        हाल LV_EVENT_DELETE:
        हाल LV_EVENT_CHILD_CREATED:
        हाल LV_EVENT_CHILD_DELETED:
        हाल LV_EVENT_CHILD_CHANGED:
        हाल LV_EVENT_आकार_CHANGED:
        हाल LV_EVENT_STYLE_CHANGED:
        हाल LV_EVENT_GET_SELF_आकार:
            वापस false;
        शेष:
            वापस true;
    }
}

#अगर LV_USE_CHECK_ARG
अटल bool event_code_in_array(lv_event_code_t code, स्थिर lv_event_code_t * arr, uint32_t len)
{
    uint32_t i;
    क्रम(i = 0; i < len; i++) {
        अगर(code == arr[i]) वापस true;
    }
    वापस false;
}
#पूर्ण_अगर
