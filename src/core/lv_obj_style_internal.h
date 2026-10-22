/**
 * @file lv_obj_style_internal.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_STYLE_అంతర్గత_H
#నిర్వచించు LV_OBJ_STYLE_అంతర్గత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_style_internal_gen.h"

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

స్థిర అంతఃవరుస lv_పాఠ్యం_align_t lv_obj_calculate_style_పాఠ్యం_align_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part,
                                                                         స్థిరరాశి అక్షరం * txt)
{
    LV_ASSERT(obj != ఖాళీ);
    LV_ASSERT(txt != ఖాళీ);

    lv_పాఠ్యం_align_t align = lv_obj_get_style_పాఠ్యం_align_అంతర్గత(obj, part);
    lv_పునాది_dir_t పునాది_dir = lv_obj_get_style_పునాది_dir_అంతర్గత(obj, part);
    lv_bidi_calculate_align(&align, &పునాది_dir, txt);
    తిరిగి align;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_space_ఎడమ_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t padding = lv_obj_get_style_pad_ఎడమ(obj, part);
    int32_t border_వెడల్పు = lv_obj_get_style_border_వెడల్పు(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    తిరిగి (border_side & LV_BORDER_SIDE_ఎడమ) ? padding + border_వెడల్పు : padding;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_space_కుడి_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t padding = lv_obj_get_style_pad_కుడి(obj, part);
    int32_t border_వెడల్పు = lv_obj_get_style_border_వెడల్పు(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    తిరిగి (border_side & LV_BORDER_SIDE_కుడి) ? padding + border_వెడల్పు : padding;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_space_శిఖరం_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t padding = lv_obj_get_style_pad_శిఖరం(obj, part);
    int32_t border_వెడల్పు = lv_obj_get_style_border_వెడల్పు(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    తిరిగి (border_side & LV_BORDER_SIDE_శిఖరం) ? padding + border_వెడల్పు : padding;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_space_bottom_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t padding = lv_obj_get_style_pad_bottom(obj, part);
    int32_t border_వెడల్పు = lv_obj_get_style_border_వెడల్పు(obj, part);
    lv_border_side_t border_side = lv_obj_get_style_border_side(obj, part);
    తిరిగి (border_side & LV_BORDER_SIDE_BOTTOM) ? padding + border_వెడల్పు : padding;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_transform_scale_x_సురక్షిత_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t scale = lv_obj_get_style_transform_scale_x(obj, part);
    తిరిగి scale > 0 ? scale : 1;
}

స్థిర అంతఃవరుస int32_t lv_obj_get_style_transform_scale_y_సురక్షిత_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part)
{
    LV_ASSERT(obj != ఖాళీ);
    int32_t scale = lv_obj_get_style_transform_scale_y(obj, part);
    తిరిగి scale > 0 ? scale : 1;
}

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_STYLE_INTERNAL_H*/
