/**
 * @file lv_refr_private.h
 *
 */

#اگر_تعریف_نشده LV_REFR_خصوصی_H
#تعریف_کن LV_REFR_خصوصی_H

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

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
 * Initialize the screen refresh subsystem
 */
فراغ lv_refr_init(فراغ);

/**
 * Deinitialize the screen refresh subsystem
 */
فراغ lv_refr_deinit(فراغ);

/**
 * Invalidate an area on display to redraw it
 * @param area_p pointer to area which should be invalidated (NULL: delete the invalidated areas)
 * @param disp pointer to display where the area should be invalidated (NULL can be used if there is
 * only one display)
 * @return LV_RESULT_OK: the area is invalidated; LV_RESULT_INVALID: the area wasn't invalidated.
 */
lv_نتیجه_t lv_inv_area(lv_display_t * disp, ثابت lv_area_t * area_p);

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(فراغ);

/**
 * Set the display which is being refreshed
 * @param disp the display being refreshed
 */
فراغ lv_refr_مجموعه_disp_refreshing(lv_display_t * disp);

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_بالاترین_obj(ثابت lv_area_t * area_p, lv_obj_t * obj);

/**
 * Render an object to a layer
 * @param layer target drawing layer
 * @param obj   object to render
 */
فراغ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_REFR_PRIVATE_H*/
