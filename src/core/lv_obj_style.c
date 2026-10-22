/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "lv_obj_private.h"
#समावेश "../lvgl_public.h"
#समावेश "../misc/lv_anim_private.h"
#समावेश "../misc/lv_bidi_private.h"
#समावेश "../misc/lv_style_private.h"
#समावेश "lv_obj_style_private.h"
#समावेश "lv_obj_class_private.h"
#समावेश "../display/lv_display_private.h"
#समावेश "../core/lv_global.h"
#समावेश "lv_observer_private.h"
#समावेश "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा MY_CLASS (&lv_obj_class)
#घोषणा style_refr LV_GLOBAL_DEFAULT()->style_refresh
#घोषणा style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#घोषणा _style_custom_prop_flag_lookup_table LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_table
#घोषणा STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#घोषणा DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

प्रकार काष्ठा {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_मूल्य_t start_मूल्य;
    lv_style_मूल्य_t end_मूल्य;
} trans_t;

प्रकार काष्ठा {
    स्थिर lv_style_t * style;
    lv_style_selector_t selector;
    int32_t मूल्य;
} bind_style_t;

प्रकार काष्ठा {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
अटल lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
अटल lv_style_res_t get_prop_core(स्थिर lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_मूल्य_t * v);
अटल व्योम report_style_change_core(व्योम * style, lv_obj_t * obj);
अटल व्योम refresh_children_style(lv_obj_t * obj);
अटल bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
अटल व्योम trans_anim_cb(व्योम * _tr, int32_t v);
अटल व्योम trans_anim_start_cb(lv_anim_t * a);
अटल व्योम trans_anim_completed_cb(lv_anim_t * a);
अटल lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
अटल bool calculate_has_blur(lv_obj_t * obj);
अटल व्योम full_cache_refresh(lv_obj_t * obj, lv_part_t part);
अटल व्योम fade_anim_cb(व्योम * obj, int32_t v);
अटल व्योम fade_in_anim_completed(lv_anim_t * a);
अटल bool style_has_flag(स्थिर lv_style_t * style, uint32_t flag);
अटल lv_style_res_t get_selector_style_prop(स्थिर lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act);
अटल व्योम remove_style_core(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector, bool theme_only);
अटल lv_obj_style_t * find_style(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector);
#अगर LV_USE_OBSERVER
    अटल व्योम bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    अटल व्योम bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#पूर्ण_अगर

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

व्योम lv_obj_style_init(व्योम)
{
    lv_ll_init(style_trans_ll_p, माप(trans_t));
}

व्योम lv_obj_style_deinit(व्योम)
{
    lv_ll_clear(style_trans_ll_p);
    अगर(_style_custom_prop_flag_lookup_table != शून्य) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = शून्य;
    }
}

व्योम lv_obj_add_style(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(style != शून्य, वापस);
    LV_CHECK_ARG_संदेश(obj->style_cnt < 63, वापस,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    अगर(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, शून्य);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_trans) जारी;
        अगर(obj->styles[i].is_local) जारी;
        अवरोध;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * माप(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    क्रम(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], माप(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    अगर(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#अगर LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        अगर(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->मूल्यावली_and_props;
            क्रम(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        अन्यथा {
            lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * माप(lv_style_मूल्य_t);
            क्रम(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#पूर्ण_अगर

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, स्थिर lv_style_t * old_style, स्थिर lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस false);
    LV_CHECK_ARG(old_style != शून्य, वापस false);
    LV_CHECK_ARG(new_style != शून्य, वापस false);
    LV_CHECK_ARG(old_style != new_style, वापस false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    अगर((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, शून्य);

    bool replaced = false;
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        अगर(obj->styles[i].is_local || obj->styles[i].is_trans) {
            जारी;
        }

        /*Skip non-matching styles*/
        अगर((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            जारी;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], माप(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    अगर(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    वापस replaced;
}

व्योम lv_obj_remove_style(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    remove_style_core(obj, style, selector, false);
}

व्योम lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    remove_style_core(obj, शून्य, selector, true);
}

व्योम lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    remove_style_core(obj, शून्य, LV_PART_ANY | LV_STATE_ANY, false);
}

व्योम lv_obj_report_style_change(lv_style_t * style)
{
    अगर(!style_refr) वापस;
    lv_display_t * d = lv_display_get_अग्र(शून्य);

    जबतक(d) {
        uint32_t i;
        क्रम(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_अग्र(d);
    }
}

व्योम lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!style_refr) वापस;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    अगर(is_layout_refr) {
        अगर(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_आकार_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_आकार_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, शून्य);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    अगर((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        अगर(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    अगर((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    अगर(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    अगर(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_आकार(obj);
    }
    lv_obj_invalidate(obj);

    अगर(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        अगर(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

व्योम lv_obj_set_style_enabled(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(style != शून्य, वापस);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    अगर(obj_style == शून्य) वापस;

    अगर(en != obj_style->is_disabled) वापस; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    अगर((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस false);
    LV_CHECK_ARG(style != शून्य, वापस false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    अगर(obj_style == शून्य) वापस false;

    वापस !obj_style->is_disabled;
}


व्योम lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_मूल्य_t lv_obj_get_style_prop(स्थिर lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != शून्य, वापस lv_style_prop_get_default(prop));
    वापस lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_मूल्य_t lv_obj_get_style_prop_internal(स्थिर lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != शून्य);
    lv_style_selector_t selector = part | obj->state;
    lv_style_मूल्य_t मूल्य_act = { .ptr = शून्य };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    अगर(found == LV_STYLE_RES_FOUND) वापस मूल्य_act;

    वापस lv_style_prop_get_default(prop);
}
lv_style_मूल्य_t lv_obj_style_apply_color_filter_internal(स्थिर lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
#अगर LV_USE_COLOR_FILTER
    LV_ASSERT(obj != शून्य);

    स्थिर lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    अगर(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        अगर(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#पूर्ण_अगर
    वापस v;
}

bool lv_obj_has_style_prop(स्थिर lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != शून्य, वापस false);

    lv_style_मूल्य_t मूल्य_act = { .ptr = शून्य };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &मूल्य_act);
    अगर(found == LV_STYLE_RES_FOUND) वापस true;

    वापस false;
}

व्योम lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_मूल्य_t मूल्य,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    अगर(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, शून्य);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, मूल्य);

    अगर(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#अगर LV_OBJ_STYLE_CACHE
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        अगर(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#पूर्ण_अगर

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_मूल्य_t * मूल्य,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(मूल्य != शून्य, वापस LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            वापस lv_style_get_prop_internal(obj->styles[i].style, prop, मूल्य);
        }
    }

    वापस LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    uint32_t i;
    /*Find the style*/
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            अवरोध;
        }
    }

    /*The style is not found*/
    अगर(i == obj->style_cnt) वापस false;

    lv_परिणाम_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    अगर(res == LV_परिणाम_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    वापस res;
}

व्योम lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t पूर्व_state, lv_state_t new_state,
                                    स्थिर lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(tr_dsc != शून्य, वापस);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = पूर्व_state;
    lv_style_मूल्य_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_मूल्य_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    अगर(v1.ptr == v2.ptr && v1.अंक == v2.अंक && lv_color_eq(v1.color, v2.color))  वापस;
    obj->state = पूर्व_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    अगर(tr_dsc->prop == LV_STYLE_RADIUS) {
        अगर(v1.अंक == LV_RADIUS_CIRCLE || v2.अंक == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            अगर(v1.अंक == LV_RADIUS_CIRCLE) v1.अंक = LV_MIN(whalf + 1, hhalf + 1);
            अगर(v2.अंक == LV_RADIUS_CIRCLE) v2.अंक = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_शीर्ष(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    अगर(tr == शून्य) वापस;
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
    lv_anim_set_user_तथ्य(&a, tr_dsc->user_तथ्य);
    lv_anim_start(&a);
}

lv_style_मूल्य_t lv_obj_style_apply_color_filter(स्थिर lv_obj_t * obj, lv_part_t part, lv_style_मूल्य_t v)
{
    LV_CHECK_ARG(obj != शून्य, वापस v);
    वापस lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_STYLE_STATE_CMP_SAME);

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
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_trans) जारी;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        अगर(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            अगर(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            अन्यथा अगर(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            स्थिर lv_style_t * style = obj->styles[i].style;
            lv_style_मूल्य_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_शीर्ष, &v))layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_बायाँ, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_दायाँ, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            अगर(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                अगर(changed_part) *changed_part = LV_PART_ANY;
                वापस LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(lv_style_get_prop_internal(style, LV_STYLE_पंक्ति_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            अन्यथा अगर(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    अगर(changed_part) *changed_part = diff_part;
    वापस res;
}

व्योम lv_obj_fade_in(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

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

व्योम lv_obj_fade_out(lv_obj_t * obj, uint32_t समय, uint32_t delay)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_मूल्यावली(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, समय);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(स्थिर lv_obj_t * obj, lv_part_t part, स्थिर अक्षर * txt)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != शून्य, वापस LV_TEXT_ALIGN_AUTO);
    वापस lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    अगर(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        वापस LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    अगर(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    अगर(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    जबतक(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        अगर(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            वापस LV_OPA_TRANSP;
        }
        अगर(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    अगर(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        वापस LV_OPA_TRANSP;
    }

    अगर(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        वापस LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    वापस opa_final;
}

व्योम lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    अगर(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    अन्यथा अगर(layer_type != LV_LAYER_TYPE_NONE) {
        अगर(!lv_obj_allocate_spec_attr(obj)) {
            वापस;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

व्योम lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    अगर(obj->is_deleting) वापस;

    bool has_blur = calculate_has_blur(obj);
    अगर(has_blur == (bool)obj->has_blur) वापस;

    obj->has_blur = has_blur;
    अगर(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    अन्यथा {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(स्थिर lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != शून्य, वापस (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    अगर(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    वापस color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != शून्य, वापस (lv_color32_t) {
        0
    });

    lv_color32_t परिणाम;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    परिणाम = lv_color_to_32(color, opa);

    अगर(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    अन्यथा {
        obj = lv_obj_get_parent(obj);
    }

    जबतक(obj) {
        परिणाम = lv_obj_style_apply_recolor(obj, part, परिणाम);
        obj = lv_obj_get_parent(obj);
    }

    वापस परिणाम;
}

व्योम lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_pad_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_दायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_pad_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_pad_दायाँ(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_pad_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_pad_bottom(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_margin_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_दायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_margin_बायाँ(obj, मूल्य, selector);
    lv_obj_set_style_margin_दायाँ(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_margin_शीर्ष(obj, मूल्य, selector);
    lv_obj_set_style_margin_bottom(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_pad_row(obj, मूल्य, selector);
    lv_obj_set_style_pad_column(obj, मूल्य, selector);
}

व्योम lv_obj_set_style_आकार(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

व्योम lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t मूल्य, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस);
    lv_obj_set_style_transform_scale_x(obj, मूल्य, selector);
    lv_obj_set_style_transform_scale_y(obj, मूल्य, selector);
}

int32_t lv_obj_get_style_space_बायाँ(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_space_बायाँ_internal(obj, part);
}

int32_t lv_obj_get_style_space_दायाँ(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_space_दायाँ_internal(obj, part);
}

int32_t lv_obj_get_style_space_शीर्ष(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_space_शीर्ष_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, वापस 0)
    वापस lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#अगर LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_मूल्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(style != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);

    अगर(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        वापस शून्य;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(माप(bind_style_t));
    LV_ASSERT_MALLOC(p);
    अगर(p == शून्य) {
        LV_LOG_WARN("Out of memory");
        वापस शून्य;
    }

    p->style = style;
    p->selector = selector;
    p->मूल्य = ref_मूल्य;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_तथ्य = 1;
    वापस observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(subject != शून्य, वापस शून्य);

    अगर(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        वापस शून्य;
    }

    bind_style_prop_t * p = lv_malloc(माप(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    अगर(p == शून्य) {
        LV_LOG_WARN("Out of memory");
        वापस शून्य;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_तथ्य = 1;
    वापस observable;
}


#पूर्ण_अगर /*LV_USE_OBSERVER*/


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
अटल lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            वापस (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(माप(lv_style_t));
    अगर(new_style == शून्य) {
        LV_LOG_WARN("couldn't allocate local style");
        वापस शून्य;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * माप(lv_obj_style_t));
    अगर(new_styles == शून्य) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        वापस शून्य;
    }
    obj->styles = new_styles;

    क्रम(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        अगर(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) अवरोध;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], माप(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    वापस (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
अटल lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_trans && obj->styles[i].selector == selector) अवरोध;
    }

    /*Already have a transition style for it*/
    अगर(i != obj->style_cnt) वापस &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(माप(lv_style_t));
    अगर(new_style == शून्य) {
        LV_LOG_WARN("couldn't allocate transition style");
        वापस शून्य;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * माप(lv_obj_style_t));
    अगर(new_styles == शून्य) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        वापस शून्य;
    }
    obj->styles = new_styles;

    क्रम(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], माप(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    वापस &obj->styles[0];
}

अटल lv_style_res_t get_prop_core(स्थिर lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_मूल्य_t * v)
{

    स्थिर uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    स्थिर lv_part_t part = lv_obj_style_get_selector_part(selector);
    स्थिर bool skip_trans = (स्थिर bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    क्रम(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        अगर(obj_style->is_trans == false) अवरोध;
        अगर(skip_trans) जारी;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        अगर(part_act != part) जारी;
        अगर((obj_style->style->has_group & group) == 0) जारी;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        अगर(found == LV_STYLE_RES_FOUND) {
            वापस LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    स्थिर lv_state_t state = lv_obj_style_get_selector_state(selector);
    स्थिर lv_state_t state_inv = ~state;
    क्रम(; i < obj->style_cnt; i++) {
        अगर((obj->styles[i].style->has_group & group) == 0) जारी;
        अगर(obj->styles[i].is_disabled) जारी;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        अगर(part_act != part) जारी;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        अगर((state_act & state_inv)) जारी;

        /*Check only better candidates*/
        अगर((int32_t)state_act <= weight) जारी;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        अगर(found == LV_STYLE_RES_FOUND) {
            अगर(state_act == state) {
                वापस LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    अगर(weight >= 0) वापस LV_STYLE_RES_FOUND;
    अन्यथा वापस LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
अटल व्योम report_style_change_core(व्योम * style, lv_obj_t * obj)
{
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(style == शून्य || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            अवरोध;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
अटल व्योम refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, शून्य);
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
अटल bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_पूर्व;
    bool removed = false;
    tr = lv_ll_get_पुच्छ(style_trans_ll_p);
    जबतक(tr != शून्य) {
        अगर(tr == tr_limit) अवरोध;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_पूर्व = lv_ll_get_पूर्व(style_trans_ll_p, tr);

        अगर(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            अगर(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            क्रम(i = 0; i < obj->style_cnt; i++) {
                अगर(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, शून्य);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_पूर्व;
    }

    वापस removed;
}

अटल व्योम trans_anim_cb(व्योम * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) जारी;

        lv_style_मूल्य_t मूल्य_final = {0};
        चयन(tr->prop) {

            हाल LV_STYLE_BORDER_SIDE:
            हाल LV_STYLE_BORDER_POST:
            हाल LV_STYLE_BLEND_MODE:
                अगर(v < 255) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अवरोध;
            हाल LV_STYLE_TRANSITION:
            हाल LV_STYLE_TEXT_FONT:
            हाल LV_STYLE_BG_IMAGE_SRC:
                अगर(v < 255) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                अवरोध;
            हाल LV_STYLE_COLOR_FILTER_DSC:
                अगर(tr->start_मूल्य.ptr == शून्य) मूल्य_final.ptr = tr->end_मूल्य.ptr;
                अन्यथा अगर(tr->end_मूल्य.ptr == शून्य) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा अगर(v < 128) मूल्य_final.ptr = tr->start_मूल्य.ptr;
                अन्यथा मूल्य_final.ptr = tr->end_मूल्य.ptr;
                अवरोध;
            /*Colors*/
            हाल LV_STYLE_RECOLOR:
            हाल LV_STYLE_BG_COLOR:
            हाल LV_STYLE_BG_GRAD_COLOR:
            हाल LV_STYLE_BORDER_COLOR:
            हाल LV_STYLE_TEXT_COLOR:
            हाल LV_STYLE_SHADOW_COLOR:
            हाल LV_STYLE_OUTLINE_COLOR:
            हाल LV_STYLE_IMAGE_RECOLOR:
                अगर(v <= 0) मूल्य_final.color = tr->start_मूल्य.color;
                अन्यथा अगर(v >= 255) मूल्य_final.color = tr->end_मूल्य.color;
                अन्यथा मूल्य_final.color = lv_color_mix(tr->end_मूल्य.color, tr->start_मूल्य.color, v);
                अवरोध;
            /*Opacities*/
            हाल LV_STYLE_OPA:
            हाल LV_STYLE_OPA_LAYERED:
            हाल LV_STYLE_RECOLOR_OPA:
            हाल LV_STYLE_COLOR_FILTER_OPA:
            हाल LV_STYLE_BG_OPA:
            हाल LV_STYLE_BG_GRAD_OPA:
            हाल LV_STYLE_BG_MAIN_OPA:
            हाल LV_STYLE_BG_IMAGE_OPA:
            हाल LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            हाल LV_STYLE_BORDER_OPA:
            हाल LV_STYLE_OUTLINE_OPA:
            हाल LV_STYLE_SHADOW_OPA:
            हाल LV_STYLE_पंक्ति_OPA:
            हाल LV_STYLE_ARC_OPA:
            हाल LV_STYLE_TEXT_OPA:
            हाल LV_STYLE_IMAGE_OPA:
            हाल LV_STYLE_IMAGE_RECOLOR_OPA:
            हाल LV_STYLE_DROP_SHADOW_OPA:
            हाल LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                अगर(v <= 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा अगर(v >= 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक =  LV_CLAMP(0, tr->start_मूल्य.अंक + ((tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v >> 8), 255);
                अवरोध;

            शेष:
                अगर(v == 0) मूल्य_final.अंक = tr->start_मूल्य.अंक;
                अन्यथा अगर(v == 255) मूल्य_final.अंक = tr->end_मूल्य.अंक;
                अन्यथा मूल्य_final.अंक = tr->start_मूल्य.अंक + ((int32_t)((int32_t)(tr->end_मूल्य.अंक - tr->start_मूल्य.अंक) * v) >> 8);
                अवरोध;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_मूल्य_t old_मूल्य = {0};
        अगर(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_मूल्य) == LV_STYLE_RES_FOUND) {
            अगर(मूल्य_final.ptr == old_मूल्य.ptr &&
               lv_color_eq(मूल्य_final.color, old_मूल्य.color) &&
               मूल्य_final.अंक == old_मूल्य.अंक) {
                अवरोध;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        अगर(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, मूल्य_final);

#अगर LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        अगर(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#पूर्ण_अगर

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        अवरोध;
    }

}

अटल व्योम trans_anim_start_cb(lv_anim_t * a)
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
#अगर LV_OBJ_STYLE_CACHE
    अगर(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    अन्यथा {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        अगर(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        अन्यथा {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#अन्यथा
    LV_UNUSED(trans_removed);
#पूर्ण_अगर

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

अटल व्योम trans_anim_completed_cb(lv_anim_t * a)
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
        अगर(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            अवरोध;
        }
    }

    अगर(!running) {
        uint32_t i;
        क्रम(i = 0; i < obj->style_cnt; i++) {
            अगर(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                अगर(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                अवरोध;
            }
        }
    }
}

अटल lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    अगर(lv_obj_get_transform(obj) != शून्य) वापस LV_LAYER_TYPE_TRANSFORM;
#पूर्ण_अगर
    अगर(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) वापस LV_LAYER_TYPE_TRANSFORM;
    अगर(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) वापस LV_LAYER_TYPE_TRANSFORM;
    अगर(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) वापस LV_LAYER_TYPE_TRANSFORM;
    अगर(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) वापस LV_LAYER_TYPE_TRANSFORM;
    अगर(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) वापस LV_LAYER_TYPE_TRANSFORM;
    अगर(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) वापस LV_LAYER_TYPE_SIMPLE;
    अगर(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != शून्य) वापस LV_LAYER_TYPE_SIMPLE;
    अगर(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) वापस LV_LAYER_TYPE_SIMPLE;
    वापस LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
अटल bool calculate_has_blur(lv_obj_t * obj)
{
    स्थिर uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    स्थिर uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    स्थिर lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    स्थिर lv_state_t state_inv = ~state;
    lv_style_मूल्य_t v;
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        अगर(obj_style->is_disabled) जारी;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        अगर((state_style & state_inv)) जारी;

        अगर((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            अगर(v.अंक > 0) वापस true;
        }
        अगर((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            अगर(v.अंक > 0) वापस true;
        }
    }

    वापस false;
}

अटल व्योम full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#अगर LV_OBJ_STYLE_CACHE
    uint32_t i;
    अगर(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        क्रम(i = 0; i < obj->style_cnt; i++) {
            अगर(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) जारी;
            अगर(obj->styles[i].is_disabled) जारी;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            अगर(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्यावली_and_props;
                क्रम(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * माप(lv_style_मूल्य_t);
                क्रम(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    अगर(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        क्रम(i = 0; i < obj->style_cnt; i++) {
            अगर(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) जारी;
            अगर(obj->styles[i].is_disabled) जारी;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            अगर(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->मूल्यावली_and_props;
                क्रम(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            अन्यथा {
                lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * माप(lv_style_मूल्य_t);
                क्रम(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(part);
#पूर्ण_अगर
}

अटल व्योम fade_anim_cb(व्योम * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

अटल व्योम fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

अटल bool style_has_flag(स्थिर lv_style_t * style, uint32_t flag)
{
    अगर(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->मूल्यावली_and_props;
        uint32_t i;
        क्रम(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            अगर(lv_style_prop_has_flag(props[i].prop, flag)) {
                वापस true;
            }
        }
    }
    अन्यथा {
        lv_style_prop_t * props = (lv_style_prop_t *)style->मूल्यावली_and_props + style->prop_cnt * माप(lv_style_मूल्य_t);
        uint32_t i;
        क्रम(i = 0; i < style->prop_cnt; i++) {
            अगर(lv_style_prop_has_flag(props[i], flag)) {
                वापस true;
            }
        }
    }
    वापस false;
}

अटल lv_style_res_t get_selector_style_prop(स्थिर lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_मूल्य_t * मूल्य_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#अगर LV_OBJ_STYLE_CACHE
    स्थिर uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    अगर((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#पूर्ण_अगर
    {
        found = get_prop_core(obj, selector, prop, मूल्य_act);
        अगर(found == LV_STYLE_RES_FOUND) वापस LV_STYLE_RES_FOUND;
    }

    बाह्य स्थिर uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    अगर(prop < LV_STYLE_अंक_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    अन्यथा {
        अगर(_style_custom_prop_flag_lookup_table != शून्य) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_अंक_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    अगर(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        अगर(part != LV_PART_MAIN) part = LV_PART_MAIN;
        अन्यथा obj = obj->parent;

        जबतक(obj) {
#अगर LV_OBJ_STYLE_CACHE
            अगर(obj->style_main_prop_is_set & prop_shifted)
#पूर्ण_अगर
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, मूल्य_act);
                अगर(found == LV_STYLE_RES_FOUND) वापस LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    अन्यथा {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        अगर(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            स्थिर lv_obj_class_t * cls = obj->class_p;
            जबतक(cls) {
                अगर(prop == LV_STYLE_WIDTH) {
                    अगर(cls->width_def != 0)  {
                        मूल्य_act->अंक = cls->width_def;
                        वापस LV_STYLE_RES_FOUND;
                    }
                }
                अन्यथा {
                    अगर(cls->height_def != 0) {
                        मूल्य_act->अंक = cls->height_def;
                        वापस LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    वापस LV_STYLE_RES_NOT_FOUND;
}

अटल व्योम remove_style_core(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != शून्य);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    अगर((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == शून्य || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_गिनती = 0;
    bool deleted = false;

    क्रम(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        अगर(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        अन्यथा अगर((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != शून्य && style != obj->styles[i].style)) {
            should_remove = false;
        }

        अगर(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            अगर(style_गिनती != i) {
                obj->styles[style_गिनती] = obj->styles[i];
            }
            style_गिनती++;
            जारी;
        }

        /*This entry is being removed*/
        अगर(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, शून्य);
        }
        अगर(obj->styles[i].is_local || obj->styles[i].is_trans) {
            अगर(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = शून्य;
        }
        deleted = true;
    }

    अगर(!deleted) {
        वापस;
    }

    obj->style_cnt = style_गिनती;
    अगर(style_गिनती > 0) {
        obj->styles = lv_realloc(obj->styles, style_गिनती * माप(lv_obj_style_t));
    }
    अन्यथा {
        lv_free(obj->styles);
        obj->styles = शून्य;
    }

    अगर(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

अटल lv_obj_style_t * find_style(lv_obj_t * obj, स्थिर lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    क्रम(i = 0; i < obj->style_cnt; i++) {
        अगर(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            वापस &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (व्योम *)style, (व्योम *)obj, selector);
    वापस शून्य;
}



#अगर LV_USE_OBSERVER

अटल व्योम bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_तथ्य(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->मूल्य);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

अटल व्योम bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_तथ्य(observer);

    lv_style_मूल्य_t style_v;
    अगर(subject->type == LV_SUBJECT_TYPE_INT) style_v.अंक = lv_subject_get_int(subject);
    अन्यथा अगर(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    अन्यथा अगर(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    अन्यथा {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        वापस;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#पूर्ण_अगर /*LV_USE_OBSERVER*/
