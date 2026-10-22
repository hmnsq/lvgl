/**
 * @file lv_obj_draw_private.h
 *
 */

#که_نه_تعریف_شوی LV_OBJ_DRAW_شخصي_H
#تعریف_کړه LV_OBJ_DRAW_شخصي_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

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

#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_OBJ_DRAW_PRIVATE_H*/
