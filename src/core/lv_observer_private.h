/**
 * @file lv_observer_private.h
 *
 */

#अगर_अघोषित LV_OBSERVER_PRIVATE_H
#घोषणा LV_OBSERVER_PRIVATE_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*********************
 *      INCLUDES
 *********************/

#समावेश "../lvgl_public.h"

#अगर LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
काष्ठा _lv_observer_t {
    lv_subject_t * subject;             /**< Observed subject */
    lv_observer_cb_t cb;                /**< Callback that notifies when value changes */
    व्योम * target;                      /**< A target for the observer, e.g. a widget or any pointer */
    व्योम * user_तथ्य;                   /**< Additional parameter, can be used freely by user */
    व्योम (*user_cb)(व्योम);              /**< Additional function pointer, can be used freely by user */
    uint32_t auto_free_user_तथ्य : 1;   /**< Automatically free user data when observer is removed */
    uint32_t notified : 1;              /**< Was observer already notified? */
    uint32_t for_obj : 1;               /**< Is `target` a pointer to a Widget (`lv_obj_t *`)? */
};

/**
 * Descriptor created by `lv_obj_add_subject_increment_event()`
 */
काष्ठा _lv_subject_increment_dsc_t {
    lv_subject_t * subject; /**< The subject to adjust*/
    int32_t step;           /**< The step add to the subject */
    bool rollover;          /**< Where to start over from the other end when one end is exceeded*/
    int32_t min_मूल्य;      /**< Don't set a value smaller than this */
    int32_t max_मूल्य;      /**< Don't set a value larger than this */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*TODO: v10 rename to plain init/deinit after removing old init/deinit functions*/
व्योम lv_subject_global_init(व्योम);
व्योम lv_subject_global_deinit(व्योम);

/**********************
 *      MACROS
 **********************/

#पूर्ण_अगर /* LV_USE_OBSERVER */

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_OBSERVER_PRIVATE_H*/
