/**
 * @file lv_obj_scroll_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_SCROLL_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_SCROLL_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

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
lv_ਨਤੀਜਾ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBJ_SCROLL_PRIVATE_H*/
