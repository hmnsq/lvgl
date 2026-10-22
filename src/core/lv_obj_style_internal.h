/**
 * @file lv_obj_style_internal.h
 *
 */

#که_نه_تعریف_شوی LV_OBJ_STYLE_دننی_H
#تعریف_کړه LV_OBJ_STYLE_دننی_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

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

ساكن مضمن lv_متن_align_t lv_obj_calculate_style_متن_align_دننی(ثابت lv_obj_t * obj, lv_part_t part,
                                                                         ثابت حرف * txt)
{
    LV_ASSERT(obj != عدم);
    LV_ASSERT(txt != عدم);

    lv_متن_align_t align = lv_obj_get_style_متن_align_دننی(obj, part);
    lv_بنسټ_dir_t بنسټ_dir = lv_obj_get_style_بنسټ_dir_دننی(obj, part);
    lv_bidi_calculate_align(&align, &بنسټ_dir, txt);
    أرجع align;
}

ساكن مضمن int32_t lv_obj_get_style_space_کیڼ_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_کیڼ(obj, part);
    int32_t border_پلنوالی = lv_obj_get_style_border_پلنوالی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_کیڼ) ? padding + border_پلنوالی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_ښی_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_ښی(obj, part);
    int32_t border_پلنوالی = lv_obj_get_style_border_پلنوالی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_ښی) ? padding + border_پلنوالی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_سر_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_سر(obj, part);
    int32_t border_پلنوالی = lv_obj_get_style_border_پلنوالی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_سر) ? padding + border_پلنوالی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_space_bottom_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_پلنوالی = lv_obj_get_style_border_پلنوالی(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    أرجع (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_پلنوالی : padding;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_x_خوندي_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    أرجع scale > 0 ? scale : 1;
}

ساكن مضمن int32_t lv_obj_get_style_transform_scale_y_خوندي_دننی(ثابت lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != عدم);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    أرجع scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_OBJ_STYLE_INTERNAL_H*/
