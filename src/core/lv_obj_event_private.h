/**
 * @file lv_obj_event_private.h
 *
 */

#यदि_अपरिभाषितम् LV_OBJ_EVENT_वैयक्तिकम्_H
#परिभाषय LV_OBJ_EVENT_वैयक्तिकम्_H

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

/**
 * Used as the event parameter of ::LV_EVENT_HIT_TEST to check if an `point` can click the object or not.
 * `res` should be set like this:
 *   - If already set to `false` another event wants that point non clickable. If you want to respect it leave it as `false` or set `true` to overwrite it.
 *   - If already set `true` and `point` shouldn't be clickable set to `false`
 *   - If already set to `true` you agree that `point` can click the object leave it as `true`
 */
संरचना lv_लब्धम्_test_info_t_2 {
    ध्रुव lv_point_t * point;   /**< A point relative to screen to check if it can click the object or not*/
    bool res;                   /**< true: `point` can click the object; false: it cannot*/
};

/**
 * Used as the event parameter of ::LV_EVENT_COVER_CHECK to check if an area is covered by the object or not.
 * In the event use `const lv_area_t * area = lv_event_get_cover_area(e)` to get the area to check
 * and `lv_event_set_cover_res(e, res)` to set the result.
 */
संरचना _lv_cover_check_info_t {
    lv_cover_res_t res;
    ध्रुव lv_area_t * area;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_OBJ_EVENT_PRIVATE_H*/
