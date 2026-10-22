/**
 * @file lv_obj_draw_private.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_OBJ_DRAW_نجی_H
#تعریف_کرو LV_OBJ_DRAW_نجی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*********************
 *      INCLUDES
 *********************/

#شامل "../lvgl_public.h"

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
int32_t lv_obj_get_ext_draw_سائز(ثابت lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_OBJ_DRAW_PRIVATE_H*/
