/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_draw_private.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_private.h"
#ضمن "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن مضمن lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_ادھار_t * ادھار_dsc);
ساكن lv_color_t عام_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_ادھار_t * ادھار_dsc,
                                             lv_color_t color);
ساكن lv_color32_t image_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,
                                              ثابت lv_draw_dsc_ادھار_t * ادھار_dsc, lv_color_t color, lv_opa_t opa);

ساكن فراغ drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ادھار_t * ادھار_dsc);

ساكن فراغ سیٹ_ext_draw_ناپ_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ادھار);
    إذا(part != LV_PART_MAIN) {
        إذا(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_سرا;
            أرجع;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_اندرونی(obj, part);

    إذا(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_اندرونی(obj, part);
        إذا(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_اندرونی(obj, part);
            draw_dsc->bg_color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, bg_color);
            ثابت lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_اندرونی(obj, part);
            إذا(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, حجم(*grad));
            }
            وإلا {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_اندرونی(obj, part);
                إذا(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_اندرونی(obj, part);
                }
            }
        }
    }

    إذا(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_چوڑائی = lv_obj_get_style_border_چوڑائی_اندرونی(obj, part);
        إذا(draw_dsc->border_چوڑائی) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_اندرونی(obj, part);
            إذا(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_اندرونی(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_اندرونی(obj, part);
                draw_dsc->border_color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, border_color);
            }
        }
    }

    إذا(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_چوڑائی = lv_obj_get_style_outline_چوڑائی_اندرونی(obj, part);
        إذا(draw_dsc->outline_چوڑائی) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_اندرونی(obj, part);
            إذا(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_اندرونی(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_اندرونی(obj, part);
                draw_dsc->outline_color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, outline_color);
            }
        }
    }

    إذا(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_اندرونی(obj, part);
        إذا(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_اندرونی(obj, part);
            إذا(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                إذا(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_علامت) {
                    draw_dsc->bg_image_علامت_font = lv_obj_get_style_متن_font_اندرونی(obj, part);
                    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_اندرونی(obj, part);
                    draw_dsc->bg_image_recolor = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, متن_color);
                }
                وإلا {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_اندرونی(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_اندرونی(obj, part);
                    lv_color32_t نتیجہ = image_apply_layer_recolor(obj, part, &draw_dsc->ادھار, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = نتیجہ.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(نتیجہ.red, نتیجہ.green, نتیجہ.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_اندرونی(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_اندرونی(obj, part);
            }
        }
    }

    إذا(draw_dsc->shadow_opa) {
        draw_dsc->shadow_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, part);
        إذا(draw_dsc->shadow_چوڑائی) {
            إذا(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, part);
                إذا(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_وتھ_x = lv_obj_get_style_shadow_وتھ_x_اندرونی(obj, part);
                    draw_dsc->shadow_وتھ_y = lv_obj_get_style_shadow_وتھ_y_اندرونی(obj, part);
                    draw_dsc->shadow_پھیلاؤ = lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_اندرونی(obj, part);
                    draw_dsc->shadow_color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, shadow_color);
                }
            }
        }
    }

    إذا(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->ادھار);

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    draw_dsc->opa = lv_obj_get_style_متن_opa_اندرونی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ادھار);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }


    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_اندرونی(obj, part);
    draw_dsc->color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, متن_color);
    draw_dsc->letter_space = lv_obj_get_style_متن_letter_space_اندرونی(obj, part);
    draw_dsc->سطر_space = lv_obj_get_style_متن_سطر_space_اندرونی(obj, part);
    draw_dsc->decor = lv_obj_get_style_متن_decor_اندرونی(obj, part);

    draw_dsc->font = lv_obj_get_style_متن_font_اندرونی(obj, part);

#إذا LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_MAIN);
#جے_انت

    draw_dsc->align = lv_obj_get_style_متن_align_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ادھار);

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_اندرونی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ادھار);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->دھرا.x = lv_area_get_چوڑائی(&obj->coords) / 2;
    draw_dsc->دھرا.y = lv_area_get_اچائی(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_اندرونی(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_اندرونی(obj, part);
    lv_color32_t نتیجہ = image_apply_layer_recolor(obj, part, &draw_dsc->ادھار, recolor, recolor_opa);
    draw_dsc->recolor_opa = نتیجہ.alpha;
    draw_dsc->recolor = lv_color_make(نتیجہ.red, نتیجہ.green, نتیجہ.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_اندرونی(obj, part);

    إذا(part != LV_PART_MAIN) draw_dsc->blend_منوال = lv_obj_get_style_blend_منوال_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ادھار);

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_init_draw_سطر_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_سطر_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    draw_dsc->opa = lv_obj_get_style_سطر_opa_اندرونی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ادھار);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    draw_dsc->چوڑائی = lv_obj_get_style_سطر_چوڑائی_اندرونی(obj, part);
    إذا(draw_dsc->چوڑائی == 0) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_color_t سطر_color = lv_obj_get_style_سطر_color_filtered_اندرونی(obj, part);
    draw_dsc->color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, سطر_color);

    draw_dsc->dash_چوڑائی = lv_obj_get_style_سطر_dash_چوڑائی_اندرونی(obj, part);
    إذا(draw_dsc->dash_چوڑائی) {
        draw_dsc->dash_gap = lv_obj_get_style_سطر_dash_gap_اندرونی(obj, part);
    }

    draw_dsc->گیڑا_شروع = lv_obj_get_style_سطر_rounded_اندرونی(obj, part);
    draw_dsc->گیڑا_سرا = draw_dsc->گیڑا_شروع;

    drop_shadow_init(obj, part, &draw_dsc->ادھار);

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    draw_dsc->چوڑائی = lv_obj_get_style_arc_چوڑائی_اندرونی(obj, part);
    إذا(draw_dsc->چوڑائی == 0) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_اندرونی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->ادھار);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_سرا;
        أرجع;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_اندرونی(obj, part);
    draw_dsc->color = عام_apply_layer_recolor(obj, part, &draw_dsc->ادھار, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_اندرونی(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->ادھار);

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->ادھار.obj = obj;
    draw_dsc->ادھار.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_اندرونی(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_اندرونی(obj, part);

    /*Radius might be set earlier as it's already known*/
    إذا(draw_dsc->کونا_radius == 0) {
        draw_dsc->کونا_radius = lv_obj_get_style_radius_اندرونی(obj, part);
    }

    LV_PROFILER_DRAW_سرا;
}

فراغ lv_obj_سیٹ_ext_draw_ناپ(lv_obj_t * obj, int32_t ناپ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_remove_event_cb(obj, سیٹ_ext_draw_ناپ_event_cb);
    lv_obj_جوڑو_event_cb(obj, سیٹ_ext_draw_ناپ_event_cb, LV_EVENT_REFR_EXT_DRAW_ناپ, (فراغ *)(lv_uintptr_t)ناپ);
}

int32_t lv_obj_calculate_ext_draw_ناپ(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, part);
    إذا(sh_چوڑائی) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, part);
        إذا(sh_opa > LV_OPA_MIN) {
            sh_چوڑائی = sh_چوڑائی / 2 + 1;    /*The blur adds only half width*/
            sh_چوڑائی += lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_وتھ_x_اندرونی(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_وتھ_y_اندرونی(obj, part);
            sh_چوڑائی += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_چوڑائی);
        }
    }

    int32_t outline_چوڑائی = lv_obj_get_style_outline_چوڑائی_اندرونی(obj, part);
    إذا(outline_چوڑائی) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_اندرونی(obj, part);
        إذا(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_اندرونی(obj, part);
            s = LV_MAX(s, outline_pad + outline_چوڑائی);
        }
    }

    int32_t drop_shadow_ناپ = 0;
    إذا(lv_obj_get_style_drop_shadow_opa_اندرونی(obj, part) > 0) {
        drop_shadow_ناپ += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_وتھ_x_اندرونی(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_وتھ_y_اندرونی(obj, part)));

        drop_shadow_ناپ += lv_obj_get_style_drop_shadow_radius_اندرونی(obj, part) + 1;
    }
    s += drop_shadow_ناپ;

    int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, part);
    int32_t h = lv_obj_get_style_transform_اچائی_اندرونی(obj, part);
    int32_t wh = LV_MAX(w, h);
    إذا(wh > 0) s += wh;

    LV_PROFILER_DRAW_سرا;
    أرجع s;
}

فراغ lv_obj_refresh_ext_draw_ناپ(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    int32_t s_old = lv_obj_get_ext_draw_ناپ(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_ناپ, &s_new);

    /*Store the result if the special attrs already allocated*/
    إذا(obj->spec_attr) {
        obj->spec_attr->ext_draw_ناپ = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    وإلا إذا(s_new != 0) {
        إذا(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_سرا;
            أرجع;
        }
        obj->spec_attr->ext_draw_ناپ = s_new;
    }

    إذا(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_سرا;
}

int32_t lv_obj_get_ext_draw_ناپ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr) أرجع obj->spec_attr->ext_draw_ناپ;
    وإلا أرجع 0;
}

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_LAYER_TYPE_NONE);

    إذا(obj->spec_attr) أرجع (lv_layer_type_t)obj->spec_attr->layer_type;
    وإلا أرجع LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن مضمن lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_ادھار_t * ادھار_dsc)
{
    إذا(ادھار_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        إذا(part == LV_PART_MAIN) {
            أرجع ادھار_dsc->layer->opa;
        }

        أرجع LV_OPA_MIX2(ادھار_dsc->layer->opa, lv_obj_get_style_opa_اندرونی(obj, part));
    }

    /* fallback to old recursive style opa */
    أرجع lv_obj_get_style_opa_recursive(obj, part);
}


ساكن lv_color_t عام_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_ادھار_t * ادھار_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    إذا(ادھار_dsc->layer) {
        recolor = ادھار_dsc->layer->recolor;
        إذا(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    وإلا {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    أرجع lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


ساكن lv_color32_t image_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,
                                              ثابت lv_draw_dsc_ادھار_t * ادھار_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    إذا(ادھار_dsc->layer) {
        recolor = ادھار_dsc->layer->recolor;
        إذا(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    وإلا {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    إذا(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        أرجع lv_color_over32(recolor, lv_color_منزل_32(color, opa));
    }
    وإلا إذا(recolor.alpha > LV_OPA_TRANSP) {
        أرجع recolor;
    }
    وإلا {
        أرجع lv_color_منزل_32(color, opa);
    }
}

ساكن فراغ drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_ادھار_t * ادھار_dsc)
{
    ادھار_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_اندرونی(obj, part);
    إذا(ادھار_dsc->drop_shadow_opa) {
        ادھار_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_اندرونی(obj, part);
        ادھار_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_وتھ_x_اندرونی(obj, part);
        ادھار_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_وتھ_y_اندرونی(obj, part);
        ادھار_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_اندرونی(obj, part);
        ادھار_dsc->drop_shadow_color = عام_apply_layer_recolor(obj, part, ادھار_dsc, ادھار_dsc->drop_shadow_color);

        ادھار_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_اندرونی(obj, part);
    }
}

ساكن فراغ سیٹ_ext_draw_ناپ_event_cb(lv_event_t * e)
{
    int32_t ناپ = (int32_t)(lv_uintptr_t)lv_event_get_user_انکڑے(e);
    lv_event_سیٹ_ext_draw_ناپ(e, ناپ);
}
