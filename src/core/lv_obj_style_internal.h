/**
 * @file lv_obj_style_internal.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBJ_STYLE_ಆಂತರಿಕ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_STYLE_ಆಂತರಿಕ_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

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

ಸ್ಥಿರ ಸಾಲೊಳಗೆ lv_ಪಠ್ಯ_align_t lv_obj_calculate_style_ಪಠ್ಯ_align_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part,
                                                                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * txt)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    LV_ASSERT(txt != ಖಾಲಿ);

    lv_ಪಠ್ಯ_align_t align = lv_obj_get_style_ಪಠ್ಯ_align_ಆಂತರಿಕ(obj, part);
    lv_ತಳ_dir_t ತಳ_dir = lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, part);
    lv_bidi_calculate_align(&align, &ತಳ_dir, txt);
    ಹಿಂತಿರುಗಿ align;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t padding = lv_obj_get_style_pad_ಎಡ(obj, part);
    int32_t border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಹಿಂತಿರುಗಿ (border_side & LV_BORDER_SIDE_ಎಡ) ? padding + border_ಅಗಲ : padding;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t padding = lv_obj_get_style_pad_ಬಲ(obj, part);
    int32_t border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಹಿಂತಿರುಗಿ (border_side & LV_BORDER_SIDE_ಬಲ) ? padding + border_ಅಗಲ : padding;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t padding = lv_obj_get_style_pad_ತುದಿ(obj, part);
    int32_t border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಹಿಂತಿರುಗಿ (border_side & LV_BORDER_SIDE_ತುದಿ) ? padding + border_ಅಗಲ : padding;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_space_bottom_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_ಅಗಲ = lv_obj_get_style_border_ಅಗಲ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ಹಿಂತಿರುಗಿ (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_ಅಗಲ : padding;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_transform_scale_x_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    ಹಿಂತಿರುಗಿ scale > 0 ? scale : 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t lv_obj_get_style_transform_scale_y_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ಖಾಲಿ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    ಹಿಂತಿರುಗಿ scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_OBJ_STYLE_INTERNAL_H*/
