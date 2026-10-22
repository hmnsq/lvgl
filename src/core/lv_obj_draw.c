/**
 * @file lv_obj_draw.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_draw_private.h"
#شامل "../lvgl_public.h"
#شامل "lv_obj_private.h"
#شامل "lv_obj_style_internal.h"

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

ساکن اندرونی lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc);
ساکن lv_color_t معمول_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc,
                                             lv_color_t color);
ساکن lv_color32_t image_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,
                                              ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc, lv_color_t color, lv_opa_t opa);

ساکن خالی drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_بنیاد_t * بنیاد_dsc);

ساکن خالی سیٹ_ext_draw_سائز_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

خالی lv_obj_init_draw_rect_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_rect_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنیاد);
    اگر(part != LV_PART_MAIN) {
        اگر(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_انتہا;
            واپس;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_اندرونی(obj, part);

    اگر(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_اندرونی(obj, part);
        اگر(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_اندرونی(obj, part);
            draw_dsc->bg_color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, bg_color);
            ثابت lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_اندرونی(obj, part);
            اگر(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, حجم(*grad));
            }
            ورنہ {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_اندرونی(obj, part);
                اگر(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_اندرونی(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_اندرونی(obj, part);
                }
            }
        }
    }

    اگر(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_چوڑائی = lv_obj_get_style_border_چوڑائی_اندرونی(obj, part);
        اگر(draw_dsc->border_چوڑائی) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_اندرونی(obj, part);
            اگر(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_اندرونی(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_اندرونی(obj, part);
                draw_dsc->border_color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, border_color);
            }
        }
    }

    اگر(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_چوڑائی = lv_obj_get_style_outline_چوڑائی_اندرونی(obj, part);
        اگر(draw_dsc->outline_چوڑائی) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_اندرونی(obj, part);
            اگر(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_اندرونی(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_اندرونی(obj, part);
                draw_dsc->outline_color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, outline_color);
            }
        }
    }

    اگر(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_اندرونی(obj, part);
        اگر(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_اندرونی(obj, part);
            اگر(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                اگر(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_علامت) {
                    draw_dsc->bg_image_علامت_font = lv_obj_get_style_متن_font_اندرونی(obj, part);
                    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_اندرونی(obj, part);
                    draw_dsc->bg_image_recolor = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, متن_color);
                }
                ورنہ {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_اندرونی(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_اندرونی(obj, part);
                    lv_color32_t نتیجہ = image_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = نتیجہ.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(نتیجہ.red, نتیجہ.green, نتیجہ.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_اندرونی(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_اندرونی(obj, part);
            }
        }
    }

    اگر(draw_dsc->shadow_opa) {
        draw_dsc->shadow_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, part);
        اگر(draw_dsc->shadow_چوڑائی) {
            اگر(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, part);
                اگر(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_انحراف_x = lv_obj_get_style_shadow_انحراف_x_اندرونی(obj, part);
                    draw_dsc->shadow_انحراف_y = lv_obj_get_style_shadow_انحراف_y_اندرونی(obj, part);
                    draw_dsc->shadow_پھیلاؤ = lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_اندرونی(obj, part);
                    draw_dsc->shadow_color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, shadow_color);
                }
            }
        }
    }

    اگر(opa < LV_OPA_MAX) {
        draw_dsc->bg_opa = LV_OPA_MIX2(draw_dsc->bg_opa, opa);
        draw_dsc->bg_image_opa = LV_OPA_MIX2(draw_dsc->bg_image_opa, opa);
        draw_dsc->border_opa = LV_OPA_MIX2(draw_dsc->border_opa, opa);
        draw_dsc->shadow_opa = LV_OPA_MIX2(draw_dsc->shadow_opa, opa);
        draw_dsc->outline_opa = LV_OPA_MIX2(draw_dsc->outline_opa, opa);
    }

    drop_shadow_init(obj, part, &draw_dsc->بنیاد);

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    draw_dsc->opa = lv_obj_get_style_متن_opa_اندرونی(obj, part);
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنیاد);
    اگر(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }


    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_اندرونی(obj, part);
    draw_dsc->color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, متن_color);
    draw_dsc->letter_space = lv_obj_get_style_متن_letter_space_اندرونی(obj, part);
    draw_dsc->سطر_space = lv_obj_get_style_متن_سطر_space_اندرونی(obj, part);
    draw_dsc->decor = lv_obj_get_style_متن_decor_اندرونی(obj, part);

    draw_dsc->font = lv_obj_get_style_متن_font_اندرونی(obj, part);

#اگر LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN);
#اگر_ختم

    draw_dsc->align = lv_obj_get_style_متن_align_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنیاد);

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_اندرونی(obj, part);
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنیاد);
    اگر(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->محور.x = lv_area_get_چوڑائی(&obj->coords) / 2;
    draw_dsc->محور.y = lv_area_get_اونچائی(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_اندرونی(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_اندرونی(obj, part);
    lv_color32_t نتیجہ = image_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, recolor, recolor_opa);
    draw_dsc->recolor_opa = نتیجہ.alpha;
    draw_dsc->recolor = lv_color_make(نتیجہ.red, نتیجہ.green, نتیجہ.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_اندرونی(obj, part);

    اگر(part != LV_PART_MAIN) draw_dsc->blend_منوال = lv_obj_get_style_blend_منوال_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنیاد);

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_init_draw_سطر_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_سطر_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    draw_dsc->opa = lv_obj_get_style_سطر_opa_اندرونی(obj, part);
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنیاد);
    اگر(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    draw_dsc->چوڑائی = lv_obj_get_style_سطر_چوڑائی_اندرونی(obj, part);
    اگر(draw_dsc->چوڑائی == 0) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_color_t سطر_color = lv_obj_get_style_سطر_color_filtered_اندرونی(obj, part);
    draw_dsc->color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, سطر_color);

    draw_dsc->dash_چوڑائی = lv_obj_get_style_سطر_dash_چوڑائی_اندرونی(obj, part);
    اگر(draw_dsc->dash_چوڑائی) {
        draw_dsc->dash_gap = lv_obj_get_style_سطر_dash_gap_اندرونی(obj, part);
    }

    draw_dsc->باری_آغاز = lv_obj_get_style_سطر_rounded_اندرونی(obj, part);
    draw_dsc->باری_انتہا = draw_dsc->باری_آغاز;

    drop_shadow_init(obj, part, &draw_dsc->بنیاد);

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    draw_dsc->چوڑائی = lv_obj_get_style_arc_چوڑائی_اندرونی(obj, part);
    اگر(draw_dsc->چوڑائی == 0) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_اندرونی(obj, part);
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنیاد);
    اگر(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    اگر(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_انتہا;
        واپس;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_اندرونی(obj, part);
    draw_dsc->color = معمول_apply_layer_recolor(obj, part, &draw_dsc->بنیاد, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_اندرونی(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_اندرونی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنیاد);

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(draw_dsc != تہی, واپس);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنیاد.obj = obj;
    draw_dsc->بنیاد.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_اندرونی(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_اندرونی(obj, part);

    /*Radius might be set earlier as it's already known*/
    اگر(draw_dsc->کونا_radius == 0) {
        draw_dsc->کونا_radius = lv_obj_get_style_radius_اندرونی(obj, part);
    }

    LV_PROFILER_DRAW_انتہا;
}

خالی lv_obj_سیٹ_ext_draw_سائز(lv_obj_t * obj, int32_t سائز)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_remove_event_cb(obj, سیٹ_ext_draw_سائز_event_cb);
    lv_obj_جوڑو_event_cb(obj, سیٹ_ext_draw_سائز_event_cb, LV_EVENT_REFR_EXT_DRAW_سائز, (خالی *)(lv_uintptr_t)سائز);
}

int32_t lv_obj_calculate_ext_draw_سائز(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_چوڑائی = lv_obj_get_style_shadow_چوڑائی_اندرونی(obj, part);
    اگر(sh_چوڑائی) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_اندرونی(obj, part);
        اگر(sh_opa > LV_OPA_MIN) {
            sh_چوڑائی = sh_چوڑائی / 2 + 1;    /*The blur adds only half width*/
            sh_چوڑائی += lv_obj_get_style_shadow_پھیلاؤ_اندرونی(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_انحراف_x_اندرونی(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_انحراف_y_اندرونی(obj, part);
            sh_چوڑائی += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_چوڑائی);
        }
    }

    int32_t outline_چوڑائی = lv_obj_get_style_outline_چوڑائی_اندرونی(obj, part);
    اگر(outline_چوڑائی) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_اندرونی(obj, part);
        اگر(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_اندرونی(obj, part);
            s = LV_MAX(s, outline_pad + outline_چوڑائی);
        }
    }

    int32_t drop_shadow_سائز = 0;
    اگر(lv_obj_get_style_drop_shadow_opa_اندرونی(obj, part) > 0) {
        drop_shadow_سائز += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_انحراف_x_اندرونی(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_انحراف_y_اندرونی(obj, part)));

        drop_shadow_سائز += lv_obj_get_style_drop_shadow_radius_اندرونی(obj, part) + 1;
    }
    s += drop_shadow_سائز;

    int32_t w = lv_obj_get_style_transform_چوڑائی_اندرونی(obj, part);
    int32_t h = lv_obj_get_style_transform_اونچائی_اندرونی(obj, part);
    int32_t wh = LV_MAX(w, h);
    اگر(wh > 0) s += wh;

    LV_PROFILER_DRAW_انتہا;
    واپس s;
}

خالی lv_obj_refresh_ext_draw_سائز(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    int32_t s_old = lv_obj_get_ext_draw_سائز(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_سائز, &s_new);

    /*Store the result if the special attrs already allocated*/
    اگر(obj->spec_attr) {
        obj->spec_attr->ext_draw_سائز = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    ورنہ اگر(s_new != 0) {
        اگر(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_انتہا;
            واپس;
        }
        obj->spec_attr->ext_draw_سائز = s_new;
    }

    اگر(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_انتہا;
}

int32_t lv_obj_get_ext_draw_سائز(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    اگر(obj->spec_attr) واپس obj->spec_attr->ext_draw_سائز;
    ورنہ واپس 0;
}

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_LAYER_TYPE_NONE);

    اگر(obj->spec_attr) واپس (lv_layer_type_t)obj->spec_attr->layer_type;
    ورنہ واپس LV_LAYER_TYPE_NONE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن اندرونی lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc)
{
    اگر(بنیاد_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        اگر(part == LV_PART_MAIN) {
            واپس بنیاد_dsc->layer->opa;
        }

        واپس LV_OPA_MIX2(بنیاد_dsc->layer->opa, lv_obj_get_style_opa_اندرونی(obj, part));
    }

    /* fallback to old recursive style opa */
    واپس lv_obj_get_style_opa_recursive(obj, part);
}


ساکن lv_color_t معمول_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    اگر(بنیاد_dsc->layer) {
        recolor = بنیاد_dsc->layer->recolor;
        اگر(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ورنہ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    واپس lv_color_mix(lv_color_make(recolor.red, recolor.green, recolor.blue), color, recolor.alpha);
}


ساکن lv_color32_t image_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,
                                              ثابت lv_draw_dsc_بنیاد_t * بنیاد_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    اگر(بنیاد_dsc->layer) {
        recolor = بنیاد_dsc->layer->recolor;
        اگر(part != LV_PART_MAIN) {
            recolor = lv_obj_style_apply_recolor(obj, part, recolor);
        }
    }
    ورنہ {
        recolor = lv_obj_get_style_recolor_recursive(obj, part);
    }

    اگر(opa > LV_OPA_TRANSP && recolor.alpha > LV_OPA_TRANSP) {
        واپس lv_color_over32(recolor, lv_color_مقصد_32(color, opa));
    }
    ورنہ اگر(recolor.alpha > LV_OPA_TRANSP) {
        واپس recolor;
    }
    ورنہ {
        واپس lv_color_مقصد_32(color, opa);
    }
}

ساکن خالی drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_بنیاد_t * بنیاد_dsc)
{
    بنیاد_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_اندرونی(obj, part);
    اگر(بنیاد_dsc->drop_shadow_opa) {
        بنیاد_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_اندرونی(obj, part);
        بنیاد_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_انحراف_x_اندرونی(obj, part);
        بنیاد_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_انحراف_y_اندرونی(obj, part);
        بنیاد_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_اندرونی(obj, part);
        بنیاد_dsc->drop_shadow_color = معمول_apply_layer_recolor(obj, part, بنیاد_dsc, بنیاد_dsc->drop_shadow_color);

        بنیاد_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_اندرونی(obj, part);
    }
}

ساکن خالی سیٹ_ext_draw_سائز_event_cb(lv_event_t * e)
{
    int32_t سائز = (int32_t)(lv_uintptr_t)lv_event_get_user_ڈیٹا(e);
    lv_event_سیٹ_ext_draw_سائز(e, سائز);
}
