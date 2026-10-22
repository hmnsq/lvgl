/**
 * @file lv_observer_private.h
 *
 */

#यदि_अपरिभाषितम् LV_OBSERVER_वैयक्तिकम्_H
#परिभाषय LV_OBSERVER_वैयक्तिकम्_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*********************
 *      INCLUDES
 *********************/

#अन्तर्भाव "../lvgl_public.h"

#यदि LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
संरचना _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    शून्य * target;                      /**< A target for the observer, e.g. a widget or any pointer */
    शून्य * user_दत्त;                   /**< Additional parameter, can be used freely by user */
    शून्य (*user_cb)(शून्य);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_दत्त : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
संरचना _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t चरणः;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_मानम्;      /**< Don't set a value smaller than this */
    int32_t max_मानम्;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
शून्य lv_subject_global_init(शून्य);
शून्य lv_subject_global_deinit(शून्य);

/**********************
 *      MACROS
 **********************/

#यदि_अन्तः /* LV_USE_OBSERVER */

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_OBSERVER_PRIVATE_H*/
