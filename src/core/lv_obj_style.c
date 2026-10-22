/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_anim_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_bidi_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_style_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "lv_observer_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#define style_refr LV_GLOBAL_DEFAULT()->style_refresh
#define style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#define _style_custom_prop_flag_lookup_table LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_table
#define STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#define DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_ಮೌಲ್ಯ_t start_ಮೌಲ್ಯ;
    lv_style_ಮೌಲ್ಯ_t end_ಮೌಲ್ಯ;
} trans_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಧ್ರುವ lv_style_t * style;
    lv_style_selector_t selector;
    int32_t ಮೌಲ್ಯ;
} bind_style_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
ಸ್ಥಿರ lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
ಸ್ಥಿರ lv_style_res_t get_prop_core(ಧ್ರುವ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ಮೌಲ್ಯ_t * v);
ಸ್ಥಿರ ಶೂನ್ಯ report_style_change_core(ಶೂನ್ಯ * style, lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ refresh_children_style(lv_obj_t * obj);
ಸ್ಥಿರ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_cb(ಶೂನ್ಯ * _tr, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_start_cb(lv_anim_t * a);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_completed_cb(lv_anim_t * a);
ಸ್ಥಿರ lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
ಸ್ಥಿರ bool calculate_has_blur(lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ full_cache_refresh(lv_obj_t * obj, lv_part_t part);
ಸ್ಥಿರ ಶೂನ್ಯ fade_anim_cb(ಶೂನ್ಯ * obj, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ fade_in_anim_completed(lv_anim_t * a);
ಸ್ಥಿರ bool style_has_flag(ಧ್ರುವ lv_style_t * style, uint32_t flag);
ಸ್ಥಿರ lv_style_res_t get_selector_style_prop(ಧ್ರುವ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ_act);
ಸ್ಥಿರ ಶೂನ್ಯ remove_style_core(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector, bool theme_only);
ಸ್ಥಿರ lv_obj_style_t * find_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector);
#ಯದಿ LV_USE_OBSERVER
    ಸ್ಥಿರ ಶೂನ್ಯ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    ಸ್ಥಿರ ಶೂನ್ಯ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_obj_style_init(ಶೂನ್ಯ)
{
    lv_ll_init(style_trans_ll_p, ಪರಿಮಾಣ(trans_t));
}

ಶೂನ್ಯ lv_obj_style_deinit(ಶೂನ್ಯ)
{
    lv_ll_clear(style_trans_ll_p);
    ಯದಿ(_style_custom_prop_flag_lookup_table != ರಿಕ್ತ) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = ರಿಕ್ತ;
    }
}

ಶೂನ್ಯ lv_obj_add_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG_ಸಂದೇಶ(obj->style_cnt < 63, ಪ್ರತ್ಯಾಗಮ,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಯದಿ(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ರಿಕ್ತ);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_trans) ಅನುವರ್ತ;
        ಯದಿ(obj->styles[i].is_local) ಅನುವರ್ತ;
        ಭಙ್ಗ;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * ಪರಿಮಾಣ(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    ಪ್ರತಿ(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], ಪರಿಮಾಣ(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    ಯದಿ(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#ಯದಿ LV_OBJ_STYLE_CACHE
    ಅನ್ಯಥಾ {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        ಯದಿ(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
            ಪ್ರತಿ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        ಅನ್ಯಥಾ {
            lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಪರಿಮಾಣ(lv_style_ಮೌಲ್ಯ_t);
            ಪ್ರತಿ(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * old_style, ಧ್ರುವ lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    LV_CHECK_ARG(old_style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    LV_CHECK_ARG(new_style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    LV_CHECK_ARG(old_style != new_style, ಪ್ರತ್ಯಾಗಮ false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಯದಿ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ರಿಕ್ತ);

    bool replaced = false;
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        ಯದಿ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ಅನುವರ್ತ;
        }

        /*Skip non-matching styles*/
        ಯದಿ((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            ಅನುವರ್ತ;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], ಪರಿಮಾಣ(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    ಯದಿ(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    ಪ್ರತ್ಯಾಗಮ replaced;
}

ಶೂನ್ಯ lv_obj_remove_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    remove_style_core(obj, style, selector, false);
}

ಶೂನ್ಯ lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    remove_style_core(obj, ರಿಕ್ತ, selector, true);
}

ಶೂನ್ಯ lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    remove_style_core(obj, ರಿಕ್ತ, LV_PART_ANY | LV_STATE_ANY, false);
}

ಶೂನ್ಯ lv_obj_report_style_change(lv_style_t * style)
{
    ಯದಿ(!style_refr) ಪ್ರತ್ಯಾಗಮ;
    lv_display_t * d = lv_display_get_next(ರಿಕ್ತ);

    ಯಾವತ್(d) {
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

ಶೂನ್ಯ lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(!style_refr) ಪ್ರತ್ಯಾಗಮ;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    ಯದಿ(is_layout_refr) {
        ಯದಿ(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, ರಿಕ್ತ);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ಯದಿ((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        ಯದಿ(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    ಯದಿ((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    ಯದಿ(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    ಯದಿ(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    ಯದಿ(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        ಯದಿ(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

ಶೂನ್ಯ lv_obj_set_style_enabled(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    ಯದಿ(obj_style == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ(en != obj_style->is_disabled) ಪ್ರತ್ಯಾಗಮ; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಯದಿ((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    LV_CHECK_ARG(style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    ಯದಿ(obj_style == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ false;

    ಪ್ರತ್ಯಾಗಮ !obj_style->is_disabled;
}


ಶೂನ್ಯ lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_ಮೌಲ್ಯ_t lv_obj_get_style_prop(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ lv_style_prop_get_default(prop));
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_ಮೌಲ್ಯ_t lv_obj_get_style_prop_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    lv_style_selector_t selector = part | obj->state;
    lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_act = { .ptr = ರಿಕ್ತ };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &ಮೌಲ್ಯ_act);
    ಯದಿ(found == LV_STYLE_RES_FOUND) ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ_act;

    ಪ್ರತ್ಯಾಗಮ lv_style_prop_get_default(prop);
}
lv_style_ಮೌಲ್ಯ_t lv_obj_style_apply_color_filter_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_style_ಮೌಲ್ಯ_t v)
{
#ಯದಿ LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ರಿಕ್ತ);

    ಧ್ರುವ lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    ಯದಿ(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        ಯದಿ(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#ಅನ್ಯಥಾ
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    ಪ್ರತ್ಯಾಗಮ v;
}

bool lv_obj_has_style_prop(ಧ್ರುವ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);

    lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_act = { .ptr = ರಿಕ್ತ };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &ಮೌಲ್ಯ_act);
    ಯದಿ(found == LV_STYLE_RES_FOUND) ಪ್ರತ್ಯಾಗಮ true;

    ಪ್ರತ್ಯಾಗಮ false;
}

ಶೂನ್ಯ lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    ಯದಿ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ರಿಕ್ತ);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, ಮೌಲ್ಯ);

    ಯದಿ(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#ಯದಿ LV_OBJ_STYLE_CACHE
    ಅನ್ಯಥಾ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        ಯದಿ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        ಅನ್ಯಥಾ {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ಪ್ರತ್ಯಾಗಮ lv_style_get_prop_internal(obj->styles[i].style, prop, ಮೌಲ್ಯ);
        }
    }

    ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);

    uint32_t i;
    /*Find the style*/
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ಭಙ್ಗ;
        }
    }

    /*The style is not found*/
    ಯದಿ(i == obj->style_cnt) ಪ್ರತ್ಯಾಗಮ false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    ಯದಿ(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ಧ್ರುವ lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(tr_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_ಮೌಲ್ಯ_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_ಮೌಲ್ಯ_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    ಯದಿ(v1.ptr == v2.ptr && v1.ಅಂಕೆ == v2.ಅಂಕೆ && lv_color_eq(v1.color, v2.color))  ಪ್ರತ್ಯಾಗಮ;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    ಯದಿ(tr_dsc->prop == LV_STYLE_RADIUS) {
        ಯದಿ(v1.ಅಂಕೆ == LV_RADIUS_CIRCLE || v2.ಅಂಕೆ == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            ಯದಿ(v1.ಅಂಕೆ == LV_RADIUS_CIRCLE) v1.ಅಂಕೆ = LV_MIN(whalf + 1, hhalf + 1);
            ಯದಿ(v2.ಅಂಕೆ == LV_RADIUS_CIRCLE) v2.ಅಂಕೆ = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    ಯದಿ(tr == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;
    tr->start_ಮೌಲ್ಯ = v1;
    tr->end_ಮೌಲ್ಯ = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_ಮೌಲ್ಯಗಳು(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->time);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_ಮೌಲ್ಯ_t lv_obj_style_apply_color_filter(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_style_ಮೌಲ್ಯ_t v)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ v);
    ಪ್ರತ್ಯಾಗಮ lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_STYLE_STATE_CMP_SAME);

    lv_style_state_cmp_t res = LV_STYLE_STATE_CMP_SAME;

    /*Accumulate which part the differing styles belong to: the lone differing
     *part, or LV_PART_ANY once styles from more than one part differ. Reported
     *via changed_part so the caller can scope the refresh to a single part.
     *diff_part_found is needed as a separate flag because a style's part can
     *itself be LV_PART_ANY, so LV_PART_ANY can't double as "none seen yet".*/
    lv_part_t diff_part = LV_PART_ANY;
    bool diff_part_found = false;

    /*Are there any new styles for the new state?*/
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_trans) ಅನುವರ್ತ;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        ಯದಿ(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            ಯದಿ(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            ಅನ್ಯಥಾ ಯದಿ(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ಧ್ರುವ lv_style_t * style = obj->styles[i].style;
            lv_style_ಮೌಲ್ಯ_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_ತುದಿ, &v))layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_ಎಡ, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_ಬಲ, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            ಯದಿ(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                ಯದಿ(changed_part) *changed_part = LV_PART_ANY;
                ಪ್ರತ್ಯಾಗಮ LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(lv_style_get_prop_internal(style, LV_STYLE_ಸಾಲು_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಅನ್ಯಥಾ ಯದಿ(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    ಯದಿ(changed_part) *changed_part = diff_part;
    ಪ್ರತ್ಯಾಗಮ res;
}

ಶೂನ್ಯ lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_ಮೌಲ್ಯಗಳು(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

ಶೂನ್ಯ lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_ಮೌಲ್ಯಗಳು(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, ಧ್ರುವ ವರ್ಣ * txt)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_TEXT_ALIGN_AUTO);
    ಪ್ರತ್ಯಾಗಮ lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    ಯದಿ(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        ಪ್ರತ್ಯಾಗಮ LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    ಯದಿ(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    ಯದಿ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ಅನ್ಯಥಾ {
        obj = lv_obj_get_parent(obj);
    }

    ಯಾವತ್(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        ಯದಿ(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            ಪ್ರತ್ಯಾಗಮ LV_OPA_TRANSP;
        }
        ಯದಿ(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    ಯದಿ(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        ಪ್ರತ್ಯಾಗಮ LV_OPA_TRANSP;
    }

    ಯದಿ(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        ಪ್ರತ್ಯಾಗಮ LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    ಪ್ರತ್ಯಾಗಮ opa_final;
}

ಶೂನ್ಯ lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    ಯದಿ(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    ಅನ್ಯಥಾ ಯದಿ(layer_type != LV_LAYER_TYPE_NONE) {
        ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
            ಪ್ರತ್ಯಾಗಮ;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

ಶೂನ್ಯ lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    ಯದಿ(obj->is_deleting) ಪ್ರತ್ಯಾಗಮ;

    bool has_blur = calculate_has_blur(obj);
    ಯದಿ(has_blur == (bool)obj->has_blur) ಪ್ರತ್ಯಾಗಮ;

    obj->has_blur = has_blur;
    ಯದಿ(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    ಅನ್ಯಥಾ {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    ಯದಿ(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    ಪ್ರತ್ಯಾಗಮ color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    ಯದಿ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ಅನ್ಯಥಾ {
        obj = lv_obj_get_parent(obj);
    }

    ಯಾವತ್(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    ಪ್ರತ್ಯಾಗಮ result;
}

ಶೂನ್ಯ lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_pad_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_ಬಲ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_pad_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_ಬಲ(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_pad_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_margin_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_margin_ಬಲ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_margin_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_margin_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_margin_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_margin_ಬಲ(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_margin_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_margin_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_pad_row(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_pad_column(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

ಶೂನ್ಯ lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ);
    lv_obj_set_style_transform_scale_x(obj, ಮೌಲ್ಯ, selector);
    lv_obj_set_style_transform_scale_y(obj, ಮೌಲ್ಯ, selector);
}

int32_t lv_obj_get_style_space_ಎಡ(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_space_ಎಡ_internal(obj, part);
}

int32_t lv_obj_get_style_space_ಬಲ(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_space_ಬಲ_internal(obj, part);
}

int32_t lv_obj_get_style_space_ತುದಿ(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_space_ತುದಿ_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಪ್ರತ್ಯಾಗಮ 0)
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#ಯದಿ LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(style != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(ಪರಿಮಾಣ(bind_style_t));
    LV_ASSERT_MALLOC(p);
    ಯದಿ(p == ರಿಕ್ತ) {
        LV_LOG_WARN("Out of memory");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    p->style = style;
    p->selector = selector;
    p->ಮೌಲ್ಯ = ref_ಮೌಲ್ಯ;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    ಪ್ರತ್ಯಾಗಮ observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(subject != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಯದಿ(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    bind_style_prop_t * p = lv_malloc(ಪರಿಮಾಣ(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    ಯದಿ(p == ರಿಕ್ತ) {
        LV_LOG_WARN("Out of memory");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    ಪ್ರತ್ಯಾಗಮ observable;
}


#endif /*LV_USE_OBSERVER*/


/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Get the local style of an object for a given part and for a given state.
 * If the local style for the part-state pair doesn't exist allocate and return it.
 * @param obj pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the local style
 */
ಸ್ಥಿರ lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ಪ್ರತ್ಯಾಗಮ (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(ಪರಿಮಾಣ(lv_style_t));
    ಯದಿ(new_style == ರಿಕ್ತ) {
        LV_LOG_WARN("couldn't allocate local style");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ಪರಿಮಾಣ(lv_obj_style_t));
    ಯದಿ(new_styles == ರಿಕ್ತ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    obj->styles = new_styles;

    ಪ್ರತಿ(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        ಯದಿ(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ಭಙ್ಗ;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], ಪರಿಮಾಣ(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    ಪ್ರತ್ಯಾಗಮ (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
ಸ್ಥಿರ lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_trans && obj->styles[i].selector == selector) ಭಙ್ಗ;
    }

    /*Already have a transition style for it*/
    ಯದಿ(i != obj->style_cnt) ಪ್ರತ್ಯಾಗಮ &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(ಪರಿಮಾಣ(lv_style_t));
    ಯದಿ(new_style == ರಿಕ್ತ) {
        LV_LOG_WARN("couldn't allocate transition style");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ಪರಿಮಾಣ(lv_obj_style_t));
    ಯದಿ(new_styles == ರಿಕ್ತ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    obj->styles = new_styles;

    ಪ್ರತಿ(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], ಪರಿಮಾಣ(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    ಪ್ರತ್ಯಾಗಮ &obj->styles[0];
}

ಸ್ಥಿರ lv_style_res_t get_prop_core(ಧ್ರುವ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ಮೌಲ್ಯ_t * v)
{

    ಧ್ರುವ uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    ಧ್ರುವ lv_part_t part = lv_obj_style_get_selector_part(selector);
    ಧ್ರುವ bool skip_trans = (ಧ್ರುವ bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ಯದಿ(obj_style->is_trans == false) ಭಙ್ಗ;
        ಯದಿ(skip_trans) ಅನುವರ್ತ;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        ಯದಿ(part_act != part) ಅನುವರ್ತ;
        ಯದಿ((obj_style->style->has_group & group) == 0) ಅನುವರ್ತ;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        ಯದಿ(found == LV_STYLE_RES_FOUND) {
            ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ಧ್ರುವ lv_state_t state = lv_obj_style_get_selector_state(selector);
    ಧ್ರುವ lv_state_t state_inv = ~state;
    ಪ್ರತಿ(; i < obj->style_cnt; i++) {
        ಯದಿ((obj->styles[i].style->has_group & group) == 0) ಅನುವರ್ತ;
        ಯದಿ(obj->styles[i].is_disabled) ಅನುವರ್ತ;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ಯದಿ(part_act != part) ಅನುವರ್ತ;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ಯದಿ((state_act & state_inv)) ಅನುವರ್ತ;

        /*Check only better candidates*/
        ಯದಿ((int32_t)state_act <= weight) ಅನುವರ್ತ;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        ಯದಿ(found == LV_STYLE_RES_FOUND) {
            ಯದಿ(state_act == state) {
                ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    ಯದಿ(weight >= 0) ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
ಸ್ಥಿರ ಶೂನ್ಯ report_style_change_core(ಶೂನ್ಯ * style, lv_obj_t * obj)
{
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(style == ರಿಕ್ತ || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ಭಙ್ಗ;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_ಎಣಿಕೆ(obj);
    ಪ್ರತಿ(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
ಸ್ಥಿರ ಶೂನ್ಯ refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ಎಣಿಕೆ(obj);
    ಪ್ರತಿ(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, ರಿಕ್ತ);
        lv_obj_invalidate(child);

        refresh_children_style(child); /*Check children too*/
    }
}

/**
 * Remove the transition from object's part's property.
 * - Remove the transition from `lv_obj_style_trans_ll` and free it
 * - Delete pending transitions
 * @param obj pointer to an object which transition(s) should be removed
 * @param part a part of object or 0xFF to remove from all parts
 * @param prop a property or 0xFF to remove all properties
 * @param tr_limit delete transitions only "older" than this. `NULL` if not used
 * @return true: at least 1 transition style was removed.
 * @note it doesn't update the cache, `full_cache_refresh` needs to be called manually
 */
ಸ್ಥಿರ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    ಯಾವತ್(tr != ರಿಕ್ತ) {
        ಯದಿ(tr == tr_limit) ಭಙ್ಗ;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        ಯದಿ(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            ಯದಿ(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
                ಯದಿ(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ರಿಕ್ತ);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    ಪ್ರತ್ಯಾಗಮ removed;
}

ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_cb(ಶೂನ್ಯ * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) ಅನುವರ್ತ;

        lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_final = {0};
        ವಿಕಲ್ಪ(tr->prop) {

            ಪಕ್ಷ LV_STYLE_BORDER_SIDE:
            ಪಕ್ಷ LV_STYLE_BORDER_POST:
            ಪಕ್ಷ LV_STYLE_BLEND_MODE:
                ಯದಿ(v < 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->start_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->end_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಭಙ್ಗ;
            ಪಕ್ಷ LV_STYLE_TRANSITION:
            ಪಕ್ಷ LV_STYLE_TEXT_FONT:
            ಪಕ್ಷ LV_STYLE_BG_IMAGE_SRC:
                ಯದಿ(v < 255) ಮೌಲ್ಯ_final.ptr = tr->start_ಮೌಲ್ಯ.ptr;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.ptr = tr->end_ಮೌಲ್ಯ.ptr;
                ಭಙ್ಗ;
            ಪಕ್ಷ LV_STYLE_COLOR_FILTER_DSC:
                ಯದಿ(tr->start_ಮೌಲ್ಯ.ptr == ರಿಕ್ತ) ಮೌಲ್ಯ_final.ptr = tr->end_ಮೌಲ್ಯ.ptr;
                ಅನ್ಯಥಾ ಯದಿ(tr->end_ಮೌಲ್ಯ.ptr == ರಿಕ್ತ) ಮೌಲ್ಯ_final.ptr = tr->start_ಮೌಲ್ಯ.ptr;
                ಅನ್ಯಥಾ ಯದಿ(v < 128) ಮೌಲ್ಯ_final.ptr = tr->start_ಮೌಲ್ಯ.ptr;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.ptr = tr->end_ಮೌಲ್ಯ.ptr;
                ಭಙ್ಗ;
            /*Colors*/
            ಪಕ್ಷ LV_STYLE_RECOLOR:
            ಪಕ್ಷ LV_STYLE_BG_COLOR:
            ಪಕ್ಷ LV_STYLE_BG_GRAD_COLOR:
            ಪಕ್ಷ LV_STYLE_BORDER_COLOR:
            ಪಕ್ಷ LV_STYLE_TEXT_COLOR:
            ಪಕ್ಷ LV_STYLE_SHADOW_COLOR:
            ಪಕ್ಷ LV_STYLE_OUTLINE_COLOR:
            ಪಕ್ಷ LV_STYLE_IMAGE_RECOLOR:
                ಯದಿ(v <= 0) ಮೌಲ್ಯ_final.color = tr->start_ಮೌಲ್ಯ.color;
                ಅನ್ಯಥಾ ಯದಿ(v >= 255) ಮೌಲ್ಯ_final.color = tr->end_ಮೌಲ್ಯ.color;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.color = lv_color_mix(tr->end_ಮೌಲ್ಯ.color, tr->start_ಮೌಲ್ಯ.color, v);
                ಭಙ್ಗ;
            /*Opacities*/
            ಪಕ್ಷ LV_STYLE_OPA:
            ಪಕ್ಷ LV_STYLE_OPA_LAYERED:
            ಪಕ್ಷ LV_STYLE_RECOLOR_OPA:
            ಪಕ್ಷ LV_STYLE_COLOR_FILTER_OPA:
            ಪಕ್ಷ LV_STYLE_BG_OPA:
            ಪಕ್ಷ LV_STYLE_BG_GRAD_OPA:
            ಪಕ್ಷ LV_STYLE_BG_MAIN_OPA:
            ಪಕ್ಷ LV_STYLE_BG_IMAGE_OPA:
            ಪಕ್ಷ LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            ಪಕ್ಷ LV_STYLE_BORDER_OPA:
            ಪಕ್ಷ LV_STYLE_OUTLINE_OPA:
            ಪಕ್ಷ LV_STYLE_SHADOW_OPA:
            ಪಕ್ಷ LV_STYLE_ಸಾಲು_OPA:
            ಪಕ್ಷ LV_STYLE_ARC_OPA:
            ಪಕ್ಷ LV_STYLE_TEXT_OPA:
            ಪಕ್ಷ LV_STYLE_IMAGE_OPA:
            ಪಕ್ಷ LV_STYLE_IMAGE_RECOLOR_OPA:
            ಪಕ್ಷ LV_STYLE_DROP_SHADOW_OPA:
            ಪಕ್ಷ LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                ಯದಿ(v <= 0) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->start_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಅನ್ಯಥಾ ಯದಿ(v >= 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->end_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.ಅಂಕೆ =  LV_CLAMP(0, tr->start_ಮೌಲ್ಯ.ಅಂಕೆ + ((tr->end_ಮೌಲ್ಯ.ಅಂಕೆ - tr->start_ಮೌಲ್ಯ.ಅಂಕೆ) * v >> 8), 255);
                ಭಙ್ಗ;

            ಸ್ವಭಾವ:
                ಯದಿ(v == 0) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->start_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಅನ್ಯಥಾ ಯದಿ(v == 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->end_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಅನ್ಯಥಾ ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->start_ಮೌಲ್ಯ.ಅಂಕೆ + ((int32_t)((int32_t)(tr->end_ಮೌಲ್ಯ.ಅಂಕೆ - tr->start_ಮೌಲ್ಯ.ಅಂಕೆ) * v) >> 8);
                ಭಙ್ಗ;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_ಮೌಲ್ಯ_t old_ಮೌಲ್ಯ = {0};
        ಯದಿ(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_ಮೌಲ್ಯ) == LV_STYLE_RES_FOUND) {
            ಯದಿ(ಮೌಲ್ಯ_final.ptr == old_ಮೌಲ್ಯ.ptr &&
               lv_color_eq(ಮೌಲ್ಯ_final.color, old_ಮೌಲ್ಯ.color) &&
               ಮೌಲ್ಯ_final.ಅಂಕೆ == old_ಮೌಲ್ಯ.ಅಂಕೆ) {
                ಭಙ್ಗ;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        ಯದಿ(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, ಮೌಲ್ಯ_final);

#ಯದಿ LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ಯದಿ(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        ಅನ್ಯಥಾ {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ಭಙ್ಗ;
    }

}

ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_ಮೌಲ್ಯ = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_ಮೌಲ್ಯ);
#ಯದಿ LV_OBJ_STYLE_CACHE
    ಯದಿ(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    ಅನ್ಯಥಾ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ಯದಿ(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        ಅನ್ಯಥಾ {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#ಅನ್ಯಥಾ
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_completed_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;
    lv_obj_t * obj = tr->obj;
    lv_style_prop_t prop = tr->prop;

    /*Remove the transitioned property from trans. style
     *if there no more transitions for this property
     *It allows changing it by normal styles*/
    bool running = false;
    trans_t * tr_i;
    LV_LL_READ(style_trans_ll_p, tr_i) {
        ಯದಿ(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ(!running) {
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
            ಯದಿ(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                ಯದಿ(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ಭಙ್ಗ;
            }
        }
    }
}

ಸ್ಥಿರ lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#ಯದಿ LV_DRAW_TRANSFORM_USE_MATRIX
    ಯದಿ(lv_obj_get_transform(obj) != ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
#endif
    ಯದಿ(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
    ಯದಿ(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
    ಯದಿ(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
    ಯದಿ(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
    ಯದಿ(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_TRANSFORM;
    ಯದಿ(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_SIMPLE;
    ಯದಿ(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_SIMPLE;
    ಯದಿ(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_SIMPLE;
    ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
ಸ್ಥಿರ bool calculate_has_blur(lv_obj_t * obj)
{
    ಧ್ರುವ uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    ಧ್ರುವ uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    ಧ್ರುವ lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ಧ್ರುವ lv_state_t state_inv = ~state;
    lv_style_ಮೌಲ್ಯ_t v;
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ಯದಿ(obj_style->is_disabled) ಅನುವರ್ತ;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ಯದಿ((state_style & state_inv)) ಅನುವರ್ತ;

        ಯದಿ((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            ಯದಿ(v.ಅಂಕೆ > 0) ಪ್ರತ್ಯಾಗಮ true;
        }
        ಯದಿ((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            ಯದಿ(v.ಅಂಕೆ > 0) ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#ಯದಿ LV_OBJ_STYLE_CACHE
    uint32_t i;
    ಯದಿ(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
            ಯದಿ(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) ಅನುವರ್ತ;
            ಯದಿ(obj->styles[i].is_disabled) ಅನುವರ್ತ;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ಯದಿ(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
                ಪ್ರತಿ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ಅನ್ಯಥಾ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಪರಿಮಾಣ(lv_style_ಮೌಲ್ಯ_t);
                ಪ್ರತಿ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    ಯದಿ(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
            ಯದಿ(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) ಅನುವರ್ತ;
            ಯದಿ(obj->styles[i].is_disabled) ಅನುವರ್ತ;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ಯದಿ(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
                ಪ್ರತಿ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ಅನ್ಯಥಾ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಪರಿಮಾಣ(lv_style_ಮೌಲ್ಯ_t);
                ಪ್ರತಿ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#ಅನ್ಯಥಾ
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ fade_anim_cb(ಶೂನ್ಯ * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

ಸ್ಥಿರ bool style_has_flag(ಧ್ರುವ lv_style_t * style, uint32_t flag)
{
    ಯದಿ(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
        uint32_t i;
        ಪ್ರತಿ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            ಯದಿ(lv_style_prop_has_flag(props[i].prop, flag)) {
                ಪ್ರತ್ಯಾಗಮ true;
            }
        }
    }
    ಅನ್ಯಥಾ {
        lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಪರಿಮಾಣ(lv_style_ಮೌಲ್ಯ_t);
        uint32_t i;
        ಪ್ರತಿ(i = 0; i < style->prop_cnt; i++) {
            ಯದಿ(lv_style_prop_has_flag(props[i], flag)) {
                ಪ್ರತ್ಯಾಗಮ true;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ lv_style_res_t get_selector_style_prop(ಧ್ರುವ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#ಯದಿ LV_OBJ_STYLE_CACHE
    ಧ್ರುವ uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    ಯದಿ((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, ಮೌಲ್ಯ_act);
        ಯದಿ(found == LV_STYLE_RES_FOUND) ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
    }

    ಬಾಹ್ಯ ಧ್ರುವ uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    ಯದಿ(prop < LV_STYLE_ಅಂಕೆ_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    ಅನ್ಯಥಾ {
        ಯದಿ(_style_custom_prop_flag_lookup_table != ರಿಕ್ತ) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_ಅಂಕೆ_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    ಯದಿ(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        ಯದಿ(part != LV_PART_MAIN) part = LV_PART_MAIN;
        ಅನ್ಯಥಾ obj = obj->parent;

        ಯಾವತ್(obj) {
#ಯದಿ LV_OBJ_STYLE_CACHE
            ಯದಿ(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, ಮೌಲ್ಯ_act);
                ಯದಿ(found == LV_STYLE_RES_FOUND) ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    ಅನ್ಯಥಾ {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        ಯದಿ(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            ಧ್ರುವ lv_obj_class_t * cls = obj->class_p;
            ಯಾವತ್(cls) {
                ಯದಿ(prop == LV_STYLE_WIDTH) {
                    ಯದಿ(cls->width_def != 0)  {
                        ಮೌಲ್ಯ_act->ಅಂಕೆ = cls->width_def;
                        ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
                    }
                }
                ಅನ್ಯಥಾ {
                    ಯದಿ(cls->height_def != 0) {
                        ಮೌಲ್ಯ_act->ಅಂಕೆ = cls->height_def;
                        ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ LV_STYLE_RES_NOT_FOUND;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_style_core(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    ಯದಿ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ರಿಕ್ತ || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ಎಣಿಕೆ = 0;
    bool deleted = false;

    ಪ್ರತಿ(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        ಯದಿ(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        ಅನ್ಯಥಾ ಯದಿ((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ರಿಕ್ತ && style != obj->styles[i].style)) {
            should_remove = false;
        }

        ಯದಿ(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            ಯದಿ(style_ಎಣಿಕೆ != i) {
                obj->styles[style_ಎಣಿಕೆ] = obj->styles[i];
            }
            style_ಎಣಿಕೆ++;
            ಅನುವರ್ತ;
        }

        /*This entry is being removed*/
        ಯದಿ(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ರಿಕ್ತ);
        }
        ಯದಿ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ಯದಿ(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ರಿಕ್ತ;
        }
        deleted = true;
    }

    ಯದಿ(!deleted) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    obj->style_cnt = style_ಎಣಿಕೆ;
    ಯದಿ(style_ಎಣಿಕೆ > 0) {
        obj->styles = lv_realloc(obj->styles, style_ಎಣಿಕೆ * ಪರಿಮಾಣ(lv_obj_style_t));
    }
    ಅನ್ಯಥಾ {
        lv_free(obj->styles);
        obj->styles = ರಿಕ್ತ;
    }

    ಯದಿ(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

ಸ್ಥಿರ lv_obj_style_t * find_style(lv_obj_t * obj, ಧ್ರುವ lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    ಪ್ರತಿ(i = 0; i < obj->style_cnt; i++) {
        ಯದಿ(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            ಪ್ರತ್ಯಾಗಮ &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (ಶೂನ್ಯ *)style, (ಶೂನ್ಯ *)obj, selector);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}



#ಯದಿ LV_USE_OBSERVER

ಸ್ಥಿರ ಶೂನ್ಯ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->ಮೌಲ್ಯ);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

ಸ್ಥಿರ ಶೂನ್ಯ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_ಮೌಲ್ಯ_t style_v;
    ಯದಿ(subject->type == LV_SUBJECT_TYPE_INT) style_v.ಅಂಕೆ = lv_subject_get_int(subject);
    ಅನ್ಯಥಾ ಯದಿ(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    ಅನ್ಯಥಾ ಯದಿ(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    ಅನ್ಯಥಾ {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
