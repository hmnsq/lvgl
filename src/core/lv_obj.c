/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_private.h"
#شامل "../lvgl_public.h"
#شامل "../misc/lv_event_private.h"
#شامل "../misc/lv_style_private.h"
#شامل "../misc/lv_area_private.h"
#شامل "lv_obj_style_private.h"
#شامل "lv_obj_event_private.h"
#شامل "lv_obj_class_private.h"
#شامل "../indev/lv_indev_private.h"
#شامل "../display/lv_display_private.h"
#شامل "lv_obj_draw_private.h"
#شامل "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)
#تعریف_کرو LV_OBJ_DEF_چوڑائی    (LV_DPX(100))
#تعریف_کرو LV_OBJ_DEF_اونچائی   (LV_DPX(50))
#تعریف_کرو STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

قسمتعریف ساخت {
    lv_screen_بھرو_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    اتحاد {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } ہدف;
} screen_بھرو_anim_dsc_t;

قسمتعریف ساخت {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool الٹ;
} timeline_play_dsc_t;

ساخت _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    خالی * user_ڈیٹا;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساکن خالی lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساکن خالی lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساکن خالی lv_obj_draw(lv_event_t * e);
ساکن خالی lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e);
ساکن خالی draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
ساکن lv_نتیجہ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
ساکن خالی obj_transition_حالتیں(lv_obj_t * obj, lv_state_t پچھلا_state, lv_state_t new_state);
ساکن خالی update_obj_state(lv_obj_t * obj, lv_state_t new_state);
ساکن خالی lv_obj_children_جوڑو_state(lv_obj_t * obj, lv_state_t state);
ساکن خالی lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
ساکن خالی null_on_delete_cb(lv_event_t * e);
ساکن خالی screen_بھرو_on_trigger_event_cb(lv_event_t * e);
ساکن خالی screen_create_on_trigger_event_cb(lv_event_t * e);
ساکن خالی play_timeline_on_trigger_event_cb(lv_event_t * e);
ساکن خالی delete_on_screen_unloaded_event_cb(lv_event_t * e);
ساکن خالی call_delete_cb(lv_event_t * e);

ساکن اندرونی خالی سیٹ_state(lv_obj_t * obj, lv_state_t state,  bool قدر);
ساکن اندرونی bool has_state(ثابت lv_obj_t * obj, lv_state_t state);
#اگر LV_USE_CHECK_OBJ_والد_LINK && LV_USE_ASSERT
    ساکن bool والد_شامل_ہے_بچہ(ثابت lv_obj_t * والد, ثابت lv_obj_t * بچہ);
#اگر_ختم

#اگر LV_USE_OBJ_PROPERTY
    ساکن lv_نتیجہ_t lv_obj_سیٹ_any(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);
    ساکن lv_نتیجہ_t lv_obj_get_any(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

    ساکن lv_نقطہ_t lv_obj_get_scroll_انتہا_helper(lv_obj_t * obj);

    ساکن خالی obj_سیٹ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    ساکن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f);
#اگر_ختم

/**********************
 *  STATIC VARIABLES
 **********************/
#اگر LV_USE_OBJ_PROPERTY
ساکن ثابت lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_والد,
        .setter = lv_obj_سیٹ_والد,
        .getter = lv_obj_get_والد,
    },
    {
        .id = LV_PROPERTY_OBJ_X,
        .setter = lv_obj_سیٹ_x,
        .getter = lv_obj_get_x,
    },
    {
        .id = LV_PROPERTY_OBJ_Y,
        .setter = lv_obj_سیٹ_y,
        .getter = lv_obj_get_y,
    },
    {
        .id = LV_PROPERTY_OBJ_W,
        .setter = lv_obj_سیٹ_چوڑائی,
        .getter = lv_obj_get_چوڑائی,
    },
    {
        .id = LV_PROPERTY_OBJ_H,
        .setter = lv_obj_سیٹ_اونچائی,
        .getter = lv_obj_get_اونچائی,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_چوڑائی,
        .setter = lv_obj_سیٹ_content_چوڑائی,
        .getter = lv_obj_get_content_چوڑائی,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_اونچائی,
        .setter = lv_obj_سیٹ_content_اونچائی,
        .getter = lv_obj_get_content_اونچائی,
    },
    {
        .id = LV_PROPERTY_OBJ_LAYOUT,
        .setter = lv_obj_سیٹ_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_ALIGN,
        .setter = lv_obj_سیٹ_align,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLBAR_منوال,
        .setter = lv_obj_سیٹ_scrollbar_منوال,
        .getter = lv_obj_get_scrollbar_منوال,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_DIR,
        .setter = lv_obj_سیٹ_scroll_dir,
        .getter = lv_obj_get_scroll_dir,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_X,
        .setter = lv_obj_سیٹ_scroll_snap_x,
        .getter = lv_obj_get_scroll_snap_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_Y,
        .setter = lv_obj_سیٹ_scroll_snap_y,
        .getter = lv_obj_get_scroll_snap_y,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_X,
        .getter = lv_obj_get_scroll_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_Y,
        .getter = lv_obj_get_scroll_y,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_چوٹی,
        .getter = lv_obj_get_scroll_چوٹی,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_بایاں,
        .getter = lv_obj_get_scroll_بایاں,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_دایاں,
        .getter = lv_obj_get_scroll_دایاں,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_انتہا,
        .getter = lv_obj_get_scroll_انتہا_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_سائز,
        .getter = lv_obj_get_ext_draw_سائز,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_گنتی,
        .getter = lv_obj_get_event_گنتی,
    },
    {
        .id = LV_PROPERTY_OBJ_SCREEN,
        .getter = lv_obj_get_screen,
    },
    {
        .id = LV_PROPERTY_OBJ_DISPLAY,
        .getter = lv_obj_get_display,
    },
    {
        .id = LV_PROPERTY_OBJ_بچہ_گنتی,
        .getter = lv_obj_get_بچہ_گنتی,
    },
    {
        .id = LV_PROPERTY_OBJ_اشاریہ,
        .getter = lv_obj_get_اشاریہ,
    },
#اگر LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_سیٹ_name,
    },
#اگر_ختم
    {
        .id = LV_PROPERTY_OBJ_HIDDEN,
        .setter = lv_obj_سیٹ_hidden,
        .getter = lv_obj_is_hidden,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICKABLE,
        .setter = lv_obj_سیٹ_clickable,
        .getter = lv_obj_is_clickable,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICK_FOCUSABLE,
        .setter = lv_obj_سیٹ_click_focusable,
        .getter = lv_obj_is_click_focusable,
    },
    {
        .id = LV_PROPERTY_OBJ_CHECKABLE,
        .setter = lv_obj_سیٹ_checkable,
        .getter = lv_obj_is_checkable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLABLE,
        .setter = lv_obj_سیٹ_scrollable,
        .getter = lv_obj_is_scrollable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ELASTIC,
        .setter = lv_obj_سیٹ_scroll_elastic,
        .getter = lv_obj_is_scroll_elastic,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_MOMENTUM,
        .setter = lv_obj_سیٹ_scroll_momentum,
        .getter = lv_obj_is_scroll_momentum,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ONE,
        .setter = lv_obj_سیٹ_scroll_one,
        .getter = lv_obj_is_scroll_one,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_HOR,
        .setter = lv_obj_سیٹ_scroll_chain_hor,
        .getter = lv_obj_is_scroll_chain_hor,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_VER,
        .setter = lv_obj_سیٹ_scroll_chain_ver,
        .getter = lv_obj_is_scroll_chain_ver,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ON_FOCUS,
        .setter = lv_obj_سیٹ_scroll_on_focus,
        .getter = lv_obj_is_scroll_on_focus,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_WITH_ARROW,
        .setter = lv_obj_سیٹ_scroll_with_arrow,
        .getter = lv_obj_is_scroll_with_arrow,
    },
    {
        .id = LV_PROPERTY_OBJ_SNAPPABLE,
        .setter = lv_obj_سیٹ_snappable,
        .getter = lv_obj_is_snappable,
    },
    {
        .id = LV_PROPERTY_OBJ_PRESS_تالا,
        .setter = lv_obj_سیٹ_press_تالا,
        .getter = lv_obj_is_press_تالا,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_BUBBLE,
        .setter = lv_obj_سیٹ_event_bubble,
        .getter = lv_obj_is_event_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_GESTURE_BUBBLE,
        .setter = lv_obj_سیٹ_gesture_bubble,
        .getter = lv_obj_is_gesture_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_ADV_HITTEST,
        .setter = lv_obj_سیٹ_adv_hittest,
        .getter = lv_obj_is_adv_hittest,
    },
    {
        .id = LV_PROPERTY_OBJ_IGNORE_LAYOUT,
        .setter = lv_obj_سیٹ_ignore_layout,
        .getter = lv_obj_is_ignore_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_FLOATING,
        .setter = lv_obj_سیٹ_floating,
        .getter = lv_obj_is_floating,
    },
    {
        .id = LV_PROPERTY_OBJ_SEND_DRAW_TASK_EVENTS,
        .setter = lv_obj_سیٹ_send_draw_task_events,
        .getter = lv_obj_is_send_draw_task_events,
    },
    {
        .id = LV_PROPERTY_OBJ_حد_سے_تجاوز_VISIBLE,
        .setter = lv_obj_سیٹ_حد_سے_تجاوز_visible,
        .getter = lv_obj_is_حد_سے_تجاوز_visible,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_TRICKLE,
        .setter = lv_obj_سیٹ_event_trickle,
        .getter = lv_obj_is_event_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_STATE_TRICKLE,
        .setter = lv_obj_سیٹ_state_trickle,
        .getter = lv_obj_is_state_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_FLEX_اندر_NEW_TRACK,
        .setter = lv_obj_سیٹ_flex_اندر_new_track,
        .getter = lv_obj_is_flex_اندر_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_سیٹ_any,
        .getter = lv_obj_get_any,
    },
};
#اگر_ختم

ثابت lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .چوڑائی_def = LV_DPI_DEF,
    .اونچائی_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .گروہ_def = LV_OBJ_CLASS_گروہ_DEF_FALSE,
    .instance_سائز = (حجم(lv_obj_t)),
    .بنیاد_class = تہی,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * والد)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, والد);

    اگر(obj == تہی) واپس تہی;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    واپس obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

خالی lv_obj_سیٹ_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    اگر(obj->hidden == en) واپس;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    اگر(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    اگر(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    اگر(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
        اگر(گروہ != تہی) {
            lv_گروہ_focus_اگلا(گروہ);
            lv_obj_t * اگلا_obj = lv_گروہ_get_focused(گروہ);
            اگر(اگلا_obj != تہی) {
                lv_obj_invalidate(اگلا_obj);
            }
        }
    }

    اگر((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_والد(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

خالی lv_obj_سیٹ_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->clickable = en;

}

خالی lv_obj_سیٹ_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->click_focusable = en;
}

خالی lv_obj_سیٹ_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->checkable = en;
}

خالی lv_obj_سیٹ_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(obj->scrollable == en) واپس;

    /*Invalidate the scrollbars while scrollable*/
    اگر(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    اگر(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

خالی lv_obj_سیٹ_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_elastic = en;
}

خالی lv_obj_سیٹ_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_momentum = en;
}

خالی lv_obj_سیٹ_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_one = en;
}

خالی lv_obj_سیٹ_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

خالی lv_obj_سیٹ_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_chain_hor = en;
}

خالی lv_obj_سیٹ_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_chain_ver = en;
}

خالی lv_obj_سیٹ_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_on_focus = en;
}

خالی lv_obj_سیٹ_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->scroll_with_arrow = en;
}

خالی lv_obj_سیٹ_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->snappable = en;
}

خالی lv_obj_سیٹ_press_تالا(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->press_تالا = en;
}

خالی lv_obj_سیٹ_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->event_bubble = en;
}

خالی lv_obj_سیٹ_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->gesture_bubble = en;
}

خالی lv_obj_سیٹ_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->adv_hittest = en;
}

خالی lv_obj_سیٹ_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(obj->ignore_layout == en) واپس;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    اگر((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_والد(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

خالی lv_obj_سیٹ_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(obj->floating == en) واپس;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    اگر((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_والد(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

خالی lv_obj_سیٹ_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->send_draw_task_events = en;
}

خالی lv_obj_سیٹ_حد_سے_تجاوز_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->حد_سے_تجاوز_visible = en;
}

خالی lv_obj_سیٹ_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->event_trickle = en;
}

خالی lv_obj_سیٹ_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->state_trickle = en;
}

خالی lv_obj_سیٹ_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->radio_button = en;
}

خالی lv_obj_سیٹ_flex_اندر_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    اگر(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    ورنہ {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        اگر(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_والد(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

خالی lv_obj_جوڑو_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_state_t new_state = obj->state | state;
    اگر(obj->state != new_state) {
        update_obj_state(obj, new_state);
        اگر(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_جوڑو_state(obj, state);
        }
    }
}

خالی lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_state_t new_state = obj->state & (~state);
    اگر(obj->state != new_state) {
        update_obj_state(obj, new_state);
        اگر(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

خالی lv_obj_سیٹ_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, state, v);
}

خالی lv_obj_سیٹ_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_ALT, en);
}

خالی lv_obj_سیٹ_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_CHECKED, en);
}

خالی lv_obj_سیٹ_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_FOCUSED, en);
}

خالی lv_obj_سیٹ_focus_کلید(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_FOCUS_کلید, en);
}

خالی lv_obj_سیٹ_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_EDITED, en);
}

خالی lv_obj_سیٹ_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_HOVERED, en);
}

خالی lv_obj_سیٹ_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_PRESSED, en);
}

خالی lv_obj_سیٹ_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_SCROLLED, en);
}

خالی lv_obj_سیٹ_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_DISABLED, en);
}

خالی lv_obj_سیٹ_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_USER_1, en);
}

خالی lv_obj_سیٹ_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_USER_2, en);
}

خالی lv_obj_سیٹ_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_USER_3, en);
}

خالی lv_obj_سیٹ_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    سیٹ_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->hidden;
}

bool lv_obj_is_clickable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->clickable;
}

bool lv_obj_is_click_focusable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->click_focusable;
}

bool lv_obj_is_checkable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->checkable;
}

bool lv_obj_is_scrollable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scrollable;
}

bool lv_obj_is_scroll_elastic(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->snappable;
}

bool lv_obj_is_press_تالا(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->press_تالا;
}

bool lv_obj_is_event_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->ignore_layout;
}

bool lv_obj_is_floating(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->floating;
}

bool lv_obj_is_send_draw_task_events(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->send_draw_task_events;
}

bool lv_obj_is_حد_سے_تجاوز_visible(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->حد_سے_تجاوز_visible;
}

bool lv_obj_is_event_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->event_trickle;
}

bool lv_obj_is_state_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->state_trickle;
}

bool lv_obj_is_radio_button(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس obj->radio_button;
}

bool lv_obj_is_flex_اندر_new_track(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    اگر(obj->spec_attr == تہی) واپس false;

    واپس obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس obj->state;
}

bool lv_obj_has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, state);
}

bool lv_obj_is_alt(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_کلید(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_FOCUS_کلید);
}

bool lv_obj_is_edited(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    واپس has_state(obj, LV_STATE_USER_4);
}

lv_گروہ_t * lv_obj_get_گروہ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    اگر(obj->spec_attr) واپس obj->spec_attr->گروہ_p;
    ورنہ واپس تہی;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_نتیجہ_t lv_obj_جوڑو_بچہ(lv_obj_t * والد, lv_obj_t * بچہ)
{
    LV_CHECK_OBJ(والد, MY_CLASS, واپس LV_نتیجہ_INVALID);
    LV_CHECK_ARG(بچہ != تہی, واپس LV_نتیجہ_INVALID);

    uint16_t new_بچہ_cnt = والد->spec_attr->بچہ_cnt + 1;

    lv_obj_t ** children = lv_realloc(والد->spec_attr->children,
                                      new_بچہ_cnt * (حجم(lv_obj_t *)));
    اگر(!children) {
        واپس LV_نتیجہ_INVALID;
    }
    children[new_بچہ_cnt - 1] = بچہ;

    والد->spec_attr->بچہ_cnt = new_بچہ_cnt;
    والد->spec_attr->children = children;
    واپس LV_نتیجہ_OK;
}

خالی lv_obj_remove_بچہ(lv_obj_t * والد, lv_obj_t * بچہ)
{
    LV_CHECK_OBJ(والد, MY_CLASS, واپس);
    LV_CHECK_OBJ(بچہ, MY_CLASS, واپس);

    برائے(int32_t i = lv_obj_get_اشاریہ(بچہ); i < (int32_t)والد->spec_attr->بچہ_cnt - 1; i++) {
        والد->spec_attr->children[i] = والد->spec_attr->children[i + 1];
    }

    /* No more children*/
    اگر(والد->spec_attr->بچہ_cnt == 1) {
        lv_free(والد->spec_attr->children);
        والد->spec_attr->children = تہی;
        والد->spec_attr->بچہ_cnt = 0;
        واپس;
    }
    والد->spec_attr->بچہ_cnt--;
    lv_obj_t ** new_children = lv_realloc(والد->spec_attr->children, والد->spec_attr->بچہ_cnt * (حجم(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    اگر(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        واپس;
    }
    والد->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    اگر(obj->spec_attr) {
        واپس obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(حجم(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    اگر(!spec_attr) {
        واپس تہی;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_منوال = LV_SCROLLBAR_منوال_AUTO;
    obj->spec_attr = spec_attr;
    واپس spec_attr;
}


bool lv_obj_check_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != تہی, واپس false);
    LV_CHECK_ARG(class_p != تہی, واپس false);
    واپس obj->class_p == class_p;
}

bool lv_obj_has_class(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != تہی, واپس false);
    LV_CHECK_ARG(class_p != تہی, واپس false);

    ثابت lv_obj_class_t * obj_class = obj->class_p;
    جب(obj_class) {
        اگر(obj_class == class_p) واپس true;
        obj_class = obj_class->بنیاد_class;
    }

    واپس false;
}

ثابت lv_obj_class_t * lv_obj_get_class(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    واپس obj->class_p;
}

bool lv_obj_is_اندر_widget_درخت(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    ثابت lv_obj_t * current = obj;

    جب(current->والد != تہی) {
#اگر LV_USE_CHECK_OBJ_والد_LINK && LV_USE_ASSERT
        LV_ASSERT(والد_شامل_ہے_بچہ(current->والد, current));
#اگر_ختم
        current = current->والد;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_اگلا(تہی);
    جب(disp) {
        uint32_t i;
        برائے(i = 0; i < disp->screen_cnt; i++) {
            اگر(disp->screens[i] == current) واپس true;
        }
        disp = lv_display_get_اگلا(disp);
    }

    واپس false;
}

خالی lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != تہی, واپس);
    LV_CHECK_ARG(*obj_ptr != تہی, واپس);

    lv_obj_جوڑو_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#اگر LV_USE_OBJ_ID
خالی * lv_obj_get_id(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    واپس obj->id;
}

#اگر_ختم

خالی lv_obj_جوڑو_screen_بھرو_event(lv_obj_t * obj, lv_event_رمز_t trigger, lv_obj_t * screen,
                                  lv_screen_بھرو_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG_پیغام(screen != تہی, واپس, "can't load a non-existing screen");

    screen_بھرو_anim_dsc_t * dsc = lv_malloc(حجم(screen_بھرو_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_بھرو_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ہدف.screen = screen;

    lv_obj_جوڑو_event_cb(obj, screen_بھرو_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, dsc);
}

خالی lv_obj_جوڑو_screen_create_event(lv_obj_t * obj, lv_event_رمز_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_بھرو_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(screen_create_cb != تہی, واپس);

    screen_بھرو_anim_dsc_t * dsc = lv_malloc(حجم(screen_بھرو_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_بھرو_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ہدف.create_cb = screen_create_cb;

    lv_obj_جوڑو_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, dsc);
}

خالی lv_obj_جوڑو_play_timeline_event(lv_obj_t * obj, lv_event_رمز_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool الٹ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(at != تہی, واپس);

    timeline_play_dsc_t * dsc = lv_malloc(حجم(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->الٹ = الٹ;

    lv_obj_جوڑو_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_ڈیٹا_cb, LV_EVENT_DELETE, dsc);
}

خالی lv_obj_سیٹ_user_ڈیٹا(lv_obj_t * obj, خالی * user_ڈیٹا)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    obj->user_ڈیٹا = user_ڈیٹا;
}

خالی lv_obj_سیٹ_user_flag(lv_obj_t * obj, uint32_t ثنائی_ہندسہ, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(ثنائی_ہندسہ < 4, واپس);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    اگر(spec_attr == تہی) واپس;

    اگر(v) spec_attr->user_flags |= 1 << (ثنائی_ہندسہ + 4);
    ورنہ spec_attr->user_flags &= ~(1 << (ثنائی_ہندسہ + 4));
}

خالی * lv_obj_get_user_ڈیٹا(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    واپس obj->user_ڈیٹا;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t ثنائی_ہندسہ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);
    LV_CHECK_ARG(ثنائی_ہندسہ < 4, واپس false);

    اگر(obj->spec_attr == تہی) واپس false;
    ورنہ واپس (obj->spec_attr->user_flags & 1 << (ثنائی_ہندسہ + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_جوڑو_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, خالی * user_ڈیٹا)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);
    LV_CHECK_ARG(cb != تہی, واپس تہی);

    lv_delete_dsc_t * dsc = lv_malloc(حجم(*dsc));
    اگر(!dsc) {
        واپس تہی;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_ڈیٹا = user_ڈیٹا;

    lv_event_dsc_t * event_dsc = lv_obj_جوڑو_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    اگر(!event_dsc) {
        lv_free(dsc);
        واپس تہی;
    }
    واپس dsc;
}

خالی lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    اگر(!dsc) {
        واپس;
    }

    uint32_t گنتی = lv_obj_remove_event_cb_with_user_ڈیٹا(dsc->obj, call_delete_cb, dsc);
    اگر(گنتی == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        واپس;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن خالی lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != تہی);

    LV_غیر_مستعمل(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * والد = obj->والد;
    اگر(والد) {
        int32_t sl = lv_obj_get_scroll_بایاں(والد);
        int32_t st = lv_obj_get_scroll_چوٹی(والد);

        obj->coords.y1 = والد->coords.y1 + lv_obj_get_style_pad_چوٹی_اندرونی(والد, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = والد->coords.x1 + lv_obj_get_style_pad_بایاں_اندرونی(والد, LV_PART_MAIN) - sl;
        obj->coords.x2  = obj->coords.x1 - 1;
    }

    /*Set attributes*/
    obj->clickable = 1;
    obj->snappable = 1;
    obj->click_focusable = 1;
    obj->scrollable = 1;
    obj->scroll_elastic = 1;
    obj->scroll_momentum = 1;
    obj->scroll_with_arrow = 1;
    اگر(والد) {
        obj->press_تالا = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#اگر LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_تفویض
    lv_obj_تفویض_id(class_p, obj);
#اگر_ختم

    LV_TRACE_OBJ_CREATE("finished");
}

ساکن خالی lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != تہی);

    LV_غیر_مستعمل(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, تہی);

    /*Delete from the group*/
    lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
    اگر(گروہ) lv_گروہ_remove_obj(obj);

    اگر(obj->spec_attr) {
        اگر(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = تہی;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#اگر LV_USE_OBJ_NAME
        اگر(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((خالی *)obj->spec_attr->name);
        }
#اگر_ختم

#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
        اگر(obj->spec_attr->میٹرکس) {
            lv_free(obj->spec_attr->میٹرکس);
            obj->spec_attr->میٹرکس = تہی;
        }
#اگر_ختم

        lv_free(obj->spec_attr);
        obj->spec_attr = تہی;
    }

#اگر LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_تفویض
    lv_obj_free_id(obj);
#اگر_ختم
}

ساکن خالی lv_obj_draw(lv_event_t * e)
{
    lv_event_رمز_t رمز = lv_event_get_رمز(e);
    lv_obj_t * obj = lv_event_get_current_ہدف(e);
    اگر(رمز == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        اگر(info->res == LV_COVER_RES_MASKED) واپس;
        اگر(lv_obj_get_style_clip_کونا_اندرونی(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            واپس;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_اندرونی(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_اونچائی_اندرونی(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        اگر(lv_area_is_اندر(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            واپس;
        }

        اگر(lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            واپس;
        }

        اگر(lv_obj_get_style_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            واپس;
        }

        اگر(lv_obj_get_style_bg_grad_dir_اندرونی(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            اگر(lv_obj_get_style_bg_grad_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                واپس;
            }
        }
        ثابت lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_اندرونی(obj, LV_PART_MAIN);
        اگر(grad_dsc) {
            uint32_t i;
            برائے(i = 0; i < grad_dsc->stops_گنتی; i++) {
                اگر(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    واپس;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    ورنہ اگر(رمز == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.بنیاد.layer = layer;

        int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_اونچائی_اندرونی(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_MAIN);
        اگر(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.کونا_radius = draw_dsc.radius;
            blur_dsc.بنیاد.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        اگر(lv_obj_get_style_border_post_اندرونی(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    ورنہ اگر(رمز == LV_EVENT_DRAW_MAIN_انتہا) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_MAIN);
        اگر(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_اونچائی_اندرونی(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.کونا_radius = lv_obj_get_style_radius_اندرونی(obj, LV_PART_MAIN);
            blur_dsc.بنیاد.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        اگر(lv_obj_get_style_border_چوڑائی_اندرونی(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_اندرونی(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.بنیاد.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_اونچائی_اندرونی(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_rect(layer, &draw_dsc, &coords);
        }
    }
}

ساکن خالی draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    اگر(lv_area_get_سائز(&hor_area) <= 0 && lv_area_get_سائز(&ver_area) <= 0) واپس;

    lv_draw_rect_dsc_t rect_dsc;
    lv_نتیجہ_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    اگر(sb_res != LV_نتیجہ_OK) واپس;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.کونا_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_اندرونی(obj, LV_PART_SCROLLBAR);

    اگر(lv_area_get_سائز(&hor_area) > 0) {
        اگر(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.بنیاد.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.بنیاد.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        اگر(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    اگر(lv_area_get_سائز(&ver_area) > 0) {
        blur_dsc.بنیاد.id1 = 1;
        اگر(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.بنیاد.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        اگر(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
ساکن lv_نتیجہ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    اگر(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_اندرونی(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    اگر(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_چوڑائی = lv_obj_get_style_border_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
        اگر(dsc->border_چوڑائی > 0) {
            dsc->border_color = lv_obj_get_style_border_color_اندرونی(obj, LV_PART_SCROLLBAR);
        }
        ورنہ {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    اگر(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
        اگر(dsc->shadow_چوڑائی > 0) {
            dsc->shadow_پھیلاؤ = lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_اندرونی(obj, LV_PART_SCROLLBAR);
        }
        ورنہ {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    اگر(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    اگر(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_اندرونی(obj, LV_PART_SCROLLBAR);
        واپس LV_نتیجہ_OK;
    }
    ورنہ {
        واپس LV_نتیجہ_INVALID;
    }
}

ساکن خالی lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_غیر_مستعمل(class_p);

    lv_event_رمز_t رمز = lv_event_get_رمز(e);
    lv_obj_t * obj = lv_event_get_current_ہدف(e);
    اگر(رمز == LV_EVENT_PRESSED) {
        lv_obj_جوڑو_state(obj, LV_STATE_PRESSED);
    }
    ورنہ اگر(رمز == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        اگر((indev == تہی || lv_indev_get_scroll_obj(indev) == تہی) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            اگر(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_جوڑو_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            ورنہ اگر(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            اگر(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_قدر_بدلا, تہی);
                اگر(res != LV_نتیجہ_OK) واپس;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, تہی);
                اگر(res != LV_نتیجہ_OK) واپس;
            }
        }
    }
    ورنہ اگر(رمز == LV_EVENT_قدر_بدلا) {
        اگر(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * والد = lv_obj_get_والد(obj);
            اگر(والد) {
                uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(والد);
                uint32_t i;
                برائے(i = 0; i < بچہ_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_بچہ(والد, i);
                    اگر(obj == sibling) جاری;

                    اگر(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_نتیجہ_t res = lv_obj_send_event(sibling, LV_EVENT_قدر_بدلا, تہی);
                        اگر(res != LV_نتیجہ_OK) واپس;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, تہی);
                        اگر(res != LV_نتیجہ_OK) واپس;
                    }
                }
            }
        }
    }
    ورنہ اگر(رمز == LV_EVENT_PRESS_گمشدہ) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    ورنہ اگر(رمز == LV_EVENT_STYLE_بدلا) {
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        برائے(uint32_t i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(بچہ);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_کلید) {
        اگر(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_کلید(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            اگر(c == LV_کلید_دایاں || c == LV_کلید_اوپر) {
                lv_obj_جوڑو_state(obj, LV_STATE_CHECKED);
            }
            ورنہ اگر(c == LV_کلید_بایاں || c == LV_کلید_نیچے_کو) {
                /*Radio buttons can't be checked off directly*/
                اگر(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            اگر(c != LV_کلید_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_قدر_بدلا, تہی);
                اگر(res != LV_نتیجہ_OK) واپس;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, تہی);
                اگر(res != LV_نتیجہ_OK) واپس;
            }
        }
        ورنہ اگر(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_بایاں(obj);
            int32_t sr = lv_obj_get_scroll_دایاں(obj);
            uint32_t c = lv_event_get_کلید(e);
            اگر(c == LV_کلید_نیچے_کو) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_مقصد_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_اونچائی(obj) / 4, anim_enable);
            }
            ورنہ اگر(c == LV_کلید_اوپر) {
                lv_obj_scroll_مقصد_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_اونچائی(obj) / 4, anim_enable);
            }
            ورنہ اگر(c == LV_کلید_دایاں) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                اگر(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_مقصد_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_اونچائی(obj) / 4, anim_enable);
                ورنہ
                    lv_obj_scroll_مقصد_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_چوڑائی(obj) / 4, anim_enable);
            }
            ورنہ اگر(c == LV_کلید_بایاں) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                اگر(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_مقصد_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_اونچائی(obj) / 4, anim_enable);
                ورنہ
                    lv_obj_scroll_مقصد_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_چوڑائی(obj) / 4, anim_enable);
            }
        }
    }
    ورنہ اگر(رمز == LV_EVENT_FOCUSED) {
        اگر(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_مقصد_view_recursive(obj, LV_ANIM_ON);
        }

        lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
        ثابت bool editing = گروہ && lv_گروہ_get_editing(گروہ);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_فعال();
        اگر(indev == تہی) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != تہی ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        اگر(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_کلید;
        اگر(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_جوڑو_state(obj, state);
        }
        ورنہ {
            lv_obj_جوڑو_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_جوڑو_state(obj, LV_STATE_SCROLLED);
    }
    ورنہ اگر(رمز == LV_EVENT_SCROLL_انتہا) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        اگر(lv_obj_get_scrollbar_منوال(obj) == LV_SCROLLBAR_منوال_فعال) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_کلید);
    }
    ورنہ اگر(رمز == LV_EVENT_سائز_بدلا) {
        int32_t align = lv_obj_get_style_align_اندرونی(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_اندرونی(obj, LV_PART_MAIN);
        اگر(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        برائے(i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(بچہ);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_بچہ_بدلا) {
        int32_t align = lv_obj_get_style_align_اندرونی(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_اندرونی(obj, LV_PART_MAIN);
        اگر(layout || align || lv_obj_is_style_any_چوڑائی_content(obj) || lv_obj_is_style_any_اونچائی_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ورنہ اگر(رمز == LV_EVENT_بچہ_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    ورنہ اگر(رمز == LV_EVENT_REFR_EXT_DRAW_سائز) {
        int32_t d = lv_obj_calculate_ext_draw_سائز(obj, LV_PART_MAIN);
        lv_event_سیٹ_ext_draw_سائز(e, d);
    }
    ورنہ اگر(رمز == LV_EVENT_DRAW_MAIN || رمز == LV_EVENT_DRAW_POST || رمز == LV_EVENT_DRAW_MAIN_انتہا ||
            رمز == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    ورنہ اگر(رمز == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    ورنہ اگر(رمز == LV_EVENT_HOVER_OVER) {
        lv_obj_جوڑو_state(obj, LV_STATE_HOVERED);
    }
    ورنہ اگر(رمز == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
ساکن خالی update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != تہی);

    اگر(obj->state == new_state) واپس;


    lv_state_t پچھلا_state = obj->state;

    lv_part_t بدلا_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, پچھلا_state, new_state, &بدلا_part);
    /*If there is no difference in styles there is nothing else to do*/
    اگر(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_بدلا, &پچھلا_state);
        واپس;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_حالت(obj);

    /*Skip transitions if the widget is not rendered yet. */
    اگر(!obj->rendered) {
        lv_obj_invalidate(obj);
        اگر(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_سائز(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_بدلا, &پچھلا_state);
        واپس;
    }

    obj_transition_حالتیں(obj, پچھلا_state, new_state);

    اگر(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && بدلا_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    ورنہ اگر(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ورنہ اگر(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ورنہ اگر(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_سائز(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_بدلا, &پچھلا_state);
}

ساکن خالی obj_transition_حالتیں(lv_obj_t * obj, lv_state_t پچھلا_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(حجم(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    اگر(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        واپس;
    }
    uint32_t tsi = 0;
    uint32_t i;
    برائے(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        اگر(state_act & (~new_state)) جاری; /*Skip unrelated styles*/
        اگر(obj_style->is_trans) جاری;

        lv_style_قدر_t v;
        اگر(lv_style_get_prop_اندرونی(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_ملا) جاری;
        ثابت lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        برائے(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            برائے(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                اگر(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) توڑو;
            }

            /*If not found  add it*/
            اگر(t == tsi) {
                ts[tsi].وقت = tr->وقت;
                ts[tsi].delay = tr->delay;
                ts[tsi].راستہ_cb = tr->راستہ_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_ڈیٹا = tr->user_ڈیٹا;
                ts[tsi].selector = obj_style->selector;
                tsi++;
            }
        }
    }

    برائے(i = 0; i < tsi; i++) {
        lv_part_t part_act = lv_obj_style_get_selector_part(ts[i].selector);
        lv_obj_style_create_transition(obj, part_act, پچھلا_state, new_state, &ts[i]);
    }

    lv_free(ts);
}

/**
 * Apply the state to the children of the object
 * @param obj pointer to an object
 * @param state the state to apply
 */
ساکن خالی lv_obj_children_جوڑو_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(obj);

    برائے(uint32_t i = 0; i < بچہ_گنتی; i++) {
        lv_obj_t * بچہ = lv_obj_get_بچہ(obj, i);
        اگر(بچہ) {
            lv_obj_جوڑو_state(بچہ, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
ساکن خالی lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(obj);

    برائے(uint32_t i = 0; i < بچہ_گنتی; i++) {
        lv_obj_t * بچہ = lv_obj_get_بچہ(obj, i);
        اگر(بچہ) {
            lv_obj_remove_state(بچہ, state);
        }
    }
}

ساکن خالی null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_ڈیٹا(e);
    *obj_ptr = تہی;
}

ساکن خالی screen_بھرو_on_trigger_event_cb(lv_event_t * e)
{
    screen_بھرو_anim_dsc_t * dsc = lv_event_get_user_ڈیٹا(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_بھرو_anim(dsc->ہدف.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

ساکن خالی screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_بھرو_anim_dsc_t * dsc = lv_event_get_user_ڈیٹا(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->ہدف.create_cb();
    lv_screen_بھرو_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_جوڑو_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, تہی);
}

ساکن خالی play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_ڈیٹا(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t پیش_رفت = lv_anim_timeline_get_پیش_رفت(dsc->at);
    اگر(dsc->الٹ) {
        اگر(پیش_رفت == 0) {
            lv_anim_timeline_سیٹ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_سیٹ_پیش_رفت(dsc->at, LV_ANIM_TIMELINE_پیش_رفت_MAX);
        }
    }
    ورنہ {
        اگر(پیش_رفت == LV_ANIM_TIMELINE_پیش_رفت_MAX) {
            lv_anim_timeline_سیٹ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_سیٹ_پیش_رفت(dsc->at, 0);
        }

    }
    lv_anim_timeline_سیٹ_الٹ(dsc->at, dsc->الٹ);
    lv_anim_timeline_آغاز(dsc->at);
}


ساکن خالی delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_ہدف_obj(e));
}

ساکن خالی call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != تہی);
    lv_obj_t * obj = lv_event_get_ہدف_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_ڈیٹا(e);
#اگر !LV_USE_ASSERT
    LV_غیر_مستعمل(obj);
#اگر_ختم
    LV_ASSERT(dsc != تہی);
    LV_ASSERT(dsc->cb != تہی);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_ڈیٹا);
    lv_obj_remove_delete_cb(dsc);
}

ساکن اندرونی خالی سیٹ_state(lv_obj_t * obj, lv_state_t state,  bool قدر)
{
    LV_ASSERT(obj != تہی);
    اگر(قدر) lv_obj_جوڑو_state(obj, state);
    ورنہ lv_obj_remove_state(obj, state);
}

ساکن اندرونی bool has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != تہی);

    واپس !!(obj->state & state);
}


#اگر LV_USE_CHECK_OBJ_والد_LINK && LV_USE_ASSERT
ساکن bool والد_شامل_ہے_بچہ(ثابت lv_obj_t * والد, ثابت lv_obj_t * بچہ)
{
    uint32_t بچہ_cnt = والد->spec_attr ? والد->spec_attr->بچہ_cnt : 0;
    uint32_t i;
    برائے(i = 0; i < بچہ_cnt; i++) {
        اگر(والد->spec_attr->children[i] == بچہ) واپس true;
    }
    واپس false;
}
#اگر_ختم


#اگر LV_USE_OBJ_PROPERTY
ساکن lv_نقطہ_t lv_obj_get_scroll_انتہا_helper(lv_obj_t * obj)
{
    lv_نقطہ_t نقطہ;
    lv_obj_get_scroll_انتہا(obj, &نقطہ);
    واپس نقطہ;
}

ساکن خالی obj_سیٹ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    اگر(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_سیٹ_hidden(obj, v);
    اگر(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_سیٹ_clickable(obj, v);
    اگر(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_سیٹ_click_focusable(obj, v);
    اگر(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_سیٹ_checkable(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_سیٹ_scrollable(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_سیٹ_scroll_elastic(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_سیٹ_scroll_momentum(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_سیٹ_scroll_one(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_سیٹ_scroll_chain_hor(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_سیٹ_scroll_chain_ver(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_سیٹ_scroll_on_focus(obj, v);
    اگر(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_سیٹ_scroll_with_arrow(obj, v);
    اگر(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_سیٹ_snappable(obj, v);
    اگر(f & LV_OBJ_FLAG_PRESS_تالا)            lv_obj_سیٹ_press_تالا(obj, v);
    اگر(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_سیٹ_event_bubble(obj, v);
    اگر(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_سیٹ_gesture_bubble(obj, v);
    اگر(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_سیٹ_adv_hittest(obj, v);
    اگر(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_سیٹ_ignore_layout(obj, v);
    اگر(f & LV_OBJ_FLAG_FLOATING)              lv_obj_سیٹ_floating(obj, v);
    اگر(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_سیٹ_send_draw_task_events(obj, v);
    اگر(f & LV_OBJ_FLAG_حد_سے_تجاوز_VISIBLE)      lv_obj_سیٹ_حد_سے_تجاوز_visible(obj, v);
    اگر(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_سیٹ_event_trickle(obj, v);
    اگر(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_سیٹ_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_بٹ = 0;
    اگر(f & LV_OBJ_FLAG_LAYOUT_1)              user_بٹ |= 1 << 0;
    اگر(f & LV_OBJ_FLAG_LAYOUT_2)              user_بٹ |= 1 << 1;
    اگر(f & LV_OBJ_FLAG_WIDGET_1)              user_بٹ |= 1 << 2;
    اگر(f & LV_OBJ_FLAG_WIDGET_2)              user_بٹ |= 1 << 3;
    اگر(f & LV_OBJ_FLAG_USER_1)                user_بٹ |= 1 << 4;
    اگر(f & LV_OBJ_FLAG_USER_2)                user_بٹ |= 1 << 5;
    اگر(f & LV_OBJ_FLAG_USER_3)                user_بٹ |= 1 << 6;
    اگر(f & LV_OBJ_FLAG_USER_4)                user_بٹ |= 1 << 7;
    اگر(user_بٹ == 0) واپس;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    اگر(v) {
        obj->spec_attr->user_flags |= user_بٹ;
    }
    ورنہ {
        obj->spec_attr->user_flags &= ~user_بٹ;
    }

    ثابت uint8_t layout_نقاب = 0x3;
    ثابت bool layout_flag_بدلا = (old_user_flags ^ obj->spec_attr->user_flags) & layout_نقاب;
    اگر(layout_flag_بدلا) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_والد(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ساکن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    اگر((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) واپس true;
    اگر((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) واپس true;
    اگر((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) واپس true;
    اگر((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) واپس true;
    اگر((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) واپس true;
    اگر((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) واپس true;
    اگر((f & LV_OBJ_FLAG_PRESS_تالا) && obj->press_تالا) واپس true;
    اگر((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) واپس true;
    اگر((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) واپس true;
    اگر((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) واپس true;
    اگر((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) واپس true;
    اگر((f & LV_OBJ_FLAG_FLOATING) && obj->floating) واپس true;
    اگر((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) واپس true;
    اگر((f & LV_OBJ_FLAG_حد_سے_تجاوز_VISIBLE) && obj->حد_سے_تجاوز_visible) واپس true;
    اگر((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) واپس true;
    اگر((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) واپس true;

    اگر(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == تہی) واپس false;

    اگر((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) واپس true;
    اگر((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) واپس true;
    اگر((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) واپس true;
    اگر((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) واپس true;
    اگر((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) واپس true;
    اگر((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) واپس true;
    اگر((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) واپس true;
    اگر((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) واپس true;

    واپس false;
}

ساکن lv_نتیجہ_t lv_obj_سیٹ_any(lv_obj_t * obj, lv_prop_id_t id, ثابت lv_property_t * prop)
{
    LV_ASSERT(obj != تہی);

    اگر(id >= LV_PROPERTY_OBJ_FLAG_آغاز && id <= LV_PROPERTY_OBJ_FLAG_انتہا) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_آغاز);
        obj_سیٹ_flag(obj, flag, prop->عدد != 0);
        واپس LV_نتیجہ_OK;
    }
    ورنہ اگر(id >= LV_PROPERTY_OBJ_STATE_آغاز && id <= LV_PROPERTY_OBJ_STATE_انتہا) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_آغاز);
        اگر(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        اگر(prop->عدد) lv_obj_جوڑو_state(obj, state);
        ورنہ lv_obj_remove_state(obj, state);
        واپس LV_نتیجہ_OK;
    }
    ورنہ {
        واپس LV_نتیجہ_INVALID;
    }
}

ساکن lv_نتیجہ_t lv_obj_get_any(ثابت lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != تہی);
    اگر(id >= LV_PROPERTY_OBJ_FLAG_آغاز && id <= LV_PROPERTY_OBJ_FLAG_انتہا) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_آغاز);
        prop->id = id;
        prop->عدد = obj_has_flag_any(obj, flag);
        واپس LV_نتیجہ_OK;
    }
    ورنہ اگر(id >= LV_PROPERTY_OBJ_STATE_آغاز && id <= LV_PROPERTY_OBJ_STATE_انتہا) {
        prop->id = id;
        اگر(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->عدد = obj->state;
        }
        ورنہ {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_آغاز);
            prop->عدد = obj->state & flag;
        }
        واپس LV_نتیجہ_OK;
    }
    ورنہ {
        واپس LV_نتیجہ_INVALID;
    }
}
#اگر_ختم /*LV_USE_OBJ_PROPERTY*/

