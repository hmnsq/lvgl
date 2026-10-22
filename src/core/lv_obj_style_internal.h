/**
 * @file lv_obj_style_internal.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_OBJ_STYLE_اندرونی_H
#تعریف_کرو LV_OBJ_STYLE_اندرونی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

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

ساکن اندرونی lv_متن_align_t lv_obj_calculate_style_متن_align_اندرونی(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != تہی);
    LV_ASSERT(txt != تہی);

    lv_متن_align_t align = lv_obj_get_style_متن_align_اندرونی(obj, part);
    lv_بنیاد_dir_t بنیاد_dir = lv_obj_get_style_بنیاد_dir_اندرونی(obj, part);
    lv_bidi_calculate_align(&align, &بنیاد_dir, txt);
    واپس align;
}

ساکن اندرونی int32_t lv_obj_get_style_space_بایاں_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t padding = lv_obj_get_style_pad_بایاں(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    واپس (border_side & LV_BORDER_SIDE_بایاں) ? padding + border_چوڑائی : padding;
}

ساکن اندرونی int32_t lv_obj_get_style_space_دایاں_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t padding = lv_obj_get_style_pad_دایاں(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    واپس (border_side & LV_BORDER_SIDE_دایاں) ? padding + border_چوڑائی : padding;
}

ساکن اندرونی int32_t lv_obj_get_style_space_چوٹی_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t padding = lv_obj_get_style_pad_چوٹی(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    واپس (border_side & LV_BORDER_SIDE_چوٹی) ? padding + border_چوڑائی : padding;
}

ساکن اندرونی int32_t lv_obj_get_style_space_bottom_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    واپس (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_چوڑائی : padding;
}

ساکن اندرونی int32_t lv_obj_get_style_transform_scale_x_محفوظ_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    واپس scale > 0 ? scale : 1;
}

ساکن اندرونی int32_t lv_obj_get_style_transform_scale_y_محفوظ_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != تہی);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    واپس scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_OBJ_STYLE_INTERNAL_H*/
