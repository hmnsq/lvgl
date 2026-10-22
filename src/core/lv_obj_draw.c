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
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ lv_opa_t get_layer_opa(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc);
ಸ್ಥಿರ lv_color_t ಸಾಮಾನ್ಯ_apply_layer_recolor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part,  ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc,
                                             lv_color_t color);
ಸ್ಥಿರ lv_color32_t image_apply_layer_recolor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part,
                                              ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc, lv_color_t color, lv_opa_t opa);

ಸ್ಥಿರ ಶೂನ್ಯ drop_shadow_init(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ತಳ_t * ತಳ_dsc);

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ext_draw_ಗಾತ್ರ_event_cb(lv_event_t * e);

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
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ತಳ);
    ಆದರೆ(part != LV_PART_MAIN) {
        ಆದರೆ(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_ಕೊನೆ;
            ಹಿಂತಿರುಗಿ;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, part);

    ಆದರೆ(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_ಆಂತರಿಕ(obj, part);
            draw_dsc->bg_color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, bg_color);
            ಸ್ಥಿರಾಂಕ lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_ಆಂತರಿಕ(obj, part);
            ಆದರೆ(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, ಗಾತ್ರ(*grad));
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_ಆಂತರಿಕ(obj, part);
                ಆದರೆ(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_ಆಂತರಿಕ(obj, part);
                    draw_dsc->bg_grad.stops[1].color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_ಆಂತರಿಕ(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_ಆಂತರಿಕ(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_ಆಂತರಿಕ(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_ಆಂತರಿಕ(obj, part);
                }
            }
        }
    }

    ಆದರೆ(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(draw_dsc->border_ಅಗಲ) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_ಆಂತರಿಕ(obj, part);
            ಆದರೆ(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_ಆಂತರಿಕ(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_ಆಂತರಿಕ(obj, part);
                draw_dsc->border_color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, border_color);
            }
        }
    }

    ಆದರೆ(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_ಅಗಲ = lv_obj_get_style_outline_ಅಗಲ_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(draw_dsc->outline_ಅಗಲ) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_ಆಂತರಿಕ(obj, part);
            ಆದರೆ(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_ಆಂತರಿಕ(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_ಆಂತರಿಕ(obj, part);
                draw_dsc->outline_color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, outline_color);
            }
        }
    }

    ಆದರೆ(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_ಆಂತರಿಕ(obj, part);
            ಆದರೆ(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                ಆದರೆ(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_ಚಿಹ್ನೆ) {
                    draw_dsc->bg_image_ಚಿಹ್ನೆ_font = lv_obj_get_style_ಪಠ್ಯ_font_ಆಂತರಿಕ(obj, part);
                    lv_color_t ಪಠ್ಯ_color = lv_obj_get_style_ಪಠ್ಯ_color_filtered_ಆಂತರಿಕ(obj, part);
                    draw_dsc->bg_image_recolor = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, ಪಠ್ಯ_color);
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_ಆಂತರಿಕ(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_ಆಂತರಿಕ(obj, part);
                    lv_color32_t ಫಲಿತಾಂಶ = image_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = ಫಲಿತಾಂಶ.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(ಫಲಿತಾಂಶ.red, ಫಲಿತಾಂಶ.green, ಫಲಿತಾಂಶ.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_ಆಂತರಿಕ(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_ಆಂತರಿಕ(obj, part);
            }
        }
    }

    ಆದರೆ(draw_dsc->shadow_opa) {
        draw_dsc->shadow_ಅಗಲ = lv_obj_get_style_shadow_ಅಗಲ_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(draw_dsc->shadow_ಅಗಲ) {
            ಆದರೆ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_ಆಂತರಿಕ(obj, part);
                ಆದರೆ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_ಸ್ಥಾನಾಂತರ_x = lv_obj_get_style_shadow_ಸ್ಥಾನಾಂತರ_x_ಆಂತರಿಕ(obj, part);
                    draw_dsc->shadow_ಸ್ಥಾನಾಂತರ_y = lv_obj_get_style_shadow_ಸ್ಥಾನಾಂತರ_y_ಆಂತರಿಕ(obj, part);
                    draw_dsc->shadow_ಹರಡುವಿಕೆ = lv_obj_get_style_shadow_ಹರಡುವಿಕೆ_ಆಂತರಿಕ(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_ಆಂತರಿಕ(obj, part);
                    draw_dsc->shadow_color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, shadow_color);
                }
            }
        }
    }

    ಆದರೆ(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->ತಳ);

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    draw_dsc->opa = lv_obj_get_style_ಪಠ್ಯ_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ತಳ);
    ಆದರೆ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }


    lv_color_t ಪಠ್ಯ_color = lv_obj_get_style_ಪಠ್ಯ_color_filtered_ಆಂತರಿಕ(obj, part);
    draw_dsc->color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, ಪಠ್ಯ_color);
    draw_dsc->letter_space = lv_obj_get_style_ಪಠ್ಯ_letter_space_ಆಂತರಿಕ(obj, part);
    draw_dsc->ಸಾಲು_space = lv_obj_get_style_ಪಠ್ಯ_ಸಾಲು_space_ಆಂತರಿಕ(obj, part);
    draw_dsc->decor = lv_obj_get_style_ಪಠ್ಯ_decor_ಆಂತರಿಕ(obj, part);

    draw_dsc->font = lv_obj_get_style_ಪಠ್ಯ_font_ಆಂತರಿಕ(obj, part);

#ಆದರೆ LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN);
#ಆದರೆ_ಅಂತ್ಯ

    draw_dsc->align = lv_obj_get_style_ಪಠ್ಯ_align_ಆಂತರಿಕ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ತಳ);

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ತಳ);
    ಆದರೆ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->ಅಚ್ಚು.x = lv_area_get_ಅಗಲ(&obj->coords) / 2;
    draw_dsc->ಅಚ್ಚು.y = lv_area_get_ಎತ್ತರ(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_ಆಂತರಿಕ(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_ಆಂತರಿಕ(obj, part);
    lv_color32_t ಫಲಿತಾಂಶ = image_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, recolor, recolor_opa);
    draw_dsc->recolor_opa = ಫಲಿತಾಂಶ.alpha;
    draw_dsc->recolor = lv_color_make(ಫಲಿತಾಂಶ.red, ಫಲಿತಾಂಶ.green, ಫಲಿತಾಂಶ.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_ಆಂತರಿಕ(obj, part);

    ಆದರೆ(part != LV_PART_MAIN) draw_dsc->blend_ಬಹುಲಕ = lv_obj_get_style_blend_ಬಹುಲಕ_ಆಂತರಿಕ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ತಳ);

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_init_draw_ಸಾಲು_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_ಸಾಲು_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    draw_dsc->opa = lv_obj_get_style_ಸಾಲು_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ತಳ);
    ಆದರೆ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    draw_dsc->ಅಗಲ = lv_obj_get_style_ಸಾಲು_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->ಅಗಲ == 0) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_color_t ಸಾಲು_color = lv_obj_get_style_ಸಾಲು_color_filtered_ಆಂತರಿಕ(obj, part);
    draw_dsc->color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, ಸಾಲು_color);

    draw_dsc->dash_ಅಗಲ = lv_obj_get_style_ಸಾಲು_dash_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->dash_ಅಗಲ) {
        draw_dsc->dash_gap = lv_obj_get_style_ಸಾಲು_dash_gap_ಆಂತರಿಕ(obj, part);
    }

    draw_dsc->ಸುತ್ತು_ಆರಂಭ = lv_obj_get_style_ಸಾಲು_rounded_ಆಂತರಿಕ(obj, part);
    draw_dsc->ಸುತ್ತು_ಕೊನೆ = draw_dsc->ಸುತ್ತು_ಆರಂಭ;

    drop_shadow_init(obj, part, &draw_dsc->ತಳ);

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    draw_dsc->ಅಗಲ = lv_obj_get_style_arc_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->ಅಗಲ == 0) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ತಳ);
    ಆದರೆ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ಆದರೆ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_ಆಂತರಿಕ(obj, part);
    draw_dsc->color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, &draw_dsc->ತಳ, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_ಆಂತರಿಕ(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_ಆಂತರಿಕ(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ತಳ);

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(draw_dsc != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ತಳ.obj = obj;
    draw_dsc->ತಳ.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_ಆಂತರಿಕ(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_ಆಂತರಿಕ(obj, part);

    /*Radius might be set earlier as it's already known*/
    ಆದರೆ(draw_dsc->ಮೂಲೆ_radius == 0) {
        draw_dsc->ಮೂಲೆ_radius = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, part);
    }

    LV_PROFILER_DRAW_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_ext_draw_ಗಾತ್ರ(lv_obj_t * obj, int32_t ಗಾತ್ರ_2)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_remove_event_cb(obj, ಗಣ_ext_draw_ಗಾತ್ರ_event_cb);
    lv_obj_ಕೂಡಿಸು_event_cb(obj, ಗಣ_ext_draw_ಗಾತ್ರ_event_cb, LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ, (ಶೂನ್ಯ *)(lv_uintptr_t)ಗಾತ್ರ_2);
}

int32_t lv_obj_calculate_ext_draw_ಗಾತ್ರ(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_ಅಗಲ = lv_obj_get_style_shadow_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(sh_ಅಗಲ) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(sh_opa > LV_OPA_MIN) {
            sh_ಅಗಲ = sh_ಅಗಲ / 2 + 1;    /*The blur adds only half width*/
            sh_ಅಗಲ += lv_obj_get_style_shadow_ಹರಡುವಿಕೆ_ಆಂತರಿಕ(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_ಸ್ಥಾನಾಂತರ_x_ಆಂತರಿಕ(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_ಸ್ಥಾನಾಂತರ_y_ಆಂತರಿಕ(obj, part);
            sh_ಅಗಲ += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_ಅಗಲ);
        }
    }

    int32_t outline_ಅಗಲ = lv_obj_get_style_outline_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(outline_ಅಗಲ) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_ಆಂತರಿಕ(obj, part);
        ಆದರೆ(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_ಆಂತರಿಕ(obj, part);
            s = LV_MAX(s, outline_pad + outline_ಅಗಲ);
        }
    }

    int32_t drop_shadow_ಗಾತ್ರ = 0;
    ಆದರೆ(lv_obj_get_style_drop_shadow_opa_ಆಂತರಿಕ(obj, part) > 0) {
        drop_shadow_ಗಾತ್ರ += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_ಸ್ಥಾನಾಂತರ_x_ಆಂತರಿಕ(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_ಸ್ಥಾನಾಂತರ_y_ಆಂತರಿಕ(obj, part)));

        drop_shadow_ಗಾತ್ರ += lv_obj_get_style_drop_shadow_radius_ಆಂತರಿಕ(obj, part) + 1;
    }
    s += drop_shadow_ಗಾತ್ರ;

    int32_t w = lv_obj_get_style_transform_ಅಗಲ_ಆಂತರಿಕ(obj, part);
    int32_t h = lv_obj_get_style_transform_ಎತ್ತರ_ಆಂತರಿಕ(obj, part);
    int32_t wh = LV_MAX(w, h);
    ಆದರೆ(wh > 0) s += wh;

    LV_PROFILER_DRAW_ಕೊನೆ;
    ಹಿಂತಿರುಗಿ s;
}

ಶೂನ್ಯ lv_obj_refresh_ext_draw_ಗಾತ್ರ(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    int32_t s_old = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_ಗಾತ್ರ, &s_new);

    /*Store the result if the special attrs already allocated*/
    ಆದರೆ(obj->spec_attr) {
        obj->spec_attr->ext_draw_ಗಾತ್ರ = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(s_new != 0) {
        ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_ಕೊನೆ;
            ಹಿಂತಿರುಗಿ;
        }
        obj->spec_attr->ext_draw_ಗಾತ್ರ = s_new;
    }

    ಆದರೆ(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_ಕೊನೆ;
}

int32_t lv_obj_get_ext_draw_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ obj->spec_attr->ext_draw_ಗಾತ್ರ;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ 0;
}

lv_layer_type_t lv_obj_get_layer_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_NONE);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ (lv_layer_type_t)obj->spec_attr->layer_type;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ lv_opa_t get_layer_opa(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc)
{
    ಆದರೆ(ತಳ_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        ಆದರೆ(part == LV_PART_MAIN) {
            ಹಿಂತಿರುಗಿ ತಳ_dsc->layer->opa;
        }

        ಹಿಂತಿರುಗಿ LV_OPA_MIX2(ತಳ_dsc->layer->opa, lv_obj_get_style_opa_ಆಂತರಿಕ(obj, part));
    }

    /* fallback to old recursive style opa */
    ಹಿಂತಿರುಗಿ lv_obj_get_style_opa_recursive(obj, part);
}


ಸ್ಥಿರ lv_color_t ಸಾಮಾನ್ಯ_apply_layer_recolor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part,  ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    ಆದರೆ(ತಳ_dsc->layer) {
        recolor = ತಳ_dsc->layer->recolor;
        ಆದರೆ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ಹಿಂತಿರುಗಿ lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


ಸ್ಥಿರ lv_color32_t image_apply_layer_recolor(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part,
                                              ಸ್ಥಿರಾಂಕ lv_draw_dsc_ತಳ_t * ತಳ_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    ಆದರೆ(ತಳ_dsc->layer) {
        recolor = ತಳ_dsc->layer->recolor;
        ಆದರೆ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ಆದರೆ(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        ಹಿಂತಿರುಗಿ lv_color_over32(recolor, lv_color_ಗಮ್ಯ_32(color, opa));
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(recolor.alpha > LV_OPA_TRANSP) {
        ಹಿಂತಿರುಗಿ recolor;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ lv_color_ಗಮ್ಯ_32(color, opa);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ drop_shadow_init(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ತಳ_t * ತಳ_dsc)
{
    ತಳ_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_ಆಂತರಿಕ(obj, part);
    ಆದರೆ(ತಳ_dsc->drop_shadow_opa) {
        ತಳ_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_ಆಂತರಿಕ(obj, part);
        ತಳ_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_ಸ್ಥಾನಾಂತರ_x_ಆಂತರಿಕ(obj, part);
        ತಳ_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_ಸ್ಥಾನಾಂತರ_y_ಆಂತರಿಕ(obj, part);
        ತಳ_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_ಆಂತರಿಕ(obj, part);
        ತಳ_dsc->drop_shadow_color = ಸಾಮಾನ್ಯ_apply_layer_recolor(obj, part, ತಳ_dsc, ತಳ_dsc->drop_shadow_color);

        ತಳ_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_ಆಂತರಿಕ(obj, part);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ext_draw_ಗಾತ್ರ_event_cb(lv_event_t * e)
{
    int32_t ಗಾತ್ರ_2 = (int32_t)(lv_uintptr_t)lv_event_get_user_ದತ್ತಾಂಶ(e);
    lv_event_ಗಣ_ext_draw_ಗಾತ್ರ(e, ಗಾತ್ರ_2);
}
