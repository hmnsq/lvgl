/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_private.h"
#ضمن "../lvgl_public.h"
#ضمن "../misc/lv_event_private.h"
#ضمن "../misc/lv_style_private.h"
#ضمن "../misc/lv_area_private.h"
#ضمن "lv_obj_style_private.h"
#ضمن "lv_obj_event_private.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../indev/lv_indev_private.h"
#ضمن "../display/lv_display_private.h"
#ضمن "lv_obj_draw_private.h"
#ضمن "lv_obj_style_internal.h"

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

تعريفنوع بنية {
    lv_screen_load_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    اتحاد {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } target;
} screen_load_anim_dsc_t;

تعريفنوع بنية {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool reverse;
} timeline_play_dsc_t;

بنية _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    فراغ * user_data;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساكن فراغ lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساكن فراغ lv_obj_draw(lv_event_t * e);
ساكن فراغ lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e);
ساكن فراغ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
ساكن lv_result_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
ساكن فراغ obj_transition_states(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
ساكن فراغ update_obj_state(lv_obj_t * obj, lv_state_t new_state);
ساكن فراغ lv_obj_children_add_state(lv_obj_t * obj, lv_state_t state);
ساكن فراغ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
ساكن فراغ null_on_delete_cb(lv_event_t * e);
ساكن فراغ screen_load_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ screen_create_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ play_timeline_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ delete_on_screen_unloaded_event_cb(lv_event_t * e);
ساكن فراغ call_delete_cb(lv_event_t * e);

ساكن مضمن فراغ set_state(lv_obj_t * obj, lv_state_t state,  bool قيمة);
ساكن مضمن bool has_state(ثابت lv_obj_t * obj, lv_state_t state);
#إذا LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
    ساكن bool parent_contains_child(ثابت lv_obj_t * parent, ثابت lv_obj_t * child);
#endif

#إذا LV_USE_OBJ_PROPERTY
    ساكن lv_result_t lv_obj_set_any(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);
    ساكن lv_result_t lv_obj_get_any(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

    ساكن lv_point_t lv_obj_get_scroll_end_helper(lv_obj_t * obj);

    ساكن فراغ obj_set_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    ساكن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
#إذا LV_USE_OBJ_PROPERTY
ساكن ثابت lv_property_ops_t lv_obj_properties[] = {
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
        .id = LV_PROPERTY_OBJ_SCROLL_قمة,
        .getter = lv_obj_get_scroll_قمة,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_يسار,
        .getter = lv_obj_get_scroll_يسار,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_يمين,
        .getter = lv_obj_get_scroll_يمين,
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
        .id = LV_PROPERTY_OBJ_EVENT_تعداد,
        .getter = lv_obj_get_event_تعداد,
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
        .id = LV_PROPERTY_OBJ_CHILD_تعداد,
        .getter = lv_obj_get_child_تعداد,
    },
    {
        .id = LV_PROPERTY_OBJ_INDEX,
        .getter = lv_obj_get_index,
    },
#إذا LV_USE_OBJ_NAME
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

ثابت lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .width_def = LV_DPI_DEF,
    .height_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_FALSE,
    .instance_size = (حجم(lv_obj_t)),
    .base_class = عدم,
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

    إذا(obj == عدم) إرجاع عدم;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    إرجاع obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

فراغ lv_obj_set_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    إذا(obj->hidden == en) إرجاع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    إذا(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    إذا(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    إذا(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_group_t * group = lv_obj_get_group(obj);
        إذا(group != عدم) {
            lv_group_focus_next(group);
            lv_obj_t * next_obj = lv_group_get_focused(group);
            إذا(next_obj != عدم) {
                lv_obj_invalidate(next_obj);
            }
        }
    }

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_set_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->clickable = en;

}

فراغ lv_obj_set_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->click_focusable = en;
}

فراغ lv_obj_set_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->checkable = en;
}

فراغ lv_obj_set_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    إذا(obj->scrollable == en) إرجاع;

    /*Invalidate the scrollbars while scrollable*/
    إذا(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    إذا(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

فراغ lv_obj_set_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_elastic = en;
}

فراغ lv_obj_set_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_momentum = en;
}

فراغ lv_obj_set_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_one = en;
}

فراغ lv_obj_set_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

فراغ lv_obj_set_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_chain_hor = en;
}

فراغ lv_obj_set_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_chain_ver = en;
}

فراغ lv_obj_set_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_on_focus = en;
}

فراغ lv_obj_set_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->scroll_with_arrow = en;
}

فراغ lv_obj_set_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->snappable = en;
}

فراغ lv_obj_set_press_lock(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->press_lock = en;
}

فراغ lv_obj_set_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->event_bubble = en;
}

فراغ lv_obj_set_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->gesture_bubble = en;
}

فراغ lv_obj_set_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->adv_hittest = en;
}

فراغ lv_obj_set_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    إذا(obj->ignore_layout == en) إرجاع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_set_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    إذا(obj->floating == en) إرجاع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_set_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->send_draw_task_events = en;
}

فراغ lv_obj_set_overflow_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->overflow_visible = en;
}

فراغ lv_obj_set_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->event_trickle = en;
}

فراغ lv_obj_set_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->state_trickle = en;
}

فراغ lv_obj_set_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->radio_button = en;
}

فراغ lv_obj_set_flex_in_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    إذا(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    وإلا {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        إذا(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

فراغ lv_obj_add_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    lv_state_t new_state = obj->state | state;
    إذا(obj->state != new_state) {
        update_obj_state(obj, new_state);
        إذا(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_add_state(obj, state);
        }
    }
}

فراغ lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    lv_state_t new_state = obj->state & (~state);
    إذا(obj->state != new_state) {
        update_obj_state(obj, new_state);
        إذا(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

فراغ lv_obj_set_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, state, v);
}

فراغ lv_obj_set_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_ALT, en);
}

فراغ lv_obj_set_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_CHECKED, en);
}

فراغ lv_obj_set_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_FOCUSED, en);
}

فراغ lv_obj_set_focus_مفتاح(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_FOCUS_مفتاح, en);
}

فراغ lv_obj_set_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_EDITED, en);
}

فراغ lv_obj_set_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_HOVERED, en);
}

فراغ lv_obj_set_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_PRESSED, en);
}

فراغ lv_obj_set_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_SCROLLED, en);
}

فراغ lv_obj_set_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_DISABLED, en);
}

فراغ lv_obj_set_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_USER_1, en);
}

فراغ lv_obj_set_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_USER_2, en);
}

فراغ lv_obj_set_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_USER_3, en);
}

فراغ lv_obj_set_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    set_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->hidden;
}

bool lv_obj_is_clickable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->clickable;
}

bool lv_obj_is_click_focusable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->click_focusable;
}

bool lv_obj_is_checkable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->checkable;
}

bool lv_obj_is_scrollable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scrollable;
}

bool lv_obj_is_scroll_elastic(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->snappable;
}

bool lv_obj_is_press_lock(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->press_lock;
}

bool lv_obj_is_event_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->ignore_layout;
}

bool lv_obj_is_floating(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->floating;
}

bool lv_obj_is_send_draw_task_events(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->send_draw_task_events;
}

bool lv_obj_is_overflow_visible(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->overflow_visible;
}

bool lv_obj_is_event_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->event_trickle;
}

bool lv_obj_is_state_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->state_trickle;
}

bool lv_obj_is_radio_button(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع obj->radio_button;
}

bool lv_obj_is_flex_in_new_track(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    إذا(obj->spec_attr == عدم) إرجاع false;

    إرجاع obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع 0);

    إرجاع obj->state;
}

bool lv_obj_has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, state);
}

bool lv_obj_is_alt(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_مفتاح(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_FOCUS_مفتاح);
}

bool lv_obj_is_edited(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    إرجاع has_state(obj, LV_STATE_USER_4);
}

lv_group_t * lv_obj_get_group(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع عدم);

    إذا(obj->spec_attr) إرجاع obj->spec_attr->group_p;
    وإلا إرجاع عدم;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_result_t lv_obj_add_child(lv_obj_t * parent, lv_obj_t * child)
{
    LV_CHECK_OBJ(parent, MY_CLASS, إرجاع LV_RESULT_INVALID);
    LV_CHECK_ARG(child != عدم, إرجاع LV_RESULT_INVALID);

    uint16_t new_child_cnt = parent->spec_attr->child_cnt + 1;

    lv_obj_t ** children = lv_realloc(parent->spec_attr->children,
                                      new_child_cnt * (حجم(lv_obj_t *)));
    إذا(!children) {
        إرجاع LV_RESULT_INVALID;
    }
    children[new_child_cnt - 1] = child;

    parent->spec_attr->child_cnt = new_child_cnt;
    parent->spec_attr->children = children;
    إرجاع LV_RESULT_OK;
}

فراغ lv_obj_remove_child(lv_obj_t * parent, lv_obj_t * child)
{
    LV_CHECK_OBJ(parent, MY_CLASS, إرجاع);
    LV_CHECK_OBJ(child, MY_CLASS, إرجاع);

    لكل(int32_t i = lv_obj_get_index(child); i < (int32_t)parent->spec_attr->child_cnt - 1; i++) {
        parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
    }

    /* No more children*/
    إذا(parent->spec_attr->child_cnt == 1) {
        lv_free(parent->spec_attr->children);
        parent->spec_attr->children = عدم;
        parent->spec_attr->child_cnt = 0;
        إرجاع;
    }
    parent->spec_attr->child_cnt--;
    lv_obj_t ** new_children = lv_realloc(parent->spec_attr->children, parent->spec_attr->child_cnt * (حجم(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    إذا(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        إرجاع;
    }
    parent->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع عدم);

    إذا(obj->spec_attr) {
        إرجاع obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(حجم(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    إذا(!spec_attr) {
        إرجاع عدم;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_mode = LV_SCROLLBAR_MODE_AUTO;
    obj->spec_attr = spec_attr;
    إرجاع spec_attr;
}


bool lv_obj_check_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != عدم, إرجاع false);
    LV_CHECK_ARG(class_p != عدم, إرجاع false);
    إرجاع obj->class_p == class_p;
}

bool lv_obj_has_class(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != عدم, إرجاع false);
    LV_CHECK_ARG(class_p != عدم, إرجاع false);

    ثابت lv_obj_class_t * obj_class = obj->class_p;
    بينما(obj_class) {
        إذا(obj_class == class_p) إرجاع true;
        obj_class = obj_class->base_class;
    }

    إرجاع false;
}

ثابت lv_obj_class_t * lv_obj_get_class(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, إرجاع عدم);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    إرجاع obj->class_p;
}

bool lv_obj_is_in_widget_شجرة(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, إرجاع false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    ثابت lv_obj_t * current = obj;

    بينما(current->parent != عدم) {
#إذا LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
        LV_ASSERT(parent_contains_child(current->parent, current));
#endif
        current = current->parent;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_next(عدم);
    بينما(disp) {
        uint32_t i;
        لكل(i = 0; i < disp->screen_cnt; i++) {
            إذا(disp->screens[i] == current) إرجاع true;
        }
        disp = lv_display_get_next(disp);
    }

    إرجاع false;
}

فراغ lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != عدم, إرجاع);
    LV_CHECK_ARG(*obj_ptr != عدم, إرجاع);

    lv_obj_add_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#إذا LV_USE_OBJ_ID
فراغ * lv_obj_get_id(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, إرجاع عدم);
    إرجاع obj->id;
}

#endif

فراغ lv_obj_add_screen_load_event(lv_obj_t * obj, lv_event_code_t trigger, lv_obj_t * screen,
                                  lv_screen_load_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    LV_CHECK_ARG_رسالة(screen != عدم, إرجاع, "can't load a non-existing screen");

    screen_load_anim_dsc_t * dsc = lv_malloc(حجم(screen_load_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_load_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->target.screen = screen;

    lv_obj_add_event_cb(obj, screen_load_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_add_screen_create_event(lv_obj_t * obj, lv_event_code_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_load_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    LV_CHECK_ARG(screen_create_cb != عدم, إرجاع);

    screen_load_anim_dsc_t * dsc = lv_malloc(حجم(screen_load_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_load_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->target.create_cb = screen_create_cb;

    lv_obj_add_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_add_play_timeline_event(lv_obj_t * obj, lv_event_code_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool reverse)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    LV_CHECK_ARG(at != عدم, إرجاع);

    timeline_play_dsc_t * dsc = lv_malloc(حجم(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->reverse = reverse;

    lv_obj_add_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_add_event_cb(obj, lv_event_free_user_data_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_set_user_data(lv_obj_t * obj, فراغ * user_data)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    obj->user_data = user_data;
}

فراغ lv_obj_set_user_flag(lv_obj_t * obj, uint32_t bit, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);
    LV_CHECK_ARG(bit < 4, إرجاع);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    إذا(spec_attr == عدم) إرجاع;

    إذا(v) spec_attr->user_flags |= 1 << (bit + 4);
    وإلا spec_attr->user_flags &= ~(1 << (bit + 4));
}

فراغ * lv_obj_get_user_data(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع عدم);

    إرجاع obj->user_data;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t bit)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);
    LV_CHECK_ARG(bit < 4, إرجاع false);

    إذا(obj->spec_attr == عدم) إرجاع false;
    وإلا إرجاع (obj->spec_attr->user_flags & 1 << (bit + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_add_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, فراغ * user_data)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع عدم);
    LV_CHECK_ARG(cb != عدم, إرجاع عدم);

    lv_delete_dsc_t * dsc = lv_malloc(حجم(*dsc));
    إذا(!dsc) {
        إرجاع عدم;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_data = user_data;

    lv_event_dsc_t * event_dsc = lv_obj_add_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    إذا(!event_dsc) {
        lv_free(dsc);
        إرجاع عدم;
    }
    إرجاع dsc;
}

فراغ lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    إذا(!dsc) {
        إرجاع;
    }

    uint32_t تعداد_2 = lv_obj_remove_event_cb_with_user_data(dsc->obj, call_delete_cb, dsc);
    إذا(تعداد_2 == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        إرجاع;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * parent = obj->parent;
    إذا(parent) {
        int32_t sl = lv_obj_get_scroll_يسار(parent);
        int32_t st = lv_obj_get_scroll_قمة(parent);

        obj->coords.y1 = parent->coords.y1 + lv_obj_get_style_pad_قمة_internal(parent, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = parent->coords.x1 + lv_obj_get_style_pad_يسار_internal(parent, LV_PART_MAIN) - sl;
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
    إذا(parent) {
        obj->press_lock = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#إذا LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ASSIGN
    lv_obj_assign_id(class_p, obj);
#endif

    LV_TRACE_OBJ_CREATE("finished");
}

ساكن فراغ lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    LV_UNUSED(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, عدم);

    /*Delete from the group*/
    lv_group_t * group = lv_obj_get_group(obj);
    إذا(group) lv_group_remove_obj(obj);

    إذا(obj->spec_attr) {
        إذا(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = عدم;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#إذا LV_USE_OBJ_NAME
        إذا(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((فراغ *)obj->spec_attr->name);
        }
#endif

#إذا LV_DRAW_TRANSFORM_USE_MATRIX
        إذا(obj->spec_attr->matrix) {
            lv_free(obj->spec_attr->matrix);
            obj->spec_attr->matrix = عدم;
        }
#endif

        lv_free(obj->spec_attr);
        obj->spec_attr = عدم;
    }

#إذا LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_ASSIGN
    lv_obj_free_id(obj);
#endif
}

ساكن فراغ lv_obj_draw(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    إذا(code == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        إذا(info->res == LV_COVER_RES_MASKED) إرجاع;
        إذا(lv_obj_get_style_clip_corner_internal(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            إرجاع;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_internal(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_width_internal(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_height_internal(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        إذا(lv_area_is_in(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            إرجاع;
        }

        إذا(lv_obj_get_style_bg_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            إرجاع;
        }

        إذا(lv_obj_get_style_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            إرجاع;
        }

        إذا(lv_obj_get_style_bg_grad_dir_internal(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            إذا(lv_obj_get_style_bg_grad_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                إرجاع;
            }
        }
        ثابت lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_internal(obj, LV_PART_MAIN);
        إذا(grad_dsc) {
            uint32_t i;
            لكل(i = 0; i < grad_dsc->stops_تعداد; i++) {
                إذا(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    إرجاع;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    وإلا إذا(code == LV_EVENT_DRAW_MAIN) {
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
        إذا(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.corner_radius = draw_dsc.radius;
            blur_dsc.base.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        إذا(lv_obj_get_style_border_post_internal(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    وإلا إذا(code == LV_EVENT_DRAW_MAIN_END) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_internal(obj, LV_PART_MAIN);
        إذا(!backdrop_blur) {
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
    وإلا إذا(code == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        إذا(lv_obj_get_style_border_width_internal(obj, LV_PART_MAIN) &&
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

ساكن فراغ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    إذا(lv_area_get_size(&hor_area) <= 0 && lv_area_get_size(&ver_area) <= 0) إرجاع;

    lv_draw_rect_dsc_t rect_dsc;
    lv_result_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    إذا(sb_res != LV_RESULT_OK) إرجاع;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_internal(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.corner_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_internal(obj, LV_PART_SCROLLBAR);

    إذا(lv_area_get_size(&hor_area) > 0) {
        إذا(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.base.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.base.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        إذا(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    إذا(lv_area_get_size(&ver_area) > 0) {
        blur_dsc.base.id1 = 1;
        إذا(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.base.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        إذا(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
ساكن lv_result_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR);
    إذا(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_internal(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR);
    إذا(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_width = lv_obj_get_style_border_width_internal(obj, LV_PART_SCROLLBAR);
        إذا(dsc->border_width > 0) {
            dsc->border_color = lv_obj_get_style_border_color_internal(obj, LV_PART_SCROLLBAR);
        }
        وإلا {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_internal(obj, LV_PART_SCROLLBAR);
    إذا(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_width = lv_obj_get_style_shadow_width_internal(obj, LV_PART_SCROLLBAR);
        إذا(dsc->shadow_width > 0) {
            dsc->shadow_spread = lv_obj_get_style_shadow_spread_internal(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_internal(obj, LV_PART_SCROLLBAR);
        }
        وإلا {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    إذا(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    إذا(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_internal(obj, LV_PART_SCROLLBAR);
        إرجاع LV_RESULT_OK;
    }
    وإلا {
        إرجاع LV_RESULT_INVALID;
    }
}

ساكن فراغ lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    إذا(code == LV_EVENT_PRESSED) {
        lv_obj_add_state(obj, LV_STATE_PRESSED);
    }
    وإلا إذا(code == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        إذا((indev == عدم || lv_indev_get_scroll_obj(indev) == عدم) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            إذا(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_add_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            وإلا إذا(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            إذا(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_result_t res = lv_obj_send_event(obj, LV_EVENT_قيمة_CHANGED, عدم);
                إذا(res != LV_RESULT_OK) إرجاع;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, عدم);
                إذا(res != LV_RESULT_OK) إرجاع;
            }
        }
    }
    وإلا إذا(code == LV_EVENT_قيمة_CHANGED) {
        إذا(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * parent = lv_obj_get_parent(obj);
            إذا(parent) {
                uint32_t child_cnt = lv_obj_get_child_تعداد(parent);
                uint32_t i;
                لكل(i = 0; i < child_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_child(parent, i);
                    إذا(obj == sibling) استمر;

                    إذا(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_result_t res = lv_obj_send_event(sibling, LV_EVENT_قيمة_CHANGED, عدم);
                        إذا(res != LV_RESULT_OK) إرجاع;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, عدم);
                        إذا(res != LV_RESULT_OK) إرجاع;
                    }
                }
            }
        }
    }
    وإلا إذا(code == LV_EVENT_PRESS_LOST) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    وإلا إذا(code == LV_EVENT_STYLE_CHANGED) {
        uint32_t child_cnt = lv_obj_get_child_تعداد(obj);
        لكل(uint32_t i = 0; i < child_cnt; i++) {
            lv_obj_t * child = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(child);
        }
    }
    وإلا إذا(code == LV_EVENT_مفتاح) {
        إذا(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_مفتاح(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            إذا(c == LV_مفتاح_يمين || c == LV_مفتاح_UP) {
                lv_obj_add_state(obj, LV_STATE_CHECKED);
            }
            وإلا إذا(c == LV_مفتاح_يسار || c == LV_مفتاح_DOWN) {
                /*Radio buttons can't be checked off directly*/
                إذا(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            إذا(c != LV_مفتاح_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_result_t res = lv_obj_send_event(obj, LV_EVENT_قيمة_CHANGED, عدم);
                إذا(res != LV_RESULT_OK) إرجاع;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, عدم);
                إذا(res != LV_RESULT_OK) إرجاع;
            }
        }
        وإلا إذا(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_يسار(obj);
            int32_t sr = lv_obj_get_scroll_يمين(obj);
            uint32_t c = lv_event_get_مفتاح(e);
            إذا(c == LV_مفتاح_DOWN) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_height(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_مفتاح_UP) {
                lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_height(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_مفتاح_يمين) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                إذا(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_height(obj) / 4, anim_enable);
                وإلا
                    lv_obj_scroll_to_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_width(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_مفتاح_يسار) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                إذا(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_to_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_height(obj) / 4, anim_enable);
                وإلا
                    lv_obj_scroll_to_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_width(obj) / 4, anim_enable);
            }
        }
    }
    وإلا إذا(code == LV_EVENT_FOCUSED) {
        إذا(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_to_view_recursive(obj, LV_ANIM_ON);
        }

        lv_group_t * group = lv_obj_get_group(obj);
        ثابت bool editing = group && lv_group_get_editing(group);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_active();
        إذا(indev == عدم) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != عدم ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        إذا(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_مفتاح;
        إذا(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_add_state(obj, state);
        }
        وإلا {
            lv_obj_add_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    وإلا إذا(code == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_add_state(obj, LV_STATE_SCROLLED);
    }
    وإلا إذا(code == LV_EVENT_SCROLL_END) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        إذا(lv_obj_get_scrollbar_mode(obj) == LV_SCROLLBAR_MODE_ACTIVE) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    وإلا إذا(code == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_مفتاح);
    }
    وإلا إذا(code == LV_EVENT_SIZE_CHANGED) {
        int32_t align = lv_obj_get_style_align_internal(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_internal(obj, LV_PART_MAIN);
        إذا(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_تعداد(obj);
        لكل(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(child);
        }
    }
    وإلا إذا(code == LV_EVENT_CHILD_CHANGED) {
        int32_t align = lv_obj_get_style_align_internal(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_internal(obj, LV_PART_MAIN);
        إذا(layout || align || lv_obj_is_style_any_width_content(obj) || lv_obj_is_style_any_height_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    وإلا إذا(code == LV_EVENT_CHILD_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    وإلا إذا(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        int32_t d = lv_obj_calculate_ext_draw_size(obj, LV_PART_MAIN);
        lv_event_set_ext_draw_size(e, d);
    }
    وإلا إذا(code == LV_EVENT_DRAW_MAIN || code == LV_EVENT_DRAW_POST || code == LV_EVENT_DRAW_MAIN_END ||
            code == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    وإلا إذا(code == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    وإلا إذا(code == LV_EVENT_HOVER_OVER) {
        lv_obj_add_state(obj, LV_STATE_HOVERED);
    }
    وإلا إذا(code == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
ساكن فراغ update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != عدم);

    إذا(obj->state == new_state) إرجاع;


    lv_state_t prev_state = obj->state;

    lv_part_t changed_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &changed_part);
    /*If there is no difference in styles there is nothing else to do*/
    إذا(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
        إرجاع;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_status(obj);

    /*Skip transitions if the widget is not rendered yet. */
    إذا(!obj->rendered) {
        lv_obj_invalidate(obj);
        إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_size(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
        إرجاع;
    }

    obj_transition_states(obj, prev_state, new_state);

    إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && changed_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    وإلا إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    وإلا إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    وإلا إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_size(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_CHANGED, &prev_state);
}

ساكن فراغ obj_transition_states(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(حجم(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    إذا(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        إرجاع;
    }
    uint32_t tsi = 0;
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        إذا(state_act & (~new_state)) استمر; /*Skip unrelated styles*/
        إذا(obj_style->is_trans) استمر;

        lv_style_قيمة_t v;
        إذا(lv_style_get_prop_internal(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_FOUND) استمر;
        ثابت lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        لكل(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            لكل(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                إذا(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) اكسر;
            }

            /*If not found  add it*/
            إذا(t == tsi) {
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

    لكل(i = 0; i < tsi; i++) {
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
ساكن فراغ lv_obj_children_add_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t child_تعداد = lv_obj_get_child_تعداد(obj);

    لكل(uint32_t i = 0; i < child_تعداد; i++) {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        إذا(child) {
            lv_obj_add_state(child, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
ساكن فراغ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t child_تعداد = lv_obj_get_child_تعداد(obj);

    لكل(uint32_t i = 0; i < child_تعداد; i++) {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        إذا(child) {
            lv_obj_remove_state(child, state);
        }
    }
}

ساكن فراغ null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_data(e);
    *obj_ptr = عدم;
}

ساكن فراغ screen_load_on_trigger_event_cb(lv_event_t * e)
{
    screen_load_anim_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_load_anim(dsc->target.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

ساكن فراغ screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_load_anim_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->target.create_cb();
    lv_screen_load_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_add_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, عدم);
}

ساكن فراغ play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_data(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t progress = lv_anim_timeline_get_progress(dsc->at);
    إذا(dsc->reverse) {
        إذا(progress == 0) {
            lv_anim_timeline_set_delay(dsc->at, dsc->delay);
            lv_anim_timeline_set_progress(dsc->at, LV_ANIM_TIMELINE_PROGRESS_MAX);
        }
    }
    وإلا {
        إذا(progress == LV_ANIM_TIMELINE_PROGRESS_MAX) {
            lv_anim_timeline_set_delay(dsc->at, dsc->delay);
            lv_anim_timeline_set_progress(dsc->at, 0);
        }

    }
    lv_anim_timeline_set_reverse(dsc->at, dsc->reverse);
    lv_anim_timeline_start(dsc->at);
}


ساكن فراغ delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_target_obj(e));
}

ساكن فراغ call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_data(e);
#إذا !LV_USE_ASSERT
    LV_UNUSED(obj);
#endif
    LV_ASSERT(dsc != عدم);
    LV_ASSERT(dsc->cb != عدم);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_data);
    lv_obj_remove_delete_cb(dsc);
}

ساكن مضمن فراغ set_state(lv_obj_t * obj, lv_state_t state,  bool قيمة)
{
    LV_ASSERT(obj != عدم);
    إذا(قيمة) lv_obj_add_state(obj, state);
    وإلا lv_obj_remove_state(obj, state);
}

ساكن مضمن bool has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != عدم);

    إرجاع !!(obj->state & state);
}


#إذا LV_USE_CHECK_OBJ_PARENT_LINK && LV_USE_ASSERT
ساكن bool parent_contains_child(ثابت lv_obj_t * parent, ثابت lv_obj_t * child)
{
    uint32_t child_cnt = parent->spec_attr ? parent->spec_attr->child_cnt : 0;
    uint32_t i;
    لكل(i = 0; i < child_cnt; i++) {
        إذا(parent->spec_attr->children[i] == child) إرجاع true;
    }
    إرجاع false;
}
#endif


#إذا LV_USE_OBJ_PROPERTY
ساكن lv_point_t lv_obj_get_scroll_end_helper(lv_obj_t * obj)
{
    lv_point_t point;
    lv_obj_get_scroll_end(obj, &point);
    إرجاع point;
}

ساكن فراغ obj_set_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع);

    إذا(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    إذا(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_set_hidden(obj, v);
    إذا(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_set_clickable(obj, v);
    إذا(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_set_click_focusable(obj, v);
    إذا(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_set_checkable(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_set_scrollable(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_set_scroll_elastic(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_set_scroll_momentum(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_set_scroll_one(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_set_scroll_chain_hor(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_set_scroll_chain_ver(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_set_scroll_on_focus(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_set_scroll_with_arrow(obj, v);
    إذا(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_set_snappable(obj, v);
    إذا(f & LV_OBJ_FLAG_PRESS_LOCK)            lv_obj_set_press_lock(obj, v);
    إذا(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_set_event_bubble(obj, v);
    إذا(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_set_gesture_bubble(obj, v);
    إذا(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_set_adv_hittest(obj, v);
    إذا(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_set_ignore_layout(obj, v);
    إذا(f & LV_OBJ_FLAG_FLOATING)              lv_obj_set_floating(obj, v);
    إذا(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_set_send_draw_task_events(obj, v);
    إذا(f & LV_OBJ_FLAG_OVERFLOW_VISIBLE)      lv_obj_set_overflow_visible(obj, v);
    إذا(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_set_event_trickle(obj, v);
    إذا(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_set_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_bits = 0;
    إذا(f & LV_OBJ_FLAG_LAYOUT_1)              user_bits |= 1 << 0;
    إذا(f & LV_OBJ_FLAG_LAYOUT_2)              user_bits |= 1 << 1;
    إذا(f & LV_OBJ_FLAG_WIDGET_1)              user_bits |= 1 << 2;
    إذا(f & LV_OBJ_FLAG_WIDGET_2)              user_bits |= 1 << 3;
    إذا(f & LV_OBJ_FLAG_USER_1)                user_bits |= 1 << 4;
    إذا(f & LV_OBJ_FLAG_USER_2)                user_bits |= 1 << 5;
    إذا(f & LV_OBJ_FLAG_USER_3)                user_bits |= 1 << 6;
    إذا(f & LV_OBJ_FLAG_USER_4)                user_bits |= 1 << 7;
    إذا(user_bits == 0) إرجاع;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    إذا(v) {
        obj->spec_attr->user_flags |= user_bits;
    }
    وإلا {
        obj->spec_attr->user_flags &= ~user_bits;
    }

    ثابت uint8_t layout_mask = 0x3;
    ثابت bool layout_flag_changed = (old_user_flags ^ obj->spec_attr->user_flags) & layout_mask;
    إذا(layout_flag_changed) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_parent(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ساكن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, إرجاع false);

    إذا((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) إرجاع true;
    إذا((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) إرجاع true;
    إذا((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) إرجاع true;
    إذا((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) إرجاع true;
    إذا((f & LV_OBJ_FLAG_PRESS_LOCK) && obj->press_lock) إرجاع true;
    إذا((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) إرجاع true;
    إذا((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) إرجاع true;
    إذا((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) إرجاع true;
    إذا((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) إرجاع true;
    إذا((f & LV_OBJ_FLAG_FLOATING) && obj->floating) إرجاع true;
    إذا((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) إرجاع true;
    إذا((f & LV_OBJ_FLAG_OVERFLOW_VISIBLE) && obj->overflow_visible) إرجاع true;
    إذا((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) إرجاع true;
    إذا((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) إرجاع true;

    إذا(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == عدم) إرجاع false;

    إذا((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) إرجاع true;
    إذا((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) إرجاع true;

    إرجاع false;
}

ساكن lv_result_t lv_obj_set_any(lv_obj_t * obj, lv_prop_id_t id, ثابت lv_property_t * prop)
{
    LV_ASSERT(obj != عدم);

    إذا(id >= LV_PROPERTY_OBJ_FLAG_START && id <= LV_PROPERTY_OBJ_FLAG_END) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_START);
        obj_set_flag(obj, flag, prop->رقم != 0);
        إرجاع LV_RESULT_OK;
    }
    وإلا إذا(id >= LV_PROPERTY_OBJ_STATE_START && id <= LV_PROPERTY_OBJ_STATE_END) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_START);
        إذا(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        إذا(prop->رقم) lv_obj_add_state(obj, state);
        وإلا lv_obj_remove_state(obj, state);
        إرجاع LV_RESULT_OK;
    }
    وإلا {
        إرجاع LV_RESULT_INVALID;
    }
}

ساكن lv_result_t lv_obj_get_any(ثابت lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != عدم);
    إذا(id >= LV_PROPERTY_OBJ_FLAG_START && id <= LV_PROPERTY_OBJ_FLAG_END) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_START);
        prop->id = id;
        prop->رقم = obj_has_flag_any(obj, flag);
        إرجاع LV_RESULT_OK;
    }
    وإلا إذا(id >= LV_PROPERTY_OBJ_STATE_START && id <= LV_PROPERTY_OBJ_STATE_END) {
        prop->id = id;
        إذا(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->رقم = obj->state;
        }
        وإلا {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_START);
            prop->رقم = obj->state & flag;
        }
        إرجاع LV_RESULT_OK;
    }
    وإلا {
        إرجاع LV_RESULT_INVALID;
    }
}
#endif /*LV_USE_OBJ_PROPERTY*/

