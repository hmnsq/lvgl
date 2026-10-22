/**
 * @file lv_observer_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBSERVER_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBSERVER_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

#ਜੇ LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
ਬਣਤਰ _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    ਖਾਲੀ * ਨਿਸ਼ਾਨਾ;                      /**< A target for the observer, e.g. a widget or any pointer */
    ਖਾਲੀ * user_ਅੰਕੜੇ;                   /**< Additional parameter, can be used freely by user */
    ਖਾਲੀ (*user_cb)(ਖਾਲੀ);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_ਅੰਕੜੇ : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
ਬਣਤਰ _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t ਕਦਮ;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_ਮੁੱਲ;      /**< Don't set a value smaller than this */
    int32_t max_ਮੁੱਲ;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
ਖਾਲੀ lv_subject_global_init(ਖਾਲੀ);
ਖਾਲੀ lv_subject_global_deinit(ਖਾਲੀ);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਅੰਤ /* LV_USE_OBSERVER */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBSERVER_PRIVATE_H*/
