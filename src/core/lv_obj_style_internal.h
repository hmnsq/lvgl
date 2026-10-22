/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
خارجي "C" {
#endif

#ضمن "../lvgl_public.h"
#ضمن "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ساكن مضمن lv_text_align_t lv_obj_calculate_style_text_align_internal(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != عدم);
    LV_ASSERT(txt != عدم);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    أرجع align;
}

ساكن مضمن int32_t lv_obj_get_style_space_بایاں_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_بایاں(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_بایاں) ? padding + border_width : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_دایاں_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_دایاں(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_دایاں) ? padding + border_width : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_چوٹی_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_چوٹی(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_چوٹی) ? padding + border_width : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_bottom_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_x_safe_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    أرجع scale > 0 ? scale : 1;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_y_safe_internal(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    أرجع scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
