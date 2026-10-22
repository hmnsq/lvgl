/**
 * @file lv_refr_private.h
 *
 */

#यदि_अपरिभाषितम् LV_REFR_वैयक्तिकम्_H
#परिभाषय LV_REFR_वैयक्तिकम्_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "../lvgl_public.h"

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
शून्य lv_refr_init(शून्य);

/**
 * Deinitialize the screen refresh subsystem
 */
शून्य lv_refr_deinit(शून्य);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_परिणाम_t lv_inv_area(lv_display_t * disp, ध्रुव lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(शून्य);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
शून्य lv_refr_समुच्चयः_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_शिखरम्_obj(ध्रुव lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
शून्य lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_REFR_PRIVATE_H*/
