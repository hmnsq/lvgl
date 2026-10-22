/**
 * @file lv_obj_scroll_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_SCROLL_వ్యక్తిగత_H
#నిర్వచించు LV_OBJ_SCROLL_వ్యక్తిగత_H

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
 * Low level function to scroll by given x and y coordinates.
 * `LV_EVENT_SCROLL` is sent.
 * @param obj       pointer to an object to scroll
 * @param x         pixels to scroll horizontally
 * @param y         pixels to scroll vertically
 * @return          `LV_RESULT_INVALID`: to object was deleted in `LV_EVENT_SCROLL`;
 *                  `LV_RESULT_OK`: if the object is still valid
 */
lv_ఫలితం_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_SCROLL_PRIVATE_H*/
