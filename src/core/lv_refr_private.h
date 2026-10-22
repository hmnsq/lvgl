/**
 * @file lv_refr_private.h
 *
 */

#अगर_अघोषित LV_REFR_PRIVATE_H
#घोषणा LV_REFR_PRIVATE_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*********************
 *      INCLUDES
 *********************/

#समावेश "../lvgl_public.h"

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
व्योम lv_refr_init(व्योम);

/**
 * Deinitialize the screen refresh subsystem
 */
व्योम lv_refr_deinit(व्योम);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_परिणाम_t lv_inv_area(lv_display_t * disp, स्थिर lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(व्योम);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
व्योम lv_refr_set_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_शीर्ष_obj(स्थिर lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
व्योम lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_REFR_PRIVATE_H*/
