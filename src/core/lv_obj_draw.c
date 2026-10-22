/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ਸਥਿਰ ਸਤਰਅੰਦਰ lv_opa_t get_layer_opa(ਅਚਲ lv_obj_t * obj, lv_part_t part, ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc);
ਸਥਿਰ lv_color_t ਸਾਧਾਰਨ_apply_layer_recolor(ਅਚਲ lv_obj_t * obj, lv_part_t part,  ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc,
                                             lv_color_t color);
ਸਥਿਰ lv_color32_t image_apply_layer_recolor(ਅਚਲ lv_obj_t * obj, lv_part_t part,
                                              ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc, lv_color_t color, lv_opa_t opa);

ਸਥਿਰ ਖਾਲੀ drop_shadow_init(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc);

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ext_draw_ਮਾਪ_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ਖਾਲੀ lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ਅਧਾਰ);
    ਜੇ(part != LV_PART_MAIN) {
        ਜੇ(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_ਸਿਰਾ;
            ਵਾਪਸ;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, part);

    ਜੇ(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
            draw_dsc->bg_color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, bg_color);
            ਅਚਲ lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_ਅੰਦਰੂਨੀ(obj, part);
            ਜੇ(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, ਆਕਾਰ(*grad));
            }
            ਨਹੀਂਤਾਂ {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_ਅੰਦਰੂਨੀ(obj, part);
                ਜੇ(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->bg_grad.stops[1].color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_ਅੰਦਰੂਨੀ(obj, part);
                }
            }
        }
    }

    ਜੇ(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(draw_dsc->border_ਚੌੜਾਈ) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_ਅੰਦਰੂਨੀ(obj, part);
            ਜੇ(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_ਅੰਦਰੂਨੀ(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
                draw_dsc->border_color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, border_color);
            }
        }
    }

    ਜੇ(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_ਚੌੜਾਈ = lv_obj_get_style_outline_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(draw_dsc->outline_ਚੌੜਾਈ) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_ਅੰਦਰੂਨੀ(obj, part);
            ਜੇ(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_ਅੰਦਰੂਨੀ(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
                draw_dsc->outline_color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, outline_color);
            }
        }
    }

    ਜੇ(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_ਅੰਦਰੂਨੀ(obj, part);
            ਜੇ(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                ਜੇ(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_ਚਿੰਨ੍ਹ) {
                    draw_dsc->bg_image_ਚਿੰਨ੍ਹ_font = lv_obj_get_style_ਪਾਠ_font_ਅੰਦਰੂਨੀ(obj, part);
                    lv_color_t ਪਾਠ_color = lv_obj_get_style_ਪਾਠ_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->bg_image_recolor = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, ਪਾਠ_color);
                }
                ਨਹੀਂਤਾਂ {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_ਅੰਦਰੂਨੀ(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_ਅੰਦਰੂਨੀ(obj, part);
                    lv_color32_t ਨਤੀਜਾ = image_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = ਨਤੀਜਾ.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(ਨਤੀਜਾ.red, ਨਤੀਜਾ.green, ਨਤੀਜਾ.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_ਅੰਦਰੂਨੀ(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_ਅੰਦਰੂਨੀ(obj, part);
            }
        }
    }

    ਜੇ(draw_dsc->shadow_opa) {
        draw_dsc->shadow_ਚੌੜਾਈ = lv_obj_get_style_shadow_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(draw_dsc->shadow_ਚੌੜਾਈ) {
            ਜੇ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_ਅੰਦਰੂਨੀ(obj, part);
                ਜੇ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_ਵਿੱਥ_x = lv_obj_get_style_shadow_ਵਿੱਥ_x_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->shadow_ਵਿੱਥ_y = lv_obj_get_style_shadow_ਵਿੱਥ_y_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->shadow_ਫੈਲਾਅ = lv_obj_get_style_shadow_ਫੈਲਾਅ_ਅੰਦਰੂਨੀ(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
                    draw_dsc->shadow_color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, shadow_color);
                }
            }
        }
    }

    ਜੇ(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->ਅਧਾਰ);

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    draw_dsc->opa = lv_obj_get_style_ਪਾਠ_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ਅਧਾਰ);
    ਜੇ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }


    lv_color_t ਪਾਠ_color = lv_obj_get_style_ਪਾਠ_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, ਪਾਠ_color);
    draw_dsc->letter_space = lv_obj_get_style_ਪਾਠ_letter_space_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->ਸਤਰ_space = lv_obj_get_style_ਪਾਠ_ਸਤਰ_space_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->decor = lv_obj_get_style_ਪਾਠ_decor_ਅੰਦਰੂਨੀ(obj, part);

    draw_dsc->font = lv_obj_get_style_ਪਾਠ_font_ਅੰਦਰੂਨੀ(obj, part);

#ਜੇ LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
#ਜੇ_ਅੰਤ

    draw_dsc->align = lv_obj_get_style_ਪਾਠ_align_ਅੰਦਰੂਨੀ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ਅਧਾਰ);

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ਅਧਾਰ);
    ਜੇ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->ਧੁਰਾ.x = lv_area_get_ਚੌੜਾਈ(&obj->coords) / 2;
    draw_dsc->ਧੁਰਾ.y = lv_area_get_ਉਚਾਈ(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_ਅੰਦਰੂਨੀ(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_ਅੰਦਰੂਨੀ(obj, part);
    lv_color32_t ਨਤੀਜਾ = image_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, recolor, recolor_opa);
    draw_dsc->recolor_opa = ਨਤੀਜਾ.alpha;
    draw_dsc->recolor = lv_color_make(ਨਤੀਜਾ.red, ਨਤੀਜਾ.green, ਨਤੀਜਾ.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_ਅੰਦਰੂਨੀ(obj, part);

    ਜੇ(part != LV_PART_MAIN) draw_dsc->blend_ਬਹੁਲਕ = lv_obj_get_style_blend_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ਅਧਾਰ);

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_init_draw_ਸਤਰ_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_ਸਤਰ_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    draw_dsc->opa = lv_obj_get_style_ਸਤਰ_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ਅਧਾਰ);
    ਜੇ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    draw_dsc->ਚੌੜਾਈ = lv_obj_get_style_ਸਤਰ_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->ਚੌੜਾਈ == 0) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_color_t ਸਤਰ_color = lv_obj_get_style_ਸਤਰ_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, ਸਤਰ_color);

    draw_dsc->dash_ਚੌੜਾਈ = lv_obj_get_style_ਸਤਰ_dash_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->dash_ਚੌੜਾਈ) {
        draw_dsc->dash_gap = lv_obj_get_style_ਸਤਰ_dash_gap_ਅੰਦਰੂਨੀ(obj, part);
    }

    draw_dsc->ਗੇੜਾ_ਸ਼ੁਰੂ = lv_obj_get_style_ਸਤਰ_rounded_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->ਗੇੜਾ_ਸਿਰਾ = draw_dsc->ਗੇੜਾ_ਸ਼ੁਰੂ;

    drop_shadow_init(obj, part, &draw_dsc->ਅਧਾਰ);

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    draw_dsc->ਚੌੜਾਈ = lv_obj_get_style_arc_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->ਚੌੜਾਈ == 0) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ਅਧਾਰ);
    ਜੇ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ਜੇ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, &draw_dsc->ਅਧਾਰ, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_ਅੰਦਰੂਨੀ(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_ਅੰਦਰੂਨੀ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ਅਧਾਰ);

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(draw_dsc != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ਅਧਾਰ.obj = obj;
    draw_dsc->ਅਧਾਰ.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_ਅੰਦਰੂਨੀ(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_ਅੰਦਰੂਨੀ(obj, part);

    /*Radius might be set earlier as it's already known*/
    ਜੇ(draw_dsc->ਕੋਨਾ_radius == 0) {
        draw_dsc->ਕੋਨਾ_radius = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, part);
    }

    LV_PROFILER_DRAW_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ext_draw_ਮਾਪ(lv_obj_t * obj, int32_t ਮਾਪ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_remove_event_cb(obj, ਸੈੱਟ_ext_draw_ਮਾਪ_event_cb);
    lv_obj_ਜੋੜੋ_event_cb(obj, ਸੈੱਟ_ext_draw_ਮਾਪ_event_cb, LV_EVENT_REFR_EXT_DRAW_ਮਾਪ, (ਖਾਲੀ *)(lv_uintptr_t)ਮਾਪ);
}

int32_t lv_obj_calculate_ext_draw_ਮਾਪ(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_ਚੌੜਾਈ = lv_obj_get_style_shadow_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(sh_ਚੌੜਾਈ) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(sh_opa > LV_OPA_MIN) {
            sh_ਚੌੜਾਈ = sh_ਚੌੜਾਈ / 2 + 1;    /*The blur adds only half width*/
            sh_ਚੌੜਾਈ += lv_obj_get_style_shadow_ਫੈਲਾਅ_ਅੰਦਰੂਨੀ(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_ਵਿੱਥ_x_ਅੰਦਰੂਨੀ(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_ਵਿੱਥ_y_ਅੰਦਰੂਨੀ(obj, part);
            sh_ਚੌੜਾਈ += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_ਚੌੜਾਈ);
        }
    }

    int32_t outline_ਚੌੜਾਈ = lv_obj_get_style_outline_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(outline_ਚੌੜਾਈ) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_ਅੰਦਰੂਨੀ(obj, part);
        ਜੇ(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_ਅੰਦਰੂਨੀ(obj, part);
            s = LV_MAX(s, outline_pad + outline_ਚੌੜਾਈ);
        }
    }

    int32_t drop_shadow_ਮਾਪ = 0;
    ਜੇ(lv_obj_get_style_drop_shadow_opa_ਅੰਦਰੂਨੀ(obj, part) > 0) {
        drop_shadow_ਮਾਪ += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_ਵਿੱਥ_x_ਅੰਦਰੂਨੀ(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_ਵਿੱਥ_y_ਅੰਦਰੂਨੀ(obj, part)));

        drop_shadow_ਮਾਪ += lv_obj_get_style_drop_shadow_radius_ਅੰਦਰੂਨੀ(obj, part) + 1;
    }
    s += drop_shadow_ਮਾਪ;

    int32_t w = lv_obj_get_style_transform_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    int32_t h = lv_obj_get_style_transform_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, part);
    int32_t wh = LV_MAX(w, h);
    ਜੇ(wh > 0) s += wh;

    LV_PROFILER_DRAW_ਸਿਰਾ;
    ਵਾਪਸ s;
}

ਖਾਲੀ lv_obj_refresh_ext_draw_ਮਾਪ(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    int32_t s_old = lv_obj_get_ext_draw_ਮਾਪ(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_ਮਾਪ, &s_new);

    /*Store the result if the special attrs already allocated*/
    ਜੇ(obj->spec_attr) {
        obj->spec_attr->ext_draw_ਮਾਪ = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    ਨਹੀਂਤਾਂ ਜੇ(s_new != 0) {
        ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_ਸਿਰਾ;
            ਵਾਪਸ;
        }
        obj->spec_attr->ext_draw_ਮਾਪ = s_new;
    }

    ਜੇ(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_ਸਿਰਾ;
}

int32_t lv_obj_get_ext_draw_ਮਾਪ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਜੇ(obj->spec_attr) ਵਾਪਸ obj->spec_attr->ext_draw_ਮਾਪ;
    ਨਹੀਂਤਾਂ ਵਾਪਸ 0;
}

lv_layer_type_t lv_obj_get_layer_type(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_LAYER_TYPE_NONE);

    ਜੇ(obj->spec_attr) ਵਾਪਸ (lv_layer_type_t)obj->spec_attr->layer_type;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਸਤਰਅੰਦਰ lv_opa_t get_layer_opa(ਅਚਲ lv_obj_t * obj, lv_part_t part, ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc)
{
    ਜੇ(ਅਧਾਰ_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        ਜੇ(part == LV_PART_MAIN) {
            ਵਾਪਸ ਅਧਾਰ_dsc->layer->opa;
        }

        ਵਾਪਸ LV_OPA_MIX2(ਅਧਾਰ_dsc->layer->opa, lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, part));
    }

    /* fallback to old recursive style opa */
    ਵਾਪਸ lv_obj_get_style_opa_recursive(obj, part);
}


ਸਥਿਰ lv_color_t ਸਾਧਾਰਨ_apply_layer_recolor(ਅਚਲ lv_obj_t * obj, lv_part_t part,  ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    ਜੇ(ਅਧਾਰ_dsc->layer) {
        recolor = ਅਧਾਰ_dsc->layer->recolor;
        ਜੇ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ਨਹੀਂਤਾਂ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ਵਾਪਸ lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


ਸਥਿਰ lv_color32_t image_apply_layer_recolor(ਅਚਲ lv_obj_t * obj, lv_part_t part,
                                              ਅਚਲ lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    ਜੇ(ਅਧਾਰ_dsc->layer) {
        recolor = ਅਧਾਰ_dsc->layer->recolor;
        ਜੇ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ਨਹੀਂਤਾਂ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ਜੇ(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        ਵਾਪਸ lv_color_over32(recolor, lv_color_ਮੰਜ਼ਿਲ_32(color, opa));
    }
    ਨਹੀਂਤਾਂ ਜੇ(recolor.alpha > LV_OPA_TRANSP) {
        ਵਾਪਸ recolor;
    }
    ਨਹੀਂਤਾਂ {
        ਵਾਪਸ lv_color_ਮੰਜ਼ਿਲ_32(color, opa);
    }
}

ਸਥਿਰ ਖਾਲੀ drop_shadow_init(ਅਚਲ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ਅਧਾਰ_t * ਅਧਾਰ_dsc)
{
    ਅਧਾਰ_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_ਅੰਦਰੂਨੀ(obj, part);
    ਜੇ(ਅਧਾਰ_dsc->drop_shadow_opa) {
        ਅਧਾਰ_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_ਅੰਦਰੂਨੀ(obj, part);
        ਅਧਾਰ_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_ਵਿੱਥ_x_ਅੰਦਰੂਨੀ(obj, part);
        ਅਧਾਰ_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_ਵਿੱਥ_y_ਅੰਦਰੂਨੀ(obj, part);
        ਅਧਾਰ_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_ਅੰਦਰੂਨੀ(obj, part);
        ਅਧਾਰ_dsc->drop_shadow_color = ਸਾਧਾਰਨ_apply_layer_recolor(obj, part, ਅਧਾਰ_dsc, ਅਧਾਰ_dsc->drop_shadow_color);

        ਅਧਾਰ_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_ਅੰਦਰੂਨੀ(obj, part);
    }
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ext_draw_ਮਾਪ_event_cb(lv_event_t * e)
{
    int32_t ਮਾਪ = (int32_t)(lv_uintptr_t)lv_event_get_user_ਅੰਕੜੇ(e);
    lv_event_ਸੈੱਟ_ext_draw_ਮਾਪ(e, ਮਾਪ);
}
