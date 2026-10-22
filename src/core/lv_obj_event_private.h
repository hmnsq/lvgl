/**
 * @file lv_obj_event_private.h
 *
 */

#که_نه_تعریف_شوی LV_OBJ_EVENT_شخصي_H
#تعریف_کړه LV_OBJ_EVENT_شخصي_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

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

/**
 * Used as the event parameter of ::LV_EVENT_HIT_TEST to check if an `point` can click the object or not.
 * `res` should be set like this:
 *   - If already set to `false` another event wants that point non clickable. If you want to respect it leave it as `false` or set `true` to overwrite it.
 *   - If already set `true` and `point` shouldn't be clickable set to `false`
 *   - If already set to `true` you agree that `point` can click the object leave it as `true`
 */
بنية lv_موندنه_test_info_t_2 {
    ثابت lv_ټکی_t * ټکی;   /**< A point relative to screen to check if it can click the object or not*/
    bool res;                   /**< true: `point` can click the object; false: it cannot*/
};

/**
 * Used as the event parameter of ::LV_EVENT_COVER_CHECK to check if an area is covered by the object or not.
 * In the event use `const lv_area_t * area = lv_event_get_cover_area(e)` to get the area to check
 * and `lv_event_set_cover_res(e, res)` to set the result.
 */
بنية _lv_cover_check_info_t {
    lv_cover_res_t res;
    ثابت lv_area_t * area;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_OBJ_EVENT_PRIVATE_H*/
