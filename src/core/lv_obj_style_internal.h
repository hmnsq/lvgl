/**
 * @file lv_obj_style_internal.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_OBJ_STYLE_اندروني_H
#وصف_ڪريو LV_OBJ_STYLE_اندروني_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

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

ساكن مضمن lv_متن_align_t lv_obj_calculate_style_متن_align_اندروني(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != عدم);
    LV_ASSERT(txt != عدم);

    lv_متن_align_t align = lv_obj_get_style_متن_align_اندروني(obj, part);
    lv_بنياد_dir_t بنياد_dir = lv_obj_get_style_بنياد_dir_اندروني(obj, part);
    lv_bidi_calculate_align(&align, &بنياد_dir, txt);
    أرجع align;
}

ساكن مضمن int32_t lv_obj_get_style_space_کاٻو_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_کاٻو(obj, part);
    int32_t border_ويڪر = lv_obj_get_style_border_ويڪر(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_کاٻو) ? padding + border_ويڪر : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_ساڄو_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_ساڄو(obj, part);
    int32_t border_ويڪر = lv_obj_get_style_border_ويڪر(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_ساڄو) ? padding + border_ويڪر : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_چوٽي_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_چوٽي(obj, part);
    int32_t border_ويڪر = lv_obj_get_style_border_ويڪر(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_چوٽي) ? padding + border_ويڪر : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_bottom_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_ويڪر = lv_obj_get_style_border_ويڪر(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_ويڪر : padding;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_x_محفوظ_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    أرجع scale > 0 ? scale : 1;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_y_محفوظ_اندروني(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    أرجع scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_OBJ_STYLE_INTERNAL_H*/
