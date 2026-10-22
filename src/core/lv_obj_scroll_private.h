/**
 * @file lv_obj_scroll_private.h
 *
 */

#अगर_अघोषित LV_OBJ_SCROLL_PRIVATE_H
#घोषणा LV_OBJ_SCROLL_PRIVATE_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*********************
 *      INCLUDES
 *********************/

#समावेश "../lvgl_public.h"

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
lv_परिणाम_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y);

/**********************
 *      MACROS
 **********************/

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_OBJ_SCROLL_PRIVATE_H*/
