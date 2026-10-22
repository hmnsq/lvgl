/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "lv_obj_private.h"
#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "../misc/lv_anim_private.h"
#અન્તર્ભાવ "../misc/lv_bidi_private.h"
#અન્તર્ભાવ "../misc/lv_style_private.h"
#અન્તર્ભાવ "lv_obj_style_private.h"
#અન્તર્ભાવ "lv_obj_class_private.h"
#અન્તર્ભાવ "../display/lv_display_private.h"
#અન્તર્ભાવ "../core/lv_global.h"
#અન્તર્ભાવ "lv_observer_private.h"
#અન્તર્ભાવ "lv_obj_style_internal.h"

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

પ્રકારવ્યાખ્યા સંરચના {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_કિંમત_t start_કિંમત;
    lv_style_કિંમત_t end_કિંમત;
} trans_t;

પ્રકારવ્યાખ્યા સંરચના {
    અચલ lv_style_t * style;
    lv_style_selector_t selector;
    int32_t કિંમત;
} bind_style_t;

પ્રકારવ્યાખ્યા સંરચના {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
સ્થિર lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
સ્થિર lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
સ્થિર lv_style_res_t get_prop_core(અચલ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_કિંમત_t * v);
સ્થિર શૂન્ય report_style_change_core(શૂન્ય * style, lv_obj_t * obj);
સ્થિર શૂન્ય refresh_children_style(lv_obj_t * obj);
સ્થિર bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
સ્થિર શૂન્ય trans_anim_cb(શૂન્ય * _tr, int32_t v);
સ્થિર શૂન્ય trans_anim_start_cb(lv_anim_t * a);
સ્થિર શૂન્ય trans_anim_completed_cb(lv_anim_t * a);
સ્થિર lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
સ્થિર bool calculate_has_blur(lv_obj_t * obj);
સ્થિર શૂન્ય full_cache_refresh(lv_obj_t * obj, lv_part_t part);
સ્થિર શૂન્ય fade_anim_cb(શૂન્ય * obj, int32_t v);
સ્થિર શૂન્ય fade_in_anim_completed(lv_anim_t * a);
સ્થિર bool style_has_flag(અચલ lv_style_t * style, uint32_t flag);
સ્થિર lv_style_res_t get_selector_style_prop(અચલ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_કિંમત_t * કિંમત_act);
સ્થિર શૂન્ય remove_style_core(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector, bool theme_only);
સ્થિર lv_obj_style_t * find_style(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector);
#જો LV_USE_OBSERVER
    સ્થિર શૂન્ય bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    સ્થિર શૂન્ય bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
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

શૂન્ય lv_obj_style_init(શૂન્ય)
{
    lv_ll_init(style_trans_ll_p, કદ(trans_t));
}

શૂન્ય lv_obj_style_deinit(શૂન્ય)
{
    lv_ll_clear(style_trans_ll_p);
    જો(_style_custom_prop_flag_lookup_table != ખાલી) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = ખાલી;
    }
}

શૂન્ય lv_obj_add_style(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(style != ખાલી, પાછા);
    LV_CHECK_ARG_સંદેશ(obj->style_cnt < 63, પાછા,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    જો(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ખાલી);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_trans) ચાલુ;
        જો(obj->styles[i].is_local) ચાલુ;
        તોડો;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * કદ(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    માટે(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], કદ(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    જો(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#જો LV_OBJ_STYLE_CACHE
    નહીંતર {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        જો(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->કિંમતો_and_props;
            માટે(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        નહીંતર {
            lv_style_prop_t * props = (lv_style_prop_t *)style->કિંમતો_and_props + style->prop_cnt * કદ(lv_style_કિંમત_t);
            માટે(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, અચલ lv_style_t * old_style, અચલ lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);
    LV_CHECK_ARG(old_style != ખાલી, પાછા false);
    LV_CHECK_ARG(new_style != ખાલી, પાછા false);
    LV_CHECK_ARG(old_style != new_style, પાછા false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    જો((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ખાલી);

    bool replaced = false;
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        જો(obj->styles[i].is_local || obj->styles[i].is_trans) {
            ચાલુ;
        }

        /*Skip non-matching styles*/
        જો((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            ચાલુ;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], કદ(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    જો(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    પાછા replaced;
}

શૂન્ય lv_obj_remove_style(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    remove_style_core(obj, style, selector, false);
}

શૂન્ય lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    remove_style_core(obj, ખાલી, selector, true);
}

શૂન્ય lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    remove_style_core(obj, ખાલી, LV_PART_ANY | LV_STATE_ANY, false);
}

શૂન્ય lv_obj_report_style_change(lv_style_t * style)
{
    જો(!style_refr) પાછા;
    lv_display_t * d = lv_display_get_next(ખાલી);

    જ્યારે(d) {
        uint32_t i;
        માટે(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

શૂન્ય lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(!style_refr) પાછા;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    જો(is_layout_refr) {
        જો(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, ખાલી);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    જો((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        જો(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    જો((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    જો(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    જો(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    જો(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        જો(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

શૂન્ય lv_obj_set_style_enabled(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(style != ખાલી, પાછા);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    જો(obj_style == ખાલી) પાછા;

    જો(en != obj_style->is_disabled) પાછા; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    જો((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);
    LV_CHECK_ARG(style != ખાલી, પાછા false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    જો(obj_style == ખાલી) પાછા false;

    પાછા !obj_style->is_disabled;
}


શૂન્ય lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_કિંમત_t lv_obj_get_style_prop(અચલ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા lv_style_prop_get_default(prop));
    પાછા lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_કિંમત_t lv_obj_get_style_prop_internal(અચલ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != ખાલી);
    lv_style_selector_t selector = part | obj->state;
    lv_style_કિંમત_t કિંમત_act = { .ptr = ખાલી };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &કિંમત_act);
    જો(found == LV_STYLE_RES_FOUND) પાછા કિંમત_act;

    પાછા lv_style_prop_get_default(prop);
}
lv_style_કિંમત_t lv_obj_style_apply_color_filter_internal(અચલ lv_obj_t * obj, lv_part_t part, lv_style_કિંમત_t v)
{
#જો LV_USE_COLOR_FILTER
    LV_ASSERT(obj != ખાલી);

    અચલ lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    જો(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        જો(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#નહીંતર
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    પાછા v;
}

bool lv_obj_has_style_prop(અચલ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા false);

    lv_style_કિંમત_t કિંમત_act = { .ptr = ખાલી };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &કિંમત_act);
    જો(found == LV_STYLE_RES_FOUND) પાછા true;

    પાછા false;
}

શૂન્ય lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_કિંમત_t કિંમત,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    જો(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, ખાલી);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, કિંમત);

    જો(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#જો LV_OBJ_STYLE_CACHE
    નહીંતર {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        જો(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        નહીંતર {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_કિંમત_t * કિંમત,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(કિંમત != ખાલી, પાછા LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            પાછા lv_style_get_prop_internal(obj->styles[i].style, prop, કિંમત);
        }
    }

    પાછા LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા 0);

    uint32_t i;
    /*Find the style*/
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            તોડો;
        }
    }

    /*The style is not found*/
    જો(i == obj->style_cnt) પાછા false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    જો(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    પાછા res;
}

શૂન્ય lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    અચલ lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(tr_dsc != ખાલી, પાછા);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_કિંમત_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_કિંમત_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    જો(v1.ptr == v2.ptr && v1.અંક == v2.અંક && lv_color_eq(v1.color, v2.color))  પાછા;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    જો(tr_dsc->prop == LV_STYLE_RADIUS) {
        જો(v1.અંક == LV_RADIUS_CIRCLE || v2.અંક == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            જો(v1.અંક == LV_RADIUS_CIRCLE) v1.અંક = LV_MIN(whalf + 1, hhalf + 1);
            જો(v2.અંક == LV_RADIUS_CIRCLE) v2.અંક = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    જો(tr == ખાલી) પાછા;
    tr->start_કિંમત = v1;
    tr->end_કિંમત = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_કિંમતો(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->time);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_કિંમત_t lv_obj_style_apply_color_filter(અચલ lv_obj_t * obj, lv_part_t part, lv_style_કિંમત_t v)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા v);
    પાછા lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_STYLE_STATE_CMP_SAME);

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
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_trans) ચાલુ;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        જો(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            જો(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            નહીંતર જો(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            અચલ lv_style_t * style = obj->styles[i].style;
            lv_style_કિંમત_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_ટોચ, &v))layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_ડાબું, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_જમણું, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            જો(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                જો(changed_part) *changed_part = LV_PART_ANY;
                પાછા LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(lv_style_get_prop_internal(style, LV_STYLE_લીટી_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            નહીંતર જો(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    જો(changed_part) *changed_part = diff_part;
    પાછા res;
}

શૂન્ય lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_કિંમતો(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

શૂન્ય lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_કિંમતો(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(અચલ lv_obj_t * obj, lv_part_t part, અચલ અક્ષર * txt)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != ખાલી, પાછા LV_TEXT_ALIGN_AUTO);
    પાછા lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    જો(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        પાછા LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    જો(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    જો(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    નહીંતર {
        obj = lv_obj_get_parent(obj);
    }

    જ્યારે(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        જો(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            પાછા LV_OPA_TRANSP;
        }
        જો(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    જો(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        પાછા LV_OPA_TRANSP;
    }

    જો(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        પાછા LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    પાછા opa_final;
}

શૂન્ય lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    જો(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    નહીંતર જો(layer_type != LV_LAYER_TYPE_NONE) {
        જો(!lv_obj_allocate_spec_attr(obj)) {
            પાછા;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

શૂન્ય lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    જો(obj->is_deleting) પાછા;

    bool has_blur = calculate_has_blur(obj);
    જો(has_blur == (bool)obj->has_blur) પાછા;

    obj->has_blur = has_blur;
    જો(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    નહીંતર {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(અચલ lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    જો(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    પાછા color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    જો(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    નહીંતર {
        obj = lv_obj_get_parent(obj);
    }

    જ્યારે(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    પાછા result;
}

શૂન્ય lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_pad_ડાબું(obj, કિંમત, selector);
    lv_obj_set_style_pad_જમણું(obj, કિંમત, selector);
    lv_obj_set_style_pad_ટોચ(obj, કિંમત, selector);
    lv_obj_set_style_pad_bottom(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_pad_ડાબું(obj, કિંમત, selector);
    lv_obj_set_style_pad_જમણું(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_pad_ટોચ(obj, કિંમત, selector);
    lv_obj_set_style_pad_bottom(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_margin_ડાબું(obj, કિંમત, selector);
    lv_obj_set_style_margin_જમણું(obj, કિંમત, selector);
    lv_obj_set_style_margin_ટોચ(obj, કિંમત, selector);
    lv_obj_set_style_margin_bottom(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_margin_ડાબું(obj, કિંમત, selector);
    lv_obj_set_style_margin_જમણું(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_margin_ટોચ(obj, કિંમત, selector);
    lv_obj_set_style_margin_bottom(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_pad_row(obj, કિંમત, selector);
    lv_obj_set_style_pad_column(obj, કિંમત, selector);
}

શૂન્ય lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

શૂન્ય lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t કિંમત, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા);
    lv_obj_set_style_transform_scale_x(obj, કિંમત, selector);
    lv_obj_set_style_transform_scale_y(obj, કિંમત, selector);
}

int32_t lv_obj_get_style_space_ડાબું(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_space_ડાબું_internal(obj, part);
}

int32_t lv_obj_get_style_space_જમણું(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_space_જમણું_internal(obj, part);
}

int32_t lv_obj_get_style_space_ટોચ(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_space_ટોચ_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, પાછા 0)
    પાછા lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#જો LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_કિંમત)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(style != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);

    જો(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        પાછા ખાલી;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(કદ(bind_style_t));
    LV_ASSERT_MALLOC(p);
    જો(p == ખાલી) {
        LV_LOG_WARN("Out of memory");
        પાછા ખાલી;
    }

    p->style = style;
    p->selector = selector;
    p->કિંમત = ref_કિંમત;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    પાછા observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(subject != ખાલી, પાછા ખાલી);

    જો(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        પાછા ખાલી;
    }

    bind_style_prop_t * p = lv_malloc(કદ(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    જો(p == ખાલી) {
        LV_LOG_WARN("Out of memory");
        પાછા ખાલી;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    પાછા observable;
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
સ્થિર lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            પાછા (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(કદ(lv_style_t));
    જો(new_style == ખાલી) {
        LV_LOG_WARN("couldn't allocate local style");
        પાછા ખાલી;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * કદ(lv_obj_style_t));
    જો(new_styles == ખાલી) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        પાછા ખાલી;
    }
    obj->styles = new_styles;

    માટે(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        જો(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) તોડો;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], કદ(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    પાછા (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
સ્થિર lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_trans && obj->styles[i].selector == selector) તોડો;
    }

    /*Already have a transition style for it*/
    જો(i != obj->style_cnt) પાછા &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(કદ(lv_style_t));
    જો(new_style == ખાલી) {
        LV_LOG_WARN("couldn't allocate transition style");
        પાછા ખાલી;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * કદ(lv_obj_style_t));
    જો(new_styles == ખાલી) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        પાછા ખાલી;
    }
    obj->styles = new_styles;

    માટે(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], કદ(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    પાછા &obj->styles[0];
}

સ્થિર lv_style_res_t get_prop_core(અચલ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_કિંમત_t * v)
{

    અચલ uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    અચલ lv_part_t part = lv_obj_style_get_selector_part(selector);
    અચલ bool skip_trans = (અચલ bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    માટે(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        જો(obj_style->is_trans == false) તોડો;
        જો(skip_trans) ચાલુ;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        જો(part_act != part) ચાલુ;
        જો((obj_style->style->has_group & group) == 0) ચાલુ;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        જો(found == LV_STYLE_RES_FOUND) {
            પાછા LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    અચલ lv_state_t state = lv_obj_style_get_selector_state(selector);
    અચલ lv_state_t state_inv = ~state;
    માટે(; i < obj->style_cnt; i++) {
        જો((obj->styles[i].style->has_group & group) == 0) ચાલુ;
        જો(obj->styles[i].is_disabled) ચાલુ;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        જો(part_act != part) ચાલુ;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        જો((state_act & state_inv)) ચાલુ;

        /*Check only better candidates*/
        જો((int32_t)state_act <= weight) ચાલુ;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        જો(found == LV_STYLE_RES_FOUND) {
            જો(state_act == state) {
                પાછા LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    જો(weight >= 0) પાછા LV_STYLE_RES_FOUND;
    નહીંતર પાછા LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
સ્થિર શૂન્ય report_style_change_core(શૂન્ય * style, lv_obj_t * obj)
{
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(style == ખાલી || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            તોડો;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_ગણતરી(obj);
    માટે(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
સ્થિર શૂન્ય refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ગણતરી(obj);
    માટે(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, ખાલી);
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
સ્થિર bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    જ્યારે(tr != ખાલી) {
        જો(tr == tr_limit) તોડો;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        જો(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            જો(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            માટે(i = 0; i < obj->style_cnt; i++) {
                જો(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, ખાલી);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    પાછા removed;
}

સ્થિર શૂન્ય trans_anim_cb(શૂન્ય * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) ચાલુ;

        lv_style_કિંમત_t કિંમત_final = {0};
        પસંદગી(tr->prop) {

            સ્થિતિ LV_STYLE_BORDER_SIDE:
            સ્થિતિ LV_STYLE_BORDER_POST:
            સ્થિતિ LV_STYLE_BLEND_MODE:
                જો(v < 255) કિંમત_final.અંક = tr->start_કિંમત.અંક;
                નહીંતર કિંમત_final.અંક = tr->end_કિંમત.અંક;
                તોડો;
            સ્થિતિ LV_STYLE_TRANSITION:
            સ્થિતિ LV_STYLE_TEXT_FONT:
            સ્થિતિ LV_STYLE_BG_IMAGE_SRC:
                જો(v < 255) કિંમત_final.ptr = tr->start_કિંમત.ptr;
                નહીંતર કિંમત_final.ptr = tr->end_કિંમત.ptr;
                તોડો;
            સ્થિતિ LV_STYLE_COLOR_FILTER_DSC:
                જો(tr->start_કિંમત.ptr == ખાલી) કિંમત_final.ptr = tr->end_કિંમત.ptr;
                નહીંતર જો(tr->end_કિંમત.ptr == ખાલી) કિંમત_final.ptr = tr->start_કિંમત.ptr;
                નહીંતર જો(v < 128) કિંમત_final.ptr = tr->start_કિંમત.ptr;
                નહીંતર કિંમત_final.ptr = tr->end_કિંમત.ptr;
                તોડો;
            /*Colors*/
            સ્થિતિ LV_STYLE_RECOLOR:
            સ્થિતિ LV_STYLE_BG_COLOR:
            સ્થિતિ LV_STYLE_BG_GRAD_COLOR:
            સ્થિતિ LV_STYLE_BORDER_COLOR:
            સ્થિતિ LV_STYLE_TEXT_COLOR:
            સ્થિતિ LV_STYLE_SHADOW_COLOR:
            સ્થિતિ LV_STYLE_OUTLINE_COLOR:
            સ્થિતિ LV_STYLE_IMAGE_RECOLOR:
                જો(v <= 0) કિંમત_final.color = tr->start_કિંમત.color;
                નહીંતર જો(v >= 255) કિંમત_final.color = tr->end_કિંમત.color;
                નહીંતર કિંમત_final.color = lv_color_mix(tr->end_કિંમત.color, tr->start_કિંમત.color, v);
                તોડો;
            /*Opacities*/
            સ્થિતિ LV_STYLE_OPA:
            સ્થિતિ LV_STYLE_OPA_LAYERED:
            સ્થિતિ LV_STYLE_RECOLOR_OPA:
            સ્થિતિ LV_STYLE_COLOR_FILTER_OPA:
            સ્થિતિ LV_STYLE_BG_OPA:
            સ્થિતિ LV_STYLE_BG_GRAD_OPA:
            સ્થિતિ LV_STYLE_BG_MAIN_OPA:
            સ્થિતિ LV_STYLE_BG_IMAGE_OPA:
            સ્થિતિ LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            સ્થિતિ LV_STYLE_BORDER_OPA:
            સ્થિતિ LV_STYLE_OUTLINE_OPA:
            સ્થિતિ LV_STYLE_SHADOW_OPA:
            સ્થિતિ LV_STYLE_લીટી_OPA:
            સ્થિતિ LV_STYLE_ARC_OPA:
            સ્થિતિ LV_STYLE_TEXT_OPA:
            સ્થિતિ LV_STYLE_IMAGE_OPA:
            સ્થિતિ LV_STYLE_IMAGE_RECOLOR_OPA:
            સ્થિતિ LV_STYLE_DROP_SHADOW_OPA:
            સ્થિતિ LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                જો(v <= 0) કિંમત_final.અંક = tr->start_કિંમત.અંક;
                નહીંતર જો(v >= 255) કિંમત_final.અંક = tr->end_કિંમત.અંક;
                નહીંતર કિંમત_final.અંક =  LV_CLAMP(0, tr->start_કિંમત.અંક + ((tr->end_કિંમત.અંક - tr->start_કિંમત.અંક) * v >> 8), 255);
                તોડો;

            મૂળભૂત:
                જો(v == 0) કિંમત_final.અંક = tr->start_કિંમત.અંક;
                નહીંતર જો(v == 255) કિંમત_final.અંક = tr->end_કિંમત.અંક;
                નહીંતર કિંમત_final.અંક = tr->start_કિંમત.અંક + ((int32_t)((int32_t)(tr->end_કિંમત.અંક - tr->start_કિંમત.અંક) * v) >> 8);
                તોડો;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_કિંમત_t old_કિંમત = {0};
        જો(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_કિંમત) == LV_STYLE_RES_FOUND) {
            જો(કિંમત_final.ptr == old_કિંમત.ptr &&
               lv_color_eq(કિંમત_final.color, old_કિંમત.color) &&
               કિંમત_final.અંક == old_કિંમત.અંક) {
                તોડો;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        જો(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, કિંમત_final);

#જો LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        જો(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        નહીંતર {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        તોડો;
    }

}

સ્થિર શૂન્ય trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_કિંમત = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_કિંમત);
#જો LV_OBJ_STYLE_CACHE
    જો(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    નહીંતર {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        જો(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        નહીંતર {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#નહીંતર
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

સ્થિર શૂન્ય trans_anim_completed_cb(lv_anim_t * a)
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
        જો(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            તોડો;
        }
    }

    જો(!running) {
        uint32_t i;
        માટે(i = 0; i < obj->style_cnt; i++) {
            જો(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                જો(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                તોડો;
            }
        }
    }
}

સ્થિર lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#જો LV_DRAW_TRANSFORM_USE_MATRIX
    જો(lv_obj_get_transform(obj) != ખાલી) પાછા LV_LAYER_TYPE_TRANSFORM;
#endif
    જો(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) પાછા LV_LAYER_TYPE_TRANSFORM;
    જો(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) પાછા LV_LAYER_TYPE_TRANSFORM;
    જો(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) પાછા LV_LAYER_TYPE_TRANSFORM;
    જો(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) પાછા LV_LAYER_TYPE_TRANSFORM;
    જો(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) પાછા LV_LAYER_TYPE_TRANSFORM;
    જો(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) પાછા LV_LAYER_TYPE_SIMPLE;
    જો(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != ખાલી) પાછા LV_LAYER_TYPE_SIMPLE;
    જો(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) પાછા LV_LAYER_TYPE_SIMPLE;
    પાછા LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
સ્થિર bool calculate_has_blur(lv_obj_t * obj)
{
    અચલ uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    અચલ uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    અચલ lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    અચલ lv_state_t state_inv = ~state;
    lv_style_કિંમત_t v;
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        જો(obj_style->is_disabled) ચાલુ;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        જો((state_style & state_inv)) ચાલુ;

        જો((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            જો(v.અંક > 0) પાછા true;
        }
        જો((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            જો(v.અંક > 0) પાછા true;
        }
    }

    પાછા false;
}

સ્થિર શૂન્ય full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#જો LV_OBJ_STYLE_CACHE
    uint32_t i;
    જો(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        માટે(i = 0; i < obj->style_cnt; i++) {
            જો(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) ચાલુ;
            જો(obj->styles[i].is_disabled) ચાલુ;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            જો(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->કિંમતો_and_props;
                માટે(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            નહીંતર {
                lv_style_prop_t * props = (lv_style_prop_t *)style->કિંમતો_and_props + style->prop_cnt * કદ(lv_style_કિંમત_t);
                માટે(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    જો(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        માટે(i = 0; i < obj->style_cnt; i++) {
            જો(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) ચાલુ;
            જો(obj->styles[i].is_disabled) ચાલુ;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            જો(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->કિંમતો_and_props;
                માટે(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            નહીંતર {
                lv_style_prop_t * props = (lv_style_prop_t *)style->કિંમતો_and_props + style->prop_cnt * કદ(lv_style_કિંમત_t);
                માટે(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#નહીંતર
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

સ્થિર શૂન્ય fade_anim_cb(શૂન્ય * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

સ્થિર શૂન્ય fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

સ્થિર bool style_has_flag(અચલ lv_style_t * style, uint32_t flag)
{
    જો(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->કિંમતો_and_props;
        uint32_t i;
        માટે(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            જો(lv_style_prop_has_flag(props[i].prop, flag)) {
                પાછા true;
            }
        }
    }
    નહીંતર {
        lv_style_prop_t * props = (lv_style_prop_t *)style->કિંમતો_and_props + style->prop_cnt * કદ(lv_style_કિંમત_t);
        uint32_t i;
        માટે(i = 0; i < style->prop_cnt; i++) {
            જો(lv_style_prop_has_flag(props[i], flag)) {
                પાછા true;
            }
        }
    }
    પાછા false;
}

સ્થિર lv_style_res_t get_selector_style_prop(અચલ lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_કિંમત_t * કિંમત_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#જો LV_OBJ_STYLE_CACHE
    અચલ uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    જો((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, કિંમત_act);
        જો(found == LV_STYLE_RES_FOUND) પાછા LV_STYLE_RES_FOUND;
    }

    બાહ્ય અચલ uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    જો(prop < LV_STYLE_અંક_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    નહીંતર {
        જો(_style_custom_prop_flag_lookup_table != ખાલી) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_અંક_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    જો(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        જો(part != LV_PART_MAIN) part = LV_PART_MAIN;
        નહીંતર obj = obj->parent;

        જ્યારે(obj) {
#જો LV_OBJ_STYLE_CACHE
            જો(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, કિંમત_act);
                જો(found == LV_STYLE_RES_FOUND) પાછા LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    નહીંતર {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        જો(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            અચલ lv_obj_class_t * cls = obj->class_p;
            જ્યારે(cls) {
                જો(prop == LV_STYLE_WIDTH) {
                    જો(cls->width_def != 0)  {
                        કિંમત_act->અંક = cls->width_def;
                        પાછા LV_STYLE_RES_FOUND;
                    }
                }
                નહીંતર {
                    જો(cls->height_def != 0) {
                        કિંમત_act->અંક = cls->height_def;
                        પાછા LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    પાછા LV_STYLE_RES_NOT_FOUND;
}

સ્થિર શૂન્ય remove_style_core(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != ખાલી);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    જો((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == ખાલી || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ગણતરી = 0;
    bool deleted = false;

    માટે(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        જો(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        નહીંતર જો((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != ખાલી && style != obj->styles[i].style)) {
            should_remove = false;
        }

        જો(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            જો(style_ગણતરી != i) {
                obj->styles[style_ગણતરી] = obj->styles[i];
            }
            style_ગણતરી++;
            ચાલુ;
        }

        /*This entry is being removed*/
        જો(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, ખાલી);
        }
        જો(obj->styles[i].is_local || obj->styles[i].is_trans) {
            જો(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = ખાલી;
        }
        deleted = true;
    }

    જો(!deleted) {
        પાછા;
    }

    obj->style_cnt = style_ગણતરી;
    જો(style_ગણતરી > 0) {
        obj->styles = lv_realloc(obj->styles, style_ગણતરી * કદ(lv_obj_style_t));
    }
    નહીંતર {
        lv_free(obj->styles);
        obj->styles = ખાલી;
    }

    જો(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

સ્થિર lv_obj_style_t * find_style(lv_obj_t * obj, અચલ lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    માટે(i = 0; i < obj->style_cnt; i++) {
        જો(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            પાછા &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (શૂન્ય *)style, (શૂન્ય *)obj, selector);
    પાછા ખાલી;
}



#જો LV_USE_OBSERVER

સ્થિર શૂન્ય bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->કિંમત);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

સ્થિર શૂન્ય bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_કિંમત_t style_v;
    જો(subject->type == LV_SUBJECT_TYPE_INT) style_v.અંક = lv_subject_get_int(subject);
    નહીંતર જો(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    નહીંતર જો(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    નહીંતર {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        પાછા;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
