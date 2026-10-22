/**
 * @file lv_obj_draw_private.h
 *
 */

#ifndef LV_OBJ_DRAW_PRIVATE_H
#define LV_OBJ_DRAW_PRIVATE_H

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#അന്തര്ഭാവ "../lvgl_public.h"

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
int32_t lv_obj_get_ext_draw_size(സ്ഥിരം lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(സ്ഥിരം lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_DRAW_PRIVATE_H*/
