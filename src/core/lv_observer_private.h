/**
 * @file lv_observer_private.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_OBSERVER_نجی_H
#تعریف_کرو LV_OBSERVER_نجی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*********************
 *      INCLUDES
 *********************/

#شامل "../lvgl_public.h"

#اگر LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
ساخت _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    خالی * ہدف;                      /**< A target for the observer, e.g. a widget or any pointer */
    خالی * user_ڈیٹا;                   /**< Additional parameter, can be used freely by user */
    خالی (*user_cb)(خالی);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_ڈیٹا : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
ساخت _lv_subject_increment_dsc_t {
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
خالی lv_subject_global_init(خالی);
خالی lv_subject_global_deinit(خالی);

/**********************
 *      MACROS
 **********************/

#اگر_ختم /* LV_USE_OBSERVER */

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_OBSERVER_PRIVATE_H*/
