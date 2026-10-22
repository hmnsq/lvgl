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
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)
#ವ್ಯಾಖ್ಯಾನಿಸು style_refr LV_GLOBAL_DEFAULT()->style_refresh
#ವ್ಯಾಖ್ಯಾನಿಸು style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#ವ್ಯಾಖ್ಯಾನಿಸು style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2 LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ
#ವ್ಯಾಖ್ಯಾನಿಸು STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#ವ್ಯಾಖ್ಯಾನಿಸು DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_ಮೌಲ್ಯ_t ಆರಂಭ_ಮೌಲ್ಯ;
    lv_style_ಮೌಲ್ಯ_t ಕೊನೆ_ಮೌಲ್ಯ;
} trans_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸ್ಥಿರಾಂಕ lv_style_t * style;
    lv_style_selector_t selector;
    int32_t ಮೌಲ್ಯ;
} bind_style_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
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
ಸ್ಥಿರ lv_style_res_t get_prop_core(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ಮೌಲ್ಯ_t * v);
ಸ್ಥಿರ ಶೂನ್ಯ report_style_change_core(ಶೂನ್ಯ * style, lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ refresh_children_style(lv_obj_t * obj);
ಸ್ಥಿರ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_ಎಲ್ಲೆ);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_cb(ಶೂನ್ಯ * _tr, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_ಆರಂಭ_cb(lv_anim_t * a);
ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_completed_cb(lv_anim_t * a);
ಸ್ಥಿರ lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
ಸ್ಥಿರ bool calculate_has_blur(lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬಿದ_cache_refresh(lv_obj_t * obj, lv_part_t part);
ಸ್ಥಿರ ಶೂನ್ಯ fade_anim_cb(ಶೂನ್ಯ * obj, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ fade_ಒಳ_anim_completed(lv_anim_t * a);
ಸ್ಥಿರ bool style_has_flag(ಸ್ಥಿರಾಂಕ lv_style_t * style, uint32_t flag);
ಸ್ಥಿರ lv_style_res_t get_selector_style_prop(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ_act);
ಸ್ಥಿರ ಶೂನ್ಯ remove_style_core(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector, bool theme_only);
ಸ್ಥಿರ lv_obj_style_t * ಕಂಡುಹಿಡಿ_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector);
#ಆದರೆ LV_USE_OBSERVER
    ಸ್ಥಿರ ಶೂನ್ಯ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    ಸ್ಥಿರ ಶೂನ್ಯ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#ಆದರೆ_ಅಂತ್ಯ

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
    lv_ll_init(style_trans_ll_p, ಗಾತ್ರ(trans_t));
}

ಶೂನ್ಯ lv_obj_style_deinit(ಶೂನ್ಯ)
{
    lv_ll_clear(style_trans_ll_p);
    ಆದರೆ(style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2 != ಖಾಲಿ) {
        lv_free(style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2);
        style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2 = ಖಾಲಿ;
    }
}

ಶೂನ್ಯ lv_obj_ಕೂಡಿಸು_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG_ಸಂದೇಶ(obj->style_cnt < 63, ಹಿಂತಿರುಗಿ,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಆದರೆ(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ಖಾಲಿ);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_trans) ಮುಂದುವರಿ;
        ಆದರೆ(obj->styles[i].is_local) ಮುಂದುವರಿ;
        ನಿಲ್ಲಿಸು;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * ಗಾತ್ರ(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    ಗಾಗಿ(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], ಗಾತ್ರ(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    ಆದರೆ(trans_removed) {
        ತುಂಬಿದ_cache_refresh(obj, LV_PART_ANY);
    }
#ಆದರೆ LV_OBJ_STYLE_CACHE
    ಇಲ್ಲದಿದ್ದರೆ {
        uint32_t * prop_is_ಗಣ = part == LV_PART_MAIN ? &obj->style_main_prop_is_ಗಣ : &obj->style_other_prop_is_ಗಣ;
        ಆದರೆ(lv_style_is_const_ಆಂತರಿಕ(style)) {
            lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
            ಗಾಗಿ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_ಗಣ) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಗಾತ್ರ(lv_style_ಮೌಲ್ಯ_t);
            ಗಾಗಿ(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_ಗಣ) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * old_style, ಸ್ಥಿರಾಂಕ lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(old_style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(new_style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(old_style != new_style, ಹಿಂತಿರುಗಿ false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಆದರೆ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ಖಾಲಿ);

    bool replaced = false;
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        ಆದರೆ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ಮುಂದುವರಿ;
        }

        /*Skip non-matching styles*/
        ಆದರೆ((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            ಮುಂದುವರಿ;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], ಗಾತ್ರ(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    ಆದರೆ(replaced || trans_removed) {
        ತುಂಬಿದ_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    ಹಿಂತಿರುಗಿ replaced;
}

ಶೂನ್ಯ lv_obj_remove_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    remove_style_core(obj, style, selector, false);
}

ಶೂನ್ಯ lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    remove_style_core(obj, ಖಾಲಿ, selector, true);
}

ಶೂನ್ಯ lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    remove_style_core(obj, ಖಾಲಿ, LV_PART_ANY | LV_STATE_ANY, false);
}

ಶೂನ್ಯ lv_obj_report_style_change(lv_style_t * style)
{
    ಆದರೆ(!style_refr) ಹಿಂತಿರುಗಿ;
    lv_display_t * d = lv_display_get_ಮುಂದಿನ(ಖಾಲಿ);

    ಆಗ(d) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_ಮುಂದಿನ(d);
    }
}

ಶೂನ್ಯ lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!style_refr) ಹಿಂತಿರುಗಿ;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    ಆದರೆ(is_layout_refr) {
        ಆದರೆ(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN) == LV_ಗಾತ್ರ_CONTENT ||
           lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN) == LV_ಗಾತ್ರ_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_ಬದಲಾಯಿತು, ಖಾಲಿ);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ಆದರೆ((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
        ಆದರೆ(ಪೋಷಕ) lv_obj_mark_layout_as_dirty(ಪೋಷಕ);
    }

    /*Cache the layer type*/
    ಆದರೆ((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    ಆದರೆ(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_ಸ್ಥಿತಿ(obj);
    }

    ಆದರೆ(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_ಗಾತ್ರ(obj);
    }
    lv_obj_invalidate(obj);

    ಆದರೆ(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        ಆದರೆ(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_enabled(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_obj_style_t * obj_style = ಕಂಡುಹಿಡಿ_style(obj, style, selector);
    ಆದರೆ(obj_style == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(en != obj_style->is_disabled) ಹಿಂತಿರುಗಿ; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ಆದರೆ((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    ತುಂಬಿದ_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_ARG(style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    lv_obj_style_t * obj_style = ಕಂಡುಹಿಡಿ_style(obj, style, selector);
    ಆದರೆ(obj_style == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    ಹಿಂತಿರುಗಿ !obj_style->is_disabled;
}


ಶೂನ್ಯ lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_ಮೌಲ್ಯ_t lv_obj_get_style_prop(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ lv_style_prop_get_default(prop));
    ಹಿಂತಿರುಗಿ lv_obj_get_style_prop_ಆಂತರಿಕ(obj, part, prop);
}

lv_style_ಮೌಲ್ಯ_t lv_obj_get_style_prop_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    lv_style_selector_t selector = part | obj->state;
    lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_act = { .ptr = ಖಾಲಿ };
    lv_style_res_t ಸಿಕ್ಕಿತು;

    ಸಿಕ್ಕಿತು = get_selector_style_prop(obj, selector, prop, &ಮೌಲ್ಯ_act);
    ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ_act;

    ಹಿಂತಿರುಗಿ lv_style_prop_get_default(prop);
}
lv_style_ಮೌಲ್ಯ_t lv_obj_style_apply_color_filter_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_ಮೌಲ್ಯ_t v)
{
#ಆದರೆ LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ಖಾಲಿ);

    ಸ್ಥಿರಾಂಕ lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
    LV_ಬಳಸದ(part);
    LV_ಬಳಸದ(v);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ v;
}

bool lv_obj_has_style_prop(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_act = { .ptr = ಖಾಲಿ };
    lv_style_res_t ಸಿಕ್ಕಿತು;

    ಸಿಕ್ಕಿತು = get_selector_style_prop(obj, selector, prop, &ಮೌಲ್ಯ_act);
    ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ true;

    ಹಿಂತಿರುಗಿ false;
}

ಶೂನ್ಯ lv_obj_ಗಣ_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    ಆದರೆ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ಖಾಲಿ);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_ಗಣ_prop(style, prop, ಮೌಲ್ಯ);

    ಆದರೆ(trans_removed) {
        ತುಂಬಿದ_cache_refresh(obj, LV_PART_ANY);
    }
#ಆದರೆ LV_OBJ_STYLE_CACHE
    ಇಲ್ಲದಿದ್ದರೆ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        ಆದರೆ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_ಗಣ |= prop_shifted;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            obj->style_other_prop_is_ಗಣ |= prop_shifted;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_ಕೊನೆ;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು);

    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ಹಿಂತಿರುಗಿ lv_style_get_prop_ಆಂತರಿಕ(obj->styles[i].style, prop, ಮೌಲ್ಯ);
        }
    }

    ಹಿಂತಿರುಗಿ LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    uint32_t i;
    /*Find the style*/
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ನಿಲ್ಲಿಸು;
        }
    }

    /*The style is not found*/
    ಆದರೆ(i == obj->style_cnt) ಹಿಂತಿರುಗಿ false;

    lv_ಫಲಿತಾಂಶ_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    ಆದರೆ(res == LV_ಫಲಿತಾಂಶ_OK) {
        ತುಂಬಿದ_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ಸ್ಥಿರಾಂಕ lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(tr_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_ಮೌಲ್ಯ_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_ಮೌಲ್ಯ_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    ಆದರೆ(v1.ptr == v2.ptr && v1.ಅಂಕೆ == v2.ಅಂಕೆ && lv_color_eq(v1.color, v2.color))  ಹಿಂತಿರುಗಿ;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_ಗಣ_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    ಆದರೆ(tr_dsc->prop == LV_STYLE_RADIUS) {
        ಆದರೆ(v1.ಅಂಕೆ == LV_RADIUS_CIRCLE || v2.ಅಂಕೆ == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_ಅಗಲ(obj) / 2;
            int32_t hhalf = lv_obj_get_ಎತ್ತರ(obj) / 2;
            ಆದರೆ(v1.ಅಂಕೆ == LV_RADIUS_CIRCLE) v1.ಅಂಕೆ = LV_MIN(whalf + 1, hhalf + 1);
            ಆದರೆ(v2.ಅಂಕೆ == LV_RADIUS_CIRCLE) v2.ಅಂಕೆ = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_ತಲೆ(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    ಆದರೆ(tr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
    tr->ಆರಂಭ_ಮೌಲ್ಯ = v1;
    tr->ಕೊನೆ_ಮೌಲ್ಯ = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ಗಣ_var(&a, tr);
    lv_anim_ಗಣ_exec_cb(&a, trans_anim_cb);
    lv_anim_ಗಣ_ಆರಂಭ_cb(&a, trans_anim_ಆರಂಭ_cb);
    lv_anim_ಗಣ_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_ಗಣ_ಮೌಲ್ಯಗಳು(&a, 0x00, 0xFF);
    lv_anim_ಗಣ_duration(&a, tr_dsc->ಸಮಯ);
    lv_anim_ಗಣ_delay(&a, tr_dsc->delay);
    lv_anim_ಗಣ_ಮಾರ್ಗ_cb(&a, tr_dsc->ಮಾರ್ಗ_cb);
    lv_anim_ಗಣ_early_apply(&a, false);
    lv_anim_ಗಣ_user_ದತ್ತಾಂಶ(&a, tr_dsc->user_ದತ್ತಾಂಶ);
    lv_anim_ಆರಂಭ(&a);
}

lv_style_ಮೌಲ್ಯ_t lv_obj_style_apply_color_filter(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_ಮೌಲ್ಯ_t v)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ v);
    ಹಿಂತಿರುಗಿ lv_obj_style_apply_color_filter_ಆಂತರಿಕ(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * ಬದಲಾಯಿತು_part)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_STYLE_STATE_CMP_SAME);

    lv_style_state_cmp_t res = LV_STYLE_STATE_CMP_SAME;

    /*Accumulate which part the differing styles belong to: the lone differing
     *part, or LV_PART_ANY once styles from more than one part differ. Reported
     *via changed_part so the caller can scope the refresh to a single part.
     *diff_part_found is needed as a separate flag because a style's part can
     *itself be LV_PART_ANY, so LV_PART_ANY can't double as "none seen yet".*/
    lv_part_t diff_part = LV_PART_ANY;
    bool diff_part_ಸಿಕ್ಕಿತು = false;

    /*Are there any new styles for the new state?*/
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_trans) ಮುಂದುವರಿ;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        ಆದರೆ(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            ಆದರೆ(!diff_part_ಸಿಕ್ಕಿತು) {
                diff_part = part_act;
                diff_part_ಸಿಕ್ಕಿತು = true;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ಸ್ಥಿರಾಂಕ lv_style_t * style = obj->styles[i].style;
            lv_style_ಮೌಲ್ಯ_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_ತುದಿ, &v))layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_ಎಡ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_ಬಲ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_ಸ್ತಂಭ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_PAD_ಅಡ್ಡಸಾಲು, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_ಅಗಲ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_ಎತ್ತರ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_MIN_ಅಗಲ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_MAX_ಅಗಲ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_MIN_ಎತ್ತರ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_MAX_ಎತ್ತರ, &v)) layout_diff = true;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_BORDER_ಅಗಲ, &v)) layout_diff = true;

            ಆದರೆ(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                ಆದರೆ(ಬದಲಾಯಿತು_part) *ಬದಲಾಯಿತು_part = LV_PART_ANY;
                ಹಿಂತಿರುಗಿ LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSFORM_ಅಗಲ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSFORM_ಎತ್ತರ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_OUTLINE_ಅಗಲ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_SHADOW_ಅಗಲ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_SHADOW_ಸ್ಥಾನಾಂತರ_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_SHADOW_ಸ್ಥಾನಾಂತರ_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_SHADOW_ಹರಡುವಿಕೆ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(style, LV_STYLE_ಸಾಲು_ಅಗಲ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    ಆದರೆ(ಬದಲಾಯಿತು_part) *ಬದಲಾಯಿತು_part = diff_part;
    ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ lv_obj_fade_ಒಳ(lv_obj_t * obj, uint32_t ಸಮಯ, uint32_t delay)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ಗಣ_var(&a, obj);
    lv_anim_ಗಣ_ಮೌಲ್ಯಗಳು(&a, 0, LV_OPA_COVER);
    lv_anim_ಗಣ_exec_cb(&a, fade_anim_cb);
    lv_anim_ಗಣ_completed_cb(&a, fade_ಒಳ_anim_completed);
    lv_anim_ಗಣ_duration(&a, ಸಮಯ);
    lv_anim_ಗಣ_delay(&a, delay);
    lv_anim_ಆರಂಭ(&a);
}

ಶೂನ್ಯ lv_obj_fade_ಹೊರಹೋಗು(lv_obj_t * obj, uint32_t ಸಮಯ, uint32_t delay)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ಗಣ_var(&a, obj);
    lv_anim_ಗಣ_ಮೌಲ್ಯಗಳು(&a, lv_obj_get_style_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_ಗಣ_exec_cb(&a, fade_anim_cb);
    lv_anim_ಗಣ_duration(&a, ಸಮಯ);
    lv_anim_ಗಣ_delay(&a, delay);
    lv_anim_ಆರಂಭ(&a);
}

lv_ಪಠ್ಯ_align_t lv_obj_calculate_style_ಪಠ್ಯ_align(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * txt)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಪಠ್ಯ_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಪಠ್ಯ_ALIGN_AUTO);
    ಹಿಂತಿರುಗಿ lv_obj_calculate_style_ಪಠ್ಯ_align_ಆಂತರಿಕ(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    ಆದರೆ(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    ಆದರೆ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj = lv_obj_get_ಪೋಷಕ(obj);
    }

    ಆಗ(obj) {
        opa_obj = lv_obj_get_style_opa_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_ಕೊನೆ;
            ಹಿಂತಿರುಗಿ LV_OPA_TRANSP;
        }
        ಆದರೆ(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_ಪೋಷಕ(obj);
    }

    ಆದರೆ(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ LV_OPA_TRANSP;
    }

    ಆದರೆ(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_ಕೊನೆ;
    ಹಿಂತಿರುಗಿ opa_final;
}

ಶೂನ್ಯ lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    ಆದರೆ(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(layer_type != LV_LAYER_TYPE_NONE) {
        ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
            ಹಿಂತಿರುಗಿ;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

ಶೂನ್ಯ lv_obj_update_blur_ಸ್ಥಿತಿ(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    ಆದರೆ(obj->is_deleting) ಹಿಂತಿರುಗಿ;

    bool has_blur = calculate_has_blur(obj);
    ಆದರೆ(has_blur == (bool)obj->has_blur) ಹಿಂತಿರುಗಿ;

    obj->has_blur = has_blur;
    ಆದರೆ(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_ಆಂತರಿಕ(obj, part);
        color = lv_color_over32(color, lv_color_ಗಮ್ಯ_32(recolor, opa));
    }

    ಹಿಂತಿರುಗಿ color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ (lv_color32_t) {
        0
    });

    lv_color32_t ಫಲಿತಾಂಶ;

    lv_color_t color = lv_obj_get_style_recolor_ಆಂತರಿಕ(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_ಆಂತರಿಕ(obj, part);

    ಫಲಿತಾಂಶ = lv_color_ಗಮ್ಯ_32(color, opa);

    ಆದರೆ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj = lv_obj_get_ಪೋಷಕ(obj);
    }

    ಆಗ(obj) {
        ಫಲಿತಾಂಶ = lv_obj_style_apply_recolor(obj, part, ಫಲಿತಾಂಶ);
        obj = lv_obj_get_ಪೋಷಕ(obj);
    }

    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_pad_all(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_pad_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_ಬಲ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_pad_hor(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_pad_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_ಬಲ(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_pad_ver(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_pad_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_margin_all(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_margin_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_margin_ಬಲ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_margin_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_margin_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_margin_hor(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_margin_ಎಡ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_margin_ಬಲ(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_margin_ver(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_margin_ತುದಿ(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_margin_bottom(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_pad_gap(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_pad_ಅಡ್ಡಸಾಲು(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_pad_ಸ್ತಂಭ(obj, ಮೌಲ್ಯ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_ಗಾತ್ರ(lv_obj_t * obj, int32_t ಅಗಲ, int32_t ಎತ್ತರ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_ಅಗಲ(obj, ಅಗಲ, selector);
    lv_obj_ಗಣ_style_ಎತ್ತರ(obj, ಎತ್ತರ, selector);
}

ಶೂನ್ಯ lv_obj_ಗಣ_style_transform_scale(lv_obj_t * obj, int32_t ಮೌಲ್ಯ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_transform_scale_x(obj, ಮೌಲ್ಯ, selector);
    lv_obj_ಗಣ_style_transform_scale_y(obj, ಮೌಲ್ಯ, selector);
}

int32_t lv_obj_get_style_space_ಎಡ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, part);
}

int32_t lv_obj_get_style_space_ಬಲ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, part);
}

int32_t lv_obj_get_style_space_ತುದಿ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_transform_scale_x_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_ಸುರಕ್ಷಿತ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ಹಿಂತಿರುಗಿ 0)
    ಹಿಂತಿರುಗಿ lv_obj_get_style_transform_scale_y_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(obj, part);
}


#ಆದರೆ LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(style != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    lv_obj_ಕೂಡಿಸು_style(obj, style, selector);

    bind_style_t * p = lv_malloc(ಗಾತ್ರ(bind_style_t));
    LV_ASSERT_MALLOC(p);
    ಆದರೆ(p == ಖಾಲಿ) {
        LV_LOG_WARN("Out of memory");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    p->style = style;
    p->selector = selector;
    p->ಮೌಲ್ಯ = ref_ಮೌಲ್ಯ;

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_ದತ್ತಾಂಶ = 1;
    ಹಿಂತಿರುಗಿ observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(subject != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಆದರೆ(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_ಸೂಚಕ) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    bind_style_prop_t * p = lv_malloc(ಗಾತ್ರ(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    ಆದರೆ(p == ಖಾಲಿ) {
        LV_LOG_WARN("Out of memory");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_ಕೂಡಿಸು_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_ದತ್ತಾಂಶ = 1;
    ಹಿಂತಿರುಗಿ observable;
}


#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBSERVER*/


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
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ಹಿಂತಿರುಗಿ (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(ಗಾತ್ರ(lv_style_t));
    ಆದರೆ(new_style == ಖಾಲಿ) {
        LV_LOG_WARN("couldn't allocate local style");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ಗಾತ್ರ(lv_obj_style_t));
    ಆದರೆ(new_styles == ಖಾಲಿ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    obj->styles = new_styles;

    ಗಾಗಿ(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        ಆದರೆ(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ನಿಲ್ಲಿಸು;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], ಗಾತ್ರ(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    ಹಿಂತಿರುಗಿ (lv_style_t *)obj->styles[i].style;
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
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_trans && obj->styles[i].selector == selector) ನಿಲ್ಲಿಸು;
    }

    /*Already have a transition style for it*/
    ಆದರೆ(i != obj->style_cnt) ಹಿಂತಿರುಗಿ &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(ಗಾತ್ರ(lv_style_t));
    ಆದರೆ(new_style == ಖಾಲಿ) {
        LV_LOG_WARN("couldn't allocate transition style");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ಗಾತ್ರ(lv_obj_style_t));
    ಆದರೆ(new_styles == ಖಾಲಿ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    obj->styles = new_styles;

    ಗಾಗಿ(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], ಗಾತ್ರ(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    ಹಿಂತಿರುಗಿ &obj->styles[0];
}

ಸ್ಥಿರ lv_style_res_t get_prop_core(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ಮೌಲ್ಯ_t * v)
{

    ಸ್ಥಿರಾಂಕ uint32_t ಗುಂಪು = (uint32_t)1 << lv_style_get_prop_ಗುಂಪು(prop);
    ಸ್ಥಿರಾಂಕ lv_part_t part = lv_obj_style_get_selector_part(selector);
    ಸ್ಥಿರಾಂಕ bool skip_trans = (ಸ್ಥಿರಾಂಕ bool) obj->skip_trans;
    int32_t ತೂಕ = -1;
    lv_style_res_t ಸಿಕ್ಕಿತು;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ಆದರೆ(obj_style->is_trans == false) ನಿಲ್ಲಿಸು;
        ಆದರೆ(skip_trans) ಮುಂದುವರಿ;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        ಆದರೆ(part_act != part) ಮುಂದುವರಿ;
        ಆದರೆ((obj_style->style->has_ಗುಂಪು & ಗುಂಪು) == 0) ಮುಂದುವರಿ;
        ಸಿಕ್ಕಿತು = lv_style_get_prop_ಆಂತರಿಕ(obj_style->style, prop, v);
        ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) {
            ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ಸ್ಥಿರಾಂಕ lv_state_t state = lv_obj_style_get_selector_state(selector);
    ಸ್ಥಿರಾಂಕ lv_state_t state_inv = ~state;
    ಗಾಗಿ(; i < obj->style_cnt; i++) {
        ಆದರೆ((obj->styles[i].style->has_ಗುಂಪು & ಗುಂಪು) == 0) ಮುಂದುವರಿ;
        ಆದರೆ(obj->styles[i].is_disabled) ಮುಂದುವರಿ;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ಆದರೆ(part_act != part) ಮುಂದುವರಿ;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ಆದರೆ((state_act & state_inv)) ಮುಂದುವರಿ;

        /*Check only better candidates*/
        ಆದರೆ((int32_t)state_act <= ತೂಕ) ಮುಂದುವರಿ;

        ಸಿಕ್ಕಿತು = lv_style_get_prop_ಆಂತರಿಕ(obj_style->style, prop, v);
        ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) {
            ಆದರೆ(state_act == state) {
                ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
            }
            ತೂಕ = state_act;
        }
    }

    ಆದರೆ(ತೂಕ >= 0) ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
ಸ್ಥಿರ ಶೂನ್ಯ report_style_change_core(ಶೂನ್ಯ * style, lv_obj_t * obj)
{
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(style == ಖಾಲಿ || obj->styles[i].style == style) {
            ತುಂಬಿದ_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ನಿಲ್ಲಿಸು;
        }
    }

    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
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
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        lv_obj_invalidate(ಮಗು);
        lv_obj_send_event(ಮಗು, LV_EVENT_STYLE_ಬದಲಾಯಿತು, ಖಾಲಿ);
        lv_obj_invalidate(ಮಗು);

        refresh_children_style(ಮಗು); /*Check children too*/
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
ಸ್ಥಿರ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_ಎಲ್ಲೆ)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_ಬಾಲ(style_trans_ll_p);
    ಆಗ(tr != ಖಾಲಿ) {
        ಆದರೆ(tr == tr_ಎಲ್ಲೆ) ನಿಲ್ಲಿಸು;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        ಆದರೆ(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            ಆದರೆ(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
                ಆದರೆ(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ಖಾಲಿ);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    ಹಿಂತಿರುಗಿ removed;
}

ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_cb(ಶೂನ್ಯ * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) ಮುಂದುವರಿ;

        lv_style_ಮೌಲ್ಯ_t ಮೌಲ್ಯ_final = {0};
        ಆಯ್ಕೆ(tr->prop) {

            ಸ್ಥಿತಿ LV_STYLE_BORDER_SIDE:
            ಸ್ಥಿತಿ LV_STYLE_BORDER_POST:
            ಸ್ಥಿತಿ LV_STYLE_BLEND_ಬಹುಲಕ:
                ಆದರೆ(v < 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಕೊನೆ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_STYLE_TRANSITION:
            ಸ್ಥಿತಿ LV_STYLE_ಪಠ್ಯ_FONT:
            ಸ್ಥಿತಿ LV_STYLE_BG_IMAGE_SRC:
                ಆದರೆ(v < 255) ಮೌಲ್ಯ_final.ptr = tr->ಆರಂಭ_ಮೌಲ್ಯ.ptr;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.ptr = tr->ಕೊನೆ_ಮೌಲ್ಯ.ptr;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_STYLE_COLOR_FILTER_DSC:
                ಆದರೆ(tr->ಆರಂಭ_ಮೌಲ್ಯ.ptr == ಖಾಲಿ) ಮೌಲ್ಯ_final.ptr = tr->ಕೊನೆ_ಮೌಲ್ಯ.ptr;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(tr->ಕೊನೆ_ಮೌಲ್ಯ.ptr == ಖಾಲಿ) ಮೌಲ್ಯ_final.ptr = tr->ಆರಂಭ_ಮೌಲ್ಯ.ptr;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(v < 128) ಮೌಲ್ಯ_final.ptr = tr->ಆರಂಭ_ಮೌಲ್ಯ.ptr;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.ptr = tr->ಕೊನೆ_ಮೌಲ್ಯ.ptr;
                ನಿಲ್ಲಿಸು;
            /*Colors*/
            ಸ್ಥಿತಿ LV_STYLE_RECOLOR:
            ಸ್ಥಿತಿ LV_STYLE_BG_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_BG_GRAD_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_BORDER_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_ಪಠ್ಯ_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_SHADOW_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_OUTLINE_COLOR:
            ಸ್ಥಿತಿ LV_STYLE_IMAGE_RECOLOR:
                ಆದರೆ(v <= 0) ಮೌಲ್ಯ_final.color = tr->ಆರಂಭ_ಮೌಲ್ಯ.color;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(v >= 255) ಮೌಲ್ಯ_final.color = tr->ಕೊನೆ_ಮೌಲ್ಯ.color;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.color = lv_color_mix(tr->ಕೊನೆ_ಮೌಲ್ಯ.color, tr->ಆರಂಭ_ಮೌಲ್ಯ.color, v);
                ನಿಲ್ಲಿಸು;
            /*Opacities*/
            ಸ್ಥಿತಿ LV_STYLE_OPA:
            ಸ್ಥಿತಿ LV_STYLE_OPA_LAYERED:
            ಸ್ಥಿತಿ LV_STYLE_RECOLOR_OPA:
            ಸ್ಥಿತಿ LV_STYLE_COLOR_FILTER_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BG_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BG_GRAD_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BG_MAIN_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BG_IMAGE_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            ಸ್ಥಿತಿ LV_STYLE_BORDER_OPA:
            ಸ್ಥಿತಿ LV_STYLE_OUTLINE_OPA:
            ಸ್ಥಿತಿ LV_STYLE_SHADOW_OPA:
            ಸ್ಥಿತಿ LV_STYLE_ಸಾಲು_OPA:
            ಸ್ಥಿತಿ LV_STYLE_ARC_OPA:
            ಸ್ಥಿತಿ LV_STYLE_ಪಠ್ಯ_OPA:
            ಸ್ಥಿತಿ LV_STYLE_IMAGE_OPA:
            ಸ್ಥಿತಿ LV_STYLE_IMAGE_RECOLOR_OPA:
            ಸ್ಥಿತಿ LV_STYLE_DROP_SHADOW_OPA:
            ಸ್ಥಿತಿ LV_STYLE_ಪಠ್ಯ_OUTLINE_STROKE_OPA:
                ಆದರೆ(v <= 0) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(v >= 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಕೊನೆ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.ಅಂಕೆ =  LV_CLAMP(0, tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ + ((tr->ಕೊನೆ_ಮೌಲ್ಯ.ಅಂಕೆ - tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ) * v >> 8), 255);
                ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
                ಆದರೆ(v == 0) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(v == 255) ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಕೊನೆ_ಮೌಲ್ಯ.ಅಂಕೆ;
                ಇಲ್ಲದಿದ್ದರೆ ಮೌಲ್ಯ_final.ಅಂಕೆ = tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ + ((int32_t)((int32_t)(tr->ಕೊನೆ_ಮೌಲ್ಯ.ಅಂಕೆ - tr->ಆರಂಭ_ಮೌಲ್ಯ.ಅಂಕೆ) * v) >> 8);
                ನಿಲ್ಲಿಸು;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_ಮೌಲ್ಯ_t old_ಮೌಲ್ಯ = {0};
        ಆದರೆ(lv_style_get_prop_ಆಂತರಿಕ(obj->styles[i].style, tr->prop, &old_ಮೌಲ್ಯ) == LV_STYLE_RES_ಸಿಕ್ಕಿತು) {
            ಆದರೆ(ಮೌಲ್ಯ_final.ptr == old_ಮೌಲ್ಯ.ptr &&
               lv_color_eq(ಮೌಲ್ಯ_final.color, old_ಮೌಲ್ಯ.color) &&
               ಮೌಲ್ಯ_final.ಅಂಕೆ == old_ಮೌಲ್ಯ.ಅಂಕೆ) {
                ನಿಲ್ಲಿಸು;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        ಆದರೆ(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_ಗಣ_prop((lv_style_t *)obj->styles[i].style, tr->prop, ಮೌಲ್ಯ_final);

#ಆದರೆ LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ಆದರೆ(part == LV_PART_MAIN) {
            obj->style_main_prop_is_ಗಣ |= prop_shifted;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            obj->style_other_prop_is_ಗಣ |= prop_shifted;
        }
#ಆದರೆ_ಅಂತ್ಯ

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ನಿಲ್ಲಿಸು;
    }

}

ಸ್ಥಿರ ಶೂನ್ಯ trans_anim_ಆರಂಭ_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->ಆರಂಭ_ಮೌಲ್ಯ = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_ಗಣ_prop((lv_style_t *)style_trans->style, tr->prop, tr->ಆರಂಭ_ಮೌಲ್ಯ);
#ಆದರೆ LV_OBJ_STYLE_CACHE
    ಆದರೆ(trans_removed) {
        ತುಂಬಿದ_cache_refresh(tr->obj, part);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ಆದರೆ(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_ಗಣ |= prop_shifted;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            tr->obj->style_other_prop_is_ಗಣ |= prop_shifted;
        }
    }
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(trans_removed);
#ಆದರೆ_ಅಂತ್ಯ

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
        ಆದರೆ(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ(!running) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
            ಆದರೆ(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                ಆದರೆ(lv_style_is_ಖಾಲಿಗಳು(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ನಿಲ್ಲಿಸು;
            }
        }
    }
}

ಸ್ಥಿರ lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಆದರೆ(lv_obj_get_transform(obj) != ಖಾಲಿ) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ(lv_obj_get_style_transform_rotation_ಆಂತರಿಕ(obj, LV_PART_MAIN) != 0) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
    ಆದರೆ(lv_obj_get_style_transform_scale_x_ಆಂತರಿಕ(obj, LV_PART_MAIN) != 256) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
    ಆದರೆ(lv_obj_get_style_transform_scale_y_ಆಂತರಿಕ(obj, LV_PART_MAIN) != 256) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
    ಆದರೆ(lv_obj_get_style_transform_skew_x_ಆಂತರಿಕ(obj, LV_PART_MAIN) != 0) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
    ಆದರೆ(lv_obj_get_style_transform_skew_y_ಆಂತರಿಕ(obj, LV_PART_MAIN) != 0) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_TRANSFORM;
    ಆದರೆ(lv_obj_get_style_opa_layered_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_OPA_COVER) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_SIMPLE;
    ಆದರೆ(lv_obj_get_style_bitmap_ಮುಖವಾಡ_src_ಆಂತರಿಕ(obj, LV_PART_MAIN) != ಖಾಲಿ) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_SIMPLE;
    ಆದರೆ(lv_obj_get_style_blend_ಬಹುಲಕ_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_BLEND_ಬಹುಲಕ_ಸಾಮಾನ್ಯ) ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_SIMPLE;
    ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
ಸ್ಥಿರ bool calculate_has_blur(lv_obj_t * obj)
{
    ಸ್ಥಿರಾಂಕ uint32_t ಗುಂಪು_blur = (uint32_t)1 << lv_style_get_prop_ಗುಂಪು(LV_STYLE_BLUR_RADIUS);
    ಸ್ಥಿರಾಂಕ uint32_t ಗುಂಪು_dropshadow = (uint32_t)1 << lv_style_get_prop_ಗುಂಪು(LV_STYLE_DROP_SHADOW_OPA);
    ಸ್ಥಿರಾಂಕ lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ಸ್ಥಿರಾಂಕ lv_state_t state_inv = ~state;
    lv_style_ಮೌಲ್ಯ_t v;
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ಆದರೆ(obj_style->is_disabled) ಮುಂದುವರಿ;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ಆದರೆ((state_style & state_inv)) ಮುಂದುವರಿ;

        ಆದರೆ((obj_style->style->has_ಗುಂಪು & ಗುಂಪು_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            ಆದರೆ(v.ಅಂಕೆ > 0) ಹಿಂತಿರುಗಿ true;
        }
        ಆದರೆ((obj_style->style->has_ಗುಂಪು & ಗುಂಪು_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            ಆದರೆ(v.ಅಂಕೆ > 0) ಹಿಂತಿರುಗಿ true;
        }
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬಿದ_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#ಆದರೆ LV_OBJ_STYLE_CACHE
    uint32_t i;
    ಆದರೆ(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_ಗಣ = 0;
        ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
            ಆದರೆ(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) ಮುಂದುವರಿ;
            ಆದರೆ(obj->styles[i].is_disabled) ಮುಂದುವರಿ;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ಆದರೆ(lv_style_is_const_ಆಂತರಿಕ(style)) {
                lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
                ಗಾಗಿ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_ಗಣ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಗಾತ್ರ(lv_style_ಮೌಲ್ಯ_t);
                ಗಾಗಿ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_ಗಣ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    ಆದರೆ(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_ಗಣ = 0;
        ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
            ಆದರೆ(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) ಮುಂದುವರಿ;
            ಆದರೆ(obj->styles[i].is_disabled) ಮುಂದುವರಿ;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ಆದರೆ(lv_style_is_const_ಆಂತರಿಕ(style)) {
                lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
                ಗಾಗಿ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_ಗಣ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಗಾತ್ರ(lv_style_ಮೌಲ್ಯ_t);
                ಗಾಗಿ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_ಗಣ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
    LV_ಬಳಸದ(part);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ fade_anim_cb(ಶೂನ್ಯ * obj, int32_t v)
{
    lv_obj_ಗಣ_style_opa(obj, v, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ fade_ಒಳ_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

ಸ್ಥಿರ bool style_has_flag(ಸ್ಥಿರಾಂಕ lv_style_t * style, uint32_t flag)
{
    ಆದರೆ(lv_style_is_const_ಆಂತರಿಕ(style)) {
        lv_style_const_prop_t * props = style->ಮೌಲ್ಯಗಳು_and_props;
        uint32_t i;
        ಗಾಗಿ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            ಆದರೆ(lv_style_prop_has_flag(props[i].prop, flag)) {
                ಹಿಂತಿರುಗಿ true;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_style_prop_t * props = (lv_style_prop_t *)style->ಮೌಲ್ಯಗಳು_and_props + style->prop_cnt * ಗಾತ್ರ(lv_style_ಮೌಲ್ಯ_t);
        uint32_t i;
        ಗಾಗಿ(i = 0; i < style->prop_cnt; i++) {
            ಆದರೆ(lv_style_prop_has_flag(props[i], flag)) {
                ಹಿಂತಿರುಗಿ true;
            }
        }
    }
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ lv_style_res_t get_selector_style_prop(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ಮೌಲ್ಯ_t * ಮೌಲ್ಯ_act)
{
    lv_style_res_t ಸಿಕ್ಕಿತು;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#ಆದರೆ LV_OBJ_STYLE_CACHE
    ಸ್ಥಿರಾಂಕ uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    ಆದರೆ((part == LV_PART_MAIN ? obj->style_main_prop_is_ಗಣ : obj->style_other_prop_is_ಗಣ) & prop_shifted)
#ಆದರೆ_ಅಂತ್ಯ
    {
        ಸಿಕ್ಕಿತು = get_prop_core(obj, selector, prop, ಮೌಲ್ಯ_act);
        ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
    }

    ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ uint8_t lv_style_builtin_prop_flag_lookup_ಕೋಷ್ಟಕ[];
    bool inheritable = false;
    ಆದರೆ(prop < LV_STYLE_ಅಂಕೆ_BUILT_ಒಳ_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_ಕೋಷ್ಟಕ[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ(style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2 != ಖಾಲಿ) {
            inheritable = style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ_2[prop - LV_STYLE_ಅಂಕೆ_BUILT_ಒಳ_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    ಆದರೆ(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        ಆದರೆ(part != LV_PART_MAIN) part = LV_PART_MAIN;
        ಇಲ್ಲದಿದ್ದರೆ obj = obj->ಪೋಷಕ;

        ಆಗ(obj) {
#ಆದರೆ LV_OBJ_STYLE_CACHE
            ಆದರೆ(obj->style_main_prop_is_ಗಣ & prop_shifted)
#ಆದರೆ_ಅಂತ್ಯ
            {
                selector = part | obj->state;
                ಸಿಕ್ಕಿತು = get_prop_core(obj, selector, prop, ಮೌಲ್ಯ_act);
                ಆದರೆ(ಸಿಕ್ಕಿತು == LV_STYLE_RES_ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
            }
            /*Check the parent too.*/
            obj = obj->ಪೋಷಕ;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        ಆದರೆ(part == LV_PART_MAIN && (prop == LV_STYLE_ಅಗಲ || prop == LV_STYLE_ಎತ್ತರ)) {
            ಸ್ಥಿರಾಂಕ lv_obj_class_t * cls = obj->class_p;
            ಆಗ(cls) {
                ಆದರೆ(prop == LV_STYLE_ಅಗಲ) {
                    ಆದರೆ(cls->ಅಗಲ_def != 0)  {
                        ಮೌಲ್ಯ_act->ಅಂಕೆ = cls->ಅಗಲ_def;
                        ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    ಆದರೆ(cls->ಎತ್ತರ_def != 0) {
                        ಮೌಲ್ಯ_act->ಅಂಕೆ = cls->ಎತ್ತರ_def;
                        ಹಿಂತಿರುಗಿ LV_STYLE_RES_ಸಿಕ್ಕಿತು;
                    }
                }
                cls = cls->ತಳ_class;
            }
        }
    }

    ಹಿಂತಿರುಗಿ LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು;
}

ಸ್ಥಿರ ಶೂನ್ಯ remove_style_core(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    ಆದರೆ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ಖಾಲಿ || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ಎಣಿಕೆ = 0;
    bool deleted = false;

    ಗಾಗಿ(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        ಆದರೆ(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ಖಾಲಿ && style != obj->styles[i].style)) {
            should_remove = false;
        }

        ಆದರೆ(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            ಆದರೆ(style_ಎಣಿಕೆ != i) {
                obj->styles[style_ಎಣಿಕೆ] = obj->styles[i];
            }
            style_ಎಣಿಕೆ++;
            ಮುಂದುವರಿ;
        }

        /*This entry is being removed*/
        ಆದರೆ(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ಖಾಲಿ);
        }
        ಆದರೆ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ಆದರೆ(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ಖಾಲಿ;
        }
        deleted = true;
    }

    ಆದರೆ(!deleted) {
        ಹಿಂತಿರುಗಿ;
    }

    obj->style_cnt = style_ಎಣಿಕೆ;
    ಆದರೆ(style_ಎಣಿಕೆ > 0) {
        obj->styles = lv_realloc(obj->styles, style_ಎಣಿಕೆ * ಗಾತ್ರ(lv_obj_style_t));
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_free(obj->styles);
        obj->styles = ಖಾಲಿ;
    }

    ಆದರೆ(deleted) {
        ತುಂಬಿದ_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

ಸ್ಥಿರ lv_obj_style_t * ಕಂಡುಹಿಡಿ_style(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    ಗಾಗಿ(i = 0; i < obj->style_cnt; i++) {
        ಆದರೆ(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            ಹಿಂತಿರುಗಿ &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (ಶೂನ್ಯ *)style, (ಶೂನ್ಯ *)obj, selector);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}



#ಆದರೆ LV_USE_OBSERVER

ಸ್ಥಿರ ಶೂನ್ಯ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_ದತ್ತಾಂಶ(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->ಮೌಲ್ಯ);
    lv_obj_ಗಣ_style_enabled(observer->ಗುರಿ, p->style, p->selector, en);
}

ಸ್ಥಿರ ಶೂನ್ಯ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_ದತ್ತಾಂಶ(observer);

    lv_style_ಮೌಲ್ಯ_t style_v;
    ಆದರೆ(subject->type == LV_SUBJECT_TYPE_INT) style_v.ಅಂಕೆ = lv_subject_get_int(subject);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(subject->type == LV_SUBJECT_TYPE_ಸೂಚಕ) style_v.ptr = lv_subject_get_ಸೂಚಕ(subject);
    ಇಲ್ಲದಿದ್ದರೆ {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        ಹಿಂತಿರುಗಿ;
    }

    lv_obj_ಗಣ_local_style_prop(observer->ಗುರಿ, p->prop, style_v, p->selector);
}


#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBSERVER*/
