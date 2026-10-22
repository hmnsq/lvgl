/**
 * @file lv_obj_style_internal.h
 *
 */

#اگر_تعریف_نشده LV_OBJ_STYLE_داخلی_H
#تعریف_کن LV_OBJ_STYLE_داخلی_H

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

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

ساكن مضمن lv_متن_align_t lv_obj_calculate_style_متن_align_داخلی(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != عدم);
    LV_ASSERT(txt != عدم);

    lv_متن_align_t align = lv_obj_get_style_متن_align_داخلی(obj, part);
    lv_پایه_dir_t پایه_dir = lv_obj_get_style_پایه_dir_داخلی(obj, part);
    lv_bidi_calculate_align(&align, &پایه_dir, txt);
    أرجع align;
}

ساكن مضمن int32_t lv_obj_get_style_space_چپ_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_چپ(obj, part);
    int32_t border_عرض = lv_obj_get_style_border_عرض(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_چپ) ? padding + border_عرض : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_راست_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_راست(obj, part);
    int32_t border_عرض = lv_obj_get_style_border_عرض(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_راست) ? padding + border_عرض : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_بالاترین_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_بالاترین(obj, part);
    int32_t border_عرض = lv_obj_get_style_border_عرض(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_بالاترین) ? padding + border_عرض : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_bottom_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_عرض = lv_obj_get_style_border_عرض(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_عرض : padding;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_x_امن_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    أرجع scale > 0 ? scale : 1;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_y_امن_داخلی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    أرجع scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_OBJ_STYLE_INTERNAL_H*/
