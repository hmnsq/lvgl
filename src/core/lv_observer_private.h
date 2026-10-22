/**
 * @file lv_observer_private.h
 *
 */

#که_نه_تعریف_شوی LV_OBSERVER_شخصي_H
#تعریف_کړه LV_OBSERVER_شخصي_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

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
    فراغ * موخه;                      /**< A target for the observer, e.g. a widget or any pointer */
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
    int32_t ګام;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_ارزښت;      /**< Don't set a value smaller than this */
    int32_t max_ارزښت;      /**< Don't set a value larger than this */
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

#د_که_پای /* LV_USE_OBSERVER */

#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_OBSERVER_PRIVATE_H*/
