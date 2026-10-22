/**
 * @file lv_obj_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_event_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * Special, rarely used attributes.
 * They are allocated automatically if any elements is set.
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ _lv_obj_spec_attr_t {
    lv_obj_t ** children;           /**< Store the pointer of the children in an array.*/
    lv_ਸਮੂਹ_t * ਸਮੂਹ_p;
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    lv_ਮੈਟ੍ਰਿਕਸ_t * ਮੈਟ੍ਰਿਕਸ;           /**< The transform matrix*/
#ਜੇ_ਅੰਤ
    lv_event_list_t event_list;
#ਜੇ LV_USE_OBJ_NAME
    ਅਚਲ ਅੱਖਰ * name;              /**< Pointer to the name */
#ਜੇ_ਅੰਤ
    lv_ਬਿੰਦੂ_t scroll;              /**< The current X/Y scroll offset*/

    int32_t ext_click_pad;          /**< Extra click padding in all direction*/
    int32_t ext_draw_ਮਾਪ;          /**< EXTend the size in every direction for drawing.*/

    uint16_t ਬੱਚਾ_cnt;             /**< Number of children*/
    uint16_t scrollbar_ਬਹੁਲਕ : 2;    /**< How to display scrollbars, see `lv_scrollbar_mode_t`*/
    uint16_t scroll_snap_x : 2;     /**< Where to align the snappable children horizontally, see `lv_scroll_snap_t`*/
    uint16_t scroll_snap_y : 2;     /**< Where to align the snappable children vertically*/
    uint16_t scroll_dir : 4;        /**< The allowed scroll direction(s), see `lv_dir_t`*/
    uint16_t layer_type : 2;        /**< Cache the layer type here. Element of lv_intermediate_layer_type_t */
    uint16_t name_static : 1;       /**< 1: `name` was not dynamically allocated */
    uint16_t user_flags : 8;         /**< Store custom flags */
} lv_obj_spec_attr_t;


ਬਣਤਰ _lv_obj_t {
#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    lv_ext_ਅੰਕੜੇ_t ext_ਅੰਕੜੇ;
#ਜੇ_ਅੰਤ
    ਅਚਲ lv_obj_class_t * class_p;
    lv_obj_t * ਮਾਪਾ;
    lv_obj_spec_attr_t * spec_attr;
    lv_obj_style_t * styles;
#ਜੇ LV_OBJ_STYLE_CACHE
    uint32_t style_main_prop_is_ਸੈੱਟ;
    uint32_t style_other_prop_is_ਸੈੱਟ;
#ਜੇ_ਅੰਤ
    ਖਾਲੀ * user_ਅੰਕੜੇ;
#ਜੇ LV_USE_OBJ_ID
    ਖਾਲੀ * id;
#ਜੇ_ਅੰਤ
    lv_area_t coords;
    /** Make the object hidden. (Like it wasn't there at all) */
    uint32_t hidden : 1;

    /** Make the object clickable by the input devices */
    uint32_t clickable : 1;

    /** Add focused state to the object when clicked */
    uint32_t click_focusable : 1;

    /** Toggle checked state when the object is clicked */
    uint32_t checkable : 1;

    /** Make the object scrollable */
    uint32_t scrollable : 1;

    /** Allow scrolling inside but with slower speed */
    uint32_t scroll_elastic : 1;

    /** Make the object scroll further when "thrown" */
    uint32_t scroll_momentum : 1;

    /** Allow scrolling only one snappable child */
    uint32_t scroll_one : 1;

    /** Allow propagating the horizontal scroll to a parent */
    uint32_t scroll_chain_hor : 1;

    /** Allow propagating the vertical scroll to a parent */
    uint32_t scroll_chain_ver : 1;

    /** Automatically scroll object to make it visible when focused */
    uint32_t scroll_on_focus : 1;

    /** Allow scrolling the focused object with arrow keys */
    uint32_t scroll_with_arrow : 1;

    /** If scroll snap is enabled on the parent it can snap to this object */
    uint32_t snappable : 1;

    /** Keep the object pressed even if the press slid from the object */
    uint32_t press_ਤਾਲਾ : 1;

    /** Propagate the events to the parent too */
    uint32_t event_bubble : 1;

    /** Propagate the gestures to the parent */
    uint32_t gesture_bubble : 1;

    /** Allow performing more accurate hit (click) test */
    uint32_t adv_hittest : 1;

    /** Make the object not positioned by the layouts */
    uint32_t ignore_layout : 1;

    /** Do not scroll the object when the parent scrolls and ignore layout */
    uint32_t floating : 1;

    /** Send LV_EVENT_DRAW_TASK_ADDED events */
    uint32_t send_draw_task_events : 1;

    /** Do not clip the children to the parent's ext draw size */
    uint32_t ਉਛਾਲ_visible : 1;

    /** Propagate the events to the children too */
    uint32_t event_trickle : 1;

    /** Propagate the states to the children too */
    uint32_t state_trickle : 1;

    /** Allow only one RADIO_BUTTON sibling to be checked */
    uint32_t radio_button : 1;

    uint16_t state;
    uint16_t layout_inv : 1;
    uint16_t readjust_scroll_after_layout : 1;
    uint16_t scr_layout_inv : 1;
    uint16_t skip_trans : 1;
    uint16_t style_cnt  : 6;
    uint16_t h_layout   : 1;
    uint16_t w_layout   : 1;
    uint16_t h_ignore_ਮਾਪ : 1; /* ignore this obj when calculating content height of parent */
    uint16_t w_ignore_ਮਾਪ : 1; /* ignore this obj when calculating content width of parent */
    uint16_t is_deleting : 1;

    /** The widget is rendered at least once already.
     * It's used to skip initial animations and transitions. */
    uint16_t rendered : 1;

    /** The widget has blur or a drop shadow in its current state, so a change
     * behind it must invalidate its full extent. It's updated by
     * lv_obj_update_blur_status() and counted in blur_obj_cnt in lv_global_t. */
    uint16_t has_blur : 1;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Allocate special data for an object if not allocated yet.
 * @param obj   pointer to an object
 * @return the spec_attr created or NULL if something went wrong
 */
lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj);

lv_ਨਤੀਜਾ_t lv_obj_ਜੋੜੋ_ਬੱਚਾ(lv_obj_t * ਮਾਪਾ, lv_obj_t * ਬੱਚਾ);
ਖਾਲੀ lv_obj_remove_ਬੱਚਾ(lv_obj_t * ਮਾਪਾ, lv_obj_t * ਬੱਚਾ);

/**
 * Expand the display's invalidated areas to cover blur objects whose background
 * changed. It's called once per frame and invalidates the full extent of every
 * blur object that overlaps an invalidated area, repeating the walk until a
 * pass adds no new areas to cover transitive blur-over-blur cases.
 * Does nothing while no widget has blur (blur_obj_cnt in lv_global_t is zero).
 * @param disp  pointer to a display
 */
ਖਾਲੀ lv_obj_invalidate_expand_blur(lv_display_t * disp);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBJ_PRIVATE_H*/
