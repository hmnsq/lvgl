/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
بیرونی "C" {
#endif

#شامل "../lvgl_public.h"
#شامل "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ایستا درونخطی lv_text_align_t lv_obj_calculate_style_text_align_internal(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت نویسه * txt)
{
    LV_ASSERT(obj != پوچ);
    LV_ASSERT(txt != پوچ);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    بازگشت align;
}

ایستا درونخطی int32_t lv_obj_get_style_space_چپ_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t padding = lv_obj_get_style_pad_چپ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    بازگشت (border_side & LV_BORDER_SIDE_چپ) ? padding + border_width : padding;
}

ایستا درونخطی int32_t lv_obj_get_style_space_راست_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t padding = lv_obj_get_style_pad_راست(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    بازگشت (border_side & LV_BORDER_SIDE_راست) ? padding + border_width : padding;
}

ایستا درونخطی int32_t lv_obj_get_style_space_بالاترین_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t padding = lv_obj_get_style_pad_بالاترین(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    بازگشت (border_side & LV_BORDER_SIDE_بالاترین) ? padding + border_width : padding;
}

ایستا درونخطی int32_t lv_obj_get_style_space_bottom_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    بازگشت (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

ایستا درونخطی int32_t lv_obj_get_style_transform_scale_x_safe_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    بازگشت scale > 0 ? scale : 1;
}

ایستا درونخطی int32_t lv_obj_get_style_transform_scale_y_safe_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != پوچ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    بازگشت scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
