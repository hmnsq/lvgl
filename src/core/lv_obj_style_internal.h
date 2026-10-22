/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
வெளி "C" {
#endif

#அந்தர்பாவ "../lvgl_public.h"
#அந்தர்பாவ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

நிலையான உள்வரி lv_text_align_t lv_obj_calculate_style_text_align_internal(மாறிலி lv_obj_t * obj, lv_part_t part,
                                                                         மாறிலி எழுத்து * txt)
{
    LV_ASSERT(obj != வெற்று);
    LV_ASSERT(txt != வெற்று);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    திரும்பு align;
}

நிலையான உள்வரி int32_t lv_obj_get_style_space_இடது_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t padding = lv_obj_get_style_pad_இடது(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    திரும்பு (border_side & LV_BORDER_SIDE_இடது) ? padding + border_width : padding;
}

நிலையான உள்வரி int32_t lv_obj_get_style_space_வலது_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t padding = lv_obj_get_style_pad_வலது(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    திரும்பு (border_side & LV_BORDER_SIDE_வலது) ? padding + border_width : padding;
}

நிலையான உள்வரி int32_t lv_obj_get_style_space_உச்சி_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t padding = lv_obj_get_style_pad_உச்சி(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    திரும்பு (border_side & LV_BORDER_SIDE_உச்சி) ? padding + border_width : padding;
}

நிலையான உள்வரி int32_t lv_obj_get_style_space_bottom_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    திரும்பு (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

நிலையான உள்வரி int32_t lv_obj_get_style_transform_scale_x_safe_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    திரும்பு scale > 0 ? scale : 1;
}

நிலையான உள்வரி int32_t lv_obj_get_style_transform_scale_y_safe_internal(மாறிலி lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != வெற்று);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    திரும்பு scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
