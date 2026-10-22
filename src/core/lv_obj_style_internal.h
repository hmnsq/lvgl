/**
 * @file lv_obj_style_internal.h
 *
 */

#ifndef LV_OBJ_STYLE_INTERNAL_H
#define LV_OBJ_STYLE_INTERNAL_H

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ lv_text_align_t lv_obj_calculate_style_text_align_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part,
                                                                         ଅଚଳ ଅକ୍ଷର * txt)
{
    LV_ASSERT(obj != ଖାଲି);
    LV_ASSERT(txt != ଖାଲି);

    lv_text_align_t align = lv_obj_get_style_text_align_internal(obj, part);
    lv_base_dir_t base_dir = lv_obj_get_style_base_dir_internal(obj, part);
    lv_bidi_calculate_align(&align, &base_dir, txt);
    ଫେରନ୍ତୁ align;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_space_ବାମ_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t padding = lv_obj_get_style_pad_ବାମ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ଫେରନ୍ତୁ (border_side & LV_BORDER_SIDE_ବାମ) ? padding + border_width : padding;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_space_ଡାହାଣ_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t padding = lv_obj_get_style_pad_ଡାହାଣ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ଫେରନ୍ତୁ (border_side & LV_BORDER_SIDE_ଡାହାଣ) ? padding + border_width : padding;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_space_ଶୀର୍ଷ_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t padding = lv_obj_get_style_pad_ଶୀର୍ଷ(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ଫେରନ୍ତୁ (border_side & LV_BORDER_SIDE_ଶୀର୍ଷ) ? padding + border_width : padding;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_space_bottom_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_width = lv_obj_get_style_border_width(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    ଫେରନ୍ତୁ (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_width : padding;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_transform_scale_x_safe_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    ଫେରନ୍ତୁ scale > 0 ? scale : 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int32_t lv_obj_get_style_transform_scale_y_safe_internal(ଅଚଳ lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ଖାଲି);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    ଫେରନ୍ତୁ scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_STYLE_INTERNAL_H*/
