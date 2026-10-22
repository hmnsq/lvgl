/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_draw_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ lv_opa_t get_layer_opa(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, ಧ್ರುವ lv_draw_dsc_base_t * base_dsc);
ಸ್ಥಿರ lv_color_t normal_apply_layer_recolor(ಧ್ರುವ lv_obj_t * obj, lv_part_t part,  ಧ್ರುವ lv_draw_dsc_base_t * base_dsc,
                                             lv_color_t color);
ಸ್ಥಿರ lv_color32_t image_apply_layer_recolor(ಧ್ರುವ lv_obj_t * obj, lv_part_t part,
                                              ಧ್ರುವ lv_draw_dsc_base_t * base_dsc, lv_color_t color, lv_opa_t opa);

ಸ್ಥಿರ ಶೂನ್ಯ drop_shadow_init(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_base_t * base_dsc);

ಸ್ಥಿರ ಶೂನ್ಯ set_ext_draw_size_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->base);
    ಯದಿ(part != LV_PART_MAIN) {
        ಯದಿ(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_END;
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_internal(obj, part);

    ಯದಿ(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_internal(obj, part);
        ಯದಿ(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_internal(obj, part);
            draw_dsc->bg_color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, bg_color);
            ಧ್ರುವ lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_internal(obj, part);
            ಯದಿ(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, ಪರಿಮಾಣ(*grad));
            }
            ಅನ್ಯಥಾ {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_internal(obj, part);
                ಯದಿ(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_internal(obj, part);
                    draw_dsc->bg_grad.stops[1].color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_internal(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_internal(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_internal(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_internal(obj, part);
                }
            }
        }
    }

    ಯದಿ(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_width = lv_obj_get_style_border_width_internal(obj, part);
        ಯದಿ(draw_dsc->border_width) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_internal(obj, part);
            ಯದಿ(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_internal(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_internal(obj, part);
                draw_dsc->border_color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, border_color);
            }
        }
    }

    ಯದಿ(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_width = lv_obj_get_style_outline_width_internal(obj, part);
        ಯದಿ(draw_dsc->outline_width) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_internal(obj, part);
            ಯದಿ(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_internal(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_internal(obj, part);
                draw_dsc->outline_color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, outline_color);
            }
        }
    }

    ಯದಿ(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_internal(obj, part);
        ಯದಿ(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_internal(obj, part);
            ಯದಿ(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                ಯದಿ(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_ಚಿಹ್ನೆ) {
                    draw_dsc->bg_image_ಚಿಹ್ನೆ_font = lv_obj_get_style_text_font_internal(obj, part);
                    lv_color_t text_color = lv_obj_get_style_text_color_filtered_internal(obj, part);
                    draw_dsc->bg_image_recolor = normal_apply_layer_recolor(obj, part, &draw_dsc->base, text_color);
                }
                ಅನ್ಯಥಾ {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_internal(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_internal(obj, part);
                    lv_color32_t result = image_apply_layer_recolor(obj, part, &draw_dsc->base, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = result.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(result.red, result.green, result.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_internal(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_internal(obj, part);
            }
        }
    }

    ಯದಿ(draw_dsc->shadow_opa) {
        draw_dsc->shadow_width = lv_obj_get_style_shadow_width_internal(obj, part);
        ಯದಿ(draw_dsc->shadow_width) {
            ಯದಿ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_internal(obj, part);
                ಯದಿ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_offset_x = lv_obj_get_style_shadow_offset_x_internal(obj, part);
                    draw_dsc->shadow_offset_y = lv_obj_get_style_shadow_offset_y_internal(obj, part);
                    draw_dsc->shadow_spread = lv_obj_get_style_shadow_spread_internal(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_internal(obj, part);
                    draw_dsc->shadow_color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, shadow_color);
                }
            }
        }
    }

    ಯದಿ(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->base);

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    draw_dsc->opa = lv_obj_get_style_text_opa_internal(obj, part);
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->base);
    ಯದಿ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }


    lv_color_t text_color = lv_obj_get_style_text_color_filtered_internal(obj, part);
    draw_dsc->color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, text_color);
    draw_dsc->letter_space = lv_obj_get_style_text_letter_space_internal(obj, part);
    draw_dsc->ಸಾಲು_space = lv_obj_get_style_text_ಸಾಲು_space_internal(obj, part);
    draw_dsc->decor = lv_obj_get_style_text_decor_internal(obj, part);

    draw_dsc->font = lv_obj_get_style_text_font_internal(obj, part);

#ಯದಿ LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN);
#endif

    draw_dsc->align = lv_obj_get_style_text_align_internal(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->base);

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_internal(obj, part);
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->base);
    ಯದಿ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->pivot.x = lv_area_get_width(&obj->coords) / 2;
    draw_dsc->pivot.y = lv_area_get_height(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_internal(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_internal(obj, part);
    lv_color32_t result = image_apply_layer_recolor(obj, part, &draw_dsc->base, recolor, recolor_opa);
    draw_dsc->recolor_opa = result.alpha;
    draw_dsc->recolor = lv_color_make(result.red, result.green, result.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_internal(obj, part);

    ಯದಿ(part != LV_PART_MAIN) draw_dsc->blend_mode = lv_obj_get_style_blend_mode_internal(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->base);

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_init_draw_ಸಾಲು_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_ಸಾಲು_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    draw_dsc->opa = lv_obj_get_style_ಸಾಲು_opa_internal(obj, part);
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->base);
    ಯದಿ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    draw_dsc->width = lv_obj_get_style_ಸಾಲು_width_internal(obj, part);
    ಯದಿ(draw_dsc->width == 0) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_color_t ಸಾಲು_color = lv_obj_get_style_ಸಾಲು_color_filtered_internal(obj, part);
    draw_dsc->color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, ಸಾಲು_color);

    draw_dsc->dash_width = lv_obj_get_style_ಸಾಲು_dash_width_internal(obj, part);
    ಯದಿ(draw_dsc->dash_width) {
        draw_dsc->dash_gap = lv_obj_get_style_ಸಾಲು_dash_gap_internal(obj, part);
    }

    draw_dsc->round_start = lv_obj_get_style_ಸಾಲು_rounded_internal(obj, part);
    draw_dsc->round_end = draw_dsc->round_start;

    drop_shadow_init(obj, part, &draw_dsc->base);

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    draw_dsc->width = lv_obj_get_style_arc_width_internal(obj, part);
    ಯದಿ(draw_dsc->width == 0) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_internal(obj, part);
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->base);
    ಯದಿ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಯದಿ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_END;
        ಪ್ರತ್ಯಾಗಮ;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_internal(obj, part);
    draw_dsc->color = normal_apply_layer_recolor(obj, part, &draw_dsc->base, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_internal(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_internal(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->base);

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(draw_dsc != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->base.obj = obj;
    draw_dsc->base.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_internal(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_internal(obj, part);

    /*Radius might be set earlier as it's already known*/
    ಯದಿ(draw_dsc->corner_radius == 0) {
        draw_dsc->corner_radius = lv_obj_get_style_radius_internal(obj, part);
    }

    LV_PROFILER_DRAW_END;
}

ಶೂನ್ಯ lv_obj_set_ext_draw_size(lv_obj_t * obj, int32_t size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    lv_obj_remove_event_cb(obj, set_ext_draw_size_event_cb);
    lv_obj_add_event_cb(obj, set_ext_draw_size_event_cb, LV_EVENT_REFR_EXT_DRAW_SIZE, (ಶೂನ್ಯ *)(lv_uintptr_t)size);
}

int32_t lv_obj_calculate_ext_draw_size(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_width = lv_obj_get_style_shadow_width_internal(obj, part);
    ಯದಿ(sh_width) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_internal(obj, part);
        ಯದಿ(sh_opa > LV_OPA_MIN) {
            sh_width = sh_width / 2 + 1;    /*The blur adds only half width*/
            sh_width += lv_obj_get_style_shadow_spread_internal(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_offset_x_internal(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_offset_y_internal(obj, part);
            sh_width += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_width);
        }
    }

    int32_t outline_width = lv_obj_get_style_outline_width_internal(obj, part);
    ಯದಿ(outline_width) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_internal(obj, part);
        ಯದಿ(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_internal(obj, part);
            s = LV_MAX(s, outline_pad + outline_width);
        }
    }

    int32_t drop_shadow_size = 0;
    ಯದಿ(lv_obj_get_style_drop_shadow_opa_internal(obj, part) > 0) {
        drop_shadow_size += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_offset_x_internal(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_offset_y_internal(obj, part)));

        drop_shadow_size += lv_obj_get_style_drop_shadow_radius_internal(obj, part) + 1;
    }
    s += drop_shadow_size;

    int32_t w = lv_obj_get_style_transform_width_internal(obj, part);
    int32_t h = lv_obj_get_style_transform_height_internal(obj, part);
    int32_t wh = LV_MAX(w, h);
    ಯದಿ(wh > 0) s += wh;

    LV_PROFILER_DRAW_END;
    ಪ್ರತ್ಯಾಗಮ s;
}

ಶೂನ್ಯ lv_obj_refresh_ext_draw_size(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ);

    int32_t s_old = lv_obj_get_ext_draw_size(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_SIZE, &s_new);

    /*Store the result if the special attrs already allocated*/
    ಯದಿ(obj->spec_attr) {
        obj->spec_attr->ext_draw_size = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    ಅನ್ಯಥಾ ಯದಿ(s_new != 0) {
        ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_END;
            ಪ್ರತ್ಯಾಗಮ;
        }
        obj->spec_attr->ext_draw_size = s_new;
    }

    ಯದಿ(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_END;
}

int32_t lv_obj_get_ext_draw_size(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ 0);

    ಯದಿ(obj->spec_attr) ಪ್ರತ್ಯಾಗಮ obj->spec_attr->ext_draw_size;
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ 0;
}

lv_layer_type_t lv_obj_get_layer_type(ಧ್ರುವ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_NONE);

    ಯದಿ(obj->spec_attr) ಪ್ರತ್ಯಾಗಮ (lv_layer_type_t)obj->spec_attr->layer_type;
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ lv_opa_t get_layer_opa(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, ಧ್ರುವ lv_draw_dsc_base_t * base_dsc)
{
    ಯದಿ(base_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        ಯದಿ(part == LV_PART_MAIN) {
            ಪ್ರತ್ಯಾಗಮ base_dsc->layer->opa;
        }

        ಪ್ರತ್ಯಾಗಮ LV_OPA_MIX2(base_dsc->layer->opa, lv_obj_get_style_opa_internal(obj, part));
    }

    /* fallback to old recursive style opa */
    ಪ್ರತ್ಯಾಗಮ lv_obj_get_style_opa_recursive(obj, part);
}


ಸ್ಥಿರ lv_color_t normal_apply_layer_recolor(ಧ್ರುವ lv_obj_t * obj, lv_part_t part,  ಧ್ರುವ lv_draw_dsc_base_t * base_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    ಯದಿ(base_dsc->layer) {
        recolor = base_dsc->layer->recolor;
        ಯದಿ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ಅನ್ಯಥಾ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ಪ್ರತ್ಯಾಗಮ lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


ಸ್ಥಿರ lv_color32_t image_apply_layer_recolor(ಧ್ರುವ lv_obj_t * obj, lv_part_t part,
                                              ಧ್ರುವ lv_draw_dsc_base_t * base_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    ಯದಿ(base_dsc->layer) {
        recolor = base_dsc->layer->recolor;
        ಯದಿ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ಅನ್ಯಥಾ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ಯದಿ(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        ಪ್ರತ್ಯಾಗಮ lv_color_over32(recolor, lv_color_to_32(color, opa));
    }
    ಅನ್ಯಥಾ ಯದಿ(recolor.alpha > LV_OPA_TRANSP) {
        ಪ್ರತ್ಯಾಗಮ recolor;
    }
    ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ lv_color_to_32(color, opa);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ drop_shadow_init(ಧ್ರುವ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_base_t * base_dsc)
{
    base_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_internal(obj, part);
    ಯದಿ(base_dsc->drop_shadow_opa) {
        base_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_internal(obj, part);
        base_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_offset_x_internal(obj, part);
        base_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_offset_y_internal(obj, part);
        base_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_internal(obj, part);
        base_dsc->drop_shadow_color = normal_apply_layer_recolor(obj, part, base_dsc, base_dsc->drop_shadow_color);

        base_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_internal(obj, part);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ set_ext_draw_size_event_cb(lv_event_t * e)
{
    int32_t size = (int32_t)(lv_uintptr_t)lv_event_get_user_data(e);
    lv_event_set_ext_draw_size(e, size);
}
