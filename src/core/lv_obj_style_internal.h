/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

സ്ഥിതം വരിയിൽ lv_text_align_t lv_obj_calculate_style_text_align_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part,
                                                                         സ്ഥിരം അക്ഷരം * txt)
{
    LV_ASSERT(obj != ശൂന്യ);
    LV_ASSERT(txt != ശൂന്യ);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    തിരികെ align;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_space_ഇടത്_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t padding = lv_obj_get_style_pad_ഇടത്(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    തിരികെ (border_side & LV_BORDER_SIDE_ഇടത്) ? padding + border_width : padding;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_space_വലത്_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t padding = lv_obj_get_style_pad_വലത്(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    തിരികെ (border_side & LV_BORDER_SIDE_വലത്) ? padding + border_width : padding;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_space_മുകൾ_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t padding = lv_obj_get_style_pad_മുകൾ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    തിരികെ (border_side & LV_BORDER_SIDE_മുകൾ) ? padding + border_width : padding;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_space_bottom_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    തിരികെ (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_transform_scale_x_safe_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    തിരികെ scale > 0 ? scale : 1;
}

സ്ഥിതം വരിയിൽ int32_t lv_obj_get_style_transform_scale_y_safe_internal(സ്ഥിരം lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ശൂന്യ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    തിരികെ scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
