/**
 * @file lv_refr_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_REFR_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_REFR_ಖಾಸಗಿ_H

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
 * Initialize the screen refresh subsystem
 */
ಶೂನ್ಯ lv_refr_init(ಶೂನ್ಯ);

/**
 * Deinitialize the screen refresh subsystem
 */
ಶೂನ್ಯ lv_refr_deinit(ಶೂನ್ಯ);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_ಫಲಿತಾಂಶ_t lv_inv_area(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(ಶೂನ್ಯ);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
ಶೂನ್ಯ lv_refr_ಗಣ_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_ತುದಿ_obj(ಸ್ಥಿರಾಂಕ lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
ಶೂನ್ಯ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_REFR_PRIVATE_H*/
