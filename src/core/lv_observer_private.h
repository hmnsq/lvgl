/**
 * @file lv_observer_private.h
 *
 */

#ifndef LV_OBSERVER_PRIVATE_H
#define LV_OBSERVER_PRIVATE_H

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"

#ଯଦି LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
ସଂରଚନା _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    ଶୂନ୍ୟ * target;                      /**< A target for the observer, e.g. a widget or any pointer */
    ଶୂନ୍ୟ * user_data;                   /**< Additional parameter, can be used freely by user */
    ଶୂନ୍ୟ (*user_cb)(ଶୂନ୍ୟ);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_data : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
ସଂରଚନା _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t step;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_ମୂଲ୍ୟ;      /**< Don't set a value smaller than this */
    int32_t max_ମୂଲ୍ୟ;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
ଶୂନ୍ୟ lv_subject_global_init(ଶୂନ୍ୟ);
ଶୂନ୍ୟ lv_subject_global_deinit(ଶୂନ୍ୟ);

/**********************
 *      MACROS
 **********************/

#endif /* LV_USE_OBSERVER */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBSERVER_PRIVATE_H*/
