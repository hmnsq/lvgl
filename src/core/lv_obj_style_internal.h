/**
 * @file lv_obj_style_internal.h
 *
 */

#अगर_अघोषित LV_OBJ_STYLE_INTERNAL_H
#घोषणा LV_OBJ_STYLE_INTERNAL_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

#समावेश "../lvgl_public.h"
#समावेश "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

अटल अंतरभूत lv_text_align_t lv_obj_calculate_style_text_align_internal(स्थिर lv_obj_t * obj, lv_part_t part,
                                                                         स्थिर अक्षर * txt)
{
    LV_ASSERT(obj != शून्य);
    LV_ASSERT(txt != शून्य);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    वापस align;
}

अटल अंतरभूत int32_t lv_obj_get_style_space_बायाँ_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t padding = lv_obj_get_style_pad_बायाँ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    वापस (border_side & LV_BORDER_SIDE_बायाँ) ? padding + border_width : padding;
}

अटल अंतरभूत int32_t lv_obj_get_style_space_दायाँ_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t padding = lv_obj_get_style_pad_दायाँ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    वापस (border_side & LV_BORDER_SIDE_दायाँ) ? padding + border_width : padding;
}

अटल अंतरभूत int32_t lv_obj_get_style_space_शीर्ष_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t padding = lv_obj_get_style_pad_शीर्ष(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    वापस (border_side & LV_BORDER_SIDE_शीर्ष) ? padding + border_width : padding;
}

अटल अंतरभूत int32_t lv_obj_get_style_space_bottom_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    वापस (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

अटल अंतरभूत int32_t lv_obj_get_style_transform_scale_x_safe_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    वापस scale > 0 ? scale : 1;
}

अटल अंतरभूत int32_t lv_obj_get_style_transform_scale_y_safe_internal(स्थिर lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != शून्य);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    वापस scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_OBJ_STYLE_INTERNAL_H*/
