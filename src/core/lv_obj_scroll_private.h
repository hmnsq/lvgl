/**
 * @file lv_obj_scroll_private.h
 *
 */

#جے_بنا_تعریف LV_OBJ_SCROLL_نجی_H
#تعریف_کرو LV_OBJ_SCROLL_نجی_H

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

/*********************
 *      INCLUDES
 *********************/

#ضمن "../lvgl_public.h"

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
lv_نتیجہ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);

/**********************
 *      MACROS
 **********************/

#جے_تعریف_شدہ __cplusplus
} /*extern "C"*/
#جے_انت

#جے_انت /*LV_OBJ_SCROLL_PRIVATE_H*/
