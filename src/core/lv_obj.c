/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "lv_obj_private.h"
#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "../misc/lv_event_private.h"
#અન્તર્ભાવ "../misc/lv_style_private.h"
#અન્તર્ભાવ "../misc/lv_area_private.h"
#અન્તર્ભાવ "lv_obj_style_private.h"
#અન્તર્ભાવ "lv_obj_event_private.h"
#અન્તર્ભાવ "lv_obj_class_private.h"
#અન્તર્ભાવ "../indev/lv_indev_private.h"
#અન્તર્ભાવ "../display/lv_display_private.h"
#અન્તર્ભાવ "lv_obj_draw_private.h"
#અન્તર્ભાવ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#define LV_OBJ_DEF_WIDTH    (LV_DPX(100))
#define LV_OBJ_DEF_HEIGHT   (LV_DPX(50))
#define STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

પ્રકારવ્યાખ્યા સંરચના {
    lv_screen_load_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    સંઘ {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } target;
} screen_load_anim_dsc_t;

પ્રકારવ્યાખ્યા સંરચના {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool reverse;
} timeline_play_dsc_t;

સંરચના _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    શૂન્ય * user_data;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
સ્થિર શૂન્ય lv_obj_constructor(અચલ lv_obj_class_t * class_p, lv_obj_t * obj);
સ્થિર શૂન્ય lv_obj_destructor(અચલ lv_obj_class_t * class_p, lv_obj_t * obj);
સ્થિર શૂન્ય lv_obj_draw(lv_event_t * e);
સ્થિર શૂન્ય lv_obj_event(અચલ lv_obj_class_t * class_p, lv_event_t * e);
સ્થિર શૂન્ય draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
સ્થિર lv_result_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
સ્થિર શૂન્ય obj_transition_states(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
સ્થિર શૂન્ય update_obj_state(lv_obj_t * obj, lv_state_t new_state);
સ્થિર શૂન્ય lv_obj_children_add_state(lv_obj_t * obj, lv_state_t state);
સ્થિર શૂન્ય lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
સ્થિર શૂન્ય null_on_delete_cb(lv_event_t * e);
સ્થિર શૂન્ય screen_load_on_trigger_event_cb(lv_event_t * e);
સ્થિર શૂન્ય screen_create_on_trigger_event_cb(lv_event_t * e);
સ્થિર શૂન્ય play_timeline_on_trigger_event_cb(lv_event_t * e);
સ્થિર શૂન્ય delete_on_screen_unloaded_event_cb(lv_event_t * e);
સ્થિર શૂન્ય call_delete_cb(lv_event_t * e);

સ્થિર અંતઃરેખ શૂન્ય set_state(lv_obj_t * obj, lv_state_t state,  bool કિંમત);
સ્થિર અંતઃરેખ bool has_state(અચલ lv_obj_t * obj, lv_state_t state);
#જો LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
    સ્થિર bool parent_contains_child(અચલ lv_obj_t * parent, અચલ lv_obj_t * child);
#endif

#જો LV_USE_OBJ_PROPERTY
    સ્થિર lv_result_t lv_obj_set_any(lv_obj_t *, lv_prop_id_t, અચલ lv_property_t *);
    સ્થિર lv_result_t lv_obj_get_any(અચલ lv_obj_t *, lv_prop_id_t, lv_property_t *);

    સ્થિર lv_point_t lv_obj_get_scroll_end_helper(lv_obj_t * obj);

    સ્થિર શૂન્ય obj_set_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    સ્થિર bool obj_has_flag_any(અચલ lv_obj_t * obj, lv_obj_flag_t f);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
#જો LV_USE_OBJ_PROPERTY
સ્થિર અચલ lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_PARENT,
        .setter = lv_obj_set_parent,
        .getter = lv_obj_get_parent,
    },
    {
        .id = LV_PROPERTY_OBJ_X,
        .setter = lv_obj_set_x,
        .getter = lv_obj_get_x,
    },
    {
        .id = LV_PROPERTY_OBJ_Y,
        .setter = lv_obj_set_y,
        .getter = lv_obj_get_y,
    },
    {
        .id = LV_PROPERTY_OBJ_W,
        .setter = lv_obj_set_width,
        .getter = lv_obj_get_width,
    },
    {
        .id = LV_PROPERTY_OBJ_H,
        .setter = lv_obj_set_height,
        .getter = lv_obj_get_height,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_WIDTH,
        .setter = lv_obj_set_content_width,
        .getter = lv_obj_get_content_width,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_HEIGHT,
        .setter = lv_obj_set_content_height,
        .getter = lv_obj_get_content_height,
    },
    {
        .id = LV_PROPERTY_OBJ_LAYOUT,
        .setter = lv_obj_set_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_ALIGN,
        .setter = lv_obj_set_align,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLBAR_MODE,
        .setter = lv_obj_set_scrollbar_mode,
        .getter = lv_obj_get_scrollbar_mode,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_DIR,
        .setter = lv_obj_set_scroll_dir,
        .getter = lv_obj_get_scroll_dir,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_X,
        .setter = lv_obj_set_scroll_snap_x,
        .getter = lv_obj_get_scroll_snap_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_Y,
        .setter = lv_obj_set_scroll_snap_y,
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
        .id = LV_PROPERTY_OBJ_SCROLL_ટોચ,
        .getter = lv_obj_get_scroll_ટોચ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ડાબું,
        .getter = lv_obj_get_scroll_ડાબું,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_જમણું,
        .getter = lv_obj_get_scroll_જમણું,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_END,
        .getter = lv_obj_get_scroll_end_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_SIZE,
        .getter = lv_obj_get_ext_draw_size,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_ગણતરી,
        .getter = lv_obj_get_event_ગણતરી,
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
        .id = LV_PROPERTY_OBJ_CHILD_ગણતરી,
        .getter = lv_obj_get_child_ગણતરી,
    },
    {
        .id = LV_PROPERTY_OBJ_INDEX,
        .getter = lv_obj_get_index,
    },
#જો LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_set_name,
    },
#endif
    {
        .id = LV_PROPERTY_OBJ_HIDDEN,
        .setter = lv_obj_set_hidden,
        .getter = lv_obj_is_hidden,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICKABLE,
        .setter = lv_obj_set_clickable,
        .getter = lv_obj_is_clickable,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICK_FOCUSABLE,
        .setter = lv_obj_set_click_focusable,
        .getter = lv_obj_is_click_focusable,
    },
    {
        .id = LV_PROPERTY_OBJ_CHECKABLE,
        .setter = lv_obj_set_checkable,
        .getter = lv_obj_is_checkable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLABLE,
        .setter = lv_obj_set_scrollable,
        .getter = lv_obj_is_scrollable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ELASTIC,
        .setter = lv_obj_set_scroll_elastic,
        .getter = lv_obj_is_scroll_elastic,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_MOMENTUM,
        .setter = lv_obj_set_scroll_momentum,
        .getter = lv_obj_is_scroll_momentum,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ONE,
        .setter = lv_obj_set_scroll_one,
        .getter = lv_obj_is_scroll_one,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_HOR,
        .setter = lv_obj_set_scroll_chain_hor,
        .getter = lv_obj_is_scroll_chain_hor,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_VER,
        .setter = lv_obj_set_scroll_chain_ver,
        .getter = lv_obj_is_scroll_chain_ver,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ON_FOCUS,
        .setter = lv_obj_set_scroll_on_focus,
        .getter = lv_obj_is_scroll_on_focus,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_WITH_ARROW,
        .setter = lv_obj_set_scroll_with_arrow,
        .getter = lv_obj_is_scroll_with_arrow,
    },
    {
        .id = LV_PROPERTY_OBJ_SNAPPABLE,
        .setter = lv_obj_set_snappable,
        .getter = lv_obj_is_snappable,
    },
    {
        .id = LV_PROPERTY_OBJ_PRESS_LOCK,
        .setter = lv_obj_set_press_lock,
        .getter = lv_obj_is_press_lock,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_BUBBLE,
        .setter = lv_obj_set_event_bubble,
        .getter = lv_obj_is_event_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_GESTURE_BUBBLE,
        .setter = lv_obj_set_gesture_bubble,
        .getter = lv_obj_is_gesture_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_ADV_HITTEST,
        .setter = lv_obj_set_adv_hittest,
        .getter = lv_obj_is_adv_hittest,
    },
    {
        .id = LV_PROPERTY_OBJ_IGNORE_LAYOUT,
        .setter = lv_obj_set_ignore_layout,
        .getter = lv_obj_is_ignore_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_FLOATING,
        .setter = lv_obj_set_floating,
        .getter = lv_obj_is_floating,
    },
    {
        .id = LV_PROPERTY_OBJ_SEND_DRAW_TASK_EVENTS,
        .setter = lv_obj_set_send_draw_task_events,
        .getter = lv_obj_is_send_draw_task_events,
    },
    {
        .id = LV_PROPERTY_OBJ_OVERFLOW_VISIBLE,
        .setter = lv_obj_set_overflow_visible,
        .getter = lv_obj_is_overflow_visible,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_TRICKLE,
        .setter = lv_obj_set_event_trickle,
        .getter = lv_obj_is_event_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_STATE_TRICKLE,
        .setter = lv_obj_set_state_trickle,
        .getter = lv_obj_is_state_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_FLEX_IN_NEW_TRACK,
        .setter = lv_obj_set_flex_in_new_track,
        .getter = lv_obj_is_flex_in_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_set_any,
        .getter = lv_obj_get_any,
    },
};
#endif

અચલ lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .width_def = LV_DPI_DEF,
    .height_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_FALSE,
    .instance_size = (કદ(lv_obj_t)),
    .base_class = ખાલી,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);

    જો(obj == ખાલી) પાછા ખાલી;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    પાછા obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

શૂન્ય lv_obj_set_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    જો(obj->hidden == en) પાછા;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    જો(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    જો(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    જો(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_group_t * group = lv_obj_get_group(obj);
        જો(group != ખાલી) {
            lv_group_focus_next(group);
            lv_obj_t * next_obj = lv_group_get_focused(group);
            જો(next_obj != ખાલી) {
                lv_obj_invalidate(next_obj);
            }
        }
    }

    જો((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

શૂન્ય lv_obj_set_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->clickable = en;

}

શૂન્ય lv_obj_set_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->click_focusable = en;
}

શૂન્ય lv_obj_set_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->checkable = en;
}

શૂન્ય lv_obj_set_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(obj->scrollable == en) પાછા;

    /*Invalidate the scrollbars while scrollable*/
    જો(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    જો(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

શૂન્ય lv_obj_set_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_elastic = en;
}

શૂન્ય lv_obj_set_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_momentum = en;
}

શૂન્ય lv_obj_set_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_one = en;
}

શૂન્ય lv_obj_set_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

શૂન્ય lv_obj_set_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_chain_hor = en;
}

શૂન્ય lv_obj_set_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_chain_ver = en;
}

શૂન્ય lv_obj_set_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_on_focus = en;
}

શૂન્ય lv_obj_set_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->scroll_with_arrow = en;
}

શૂન્ય lv_obj_set_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->snappable = en;
}

શૂન્ય lv_obj_set_press_lock(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->press_lock = en;
}

શૂન્ય lv_obj_set_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->event_bubble = en;
}

શૂન્ય lv_obj_set_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->gesture_bubble = en;
}

શૂન્ય lv_obj_set_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->adv_hittest = en;
}

શૂન્ય lv_obj_set_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(obj->ignore_layout == en) પાછા;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    જો((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

શૂન્ય lv_obj_set_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(obj->floating == en) પાછા;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    જો((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

શૂન્ય lv_obj_set_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->send_draw_task_events = en;
}

શૂન્ય lv_obj_set_overflow_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->overflow_visible = en;
}

શૂન્ય lv_obj_set_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->event_trickle = en;
}

શૂન્ય lv_obj_set_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->state_trickle = en;
}

શૂન્ય lv_obj_set_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->radio_button = en;
}

શૂન્ય lv_obj_set_flex_in_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    જો(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    નહીંતર {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        જો(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

શૂન્ય lv_obj_add_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    lv_state_t new_state = obj->state | state;
    જો(obj->state != new_state) {
        update_obj_state(obj, new_state);
        જો(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_add_state(obj, state);
        }
    }
}

શૂન્ય lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    lv_state_t new_state = obj->state & (~state);
    જો(obj->state != new_state) {
        update_obj_state(obj, new_state);
        જો(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

શૂન્ય lv_obj_set_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, state, v);
}

શૂન્ય lv_obj_set_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_ALT, en);
}

શૂન્ય lv_obj_set_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_CHECKED, en);
}

શૂન્ય lv_obj_set_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_FOCUSED, en);
}

શૂન્ય lv_obj_set_focus_ચાવી(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_FOCUS_ચાવી, en);
}

શૂન્ય lv_obj_set_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_EDITED, en);
}

શૂન્ય lv_obj_set_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_HOVERED, en);
}

શૂન્ય lv_obj_set_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_PRESSED, en);
}

શૂન્ય lv_obj_set_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_SCROLLED, en);
}

શૂન્ય lv_obj_set_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_DISABLED, en);
}

શૂન્ય lv_obj_set_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_USER_1, en);
}

શૂન્ય lv_obj_set_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_USER_2, en);
}

શૂન્ય lv_obj_set_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_USER_3, en);
}

શૂન્ય lv_obj_set_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    set_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->hidden;
}

bool lv_obj_is_clickable(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->clickable;
}

bool lv_obj_is_click_focusable(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->click_focusable;
}

bool lv_obj_is_checkable(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->checkable;
}

bool lv_obj_is_scrollable(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scrollable;
}

bool lv_obj_is_scroll_elastic(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->snappable;
}

bool lv_obj_is_press_lock(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->press_lock;
}

bool lv_obj_is_event_bubble(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->ignore_layout;
}

bool lv_obj_is_floating(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->floating;
}

bool lv_obj_is_send_draw_task_events(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->send_draw_task_events;
}

bool lv_obj_is_overflow_visible(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->overflow_visible;
}

bool lv_obj_is_event_trickle(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->event_trickle;
}

bool lv_obj_is_state_trickle(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->state_trickle;
}

bool lv_obj_is_radio_button(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા obj->radio_button;
}

bool lv_obj_is_flex_in_new_track(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    જો(obj->spec_attr == ખાલી) પાછા false;

    પાછા obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા 0);

    પાછા obj->state;
}

bool lv_obj_has_state(અચલ lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, state);
}

bool lv_obj_is_alt(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_ચાવી(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_FOCUS_ચાવી);
}

bool lv_obj_is_edited(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    પાછા has_state(obj, LV_STATE_USER_4);
}

lv_group_t * lv_obj_get_group(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    જો(obj->spec_attr) પાછા obj->spec_attr->group_p;
    નહીંતર પાછા ખાલી;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_result_t lv_obj_add_child(lv_obj_t * parent, lv_obj_t * child)
{
    LV_CHECK_OBJ(parent, MY_CLASS, પાછા LV_RESULT_INVALID);
    LV_CHECK_ARG(child != ખાલી, પાછા LV_RESULT_INVALID);

    uint16_t new_child_cnt = parent->spec_attr->child_cnt + 1;

    lv_obj_t ** children = lv_realloc(parent->spec_attr->children,
                                      new_child_cnt * (કદ(lv_obj_t *)));
    જો(!children) {
        પાછા LV_RESULT_INVALID;
    }
    children[new_child_cnt - 1] = child;

    parent->spec_attr->child_cnt = new_child_cnt;
    parent->spec_attr->children = children;
    પાછા LV_RESULT_OK;
}

શૂન્ય lv_obj_remove_child(lv_obj_t * parent, lv_obj_t * child)
{
    LV_CHECK_OBJ(parent, MY_CLASS, પાછા);
    LV_CHECK_OBJ(child, MY_CLASS, પાછા);

    માટે(int32_t i = lv_obj_get_index(child); i < (int32_t)parent->spec_attr->child_cnt - 1; i++) {
        parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
    }

    /* No more children*/
    જો(parent->spec_attr->child_cnt == 1) {
        lv_free(parent->spec_attr->children);
        parent->spec_attr->children = ખાલી;
        parent->spec_attr->child_cnt = 0;
        પાછા;
    }
    parent->spec_attr->child_cnt--;
    lv_obj_t ** new_children = lv_realloc(parent->spec_attr->children, parent->spec_attr->child_cnt * (કદ(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    જો(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        પાછા;
    }
    parent->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    જો(obj->spec_attr) {
        પાછા obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(કદ(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    જો(!spec_attr) {
        પાછા ખાલી;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_mode = LV_SCROLLBAR_MODE_AUTO;
    obj->spec_attr = spec_attr;
    પાછા spec_attr;
}


bool lv_obj_check_type(અચલ lv_obj_t * obj, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);
    LV_CHECK_ARG(class_p != ખાલી, પાછા false);
    પાછા obj->class_p == class_p;
}

bool lv_obj_has_class(અચલ lv_obj_t * obj, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);
    LV_CHECK_ARG(class_p != ખાલી, પાછા false);

    અચલ lv_obj_class_t * obj_class = obj->class_p;
    જ્યારે(obj_class) {
        જો(obj_class == class_p) પાછા true;
        obj_class = obj_class->base_class;
    }

    પાછા false;
}

અચલ lv_obj_class_t * lv_obj_get_class(અચલ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    પાછા obj->class_p;
}

bool lv_obj_is_in_widget_વૃક્ષ(અચલ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    અચલ lv_obj_t * current = obj;

    જ્યારે(current->parent != ખાલી) {
#જો LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
        LV_ASSERT(parent_contains_child(current->parent, current));
#endif
        current = current->parent;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_next(ખાલી);
    જ્યારે(disp) {
        uint32_t i;
        માટે(i = 0; i < disp->screen_cnt; i++) {
            જો(disp->screens[i] == current) પાછા true;
        }
        disp = lv_display_get_next(disp);
    }

    પાછા false;
}

શૂન્ય lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != ખાલી, પાછા);
    LV_CHECK_ARG(*obj_ptr != ખાલી, પાછા);

    lv_obj_add_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#જો LV_USE_OBJ_ID
શૂન્ય * lv_obj_get_id(અચલ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    પાછા obj->id;
}

#endif

શૂન્ય lv_obj_add_screen_load_event(lv_obj_t * obj, lv_event_code_t trigger, lv_obj_t * screen,
                                  lv_screen_load_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG_સંદેશ(screen != ખાલી, પાછા, "can't load a non-existing screen");

    screen_load_anim_dsc_t * dsc = lv_malloc(કદ(screen_load_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, કદ(screen_load_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->target.screen = screen;

    lv_obj_add_event_cb(obj, screen_load_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

શૂન્ય lv_obj_add_screen_create_event(lv_obj_t * obj, lv_event_code_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_load_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG(screen_create_cb != ખાલી, પાછા);

    screen_load_anim_dsc_t * dsc = lv_malloc(કદ(screen_load_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, કદ(screen_load_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->target.create_cb = screen_create_cb;

    lv_obj_add_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

શૂન્ય lv_obj_add_play_timeline_event(lv_obj_t * obj, lv_event_code_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool reverse)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG(at != ખાલી, પાછા);

    timeline_play_dsc_t * dsc = lv_malloc(કદ(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, કદ(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->reverse = reverse;

    lv_obj_add_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

શૂન્ય lv_obj_set_user_data(lv_obj_t * obj, શૂન્ય * user_data)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    obj->user_data = user_data;
}

શૂન્ય lv_obj_set_user_flag(lv_obj_t * obj, uint32_t bit, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG(bit < 4, પાછા);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    જો(spec_attr == ખાલી) પાછા;

    જો(v) spec_attr->user_flags |= 1 << (bit + 4);
    નહીંતર spec_attr->user_flags &= ~(1 << (bit + 4));
}

શૂન્ય * lv_obj_get_user_data(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    પાછા obj->user_data;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t bit)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);
    LV_CHECK_ARG(bit < 4, પાછા false);

    જો(obj->spec_attr == ખાલી) પાછા false;
    નહીંતર પાછા (obj->spec_attr->user_flags & 1 << (bit + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_add_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, શૂન્ય * user_data)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);
    LV_CHECK_ARG(cb != ખાલી, પાછા ખાલી);

    lv_delete_dsc_t * dsc = lv_malloc(કદ(*dsc));
    જો(!dsc) {
        પાછા ખાલી;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_data = user_data;

    lv_event_dsc_t * event_dsc = lv_obj_add_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    જો(!event_dsc) {
        lv_free(dsc);
        પાછા ખાલી;
    }
    પાછા dsc;
}

શૂન્ય lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    જો(!dsc) {
        પાછા;
    }

    uint32_t ગણતરી = lv_obj_remove_event_cb_with_user_data(dsc->obj, call_delete_cb, dsc);
    જો(ગણતરી == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        પાછા;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

સ્થિર શૂન્ય lv_obj_constructor(અચલ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ખાલી);

    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * parent = obj->parent;
    જો(parent) {
        int32_t sl = lv_obj_get_scroll_ડાબું(parent);
        int32_t st = lv_obj_get_scroll_ટોચ(parent);

        obj->coords.y1 = parent->coords.y1 + lv_obj_get_style_pad_ટોચ_internal(parent, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = parent->coords.x1 + lv_obj_get_style_pad_ડાબું_internal(parent, LV_PART_MAIN) - sl;
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
    જો(parent) {
        obj->press_lock = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#જો LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ASSIGN
    lv_obj_assign_id(class_p, obj);
#endif

    LV_TRACE_OBJ_CREATE("finished");
}

સ્થિર શૂન્ય lv_obj_destructor(અચલ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ખાલી);

    LV_UNUSED(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, ખાલી);

    /*Delete from the group*/
    lv_group_t * group = lv_obj_get_group(obj);
    જો(group) lv_group_remove_obj(obj);

    જો(obj->spec_attr) {
        જો(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = ખાલી;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#જો LV_USE_OBJ_NAME
        જો(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((શૂન્ય *)obj->spec_attr->name);
        }
#endif

#જો LV_DRAW_TRANSFORM_USE_MATRIX
        જો(obj->spec_attr->matrix) {
            lv_free(obj->spec_attr->matrix);
            obj->spec_attr->matrix = ખાલી;
        }
#endif

        lv_free(obj->spec_attr);
        obj->spec_attr = ખાલી;
    }

#જો LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ASSIGN
    lv_obj_free_id(obj);
#endif
}

સ્થિર શૂન્ય lv_obj_draw(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    જો(code == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        જો(info->res == LV_COVER_RES_MASKED) પાછા;
        જો(lv_obj_get_style_clip_corner_internal(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            પાછા;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_internal(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_width_internal(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_height_internal(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        જો(lv_area_is_in(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            પાછા;
        }

        જો(lv_obj_get_style_bg_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            પાછા;
        }

        જો(lv_obj_get_style_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            પાછા;
        }

        જો(lv_obj_get_style_bg_grad_dir_internal(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            જો(lv_obj_get_style_bg_grad_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                પાછા;
            }
        }
        અચલ lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_internal(obj, LV_PART_MAIN);
        જો(grad_dsc) {
            uint32_t i;
            માટે(i = 0; i < grad_dsc->stops_ગણતરી; i++) {
                જો(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    પાછા;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    નહીંતર જો(code == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.base.layer = layer;

        int32_t w = lv_obj_get_style_transform_width_internal(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_height_internal(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_internal(obj, LV_PART_MAIN);
        જો(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.corner_radius = draw_dsc.radius;
            blur_dsc.base.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        જો(lv_obj_get_style_border_post_internal(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    નહીંતર જો(code == LV_EVENT_DRAW_MAIN_END) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_internal(obj, LV_PART_MAIN);
        જો(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_width_internal(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_height_internal(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.corner_radius = lv_obj_get_style_radius_internal(obj, LV_PART_MAIN);
            blur_dsc.base.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    નહીંતર જો(code == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        જો(lv_obj_get_style_border_width_internal(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_internal(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.base.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_width_internal(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_height_internal(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_rect(layer, &draw_dsc, &coords);
        }
    }
}

સ્થિર શૂન્ય draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    જો(lv_area_get_size(&hor_area) <= 0 && lv_area_get_size(&ver_area) <= 0) પાછા;

    lv_draw_rect_dsc_t rect_dsc;
    lv_result_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    જો(sb_res != LV_RESULT_OK) પાછા;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_internal(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.corner_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_internal(obj, LV_PART_SCROLLBAR);

    જો(lv_area_get_size(&hor_area) > 0) {
        જો(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.base.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.base.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        જો(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    જો(lv_area_get_size(&ver_area) > 0) {
        blur_dsc.base.id1 = 1;
        જો(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.base.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        જો(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
સ્થિર lv_result_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR);
    જો(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_internal(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR);
    જો(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_width = lv_obj_get_style_border_width_internal(obj, LV_PART_SCROLLBAR);
        જો(dsc->border_width > 0) {
            dsc->border_color = lv_obj_get_style_border_color_internal(obj, LV_PART_SCROLLBAR);
        }
        નહીંતર {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_internal(obj, LV_PART_SCROLLBAR);
    જો(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_width = lv_obj_get_style_shadow_width_internal(obj, LV_PART_SCROLLBAR);
        જો(dsc->shadow_width > 0) {
            dsc->shadow_spread = lv_obj_get_style_shadow_spread_internal(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_internal(obj, LV_PART_SCROLLBAR);
        }
        નહીંતર {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    જો(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    જો(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_internal(obj, LV_PART_SCROLLBAR);
        પાછા LV_RESULT_OK;
    }
    નહીંતર {
        પાછા LV_RESULT_INVALID;
    }
}

સ્થિર શૂન્ય lv_obj_event(અચલ lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    જો(code == LV_EVENT_PRESSED) {
        lv_obj_add_state(obj, LV_STATE_PRESSED);
    }
    નહીંતર જો(code == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        જો((indev == ખાલી || lv_indev_get_scroll_obj(indev) == ખાલી) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            જો(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_add_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            નહીંતર જો(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            જો(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_result_t res = lv_obj_send_event(obj, LV_EVENT_કિંમત_CHANGED, ખાલી);
                જો(res != LV_RESULT_OK) પાછા;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ખાલી);
                જો(res != LV_RESULT_OK) પાછા;
            }
        }
    }
    નહીંતર જો(code == LV_EVENT_કિંમત_CHANGED) {
        જો(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * parent = lv_obj_get_parent(obj);
            જો(parent) {
                uint32_t child_cnt = lv_obj_get_child_ગણતરી(parent);
                uint32_t i;
                માટે(i = 0; i < child_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_child(parent, i);
                    જો(obj == sibling) ચાલુ;

                    જો(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_result_t res = lv_obj_send_event(sibling, LV_EVENT_કિંમત_CHANGED, ખાલી);
                        જો(res != LV_RESULT_OK) પાછા;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, ખાલી);
                        જો(res != LV_RESULT_OK) પાછા;
                    }
                }
            }
        }
    }
    નહીંતર જો(code == LV_EVENT_PRESS_LOST) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    નહીંતર જો(code == LV_EVENT_STYLE_CHANGED) {
        uint32_t child_cnt = lv_obj_get_child_ગણતરી(obj);
        માટે(uint32_t i = 0; i < child_cnt; i++) {
            lv_obj_t * child = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(child);
        }
    }
    નહીંતર જો(code == LV_EVENT_ચાવી) {
        જો(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_ચાવી(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            જો(c == LV_ચાવી_જમણું || c == LV_ચાવી_UP) {
                lv_obj_add_state(obj, LV_STATE_CHECKED);
            }
            નહીંતર જો(c == LV_ચાવી_ડાબું || c == LV_ચાવી_DOWN) {
                /*Radio buttons can't be checked off directly*/
                જો(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            જો(c != LV_ચાવી_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_result_t res = lv_obj_send_event(obj, LV_EVENT_કિંમત_CHANGED, ખાલી);
                જો(res != LV_RESULT_OK) પાછા;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ખાલી);
                જો(res != LV_RESULT_OK) પાછા;
            }
        }
        નહીંતર જો(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_ડાબું(obj);
            int32_t sr = lv_obj_get_scroll_જમણું(obj);
            uint32_t c = lv_event_get_ચાવી(e);
            જો(c == LV_ચાવી_DOWN) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_height(obj) / 4, anim_enable);
            }
            નહીંતર જો(c == LV_ચાવી_UP) {
                lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_height(obj) / 4, anim_enable);
            }
            નહીંતર જો(c == LV_ચાવી_જમણું) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                જો(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_height(obj) / 4, anim_enable);
                નહીંતર
                    lv_obj_scroll_to_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_width(obj) / 4, anim_enable);
            }
            નહીંતર જો(c == LV_ચાવી_ડાબું) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                જો(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_height(obj) / 4, anim_enable);
                નહીંતર
                    lv_obj_scroll_to_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_width(obj) / 4, anim_enable);
            }
        }
    }
    નહીંતર જો(code == LV_EVENT_FOCUSED) {
        જો(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_to_view_recursive(obj, LV_ANIM_ON);
        }

        lv_group_t * group = lv_obj_get_group(obj);
        અચલ bool editing = group && lv_group_get_editing(group);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_active();
        જો(indev == ખાલી) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != ખાલી ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        જો(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_ચાવી;
        જો(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_add_state(obj, state);
        }
        નહીંતર {
            lv_obj_add_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    નહીંતર જો(code == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_add_state(obj, LV_STATE_SCROLLED);
    }
    નહીંતર જો(code == LV_EVENT_SCROLL_END) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        જો(lv_obj_get_scrollbar_mode(obj) == LV_SCROLLBAR_MODE_ACTIVE) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    નહીંતર જો(code == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_ચાવી);
    }
    નહીંતર જો(code == LV_EVENT_SIZE_CHANGED) {
        int32_t align = lv_obj_get_style_align_internal(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_internal(obj, LV_PART_MAIN);
        જો(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_ગણતરી(obj);
        માટે(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(child);
        }
    }
    નહીંતર જો(code == LV_EVENT_CHILD_CHANGED) {
        int32_t align = lv_obj_get_style_align_internal(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_internal(obj, LV_PART_MAIN);
        જો(layout || align || lv_obj_is_style_any_width_content(obj) || lv_obj_is_style_any_height_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    નહીંતર જો(code == LV_EVENT_CHILD_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    નહીંતર જો(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        int32_t d = lv_obj_calculate_ext_draw_size(obj, LV_PART_MAIN);
        lv_event_set_ext_draw_size(e, d);
    }
    નહીંતર જો(code == LV_EVENT_DRAW_MAIN || code == LV_EVENT_DRAW_POST || code == LV_EVENT_DRAW_MAIN_END ||
            code == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    નહીંતર જો(code == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    નહીંતર જો(code == LV_EVENT_HOVER_OVER) {
        lv_obj_add_state(obj, LV_STATE_HOVERED);
    }
    નહીંતર જો(code == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
સ્થિર શૂન્ય update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != ખાલી);

    જો(obj->state == new_state) પાછા;


    lv_state_t prev_state = obj->state;

    lv_part_t changed_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &changed_part);
    /*If there is no difference in styles there is nothing else to do*/
    જો(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
        પાછા;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_status(obj);

    /*Skip transitions if the widget is not rendered yet. */
    જો(!obj->rendered) {
        lv_obj_invalidate(obj);
        જો(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_size(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
        પાછા;
    }

    obj_transition_states(obj, prev_state, new_state);

    જો(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && changed_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    નહીંતર જો(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    નહીંતર જો(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    નહીંતર જો(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_size(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
}

સ્થિર શૂન્ય obj_transition_states(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(કદ(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    જો(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        પાછા;
    }
    uint32_t tsi = 0;
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        જો(state_act & (~new_state)) ચાલુ; /*Skip unrelated styles*/
        જો(obj_style->is_trans) ચાલુ;

        lv_style_કિંમત_t v;
        જો(lv_style_get_prop_internal(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_FOUND) ચાલુ;
        અચલ lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        માટે(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            માટે(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                જો(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) તોડો;
            }

            /*If not found  add it*/
            જો(t == tsi) {
                ts[tsi].time = tr->time;
                ts[tsi].delay = tr->delay;
                ts[tsi].path_cb = tr->path_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_data = tr->user_data;
                ts[tsi].selector = obj_style->selector;
                tsi++;
            }
        }
    }

    માટે(i = 0; i < tsi; i++) {
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
સ્થિર શૂન્ય lv_obj_children_add_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t child_ગણતરી = lv_obj_get_child_ગણતરી(obj);

    માટે(uint32_t i = 0; i < child_ગણતરી; i++) {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        જો(child) {
            lv_obj_add_state(child, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
સ્થિર શૂન્ય lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t child_ગણતરી = lv_obj_get_child_ગણતરી(obj);

    માટે(uint32_t i = 0; i < child_ગણતરી; i++) {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        જો(child) {
            lv_obj_remove_state(child, state);
        }
    }
}

સ્થિર શૂન્ય null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_data(e);
    *obj_ptr = ખાલી;
}

સ્થિર શૂન્ય screen_load_on_trigger_event_cb(lv_event_t * e)
{
    screen_load_anim_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_load_anim(dsc->target.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

સ્થિર શૂન્ય screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_load_anim_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->target.create_cb();
    lv_screen_load_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_add_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, ખાલી);
}

સ્થિર શૂન્ય play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t progress = lv_anim_timeline_get_progress(dsc->at);
    જો(dsc->reverse) {
        જો(progress == 0) {
            lv_anim_timeline_set_delay(dsc->at, dsc->delay);
            lv_anim_timeline_set_progress(dsc->at, LV_ANIM_TIMELINE_PROGRESS_MAX);
        }
    }
    નહીંતર {
        જો(progress == LV_ANIM_TIMELINE_PROGRESS_MAX) {
            lv_anim_timeline_set_delay(dsc->at, dsc->delay);
            lv_anim_timeline_set_progress(dsc->at, 0);
        }

    }
    lv_anim_timeline_set_reverse(dsc->at, dsc->reverse);
    lv_anim_timeline_start(dsc->at);
}


સ્થિર શૂન્ય delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_target_obj(e));
}

સ્થિર શૂન્ય call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ખાલી);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_data(e);
#જો !LV_USE_ASSERT
    LV_UNUSED(obj);
#endif
    LV_ASSERT(dsc != ખાલી);
    LV_ASSERT(dsc->cb != ખાલી);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_data);
    lv_obj_remove_delete_cb(dsc);
}

સ્થિર અંતઃરેખ શૂન્ય set_state(lv_obj_t * obj, lv_state_t state,  bool કિંમત)
{
    LV_ASSERT(obj != ખાલી);
    જો(કિંમત) lv_obj_add_state(obj, state);
    નહીંતર lv_obj_remove_state(obj, state);
}

સ્થિર અંતઃરેખ bool has_state(અચલ lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != ખાલી);

    પાછા !!(obj->state & state);
}


#જો LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
સ્થિર bool parent_contains_child(અચલ lv_obj_t * parent, અચલ lv_obj_t * child)
{
    uint32_t child_cnt = parent->spec_attr ? parent->spec_attr->child_cnt : 0;
    uint32_t i;
    માટે(i = 0; i < child_cnt; i++) {
        જો(parent->spec_attr->children[i] == child) પાછા true;
    }
    પાછા false;
}
#endif


#જો LV_USE_OBJ_PROPERTY
સ્થિર lv_point_t lv_obj_get_scroll_end_helper(lv_obj_t * obj)
{
    lv_point_t point;
    lv_obj_get_scroll_end(obj, &point);
    પાછા point;
}

સ્થિર શૂન્ય obj_set_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    જો(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_set_hidden(obj, v);
    જો(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_set_clickable(obj, v);
    જો(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_set_click_focusable(obj, v);
    જો(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_set_checkable(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_set_scrollable(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_set_scroll_elastic(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_set_scroll_momentum(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_set_scroll_one(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_set_scroll_chain_hor(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_set_scroll_chain_ver(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_set_scroll_on_focus(obj, v);
    જો(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_set_scroll_with_arrow(obj, v);
    જો(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_set_snappable(obj, v);
    જો(f & LV_OBJ_FLAG_PRESS_LOCK)            lv_obj_set_press_lock(obj, v);
    જો(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_set_event_bubble(obj, v);
    જો(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_set_gesture_bubble(obj, v);
    જો(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_set_adv_hittest(obj, v);
    જો(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_set_ignore_layout(obj, v);
    જો(f & LV_OBJ_FLAG_FLOATING)              lv_obj_set_floating(obj, v);
    જો(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_set_send_draw_task_events(obj, v);
    જો(f & LV_OBJ_FLAG_OVERFLOW_VISIBLE)      lv_obj_set_overflow_visible(obj, v);
    જો(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_set_event_trickle(obj, v);
    જો(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_set_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_bits = 0;
    જો(f & LV_OBJ_FLAG_LAYOUT_1)              user_bits |= 1 << 0;
    જો(f & LV_OBJ_FLAG_LAYOUT_2)              user_bits |= 1 << 1;
    જો(f & LV_OBJ_FLAG_WIDGET_1)              user_bits |= 1 << 2;
    જો(f & LV_OBJ_FLAG_WIDGET_2)              user_bits |= 1 << 3;
    જો(f & LV_OBJ_FLAG_USER_1)                user_bits |= 1 << 4;
    જો(f & LV_OBJ_FLAG_USER_2)                user_bits |= 1 << 5;
    જો(f & LV_OBJ_FLAG_USER_3)                user_bits |= 1 << 6;
    જો(f & LV_OBJ_FLAG_USER_4)                user_bits |= 1 << 7;
    જો(user_bits == 0) પાછા;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    જો(v) {
        obj->spec_attr->user_flags |= user_bits;
    }
    નહીંતર {
        obj->spec_attr->user_flags &= ~user_bits;
    }

    અચલ uint8_t layout_mask = 0x3;
    અચલ bool layout_flag_changed = (old_user_flags ^ obj->spec_attr->user_flags) & layout_mask;
    જો(layout_flag_changed) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

સ્થિર bool obj_has_flag_any(અચલ lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા false);

    જો((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) પાછા true;
    જો((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) પાછા true;
    જો((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) પાછા true;
    જો((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) પાછા true;
    જો((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) પાછા true;
    જો((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) પાછા true;
    જો((f & LV_OBJ_FLAG_PRESS_LOCK) && obj->press_lock) પાછા true;
    જો((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) પાછા true;
    જો((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) પાછા true;
    જો((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) પાછા true;
    જો((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) પાછા true;
    જો((f & LV_OBJ_FLAG_FLOATING) && obj->floating) પાછા true;
    જો((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) પાછા true;
    જો((f & LV_OBJ_FLAG_OVERFLOW_VISIBLE) && obj->overflow_visible) પાછા true;
    જો((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) પાછા true;
    જો((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) પાછા true;

    જો(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == ખાલી) પાછા false;

    જો((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) પાછા true;
    જો((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) પાછા true;
    જો((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) પાછા true;
    જો((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) પાછા true;
    જો((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) પાછા true;
    જો((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) પાછા true;
    જો((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) પાછા true;
    જો((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) પાછા true;

    પાછા false;
}

સ્થિર lv_result_t lv_obj_set_any(lv_obj_t * obj, lv_prop_id_t id, અચલ lv_property_t * prop)
{
    LV_ASSERT(obj != ખાલી);

    જો(id >= LV_PROPERTY_OBJ_FLAG_START && id <= LV_PROPERTY_OBJ_FLAG_END) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_START);
        obj_set_flag(obj, flag, prop->અંક != 0);
        પાછા LV_RESULT_OK;
    }
    નહીંતર જો(id >= LV_PROPERTY_OBJ_STATE_START && id <= LV_PROPERTY_OBJ_STATE_END) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_START);
        જો(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        જો(prop->અંક) lv_obj_add_state(obj, state);
        નહીંતર lv_obj_remove_state(obj, state);
        પાછા LV_RESULT_OK;
    }
    નહીંતર {
        પાછા LV_RESULT_INVALID;
    }
}

સ્થિર lv_result_t lv_obj_get_any(અચલ lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != ખાલી);
    જો(id >= LV_PROPERTY_OBJ_FLAG_START && id <= LV_PROPERTY_OBJ_FLAG_END) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_START);
        prop->id = id;
        prop->અંક = obj_has_flag_any(obj, flag);
        પાછા LV_RESULT_OK;
    }
    નહીંતર જો(id >= LV_PROPERTY_OBJ_STATE_START && id <= LV_PROPERTY_OBJ_STATE_END) {
        prop->id = id;
        જો(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->અંક = obj->state;
        }
        નહીંતર {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_START);
            prop->અંક = obj->state & flag;
        }
        પાછા LV_RESULT_OK;
    }
    નહીંતર {
        પાછા LV_RESULT_INVALID;
    }
}
#endif /*LV_USE_OBJ_PROPERTY*/

