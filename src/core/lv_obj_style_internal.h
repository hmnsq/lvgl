/**
 * @file lv_obj_style_internal.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_STYLE_ਅੰਦਰੂਨੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_STYLE_ਅੰਦਰੂਨੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ਸਥਿਰ ਸਤਰਅੰਦਰ lv_ਪਾਠ_align_t lv_obj_calculate_style_ਪਾਠ_align_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part,
                                                                         ਅਚਲ ਅੱਖਰ * txt)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    LV_ASSERT(txt != ਨਿਰਾ);

    lv_ਪਾਠ_align_t align = lv_obj_get_style_ਪਾਠ_align_ਅੰਦਰੂਨੀ(obj, part);
    lv_ਅਧਾਰ_dir_t ਅਧਾਰ_dir = lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, part);
    lv_bidi_calculate_align(&align, &ਅਧਾਰ_dir, txt);
    ਵਾਪਸ align;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t padding = lv_obj_get_style_pad_ਖੱਬਾ(obj, part);
    int32_t border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ਵਾਪਸ (border_side & LV_BORDER_SIDE_ਖੱਬਾ) ? padding + border_ਚੌੜਾਈ : padding;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t padding = lv_obj_get_style_pad_ਸੱਜਾ(obj, part);
    int32_t border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ਵਾਪਸ (border_side & LV_BORDER_SIDE_ਸੱਜਾ) ? padding + border_ਚੌੜਾਈ : padding;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t padding = lv_obj_get_style_pad_ਸਿਖਰ(obj, part);
    int32_t border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ਵਾਪਸ (border_side & LV_BORDER_SIDE_ਸਿਖਰ) ? padding + border_ਚੌੜਾਈ : padding;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_ਚੌੜਾਈ = lv_obj_get_style_border_ਚੌੜਾਈ(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ਵਾਪਸ (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_ਚੌੜਾਈ : padding;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_transform_scale_x_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    ਵਾਪਸ scale > 0 ? scale : 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t lv_obj_get_style_transform_scale_y_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    ਵਾਪਸ scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBJ_STYLE_INTERNAL_H*/
