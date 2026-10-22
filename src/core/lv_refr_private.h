/**
 * @file lv_refr_private.h
 *
 */

#ifndef LV_REFR_PRIVATE_H
#define LV_REFR_PRIVATE_H

#ifdef __cplusplus
બાહ્ય "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#અન્તર્ભાવ "../lvgl_public.h"

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
શૂન્ય lv_refr_init(શૂન્ય);

/**
 * Deinitialize the screen refresh subsystem
 */
શૂન્ય lv_refr_deinit(શૂન્ય);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_result_t lv_inv_area(lv_display_t * disp, અચલ lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(શૂન્ય);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
શૂન્ય lv_refr_set_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_ટોચ_obj(અચલ lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
શૂન્ય lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_REFR_PRIVATE_H*/
