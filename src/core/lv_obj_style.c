/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../misc/lv_anim_private.h"
#अन्तर्भाव "../misc/lv_bidi_private.h"
#अन्तर्भाव "../misc/lv_style_private.h"
#अन्तर्भाव "lv_obj_style_private.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "../display/lv_display_private.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "lv_observer_private.h"
#अन्तर्भाव "lv_obj_style_internal.h"

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

प्रकारनिर्देश संरचना {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_मूल्य_t start_मूल्य;
    lv_style_मूल्य_t end_मूल्य;
} trans_t;

प्रकारनिर्देश संरचना {
    ध्रुव lv_style_t * style;
    lv_style_selector_t selector;
    int32_t मूल्य;
} bind_style_t;

प्रकारनिर्देश संरचना {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
स्थिर lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
स्थिर lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
स्थिर lv_style_res_t get_prop_core(ध्रुव lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_मूल्य_t * v);
स्थिर शून्य report_style_change_core(शून्य * style, lv_obj_t * obj);
स्थिर शून्य refresh_children_style(lv_obj_t * obj);
स्थिर bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
स्थिर शून्य trans_anim_cb(शून्य * _tr, int32_t v);
स्थिर शून्य trans_anim_start_cb(lv_anim_t * a);
स्थिर शून्य trans_anim_completed_cb(lv_anim_t * a);
स्थिर lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
स्थिर bool calculate_has_blur(lv_obj_t * obj);
स्थिर शून्य full_cache_refresh(lv_obj_t * obj, lv_part_t part);
स्थिर शून्य fade_anim_cb(शून्य * obj, int32_t v);
स्थिर शून्य fade_in_anim_completed(lv_anim_t * a);
स्थिर bool style_has_flag(ध्रुव lv_style_t * style, uint32_t flag);
स्थिर lv_style_res_t get_selector_style_prop(ध्रुव lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act);
स्थिर शून्य remove_style_core(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector, bool theme_only);
स्थिर lv_obj_style_t * find_style(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector);
#यदि LV_USE_OBSERVER
    स्थिर शून्य bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    स्थिर शून्य bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
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

शून्य lv_obj_style_init(शून्य)
{
    lv_ll_init(style_trans_ll_p, परिमाण(trans_t));
}

शून्य lv_obj_style_deinit(शून्य)
{
    lv_ll_clear(style_trans_ll_p);
    यदि(_style_custom_prop_flag_lookup_table != रिक्त) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = रिक्त;
    }
}

शून्य lv_obj_add_style(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(style != रिक्त, प्रत्यागम);
    LV_CHECK_ARG_संदेश(obj->style_cnt < 63, प्रत्यागम,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    यदि(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_trans) अनुवर्त;
        यदि(obj->styles[i].is_local) अनुवर्त;
        भङ्ग;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * परिमाण(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    प्रति(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], परिमाण(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    यदि(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#यदि LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        यदि(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->मूल्यावली_and_props;
            प्रति(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        अन्यथा {
            lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * परिमाण(lv_style_मूल्य_t);
            प्रति(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ध्रुव lv_style_t * old_style, ध्रुव lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);
    LV_CHECK_ARG(old_style != रिक्त, प्रत्यागम false);
    LV_CHECK_ARG(new_style != रिक्त, प्रत्यागम false);
    LV_CHECK_ARG(old_style != new_style, प्रत्यागम false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    यदि((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);

    bool replaced = false;
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        यदि(obj->styles[i].is_local || obj->styles[i].is_trans) {
            अनुवर्त;
        }

        /*Skip non-matching styles*/
        यदि((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            अनुवर्त;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], परिमाण(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    यदि(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    प्रत्यागम replaced;
}

शून्य lv_obj_remove_style(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    remove_style_core(obj, style, selector, false);
}

शून्य lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    remove_style_core(obj, रिक्त, selector, true);
}

शून्य lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    remove_style_core(obj, रिक्त, LV_PART_ANY | LV_STATE_ANY, false);
}

शून्य lv_obj_report_style_change(lv_style_t * style)
{
    यदि(!style_refr) प्रत्यागम;
    lv_display_t * d = lv_display_get_next(रिक्त);

    यावत्(d) {
        uint32_t i;
        प्रति(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

शून्य lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    यदि(!style_refr) प्रत्यागम;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    यदि(is_layout_refr) {
        यदि(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, रिक्त);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    यदि((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        यदि(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    यदि((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    यदि(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    यदि(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    यदि(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        यदि(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

शून्य lv_obj_set_style_enabled(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(style != रिक्त, प्रत्यागम);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    यदि(obj_style == रिक्त) प्रत्यागम;

    यदि(en != obj_style->is_disabled) प्रत्यागम; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    यदि((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);
    LV_CHECK_ARG(style != रिक्त, प्रत्यागम false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    यदि(obj_style == रिक्त) प्रत्यागम false;

    प्रत्यागम !obj_style->is_disabled;
}


शून्य lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_मूल्य_t lv_obj_get_style_prop(ध्रुव lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम lv_style_prop_get_default(prop));
    प्रत्यागम lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_मूल्य_t lv_obj_get_style_prop_internal(ध्रुव lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != रिक्त);
    lv_style_selector_t selector = part | obj->state;
    lv_style_मूल्य_t मूल्य_act = { .ptr = रिक्त };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    यदि(found == LV_STYLE_RES_FOUND) प्रत्यागम मूल्य_act;

    प्रत्यागम lv_style_prop_get_default(prop);
}
lv_style_मूल्य_t lv_obj_style_apply_color_filter_internal(ध्रुव lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
#यदि LV_USE_COLOR_FILTER
    LV_ASSERT(obj != रिक्त);

    ध्रुव lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    यदि(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        यदि(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    प्रत्यागम v;
}

bool lv_obj_has_style_prop(ध्रुव lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);

    lv_style_मूल्य_t मूल्य_act = { .ptr = रिक्त };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    यदि(found == LV_STYLE_RES_FOUND) प्रत्यागम true;

    प्रत्यागम false;
}

शून्य lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_मूल्य_t मूल्य,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    यदि(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, रिक्त);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, मूल्य);

    यदि(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#यदि LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        यदि(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_मूल्य_t * मूल्य,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(मूल्य != रिक्त, प्रत्यागम LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            प्रत्यागम lv_style_get_prop_internal(obj->styles[i].style, prop, मूल्य);
        }
    }

    प्रत्यागम LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    uint32_t i;
    /*Find the style*/
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            भङ्ग;
        }
    }

    /*The style is not found*/
    यदि(i == obj->style_cnt) प्रत्यागम false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    यदि(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    प्रत्यागम res;
}

शून्य lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ध्रुव lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(tr_dsc != रिक्त, प्रत्यागम);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_मूल्य_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_मूल्य_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    यदि(v1.ptr == v2.ptr && v1.अंक == v2.अंक && lv_color_eq(v1.color, v2.color))  प्रत्यागम;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    यदि(tr_dsc->prop == LV_STYLE_RADIUS) {
        यदि(v1.अंक == LV_RADIUS_CIRCLE || v2.अंक == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            यदि(v1.अंक == LV_RADIUS_CIRCLE) v1.अंक = LV_MIN(whalf + 1, hhalf + 1);
            यदि(v2.अंक == LV_RADIUS_CIRCLE) v2.अंक = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    यदि(tr == रिक्त) प्रत्यागम;
    tr->start_मूल्य = v1;
    tr->end_मूल्य = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_मूल्यावली(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->समय);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_मूल्य_t lv_obj_style_apply_color_filter(ध्रुव lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम v);
    प्रत्यागम lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_STYLE_STATE_CMP_SAME);

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
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_trans) अनुवर्त;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        यदि(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            यदि(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            अन्यथा यदि(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ध्रुव lv_style_t * style = obj->styles[i].style;
            lv_style_मूल्य_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_शीर्ष, &v))layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_बायाँ, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_दायाँ, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            यदि(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                यदि(changed_part) *changed_part = LV_PART_ANY;
                प्रत्यागम LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(lv_style_get_prop_internal(style, LV_STYLE_पंक्ति_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा यदि(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    यदि(changed_part) *changed_part = diff_part;
    प्रत्यागम res;
}

शून्य lv_obj_fade_in(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_मूल्यावली(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, समय);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

शून्य lv_obj_fade_out(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_मूल्यावली(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, समय);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(ध्रुव lv_obj_t * obj, lv_part_t part, ध्रुव वर्ण * txt)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != रिक्त, प्रत्यागम LV_TEXT_ALIGN_AUTO);
    प्रत्यागम lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    यदि(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        प्रत्यागम LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    यदि(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    यदि(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    यावत्(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        यदि(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            प्रत्यागम LV_OPA_TRANSP;
        }
        यदि(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    यदि(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        प्रत्यागम LV_OPA_TRANSP;
    }

    यदि(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        प्रत्यागम LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    प्रत्यागम opa_final;
}

शून्य lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    यदि(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    अन्यथा यदि(layer_type != LV_LAYER_TYPE_NONE) {
        यदि(!lv_obj_allocate_spec_attr(obj)) {
            प्रत्यागम;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

शून्य lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    यदि(obj->is_deleting) प्रत्यागम;

    bool has_blur = calculate_has_blur(obj);
    यदि(has_blur == (bool)obj->has_blur) प्रत्यागम;

    obj->has_blur = has_blur;
    यदि(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    अन्यथा {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ध्रुव lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    यदि(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    प्रत्यागम color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    यदि(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    यावत्(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    प्रत्यागम result;
}

शून्य lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_pad_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_दायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_pad_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_दायाँ(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_margin_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_दायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_margin_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_दायाँ(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_pad_row(obj, मूल्य, selector);
    lv_obj_set_style_pad_column(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

शून्य lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम);
    lv_obj_set_style_transform_scale_x(obj, मूल्य, selector);
    lv_obj_set_style_transform_scale_y(obj, मूल्य, selector);
}

int32_t lv_obj_get_style_space_बायाँ(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_space_बायाँ_internal(obj, part);
}

int32_t lv_obj_get_style_space_दायाँ(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_space_दायाँ_internal(obj, part);
}

int32_t lv_obj_get_style_space_शीर्ष(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_space_शीर्ष_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, प्रत्यागम 0)
    प्रत्यागम lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#यदि LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(style != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);

    यदि(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        प्रत्यागम रिक्त;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(परिमाण(bind_style_t));
    LV_ASSERT_MALLOC(p);
    यदि(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        प्रत्यागम रिक्त;
    }

    p->style = style;
    p->selector = selector;
    p->मूल्य = ref_मूल्य;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    प्रत्यागम observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(subject != रिक्त, प्रत्यागम रिक्त);

    यदि(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        प्रत्यागम रिक्त;
    }

    bind_style_prop_t * p = lv_malloc(परिमाण(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    यदि(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        प्रत्यागम रिक्त;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    प्रत्यागम observable;
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
स्थिर lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            प्रत्यागम (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(परिमाण(lv_style_t));
    यदि(new_style == रिक्त) {
        LV_LOG_WARN("couldn't allocate local style");
        प्रत्यागम रिक्त;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * परिमाण(lv_obj_style_t));
    यदि(new_styles == रिक्त) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        प्रत्यागम रिक्त;
    }
    obj->styles = new_styles;

    प्रति(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        यदि(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) भङ्ग;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], परिमाण(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    प्रत्यागम (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
स्थिर lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_trans && obj->styles[i].selector == selector) भङ्ग;
    }

    /*Already have a transition style for it*/
    यदि(i != obj->style_cnt) प्रत्यागम &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(परिमाण(lv_style_t));
    यदि(new_style == रिक्त) {
        LV_LOG_WARN("couldn't allocate transition style");
        प्रत्यागम रिक्त;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * परिमाण(lv_obj_style_t));
    यदि(new_styles == रिक्त) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        प्रत्यागम रिक्त;
    }
    obj->styles = new_styles;

    प्रति(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], परिमाण(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    प्रत्यागम &obj->styles[0];
}

स्थिर lv_style_res_t get_prop_core(ध्रुव lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_मूल्य_t * v)
{

    ध्रुव uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    ध्रुव lv_part_t part = lv_obj_style_get_selector_part(selector);
    ध्रुव bool skip_trans = (ध्रुव bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    प्रति(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        यदि(obj_style->is_trans == false) भङ्ग;
        यदि(skip_trans) अनुवर्त;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        यदि(part_act != part) अनुवर्त;
        यदि((obj_style->style->has_group & group) == 0) अनुवर्त;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        यदि(found == LV_STYLE_RES_FOUND) {
            प्रत्यागम LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ध्रुव lv_state_t state = lv_obj_style_get_selector_state(selector);
    ध्रुव lv_state_t state_inv = ~state;
    प्रति(; i < obj->style_cnt; i++) {
        यदि((obj->styles[i].style->has_group & group) == 0) अनुवर्त;
        यदि(obj->styles[i].is_disabled) अनुवर्त;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        यदि(part_act != part) अनुवर्त;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        यदि((state_act & state_inv)) अनुवर्त;

        /*Check only better candidates*/
        यदि((int32_t)state_act <= weight) अनुवर्त;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        यदि(found == LV_STYLE_RES_FOUND) {
            यदि(state_act == state) {
                प्रत्यागम LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    यदि(weight >= 0) प्रत्यागम LV_STYLE_RES_FOUND;
    अन्यथा प्रत्यागम LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
स्थिर शून्य report_style_change_core(शून्य * style, lv_obj_t * obj)
{
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(style == रिक्त || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            भङ्ग;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    प्रति(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
स्थिर शून्य refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    प्रति(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, रिक्त);
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
स्थिर bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    यावत्(tr != रिक्त) {
        यदि(tr == tr_limit) भङ्ग;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        यदि(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            यदि(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            प्रति(i = 0; i < obj->style_cnt; i++) {
                यदि(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, रिक्त);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    प्रत्यागम removed;
}

स्थिर शून्य trans_anim_cb(शून्य * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) अनुवर्त;

        lv_style_मूल्य_t मूल्य_final = {0};
        विकल्प(tr->prop) {

            पक्ष LV_STYLE_BORDER_SIDE:
            पक्ष LV_STYLE_BORDER_POST:
            पक्ष LV_STYLE_BLEND_MODE:
                यदि(v < 255) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->end_मूल्य.अंक;
                भङ्ग;
            पक्ष LV_STYLE_TRANSITION:
            पक्ष LV_STYLE_TEXT_FONT:
            पक्ष LV_STYLE_BG_IMAGE_SRC:
                यदि(v < 255) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                भङ्ग;
            पक्ष LV_STYLE_COLOR_FILTER_DSC:
                यदि(tr->start_मूल्य.ptr == रिक्त) मूल्य_final.ptr = tr->end_मूल्य.ptr;
                अन्यथा यदि(tr->end_मूल्य.ptr == रिक्त) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा यदि(v < 128) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                भङ्ग;
            /*Colors*/
            पक्ष LV_STYLE_RECOLOR:
            पक्ष LV_STYLE_BG_COLOR:
            पक्ष LV_STYLE_BG_GRAD_COLOR:
            पक्ष LV_STYLE_BORDER_COLOR:
            पक्ष LV_STYLE_TEXT_COLOR:
            पक्ष LV_STYLE_SHADOW_COLOR:
            पक्ष LV_STYLE_OUTLINE_COLOR:
            पक्ष LV_STYLE_IMAGE_RECOLOR:
                यदि(v <= 0) मूल्य_final.color = tr->start_मूल्य.color;
                अन्यथा यदि(v >= 255) मूल्य_final.color = tr->end_मूल्य.color;
                अन्यथा मूल्य_final.color = lv_color_mix(tr->end_मूल्य.color, tr->start_मूल्य.color, v);
                भङ्ग;
            /*Opacities*/
            पक्ष LV_STYLE_OPA:
            पक्ष LV_STYLE_OPA_LAYERED:
            पक्ष LV_STYLE_RECOLOR_OPA:
            पक्ष LV_STYLE_COLOR_FILTER_OPA:
            पक्ष LV_STYLE_BG_OPA:
            पक्ष LV_STYLE_BG_GRAD_OPA:
            पक्ष LV_STYLE_BG_MAIN_OPA:
            पक्ष LV_STYLE_BG_IMAGE_OPA:
            पक्ष LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            पक्ष LV_STYLE_BORDER_OPA:
            पक्ष LV_STYLE_OUTLINE_OPA:
            पक्ष LV_STYLE_SHADOW_OPA:
            पक्ष LV_STYLE_पंक्ति_OPA:
            पक्ष LV_STYLE_ARC_OPA:
            पक्ष LV_STYLE_TEXT_OPA:
            पक्ष LV_STYLE_IMAGE_OPA:
            पक्ष LV_STYLE_IMAGE_RECOLOR_OPA:
            पक्ष LV_STYLE_DROP_SHADOW_OPA:
            पक्ष LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                यदि(v <= 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा यदि(v >= 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक =  LV_CLAMP(0, tr->start_मूल्य.अंक + ((tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v >> 8), 255);
                भङ्ग;

            स्वभाव:
                यदि(v == 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा यदि(v == 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->start_मूल्य.अंक + ((int32_t)((int32_t)(tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v) >> 8);
                भङ्ग;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_मूल्य_t old_मूल्य = {0};
        यदि(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_मूल्य) == LV_STYLE_RES_FOUND) {
            यदि(मूल्य_final.ptr == old_मूल्य.ptr &&
               lv_color_eq(मूल्य_final.color, old_मूल्य.color) &&
               मूल्य_final.अंक == old_मूल्य.अंक) {
                भङ्ग;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        यदि(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, मूल्य_final);

#यदि LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        यदि(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        भङ्ग;
    }

}

स्थिर शून्य trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_मूल्य = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_मूल्य);
#यदि LV_OBJ_STYLE_CACHE
    यदि(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        यदि(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#अन्यथा
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

स्थिर शून्य trans_anim_completed_cb(lv_anim_t * a)
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
        यदि(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            भङ्ग;
        }
    }

    यदि(!running) {
        uint32_t i;
        प्रति(i = 0; i < obj->style_cnt; i++) {
            यदि(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                यदि(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                भङ्ग;
            }
        }
    }
}

स्थिर lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    यदि(lv_obj_get_transform(obj) != रिक्त) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
#endif
    यदि(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
    यदि(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
    यदि(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
    यदि(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
    यदि(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) प्रत्यागम LV_LAYER_TYPE_TRANSFORM;
    यदि(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) प्रत्यागम LV_LAYER_TYPE_SIMPLE;
    यदि(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != रिक्त) प्रत्यागम LV_LAYER_TYPE_SIMPLE;
    यदि(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) प्रत्यागम LV_LAYER_TYPE_SIMPLE;
    प्रत्यागम LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
स्थिर bool calculate_has_blur(lv_obj_t * obj)
{
    ध्रुव uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    ध्रुव uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    ध्रुव lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ध्रुव lv_state_t state_inv = ~state;
    lv_style_मूल्य_t v;
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        यदि(obj_style->is_disabled) अनुवर्त;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        यदि((state_style & state_inv)) अनुवर्त;

        यदि((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            यदि(v.अंक > 0) प्रत्यागम true;
        }
        यदि((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            यदि(v.अंक > 0) प्रत्यागम true;
        }
    }

    प्रत्यागम false;
}

स्थिर शून्य full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#यदि LV_OBJ_STYLE_CACHE
    uint32_t i;
    यदि(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        प्रति(i = 0; i < obj->style_cnt; i++) {
            यदि(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) अनुवर्त;
            यदि(obj->styles[i].is_disabled) अनुवर्त;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            यदि(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्यावली_and_props;
                प्रति(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * परिमाण(lv_style_मूल्य_t);
                प्रति(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    यदि(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        प्रति(i = 0; i < obj->style_cnt; i++) {
            यदि(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) अनुवर्त;
            यदि(obj->styles[i].is_disabled) अनुवर्त;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            यदि(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्यावली_and_props;
                प्रति(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * परिमाण(lv_style_मूल्य_t);
                प्रति(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

स्थिर शून्य fade_anim_cb(शून्य * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

स्थिर शून्य fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

स्थिर bool style_has_flag(ध्रुव lv_style_t * style, uint32_t flag)
{
    यदि(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->मूल्यावली_and_props;
        uint32_t i;
        प्रति(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            यदि(lv_style_prop_has_flag(props[i].prop, flag)) {
                प्रत्यागम true;
            }
        }
    }
    अन्यथा {
        lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * परिमाण(lv_style_मूल्य_t);
        uint32_t i;
        प्रति(i = 0; i < style->prop_cnt; i++) {
            यदि(lv_style_prop_has_flag(props[i], flag)) {
                प्रत्यागम true;
            }
        }
    }
    प्रत्यागम false;
}

स्थिर lv_style_res_t get_selector_style_prop(ध्रुव lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#यदि LV_OBJ_STYLE_CACHE
    ध्रुव uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    यदि((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, मूल्य_act);
        यदि(found == LV_STYLE_RES_FOUND) प्रत्यागम LV_STYLE_RES_FOUND;
    }

    बाह्य ध्रुव uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    यदि(prop < LV_STYLE_अंक_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    अन्यथा {
        यदि(_style_custom_prop_flag_lookup_table != रिक्त) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_अंक_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    यदि(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        यदि(part != LV_PART_MAIN) part = LV_PART_MAIN;
        अन्यथा obj = obj->parent;

        यावत्(obj) {
#यदि LV_OBJ_STYLE_CACHE
            यदि(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, मूल्य_act);
                यदि(found == LV_STYLE_RES_FOUND) प्रत्यागम LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    अन्यथा {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        यदि(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            ध्रुव lv_obj_class_t * cls = obj->class_p;
            यावत्(cls) {
                यदि(prop == LV_STYLE_WIDTH) {
                    यदि(cls->width_def != 0)  {
                        मूल्य_act->अंक = cls->width_def;
                        प्रत्यागम LV_STYLE_RES_FOUND;
                    }
                }
                अन्यथा {
                    यदि(cls->height_def != 0) {
                        मूल्य_act->अंक = cls->height_def;
                        प्रत्यागम LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    प्रत्यागम LV_STYLE_RES_NOT_FOUND;
}

स्थिर शून्य remove_style_core(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != रिक्त);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    यदि((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == रिक्त || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_गिनती = 0;
    bool deleted = false;

    प्रति(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        यदि(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        अन्यथा यदि((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != रिक्त && style != obj->styles[i].style)) {
            should_remove = false;
        }

        यदि(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            यदि(style_गिनती != i) {
                obj->styles[style_गिनती] = obj->styles[i];
            }
            style_गिनती++;
            अनुवर्त;
        }

        /*This entry is being removed*/
        यदि(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);
        }
        यदि(obj->styles[i].is_local || obj->styles[i].is_trans) {
            यदि(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = रिक्त;
        }
        deleted = true;
    }

    यदि(!deleted) {
        प्रत्यागम;
    }

    obj->style_cnt = style_गिनती;
    यदि(style_गिनती > 0) {
        obj->styles = lv_realloc(obj->styles, style_गिनती * परिमाण(lv_obj_style_t));
    }
    अन्यथा {
        lv_free(obj->styles);
        obj->styles = रिक्त;
    }

    यदि(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

स्थिर lv_obj_style_t * find_style(lv_obj_t * obj, ध्रुव lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    प्रति(i = 0; i < obj->style_cnt; i++) {
        यदि(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            प्रत्यागम &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (शून्य *)style, (शून्य *)obj, selector);
    प्रत्यागम रिक्त;
}



#यदि LV_USE_OBSERVER

स्थिर शून्य bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->मूल्य);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

स्थिर शून्य bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_मूल्य_t style_v;
    यदि(subject->type == LV_SUBJECT_TYPE_INT) style_v.अंक = lv_subject_get_int(subject);
    अन्यथा यदि(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    अन्यथा यदि(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    अन्यथा {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        प्रत्यागम;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
