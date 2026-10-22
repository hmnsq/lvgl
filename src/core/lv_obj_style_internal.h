/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
બાહ્ય "C" {
#endif

#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

સ્થિર અંતઃરેખ lv_text_align_t lv_obj_calculate_style_text_align_internal(અચલ lv_obj_t * obj, lv_part_t part,
                                                                         અચલ અક્ષર * txt)
{
    LV_ASSERT(obj != ખાલી);
    LV_ASSERT(txt != ખાલી);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    પાછા align;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_space_ડાબું_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t padding = lv_obj_get_style_pad_ડાબું(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    પાછા (border_side & LV_BORDER_SIDE_ડાબું) ? padding + border_width : padding;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_space_જમણું_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t padding = lv_obj_get_style_pad_જમણું(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    પાછા (border_side & LV_BORDER_SIDE_જમણું) ? padding + border_width : padding;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_space_ટોચ_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t padding = lv_obj_get_style_pad_ટોચ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    પાછા (border_side & LV_BORDER_SIDE_ટોચ) ? padding + border_width : padding;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_space_bottom_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    પાછા (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_transform_scale_x_safe_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    પાછા scale > 0 ? scale : 1;
}

સ્થિર અંતઃરેખ int32_t lv_obj_get_style_transform_scale_y_safe_internal(અચલ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ખાલી);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    પાછા scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
