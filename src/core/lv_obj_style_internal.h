/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ lv_text_align_t lv_obj_calculate_style_text_align_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part,
                                                                         ಧ್ರುವ ವರ್ಣ * txt)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    LV_ASSERT(txt != ರಿಕ್ತ);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    ಪ್ರತ್ಯಾಗಮ align;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_space_ಎಡ_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t padding = lv_obj_get_style_pad_ಎಡ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಪ್ರತ್ಯಾಗಮ (border_side & LV_BORDER_SIDE_ಎಡ) ? padding + border_width : padding;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_space_ಬಲ_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t padding = lv_obj_get_style_pad_ಬಲ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಪ್ರತ್ಯಾಗಮ (border_side & LV_BORDER_SIDE_ಬಲ) ? padding + border_width : padding;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_space_ತುದಿ_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t padding = lv_obj_get_style_pad_ತುದಿ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಪ್ರತ್ಯಾಗಮ (border_side & LV_BORDER_SIDE_ತುದಿ) ? padding + border_width : padding;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_space_bottom_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಪ್ರತ್ಯಾಗಮ (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_transform_scale_x_safe_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    ಪ್ರತ್ಯಾಗಮ scale > 0 ? scale : 1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int32_t lv_obj_get_style_transform_scale_y_safe_internal(ಧ್ರುವ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ರಿಕ್ತ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    ಪ್ರತ್ಯಾಗಮ scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
