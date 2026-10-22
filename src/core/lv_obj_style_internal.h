/**
 * @file lv_obj_style_internal.h
 *
 */

#جے_بنا_تعریف LV_OBJ_STYLE_اندرونی_H
#تعریف_کرو LV_OBJ_STYLE_اندرونی_H

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

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

ساكن مضمن lv_متن_align_t lv_obj_calculate_style_متن_align_اندرونی(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != عدم);
    LV_ASSERT(txt != عدم);

    lv_متن_align_t align = lv_obj_get_style_متن_align_اندرونی(obj, part);
    lv_ادھار_dir_t ادھار_dir = lv_obj_get_style_ادھار_dir_اندرونی(obj, part);
    lv_bidi_calculate_align(&align, &ادھار_dir, txt);
    أرجع align;
}

ساكن مضمن int32_t lv_obj_get_style_space_بایاں_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_بایاں(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_بایاں) ? padding + border_چوڑائی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_دایاں_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_دایاں(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_دایاں) ? padding + border_چوڑائی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_چوٹی_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_چوٹی(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_چوٹی) ? padding + border_چوڑائی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_bottom_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_چوڑائی = lv_obj_get_style_border_چوڑائی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_چوڑائی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_x_محفوظ_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    أرجع scale > 0 ? scale : 1;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_y_محفوظ_اندرونی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    أرجع scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#جے_تعریف_شدہ __cplusplus
} /*extern "C"*/
#جے_انت

#جے_انت /*LV_OBJ_STYLE_INTERNAL_H*/
