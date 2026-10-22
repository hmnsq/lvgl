/**
 * @file lv_obj_event_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_EVENT_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_EVENT_ਨਿੱਜੀ_H

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

/**
 * Used as the event parameter of ::LV_EVENT_HIT_TEST to check if an `point` can click the object or not.
 * `res` should be set like this:
 *   - If already set to `false` another event wants that point non clickable. If you want to respect it leave it as `false` or set `true` to overwrite it.
 *   - If already set `true` and `point` shouldn't be clickable set to `false`
 *   - If already set to `true` you agree that `point` can click the object leave it as `true`
 */
ਬਣਤਰ lv_ਪ੍ਰਾਪਤੀ_test_info_t_2 {
    ਅਚਲ lv_ਬਿੰਦੂ_t * ਬਿੰਦੂ;   /**< A point relative to screen to check if it can click the object or not*/
    bool res;                   /**< true: `point` can click the object; false: it cannot*/
};

/**
 * Used as the event parameter of ::LV_EVENT_COVER_CHECK to check if an area is covered by the object or not.
 * In the event use `const lv_area_t * area = lv_event_get_cover_area(e)` to get the area to check
 * and `lv_event_set_cover_res(e, res)` to set the result.
 */
ਬਣਤਰ _lv_cover_check_info_t {
    lv_cover_res_t res;
    ਅਚਲ lv_area_t * area;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBJ_EVENT_PRIVATE_H*/
