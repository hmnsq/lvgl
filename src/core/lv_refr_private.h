/**
 * @file lv_refr_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_REFR_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_REFR_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

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
ਖਾਲੀ lv_refr_init(ਖਾਲੀ);

/**
 * Deinitialize the screen refresh subsystem
 */
ਖਾਲੀ lv_refr_deinit(ਖਾਲੀ);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_ਨਤੀਜਾ_t lv_inv_area(lv_display_t * disp, ਅਚਲ lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(ਖਾਲੀ);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
ਖਾਲੀ lv_refr_ਸੈੱਟ_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_ਸਿਖਰ_obj(ਅਚਲ lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
ਖਾਲੀ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_REFR_PRIVATE_H*/
