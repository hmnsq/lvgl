/**
 * @file lv_observer_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBSERVER_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBSERVER_ಖಾಸಗಿ_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/*********************
 *      INCLUDES
 *********************/

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

#ಆದರೆ LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
ರಚನೆ _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    ಶೂನ್ಯ * ಗುರಿ;                      /**< A target for the observer, e.g. a widget or any pointer */
    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;                   /**< Additional parameter, can be used freely by user */
    ಶೂನ್ಯ (*user_cb)(ಶೂನ್ಯ);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_ದತ್ತಾಂಶ : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
ರಚನೆ _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t ಹೆಜ್ಜೆ;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_ಮೌಲ್ಯ;      /**< Don't set a value smaller than this */
    int32_t max_ಮೌಲ್ಯ;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
ಶೂನ್ಯ lv_subject_global_init(ಶೂನ್ಯ);
ಶೂನ್ಯ lv_subject_global_deinit(ಶೂನ್ಯ);

/**********************
 *      MACROS
 **********************/

#ಆದರೆ_ಅಂತ್ಯ /* LV_USE_OBSERVER */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_OBSERVER_PRIVATE_H*/
