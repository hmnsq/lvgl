/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
বাহ্য "C" {
#endif

#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

স্থির অন্তঃস্থ lv_text_align_t lv_obj_calculate_style_text_align_internal(ধ্রুব lv_obj_t * obj, lv_part_t part,
                                                                         ধ্রুব বর্ণ * txt)
{
    LV_ASSERT(obj != রিক্ত);
    LV_ASSERT(txt != রিক্ত);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    প্রত্যাগম align;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_space_বাম_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t padding = lv_obj_get_style_pad_বাম(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    প্রত্যাগম (border_side & LV_BORDER_SIDE_বাম) ? padding + border_width : padding;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_space_ডান_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t padding = lv_obj_get_style_pad_ডান(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    প্রত্যাগম (border_side & LV_BORDER_SIDE_ডান) ? padding + border_width : padding;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_space_শীর্ষ_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t padding = lv_obj_get_style_pad_শীর্ষ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    প্রত্যাগম (border_side & LV_BORDER_SIDE_শীর্ষ) ? padding + border_width : padding;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_space_bottom_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    প্রত্যাগম (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_transform_scale_x_safe_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    প্রত্যাগম scale > 0 ? scale : 1;
}

স্থির অন্তঃস্থ int32_t lv_obj_get_style_transform_scale_y_safe_internal(ধ্রুব lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != রিক্ত);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    প্রত্যাগম scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
