/**
 * @file lv_obj_style.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_private.h"
#ضمن "../lvgl_public.h"
#ضمن "../misc/lv_anim_private.h"
#ضمن "../misc/lv_bidi_private.h"
#ضمن "../misc/lv_style_private.h"
#ضمن "lv_obj_style_private.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../display/lv_display_private.h"
#ضمن "../core/lv_global.h"
#ضمن "lv_observer_private.h"
#ضمن "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#وصف_ڪريو MY_CLASS (&lv_obj_class)
#وصف_ڪريو style_refr LV_GLOBAL_DEFAULT()->style_refresh
#وصف_ڪريو style_trans_ll_p &(LV_GLOBAL_DEFAULT()->style_trans_ll)
#وصف_ڪريو style_custom_prop_flag_lookup_جدول_2 LV_GLOBAL_DEFAULT()->style_custom_prop_flag_lookup_جدول
#وصف_ڪريو STYLE_PROP_SHIFTED(prop) ((uint32_t)1 << ((prop) >> 3))

/*Property flags that can change the area where the widget is drawn.
 *Before changing such a property the old area needs to be invalidated,
 *else the already drawn pixels are left on the screen.*/
#وصف_ڪريو DRAW_AREA_FLAGS (LV_STYLE_PROP_FLAG_TRANSFORM | LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE)

/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع بنية {
    lv_obj_t * obj;
    lv_style_prop_t prop;
    lv_style_selector_t selector;
    lv_style_قدر_t شروعات_قدر;
    lv_style_قدر_t پڇاڙي_قدر;
} trans_t;

تعريفنوع بنية {
    ثابت lv_style_t * style;
    lv_style_selector_t selector;
    int32_t قدر;
} bind_style_t;

تعريفنوع بنية {
    lv_style_prop_t prop;
    lv_style_selector_t selector;
} bind_style_prop_t;

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector);
ساكن lv_obj_style_t * get_trans_style(lv_obj_t * obj, lv_style_selector_t selector);
ساكن lv_style_res_t get_prop_core(ثابت lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_قدر_t * v);
ساكن فراغ report_style_change_core(فراغ * style, lv_obj_t * obj);
ساكن فراغ refresh_children_style(lv_obj_t * obj);
ساكن bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_حد_بندي);
ساكن فراغ trans_anim_cb(فراغ * _tr, int32_t v);
ساكن فراغ trans_anim_شروعات_cb(lv_anim_t * a);
ساكن فراغ trans_anim_completed_cb(lv_anim_t * a);
ساكن lv_layer_type_t calculate_layer_type(lv_obj_t * obj);
ساكن bool calculate_has_blur(lv_obj_t * obj);
ساكن فراغ ڀريل_cache_refresh(lv_obj_t * obj, lv_part_t part);
ساكن فراغ fade_anim_cb(فراغ * obj, int32_t v);
ساكن فراغ fade_اندريون_anim_completed(lv_anim_t * a);
ساكن bool style_has_flag(ثابت lv_style_t * style, uint32_t flag);
ساكن lv_style_res_t get_selector_style_prop(ثابت lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_قدر_t * قدر_act);
ساكن فراغ remove_style_core(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector, bool theme_only);
ساكن lv_obj_style_t * ڳوليو_style(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector);
#إذا LV_USE_OBSERVER
    ساكن فراغ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
    ساكن فراغ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#جيڪڏهن_پورو

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_obj_style_init(فراغ)
{
    lv_ll_init(style_trans_ll_p, حجم(trans_t));
}

فراغ lv_obj_style_deinit(فراغ)
{
    lv_ll_clear(style_trans_ll_p);
    إذا(style_custom_prop_flag_lookup_جدول_2 != عدم) {
        lv_free(style_custom_prop_flag_lookup_جدول_2);
        style_custom_prop_flag_lookup_جدول_2 = عدم;
    }
}

فراغ lv_obj_جوڙيو_style(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(style != عدم, أرجع);
    LV_CHECK_ARG_نياپو(obj->style_cnt < 63, أرجع,
                     "obj->style_cnt is restricted to 6 bits, so we can't store more than 63 styles");

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    إذا(part == LV_PART_MAIN && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, عدم);

    /*Try removing the style first to be sure it won't be added twice*/
    lv_obj_remove_style(obj, style, selector);

    uint32_t i;
    /*Go after the transition and local styles*/
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_trans) استمر;
        إذا(obj->styles[i].is_local) استمر;
        اكسر;
    }

    /*Now `i` is at the first normal style. Insert the new style before this*/

    /*Allocate space for the new style and shift the rest of the style to the end*/
    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    obj->styles = lv_realloc(obj->styles, obj->style_cnt * حجم(lv_obj_style_t));
    LV_ASSERT_MALLOC(obj->styles);

    uint32_t j;
    لكل(j = obj->style_cnt - 1; j > i ; j--) {
        obj->styles[j] = obj->styles[j - 1];
    }

    lv_memzero(&obj->styles[i], حجم(lv_obj_style_t));
    obj->styles[i].style = style;
    obj->styles[i].selector = selector;

    إذا(trans_removed) {
        ڀريل_cache_refresh(obj, LV_PART_ANY);
    }
#إذا LV_OBJ_STYLE_CACHE
    وإلا {
        uint32_t * prop_is_سيٽ = part == LV_PART_MAIN ? &obj->style_main_prop_is_سيٽ : &obj->style_other_prop_is_سيٽ;
        إذا(lv_style_is_const_اندروني(style)) {
            lv_style_const_prop_t * props = style->قدرون_and_props;
            لكل(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
                (*prop_is_سيٽ) |= STYLE_PROP_SHIFTED(props[i].prop);
            }
        }
        وإلا {
            lv_style_prop_t * props = (lv_style_prop_t *)style->قدرون_and_props + style->prop_cnt * حجم(lv_style_قدر_t);
            لكل(i = 0; i < style->prop_cnt; i++) {
                (*prop_is_سيٽ) |= STYLE_PROP_SHIFTED(props[i]);
            }
        }
    }
#جيڪڏهن_پورو

    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_replace_style(lv_obj_t * obj, ثابت lv_style_t * old_style, ثابت lv_style_t * new_style,
                          lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(old_style != عدم, أرجع false);
    LV_CHECK_ARG(new_style != عدم, أرجع false);
    LV_CHECK_ARG(old_style != new_style, أرجع false);

    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    إذا((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style_has_flag(old_style, DRAW_AREA_FLAGS) || style_has_flag(new_style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    /*Similar to lv_obj_add_style, delete transition*/
    bool trans_removed = remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, عدم);

    bool replaced = false;
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        /*Skip local styles and transitions*/
        إذا(obj->styles[i].is_local || obj->styles[i].is_trans) {
            استمر;
        }

        /*Skip non-matching styles*/
        إذا((state != LV_STATE_ANY && state_act != state) ||
           (part != LV_PART_ANY && part_act != part) ||
           (old_style != obj->styles[i].style)) {
            استمر;
        }

        lv_style_selector_t selector_act = obj->styles[i].selector;
        lv_memzero(&obj->styles[i], حجم(lv_obj_style_t));
        obj->styles[i].style = new_style;
        obj->styles[i].selector = selector_act;

        replaced = true;
        /*Don't break and continue replacing other occurrences*/
    }
    إذا(replaced || trans_removed) {
        ڀريل_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
    أرجع replaced;
}

فراغ lv_obj_remove_style(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    remove_style_core(obj, style, selector, false);
}

فراغ lv_obj_remove_theme(lv_obj_t * obj, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    remove_style_core(obj, عدم, selector, true);
}

فراغ lv_obj_remove_style_all(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    remove_style_core(obj, عدم, LV_PART_ANY | LV_STATE_ANY, false);
}

فراغ lv_obj_report_style_change(lv_style_t * style)
{
    إذا(!style_refr) أرجع;
    lv_display_t * d = lv_display_get_ايندڙ(عدم);

    طالما(d) {
        uint32_t i;
        لكل(i = 0; i < d->screen_cnt; i++) {
            report_style_change_core(style, d->screens[i]);
        }
        d = lv_display_get_ايندڙ(d);
    }
}

فراغ lv_obj_refresh_style(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!style_refr) أرجع;

    LV_PROFILER_STYLE_BEGIN;

    lv_obj_invalidate(obj);

    bool is_layout_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYOUT_UPDATE);
    bool is_ext_draw = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_EXT_DRAW_UPDATE);
    bool is_inheritable = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_INHERITABLE);
    bool is_layer_refr = lv_style_prop_has_flag(prop, LV_STYLE_PROP_FLAG_LAYER_UPDATE);

    إذا(is_layout_refr) {
        إذا(part == LV_PART_ANY ||
           part == LV_PART_MAIN ||
           lv_obj_get_style_اوچائي_اندروني(obj, LV_PART_MAIN) == LV_سائيز_CONTENT ||
           lv_obj_get_style_ويڪر_اندروني(obj, LV_PART_MAIN) == LV_سائيز_CONTENT) {
            lv_obj_send_event(obj, LV_EVENT_STYLE_بدليو, عدم);
            lv_obj_mark_layout_as_dirty(obj);
        }
    }
    إذا((part == LV_PART_ANY || part == LV_PART_MAIN) && (prop == LV_STYLE_PROP_ANY || is_layout_refr)) {
        lv_obj_t * والد = lv_obj_get_والد(obj);
        إذا(والد) lv_obj_mark_layout_as_dirty(والد);
    }

    /*Cache the layer type*/
    إذا((part == LV_PART_ANY || part == LV_PART_MAIN) && is_layer_refr) {
        lv_obj_update_layer_type(obj);
    }

    /*Cache whether the widget has blur or drop shadow*/
    إذا(prop == LV_STYLE_PROP_ANY || prop == LV_STYLE_BLUR_RADIUS || prop == LV_STYLE_DROP_SHADOW_OPA) {
        lv_obj_update_blur_حالت(obj);
    }

    إذا(prop == LV_STYLE_PROP_ANY || is_ext_draw) {
        lv_obj_refresh_ext_draw_سائيز(obj);
    }
    lv_obj_invalidate(obj);

    إذا(prop == LV_STYLE_PROP_ANY || (is_inheritable && (is_ext_draw || is_layout_refr))) {
        إذا(part != LV_PART_SCROLLBAR) {
            refresh_children_style(obj);
        }
    }

    LV_PROFILER_STYLE_پڇاڙي;
}

فراغ lv_obj_سيٽ_style_enabled(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector, bool en)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(style != عدم, أرجع);

    lv_obj_style_t * obj_style = ڳوليو_style(obj, style, selector);
    إذا(obj_style == عدم) أرجع;

    إذا(en != obj_style->is_disabled) أرجع; /*Already in the right state*/

    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first*/
    إذا((part == LV_PART_MAIN || part == LV_PART_ANY) && style_has_flag(style, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    obj_style->is_disabled = !en;
    ڀريل_cache_refresh(obj, part);
    lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
}

bool lv_obj_get_style_enabled(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);
    LV_CHECK_ARG(style != عدم, أرجع false);

    lv_obj_style_t * obj_style = ڳوليو_style(obj, style, selector);
    إذا(obj_style == عدم) أرجع false;

    أرجع !obj_style->is_disabled;
}


فراغ lv_obj_enable_style_refresh(bool en)
{
    style_refr = en;
}

lv_style_قدر_t lv_obj_get_style_prop(ثابت lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != عدم, أرجع lv_style_prop_get_default(prop));
    أرجع lv_obj_get_style_prop_اندروني(obj, part, prop);
}

lv_style_قدر_t lv_obj_get_style_prop_اندروني(ثابت lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop)
{
    LV_ASSERT(obj != عدم);
    lv_style_selector_t selector = part | obj->state;
    lv_style_قدر_t قدر_act = { .ptr = عدم };
    lv_style_res_t مليو;

    مليو = get_selector_style_prop(obj, selector, prop, &قدر_act);
    إذا(مليو == LV_STYLE_RES_مليو) أرجع قدر_act;

    أرجع lv_style_prop_get_default(prop);
}
lv_style_قدر_t lv_obj_style_apply_color_filter_اندروني(ثابت lv_obj_t * obj, lv_part_t part, lv_style_قدر_t v)
{
#إذا LV_USE_COLOR_FILTER
    LV_ASSERT(obj != عدم);

    ثابت lv_color_filter_dsc_t * f = lv_obj_get_style_color_filter_dsc_اندروني(obj, part);
    إذا(f && f->filter_cb) {
        lv_opa_t f_opa = lv_obj_get_style_color_filter_opa_اندروني(obj, part);
        إذا(f_opa != 0) v.color = f->filter_cb(f, v.color, f_opa);
    }
#وإلا
    LV_اڻ_استعمال_ٿيل(obj);
    LV_اڻ_استعمال_ٿيل(part);
    LV_اڻ_استعمال_ٿيل(v);
#جيڪڏهن_پورو
    أرجع v;
}

bool lv_obj_has_style_prop(ثابت lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);

    lv_style_قدر_t قدر_act = { .ptr = عدم };
    lv_style_res_t مليو;

    مليو = get_selector_style_prop(obj, selector, prop, &قدر_act);
    إذا(مليو == LV_STYLE_RES_مليو) أرجع true;

    أرجع false;
}

فراغ lv_obj_سيٽ_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_قدر_t قدر,
                                 lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_PROFILER_STYLE_BEGIN;

    /*If the draw size changes invalidate the old area first*/
    إذا(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN && lv_style_prop_has_flag(prop, DRAW_AREA_FLAGS)) {
        lv_obj_invalidate(obj);
    }

    /*Stop running transitions with this property */
    bool trans_removed = remove_trans_styles(obj, lv_obj_style_get_selector_part(selector), prop, عدم);

    lv_style_t * style = get_local_style(obj, selector);

    lv_style_سيٽ_prop(style, prop, قدر);

    إذا(trans_removed) {
        ڀريل_cache_refresh(obj, LV_PART_ANY);
    }
#إذا LV_OBJ_STYLE_CACHE
    وإلا {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
        إذا(lv_obj_style_get_selector_part(selector) == LV_PART_MAIN) {
            obj->style_main_prop_is_سيٽ |= prop_shifted;
        }
        وإلا {
            obj->style_other_prop_is_سيٽ |= prop_shifted;
        }
    }
#جيڪڏهن_پورو

    lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    LV_PROFILER_STYLE_پڇاڙي;
}

lv_style_res_t lv_obj_get_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_قدر_t * قدر,
                                           lv_style_selector_t selector)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_STYLE_RES_NOT_مليو);
    LV_CHECK_ARG(قدر != عدم, أرجع LV_STYLE_RES_NOT_مليو);

    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            أرجع lv_style_get_prop_اندروني(obj->styles[i].style, prop, قدر);
        }
    }

    أرجع LV_STYLE_RES_NOT_مليو;
}

bool lv_obj_remove_local_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    uint32_t i;
    /*Find the style*/
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            اكسر;
        }
    }

    /*The style is not found*/
    إذا(i == obj->style_cnt) أرجع false;

    lv_نتيجو_t res = lv_style_remove_prop((lv_style_t *)obj->styles[i].style, prop);
    إذا(res == LV_نتيجو_OK) {
        ڀريل_cache_refresh(obj, lv_obj_style_get_selector_part(selector));
        lv_obj_refresh_style(obj, lv_obj_style_get_selector_part(selector), prop);
    }

    أرجع res;
}

فراغ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state, lv_state_t new_state,
                                    ثابت lv_obj_style_transition_dsc_t * tr_dsc)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(tr_dsc != عدم, أرجع);

    trans_t * tr;

    /*Get the previous and current values*/
    obj->skip_trans = 1;
    obj->state = prev_state;
    lv_style_قدر_t v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;
    lv_style_قدر_t v2 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->skip_trans = 0;

    إذا(v1.ptr == v2.ptr && v1.انگ == v2.انگ && lv_color_eq(v1.color, v2.color))  أرجع;
    obj->state = prev_state;
    v1 = lv_obj_get_style_prop(obj, part, tr_dsc->prop);
    obj->state = new_state;

    lv_obj_style_t * style_trans = get_trans_style(obj, part);
    lv_style_سيٽ_prop((lv_style_t *)style_trans->style, tr_dsc->prop, v1);  /*Be sure `trans_style` has a valid value*/
    lv_obj_refresh_style(obj, part, tr_dsc->prop);

    إذا(tr_dsc->prop == LV_STYLE_RADIUS) {
        إذا(v1.انگ == LV_RADIUS_CIRCLE || v2.انگ == LV_RADIUS_CIRCLE) {
            int32_t whalf = lv_obj_get_ويڪر(obj) / 2;
            int32_t hhalf = lv_obj_get_اوچائي(obj) / 2;
            إذا(v1.انگ == LV_RADIUS_CIRCLE) v1.انگ = LV_MIN(whalf + 1, hhalf + 1);
            إذا(v2.انگ == LV_RADIUS_CIRCLE) v2.انگ = LV_MIN(whalf + 1, hhalf + 1);
        }
    }

    tr = lv_ll_ins_سر(style_trans_ll_p);
    LV_ASSERT_MALLOC(tr);
    إذا(tr == عدم) أرجع;
    tr->شروعات_قدر = v1;
    tr->پڇاڙي_قدر = v2;
    tr->obj = obj;
    tr->prop = tr_dsc->prop;
    tr->selector = part;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سيٽ_var(&a, tr);
    lv_anim_سيٽ_exec_cb(&a, trans_anim_cb);
    lv_anim_سيٽ_شروعات_cb(&a, trans_anim_شروعات_cb);
    lv_anim_سيٽ_completed_cb(&a, trans_anim_completed_cb);
    lv_anim_سيٽ_قدرون(&a, 0x00, 0xFF);
    lv_anim_سيٽ_duration(&a, tr_dsc->وقت);
    lv_anim_سيٽ_delay(&a, tr_dsc->delay);
    lv_anim_سيٽ_رستو_cb(&a, tr_dsc->رستو_cb);
    lv_anim_سيٽ_early_apply(&a, false);
    lv_anim_سيٽ_user_معلومات(&a, tr_dsc->user_معلومات);
    lv_anim_شروعات(&a);
}

lv_style_قدر_t lv_obj_style_apply_color_filter(ثابت lv_obj_t * obj, lv_part_t part, lv_style_قدر_t v)
{
    LV_CHECK_ARG(obj != عدم, أرجع v);
    أرجع lv_obj_style_apply_color_filter_اندروني(obj, part, v);
}

lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * بدليو_part)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_STYLE_STATE_CMP_SAME);

    lv_style_state_cmp_t res = LV_STYLE_STATE_CMP_SAME;

    /*Accumulate which part the differing styles belong to: the lone differing
     *part, or LV_PART_ANY once styles from more than one part differ. Reported
     *via changed_part so the caller can scope the refresh to a single part.
     *diff_part_found is needed as a separate flag because a style's part can
     *itself be LV_PART_ANY, so LV_PART_ANY can't double as "none seen yet".*/
    lv_part_t diff_part = LV_PART_ANY;
    bool diff_part_مليو = false;

    /*Are there any new styles for the new state?*/
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_trans) استمر;

        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        /*The style is valid for a state but not the other*/
        bool valid1 = state_act & (~state1) ? false : true;
        bool valid2 = state_act & (~state2) ? false : true;
        إذا(valid1 != valid2) {
            /*Record this differing style's part: keep it if it's the only one
             *seen so far, otherwise collapse to LV_PART_ANY (more than one part).*/
            lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
            إذا(!diff_part_مليو) {
                diff_part = part_act;
                diff_part_مليو = true;
            }
            وإلا إذا(part_act != diff_part) {
                diff_part = LV_PART_ANY;
            }

            ثابت lv_style_t * style = obj->styles[i].style;
            lv_style_قدر_t v;
            /*If there is layout difference on the main part, return immediately. There is no more serious difference*/
            bool layout_diff = false;
            إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_چوٽي, &v))layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_BOTTOM, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_کاٻو, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_ساڄو, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_ڪالم, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_PAD_صف, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_LAYOUT, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSLATE_X, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSLATE_Y, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_ويڪر, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_اوچائي, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_MIN_ويڪر, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_MAX_ويڪر, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_MIN_اوچائي, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_MAX_اوچائي, &v)) layout_diff = true;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_BORDER_ويڪر, &v)) layout_diff = true;

            إذا(layout_diff) {
                /*Returning early, so the full set of changed parts is unknown:
                 *report LV_PART_ANY rather than a possibly-incomplete part.*/
                إذا(بدليو_part) *بدليو_part = LV_PART_ANY;
                أرجع LV_STYLE_STATE_CMP_DIFF_LAYOUT;
            }

            /*Check for draw pad changes*/
            إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSFORM_ويڪر, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSFORM_اوچائي, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSFORM_ROTATION, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSFORM_SCALE_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_TRANSFORM_SCALE_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_OUTLINE_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_OUTLINE_PAD, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_OUTLINE_ويڪر, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_SHADOW_ويڪر, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_SHADOW_OPA, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_SHADOW_وٿي_X, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_SHADOW_وٿي_Y, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_SHADOW_پکيڙ, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(lv_style_get_prop_اندروني(style, LV_STYLE_سٽ_ويڪر, &v)) res = LV_STYLE_STATE_CMP_DIFF_DRAW_PAD;
            وإلا إذا(res == LV_STYLE_STATE_CMP_SAME) res = LV_STYLE_STATE_CMP_DIFF_REDRAW;
        }
    }

    إذا(بدليو_part) *بدليو_part = diff_part;
    أرجع res;
}

فراغ lv_obj_fade_اندريون(lv_obj_t * obj, uint32_t وقت, uint32_t delay)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سيٽ_var(&a, obj);
    lv_anim_سيٽ_قدرون(&a, 0, LV_OPA_COVER);
    lv_anim_سيٽ_exec_cb(&a, fade_anim_cb);
    lv_anim_سيٽ_completed_cb(&a, fade_اندريون_anim_completed);
    lv_anim_سيٽ_duration(&a, وقت);
    lv_anim_سيٽ_delay(&a, delay);
    lv_anim_شروعات(&a);
}

فراغ lv_obj_fade_ٻاهريون(lv_obj_t * obj, uint32_t وقت, uint32_t delay)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سيٽ_var(&a, obj);
    lv_anim_سيٽ_قدرون(&a, lv_obj_get_style_opa_اندروني(obj, LV_PART_MAIN), LV_OPA_TRANSP);
    lv_anim_سيٽ_exec_cb(&a, fade_anim_cb);
    lv_anim_سيٽ_duration(&a, وقت);
    lv_anim_سيٽ_delay(&a, delay);
    lv_anim_شروعات(&a);
}

lv_متن_align_t lv_obj_calculate_style_متن_align(ثابت lv_obj_t * obj, lv_part_t part, ثابت حرف * txt)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_متن_ALIGN_AUTO);
    LV_CHECK_ARG(txt != عدم, أرجع LV_متن_ALIGN_AUTO);
    أرجع lv_obj_calculate_style_متن_align_اندروني(obj, part, txt);
}

lv_opa_t lv_obj_get_style_opa_recursive(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_OPA_TRANSP);

    LV_PROFILER_STYLE_BEGIN;
    lv_opa_t opa_obj = lv_obj_get_style_opa_اندروني(obj, part);
    إذا(opa_obj <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_پڇاڙي;
        أرجع LV_OPA_TRANSP;
    }

    lv_opa_t opa_final = LV_OPA_COVER;
    إذا(opa_obj < LV_OPA_MAX) {
        opa_final = LV_OPA_MIX2(opa_final, opa_obj);
    }

    إذا(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    وإلا {
        obj = lv_obj_get_والد(obj);
    }

    طالما(obj) {
        opa_obj = lv_obj_get_style_opa_اندروني(obj, part);
        إذا(opa_obj <= LV_OPA_MIN) {
            LV_PROFILER_STYLE_پڇاڙي;
            أرجع LV_OPA_TRANSP;
        }
        إذا(opa_obj < LV_OPA_MAX) {
            opa_final = LV_OPA_MIX2(opa_final, opa_obj);
        }

        obj = lv_obj_get_والد(obj);
    }

    إذا(opa_final <= LV_OPA_MIN) {
        LV_PROFILER_STYLE_پڇاڙي;
        أرجع LV_OPA_TRANSP;
    }

    إذا(opa_final >= LV_OPA_MAX) {
        LV_PROFILER_STYLE_پڇاڙي;
        أرجع LV_OPA_COVER;
    }

    LV_PROFILER_STYLE_پڇاڙي;
    أرجع opa_final;
}

فراغ lv_obj_update_layer_type(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_layer_type_t layer_type = calculate_layer_type(obj);
    إذا(obj->spec_attr) obj->spec_attr->layer_type = layer_type;
    وإلا إذا(layer_type != LV_LAYER_TYPE_NONE) {
        إذا(!lv_obj_allocate_spec_attr(obj)) {
            أرجع;
        }
        obj->spec_attr->layer_type = layer_type;
    }
}

فراغ lv_obj_update_blur_حالت(lv_obj_t * obj)
{
    LV_ASSERT_NULL(obj);

    /*Deletion releases the count itself (see obj_delete_core())*/
    إذا(obj->is_deleting) أرجع;

    bool has_blur = calculate_has_blur(obj);
    إذا(has_blur == (bool)obj->has_blur) أرجع;

    obj->has_blur = has_blur;
    إذا(has_blur) {
        LV_GLOBAL_DEFAULT()->blur_obj_cnt++;
    }
    وإلا {
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }
}

lv_color32_t lv_obj_style_apply_recolor(ثابت lv_obj_t * obj, lv_part_t part, lv_color32_t color)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_color32_t) {
        0
    });

    lv_opa_t opa = lv_obj_get_style_recolor_opa_اندروني(obj, part);
    إذا(opa > LV_OPA_TRANSP) {
        lv_color_t recolor = lv_obj_get_style_recolor_اندروني(obj, part);
        color = lv_color_over32(color, lv_color_منزل_32(recolor, opa));
    }

    أرجع color;
}

lv_color32_t lv_obj_get_style_recolor_recursive(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_color32_t) {
        0
    });

    lv_color32_t نتيجو;

    lv_color_t color = lv_obj_get_style_recolor_اندروني(obj, part);
    lv_opa_t opa = lv_obj_get_style_recolor_opa_اندروني(obj, part);

    نتيجو = lv_color_منزل_32(color, opa);

    إذا(part != LV_PART_MAIN) {
        part = LV_PART_MAIN;
    }
    وإلا {
        obj = lv_obj_get_والد(obj);
    }

    طالما(obj) {
        نتيجو = lv_obj_style_apply_recolor(obj, part, نتيجو);
        obj = lv_obj_get_والد(obj);
    }

    أرجع نتيجو;
}

فراغ lv_obj_سيٽ_style_pad_all(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_pad_کاٻو(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_ساڄو(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_چوٽي(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_bottom(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_pad_hor(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_pad_کاٻو(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_ساڄو(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_pad_ver(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_pad_چوٽي(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_bottom(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_margin_all(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_margin_کاٻو(obj, قدر, selector);
    lv_obj_سيٽ_style_margin_ساڄو(obj, قدر, selector);
    lv_obj_سيٽ_style_margin_چوٽي(obj, قدر, selector);
    lv_obj_سيٽ_style_margin_bottom(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_margin_hor(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_margin_کاٻو(obj, قدر, selector);
    lv_obj_سيٽ_style_margin_ساڄو(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_margin_ver(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_margin_چوٽي(obj, قدر, selector);
    lv_obj_سيٽ_style_margin_bottom(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_pad_gap(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_pad_صف(obj, قدر, selector);
    lv_obj_سيٽ_style_pad_ڪالم(obj, قدر, selector);
}

فراغ lv_obj_سيٽ_style_سائيز(lv_obj_t * obj, int32_t ويڪر, int32_t اوچائي, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_ويڪر(obj, ويڪر, selector);
    lv_obj_سيٽ_style_اوچائي(obj, اوچائي, selector);
}

فراغ lv_obj_سيٽ_style_transform_scale(lv_obj_t * obj, int32_t قدر, lv_style_selector_t selector)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع);
    lv_obj_سيٽ_style_transform_scale_x(obj, قدر, selector);
    lv_obj_سيٽ_style_transform_scale_y(obj, قدر, selector);
}

int32_t lv_obj_get_style_space_کاٻو(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_space_کاٻو_اندروني(obj, part);
}

int32_t lv_obj_get_style_space_ساڄو(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_space_ساڄو_اندروني(obj, part);
}

int32_t lv_obj_get_style_space_چوٽي(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_space_چوٽي_اندروني(obj, part);
}

int32_t lv_obj_get_style_space_bottom(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_space_bottom_اندروني(obj, part);
}

int32_t lv_obj_get_style_transform_scale_x_محفوظ(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_transform_scale_x_محفوظ_اندروني(obj, part);
}

int32_t lv_obj_get_style_transform_scale_y_محفوظ(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, &lv_obj_class, أرجع 0)
    أرجع lv_obj_get_style_transform_scale_y_محفوظ_اندروني(obj, part);
}


#إذا LV_USE_OBSERVER

lv_observer_t * lv_obj_bind_style(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector,
                                  lv_subject_t * subject, int32_t ref_قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(style != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);

    إذا(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type must be `int` (was %d)", subject->type);
        أرجع عدم;
    }

    lv_obj_جوڙيو_style(obj, style, selector);

    bind_style_t * p = lv_malloc(حجم(bind_style_t));
    LV_ASSERT_MALLOC(p);
    إذا(p == عدم) {
        LV_LOG_WARN("Out of memory");
        أرجع عدم;
    }

    p->style = style;
    p->selector = selector;
    p->قدر = ref_قدر;

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, bind_style_observer_cb, obj, p);
    observable->auto_free_user_معلومات = 1;
    أرجع observable;
}

lv_observer_t * lv_obj_bind_style_prop(lv_obj_t * obj, lv_style_prop_t prop, lv_style_selector_t selector,
                                       lv_subject_t * subject)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(subject != عدم, أرجع عدم);

    إذا(subject->type != LV_SUBJECT_TYPE_INT && subject->type != LV_SUBJECT_TYPE_COLOR &&
       subject->type != LV_SUBJECT_TYPE_اشارو) {
        LV_LOG_WARN("Subject type must be `int`, `color`, or `pointer` (was %"LV_PRIu32")", (uint32_t) subject->type);
        أرجع عدم;
    }

    bind_style_prop_t * p = lv_malloc(حجم(bind_style_prop_t));
    LV_ASSERT_MALLOC(p);
    إذا(p == عدم) {
        LV_LOG_WARN("Out of memory");
        أرجع عدم;
    }

    p->prop = prop;
    p->selector = selector;

    lv_observer_t * observable = lv_subject_جوڙيو_observer_obj(subject, bind_style_prop_observer_cb, obj, p);
    observable->auto_free_user_معلومات = 1;
    أرجع observable;
}


#جيڪڏهن_پورو /*LV_USE_OBSERVER*/


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
ساكن lv_style_t * get_local_style(lv_obj_t * obj, lv_style_selector_t selector)
{
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_local &&
           obj->styles[i].selector == selector) {
            أرجع (lv_style_t *)obj->styles[i].style;
        }
    }

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc_zeroed(حجم(lv_style_t));
    إذا(new_style == عدم) {
        LV_LOG_WARN("couldn't allocate local style");
        أرجع عدم;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * حجم(lv_obj_style_t));
    إذا(new_styles == عدم) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        أرجع عدم;
    }
    obj->styles = new_styles;

    لكل(i = obj->style_cnt - 1; i > 0 ; i--) {
        /*Copy only normal styles (not local and transition).
         *The new local style will be added as the last local style*/
        إذا(obj->styles[i - 1].is_local || obj->styles[i - 1].is_trans) اكسر;
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[i], حجم(lv_obj_style_t));
    obj->styles[i].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[i].style);

    obj->styles[i].is_local = 1;
    obj->styles[i].selector = selector;
    أرجع (lv_style_t *)obj->styles[i].style;
}

/**
 * Get the transition style of an object for a given part and for a given state.
 * If the transition style for the part-state pair doesn't exist allocate and return it.
 * @param obj   pointer to an object
 * @param selector OR-ed value of parts and state for which the style should be get
 * @return pointer to the transition style
 */
ساكن lv_obj_style_t * get_trans_style(lv_obj_t * obj,  lv_style_selector_t selector)
{
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_trans && obj->styles[i].selector == selector) اكسر;
    }

    /*Already have a transition style for it*/
    إذا(i != obj->style_cnt) أرجع &obj->styles[i];

    /*Allocate the individual style first so obj->styles stays valid on failure*/
    lv_style_t * new_style = lv_malloc(حجم(lv_style_t));
    إذا(new_style == عدم) {
        LV_LOG_WARN("couldn't allocate transition style");
        أرجع عدم;
    }

    obj->style_cnt++;
    LV_ASSERT(obj->style_cnt != 0);
    lv_obj_style_t * new_styles = lv_realloc(obj->styles, obj->style_cnt * حجم(lv_obj_style_t));
    إذا(new_styles == عدم) {
        obj->style_cnt--;
        lv_free(new_style);
        LV_LOG_WARN("couldn't allocate styles");
        أرجع عدم;
    }
    obj->styles = new_styles;

    لكل(i = obj->style_cnt - 1; i > 0 ; i--) {
        obj->styles[i] = obj->styles[i - 1];
    }

    lv_memzero(&obj->styles[0], حجم(lv_obj_style_t));
    obj->styles[0].style = new_style;
    lv_style_init((lv_style_t *)obj->styles[0].style);

    obj->styles[0].is_trans = 1;
    obj->styles[0].selector = selector;
    أرجع &obj->styles[0];
}

ساكن lv_style_res_t get_prop_core(ثابت lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                    lv_style_قدر_t * v)
{

    ثابت uint32_t گروهه = (uint32_t)1 << lv_style_get_prop_گروهه(prop);
    ثابت lv_part_t part = lv_obj_style_get_selector_part(selector);
    ثابت bool skip_trans = (ثابت bool) obj->skip_trans;
    int32_t وزن = -1;
    lv_style_res_t مليو;
    uint32_t i;
    /*Handle the transition styles first. They are at the beginning of the style list
     *as the have the highest precedence. It's a different for loop as in case of transitions
     *the state doesn't need to be checked. */
    لكل(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        إذا(obj_style->is_trans == false) اكسر;
        إذا(skip_trans) استمر;

        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        إذا(part_act != part) استمر;
        إذا((obj_style->style->has_گروهه & گروهه) == 0) استمر;
        مليو = lv_style_get_prop_اندروني(obj_style->style, prop, v);
        إذا(مليو == LV_STYLE_RES_مليو) {
            أرجع LV_STYLE_RES_مليو;
        }
    }

    /*Check the normal local and global styles that are assigned to given state*/
    ثابت lv_state_t state = lv_obj_style_get_selector_state(selector);
    ثابت lv_state_t state_inv = ~state;
    لكل(; i < obj->style_cnt; i++) {
        إذا((obj->styles[i].style->has_گروهه & گروهه) == 0) استمر;
        إذا(obj->styles[i].is_disabled) استمر;
        lv_obj_style_t * obj_style = &obj->styles[i];
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);
        إذا(part_act != part) استمر;

        /*Be sure the style not specifies other state than the requested.
         *E.g. For HOVER+PRESS object state, HOVER style only is OK, but HOVER+FOCUS style is not*/
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        إذا((state_act & state_inv)) استمر;

        /*Check only better candidates*/
        إذا((int32_t)state_act <= وزن) استمر;

        مليو = lv_style_get_prop_اندروني(obj_style->style, prop, v);
        إذا(مليو == LV_STYLE_RES_مليو) {
            إذا(state_act == state) {
                أرجع LV_STYLE_RES_مليو;
            }
            وزن = state_act;
        }
    }

    إذا(وزن >= 0) أرجع LV_STYLE_RES_مليو;
    وإلا أرجع LV_STYLE_RES_NOT_مليو;
}

/**
 * Refresh the style of all children of an object. (Called recursively)
 * @param style refresh objects only with this
 * @param obj pointer to an object
 */
ساكن فراغ report_style_change_core(فراغ * style, lv_obj_t * obj)
{
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(style == عدم || obj->styles[i].style == style) {
            ڀريل_cache_refresh(obj, lv_obj_style_get_selector_part(obj->styles[i].selector));
            lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);
            اكسر;
        }
    }

    uint32_t ٻار_cnt = lv_obj_get_ٻار_ڳڻپ(obj);
    لكل(i = 0; i < ٻار_cnt; i++) {
        report_style_change_core(style, obj->spec_attr->children[i]);
    }
}

/**
 * Recursively refresh the style of the children. Go deeper until a not NULL style is found
 * because the NULL styles are inherited from the parent
 * @param obj pointer to an object
 */
ساكن فراغ refresh_children_style(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t ٻار_cnt = lv_obj_get_ٻار_ڳڻپ(obj);
    لكل(i = 0; i < ٻار_cnt; i++) {
        lv_obj_t * ٻار = obj->spec_attr->children[i];
        lv_obj_invalidate(ٻار);
        lv_obj_send_event(ٻار, LV_EVENT_STYLE_بدليو, عدم);
        lv_obj_invalidate(ٻار);

        refresh_children_style(ٻار); /*Check children too*/
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
ساكن bool remove_trans_styles(lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop, trans_t * tr_حد_بندي)
{
    trans_t * tr;
    trans_t * tr_prev;
    bool removed = false;
    tr = lv_ll_get_پڇ(style_trans_ll_p);
    طالما(tr != عدم) {
        إذا(tr == tr_حد_بندي) اكسر;

        /*'tr' might be deleted, so get the next object while 'tr' is valid*/
        tr_prev = lv_ll_get_prev(style_trans_ll_p, tr);

        إذا(tr->obj == obj && (part == tr->selector || part == LV_PART_ANY) && (prop == tr->prop || prop == LV_STYLE_PROP_ANY)) {
            /*Dropping the transitioned value changes the drawn area right away,
             *so invalidate the old area first*/
            إذا(lv_obj_style_get_selector_part(tr->selector) == LV_PART_MAIN &&
               lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
                lv_obj_invalidate(obj);
            }

            /*Remove any transitioned properties from the trans. style
             *to allow changing it by normal styles*/
            uint32_t i;
            لكل(i = 0; i < obj->style_cnt; i++) {
                إذا(obj->styles[i].is_trans && (part == LV_PART_ANY || obj->styles[i].selector == part)) {
                    lv_style_remove_prop((lv_style_t *)obj->styles[i].style, tr->prop);
                }
            }

            /*Free the transition descriptor too*/
            lv_anim_delete(tr, عدم);
            lv_ll_remove(style_trans_ll_p, tr);
            lv_free(tr);
            removed = true;

        }
        tr = tr_prev;
    }

    أرجع removed;
}

ساكن فراغ trans_anim_cb(فراغ * _tr, int32_t v)
{
    trans_t * tr = _tr;
    lv_obj_t * obj = tr->obj;

    uint32_t i;
    /*Find the transition style of of the descriptor*/
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].is_trans == 0 || obj->styles[i].selector != tr->selector) استمر;

        lv_style_قدر_t قدر_final = {0};
        بدل(tr->prop) {

            حالة LV_STYLE_BORDER_SIDE:
            حالة LV_STYLE_BORDER_POST:
            حالة LV_STYLE_BLEND_گهڻائي:
                إذا(v < 255) قدر_final.انگ = tr->شروعات_قدر.انگ;
                وإلا قدر_final.انگ = tr->پڇاڙي_قدر.انگ;
                اكسر;
            حالة LV_STYLE_TRANSITION:
            حالة LV_STYLE_متن_FONT:
            حالة LV_STYLE_BG_IMAGE_SRC:
                إذا(v < 255) قدر_final.ptr = tr->شروعات_قدر.ptr;
                وإلا قدر_final.ptr = tr->پڇاڙي_قدر.ptr;
                اكسر;
            حالة LV_STYLE_COLOR_FILTER_DSC:
                إذا(tr->شروعات_قدر.ptr == عدم) قدر_final.ptr = tr->پڇاڙي_قدر.ptr;
                وإلا إذا(tr->پڇاڙي_قدر.ptr == عدم) قدر_final.ptr = tr->شروعات_قدر.ptr;
                وإلا إذا(v < 128) قدر_final.ptr = tr->شروعات_قدر.ptr;
                وإلا قدر_final.ptr = tr->پڇاڙي_قدر.ptr;
                اكسر;
            /*Colors*/
            حالة LV_STYLE_RECOLOR:
            حالة LV_STYLE_BG_COLOR:
            حالة LV_STYLE_BG_GRAD_COLOR:
            حالة LV_STYLE_BORDER_COLOR:
            حالة LV_STYLE_متن_COLOR:
            حالة LV_STYLE_SHADOW_COLOR:
            حالة LV_STYLE_OUTLINE_COLOR:
            حالة LV_STYLE_IMAGE_RECOLOR:
                إذا(v <= 0) قدر_final.color = tr->شروعات_قدر.color;
                وإلا إذا(v >= 255) قدر_final.color = tr->پڇاڙي_قدر.color;
                وإلا قدر_final.color = lv_color_mix(tr->پڇاڙي_قدر.color, tr->شروعات_قدر.color, v);
                اكسر;
            /*Opacities*/
            حالة LV_STYLE_OPA:
            حالة LV_STYLE_OPA_LAYERED:
            حالة LV_STYLE_RECOLOR_OPA:
            حالة LV_STYLE_COLOR_FILTER_OPA:
            حالة LV_STYLE_BG_OPA:
            حالة LV_STYLE_BG_GRAD_OPA:
            حالة LV_STYLE_BG_MAIN_OPA:
            حالة LV_STYLE_BG_IMAGE_OPA:
            حالة LV_STYLE_BG_IMAGE_RECOLOR_OPA:
            حالة LV_STYLE_BORDER_OPA:
            حالة LV_STYLE_OUTLINE_OPA:
            حالة LV_STYLE_SHADOW_OPA:
            حالة LV_STYLE_سٽ_OPA:
            حالة LV_STYLE_ARC_OPA:
            حالة LV_STYLE_متن_OPA:
            حالة LV_STYLE_IMAGE_OPA:
            حالة LV_STYLE_IMAGE_RECOLOR_OPA:
            حالة LV_STYLE_DROP_SHADOW_OPA:
            حالة LV_STYLE_متن_OUTLINE_STROKE_OPA:
                إذا(v <= 0) قدر_final.انگ = tr->شروعات_قدر.انگ;
                وإلا إذا(v >= 255) قدر_final.انگ = tr->پڇاڙي_قدر.انگ;
                وإلا قدر_final.انگ =  LV_CLAMP(0, tr->شروعات_قدر.انگ + ((tr->پڇاڙي_قدر.انگ - tr->شروعات_قدر.انگ) * v >> 8), 255);
                اكسر;

            افتراضي:
                إذا(v == 0) قدر_final.انگ = tr->شروعات_قدر.انگ;
                وإلا إذا(v == 255) قدر_final.انگ = tr->پڇاڙي_قدر.انگ;
                وإلا قدر_final.انگ = tr->شروعات_قدر.انگ + ((int32_t)((int32_t)(tr->پڇاڙي_قدر.انگ - tr->شروعات_قدر.انگ) * v) >> 8);
                اكسر;
        }

        /*Don't do anything is the prop didn't changed*/
        lv_style_قدر_t old_قدر = {0};
        إذا(lv_style_get_prop_اندروني(obj->styles[i].style, tr->prop, &old_قدر) == LV_STYLE_RES_مليو) {
            إذا(قدر_final.ptr == old_قدر.ptr &&
               lv_color_eq(قدر_final.color, old_قدر.color) &&
               قدر_final.انگ == old_قدر.انگ) {
                اكسر;
            }
        }

        /*If the draw size changes invalidate the old area first*/
        lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
        إذا(part == LV_PART_MAIN && lv_style_prop_has_flag(tr->prop, DRAW_AREA_FLAGS)) {
            lv_obj_invalidate(tr->obj);
        }
        lv_style_سيٽ_prop((lv_style_t *)obj->styles[i].style, tr->prop, قدر_final);

#إذا LV_OBJ_STYLE_CACHE
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        إذا(part == LV_PART_MAIN) {
            obj->style_main_prop_is_سيٽ |= prop_shifted;
        }
        وإلا {
            obj->style_other_prop_is_سيٽ |= prop_shifted;
        }
#جيڪڏهن_پورو

        lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);
        اكسر;
    }

}

ساكن فراغ trans_anim_شروعات_cb(lv_anim_t * a)
{
    trans_t * tr = a->var;

    lv_part_t part = lv_obj_style_get_selector_part(tr->selector);
    tr->شروعات_قدر = lv_obj_get_style_prop(tr->obj, part, tr->prop);

    /*Init prop to an invalid values to be sure `trans_del` won't delete this added `tr`*/
    lv_style_prop_t prop_tmp = tr->prop;
    tr->prop = LV_STYLE_PROP_INV;

    /*Delete the related transitions if any*/
    bool trans_removed = remove_trans_styles(tr->obj, part, prop_tmp, tr);

    tr->prop = prop_tmp;

    lv_obj_style_t * style_trans = get_trans_style(tr->obj, tr->selector);
    /*Be sure `trans_style` has a valid value*/
    lv_style_سيٽ_prop((lv_style_t *)style_trans->style, tr->prop, tr->شروعات_قدر);
#إذا LV_OBJ_STYLE_CACHE
    إذا(trans_removed) {
        ڀريل_cache_refresh(tr->obj, part);
    }
    وإلا {
        uint32_t prop_shifted = STYLE_PROP_SHIFTED(tr->prop);
        إذا(part == LV_PART_MAIN) {
            tr->obj->style_main_prop_is_سيٽ |= prop_shifted;
        }
        وإلا {
            tr->obj->style_other_prop_is_سيٽ |= prop_shifted;
        }
    }
#وإلا
    LV_اڻ_استعمال_ٿيل(trans_removed);
#جيڪڏهن_پورو

    lv_obj_refresh_style(tr->obj, tr->selector, tr->prop);

}

ساكن فراغ trans_anim_completed_cb(lv_anim_t * a)
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
        إذا(tr_i != tr && tr_i->obj == tr->obj && tr_i->selector == tr->selector && tr_i->prop == tr->prop) {
            running = true;
            اكسر;
        }
    }

    إذا(!running) {
        uint32_t i;
        لكل(i = 0; i < obj->style_cnt; i++) {
            إذا(obj->styles[i].is_trans && obj->styles[i].selector == tr->selector) {
                lv_ll_remove(style_trans_ll_p, tr);
                lv_free(tr);

                lv_obj_style_t * obj_style = &obj->styles[i];
                lv_style_remove_prop((lv_style_t *)obj_style->style, prop);

                إذا(lv_style_is_خالي_جايون(obj->styles[i].style)) {
                    lv_obj_remove_style(obj, (lv_style_t *)obj_style->style, obj_style->selector);

                }
                اكسر;
            }
        }
    }
}

ساكن lv_layer_type_t calculate_layer_type(lv_obj_t * obj)
{
#إذا LV_DRAW_TRANSFORM_USE_ميٽرڪس
    إذا(lv_obj_get_transform(obj) != عدم) أرجع LV_LAYER_TYPE_TRANSFORM;
#جيڪڏهن_پورو
    إذا(lv_obj_get_style_transform_rotation_اندروني(obj, LV_PART_MAIN) != 0) أرجع LV_LAYER_TYPE_TRANSFORM;
    إذا(lv_obj_get_style_transform_scale_x_اندروني(obj, LV_PART_MAIN) != 256) أرجع LV_LAYER_TYPE_TRANSFORM;
    إذا(lv_obj_get_style_transform_scale_y_اندروني(obj, LV_PART_MAIN) != 256) أرجع LV_LAYER_TYPE_TRANSFORM;
    إذا(lv_obj_get_style_transform_skew_x_اندروني(obj, LV_PART_MAIN) != 0) أرجع LV_LAYER_TYPE_TRANSFORM;
    إذا(lv_obj_get_style_transform_skew_y_اندروني(obj, LV_PART_MAIN) != 0) أرجع LV_LAYER_TYPE_TRANSFORM;
    إذا(lv_obj_get_style_opa_layered_اندروني(obj, LV_PART_MAIN) != LV_OPA_COVER) أرجع LV_LAYER_TYPE_SIMPLE;
    إذا(lv_obj_get_style_bitmap_نقاب_src_اندروني(obj, LV_PART_MAIN) != عدم) أرجع LV_LAYER_TYPE_SIMPLE;
    إذا(lv_obj_get_style_blend_گهڻائي_اندروني(obj, LV_PART_MAIN) != LV_BLEND_گهڻائي_عام) أرجع LV_LAYER_TYPE_SIMPLE;
    أرجع LV_LAYER_TYPE_NONE;
}

/*True if any style attached to the widget enables blur or a drop shadow in the
 *widget's current state. The per-style has_group bitmask makes the common
 *no-blur case a cheap scan.*/
ساكن bool calculate_has_blur(lv_obj_t * obj)
{
    ثابت uint32_t گروهه_blur = (uint32_t)1 << lv_style_get_prop_گروهه(LV_STYLE_BLUR_RADIUS);
    ثابت uint32_t گروهه_dropshadow = (uint32_t)1 << lv_style_get_prop_گروهه(LV_STYLE_DROP_SHADOW_OPA);
    ثابت lv_state_t state = lv_obj_style_get_selector_state(lv_obj_get_state(obj));
    ثابت lv_state_t state_inv = ~state;
    lv_style_قدر_t v;
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        lv_obj_style_t * obj_style = &obj->styles[i];
        إذا(obj_style->is_disabled) استمر;

        lv_state_t state_style = lv_obj_style_get_selector_state(obj->styles[i].selector);
        إذا((state_style & state_inv)) استمر;

        إذا((obj_style->style->has_گروهه & گروهه_blur) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_BLUR_RADIUS, &v)) {
            إذا(v.انگ > 0) أرجع true;
        }
        إذا((obj_style->style->has_گروهه & گروهه_dropshadow) &&
           lv_style_get_prop(obj_style->style, LV_STYLE_DROP_SHADOW_OPA, &v)) {
            إذا(v.انگ > 0) أرجع true;
        }
    }

    أرجع false;
}

ساكن فراغ ڀريل_cache_refresh(lv_obj_t * obj, lv_part_t part)
{
#إذا LV_OBJ_STYLE_CACHE
    uint32_t i;
    إذا(part == LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_main_prop_is_سيٽ = 0;
        لكل(i = 0; i < obj->style_cnt; i++) {
            إذا(lv_obj_style_get_selector_part(obj->styles[i].selector) != LV_PART_MAIN) استمر;
            إذا(obj->styles[i].is_disabled) استمر;
            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            إذا(lv_style_is_const_اندروني(style)) {
                lv_style_const_prop_t * props = style->قدرون_and_props;
                لكل(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_main_prop_is_سيٽ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            وإلا {
                lv_style_prop_t * props = (lv_style_prop_t *)style->قدرون_and_props + style->prop_cnt * حجم(lv_style_قدر_t);
                لكل(j = 0; j < style->prop_cnt; j++) {
                    obj->style_main_prop_is_سيٽ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
    إذا(part != LV_PART_MAIN || part == LV_PART_ANY) {
        obj->style_other_prop_is_سيٽ = 0;
        لكل(i = 0; i < obj->style_cnt; i++) {
            إذا(lv_obj_style_get_selector_part(obj->styles[i].selector) == LV_PART_MAIN) استمر;
            إذا(obj->styles[i].is_disabled) استمر;

            lv_style_t * style = (lv_style_t *)obj->styles[i].style;
            uint32_t j;
            إذا(lv_style_is_const_اندروني(style)) {
                lv_style_const_prop_t * props = style->قدرون_and_props;
                لكل(j = 0; props[j].prop != LV_STYLE_PROP_INV; j++) {
                    obj->style_other_prop_is_سيٽ |= STYLE_PROP_SHIFTED(props[j].prop);
                }
            }
            وإلا {
                lv_style_prop_t * props = (lv_style_prop_t *)style->قدرون_and_props + style->prop_cnt * حجم(lv_style_قدر_t);
                لكل(j = 0; j < style->prop_cnt; j++) {
                    obj->style_other_prop_is_سيٽ |= STYLE_PROP_SHIFTED(props[j]);
                }
            }
        }
    }
#وإلا
    LV_اڻ_استعمال_ٿيل(obj);
    LV_اڻ_استعمال_ٿيل(part);
#جيڪڏهن_پورو
}

ساكن فراغ fade_anim_cb(فراغ * obj, int32_t v)
{
    lv_obj_سيٽ_style_opa(obj, v, 0);
}

ساكن فراغ fade_اندريون_anim_completed(lv_anim_t * a)
{
    lv_obj_remove_local_style_prop(a->var, LV_STYLE_OPA, 0);
}

ساكن bool style_has_flag(ثابت lv_style_t * style, uint32_t flag)
{
    إذا(lv_style_is_const_اندروني(style)) {
        lv_style_const_prop_t * props = style->قدرون_and_props;
        uint32_t i;
        لكل(i = 0; props[i].prop != LV_STYLE_PROP_INV; i++) {
            إذا(lv_style_prop_has_flag(props[i].prop, flag)) {
                أرجع true;
            }
        }
    }
    وإلا {
        lv_style_prop_t * props = (lv_style_prop_t *)style->قدرون_and_props + style->prop_cnt * حجم(lv_style_قدر_t);
        uint32_t i;
        لكل(i = 0; i < style->prop_cnt; i++) {
            إذا(lv_style_prop_has_flag(props[i], flag)) {
                أرجع true;
            }
        }
    }
    أرجع false;
}

ساكن lv_style_res_t get_selector_style_prop(ثابت lv_obj_t * obj, lv_style_selector_t selector, lv_style_prop_t prop,
                                              lv_style_قدر_t * قدر_act)
{
    lv_style_res_t مليو;
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*The happy path*/
#إذا LV_OBJ_STYLE_CACHE
    ثابت uint32_t prop_shifted = STYLE_PROP_SHIFTED(prop);
    إذا((part == LV_PART_MAIN ? obj->style_main_prop_is_سيٽ : obj->style_other_prop_is_سيٽ) & prop_shifted)
#جيڪڏهن_پورو
    {
        مليو = get_prop_core(obj, selector, prop, قدر_act);
        إذا(مليو == LV_STYLE_RES_مليو) أرجع LV_STYLE_RES_مليو;
    }

    خارجي ثابت uint8_t lv_style_builtin_prop_flag_lookup_جدول[];
    bool inheritable = false;
    إذا(prop < LV_STYLE_انگ_BUILT_اندريون_PROPS) {
        inheritable = lv_style_builtin_prop_flag_lookup_جدول[prop] & LV_STYLE_PROP_FLAG_INHERITABLE;
    }
    وإلا {
        إذا(style_custom_prop_flag_lookup_جدول_2 != عدم) {
            inheritable = style_custom_prop_flag_lookup_جدول_2[prop - LV_STYLE_انگ_BUILT_اندريون_PROPS] &
                          LV_STYLE_PROP_FLAG_INHERITABLE;
        }
    }

    إذا(inheritable) {
        /*If not found, check the `MAIN` style first, if already on the MAIN part go to the parent*/
        إذا(part != LV_PART_MAIN) part = LV_PART_MAIN;
        وإلا obj = obj->والد;

        طالما(obj) {
#إذا LV_OBJ_STYLE_CACHE
            إذا(obj->style_main_prop_is_سيٽ & prop_shifted)
#جيڪڏهن_پورو
            {
                selector = part | obj->state;
                مليو = get_prop_core(obj, selector, prop, قدر_act);
                إذا(مليو == LV_STYLE_RES_مليو) أرجع LV_STYLE_RES_مليو;
            }
            /*Check the parent too.*/
            obj = obj->والد;
        }
    }
    وإلا {
        /*Get the width and height from the class.
                * WIDTH and HEIGHT are not inherited so add them in the `else` to skip checking them for inherited properties */
        إذا(part == LV_PART_MAIN && (prop == LV_STYLE_ويڪر || prop == LV_STYLE_اوچائي)) {
            ثابت lv_obj_class_t * cls = obj->class_p;
            طالما(cls) {
                إذا(prop == LV_STYLE_ويڪر) {
                    إذا(cls->ويڪر_def != 0)  {
                        قدر_act->انگ = cls->ويڪر_def;
                        أرجع LV_STYLE_RES_مليو;
                    }
                }
                وإلا {
                    إذا(cls->اوچائي_def != 0) {
                        قدر_act->انگ = cls->اوچائي_def;
                        أرجع LV_STYLE_RES_مليو;
                    }
                }
                cls = cls->بنياد_class;
            }
        }
    }

    أرجع LV_STYLE_RES_NOT_مليو;
}

ساكن فراغ remove_style_core(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector, bool theme_only)
{
    LV_ASSERT(obj != عدم);
    lv_state_t state = lv_obj_style_get_selector_state(selector);
    lv_part_t part = lv_obj_style_get_selector_part(selector);

    /*If the draw size changes invalidate the old area first.
     *`style == NULL` means "any style" (e.g. `lv_obj_remove_style_all()`) so it can't be checked for flags.*/
    إذا((part == LV_PART_MAIN || part == LV_PART_ANY) &&
       (style == عدم || style_has_flag(style, DRAW_AREA_FLAGS))) {
        lv_obj_invalidate(obj);
    }

    uint32_t style_ڳڻپ = 0;
    bool deleted = false;

    لكل(uint32_t i = 0; i < obj->style_cnt; i++) {
        lv_state_t state_act = lv_obj_style_get_selector_state(obj->styles[i].selector);
        lv_part_t part_act = lv_obj_style_get_selector_part(obj->styles[i].selector);

        bool should_remove = true;
        إذا(theme_only && !obj->styles[i].is_theme) {
            should_remove = false;
        }
        وإلا إذا((state != LV_STATE_ANY && state_act != state) ||
                (part != LV_PART_ANY && part_act != part) ||
                (style != عدم && style != obj->styles[i].style)) {
            should_remove = false;
        }

        إذا(!should_remove) {
            /*Keep this entry: copy it down to the current write position if needed*/
            إذا(style_ڳڻپ != i) {
                obj->styles[style_ڳڻپ] = obj->styles[i];
            }
            style_ڳڻپ++;
            استمر;
        }

        /*This entry is being removed*/
        إذا(obj->styles[i].is_trans) {
            remove_trans_styles(obj, part, LV_STYLE_PROP_ANY, عدم);
        }
        إذا(obj->styles[i].is_local || obj->styles[i].is_trans) {
            إذا(obj->styles[i].style) lv_style_reset((lv_style_t *)obj->styles[i].style);
            lv_free((lv_style_t *)obj->styles[i].style);
            obj->styles[i].style = عدم;
        }
        deleted = true;
    }

    إذا(!deleted) {
        أرجع;
    }

    obj->style_cnt = style_ڳڻپ;
    إذا(style_ڳڻپ > 0) {
        obj->styles = lv_realloc(obj->styles, style_ڳڻپ * حجم(lv_obj_style_t));
    }
    وإلا {
        lv_free(obj->styles);
        obj->styles = عدم;
    }

    إذا(deleted) {
        ڀريل_cache_refresh(obj, part);
        lv_obj_refresh_style(obj, part, LV_STYLE_PROP_ANY);
    }
}

ساكن lv_obj_style_t * ڳوليو_style(lv_obj_t * obj, ثابت lv_style_t * style, lv_style_selector_t selector)
{
    uint32_t i;
    لكل(i = 0; i < obj->style_cnt; i++) {
        إذا(obj->styles[i].style == style && obj->styles[i].selector == selector) {
            أرجع &obj->styles[i];
        }
    }

    LV_LOG_WARN("%p style was not found on %p widget with %6" LV_PRIx32 " selector", (فراغ *)style, (فراغ *)obj, selector);
    أرجع عدم;
}



#إذا LV_USE_OBSERVER

ساكن فراغ bind_style_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_t * p = lv_observer_get_user_معلومات(observer);

    int32_t v = lv_subject_get_int(subject);
    bool en = (v == p->قدر);
    lv_obj_سيٽ_style_enabled(observer->هدف, p->style, p->selector, en);
}

ساكن فراغ bind_style_prop_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    bind_style_prop_t * p = lv_observer_get_user_معلومات(observer);

    lv_style_قدر_t style_v;
    إذا(subject->type == LV_SUBJECT_TYPE_INT) style_v.انگ = lv_subject_get_int(subject);
    وإلا إذا(subject->type == LV_SUBJECT_TYPE_COLOR) style_v.color = lv_subject_get_color(subject);
    وإلا إذا(subject->type == LV_SUBJECT_TYPE_اشارو) style_v.ptr = lv_subject_get_اشارو(subject);
    وإلا {
        LV_LOG_WARN("Not supported subject type (%"LV_PRIu32")", (uint32_t) subject->type);
        أرجع;
    }

    lv_obj_سيٽ_local_style_prop(observer->هدف, p->prop, style_v, p->selector);
}


#جيڪڏهن_پورو /*LV_USE_OBSERVER*/
