/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj_private.h"
#include "../lvgl_public.h"
#include "../misc/lv_anim_private.h"
#include "../misc/lv_bidi_private.h"
#include "../misc/lv_style_private.h"
#include "lv_obj_style_private.h"
#include "lv_obj_class_private.h"
#include "../display/lv_display_private.h"
#include "../core/lv_global.h"
#include "lv_observer_private.h"
#include "lv_obj_style_internal.h"

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

typedef struct {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_value_t start_value;
    lv_style_value_t end_value;
} trans_t;

typedef struct {
    const lv_style_t * style;
    lv_style_selector_t selector;
    int32_t value;
} bind_style_t;

typedef struct {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
static lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
static lv_style_res_t get_prop_core(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_value_t * v);
static void report_style_change_core(void * style, lv_obj_t * obj);
static void refresh_children_style(lv_obj_t * obj);
static bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit);
static void trans_anim_cb(void * _tr, int32_t v);
static void trans_anim_start_cb(lv_anim_t * a);
static void trans_anim_completed_cb(lv_anim_t * a);
static lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
static bool calculate_has_blur(lv_obj_t * obj);
static void full_cache_refresh(lv_obj_t * obj, lv_part_t part);
static void fade_anim_cb(void * obj, int32_t v);
static void fade_in_anim_completed(lv_anim_t * a);
static bool style_has_flag(const lv_style_t * style, uint32_t flag);
static lv_style_res_t get_selector_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_value_t * value_act);
static void remove_style_core(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector, bool theme_only);
static lv_obj_style_t * find_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector);
#якщо LV_USE_OBSERVER
    static void bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    static void bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
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

void lv_obj_style_init(void)
{
    lv_ll_init(style_trans_ll_p, sizeof(trans_t));
}

void lv_obj_style_deinit(void)
{
    lv_ll_clear(style_trans_ll_p);
    якщо(_style_custom_prop_flag_lookup_table != NULL) {
        lv_free(_style_custom_prop_flag_lookup_table);
        _style_custom_prop_flag_lookup_table = NULL;
    }
}

void lv_obj_add_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    LV_CHECK_ARG(style != NULL, повернути);
    LV_CHECK_ARG_MSG(obj->style_cnt < 63, повернути,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    якщо(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, NULL);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_trans) continue;
        якщо(obj->styles[i].is_local) continue;
        break;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * sizeof(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    для(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], sizeof(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    якщо(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#якщо LV_OBJ_STYLE_CACHE
    інакше {
        uint32_t * prop_is_set = part == LV_PART_MAIN ? &obj->style_main_prop_is_set : &obj->style_other_prop_is_set;
        якщо(lv_style_is_const_internal(style)) {
            lv_style_const_prop_t * props = style->values_and_props;
            для(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        інакше {
            lv_style_prop_t * props = (lv_style_prop_t *)style->values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
            для(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_set) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#endif

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, const lv_style_t * old_style, const lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути false);
    LV_CHECK_ARG(old_style != NULL, повернути false);
    LV_CHECK_ARG(new_style != NULL, повернути false);
    LV_CHECK_ARG(old_style != new_style, повернути false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    якщо((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, NULL);

    bool replaced = false;
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        якщо(obj->styles[i].is_local || obj->styles[i].is_trans) {
            continue;
        }

        /*Skip non-matching styles*/
        якщо((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            continue;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], sizeof(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    якщо(replaced || trans_removed) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    повернути replaced;
}

void lv_obj_remove_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    remove_style_core(obj, style, selector, false);
}

void lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    remove_style_core(obj, NULL, selector, true);
}

void lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    remove_style_core(obj, NULL, LV_PART_ANY | LV_STATE_ANY, false);
}

void lv_obj_report_style_change(lv_style_t * style)
{
    якщо(!style_refr) повернути;
    lv_display_t * d = lv_display_get_next(NULL);

    поки(d) {
        uint32_t i;
        для(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_next(d);
    }
}

void lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, повернути);

    якщо(!style_refr) повернути;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    якщо(is_layout_refr) {
        якщо(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_height_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT ||
           lv_obj_get_style_width_internal(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_CHANGED, NULL);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    якщо((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        якщо(parent) lv_obj_mark_layout_as_dirty(parent);
    }

    /*Cache the layer type*/
    якщо((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    якщо(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_status(obj);
    }

    якщо(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_size(obj);
    }
    lv_obj_invalidate(obj);

    якщо(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        якщо(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_END;
}

void lv_obj_set_style_enabled(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    LV_CHECK_ARG(style != NULL, повернути);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    якщо(obj_style == NULL) повернути;

    якщо(en != obj_style->is_disabled) повернути; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    якщо((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    full_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути false);
    LV_CHECK_ARG(style != NULL, повернути false);

    lv_obj_style_t * obj_style = find_style(obj, style, selector);
    якщо(obj_style == NULL) повернути false;

    повернути !obj_style->is_disabled;
}


void lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_value_t lv_obj_get_style_prop(const lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != NULL, повернути lv_style_prop_get_default(prop));
    повернути lv_obj_get_style_prop_internal(obj, part, prop);
}

lv_style_value_t lv_obj_get_style_prop_internal(const lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != NULL);
    lv_style_selector_t selector = part | obj->state;
    lv_style_value_t value_act = { .ptr = NULL };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &value_act);
    якщо(found == LV_STYLE_RES_FOUND) повернути value_act;

    повернути lv_style_prop_get_default(prop);
}
lv_style_value_t lv_obj_style_apply_color_filter_internal(const lv_obj_t * obj, lv_part_t part, lv_style_value_t v)
{
#якщо LV_USE_COLOR_FILTER
    LV_ASSERT(obj != NULL);

    const lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_internal(obj, part);
    якщо(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_internal(obj, part);
        якщо(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#інакше
    LV_UNUSED(obj);
    LV_UNUSED(part);
    LV_UNUSED(v);
#endif
    повернути v;
}

bool lv_obj_has_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != NULL, повернути false);

    lv_style_value_t value_act = { .ptr = NULL };
    lv_style_res_t found;

    found = get_selector_style_prop(obj, selector, prop, &value_act);
    якщо(found == LV_STYLE_RES_FOUND) повернути true;

    повернути false;
}

void lv_obj_set_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t value,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    якщо(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, NULL);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_set_prop(style, prop, value);

    якщо(trans_removed) {
        full_cache_refresh(obj, LV_PART_ANY);
    }
#якщо LV_OBJ_STYLE_CACHE
    інакше {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        якщо(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        інакше {
            obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#endif

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_END;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_value_t * value,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_STYLE_RES_NOT_FOUND);
    LV_CHECK_ARG(value != NULL, повернути LV_STYLE_RES_NOT_FOUND);

    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            повернути lv_style_get_prop_internal(obj->styles[i].style, prop, value);
        }
    }

    повернути LV_STYLE_RES_NOT_FOUND;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, повернути 0);

    uint32_t i;
    /*Find the style*/
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            break;
        }
    }

    /*The style is not found*/
    якщо(i == obj->style_cnt) повернути false;

    lv_result_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    якщо(res == LV_RESULT_OK) {
        full_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    повернути res;
}

void lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    const lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    LV_CHECK_ARG(tr_dsc != NULL, повернути);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_value_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_value_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    якщо(v1.ptr == v2.ptr && v1.num == v2.num && lv_color_eq(v1.color, v2.color))  повернути;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_set_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    якщо(tr_dsc->prop == LV_STYLE_RADIUS) {
        якщо(v1.num == LV_RADIUS_CIRCLE || v2.num == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_width(obj) / 2;
            int32_t hhalf = lv_obj_get_height(obj) / 2;
            якщо(v1.num == LV_RADIUS_CIRCLE) v1.num = LV_MIN(whalf + 1, hhalf + 1);
            якщо(v2.num == LV_RADIUS_CIRCLE) v2.num = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_head(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    якщо(tr == NULL) повернути;
    tr->start_value = v1;
    tr->end_value = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, tr);
    lv_anim_set_exec_cb(&a, trans_anim_cb);
    lv_anim_set_start_cb(&a, trans_anim_start_cb);
    lv_anim_set_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_set_values(&a, 0x00, 0xFF);
    lv_anim_set_duration(&a, tr_dsc->time);
    lv_anim_set_delay(&a, tr_dsc->delay);
    lv_anim_set_path_cb(&a, tr_dsc->path_cb);
    lv_anim_set_early_apply(&a, false);
    lv_anim_set_user_data(&a, tr_dsc->user_data);
    lv_anim_start(&a);
}

lv_style_value_t lv_obj_style_apply_color_filter(const lv_obj_t * obj, lv_part_t part, lv_style_value_t v)
{
    LV_CHECK_ARG(obj != NULL, повернути v);
    повернути lv_obj_style_apply_color_filter_internal(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * changed_part)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_STYLE_STATE_CMP_SAME);

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
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_trans) continue;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        якщо(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            якщо(!diff_part_found) {
                diff_part = part_act;
                diff_part_found = true;
            }
            інакше якщо(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            const lv_style_t * style = obj->styles[i].style;
            lv_style_value_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_TOP, &v))layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_LEFT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_RIGHT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_COLUMN, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_PAD_ROW, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_WIDTH, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_HEIGHT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_MIN_WIDTH, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_MAX_WIDTH, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_MIN_HEIGHT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_MAX_HEIGHT, &v)) layout_diff = true;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_BORDER_WIDTH, &v)) layout_diff = true;

            якщо(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                якщо(changed_part) *changed_part = LV_PART_ANY;
                повернути LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_HEIGHT, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_OUTLINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_OFFSET_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_SHADOW_SPREAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(lv_style_get_prop_internal(style, LV_STYLE_LINE_WIDTH, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            інакше якщо(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    якщо(changed_part) *changed_part = diff_part;
    повернути res;
}

void lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 0, LV_OPA_COVER);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_completed_cb(&a, fade_in_anim_completed);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

void lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, lv_obj_get_style_opa_internal(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_set_exec_cb(&a, fade_anim_cb);
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
}

lv_text_align_t lv_obj_calculate_style_text_align(const lv_obj_t * obj, lv_part_t part, const char * txt)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_TEXT_ALIGN_AUTO);
    LV_CHECK_ARG(txt != NULL, повернути LV_TEXT_ALIGN_AUTO);
    повернути lv_obj_calculate_style_text_align_internal(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_internal(obj, part);
    якщо(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        повернути LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    якщо(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    якщо(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    інакше {
        obj = lv_obj_get_parent(obj);
    }

    поки(obj) {
        opa_obj = lv_obj_get_style_opa_internal(obj, part);
        якщо(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_END;
            повернути LV_OPA_TRANSP;
        }
        якщо(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_parent(obj);
    }

    якщо(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_END;
        повернути LV_OPA_TRANSP;
    }

    якщо(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_END;
        повернути LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_END;
    повернути opa_final;
}

void lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, повернути);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    якщо(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    інакше якщо(layer_type != LV_LAYER_TYPE_NONE) {
        якщо(!lv_obj_allocate_spec_attr(obj)) {
            повернути;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

void lv_obj_update_blur_status(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    якщо(obj->is_deleting) повернути;

    bool has_blur = calculate_has_blur(obj);
    якщо(has_blur == (bool)obj->has_blur) повернути;

    obj->has_blur = has_blur;
    якщо(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    інакше {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(const lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != NULL, повернути (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);
    якщо(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_internal(obj, part);
        color = lv_color_over32(color, lv_color_to_32(recolor, opa));
    }

    повернути color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != NULL, повернути (lv_color32_t) {
        0
    });

    lv_color32_t result;

    lv_color_t color = lv_obj_get_style_recolor_internal(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_internal(obj, part);

    result = lv_color_to_32(color, opa);

    якщо(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    інакше {
        obj = lv_obj_get_parent(obj);
    }

    поки(obj) {
        result = lv_obj_style_apply_recolor(obj, part, result);
        obj = lv_obj_get_parent(obj);
    }

    повернути result;
}

void lv_obj_set_style_pad_all(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_pad_left(obj, value, selector);
    lv_obj_set_style_pad_right(obj, value, selector);
    lv_obj_set_style_pad_top(obj, value, selector);
    lv_obj_set_style_pad_bottom(obj, value, selector);
}

void lv_obj_set_style_pad_hor(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_pad_left(obj, value, selector);
    lv_obj_set_style_pad_right(obj, value, selector);
}

void lv_obj_set_style_pad_ver(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_pad_top(obj, value, selector);
    lv_obj_set_style_pad_bottom(obj, value, selector);
}

void lv_obj_set_style_margin_all(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_margin_left(obj, value, selector);
    lv_obj_set_style_margin_right(obj, value, selector);
    lv_obj_set_style_margin_top(obj, value, selector);
    lv_obj_set_style_margin_bottom(obj, value, selector);
}

void lv_obj_set_style_margin_hor(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_margin_left(obj, value, selector);
    lv_obj_set_style_margin_right(obj, value, selector);
}

void lv_obj_set_style_margin_ver(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_margin_top(obj, value, selector);
    lv_obj_set_style_margin_bottom(obj, value, selector);
}

void lv_obj_set_style_pad_gap(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_pad_row(obj, value, selector);
    lv_obj_set_style_pad_column(obj, value, selector);
}

void lv_obj_set_style_size(lv_obj_t * obj, int32_t width, int32_t height, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_width(obj, width, selector);
    lv_obj_set_style_height(obj, height, selector);
}

void lv_obj_set_style_transform_scale(lv_obj_t * obj, int32_t value, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути);
    lv_obj_set_style_transform_scale_x(obj, value, selector);
    lv_obj_set_style_transform_scale_y(obj, value, selector);
}

int32_t lv_obj_get_style_space_left(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_space_left_internal(obj, part);
}

int32_t lv_obj_get_style_space_right(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_space_right_internal(obj, part);
}

int32_t lv_obj_get_style_space_top(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_space_top_internal(obj, part);
}

int32_t lv_obj_get_style_space_bottom(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_space_bottom_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_safe(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_transform_scale_x_safe_internal(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_safe(const lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, повернути 0)
    повернути lv_obj_get_style_transform_scale_y_safe_internal(obj, part);
}


#якщо LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_value)
{
    LV_CHECK_ARG(obj != NULL, повернути NULL);
    LV_CHECK_ARG(style != NULL, повернути NULL);
    LV_CHECK_ARG(subject != NULL, повернути NULL);

    якщо(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        повернути NULL;
    }

    lv_obj_add_style(obj, style, selector);

    bind_style_t * p = lv_malloc(sizeof(bind_style_t));
    LV_ASSERT_MALLOC(p);
    якщо(p == NULL) {
        LV_LOG_WARN("Out of memory");
        повернути NULL;
    }

    p->style = style;
    p->selector = selector;
    p->value = ref_value;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    повернути observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != NULL, повернути NULL);
    LV_CHECK_ARG(subject != NULL, повернути NULL);

    якщо(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        повернути NULL;
    }

    bind_style_prop_t * p = lv_malloc(sizeof(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    якщо(p == NULL) {
        LV_LOG_WARN("Out of memory");
        повернути NULL;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_data = 1;
    повернути observable;
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
static lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            повернути (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(sizeof(lv_style_t));
    якщо(new_style == NULL) {
        LV_LOG_WARN("couldn't allocate local style");
        повернути NULL;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * sizeof(lv_obj_style_t));
    якщо(new_styles == NULL) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        повернути NULL;
    }
    obj->styles = new_styles;

    для(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        якщо(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) break;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], sizeof(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    повернути (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
static lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_trans && obj->styles[i].selector == selector) break;
    }

    /*Already have a transition style for it*/
    якщо(i != obj->style_cnt) повернути &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(sizeof(lv_style_t));
    якщо(new_style == NULL) {
        LV_LOG_WARN("couldn't allocate transition style");
        повернути NULL;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * sizeof(lv_obj_style_t));
    якщо(new_styles == NULL) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        повернути NULL;
    }
    obj->styles = new_styles;

    для(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], sizeof(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    повернути &obj->styles[0];
}

static lv_style_res_t get_prop_core(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_value_t * v)
{

    const uint32_t group = (uint32_t)1 << lv_style_get_prop_group(prop);
    const lv_part_t part = lv_obj_style_get_selector_part(selector);
    const bool skip_trans = (const bool) obj->skip_trans;
    int32_t weight = -1;
    lv_style_res_t found;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    для(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        якщо(obj_style->is_trans == false) break;
        якщо(skip_trans) continue;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        якщо(part_act != part) continue;
        якщо((obj_style->style->has_group & group) == 0) continue;
        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        якщо(found == LV_STYLE_RES_FOUND) {
            повернути LV_STYLE_RES_FOUND;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    const lv_state_t state = lv_obj_style_get_selector_state(selector);
    const lv_state_t state_inv = ~state;
    для(; i < obj->style_cnt; i++) {
        якщо((obj->styles[i].style->has_group & group) == 0) continue;
        якщо(obj->styles[i].is_disabled) continue;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        якщо(part_act != part) continue;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        якщо((state_act & state_inv)) continue;

        /*Check only better candidates*/
        якщо((int32_t)state_act <= weight) continue;

        found = lv_style_get_prop_internal(obj_style->style, prop, v);
        якщо(found == LV_STYLE_RES_FOUND) {
            якщо(state_act == state) {
                повернути LV_STYLE_RES_FOUND;
            }
            weight = state_act;
        }
    }

    якщо(weight >= 0) повернути LV_STYLE_RES_FOUND;
    інакше повернути LV_STYLE_RES_NOT_FOUND;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
static void report_style_change_core(void * style, lv_obj_t * obj)
{
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(style == NULL || obj->styles[i].style == style) {
            full_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            break;
        }
    }

    uint32_t child_cnt = lv_obj_get_child_count(obj);
    для(i = 0; i < child_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
static void refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    для(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        lv_obj_invalidate(child);
        lv_obj_send_event(child, LV_EVENT_STYLE_CHANGED, NULL);
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
static bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_limit)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_tail(style_trans_ll_p);
    поки(tr != NULL) {
        якщо(tr == tr_limit) break;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        якщо(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            якщо(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            для(i = 0; i < obj->style_cnt; i++) {
                якщо(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, NULL);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    повернути removed;
}

static void trans_anim_cb(void * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) continue;

        lv_style_value_t value_final = {0};
        switch(tr->prop) {

            case LV_STYLE_BORDER_SIDE:
            case LV_STYLE_BORDER_POST:
            case LV_STYLE_BLEND_MODE:
                якщо(v < 255) value_final.num = tr->start_value.num;
                інакше value_final.num = tr->end_value.num;
                break;
            case LV_STYLE_TRANSITION:
            case LV_STYLE_TEXT_FONT:
            case LV_STYLE_BG_IMAGE_SRC:
                якщо(v < 255) value_final.ptr = tr->start_value.ptr;
                інакше value_final.ptr = tr->end_value.ptr;
                break;
            case LV_STYLE_COLOR_FILTER_DSC:
                якщо(tr->start_value.ptr == NULL) value_final.ptr = tr->end_value.ptr;
                інакше якщо(tr->end_value.ptr == NULL) value_final.ptr = tr->start_value.ptr;
                інакше якщо(v < 128) value_final.ptr = tr->start_value.ptr;
                інакше value_final.ptr = tr->end_value.ptr;
                break;
            /*Colors*/
            case LV_STYLE_RECOLOR:
            case LV_STYLE_BG_COLOR:
            case LV_STYLE_BG_GRAD_COLOR:
            case LV_STYLE_BORDER_COLOR:
            case LV_STYLE_TEXT_COLOR:
            case LV_STYLE_SHADOW_COLOR:
            case LV_STYLE_OUTLINE_COLOR:
            case LV_STYLE_IMAGE_RECOLOR:
                якщо(v <= 0) value_final.color = tr->start_value.color;
                інакше якщо(v >= 255) value_final.color = tr->end_value.color;
                інакше value_final.color = lv_color_mix(tr->end_value.color, tr->start_value.color, v);
                break;
            /*Opacities*/
            case LV_STYLE_OPA:
            case LV_STYLE_OPA_LAYERED:
            case LV_STYLE_RECOLOR_OPA:
            case LV_STYLE_COLOR_FILTER_OPA:
            case LV_STYLE_BG_OPA:
            case LV_STYLE_BG_GRAD_OPA:
            case LV_STYLE_BG_MAIN_OPA:
            case LV_STYLE_BG_IMAGE_OPA:
            case LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            case LV_STYLE_BORDER_OPA:
            case LV_STYLE_OUTLINE_OPA:
            case LV_STYLE_SHADOW_OPA:
            case LV_STYLE_LINE_OPA:
            case LV_STYLE_ARC_OPA:
            case LV_STYLE_TEXT_OPA:
            case LV_STYLE_IMAGE_OPA:
            case LV_STYLE_IMAGE_RECOLOR_OPA:
            case LV_STYLE_DROP_SHADOW_OPA:
            case LV_STYLE_TEXT_OUTLINE_STROKE_OPA:
                якщо(v <= 0) value_final.num = tr->start_value.num;
                інакше якщо(v >= 255) value_final.num = tr->end_value.num;
                інакше value_final.num =  LV_CLAMP(0, tr->start_value.num + ((tr->end_value.num - tr->start_value.num) * v >> 8), 255);
                break;

            default:
                якщо(v == 0) value_final.num = tr->start_value.num;
                інакше якщо(v == 255) value_final.num = tr->end_value.num;
                інакше value_final.num = tr->start_value.num + ((int32_t)((int32_t)(tr->end_value.num - tr->start_value.num) * v) >> 8);
                break;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_value_t old_value = {0};
        якщо(lv_style_get_prop_internal(obj->styles[i].style, tr->prop, &old_value) == LV_STYLE_RES_FOUND) {
            якщо(value_final.ptr == old_value.ptr &&
               lv_color_eq(value_final.color, old_value.color) &&
               value_final.num == old_value.num) {
                break;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        якщо(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_set_prop((lv_style_t *)obj->styles[i].style, tr->prop, value_final);

#якщо LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        якщо(part == LV_PART_MAIN) {
            obj->style_main_prop_is_set |= prop_shifted;
        }
        інакше {
            obj->style_other_prop_is_set |= prop_shifted;
        }
#endif

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        break;
    }

}

static void trans_anim_start_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->start_value = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_set_prop((lv_style_t *)style_trans->style, tr->prop, tr->start_value);
#якщо LV_OBJ_STYLE_CACHE
    якщо(trans_removed) {
        full_cache_refresh(tr->obj, part);
    }
    інакше {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        якщо(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_set |= prop_shifted;
        }
        інакше {
            tr->obj->style_other_prop_is_set |= prop_shifted;
        }
    }
#інакше
    LV_UNUSED(trans_removed);
#endif

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

static void trans_anim_completed_cb(lv_anim_t * a)
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
        якщо(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            break;
        }
    }

    якщо(!running) {
        uint32_t i;
        для(i = 0; i < obj->style_cnt; i++) {
            якщо(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                якщо(lv_style_is_empty(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                break;
            }
        }
    }
}

static lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#якщо LV_DRAW_TRANSFORM_USE_MATRIX
    якщо(lv_obj_get_transform(obj) != NULL) повернути LV_LAYER_TYPE_TRANSFORM;
#endif
    якщо(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) != 0) повернути LV_LAYER_TYPE_TRANSFORM;
    якщо(lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN) != 256) повернути LV_LAYER_TYPE_TRANSFORM;
    якщо(lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN) != 256) повернути LV_LAYER_TYPE_TRANSFORM;
    якщо(lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN) != 0) повернути LV_LAYER_TYPE_TRANSFORM;
    якщо(lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN) != 0) повернути LV_LAYER_TYPE_TRANSFORM;
    якщо(lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN) != LV_OPA_COVER) повернути LV_LAYER_TYPE_SIMPLE;
    якщо(lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN) != NULL) повернути LV_LAYER_TYPE_SIMPLE;
    якщо(lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN) != LV_BLEND_MODE_NORMAL) повернути LV_LAYER_TYPE_SIMPLE;
    повернути LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
static bool calculate_has_blur(lv_obj_t * obj)
{
    const uint32_t group_blur = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_BLUR_RADIUS);
    const uint32_t group_dropshadow = (uint32_t)1 << lv_style_get_prop_group(LV_STYLE_DROP_SHADOW_OPA);
    const lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    const lv_state_t state_inv = ~state;
    lv_style_value_t v;
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        якщо(obj_style->is_disabled) continue;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        якщо((state_style & state_inv)) continue;

        якщо((obj_style->style->has_group & group_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            якщо(v.num > 0) повернути true;
        }
        якщо((obj_style->style->has_group & group_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            якщо(v.num > 0) повернути true;
        }
    }

    повернути false;
}

static void full_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#якщо LV_OBJ_STYLE_CACHE
    uint32_t i;
    якщо(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_set = 0;
        для(i = 0; i < obj->style_cnt; i++) {
            якщо(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) continue;
            якщо(obj->styles[i].is_disabled) continue;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            якщо(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->values_and_props;
                для(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            інакше {
                lv_style_prop_t * props = (lv_style_prop_t *)style->values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
                для(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    якщо(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_set = 0;
        для(i = 0; i < obj->style_cnt; i++) {
            якщо(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) continue;
            якщо(obj->styles[i].is_disabled) continue;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            якщо(lv_style_is_const_internal(style)) {
                lv_style_const_prop_t * props = style->values_and_props;
                для(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            інакше {
                lv_style_prop_t * props = (lv_style_prop_t *)style->values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
                для(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_set |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#інакше
    LV_UNUSED(obj);
    LV_UNUSED(part);
#endif
}

static void fade_anim_cb(void * obj, int32_t v)
{
    lv_obj_set_style_opa(obj, v, 0);
}

static void fade_in_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

static bool style_has_flag(const lv_style_t * style, uint32_t flag)
{
    якщо(lv_style_is_const_internal(style)) {
        lv_style_const_prop_t * props = style->values_and_props;
        uint32_t i;
        для(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            якщо(lv_style_prop_has_flag(props[i].prop, flag)) {
                повернути true;
            }
        }
    }
    інакше {
        lv_style_prop_t * props = (lv_style_prop_t *)style->values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
        uint32_t i;
        для(i = 0; i < style->prop_cnt; i++) {
            якщо(lv_style_prop_has_flag(props[i], flag)) {
                повернути true;
            }
        }
    }
    повернути false;
}

static lv_style_res_t get_selector_style_prop(const lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_value_t * value_act)
{
    lv_style_res_t found;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#якщо LV_OBJ_STYLE_CACHE
    const uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    якщо((part == LV_PART_MAIN ? obj->style_main_prop_is_set : obj->style_other_prop_is_set) & prop_shifted)
#endif
    {
        found = get_prop_core(obj, selector, prop, value_act);
        якщо(found == LV_STYLE_RES_FOUND) повернути LV_STYLE_RES_FOUND;
    }

    extern const uint8_t lv_style_builtin_prop_flag_lookup_table[];
    bool inheritable = false;
    якщо(prop < LV_STYLE_NUM_BUILT_IN_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_table[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    інакше {
        якщо(_style_custom_prop_flag_lookup_table != NULL) {
            inheritable = _style_custom_prop_flag_lookup_table[prop - LV_STYLE_NUM_BUILT_IN_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    якщо(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        якщо(part != LV_PART_MAIN) part = LV_PART_MAIN;
        інакше obj = obj->parent;

        поки(obj) {
#якщо LV_OBJ_STYLE_CACHE
            якщо(obj->style_main_prop_is_set & prop_shifted)
#endif
            {
                selector = part | obj->state;
                found = get_prop_core(obj, selector, prop, value_act);
                якщо(found == LV_STYLE_RES_FOUND) повернути LV_STYLE_RES_FOUND;
            }
            /*Check the parent too.*/
            obj = obj->parent;
        }
    }
    інакше {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        якщо(part == LV_PART_MAIN && (prop == LV_STYLE_WIDTH || prop == LV_STYLE_HEIGHT)) {
            const lv_obj_class_t * cls = obj->class_p;
            поки(cls) {
                якщо(prop == LV_STYLE_WIDTH) {
                    якщо(cls->width_def != 0)  {
                        value_act->num = cls->width_def;
                        повернути LV_STYLE_RES_FOUND;
                    }
                }
                інакше {
                    якщо(cls->height_def != 0) {
                        value_act->num = cls->height_def;
                        повернути LV_STYLE_RES_FOUND;
                    }
                }
                cls = cls->base_class;
            }
        }
    }

    повернути LV_STYLE_RES_NOT_FOUND;
}

static void remove_style_core(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != NULL);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    якщо((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == NULL || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_count = 0;
    bool deleted = false;

    для(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        якщо(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        інакше якщо((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != NULL && style != obj->styles[i].style)) {
            should_remove = false;
        }

        якщо(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            якщо(style_count != i) {
                obj->styles[style_count] = obj->styles[i];
            }
            style_count++;
            continue;
        }

        /*This entry is being removed*/
        якщо(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, NULL);
        }
        якщо(obj->styles[i].is_local || obj->styles[i].is_trans) {
            якщо(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = NULL;
        }
        deleted = true;
    }

    якщо(!deleted) {
        повернути;
    }

    obj->style_cnt = style_count;
    якщо(style_count > 0) {
        obj->styles = lv_realloc(obj->styles, style_count * sizeof(lv_obj_style_t));
    }
    інакше {
        lv_free(obj->styles);
        obj->styles = NULL;
    }

    якщо(deleted) {
        full_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

static lv_obj_style_t * find_style(lv_obj_t * obj, const lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    для(i = 0; i < obj->style_cnt; i++) {
        якщо(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            повернути &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (void *)style, (void *)obj, selector);
    повернути NULL;
}



#якщо LV_USE_OBSERVER

static void bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_data(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->value);
    lv_obj_set_style_enabled(observer->target, p->style, p->selector, en);
}

static void bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_data(observer);

    lv_style_value_t style_v;
    якщо(subject->type == LV_SUBJECT_TYPE_INT) style_v.num = lv_subject_get_int(subject);
    інакше якщо(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    інакше якщо(subject->type == LV_SUBJECT_TYPE_POINTER) style_v.ptr = lv_subject_get_pointer(subject);
    інакше {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        повернути;
    }

    lv_obj_set_local_style_prop(observer->target, p->prop, style_v, p->selector);
}


#endif /*LV_USE_OBSERVER*/
