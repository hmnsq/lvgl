/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "lv_obj_private.h"
#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "../misc/lv_anim_private.h"
#অন্তর্ভাব "../misc/lv_bidi_private.h"
#অন্তর্ভাব "../misc/lv_style_private.h"
#অন্তর্ভাব "lv_obj_style_private.h"
#অন্তর্ভাব "lv_obj_class_private.h"
#অন্তর্ভাব "../display/lv_display_private.h"
#অন্তর্ভাব "../core/lv_global.h"
#অন্তর্ভাব "lv_observer_private.h"
#অন্তর্ভাব "lv_obj_style_internal.h"

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

প্রকারনির্দেশ সংরচনা {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_মান_t start_মান;
    lv_style_মান_t end_মান;
} trans_t;

প্রকারনির্দেশ সংরচনা {
    ধ্রুব lv_style_t * style;
    lv_style_selector_t selector;
    int32_t মান;
} bind_style_t;

প্রকারনির্দেশ সংরচনা {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
স্থির lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
স্থির lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
স্থির lv_style_res_t get_prop_core(ধ্রুব lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_মান_t * v);
স্থির শূন্য report_style_change_core(শূন্য * style, lv_obj_t * obj);
স্থির শূন্য refresh_children_style(lv_obj_t * obj);
স্থির bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
স্থির শূন্য trans_anim_cb(শূন্য * _tr, int32_t v);
স্থির শূন্য trans_anim_start_cb(lv_anim_t * a);
স্থির শূন্য trans_anim_completed_cb(lv_anim_t * a);
স্থির lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
স্থির bool calculate_has_blur(lv_obj_t * obj);
স্থির শূন্য full_cache_refresh(lv_obj_t * obj, lv_part_t part);
স্থির শূন্য fade_anim_cb(শূন্য * obj, int32_t v);
স্থির শূন্য fade_in_anim_completed(lv_anim_t * a);
স্থির bool style_has_flag(ধ্রুব lv_style_t * style, uint32_t flag);
স্থির lv_style_res_t get_selector_style_prop(ধ্রুব lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_মান_t * মান_act);
স্থির শূন্য remove_style_core(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector, bool theme_only);
স্থির lv_obj_style_t * find_style(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector);
#যদি LV_USE_OBSERVER
    স্থির শূন্য bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    স্থির শূন্য bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
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

শূন্য lv_obj_style_init(শূন্য)
{
    lv_ll_init(style_trans_ll_p, পরিমাণ(trans_t));
}

শূন্য lv_obj_style_deinit(শূন্য)
{
    lv_ll_clear(style_trans_ll_p);
    যদি(_style_custom_prop_flag_lookup_table != রিক্ত) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = রিক্ত;
    }
}

শূন্য lv_obj_add_style(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(style != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG_বার্তা(obj->style_cnt < 63, প্রত্যাগম,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    যদি(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, রিক্ত);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_trans) অনুবর্ত;
        যদি(obj->styles[i].is_local) অনুবর্ত;
        ভঙ্গ;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * পরিমাণ(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    প্রতি(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], পরিমাণ(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    যদি(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#যদি LV_OBJ_STYLE_CACHE
    অন্যথা {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        যদি(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->মানসমূহ_and_props;
            প্রতি(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        অন্যথা {
            lv_style_prop_t * props = (lv_style_prop_t *)style->মানসমূহ_and_props + style->prop_cnt * পরিমাণ(lv_style_মান_t);
            প্রতি(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ধ্রুব lv_style_t * old_style, ধ্রুব lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম false);
    LV_CHECK_ARG(old_style != রিক্ত, প্রত্যাগম false);
    LV_CHECK_ARG(new_style != রিক্ত, প্রত্যাগম false);
    LV_CHECK_ARG(old_style != new_style, প্রত্যাগম false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    যদি((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, রিক্ত);

    bool replaced = false;
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        যদি(obj->styles[i].is_local || obj->styles[i].is_trans) {
            অনুবর্ত;
        }

        /*Skip non-matching styles*/
        যদি((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            অনুবর্ত;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], পরিমাণ(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    যদি(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    প্রত্যাগম replaced;
}

শূন্য lv_obj_remove_style(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    remove_style_core(obj, style, selector, false);
}

শূন্য lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    remove_style_core(obj, রিক্ত, selector, true);
}

শূন্য lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    remove_style_core(obj, রিক্ত, LV_PART_ANY | LV_STATE_ANY, false);
}

শূন্য lv_obj_report_style_change(lv_style_t * style)
{
    যদি(!style_refr) প্রত্যাগম;
    lv_display_t * d = lv_display_get_next(রিক্ত);

    যাবত্(d) {
        uint32_t i;
        প্রতি(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

শূন্য lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম);

    যদি(!style_refr) প্রত্যাগম;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    যদি(is_layout_refr) {
        যদি(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, রিক্ত);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    যদি((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        যদি(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    যদি((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    যদি(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    যদি(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    যদি(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        যদি(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

শূন্য lv_obj_set_style_enabled(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(style != রিক্ত, প্রত্যাগম);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    যদি(obj_style == রিক্ত) প্রত্যাগম;

    যদি(en != obj_style->is_disabled) প্রত্যাগম; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    যদি((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম false);
    LV_CHECK_ARG(style != রিক্ত, প্রত্যাগম false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    যদি(obj_style == রিক্ত) প্রত্যাগম false;

    প্রত্যাগম !obj_style->is_disabled;
}


শূন্য lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_মান_t lv_obj_get_style_prop(ধ্রুব lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম lv_style_prop_get_default(prop));
    প্রত্যাগম lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_মান_t lv_obj_get_style_prop_internal(ধ্রুব lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != রিক্ত);
    lv_style_selector_t selector = part | obj->state;
    lv_style_মান_t মান_act = { .ptr = রিক্ত };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &মান_act);
    যদি(found == LV_STYLE_RES_FOUND) প্রত্যাগম মান_act;

    প্রত্যাগম lv_style_prop_get_default(prop);
}
lv_style_মান_t lv_obj_style_apply_color_filter_internal(ধ্রুব lv_obj_t * obj, lv_part_t part, lv_style_মান_t v)
{
#যদি LV_USE_COLOR_FILTER
    LV_ASSERT(obj != রিক্ত);

    ধ্রুব lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    যদি(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        যদি(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#অন্যথা
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    প্রত্যাগম v;
}

bool lv_obj_has_style_prop(ধ্রুব lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম false);

    lv_style_মান_t মান_act = { .ptr = রিক্ত };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &মান_act);
    যদি(found == LV_STYLE_RES_FOUND) প্রত্যাগম true;

    প্রত্যাগম false;
}

শূন্য lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_মান_t মান,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    যদি(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, রিক্ত);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, মান);

    যদি(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#যদি LV_OBJ_STYLE_CACHE
    অন্যথা {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        যদি(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        অন্যথা {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_মান_t * মান,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(মান != রিক্ত, প্রত্যাগম LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            প্রত্যাগম lv_style_get_prop_internal(obj->styles[i].style, prop, মান);
        }
    }

    প্রত্যাগম LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, প্রত্যাগম 0);

    uint32_t i;
    /*Find the style*/
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            ভঙ্গ;
        }
    }

    /*The style is not found*/
    যদি(i == obj->style_cnt) প্রত্যাগম false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    যদি(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    প্রত্যাগম res;
}

শূন্য lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ধ্রুব lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(tr_dsc != রিক্ত, প্রত্যাগম);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_মান_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_মান_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    যদি(v1.ptr == v2.ptr && v1.অঙ্ক == v2.অঙ্ক && lv_color_eq(v1.color, v2.color))  প্রত্যাগম;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    যদি(tr_dsc->prop == LV_STYLE_RADIUS) {
        যদি(v1.অঙ্ক == LV_RADIUS_CIRCLE || v2.অঙ্ক == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            যদি(v1.অঙ্ক == LV_RADIUS_CIRCLE) v1.অঙ্ক = LV_MIN(whalf + 1, hhalf + 1);
            যদি(v2.অঙ্ক == LV_RADIUS_CIRCLE) v2.অঙ্ক = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    যদি(tr == রিক্ত) প্রত্যাগম;
    tr->start_মান = v1;
    tr->end_মান = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_মানসমূহ(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->time);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_মান_t lv_obj_style_apply_color_filter(ধ্রুব lv_obj_t * obj, lv_part_t part, lv_style_মান_t v)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম v);
    প্রত্যাগম lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম LV_STYLE_STATE_CMP_SAME);

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
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_trans) অনুবর্ত;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        যদি(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            যদি(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            অন্যথা যদি(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ধ্রুব lv_style_t * style = obj->styles[i].style;
            lv_style_মান_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_শীর্ষ, &v))layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_বাম, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_ডান, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            যদি(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                যদি(changed_part) *changed_part = LV_PART_ANY;
                প্রত্যাগম LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(lv_style_get_prop_internal(style, LV_STYLE_সারি_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            অন্যথা যদি(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    যদি(changed_part) *changed_part = diff_part;
    প্রত্যাগম res;
}

শূন্য lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_মানসমূহ(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

শূন্য lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_মানসমূহ(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(ধ্রুব lv_obj_t * obj, lv_part_t part, ধ্রুব বর্ণ * txt)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != রিক্ত, প্রত্যাগম LV_TEXT_ALIGN_AUTO);
    প্রত্যাগম lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    যদি(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        প্রত্যাগম LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    যদি(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    যদি(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    অন্যথা {
        obj = lv_obj_get_parent(obj);
    }

    যাবত্(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        যদি(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            প্রত্যাগম LV_OPA_TRANSP;
        }
        যদি(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    যদি(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        প্রত্যাগম LV_OPA_TRANSP;
    }

    যদি(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        প্রত্যাগম LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    প্রত্যাগম opa_final;
}

শূন্য lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    যদি(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    অন্যথা যদি(layer_type != LV_LAYER_TYPE_NONE) {
        যদি(!lv_obj_allocate_spec_attr(obj)) {
            প্রত্যাগম;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

শূন্য lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    যদি(obj->is_deleting) প্রত্যাগম;

    bool has_blur = calculate_has_blur(obj);
    যদি(has_blur == (bool)obj->has_blur) প্রত্যাগম;

    obj->has_blur = has_blur;
    যদি(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    অন্যথা {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ধ্রুব lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    যদি(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    প্রত্যাগম color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    যদি(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    অন্যথা {
        obj = lv_obj_get_parent(obj);
    }

    যাবত্(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    প্রত্যাগম result;
}

শূন্য lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_pad_বাম(obj, মান, selector);
    lv_obj_set_style_pad_ডান(obj, মান, selector);
    lv_obj_set_style_pad_শীর্ষ(obj, মান, selector);
    lv_obj_set_style_pad_bottom(obj, মান, selector);
}

শূন্য lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_pad_বাম(obj, মান, selector);
    lv_obj_set_style_pad_ডান(obj, মান, selector);
}

শূন্য lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_pad_শীর্ষ(obj, মান, selector);
    lv_obj_set_style_pad_bottom(obj, মান, selector);
}

শূন্য lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_margin_বাম(obj, মান, selector);
    lv_obj_set_style_margin_ডান(obj, মান, selector);
    lv_obj_set_style_margin_শীর্ষ(obj, মান, selector);
    lv_obj_set_style_margin_bottom(obj, মান, selector);
}

শূন্য lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_margin_বাম(obj, মান, selector);
    lv_obj_set_style_margin_ডান(obj, মান, selector);
}

শূন্য lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_margin_শীর্ষ(obj, মান, selector);
    lv_obj_set_style_margin_bottom(obj, মান, selector);
}

শূন্য lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_pad_row(obj, মান, selector);
    lv_obj_set_style_pad_column(obj, মান, selector);
}

শূন্য lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

শূন্য lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t মান, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম);
    lv_obj_set_style_transform_scale_x(obj, মান, selector);
    lv_obj_set_style_transform_scale_y(obj, মান, selector);
}

int32_t lv_obj_get_style_space_বাম(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_space_বাম_internal(obj, part);
}

int32_t lv_obj_get_style_space_ডান(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_space_ডান_internal(obj, part);
}

int32_t lv_obj_get_style_space_শীর্ষ(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_space_শীর্ষ_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, প্রত্যাগম 0)
    প্রত্যাগম lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#যদি LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_মান)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(style != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);

    যদি(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        প্রত্যাগম রিক্ত;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(পরিমাণ(bind_style_t));
    LV_ASSERT_MALLOC(p);
    যদি(p == রিক্ত) {
        LV_LOG_WARN("Out of memory");
        প্রত্যাগম রিক্ত;
    }

    p->style = style;
    p->selector = selector;
    p->মান = ref_মান;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    প্রত্যাগম observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(subject != রিক্ত, প্রত্যাগম রিক্ত);

    যদি(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        প্রত্যাগম রিক্ত;
    }

    bind_style_prop_t * p = lv_malloc(পরিমাণ(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    যদি(p == রিক্ত) {
        LV_LOG_WARN("Out of memory");
        প্রত্যাগম রিক্ত;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    প্রত্যাগম observable;
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
স্থির lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            প্রত্যাগম (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(পরিমাণ(lv_style_t));
    যদি(new_style == রিক্ত) {
        LV_LOG_WARN("couldn't allocate local style");
        প্রত্যাগম রিক্ত;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * পরিমাণ(lv_obj_style_t));
    যদি(new_styles == রিক্ত) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        প্রত্যাগম রিক্ত;
    }
    obj->styles = new_styles;

    প্রতি(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        যদি(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) ভঙ্গ;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], পরিমাণ(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    প্রত্যাগম (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
স্থির lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_trans && obj->styles[i].selector == selector) ভঙ্গ;
    }

    /*Already have a transition style for it*/
    যদি(i != obj->style_cnt) প্রত্যাগম &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(পরিমাণ(lv_style_t));
    যদি(new_style == রিক্ত) {
        LV_LOG_WARN("couldn't allocate transition style");
        প্রত্যাগম রিক্ত;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * পরিমাণ(lv_obj_style_t));
    যদি(new_styles == রিক্ত) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        প্রত্যাগম রিক্ত;
    }
    obj->styles = new_styles;

    প্রতি(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], পরিমাণ(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    প্রত্যাগম &obj->styles[0];
}

স্থির lv_style_res_t get_prop_core(ধ্রুব lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_মান_t * v)
{

    ধ্রুব uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    ধ্রুব lv_part_t part = lv_obj_style_get_selector_part(selector);
    ধ্রুব bool skip_trans = (ধ্রুব bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        যদি(obj_style->is_trans == false) ভঙ্গ;
        যদি(skip_trans) অনুবর্ত;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        যদি(part_act != part) অনুবর্ত;
        যদি((obj_style->style->has_group & group) == 0) অনুবর্ত;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        যদি(found == LV_STYLE_RES_FOUND) {
            প্রত্যাগম LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ধ্রুব lv_state_t state = lv_obj_style_get_selector_state(selector);
    ধ্রুব lv_state_t state_inv = ~state;
    প্রতি(; i < obj->style_cnt; i++) {
        যদি((obj->styles[i].style->has_group & group) == 0) অনুবর্ত;
        যদি(obj->styles[i].is_disabled) অনুবর্ত;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        যদি(part_act != part) অনুবর্ত;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        যদি((state_act & state_inv)) অনুবর্ত;

        /*Check only better candidates*/
        যদি((int32_t)state_act <= weight) অনুবর্ত;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        যদি(found == LV_STYLE_RES_FOUND) {
            যদি(state_act == state) {
                প্রত্যাগম LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    যদি(weight >= 0) প্রত্যাগম LV_STYLE_RES_FOUND;
    অন্যথা প্রত্যাগম LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
স্থির শূন্য report_style_change_core(শূন্য * style, lv_obj_t * obj)
{
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(style == রিক্ত || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            ভঙ্গ;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_গুনতি(obj);
    প্রতি(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
স্থির শূন্য refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(obj);
    প্রতি(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, রিক্ত);
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
স্থির bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    যাবত্(tr != রিক্ত) {
        যদি(tr == tr_limit) ভঙ্গ;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        যদি(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            যদি(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            প্রতি(i = 0; i < obj->style_cnt; i++) {
                যদি(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, রিক্ত);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    প্রত্যাগম removed;
}

স্থির শূন্য trans_anim_cb(শূন্য * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) অনুবর্ত;

        lv_style_মান_t মান_final = {0};
        বিকল্প(tr->prop) {

            পক্ষ LV_STYLE_BORDER_SIDE:
            পক্ষ LV_STYLE_BORDER_POST:
            পক্ষ LV_STYLE_BLEND_MODE:
                যদি(v < 255) মান_final.অঙ্ক = tr->start_মান.অঙ্ক;
                অন্যথা মান_final.অঙ্ক = tr->end_মান.অঙ্ক;
                ভঙ্গ;
            পক্ষ LV_STYLE_TRANSITION:
            পক্ষ LV_STYLE_TEXT_FONT:
            পক্ষ LV_STYLE_BG_IMAGE_SRC:
                যদি(v < 255) মান_final.ptr = tr->start_মান.ptr;
                অন্যথা মান_final.ptr = tr->end_মান.ptr;
                ভঙ্গ;
            পক্ষ LV_STYLE_COLOR_FILTER_DSC:
                যদি(tr->start_মান.ptr == রিক্ত) মান_final.ptr = tr->end_মান.ptr;
                অন্যথা যদি(tr->end_মান.ptr == রিক্ত) মান_final.ptr = tr->start_মান.ptr;
                অন্যথা যদি(v < 128) মান_final.ptr = tr->start_মান.ptr;
                অন্যথা মান_final.ptr = tr->end_মান.ptr;
                ভঙ্গ;
            /*Colors*/
            পক্ষ LV_STYLE_RECOLOR:
            পক্ষ LV_STYLE_BG_COLOR:
            পক্ষ LV_STYLE_BG_GRAD_COLOR:
            পক্ষ LV_STYLE_BORDER_COLOR:
            পক্ষ LV_STYLE_TEXT_COLOR:
            পক্ষ LV_STYLE_SHADOW_COLOR:
            পক্ষ LV_STYLE_OUTLINE_COLOR:
            পক্ষ LV_STYLE_IMAGE_RECOLOR:
                যদি(v <= 0) মান_final.color = tr->start_মান.color;
                অন্যথা যদি(v >= 255) মান_final.color = tr->end_মান.color;
                অন্যথা মান_final.color = lv_color_mix(tr->end_মান.color, tr->start_মান.color, v);
                ভঙ্গ;
            /*Opacities*/
            পক্ষ LV_STYLE_OPA:
            পক্ষ LV_STYLE_OPA_LAYERED:
            পক্ষ LV_STYLE_RECOLOR_OPA:
            পক্ষ LV_STYLE_COLOR_FILTER_OPA:
            পক্ষ LV_STYLE_BG_OPA:
            পক্ষ LV_STYLE_BG_GRAD_OPA:
            পক্ষ LV_STYLE_BG_MAIN_OPA:
            পক্ষ LV_STYLE_BG_IMAGE_OPA:
            পক্ষ LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            পক্ষ LV_STYLE_BORDER_OPA:
            পক্ষ LV_STYLE_OUTLINE_OPA:
            পক্ষ LV_STYLE_SHADOW_OPA:
            পক্ষ LV_STYLE_সারি_OPA:
            পক্ষ LV_STYLE_ARC_OPA:
            পক্ষ LV_STYLE_TEXT_OPA:
            পক্ষ LV_STYLE_IMAGE_OPA:
            পক্ষ LV_STYLE_IMAGE_RECOLOR_OPA:
            পক্ষ LV_STYLE_DROP_SHADOW_OPA:
            পক্ষ LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                যদি(v <= 0) মান_final.অঙ্ক = tr->start_মান.অঙ্ক;
                অন্যথা যদি(v >= 255) মান_final.অঙ্ক = tr->end_মান.অঙ্ক;
                অন্যথা মান_final.অঙ্ক =  LV_CLAMP(0, tr->start_মান.অঙ্ক + ((tr->end_মান.অঙ্ক - tr->start_মান.অঙ্ক) * v >> 8), 255);
                ভঙ্গ;

            স্বভাব:
                যদি(v == 0) মান_final.অঙ্ক = tr->start_মান.অঙ্ক;
                অন্যথা যদি(v == 255) মান_final.অঙ্ক = tr->end_মান.অঙ্ক;
                অন্যথা মান_final.অঙ্ক = tr->start_মান.অঙ্ক + ((int32_t)((int32_t)(tr->end_মান.অঙ্ক - tr->start_মান.অঙ্ক) * v) >> 8);
                ভঙ্গ;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_মান_t old_মান = {0};
        যদি(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_মান) == LV_STYLE_RES_FOUND) {
            যদি(মান_final.ptr == old_মান.ptr &&
               lv_color_eq(মান_final.color, old_মান.color) &&
               মান_final.অঙ্ক == old_মান.অঙ্ক) {
                ভঙ্গ;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        যদি(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, মান_final);

#যদি LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        যদি(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        অন্যথা {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        ভঙ্গ;
    }

}

স্থির শূন্য trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_মান = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_মান);
#যদি LV_OBJ_STYLE_CACHE
    যদি(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    অন্যথা {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        যদি(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        অন্যথা {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#অন্যথা
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

স্থির শূন্য trans_anim_completed_cb(lv_anim_t * a)
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
        যদি(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            ভঙ্গ;
        }
    }

    যদি(!running) {
        uint32_t i;
        প্রতি(i = 0; i < obj->style_cnt; i++) {
            যদি(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                যদি(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                ভঙ্গ;
            }
        }
    }
}

স্থির lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#যদি LV_DRAW_TRANSFORM_USE_MATRIX
    যদি(lv_obj_get_transform(obj) != রিক্ত) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
#endif
    যদি(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
    যদি(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
    যদি(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
    যদি(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
    যদি(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) প্রত্যাগম LV_LAYER_TYPE_TRANSFORM;
    যদি(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) প্রত্যাগম LV_LAYER_TYPE_SIMPLE;
    যদি(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != রিক্ত) প্রত্যাগম LV_LAYER_TYPE_SIMPLE;
    যদি(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) প্রত্যাগম LV_LAYER_TYPE_SIMPLE;
    প্রত্যাগম LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
স্থির bool calculate_has_blur(lv_obj_t * obj)
{
    ধ্রুব uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    ধ্রুব uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    ধ্রুব lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ধ্রুব lv_state_t state_inv = ~state;
    lv_style_মান_t v;
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        যদি(obj_style->is_disabled) অনুবর্ত;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        যদি((state_style & state_inv)) অনুবর্ত;

        যদি((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            যদি(v.অঙ্ক > 0) প্রত্যাগম true;
        }
        যদি((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            যদি(v.অঙ্ক > 0) প্রত্যাগম true;
        }
    }

    প্রত্যাগম false;
}

স্থির শূন্য full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#যদি LV_OBJ_STYLE_CACHE
    uint32_t i;
    যদি(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        প্রতি(i = 0; i < obj->style_cnt; i++) {
            যদি(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) অনুবর্ত;
            যদি(obj->styles[i].is_disabled) অনুবর্ত;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            যদি(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->মানসমূহ_and_props;
                প্রতি(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            অন্যথা {
                lv_style_prop_t * props = (lv_style_prop_t *)style->মানসমূহ_and_props + style->prop_cnt * পরিমাণ(lv_style_মান_t);
                প্রতি(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    যদি(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        প্রতি(i = 0; i < obj->style_cnt; i++) {
            যদি(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) অনুবর্ত;
            যদি(obj->styles[i].is_disabled) অনুবর্ত;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            যদি(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->মানসমূহ_and_props;
                প্রতি(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            অন্যথা {
                lv_style_prop_t * props = (lv_style_prop_t *)style->মানসমূহ_and_props + style->prop_cnt * পরিমাণ(lv_style_মান_t);
                প্রতি(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#অন্যথা
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

স্থির শূন্য fade_anim_cb(শূন্য * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

স্থির শূন্য fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

স্থির bool style_has_flag(ধ্রুব lv_style_t * style, uint32_t flag)
{
    যদি(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->মানসমূহ_and_props;
        uint32_t i;
        প্রতি(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            যদি(lv_style_prop_has_flag(props[i].prop, flag)) {
                প্রত্যাগম true;
            }
        }
    }
    অন্যথা {
        lv_style_prop_t * props = (lv_style_prop_t *)style->মানসমূহ_and_props + style->prop_cnt * পরিমাণ(lv_style_মান_t);
        uint32_t i;
        প্রতি(i = 0; i < style->prop_cnt; i++) {
            যদি(lv_style_prop_has_flag(props[i], flag)) {
                প্রত্যাগম true;
            }
        }
    }
    প্রত্যাগম false;
}

স্থির lv_style_res_t get_selector_style_prop(ধ্রুব lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_মান_t * মান_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#যদি LV_OBJ_STYLE_CACHE
    ধ্রুব uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    যদি((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, মান_act);
        যদি(found == LV_STYLE_RES_FOUND) প্রত্যাগম LV_STYLE_RES_FOUND;
    }

    বাহ্য ধ্রুব uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    যদি(prop < LV_STYLE_অঙ্ক_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    অন্যথা {
        যদি(_style_custom_prop_flag_lookup_table != রিক্ত) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_অঙ্ক_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    যদি(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        যদি(part != LV_PART_MAIN) part = LV_PART_MAIN;
        অন্যথা obj = obj->parent;

        যাবত্(obj) {
#যদি LV_OBJ_STYLE_CACHE
            যদি(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, মান_act);
                যদি(found == LV_STYLE_RES_FOUND) প্রত্যাগম LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    অন্যথা {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        যদি(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            ধ্রুব lv_obj_class_t * cls = obj->class_p;
            যাবত্(cls) {
                যদি(prop == LV_STYLE_WIDTH) {
                    যদি(cls->width_def != 0)  {
                        মান_act->অঙ্ক = cls->width_def;
                        প্রত্যাগম LV_STYLE_RES_FOUND;
                    }
                }
                অন্যথা {
                    যদি(cls->height_def != 0) {
                        মান_act->অঙ্ক = cls->height_def;
                        প্রত্যাগম LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    প্রত্যাগম LV_STYLE_RES_NOT_FOUND;
}

স্থির শূন্য remove_style_core(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != রিক্ত);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    যদি((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == রিক্ত || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_গুনতি = 0;
    bool deleted = false;

    প্রতি(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        যদি(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        অন্যথা যদি((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != রিক্ত && style != obj->styles[i].style)) {
            should_remove = false;
        }

        যদি(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            যদি(style_গুনতি != i) {
                obj->styles[style_গুনতি] = obj->styles[i];
            }
            style_গুনতি++;
            অনুবর্ত;
        }

        /*This entry is being removed*/
        যদি(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, রিক্ত);
        }
        যদি(obj->styles[i].is_local || obj->styles[i].is_trans) {
            যদি(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = রিক্ত;
        }
        deleted = true;
    }

    যদি(!deleted) {
        প্রত্যাগম;
    }

    obj->style_cnt = style_গুনতি;
    যদি(style_গুনতি > 0) {
        obj->styles = lv_realloc(obj->styles, style_গুনতি * পরিমাণ(lv_obj_style_t));
    }
    অন্যথা {
        lv_free(obj->styles);
        obj->styles = রিক্ত;
    }

    যদি(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

স্থির lv_obj_style_t * find_style(lv_obj_t * obj, ধ্রুব lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    প্রতি(i = 0; i < obj->style_cnt; i++) {
        যদি(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            প্রত্যাগম &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (শূন্য *)style, (শূন্য *)obj, selector);
    প্রত্যাগম রিক্ত;
}



#যদি LV_USE_OBSERVER

স্থির শূন্য bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->মান);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

স্থির শূন্য bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_মান_t style_v;
    যদি(subject->type == LV_SUBJECT_TYPE_INT) style_v.অঙ্ক = lv_subject_get_int(subject);
    অন্যথা যদি(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    অন্যথা যদি(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    অন্যথা {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        প্রত্যাগম;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
