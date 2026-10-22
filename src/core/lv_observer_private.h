/**
 * @file lv_observer_private.h
 *
 */

#ifndef LV_OBSERVER_PRIVATE_H
#define LV_OBSERVER_PRIVATE_H

#ifdef __cplusplus
வெளி "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#அந்தர்பாவ "../lvgl_public.h"

#எனில் LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
கட்டமைப்பு _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    வெறுமை * target;                      /**< A target for the observer, e.g. a widget or any pointer */
    வெறுமை * user_data;                   /**< Additional parameter, can be used freely by user */
    வெறுமை (*user_cb)(வெறுமை);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_data : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
கட்டமைப்பு _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t step;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_மதிப்பு;      /**< Don't set a value smaller than this */
    int32_t max_மதிப்பு;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
வெறுமை lv_subject_global_init(வெறுமை);
வெறுமை lv_subject_global_deinit(வெறுமை);

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_OBSERVER */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBSERVER_PRIVATE_H*/
