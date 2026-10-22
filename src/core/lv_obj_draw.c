/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_draw_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

స్థిర అంతఃవరుస lv_opa_t get_layer_opa(స్థిరరాశి lv_obj_t * obj, lv_part_t part, స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc);
స్థిర lv_color_t సాధారణ_apply_layer_recolor(స్థిరరాశి lv_obj_t * obj, lv_part_t part,  స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc,
                                             lv_color_t color);
స్థిర lv_color32_t image_apply_layer_recolor(స్థిరరాశి lv_obj_t * obj, lv_part_t part,
                                              స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc, lv_color_t color, lv_opa_t opa);

స్థిర శూన్య drop_shadow_init(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_draw_dsc_పునాది_t * పునాది_dsc);

స్థిర శూన్య సమితి_ext_draw_కొలత_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

శూన్య lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->పునాది);
    ఒకవేళ(part != LV_PART_MAIN) {
        ఒకవేళ(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_చివర;
            తిరిగి;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_అంతర్గత(obj, part);

    ఒకవేళ(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_అంతర్గత(obj, part);
        ఒకవేళ(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_అంతర్గత(obj, part);
            draw_dsc->bg_color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, bg_color);
            స్థిరరాశి lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_అంతర్గత(obj, part);
            ఒకవేళ(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, పరిమాణం(*grad));
            }
            లేకపోతే {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_అంతర్గత(obj, part);
                ఒకవేళ(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_అంతర్గత(obj, part);
                    draw_dsc->bg_grad.stops[1].color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_అంతర్గత(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_అంతర్గత(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_అంతర్గత(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_అంతర్గత(obj, part);
                }
            }
        }
    }

    ఒకవేళ(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_వెడల్పు = lv_obj_get_style_border_వెడల్పు_అంతర్గత(obj, part);
        ఒకవేళ(draw_dsc->border_వెడల్పు) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_అంతర్గత(obj, part);
            ఒకవేళ(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_అంతర్గత(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_అంతర్గత(obj, part);
                draw_dsc->border_color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, border_color);
            }
        }
    }

    ఒకవేళ(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_వెడల్పు = lv_obj_get_style_outline_వెడల్పు_అంతర్గత(obj, part);
        ఒకవేళ(draw_dsc->outline_వెడల్పు) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_అంతర్గత(obj, part);
            ఒకవేళ(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_అంతర్గత(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_అంతర్గత(obj, part);
                draw_dsc->outline_color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, outline_color);
            }
        }
    }

    ఒకవేళ(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_అంతర్గత(obj, part);
        ఒకవేళ(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_అంతర్గత(obj, part);
            ఒకవేళ(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                ఒకవేళ(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_చిహ్నం) {
                    draw_dsc->bg_image_చిహ్నం_font = lv_obj_get_style_పాఠ్యం_font_అంతర్గత(obj, part);
                    lv_color_t పాఠ్యం_color = lv_obj_get_style_పాఠ్యం_color_filtered_అంతర్గత(obj, part);
                    draw_dsc->bg_image_recolor = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, పాఠ్యం_color);
                }
                లేకపోతే {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_అంతర్గత(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_అంతర్గత(obj, part);
                    lv_color32_t ఫలితం = image_apply_layer_recolor(obj, part, &draw_dsc->పునాది, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = ఫలితం.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(ఫలితం.red, ఫలితం.green, ఫలితం.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_అంతర్గత(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_అంతర్గత(obj, part);
            }
        }
    }

    ఒకవేళ(draw_dsc->shadow_opa) {
        draw_dsc->shadow_వెడల్పు = lv_obj_get_style_shadow_వెడల్పు_అంతర్గత(obj, part);
        ఒకవేళ(draw_dsc->shadow_వెడల్పు) {
            ఒకవేళ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_అంతర్గత(obj, part);
                ఒకవేళ(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_స్థానభ్రంశం_x = lv_obj_get_style_shadow_స్థానభ్రంశం_x_అంతర్గత(obj, part);
                    draw_dsc->shadow_స్థానభ్రంశం_y = lv_obj_get_style_shadow_స్థానభ్రంశం_y_అంతర్గత(obj, part);
                    draw_dsc->shadow_విస్తరణ = lv_obj_get_style_shadow_విస్తరణ_అంతర్గత(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_అంతర్గత(obj, part);
                    draw_dsc->shadow_color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, shadow_color);
                }
            }
        }
    }

    ఒకవేళ(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->పునాది);

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    draw_dsc->opa = lv_obj_get_style_పాఠ్యం_opa_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->పునాది);
    ఒకవేళ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }


    lv_color_t పాఠ్యం_color = lv_obj_get_style_పాఠ్యం_color_filtered_అంతర్గత(obj, part);
    draw_dsc->color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, పాఠ్యం_color);
    draw_dsc->letter_space = lv_obj_get_style_పాఠ్యం_letter_space_అంతర్గత(obj, part);
    draw_dsc->పంక్తి_space = lv_obj_get_style_పాఠ్యం_పంక్తి_space_అంతర్గత(obj, part);
    draw_dsc->decor = lv_obj_get_style_పాఠ్యం_decor_అంతర్గత(obj, part);

    draw_dsc->font = lv_obj_get_style_పాఠ్యం_font_అంతర్గత(obj, part);

#ఒకవేళ LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN);
#అయితే_ముగింపు

    draw_dsc->align = lv_obj_get_style_పాఠ్యం_align_అంతర్గత(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->పునాది);

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->పునాది);
    ఒకవేళ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->ఇరుసు.x = lv_area_get_వెడల్పు(&obj->coords) / 2;
    draw_dsc->ఇరుసు.y = lv_area_get_ఎత్తు(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_అంతర్గత(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_అంతర్గత(obj, part);
    lv_color32_t ఫలితం = image_apply_layer_recolor(obj, part, &draw_dsc->పునాది, recolor, recolor_opa);
    draw_dsc->recolor_opa = ఫలితం.alpha;
    draw_dsc->recolor = lv_color_make(ఫలితం.red, ఫలితం.green, ఫలితం.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_అంతర్గత(obj, part);

    ఒకవేళ(part != LV_PART_MAIN) draw_dsc->blend_బహుళకం = lv_obj_get_style_blend_బహుళకం_అంతర్గత(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->పునాది);

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_init_draw_పంక్తి_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_పంక్తి_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    draw_dsc->opa = lv_obj_get_style_పంక్తి_opa_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->పునాది);
    ఒకవేళ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    draw_dsc->వెడల్పు = lv_obj_get_style_పంక్తి_వెడల్పు_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->వెడల్పు == 0) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_color_t పంక్తి_color = lv_obj_get_style_పంక్తి_color_filtered_అంతర్గత(obj, part);
    draw_dsc->color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, పంక్తి_color);

    draw_dsc->dash_వెడల్పు = lv_obj_get_style_పంక్తి_dash_వెడల్పు_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->dash_వెడల్పు) {
        draw_dsc->dash_gap = lv_obj_get_style_పంక్తి_dash_gap_అంతర్గత(obj, part);
    }

    draw_dsc->విడత_ప్రారంభం = lv_obj_get_style_పంక్తి_rounded_అంతర్గత(obj, part);
    draw_dsc->విడత_చివర = draw_dsc->విడత_ప్రారంభం;

    drop_shadow_init(obj, part, &draw_dsc->పునాది);

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    draw_dsc->వెడల్పు = lv_obj_get_style_arc_వెడల్పు_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->వెడల్పు == 0) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_అంతర్గత(obj, part);
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->పునాది);
    ఒకవేళ(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    ఒకవేళ(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_చివర;
        తిరిగి;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_అంతర్గత(obj, part);
    draw_dsc->color = సాధారణ_apply_layer_recolor(obj, part, &draw_dsc->పునాది, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_అంతర్గత(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_అంతర్గత(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->పునాది);

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(draw_dsc != ఖాళీ, తిరిగి);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->పునాది.obj = obj;
    draw_dsc->పునాది.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_అంతర్గత(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_అంతర్గత(obj, part);

    /*Radius might be set earlier as it's already known*/
    ఒకవేళ(draw_dsc->మూల_radius == 0) {
        draw_dsc->మూల_radius = lv_obj_get_style_radius_అంతర్గత(obj, part);
    }

    LV_PROFILER_DRAW_చివర;
}

శూన్య lv_obj_సమితి_ext_draw_కొలత(lv_obj_t * obj, int32_t కొలత)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_remove_event_cb(obj, సమితి_ext_draw_కొలత_event_cb);
    lv_obj_కూడు_event_cb(obj, సమితి_ext_draw_కొలత_event_cb, LV_EVENT_REFR_EXT_DRAW_కొలత, (శూన్య *)(lv_uintptr_t)కొలత);
}

int32_t lv_obj_calculate_ext_draw_కొలత(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_వెడల్పు = lv_obj_get_style_shadow_వెడల్పు_అంతర్గత(obj, part);
    ఒకవేళ(sh_వెడల్పు) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_అంతర్గత(obj, part);
        ఒకవేళ(sh_opa > LV_OPA_MIN) {
            sh_వెడల్పు = sh_వెడల్పు / 2 + 1;    /*The blur adds only half width*/
            sh_వెడల్పు += lv_obj_get_style_shadow_విస్తరణ_అంతర్గత(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_స్థానభ్రంశం_x_అంతర్గత(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_స్థానభ్రంశం_y_అంతర్గత(obj, part);
            sh_వెడల్పు += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_వెడల్పు);
        }
    }

    int32_t outline_వెడల్పు = lv_obj_get_style_outline_వెడల్పు_అంతర్గత(obj, part);
    ఒకవేళ(outline_వెడల్పు) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_అంతర్గత(obj, part);
        ఒకవేళ(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_అంతర్గత(obj, part);
            s = LV_MAX(s, outline_pad + outline_వెడల్పు);
        }
    }

    int32_t drop_shadow_కొలత = 0;
    ఒకవేళ(lv_obj_get_style_drop_shadow_opa_అంతర్గత(obj, part) > 0) {
        drop_shadow_కొలత += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_స్థానభ్రంశం_x_అంతర్గత(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_స్థానభ్రంశం_y_అంతర్గత(obj, part)));

        drop_shadow_కొలత += lv_obj_get_style_drop_shadow_radius_అంతర్గత(obj, part) + 1;
    }
    s += drop_shadow_కొలత;

    int32_t w = lv_obj_get_style_transform_వెడల్పు_అంతర్గత(obj, part);
    int32_t h = lv_obj_get_style_transform_ఎత్తు_అంతర్గత(obj, part);
    int32_t wh = LV_MAX(w, h);
    ఒకవేళ(wh > 0) s += wh;

    LV_PROFILER_DRAW_చివర;
    తిరిగి s;
}

శూన్య lv_obj_refresh_ext_draw_కొలత(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    int32_t s_old = lv_obj_get_ext_draw_కొలత(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_కొలత, &s_new);

    /*Store the result if the special attrs already allocated*/
    ఒకవేళ(obj->spec_attr) {
        obj->spec_attr->ext_draw_కొలత = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    లేకపోతే ఒకవేళ(s_new != 0) {
        ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_చివర;
            తిరిగి;
        }
        obj->spec_attr->ext_draw_కొలత = s_new;
    }

    ఒకవేళ(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_చివర;
}

int32_t lv_obj_get_ext_draw_కొలత(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    ఒకవేళ(obj->spec_attr) తిరిగి obj->spec_attr->ext_draw_కొలత;
    లేకపోతే తిరిగి 0;
}

lv_layer_type_t lv_obj_get_layer_type(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_LAYER_TYPE_NONE);

    ఒకవేళ(obj->spec_attr) తిరిగి (lv_layer_type_t)obj->spec_attr->layer_type;
    లేకపోతే తిరిగి LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర అంతఃవరుస lv_opa_t get_layer_opa(స్థిరరాశి lv_obj_t * obj, lv_part_t part, స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc)
{
    ఒకవేళ(పునాది_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        ఒకవేళ(part == LV_PART_MAIN) {
            తిరిగి పునాది_dsc->layer->opa;
        }

        తిరిగి LV_OPA_MIX2(పునాది_dsc->layer->opa, lv_obj_get_style_opa_అంతర్గత(obj, part));
    }

    /* fallback to old recursive style opa */
    తిరిగి lv_obj_get_style_opa_recursive(obj, part);
}


స్థిర lv_color_t సాధారణ_apply_layer_recolor(స్థిరరాశి lv_obj_t * obj, lv_part_t part,  స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    ఒకవేళ(పునాది_dsc->layer) {
        recolor = పునాది_dsc->layer->recolor;
        ఒకవేళ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    లేకపోతే {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    తిరిగి lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


స్థిర lv_color32_t image_apply_layer_recolor(స్థిరరాశి lv_obj_t * obj, lv_part_t part,
                                              స్థిరరాశి lv_draw_dsc_పునాది_t * పునాది_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    ఒకవేళ(పునాది_dsc->layer) {
        recolor = పునాది_dsc->layer->recolor;
        ఒకవేళ(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    లేకపోతే {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    ఒకవేళ(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        తిరిగి lv_color_over32(recolor, lv_color_గమ్యం_32(color, opa));
    }
    లేకపోతే ఒకవేళ(recolor.alpha > LV_OPA_TRANSP) {
        తిరిగి recolor;
    }
    లేకపోతే {
        తిరిగి lv_color_గమ్యం_32(color, opa);
    }
}

స్థిర శూన్య drop_shadow_init(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_draw_dsc_పునాది_t * పునాది_dsc)
{
    పునాది_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_అంతర్గత(obj, part);
    ఒకవేళ(పునాది_dsc->drop_shadow_opa) {
        పునాది_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_అంతర్గత(obj, part);
        పునాది_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_స్థానభ్రంశం_x_అంతర్గత(obj, part);
        పునాది_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_స్థానభ్రంశం_y_అంతర్గత(obj, part);
        పునాది_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_అంతర్గత(obj, part);
        పునాది_dsc->drop_shadow_color = సాధారణ_apply_layer_recolor(obj, part, పునాది_dsc, పునాది_dsc->drop_shadow_color);

        పునాది_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_అంతర్గత(obj, part);
    }
}

స్థిర శూన్య సమితి_ext_draw_కొలత_event_cb(lv_event_t * e)
{
    int32_t కొలత = (int32_t)(lv_uintptr_t)lv_event_get_user_దత్తాంశం(e);
    lv_event_సమితి_ext_draw_కొలత(e, కొలత);
}
