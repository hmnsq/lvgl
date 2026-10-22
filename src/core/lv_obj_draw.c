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
#تعریف_کړه MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن مضمن lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc);
ساكن lv_color_t نورمال_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc,
                                             lv_color_t color);
ساكن lv_color32_t image_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,
                                              ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc, lv_color_t color, lv_opa_t opa);

ساكن فراغ drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_بنسټ_t * بنسټ_dsc);

ساكن فراغ سېټ_ext_draw_اندازه_event_cb(lv_event_t * e);

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
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنسټ);
    إذا(part != LV_PART_MAIN) {
        إذا(opa <= LV_OPA_MIN) {
            draw_dsc->bg_opa = LV_OPA_TRANSP;
            draw_dsc->bg_image_opa = LV_OPA_TRANSP;
            draw_dsc->border_opa = LV_OPA_TRANSP;
            draw_dsc->outline_opa = LV_OPA_TRANSP;
            draw_dsc->shadow_opa = LV_OPA_TRANSP;
            LV_PROFILER_DRAW_پای_ټکی;
            أرجع;
        }
    }

    draw_dsc->radius = lv_obj_get_style_radius_دننی(obj, part);

    إذا(draw_dsc->bg_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_opa = lv_obj_get_style_bg_opa_دننی(obj, part);
        إذا(draw_dsc->bg_opa > LV_OPA_MIN) {
            lv_color_t bg_color = lv_obj_get_style_bg_color_filtered_دننی(obj, part);
            draw_dsc->bg_color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, bg_color);
            ثابت lv_grad_dsc_t * grad = lv_obj_get_style_bg_grad_دننی(obj, part);
            إذا(grad && grad->dir != LV_GRAD_DIR_NONE) {
                lv_memcpy(&draw_dsc->bg_grad, grad, حجم(*grad));
            }
            وإلا {
                draw_dsc->bg_grad.dir = lv_obj_get_style_bg_grad_dir_دننی(obj, part);
                إذا(draw_dsc->bg_grad.dir != LV_GRAD_DIR_NONE) {
                    draw_dsc->bg_grad.stops[0].color = draw_dsc->bg_color;
                    lv_color_t bg_grad_color = lv_obj_get_style_bg_grad_color_filtered_دننی(obj, part);
                    draw_dsc->bg_grad.stops[1].color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, bg_grad_color);
                    draw_dsc->bg_grad.stops[0].frac = lv_obj_get_style_bg_main_stop_دننی(obj, part);
                    draw_dsc->bg_grad.stops[1].frac = lv_obj_get_style_bg_grad_stop_دننی(obj, part);
                    draw_dsc->bg_grad.stops[0].opa = lv_obj_get_style_bg_main_opa_دننی(obj, part);
                    draw_dsc->bg_grad.stops[1].opa = lv_obj_get_style_bg_grad_opa_دننی(obj, part);
                }
            }
        }
    }

    إذا(draw_dsc->border_opa != LV_OPA_TRANSP) {
        draw_dsc->border_پلنوالی = lv_obj_get_style_border_پلنوالی_دننی(obj, part);
        إذا(draw_dsc->border_پلنوالی) {
            draw_dsc->border_opa = lv_obj_get_style_border_opa_دننی(obj, part);
            إذا(draw_dsc->border_opa > LV_OPA_MIN) {
                draw_dsc->border_side = lv_obj_get_style_border_side_دننی(obj, part);
                lv_color_t border_color = lv_obj_get_style_border_color_filtered_دننی(obj, part);
                draw_dsc->border_color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, border_color);
            }
        }
    }

    إذا(draw_dsc->outline_opa != LV_OPA_TRANSP) {
        draw_dsc->outline_پلنوالی = lv_obj_get_style_outline_پلنوالی_دننی(obj, part);
        إذا(draw_dsc->outline_پلنوالی) {
            draw_dsc->outline_opa = lv_obj_get_style_outline_opa_دننی(obj, part);
            إذا(draw_dsc->outline_opa > LV_OPA_MIN) {
                draw_dsc->outline_pad = lv_obj_get_style_outline_pad_دننی(obj, part);
                lv_color_t outline_color = lv_obj_get_style_outline_color_filtered_دننی(obj, part);
                draw_dsc->outline_color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, outline_color);
            }
        }
    }

    إذا(draw_dsc->bg_image_opa != LV_OPA_TRANSP) {
        draw_dsc->bg_image_src = lv_obj_get_style_bg_image_src_دننی(obj, part);
        إذا(draw_dsc->bg_image_src) {
            draw_dsc->bg_image_opa = lv_obj_get_style_bg_image_opa_دننی(obj, part);
            إذا(draw_dsc->bg_image_opa > LV_OPA_MIN) {
                إذا(lv_image_src_get_type(draw_dsc->bg_image_src) == LV_IMAGE_SRC_نښه) {
                    draw_dsc->bg_image_نښه_font = lv_obj_get_style_متن_font_دننی(obj, part);
                    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_دننی(obj, part);
                    draw_dsc->bg_image_recolor = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, متن_color);
                }
                وإلا {
                    lv_color_t bg_image_recolor = lv_obj_get_style_bg_image_recolor_filtered_دننی(obj, part);
                    lv_opa_t bg_image_recolor_opa = lv_obj_get_style_bg_image_recolor_opa_دننی(obj, part);
                    lv_color32_t پایله = image_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, bg_image_recolor, bg_image_recolor_opa);
                    draw_dsc->bg_image_recolor_opa = پایله.alpha;
                    draw_dsc->bg_image_recolor = lv_color_make(پایله.red, پایله.green, پایله.blue);
                    draw_dsc->bg_image_tiled = lv_obj_get_style_bg_image_tiled_دننی(obj, part);
                }

                draw_dsc->bg_image_colorkey = lv_obj_get_style_image_colorkey_دننی(obj, part);
            }
        }
    }

    إذا(draw_dsc->shadow_opa) {
        draw_dsc->shadow_پلنوالی = lv_obj_get_style_shadow_پلنوالی_دننی(obj, part);
        إذا(draw_dsc->shadow_پلنوالی) {
            إذا(draw_dsc->shadow_opa > LV_OPA_MIN) {
                draw_dsc->shadow_opa = lv_obj_get_style_shadow_opa_دننی(obj, part);
                إذا(draw_dsc->shadow_opa > LV_OPA_MIN) {
                    draw_dsc->shadow_دننه_واټن_x = lv_obj_get_style_shadow_دننه_واټن_x_دننی(obj, part);
                    draw_dsc->shadow_دننه_واټن_y = lv_obj_get_style_shadow_دننه_واټن_y_دننی(obj, part);
                    draw_dsc->shadow_خپرېدنه = lv_obj_get_style_shadow_خپرېدنه_دننی(obj, part);
                    lv_color_t shadow_color = lv_obj_get_style_shadow_color_filtered_دننی(obj, part);
                    draw_dsc->shadow_color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, shadow_color);
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

    drop_shadow_init(obj, part, &draw_dsc->بنسټ);

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_init_draw_label_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_label_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    draw_dsc->opa = lv_obj_get_style_متن_opa_دننی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنسټ);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }


    lv_color_t متن_color = lv_obj_get_style_متن_color_filtered_دننی(obj, part);
    draw_dsc->color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, متن_color);
    draw_dsc->letter_space = lv_obj_get_style_متن_letter_space_دننی(obj, part);
    draw_dsc->کرښه_space = lv_obj_get_style_متن_کرښه_space_دننی(obj, part);
    draw_dsc->decor = lv_obj_get_style_متن_decor_دننی(obj, part);

    draw_dsc->font = lv_obj_get_style_متن_font_دننی(obj, part);

#إذا LV_USE_BIDI
    draw_dsc->bidi_dir = lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_MAIN);
#د_که_پای

    draw_dsc->align = lv_obj_get_style_متن_align_دننی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنسټ);

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_init_draw_image_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_image_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    draw_dsc->opa = lv_obj_get_style_image_opa_دننی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنسټ);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    draw_dsc->rotation = 0;
    draw_dsc->scale_x = LV_SCALE_NONE;
    draw_dsc->scale_y = LV_SCALE_NONE;
    draw_dsc->محور.x = lv_area_get_پلنوالی(&obj->coords) / 2;
    draw_dsc->محور.y = lv_area_get_لوړوالی(&obj->coords) / 2;

    lv_color_t recolor = lv_obj_get_style_image_recolor_filtered_دننی(obj, part);
    lv_opa_t recolor_opa = lv_obj_get_style_image_recolor_opa_دننی(obj, part);
    lv_color32_t پایله = image_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, recolor, recolor_opa);
    draw_dsc->recolor_opa = پایله.alpha;
    draw_dsc->recolor = lv_color_make(پایله.red, پایله.green, پایله.blue);

    draw_dsc->colorkey = lv_obj_get_style_image_colorkey_دننی(obj, part);

    إذا(part != LV_PART_MAIN) draw_dsc->blend_ډېرپېښ = lv_obj_get_style_blend_ډېرپېښ_دننی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنسټ);

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_init_draw_کرښه_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_کرښه_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    draw_dsc->opa = lv_obj_get_style_کرښه_opa_دننی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنسټ);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    draw_dsc->پلنوالی = lv_obj_get_style_کرښه_پلنوالی_دننی(obj, part);
    إذا(draw_dsc->پلنوالی == 0) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_color_t کرښه_color = lv_obj_get_style_کرښه_color_filtered_دننی(obj, part);
    draw_dsc->color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, کرښه_color);

    draw_dsc->dash_پلنوالی = lv_obj_get_style_کرښه_dash_پلنوالی_دننی(obj, part);
    إذا(draw_dsc->dash_پلنوالی) {
        draw_dsc->dash_gap = lv_obj_get_style_کرښه_dash_gap_دننی(obj, part);
    }

    draw_dsc->پړاو_پیل = lv_obj_get_style_کرښه_rounded_دننی(obj, part);
    draw_dsc->پړاو_پای_ټکی = draw_dsc->پړاو_پیل;

    drop_shadow_init(obj, part, &draw_dsc->بنسټ);

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_init_draw_arc_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_arc_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    draw_dsc->پلنوالی = lv_obj_get_style_arc_پلنوالی_دننی(obj, part);
    إذا(draw_dsc->پلنوالی == 0) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    draw_dsc->opa = lv_obj_get_style_arc_opa_دننی(obj, part);
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_opa_t opa = get_layer_opa(obj, part, &draw_dsc->بنسټ);
    إذا(opa < LV_OPA_MAX) {
        draw_dsc->opa = LV_OPA_MIX2(draw_dsc->opa, opa);
    }
    إذا(draw_dsc->opa <= LV_OPA_MIN) {
        LV_PROFILER_DRAW_پای_ټکی;
        أرجع;
    }

    lv_color_t arc_color = lv_obj_get_style_arc_color_filtered_دننی(obj, part);
    draw_dsc->color = نورمال_apply_layer_recolor(obj, part, &draw_dsc->بنسټ, arc_color);
    draw_dsc->img_src = lv_obj_get_style_arc_image_src_دننی(obj, part);

    draw_dsc->rounded = lv_obj_get_style_arc_rounded_دننی(obj, part);

    drop_shadow_init(obj, part, &draw_dsc->بنسټ);

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_init_draw_blur_dsc(lv_obj_t * obj, lv_part_t part, lv_draw_blur_dsc_t * draw_dsc)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(draw_dsc != عدم, أرجع);

    LV_PROFILER_DRAW_BEGIN;
    draw_dsc->بنسټ.obj = obj;
    draw_dsc->بنسټ.part = part;

    draw_dsc->blur_radius = lv_obj_get_style_blur_radius_دننی(obj, part);
    draw_dsc->quality = lv_obj_get_style_blur_quality_دننی(obj, part);

    /*Radius might be set earlier as it's already known*/
    إذا(draw_dsc->کونج_radius == 0) {
        draw_dsc->کونج_radius = lv_obj_get_style_radius_دننی(obj, part);
    }

    LV_PROFILER_DRAW_پای_ټکی;
}

فراغ lv_obj_سېټ_ext_draw_اندازه(lv_obj_t * obj, int32_t اندازه)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_remove_event_cb(obj, سېټ_ext_draw_اندازه_event_cb);
    lv_obj_جمع_کړه_event_cb(obj, سېټ_ext_draw_اندازه_event_cb, LV_EVENT_REFR_EXT_DRAW_اندازه, (فراغ *)(lv_uintptr_t)اندازه);
}

int32_t lv_obj_calculate_ext_draw_اندازه(lv_obj_t * obj, lv_part_t part)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    LV_PROFILER_DRAW_BEGIN;
    int32_t s = 0;

    int32_t sh_پلنوالی = lv_obj_get_style_shadow_پلنوالی_دننی(obj, part);
    إذا(sh_پلنوالی) {
        lv_opa_t sh_opa = lv_obj_get_style_shadow_opa_دننی(obj, part);
        إذا(sh_opa > LV_OPA_MIN) {
            sh_پلنوالی = sh_پلنوالی / 2 + 1;    /*The blur adds only half width*/
            sh_پلنوالی += lv_obj_get_style_shadow_خپرېدنه_دننی(obj, part);
            int32_t sh_ofs_x = lv_obj_get_style_shadow_دننه_واټن_x_دننی(obj, part);
            int32_t sh_ofs_y = lv_obj_get_style_shadow_دننه_واټن_y_دننی(obj, part);
            sh_پلنوالی += LV_MAX(LV_ABS(sh_ofs_x), LV_ABS(sh_ofs_y));
            s = LV_MAX(s, sh_پلنوالی);
        }
    }

    int32_t outline_پلنوالی = lv_obj_get_style_outline_پلنوالی_دننی(obj, part);
    إذا(outline_پلنوالی) {
        lv_opa_t outline_opa = lv_obj_get_style_outline_opa_دننی(obj, part);
        إذا(outline_opa > LV_OPA_MIN) {
            int32_t outline_pad = lv_obj_get_style_outline_pad_دننی(obj, part);
            s = LV_MAX(s, outline_pad + outline_پلنوالی);
        }
    }

    int32_t drop_shadow_اندازه = 0;
    إذا(lv_obj_get_style_drop_shadow_opa_دننی(obj, part) > 0) {
        drop_shadow_اندازه += LV_MAX(LV_ABS(lv_obj_get_style_drop_shadow_دننه_واټن_x_دننی(obj, part)),
                                   LV_ABS(lv_obj_get_style_drop_shadow_دننه_واټن_y_دننی(obj, part)));

        drop_shadow_اندازه += lv_obj_get_style_drop_shadow_radius_دننی(obj, part) + 1;
    }
    s += drop_shadow_اندازه;

    int32_t w = lv_obj_get_style_transform_پلنوالی_دننی(obj, part);
    int32_t h = lv_obj_get_style_transform_لوړوالی_دننی(obj, part);
    int32_t wh = LV_MAX(w, h);
    إذا(wh > 0) s += wh;

    LV_PROFILER_DRAW_پای_ټکی;
    أرجع s;
}

فراغ lv_obj_refresh_ext_draw_اندازه(lv_obj_t * obj)
{
    LV_PROFILER_DRAW_BEGIN;
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    int32_t s_old = lv_obj_get_ext_draw_اندازه(obj);
    int32_t s_new = 0;
    lv_obj_send_event(obj, LV_EVENT_REFR_EXT_DRAW_اندازه, &s_new);

    /*Store the result if the special attrs already allocated*/
    إذا(obj->spec_attr) {
        obj->spec_attr->ext_draw_اندازه = s_new;
    }
    /*Allocate spec. attrs. only if the result is not zero.
     *Zero is the default value if the spec. attr. are not defined.*/
    وإلا إذا(s_new != 0) {
        إذا(!lv_obj_allocate_spec_attr(obj)) {
            LV_PROFILER_DRAW_پای_ټکی;
            أرجع;
        }
        obj->spec_attr->ext_draw_اندازه = s_new;
    }

    إذا(s_new != s_old) lv_obj_invalidate(obj);
    LV_PROFILER_DRAW_پای_ټکی;
}

int32_t lv_obj_get_ext_draw_اندازه(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr) أرجع obj->spec_attr->ext_draw_اندازه;
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

ساكن مضمن lv_opa_t get_layer_opa(ثابت lv_obj_t * obj, lv_part_t part, ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc)
{
    إذا(بنسټ_dsc->layer) {
        /* Accessing the layer opa directly is faster than using get style opa recursive */
        إذا(part == LV_PART_MAIN) {
            أرجع بنسټ_dsc->layer->opa;
        }

        أرجع LV_OPA_MIX2(بنسټ_dsc->layer->opa, lv_obj_get_style_opa_دننی(obj, part));
    }

    /* fallback to old recursive style opa */
    أرجع lv_obj_get_style_opa_recursive(obj, part);
}


ساكن lv_color_t نورمال_apply_layer_recolor(ثابت lv_obj_t * obj, lv_part_t part,  ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc,
                                             lv_color_t color)
{
    lv_color32_t recolor;

    إذا(بنسټ_dsc->layer) {
        recolor = بنسټ_dsc->layer->recolor;
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
                                              ثابت lv_draw_dsc_بنسټ_t * بنسټ_dsc, lv_color_t color, lv_opa_t opa)
{
    lv_color32_t recolor;

    إذا(بنسټ_dsc->layer) {
        recolor = بنسټ_dsc->layer->recolor;
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

ساكن فراغ drop_shadow_init(ثابت lv_obj_t * obj, lv_part_t part, lv_draw_dsc_بنسټ_t * بنسټ_dsc)
{
    بنسټ_dsc->drop_shadow_opa = lv_obj_get_style_drop_shadow_opa_دننی(obj, part);
    إذا(بنسټ_dsc->drop_shadow_opa) {
        بنسټ_dsc->drop_shadow_blur_radius = lv_obj_get_style_drop_shadow_radius_دننی(obj, part);
        بنسټ_dsc->drop_shadow_ofs_x = lv_obj_get_style_drop_shadow_دننه_واټن_x_دننی(obj, part);
        بنسټ_dsc->drop_shadow_ofs_y = lv_obj_get_style_drop_shadow_دننه_واټن_y_دننی(obj, part);
        بنسټ_dsc->drop_shadow_color = lv_obj_get_style_drop_shadow_color_دننی(obj, part);
        بنسټ_dsc->drop_shadow_color = نورمال_apply_layer_recolor(obj, part, بنسټ_dsc, بنسټ_dsc->drop_shadow_color);

        بنسټ_dsc->drop_shadow_quality = lv_obj_get_style_drop_shadow_quality_دننی(obj, part);
    }
}

ساكن فراغ سېټ_ext_draw_اندازه_event_cb(lv_event_t * e)
{
    int32_t اندازه = (int32_t)(lv_uintptr_t)lv_event_get_user_معلومات(e);
    lv_event_سېټ_ext_draw_اندازه(e, اندازه);
}
