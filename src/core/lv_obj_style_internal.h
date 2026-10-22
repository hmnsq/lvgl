/**
 * @file lv_obj_style_internal.h
 *
 */

#यदि_अपरिभाषितम् LV_OBJ_STYLE_आन्तरिकम्_H
#परिभाषय LV_OBJ_STYLE_आन्तरिकम्_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

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

स्थिर अन्तःस्थ lv_text_align_t lv_obj_calculate_style_text_align_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part,
                                                                         ध्रुव वर्ण * txt)
{
    LV_ASSERT(obj != रिक्त);
    LV_ASSERT(txt != रिक्त);

    lv_text_align_t align = lv_obj_get_style_text_align_आन्तरिकम्(obj, part);
    lv_आधारः_dir_t आधारः_dir = lv_obj_get_style_आधारः_dir_आन्तरिकम्(obj, part);
    lv_bidi_calculate_align(&align, &आधारः_dir, txt);
    प्रत्यागम align;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_space_वामः_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_वामः(obj, part);
    int32_t border_विस्तारः = lv_obj_get_style_border_विस्तारः(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    प्रत्यागम (border_side & LV_BORDER_SIDE_वामः) ? padding + border_विस्तारः : padding;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_space_दक्षिणः_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_दक्षिणः(obj, part);
    int32_t border_विस्तारः = lv_obj_get_style_border_विस्तारः(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    प्रत्यागम (border_side & LV_BORDER_SIDE_दक्षिणः) ? padding + border_विस्तारः : padding;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_space_शिखरम्_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_शिखरम्(obj, part);
    int32_t border_विस्तारः = lv_obj_get_style_border_विस्तारः(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    प्रत्यागम (border_side & LV_BORDER_SIDE_शिखरम्) ? padding + border_विस्तारः : padding;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_space_bottom_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_विस्तारः = lv_obj_get_style_border_विस्तारः(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    प्रत्यागम (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_विस्तारः : padding;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_transform_scale_x_safe_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    प्रत्यागम scale > 0 ? scale : 1;
}

स्थिर अन्तःस्थ int32_t lv_obj_get_style_transform_scale_y_safe_आन्तरिकम्(ध्रुव lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != रिक्त);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    प्रत्यागम scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_OBJ_STYLE_INTERNAL_H*/
