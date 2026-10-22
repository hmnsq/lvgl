/**
 * @file lv_obj_draw_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_DRAW_వ్యక్తిగత_H
#నిర్వచించు LV_OBJ_DRAW_వ్యక్తిగత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Get the extended draw area of an object.
 * @param obj       pointer to an object
 * @return          the size extended draw area around the real coordinates
 */
int32_t lv_obj_get_ext_draw_కొలత(స్థిరరాశి lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(స్థిరరాశి lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_DRAW_PRIVATE_H*/
