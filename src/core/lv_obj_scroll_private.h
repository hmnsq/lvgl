/**
 * @file lv_obj_scroll_private.h
 *
 */

#اگر_تعریف_نشده LV_OBJ_SCROLL_خصوصی_H
#تعریف_کن LV_OBJ_SCROLL_خصوصی_H

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

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
lv_نتیجه_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_OBJ_SCROLL_PRIVATE_H*/
