/**
 * @file lv_obj_draw_private.h
 *
 */

#اگر_تعریف_نشده LV_OBJ_DRAW_خصوصی_H
#تعریف_کن LV_OBJ_DRAW_خصوصی_H

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
 * Get the extended draw area of an object.
 * @param obj       pointer to an object
 * @return          the size extended draw area around the real coordinates
 */
int32_t lv_obj_get_ext_draw_اندازه(ثابت lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_OBJ_DRAW_PRIVATE_H*/
