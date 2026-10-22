/**
 * @file lv_obj_draw_private.h
 *
 */

#جے_بنا_تعریف LV_OBJ_DRAW_نجی_H
#تعریف_کرو LV_OBJ_DRAW_نجی_H

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
 * Get the extended draw area of an object.
 * @param obj       pointer to an object
 * @return          the size extended draw area around the real coordinates
 */
int32_t lv_obj_get_ext_draw_ناپ(ثابت lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#جے_تعریف_شدہ __cplusplus
} /*extern "C"*/
#جے_انت

#جے_انت /*LV_OBJ_DRAW_PRIVATE_H*/
