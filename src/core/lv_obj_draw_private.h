/**
 * @file lv_obj_draw_private.h
 *
 */

#अगर_अघोषित LV_OBJ_DRAW_PRIVATE_H
#घोषणा LV_OBJ_DRAW_PRIVATE_H

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
 * Get the extended draw area of an object.
 * @param obj       pointer to an object
 * @return          the size extended draw area around the real coordinates
 */
int32_t lv_obj_get_ext_draw_आकार(स्थिर lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(स्थिर lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_OBJ_DRAW_PRIVATE_H*/
