/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
बाह्य "C" {
#endif

#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

स्थिर ओळीत lv_text_align_t lv_obj_calculate_style_text_align_internal(स्थिरांक lv_obj_t * obj, lv_part_t part,
                                                                         स्थिरांक अक्षर * txt)
{
    LV_ASSERT(obj != रिक्त);
    LV_ASSERT(txt != रिक्त);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    परत align;
}

स्थिर ओळीत int32_t lv_obj_get_style_space_डावा_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_डावा(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    परत (border_side & LV_BORDER_SIDE_डावा) ? padding + border_width : padding;
}

स्थिर ओळीत int32_t lv_obj_get_style_space_उजवा_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_उजवा(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    परत (border_side & LV_BORDER_SIDE_उजवा) ? padding + border_width : padding;
}

स्थिर ओळीत int32_t lv_obj_get_style_space_शीर्ष_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_शीर्ष(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    परत (border_side & LV_BORDER_SIDE_शीर्ष) ? padding + border_width : padding;
}

स्थिर ओळीत int32_t lv_obj_get_style_space_bottom_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    परत (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

स्थिर ओळीत int32_t lv_obj_get_style_transform_scale_x_safe_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    परत scale > 0 ? scale : 1;
}

स्थिर ओळीत int32_t lv_obj_get_style_transform_scale_y_safe_internal(स्थिरांक lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    परत scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
