/**
 * @file lv_obj_draw_private.h
 *
 */

#यदि_अपरिभाषितम् LV_OBJ_DRAW_वैयक्तिकम्_H
#परिभाषय LV_OBJ_DRAW_वैयक्तिकम्_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "../lvgl_public.h"

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
int32_t lv_obj_get_ext_draw_आकार(ध्रुव lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ध्रुव lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_OBJ_DRAW_PRIVATE_H*/
