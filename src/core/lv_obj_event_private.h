/**
 * @file lv_obj_event_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_EVENT_వ్యక్తిగత_H
#నిర్వచించు LV_OBJ_EVENT_వ్యక్తిగత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * Used as the event parameter of ::LV_EVENT_HIT_TEST to check if an `point` can click the object or not.
 * `res` should be set like this:
 *   - If already set to `false` another event wants that point non clickable. If you want to respect it leave it as `false` or set `true` to overwrite it.
 *   - If already set `true` and `point` shouldn't be clickable set to `false`
 *   - If already set to `true` you agree that `point` can click the object leave it as `true`
 */
నిర్మాణం lv_తగిలింది_test_info_t_2 {
    స్థిరరాశి lv_బిందువు_t * బిందువు;   /**< A point relative to screen to check if it can click the object or not*/
    bool res;                   /**< true: `point` can click the object; false: it cannot*/
};

/**
 * Used as the event parameter of ::LV_EVENT_COVER_CHECK to check if an area is covered by the object or not.
 * In the event use `const lv_area_t * area = lv_event_get_cover_area(e)` to get the area to check
 * and `lv_event_set_cover_res(e, res)` to set the result.
 */
నిర్మాణం _lv_cover_check_info_t {
    lv_cover_res_t res;
    స్థిరరాశి lv_area_t * area;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_EVENT_PRIVATE_H*/
