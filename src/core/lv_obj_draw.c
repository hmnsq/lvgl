/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_obj_draw_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#परिभाषय MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर अन्तःस्थ lv_opa_t get_layer_opa(ध्रुव lv_obj_t * obj, lv_part_t part, ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc);
स्थिर lv_color_t प्रसामान्यम्_apply_layer_recolor(ध्रुव lv_obj_t * obj, lv_part_t part,  ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc,
                                             lv_color_t color);
स्थिर lv_color32_t image_apply_layer_recolor(ध्रुव lv_obj_t * obj, lv_part_t part,
                                              ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc, lv_color_t color, lv_opa_t opa);

स्थिर शून्य drop_shadow_init(ध्रुव lv_obj_t * obj, lv_part_t part, lv_draw_dsc_आधारः_t * आधारः_dsc);

स्थिर शून्य समुच्चयः_ext_draw_आकार_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

शून्य lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->आधारः);
    यदि(part != LV_PART_MAIN) {
        यदि(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_अन्तः;
            प्रत्यागम;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_आन्तरिकम्(obj, part);

    यदि(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_आन्तरिकम्(obj, part);
        यदि(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_आन्तरिकम्(obj, part);
            draw_dsc->bg_color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, bg_color);
            ध्रुव lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_आन्तरिकम्(obj, part);
            यदि(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, परिमाण(*grad));
            }
            अन्यथा {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_आन्तरिकम्(obj, part);
                यदि(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_आन्तरिकम्(obj, part);
                    draw_dsc->bg_grad.stops[1].color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_आन्तरिकम्(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_आन्तरिकम्(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_आन्तरिकम्(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_आन्तरिकम्(obj, part);
                }
            }
        }
    }

    यदि(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_विस्तारः = lv_obj_get_style_border_विस्तारः_आन्तरिकम्(obj, part);
        यदि(draw_dsc->border_विस्तारः) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_आन्तरिकम्(obj, part);
            यदि(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_आन्तरिकम्(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_आन्तरिकम्(obj, part);
                draw_dsc->border_color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, border_color);
            }
        }
    }

    यदि(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_विस्तारः = lv_obj_get_style_outline_विस्तारः_आन्तरिकम्(obj, part);
        यदि(draw_dsc->outline_विस्तारः) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_आन्तरिकम्(obj, part);
            यदि(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_आन्तरिकम्(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_आन्तरिकम्(obj, part);
                draw_dsc->outline_color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, outline_color);
            }
        }
    }

    यदि(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_आन्तरिकम्(obj, part);
        यदि(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_आन्तरिकम्(obj, part);
            यदि(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                यदि(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_चिह्नम्) {
                    draw_dsc->bg_image_चिह्नम्_font = lv_obj_get_style_text_font_आन्तरिकम्(obj, part);
                    lv_color_t text_color = lv_obj_get_style_text_color_filtered_आन्तरिकम्(obj, part);
                    draw_dsc->bg_image_recolor = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, text_color);
                }
                अन्यथा {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_आन्तरिकम्(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_आन्तरिकम्(obj, part);
                    lv_color32_t परिणाम = image_apply_layer_recolor(obj, part, &draw_dsc->आधारः, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = परिणाम.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(परिणाम.red, परिणाम.green, परिणाम.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_आन्तरिकम्(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_आन्तरिकम्(obj, part);
            }
        }
    }

    यदि(draw_dsc->shadow_opa) {
        draw_dsc->shadow_विस्तारः = lv_obj_get_style_shadow_विस्तारः_आन्तरिकम्(obj, part);
        यदि(draw_dsc->shadow_विस्तारः) {
            यदि(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_आन्तरिकम्(obj, part);
                यदि(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_विस्थापनम्_x = lv_obj_get_style_shadow_विस्थापनम्_x_आन्तरिकम्(obj, part);
                    draw_dsc->shadow_विस्थापनम्_y = lv_obj_get_style_shadow_विस्थापनम्_y_आन्तरिकम्(obj, part);
                    draw_dsc->shadow_प्रसारः = lv_obj_get_style_shadow_प्रसारः_आन्तरिकम्(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_आन्तरिकम्(obj, part);
                    draw_dsc->shadow_color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, shadow_color);
                }
            }
        }
    }

    यदि(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->आधारः);

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    draw_dsc->opa = lv_obj_get_style_text_opa_आन्तरिकम्(obj, part);
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->आधारः);
    यदि(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }


    lv_color_t text_color = lv_obj_get_style_text_color_filtered_आन्तरिकम्(obj, part);
    draw_dsc->color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, text_color);
    draw_dsc->letter_space = lv_obj_get_style_text_letter_space_आन्तरिकम्(obj, part);
    draw_dsc->पङ्क्तिः_space = lv_obj_get_style_text_पङ्क्तिः_space_आन्तरिकम्(obj, part);
    draw_dsc->decor = lv_obj_get_style_text_decor_आन्तरिकम्(obj, part);

    draw_dsc->font = lv_obj_get_style_text_font_आन्तरिकम्(obj, part);

#यदि LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_आधारः_dir_आन्तरिकम्(obj, LV_PART_MAIN);
#यदि_अन्तः

    draw_dsc->align = lv_obj_get_style_text_align_आन्तरिकम्(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->आधारः);

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_आन्तरिकम्(obj, part);
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->आधारः);
    यदि(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->pivot.x = lv_area_get_विस्तारः(&obj->coords) / 2;
    draw_dsc->pivot.y = lv_area_get_height(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_आन्तरिकम्(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_आन्तरिकम्(obj, part);
    lv_color32_t परिणाम = image_apply_layer_recolor(obj, part, &draw_dsc->आधारः, recolor, recolor_opa);
    draw_dsc->recolor_opa = परिणाम.alpha;
    draw_dsc->recolor = lv_color_make(परिणाम.red, परिणाम.green, परिणाम.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_आन्तरिकम्(obj, part);

    यदि(part != LV_PART_MAIN) draw_dsc->blend_बहुलकम् = lv_obj_get_style_blend_बहुलकम्_आन्तरिकम्(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->आधारः);

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_init_draw_पङ्क्तिः_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_पङ्क्तिः_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    draw_dsc->opa = lv_obj_get_style_पङ्क्तिः_opa_आन्तरिकम्(obj, part);
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->आधारः);
    यदि(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    draw_dsc->विस्तारः = lv_obj_get_style_पङ्क्तिः_विस्तारः_आन्तरिकम्(obj, part);
    यदि(draw_dsc->विस्तारः == 0) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_color_t पङ्क्तिः_color = lv_obj_get_style_पङ्क्तिः_color_filtered_आन्तरिकम्(obj, part);
    draw_dsc->color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, पङ्क्तिः_color);

    draw_dsc->dash_विस्तारः = lv_obj_get_style_पङ्क्तिः_dash_विस्तारः_आन्तरिकम्(obj, part);
    यदि(draw_dsc->dash_विस्तारः) {
        draw_dsc->dash_gap = lv_obj_get_style_पङ्क्तिः_dash_gap_आन्तरिकम्(obj, part);
    }

    draw_dsc->आवर्तनम्_start = lv_obj_get_style_पङ्क्तिः_rounded_आन्तरिकम्(obj, part);
    draw_dsc->आवर्तनम्_अन्तः = draw_dsc->आवर्तनम्_start;

    drop_shadow_init(obj, part, &draw_dsc->आधारः);

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    draw_dsc->विस्तारः = lv_obj_get_style_arc_विस्तारः_आन्तरिकम्(obj, part);
    यदि(draw_dsc->विस्तारः == 0) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_आन्तरिकम्(obj, part);
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->आधारः);
    यदि(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    यदि(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_अन्तः;
        प्रत्यागम;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_आन्तरिकम्(obj, part);
    draw_dsc->color = प्रसामान्यम्_apply_layer_recolor(obj, part, &draw_dsc->आधारः, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_आन्तरिकम्(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_आन्तरिकम्(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->आधारः);

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(draw_dsc != रिक्त, प्रत्यागम);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->आधारः.obj = obj;
    draw_dsc->आधारः.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_आन्तरिकम्(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_आन्तरिकम्(obj, part);

    /*Radius might be set earlier as it's already known*/
    यदि(draw_dsc->corner_radius == 0) {
        draw_dsc->corner_radius = lv_obj_get_style_radius_आन्तरिकम्(obj, part);
    }

    LV_PROFILER_DRAW_अन्तः;
}

शून्य lv_obj_समुच्चयः_ext_draw_आकार(lv_obj_t * obj, int32_t आकार)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_remove_event_cb(obj, समुच्चयः_ext_draw_आकार_event_cb);
    lv_obj_सङ्कलय_event_cb(obj, समुच्चयः_ext_draw_आकार_event_cb, LV_EVENT_REFR_EXT_DRAW_आकार, (शून्य *)(lv_uintptr_t)आकार);
}

int32_t lv_obj_calculate_ext_draw_आकार(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_विस्तारः = lv_obj_get_style_shadow_विस्तारः_आन्तरिकम्(obj, part);
    यदि(sh_विस्तारः) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_आन्तरिकम्(obj, part);
        यदि(sh_opa > LV_OPA_MIN) {
            sh_विस्तारः = sh_विस्तारः / 2 + 1;    /*The blur adds only half width*/
            sh_विस्तारः += lv_obj_get_style_shadow_प्रसारः_आन्तरिकम्(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_विस्थापनम्_x_आन्तरिकम्(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_विस्थापनम्_y_आन्तरिकम्(obj, part);
            sh_विस्तारः += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_विस्तारः);
        }
    }

    int32_t outline_विस्तारः = lv_obj_get_style_outline_विस्तारः_आन्तरिकम्(obj, part);
    यदि(outline_विस्तारः) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_आन्तरिकम्(obj, part);
        यदि(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_आन्तरिकम्(obj, part);
            s = LV_MAX(s, outline_pad + outline_विस्तारः);
        }
    }

    int32_t drop_shadow_आकार = 0;
    यदि(lv_obj_get_style_drop_shadow_opa_आन्तरिकम्(obj, part) > 0) {
        drop_shadow_आकार += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_विस्थापनम्_x_आन्तरिकम्(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_विस्थापनम्_y_आन्तरिकम्(obj, part)));

        drop_shadow_आकार += lv_obj_get_style_drop_shadow_radius_आन्तरिकम्(obj, part) + 1;
    }
    s += drop_shadow_आकार;

    int32_t w = lv_obj_get_style_transform_विस्तारः_आन्तरिकम्(obj, part);
    int32_t h = lv_obj_get_style_transform_height_आन्तरिकम्(obj, part);
    int32_t wh = LV_MAX(w, h);
    यदि(wh > 0) s += wh;

    LV_PROFILER_DRAW_अन्तः;
    प्रत्यागम s;
}

शून्य lv_obj_refresh_ext_draw_आकार(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    int32_t s_old = lv_obj_get_ext_draw_आकार(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_आकार, &s_new);

    /*Store the result if the special attrs already allocated*/
    यदि(obj->spec_attr) {
        obj->spec_attr->ext_draw_आकार = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    अन्यथा यदि(s_new != 0) {
        यदि(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_अन्तः;
            प्रत्यागम;
        }
        obj->spec_attr->ext_draw_आकार = s_new;
    }

    यदि(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_अन्तः;
}

int32_t lv_obj_get_ext_draw_आकार(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    यदि(obj->spec_attr) प्रत्यागम obj->spec_attr->ext_draw_आकार;
    अन्यथा प्रत्यागम 0;
}

lv_layer_type_t lv_obj_get_layer_type(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम LV_LAYER_TYPE_NONE);

    यदि(obj->spec_attr) प्रत्यागम (lv_layer_type_t)obj->spec_attr->layer_type;
    अन्यथा प्रत्यागम LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर अन्तःस्थ lv_opa_t get_layer_opa(ध्रुव lv_obj_t * obj, lv_part_t part, ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc)
{
    यदि(आधारः_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        यदि(part == LV_PART_MAIN) {
            प्रत्यागम आधारः_dsc->layer->opa;
        }

        प्रत्यागम LV_OPA_MIX2(आधारः_dsc->layer->opa, lv_obj_get_style_opa_आन्तरिकम्(obj, part));
    }

    /* fallback to old recursive style opa */
    प्रत्यागम lv_obj_get_style_opa_recursive(obj, part);
}


स्थिर lv_color_t प्रसामान्यम्_apply_layer_recolor(ध्रुव lv_obj_t * obj, lv_part_t part,  ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    यदि(आधारः_dsc->layer) {
        recolor = आधारः_dsc->layer->recolor;
        यदि(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    अन्यथा {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    प्रत्यागम lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


स्थिर lv_color32_t image_apply_layer_recolor(ध्रुव lv_obj_t * obj, lv_part_t part,
                                              ध्रुव lv_draw_dsc_आधारः_t * आधारः_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    यदि(आधारः_dsc->layer) {
        recolor = आधारः_dsc->layer->recolor;
        यदि(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    अन्यथा {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    यदि(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        प्रत्यागम lv_color_over32(recolor, lv_color_गन्तव्यम्_32(color, opa));
    }
    अन्यथा यदि(recolor.alpha > LV_OPA_TRANSP) {
        प्रत्यागम recolor;
    }
    अन्यथा {
        प्रत्यागम lv_color_गन्तव्यम्_32(color, opa);
    }
}

स्थिर शून्य drop_shadow_init(ध्रुव lv_obj_t * obj, lv_part_t part, lv_draw_dsc_आधारः_t * आधारः_dsc)
{
    आधारः_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_आन्तरिकम्(obj, part);
    यदि(आधारः_dsc->drop_shadow_opa) {
        आधारः_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_आन्तरिकम्(obj, part);
        आधारः_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_विस्थापनम्_x_आन्तरिकम्(obj, part);
        आधारः_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_विस्थापनम्_y_आन्तरिकम्(obj, part);
        आधारः_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_आन्तरिकम्(obj, part);
        आधारः_dsc->drop_shadow_color = प्रसामान्यम्_apply_layer_recolor(obj, part, आधारः_dsc, आधारः_dsc->drop_shadow_color);

        आधारः_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_आन्तरिकम्(obj, part);
    }
}

स्थिर शून्य समुच्चयः_ext_draw_आकार_event_cb(lv_event_t * e)
{
    int32_t आकार = (int32_t)(lv_uintptr_t)lv_event_get_user_दत्त(e);
    lv_event_समुच्चयः_ext_draw_आकार(e, आकार);
}
