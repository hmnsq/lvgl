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
#تعریف_کرو MY_CLASS (&lv_obj_class)
#تعریف_کرو LV_OBJ_DEF_چوڑائی    (LV_DPX(100))
#تعریف_کرو LV_OBJ_DEF_اچائی   (LV_DPX(50))
#تعریف_کرو STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع بنية {
    lv_screen_لدو_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    اتحاد {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } نشانہ;
} screen_لدو_anim_dsc_t;

تعريفنوع بنية {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool الٹ;
} timeline_play_dsc_t;

بنية _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    فراغ * user_انکڑے;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساكن فراغ lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساكن فراغ lv_obj_draw(lv_event_t * e);
ساكن فراغ lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e);
ساكن فراغ draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
ساكن lv_نتیجہ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
ساكن فراغ obj_transition_حالتاں(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
ساكن فراغ update_obj_state(lv_obj_t * obj, lv_state_t new_state);
ساكن فراغ lv_obj_children_جوڑو_state(lv_obj_t * obj, lv_state_t state);
ساكن فراغ lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
ساكن فراغ null_on_delete_cb(lv_event_t * e);
ساكن فراغ screen_لدو_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ screen_create_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ play_timeline_on_trigger_event_cb(lv_event_t * e);
ساكن فراغ delete_on_screen_unloaded_event_cb(lv_event_t * e);
ساكن فراغ call_delete_cb(lv_event_t * e);

ساكن مضمن فراغ سیٹ_state(lv_obj_t * obj, lv_state_t state,  bool قدر);
ساكن مضمن bool has_state(ثابت lv_obj_t * obj, lv_state_t state);
#إذا LV_USE_CHECK_OBJ_ماپا_LINK && LV_USE_ASSERT
    ساكن bool ماپا_شامل_اے_بچہ(ثابت lv_obj_t * ماپا, ثابت lv_obj_t * بچہ);
#جے_انت

#إذا LV_USE_OBJ_PROPERTY
    ساكن lv_نتیجہ_t lv_obj_سیٹ_any(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);
    ساكن lv_نتیجہ_t lv_obj_get_any(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

    ساكن lv_نقطہ_t lv_obj_get_scroll_سرا_helper(lv_obj_t * obj);

    ساكن فراغ obj_سیٹ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    ساكن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f);
#جے_انت

/**********************
 *  STATIC VARIABLES
 **********************/
#إذا LV_USE_OBJ_PROPERTY
ساكن ثابت lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_ماپا,
        .setter = lv_obj_سیٹ_ماپا,
        .getter = lv_obj_get_ماپا,
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
        .setter = lv_obj_سیٹ_اچائی,
        .getter = lv_obj_get_اچائی,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_چوڑائی,
        .setter = lv_obj_سیٹ_content_چوڑائی,
        .getter = lv_obj_get_content_چوڑائی,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_اچائی,
        .setter = lv_obj_سیٹ_content_اچائی,
        .getter = lv_obj_get_content_اچائی,
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
        .id = LV_PROPERTY_OBJ_SCROLL_سرا,
        .getter = lv_obj_get_scroll_سرا_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_ناپ,
        .getter = lv_obj_get_ext_draw_ناپ,
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
#إذا LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_سیٹ_name,
    },
#جے_انت
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
        .id = LV_PROPERTY_OBJ_PRESS_جندرا,
        .setter = lv_obj_سیٹ_press_جندرا,
        .getter = lv_obj_is_press_جندرا,
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
        .id = LV_PROPERTY_OBJ_حد_توں_ودھ_VISIBLE,
        .setter = lv_obj_سیٹ_حد_توں_ودھ_visible,
        .getter = lv_obj_is_حد_توں_ودھ_visible,
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
        .id = LV_PROPERTY_OBJ_FLEX_وچ_NEW_TRACK,
        .setter = lv_obj_سیٹ_flex_وچ_new_track,
        .getter = lv_obj_is_flex_وچ_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_سیٹ_any,
        .getter = lv_obj_get_any,
    },
};
#جے_انت

ثابت lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .چوڑائی_def = LV_DPI_DEF,
    .اچائی_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .گروہ_def = LV_OBJ_CLASS_گروہ_DEF_FALSE,
    .instance_ناپ = (حجم(lv_obj_t)),
    .ادھار_class = عدم,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * ماپا)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, ماپا);

    إذا(obj == عدم) أرجع عدم;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    أرجع obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

فراغ lv_obj_سیٹ_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    إذا(obj->hidden == en) أرجع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    إذا(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    إذا(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    إذا(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
        إذا(گروہ != عدم) {
            lv_گروہ_focus_اگلا(گروہ);
            lv_obj_t * اگلا_obj = lv_گروہ_get_focused(گروہ);
            إذا(اگلا_obj != عدم) {
                lv_obj_invalidate(اگلا_obj);
            }
        }
    }

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ماپا(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_سیٹ_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->clickable = en;

}

فراغ lv_obj_سیٹ_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->click_focusable = en;
}

فراغ lv_obj_سیٹ_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->checkable = en;
}

فراغ lv_obj_سیٹ_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(obj->scrollable == en) أرجع;

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

فراغ lv_obj_سیٹ_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_elastic = en;
}

فراغ lv_obj_سیٹ_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_momentum = en;
}

فراغ lv_obj_سیٹ_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_one = en;
}

فراغ lv_obj_سیٹ_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

فراغ lv_obj_سیٹ_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_chain_hor = en;
}

فراغ lv_obj_سیٹ_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_chain_ver = en;
}

فراغ lv_obj_سیٹ_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_on_focus = en;
}

فراغ lv_obj_سیٹ_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->scroll_with_arrow = en;
}

فراغ lv_obj_سیٹ_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->snappable = en;
}

فراغ lv_obj_سیٹ_press_جندرا(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->press_جندرا = en;
}

فراغ lv_obj_سیٹ_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->event_bubble = en;
}

فراغ lv_obj_سیٹ_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->gesture_bubble = en;
}

فراغ lv_obj_سیٹ_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->adv_hittest = en;
}

فراغ lv_obj_سیٹ_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(obj->ignore_layout == en) أرجع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ماپا(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_سیٹ_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(obj->floating == en) أرجع;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    إذا((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ماپا(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

فراغ lv_obj_سیٹ_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->send_draw_task_events = en;
}

فراغ lv_obj_سیٹ_حد_توں_ودھ_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->حد_توں_ودھ_visible = en;
}

فراغ lv_obj_سیٹ_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->event_trickle = en;
}

فراغ lv_obj_سیٹ_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->state_trickle = en;
}

فراغ lv_obj_سیٹ_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->radio_button = en;
}

فراغ lv_obj_سیٹ_flex_وچ_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

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

    lv_obj_mark_layout_as_dirty(lv_obj_get_ماپا(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

فراغ lv_obj_جوڑو_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_state_t new_state = obj->state | state;
    إذا(obj->state != new_state) {
        update_obj_state(obj, new_state);
        إذا(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_جوڑو_state(obj, state);
        }
    }
}

فراغ lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_state_t new_state = obj->state & (~state);
    إذا(obj->state != new_state) {
        update_obj_state(obj, new_state);
        إذا(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

فراغ lv_obj_سیٹ_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, state, v);
}

فراغ lv_obj_سیٹ_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_ALT, en);
}

فراغ lv_obj_سیٹ_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_CHECKED, en);
}

فراغ lv_obj_سیٹ_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_FOCUSED, en);
}

فراغ lv_obj_سیٹ_focus_کلید(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_FOCUS_کلید, en);
}

فراغ lv_obj_سیٹ_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_EDITED, en);
}

فراغ lv_obj_سیٹ_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_HOVERED, en);
}

فراغ lv_obj_سیٹ_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_PRESSED, en);
}

فراغ lv_obj_سیٹ_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_SCROLLED, en);
}

فراغ lv_obj_سیٹ_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_DISABLED, en);
}

فراغ lv_obj_سیٹ_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_USER_1, en);
}

فراغ lv_obj_سیٹ_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_USER_2, en);
}

فراغ lv_obj_سیٹ_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_USER_3, en);
}

فراغ lv_obj_سیٹ_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    سیٹ_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->hidden;
}

bool lv_obj_is_clickable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->clickable;
}

bool lv_obj_is_click_focusable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->click_focusable;
}

bool lv_obj_is_checkable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->checkable;
}

bool lv_obj_is_scrollable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scrollable;
}

bool lv_obj_is_scroll_elastic(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->snappable;
}

bool lv_obj_is_press_جندرا(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->press_جندرا;
}

bool lv_obj_is_event_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->ignore_layout;
}

bool lv_obj_is_floating(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->floating;
}

bool lv_obj_is_send_draw_task_events(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->send_draw_task_events;
}

bool lv_obj_is_حد_توں_ودھ_visible(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->حد_توں_ودھ_visible;
}

bool lv_obj_is_event_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->event_trickle;
}

bool lv_obj_is_state_trickle(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->state_trickle;
}

bool lv_obj_is_radio_button(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع obj->radio_button;
}

bool lv_obj_is_flex_وچ_new_track(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    إذا(obj->spec_attr == عدم) أرجع false;

    أرجع obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    أرجع obj->state;
}

bool lv_obj_has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, state);
}

bool lv_obj_is_alt(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_کلید(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_FOCUS_کلید);
}

bool lv_obj_is_edited(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    أرجع has_state(obj, LV_STATE_USER_4);
}

lv_گروہ_t * lv_obj_get_گروہ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr) أرجع obj->spec_attr->گروہ_p;
    وإلا أرجع عدم;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_نتیجہ_t lv_obj_جوڑو_بچہ(lv_obj_t * ماپا, lv_obj_t * بچہ)
{
    LV_CHECK_OBJ(ماپا, MY_CLASS, أرجع LV_نتیجہ_INVALID);
    LV_CHECK_ARG(بچہ != عدم, أرجع LV_نتیجہ_INVALID);

    uint16_t new_بچہ_cnt = ماپا->spec_attr->بچہ_cnt + 1;

    lv_obj_t ** children = lv_realloc(ماپا->spec_attr->children,
                                      new_بچہ_cnt * (حجم(lv_obj_t *)));
    إذا(!children) {
        أرجع LV_نتیجہ_INVALID;
    }
    children[new_بچہ_cnt - 1] = بچہ;

    ماپا->spec_attr->بچہ_cnt = new_بچہ_cnt;
    ماپا->spec_attr->children = children;
    أرجع LV_نتیجہ_OK;
}

فراغ lv_obj_remove_بچہ(lv_obj_t * ماپا, lv_obj_t * بچہ)
{
    LV_CHECK_OBJ(ماپا, MY_CLASS, أرجع);
    LV_CHECK_OBJ(بچہ, MY_CLASS, أرجع);

    لكل(int32_t i = lv_obj_get_اشاریہ(بچہ); i < (int32_t)ماپا->spec_attr->بچہ_cnt - 1; i++) {
        ماپا->spec_attr->children[i] = ماپا->spec_attr->children[i + 1];
    }

    /* No more children*/
    إذا(ماپا->spec_attr->بچہ_cnt == 1) {
        lv_free(ماپا->spec_attr->children);
        ماپا->spec_attr->children = عدم;
        ماپا->spec_attr->بچہ_cnt = 0;
        أرجع;
    }
    ماپا->spec_attr->بچہ_cnt--;
    lv_obj_t ** new_children = lv_realloc(ماپا->spec_attr->children, ماپا->spec_attr->بچہ_cnt * (حجم(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    إذا(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        أرجع;
    }
    ماپا->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr) {
        أرجع obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(حجم(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    إذا(!spec_attr) {
        أرجع عدم;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_منوال = LV_SCROLLBAR_منوال_AUTO;
    obj->spec_attr = spec_attr;
    أرجع spec_attr;
}


bool lv_obj_check_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(class_p != عدم, أرجع false);
    أرجع obj->class_p == class_p;
}

bool lv_obj_has_class(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(class_p != عدم, أرجع false);

    ثابت lv_obj_class_t * obj_class = obj->class_p;
    طالما(obj_class) {
        إذا(obj_class == class_p) أرجع true;
        obj_class = obj_class->ادھار_class;
    }

    أرجع false;
}

ثابت lv_obj_class_t * lv_obj_get_class(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    أرجع obj->class_p;
}

bool lv_obj_is_وچ_widget_درخت(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    ثابت lv_obj_t * current = obj;

    طالما(current->ماپا != عدم) {
#إذا LV_USE_CHECK_OBJ_ماپا_LINK && LV_USE_ASSERT
        LV_ASSERT(ماپا_شامل_اے_بچہ(current->ماپا, current));
#جے_انت
        current = current->ماپا;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_اگلا(عدم);
    طالما(disp) {
        uint32_t i;
        لكل(i = 0; i < disp->screen_cnt; i++) {
            إذا(disp->screens[i] == current) أرجع true;
        }
        disp = lv_display_get_اگلا(disp);
    }

    أرجع false;
}

فراغ lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != عدم, أرجع);
    LV_CHECK_ARG(*obj_ptr != عدم, أرجع);

    lv_obj_جوڑو_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#إذا LV_USE_OBJ_ID
فراغ * lv_obj_get_id(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    أرجع obj->id;
}

#جے_انت

فراغ lv_obj_جوڑو_screen_لدو_event(lv_obj_t * obj, lv_event_کوڈ_t trigger, lv_obj_t * screen,
                                  lv_screen_لدو_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG_پیغام(screen != عدم, أرجع, "can't load a non-existing screen");

    screen_لدو_anim_dsc_t * dsc = lv_malloc(حجم(screen_لدو_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_لدو_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->نشانہ.screen = screen;

    lv_obj_جوڑو_event_cb(obj, screen_لدو_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_جوڑو_screen_create_event(lv_obj_t * obj, lv_event_کوڈ_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_لدو_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(screen_create_cb != عدم, أرجع);

    screen_لدو_anim_dsc_t * dsc = lv_malloc(حجم(screen_لدو_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(screen_لدو_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->نشانہ.create_cb = screen_create_cb;

    lv_obj_جوڑو_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_جوڑو_play_timeline_event(lv_obj_t * obj, lv_event_کوڈ_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool الٹ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(at != عدم, أرجع);

    timeline_play_dsc_t * dsc = lv_malloc(حجم(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, حجم(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->الٹ = الٹ;

    lv_obj_جوڑو_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_جوڑو_event_cb(obj, lv_event_free_user_انکڑے_cb, LV_EVENT_DELETE, dsc);
}

فراغ lv_obj_سیٹ_user_انکڑے(lv_obj_t * obj, فراغ * user_انکڑے)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    obj->user_انکڑے = user_انکڑے;
}

فراغ lv_obj_سیٹ_user_flag(lv_obj_t * obj, uint32_t ثنائی_ہندسہ, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(ثنائی_ہندسہ < 4, أرجع);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    إذا(spec_attr == عدم) أرجع;

    إذا(v) spec_attr->user_flags |= 1 << (ثنائی_ہندسہ + 4);
    وإلا spec_attr->user_flags &= ~(1 << (ثنائی_ہندسہ + 4));
}

فراغ * lv_obj_get_user_انکڑے(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    أرجع obj->user_انکڑے;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t ثنائی_ہندسہ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);
    LV_CHECK_ARG(ثنائی_ہندسہ < 4, أرجع false);

    إذا(obj->spec_attr == عدم) أرجع false;
    وإلا أرجع (obj->spec_attr->user_flags & 1 << (ثنائی_ہندسہ + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_جوڑو_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, فراغ * user_انکڑے)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(cb != عدم, أرجع عدم);

    lv_delete_dsc_t * dsc = lv_malloc(حجم(*dsc));
    إذا(!dsc) {
        أرجع عدم;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_انکڑے = user_انکڑے;

    lv_event_dsc_t * event_dsc = lv_obj_جوڑو_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    إذا(!event_dsc) {
        lv_free(dsc);
        أرجع عدم;
    }
    أرجع dsc;
}

فراغ lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    إذا(!dsc) {
        أرجع;
    }

    uint32_t گنتی = lv_obj_remove_event_cb_with_user_انکڑے(dsc->obj, call_delete_cb, dsc);
    إذا(گنتی == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        أرجع;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_obj_constructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    LV_بن_ورتیا(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * ماپا = obj->ماپا;
    إذا(ماپا) {
        int32_t sl = lv_obj_get_scroll_بایاں(ماپا);
        int32_t st = lv_obj_get_scroll_چوٹی(ماپا);

        obj->coords.y1 = ماپا->coords.y1 + lv_obj_get_style_pad_چوٹی_اندرونی(ماپا, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = ماپا->coords.x1 + lv_obj_get_style_pad_بایاں_اندرونی(ماپا, LV_PART_MAIN) - sl;
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
    إذا(ماپا) {
        obj->press_جندرا = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#إذا LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_سونپو
    lv_obj_سونپو_id(class_p, obj);
#جے_انت

    LV_TRACE_OBJ_CREATE("finished");
}

ساكن فراغ lv_obj_destructor(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    LV_بن_ورتیا(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, عدم);

    /*Delete from the group*/
    lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
    إذا(گروہ) lv_گروہ_remove_obj(obj);

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
#جے_انت

#إذا LV_DRAW_TRANSFORM_USE_میٹرکس
        إذا(obj->spec_attr->میٹرکس) {
            lv_free(obj->spec_attr->میٹرکس);
            obj->spec_attr->میٹرکس = عدم;
        }
#جے_انت

        lv_free(obj->spec_attr);
        obj->spec_attr = عدم;
    }

#إذا LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_سونپو
    lv_obj_free_id(obj);
#جے_انت
}

ساكن فراغ lv_obj_draw(lv_event_t * e)
{
    lv_event_کوڈ_t کوڈ = lv_event_get_کوڈ(e);
    lv_obj_t * obj = lv_event_get_current_نشانہ(e);
    إذا(کوڈ == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        إذا(info->res == LV_COVER_RES_MASKED) أرجع;
        إذا(lv_obj_get_style_clip_کونا_اندرونی(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            أرجع;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_اندرونی(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_اچائی_اندرونی(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        إذا(lv_area_is_وچ(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            أرجع;
        }

        إذا(lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            أرجع;
        }

        إذا(lv_obj_get_style_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            أرجع;
        }

        إذا(lv_obj_get_style_bg_grad_dir_اندرونی(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            إذا(lv_obj_get_style_bg_grad_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                أرجع;
            }
        }
        ثابت lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_اندرونی(obj, LV_PART_MAIN);
        إذا(grad_dsc) {
            uint32_t i;
            لكل(i = 0; i < grad_dsc->stops_گنتی; i++) {
                إذا(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    أرجع;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    وإلا إذا(کوڈ == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.ادھار.layer = layer;

        int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_اچائی_اندرونی(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_MAIN);
        إذا(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.کونا_radius = draw_dsc.radius;
            blur_dsc.ادھار.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        إذا(lv_obj_get_style_border_post_اندرونی(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    وإلا إذا(کوڈ == LV_EVENT_DRAW_MAIN_سرا) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_MAIN);
        إذا(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_اچائی_اندرونی(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.کونا_radius = lv_obj_get_style_radius_اندرونی(obj, LV_PART_MAIN);
            blur_dsc.ادھار.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        إذا(lv_obj_get_style_border_چوڑائی_اندرونی(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_اندرونی(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.ادھار.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_اچائی_اندرونی(obj, LV_PART_MAIN);
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

    إذا(lv_area_get_ناپ(&hor_area) <= 0 && lv_area_get_ناپ(&ver_area) <= 0) أرجع;

    lv_draw_rect_dsc_t rect_dsc;
    lv_نتیجہ_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    إذا(sb_res != LV_نتیجہ_OK) أرجع;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_اندرونی(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.کونا_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_اندرونی(obj, LV_PART_SCROLLBAR);

    إذا(lv_area_get_ناپ(&hor_area) > 0) {
        إذا(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.ادھار.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.ادھار.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        إذا(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    إذا(lv_area_get_ناپ(&ver_area) > 0) {
        blur_dsc.ادھار.id1 = 1;
        إذا(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.ادھار.id1 = 1;
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
ساكن lv_نتیجہ_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    إذا(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_اندرونی(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    إذا(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_چوڑائی = lv_obj_get_style_border_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
        إذا(dsc->border_چوڑائی > 0) {
            dsc->border_color = lv_obj_get_style_border_color_اندرونی(obj, LV_PART_SCROLLBAR);
        }
        وإلا {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, LV_PART_SCROLLBAR);
    إذا(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
        إذا(dsc->shadow_چوڑائی > 0) {
            dsc->shadow_پھیلاؤ = lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_اندرونی(obj, LV_PART_SCROLLBAR);
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
        dsc->radius = lv_obj_get_style_radius_اندرونی(obj, LV_PART_SCROLLBAR);
        أرجع LV_نتیجہ_OK;
    }
    وإلا {
        أرجع LV_نتیجہ_INVALID;
    }
}

ساكن فراغ lv_obj_event(ثابت lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_بن_ورتیا(class_p);

    lv_event_کوڈ_t کوڈ = lv_event_get_کوڈ(e);
    lv_obj_t * obj = lv_event_get_current_نشانہ(e);
    إذا(کوڈ == LV_EVENT_PRESSED) {
        lv_obj_جوڑو_state(obj, LV_STATE_PRESSED);
    }
    وإلا إذا(کوڈ == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        إذا((indev == عدم || lv_indev_get_scroll_obj(indev) == عدم) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            إذا(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_جوڑو_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            وإلا إذا(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            إذا(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_قدر_بدلیا, عدم);
                إذا(res != LV_نتیجہ_OK) أرجع;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, عدم);
                إذا(res != LV_نتیجہ_OK) أرجع;
            }
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_قدر_بدلیا) {
        إذا(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
            إذا(ماپا) {
                uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(ماپا);
                uint32_t i;
                لكل(i = 0; i < بچہ_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_بچہ(ماپا, i);
                    إذا(obj == sibling) استمر;

                    إذا(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_نتیجہ_t res = lv_obj_send_event(sibling, LV_EVENT_قدر_بدلیا, عدم);
                        إذا(res != LV_نتیجہ_OK) أرجع;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, عدم);
                        إذا(res != LV_نتیجہ_OK) أرجع;
                    }
                }
            }
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_PRESS_گواچا) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    وإلا إذا(کوڈ == LV_EVENT_STYLE_بدلیا) {
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        لكل(uint32_t i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(بچہ);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_کلید) {
        إذا(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_کلید(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            إذا(c == LV_کلید_دایاں || c == LV_کلید_اتے) {
                lv_obj_جوڑو_state(obj, LV_STATE_CHECKED);
            }
            وإلا إذا(c == LV_کلید_بایاں || c == LV_کلید_تھلے) {
                /*Radio buttons can't be checked off directly*/
                إذا(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            إذا(c != LV_کلید_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_قدر_بدلیا, عدم);
                إذا(res != LV_نتیجہ_OK) أرجع;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, عدم);
                إذا(res != LV_نتیجہ_OK) أرجع;
            }
        }
        وإلا إذا(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_بایاں(obj);
            int32_t sr = lv_obj_get_scroll_دایاں(obj);
            uint32_t c = lv_event_get_کلید(e);
            إذا(c == LV_کلید_تھلے) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_منزل_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_اچائی(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_کلید_اتے) {
                lv_obj_scroll_منزل_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_اچائی(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_کلید_دایاں) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                إذا(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_منزل_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_اچائی(obj) / 4, anim_enable);
                وإلا
                    lv_obj_scroll_منزل_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_چوڑائی(obj) / 4, anim_enable);
            }
            وإلا إذا(c == LV_کلید_بایاں) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                إذا(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_منزل_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_اچائی(obj) / 4, anim_enable);
                وإلا
                    lv_obj_scroll_منزل_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_چوڑائی(obj) / 4, anim_enable);
            }
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_FOCUSED) {
        إذا(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_منزل_view_recursive(obj, LV_ANIM_ON);
        }

        lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);
        ثابت bool editing = گروہ && lv_گروہ_get_editing(گروہ);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_سرگرم();
        إذا(indev == عدم) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != عدم ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        إذا(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_کلید;
        إذا(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_جوڑو_state(obj, state);
        }
        وإلا {
            lv_obj_جوڑو_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_جوڑو_state(obj, LV_STATE_SCROLLED);
    }
    وإلا إذا(کوڈ == LV_EVENT_SCROLL_سرا) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        إذا(lv_obj_get_scrollbar_منوال(obj) == LV_SCROLLBAR_منوال_سرگرم) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_کلید);
    }
    وإلا إذا(کوڈ == LV_EVENT_ناپ_بدلیا) {
        int32_t align = lv_obj_get_style_align_اندرونی(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_اندرونی(obj, LV_PART_MAIN);
        إذا(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        لكل(i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(بچہ);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_بچہ_بدلیا) {
        int32_t align = lv_obj_get_style_align_اندرونی(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_اندرونی(obj, LV_PART_MAIN);
        إذا(layout || align || lv_obj_is_style_any_چوڑائی_content(obj) || lv_obj_is_style_any_اچائی_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    وإلا إذا(کوڈ == LV_EVENT_بچہ_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    وإلا إذا(کوڈ == LV_EVENT_REFR_EXT_DRAW_ناپ) {
        int32_t d = lv_obj_calculate_ext_draw_ناپ(obj, LV_PART_MAIN);
        lv_event_سیٹ_ext_draw_ناپ(e, d);
    }
    وإلا إذا(کوڈ == LV_EVENT_DRAW_MAIN || کوڈ == LV_EVENT_DRAW_POST || کوڈ == LV_EVENT_DRAW_MAIN_سرا ||
            کوڈ == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    وإلا إذا(کوڈ == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    وإلا إذا(کوڈ == LV_EVENT_HOVER_OVER) {
        lv_obj_جوڑو_state(obj, LV_STATE_HOVERED);
    }
    وإلا إذا(کوڈ == LV_EVENT_HOVER_LEAVE) {
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

    إذا(obj->state == new_state) أرجع;


    lv_state_t prev_state = obj->state;

    lv_part_t بدلیا_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &بدلیا_part);
    /*If there is no difference in styles there is nothing else to do*/
    إذا(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_بدلیا, &prev_state);
        أرجع;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_حالت(obj);

    /*Skip transitions if the widget is not rendered yet. */
    إذا(!obj->rendered) {
        lv_obj_invalidate(obj);
        إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_ناپ(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_بدلیا, &prev_state);
        أرجع;
    }

    obj_transition_حالتاں(obj, prev_state, new_state);

    إذا(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && بدلیا_part == LV_PART_SCROLLBAR) {
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
        lv_obj_refresh_ext_draw_ناپ(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_بدلیا, &prev_state);
}

ساكن فراغ obj_transition_حالتاں(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(حجم(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    إذا(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        أرجع;
    }
    uint32_t tsi = 0;
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        إذا(state_act & (~new_state)) استمر; /*Skip unrelated styles*/
        إذا(obj_style->is_trans) استمر;

        lv_style_قدر_t v;
        إذا(lv_style_get_prop_اندرونی(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_لبھیا) استمر;
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
                ts[tsi].ویلا = tr->ویلا;
                ts[tsi].delay = tr->delay;
                ts[tsi].رستہ_cb = tr->رستہ_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_انکڑے = tr->user_انکڑے;
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
ساكن فراغ lv_obj_children_جوڑو_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(obj);

    لكل(uint32_t i = 0; i < بچہ_گنتی; i++) {
        lv_obj_t * بچہ = lv_obj_get_بچہ(obj, i);
        إذا(بچہ) {
            lv_obj_جوڑو_state(بچہ, state);
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
    uint32_t بچہ_گنتی = lv_obj_get_بچہ_گنتی(obj);

    لكل(uint32_t i = 0; i < بچہ_گنتی; i++) {
        lv_obj_t * بچہ = lv_obj_get_بچہ(obj, i);
        إذا(بچہ) {
            lv_obj_remove_state(بچہ, state);
        }
    }
}

ساكن فراغ null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_انکڑے(e);
    *obj_ptr = عدم;
}

ساكن فراغ screen_لدو_on_trigger_event_cb(lv_event_t * e)
{
    screen_لدو_anim_dsc_t * dsc = lv_event_get_user_انکڑے(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_لدو_anim(dsc->نشانہ.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

ساكن فراغ screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_لدو_anim_dsc_t * dsc = lv_event_get_user_انکڑے(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->نشانہ.create_cb();
    lv_screen_لدو_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_جوڑو_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, عدم);
}

ساكن فراغ play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_انکڑے(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t ترقی = lv_anim_timeline_get_ترقی(dsc->at);
    إذا(dsc->الٹ) {
        إذا(ترقی == 0) {
            lv_anim_timeline_سیٹ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_سیٹ_ترقی(dsc->at, LV_ANIM_TIMELINE_ترقی_MAX);
        }
    }
    وإلا {
        إذا(ترقی == LV_ANIM_TIMELINE_ترقی_MAX) {
            lv_anim_timeline_سیٹ_delay(dsc->at, dsc->delay);
            lv_anim_timeline_سیٹ_ترقی(dsc->at, 0);
        }

    }
    lv_anim_timeline_سیٹ_الٹ(dsc->at, dsc->الٹ);
    lv_anim_timeline_شروع(dsc->at);
}


ساكن فراغ delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_نشانہ_obj(e));
}

ساكن فراغ call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != عدم);
    lv_obj_t * obj = lv_event_get_نشانہ_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_انکڑے(e);
#إذا !LV_USE_ASSERT
    LV_بن_ورتیا(obj);
#جے_انت
    LV_ASSERT(dsc != عدم);
    LV_ASSERT(dsc->cb != عدم);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_انکڑے);
    lv_obj_remove_delete_cb(dsc);
}

ساكن مضمن فراغ سیٹ_state(lv_obj_t * obj, lv_state_t state,  bool قدر)
{
    LV_ASSERT(obj != عدم);
    إذا(قدر) lv_obj_جوڑو_state(obj, state);
    وإلا lv_obj_remove_state(obj, state);
}

ساكن مضمن bool has_state(ثابت lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != عدم);

    أرجع !!(obj->state & state);
}


#إذا LV_USE_CHECK_OBJ_ماپا_LINK && LV_USE_ASSERT
ساكن bool ماپا_شامل_اے_بچہ(ثابت lv_obj_t * ماپا, ثابت lv_obj_t * بچہ)
{
    uint32_t بچہ_cnt = ماپا->spec_attr ? ماپا->spec_attr->بچہ_cnt : 0;
    uint32_t i;
    لكل(i = 0; i < بچہ_cnt; i++) {
        إذا(ماپا->spec_attr->children[i] == بچہ) أرجع true;
    }
    أرجع false;
}
#جے_انت


#إذا LV_USE_OBJ_PROPERTY
ساكن lv_نقطہ_t lv_obj_get_scroll_سرا_helper(lv_obj_t * obj)
{
    lv_نقطہ_t نقطہ;
    lv_obj_get_scroll_سرا(obj, &نقطہ);
    أرجع نقطہ;
}

ساكن فراغ obj_سیٹ_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    إذا(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_سیٹ_hidden(obj, v);
    إذا(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_سیٹ_clickable(obj, v);
    إذا(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_سیٹ_click_focusable(obj, v);
    إذا(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_سیٹ_checkable(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_سیٹ_scrollable(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_سیٹ_scroll_elastic(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_سیٹ_scroll_momentum(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_سیٹ_scroll_one(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_سیٹ_scroll_chain_hor(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_سیٹ_scroll_chain_ver(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_سیٹ_scroll_on_focus(obj, v);
    إذا(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_سیٹ_scroll_with_arrow(obj, v);
    إذا(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_سیٹ_snappable(obj, v);
    إذا(f & LV_OBJ_FLAG_PRESS_جندرا)            lv_obj_سیٹ_press_جندرا(obj, v);
    إذا(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_سیٹ_event_bubble(obj, v);
    إذا(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_سیٹ_gesture_bubble(obj, v);
    إذا(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_سیٹ_adv_hittest(obj, v);
    إذا(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_سیٹ_ignore_layout(obj, v);
    إذا(f & LV_OBJ_FLAG_FLOATING)              lv_obj_سیٹ_floating(obj, v);
    إذا(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_سیٹ_send_draw_task_events(obj, v);
    إذا(f & LV_OBJ_FLAG_حد_توں_ودھ_VISIBLE)      lv_obj_سیٹ_حد_توں_ودھ_visible(obj, v);
    إذا(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_سیٹ_event_trickle(obj, v);
    إذا(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_سیٹ_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_بٹ = 0;
    إذا(f & LV_OBJ_FLAG_LAYOUT_1)              user_بٹ |= 1 << 0;
    إذا(f & LV_OBJ_FLAG_LAYOUT_2)              user_بٹ |= 1 << 1;
    إذا(f & LV_OBJ_FLAG_WIDGET_1)              user_بٹ |= 1 << 2;
    إذا(f & LV_OBJ_FLAG_WIDGET_2)              user_بٹ |= 1 << 3;
    إذا(f & LV_OBJ_FLAG_USER_1)                user_بٹ |= 1 << 4;
    إذا(f & LV_OBJ_FLAG_USER_2)                user_بٹ |= 1 << 5;
    إذا(f & LV_OBJ_FLAG_USER_3)                user_بٹ |= 1 << 6;
    إذا(f & LV_OBJ_FLAG_USER_4)                user_بٹ |= 1 << 7;
    إذا(user_بٹ == 0) أرجع;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    إذا(v) {
        obj->spec_attr->user_flags |= user_بٹ;
    }
    وإلا {
        obj->spec_attr->user_flags &= ~user_بٹ;
    }

    ثابت uint8_t layout_نقاب = 0x3;
    ثابت bool layout_flag_بدلیا = (old_user_flags ^ obj->spec_attr->user_flags) & layout_نقاب;
    إذا(layout_flag_بدلیا) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_ماپا(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

ساكن bool obj_has_flag_any(ثابت lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);

    إذا((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) أرجع true;
    إذا((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) أرجع true;
    إذا((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) أرجع true;
    إذا((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) أرجع true;
    إذا((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) أرجع true;
    إذا((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) أرجع true;
    إذا((f & LV_OBJ_FLAG_PRESS_جندرا) && obj->press_جندرا) أرجع true;
    إذا((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) أرجع true;
    إذا((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) أرجع true;
    إذا((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) أرجع true;
    إذا((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) أرجع true;
    إذا((f & LV_OBJ_FLAG_FLOATING) && obj->floating) أرجع true;
    إذا((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) أرجع true;
    إذا((f & LV_OBJ_FLAG_حد_توں_ودھ_VISIBLE) && obj->حد_توں_ودھ_visible) أرجع true;
    إذا((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) أرجع true;
    إذا((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) أرجع true;

    إذا(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == عدم) أرجع false;

    إذا((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) أرجع true;
    إذا((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) أرجع true;
    إذا((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) أرجع true;
    إذا((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) أرجع true;
    إذا((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) أرجع true;
    إذا((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) أرجع true;
    إذا((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) أرجع true;
    إذا((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) أرجع true;

    أرجع false;
}

ساكن lv_نتیجہ_t lv_obj_سیٹ_any(lv_obj_t * obj, lv_prop_id_t id, ثابت lv_property_t * prop)
{
    LV_ASSERT(obj != عدم);

    إذا(id >= LV_PROPERTY_OBJ_FLAG_شروع && id <= LV_PROPERTY_OBJ_FLAG_سرا) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_شروع);
        obj_سیٹ_flag(obj, flag, prop->عدد != 0);
        أرجع LV_نتیجہ_OK;
    }
    وإلا إذا(id >= LV_PROPERTY_OBJ_STATE_شروع && id <= LV_PROPERTY_OBJ_STATE_سرا) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_شروع);
        إذا(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        إذا(prop->عدد) lv_obj_جوڑو_state(obj, state);
        وإلا lv_obj_remove_state(obj, state);
        أرجع LV_نتیجہ_OK;
    }
    وإلا {
        أرجع LV_نتیجہ_INVALID;
    }
}

ساكن lv_نتیجہ_t lv_obj_get_any(ثابت lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != عدم);
    إذا(id >= LV_PROPERTY_OBJ_FLAG_شروع && id <= LV_PROPERTY_OBJ_FLAG_سرا) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_شروع);
        prop->id = id;
        prop->عدد = obj_has_flag_any(obj, flag);
        أرجع LV_نتیجہ_OK;
    }
    وإلا إذا(id >= LV_PROPERTY_OBJ_STATE_شروع && id <= LV_PROPERTY_OBJ_STATE_سرا) {
        prop->id = id;
        إذا(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->عدد = obj->state;
        }
        وإلا {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_شروع);
            prop->عدد = obj->state & flag;
        }
        أرجع LV_نتیجہ_OK;
    }
    وإلا {
        أرجع LV_نتیجہ_INVALID;
    }
}
#جے_انت /*LV_USE_OBJ_PROPERTY*/

