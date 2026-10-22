/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_anim_private.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_bidi_private.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_style_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_style_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_class_private.h"
#ଅନ୍ତର୍ଭାଵ "../display/lv_display_private.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_global.h"
#ଅନ୍ତର୍ଭାଵ "lv_observer_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_style_internal.h"

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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_ମୂଲ୍ୟ_t start_ମୂଲ୍ୟ;
    lv_style_ମୂଲ୍ୟ_t end_ମୂଲ୍ୟ;
} trans_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅଚଳ lv_style_t * style;
    lv_style_selector_t selector;
    int32_t ମୂଲ୍ୟ;
} bind_style_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ସ୍ଥିର lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
ସ୍ଥିର lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
ସ୍ଥିର lv_style_res_t get_prop_core(ଅଚଳ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ମୂଲ୍ୟ_t * v);
ସ୍ଥିର ଶୂନ୍ୟ report_style_change_core(ଶୂନ୍ୟ * style, lv_obj_t * obj);
ସ୍ଥିର ଶୂନ୍ୟ refresh_children_style(lv_obj_t * obj);
ସ୍ଥିର bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
ସ୍ଥିର ଶୂନ୍ୟ trans_anim_cb(ଶୂନ୍ୟ * _tr, int32_t v);
ସ୍ଥିର ଶୂନ୍ୟ trans_anim_start_cb(lv_anim_t * a);
ସ୍ଥିର ଶୂନ୍ୟ trans_anim_completed_cb(lv_anim_t * a);
ସ୍ଥିର lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
ସ୍ଥିର bool calculate_has_blur(lv_obj_t * obj);
ସ୍ଥିର ଶୂନ୍ୟ full_cache_refresh(lv_obj_t * obj, lv_part_t part);
ସ୍ଥିର ଶୂନ୍ୟ fade_anim_cb(ଶୂନ୍ୟ * obj, int32_t v);
ସ୍ଥିର ଶୂନ୍ୟ fade_in_anim_completed(lv_anim_t * a);
ସ୍ଥିର bool style_has_flag(ଅଚଳ lv_style_t * style, uint32_t flag);
ସ୍ଥିର lv_style_res_t get_selector_style_prop(ଅଚଳ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ମୂଲ୍ୟ_t * ମୂଲ୍ୟ_act);
ସ୍ଥିର ଶୂନ୍ୟ remove_style_core(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector, bool theme_only);
ସ୍ଥିର lv_obj_style_t * find_style(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector);
#ଯଦି LV_USE_OBSERVER
    ସ୍ଥିର ଶୂନ୍ୟ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    ସ୍ଥିର ଶୂନ୍ୟ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
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

ଶୂନ୍ୟ lv_obj_style_init(ଶୂନ୍ୟ)
{
    lv_ll_init(style_trans_ll_p, ଆକାର(trans_t));
}

ଶୂନ୍ୟ lv_obj_style_deinit(ଶୂନ୍ୟ)
{
    lv_ll_clear(style_trans_ll_p);
    ଯଦି(_style_custom_prop_flag_lookup_table != ଖାଲି) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = ଖାଲି;
    }
}

ଶୂନ୍ୟ lv_obj_add_style(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(style != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG_ସନ୍ଦେଶ(obj->style_cnt < 63, ଫେରନ୍ତୁ,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ଯଦି(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ଖାଲି);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_trans) ଜାରି;
        ଯଦି(obj->styles[i].is_local) ଜାରି;
        ଭାଙ୍ଗ;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * ଆକାର(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    ପାଇଁ(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], ଆକାର(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    ଯଦି(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#ଯଦି LV_OBJ_STYLE_CACHE
    ଅନ୍ୟଥା {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        ଯଦି(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props;
            ପାଇଁ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        ଅନ୍ୟଥା {
            lv_style_prop_t * props = (lv_style_prop_t *)style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props + style->prop_cnt * ଆକାର(lv_style_ମୂଲ୍ୟ_t);
            ପାଇଁ(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ଅଚଳ lv_style_t * old_style, ଅଚଳ lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ false);
    LV_CHECK_ARG(old_style != ଖାଲି, ଫେରନ୍ତୁ false);
    LV_CHECK_ARG(new_style != ଖାଲି, ଫେରନ୍ତୁ false);
    LV_CHECK_ARG(old_style != new_style, ଫେରନ୍ତୁ false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ଯଦି((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ଖାଲି);

    bool replaced = false;
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        ଯଦି(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ଜାରି;
        }

        /*Skip non-matching styles*/
        ଯଦି((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            ଜାରି;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], ଆକାର(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    ଯଦି(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    ଫେରନ୍ତୁ replaced;
}

ଶୂନ୍ୟ lv_obj_remove_style(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    remove_style_core(obj, style, selector, false);
}

ଶୂନ୍ୟ lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    remove_style_core(obj, ଖାଲି, selector, true);
}

ଶୂନ୍ୟ lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    remove_style_core(obj, ଖାଲି, LV_PART_ANY | LV_STATE_ANY, false);
}

ଶୂନ୍ୟ lv_obj_report_style_change(lv_style_t * style)
{
    ଯଦି(!style_refr) ଫେରନ୍ତୁ;
    lv_display_t * d = lv_display_get_next(ଖାଲି);

    ଯେତେବେଳେ(d) {
        uint32_t i;
        ପାଇଁ(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

ଶୂନ୍ୟ lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);

    ଯଦି(!style_refr) ଫେରନ୍ତୁ;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    ଯଦି(is_layout_refr) {
        ଯଦି(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, ଖାଲି);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    ଯଦି((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        ଯଦି(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    ଯଦି((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    ଯଦି(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    ଯଦି(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    ଯଦି(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        ଯଦି(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

ଶୂନ୍ୟ lv_obj_set_style_enabled(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(style != ଖାଲି, ଫେରନ୍ତୁ);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    ଯଦି(obj_style == ଖାଲି) ଫେରନ୍ତୁ;

    ଯଦି(en != obj_style->is_disabled) ଫେରନ୍ତୁ; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    ଯଦି((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ false);
    LV_CHECK_ARG(style != ଖାଲି, ଫେରନ୍ତୁ false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    ଯଦି(obj_style == ଖାଲି) ଫେରନ୍ତୁ false;

    ଫେରନ୍ତୁ !obj_style->is_disabled;
}


ଶୂନ୍ୟ lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_ମୂଲ୍ୟ_t lv_obj_get_style_prop(ଅଚଳ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ lv_style_prop_get_default(prop));
    ଫେରନ୍ତୁ lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_ମୂଲ୍ୟ_t lv_obj_get_style_prop_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ଖାଲି);
    lv_style_selector_t selector = part | obj->state;
    lv_style_ମୂଲ୍ୟ_t ମୂଲ୍ୟ_act = { .ptr = ଖାଲି };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &ମୂଲ୍ୟ_act);
    ଯଦି(found == LV_STYLE_RES_FOUND) ଫେରନ୍ତୁ ମୂଲ୍ୟ_act;

    ଫେରନ୍ତୁ lv_style_prop_get_default(prop);
}
lv_style_ମୂଲ୍ୟ_t lv_obj_style_apply_color_filter_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part, lv_style_ମୂଲ୍ୟ_t v)
{
#ଯଦି LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ଖାଲି);

    ଅଚଳ lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    ଯଦି(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        ଯଦି(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#ଅନ୍ୟଥା
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    ଫେରନ୍ତୁ v;
}

bool lv_obj_has_style_prop(ଅଚଳ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ false);

    lv_style_ମୂଲ୍ୟ_t ମୂଲ୍ୟ_act = { .ptr = ଖାଲି };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &ମୂଲ୍ୟ_act);
    ଯଦି(found == LV_STYLE_RES_FOUND) ଫେରନ୍ତୁ true;

    ଫେରନ୍ତୁ false;
}

ଶୂନ୍ୟ lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ମୂଲ୍ୟ_t ମୂଲ୍ୟ,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    ଯଦି(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ଖାଲି);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, ମୂଲ୍ୟ);

    ଯଦି(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#ଯଦି LV_OBJ_STYLE_CACHE
    ଅନ୍ୟଥା {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        ଯଦି(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        ଅନ୍ୟଥା {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_ମୂଲ୍ୟ_t * ମୂଲ୍ୟ,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(ମୂଲ୍ୟ != ଖାଲି, ଫେରନ୍ତୁ LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ଫେରନ୍ତୁ lv_style_get_prop_internal(obj->styles[i].style, prop, ମୂଲ୍ୟ);
        }
    }

    ଫେରନ୍ତୁ LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ 0);

    uint32_t i;
    /*Find the style*/
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ଭାଙ୍ଗ;
        }
    }

    /*The style is not found*/
    ଯଦି(i == obj->style_cnt) ଫେରନ୍ତୁ false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    ଯଦି(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ଅଚଳ lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(tr_dsc != ଖାଲି, ଫେରନ୍ତୁ);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_ମୂଲ୍ୟ_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_ମୂଲ୍ୟ_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    ଯଦି(v1.ptr == v2.ptr && v1.ଅଙ୍କ == v2.ଅଙ୍କ && lv_color_eq(v1.color, v2.color))  ଫେରନ୍ତୁ;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    ଯଦି(tr_dsc->prop == LV_STYLE_RADIUS) {
        ଯଦି(v1.ଅଙ୍କ == LV_RADIUS_CIRCLE || v2.ଅଙ୍କ == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            ଯଦି(v1.ଅଙ୍କ == LV_RADIUS_CIRCLE) v1.ଅଙ୍କ = LV_MIN(whalf + 1, hhalf + 1);
            ଯଦି(v2.ଅଙ୍କ == LV_RADIUS_CIRCLE) v2.ଅଙ୍କ = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    ଯଦି(tr == ଖାଲି) ଫେରନ୍ତୁ;
    tr->start_ମୂଲ୍ୟ = v1;
    tr->end_ମୂଲ୍ୟ = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_ମୂଲ୍ୟଗୁଡ଼ିକ(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->time);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_ମୂଲ୍ୟ_t lv_obj_style_apply_color_filter(ଅଚଳ lv_obj_t * obj, lv_part_t part, lv_style_ମୂଲ୍ୟ_t v)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ v);
    ଫେରନ୍ତୁ lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_STYLE_STATE_CMP_SAME);

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
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_trans) ଜାରି;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        ଯଦି(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            ଯଦି(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            ଅନ୍ୟଥା ଯଦି(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ଅଚଳ lv_style_t * style = obj->styles[i].style;
            lv_style_ମୂଲ୍ୟ_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_ଶୀର୍ଷ, &v))layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_ବାମ, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_ଡାହାଣ, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            ଯଦି(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                ଯଦି(changed_part) *changed_part = LV_PART_ANY;
                ଫେରନ୍ତୁ LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(lv_style_get_prop_internal(style, LV_STYLE_ଧାଡ଼ି_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            ଅନ୍ୟଥା ଯଦି(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    ଯଦି(changed_part) *changed_part = diff_part;
    ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_ମୂଲ୍ୟଗୁଡ଼ିକ(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

ଶୂନ୍ୟ lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_ମୂଲ୍ୟଗୁଡ଼ିକ(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(ଅଚଳ lv_obj_t * obj, lv_part_t part, ଅଚଳ ଅକ୍ଷର * txt)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ଖାଲି, ଫେରନ୍ତୁ LV_TEXT_ALIGN_AUTO);
    ଫେରନ୍ତୁ lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    ଯଦି(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        ଫେରନ୍ତୁ LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    ଯଦି(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    ଯଦି(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ଅନ୍ୟଥା {
        obj = lv_obj_get_parent(obj);
    }

    ଯେତେବେଳେ(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        ଯଦି(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            ଫେରନ୍ତୁ LV_OPA_TRANSP;
        }
        ଯଦି(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    ଯଦି(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        ଫେରନ୍ତୁ LV_OPA_TRANSP;
    }

    ଯଦି(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        ଫେରନ୍ତୁ LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    ଫେରନ୍ତୁ opa_final;
}

ଶୂନ୍ୟ lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    ଯଦି(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    ଅନ୍ୟଥା ଯଦି(layer_type != LV_LAYER_TYPE_NONE) {
        ଯଦି(!lv_obj_allocate_spec_attr(obj)) {
            ଫେରନ୍ତୁ;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

ଶୂନ୍ୟ lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    ଯଦି(obj->is_deleting) ଫେରନ୍ତୁ;

    bool has_blur = calculate_has_blur(obj);
    ଯଦି(has_blur == (bool)obj->has_blur) ଫେରନ୍ତୁ;

    obj->has_blur = has_blur;
    ଯଦି(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    ଅନ୍ୟଥା {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ଅଚଳ lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    ଯଦି(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    ଫେରନ୍ତୁ color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    ଯଦି(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    ଅନ୍ୟଥା {
        obj = lv_obj_get_parent(obj);
    }

    ଯେତେବେଳେ(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    ଫେରନ୍ତୁ result;
}

ଶୂନ୍ୟ lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_pad_ବାମ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_ଡାହାଣ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_ଶୀର୍ଷ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_bottom(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_pad_ବାମ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_ଡାହାଣ(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_pad_ଶୀର୍ଷ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_bottom(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_margin_ବାମ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_margin_ଡାହାଣ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_margin_ଶୀର୍ଷ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_margin_bottom(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_margin_ବାମ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_margin_ଡାହାଣ(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_margin_ଶୀର୍ଷ(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_margin_bottom(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_pad_row(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_pad_column(obj, ମୂଲ୍ୟ, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

ଶୂନ୍ୟ lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t ମୂଲ୍ୟ, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ);
    lv_obj_set_style_transform_scale_x(obj, ମୂଲ୍ୟ, selector);
    lv_obj_set_style_transform_scale_y(obj, ମୂଲ୍ୟ, selector);
}

int32_t lv_obj_get_style_space_ବାମ(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_space_ବାମ_internal(obj, part);
}

int32_t lv_obj_get_style_space_ଡାହାଣ(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_space_ଡାହାଣ_internal(obj, part);
}

int32_t lv_obj_get_style_space_ଶୀର୍ଷ(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_space_ଶୀର୍ଷ_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, ଫେରନ୍ତୁ 0)
    ଫେରନ୍ତୁ lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#ଯଦି LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(style != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(ଆକାର(bind_style_t));
    LV_ASSERT_MALLOC(p);
    ଯଦି(p == ଖାଲି) {
        LV_LOG_WARN("Out of memory");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    p->style = style;
    p->selector = selector;
    p->ମୂଲ୍ୟ = ref_ମୂଲ୍ୟ;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    ଫେରନ୍ତୁ observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(subject != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    bind_style_prop_t * p = lv_malloc(ଆକାର(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    ଯଦି(p == ଖାଲି) {
        LV_LOG_WARN("Out of memory");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    ଫେରନ୍ତୁ observable;
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
ସ୍ଥିର lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ଫେରନ୍ତୁ (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(ଆକାର(lv_style_t));
    ଯଦି(new_style == ଖାଲି) {
        LV_LOG_WARN("couldn't allocate local style");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ଆକାର(lv_obj_style_t));
    ଯଦି(new_styles == ଖାଲି) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ଫେରନ୍ତୁ ଖାଲି;
    }
    obj->styles = new_styles;

    ପାଇଁ(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        ଯଦି(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ଭାଙ୍ଗ;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], ଆକାର(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    ଫେରନ୍ତୁ (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
ସ୍ଥିର lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_trans && obj->styles[i].selector == selector) ଭାଙ୍ଗ;
    }

    /*Already have a transition style for it*/
    ଯଦି(i != obj->style_cnt) ଫେରନ୍ତୁ &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(ଆକାର(lv_style_t));
    ଯଦି(new_style == ଖାଲି) {
        LV_LOG_WARN("couldn't allocate transition style");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * ଆକାର(lv_obj_style_t));
    ଯଦି(new_styles == ଖାଲି) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        ଫେରନ୍ତୁ ଖାଲି;
    }
    obj->styles = new_styles;

    ପାଇଁ(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], ଆକାର(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    ଫେରନ୍ତୁ &obj->styles[0];
}

ସ୍ଥିର lv_style_res_t get_prop_core(ଅଚଳ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_ମୂଲ୍ୟ_t * v)
{

    ଅଚଳ uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    ଅଚଳ lv_part_t part = lv_obj_style_get_selector_part(selector);
    ଅଚଳ bool skip_trans = (ଅଚଳ bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ଯଦି(obj_style->is_trans == false) ଭାଙ୍ଗ;
        ଯଦି(skip_trans) ଜାରି;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        ଯଦି(part_act != part) ଜାରି;
        ଯଦି((obj_style->style->has_group & group) == 0) ଜାରି;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        ଯଦି(found == LV_STYLE_RES_FOUND) {
            ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ଅଚଳ lv_state_t state = lv_obj_style_get_selector_state(selector);
    ଅଚଳ lv_state_t state_inv = ~state;
    ପାଇଁ(; i < obj->style_cnt; i++) {
        ଯଦି((obj->styles[i].style->has_group & group) == 0) ଜାରି;
        ଯଦି(obj->styles[i].is_disabled) ଜାରି;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        ଯଦି(part_act != part) ଜାରି;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ଯଦି((state_act & state_inv)) ଜାରି;

        /*Check only better candidates*/
        ଯଦି((int32_t)state_act <= weight) ଜାରି;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        ଯଦି(found == LV_STYLE_RES_FOUND) {
            ଯଦି(state_act == state) {
                ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    ଯଦି(weight >= 0) ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
    ଅନ୍ୟଥା ଫେରନ୍ତୁ LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
ସ୍ଥିର ଶୂନ୍ୟ report_style_change_core(ଶୂନ୍ୟ * style, lv_obj_t * obj)
{
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(style == ଖାଲି || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ଭାଙ୍ଗ;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_ଗଣନା(obj);
    ପାଇଁ(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
ସ୍ଥିର ଶୂନ୍ୟ refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ଗଣନା(obj);
    ପାଇଁ(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, ଖାଲି);
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
ସ୍ଥିର bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    ଯେତେବେଳେ(tr != ଖାଲି) {
        ଯଦି(tr == tr_limit) ଭାଙ୍ଗ;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        ଯଦି(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            ଯଦି(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
                ଯଦି(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ଖାଲି);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    ଫେରନ୍ତୁ removed;
}

ସ୍ଥିର ଶୂନ୍ୟ trans_anim_cb(ଶୂନ୍ୟ * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) ଜାରି;

        lv_style_ମୂଲ୍ୟ_t ମୂଲ୍ୟ_final = {0};
        ବାଛ(tr->prop) {

            ସ୍ଥିତି LV_STYLE_BORDER_SIDE:
            ସ୍ଥିତି LV_STYLE_BORDER_POST:
            ସ୍ଥିତି LV_STYLE_BLEND_MODE:
                ଯଦି(v < 255) ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->start_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->end_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି LV_STYLE_TRANSITION:
            ସ୍ଥିତି LV_STYLE_TEXT_FONT:
            ସ୍ଥିତି LV_STYLE_BG_IMAGE_SRC:
                ଯଦି(v < 255) ମୂଲ୍ୟ_final.ptr = tr->start_ମୂଲ୍ୟ.ptr;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.ptr = tr->end_ମୂଲ୍ୟ.ptr;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି LV_STYLE_COLOR_FILTER_DSC:
                ଯଦି(tr->start_ମୂଲ୍ୟ.ptr == ଖାଲି) ମୂଲ୍ୟ_final.ptr = tr->end_ମୂଲ୍ୟ.ptr;
                ଅନ୍ୟଥା ଯଦି(tr->end_ମୂଲ୍ୟ.ptr == ଖାଲି) ମୂଲ୍ୟ_final.ptr = tr->start_ମୂଲ୍ୟ.ptr;
                ଅନ୍ୟଥା ଯଦି(v < 128) ମୂଲ୍ୟ_final.ptr = tr->start_ମୂଲ୍ୟ.ptr;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.ptr = tr->end_ମୂଲ୍ୟ.ptr;
                ଭାଙ୍ଗ;
            /*Colors*/
            ସ୍ଥିତି LV_STYLE_RECOLOR:
            ସ୍ଥିତି LV_STYLE_BG_COLOR:
            ସ୍ଥିତି LV_STYLE_BG_GRAD_COLOR:
            ସ୍ଥିତି LV_STYLE_BORDER_COLOR:
            ସ୍ଥିତି LV_STYLE_TEXT_COLOR:
            ସ୍ଥିତି LV_STYLE_SHADOW_COLOR:
            ସ୍ଥିତି LV_STYLE_OUTLINE_COLOR:
            ସ୍ଥିତି LV_STYLE_IMAGE_RECOLOR:
                ଯଦି(v <= 0) ମୂଲ୍ୟ_final.color = tr->start_ମୂଲ୍ୟ.color;
                ଅନ୍ୟଥା ଯଦି(v >= 255) ମୂଲ୍ୟ_final.color = tr->end_ମୂଲ୍ୟ.color;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.color = lv_color_mix(tr->end_ମୂଲ୍ୟ.color, tr->start_ମୂଲ୍ୟ.color, v);
                ଭାଙ୍ଗ;
            /*Opacities*/
            ସ୍ଥିତି LV_STYLE_OPA:
            ସ୍ଥିତି LV_STYLE_OPA_LAYERED:
            ସ୍ଥିତି LV_STYLE_RECOLOR_OPA:
            ସ୍ଥିତି LV_STYLE_COLOR_FILTER_OPA:
            ସ୍ଥିତି LV_STYLE_BG_OPA:
            ସ୍ଥିତି LV_STYLE_BG_GRAD_OPA:
            ସ୍ଥିତି LV_STYLE_BG_MAIN_OPA:
            ସ୍ଥିତି LV_STYLE_BG_IMAGE_OPA:
            ସ୍ଥିତି LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            ସ୍ଥିତି LV_STYLE_BORDER_OPA:
            ସ୍ଥିତି LV_STYLE_OUTLINE_OPA:
            ସ୍ଥିତି LV_STYLE_SHADOW_OPA:
            ସ୍ଥିତି LV_STYLE_ଧାଡ଼ି_OPA:
            ସ୍ଥିତି LV_STYLE_ARC_OPA:
            ସ୍ଥିତି LV_STYLE_TEXT_OPA:
            ସ୍ଥିତି LV_STYLE_IMAGE_OPA:
            ସ୍ଥିତି LV_STYLE_IMAGE_RECOLOR_OPA:
            ସ୍ଥିତି LV_STYLE_DROP_SHADOW_OPA:
            ସ୍ଥିତି LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                ଯଦି(v <= 0) ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->start_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଅନ୍ୟଥା ଯଦି(v >= 255) ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->end_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.ଅଙ୍କ =  LV_CLAMP(0, tr->start_ମୂଲ୍ୟ.ଅଙ୍କ + ((tr->end_ମୂଲ୍ୟ.ଅଙ୍କ - tr->start_ମୂଲ୍ୟ.ଅଙ୍କ) * v >> 8), 255);
                ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                ଯଦି(v == 0) ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->start_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଅନ୍ୟଥା ଯଦି(v == 255) ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->end_ମୂଲ୍ୟ.ଅଙ୍କ;
                ଅନ୍ୟଥା ମୂଲ୍ୟ_final.ଅଙ୍କ = tr->start_ମୂଲ୍ୟ.ଅଙ୍କ + ((int32_t)((int32_t)(tr->end_ମୂଲ୍ୟ.ଅଙ୍କ - tr->start_ମୂଲ୍ୟ.ଅଙ୍କ) * v) >> 8);
                ଭାଙ୍ଗ;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_ମୂଲ୍ୟ_t old_ମୂଲ୍ୟ = {0};
        ଯଦି(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_ମୂଲ୍ୟ) == LV_STYLE_RES_FOUND) {
            ଯଦି(ମୂଲ୍ୟ_final.ptr == old_ମୂଲ୍ୟ.ptr &&
               lv_color_eq(ମୂଲ୍ୟ_final.color, old_ମୂଲ୍ୟ.color) &&
               ମୂଲ୍ୟ_final.ଅଙ୍କ == old_ମୂଲ୍ୟ.ଅଙ୍କ) {
                ଭାଙ୍ଗ;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        ଯଦି(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, ମୂଲ୍ୟ_final);

#ଯଦି LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ଯଦି(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        ଅନ୍ୟଥା {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ଭାଙ୍ଗ;
    }

}

ସ୍ଥିର ଶୂନ୍ୟ trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_ମୂଲ୍ୟ = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_ମୂଲ୍ୟ);
#ଯଦି LV_OBJ_STYLE_CACHE
    ଯଦି(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    ଅନ୍ୟଥା {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        ଯଦି(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        ଅନ୍ୟଥା {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#ଅନ୍ୟଥା
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

ସ୍ଥିର ଶୂନ୍ୟ trans_anim_completed_cb(lv_anim_t * a)
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
        ଯଦି(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି(!running) {
        uint32_t i;
        ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
            ଯଦି(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                ଯଦି(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ଭାଙ୍ଗ;
            }
        }
    }
}

ସ୍ଥିର lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#ଯଦି LV_DRAW_TRANSFORM_USE_MATRIX
    ଯଦି(lv_obj_get_transform(obj) != ଖାଲି) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
#endif
    ଯଦି(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
    ଯଦି(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
    ଯଦି(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
    ଯଦି(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
    ଯଦି(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) ଫେରନ୍ତୁ LV_LAYER_TYPE_TRANSFORM;
    ଯଦି(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) ଫେରନ୍ତୁ LV_LAYER_TYPE_SIMPLE;
    ଯଦି(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != ଖାଲି) ଫେରନ୍ତୁ LV_LAYER_TYPE_SIMPLE;
    ଯଦି(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) ଫେରନ୍ତୁ LV_LAYER_TYPE_SIMPLE;
    ଫେରନ୍ତୁ LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
ସ୍ଥିର bool calculate_has_blur(lv_obj_t * obj)
{
    ଅଚଳ uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    ଅଚଳ uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    ଅଚଳ lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ଅଚଳ lv_state_t state_inv = ~state;
    lv_style_ମୂଲ୍ୟ_t v;
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        ଯଦି(obj_style->is_disabled) ଜାରି;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        ଯଦି((state_style & state_inv)) ଜାରି;

        ଯଦି((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            ଯଦି(v.ଅଙ୍କ > 0) ଫେରନ୍ତୁ true;
        }
        ଯଦି((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            ଯଦି(v.ଅଙ୍କ > 0) ଫେରନ୍ତୁ true;
        }
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#ଯଦି LV_OBJ_STYLE_CACHE
    uint32_t i;
    ଯଦି(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
            ଯଦି(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) ଜାରି;
            ଯଦି(obj->styles[i].is_disabled) ଜାରି;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ଯଦି(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props;
                ପାଇଁ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ଅନ୍ୟଥା {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props + style->prop_cnt * ଆକାର(lv_style_ମୂଲ୍ୟ_t);
                ପାଇଁ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    ଯଦି(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
            ଯଦି(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) ଜାରି;
            ଯଦି(obj->styles[i].is_disabled) ଜାରି;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            ଯଦି(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props;
                ପାଇଁ(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            ଅନ୍ୟଥା {
                lv_style_prop_t * props = (lv_style_prop_t *)style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props + style->prop_cnt * ଆକାର(lv_style_ମୂଲ୍ୟ_t);
                ପାଇଁ(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#ଅନ୍ୟଥା
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ fade_anim_cb(ଶୂନ୍ୟ * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

ସ୍ଥିର bool style_has_flag(ଅଚଳ lv_style_t * style, uint32_t flag)
{
    ଯଦି(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props;
        uint32_t i;
        ପାଇଁ(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            ଯଦି(lv_style_prop_has_flag(props[i].prop, flag)) {
                ଫେରନ୍ତୁ true;
            }
        }
    }
    ଅନ୍ୟଥା {
        lv_style_prop_t * props = (lv_style_prop_t *)style->ମୂଲ୍ୟଗୁଡ଼ିକ_and_props + style->prop_cnt * ଆକାର(lv_style_ମୂଲ୍ୟ_t);
        uint32_t i;
        ପାଇଁ(i = 0; i < style->prop_cnt; i++) {
            ଯଦି(lv_style_prop_has_flag(props[i], flag)) {
                ଫେରନ୍ତୁ true;
            }
        }
    }
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର lv_style_res_t get_selector_style_prop(ଅଚଳ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_ମୂଲ୍ୟ_t * ମୂଲ୍ୟ_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#ଯଦି LV_OBJ_STYLE_CACHE
    ଅଚଳ uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    ଯଦି((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, ମୂଲ୍ୟ_act);
        ଯଦି(found == LV_STYLE_RES_FOUND) ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
    }

    ବାହ୍ୟ ଅଚଳ uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    ଯଦି(prop < LV_STYLE_ଅଙ୍କ_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    ଅନ୍ୟଥା {
        ଯଦି(_style_custom_prop_flag_lookup_table != ଖାଲି) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_ଅଙ୍କ_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    ଯଦି(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        ଯଦି(part != LV_PART_MAIN) part = LV_PART_MAIN;
        ଅନ୍ୟଥା obj = obj->parent;

        ଯେତେବେଳେ(obj) {
#ଯଦି LV_OBJ_STYLE_CACHE
            ଯଦି(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, ମୂଲ୍ୟ_act);
                ଯଦି(found == LV_STYLE_RES_FOUND) ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    ଅନ୍ୟଥା {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        ଯଦି(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            ଅଚଳ lv_obj_class_t * cls = obj->class_p;
            ଯେତେବେଳେ(cls) {
                ଯଦି(prop == LV_STYLE_WIDTH) {
                    ଯଦି(cls->width_def != 0)  {
                        ମୂଲ୍ୟ_act->ଅଙ୍କ = cls->width_def;
                        ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
                    }
                }
                ଅନ୍ୟଥା {
                    ଯଦି(cls->height_def != 0) {
                        ମୂଲ୍ୟ_act->ଅଙ୍କ = cls->height_def;
                        ଫେରନ୍ତୁ LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    ଫେରନ୍ତୁ LV_STYLE_RES_NOT_FOUND;
}

ସ୍ଥିର ଶୂନ୍ୟ remove_style_core(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ଖାଲି);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    ଯଦି((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ଖାଲି || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ଗଣନା = 0;
    bool deleted = false;

    ପାଇଁ(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        ଯଦି(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        ଅନ୍ୟଥା ଯଦି((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ଖାଲି && style != obj->styles[i].style)) {
            should_remove = false;
        }

        ଯଦି(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            ଯଦି(style_ଗଣନା != i) {
                obj->styles[style_ଗଣନା] = obj->styles[i];
            }
            style_ଗଣନା++;
            ଜାରି;
        }

        /*This entry is being removed*/
        ଯଦି(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ଖାଲି);
        }
        ଯଦି(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ଯଦି(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ଖାଲି;
        }
        deleted = true;
    }

    ଯଦି(!deleted) {
        ଫେରନ୍ତୁ;
    }

    obj->style_cnt = style_ଗଣନା;
    ଯଦି(style_ଗଣନା > 0) {
        obj->styles = lv_realloc(obj->styles, style_ଗଣନା * ଆକାର(lv_obj_style_t));
    }
    ଅନ୍ୟଥା {
        lv_free(obj->styles);
        obj->styles = ଖାଲି;
    }

    ଯଦି(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

ସ୍ଥିର lv_obj_style_t * find_style(lv_obj_t * obj, ଅଚଳ lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    ପାଇଁ(i = 0; i < obj->style_cnt; i++) {
        ଯଦି(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            ଫେରନ୍ତୁ &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (ଶୂନ୍ୟ *)style, (ଶୂନ୍ୟ *)obj, selector);
    ଫେରନ୍ତୁ ଖାଲି;
}



#ଯଦି LV_USE_OBSERVER

ସ୍ଥିର ଶୂନ୍ୟ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->ମୂଲ୍ୟ);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

ସ୍ଥିର ଶୂନ୍ୟ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_ମୂଲ୍ୟ_t style_v;
    ଯଦି(subject->type == LV_SUBJECT_TYPE_INT) style_v.ଅଙ୍କ = lv_subject_get_int(subject);
    ଅନ୍ୟଥା ଯଦି(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    ଅନ୍ୟଥା ଯଦି(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    ଅନ୍ୟଥା {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        ଫେରନ୍ତୁ;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
