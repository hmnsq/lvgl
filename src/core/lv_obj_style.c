/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_anim_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_bidi_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_style_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_observer_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ style_refr LV_GLOBAL_DEFAULT()->style_refresh
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ style_custom_prop_flag_lookup_ਸਾਰਣੀ_2 LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_ਸਾਰਣੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_ਮੁੱਲ_t ਸ਼ੁਰੂ_ਮੁੱਲ;
    lv_style_ਮੁੱਲ_t ਸਿਰਾ_ਮੁੱਲ;
} trans_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅਚਲ lv_style_t * style;
    lv_style_selector_t selector;
    int32_t ਮੁੱਲ;
} bind_style_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
ਸਥਿਰ lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
ਸਥਿਰ lv_style_res_t get_prop_core(ਅਚਲ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ਮੁੱਲ_t * v);
ਸਥਿਰ ਖਾਲੀ report_style_change_core(ਖਾਲੀ * style, lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ refresh_children_style(lv_obj_t * obj);
ਸਥਿਰ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_ਹੱਦਬੰਦੀ);
ਸਥਿਰ ਖਾਲੀ trans_anim_cb(ਖਾਲੀ * _tr, int32_t v);
ਸਥਿਰ ਖਾਲੀ trans_anim_ਸ਼ੁਰੂ_cb(lv_anim_t * a);
ਸਥਿਰ ਖਾਲੀ trans_anim_completed_cb(lv_anim_t * a);
ਸਥਿਰ lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
ਸਥਿਰ bool calculate_has_blur(lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ ਭਰਿਆ_cache_refresh(lv_obj_t * obj, lv_part_t part);
ਸਥਿਰ ਖਾਲੀ fade_anim_cb(ਖਾਲੀ * obj, int32_t v);
ਸਥਿਰ ਖਾਲੀ fade_ਵਿੱਚ_anim_completed(lv_anim_t * a);
ਸਥਿਰ bool style_has_flag(ਅਚਲ lv_style_t * style, uint32_t flag);
ਸਥਿਰ lv_style_res_t get_selector_style_prop(ਅਚਲ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ਮੁੱਲ_t * ਮੁੱਲ_act);
ਸਥਿਰ ਖਾਲੀ remove_style_core(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector, bool theme_only);
ਸਥਿਰ lv_obj_style_t * ਲੱਭੋ_style(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector);
#ਜੇ LV_USE_OBSERVER
    ਸਥਿਰ ਖਾਲੀ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    ਸਥਿਰ ਖਾਲੀ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#ਜੇ_ਅੰਤ

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ਖਾਲੀ lv_obj_style_init(ਖਾਲੀ)
{
    lv_ll_init(style_trans_ll_p, ਆਕਾਰ(trans_t));
}

ਖਾਲੀ lv_obj_style_deinit(ਖਾਲੀ)
{
    lv_ll_clear(style_trans_ll_p);
    ਜੇ(style_custom_prop_flag_lookup_ਸਾਰਣੀ_2 != ਨਿਰਾ) {
        lv_free(style_custom_prop_flag_lookup_ਸਾਰਣੀ_2);
        style_custom_prop_flag_lookup_ਸਾਰਣੀ_2 = ਨਿਰਾ;
    }
}

ਖਾਲੀ lv_obj_ਜੋੜੋ_style(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(style != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG_ਸੁਨੇਹਾ(obj->style_cnt < 63, ਵਾਪਸ,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ਜੇ(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ਨਿਰਾ);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_trans) ਜਾਰੀ;
        ਜੇ(obj->styles[i].is_local) ਜਾਰੀ;
        ਰੋਕੋ;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * ਆਕਾਰ(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    ਲਈ(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], ਆਕਾਰ(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    ਜੇ(trans_removed) {
        ਭਰਿਆ_cache_refresh(obj, LV_PART_ANY);
    }
#ਜੇ LV_OBJ_STYLE_CACHE
    ਨਹੀਂਤਾਂ {
        uint32_t * prop_is_ਸੈੱਟ = part == LV_PART_MAIN ? &obj->style_main_prop_is_ਸੈੱਟ : &obj->style_other_prop_is_ਸੈੱਟ;
        ਜੇ(lv_style_is_const_ਅੰਦਰੂਨੀ(style)) {
            lv_style_const_prop_t * props = style->ਮੁੱਲਾਂ_and_props;
            ਲਈ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_ਸੈੱਟ) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        ਨਹੀਂਤਾਂ {
            lv_style_prop_t * props = (lv_style_prop_t *)style->ਮੁੱਲਾਂ_and_props + style->prop_cnt * ਆਕਾਰ(lv_style_ਮੁੱਲ_t);
            ਲਈ(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_ਸੈੱਟ) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#ਜੇ_ਅੰਤ

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ਅਚਲ lv_style_t * old_style, ਅਚਲ lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(old_style != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(new_style != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(old_style != new_style, ਵਾਪਸ false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ਜੇ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ਨਿਰਾ);

    bool replaced = false;
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        ਜੇ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ਜਾਰੀ;
        }

        /*Skip non-matching styles*/
        ਜੇ((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            ਜਾਰੀ;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], ਆਕਾਰ(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    ਜੇ(replaced || trans_removed) {
        ਭਰਿਆ_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    ਵਾਪਸ replaced;
}

ਖਾਲੀ lv_obj_remove_style(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    remove_style_core(obj, style, selector, false);
}

ਖਾਲੀ lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    remove_style_core(obj, ਨਿਰਾ, selector, true);
}

ਖਾਲੀ lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    remove_style_core(obj, ਨਿਰਾ, LV_PART_ANY | LV_STATE_ANY, false);
}

ਖਾਲੀ lv_obj_report_style_change(lv_style_t * style)
{
    ਜੇ(!style_refr) ਵਾਪਸ;
    lv_display_t * d = lv_display_get_ਅਗਲਾ(ਨਿਰਾ);

    ਜਦੋਂ(d) {
        uint32_t i;
        ਲਈ(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_ਅਗਲਾ(d);
    }
}

ਖਾਲੀ lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!style_refr) ਵਾਪਸ;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    ਜੇ(is_layout_refr) {
        ਜੇ(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == LV_ਮਾਪ_CONTENT ||
           lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == LV_ਮਾਪ_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_ਬਦਲਿਆ, ਨਿਰਾ);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ਜੇ((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
        ਜੇ(ਮਾਪਾ) lv_obj_mark_layout_as_dirty(ਮਾਪਾ);
    }

    /*Cache the layer type*/
    ਜੇ((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    ਜੇ(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_ਸਥਿਤੀ(obj);
    }

    ਜੇ(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_ਮਾਪ(obj);
    }
    lv_obj_invalidate(obj);

    ਜੇ(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        ਜੇ(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_enabled(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(style != ਨਿਰਾ, ਵਾਪਸ);

    lv_obj_style_t * obj_style = ਲੱਭੋ_style(obj, style, selector);
    ਜੇ(obj_style == ਨਿਰਾ) ਵਾਪਸ;

    ਜੇ(en != obj_style->is_disabled) ਵਾਪਸ; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ਜੇ((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    ਭਰਿਆ_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_ARG(style != ਨਿਰਾ, ਵਾਪਸ false);

    lv_obj_style_t * obj_style = ਲੱਭੋ_style(obj, style, selector);
    ਜੇ(obj_style == ਨਿਰਾ) ਵਾਪਸ false;

    ਵਾਪਸ !obj_style->is_disabled;
}


ਖਾਲੀ lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_ਮੁੱਲ_t lv_obj_get_style_prop(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ lv_style_prop_get_default(prop));
    ਵਾਪਸ lv_obj_get_style_prop_ਅੰਦਰੂਨੀ(obj, part, prop);
}

lv_style_ਮੁੱਲ_t lv_obj_get_style_prop_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    lv_style_selector_t selector = part | obj->state;
    lv_style_ਮੁੱਲ_t ਮੁੱਲ_act = { .ptr = ਨਿਰਾ };
    lv_style_res_t ਮਿਲਿਆ;

    ਮਿਲਿਆ = get_selector_style_prop(obj, selector, prop, &ਮੁੱਲ_act);
    ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) ਵਾਪਸ ਮੁੱਲ_act;

    ਵਾਪਸ lv_style_prop_get_default(prop);
}
lv_style_ਮੁੱਲ_t lv_obj_style_apply_color_filter_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_style_ਮੁੱਲ_t v)
{
#ਜੇ LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ਨਿਰਾ);

    ਅਚਲ lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_ਅਣਵਰਤਿਆ(part);
    LV_ਅਣਵਰਤਿਆ(v);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ v;
}

bool lv_obj_has_style_prop(ਅਚਲ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);

    lv_style_ਮੁੱਲ_t ਮੁੱਲ_act = { .ptr = ਨਿਰਾ };
    lv_style_res_t ਮਿਲਿਆ;

    ਮਿਲਿਆ = get_selector_style_prop(obj, selector, prop, &ਮੁੱਲ_act);
    ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) ਵਾਪਸ true;

    ਵਾਪਸ false;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ਮੁੱਲ_t ਮੁੱਲ,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    ਜੇ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ਨਿਰਾ);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_ਸੈੱਟ_prop(style, prop, ਮੁੱਲ);

    ਜੇ(trans_removed) {
        ਭਰਿਆ_cache_refresh(obj, LV_PART_ANY);
    }
#ਜੇ LV_OBJ_STYLE_CACHE
    ਨਹੀਂਤਾਂ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        ਜੇ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_ਸੈੱਟ |= prop_shifted;
        }
        ਨਹੀਂਤਾਂ {
            obj->style_other_prop_is_ਸੈੱਟ |= prop_shifted;
        }
    }
#ਜੇ_ਅੰਤ

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_ਸਿਰਾ;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ਮੁੱਲ_t * ਮੁੱਲ,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_STYLE_RES_NOT_ਮਿਲਿਆ);
    LV_CHECK_ARG(ਮੁੱਲ != ਨਿਰਾ, ਵਾਪਸ LV_STYLE_RES_NOT_ਮਿਲਿਆ);

    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ਵਾਪਸ lv_style_get_prop_ਅੰਦਰੂਨੀ(obj->styles[i].style, prop, ਮੁੱਲ);
        }
    }

    ਵਾਪਸ LV_STYLE_RES_NOT_ਮਿਲਿਆ;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    uint32_t i;
    /*Find the style*/
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ਰੋਕੋ;
        }
    }

    /*The style is not found*/
    ਜੇ(i == obj->style_cnt) ਵਾਪਸ false;

    lv_ਨਤੀਜਾ_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    ਜੇ(res == LV_ਨਤੀਜਾ_OK) {
        ਭਰਿਆ_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    ਵਾਪਸ res;
}

ਖਾਲੀ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ਅਚਲ lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(tr_dsc != ਨਿਰਾ, ਵਾਪਸ);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_ਮੁੱਲ_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_ਮੁੱਲ_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    ਜੇ(v1.ptr == v2.ptr && v1.ਅੰਕ == v2.ਅੰਕ && lv_color_eq(v1.color, v2.color))  ਵਾਪਸ;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_ਸੈੱਟ_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    ਜੇ(tr_dsc->prop == LV_STYLE_RADIUS) {
        ਜੇ(v1.ਅੰਕ == LV_RADIUS_CIRCLE || v2.ਅੰਕ == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_ਚੌੜਾਈ(obj) / 2;
            int32_t hhalf = lv_obj_get_ਉਚਾਈ(obj) / 2;
            ਜੇ(v1.ਅੰਕ == LV_RADIUS_CIRCLE) v1.ਅੰਕ = LV_MIN(whalf + 1, hhalf + 1);
            ਜੇ(v2.ਅੰਕ == LV_RADIUS_CIRCLE) v2.ਅੰਕ = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_ਸਿਰ(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    ਜੇ(tr == ਨਿਰਾ) ਵਾਪਸ;
    tr->ਸ਼ੁਰੂ_ਮੁੱਲ = v1;
    tr->ਸਿਰਾ_ਮੁੱਲ = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ਸੈੱਟ_var(&a, tr);
    lv_anim_ਸੈੱਟ_exec_cb(&a, trans_anim_cb);
    lv_anim_ਸੈੱਟ_ਸ਼ੁਰੂ_cb(&a, trans_anim_ਸ਼ੁਰੂ_cb);
    lv_anim_ਸੈੱਟ_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_ਸੈੱਟ_ਮੁੱਲਾਂ(&a, 0x00, 0xFF);
    lv_anim_ਸੈੱਟ_duration(&a, tr_dsc->ਸਮਾਂ);
    lv_anim_ਸੈੱਟ_delay(&a, tr_dsc->delay);
    lv_anim_ਸੈੱਟ_ਰਸਤਾ_cb(&a, tr_dsc->ਰਸਤਾ_cb);
    lv_anim_ਸੈੱਟ_early_apply(&a, false);
    lv_anim_ਸੈੱਟ_user_ਅੰਕੜੇ(&a, tr_dsc->user_ਅੰਕੜੇ);
    lv_anim_ਸ਼ੁਰੂ(&a);
}

lv_style_ਮੁੱਲ_t lv_obj_style_apply_color_filter(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_style_ਮੁੱਲ_t v)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ v);
    ਵਾਪਸ lv_obj_style_apply_color_filter_ਅੰਦਰੂਨੀ(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * ਬਦਲਿਆ_part)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_STYLE_STATE_CMP_SAME);

    lv_style_state_cmp_t res = LV_STYLE_STATE_CMP_SAME;

    /*Accumulate which part the differing styles belong to: the lone differing
     *part, or LV_PART_ANY once styles from more than one part differ. Reported
     *via changed_part so the caller can scope the refresh to a single part.
     *diff_part_found is needed as a separate flag because a style's part can
     *itself be LV_PART_ANY, so LV_PART_ANY can't double as "none seen yet".*/
    lv_part_t diff_part = LV_PART_ANY;
    bool diff_part_ਮਿਲਿਆ = false;

    /*Are there any new styles for the new state?*/
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_trans) ਜਾਰੀ;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        ਜੇ(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            ਜੇ(!diff_part_ਮਿਲਿਆ) {
                diff_part = part_act;
                diff_part_ਮਿਲਿਆ = true;
            }
            ਨਹੀਂਤਾਂ ਜੇ(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ਅਚਲ lv_style_t * style = obj->styles[i].style;
            lv_style_ਮੁੱਲ_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_ਸਿਖਰ, &v))layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_ਖੱਬਾ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_ਸੱਜਾ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_ਕਾਲਮ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_PAD_ਪੰਕਤੀ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_ਚੌੜਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_ਉਚਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_MIN_ਚੌੜਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_MAX_ਚੌੜਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_MIN_ਉਚਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_MAX_ਉਚਾਈ, &v)) layout_diff = true;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_BORDER_ਚੌੜਾਈ, &v)) layout_diff = true;

            ਜੇ(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                ਜੇ(ਬਦਲਿਆ_part) *ਬਦਲਿਆ_part = LV_PART_ANY;
                ਵਾਪਸ LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSFORM_ਚੌੜਾਈ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSFORM_ਉਚਾਈ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_OUTLINE_ਚੌੜਾਈ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_SHADOW_ਚੌੜਾਈ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_SHADOW_ਵਿੱਥ_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_SHADOW_ਵਿੱਥ_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_SHADOW_ਫੈਲਾਅ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(style, LV_STYLE_ਸਤਰ_ਚੌੜਾਈ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ਨਹੀਂਤਾਂ ਜੇ(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    ਜੇ(ਬਦਲਿਆ_part) *ਬਦਲਿਆ_part = diff_part;
    ਵਾਪਸ res;
}

ਖਾਲੀ lv_obj_fade_ਵਿੱਚ(lv_obj_t * obj, uint32_t ਸਮਾਂ, uint32_t delay)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ਸੈੱਟ_var(&a, obj);
    lv_anim_ਸੈੱਟ_ਮੁੱਲਾਂ(&a, 0, LV_OPA_COVER);
    lv_anim_ਸੈੱਟ_exec_cb(&a, fade_anim_cb);
    lv_anim_ਸੈੱਟ_completed_cb(&a, fade_ਵਿੱਚ_anim_completed);
    lv_anim_ਸੈੱਟ_duration(&a, ਸਮਾਂ);
    lv_anim_ਸੈੱਟ_delay(&a, delay);
    lv_anim_ਸ਼ੁਰੂ(&a);
}

ਖਾਲੀ lv_obj_fade_ਬਾਹਰਲਾ(lv_obj_t * obj, uint32_t ਸਮਾਂ, uint32_t delay)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ਸੈੱਟ_var(&a, obj);
    lv_anim_ਸੈੱਟ_ਮੁੱਲਾਂ(&a, lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_ਸੈੱਟ_exec_cb(&a, fade_anim_cb);
    lv_anim_ਸੈੱਟ_duration(&a, ਸਮਾਂ);
    lv_anim_ਸੈੱਟ_delay(&a, delay);
    lv_anim_ਸ਼ੁਰੂ(&a);
}

lv_ਪਾਠ_align_t lv_obj_calculate_style_ਪਾਠ_align(ਅਚਲ lv_obj_t * obj, lv_part_t part, ਅਚਲ ਅੱਖਰ * txt)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_ਪਾਠ_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ਨਿਰਾ, ਵਾਪਸ LV_ਪਾਠ_ALIGN_AUTO);
    ਵਾਪਸ lv_obj_calculate_style_ਪਾਠ_align_ਅੰਦਰੂਨੀ(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_ਸਿਰਾ;
        ਵਾਪਸ LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    ਜੇ(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    ਜੇ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ਨਹੀਂਤਾਂ {
        obj = lv_obj_get_ਮਾਪਾ(obj);
    }

    ਜਦੋਂ(obj) {
        opa_obj = lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_ਸਿਰਾ;
            ਵਾਪਸ LV_OPA_TRANSP;
        }
        ਜੇ(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_ਮਾਪਾ(obj);
    }

    ਜੇ(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_ਸਿਰਾ;
        ਵਾਪਸ LV_OPA_TRANSP;
    }

    ਜੇ(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_ਸਿਰਾ;
        ਵਾਪਸ LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_ਸਿਰਾ;
    ਵਾਪਸ opa_final;
}

ਖਾਲੀ lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    ਜੇ(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    ਨਹੀਂਤਾਂ ਜੇ(layer_type != LV_LAYER_TYPE_NONE) {
        ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
            ਵਾਪਸ;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

ਖਾਲੀ lv_obj_update_blur_ਸਥਿਤੀ(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    ਜੇ(obj->is_deleting) ਵਾਪਸ;

    bool has_blur = calculate_has_blur(obj);
    ਜੇ(has_blur == (bool)obj->has_blur) ਵਾਪਸ;

    obj->has_blur = has_blur;
    ਜੇ(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    ਨਹੀਂਤਾਂ {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_ਅੰਦਰੂਨੀ(obj, part);
        color = lv_color_over32(color, lv_color_ਮੰਜ਼ਿਲ_32(recolor, opa));
    }

    ਵਾਪਸ color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ (lv_color32_t) {
        0
    });

    lv_color32_t ਨਤੀਜਾ;

    lv_color_t color = lv_obj_get_style_recolor_ਅੰਦਰੂਨੀ(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_ਅੰਦਰੂਨੀ(obj, part);

    ਨਤੀਜਾ = lv_color_ਮੰਜ਼ਿਲ_32(color, opa);

    ਜੇ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ਨਹੀਂਤਾਂ {
        obj = lv_obj_get_ਮਾਪਾ(obj);
    }

    ਜਦੋਂ(obj) {
        ਨਤੀਜਾ = lv_obj_style_apply_recolor(obj, part, ਨਤੀਜਾ);
        obj = lv_obj_get_ਮਾਪਾ(obj);
    }

    ਵਾਪਸ ਨਤੀਜਾ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_pad_all(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_pad_ਖੱਬਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_ਸੱਜਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_ਸਿਖਰ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_bottom(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_pad_hor(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_pad_ਖੱਬਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_ਸੱਜਾ(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_pad_ver(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_pad_ਸਿਖਰ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_bottom(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_margin_all(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_margin_ਖੱਬਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_margin_ਸੱਜਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_margin_ਸਿਖਰ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_margin_bottom(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_margin_hor(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_margin_ਖੱਬਾ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_margin_ਸੱਜਾ(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_margin_ver(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_margin_ਸਿਖਰ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_margin_bottom(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_pad_gap(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_pad_ਪੰਕਤੀ(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_pad_ਕਾਲਮ(obj, ਮੁੱਲ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_ਮਾਪ(lv_obj_t * obj, int32_t ਚੌੜਾਈ, int32_t ਉਚਾਈ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_ਚੌੜਾਈ(obj, ਚੌੜਾਈ, selector);
    lv_obj_ਸੈੱਟ_style_ਉਚਾਈ(obj, ਉਚਾਈ, selector);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_style_transform_scale(lv_obj_t * obj, int32_t ਮੁੱਲ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_transform_scale_x(obj, ਮੁੱਲ, selector);
    lv_obj_ਸੈੱਟ_style_transform_scale_y(obj, ਮੁੱਲ, selector);
}

int32_t lv_obj_get_style_space_ਖੱਬਾ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, part);
}

int32_t lv_obj_get_style_space_ਸੱਜਾ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, part);
}

int32_t lv_obj_get_style_space_ਸਿਖਰ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_ਸੁਰੱਖਿਅਤ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_transform_scale_x_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_ਸੁਰੱਖਿਅਤ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ਵਾਪਸ 0)
    ਵਾਪਸ lv_obj_get_style_transform_scale_y_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(obj, part);
}


#ਜੇ LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_ਮੁੱਲ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(style != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        ਵਾਪਸ ਨਿਰਾ;
    }

    lv_obj_ਜੋੜੋ_style(obj, style, selector);

    bind_style_t * p = lv_malloc(ਆਕਾਰ(bind_style_t));
    LV_ASSERT_MALLOC(p);
    ਜੇ(p == ਨਿਰਾ) {
        LV_LOG_WARN("Out of memory");
        ਵਾਪਸ ਨਿਰਾ;
    }

    p->style = style;
    p->selector = selector;
    p->ਮੁੱਲ = ref_ਮੁੱਲ;

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_ਅੰਕੜੇ = 1;
    ਵਾਪਸ observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(subject != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_ਸੂਚਕ) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        ਵਾਪਸ ਨਿਰਾ;
    }

    bind_style_prop_t * p = lv_malloc(ਆਕਾਰ(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    ਜੇ(p == ਨਿਰਾ) {
        LV_LOG_WARN("Out of memory");
        ਵਾਪਸ ਨਿਰਾ;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_ਜੋੜੋ_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_ਅੰਕੜੇ = 1;
    ਵਾਪਸ observable;
}


#ਜੇ_ਅੰਤ /*LV_USE_OBSERVER*/


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
ਸਥਿਰ lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ਵਾਪਸ (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(ਆਕਾਰ(lv_style_t));
    ਜੇ(new_style == ਨਿਰਾ) {
        LV_LOG_WARN("couldn't allocate local style");
        ਵਾਪਸ ਨਿਰਾ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ਆਕਾਰ(lv_obj_style_t));
    ਜੇ(new_styles == ਨਿਰਾ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ਵਾਪਸ ਨਿਰਾ;
    }
    obj->styles = new_styles;

    ਲਈ(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        ਜੇ(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ਰੋਕੋ;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], ਆਕਾਰ(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    ਵਾਪਸ (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
ਸਥਿਰ lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_trans && obj->styles[i].selector == selector) ਰੋਕੋ;
    }

    /*Already have a transition style for it*/
    ਜੇ(i != obj->style_cnt) ਵਾਪਸ &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(ਆਕਾਰ(lv_style_t));
    ਜੇ(new_style == ਨਿਰਾ) {
        LV_LOG_WARN("couldn't allocate transition style");
        ਵਾਪਸ ਨਿਰਾ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ਆਕਾਰ(lv_obj_style_t));
    ਜੇ(new_styles == ਨਿਰਾ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ਵਾਪਸ ਨਿਰਾ;
    }
    obj->styles = new_styles;

    ਲਈ(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], ਆਕਾਰ(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    ਵਾਪਸ &obj->styles[0];
}

ਸਥਿਰ lv_style_res_t get_prop_core(ਅਚਲ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ਮੁੱਲ_t * v)
{

    ਅਚਲ uint32_t ਸਮੂਹ = (uint32_t)1 << lv_style_get_prop_ਸਮੂਹ(prop);
    ਅਚਲ lv_part_t part = lv_obj_style_get_selector_part(selector);
    ਅਚਲ bool skip_trans = (ਅਚਲ bool) obj->skip_trans;
    int32_t ਭਾਰ = -1;
    lv_style_res_t ਮਿਲਿਆ;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ਜੇ(obj_style->is_trans == false) ਰੋਕੋ;
        ਜੇ(skip_trans) ਜਾਰੀ;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        ਜੇ(part_act != part) ਜਾਰੀ;
        ਜੇ((obj_style->style->has_ਸਮੂਹ & ਸਮੂਹ) == 0) ਜਾਰੀ;
        ਮਿਲਿਆ = lv_style_get_prop_ਅੰਦਰੂਨੀ(obj_style->style, prop, v);
        ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) {
            ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ਅਚਲ lv_state_t state = lv_obj_style_get_selector_state(selector);
    ਅਚਲ lv_state_t state_inv = ~state;
    ਲਈ(; i < obj->style_cnt; i++) {
        ਜੇ((obj->styles[i].style->has_ਸਮੂਹ & ਸਮੂਹ) == 0) ਜਾਰੀ;
        ਜੇ(obj->styles[i].is_disabled) ਜਾਰੀ;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ਜੇ(part_act != part) ਜਾਰੀ;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ਜੇ((state_act & state_inv)) ਜਾਰੀ;

        /*Check only better candidates*/
        ਜੇ((int32_t)state_act <= ਭਾਰ) ਜਾਰੀ;

        ਮਿਲਿਆ = lv_style_get_prop_ਅੰਦਰੂਨੀ(obj_style->style, prop, v);
        ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) {
            ਜੇ(state_act == state) {
                ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
            }
            ਭਾਰ = state_act;
        }
    }

    ਜੇ(ਭਾਰ >= 0) ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_STYLE_RES_NOT_ਮਿਲਿਆ;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
ਸਥਿਰ ਖਾਲੀ report_style_change_core(ਖਾਲੀ * style, lv_obj_t * obj)
{
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(style == ਨਿਰਾ || obj->styles[i].style == style) {
            ਭਰਿਆ_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ਰੋਕੋ;
        }
    }

    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
ਸਥਿਰ ਖਾਲੀ refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        lv_obj_invalidate(ਬੱਚਾ);
        lv_obj_send_event(ਬੱਚਾ, LV_EVENT_STYLE_ਬਦਲਿਆ, ਨਿਰਾ);
        lv_obj_invalidate(ਬੱਚਾ);

        refresh_children_style(ਬੱਚਾ); /*Check children too*/
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
ਸਥਿਰ bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_ਹੱਦਬੰਦੀ)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_ਪੂਛ(style_trans_ll_p);
    ਜਦੋਂ(tr != ਨਿਰਾ) {
        ਜੇ(tr == tr_ਹੱਦਬੰਦੀ) ਰੋਕੋ;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        ਜੇ(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            ਜੇ(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            ਲਈ(i = 0; i < obj->style_cnt; i++) {
                ਜੇ(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ਨਿਰਾ);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    ਵਾਪਸ removed;
}

ਸਥਿਰ ਖਾਲੀ trans_anim_cb(ਖਾਲੀ * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) ਜਾਰੀ;

        lv_style_ਮੁੱਲ_t ਮੁੱਲ_final = {0};
        ਚੋਣ(tr->prop) {

            ਹਾਲਤ LV_STYLE_BORDER_SIDE:
            ਹਾਲਤ LV_STYLE_BORDER_POST:
            ਹਾਲਤ LV_STYLE_BLEND_ਬਹੁਲਕ:
                ਜੇ(v < 255) ਮੁੱਲ_final.ਅੰਕ = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.ਅੰਕ = tr->ਸਿਰਾ_ਮੁੱਲ.ਅੰਕ;
                ਰੋਕੋ;
            ਹਾਲਤ LV_STYLE_TRANSITION:
            ਹਾਲਤ LV_STYLE_ਪਾਠ_FONT:
            ਹਾਲਤ LV_STYLE_BG_IMAGE_SRC:
                ਜੇ(v < 255) ਮੁੱਲ_final.ptr = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ptr;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.ptr = tr->ਸਿਰਾ_ਮੁੱਲ.ptr;
                ਰੋਕੋ;
            ਹਾਲਤ LV_STYLE_COLOR_FILTER_DSC:
                ਜੇ(tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ptr == ਨਿਰਾ) ਮੁੱਲ_final.ptr = tr->ਸਿਰਾ_ਮੁੱਲ.ptr;
                ਨਹੀਂਤਾਂ ਜੇ(tr->ਸਿਰਾ_ਮੁੱਲ.ptr == ਨਿਰਾ) ਮੁੱਲ_final.ptr = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ptr;
                ਨਹੀਂਤਾਂ ਜੇ(v < 128) ਮੁੱਲ_final.ptr = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ptr;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.ptr = tr->ਸਿਰਾ_ਮੁੱਲ.ptr;
                ਰੋਕੋ;
            /*Colors*/
            ਹਾਲਤ LV_STYLE_RECOLOR:
            ਹਾਲਤ LV_STYLE_BG_COLOR:
            ਹਾਲਤ LV_STYLE_BG_GRAD_COLOR:
            ਹਾਲਤ LV_STYLE_BORDER_COLOR:
            ਹਾਲਤ LV_STYLE_ਪਾਠ_COLOR:
            ਹਾਲਤ LV_STYLE_SHADOW_COLOR:
            ਹਾਲਤ LV_STYLE_OUTLINE_COLOR:
            ਹਾਲਤ LV_STYLE_IMAGE_RECOLOR:
                ਜੇ(v <= 0) ਮੁੱਲ_final.color = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.color;
                ਨਹੀਂਤਾਂ ਜੇ(v >= 255) ਮੁੱਲ_final.color = tr->ਸਿਰਾ_ਮੁੱਲ.color;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.color = lv_color_mix(tr->ਸਿਰਾ_ਮੁੱਲ.color, tr->ਸ਼ੁਰੂ_ਮੁੱਲ.color, v);
                ਰੋਕੋ;
            /*Opacities*/
            ਹਾਲਤ LV_STYLE_OPA:
            ਹਾਲਤ LV_STYLE_OPA_LAYERED:
            ਹਾਲਤ LV_STYLE_RECOLOR_OPA:
            ਹਾਲਤ LV_STYLE_COLOR_FILTER_OPA:
            ਹਾਲਤ LV_STYLE_BG_OPA:
            ਹਾਲਤ LV_STYLE_BG_GRAD_OPA:
            ਹਾਲਤ LV_STYLE_BG_MAIN_OPA:
            ਹਾਲਤ LV_STYLE_BG_IMAGE_OPA:
            ਹਾਲਤ LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            ਹਾਲਤ LV_STYLE_BORDER_OPA:
            ਹਾਲਤ LV_STYLE_OUTLINE_OPA:
            ਹਾਲਤ LV_STYLE_SHADOW_OPA:
            ਹਾਲਤ LV_STYLE_ਸਤਰ_OPA:
            ਹਾਲਤ LV_STYLE_ARC_OPA:
            ਹਾਲਤ LV_STYLE_ਪਾਠ_OPA:
            ਹਾਲਤ LV_STYLE_IMAGE_OPA:
            ਹਾਲਤ LV_STYLE_IMAGE_RECOLOR_OPA:
            ਹਾਲਤ LV_STYLE_DROP_SHADOW_OPA:
            ਹਾਲਤ LV_STYLE_ਪਾਠ_OUTLINE_STROKE_OPA:
                ਜੇ(v <= 0) ਮੁੱਲ_final.ਅੰਕ = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ;
                ਨਹੀਂਤਾਂ ਜੇ(v >= 255) ਮੁੱਲ_final.ਅੰਕ = tr->ਸਿਰਾ_ਮੁੱਲ.ਅੰਕ;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.ਅੰਕ =  LV_CLAMP(0, tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ + ((tr->ਸਿਰਾ_ਮੁੱਲ.ਅੰਕ - tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ) * v >> 8), 255);
                ਰੋਕੋ;

            ਮੂਲ:
                ਜੇ(v == 0) ਮੁੱਲ_final.ਅੰਕ = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ;
                ਨਹੀਂਤਾਂ ਜੇ(v == 255) ਮੁੱਲ_final.ਅੰਕ = tr->ਸਿਰਾ_ਮੁੱਲ.ਅੰਕ;
                ਨਹੀਂਤਾਂ ਮੁੱਲ_final.ਅੰਕ = tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ + ((int32_t)((int32_t)(tr->ਸਿਰਾ_ਮੁੱਲ.ਅੰਕ - tr->ਸ਼ੁਰੂ_ਮੁੱਲ.ਅੰਕ) * v) >> 8);
                ਰੋਕੋ;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_ਮੁੱਲ_t old_ਮੁੱਲ = {0};
        ਜੇ(lv_style_get_prop_ਅੰਦਰੂਨੀ(obj->styles[i].style, tr->prop, &old_ਮੁੱਲ) == LV_STYLE_RES_ਮਿਲਿਆ) {
            ਜੇ(ਮੁੱਲ_final.ptr == old_ਮੁੱਲ.ptr &&
               lv_color_eq(ਮੁੱਲ_final.color, old_ਮੁੱਲ.color) &&
               ਮੁੱਲ_final.ਅੰਕ == old_ਮੁੱਲ.ਅੰਕ) {
                ਰੋਕੋ;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        ਜੇ(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_ਸੈੱਟ_prop((lv_style_t *)obj->styles[i].style, tr->prop, ਮੁੱਲ_final);

#ਜੇ LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ਜੇ(part == LV_PART_MAIN) {
            obj->style_main_prop_is_ਸੈੱਟ |= prop_shifted;
        }
        ਨਹੀਂਤਾਂ {
            obj->style_other_prop_is_ਸੈੱਟ |= prop_shifted;
        }
#ਜੇ_ਅੰਤ

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ਰੋਕੋ;
    }

}

ਸਥਿਰ ਖਾਲੀ trans_anim_ਸ਼ੁਰੂ_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->ਸ਼ੁਰੂ_ਮੁੱਲ = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_ਸੈੱਟ_prop((lv_style_t *)style_trans->style, tr->prop, tr->ਸ਼ੁਰੂ_ਮੁੱਲ);
#ਜੇ LV_OBJ_STYLE_CACHE
    ਜੇ(trans_removed) {
        ਭਰਿਆ_cache_refresh(tr->obj, part);
    }
    ਨਹੀਂਤਾਂ {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ਜੇ(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_ਸੈੱਟ |= prop_shifted;
        }
        ਨਹੀਂਤਾਂ {
            tr->obj->style_other_prop_is_ਸੈੱਟ |= prop_shifted;
        }
    }
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(trans_removed);
#ਜੇ_ਅੰਤ

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

ਸਥਿਰ ਖਾਲੀ trans_anim_completed_cb(lv_anim_t * a)
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
        ਜੇ(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ਰੋਕੋ;
        }
    }

    ਜੇ(!running) {
        uint32_t i;
        ਲਈ(i = 0; i < obj->style_cnt; i++) {
            ਜੇ(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                ਜੇ(lv_style_is_ਖਾਲੀ_ਥਾਵਾਂ(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ਰੋਕੋ;
            }
        }
    }
}

ਸਥਿਰ lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਜੇ(lv_obj_get_transform(obj) != ਨਿਰਾ) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
#ਜੇ_ਅੰਤ
    ਜੇ(lv_obj_get_style_transform_rotation_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != 0) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
    ਜੇ(lv_obj_get_style_transform_scale_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != 256) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
    ਜੇ(lv_obj_get_style_transform_scale_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != 256) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
    ਜੇ(lv_obj_get_style_transform_skew_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != 0) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
    ਜੇ(lv_obj_get_style_transform_skew_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != 0) ਵਾਪਸ LV_LAYER_TYPE_TRANSFORM;
    ਜੇ(lv_obj_get_style_opa_layered_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_OPA_COVER) ਵਾਪਸ LV_LAYER_TYPE_SIMPLE;
    ਜੇ(lv_obj_get_style_bitmap_ਨਕਾਬ_src_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != ਨਿਰਾ) ਵਾਪਸ LV_LAYER_TYPE_SIMPLE;
    ਜੇ(lv_obj_get_style_blend_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_BLEND_ਬਹੁਲਕ_ਸਾਧਾਰਨ) ਵਾਪਸ LV_LAYER_TYPE_SIMPLE;
    ਵਾਪਸ LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
ਸਥਿਰ bool calculate_has_blur(lv_obj_t * obj)
{
    ਅਚਲ uint32_t ਸਮੂਹ_blur = (uint32_t)1 << lv_style_get_prop_ਸਮੂਹ(LV_STYLE_BLUR_RADIUS);
    ਅਚਲ uint32_t ਸਮੂਹ_dropshadow = (uint32_t)1 << lv_style_get_prop_ਸਮੂਹ(LV_STYLE_DROP_SHADOW_OPA);
    ਅਚਲ lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ਅਚਲ lv_state_t state_inv = ~state;
    lv_style_ਮੁੱਲ_t v;
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ਜੇ(obj_style->is_disabled) ਜਾਰੀ;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ਜੇ((state_style & state_inv)) ਜਾਰੀ;

        ਜੇ((obj_style->style->has_ਸਮੂਹ & ਸਮੂਹ_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            ਜੇ(v.ਅੰਕ > 0) ਵਾਪਸ true;
        }
        ਜੇ((obj_style->style->has_ਸਮੂਹ & ਸਮੂਹ_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            ਜੇ(v.ਅੰਕ > 0) ਵਾਪਸ true;
        }
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ ਭਰਿਆ_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#ਜੇ LV_OBJ_STYLE_CACHE
    uint32_t i;
    ਜੇ(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_ਸੈੱਟ = 0;
        ਲਈ(i = 0; i < obj->style_cnt; i++) {
            ਜੇ(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) ਜਾਰੀ;
            ਜੇ(obj->styles[i].is_disabled) ਜਾਰੀ;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ਜੇ(lv_style_is_const_ਅੰਦਰੂਨੀ(style)) {
                lv_style_const_prop_t * props = style->ਮੁੱਲਾਂ_and_props;
                ਲਈ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_ਸੈੱਟ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ਨਹੀਂਤਾਂ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ਮੁੱਲਾਂ_and_props + style->prop_cnt * ਆਕਾਰ(lv_style_ਮੁੱਲ_t);
                ਲਈ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_ਸੈੱਟ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    ਜੇ(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_ਸੈੱਟ = 0;
        ਲਈ(i = 0; i < obj->style_cnt; i++) {
            ਜੇ(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) ਜਾਰੀ;
            ਜੇ(obj->styles[i].is_disabled) ਜਾਰੀ;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ਜੇ(lv_style_is_const_ਅੰਦਰੂਨੀ(style)) {
                lv_style_const_prop_t * props = style->ਮੁੱਲਾਂ_and_props;
                ਲਈ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_ਸੈੱਟ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ਨਹੀਂਤਾਂ {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ਮੁੱਲਾਂ_and_props + style->prop_cnt * ਆਕਾਰ(lv_style_ਮੁੱਲ_t);
                ਲਈ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_ਸੈੱਟ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_ਅਣਵਰਤਿਆ(part);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ fade_anim_cb(ਖਾਲੀ * obj, int32_t v)
{
    lv_obj_ਸੈੱਟ_style_opa(obj, v, 0);
}

ਸਥਿਰ ਖਾਲੀ fade_ਵਿੱਚ_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

ਸਥਿਰ bool style_has_flag(ਅਚਲ lv_style_t * style, uint32_t flag)
{
    ਜੇ(lv_style_is_const_ਅੰਦਰੂਨੀ(style)) {
        lv_style_const_prop_t * props = style->ਮੁੱਲਾਂ_and_props;
        uint32_t i;
        ਲਈ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            ਜੇ(lv_style_prop_has_flag(props[i].prop, flag)) {
                ਵਾਪਸ true;
            }
        }
    }
    ਨਹੀਂਤਾਂ {
        lv_style_prop_t * props = (lv_style_prop_t *)style->ਮੁੱਲਾਂ_and_props + style->prop_cnt * ਆਕਾਰ(lv_style_ਮੁੱਲ_t);
        uint32_t i;
        ਲਈ(i = 0; i < style->prop_cnt; i++) {
            ਜੇ(lv_style_prop_has_flag(props[i], flag)) {
                ਵਾਪਸ true;
            }
        }
    }
    ਵਾਪਸ false;
}

ਸਥਿਰ lv_style_res_t get_selector_style_prop(ਅਚਲ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ਮੁੱਲ_t * ਮੁੱਲ_act)
{
    lv_style_res_t ਮਿਲਿਆ;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#ਜੇ LV_OBJ_STYLE_CACHE
    ਅਚਲ uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    ਜੇ((part == LV_PART_MAIN ? obj->style_main_prop_is_ਸੈੱਟ : obj->style_other_prop_is_ਸੈੱਟ) & prop_shifted)
#ਜੇ_ਅੰਤ
    {
        ਮਿਲਿਆ = get_prop_core(obj, selector, prop, ਮੁੱਲ_act);
        ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
    }

    ਬਾਹਰੀ ਅਚਲ uint8_t lv_style_builtin_prop_flag_lookup_ਸਾਰਣੀ[];
    bool inheritable = false;
    ਜੇ(prop < LV_STYLE_ਅੰਕ_BUILT_ਵਿੱਚ_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_ਸਾਰਣੀ[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    ਨਹੀਂਤਾਂ {
        ਜੇ(style_custom_prop_flag_lookup_ਸਾਰਣੀ_2 != ਨਿਰਾ) {
            inheritable = style_custom_prop_flag_lookup_ਸਾਰਣੀ_2[prop - LV_STYLE_ਅੰਕ_BUILT_ਵਿੱਚ_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    ਜੇ(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        ਜੇ(part != LV_PART_MAIN) part = LV_PART_MAIN;
        ਨਹੀਂਤਾਂ obj = obj->ਮਾਪਾ;

        ਜਦੋਂ(obj) {
#ਜੇ LV_OBJ_STYLE_CACHE
            ਜੇ(obj->style_main_prop_is_ਸੈੱਟ & prop_shifted)
#ਜੇ_ਅੰਤ
            {
                selector = part | obj->state;
                ਮਿਲਿਆ = get_prop_core(obj, selector, prop, ਮੁੱਲ_act);
                ਜੇ(ਮਿਲਿਆ == LV_STYLE_RES_ਮਿਲਿਆ) ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
            }
            /*Check the parent too.*/
            obj = obj->ਮਾਪਾ;
        }
    }
    ਨਹੀਂਤਾਂ {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        ਜੇ(part == LV_PART_MAIN && (prop == LV_STYLE_ਚੌੜਾਈ || prop == LV_STYLE_ਉਚਾਈ)) {
            ਅਚਲ lv_obj_class_t * cls = obj->class_p;
            ਜਦੋਂ(cls) {
                ਜੇ(prop == LV_STYLE_ਚੌੜਾਈ) {
                    ਜੇ(cls->ਚੌੜਾਈ_def != 0)  {
                        ਮੁੱਲ_act->ਅੰਕ = cls->ਚੌੜਾਈ_def;
                        ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
                    }
                }
                ਨਹੀਂਤਾਂ {
                    ਜੇ(cls->ਉਚਾਈ_def != 0) {
                        ਮੁੱਲ_act->ਅੰਕ = cls->ਉਚਾਈ_def;
                        ਵਾਪਸ LV_STYLE_RES_ਮਿਲਿਆ;
                    }
                }
                cls = cls->ਅਧਾਰ_class;
            }
        }
    }

    ਵਾਪਸ LV_STYLE_RES_NOT_ਮਿਲਿਆ;
}

ਸਥਿਰ ਖਾਲੀ remove_style_core(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    ਜੇ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ਨਿਰਾ || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ਗਿਣਤੀ = 0;
    bool deleted = false;

    ਲਈ(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        ਜੇ(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        ਨਹੀਂਤਾਂ ਜੇ((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ਨਿਰਾ && style != obj->styles[i].style)) {
            should_remove = false;
        }

        ਜੇ(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            ਜੇ(style_ਗਿਣਤੀ != i) {
                obj->styles[style_ਗਿਣਤੀ] = obj->styles[i];
            }
            style_ਗਿਣਤੀ++;
            ਜਾਰੀ;
        }

        /*This entry is being removed*/
        ਜੇ(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ਨਿਰਾ);
        }
        ਜੇ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ਜੇ(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ਨਿਰਾ;
        }
        deleted = true;
    }

    ਜੇ(!deleted) {
        ਵਾਪਸ;
    }

    obj->style_cnt = style_ਗਿਣਤੀ;
    ਜੇ(style_ਗਿਣਤੀ > 0) {
        obj->styles = lv_realloc(obj->styles, style_ਗਿਣਤੀ * ਆਕਾਰ(lv_obj_style_t));
    }
    ਨਹੀਂਤਾਂ {
        lv_free(obj->styles);
        obj->styles = ਨਿਰਾ;
    }

    ਜੇ(deleted) {
        ਭਰਿਆ_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

ਸਥਿਰ lv_obj_style_t * ਲੱਭੋ_style(lv_obj_t * obj, ਅਚਲ lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    ਲਈ(i = 0; i < obj->style_cnt; i++) {
        ਜੇ(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            ਵਾਪਸ &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (ਖਾਲੀ *)style, (ਖਾਲੀ *)obj, selector);
    ਵਾਪਸ ਨਿਰਾ;
}



#ਜੇ LV_USE_OBSERVER

ਸਥਿਰ ਖਾਲੀ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_ਅੰਕੜੇ(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->ਮੁੱਲ);
    lv_obj_ਸੈੱਟ_style_enabled(observer->ਨਿਸ਼ਾਨਾ, p->style, p->selector, en);
}

ਸਥਿਰ ਖਾਲੀ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_ਅੰਕੜੇ(observer);

    lv_style_ਮੁੱਲ_t style_v;
    ਜੇ(subject->type == LV_SUBJECT_TYPE_INT) style_v.ਅੰਕ = lv_subject_get_int(subject);
    ਨਹੀਂਤਾਂ ਜੇ(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    ਨਹੀਂਤਾਂ ਜੇ(subject->type == LV_SUBJECT_TYPE_ਸੂਚਕ) style_v.ptr = lv_subject_get_ਸੂਚਕ(subject);
    ਨਹੀਂਤਾਂ {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        ਵਾਪਸ;
    }

    lv_obj_ਸੈੱਟ_local_style_prop(observer->ਨਿਸ਼ਾਨਾ, p->prop, style_v, p->selector);
}


#ਜੇ_ਅੰਤ /*LV_USE_OBSERVER*/
