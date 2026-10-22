/**
 * @file lv_obj.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../misc/lv_event_private.h"
#అన్తర్భావ "../misc/lv_style_private.h"
#అన్తర్భావ "../misc/lv_area_private.h"
#అన్తర్భావ "lv_obj_style_private.h"
#అన్తర్భావ "lv_obj_event_private.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "../indev/lv_indev_private.h"
#అన్తర్భావ "../display/lv_display_private.h"
#అన్తర్భావ "lv_obj_draw_private.h"
#అన్తర్భావ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)
#నిర్వచించు LV_OBJ_DEF_వెడల్పు    (LV_DPX(100))
#నిర్వచించు LV_OBJ_DEF_ఎత్తు   (LV_DPX(50))
#నిర్వచించు STYLE_TRANSITION_MAX 32

/**********************
 *      TYPEDEFS
 **********************/

రకనిర్వచనం నిర్మాణం {
    lv_screen_ఎక్కించు_anim_t anim_type;
    uint32_t duration;
    uint32_t delay;
    సంఘం {
        lv_obj_t * screen;
        lv_screen_create_cb_t create_cb;
    } లక్ష్యం;
} screen_ఎక్కించు_anim_dsc_t;

రకనిర్వచనం నిర్మాణం {
    lv_anim_timeline_t * at;
    uint32_t delay;
    bool తిరగేయి;
} timeline_play_dsc_t;

నిర్మాణం _lv_delete_dsc_t {
    lv_obj_t * obj;
    lv_delete_cb_t cb;
    శూన్య * user_దత్తాంశం;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర శూన్య lv_obj_constructor(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj);
స్థిర శూన్య lv_obj_destructor(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj);
స్థిర శూన్య lv_obj_draw(lv_event_t * e);
స్థిర శూన్య lv_obj_event(స్థిరరాశి lv_obj_class_t * class_p, lv_event_t * e);
స్థిర శూన్య draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer);
స్థిర lv_ఫలితం_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc);
స్థిర శూన్య obj_transition_స్థితులు(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state);
స్థిర శూన్య update_obj_state(lv_obj_t * obj, lv_state_t new_state);
స్థిర శూన్య lv_obj_children_కూడు_state(lv_obj_t * obj, lv_state_t state);
స్థిర శూన్య lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state);
స్థిర శూన్య null_on_delete_cb(lv_event_t * e);
స్థిర శూన్య screen_ఎక్కించు_on_trigger_event_cb(lv_event_t * e);
స్థిర శూన్య screen_create_on_trigger_event_cb(lv_event_t * e);
స్థిర శూన్య play_timeline_on_trigger_event_cb(lv_event_t * e);
స్థిర శూన్య delete_on_screen_unloaded_event_cb(lv_event_t * e);
స్థిర శూన్య call_delete_cb(lv_event_t * e);

స్థిర అంతఃవరుస శూన్య సమితి_state(lv_obj_t * obj, lv_state_t state,  bool విలువ);
స్థిర అంతఃవరుస bool has_state(స్థిరరాశి lv_obj_t * obj, lv_state_t state);
#ఒకవేళ LV_USE_CHECK_OBJ_మాతృ_LINK && LV_USE_ASSERT
    స్థిర bool మాతృ_కలిగియుంది_బిడ్డ(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి lv_obj_t * బిడ్డ);
#అయితే_ముగింపు

#ఒకవేళ LV_USE_OBJ_PROPERTY
    స్థిర lv_ఫలితం_t lv_obj_సమితి_any(lv_obj_t *, lv_prop_id_t, స్థిరరాశి lv_property_t *);
    స్థిర lv_ఫలితం_t lv_obj_get_any(స్థిరరాశి lv_obj_t *, lv_prop_id_t, lv_property_t *);

    స్థిర lv_బిందువు_t lv_obj_get_scroll_చివర_helper(lv_obj_t * obj);

    స్థిర శూన్య obj_సమితి_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v);
    స్థిర bool obj_has_flag_any(స్థిరరాశి lv_obj_t * obj, lv_obj_flag_t f);
#అయితే_ముగింపు

/**********************
 *  STATIC VARIABLES
 **********************/
#ఒకవేళ LV_USE_OBJ_PROPERTY
స్థిర స్థిరరాశి lv_property_ops_t lv_obj_properties[] = {
    {
        .id = LV_PROPERTY_OBJ_మాతృ,
        .setter = lv_obj_సమితి_మాతృ,
        .getter = lv_obj_get_మాతృ,
    },
    {
        .id = LV_PROPERTY_OBJ_X,
        .setter = lv_obj_సమితి_x,
        .getter = lv_obj_get_x,
    },
    {
        .id = LV_PROPERTY_OBJ_Y,
        .setter = lv_obj_సమితి_y,
        .getter = lv_obj_get_y,
    },
    {
        .id = LV_PROPERTY_OBJ_W,
        .setter = lv_obj_సమితి_వెడల్పు,
        .getter = lv_obj_get_వెడల్పు,
    },
    {
        .id = LV_PROPERTY_OBJ_H,
        .setter = lv_obj_సమితి_ఎత్తు,
        .getter = lv_obj_get_ఎత్తు,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_వెడల్పు,
        .setter = lv_obj_సమితి_content_వెడల్పు,
        .getter = lv_obj_get_content_వెడల్పు,
    },
    {
        .id = LV_PROPERTY_OBJ_CONTENT_ఎత్తు,
        .setter = lv_obj_సమితి_content_ఎత్తు,
        .getter = lv_obj_get_content_ఎత్తు,
    },
    {
        .id = LV_PROPERTY_OBJ_LAYOUT,
        .setter = lv_obj_సమితి_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_ALIGN,
        .setter = lv_obj_సమితి_align,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLBAR_బహుళకం,
        .setter = lv_obj_సమితి_scrollbar_బహుళకం,
        .getter = lv_obj_get_scrollbar_బహుళకం,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_DIR,
        .setter = lv_obj_సమితి_scroll_dir,
        .getter = lv_obj_get_scroll_dir,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_X,
        .setter = lv_obj_సమితి_scroll_snap_x,
        .getter = lv_obj_get_scroll_snap_x,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_SNAP_Y,
        .setter = lv_obj_సమితి_scroll_snap_y,
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
        .id = LV_PROPERTY_OBJ_SCROLL_శిఖరం,
        .getter = lv_obj_get_scroll_శిఖరం,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_BOTTOM,
        .getter = lv_obj_get_scroll_bottom,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ఎడమ,
        .getter = lv_obj_get_scroll_ఎడమ,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_కుడి,
        .getter = lv_obj_get_scroll_కుడి,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_చివర,
        .getter = lv_obj_get_scroll_చివర_helper,
    },
    {
        .id = LV_PROPERTY_OBJ_EXT_DRAW_కొలత,
        .getter = lv_obj_get_ext_draw_కొలత,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_లెక్క,
        .getter = lv_obj_get_event_లెక్క,
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
        .id = LV_PROPERTY_OBJ_బిడ్డ_లెక్క,
        .getter = lv_obj_get_బిడ్డ_లెక్క,
    },
    {
        .id = LV_PROPERTY_OBJ_సూచీ,
        .getter = lv_obj_get_సూచీ,
    },
#ఒకవేళ LV_USE_OBJ_NAME
    {
        .id = LV_PROPERTY_OBJ_NAME,
        .getter = lv_obj_get_name,
        .setter = lv_obj_సమితి_name,
    },
#అయితే_ముగింపు
    {
        .id = LV_PROPERTY_OBJ_HIDDEN,
        .setter = lv_obj_సమితి_hidden,
        .getter = lv_obj_is_hidden,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICKABLE,
        .setter = lv_obj_సమితి_clickable,
        .getter = lv_obj_is_clickable,
    },
    {
        .id = LV_PROPERTY_OBJ_CLICK_FOCUSABLE,
        .setter = lv_obj_సమితి_click_focusable,
        .getter = lv_obj_is_click_focusable,
    },
    {
        .id = LV_PROPERTY_OBJ_CHECKABLE,
        .setter = lv_obj_సమితి_checkable,
        .getter = lv_obj_is_checkable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLLABLE,
        .setter = lv_obj_సమితి_scrollable,
        .getter = lv_obj_is_scrollable,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ELASTIC,
        .setter = lv_obj_సమితి_scroll_elastic,
        .getter = lv_obj_is_scroll_elastic,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_MOMENTUM,
        .setter = lv_obj_సమితి_scroll_momentum,
        .getter = lv_obj_is_scroll_momentum,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ONE,
        .setter = lv_obj_సమితి_scroll_one,
        .getter = lv_obj_is_scroll_one,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_HOR,
        .setter = lv_obj_సమితి_scroll_chain_hor,
        .getter = lv_obj_is_scroll_chain_hor,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_CHAIN_VER,
        .setter = lv_obj_సమితి_scroll_chain_ver,
        .getter = lv_obj_is_scroll_chain_ver,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_ON_FOCUS,
        .setter = lv_obj_సమితి_scroll_on_focus,
        .getter = lv_obj_is_scroll_on_focus,
    },
    {
        .id = LV_PROPERTY_OBJ_SCROLL_WITH_ARROW,
        .setter = lv_obj_సమితి_scroll_with_arrow,
        .getter = lv_obj_is_scroll_with_arrow,
    },
    {
        .id = LV_PROPERTY_OBJ_SNAPPABLE,
        .setter = lv_obj_సమితి_snappable,
        .getter = lv_obj_is_snappable,
    },
    {
        .id = LV_PROPERTY_OBJ_PRESS_తాళం,
        .setter = lv_obj_సమితి_press_తాళం,
        .getter = lv_obj_is_press_తాళం,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_BUBBLE,
        .setter = lv_obj_సమితి_event_bubble,
        .getter = lv_obj_is_event_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_GESTURE_BUBBLE,
        .setter = lv_obj_సమితి_gesture_bubble,
        .getter = lv_obj_is_gesture_bubble,
    },
    {
        .id = LV_PROPERTY_OBJ_ADV_HITTEST,
        .setter = lv_obj_సమితి_adv_hittest,
        .getter = lv_obj_is_adv_hittest,
    },
    {
        .id = LV_PROPERTY_OBJ_IGNORE_LAYOUT,
        .setter = lv_obj_సమితి_ignore_layout,
        .getter = lv_obj_is_ignore_layout,
    },
    {
        .id = LV_PROPERTY_OBJ_FLOATING,
        .setter = lv_obj_సమితి_floating,
        .getter = lv_obj_is_floating,
    },
    {
        .id = LV_PROPERTY_OBJ_SEND_DRAW_TASK_EVENTS,
        .setter = lv_obj_సమితి_send_draw_task_events,
        .getter = lv_obj_is_send_draw_task_events,
    },
    {
        .id = LV_PROPERTY_OBJ_అధికప్రవాహం_VISIBLE,
        .setter = lv_obj_సమితి_అధికప్రవాహం_visible,
        .getter = lv_obj_is_అధికప్రవాహం_visible,
    },
    {
        .id = LV_PROPERTY_OBJ_EVENT_TRICKLE,
        .setter = lv_obj_సమితి_event_trickle,
        .getter = lv_obj_is_event_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_STATE_TRICKLE,
        .setter = lv_obj_సమితి_state_trickle,
        .getter = lv_obj_is_state_trickle,
    },
    {
        .id = LV_PROPERTY_OBJ_FLEX_లోపల_NEW_TRACK,
        .setter = lv_obj_సమితి_flex_లోపల_new_track,
        .getter = lv_obj_is_flex_లోపల_new_track,
    },
    {
        .id = LV_PROPERTY_ID_ANY,
        .setter = lv_obj_సమితి_any,
        .getter = lv_obj_get_any,
    },
};
#అయితే_ముగింపు

స్థిరరాశి lv_obj_class_t lv_obj_class = {
    .constructor_cb = lv_obj_constructor,
    .destructor_cb = lv_obj_destructor,
    .event_cb = lv_obj_event,
    .వెడల్పు_def = LV_DPI_DEF,
    .ఎత్తు_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .సమూహం_def = LV_OBJ_CLASS_సమూహం_DEF_FALSE,
    .instance_కొలత = (పరిమాణం(lv_obj_t)),
    .పునాది_class = ఖాళీ,
    .name = "lv_obj",
    LV_PROPERTY_CLASS_FIELDS(obj, OBJ)
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_create(lv_obj_t * మాతృ)
{
    LV_LOG_INFO("begin");

    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, మాతృ);

    ఒకవేళ(obj == ఖాళీ) తిరిగి ఖాళీ;

    lv_obj_class_init_obj(obj);

    LV_LOG_TRACE("finished");

    తిరిగి obj;
}

/*=====================
 * Setter functions
 *====================*/

/*-----------------
 * Attribute set
 *----------------*/

శూన్య lv_obj_సమితి_hidden(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    ఒకవేళ(obj->hidden == en) తిరిగి;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    /*Invalidate while still visible*/
    ఒకవేళ(!obj->hidden) lv_obj_invalidate(obj);

    obj->hidden = en;

    /*Invalidate when visible again*/
    ఒకవేళ(!obj->hidden) lv_obj_invalidate(obj);

    /*Focus the next next item when this one get hidden*/
    ఒకవేళ(obj->hidden && has_state(obj, LV_STATE_FOCUSED)) {
        lv_సమూహం_t * సమూహం = lv_obj_get_సమూహం(obj);
        ఒకవేళ(సమూహం != ఖాళీ) {
            lv_సమూహం_focus_తదుపరి(సమూహం);
            lv_obj_t * తదుపరి_obj = lv_సమూహం_get_focused(సమూహం);
            ఒకవేళ(తదుపరి_obj != ఖాళీ) {
                lv_obj_invalidate(తదుపరి_obj);
            }
        }
    }

    ఒకవేళ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_మాతృ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

శూన్య lv_obj_సమితి_clickable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->clickable = en;

}

శూన్య lv_obj_సమితి_click_focusable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->click_focusable = en;
}

శూన్య lv_obj_సమితి_checkable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->checkable = en;
}

శూన్య lv_obj_సమితి_scrollable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(obj->scrollable == en) తిరిగి;

    /*Invalidate the scrollbars while scrollable*/
    ఒకవేళ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }

    obj->scrollable = en;

    /*Invalidate the scrollbars while scrollable again*/
    ఒకవేళ(obj->scrollable) {
        lv_area_t hor_area, ver_area;
        lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
        lv_obj_invalidate_area(obj, &hor_area);
        lv_obj_invalidate_area(obj, &ver_area);
    }
}

శూన్య lv_obj_సమితి_scroll_elastic(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_elastic = en;
}

శూన్య lv_obj_సమితి_scroll_momentum(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_momentum = en;
}

శూన్య lv_obj_సమితి_scroll_one(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_one = en;
}

శూన్య lv_obj_సమితి_scroll_chain(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_chain_hor = en;
    obj->scroll_chain_ver = en;
}

శూన్య lv_obj_సమితి_scroll_chain_hor(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_chain_hor = en;
}

శూన్య lv_obj_సమితి_scroll_chain_ver(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_chain_ver = en;
}

శూన్య lv_obj_సమితి_scroll_on_focus(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_on_focus = en;
}

శూన్య lv_obj_సమితి_scroll_with_arrow(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->scroll_with_arrow = en;
}

శూన్య lv_obj_సమితి_snappable(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->snappable = en;
}

శూన్య lv_obj_సమితి_press_తాళం(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->press_తాళం = en;
}

శూన్య lv_obj_సమితి_event_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->event_bubble = en;
}

శూన్య lv_obj_సమితి_gesture_bubble(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->gesture_bubble = en;
}

శూన్య lv_obj_సమితి_adv_hittest(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->adv_hittest = en;
}

శూన్య lv_obj_సమితి_ignore_layout(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(obj->ignore_layout == en) తిరిగి;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->ignore_layout = en;

    ఒకవేళ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_మాతృ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

శూన్య lv_obj_సమితి_floating(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(obj->floating == en) తిరిగి;

    bool was_on_layout = lv_obj_is_layout_positioned(obj);

    obj->floating = en;

    ఒకవేళ((was_on_layout != lv_obj_is_layout_positioned(obj))) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_మాతృ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

శూన్య lv_obj_సమితి_send_draw_task_events(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->send_draw_task_events = en;
}

శూన్య lv_obj_సమితి_అధికప్రవాహం_visible(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->అధికప్రవాహం_visible = en;
}

శూన్య lv_obj_సమితి_event_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->event_trickle = en;
}

శూన్య lv_obj_సమితి_state_trickle(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->state_trickle = en;
}

శూన్య lv_obj_సమితి_radio_button(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->radio_button = en;
}

శూన్య lv_obj_సమితి_flex_లోపల_new_track(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /*TODO For backward compatibility use it as an LV_OBJ_FLAG_LAYOUT_1
     *It will have its own bit in lv_obj_t after v9.5*/
    ఒకవేళ(en) {
        /*Need to allocate spec_attr to set the bit*/
        lv_obj_allocate_spec_attr(obj);
        obj->spec_attr->user_flags |= 1 << 0;
    }
    లేకపోతే {
        /*If spec_attr is not allocated,
         *its not needed to clear it as 0 is the default value*/
        ఒకవేళ(obj->spec_attr) obj->spec_attr->user_flags &= ~(1 << 0);
    }

    lv_obj_mark_layout_as_dirty(lv_obj_get_మాతృ(obj));
    lv_obj_mark_layout_as_dirty(obj);
}

శూన్య lv_obj_కూడు_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_state_t new_state = obj->state | state;
    ఒకవేళ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ఒకవేళ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_కూడు_state(obj, state);
        }
    }
}

శూన్య lv_obj_remove_state(lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_state_t new_state = obj->state & (~state);
    ఒకవేళ(obj->state != new_state) {
        update_obj_state(obj, new_state);
        ఒకవేళ(lv_obj_is_state_trickle(obj)) {
            lv_obj_children_remove_state(obj, state);
        }
    }
}

శూన్య lv_obj_సమితి_state(lv_obj_t * obj, lv_state_t state, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, state, v);
}

శూన్య lv_obj_సమితి_alt(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_ALT, en);
}

శూన్య lv_obj_సమితి_checked(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_CHECKED, en);
}

శూన్య lv_obj_సమితి_focused(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_FOCUSED, en);
}

శూన్య lv_obj_సమితి_focus_కీలకం(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_FOCUS_కీలకం, en);
}

శూన్య lv_obj_సమితి_edited(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_EDITED, en);
}

శూన్య lv_obj_సమితి_hovered(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_HOVERED, en);
}

శూన్య lv_obj_సమితి_pressed(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_PRESSED, en);
}

శూన్య lv_obj_సమితి_scrolled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_SCROLLED, en);
}

శూన్య lv_obj_సమితి_disabled(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_DISABLED, en);
}

శూన్య lv_obj_సమితి_state_user_1(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_USER_1, en);
}

శూన్య lv_obj_సమితి_state_user_2(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_USER_2, en);
}

శూన్య lv_obj_సమితి_state_user_3(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_USER_3, en);
}

శూన్య lv_obj_సమితి_state_user_4(lv_obj_t * obj, bool en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    సమితి_state(obj, LV_STATE_USER_4, en);
}

/*=======================
 * Getter functions
 *======================*/

bool lv_obj_is_hidden(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->hidden;
}

bool lv_obj_is_clickable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->clickable;
}

bool lv_obj_is_click_focusable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->click_focusable;
}

bool lv_obj_is_checkable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->checkable;
}

bool lv_obj_is_scrollable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scrollable;
}

bool lv_obj_is_scroll_elastic(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_elastic;
}

bool lv_obj_is_scroll_momentum(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_momentum;
}

bool lv_obj_is_scroll_one(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_one;
}

bool lv_obj_is_scroll_chain_hor(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_chain_hor;
}

bool lv_obj_is_scroll_chain_ver(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_chain_ver;
}

bool lv_obj_is_scroll_on_focus(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_on_focus;
}

bool lv_obj_is_scroll_with_arrow(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->scroll_with_arrow;
}

bool lv_obj_is_snappable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->snappable;
}

bool lv_obj_is_press_తాళం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->press_తాళం;
}

bool lv_obj_is_event_bubble(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->event_bubble;
}

bool lv_obj_is_gesture_bubble(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->gesture_bubble;
}

bool lv_obj_is_adv_hittest(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->adv_hittest;
}

bool lv_obj_is_ignore_layout(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->ignore_layout;
}

bool lv_obj_is_floating(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->floating;
}

bool lv_obj_is_send_draw_task_events(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->send_draw_task_events;
}

bool lv_obj_is_అధికప్రవాహం_visible(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->అధికప్రవాహం_visible;
}

bool lv_obj_is_event_trickle(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->event_trickle;
}

bool lv_obj_is_state_trickle(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->state_trickle;
}

bool lv_obj_is_radio_button(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి obj->radio_button;
}

bool lv_obj_is_flex_లోపల_new_track(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    /*For backward compatibility use it as as LV_OBJ_FLAG_LAYOUT_1
     *It will have it's on bin in lv_obj_t after v9.5*/
    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి false;

    తిరిగి obj->spec_attr->user_flags & (1 << 0);
}


lv_state_t lv_obj_get_state(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి obj->state;
}

bool lv_obj_has_state(స్థిరరాశి lv_obj_t * obj, lv_state_t state)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, state);
}

bool lv_obj_is_alt(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_ALT);
}

bool lv_obj_is_checked(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_CHECKED);
}

bool lv_obj_is_focused(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_FOCUSED);
}

bool lv_obj_is_focus_కీలకం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_FOCUS_కీలకం);
}

bool lv_obj_is_edited(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_EDITED);
}

bool lv_obj_is_hovered(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_HOVERED);
}

bool lv_obj_is_pressed(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_PRESSED);
}

bool lv_obj_is_scrolled(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_SCROLLED);
}

bool lv_obj_is_disabled(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_DISABLED);
}

bool lv_obj_is_state_user_1(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_USER_1);
}

bool lv_obj_is_state_user_2(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_USER_2);
}

bool lv_obj_is_state_user_3(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_USER_3);
}

bool lv_obj_is_state_user_4(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    తిరిగి has_state(obj, LV_STATE_USER_4);
}

lv_సమూహం_t * lv_obj_get_సమూహం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    ఒకవేళ(obj->spec_attr) తిరిగి obj->spec_attr->సమూహం_p;
    లేకపోతే తిరిగి ఖాళీ;
}

/*-------------------
 * OTHER FUNCTIONS
 *------------------*/

lv_ఫలితం_t lv_obj_కూడు_బిడ్డ(lv_obj_t * మాతృ, lv_obj_t * బిడ్డ)
{
    LV_CHECK_OBJ(మాతృ, MY_CLASS, తిరిగి LV_ఫలితం_INVALID);
    LV_CHECK_ARG(బిడ్డ != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);

    uint16_t new_బిడ్డ_cnt = మాతృ->spec_attr->బిడ్డ_cnt + 1;

    lv_obj_t ** children = lv_realloc(మాతృ->spec_attr->children,
                                      new_బిడ్డ_cnt * (పరిమాణం(lv_obj_t *)));
    ఒకవేళ(!children) {
        తిరిగి LV_ఫలితం_INVALID;
    }
    children[new_బిడ్డ_cnt - 1] = బిడ్డ;

    మాతృ->spec_attr->బిడ్డ_cnt = new_బిడ్డ_cnt;
    మాతృ->spec_attr->children = children;
    తిరిగి LV_ఫలితం_OK;
}

శూన్య lv_obj_remove_బిడ్డ(lv_obj_t * మాతృ, lv_obj_t * బిడ్డ)
{
    LV_CHECK_OBJ(మాతృ, MY_CLASS, తిరిగి);
    LV_CHECK_OBJ(బిడ్డ, MY_CLASS, తిరిగి);

    కొరకు(int32_t i = lv_obj_get_సూచీ(బిడ్డ); i < (int32_t)మాతృ->spec_attr->బిడ్డ_cnt - 1; i++) {
        మాతృ->spec_attr->children[i] = మాతృ->spec_attr->children[i + 1];
    }

    /* No more children*/
    ఒకవేళ(మాతృ->spec_attr->బిడ్డ_cnt == 1) {
        lv_free(మాతృ->spec_attr->children);
        మాతృ->spec_attr->children = ఖాళీ;
        మాతృ->spec_attr->బిడ్డ_cnt = 0;
        తిరిగి;
    }
    మాతృ->spec_attr->బిడ్డ_cnt--;
    lv_obj_t ** new_children = lv_realloc(మాతృ->spec_attr->children, మాతృ->spec_attr->బిడ్డ_cnt * (పరిమాణం(lv_obj_t *)));
    /* Reallocating a smaller size should never fail but just in case it's implemented as malloc + memcpy + free*/
    ఒకవేళ(!new_children) {
        LV_LOG_INFO("Failed to shrink children array");
        తిరిగి;
    }
    మాతృ->spec_attr->children = new_children;
}

lv_obj_spec_attr_t * lv_obj_allocate_spec_attr(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    ఒకవేళ(obj->spec_attr) {
        తిరిగి obj->spec_attr;
    }

    lv_obj_spec_attr_t * spec_attr = lv_malloc_zeroed(పరిమాణం(lv_obj_spec_attr_t));
    LV_ASSERT_MALLOC(spec_attr);
    ఒకవేళ(!spec_attr) {
        తిరిగి ఖాళీ;
    }

    spec_attr->scroll_dir = LV_DIR_ALL;
    spec_attr->scrollbar_బహుళకం = LV_SCROLLBAR_బహుళకం_AUTO;
    obj->spec_attr = spec_attr;
    తిరిగి spec_attr;
}


bool lv_obj_check_type(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి false);
    తిరిగి obj->class_p == class_p;
}

bool lv_obj_has_class(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి false);

    స్థిరరాశి lv_obj_class_t * obj_class = obj->class_p;
    అయితే(obj_class) {
        ఒకవేళ(obj_class == class_p) తిరిగి true;
        obj_class = obj_class->పునాది_class;
    }

    తిరిగి false;
}

స్థిరరాశి lv_obj_class_t * lv_obj_get_class(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);  /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */
    తిరిగి obj->class_p;
}

bool lv_obj_is_లోపల_widget_వృక్షం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);   /* Can't use LV_CHECK_OBJ here, it could cause an infinite recursion loop. */

    స్థిరరాశి lv_obj_t * current = obj;

    అయితే(current->మాతృ != ఖాళీ) {
#ఒకవేళ LV_USE_CHECK_OBJ_మాతృ_LINK && LV_USE_ASSERT
        LV_ASSERT(మాతృ_కలిగియుంది_బిడ్డ(current->మాతృ, current));
#అయితే_ముగింపు
        current = current->మాతృ;
    }

    /* current is now a screen (parent == NULL); verify it belongs to a display */
    lv_display_t * disp = lv_display_get_తదుపరి(ఖాళీ);
    అయితే(disp) {
        uint32_t i;
        కొరకు(i = 0; i < disp->screen_cnt; i++) {
            ఒకవేళ(disp->screens[i] == current) తిరిగి true;
        }
        disp = lv_display_get_తదుపరి(disp);
    }

    తిరిగి false;
}

శూన్య lv_obj_null_on_delete(lv_obj_t ** obj_ptr)
{
    LV_CHECK_ARG(obj_ptr != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(*obj_ptr != ఖాళీ, తిరిగి);

    lv_obj_కూడు_event_cb(*obj_ptr, null_on_delete_cb, LV_EVENT_DELETE, obj_ptr);
}

#ఒకవేళ LV_USE_OBJ_ID
శూన్య * lv_obj_get_id(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    తిరిగి obj->id;
}

#అయితే_ముగింపు

శూన్య lv_obj_కూడు_screen_ఎక్కించు_event(lv_obj_t * obj, lv_event_సంకేతం_t trigger, lv_obj_t * screen,
                                  lv_screen_ఎక్కించు_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG_సందేశం(screen != ఖాళీ, తిరిగి, "can't load a non-existing screen");

    screen_ఎక్కించు_anim_dsc_t * dsc = lv_malloc(పరిమాణం(screen_ఎక్కించు_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, పరిమాణం(screen_ఎక్కించు_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->లక్ష్యం.screen = screen;

    lv_obj_కూడు_event_cb(obj, screen_ఎక్కించు_on_trigger_event_cb, trigger, dsc);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, dsc);
}

శూన్య lv_obj_కూడు_screen_create_event(lv_obj_t * obj, lv_event_సంకేతం_t trigger, lv_screen_create_cb_t screen_create_cb,
                                    lv_screen_ఎక్కించు_anim_t anim_type, uint32_t duration, uint32_t delay)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(screen_create_cb != ఖాళీ, తిరిగి);

    screen_ఎక్కించు_anim_dsc_t * dsc = lv_malloc(పరిమాణం(screen_ఎక్కించు_anim_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, పరిమాణం(screen_ఎక్కించు_anim_dsc_t));
    dsc->anim_type = anim_type;
    dsc->duration = duration;
    dsc->delay = delay;
    dsc->లక్ష్యం.create_cb = screen_create_cb;

    lv_obj_కూడు_event_cb(obj, screen_create_on_trigger_event_cb, trigger, dsc);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, dsc);
}

శూన్య lv_obj_కూడు_play_timeline_event(lv_obj_t * obj, lv_event_సంకేతం_t trigger, lv_anim_timeline_t * at, uint32_t delay,
                                    bool తిరగేయి)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(at != ఖాళీ, తిరిగి);

    timeline_play_dsc_t * dsc = lv_malloc(పరిమాణం(timeline_play_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, పరిమాణం(timeline_play_dsc_t));
    dsc->at = at;
    dsc->delay = delay;
    dsc->తిరగేయి = తిరగేయి;

    lv_obj_కూడు_event_cb(obj, play_timeline_on_trigger_event_cb, trigger, dsc);
    lv_obj_కూడు_event_cb(obj, lv_event_free_user_దత్తాంశం_cb, LV_EVENT_DELETE, dsc);
}

శూన్య lv_obj_సమితి_user_దత్తాంశం(lv_obj_t * obj, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    obj->user_దత్తాంశం = user_దత్తాంశం;
}

శూన్య lv_obj_సమితి_user_flag(lv_obj_t * obj, uint32_t బిట్టు, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(బిట్టు < 4, తిరిగి);

    lv_obj_spec_attr_t * spec_attr = lv_obj_allocate_spec_attr(obj);
    LV_ASSERT_MALLOC(spec_attr);
    ఒకవేళ(spec_attr == ఖాళీ) తిరిగి;

    ఒకవేళ(v) spec_attr->user_flags |= 1 << (బిట్టు + 4);
    లేకపోతే spec_attr->user_flags &= ~(1 << (బిట్టు + 4));
}

శూన్య * lv_obj_get_user_దత్తాంశం(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    తిరిగి obj->user_దత్తాంశం;
}

bool lv_obj_get_user_flag(lv_obj_t * obj, uint32_t బిట్టు)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);
    LV_CHECK_ARG(బిట్టు < 4, తిరిగి false);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి false;
    లేకపోతే తిరిగి (obj->spec_attr->user_flags & 1 << (బిట్టు + 4)) ? true : false;
}

lv_delete_dsc_t * lv_obj_కూడు_delete_cb(lv_obj_t * obj, lv_delete_cb_t cb, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);
    LV_CHECK_ARG(cb != ఖాళీ, తిరిగి ఖాళీ);

    lv_delete_dsc_t * dsc = lv_malloc(పరిమాణం(*dsc));
    ఒకవేళ(!dsc) {
        తిరిగి ఖాళీ;
    }
    dsc->obj = obj;
    dsc->cb = cb;
    dsc->user_దత్తాంశం = user_దత్తాంశం;

    lv_event_dsc_t * event_dsc = lv_obj_కూడు_event_cb(obj, call_delete_cb, LV_EVENT_DELETE, dsc);
    ఒకవేళ(!event_dsc) {
        lv_free(dsc);
        తిరిగి ఖాళీ;
    }
    తిరిగి dsc;
}

శూన్య lv_obj_remove_delete_cb(lv_delete_dsc_t * dsc)
{
    ఒకవేళ(!dsc) {
        తిరిగి;
    }

    uint32_t లెక్క = lv_obj_remove_event_cb_with_user_దత్తాంశం(dsc->obj, call_delete_cb, dsc);
    ఒకవేళ(లెక్క == 0) {
        LV_LOG_WARN("Delete callback descriptor not found on object or already removed");
        తిరిగి;
    }
    lv_free(dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర శూన్య lv_obj_constructor(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ఖాళీ);

    LV_వాడని(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_obj_t * మాతృ = obj->మాతృ;
    ఒకవేళ(మాతృ) {
        int32_t sl = lv_obj_get_scroll_ఎడమ(మాతృ);
        int32_t st = lv_obj_get_scroll_శిఖరం(మాతృ);

        obj->coords.y1 = మాతృ->coords.y1 + lv_obj_get_style_pad_శిఖరం_అంతర్గత(మాతృ, LV_PART_MAIN) - st;
        obj->coords.y2 = obj->coords.y1 - 1;
        obj->coords.x1  = మాతృ->coords.x1 + lv_obj_get_style_pad_ఎడమ_అంతర్గత(మాతృ, LV_PART_MAIN) - sl;
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
    ఒకవేళ(మాతృ) {
        obj->press_తాళం = 1;
        obj->scroll_chain_hor = 1;
        obj->scroll_chain_ver = 1;
        obj->gesture_bubble = 1;
    }

#ఒకవేళ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_కేటాయించు
    lv_obj_కేటాయించు_id(class_p, obj);
#అయితే_ముగింపు

    LV_TRACE_OBJ_CREATE("finished");
}

స్థిర శూన్య lv_obj_destructor(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT(obj != ఖాళీ);

    LV_వాడని(class_p);

    lv_event_mark_deleted(obj);

    /*Remove all style*/
    lv_obj_enable_style_refresh(false); /*No need to refresh the style because the object will be deleted*/
    lv_obj_remove_style_all(obj);
    lv_obj_enable_style_refresh(true);

    /*Remove the animations from this object*/
    lv_anim_delete(obj, ఖాళీ);

    /*Delete from the group*/
    lv_సమూహం_t * సమూహం = lv_obj_get_సమూహం(obj);
    ఒకవేళ(సమూహం) lv_సమూహం_remove_obj(obj);

    ఒకవేళ(obj->spec_attr) {
        ఒకవేళ(obj->spec_attr->children) {
            lv_free(obj->spec_attr->children);
            obj->spec_attr->children = ఖాళీ;
        }

        lv_event_remove_all(&obj->spec_attr->event_list);
#ఒకవేళ LV_USE_OBJ_NAME
        ఒకవేళ(obj->spec_attr->name && !obj->spec_attr->name_static) {
            lv_free((శూన్య *)obj->spec_attr->name);
        }
#అయితే_ముగింపు

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
        ఒకవేళ(obj->spec_attr->మాతృక) {
            lv_free(obj->spec_attr->మాతృక);
            obj->spec_attr->మాతృక = ఖాళీ;
        }
#అయితే_ముగింపు

        lv_free(obj->spec_attr);
        obj->spec_attr = ఖాళీ;
    }

#ఒకవేళ LV_USE_OBJ_ID && LV_OBJ_ID_AUTO_కేటాయించు
    lv_obj_free_id(obj);
#అయితే_ముగింపు
}

స్థిర శూన్య lv_obj_draw(lv_event_t * e)
{
    lv_event_సంకేతం_t సంకేతం_2 = lv_event_get_సంకేతం(e);
    lv_obj_t * obj = lv_event_get_current_లక్ష్యం(e);
    ఒకవేళ(సంకేతం_2 == LV_EVENT_COVER_CHECK) {
        lv_cover_check_info_t * info = lv_event_get_param(e);
        ఒకవేళ(info->res == LV_COVER_RES_MASKED) తిరిగి;
        ఒకవేళ(lv_obj_get_style_clip_మూల_అంతర్గత(obj, LV_PART_MAIN)) {
            info->res = LV_COVER_RES_MASKED;
            తిరిగి;
        }

        /*Most trivial test. Is the mask fully IN the object? If no it surely doesn't cover it*/
        int32_t r = lv_obj_get_style_radius_అంతర్గత(obj, LV_PART_MAIN);
        int32_t w = lv_obj_get_style_transform_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        ఒకవేళ(lv_area_is_లోపల(info->area, &coords, r) == false) {
            info->res = LV_COVER_RES_NOT_COVER;
            తిరిగి;
        }

        ఒకవేళ(lv_obj_get_style_bg_opa_అంతర్గత(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            తిరిగి;
        }

        ఒకవేళ(lv_obj_get_style_opa_అంతర్గత(obj, LV_PART_MAIN) < LV_OPA_MAX) {
            info->res = LV_COVER_RES_NOT_COVER;
            తిరిగి;
        }

        ఒకవేళ(lv_obj_get_style_bg_grad_dir_అంతర్గత(obj, LV_PART_MAIN) != LV_GRAD_DIR_NONE) {
            ఒకవేళ(lv_obj_get_style_bg_grad_opa_అంతర్గత(obj, LV_PART_MAIN) < LV_OPA_MAX ||
               lv_obj_get_style_bg_main_opa_అంతర్గత(obj, LV_PART_MAIN) < LV_OPA_MAX) {
                info->res = LV_COVER_RES_NOT_COVER;
                తిరిగి;
            }
        }
        స్థిరరాశి lv_grad_dsc_t * grad_dsc = lv_obj_get_style_bg_grad_అంతర్గత(obj, LV_PART_MAIN);
        ఒకవేళ(grad_dsc) {
            uint32_t i;
            కొరకు(i = 0; i < grad_dsc->stops_లెక్క; i++) {
                ఒకవేళ(grad_dsc->stops[i].opa < LV_OPA_MAX) {
                    info->res = LV_COVER_RES_NOT_COVER;
                    తిరిగి;
                }
            }
        }
        info->res = LV_COVER_RES_COVER;
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_DRAW_MAIN) {
        lv_layer_t * layer = lv_event_get_layer(e);

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.పునాది.layer = layer;

        int32_t w = lv_obj_get_style_transform_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
        int32_t h = lv_obj_get_style_transform_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
        lv_area_t coords;
        coords = obj->coords;
        lv_area_increase(&coords, w, h);

        bool backdrop_blur = lv_obj_get_style_blur_backdrop_అంతర్గత(obj, LV_PART_MAIN);
        ఒకవేళ(backdrop_blur) {
            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.మూల_radius = draw_dsc.radius;
            blur_dsc.పునాది.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }

        lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);
        /*If the border is drawn later disable loading its properties*/
        ఒకవేళ(lv_obj_get_style_border_post_అంతర్గత(obj, LV_PART_MAIN)) {
            draw_dsc.border_post = 1;
        }


        lv_draw_rect(layer, &draw_dsc, &coords);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_DRAW_MAIN_చివర) {
        /*Draw the non backdrop blur when the main content is rendered the the children are not yet */
        lv_layer_t * layer = lv_event_get_layer(e);
        bool backdrop_blur = lv_obj_get_style_blur_backdrop_అంతర్గత(obj, LV_PART_MAIN);
        ఒకవేళ(!backdrop_blur) {
            int32_t w = lv_obj_get_style_transform_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_blur_dsc_t blur_dsc;
            lv_draw_blur_dsc_init(&blur_dsc);
            lv_obj_init_draw_blur_dsc(obj, LV_PART_MAIN, &blur_dsc);
            blur_dsc.మూల_radius = lv_obj_get_style_radius_అంతర్గత(obj, LV_PART_MAIN);
            blur_dsc.పునాది.layer = layer;
            lv_draw_blur(layer, &blur_dsc, &coords);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_DRAW_POST) {
        lv_layer_t * layer = lv_event_get_layer(e);
        draw_scrollbar(obj, layer);

        /*If the border is drawn later disable loading other properties*/
        ఒకవేళ(lv_obj_get_style_border_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN) &&
           lv_obj_get_style_border_post_అంతర్గత(obj, LV_PART_MAIN)) {
            lv_draw_rect_dsc_t draw_dsc;
            lv_draw_rect_dsc_init(&draw_dsc);
            draw_dsc.bg_opa = LV_OPA_TRANSP;
            draw_dsc.bg_image_opa = LV_OPA_TRANSP;
            draw_dsc.outline_opa = LV_OPA_TRANSP;
            draw_dsc.shadow_opa = LV_OPA_TRANSP;
            draw_dsc.పునాది.layer = layer;
            lv_obj_init_draw_rect_dsc(obj, LV_PART_MAIN, &draw_dsc);

            int32_t w = lv_obj_get_style_transform_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
            int32_t h = lv_obj_get_style_transform_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
            lv_area_t coords;
            coords = obj->coords;
            lv_area_increase(&coords, w, h);

            lv_draw_rect(layer, &draw_dsc, &coords);
        }
    }
}

స్థిర శూన్య draw_scrollbar(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ఒకవేళ(lv_area_get_కొలత(&hor_area) <= 0 && lv_area_get_కొలత(&ver_area) <= 0) తిరిగి;

    lv_draw_rect_dsc_t rect_dsc;
    lv_ఫలితం_t sb_res = scrollbar_init_draw_dsc(obj, &rect_dsc);
    ఒకవేళ(sb_res != LV_ఫలితం_OK) తిరిగి;

    bool backdrop_blur = lv_obj_get_style_blur_backdrop_అంతర్గత(obj, LV_PART_SCROLLBAR);
    lv_draw_blur_dsc_t blur_dsc;
    lv_draw_blur_dsc_init(&blur_dsc);
    blur_dsc.మూల_radius = rect_dsc.radius;
    blur_dsc.blur_radius = lv_obj_get_style_blur_radius_అంతర్గత(obj, LV_PART_SCROLLBAR);

    ఒకవేళ(lv_area_get_కొలత(&hor_area) > 0) {
        ఒకవేళ(backdrop_blur) lv_obj_init_draw_blur_dsc(obj, LV_PART_SCROLLBAR, &blur_dsc);
        blur_dsc.పునాది.id1 = 0;
        lv_draw_blur(layer, &blur_dsc, &hor_area);

        rect_dsc.పునాది.id1 = 0;
        lv_draw_rect(layer, &rect_dsc, &hor_area);

        ఒకవేళ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &hor_area);
    }
    ఒకవేళ(lv_area_get_కొలత(&ver_area) > 0) {
        blur_dsc.పునాది.id1 = 1;
        ఒకవేళ(backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);

        rect_dsc.పునాది.id1 = 1;
        lv_draw_rect(layer, &rect_dsc, &ver_area);

        ఒకవేళ(!backdrop_blur) lv_draw_blur(layer, &blur_dsc, &ver_area);
    }
}

/**
 * Initialize the draw descriptor for the scrollbar
 * @param obj pointer to an object
 * @param dsc the draw descriptor to initialize
 * @return LV_RESULT_OK: the scrollbar is visible; LV_RESULT_INVALID: the scrollbar is not visible
 */
స్థిర lv_ఫలితం_t scrollbar_init_draw_dsc(lv_obj_t * obj, lv_draw_rect_dsc_t * dsc)
{
    lv_draw_rect_dsc_init(dsc);
    dsc->bg_opa = lv_obj_get_style_bg_opa_అంతర్గత(obj, LV_PART_SCROLLBAR);
    ఒకవేళ(dsc->bg_opa > LV_OPA_MIN) {
        dsc->bg_color = lv_obj_get_style_bg_color_అంతర్గత(obj, LV_PART_SCROLLBAR);
    }

    dsc->border_opa = lv_obj_get_style_border_opa_అంతర్గత(obj, LV_PART_SCROLLBAR);
    ఒకవేళ(dsc->border_opa > LV_OPA_MIN) {
        dsc->border_వెడల్పు = lv_obj_get_style_border_వెడల్పు_అంతర్గత(obj, LV_PART_SCROLLBAR);
        ఒకవేళ(dsc->border_వెడల్పు > 0) {
            dsc->border_color = lv_obj_get_style_border_color_అంతర్గత(obj, LV_PART_SCROLLBAR);
        }
        లేకపోతే {
            dsc->border_opa = LV_OPA_TRANSP;
        }
    }

    dsc->shadow_opa = lv_obj_get_style_shadow_opa_అంతర్గత(obj, LV_PART_SCROLLBAR);
    ఒకవేళ(dsc->shadow_opa > LV_OPA_MIN) {
        dsc->shadow_వెడల్పు = lv_obj_get_style_shadow_వెడల్పు_అంతర్గత(obj, LV_PART_SCROLLBAR);
        ఒకవేళ(dsc->shadow_వెడల్పు > 0) {
            dsc->shadow_విస్తరణ = lv_obj_get_style_shadow_విస్తరణ_అంతర్గత(obj, LV_PART_SCROLLBAR);
            dsc->shadow_color = lv_obj_get_style_shadow_color_అంతర్గత(obj, LV_PART_SCROLLBAR);
        }
        లేకపోతే {
            dsc->shadow_opa = LV_OPA_TRANSP;
        }
    }

    lv_opa_t opa = lv_obj_get_style_opa_recursive(obj, LV_PART_SCROLLBAR);
    ఒకవేళ(opa < LV_OPA_MAX) {
        lv_opa_t v = LV_OPA_MIX2(dsc->bg_opa, opa);
        dsc->bg_opa = v;
        dsc->border_opa = v;
        dsc->shadow_opa = v;
    }

    ఒకవేళ(dsc->bg_opa != LV_OPA_TRANSP || dsc->border_opa != LV_OPA_TRANSP || dsc->shadow_opa != LV_OPA_TRANSP) {
        dsc->radius = lv_obj_get_style_radius_అంతర్గత(obj, LV_PART_SCROLLBAR);
        తిరిగి LV_ఫలితం_OK;
    }
    లేకపోతే {
        తిరిగి LV_ఫలితం_INVALID;
    }
}

స్థిర శూన్య lv_obj_event(స్థిరరాశి lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_వాడని(class_p);

    lv_event_సంకేతం_t సంకేతం_2 = lv_event_get_సంకేతం(e);
    lv_obj_t * obj = lv_event_get_current_లక్ష్యం(e);
    ఒకవేళ(సంకేతం_2 == LV_EVENT_PRESSED) {
        lv_obj_కూడు_state(obj, LV_STATE_PRESSED);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_RELEASED) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_indev_t * indev = lv_event_get_indev(e);
        /*Go the checked state if enabled*/
        ఒకవేళ((indev == ఖాళీ || lv_indev_get_scroll_obj(indev) == ఖాళీ) && lv_obj_is_checkable(obj)) {

            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ఒకవేళ(!(lv_obj_get_state(obj) & LV_STATE_CHECKED)) {
                lv_obj_కూడు_state(obj, LV_STATE_CHECKED);
            }
            /*Radio buttons can't be checked off directly*/
            లేకపోతే ఒకవేళ(!lv_obj_is_radio_button(obj)) {
                lv_obj_remove_state(obj, LV_STATE_CHECKED);
            }
            ఒకవేళ(was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ఫలితం_t res = lv_obj_send_event(obj, LV_EVENT_విలువ_మారింది, ఖాళీ);
                ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ఖాళీ);
                ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
            }
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_విలువ_మారింది) {
        ఒకవేళ(lv_obj_is_radio_button(obj) && has_state(obj, LV_STATE_CHECKED)) {
            lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
            ఒకవేళ(మాతృ) {
                uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
                uint32_t i;
                కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                    lv_obj_t * sibling = lv_obj_get_బిడ్డ(మాతృ, i);
                    ఒకవేళ(obj == sibling) కొనసాగు;

                    ఒకవేళ(lv_obj_is_radio_button(sibling) && has_state(sibling, LV_STATE_CHECKED)) {
                        lv_obj_remove_state(sibling, LV_STATE_CHECKED);
                        lv_ఫలితం_t res = lv_obj_send_event(sibling, LV_EVENT_విలువ_మారింది, ఖాళీ);
                        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

                        res = lv_obj_send_event(sibling, LV_EVENT_UNCHECKED, ఖాళీ);
                        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
                    }
                }
            }
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_PRESS_కోల్పోయిన) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_STYLE_మారింది) {
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
        కొరకు(uint32_t i = 0; i < బిడ్డ_cnt; i++) {
            lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(బిడ్డ);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_కీలకం) {
        ఒకవేళ(lv_obj_is_checkable(obj)) {
            uint32_t c = lv_event_get_కీలకం(e);
            bool was_checked = has_state(obj, LV_STATE_CHECKED);
            ఒకవేళ(c == LV_కీలకం_కుడి || c == LV_కీలకం_పైకి) {
                lv_obj_కూడు_state(obj, LV_STATE_CHECKED);
            }
            లేకపోతే ఒకవేళ(c == LV_కీలకం_ఎడమ || c == LV_కీలకం_కిందికి) {
                /*Radio buttons can't be checked off directly*/
                ఒకవేళ(!lv_obj_is_radio_button(obj)) {
                    lv_obj_remove_state(obj, LV_STATE_CHECKED);
                }
            }

            /*With Enter LV_EVENT_RELEASED will send VALUE_CHANGE event*/
            ఒకవేళ(c != LV_కీలకం_ENTER && was_checked != has_state(obj, LV_STATE_CHECKED)) {
                lv_ఫలితం_t res = lv_obj_send_event(obj, LV_EVENT_విలువ_మారింది, ఖాళీ);
                ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

                res = lv_obj_send_event(obj, was_checked ? LV_EVENT_UNCHECKED : LV_EVENT_CHECKED, ఖాళీ);
                ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
            }
        }
        లేకపోతే ఒకవేళ(lv_obj_is_scrollable(obj) && lv_obj_is_scroll_with_arrow(obj) && !lv_obj_is_editable(obj)) {
            /*scroll by keypad or encoder*/
            lv_anim_enable_t anim_enable = LV_ANIM_OFF;
            int32_t sl = lv_obj_get_scroll_ఎడమ(obj);
            int32_t sr = lv_obj_get_scroll_కుడి(obj);
            uint32_t c = lv_event_get_కీలకం(e);
            ఒకవేళ(c == LV_కీలకం_కిందికి) {
                /*use scroll_to_x/y functions to enforce scroll limits*/
                lv_obj_scroll_గమ్యం_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ఎత్తు(obj) / 4, anim_enable);
            }
            లేకపోతే ఒకవేళ(c == LV_కీలకం_పైకి) {
                lv_obj_scroll_గమ్యం_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ఎత్తు(obj) / 4, anim_enable);
            }
            లేకపోతే ఒకవేళ(c == LV_కీలకం_కుడి) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ఒకవేళ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_గమ్యం_y(obj, lv_obj_get_scroll_y(obj) + lv_obj_get_ఎత్తు(obj) / 4, anim_enable);
                లేకపోతే
                    lv_obj_scroll_గమ్యం_x(obj, lv_obj_get_scroll_x(obj) + lv_obj_get_వెడల్పు(obj) / 4, anim_enable);
            }
            లేకపోతే ఒకవేళ(c == LV_కీలకం_ఎడమ) {
                /*If the object can't be scrolled horizontally then scroll it vertically*/
                ఒకవేళ(!((lv_obj_get_scroll_dir(obj) & LV_DIR_HOR) && (sl > 0 || sr > 0)))
                    lv_obj_scroll_గమ్యం_y(obj, lv_obj_get_scroll_y(obj) - lv_obj_get_ఎత్తు(obj) / 4, anim_enable);
                లేకపోతే
                    lv_obj_scroll_గమ్యం_x(obj, lv_obj_get_scroll_x(obj) - lv_obj_get_వెడల్పు(obj) / 4, anim_enable);
            }
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_FOCUSED) {
        ఒకవేళ(lv_obj_is_scroll_on_focus(obj)) {
            lv_obj_scroll_గమ్యం_view_recursive(obj, LV_ANIM_ON);
        }

        lv_సమూహం_t * సమూహం = lv_obj_get_సమూహం(obj);
        స్థిరరాశి bool editing = సమూహం && lv_సమూహం_get_editing(సమూహం);
        lv_state_t state = LV_STATE_FOCUSED;

        /* Use the indev for then indev handler.
         * But if the obj was focused manually it returns NULL so try to
         * use the indev from the event*/
        lv_indev_t * indev = lv_indev_చురుకైన();
        ఒకవేళ(indev == ఖాళీ) indev = lv_event_get_indev(e);

        lv_indev_type_t indev_type = indev != ఖాళీ ? lv_indev_get_type(indev) : LV_INDEV_TYPE_NONE;
        ఒకవేళ(indev_type == LV_INDEV_TYPE_KEYPAD || indev_type == LV_INDEV_TYPE_ENCODER) state |= LV_STATE_FOCUS_కీలకం;
        ఒకవేళ(editing) {
            state |= LV_STATE_EDITED;
            lv_obj_కూడు_state(obj, state);
        }
        లేకపోతే {
            lv_obj_కూడు_state(obj, state);
            lv_obj_remove_state(obj, LV_STATE_EDITED);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_SCROLL_BEGIN) {
        lv_obj_కూడు_state(obj, LV_STATE_SCROLLED);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_SCROLL_చివర) {
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
        ఒకవేళ(lv_obj_get_scrollbar_బహుళకం(obj) == LV_SCROLLBAR_బహుళకం_చురుకైన) {
            lv_area_t hor_area, ver_area;
            lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);
            lv_obj_invalidate_area(obj, &hor_area);
            lv_obj_invalidate_area(obj, &ver_area);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_state(obj, LV_STATE_FOCUSED | LV_STATE_EDITED | LV_STATE_FOCUS_కీలకం);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_కొలత_మారింది) {
        int32_t align = lv_obj_get_style_align_అంతర్గత(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_అంతర్గత(obj, LV_PART_MAIN);
        ఒకవేళ(layout || align) {
            lv_obj_mark_layout_as_dirty(obj);
        }

        uint32_t i;
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
        కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
            lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
            lv_obj_mark_layout_as_dirty(బిడ్డ);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_బిడ్డ_మారింది) {
        int32_t align = lv_obj_get_style_align_అంతర్గత(obj, LV_PART_MAIN);
        uint16_t layout = lv_obj_get_style_layout_అంతర్గత(obj, LV_PART_MAIN);
        ఒకవేళ(layout || align || lv_obj_is_style_any_వెడల్పు_content(obj) || lv_obj_is_style_any_ఎత్తు_content(obj)) {
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_బిడ్డ_DELETED) {
        obj->readjust_scroll_after_layout = 1;
        lv_obj_mark_layout_as_dirty(obj);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_REFR_EXT_DRAW_కొలత) {
        int32_t d = lv_obj_calculate_ext_draw_కొలత(obj, LV_PART_MAIN);
        lv_event_సమితి_ext_draw_కొలత(e, d);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_DRAW_MAIN || సంకేతం_2 == LV_EVENT_DRAW_POST || సంకేతం_2 == LV_EVENT_DRAW_MAIN_చివర ||
            సంకేతం_2 == LV_EVENT_COVER_CHECK) {
        lv_obj_draw(e);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_INDEV_RESET) {
        lv_obj_remove_state(obj, LV_STATE_PRESSED);
        lv_obj_remove_state(obj, LV_STATE_SCROLLED);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_HOVER_OVER) {
        lv_obj_కూడు_state(obj, LV_STATE_HOVERED);
    }
    లేకపోతే ఒకవేళ(సంకేతం_2 == LV_EVENT_HOVER_LEAVE) {
        lv_obj_remove_state(obj, LV_STATE_HOVERED);
    }
}

/**
 * Set the state (fully overwrite) of an object.
 * If specified in the styles, transition animations will be started from the previous state to the current.
 * @param obj       pointer to an object
 * @param new_state the new state
 */
స్థిర శూన్య update_obj_state(lv_obj_t * obj, lv_state_t new_state)
{
    LV_ASSERT(obj != ఖాళీ);

    ఒకవేళ(obj->state == new_state) తిరిగి;


    lv_state_t prev_state = obj->state;

    lv_part_t మారింది_part = LV_PART_ANY;
    lv_style_state_cmp_t cmp_res = lv_obj_style_state_compare(obj, prev_state, new_state, &మారింది_part);
    /*If there is no difference in styles there is nothing else to do*/
    ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_SAME) {
        obj->state = new_state;
        lv_obj_send_event(obj, LV_EVENT_STATE_మారింది, &prev_state);
        తిరిగి;
    }

    /*Invalidate the object in their current state*/
    lv_obj_invalidate(obj);

    obj->state = new_state;
    lv_obj_update_layer_type(obj);
    lv_obj_update_blur_స్థితి(obj);

    /*Skip transitions if the widget is not rendered yet. */
    ఒకవేళ(!obj->rendered) {
        lv_obj_invalidate(obj);
        ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
            lv_obj_refresh_ext_draw_కొలత(obj);
        }

        lv_obj_send_event(obj, LV_EVENT_STATE_మారింది, &prev_state);
        తిరిగి;
    }

    obj_transition_స్థితులు(obj, prev_state, new_state);

    ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW && మారింది_part == LV_PART_SCROLLBAR) {
        /*A redraw-only change confined to the scrollbar: refresh just that part.
         *lv_obj_refresh_style() skips the child-subtree refresh for
         *LV_PART_SCROLLBAR, avoiding a full-subtree style cascade. This is safe
         *because the scrollbar is drawn by lv_obj and has no styled children.*/
        lv_obj_refresh_style(obj, LV_PART_SCROLLBAR, LV_STYLE_PROP_ANY);
    }
    లేకపోతే ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_DIFF_REDRAW) {
        /*Invalidation is not enough, e.g. layer type needs to be updated too*/
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    లేకపోతే ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_DIFF_LAYOUT) {
        lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
    }
    లేకపోతే ఒకవేళ(cmp_res == LV_STYLE_STATE_CMP_DIFF_DRAW_PAD) {
        lv_obj_invalidate(obj);
        lv_obj_refresh_ext_draw_కొలత(obj);
    }

    lv_obj_send_event(obj, LV_EVENT_STATE_మారింది, &prev_state);
}

స్థిర శూన్య obj_transition_స్థితులు(lv_obj_t * obj, lv_state_t prev_state, lv_state_t new_state)
{

    lv_obj_style_transition_dsc_t * ts = lv_malloc(పరిమాణం(lv_obj_style_transition_dsc_t) * STYLE_TRANSITION_MAX);
    LV_ASSERT_MALLOC(ts);
    ఒకవేళ(!ts) {
        LV_LOG_WARN("No memory to compute state transitions");
        తిరిగి;
    }
    uint32_t tsi = 0;
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt && tsi < STYLE_TRANSITION_MAX; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ఒకవేళ(state_act & (~new_state)) కొనసాగు; /*Skip unrelated styles*/
        ఒకవేళ(obj_style->is_trans) కొనసాగు;

        lv_style_విలువ_t v;
        ఒకవేళ(lv_style_get_prop_అంతర్గత(obj_style->style, LV_STYLE_TRANSITION, &v) != LV_STYLE_RES_దొరికింది) కొనసాగు;
        స్థిరరాశి lv_style_transition_dsc_t * tr = v.ptr;

        /*Add the props to the set if not added yet or added but with smaller weight*/
        uint32_t j;
        కొరకు(j = 0; tr->props[j] != 0 && tsi < STYLE_TRANSITION_MAX; j++) {
            uint32_t t;
            కొరకు(t = 0; t < tsi; t++) {
                lv_style_selector_t selector = ts[t].selector;
                lv_state_t state_ts = lv_obj_style_get_selector_state(selector);
                lv_part_t part_ts = lv_obj_style_get_selector_part(selector);
                ఒకవేళ(ts[t].prop == tr->props[j] && part_ts == part_act && state_ts >= state_act) ఆపు;
            }

            /*If not found  add it*/
            ఒకవేళ(t == tsi) {
                ts[tsi].సమయం = tr->సమయం;
                ts[tsi].delay = tr->delay;
                ts[tsi].మార్గం_cb = tr->మార్గం_xcb;
                ts[tsi].prop = tr->props[j];
                ts[tsi].user_దత్తాంశం = tr->user_దత్తాంశం;
                ts[tsi].selector = obj_style->selector;
                tsi++;
            }
        }
    }

    కొరకు(i = 0; i < tsi; i++) {
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
స్థిర శూన్య lv_obj_children_కూడు_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t బిడ్డ_లెక్క = lv_obj_get_బిడ్డ_లెక్క(obj);

    కొరకు(uint32_t i = 0; i < బిడ్డ_లెక్క; i++) {
        lv_obj_t * బిడ్డ = lv_obj_get_బిడ్డ(obj, i);
        ఒకవేళ(బిడ్డ) {
            lv_obj_కూడు_state(బిడ్డ, state);
        }
    }
}

/**
 * Remove the state from the children of the object
 * @param obj pointer to an object
 * @param state the state to remove
 */
స్థిర శూన్య lv_obj_children_remove_state(lv_obj_t * obj, lv_state_t state)
{
    uint32_t బిడ్డ_లెక్క = lv_obj_get_బిడ్డ_లెక్క(obj);

    కొరకు(uint32_t i = 0; i < బిడ్డ_లెక్క; i++) {
        lv_obj_t * బిడ్డ = lv_obj_get_బిడ్డ(obj, i);
        ఒకవేళ(బిడ్డ) {
            lv_obj_remove_state(బిడ్డ, state);
        }
    }
}

స్థిర శూన్య null_on_delete_cb(lv_event_t * e)
{
    lv_obj_t ** obj_ptr = lv_event_get_user_దత్తాంశం(e);
    *obj_ptr = ఖాళీ;
}

స్థిర శూన్య screen_ఎక్కించు_on_trigger_event_cb(lv_event_t * e)
{
    screen_ఎక్కించు_anim_dsc_t * dsc = lv_event_get_user_దత్తాంశం(e);
    LV_ASSERT_NULL(dsc);
    lv_screen_ఎక్కించు_anim(dsc->లక్ష్యం.screen, dsc->anim_type, dsc->duration, dsc->delay, false);
}

స్థిర శూన్య screen_create_on_trigger_event_cb(lv_event_t * e)
{
    screen_ఎక్కించు_anim_dsc_t * dsc = lv_event_get_user_దత్తాంశం(e);
    LV_ASSERT_NULL(dsc);

    lv_obj_t * screen = dsc->లక్ష్యం.create_cb();
    lv_screen_ఎక్కించు_anim(screen, dsc->anim_type, dsc->duration, dsc->delay, false);
    lv_obj_కూడు_event_cb(screen, delete_on_screen_unloaded_event_cb, LV_EVENT_SCREEN_UNLOADED, ఖాళీ);
}

స్థిర శూన్య play_timeline_on_trigger_event_cb(lv_event_t * e)
{
    timeline_play_dsc_t * dsc = lv_event_get_user_దత్తాంశం(e);
    LV_ASSERT_NULL(dsc);

    /*Reset the progress only if the animation was finished*/
    uint16_t పురోగతి = lv_anim_timeline_get_పురోగతి(dsc->at);
    ఒకవేళ(dsc->తిరగేయి) {
        ఒకవేళ(పురోగతి == 0) {
            lv_anim_timeline_సమితి_delay(dsc->at, dsc->delay);
            lv_anim_timeline_సమితి_పురోగతి(dsc->at, LV_ANIM_TIMELINE_పురోగతి_MAX);
        }
    }
    లేకపోతే {
        ఒకవేళ(పురోగతి == LV_ANIM_TIMELINE_పురోగతి_MAX) {
            lv_anim_timeline_సమితి_delay(dsc->at, dsc->delay);
            lv_anim_timeline_సమితి_పురోగతి(dsc->at, 0);
        }

    }
    lv_anim_timeline_సమితి_తిరగేయి(dsc->at, dsc->తిరగేయి);
    lv_anim_timeline_ప్రారంభం(dsc->at);
}


స్థిర శూన్య delete_on_screen_unloaded_event_cb(lv_event_t * e)
{
    lv_obj_delete(lv_event_get_లక్ష్యం_obj(e));
}

స్థిర శూన్య call_delete_cb(lv_event_t * e)
{
    LV_ASSERT(e != ఖాళీ);
    lv_obj_t * obj = lv_event_get_లక్ష్యం_obj(e);
    lv_delete_dsc_t * dsc = lv_event_get_user_దత్తాంశం(e);
#ఒకవేళ !LV_USE_ASSERT
    LV_వాడని(obj);
#అయితే_ముగింపు
    LV_ASSERT(dsc != ఖాళీ);
    LV_ASSERT(dsc->cb != ఖాళీ);
    LV_ASSERT(dsc->obj == obj);

    dsc->cb(dsc->user_దత్తాంశం);
    lv_obj_remove_delete_cb(dsc);
}

స్థిర అంతఃవరుస శూన్య సమితి_state(lv_obj_t * obj, lv_state_t state,  bool విలువ)
{
    LV_ASSERT(obj != ఖాళీ);
    ఒకవేళ(విలువ) lv_obj_కూడు_state(obj, state);
    లేకపోతే lv_obj_remove_state(obj, state);
}

స్థిర అంతఃవరుస bool has_state(స్థిరరాశి lv_obj_t * obj, lv_state_t state)
{
    LV_ASSERT(obj != ఖాళీ);

    తిరిగి !!(obj->state & state);
}


#ఒకవేళ LV_USE_CHECK_OBJ_మాతృ_LINK && LV_USE_ASSERT
స్థిర bool మాతృ_కలిగియుంది_బిడ్డ(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి lv_obj_t * బిడ్డ)
{
    uint32_t బిడ్డ_cnt = మాతృ->spec_attr ? మాతృ->spec_attr->బిడ్డ_cnt : 0;
    uint32_t i;
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        ఒకవేళ(మాతృ->spec_attr->children[i] == బిడ్డ) తిరిగి true;
    }
    తిరిగి false;
}
#అయితే_ముగింపు


#ఒకవేళ LV_USE_OBJ_PROPERTY
స్థిర lv_బిందువు_t lv_obj_get_scroll_చివర_helper(lv_obj_t * obj)
{
    lv_బిందువు_t బిందువు;
    lv_obj_get_scroll_చివర(obj, &బిందువు);
    తిరిగి బిందువు;
}

స్థిర శూన్య obj_సమితి_flag(lv_obj_t * obj, lv_obj_flag_t f, bool v)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(f >= LV_OBJ_FLAG_LAYOUT_1) lv_obj_allocate_spec_attr(obj);

    ఒకవేళ(f & LV_OBJ_FLAG_HIDDEN)                lv_obj_సమితి_hidden(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_CLICKABLE)             lv_obj_సమితి_clickable(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_CLICK_FOCUSABLE)       lv_obj_సమితి_click_focusable(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_CHECKABLE)             lv_obj_సమితి_checkable(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLLABLE)            lv_obj_సమితి_scrollable(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_ELASTIC)        lv_obj_సమితి_scroll_elastic(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_MOMENTUM)       lv_obj_సమితి_scroll_momentum(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_ONE)            lv_obj_సమితి_scroll_one(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR)      lv_obj_సమితి_scroll_chain_hor(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_CHAIN_VER)      lv_obj_సమితి_scroll_chain_ver(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_ON_FOCUS)       lv_obj_సమితి_scroll_on_focus(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SCROLL_WITH_ARROW)     lv_obj_సమితి_scroll_with_arrow(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SNAPPABLE)             lv_obj_సమితి_snappable(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_PRESS_తాళం)            lv_obj_సమితి_press_తాళం(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_EVENT_BUBBLE)          lv_obj_సమితి_event_bubble(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_GESTURE_BUBBLE)        lv_obj_సమితి_gesture_bubble(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_ADV_HITTEST)           lv_obj_సమితి_adv_hittest(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_IGNORE_LAYOUT)         lv_obj_సమితి_ignore_layout(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_FLOATING)              lv_obj_సమితి_floating(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) lv_obj_సమితి_send_draw_task_events(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_అధికప్రవాహం_VISIBLE)      lv_obj_సమితి_అధికప్రవాహం_visible(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_EVENT_TRICKLE)         lv_obj_సమితి_event_trickle(obj, v);
    ఒకవేళ(f & LV_OBJ_FLAG_STATE_TRICKLE)         lv_obj_సమితి_state_trickle(obj, v);

    /*The layout, widget and user flags live in spec_attr->user_flags, bits 0..7*/
    uint32_t user_బిట్లు = 0;
    ఒకవేళ(f & LV_OBJ_FLAG_LAYOUT_1)              user_బిట్లు |= 1 << 0;
    ఒకవేళ(f & LV_OBJ_FLAG_LAYOUT_2)              user_బిట్లు |= 1 << 1;
    ఒకవేళ(f & LV_OBJ_FLAG_WIDGET_1)              user_బిట్లు |= 1 << 2;
    ఒకవేళ(f & LV_OBJ_FLAG_WIDGET_2)              user_బిట్లు |= 1 << 3;
    ఒకవేళ(f & LV_OBJ_FLAG_USER_1)                user_బిట్లు |= 1 << 4;
    ఒకవేళ(f & LV_OBJ_FLAG_USER_2)                user_బిట్లు |= 1 << 5;
    ఒకవేళ(f & LV_OBJ_FLAG_USER_3)                user_బిట్లు |= 1 << 6;
    ఒకవేళ(f & LV_OBJ_FLAG_USER_4)                user_బిట్లు |= 1 << 7;
    ఒకవేళ(user_బిట్లు == 0) తిరిగి;

    uint32_t old_user_flags = obj->spec_attr->user_flags;
    ఒకవేళ(v) {
        obj->spec_attr->user_flags |= user_బిట్లు;
    }
    లేకపోతే {
        obj->spec_attr->user_flags &= ~user_బిట్లు;
    }

    స్థిరరాశి uint8_t layout_ముసుగు = 0x3;
    స్థిరరాశి bool layout_flag_మారింది = (old_user_flags ^ obj->spec_attr->user_flags) & layout_ముసుగు;
    ఒకవేళ(layout_flag_మారింది) {
        lv_obj_mark_layout_as_dirty(lv_obj_get_మాతృ(obj));
        lv_obj_mark_layout_as_dirty(obj);
    }
}

స్థిర bool obj_has_flag_any(స్థిరరాశి lv_obj_t * obj, lv_obj_flag_t f)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    ఒకవేళ((f & LV_OBJ_FLAG_HIDDEN) && obj->hidden) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_CLICKABLE) && obj->clickable) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_CLICK_FOCUSABLE) && obj->click_focusable) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_CHECKABLE) && obj->checkable) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLLABLE) && obj->scrollable) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_ELASTIC) && obj->scroll_elastic) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_MOMENTUM) && obj->scroll_momentum) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_ONE) && obj->scroll_one) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_CHAIN_HOR) && obj->scroll_chain_hor) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_CHAIN_VER) && obj->scroll_chain_ver) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_ON_FOCUS) && obj->scroll_on_focus) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SCROLL_WITH_ARROW) && obj->scroll_with_arrow) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SNAPPABLE) && obj->snappable) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_PRESS_తాళం) && obj->press_తాళం) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_EVENT_BUBBLE) && obj->event_bubble) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_GESTURE_BUBBLE) && obj->gesture_bubble) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_ADV_HITTEST) && obj->adv_hittest) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_IGNORE_LAYOUT) && obj->ignore_layout) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_FLOATING) && obj->floating) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS) && obj->send_draw_task_events) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_అధికప్రవాహం_VISIBLE) && obj->అధికప్రవాహం_visible) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_EVENT_TRICKLE) && obj->event_trickle) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_STATE_TRICKLE) && obj->state_trickle) తిరిగి true;

    ఒకవేళ(f >= LV_OBJ_FLAG_LAYOUT_1 && obj->spec_attr == ఖాళీ) తిరిగి false;

    ఒకవేళ((f & LV_OBJ_FLAG_LAYOUT_1) && (obj->spec_attr->user_flags & (1 << 0))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_LAYOUT_2) && (obj->spec_attr->user_flags & (1 << 1))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_WIDGET_1) && (obj->spec_attr->user_flags & (1 << 2))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_WIDGET_2) && (obj->spec_attr->user_flags & (1 << 3))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_USER_1) && (obj->spec_attr->user_flags & (1 << 4))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_USER_2) && (obj->spec_attr->user_flags & (1 << 5))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_USER_3) && (obj->spec_attr->user_flags & (1 << 6))) తిరిగి true;
    ఒకవేళ((f & LV_OBJ_FLAG_USER_4) && (obj->spec_attr->user_flags & (1 << 7))) తిరిగి true;

    తిరిగి false;
}

స్థిర lv_ఫలితం_t lv_obj_సమితి_any(lv_obj_t * obj, lv_prop_id_t id, స్థిరరాశి lv_property_t * prop)
{
    LV_ASSERT(obj != ఖాళీ);

    ఒకవేళ(id >= LV_PROPERTY_OBJ_FLAG_ప్రారంభం && id <= LV_PROPERTY_OBJ_FLAG_చివర) {
        /*The flag properties are kept for backward compatibility; the dedicated
         *per-flag setters are registered as normal properties (see lv_obj_properties).*/
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ప్రారంభం);
        obj_సమితి_flag(obj, flag, prop->అంకె != 0);
        తిరిగి LV_ఫలితం_OK;
    }
    లేకపోతే ఒకవేళ(id >= LV_PROPERTY_OBJ_STATE_ప్రారంభం && id <= LV_PROPERTY_OBJ_STATE_చివర) {
        lv_state_t state = 1L << (id - LV_PROPERTY_OBJ_STATE_ప్రారంభం);
        ఒకవేళ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            state = LV_STATE_ANY;
        }

        ఒకవేళ(prop->అంకె) lv_obj_కూడు_state(obj, state);
        లేకపోతే lv_obj_remove_state(obj, state);
        తిరిగి LV_ఫలితం_OK;
    }
    లేకపోతే {
        తిరిగి LV_ఫలితం_INVALID;
    }
}

స్థిర lv_ఫలితం_t lv_obj_get_any(స్థిరరాశి lv_obj_t * obj, lv_prop_id_t id, lv_property_t * prop)
{
    LV_ASSERT(obj != ఖాళీ);
    ఒకవేళ(id >= LV_PROPERTY_OBJ_FLAG_ప్రారంభం && id <= LV_PROPERTY_OBJ_FLAG_చివర) {
        lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_FLAG_ప్రారంభం);
        prop->id = id;
        prop->అంకె = obj_has_flag_any(obj, flag);
        తిరిగి LV_ఫలితం_OK;
    }
    లేకపోతే ఒకవేళ(id >= LV_PROPERTY_OBJ_STATE_ప్రారంభం && id <= LV_PROPERTY_OBJ_STATE_చివర) {
        prop->id = id;
        ఒకవేళ(id == LV_PROPERTY_OBJ_STATE_ANY) {
            prop->అంకె = obj->state;
        }
        లేకపోతే {
            lv_obj_flag_t flag = 1L << (id - LV_PROPERTY_OBJ_STATE_ప్రారంభం);
            prop->అంకె = obj->state & flag;
        }
        తిరిగి LV_ఫలితం_OK;
    }
    లేకపోతే {
        తిరిగి LV_ఫలితం_INVALID;
    }
}
#అయితే_ముగింపు /*LV_USE_OBJ_PROPERTY*/

