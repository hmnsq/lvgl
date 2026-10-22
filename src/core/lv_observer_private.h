/**
 * @file lv_observer_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBSERVER_వ్యక్తిగత_H
#నిర్వచించు LV_OBSERVER_వ్యక్తిగత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "../lvgl_public.h"

#ఒకవేళ LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
నిర్మాణం _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    శూన్య * లక్ష్యం;                      /**< A target for the observer, e.g. a widget or any pointer */
    శూన్య * user_దత్తాంశం;                   /**< Additional parameter, can be used freely by user */
    శూన్య (*user_cb)(శూన్య);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_దత్తాంశం : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
నిర్మాణం _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t అడుగు;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_విలువ;      /**< Don't set a value smaller than this */
    int32_t max_విలువ;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
శూన్య lv_subject_global_init(శూన్య);
శూన్య lv_subject_global_deinit(శూన్య);

/**********************
 *      MACROS
 **********************/

#అయితే_ముగింపు /* LV_USE_OBSERVER */

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBSERVER_PRIVATE_H*/
