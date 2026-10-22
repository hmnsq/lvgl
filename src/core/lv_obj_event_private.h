/**
 * @file lv_obj_event_private.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_OBJ_EVENT_نجی_H
#تعریف_کرو LV_OBJ_EVENT_نجی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*********************
 *      INCLUDES
 *********************/

#شامل "../lvgl_public.h"

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
ساخت lv_کامیابی_test_info_t_2 {
    ثابت lv_نقطہ_t * نقطہ;   /**< A point relative to screen to check if it can click the object or not*/
    bool res;                   /**< true: `point` can click the object; false: it cannot*/
};

/**
 * Used as the event parameter of ::LV_EVENT_COVER_CHECK to check if an area is covered by the object or not.
 * In the event use `const lv_area_t * area = lv_event_get_cover_area(e)` to get the area to check
 * and `lv_event_set_cover_res(e, res)` to set the result.
 */
ساخت _lv_cover_check_info_t {
    lv_cover_res_t res;
    ثابت lv_area_t * area;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_OBJ_EVENT_PRIVATE_H*/
