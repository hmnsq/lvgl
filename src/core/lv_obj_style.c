/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../misc/lv_anim_private.h"
#అన్తర్భావ "../misc/lv_bidi_private.h"
#అన్తర్భావ "../misc/lv_style_private.h"
#అన్తర్భావ "lv_obj_style_private.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "../display/lv_display_private.h"
#అన్తర్భావ "../core/lv_global.h"
#అన్తర్భావ "lv_observer_private.h"
#అన్తర్భావ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)
#నిర్వచించు style_refr LV_GLOBAL_DEFAULT()->style_refresh
#నిర్వచించు style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#నిర్వచించు style_custom_prop_flag_lookup_పట్టిక_2 LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_పట్టిక
#నిర్వచించు STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#నిర్వచించు DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

రకనిర్వచనం నిర్మాణం {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_విలువ_t ప్రారంభం_విలువ;
    lv_style_విలువ_t చివర_విలువ;
} trans_t;

రకనిర్వచనం నిర్మాణం {
    స్థిరరాశి lv_style_t * style;
    lv_style_selector_t selector;
    int32_t విలువ;
} bind_style_t;

రకనిర్వచనం నిర్మాణం {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
స్థిర lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
స్థిర lv_style_res_t get_prop_core(స్థిరరాశి lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_విలువ_t * v);
స్థిర శూన్య report_style_change_core(శూన్య * style, lv_obj_t * obj);
స్థిర శూన్య refresh_children_style(lv_obj_t * obj);
స్థిర bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_హద్దు);
స్థిర శూన్య trans_anim_cb(శూన్య * _tr, int32_t v);
స్థిర శూన్య trans_anim_ప్రారంభం_cb(lv_anim_t * a);
స్థిర శూన్య trans_anim_completed_cb(lv_anim_t * a);
స్థిర lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
స్థిర bool calculate_has_blur(lv_obj_t * obj);
స్థిర శూన్య నిండిన_cache_refresh(lv_obj_t * obj, lv_part_t part);
స్థిర శూన్య fade_anim_cb(శూన్య * obj, int32_t v);
స్థిర శూన్య fade_లోపల_anim_completed(lv_anim_t * a);
స్థిర bool style_has_flag(స్థిరరాశి lv_style_t * style, uint32_t flag);
స్థిర lv_style_res_t get_selector_style_prop(స్థిరరాశి lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_విలువ_t * విలువ_act);
స్థిర శూన్య remove_style_core(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector, bool theme_only);
స్థిర lv_obj_style_t * కనుగొను_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector);
#ఒకవేళ LV_USE_OBSERVER
    స్థిర శూన్య bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    స్థిర శూన్య bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#అయితే_ముగింపు

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

శూన్య lv_obj_style_init(శూన్య)
{
    lv_ll_init(style_trans_ll_p, పరిమాణం(trans_t));
}

శూన్య lv_obj_style_deinit(శూన్య)
{
    lv_ll_clear(style_trans_ll_p);
    ఒకవేళ(style_custom_prop_flag_lookup_పట్టిక_2 != ఖాళీ) {
        lv_free(style_custom_prop_flag_lookup_పట్టిక_2);
        style_custom_prop_flag_lookup_పట్టిక_2 = ఖాళీ;
    }
}

శూన్య lv_obj_కూడు_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(style != ఖాళీ, తిరిగి);
    LV_CHECK_ARG_సందేశం(obj->style_cnt < 63, తిరిగి,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ఒకవేళ(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ఖాళీ);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_trans) కొనసాగు;
        ఒకవేళ(obj->styles[i].is_local) కొనసాగు;
        ఆపు;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * పరిమాణం(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    కొరకు(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], పరిమాణం(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    ఒకవేళ(trans_removed) {
        నిండిన_cache_refresh(obj, LV_PART_ANY);
    }
#ఒకవేళ LV_OBJ_STYLE_CACHE
    లేకపోతే {
        uint32_t * prop_is_సమితి = part == LV_PART_MAIN ? &obj->style_main_prop_is_సమితి : &obj->style_other_prop_is_సమితి;
        ఒకవేళ(lv_style_is_const_అంతర్గత(style)) {
            lv_style_const_prop_t * props = style->విలువలు_and_props;
            కొరకు(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_సమితి) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        లేకపోతే {
            lv_style_prop_t * props = (lv_style_prop_t *)style->విలువలు_and_props + style->prop_cnt * పరిమాణం(lv_style_విలువ_t);
            కొరకు(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_సమితి) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#అయితే_ముగింపు

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * old_style, స్థిరరాశి lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(old_style != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(new_style != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(old_style != new_style, తిరిగి false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ఒకవేళ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ఖాళీ);

    bool replaced = false;
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        ఒకవేళ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            కొనసాగు;
        }

        /*Skip non-matching styles*/
        ఒకవేళ((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            కొనసాగు;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], పరిమాణం(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    ఒకవేళ(replaced || trans_removed) {
        నిండిన_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    తిరిగి replaced;
}

శూన్య lv_obj_remove_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    remove_style_core(obj, style, selector, false);
}

శూన్య lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    remove_style_core(obj, ఖాళీ, selector, true);
}

శూన్య lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    remove_style_core(obj, ఖాళీ, LV_PART_ANY | LV_STATE_ANY, false);
}

శూన్య lv_obj_report_style_change(lv_style_t * style)
{
    ఒకవేళ(!style_refr) తిరిగి;
    lv_display_t * d = lv_display_get_తదుపరి(ఖాళీ);

    అయితే(d) {
        uint32_t i;
        కొరకు(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_తదుపరి(d);
    }
}

శూన్య lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!style_refr) తిరిగి;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    ఒకవేళ(is_layout_refr) {
        ఒకవేళ(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN) == LV_కొలత_CONTENT ||
           lv_obj_get_style_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN) == LV_కొలత_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_మారింది, ఖాళీ);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ఒకవేళ((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
        ఒకవేళ(మాతృ) lv_obj_mark_layout_as_dirty(మాతృ);
    }

    /*Cache the layer type*/
    ఒకవేళ((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    ఒకవేళ(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_స్థితి(obj);
    }

    ఒకవేళ(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_కొలత(obj);
    }
    lv_obj_invalidate(obj);

    ఒకవేళ(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        ఒకవేళ(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_చివర;
}

శూన్య lv_obj_సమితి_style_enabled(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(style != ఖాళీ, తిరిగి);

    lv_obj_style_t * obj_style = కనుగొను_style(obj, style, selector);
    ఒకవేళ(obj_style == ఖాళీ) తిరిగి;

    ఒకవేళ(en != obj_style->is_disabled) తిరిగి; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ఒకవేళ((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    నిండిన_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);
    LV_CHECK_ARG(style != ఖాళీ, తిరిగి false);

    lv_obj_style_t * obj_style = కనుగొను_style(obj, style, selector);
    ఒకవేళ(obj_style == ఖాళీ) తిరిగి false;

    తిరిగి !obj_style->is_disabled;
}


శూన్య lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_విలువ_t lv_obj_get_style_prop(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి lv_style_prop_get_default(prop));
    తిరిగి lv_obj_get_style_prop_అంతర్గత(obj, part, prop);
}

lv_style_విలువ_t lv_obj_get_style_prop_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ఖాళీ);
    lv_style_selector_t selector = part | obj->state;
    lv_style_విలువ_t విలువ_act = { .ptr = ఖాళీ };
    lv_style_res_t దొరికింది;

    దొరికింది = get_selector_style_prop(obj, selector, prop, &విలువ_act);
    ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) తిరిగి విలువ_act;

    తిరిగి lv_style_prop_get_default(prop);
}
lv_style_విలువ_t lv_obj_style_apply_color_filter_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_విలువ_t v)
{
#ఒకవేళ LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ఖాళీ);

    స్థిరరాశి lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_అంతర్గత(obj, part);
    ఒకవేళ(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_అంతర్గత(obj, part);
        ఒకవేళ(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#లేకపోతే
    LV_వాడని(obj);
    LV_వాడని(part);
    LV_వాడని(v);
#అయితే_ముగింపు
    తిరిగి v;
}

bool lv_obj_has_style_prop(స్థిరరాశి lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);

    lv_style_విలువ_t విలువ_act = { .ptr = ఖాళీ };
    lv_style_res_t దొరికింది;

    దొరికింది = get_selector_style_prop(obj, selector, prop, &విలువ_act);
    ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) తిరిగి true;

    తిరిగి false;
}

శూన్య lv_obj_సమితి_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_విలువ_t విలువ,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    ఒకవేళ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ఖాళీ);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_సమితి_prop(style, prop, విలువ);

    ఒకవేళ(trans_removed) {
        నిండిన_cache_refresh(obj, LV_PART_ANY);
    }
#ఒకవేళ LV_OBJ_STYLE_CACHE
    లేకపోతే {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        ఒకవేళ(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_సమితి |= prop_shifted;
        }
        లేకపోతే {
            obj->style_other_prop_is_సమితి |= prop_shifted;
        }
    }
#అయితే_ముగింపు

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_చివర;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_విలువ_t * విలువ,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_STYLE_RES_NOT_దొరికింది);
    LV_CHECK_ARG(విలువ != ఖాళీ, తిరిగి LV_STYLE_RES_NOT_దొరికింది);

    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            తిరిగి lv_style_get_prop_అంతర్గత(obj->styles[i].style, prop, విలువ);
        }
    }

    తిరిగి LV_STYLE_RES_NOT_దొరికింది;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    uint32_t i;
    /*Find the style*/
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ఆపు;
        }
    }

    /*The style is not found*/
    ఒకవేళ(i == obj->style_cnt) తిరిగి false;

    lv_ఫలితం_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    ఒకవేళ(res == LV_ఫలితం_OK) {
        నిండిన_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    తిరిగి res;
}

శూన్య lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    స్థిరరాశి lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(tr_dsc != ఖాళీ, తిరిగి);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_విలువ_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_విలువ_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    ఒకవేళ(v1.ptr == v2.ptr && v1.అంకె == v2.అంకె && lv_color_eq(v1.color, v2.color))  తిరిగి;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_సమితి_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    ఒకవేళ(tr_dsc->prop == LV_STYLE_RADIUS) {
        ఒకవేళ(v1.అంకె == LV_RADIUS_CIRCLE || v2.అంకె == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_వెడల్పు(obj) / 2;
            int32_t hhalf = lv_obj_get_ఎత్తు(obj) / 2;
            ఒకవేళ(v1.అంకె == LV_RADIUS_CIRCLE) v1.అంకె = LV_MIN(whalf + 1, hhalf + 1);
            ఒకవేళ(v2.అంకె == LV_RADIUS_CIRCLE) v2.అంకె = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_తల(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    ఒకవేళ(tr == ఖాళీ) తిరిగి;
    tr->ప్రారంభం_విలువ = v1;
    tr->చివర_విలువ = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_సమితి_var(&a, tr);
    lv_anim_సమితి_exec_cb(&a, trans_anim_cb);
    lv_anim_సమితి_ప్రారంభం_cb(&a, trans_anim_ప్రారంభం_cb);
    lv_anim_సమితి_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_సమితి_విలువలు(&a, 0x00, 0xFF);
    lv_anim_సమితి_duration(&a, tr_dsc->సమయం);
    lv_anim_సమితి_delay(&a, tr_dsc->delay);
    lv_anim_సమితి_మార్గం_cb(&a, tr_dsc->మార్గం_cb);
    lv_anim_సమితి_early_apply(&a, false);
    lv_anim_సమితి_user_దత్తాంశం(&a, tr_dsc->user_దత్తాంశం);
    lv_anim_ప్రారంభం(&a);
}

lv_style_విలువ_t lv_obj_style_apply_color_filter(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_విలువ_t v)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి v);
    తిరిగి lv_obj_style_apply_color_filter_అంతర్గత(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * మారింది_part)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_STYLE_STATE_CMP_SAME);

    lv_style_state_cmp_t res = LV_STYLE_STATE_CMP_SAME;

    /*Accumulate which part the differing styles belong to: the lone differing
     *part, or LV_PART_ANY once styles from more than one part differ. Reported
     *via changed_part so the caller can scope the refresh to a single part.
     *diff_part_found is needed as a separate flag because a style's part can
     *itself be LV_PART_ANY, so LV_PART_ANY can't double as "none seen yet".*/
    lv_part_t diff_part = LV_PART_ANY;
    bool diff_part_దొరికింది = false;

    /*Are there any new styles for the new state?*/
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_trans) కొనసాగు;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        ఒకవేళ(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            ఒకవేళ(!diff_part_దొరికింది) {
                diff_part = part_act;
                diff_part_దొరికింది = true;
            }
            లేకపోతే ఒకవేళ(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            స్థిరరాశి lv_style_t * style = obj->styles[i].style;
            lv_style_విలువ_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_శిఖరం, &v))layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_ఎడమ, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_కుడి, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_స్తంభం, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_PAD_అడ్డువరుస, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_వెడల్పు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_ఎత్తు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_MIN_వెడల్పు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_MAX_వెడల్పు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_MIN_ఎత్తు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_MAX_ఎత్తు, &v)) layout_diff = true;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_BORDER_వెడల్పు, &v)) layout_diff = true;

            ఒకవేళ(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                ఒకవేళ(మారింది_part) *మారింది_part = LV_PART_ANY;
                తిరిగి LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSFORM_వెడల్పు, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSFORM_ఎత్తు, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_OUTLINE_వెడల్పు, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_SHADOW_వెడల్పు, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_SHADOW_స్థానభ్రంశం_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_SHADOW_స్థానభ్రంశం_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_SHADOW_విస్తరణ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(lv_style_get_prop_అంతర్గత(style, LV_STYLE_పంక్తి_వెడల్పు, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            లేకపోతే ఒకవేళ(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    ఒకవేళ(మారింది_part) *మారింది_part = diff_part;
    తిరిగి res;
}

శూన్య lv_obj_fade_లోపల(lv_obj_t * obj, uint32_t సమయం, uint32_t delay)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_సమితి_var(&a, obj);
    lv_anim_సమితి_విలువలు(&a, 0, LV_OPA_COVER);
    lv_anim_సమితి_exec_cb(&a, fade_anim_cb);
    lv_anim_సమితి_completed_cb(&a, fade_లోపల_anim_completed);
    lv_anim_సమితి_duration(&a, సమయం);
    lv_anim_సమితి_delay(&a, delay);
    lv_anim_ప్రారంభం(&a);
}

శూన్య lv_obj_fade_బహిర్గతం(lv_obj_t * obj, uint32_t సమయం, uint32_t delay)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_సమితి_var(&a, obj);
    lv_anim_సమితి_విలువలు(&a, lv_obj_get_style_opa_అంతర్గత(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_సమితి_exec_cb(&a, fade_anim_cb);
    lv_anim_సమితి_duration(&a, సమయం);
    lv_anim_సమితి_delay(&a, delay);
    lv_anim_ప్రారంభం(&a);
}

lv_పాఠ్యం_align_t lv_obj_calculate_style_పాఠ్యం_align(స్థిరరాశి lv_obj_t * obj, lv_part_t part, స్థిరరాశి అక్షరం * txt)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_పాఠ్యం_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ఖాళీ, తిరిగి LV_పాఠ్యం_ALIGN_AUTO);
    తిరిగి lv_obj_calculate_style_పాఠ్యం_align_అంతర్గత(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_అంతర్గత(obj, part);
    ఒకవేళ(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_చివర;
        తిరిగి LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    ఒకవేళ(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    ఒకవేళ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    లేకపోతే {
        obj = lv_obj_get_మాతృ(obj);
    }

    అయితే(obj) {
        opa_obj = lv_obj_get_style_opa_అంతర్గత(obj, part);
        ఒకవేళ(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_చివర;
            తిరిగి LV_OPA_TRANSP;
        }
        ఒకవేళ(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_మాతృ(obj);
    }

    ఒకవేళ(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_చివర;
        తిరిగి LV_OPA_TRANSP;
    }

    ఒకవేళ(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_చివర;
        తిరిగి LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_చివర;
    తిరిగి opa_final;
}

శూన్య lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    ఒకవేళ(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    లేకపోతే ఒకవేళ(layer_type != LV_LAYER_TYPE_NONE) {
        ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
            తిరిగి;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

శూన్య lv_obj_update_blur_స్థితి(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    ఒకవేళ(obj->is_deleting) తిరిగి;

    bool has_blur = calculate_has_blur(obj);
    ఒకవేళ(has_blur == (bool)obj->has_blur) తిరిగి;

    obj->has_blur = has_blur;
    ఒకవేళ(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    లేకపోతే {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_అంతర్గత(obj, part);
    ఒకవేళ(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_అంతర్గత(obj, part);
        color = lv_color_over32(color, lv_color_గమ్యం_32(recolor, opa));
    }

    తిరిగి color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి (lv_color32_t) {
        0
    });

    lv_color32_t ఫలితం;

    lv_color_t color = lv_obj_get_style_recolor_అంతర్గత(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_అంతర్గత(obj, part);

    ఫలితం = lv_color_గమ్యం_32(color, opa);

    ఒకవేళ(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    లేకపోతే {
        obj = lv_obj_get_మాతృ(obj);
    }

    అయితే(obj) {
        ఫలితం = lv_obj_style_apply_recolor(obj, part, ఫలితం);
        obj = lv_obj_get_మాతృ(obj);
    }

    తిరిగి ఫలితం;
}

శూన్య lv_obj_సమితి_style_pad_all(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_pad_ఎడమ(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_కుడి(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_శిఖరం(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_bottom(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_pad_hor(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_pad_ఎడమ(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_కుడి(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_pad_ver(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_pad_శిఖరం(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_bottom(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_margin_all(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_margin_ఎడమ(obj, విలువ, selector);
    lv_obj_సమితి_style_margin_కుడి(obj, విలువ, selector);
    lv_obj_సమితి_style_margin_శిఖరం(obj, విలువ, selector);
    lv_obj_సమితి_style_margin_bottom(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_margin_hor(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_margin_ఎడమ(obj, విలువ, selector);
    lv_obj_సమితి_style_margin_కుడి(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_margin_ver(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_margin_శిఖరం(obj, విలువ, selector);
    lv_obj_సమితి_style_margin_bottom(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_pad_gap(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_pad_అడ్డువరుస(obj, విలువ, selector);
    lv_obj_సమితి_style_pad_స్తంభం(obj, విలువ, selector);
}

శూన్య lv_obj_సమితి_style_కొలత(lv_obj_t * obj, int32_t వెడల్పు, int32_t ఎత్తు, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_వెడల్పు(obj, వెడల్పు, selector);
    lv_obj_సమితి_style_ఎత్తు(obj, ఎత్తు, selector);
}

శూన్య lv_obj_సమితి_style_transform_scale(lv_obj_t * obj, int32_t విలువ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి);
    lv_obj_సమితి_style_transform_scale_x(obj, విలువ, selector);
    lv_obj_సమితి_style_transform_scale_y(obj, విలువ, selector);
}

int32_t lv_obj_get_style_space_ఎడమ(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, part);
}

int32_t lv_obj_get_style_space_కుడి(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_space_కుడి_అంతర్గత(obj, part);
}

int32_t lv_obj_get_style_space_శిఖరం(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, part);
}

int32_t lv_obj_get_style_space_bottom(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_space_bottom_అంతర్గత(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_సురక్షిత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_transform_scale_x_సురక్షిత_అంతర్గత(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_సురక్షిత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, తిరిగి 0)
    తిరిగి lv_obj_get_style_transform_scale_y_సురక్షిత_అంతర్గత(obj, part);
}


#ఒకవేళ LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_విలువ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(style != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);

    ఒకవేళ(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        తిరిగి ఖాళీ;
    }

    lv_obj_కూడు_style(obj, style, selector);

    bind_style_t * p = lv_malloc(పరిమాణం(bind_style_t));
    LV_ASSERT_MALLOC(p);
    ఒకవేళ(p == ఖాళీ) {
        LV_LOG_WARN("Out of memory");
        తిరిగి ఖాళీ;
    }

    p->style = style;
    p->selector = selector;
    p->విలువ = ref_విలువ;

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_దత్తాంశం = 1;
    తిరిగి observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(subject != ఖాళీ, తిరిగి ఖాళీ);

    ఒకవేళ(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_సూచిక) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        తిరిగి ఖాళీ;
    }

    bind_style_prop_t * p = lv_malloc(పరిమాణం(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    ఒకవేళ(p == ఖాళీ) {
        LV_LOG_WARN("Out of memory");
        తిరిగి ఖాళీ;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_కూడు_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_దత్తాంశం = 1;
    తిరిగి observable;
}


#అయితే_ముగింపు /*LV_USE_OBSERVER*/


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
స్థిర lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            తిరిగి (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(పరిమాణం(lv_style_t));
    ఒకవేళ(new_style == ఖాళీ) {
        LV_LOG_WARN("couldn't allocate local style");
        తిరిగి ఖాళీ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * పరిమాణం(lv_obj_style_t));
    ఒకవేళ(new_styles == ఖాళీ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        తిరిగి ఖాళీ;
    }
    obj->styles = new_styles;

    కొరకు(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        ఒకవేళ(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ఆపు;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], పరిమాణం(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    తిరిగి (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
స్థిర lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_trans && obj->styles[i].selector == selector) ఆపు;
    }

    /*Already have a transition style for it*/
    ఒకవేళ(i != obj->style_cnt) తిరిగి &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(పరిమాణం(lv_style_t));
    ఒకవేళ(new_style == ఖాళీ) {
        LV_LOG_WARN("couldn't allocate transition style");
        తిరిగి ఖాళీ;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * పరిమాణం(lv_obj_style_t));
    ఒకవేళ(new_styles == ఖాళీ) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        తిరిగి ఖాళీ;
    }
    obj->styles = new_styles;

    కొరకు(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], పరిమాణం(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    తిరిగి &obj->styles[0];
}

స్థిర lv_style_res_t get_prop_core(స్థిరరాశి lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_విలువ_t * v)
{

    స్థిరరాశి uint32_t సమూహం = (uint32_t)1 << lv_style_get_prop_సమూహం(prop);
    స్థిరరాశి lv_part_t part = lv_obj_style_get_selector_part(selector);
    స్థిరరాశి bool skip_trans = (స్థిరరాశి bool) obj->skip_trans;
    int32_t బరువు = -1;
    lv_style_res_t దొరికింది;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ఒకవేళ(obj_style->is_trans == false) ఆపు;
        ఒకవేళ(skip_trans) కొనసాగు;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        ఒకవేళ(part_act != part) కొనసాగు;
        ఒకవేళ((obj_style->style->has_సమూహం & సమూహం) == 0) కొనసాగు;
        దొరికింది = lv_style_get_prop_అంతర్గత(obj_style->style, prop, v);
        ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) {
            తిరిగి LV_STYLE_RES_దొరికింది;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    స్థిరరాశి lv_state_t state = lv_obj_style_get_selector_state(selector);
    స్థిరరాశి lv_state_t state_inv = ~state;
    కొరకు(; i < obj->style_cnt; i++) {
        ఒకవేళ((obj->styles[i].style->has_సమూహం & సమూహం) == 0) కొనసాగు;
        ఒకవేళ(obj->styles[i].is_disabled) కొనసాగు;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ఒకవేళ(part_act != part) కొనసాగు;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ఒకవేళ((state_act & state_inv)) కొనసాగు;

        /*Check only better candidates*/
        ఒకవేళ((int32_t)state_act <= బరువు) కొనసాగు;

        దొరికింది = lv_style_get_prop_అంతర్గత(obj_style->style, prop, v);
        ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) {
            ఒకవేళ(state_act == state) {
                తిరిగి LV_STYLE_RES_దొరికింది;
            }
            బరువు = state_act;
        }
    }

    ఒకవేళ(బరువు >= 0) తిరిగి LV_STYLE_RES_దొరికింది;
    లేకపోతే తిరిగి LV_STYLE_RES_NOT_దొరికింది;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
స్థిర శూన్య report_style_change_core(శూన్య * style, lv_obj_t * obj)
{
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(style == ఖాళీ || obj->styles[i].style == style) {
            నిండిన_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ఆపు;
        }
    }

    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
స్థిర శూన్య refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        lv_obj_invalidate(బిడ్డ);
        lv_obj_send_event(బిడ్డ, LV_EVENT_STYLE_మారింది, ఖాళీ);
        lv_obj_invalidate(బిడ్డ);

        refresh_children_style(బిడ్డ); /*Check children too*/
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
స్థిర bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_హద్దు)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_తోక(style_trans_ll_p);
    అయితే(tr != ఖాళీ) {
        ఒకవేళ(tr == tr_హద్దు) ఆపు;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        ఒకవేళ(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            ఒకవేళ(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            కొరకు(i = 0; i < obj->style_cnt; i++) {
                ఒకవేళ(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ఖాళీ);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    తిరిగి removed;
}

స్థిర శూన్య trans_anim_cb(శూన్య * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) కొనసాగు;

        lv_style_విలువ_t విలువ_final = {0};
        ఎంపిక(tr->prop) {

            స్థితి LV_STYLE_BORDER_SIDE:
            స్థితి LV_STYLE_BORDER_POST:
            స్థితి LV_STYLE_BLEND_బహుళకం:
                ఒకవేళ(v < 255) విలువ_final.అంకె = tr->ప్రారంభం_విలువ.అంకె;
                లేకపోతే విలువ_final.అంకె = tr->చివర_విలువ.అంకె;
                ఆపు;
            స్థితి LV_STYLE_TRANSITION:
            స్థితి LV_STYLE_పాఠ్యం_FONT:
            స్థితి LV_STYLE_BG_IMAGE_SRC:
                ఒకవేళ(v < 255) విలువ_final.ptr = tr->ప్రారంభం_విలువ.ptr;
                లేకపోతే విలువ_final.ptr = tr->చివర_విలువ.ptr;
                ఆపు;
            స్థితి LV_STYLE_COLOR_FILTER_DSC:
                ఒకవేళ(tr->ప్రారంభం_విలువ.ptr == ఖాళీ) విలువ_final.ptr = tr->చివర_విలువ.ptr;
                లేకపోతే ఒకవేళ(tr->చివర_విలువ.ptr == ఖాళీ) విలువ_final.ptr = tr->ప్రారంభం_విలువ.ptr;
                లేకపోతే ఒకవేళ(v < 128) విలువ_final.ptr = tr->ప్రారంభం_విలువ.ptr;
                లేకపోతే విలువ_final.ptr = tr->చివర_విలువ.ptr;
                ఆపు;
            /*Colors*/
            స్థితి LV_STYLE_RECOLOR:
            స్థితి LV_STYLE_BG_COLOR:
            స్థితి LV_STYLE_BG_GRAD_COLOR:
            స్థితి LV_STYLE_BORDER_COLOR:
            స్థితి LV_STYLE_పాఠ్యం_COLOR:
            స్థితి LV_STYLE_SHADOW_COLOR:
            స్థితి LV_STYLE_OUTLINE_COLOR:
            స్థితి LV_STYLE_IMAGE_RECOLOR:
                ఒకవేళ(v <= 0) విలువ_final.color = tr->ప్రారంభం_విలువ.color;
                లేకపోతే ఒకవేళ(v >= 255) విలువ_final.color = tr->చివర_విలువ.color;
                లేకపోతే విలువ_final.color = lv_color_mix(tr->చివర_విలువ.color, tr->ప్రారంభం_విలువ.color, v);
                ఆపు;
            /*Opacities*/
            స్థితి LV_STYLE_OPA:
            స్థితి LV_STYLE_OPA_LAYERED:
            స్థితి LV_STYLE_RECOLOR_OPA:
            స్థితి LV_STYLE_COLOR_FILTER_OPA:
            స్థితి LV_STYLE_BG_OPA:
            స్థితి LV_STYLE_BG_GRAD_OPA:
            స్థితి LV_STYLE_BG_MAIN_OPA:
            స్థితి LV_STYLE_BG_IMAGE_OPA:
            స్థితి LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            స్థితి LV_STYLE_BORDER_OPA:
            స్థితి LV_STYLE_OUTLINE_OPA:
            స్థితి LV_STYLE_SHADOW_OPA:
            స్థితి LV_STYLE_పంక్తి_OPA:
            స్థితి LV_STYLE_ARC_OPA:
            స్థితి LV_STYLE_పాఠ్యం_OPA:
            స్థితి LV_STYLE_IMAGE_OPA:
            స్థితి LV_STYLE_IMAGE_RECOLOR_OPA:
            స్థితి LV_STYLE_DROP_SHADOW_OPA:
            స్థితి LV_STYLE_పాఠ్యం_OUTLINE_STROKE_OPA:
                ఒకవేళ(v <= 0) విలువ_final.అంకె = tr->ప్రారంభం_విలువ.అంకె;
                లేకపోతే ఒకవేళ(v >= 255) విలువ_final.అంకె = tr->చివర_విలువ.అంకె;
                లేకపోతే విలువ_final.అంకె =  LV_CLAMP(0, tr->ప్రారంభం_విలువ.అంకె + ((tr->చివర_విలువ.అంకె - tr->ప్రారంభం_విలువ.అంకె) * v >> 8), 255);
                ఆపు;

            అప్రమేయ:
                ఒకవేళ(v == 0) విలువ_final.అంకె = tr->ప్రారంభం_విలువ.అంకె;
                లేకపోతే ఒకవేళ(v == 255) విలువ_final.అంకె = tr->చివర_విలువ.అంకె;
                లేకపోతే విలువ_final.అంకె = tr->ప్రారంభం_విలువ.అంకె + ((int32_t)((int32_t)(tr->చివర_విలువ.అంకె - tr->ప్రారంభం_విలువ.అంకె) * v) >> 8);
                ఆపు;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_విలువ_t old_విలువ = {0};
        ఒకవేళ(lv_style_get_prop_అంతర్గత(obj->styles[i].style, tr->prop, &old_విలువ) == LV_STYLE_RES_దొరికింది) {
            ఒకవేళ(విలువ_final.ptr == old_విలువ.ptr &&
               lv_color_eq(విలువ_final.color, old_విలువ.color) &&
               విలువ_final.అంకె == old_విలువ.అంకె) {
                ఆపు;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        ఒకవేళ(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_సమితి_prop((lv_style_t *)obj->styles[i].style, tr->prop, విలువ_final);

#ఒకవేళ LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ఒకవేళ(part == LV_PART_MAIN) {
            obj->style_main_prop_is_సమితి |= prop_shifted;
        }
        లేకపోతే {
            obj->style_other_prop_is_సమితి |= prop_shifted;
        }
#అయితే_ముగింపు

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ఆపు;
    }

}

స్థిర శూన్య trans_anim_ప్రారంభం_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->ప్రారంభం_విలువ = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_సమితి_prop((lv_style_t *)style_trans->style, tr->prop, tr->ప్రారంభం_విలువ);
#ఒకవేళ LV_OBJ_STYLE_CACHE
    ఒకవేళ(trans_removed) {
        నిండిన_cache_refresh(tr->obj, part);
    }
    లేకపోతే {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ఒకవేళ(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_సమితి |= prop_shifted;
        }
        లేకపోతే {
            tr->obj->style_other_prop_is_సమితి |= prop_shifted;
        }
    }
#లేకపోతే
    LV_వాడని(trans_removed);
#అయితే_ముగింపు

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

స్థిర శూన్య trans_anim_completed_cb(lv_anim_t * a)
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
        ఒకవేళ(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ఆపు;
        }
    }

    ఒకవేళ(!running) {
        uint32_t i;
        కొరకు(i = 0; i < obj->style_cnt; i++) {
            ఒకవేళ(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                ఒకవేళ(lv_style_is_ఖాళీలు(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ఆపు;
            }
        }
    }
}

స్థిర lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    ఒకవేళ(lv_obj_get_transform(obj) != ఖాళీ) తిరిగి LV_LAYER_TYPE_TRANSFORM;
#అయితే_ముగింపు
    ఒకవేళ(lv_obj_get_style_transform_rotation_అంతర్గత(obj, LV_PART_MAIN) != 0) తిరిగి LV_LAYER_TYPE_TRANSFORM;
    ఒకవేళ(lv_obj_get_style_transform_scale_x_అంతర్గత(obj, LV_PART_MAIN) != 256) తిరిగి LV_LAYER_TYPE_TRANSFORM;
    ఒకవేళ(lv_obj_get_style_transform_scale_y_అంతర్గత(obj, LV_PART_MAIN) != 256) తిరిగి LV_LAYER_TYPE_TRANSFORM;
    ఒకవేళ(lv_obj_get_style_transform_skew_x_అంతర్గత(obj, LV_PART_MAIN) != 0) తిరిగి LV_LAYER_TYPE_TRANSFORM;
    ఒకవేళ(lv_obj_get_style_transform_skew_y_అంతర్గత(obj, LV_PART_MAIN) != 0) తిరిగి LV_LAYER_TYPE_TRANSFORM;
    ఒకవేళ(lv_obj_get_style_opa_layered_అంతర్గత(obj, LV_PART_MAIN) != LV_OPA_COVER) తిరిగి LV_LAYER_TYPE_SIMPLE;
    ఒకవేళ(lv_obj_get_style_bitmap_ముసుగు_src_అంతర్గత(obj, LV_PART_MAIN) != ఖాళీ) తిరిగి LV_LAYER_TYPE_SIMPLE;
    ఒకవేళ(lv_obj_get_style_blend_బహుళకం_అంతర్గత(obj, LV_PART_MAIN) != LV_BLEND_బహుళకం_సాధారణ) తిరిగి LV_LAYER_TYPE_SIMPLE;
    తిరిగి LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
స్థిర bool calculate_has_blur(lv_obj_t * obj)
{
    స్థిరరాశి uint32_t సమూహం_blur = (uint32_t)1 << lv_style_get_prop_సమూహం(LV_STYLE_BLUR_RADIUS);
    స్థిరరాశి uint32_t సమూహం_dropshadow = (uint32_t)1 << lv_style_get_prop_సమూహం(LV_STYLE_DROP_SHADOW_OPA);
    స్థిరరాశి lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    స్థిరరాశి lv_state_t state_inv = ~state;
    lv_style_విలువ_t v;
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ఒకవేళ(obj_style->is_disabled) కొనసాగు;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ఒకవేళ((state_style & state_inv)) కొనసాగు;

        ఒకవేళ((obj_style->style->has_సమూహం & సమూహం_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            ఒకవేళ(v.అంకె > 0) తిరిగి true;
        }
        ఒకవేళ((obj_style->style->has_సమూహం & సమూహం_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            ఒకవేళ(v.అంకె > 0) తిరిగి true;
        }
    }

    తిరిగి false;
}

స్థిర శూన్య నిండిన_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#ఒకవేళ LV_OBJ_STYLE_CACHE
    uint32_t i;
    ఒకవేళ(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_సమితి = 0;
        కొరకు(i = 0; i < obj->style_cnt; i++) {
            ఒకవేళ(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) కొనసాగు;
            ఒకవేళ(obj->styles[i].is_disabled) కొనసాగు;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ఒకవేళ(lv_style_is_const_అంతర్గత(style)) {
                lv_style_const_prop_t * props = style->విలువలు_and_props;
                కొరకు(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_సమితి |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            లేకపోతే {
                lv_style_prop_t * props = (lv_style_prop_t *)style->విలువలు_and_props + style->prop_cnt * పరిమాణం(lv_style_విలువ_t);
                కొరకు(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_సమితి |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    ఒకవేళ(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_సమితి = 0;
        కొరకు(i = 0; i < obj->style_cnt; i++) {
            ఒకవేళ(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) కొనసాగు;
            ఒకవేళ(obj->styles[i].is_disabled) కొనసాగు;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ఒకవేళ(lv_style_is_const_అంతర్గత(style)) {
                lv_style_const_prop_t * props = style->విలువలు_and_props;
                కొరకు(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_సమితి |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            లేకపోతే {
                lv_style_prop_t * props = (lv_style_prop_t *)style->విలువలు_and_props + style->prop_cnt * పరిమాణం(lv_style_విలువ_t);
                కొరకు(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_సమితి |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#లేకపోతే
    LV_వాడని(obj);
    LV_వాడని(part);
#అయితే_ముగింపు
}

స్థిర శూన్య fade_anim_cb(శూన్య * obj, int32_t v)
{
    lv_obj_సమితి_style_opa(obj, v, 0);
}

స్థిర శూన్య fade_లోపల_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

స్థిర bool style_has_flag(స్థిరరాశి lv_style_t * style, uint32_t flag)
{
    ఒకవేళ(lv_style_is_const_అంతర్గత(style)) {
        lv_style_const_prop_t * props = style->విలువలు_and_props;
        uint32_t i;
        కొరకు(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            ఒకవేళ(lv_style_prop_has_flag(props[i].prop, flag)) {
                తిరిగి true;
            }
        }
    }
    లేకపోతే {
        lv_style_prop_t * props = (lv_style_prop_t *)style->విలువలు_and_props + style->prop_cnt * పరిమాణం(lv_style_విలువ_t);
        uint32_t i;
        కొరకు(i = 0; i < style->prop_cnt; i++) {
            ఒకవేళ(lv_style_prop_has_flag(props[i], flag)) {
                తిరిగి true;
            }
        }
    }
    తిరిగి false;
}

స్థిర lv_style_res_t get_selector_style_prop(స్థిరరాశి lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_విలువ_t * విలువ_act)
{
    lv_style_res_t దొరికింది;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#ఒకవేళ LV_OBJ_STYLE_CACHE
    స్థిరరాశి uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    ఒకవేళ((part == LV_PART_MAIN ? obj->style_main_prop_is_సమితి : obj->style_other_prop_is_సమితి) & prop_shifted)
#అయితే_ముగింపు
    {
        దొరికింది = get_prop_core(obj, selector, prop, విలువ_act);
        ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) తిరిగి LV_STYLE_RES_దొరికింది;
    }

    బాహ్య స్థిరరాశి uint8_t lv_style_builtin_prop_flag_lookup_పట్టిక[];
    bool inheritable = false;
    ఒకవేళ(prop < LV_STYLE_అంకె_BUILT_లోపల_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_పట్టిక[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    లేకపోతే {
        ఒకవేళ(style_custom_prop_flag_lookup_పట్టిక_2 != ఖాళీ) {
            inheritable = style_custom_prop_flag_lookup_పట్టిక_2[prop - LV_STYLE_అంకె_BUILT_లోపల_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    ఒకవేళ(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        ఒకవేళ(part != LV_PART_MAIN) part = LV_PART_MAIN;
        లేకపోతే obj = obj->మాతృ;

        అయితే(obj) {
#ఒకవేళ LV_OBJ_STYLE_CACHE
            ఒకవేళ(obj->style_main_prop_is_సమితి & prop_shifted)
#అయితే_ముగింపు
            {
                selector = part | obj->state;
                దొరికింది = get_prop_core(obj, selector, prop, విలువ_act);
                ఒకవేళ(దొరికింది == LV_STYLE_RES_దొరికింది) తిరిగి LV_STYLE_RES_దొరికింది;
            }
            /*Check the parent too.*/
            obj = obj->మాతృ;
        }
    }
    లేకపోతే {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        ఒకవేళ(part == LV_PART_MAIN && (prop == LV_STYLE_వెడల్పు || prop == LV_STYLE_ఎత్తు)) {
            స్థిరరాశి lv_obj_class_t * cls = obj->class_p;
            అయితే(cls) {
                ఒకవేళ(prop == LV_STYLE_వెడల్పు) {
                    ఒకవేళ(cls->వెడల్పు_def != 0)  {
                        విలువ_act->అంకె = cls->వెడల్పు_def;
                        తిరిగి LV_STYLE_RES_దొరికింది;
                    }
                }
                లేకపోతే {
                    ఒకవేళ(cls->ఎత్తు_def != 0) {
                        విలువ_act->అంకె = cls->ఎత్తు_def;
                        తిరిగి LV_STYLE_RES_దొరికింది;
                    }
                }
                cls = cls->పునాది_class;
            }
        }
    }

    తిరిగి LV_STYLE_RES_NOT_దొరికింది;
}

స్థిర శూన్య remove_style_core(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ఖాళీ);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    ఒకవేళ((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ఖాళీ || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_లెక్క = 0;
    bool deleted = false;

    కొరకు(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        ఒకవేళ(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        లేకపోతే ఒకవేళ((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ఖాళీ && style != obj->styles[i].style)) {
            should_remove = false;
        }

        ఒకవేళ(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            ఒకవేళ(style_లెక్క != i) {
                obj->styles[style_లెక్క] = obj->styles[i];
            }
            style_లెక్క++;
            కొనసాగు;
        }

        /*This entry is being removed*/
        ఒకవేళ(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ఖాళీ);
        }
        ఒకవేళ(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ఒకవేళ(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ఖాళీ;
        }
        deleted = true;
    }

    ఒకవేళ(!deleted) {
        తిరిగి;
    }

    obj->style_cnt = style_లెక్క;
    ఒకవేళ(style_లెక్క > 0) {
        obj->styles = lv_realloc(obj->styles, style_లెక్క * పరిమాణం(lv_obj_style_t));
    }
    లేకపోతే {
        lv_free(obj->styles);
        obj->styles = ఖాళీ;
    }

    ఒకవేళ(deleted) {
        నిండిన_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

స్థిర lv_obj_style_t * కనుగొను_style(lv_obj_t * obj, స్థిరరాశి lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    కొరకు(i = 0; i < obj->style_cnt; i++) {
        ఒకవేళ(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            తిరిగి &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (శూన్య *)style, (శూన్య *)obj, selector);
    తిరిగి ఖాళీ;
}



#ఒకవేళ LV_USE_OBSERVER

స్థిర శూన్య bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_దత్తాంశం(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->విలువ);
    lv_obj_సమితి_style_enabled(observer->లక్ష్యం, p->style, p->selector, en);
}

స్థిర శూన్య bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_దత్తాంశం(observer);

    lv_style_విలువ_t style_v;
    ఒకవేళ(subject->type == LV_SUBJECT_TYPE_INT) style_v.అంకె = lv_subject_get_int(subject);
    లేకపోతే ఒకవేళ(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    లేకపోతే ఒకవేళ(subject->type == LV_SUBJECT_TYPE_సూచిక) style_v.ptr = lv_subject_get_సూచిక(subject);
    లేకపోతే {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        తిరిగి;
    }

    lv_obj_సమితి_local_style_prop(observer->లక్ష్యం, p->prop, style_v, p->selector);
}


#అయితే_ముగింపు /*LV_USE_OBSERVER*/
