/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_event_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_style_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_area_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_event_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../indev/lv_indev_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_draw_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_DEF_ಅಗಲ    (LV_DPX(100))
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_DEF_ಎತ್ತರ   (LV_DPX(50))
#ವ್ಯಾಖ್ಯಾನಿಸು STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_screen_ತುಂಬು_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    ಒಕ್ಕೂಟ {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } ಗುರಿ;
} screen_ತುಂಬು_anim_dsc_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool ಹಿಮ್ಮುಖ;
} timeline_play_dsc_t;

ರಚನೆ _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_constructor(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_destructor(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_draw(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_event(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
ಸ್ಥಿರ ಶೂನ್ಯ obj_transition_ಸ್ಥಿತಿಗಳು(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
ಸ್ಥಿರ ಶೂನ್ಯ update_obj_state(lv_obj_t * obj, lv_state_t new_state);
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_children_ಕೂಡಿಸು_state(lv_obj_t * obj, lv_state_t state);
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
ಸ್ಥಿರ ಶೂನ್ಯ null_on_delete_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ screen_ತುಂಬು_on_trigger_event_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ screen_create_on_trigger_event_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ play_timeline_on_trigger_event_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ delete_on_screen_unloaded_event_cb(lv_event_t * e);
ಸ್ಥಿರ ಶೂನ್ಯ call_delete_cb(lv_event_t * e);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_state(lv_obj_t * obj, lv_state_t state,  bool ಮೌಲ್ಯ);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool has_state(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_state_t state);
#ಆದರೆ LV_USE_CHECK_OBJ_ಪೋಷಕ_LINK && LV_USE_ASSERT
    ಸ್ಥಿರ bool ಪೋಷಕ_ಒಳಗೊಂಡಿದೆ_ಮಗು(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ lv_obj_t * ಮಗು);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_OBJ_PROPERTY
    ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t lv_obj_ಗಣ_any(lv_obj_t *, lv_prop_id_t, ಸ್ಥಿರಾಂಕ lv_property_t *);
    ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t lv_obj_get_any(ಸ್ಥಿರಾಂಕ lv_obj_t *, lv_prop_id_t, lv_property_t *);

    ಸ್ಥಿರ lv_ಬಿಂದು_t lv_obj_get_scroll_ಕೊನೆ_helper(lv_obj_t * obj);

    ಸ್ಥಿರ ಶೂನ್ಯ obj_ಗಣ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    ಸ್ಥಿರ bool obj_has_flag_any(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_obj_flag_t f);
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *  STATIC VARIABLES
 **********************/
#ಆದರೆ LV_USE_OBJ_PROPERTY
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_ಪೋಷಕ,
        .setter = lv_obj_ಗಣ_ಪೋಷಕ,
        .getter = lv_obj_get_ಪೋಷಕ,
    },
    {
        .id = LV_PROPERTY_OBJ_X,
        .setter = lv_obj_ಗಣ_x,
        .getter = lv_obj_get_x,
    },
    {
        .id = LV_PROPERTY_OBJ_Y,
        .setter = lv_obj_ಗಣ_y,
        .getter = lv_obj_get_y,
    },
    {
        .id = LV_PROPERTY_OBJ_W,
        .setter = lv_obj_ಗಣ_ಅಗಲ,
        .getter = lv_obj_get_ಅಗಲ,
    },
    {
        .id = LV_PROPERTY_OBJ_H,
        .setter = lv_obj_ಗಣ_ಎತ್ತರ,
        .getter = lv_obj_get_ಎತ್ತರ,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_ಅಗಲ,
        .setter = lv_obj_ಗಣ_content_ಅಗಲ,
        .getter = lv_obj_get_content_ಅಗಲ,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_ಎತ್ತರ,
        .setter = lv_obj_ಗಣ_content_ಎತ್ತರ,
        .getter = lv_obj_get_content_ಎತ್ತರ,
    },
    {
        .id = LV_PROPERTY_OBJ_LAYOUT,
        .setter = lv_obj_ಗಣ_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_ALIGN,
        .setter = lv_obj_ಗಣ_align,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLBAR_ಬಹುಲಕ,
        .setter = lv_obj_ಗಣ_scrollbar_ಬಹುಲಕ,
        .getter = lv_obj_get_scrollbar_ಬಹುಲಕ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_DIR,
        .setter = lv_obj_ಗಣ_scroll_dir,
        .getter = lv_obj_get_scroll_dir,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_X,
        .setter = lv_obj_ಗಣ_scroll_snap_x,
        .getter = lv_obj_get_scroll_snap_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_Y,
        .setter = lv_obj_ಗಣ_scroll_snap_y,
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
        .id = LV_PROPERTY_OBJ_SCROLL_ತುದಿ,
        .getter = lv_obj_get_scroll_ತುದಿ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ಎಡ,
        .getter = lv_obj_get_scroll_ಎಡ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ಬಲ,
        .getter = lv_obj_get_scroll_ಬಲ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ಕೊನೆ,
        .getter = lv_obj_get_scroll_ಕೊನೆ_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_ಗಾತ್ರ,
        .getter = lv_obj_get_ext_draw_ಗಾತ್ರ,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_ಎಣಿಕೆ,
        .getter = lv_obj_get_event_ಎಣಿಕೆ,
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
        .id = LV_PROPERTY_OBJ_ಮಗು_ಎಣಿಕೆ,
        .getter = lv_obj_get_ಮಗು_ಎಣಿಕೆ,
    },
    {
        .id = LV_PROPERTY_OBJ_ಸೂಚ್ಯಂಕ,
        .getter = lv_obj_get_ಸೂಚ್ಯಂಕ,
    },
#ಆದರೆ LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_ಗಣ_name,
    },
#ಆದರೆ_ಅಂತ್ಯ
    {
        .id = LV_PROPERTY_OBJ_HIDDEN,
        .setter = lv_obj_ಗಣ_hidden,
        .getter = lv_obj_is_hidden,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICKABLE,
        .setter = lv_obj_ಗಣ_clickable,
        .getter = lv_obj_is_clickable,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICK_FOCUSABLE,
        .setter = lv_obj_ಗಣ_click_focusable,
        .getter = lv_obj_is_click_focusable,
    },
    {
        .id = LV_PROPERTY_OBJ_CHECKABLE,
        .setter = lv_obj_ಗಣ_checkable,
        .getter = lv_obj_is_checkable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLABLE,
        .setter = lv_obj_ಗಣ_scrollable,
        .getter = lv_obj_is_scrollable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ELASTIC,
        .setter = lv_obj_ಗಣ_scroll_elastic,
        .getter = lv_obj_is_scroll_elastic,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_MOMENTUM,
        .setter = lv_obj_ಗಣ_scroll_momentum,
        .getter = lv_obj_is_scroll_momentum,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ONE,
        .setter = lv_obj_ಗಣ_scroll_one,
        .getter = lv_obj_is_scroll_one,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_HOR,
        .setter = lv_obj_ಗಣ_scroll_chain_hor,
        .getter = lv_obj_is_scroll_chain_hor,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_VER,
        .setter = lv_obj_ಗಣ_scroll_chain_ver,
        .getter = lv_obj_is_scroll_chain_ver,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ON_FOCUS,
        .setter = lv_obj_ಗಣ_scroll_on_focus,
        .getter = lv_obj_is_scroll_on_focus,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_WITH_ARROW,
        .setter = lv_obj_ಗಣ_scroll_with_arrow,
        .getter = lv_obj_is_scroll_with_arrow,
    },
    {
        .id = LV_PROPERTY_OBJ_SNAPPABLE,
        .setter = lv_obj_ಗಣ_snappable,
        .getter = lv_obj_is_snappable,
    },
    {
        .id = LV_PROPERTY_OBJ_PRESS_ಬೀಗ,
        .setter = lv_obj_ಗಣ_press_ಬೀಗ,
        .getter = lv_obj_is_press_ಬೀಗ,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_BUBBLE,
        .setter = lv_obj_ಗಣ_event_bubble,
        .getter = lv_obj_is_event_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_GESTURE_BUBBLE,
        .setter = lv_obj_ಗಣ_gesture_bubble,
        .getter = lv_obj_is_gesture_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_ADV_HITTEST,
        .setter = lv_obj_ಗಣ_adv_hittest,
        .getter = lv_obj_is_adv_hittest,
    },
    {
        .id = LV_PROPERTY_OBJ_IGNORE_LAYOUT,
        .setter = lv_obj_ಗಣ_ignore_layout,
        .getter = lv_obj_is_ignore_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_FLOATING,
        .setter = lv_obj_ಗಣ_floating,
        .getter = lv_obj_is_floating,
    },
    {
        .id = LV_PROPERTY_OBJ_SEND_DRAW_TASK_EVENTS,
        .setter = lv_obj_ಗಣ_send_draw_task_events,
        .getter = lv_obj_is_send_draw_task_events,
    },
    {
        .id = LV_PROPERTY_OBJ_ಅತಿಪ್ರವಾಹ_VISIBLE,
        .setter = lv_obj_ಗಣ_ಅತಿಪ್ರವಾಹ_visible,
        .getter = lv_obj_is_ಅತಿಪ್ರವಾಹ_visible,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_TRICKLE,
        .setter = lv_obj_ಗಣ_event_trickle,
        .getter = lv_obj_is_event_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_STATE_TRICKLE,
        .setter = lv_obj_ಗಣ_state_trickle,
        .getter = lv_obj_is_state_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_FLEX_ಒಳ_NEW_TRACK,
        .setter = lv_obj_ಗಣ_flex_ಒಳ_new_track,
        .getter = lv_obj_is_flex_ಒಳ_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_ಗಣ_any,
        .getter = lv_obj_get_any,
    },
};
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರಾಂಕ lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .ಅಗಲ_def = LV_DPI_DEF,
    .ಎತ್ತರ_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .ಗುಂಪು_def = LV_OBJ_CLASS_ಗುಂಪು_DEF_FALSE,
    .instance_ಗಾತ್ರ = (ಗಾತ್ರ(lv_obj_t)),
    .ತಳ_class = ಖಾಲಿ,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * ಪೋಷಕ)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, ಪೋಷಕ);

    ಆದರೆ(obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    ಹಿಂತಿರುಗಿ obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

ಶೂನ್ಯ lv_obj_ಗಣ_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಆದರೆ(obj->hidden == en) ಹಿಂತಿರುಗಿ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    ಆದರೆ(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    ಆದರೆ(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    ಆದರೆ(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_ಗುಂಪು_t * ಗುಂಪು = lv_obj_get_ಗುಂಪು(obj);
        ಆದರೆ(ಗುಂಪು != ಖಾಲಿ) {
            lv_ಗುಂಪು_focus_ಮುಂದಿನ(ಗುಂಪು);
            lv_obj_t * ಮುಂದಿನ_obj = lv_ಗುಂಪು_get_focused(ಗುಂಪು);
            ಆದರೆ(ಮುಂದಿನ_obj != ಖಾಲಿ) {
                lv_obj_invalidate(ಮುಂದಿನ_obj);
            }
        }
    }

    ಆದರೆ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ಪೋಷಕ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->clickable = en;

}

ಶೂನ್ಯ lv_obj_ಗಣ_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->click_focusable = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->checkable = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(obj->scrollable == en) ಹಿಂತಿರುಗಿ;

    /*Invalidate the scrollbars while scrollable*/
    ಆದರೆ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    ಆದರೆ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_elastic = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_momentum = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_one = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_chain_hor = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_chain_ver = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_on_focus = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->scroll_with_arrow = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->snappable = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_press_ಬೀಗ(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->press_ಬೀಗ = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->event_bubble = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->gesture_bubble = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->adv_hittest = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(obj->ignore_layout == en) ಹಿಂತಿರುಗಿ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    ಆದರೆ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ಪೋಷಕ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(obj->floating == en) ಹಿಂತಿರುಗಿ;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    ಆದರೆ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ಪೋಷಕ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->send_draw_task_events = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_ಅತಿಪ್ರವಾಹ_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->ಅತಿಪ್ರವಾಹ_visible = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->event_trickle = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->state_trickle = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->radio_button = en;
}

ಶೂನ್ಯ lv_obj_ಗಣ_flex_ಒಳ_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    ಆದರೆ(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        ಆದರೆ(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_ಪೋಷಕ(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_state_t new_state = obj->state | state;
    ಆದರೆ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ಆದರೆ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_ಕೂಡಿಸು_state(obj, state);
        }
    }
}

ಶೂನ್ಯ lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_state_t new_state = obj->state & (~state);
    ಆದರೆ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ಆದರೆ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, state, v);
}

ಶೂನ್ಯ lv_obj_ಗಣ_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_ALT, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_CHECKED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_FOCUSED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_focus_ಕೀಲಿ(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_FOCUS_ಕೀಲಿ, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_EDITED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_HOVERED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_PRESSED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_SCROLLED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_DISABLED, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_USER_1, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_USER_2, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_USER_3, en);
}

ಶೂನ್ಯ lv_obj_ಗಣ_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    ಗಣ_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->hidden;
}

bool lv_obj_is_clickable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->clickable;
}

bool lv_obj_is_click_focusable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->click_focusable;
}

bool lv_obj_is_checkable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->checkable;
}

bool lv_obj_is_scrollable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scrollable;
}

bool lv_obj_is_scroll_elastic(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->snappable;
}

bool lv_obj_is_press_ಬೀಗ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->press_ಬೀಗ;
}

bool lv_obj_is_event_bubble(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->ignore_layout;
}

bool lv_obj_is_floating(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->floating;
}

bool lv_obj_is_send_draw_task_events(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->send_draw_task_events;
}

bool lv_obj_is_ಅತಿಪ್ರವಾಹ_visible(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->ಅತಿಪ್ರವಾಹ_visible;
}

bool lv_obj_is_event_trickle(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->event_trickle;
}

bool lv_obj_is_state_trickle(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->state_trickle;
}

bool lv_obj_is_radio_button(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->radio_button;
}

bool lv_obj_is_flex_ಒಳ_new_track(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    ಹಿಂತಿರುಗಿ obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ obj->state;
}

bool lv_obj_has_state(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, state);
}

bool lv_obj_is_alt(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_ಕೀಲಿ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_FOCUS_ಕೀಲಿ);
}

bool lv_obj_is_edited(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ has_state(obj, LV_STATE_USER_4);
}

lv_ಗುಂಪು_t * lv_obj_get_ಗುಂಪು(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ obj->spec_attr->ಗುಂಪು_p;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_ಫಲಿತಾಂಶ_t lv_obj_ಕೂಡಿಸು_ಮಗು(lv_obj_t * ಪೋಷಕ, lv_obj_t * ಮಗು)
{
    LV_CHECK_OBJ(ಪೋಷಕ, MY_CLASS, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);
    LV_CHECK_ARG(ಮಗು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    uint16_t new_ಮಗು_cnt = ಪೋಷಕ->spec_attr->ಮಗು_cnt + 1;

    lv_obj_t ** children = lv_realloc(ಪೋಷಕ->spec_attr->children,
                                      new_ಮಗು_cnt * (ಗಾತ್ರ(lv_obj_t *)));
    ಆದರೆ(!children) {
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }
    children[new_ಮಗು_cnt - 1] = ಮಗು;

    ಪೋಷಕ->spec_attr->ಮಗು_cnt = new_ಮಗು_cnt;
    ಪೋಷಕ->spec_attr->children = children;
    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
}

ಶೂನ್ಯ lv_obj_remove_ಮಗು(lv_obj_t * ಪೋಷಕ, lv_obj_t * ಮಗು)
{
    LV_CHECK_OBJ(ಪೋಷಕ, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(ಮಗು, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಗಾಗಿ(int32_t i = lv_obj_get_ಸೂಚ್ಯಂಕ(ಮಗು); i < (int32_t)ಪೋಷಕ->spec_attr->ಮಗು_cnt - 1; i++) {
        ಪೋಷಕ->spec_attr->children[i] = ಪೋಷಕ->spec_attr->children[i + 1];
    }

    /* No more children*/
    ಆದರೆ(ಪೋಷಕ->spec_attr->ಮಗು_cnt == 1) {
        lv_free(ಪೋಷಕ->spec_attr->children);
        ಪೋಷಕ->spec_attr->children = ಖಾಲಿ;
        ಪೋಷಕ->spec_attr->ಮಗು_cnt = 0;
        ಹಿಂತಿರುಗಿ;
    }
    ಪೋಷಕ->spec_attr->ಮಗು_cnt--;
    lv_obj_t ** new_children = lv_realloc(ಪೋಷಕ->spec_attr->children, ಪೋಷಕ->spec_attr->ಮಗು_cnt * (ಗಾತ್ರ(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    ಆದರೆ(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        ಹಿಂತಿರುಗಿ;
    }
    ಪೋಷಕ->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(obj->spec_attr) {
        ಹಿಂತಿರುಗಿ obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(ಗಾತ್ರ(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    ಆದರೆ(!spec_attr) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_ಬಹುಲಕ = LV_SCROLLBAR_ಬಹುಲಕ_AUTO;
    obj->spec_attr = spec_attr;
    ಹಿಂತಿರುಗಿ spec_attr;
}


bool lv_obj_check_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    ಹಿಂತಿರುಗಿ obj->class_p == class_p;
}

bool lv_obj_has_class(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * obj_class = obj->class_p;
    ಆಗ(obj_class) {
        ಆದರೆ(obj_class == class_p) ಹಿಂತಿರುಗಿ true;
        obj_class = obj_class->ತಳ_class;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರಾಂಕ lv_obj_class_t * lv_obj_get_class(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    ಹಿಂತಿರುಗಿ obj->class_p;
}

bool lv_obj_is_ಒಳ_widget_ವೃಕ್ಷ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    ಸ್ಥಿರಾಂಕ lv_obj_t * current = obj;

    ಆಗ(current->ಪೋಷಕ != ಖಾಲಿ) {
#ಆದರೆ LV_USE_CHECK_OBJ_ಪೋಷಕ_LINK && LV_USE_ASSERT
        LV_ASSERT(ಪೋಷಕ_ಒಳಗೊಂಡಿದೆ_ಮಗು(current->ಪೋಷಕ, current));
#ಆದರೆ_ಅಂತ್ಯ
        current = current->ಪೋಷಕ;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_ಮುಂದಿನ(ಖಾಲಿ);
    ಆಗ(disp) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < disp->screen_cnt; i++) {
            ಆದರೆ(disp->screens[i] == current) ಹಿಂತಿರುಗಿ true;
        }
        disp = lv_display_get_ಮುಂದಿನ(disp);
    }

    ಹಿಂತಿರುಗಿ false;
}

ಶೂನ್ಯ lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(*obj_ptr != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_obj_ಕೂಡಿಸು_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#ಆದರೆ LV_USE_OBJ_ID
ಶೂನ್ಯ * lv_obj_get_id(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    ಹಿಂತಿರುಗಿ obj->id;
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_screen_ತುಂಬು_event(lv_obj_t * obj, lv_event_ಸಂಕೇತ_t trigger, lv_obj_t * screen,
                                  lv_screen_ತುಂಬು_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG_ಸಂದೇಶ(screen != ಖಾಲಿ, ಹಿಂತಿರುಗಿ, "can't load a non-existing screen");

    screen_ತುಂಬು_anim_dsc_t * dsc = lv_malloc(ಗಾತ್ರ(screen_ತುಂಬು_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ಗಾತ್ರ(screen_ತುಂಬು_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ಗುರಿ.screen = screen;

    lv_obj_ಕೂಡಿಸು_event_cb(obj, screen_ತುಂಬು_on_trigger_event_cb, trigger, dsc);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, dsc);
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_screen_create_event(lv_obj_t * obj, lv_event_ಸಂಕೇತ_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_ತುಂಬು_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(screen_create_cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    screen_ತುಂಬು_anim_dsc_t * dsc = lv_malloc(ಗಾತ್ರ(screen_ತುಂಬು_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ಗಾತ್ರ(screen_ತುಂಬು_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->ಗುರಿ.create_cb = screen_create_cb;

    lv_obj_ಕೂಡಿಸು_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, dsc);
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_play_timeline_event(lv_obj_t * obj, lv_event_ಸಂಕೇತ_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool ಹಿಮ್ಮುಖ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(at != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    timeline_play_dsc_t * dsc = lv_malloc(ಗಾತ್ರ(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, ಗಾತ್ರ(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->ಹಿಮ್ಮುಖ = ಹಿಮ್ಮುಖ;

    lv_obj_ಕೂಡಿಸು_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, lv_event_free_user_ದತ್ತಾಂಶ_cb, LV_EVENT_DELETE, dsc);
}

ಶೂನ್ಯ lv_obj_ಗಣ_user_ದತ್ತಾಂಶ(lv_obj_t * obj, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    obj->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_user_flag(lv_obj_t * obj, uint32_t ಬಿಟ್, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(ಬಿಟ್ < 4, ಹಿಂತಿರುಗಿ);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    ಆದರೆ(spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(v) spec_attr->user_flags |= 1 << (ಬಿಟ್ + 4);
    ಇಲ್ಲದಿದ್ದರೆ spec_attr->user_flags &= ~(1 << (ಬಿಟ್ + 4));
}

ಶೂನ್ಯ * lv_obj_get_user_ದತ್ತಾಂಶ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ obj->user_ದತ್ತಾಂಶ;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t ಬಿಟ್)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(ಬಿಟ್ < 4, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ (obj->spec_attr->user_flags & 1 << (ಬಿಟ್ + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_ಕೂಡಿಸು_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(cb != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_delete_dsc_t * dsc = lv_malloc(ಗಾತ್ರ(*dsc));
    ಆದರೆ(!dsc) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;

    lv_event_dsc_t * event_dsc = lv_obj_ಕೂಡಿಸು_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    ಆದರೆ(!event_dsc) {
        lv_free(dsc);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    ಹಿಂತಿರುಗಿ dsc;
}

ಶೂನ್ಯ lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    ಆದರೆ(!dsc) {
        ಹಿಂತಿರುಗಿ;
    }

    uint32_t ಎಣಿಕೆ_2 = lv_obj_remove_event_cb_with_user_ದತ್ತಾಂಶ(dsc->obj, call_delete_cb, dsc);
    ಆದರೆ(ಎಣಿಕೆ_2 == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        ಹಿಂತಿರುಗಿ;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_constructor(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    LV_ಬಳಸದ(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * ಪೋಷಕ = obj->ಪೋಷಕ;
    ಆದರೆ(ಪೋಷಕ) {
        int32_t sl = lv_obj_get_scroll_ಎಡ(ಪೋಷಕ);
        int32_t st = lv_obj_get_scroll_ತುದಿ(ಪೋಷಕ);

        obj->coords.y1 = ಪೋಷಕ->coords.y1 + lv_obj_get_style_pad_ತುದಿ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = ಪೋಷಕ->coords.x1 + lv_obj_get_style_pad_ಎಡ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) - sl;
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
    ಆದರೆ(ಪೋಷಕ) {
        obj->press_ಬೀಗ = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#ಆದರೆ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ನಿಯೋಜಿಸು
    lv_obj_ನಿಯೋಜಿಸು_id(class_p, obj);
#ಆದರೆ_ಅಂತ್ಯ

    LV_TRACE_OBJ_CREATE("finished");
}

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_destructor(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    LV_ಬಳಸದ(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, ಖಾಲಿ);

    /*Delete from the group*/
    lv_ಗುಂಪು_t * ಗುಂಪು = lv_obj_get_ಗುಂಪು(obj);
    ಆದರೆ(ಗುಂಪು) lv_ಗುಂಪು_remove_obj(obj);

    ಆದರೆ(obj->spec_attr) {
        ಆದರೆ(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = ಖಾಲಿ;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#ಆದರೆ LV_USE_OBJ_NAME
        ಆದರೆ(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((ಶೂನ್ಯ *)obj->spec_attr->name);
        }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
        ಆದರೆ(obj->spec_attr->ಮಾತೃಕೆ) {
            lv_free(obj->spec_attr->ಮಾತೃಕೆ);
            obj->spec_attr->ಮಾತೃಕೆ = ಖಾಲಿ;
        }
#ಆದರೆ_ಅಂತ್ಯ

        lv_free(obj->spec_attr);
        obj->spec_attr = ಖಾಲಿ;
    }

#ಆದರೆ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ನಿಯೋಜಿಸು
    lv_obj_free_id(obj);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_draw(lv_event_t * e)
{
    lv_event_ಸಂಕೇತ_t ಸಂಕೇತ_2 = lv_event_get_ಸಂಕೇತ(e);
    lv_obj_t * obj = lv_event_get_current_ಗುರಿ(e);
    ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        ಆದರೆ(info->res == LV_COVER_RES_MASKED) ಹಿಂತಿರುಗಿ;
        ಆದರೆ(lv_obj_get_style_clip_ಮೂಲೆ_ಆಂತರಿಕ(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            ಹಿಂತಿರುಗಿ;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        ಆದರೆ(lv_area_is_ಒಳ(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(lv_obj_get_style_bg_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(lv_obj_get_style_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(lv_obj_get_style_bg_grad_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            ಆದರೆ(lv_obj_get_style_bg_grad_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                ಹಿಂತಿರುಗಿ;
            }
        }
        ಸ್ಥಿರಾಂಕ lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        ಆದರೆ(grad_dsc) {
            uint32_t i;
            ಗಾಗಿ(i = 0; i < grad_dsc->stops_ಎಣಿಕೆ; i++) {
                ಆದರೆ(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    ಹಿಂತಿರುಗಿ;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.ತಳ.layer = layer;

        int32_t w = lv_obj_get_style_transform_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        ಆದರೆ(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.ಮೂಲೆ_radius = draw_dsc.radius;
            blur_dsc.ತಳ.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        ಆದರೆ(lv_obj_get_style_border_post_ಆಂತರಿಕ(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_DRAW_MAIN_ಕೊನೆ) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        ಆದರೆ(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.ಮೂಲೆ_radius = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            blur_dsc.ತಳ.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        ಆದರೆ(lv_obj_get_style_border_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_ಆಂತರಿಕ(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.ತಳ.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_rect(layer, &draw_dsc, &coords);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&hor_area) <= 0 && lv_area_get_ಗಾತ್ರ(&ver_area) <= 0) ಹಿಂತಿರುಗಿ;

    lv_draw_rect_dsc_t rect_dsc;
    lv_ಫಲಿತಾಂಶ_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    ಆದರೆ(sb_res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.ಮೂಲೆ_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);

    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&hor_area) > 0) {
        ಆದರೆ(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.ತಳ.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.ತಳ.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        ಆದರೆ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&ver_area) > 0) {
        blur_dsc.ತಳ.id1 = 1;
        ಆದರೆ(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.ತಳ.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        ಆದರೆ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    ಆದರೆ(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    ಆದರೆ(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
        ಆದರೆ(dsc->border_ಅಗಲ > 0) {
            dsc->border_color = lv_obj_get_style_border_color_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    ಆದರೆ(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_ಅಗಲ = lv_obj_get_style_shadow_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
        ಆದರೆ(dsc->shadow_ಅಗಲ > 0) {
            dsc->shadow_ಹರಡುವಿಕೆ = lv_obj_get_style_shadow_ಹರಡುವಿಕೆ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    ಆದರೆ(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    ಆದರೆ(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_event(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_ಬಳಸದ(class_p);

    lv_event_ಸಂಕೇತ_t ಸಂಕೇತ_2 = lv_event_get_ಸಂಕೇತ(e);
    lv_obj_t * obj = lv_event_get_current_ಗುರಿ(e);
    ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_PRESSED) {
        lv_obj_ಕೂಡಿಸು_state(obj, LV_STATE_PRESSED);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        ಆದರೆ((indev == ಖಾಲಿ || lv_indev_get_scroll_obj(indev) == ಖಾಲಿ) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ಆದರೆ(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_ಕೂಡಿಸು_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            ಆದರೆ(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(obj, LV_EVENT_ಮೌಲ್ಯ_ಬದಲಾಯಿತು, ಖಾಲಿ);
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ಖಾಲಿ);
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_ಮೌಲ್ಯ_ಬದಲಾಯಿತು) {
        ಆದರೆ(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
            ಆದರೆ(ಪೋಷಕ) {
                uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
                uint32_t i;
                ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_ಮಗು(ಪೋಷಕ, i);
                    ಆದರೆ(obj == sibling) ಮುಂದುವರಿ;

                    ಆದರೆ(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(sibling, LV_EVENT_ಮೌಲ್ಯ_ಬದಲಾಯಿತು, ಖಾಲಿ);
                        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, ಖಾಲಿ);
                        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
                    }
                }
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_PRESS_ಕಳೆದುಹೋದ) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_STYLE_ಬದಲಾಯಿತು) {
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
        ಗಾಗಿ(uint32_t i = 0; i < ಮಗು_cnt; i++) {
            lv_obj_t * ಮಗು = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(ಮಗು);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_ಕೀಲಿ) {
        ಆದರೆ(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_ಕೀಲಿ(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ಆದರೆ(c == LV_ಕೀಲಿ_ಬಲ || c == LV_ಕೀಲಿ_ಮೇಲೆ) {
                lv_obj_ಕೂಡಿಸು_state(obj, LV_STATE_CHECKED);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(c == LV_ಕೀಲಿ_ಎಡ || c == LV_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
                /*Radio buttons can't be checked off directly*/
                ಆದರೆ(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            ಆದರೆ(c != LV_ಕೀಲಿ_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(obj, LV_EVENT_ಮೌಲ್ಯ_ಬದಲಾಯಿತು, ಖಾಲಿ);
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ಖಾಲಿ);
                ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_ಎಡ(obj);
            int32_t sr = lv_obj_get_scroll_ಬಲ(obj);
            uint32_t c = lv_event_get_ಕೀಲಿ(e);
            ಆದರೆ(c == LV_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_ಗಮ್ಯ_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ಎತ್ತರ(obj) / 4, anim_enable);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(c == LV_ಕೀಲಿ_ಮೇಲೆ) {
                lv_obj_scroll_ಗಮ್ಯ_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ಎತ್ತರ(obj) / 4, anim_enable);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(c == LV_ಕೀಲಿ_ಬಲ) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ಆದರೆ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_ಗಮ್ಯ_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ಎತ್ತರ(obj) / 4, anim_enable);
                ಇಲ್ಲದಿದ್ದರೆ
                    lv_obj_scroll_ಗಮ್ಯ_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_ಅಗಲ(obj) / 4, anim_enable);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(c == LV_ಕೀಲಿ_ಎಡ) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ಆದರೆ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_ಗಮ್ಯ_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ಎತ್ತರ(obj) / 4, anim_enable);
                ಇಲ್ಲದಿದ್ದರೆ
                    lv_obj_scroll_ಗಮ್ಯ_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_ಅಗಲ(obj) / 4, anim_enable);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_FOCUSED) {
        ಆದರೆ(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_ಗಮ್ಯ_view_recursive(obj, LV_ANIM_ON);
        }

        lv_ಗುಂಪು_t * ಗುಂಪು = lv_obj_get_ಗುಂಪು(obj);
        ಸ್ಥಿರಾಂಕ bool editing = ಗುಂಪು && lv_ಗುಂಪು_get_editing(ಗುಂಪು);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_ಸಕ್ರಿಯ();
        ಆದರೆ(indev == ಖಾಲಿ) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != ಖಾಲಿ ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        ಆದರೆ(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_ಕೀಲಿ;
        ಆದರೆ(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_ಕೂಡಿಸು_state(obj, state);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            lv_obj_ಕೂಡಿಸು_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_ಕೂಡಿಸು_state(obj, LV_STATE_SCROLLED);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_SCROLL_ಕೊನೆ) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        ಆದರೆ(lv_obj_get_scrollbar_ಬಹುಲಕ(obj) == LV_SCROLLBAR_ಬಹುಲಕ_ಸಕ್ರಿಯ) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_ಕೀಲಿ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_ಗಾತ್ರ_ಬದಲಾಯಿತು) {
        int32_t align = lv_obj_get_style_align_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        ಆದರೆ(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
        ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
            lv_obj_t * ಮಗು = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(ಮಗು);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_ಮಗು_ಬದಲಾಯಿತು) {
        int32_t align = lv_obj_get_style_align_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_ಆಂತರಿಕ(obj, LV_PART_MAIN);
        ಆದರೆ(layout || align || lv_obj_is_style_any_ಅಗಲ_content(obj) || lv_obj_is_style_any_ಎತ್ತರ_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_ಮಗು_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ) {
        int32_t d = lv_obj_calculate_ext_draw_ಗಾತ್ರ(obj, LV_PART_MAIN);
        lv_event_ಗಣ_ext_draw_ಗಾತ್ರ(e, d);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_DRAW_MAIN || ಸಂಕೇತ_2 == LV_EVENT_DRAW_POST || ಸಂಕೇತ_2 == LV_EVENT_DRAW_MAIN_ಕೊನೆ ||
            ಸಂಕೇತ_2 == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_HOVER_OVER) {
        lv_obj_ಕೂಡಿಸು_state(obj, LV_STATE_HOVERED);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸಂಕೇತ_2 == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    ಆದರೆ(obj->state == new_state) ಹಿಂತಿರುಗಿ;


    lv_state_t prev_state = obj->state;

    lv_part_t ಬದಲಾಯಿತು_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &ಬದಲಾಯಿತು_part);
    /*If there is no difference in styles there is nothing else to do*/
    ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_ಬದಲಾಯಿತು, &prev_state);
        ಹಿಂತಿರುಗಿ;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_ಸ್ಥಿತಿ(obj);

    /*Skip transitions if the widget is not rendered yet. */
    ಆದರೆ(!obj->rendered) {
        lv_obj_invalidate(obj);
        ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_ಗಾತ್ರ(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_ಬದಲಾಯಿತು, &prev_state);
        ಹಿಂತಿರುಗಿ;
    }

    obj_transition_ಸ್ಥಿತಿಗಳು(obj, prev_state, new_state);

    ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && ಬದಲಾಯಿತು_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_ಗಾತ್ರ(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_ಬದಲಾಯಿತು, &prev_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_transition_ಸ್ಥಿತಿಗಳು(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(ಗಾತ್ರ(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    ಆದರೆ(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        ಹಿಂತಿರುಗಿ;
    }
    uint32_t tsi = 0;
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ಆದರೆ(state_act & (~new_state)) ಮುಂದುವರಿ; /*Skip unrelated styles*/
        ಆದರೆ(obj_style->is_trans) ಮುಂದುವರಿ;

        lv_style_ಮೌಲ್ಯ_t v;
        ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_ಸಿಕ್ಕಿತು) ಮುಂದುವರಿ;
        ಸ್ಥಿರಾಂಕ lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        ಗಾಗಿ(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            ಗಾಗಿ(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                ಆದರೆ(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) ನಿಲ್ಲಿಸು;
            }

            /*If not found  add it*/
            ಆದರೆ(t == tsi) {
                ts[tsi].ಸಮಯ = tr->ಸಮಯ;
                ts[tsi].delay = tr->delay;
                ts[tsi].ಮಾರ್ಗ_cb = tr->ಮಾರ್ಗ_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_ದತ್ತಾಂಶ = tr->user_ದತ್ತಾಂಶ;
                ts[tsi].selector = obj_style->selector;
                tsi++;
            }
        }
    }

    ಗಾಗಿ(i = 0; i < tsi; i++) {
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
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_children_ಕೂಡಿಸು_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t ಮಗು_ಎಣಿಕೆ = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);

    ಗಾಗಿ(uint32_t i = 0; i < ಮಗು_ಎಣಿಕೆ; i++) {
        lv_obj_t * ಮಗು = lv_obj_get_ಮಗು(obj, i);
        ಆದರೆ(ಮಗು) {
            lv_obj_ಕೂಡಿಸು_state(ಮಗು, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t ಮಗು_ಎಣಿಕೆ = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);

    ಗಾಗಿ(uint32_t i = 0; i < ಮಗು_ಎಣಿಕೆ; i++) {
        lv_obj_t * ಮಗು = lv_obj_get_ಮಗು(obj, i);
        ಆದರೆ(ಮಗು) {
            lv_obj_remove_state(ಮಗು, state);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_ದತ್ತಾಂಶ(e);
    *obj_ptr = ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ screen_ತುಂಬು_on_trigger_event_cb(lv_event_t * e)
{
    screen_ತುಂಬು_anim_dsc_t * dsc = lv_event_get_user_ದತ್ತಾಂಶ(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_ತುಂಬು_anim(dsc->ಗುರಿ.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

ಸ್ಥಿರ ಶೂನ್ಯ screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_ತುಂಬು_anim_dsc_t * dsc = lv_event_get_user_ದತ್ತಾಂಶ(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->ಗುರಿ.create_cb();
    lv_screen_ತುಂಬು_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_ಕೂಡಿಸು_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_ದತ್ತಾಂಶ(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t ಪ್ರಗತಿ = lv_anim_timeline_get_ಪ್ರಗತಿ(dsc->at);
    ಆದರೆ(dsc->ಹಿಮ್ಮುಖ) {
        ಆದರೆ(ಪ್ರಗತಿ == 0) {
            lv_anim_timeline_ಗಣ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_ಗಣ_ಪ್ರಗತಿ(dsc->at, LV_ANIM_TIMELINE_ಪ್ರಗತಿ_MAX);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ(ಪ್ರಗತಿ == LV_ANIM_TIMELINE_ಪ್ರಗತಿ_MAX) {
            lv_anim_timeline_ಗಣ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_ಗಣ_ಪ್ರಗತಿ(dsc->at, 0);
        }

    }
    lv_anim_timeline_ಗಣ_ಹಿಮ್ಮುಖ(dsc->at, dsc->ಹಿಮ್ಮುಖ);
    lv_anim_timeline_ಆರಂಭ(dsc->at);
}


ಸ್ಥಿರ ಶೂನ್ಯ delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_ಗುರಿ_obj(e));
}

ಸ್ಥಿರ ಶೂನ್ಯ call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ಖಾಲಿ);
    lv_obj_t * obj = lv_event_get_ಗುರಿ_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_ದತ್ತಾಂಶ(e);
#ಆದರೆ !LV_USE_ASSERT
    LV_ಬಳಸದ(obj);
#ಆದರೆ_ಅಂತ್ಯ
    LV_ASSERT(dsc != ಖಾಲಿ);
    LV_ASSERT(dsc->cb != ಖಾಲಿ);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_ದತ್ತಾಂಶ);
    lv_obj_remove_delete_cb(dsc);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_state(lv_obj_t * obj, lv_state_t state,  bool ಮೌಲ್ಯ)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    ಆದರೆ(ಮೌಲ್ಯ) lv_obj_ಕೂಡಿಸು_state(obj, state);
    ಇಲ್ಲದಿದ್ದರೆ lv_obj_remove_state(obj, state);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool has_state(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ !!(obj->state & state);
}


#ಆದರೆ LV_USE_CHECK_OBJ_ಪೋಷಕ_LINK && LV_USE_ASSERT
ಸ್ಥಿರ bool ಪೋಷಕ_ಒಳಗೊಂಡಿದೆ_ಮಗು(ಸ್ಥಿರಾಂಕ lv_obj_t * ಪೋಷಕ, ಸ್ಥಿರಾಂಕ lv_obj_t * ಮಗು)
{
    uint32_t ಮಗು_cnt = ಪೋಷಕ->spec_attr ? ಪೋಷಕ->spec_attr->ಮಗು_cnt : 0;
    uint32_t i;
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        ಆದರೆ(ಪೋಷಕ->spec_attr->children[i] == ಮಗು) ಹಿಂತಿರುಗಿ true;
    }
    ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ LV_USE_OBJ_PROPERTY
ಸ್ಥಿರ lv_ಬಿಂದು_t lv_obj_get_scroll_ಕೊನೆ_helper(lv_obj_t * obj)
{
    lv_ಬಿಂದು_t ಬಿಂದು;
    lv_obj_get_scroll_ಕೊನೆ(obj, &ಬಿಂದು);
    ಹಿಂತಿರುಗಿ ಬಿಂದು;
}

ಸ್ಥಿರ ಶೂನ್ಯ obj_ಗಣ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    ಆದರೆ(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_ಗಣ_hidden(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_ಗಣ_clickable(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_ಗಣ_click_focusable(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_ಗಣ_checkable(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_ಗಣ_scrollable(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_ಗಣ_scroll_elastic(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_ಗಣ_scroll_momentum(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_ಗಣ_scroll_one(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_ಗಣ_scroll_chain_hor(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_ಗಣ_scroll_chain_ver(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_ಗಣ_scroll_on_focus(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_ಗಣ_scroll_with_arrow(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_ಗಣ_snappable(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_PRESS_ಬೀಗ)            lv_obj_ಗಣ_press_ಬೀಗ(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_ಗಣ_event_bubble(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_ಗಣ_gesture_bubble(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_ಗಣ_adv_hittest(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_ಗಣ_ignore_layout(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_FLOATING)              lv_obj_ಗಣ_floating(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_ಗಣ_send_draw_task_events(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_ಅತಿಪ್ರವಾಹ_VISIBLE)      lv_obj_ಗಣ_ಅತಿಪ್ರವಾಹ_visible(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_ಗಣ_event_trickle(obj, v);
    ಆದರೆ(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_ಗಣ_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_ಬಿಟ್ಟುಗಳು = 0;
    ಆದರೆ(f & LV_OBJ_FLAG_LAYOUT_1)              user_ಬಿಟ್ಟುಗಳು |= 1 << 0;
    ಆದರೆ(f & LV_OBJ_FLAG_LAYOUT_2)              user_ಬಿಟ್ಟುಗಳು |= 1 << 1;
    ಆದರೆ(f & LV_OBJ_FLAG_WIDGET_1)              user_ಬಿಟ್ಟುಗಳು |= 1 << 2;
    ಆದರೆ(f & LV_OBJ_FLAG_WIDGET_2)              user_ಬಿಟ್ಟುಗಳು |= 1 << 3;
    ಆದರೆ(f & LV_OBJ_FLAG_USER_1)                user_ಬಿಟ್ಟುಗಳು |= 1 << 4;
    ಆದರೆ(f & LV_OBJ_FLAG_USER_2)                user_ಬಿಟ್ಟುಗಳು |= 1 << 5;
    ಆದರೆ(f & LV_OBJ_FLAG_USER_3)                user_ಬಿಟ್ಟುಗಳು |= 1 << 6;
    ಆದರೆ(f & LV_OBJ_FLAG_USER_4)                user_ಬಿಟ್ಟುಗಳು |= 1 << 7;
    ಆದರೆ(user_ಬಿಟ್ಟುಗಳು == 0) ಹಿಂತಿರುಗಿ;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    ಆದರೆ(v) {
        obj->spec_attr->user_flags |= user_ಬಿಟ್ಟುಗಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj->spec_attr->user_flags &= ~user_ಬಿಟ್ಟುಗಳು;
    }

    ಸ್ಥಿರಾಂಕ uint8_t layout_ಮುಖವಾಡ = 0x3;
    ಸ್ಥಿರಾಂಕ bool layout_flag_ಬದಲಾಯಿತು = (old_user_flags ^ obj->spec_attr->user_flags) & layout_ಮುಖವಾಡ;
    ಆದರೆ(layout_flag_ಬದಲಾಯಿತು) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ಪೋಷಕ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ಸ್ಥಿರ bool obj_has_flag_any(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_PRESS_ಬೀಗ) && obj->press_ಬೀಗ) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_FLOATING) && obj->floating) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_ಅತಿಪ್ರವಾಹ_VISIBLE) && obj->ಅತಿಪ್ರವಾಹ_visible) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) ಹಿಂತಿರುಗಿ true;

    ಆದರೆ(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    ಆದರೆ((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) ಹಿಂತಿರುಗಿ true;
    ಆದರೆ((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) ಹಿಂತಿರುಗಿ true;

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t lv_obj_ಗಣ_any(lv_obj_t * obj, lv_prop_id_t id, ಸ್ಥಿರಾಂಕ lv_property_t * prop)
{
    LV_ASSERT(obj != ಖಾಲಿ);

    ಆದರೆ(id >= LV_PROPERTY_OBJ_FLAG_ಆರಂಭ && id <= LV_PROPERTY_OBJ_FLAG_ಕೊನೆ) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ಆರಂಭ);
        obj_ಗಣ_flag(obj, flag, prop->ಅಂಕೆ != 0);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(id >= LV_PROPERTY_OBJ_STATE_ಆರಂಭ && id <= LV_PROPERTY_OBJ_STATE_ಕೊನೆ) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_ಆರಂಭ);
        ಆದರೆ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        ಆದರೆ(prop->ಅಂಕೆ) lv_obj_ಕೂಡಿಸು_state(obj, state);
        ಇಲ್ಲದಿದ್ದರೆ lv_obj_remove_state(obj, state);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }
}

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t lv_obj_get_any(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    ಆದರೆ(id >= LV_PROPERTY_OBJ_FLAG_ಆರಂಭ && id <= LV_PROPERTY_OBJ_FLAG_ಕೊನೆ) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ಆರಂಭ);
        prop->id = id;
        prop->ಅಂಕೆ = obj_has_flag_any(obj, flag);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(id >= LV_PROPERTY_OBJ_STATE_ಆರಂಭ && id <= LV_PROPERTY_OBJ_STATE_ಕೊನೆ) {
        prop->id = id;
        ಆದರೆ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->ಅಂಕೆ = obj->state;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_ಆರಂಭ);
            prop->ಅಂಕೆ = obj->state & flag;
        }
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }
}
#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_PROPERTY*/

