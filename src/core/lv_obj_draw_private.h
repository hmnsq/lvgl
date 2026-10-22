/**
 * @file lv_obj_draw_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBJ_DRAW_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_DRAW_ಖಾಸಗಿ_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/*********************
 *      INCLUDES
 *********************/

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

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
int32_t lv_obj_get_ext_draw_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj);

lv_layer_type_t lv_obj_get_layer_type(ಸ್ಥಿರಾಂಕ lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_OBJ_DRAW_PRIVATE_H*/
