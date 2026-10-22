/**
 * @file lv_observer_private.h
 *
 */

#جے_بنا_تعریف LV_OBSERVER_نجی_H
#تعریف_کرو LV_OBSERVER_نجی_H

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

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
    فراغ * نشانہ;                      /**< A target for the observer, e.g. a widget or any pointer */
    فراغ * user_انکڑے;                   /**< Additional parameter, can be used freely by user */
    فراغ (*user_cb)(فراغ);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_انکڑے : 1;   /**< Automatically free user data when observer is removed */
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

#جے_انت /* LV_USE_OBSERVER */

#جے_تعریف_شدہ __cplusplus
} /*extern "C"*/
#جے_انت

#جے_انت /*LV_OBSERVER_PRIVATE_H*/
