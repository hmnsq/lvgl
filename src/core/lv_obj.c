/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_style_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_area_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../indev/lv_indev_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_DEF_ਚੌੜਾਈ    (LV_DPX(100))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_DEF_ਉਚਾਈ   (LV_DPX(50))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_screen_ਲੱਦੋ_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    ਸੰਘ {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } ਨਿਸ਼ਾਨਾ;
} screen_ਲੱਦੋ_anim_dsc_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool ਉਲਟ;
} timeline_play_dsc_t;

ਬਣਤਰ _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    ਖਾਲੀ * user_ਅੰਕੜੇ;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ ਖਾਲੀ lv_obj_constructor(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ lv_obj_destructor(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ lv_obj_draw(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ lv_obj_event(ਅਚਲ lv_obj_class_t * class_p, lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
ਸਥਿਰ lv_ਨਤੀਜਾ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
ਸਥਿਰ ਖਾਲੀ obj_transition_ਹਾਲਤਾਂ(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
ਸਥਿਰ ਖਾਲੀ update_obj_state(lv_obj_t * obj, lv_state_t new_state);
ਸਥਿਰ ਖਾਲੀ lv_obj_children_ਜੋੜੋ_state(lv_obj_t * obj, lv_state_t state);
ਸਥਿਰ ਖਾਲੀ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
ਸਥਿਰ ਖਾਲੀ null_on_delete_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ screen_ਲੱਦੋ_on_trigger_event_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ screen_create_on_trigger_event_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ play_timeline_on_trigger_event_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ delete_on_screen_unloaded_event_cb(lv_event_t * e);
ਸਥਿਰ ਖਾਲੀ call_delete_cb(lv_event_t * e);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_state(lv_obj_t * obj, lv_state_t state,  bool ਮੁੱਲ);
ਸਥਿਰ ਸਤਰਅੰਦਰ bool has_state(ਅਚਲ lv_obj_t * obj, lv_state_t state);
#ਜੇ LV_USE_CHECK_OBJ_ਮਾਪਾ_LINK && LV_USE_ASSERT
    ਸਥਿਰ bool ਮਾਪਾ_ਸ਼ਾਮਲ_ਹੈ_ਬੱਚਾ(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ lv_obj_t * ਬੱਚਾ);
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_OBJ_PROPERTY
    ਸਥਿਰ lv_ਨਤੀਜਾ_t lv_obj_ਸੈੱਟ_any(lv_obj_t *, lv_prop_id_t, ਅਚਲ lv_property_t *);
    ਸਥਿਰ lv_ਨਤੀਜਾ_t lv_obj_get_any(ਅਚਲ lv_obj_t *, lv_prop_id_t, lv_property_t *);

    ਸਥਿਰ lv_ਬਿੰਦੂ_t lv_obj_get_scroll_ਸਿਰਾ_helper(lv_obj_t * obj);

    ਸਥਿਰ ਖਾਲੀ obj_ਸੈੱਟ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    ਸਥਿਰ bool obj_has_flag_any(ਅਚਲ lv_obj_t * obj, lv_obj_flag_t f);
#ਜੇ_ਅੰਤ

/**********************
 *  STATIC VARIABLES
 **********************/
#ਜੇ LV_USE_OBJ_PROPERTY
ਸਥਿਰ ਅਚਲ lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_ਮਾਪਾ,
        .setter = lv_obj_ਸੈੱਟ_ਮਾਪਾ,
        .getter = lv_obj_get_ਮਾਪਾ,
    },
    {
        .id = LV_PROPERTY_OBJ_X,
        .setter = lv_obj_ਸੈੱਟ_x,
        .getter = lv_obj_get_x,
    },
    {
        .id = LV_PROPERTY_OBJ_Y,
        .setter = lv_obj_ਸੈੱਟ_y,
        .getter = lv_obj_get_y,
    },
    {
        .id = LV_PROPERTY_OBJ_W,
        .setter = lv_obj_ਸੈੱਟ_ਚੌੜਾਈ,
        .getter = lv_obj_get_ਚੌੜਾਈ,
    },
    {
        .id = LV_PROPERTY_OBJ_H,
        .setter = lv_obj_ਸੈੱਟ_ਉਚਾਈ,
        .getter = lv_obj_get_ਉਚਾਈ,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_ਚੌੜਾਈ,
        .setter = lv_obj_ਸੈੱਟ_content_ਚੌੜਾਈ,
        .getter = lv_obj_get_content_ਚੌੜਾਈ,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_ਉਚਾਈ,
        .setter = lv_obj_ਸੈੱਟ_content_ਉਚਾਈ,
        .getter = lv_obj_get_content_ਉਚਾਈ,
    },
    {
        .id = LV_PROPERTY_OBJ_LAYOUT,
        .setter = lv_obj_ਸੈੱਟ_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_ALIGN,
        .setter = lv_obj_ਸੈੱਟ_align,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLBAR_ਬਹੁਲਕ,
        .setter = lv_obj_ਸੈੱਟ_scrollbar_ਬਹੁਲਕ,
        .getter = lv_obj_get_scrollbar_ਬਹੁਲਕ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_DIR,
        .setter = lv_obj_ਸੈੱਟ_scroll_dir,
        .getter = lv_obj_get_scroll_dir,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_X,
        .setter = lv_obj_ਸੈੱਟ_scroll_snap_x,
        .getter = lv_obj_get_scroll_snap_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_Y,
        .setter = lv_obj_ਸੈੱਟ_scroll_snap_y,
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
        .id = LV_PROPERTY_OBJ_SCROLL_ਸਿਖਰ,
        .getter = lv_obj_get_scroll_ਸਿਖਰ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ਖੱਬਾ,
        .getter = lv_obj_get_scroll_ਖੱਬਾ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ਸੱਜਾ,
        .getter = lv_obj_get_scroll_ਸੱਜਾ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ਸਿਰਾ,
        .getter = lv_obj_get_scroll_ਸਿਰਾ_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_ਮਾਪ,
        .getter = lv_obj_get_ext_draw_ਮਾਪ,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_ਗਿਣਤੀ,
        .getter = lv_obj_get_event_ਗਿਣਤੀ,
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
        .id = LV_PROPERTY_OBJ_ਬੱਚਾ_ਗਿਣਤੀ,
        .getter = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ,
    },
    {
        .id = LV_PROPERTY_OBJ_ਸੂਚਕਾਂਕ,
        .getter = lv_obj_get_ਸੂਚਕਾਂਕ,
    },
#ਜੇ LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_ਸੈੱਟ_name,
    },
#ਜੇ_ਅੰਤ
    {
        .id = LV_PROPERTY_OBJ_HIDDEN,
        .setter = lv_obj_ਸੈੱਟ_hidden,
        .getter = lv_obj_is_hidden,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICKABLE,
        .setter = lv_obj_ਸੈੱਟ_clickable,
        .getter = lv_obj_is_clickable,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICK_FOCUSABLE,
        .setter = lv_obj_ਸੈੱਟ_click_focusable,
        .getter = lv_obj_is_click_focusable,
    },
    {
        .id = LV_PROPERTY_OBJ_CHECKABLE,
        .setter = lv_obj_ਸੈੱਟ_checkable,
        .getter = lv_obj_is_checkable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLABLE,
        .setter = lv_obj_ਸੈੱਟ_scrollable,
        .getter = lv_obj_is_scrollable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ELASTIC,
        .setter = lv_obj_ਸੈੱਟ_scroll_elastic,
        .getter = lv_obj_is_scroll_elastic,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_MOMENTUM,
        .setter = lv_obj_ਸੈੱਟ_scroll_momentum,
        .getter = lv_obj_is_scroll_momentum,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ONE,
        .setter = lv_obj_ਸੈੱਟ_scroll_one,
        .getter = lv_obj_is_scroll_one,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_HOR,
        .setter = lv_obj_ਸੈੱਟ_scroll_chain_hor,
        .getter = lv_obj_is_scroll_chain_hor,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_VER,
        .setter = lv_obj_ਸੈੱਟ_scroll_chain_ver,
        .getter = lv_obj_is_scroll_chain_ver,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ON_FOCUS,
        .setter = lv_obj_ਸੈੱਟ_scroll_on_focus,
        .getter = lv_obj_is_scroll_on_focus,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_WITH_ARROW,
        .setter = lv_obj_ਸੈੱਟ_scroll_with_arrow,
        .getter = lv_obj_is_scroll_with_arrow,
    },
    {
        .id = LV_PROPERTY_OBJ_SNAPPABLE,
        .setter = lv_obj_ਸੈੱਟ_snappable,
        .getter = lv_obj_is_snappable,
    },
    {
        .id = LV_PROPERTY_OBJ_PRESS_ਤਾਲਾ,
        .setter = lv_obj_ਸੈੱਟ_press_ਤਾਲਾ,
        .getter = lv_obj_is_press_ਤਾਲਾ,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_BUBBLE,
        .setter = lv_obj_ਸੈੱਟ_event_bubble,
        .getter = lv_obj_is_event_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_GESTURE_BUBBLE,
        .setter = lv_obj_ਸੈੱਟ_gesture_bubble,
        .getter = lv_obj_is_gesture_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_ADV_HITTEST,
        .setter = lv_obj_ਸੈੱਟ_adv_hittest,
        .getter = lv_obj_is_adv_hittest,
    },
    {
        .id = LV_PROPERTY_OBJ_IGNORE_LAYOUT,
        .setter = lv_obj_ਸੈੱਟ_ignore_layout,
        .getter = lv_obj_is_ignore_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_FLOATING,
        .setter = lv_obj_ਸੈੱਟ_floating,
        .getter = lv_obj_is_floating,
    },
    {
        .id = LV_PROPERTY_OBJ_SEND_DRAW_TASK_EVENTS,
        .setter = lv_obj_ਸੈੱਟ_send_draw_task_events,
        .getter = lv_obj_is_send_draw_task_events,
    },
    {
        .id = LV_PROPERTY_OBJ_ਉਛਾਲ_VISIBLE,
        .setter = lv_obj_ਸੈੱਟ_ਉਛਾਲ_visible,
        .getter = lv_obj_is_ਉਛਾਲ_visible,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_TRICKLE,
        .setter = lv_obj_ਸੈੱਟ_event_trickle,
        .getter = lv_obj_is_event_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_STATE_TRICKLE,
        .setter = lv_obj_ਸੈੱਟ_state_trickle,
        .getter = lv_obj_is_state_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_FLEX_ਵਿੱਚ_NEW_TRACK,
        .setter = lv_obj_ਸੈੱਟ_flex_ਵਿੱਚ_new_track,
        .getter = lv_obj_is_flex_ਵਿੱਚ_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_ਸੈੱਟ_any,
        .getter = lv_obj_get_any,
    },
};
#ਜੇ_ਅੰਤ

ਅਚਲ lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .ਚੌੜਾਈ_def = LV_DPI_DEF,
    .ਉਚਾਈ_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .ਸਮੂਹ_def = LV_OBJ_CLASS_ਸਮੂਹ_DEF_FALSE,
    .instance_ਮਾਪ = (ਆਕਾਰ(lv_obj_t)),
    .ਅਧਾਰ_class = ਨਿਰਾ,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * ਮਾਪਾ)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, ਮਾਪਾ);

    ਜੇ(obj == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    ਵਾਪਸ obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

ਖਾਲੀ lv_obj_ਸੈੱਟ_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਜੇ(obj->hidden == en) ਵਾਪਸ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    ਜੇ(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    ਜੇ(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    ਜੇ(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_ਸਮੂਹ_t * ਸਮੂਹ = lv_obj_get_ਸਮੂਹ(obj);
        ਜੇ(ਸਮੂਹ != ਨਿਰਾ) {
            lv_ਸਮੂਹ_focus_ਅਗਲਾ(ਸਮੂਹ);
            lv_obj_t * ਅਗਲਾ_obj = lv_ਸਮੂਹ_get_focused(ਸਮੂਹ);
            ਜੇ(ਅਗਲਾ_obj != ਨਿਰਾ) {
                lv_obj_invalidate(ਅਗਲਾ_obj);
            }
        }
    }

    ਜੇ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ਮਾਪਾ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->clickable = en;

}

ਖਾਲੀ lv_obj_ਸੈੱਟ_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->click_focusable = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->checkable = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(obj->scrollable == en) ਵਾਪਸ;

    /*Invalidate the scrollbars while scrollable*/
    ਜੇ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    ਜੇ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_elastic = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_momentum = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_one = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_chain_hor = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_chain_ver = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_on_focus = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->scroll_with_arrow = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->snappable = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_press_ਤਾਲਾ(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->press_ਤਾਲਾ = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->event_bubble = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->gesture_bubble = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->adv_hittest = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(obj->ignore_layout == en) ਵਾਪਸ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    ਜੇ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ਮਾਪਾ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(obj->floating == en) ਵਾਪਸ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    ਜੇ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ਮਾਪਾ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->send_draw_task_events = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ਉਛਾਲ_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->ਉਛਾਲ_visible = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->event_trickle = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->state_trickle = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->radio_button = en;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_flex_ਵਿੱਚ_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    ਜੇ(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    ਨਹੀਂਤਾਂ {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        ਜੇ(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_ਮਾਪਾ(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_state_t new_state = obj->state | state;
    ਜੇ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ਜੇ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_ਜੋੜੋ_state(obj, state);
        }
    }
}

ਖਾਲੀ lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_state_t new_state = obj->state & (~state);
    ਜੇ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ਜੇ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, state, v);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_ALT, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_CHECKED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_FOCUSED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_focus_ਕੁੰਜੀ(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_FOCUS_ਕੁੰਜੀ, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_EDITED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_HOVERED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_PRESSED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_SCROLLED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_DISABLED, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_USER_1, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_USER_2, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_USER_3, en);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    ਸੈੱਟ_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->hidden;
}

bool lv_obj_is_clickable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->clickable;
}

bool lv_obj_is_click_focusable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->click_focusable;
}

bool lv_obj_is_checkable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->checkable;
}

bool lv_obj_is_scrollable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scrollable;
}

bool lv_obj_is_scroll_elastic(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->snappable;
}

bool lv_obj_is_press_ਤਾਲਾ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->press_ਤਾਲਾ;
}

bool lv_obj_is_event_bubble(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->ignore_layout;
}

bool lv_obj_is_floating(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->floating;
}

bool lv_obj_is_send_draw_task_events(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->send_draw_task_events;
}

bool lv_obj_is_ਉਛਾਲ_visible(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->ਉਛਾਲ_visible;
}

bool lv_obj_is_event_trickle(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->event_trickle;
}

bool lv_obj_is_state_trickle(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->state_trickle;
}

bool lv_obj_is_radio_button(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ obj->radio_button;
}

bool lv_obj_is_flex_ਵਿੱਚ_new_track(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ false;

    ਵਾਪਸ obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ obj->state;
}

bool lv_obj_has_state(ਅਚਲ lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, state);
}

bool lv_obj_is_alt(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_ਕੁੰਜੀ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_FOCUS_ਕੁੰਜੀ);
}

bool lv_obj_is_edited(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    ਵਾਪਸ has_state(obj, LV_STATE_USER_4);
}

lv_ਸਮੂਹ_t * lv_obj_get_ਸਮੂਹ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(obj->spec_attr) ਵਾਪਸ obj->spec_attr->ਸਮੂਹ_p;
    ਨਹੀਂਤਾਂ ਵਾਪਸ ਨਿਰਾ;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_ਨਤੀਜਾ_t lv_obj_ਜੋੜੋ_ਬੱਚਾ(lv_obj_t * ਮਾਪਾ, lv_obj_t * ਬੱਚਾ)
{
    LV_CHECK_OBJ(ਮਾਪਾ, MY_CLASS, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);
    LV_CHECK_ARG(ਬੱਚਾ != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    uint16_t new_ਬੱਚਾ_cnt = ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt + 1;

    lv_obj_t ** children = lv_realloc(ਮਾਪਾ->spec_attr->children,
                                      new_ਬੱਚਾ_cnt * (ਆਕਾਰ(lv_obj_t *)));
    ਜੇ(!children) {
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }
    children[new_ਬੱਚਾ_cnt - 1] = ਬੱਚਾ;

    ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt = new_ਬੱਚਾ_cnt;
    ਮਾਪਾ->spec_attr->children = children;
    ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
}

ਖਾਲੀ lv_obj_remove_ਬੱਚਾ(lv_obj_t * ਮਾਪਾ, lv_obj_t * ਬੱਚਾ)
{
    LV_CHECK_OBJ(ਮਾਪਾ, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_OBJ(ਬੱਚਾ, MY_CLASS, ਵਾਪਸ);

    ਲਈ(int32_t i = lv_obj_get_ਸੂਚਕਾਂਕ(ਬੱਚਾ); i < (int32_t)ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt - 1; i++) {
        ਮਾਪਾ->spec_attr->children[i] = ਮਾਪਾ->spec_attr->children[i + 1];
    }

    /* No more children*/
    ਜੇ(ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt == 1) {
        lv_free(ਮਾਪਾ->spec_attr->children);
        ਮਾਪਾ->spec_attr->children = ਨਿਰਾ;
        ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt = 0;
        ਵਾਪਸ;
    }
    ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt--;
    lv_obj_t ** new_children = lv_realloc(ਮਾਪਾ->spec_attr->children, ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt * (ਆਕਾਰ(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    ਜੇ(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        ਵਾਪਸ;
    }
    ਮਾਪਾ->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(obj->spec_attr) {
        ਵਾਪਸ obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(ਆਕਾਰ(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    ਜੇ(!spec_attr) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_ਬਹੁਲਕ = LV_SCROLLBAR_ਬਹੁਲਕ_AUTO;
    obj->spec_attr = spec_attr;
    ਵਾਪਸ spec_attr;
}


bool lv_obj_check_type(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ false);
    ਵਾਪਸ obj->class_p == class_p;
}

bool lv_obj_has_class(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ false);

    ਅਚਲ lv_obj_class_t * obj_class = obj->class_p;
    ਜਦੋਂ(obj_class) {
        ਜੇ(obj_class == class_p) ਵਾਪਸ true;
        obj_class = obj_class->ਅਧਾਰ_class;
    }

    ਵਾਪਸ false;
}

ਅਚਲ lv_obj_class_t * lv_obj_get_class(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    ਵਾਪਸ obj->class_p;
}

bool lv_obj_is_ਵਿੱਚ_widget_ਰੁੱਖ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    ਅਚਲ lv_obj_t * current = obj;

    ਜਦੋਂ(current->ਮਾਪਾ != ਨਿਰਾ) {
#ਜੇ LV_USE_CHECK_OBJ_ਮਾਪਾ_LINK && LV_USE_ASSERT
        LV_ASSERT(ਮਾਪਾ_ਸ਼ਾਮਲ_ਹੈ_ਬੱਚਾ(current->ਮਾਪਾ, current));
#ਜੇ_ਅੰਤ
        current = current->ਮਾਪਾ;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_ਅਗਲਾ(ਨਿਰਾ);
    ਜਦੋਂ(disp) {
        uint32_t i;
        ਲਈ(i = 0; i < disp->screen_cnt; i++) {
            ਜੇ(disp->screens[i] == current) ਵਾਪਸ true;
        }
        disp = lv_display_get_ਅਗਲਾ(disp);
    }

    ਵਾਪਸ false;
}

ਖਾਲੀ lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(*obj_ptr != ਨਿਰਾ, ਵਾਪਸ);

    lv_obj_ਜੋੜੋ_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#ਜੇ LV_USE_OBJ_ID
ਖਾਲੀ * lv_obj_get_id(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    ਵਾਪਸ obj->id;
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ lv_obj_ਜੋੜੋ_screen_ਲੱਦੋ_event(lv_obj_t * obj, lv_event_ਕੋਡ_t trigger, lv_obj_t * screen,
                                  lv_screen_ਲੱਦੋ_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG_ਸੁਨੇਹਾ(screen != ਨਿਰਾ, ਵਾਪਸ, "can't load a non-existing screen");

    screen_ਲੱਦੋ_anim_dsc_t * dsc = lv_malloc(ਆਕਾਰ(screen_ਲੱਦੋ_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ਆਕਾਰ(screen_ਲੱਦੋ_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ਨਿਸ਼ਾਨਾ.screen = screen;

    lv_obj_ਜੋੜੋ_event_cb(obj, screen_ਲੱਦੋ_on_trigger_event_cb, trigger, dsc);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, dsc);
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_screen_create_event(lv_obj_t * obj, lv_event_ਕੋਡ_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_ਲੱਦੋ_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(screen_create_cb != ਨਿਰਾ, ਵਾਪਸ);

    screen_ਲੱਦੋ_anim_dsc_t * dsc = lv_malloc(ਆਕਾਰ(screen_ਲੱਦੋ_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ਆਕਾਰ(screen_ਲੱਦੋ_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ਨਿਸ਼ਾਨਾ.create_cb = screen_create_cb;

    lv_obj_ਜੋੜੋ_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, dsc);
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_play_timeline_event(lv_obj_t * obj, lv_event_ਕੋਡ_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool ਉਲਟ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(at != ਨਿਰਾ, ਵਾਪਸ);

    timeline_play_dsc_t * dsc = lv_malloc(ਆਕਾਰ(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ਆਕਾਰ(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->ਉਲਟ = ਉਲਟ;

    lv_obj_ਜੋੜੋ_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_ਜੋੜੋ_event_cb(obj, lv_event_free_user_ਅੰਕੜੇ_cb, LV_EVENT_DELETE, dsc);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_user_ਅੰਕੜੇ(lv_obj_t * obj, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    obj->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_user_flag(lv_obj_t * obj, uint32_t ਦੋਆਧਾਰੀ_ਅੰਕ, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(ਦੋਆਧਾਰੀ_ਅੰਕ < 4, ਵਾਪਸ);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    ਜੇ(spec_attr == ਨਿਰਾ) ਵਾਪਸ;

    ਜੇ(v) spec_attr->user_flags |= 1 << (ਦੋਆਧਾਰੀ_ਅੰਕ + 4);
    ਨਹੀਂਤਾਂ spec_attr->user_flags &= ~(1 << (ਦੋਆਧਾਰੀ_ਅੰਕ + 4));
}

ਖਾਲੀ * lv_obj_get_user_ਅੰਕੜੇ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ obj->user_ਅੰਕੜੇ;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t ਦੋਆਧਾਰੀ_ਅੰਕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);
    LV_CHECK_ARG(ਦੋਆਧਾਰੀ_ਅੰਕ < 4, ਵਾਪਸ false);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ false;
    ਨਹੀਂਤਾਂ ਵਾਪਸ (obj->spec_attr->user_flags & 1 << (ਦੋਆਧਾਰੀ_ਅੰਕ + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_ਜੋੜੋ_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(cb != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    lv_delete_dsc_t * dsc = lv_malloc(ਆਕਾਰ(*dsc));
    ਜੇ(!dsc) {
        ਵਾਪਸ ਨਿਰਾ;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;

    lv_event_dsc_t * event_dsc = lv_obj_ਜੋੜੋ_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    ਜੇ(!event_dsc) {
        lv_free(dsc);
        ਵਾਪਸ ਨਿਰਾ;
    }
    ਵਾਪਸ dsc;
}

ਖਾਲੀ lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    ਜੇ(!dsc) {
        ਵਾਪਸ;
    }

    uint32_t ਗਿਣਤੀ_2 = lv_obj_remove_event_cb_with_user_ਅੰਕੜੇ(dsc->obj, call_delete_cb, dsc);
    ਜੇ(ਗਿਣਤੀ_2 == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        ਵਾਪਸ;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਖਾਲੀ lv_obj_constructor(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    LV_ਅਣਵਰਤਿਆ(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * ਮਾਪਾ = obj->ਮਾਪਾ;
    ਜੇ(ਮਾਪਾ) {
        int32_t sl = lv_obj_get_scroll_ਖੱਬਾ(ਮਾਪਾ);
        int32_t st = lv_obj_get_scroll_ਸਿਖਰ(ਮਾਪਾ);

        obj->coords.y1 = ਮਾਪਾ->coords.y1 + lv_obj_get_style_pad_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = ਮਾਪਾ->coords.x1 + lv_obj_get_style_pad_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) - sl;
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
    ਜੇ(ਮਾਪਾ) {
        obj->press_ਤਾਲਾ = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#ਜੇ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ਸੌਂਪੋ
    lv_obj_ਸੌਂਪੋ_id(class_p, obj);
#ਜੇ_ਅੰਤ

    LV_TRACE_OBJ_CREATE("finished");
}

ਸਥਿਰ ਖਾਲੀ lv_obj_destructor(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    LV_ਅਣਵਰਤਿਆ(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, ਨਿਰਾ);

    /*Delete from the group*/
    lv_ਸਮੂਹ_t * ਸਮੂਹ = lv_obj_get_ਸਮੂਹ(obj);
    ਜੇ(ਸਮੂਹ) lv_ਸਮੂਹ_remove_obj(obj);

    ਜੇ(obj->spec_attr) {
        ਜੇ(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = ਨਿਰਾ;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#ਜੇ LV_USE_OBJ_NAME
        ਜੇ(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((ਖਾਲੀ *)obj->spec_attr->name);
        }
#ਜੇ_ਅੰਤ

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
        ਜੇ(obj->spec_attr->ਮੈਟ੍ਰਿਕਸ) {
            lv_free(obj->spec_attr->ਮੈਟ੍ਰਿਕਸ);
            obj->spec_attr->ਮੈਟ੍ਰਿਕਸ = ਨਿਰਾ;
        }
#ਜੇ_ਅੰਤ

        lv_free(obj->spec_attr);
        obj->spec_attr = ਨਿਰਾ;
    }

#ਜੇ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ਸੌਂਪੋ
    lv_obj_free_id(obj);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ lv_obj_draw(lv_event_t * e)
{
    lv_event_ਕੋਡ_t ਕੋਡ = lv_event_get_ਕੋਡ(e);
    lv_obj_t * obj = lv_event_get_current_ਨਿਸ਼ਾਨਾ(e);
    ਜੇ(ਕੋਡ == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        ਜੇ(info->res == LV_COVER_RES_MASKED) ਵਾਪਸ;
        ਜੇ(lv_obj_get_style_clip_ਕੋਨਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            ਵਾਪਸ;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        ਜੇ(lv_area_is_ਵਿੱਚ(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            ਵਾਪਸ;
        }

        ਜੇ(lv_obj_get_style_bg_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            ਵਾਪਸ;
        }

        ਜੇ(lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            ਵਾਪਸ;
        }

        ਜੇ(lv_obj_get_style_bg_grad_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            ਜੇ(lv_obj_get_style_bg_grad_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                ਵਾਪਸ;
            }
        }
        ਅਚਲ lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        ਜੇ(grad_dsc) {
            uint32_t i;
            ਲਈ(i = 0; i < grad_dsc->stops_ਗਿਣਤੀ; i++) {
                ਜੇ(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    ਵਾਪਸ;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.ਅਧਾਰ.layer = layer;

        int32_t w = lv_obj_get_style_transform_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        ਜੇ(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.ਕੋਨਾ_radius = draw_dsc.radius;
            blur_dsc.ਅਧਾਰ.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        ਜੇ(lv_obj_get_style_border_post_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_DRAW_MAIN_ਸਿਰਾ) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        ਜੇ(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.ਕੋਨਾ_radius = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            blur_dsc.ਅਧਾਰ.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        ਜੇ(lv_obj_get_style_border_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.ਅਧਾਰ.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_rect(layer, &draw_dsc, &coords);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ਜੇ(lv_area_get_ਮਾਪ(&hor_area) <= 0 && lv_area_get_ਮਾਪ(&ver_area) <= 0) ਵਾਪਸ;

    lv_draw_rect_dsc_t rect_dsc;
    lv_ਨਤੀਜਾ_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    ਜੇ(sb_res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.ਕੋਨਾ_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);

    ਜੇ(lv_area_get_ਮਾਪ(&hor_area) > 0) {
        ਜੇ(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.ਅਧਾਰ.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.ਅਧਾਰ.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        ਜੇ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    ਜੇ(lv_area_get_ਮਾਪ(&ver_area) > 0) {
        blur_dsc.ਅਧਾਰ.id1 = 1;
        ਜੇ(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.ਅਧਾਰ.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        ਜੇ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
ਸਥਿਰ lv_ਨਤੀਜਾ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    ਜੇ(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    ਜੇ(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
        ਜੇ(dsc->border_ਚੌੜਾਈ > 0) {
            dsc->border_color = lv_obj_get_style_border_color_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
        }
        ਨਹੀਂਤਾਂ {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    ਜੇ(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_ਚੌੜਾਈ = lv_obj_get_style_shadow_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
        ਜੇ(dsc->shadow_ਚੌੜਾਈ > 0) {
            dsc->shadow_ਫੈਲਾਅ = lv_obj_get_style_shadow_ਫੈਲਾਅ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
        }
        ਨਹੀਂਤਾਂ {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    ਜੇ(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    ਜੇ(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }
    ਨਹੀਂਤਾਂ {
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }
}

ਸਥਿਰ ਖਾਲੀ lv_obj_event(ਅਚਲ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_ਅਣਵਰਤਿਆ(class_p);

    lv_event_ਕੋਡ_t ਕੋਡ = lv_event_get_ਕੋਡ(e);
    lv_obj_t * obj = lv_event_get_current_ਨਿਸ਼ਾਨਾ(e);
    ਜੇ(ਕੋਡ == LV_EVENT_PRESSED) {
        lv_obj_ਜੋੜੋ_state(obj, LV_STATE_PRESSED);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        ਜੇ((indev == ਨਿਰਾ || lv_indev_get_scroll_obj(indev) == ਨਿਰਾ) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ਜੇ(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_ਜੋੜੋ_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            ਨਹੀਂਤਾਂ ਜੇ(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            ਜੇ(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ਨਤੀਜਾ_t res = lv_obj_send_event(obj, LV_EVENT_ਮੁੱਲ_ਬਦਲਿਆ, ਨਿਰਾ);
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ਨਿਰਾ);
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_ਮੁੱਲ_ਬਦਲਿਆ) {
        ਜੇ(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
            ਜੇ(ਮਾਪਾ) {
                uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
                uint32_t i;
                ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_ਬੱਚਾ(ਮਾਪਾ, i);
                    ਜੇ(obj == sibling) ਜਾਰੀ;

                    ਜੇ(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_ਨਤੀਜਾ_t res = lv_obj_send_event(sibling, LV_EVENT_ਮੁੱਲ_ਬਦਲਿਆ, ਨਿਰਾ);
                        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, ਨਿਰਾ);
                        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
                    }
                }
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_PRESS_ਗੁਆਚਾ) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_STYLE_ਬਦਲਿਆ) {
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
        ਲਈ(uint32_t i = 0; i < ਬੱਚਾ_cnt; i++) {
            lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(ਬੱਚਾ);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_ਕੁੰਜੀ) {
        ਜੇ(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_ਕੁੰਜੀ(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ਜੇ(c == LV_ਕੁੰਜੀ_ਸੱਜਾ || c == LV_ਕੁੰਜੀ_ਉੱਪਰ) {
                lv_obj_ਜੋੜੋ_state(obj, LV_STATE_CHECKED);
            }
            ਨਹੀਂਤਾਂ ਜੇ(c == LV_ਕੁੰਜੀ_ਖੱਬਾ || c == LV_ਕੁੰਜੀ_ਥੱਲੇ) {
                /*Radio buttons can't be checked off directly*/
                ਜੇ(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            ਜੇ(c != LV_ਕੁੰਜੀ_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ਨਤੀਜਾ_t res = lv_obj_send_event(obj, LV_EVENT_ਮੁੱਲ_ਬਦਲਿਆ, ਨਿਰਾ);
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ਨਿਰਾ);
                ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_ਖੱਬਾ(obj);
            int32_t sr = lv_obj_get_scroll_ਸੱਜਾ(obj);
            uint32_t c = lv_event_get_ਕੁੰਜੀ(e);
            ਜੇ(c == LV_ਕੁੰਜੀ_ਥੱਲੇ) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_ਮੰਜ਼ਿਲ_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ਉਚਾਈ(obj) / 4, anim_enable);
            }
            ਨਹੀਂਤਾਂ ਜੇ(c == LV_ਕੁੰਜੀ_ਉੱਪਰ) {
                lv_obj_scroll_ਮੰਜ਼ਿਲ_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ਉਚਾਈ(obj) / 4, anim_enable);
            }
            ਨਹੀਂਤਾਂ ਜੇ(c == LV_ਕੁੰਜੀ_ਸੱਜਾ) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ਜੇ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_ਮੰਜ਼ਿਲ_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ਉਚਾਈ(obj) / 4, anim_enable);
                ਨਹੀਂਤਾਂ
                    lv_obj_scroll_ਮੰਜ਼ਿਲ_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_ਚੌੜਾਈ(obj) / 4, anim_enable);
            }
            ਨਹੀਂਤਾਂ ਜੇ(c == LV_ਕੁੰਜੀ_ਖੱਬਾ) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ਜੇ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_ਮੰਜ਼ਿਲ_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ਉਚਾਈ(obj) / 4, anim_enable);
                ਨਹੀਂਤਾਂ
                    lv_obj_scroll_ਮੰਜ਼ਿਲ_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_ਚੌੜਾਈ(obj) / 4, anim_enable);
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_FOCUSED) {
        ਜੇ(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_ਮੰਜ਼ਿਲ_view_recursive(obj, LV_ANIM_ON);
        }

        lv_ਸਮੂਹ_t * ਸਮੂਹ = lv_obj_get_ਸਮੂਹ(obj);
        ਅਚਲ bool editing = ਸਮੂਹ && lv_ਸਮੂਹ_get_editing(ਸਮੂਹ);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_ਸਰਗਰਮ();
        ਜੇ(indev == ਨਿਰਾ) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != ਨਿਰਾ ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        ਜੇ(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_ਕੁੰਜੀ;
        ਜੇ(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_ਜੋੜੋ_state(obj, state);
        }
        ਨਹੀਂਤਾਂ {
            lv_obj_ਜੋੜੋ_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_ਜੋੜੋ_state(obj, LV_STATE_SCROLLED);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_SCROLL_ਸਿਰਾ) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        ਜੇ(lv_obj_get_scrollbar_ਬਹੁਲਕ(obj) == LV_SCROLLBAR_ਬਹੁਲਕ_ਸਰਗਰਮ) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_ਕੁੰਜੀ);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_ਮਾਪ_ਬਦਲਿਆ) {
        int32_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        ਜੇ(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
        ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
            lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(ਬੱਚਾ);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ) {
        int32_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
        ਜੇ(layout || align || lv_obj_is_style_any_ਚੌੜਾਈ_content(obj) || lv_obj_is_style_any_ਉਚਾਈ_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_ਬੱਚਾ_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_REFR_EXT_DRAW_ਮਾਪ) {
        int32_t d = lv_obj_calculate_ext_draw_ਮਾਪ(obj, LV_PART_MAIN);
        lv_event_ਸੈੱਟ_ext_draw_ਮਾਪ(e, d);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_DRAW_MAIN || ਕੋਡ == LV_EVENT_DRAW_POST || ਕੋਡ == LV_EVENT_DRAW_MAIN_ਸਿਰਾ ||
            ਕੋਡ == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_HOVER_OVER) {
        lv_obj_ਜੋੜੋ_state(obj, LV_STATE_HOVERED);
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਕੋਡ == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
ਸਥਿਰ ਖਾਲੀ update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    ਜੇ(obj->state == new_state) ਵਾਪਸ;


    lv_state_t prev_state = obj->state;

    lv_part_t ਬਦਲਿਆ_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &ਬਦਲਿਆ_part);
    /*If there is no difference in styles there is nothing else to do*/
    ਜੇ(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_ਬਦਲਿਆ, &prev_state);
        ਵਾਪਸ;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_ਸਥਿਤੀ(obj);

    /*Skip transitions if the widget is not rendered yet. */
    ਜੇ(!obj->rendered) {
        lv_obj_invalidate(obj);
        ਜੇ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_ਮਾਪ(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_ਬਦਲਿਆ, &prev_state);
        ਵਾਪਸ;
    }

    obj_transition_ਹਾਲਤਾਂ(obj, prev_state, new_state);

    ਜੇ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && ਬਦਲਿਆ_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    ਨਹੀਂਤਾਂ ਜੇ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ਨਹੀਂਤਾਂ ਜੇ(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ਨਹੀਂਤਾਂ ਜੇ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_ਮਾਪ(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_ਬਦਲਿਆ, &prev_state);
}

ਸਥਿਰ ਖਾਲੀ obj_transition_ਹਾਲਤਾਂ(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(ਆਕਾਰ(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    ਜੇ(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        ਵਾਪਸ;
    }
    uint32_t tsi = 0;
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ਜੇ(state_act & (~new_state)) ਜਾਰੀ; /*Skip unrelated styles*/
        ਜੇ(obj_style->is_trans) ਜਾਰੀ;

        lv_style_ਮੁੱਲ_t v;
        ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_ਮਿਲਿਆ) ਜਾਰੀ;
        ਅਚਲ lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        ਲਈ(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            ਲਈ(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                ਜੇ(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) ਰੋਕੋ;
            }

            /*If not found  add it*/
            ਜੇ(t == tsi) {
                ts[tsi].ਸਮਾਂ = tr->ਸਮਾਂ;
                ts[tsi].delay = tr->delay;
                ts[tsi].ਰਸਤਾ_cb = tr->ਰਸਤਾ_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_ਅੰਕੜੇ = tr->user_ਅੰਕੜੇ;
                ts[tsi].selector = obj_style->selector;
                tsi++;
            }
        }
    }

    ਲਈ(i = 0; i < tsi; i++) {
        lv_part_t part_act = lv_obj_style_get_selector_part(ts[i].selector);
        lv_obj_style_create_transition(obj, part_act, prev_state, new_state, &ts[i]);
    }

    lv_free(ts);
}

/**
 * Apply the state to the children of the object
 * @param obj pointer to an object
 * @param state the state to apply
 */
ਸਥਿਰ ਖਾਲੀ lv_obj_children_ਜੋੜੋ_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t ਬੱਚਾ_ਗਿਣਤੀ = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);

    ਲਈ(uint32_t i = 0; i < ਬੱਚਾ_ਗਿਣਤੀ; i++) {
        lv_obj_t * ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(obj, i);
        ਜੇ(ਬੱਚਾ) {
            lv_obj_ਜੋੜੋ_state(ਬੱਚਾ, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
ਸਥਿਰ ਖਾਲੀ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t ਬੱਚਾ_ਗਿਣਤੀ = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);

    ਲਈ(uint32_t i = 0; i < ਬੱਚਾ_ਗਿਣਤੀ; i++) {
        lv_obj_t * ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(obj, i);
        ਜੇ(ਬੱਚਾ) {
            lv_obj_remove_state(ਬੱਚਾ, state);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_ਅੰਕੜੇ(e);
    *obj_ptr = ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ screen_ਲੱਦੋ_on_trigger_event_cb(lv_event_t * e)
{
    screen_ਲੱਦੋ_anim_dsc_t * dsc = lv_event_get_user_ਅੰਕੜੇ(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_ਲੱਦੋ_anim(dsc->ਨਿਸ਼ਾਨਾ.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

ਸਥਿਰ ਖਾਲੀ screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_ਲੱਦੋ_anim_dsc_t * dsc = lv_event_get_user_ਅੰਕੜੇ(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->ਨਿਸ਼ਾਨਾ.create_cb();
    lv_screen_ਲੱਦੋ_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_ਜੋੜੋ_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_ਅੰਕੜੇ(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t ਤਰੱਕੀ = lv_anim_timeline_get_ਤਰੱਕੀ(dsc->at);
    ਜੇ(dsc->ਉਲਟ) {
        ਜੇ(ਤਰੱਕੀ == 0) {
            lv_anim_timeline_ਸੈੱਟ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_ਸੈੱਟ_ਤਰੱਕੀ(dsc->at, LV_ANIM_TIMELINE_ਤਰੱਕੀ_MAX);
        }
    }
    ਨਹੀਂਤਾਂ {
        ਜੇ(ਤਰੱਕੀ == LV_ANIM_TIMELINE_ਤਰੱਕੀ_MAX) {
            lv_anim_timeline_ਸੈੱਟ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_ਸੈੱਟ_ਤਰੱਕੀ(dsc->at, 0);
        }

    }
    lv_anim_timeline_ਸੈੱਟ_ਉਲਟ(dsc->at, dsc->ਉਲਟ);
    lv_anim_timeline_ਸ਼ੁਰੂ(dsc->at);
}


ਸਥਿਰ ਖਾਲੀ delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_ਨਿਸ਼ਾਨਾ_obj(e));
}

ਸਥਿਰ ਖਾਲੀ call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ਨਿਰਾ);
    lv_obj_t * obj = lv_event_get_ਨਿਸ਼ਾਨਾ_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_ਅੰਕੜੇ(e);
#ਜੇ !LV_USE_ASSERT
    LV_ਅਣਵਰਤਿਆ(obj);
#ਜੇ_ਅੰਤ
    LV_ASSERT(dsc != ਨਿਰਾ);
    LV_ASSERT(dsc->cb != ਨਿਰਾ);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_ਅੰਕੜੇ);
    lv_obj_remove_delete_cb(dsc);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_state(lv_obj_t * obj, lv_state_t state,  bool ਮੁੱਲ)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    ਜੇ(ਮੁੱਲ) lv_obj_ਜੋੜੋ_state(obj, state);
    ਨਹੀਂਤਾਂ lv_obj_remove_state(obj, state);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool has_state(ਅਚਲ lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    ਵਾਪਸ !!(obj->state & state);
}


#ਜੇ LV_USE_CHECK_OBJ_ਮਾਪਾ_LINK && LV_USE_ASSERT
ਸਥਿਰ bool ਮਾਪਾ_ਸ਼ਾਮਲ_ਹੈ_ਬੱਚਾ(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ lv_obj_t * ਬੱਚਾ)
{
    uint32_t ਬੱਚਾ_cnt = ਮਾਪਾ->spec_attr ? ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt : 0;
    uint32_t i;
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        ਜੇ(ਮਾਪਾ->spec_attr->children[i] == ਬੱਚਾ) ਵਾਪਸ true;
    }
    ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ


#ਜੇ LV_USE_OBJ_PROPERTY
ਸਥਿਰ lv_ਬਿੰਦੂ_t lv_obj_get_scroll_ਸਿਰਾ_helper(lv_obj_t * obj)
{
    lv_ਬਿੰਦੂ_t ਬਿੰਦੂ;
    lv_obj_get_scroll_ਸਿਰਾ(obj, &ਬਿੰਦੂ);
    ਵਾਪਸ ਬਿੰਦੂ;
}

ਸਥਿਰ ਖਾਲੀ obj_ਸੈੱਟ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    ਜੇ(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_ਸੈੱਟ_hidden(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_ਸੈੱਟ_clickable(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_ਸੈੱਟ_click_focusable(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_ਸੈੱਟ_checkable(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_ਸੈੱਟ_scrollable(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_ਸੈੱਟ_scroll_elastic(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_ਸੈੱਟ_scroll_momentum(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_ਸੈੱਟ_scroll_one(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_ਸੈੱਟ_scroll_chain_hor(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_ਸੈੱਟ_scroll_chain_ver(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_ਸੈੱਟ_scroll_on_focus(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_ਸੈੱਟ_scroll_with_arrow(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_ਸੈੱਟ_snappable(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_PRESS_ਤਾਲਾ)            lv_obj_ਸੈੱਟ_press_ਤਾਲਾ(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_ਸੈੱਟ_event_bubble(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_ਸੈੱਟ_gesture_bubble(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_ਸੈੱਟ_adv_hittest(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_ਸੈੱਟ_ignore_layout(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_FLOATING)              lv_obj_ਸੈੱਟ_floating(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_ਸੈੱਟ_send_draw_task_events(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_ਉਛਾਲ_VISIBLE)      lv_obj_ਸੈੱਟ_ਉਛਾਲ_visible(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_ਸੈੱਟ_event_trickle(obj, v);
    ਜੇ(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_ਸੈੱਟ_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_ਬਿੱਟ = 0;
    ਜੇ(f & LV_OBJ_FLAG_LAYOUT_1)              user_ਬਿੱਟ |= 1 << 0;
    ਜੇ(f & LV_OBJ_FLAG_LAYOUT_2)              user_ਬਿੱਟ |= 1 << 1;
    ਜੇ(f & LV_OBJ_FLAG_WIDGET_1)              user_ਬਿੱਟ |= 1 << 2;
    ਜੇ(f & LV_OBJ_FLAG_WIDGET_2)              user_ਬਿੱਟ |= 1 << 3;
    ਜੇ(f & LV_OBJ_FLAG_USER_1)                user_ਬਿੱਟ |= 1 << 4;
    ਜੇ(f & LV_OBJ_FLAG_USER_2)                user_ਬਿੱਟ |= 1 << 5;
    ਜੇ(f & LV_OBJ_FLAG_USER_3)                user_ਬਿੱਟ |= 1 << 6;
    ਜੇ(f & LV_OBJ_FLAG_USER_4)                user_ਬਿੱਟ |= 1 << 7;
    ਜੇ(user_ਬਿੱਟ == 0) ਵਾਪਸ;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    ਜੇ(v) {
        obj->spec_attr->user_flags |= user_ਬਿੱਟ;
    }
    ਨਹੀਂਤਾਂ {
        obj->spec_attr->user_flags &= ~user_ਬਿੱਟ;
    }

    ਅਚਲ uint8_t layout_ਨਕਾਬ = 0x3;
    ਅਚਲ bool layout_flag_ਬਦਲਿਆ = (old_user_flags ^ obj->spec_attr->user_flags) & layout_ਨਕਾਬ;
    ਜੇ(layout_flag_ਬਦਲਿਆ) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ਮਾਪਾ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ਸਥਿਰ bool obj_has_flag_any(ਅਚਲ lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    ਜੇ((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_PRESS_ਤਾਲਾ) && obj->press_ਤਾਲਾ) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_FLOATING) && obj->floating) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_ਉਛਾਲ_VISIBLE) && obj->ਉਛਾਲ_visible) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) ਵਾਪਸ true;

    ਜੇ(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ false;

    ਜੇ((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) ਵਾਪਸ true;
    ਜੇ((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) ਵਾਪਸ true;

    ਵਾਪਸ false;
}

ਸਥਿਰ lv_ਨਤੀਜਾ_t lv_obj_ਸੈੱਟ_any(lv_obj_t * obj, lv_prop_id_t id, ਅਚਲ lv_property_t * prop)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    ਜੇ(id >= LV_PROPERTY_OBJ_FLAG_ਸ਼ੁਰੂ && id <= LV_PROPERTY_OBJ_FLAG_ਸਿਰਾ) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ਸ਼ੁਰੂ);
        obj_ਸੈੱਟ_flag(obj, flag, prop->ਅੰਕ != 0);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }
    ਨਹੀਂਤਾਂ ਜੇ(id >= LV_PROPERTY_OBJ_STATE_ਸ਼ੁਰੂ && id <= LV_PROPERTY_OBJ_STATE_ਸਿਰਾ) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_ਸ਼ੁਰੂ);
        ਜੇ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        ਜੇ(prop->ਅੰਕ) lv_obj_ਜੋੜੋ_state(obj, state);
        ਨਹੀਂਤਾਂ lv_obj_remove_state(obj, state);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }
    ਨਹੀਂਤਾਂ {
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }
}

ਸਥਿਰ lv_ਨਤੀਜਾ_t lv_obj_get_any(ਅਚਲ lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    ਜੇ(id >= LV_PROPERTY_OBJ_FLAG_ਸ਼ੁਰੂ && id <= LV_PROPERTY_OBJ_FLAG_ਸਿਰਾ) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ਸ਼ੁਰੂ);
        prop->id = id;
        prop->ਅੰਕ = obj_has_flag_any(obj, flag);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }
    ਨਹੀਂਤਾਂ ਜੇ(id >= LV_PROPERTY_OBJ_STATE_ਸ਼ੁਰੂ && id <= LV_PROPERTY_OBJ_STATE_ਸਿਰਾ) {
        prop->id = id;
        ਜੇ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->ਅੰਕ = obj->state;
        }
        ਨਹੀਂਤਾਂ {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_ਸ਼ੁਰੂ);
            prop->ਅੰਕ = obj->state & flag;
        }
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }
    ਨਹੀਂਤਾਂ {
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }
}
#ਜੇ_ਅੰਤ /*LV_USE_OBJ_PROPERTY*/

