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

प्रकारव्याख्या संरचना {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_मूल्य_t start_मूल्य;
    lv_style_मूल्य_t end_मूल्य;
} trans_t;

प्रकारव्याख्या संरचना {
    स्थिरांक lv_style_t * style;
    lv_style_selector_t selector;
    int32_t मूल्य;
} bind_style_t;

प्रकारव्याख्या संरचना {
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
स्थिर lv_style_res_t get_prop_core(स्थिरांक lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
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
स्थिर bool style_has_flag(स्थिरांक lv_style_t * style, uint32_t flag);
स्थिर lv_style_res_t get_selector_style_prop(स्थिरांक lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act);
स्थिर शून्य remove_style_core(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector, bool theme_only);
स्थिर lv_obj_style_t * find_style(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector);
#जर LV_USE_OBSERVER
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
    lv_ll_init(style_trans_ll_p, आकार(trans_t));
}

शून्य lv_obj_style_deinit(शून्य)
{
    lv_ll_clear(style_trans_ll_p);
    जर(_style_custom_prop_flag_lookup_table != रिक्त) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = रिक्त;
    }
}

शून्य lv_obj_add_style(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(style != रिक्त, परत);
    LV_CHECK_ARG_संदेश(obj->style_cnt < 63, परत,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    जर(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_trans) सुरूठेवा;
        जर(obj->styles[i].is_local) सुरूठेवा;
        थांबा;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * आकार(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    साठी(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], आकार(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    जर(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#जर LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        जर(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->मूल्ये_and_props;
            साठी(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        अन्यथा {
            lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्ये_and_props + style->prop_cnt * आकार(lv_style_मूल्य_t);
            साठी(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, स्थिरांक lv_style_t * old_style, स्थिरांक lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत false);
    LV_CHECK_ARG(old_style != रिक्त, परत false);
    LV_CHECK_ARG(new_style != रिक्त, परत false);
    LV_CHECK_ARG(old_style != new_style, परत false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    जर((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);

    bool replaced = false;
    uint32_t i;
    साठी(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        जर(obj->styles[i].is_local || obj->styles[i].is_trans) {
            सुरूठेवा;
        }

        /*Skip non-matching styles*/
        जर((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            सुरूठेवा;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], आकार(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    जर(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    परत replaced;
}

शून्य lv_obj_remove_style(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    remove_style_core(obj, style, selector, false);
}

शून्य lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    remove_style_core(obj, रिक्त, selector, true);
}

शून्य lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    remove_style_core(obj, रिक्त, LV_PART_ANY | LV_STATE_ANY, false);
}

शून्य lv_obj_report_style_change(lv_style_t * style)
{
    जर(!style_refr) परत;
    lv_display_t * d = lv_display_get_next(रिक्त);

    पर्यंत(d) {
        uint32_t i;
        साठी(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

शून्य lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत);

    जर(!style_refr) परत;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    जर(is_layout_refr) {
        जर(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, रिक्त);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    जर((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        जर(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    जर((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    जर(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    जर(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    जर(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        जर(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

शून्य lv_obj_set_style_enabled(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(style != रिक्त, परत);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    जर(obj_style == रिक्त) परत;

    जर(en != obj_style->is_disabled) परत; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    जर((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत false);
    LV_CHECK_ARG(style != रिक्त, परत false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    जर(obj_style == रिक्त) परत false;

    परत !obj_style->is_disabled;
}


शून्य lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_मूल्य_t lv_obj_get_style_prop(स्थिरांक lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != रिक्त, परत lv_style_prop_get_default(prop));
    परत lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_मूल्य_t lv_obj_get_style_prop_internal(स्थिरांक lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != रिक्त);
    lv_style_selector_t selector = part | obj->state;
    lv_style_मूल्य_t मूल्य_act = { .ptr = रिक्त };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    जर(found == LV_STYLE_RES_FOUND) परत मूल्य_act;

    परत lv_style_prop_get_default(prop);
}
lv_style_मूल्य_t lv_obj_style_apply_color_filter_internal(स्थिरांक lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
#जर LV_USE_COLOR_FILTER
    LV_ASSERT(obj != रिक्त);

    स्थिरांक lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    जर(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        जर(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    परत v;
}

bool lv_obj_has_style_prop(स्थिरांक lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != रिक्त, परत false);

    lv_style_मूल्य_t मूल्य_act = { .ptr = रिक्त };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    जर(found == LV_STYLE_RES_FOUND) परत true;

    परत false;
}

शून्य lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_मूल्य_t मूल्य,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    जर(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, रिक्त);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, मूल्य);

    जर(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#जर LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        जर(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
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
    LV_CHECK_ARG(obj != रिक्त, परत LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(मूल्य != रिक्त, परत LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            परत lv_style_get_prop_internal(obj->styles[i].style, prop, मूल्य);
        }
    }

    परत LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, परत 0);

    uint32_t i;
    /*Find the style*/
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            थांबा;
        }
    }

    /*The style is not found*/
    जर(i == obj->style_cnt) परत false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    जर(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    परत res;
}

शून्य lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    स्थिरांक lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(tr_dsc != रिक्त, परत);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_मूल्य_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_मूल्य_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    जर(v1.ptr == v2.ptr && v1.अंक == v2.अंक && lv_color_eq(v1.color, v2.color))  परत;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    जर(tr_dsc->prop == LV_STYLE_RADIUS) {
        जर(v1.अंक == LV_RADIUS_CIRCLE || v2.अंक == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            जर(v1.अंक == LV_RADIUS_CIRCLE) v1.अंक = LV_MIN(whalf + 1, hhalf + 1);
            जर(v2.अंक == LV_RADIUS_CIRCLE) v2.अंक = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    जर(tr == रिक्त) परत;
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
    lv_anim_set_मूल्ये(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->समय);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_मूल्य_t lv_obj_style_apply_color_filter(स्थिरांक lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
    LV_CHECK_ARG(obj != रिक्त, परत v);
    परत lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_STYLE_STATE_CMP_SAME);

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
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_trans) सुरूठेवा;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        जर(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            जर(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            अन्यथा जर(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            स्थिरांक lv_style_t * style = obj->styles[i].style;
            lv_style_मूल्य_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_शीर्ष, &v))layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_डावा, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_उजवा, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            जर(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                जर(changed_part) *changed_part = LV_PART_ANY;
                परत LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(lv_style_get_prop_internal(style, LV_STYLE_ओळ_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा जर(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    जर(changed_part) *changed_part = diff_part;
    परत res;
}

शून्य lv_obj_fade_in(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_मूल्ये(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, समय);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

शून्य lv_obj_fade_out(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_मूल्ये(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, समय);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(स्थिरांक lv_obj_t * obj, lv_part_t part, स्थिरांक अक्षर * txt)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != रिक्त, परत LV_TEXT_ALIGN_AUTO);
    परत lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    जर(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        परत LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    जर(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    जर(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    पर्यंत(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        जर(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            परत LV_OPA_TRANSP;
        }
        जर(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    जर(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        परत LV_OPA_TRANSP;
    }

    जर(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        परत LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    परत opa_final;
}

शून्य lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, परत);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    जर(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    अन्यथा जर(layer_type != LV_LAYER_TYPE_NONE) {
        जर(!lv_obj_allocate_spec_attr(obj)) {
            परत;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

शून्य lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    जर(obj->is_deleting) परत;

    bool has_blur = calculate_has_blur(obj);
    जर(has_blur == (bool)obj->has_blur) परत;

    obj->has_blur = has_blur;
    जर(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    अन्यथा {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(स्थिरांक lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != रिक्त, परत (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    जर(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    परत color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, परत (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    जर(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    पर्यंत(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    परत result;
}

शून्य lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_pad_डावा(obj, मूल्य, selector);
    lv_obj_set_style_pad_उजवा(obj, मूल्य, selector);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_pad_डावा(obj, मूल्य, selector);
    lv_obj_set_style_pad_उजवा(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_margin_डावा(obj, मूल्य, selector);
    lv_obj_set_style_margin_उजवा(obj, मूल्य, selector);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_margin_डावा(obj, मूल्य, selector);
    lv_obj_set_style_margin_उजवा(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_pad_row(obj, मूल्य, selector);
    lv_obj_set_style_pad_column(obj, मूल्य, selector);
}

शून्य lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

शून्य lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत);
    lv_obj_set_style_transform_scale_x(obj, मूल्य, selector);
    lv_obj_set_style_transform_scale_y(obj, मूल्य, selector);
}

int32_t lv_obj_get_style_space_डावा(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_space_डावा_internal(obj, part);
}

int32_t lv_obj_get_style_space_उजवा(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_space_उजवा_internal(obj, part);
}

int32_t lv_obj_get_style_space_शीर्ष(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_space_शीर्ष_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, परत 0)
    परत lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#जर LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(style != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);

    जर(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        परत रिक्त;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(आकार(bind_style_t));
    LV_ASSERT_MALLOC(p);
    जर(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        परत रिक्त;
    }

    p->style = style;
    p->selector = selector;
    p->मूल्य = ref_मूल्य;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    परत observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(subject != रिक्त, परत रिक्त);

    जर(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        परत रिक्त;
    }

    bind_style_prop_t * p = lv_malloc(आकार(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    जर(p == रिक्त) {
        LV_LOG_WARN("Out of memory");
        परत रिक्त;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    परत observable;
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
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            परत (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(आकार(lv_style_t));
    जर(new_style == रिक्त) {
        LV_LOG_WARN("couldn't allocate local style");
        परत रिक्त;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * आकार(lv_obj_style_t));
    जर(new_styles == रिक्त) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        परत रिक्त;
    }
    obj->styles = new_styles;

    साठी(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        जर(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) थांबा;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], आकार(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    परत (lv_style_t *)obj->styles[i].style;
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
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_trans && obj->styles[i].selector == selector) थांबा;
    }

    /*Already have a transition style for it*/
    जर(i != obj->style_cnt) परत &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(आकार(lv_style_t));
    जर(new_style == रिक्त) {
        LV_LOG_WARN("couldn't allocate transition style");
        परत रिक्त;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * आकार(lv_obj_style_t));
    जर(new_styles == रिक्त) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        परत रिक्त;
    }
    obj->styles = new_styles;

    साठी(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], आकार(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    परत &obj->styles[0];
}

स्थिर lv_style_res_t get_prop_core(स्थिरांक lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_मूल्य_t * v)
{

    स्थिरांक uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    स्थिरांक lv_part_t part = lv_obj_style_get_selector_part(selector);
    स्थिरांक bool skip_trans = (स्थिरांक bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    साठी(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        जर(obj_style->is_trans == false) थांबा;
        जर(skip_trans) सुरूठेवा;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        जर(part_act != part) सुरूठेवा;
        जर((obj_style->style->has_group & group) == 0) सुरूठेवा;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        जर(found == LV_STYLE_RES_FOUND) {
            परत LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    स्थिरांक lv_state_t state = lv_obj_style_get_selector_state(selector);
    स्थिरांक lv_state_t state_inv = ~state;
    साठी(; i < obj->style_cnt; i++) {
        जर((obj->styles[i].style->has_group & group) == 0) सुरूठेवा;
        जर(obj->styles[i].is_disabled) सुरूठेवा;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        जर(part_act != part) सुरूठेवा;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        जर((state_act & state_inv)) सुरूठेवा;

        /*Check only better candidates*/
        जर((int32_t)state_act <= weight) सुरूठेवा;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        जर(found == LV_STYLE_RES_FOUND) {
            जर(state_act == state) {
                परत LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    जर(weight >= 0) परत LV_STYLE_RES_FOUND;
    अन्यथा परत LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
स्थिर शून्य report_style_change_core(शून्य * style, lv_obj_t * obj)
{
    uint32_t i;
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(style == रिक्त || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            थांबा;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_मोजणी(obj);
    साठी(i = 0; i < child_cnt; i++) {
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
    uint32_t child_cnt = lv_obj_get_child_मोजणी(obj);
    साठी(i = 0; i < child_cnt; i++) {
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
    पर्यंत(tr != रिक्त) {
        जर(tr == tr_limit) थांबा;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        जर(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            जर(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            साठी(i = 0; i < obj->style_cnt; i++) {
                जर(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
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

    परत removed;
}

स्थिर शून्य trans_anim_cb(शून्य * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) सुरूठेवा;

        lv_style_मूल्य_t मूल्य_final = {0};
        निवड(tr->prop) {

            स्थिती LV_STYLE_BORDER_SIDE:
            स्थिती LV_STYLE_BORDER_POST:
            स्थिती LV_STYLE_BLEND_MODE:
                जर(v < 255) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->end_मूल्य.अंक;
                थांबा;
            स्थिती LV_STYLE_TRANSITION:
            स्थिती LV_STYLE_TEXT_FONT:
            स्थिती LV_STYLE_BG_IMAGE_SRC:
                जर(v < 255) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                थांबा;
            स्थिती LV_STYLE_COLOR_FILTER_DSC:
                जर(tr->start_मूल्य.ptr == रिक्त) मूल्य_final.ptr = tr->end_मूल्य.ptr;
                अन्यथा जर(tr->end_मूल्य.ptr == रिक्त) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा जर(v < 128) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                थांबा;
            /*Colors*/
            स्थिती LV_STYLE_RECOLOR:
            स्थिती LV_STYLE_BG_COLOR:
            स्थिती LV_STYLE_BG_GRAD_COLOR:
            स्थिती LV_STYLE_BORDER_COLOR:
            स्थिती LV_STYLE_TEXT_COLOR:
            स्थिती LV_STYLE_SHADOW_COLOR:
            स्थिती LV_STYLE_OUTLINE_COLOR:
            स्थिती LV_STYLE_IMAGE_RECOLOR:
                जर(v <= 0) मूल्य_final.color = tr->start_मूल्य.color;
                अन्यथा जर(v >= 255) मूल्य_final.color = tr->end_मूल्य.color;
                अन्यथा मूल्य_final.color = lv_color_mix(tr->end_मूल्य.color, tr->start_मूल्य.color, v);
                थांबा;
            /*Opacities*/
            स्थिती LV_STYLE_OPA:
            स्थिती LV_STYLE_OPA_LAYERED:
            स्थिती LV_STYLE_RECOLOR_OPA:
            स्थिती LV_STYLE_COLOR_FILTER_OPA:
            स्थिती LV_STYLE_BG_OPA:
            स्थिती LV_STYLE_BG_GRAD_OPA:
            स्थिती LV_STYLE_BG_MAIN_OPA:
            स्थिती LV_STYLE_BG_IMAGE_OPA:
            स्थिती LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            स्थिती LV_STYLE_BORDER_OPA:
            स्थिती LV_STYLE_OUTLINE_OPA:
            स्थिती LV_STYLE_SHADOW_OPA:
            स्थिती LV_STYLE_ओळ_OPA:
            स्थिती LV_STYLE_ARC_OPA:
            स्थिती LV_STYLE_TEXT_OPA:
            स्थिती LV_STYLE_IMAGE_OPA:
            स्थिती LV_STYLE_IMAGE_RECOLOR_OPA:
            स्थिती LV_STYLE_DROP_SHADOW_OPA:
            स्थिती LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                जर(v <= 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा जर(v >= 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक =  LV_CLAMP(0, tr->start_मूल्य.अंक + ((tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v >> 8), 255);
                थांबा;

            पूर्वनिर्धारित:
                जर(v == 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा जर(v == 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->start_मूल्य.अंक + ((int32_t)((int32_t)(tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v) >> 8);
                थांबा;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_मूल्य_t old_मूल्य = {0};
        जर(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_मूल्य) == LV_STYLE_RES_FOUND) {
            जर(मूल्य_final.ptr == old_मूल्य.ptr &&
               lv_color_eq(मूल्य_final.color, old_मूल्य.color) &&
               मूल्य_final.अंक == old_मूल्य.अंक) {
                थांबा;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        जर(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, मूल्य_final);

#जर LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        जर(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        थांबा;
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
#जर LV_OBJ_STYLE_CACHE
    जर(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        जर(part == LV_PART_MAIN) {
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
        जर(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            थांबा;
        }
    }

    जर(!running) {
        uint32_t i;
        साठी(i = 0; i < obj->style_cnt; i++) {
            जर(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                जर(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                थांबा;
            }
        }
    }
}

स्थिर lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#जर LV_DRAW_TRANSFORM_USE_MATRIX
    जर(lv_obj_get_transform(obj) != रिक्त) परत LV_LAYER_TYPE_TRANSFORM;
#endif
    जर(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) परत LV_LAYER_TYPE_TRANSFORM;
    जर(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) परत LV_LAYER_TYPE_TRANSFORM;
    जर(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) परत LV_LAYER_TYPE_TRANSFORM;
    जर(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) परत LV_LAYER_TYPE_TRANSFORM;
    जर(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) परत LV_LAYER_TYPE_TRANSFORM;
    जर(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) परत LV_LAYER_TYPE_SIMPLE;
    जर(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != रिक्त) परत LV_LAYER_TYPE_SIMPLE;
    जर(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) परत LV_LAYER_TYPE_SIMPLE;
    परत LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
स्थिर bool calculate_has_blur(lv_obj_t * obj)
{
    स्थिरांक uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    स्थिरांक uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    स्थिरांक lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    स्थिरांक lv_state_t state_inv = ~state;
    lv_style_मूल्य_t v;
    uint32_t i;
    साठी(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        जर(obj_style->is_disabled) सुरूठेवा;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        जर((state_style & state_inv)) सुरूठेवा;

        जर((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            जर(v.अंक > 0) परत true;
        }
        जर((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            जर(v.अंक > 0) परत true;
        }
    }

    परत false;
}

स्थिर शून्य full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#जर LV_OBJ_STYLE_CACHE
    uint32_t i;
    जर(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        साठी(i = 0; i < obj->style_cnt; i++) {
            जर(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) सुरूठेवा;
            जर(obj->styles[i].is_disabled) सुरूठेवा;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            जर(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्ये_and_props;
                साठी(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्ये_and_props + style->prop_cnt * आकार(lv_style_मूल्य_t);
                साठी(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    जर(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        साठी(i = 0; i < obj->style_cnt; i++) {
            जर(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) सुरूठेवा;
            जर(obj->styles[i].is_disabled) सुरूठेवा;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            जर(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्ये_and_props;
                साठी(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्ये_and_props + style->prop_cnt * आकार(lv_style_मूल्य_t);
                साठी(j = 0; j < style->prop_cnt; j++) {
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

स्थिर bool style_has_flag(स्थिरांक lv_style_t * style, uint32_t flag)
{
    जर(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->मूल्ये_and_props;
        uint32_t i;
        साठी(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            जर(lv_style_prop_has_flag(props[i].prop, flag)) {
                परत true;
            }
        }
    }
    अन्यथा {
        lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्ये_and_props + style->prop_cnt * आकार(lv_style_मूल्य_t);
        uint32_t i;
        साठी(i = 0; i < style->prop_cnt; i++) {
            जर(lv_style_prop_has_flag(props[i], flag)) {
                परत true;
            }
        }
    }
    परत false;
}

स्थिर lv_style_res_t get_selector_style_prop(स्थिरांक lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#जर LV_OBJ_STYLE_CACHE
    स्थिरांक uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    जर((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, मूल्य_act);
        जर(found == LV_STYLE_RES_FOUND) परत LV_STYLE_RES_FOUND;
    }

    बाह्य स्थिरांक uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    जर(prop < LV_STYLE_अंक_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    अन्यथा {
        जर(_style_custom_prop_flag_lookup_table != रिक्त) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_अंक_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    जर(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        जर(part != LV_PART_MAIN) part = LV_PART_MAIN;
        अन्यथा obj = obj->parent;

        पर्यंत(obj) {
#जर LV_OBJ_STYLE_CACHE
            जर(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, मूल्य_act);
                जर(found == LV_STYLE_RES_FOUND) परत LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    अन्यथा {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        जर(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            स्थिरांक lv_obj_class_t * cls = obj->class_p;
            पर्यंत(cls) {
                जर(prop == LV_STYLE_WIDTH) {
                    जर(cls->width_def != 0)  {
                        मूल्य_act->अंक = cls->width_def;
                        परत LV_STYLE_RES_FOUND;
                    }
                }
                अन्यथा {
                    जर(cls->height_def != 0) {
                        मूल्य_act->अंक = cls->height_def;
                        परत LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    परत LV_STYLE_RES_NOT_FOUND;
}

स्थिर शून्य remove_style_core(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != रिक्त);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    जर((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == रिक्त || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_मोजणी = 0;
    bool deleted = false;

    साठी(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        जर(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        अन्यथा जर((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != रिक्त && style != obj->styles[i].style)) {
            should_remove = false;
        }

        जर(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            जर(style_मोजणी != i) {
                obj->styles[style_मोजणी] = obj->styles[i];
            }
            style_मोजणी++;
            सुरूठेवा;
        }

        /*This entry is being removed*/
        जर(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, रिक्त);
        }
        जर(obj->styles[i].is_local || obj->styles[i].is_trans) {
            जर(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = रिक्त;
        }
        deleted = true;
    }

    जर(!deleted) {
        परत;
    }

    obj->style_cnt = style_मोजणी;
    जर(style_मोजणी > 0) {
        obj->styles = lv_realloc(obj->styles, style_मोजणी * आकार(lv_obj_style_t));
    }
    अन्यथा {
        lv_free(obj->styles);
        obj->styles = रिक्त;
    }

    जर(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

स्थिर lv_obj_style_t * find_style(lv_obj_t * obj, स्थिरांक lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    साठी(i = 0; i < obj->style_cnt; i++) {
        जर(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            परत &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (शून्य *)style, (शून्य *)obj, selector);
    परत रिक्त;
}



#जर LV_USE_OBSERVER

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
    जर(subject->type == LV_SUBJECT_TYPE_INT) style_v.अंक = lv_subject_get_int(subject);
    अन्यथा जर(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    अन्यथा जर(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    अन्यथा {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        परत;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
