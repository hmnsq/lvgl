/**
 * @file lv_observer_private.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_OBSERVER_ذاتي_H
#وصف_ڪريو LV_OBSERVER_ذاتي_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/*********************
 *      INCLUDES
 *********************/

#ضمن "../lvgl_public.h"

#إذا LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
بنية _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    فراغ * هدف;                      /**< A target for the observer, e.g. a widget or any pointer */
    فراغ * user_معلومات;                   /**< Additional parameter, can be used freely by user */
    فراغ (*user_cb)(فراغ);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_معلومات : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
بنية _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t قدم;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_قدر;      /**< Don't set a value smaller than this */
    int32_t max_قدر;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
فراغ lv_subject_global_init(فراغ);
فراغ lv_subject_global_deinit(فراغ);

/**********************
 *      MACROS
 **********************/

#جيڪڏهن_پورو /* LV_USE_OBSERVER */

#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_OBSERVER_PRIVATE_H*/
