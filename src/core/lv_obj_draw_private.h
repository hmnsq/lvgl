/**
 * @file lv_obj_draw_private.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_OBJ_DRAW_ذاتي_H
#وصف_ڪريو LV_OBJ_DRAW_ذاتي_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

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
int32_t lv_obj_get_ext_draw_سائيز(ثابت lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ثابت lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_OBJ_DRAW_PRIVATE_H*/
