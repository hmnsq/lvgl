/**
 * @file lv_obj_style_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBJ_STYLE_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_STYLE_ಖಾಸಗಿ_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/*********************
 *      INCLUDES
 *********************/

#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

ರಚನೆ _lv_obj_style_t {
    ಸ್ಥಿರಾಂಕ lv_style_t * style;
    uint32_t selector : 24;
    uint32_t is_local : 1;
    uint32_t is_trans : 1;
    uint32_t is_disabled : 1;
    uint32_t is_theme : 1;  /**< The style is added by a theme */
};

ರಚನೆ _lv_obj_style_transition_dsc_t {
    lv_anim_ಮಾರ್ಗ_cb_t ಮಾರ್ಗ_cb;
    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;
    lv_style_selector_t selector;
    uint16_t ಸಮಯ;
    uint16_t delay;
    lv_style_prop_t prop;
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the object related style manager module.
 * Called by LVGL in `lv_init()`
 */
ಶೂನ್ಯ lv_obj_style_init(ಶೂನ್ಯ);

/**
 * Deinitialize the object related style manager module.
 * Called by LVGL in `lv_deinit()`
 */
ಶೂನ್ಯ lv_obj_style_deinit(ಶೂನ್ಯ);

/**
 * Used internally to create a style transition
 * @param obj
 * @param part
 * @param prev_state
 * @param new_state
 * @param tr_dsc
 */
ಶೂನ್ಯ lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state,
                                    lv_state_t new_state, ಸ್ಥಿರಾಂಕ lv_obj_style_transition_dsc_t * tr_dsc);

/**
 * Used internally to compare the appearance of an object in 2 states
 * @param obj
 * @param state1
 * @param state2
 * @param changed_part  optional (may be NULL) out-param. Receives the part that
 *                      the differing styles belong to, or `LV_PART_ANY` if more
 *                      than one part differs (or none does). Lets the caller scope
 *                      the refresh to a single part when possible.
 * @return
 */
lv_style_state_cmp_t lv_obj_style_state_compare(lv_obj_t * obj, lv_state_t state1, lv_state_t state2,
                                                lv_part_t * ಬದಲಾಯಿತು_part);

/**
 * Update the layer type of a widget bayed on its current styles.
 * The result will be stored in `obj->spec_attr->layer_type`
 * @param obj       the object whose layer should be updated
 */
ಶೂನ್ಯ lv_obj_update_layer_type(lv_obj_t * obj);

/**
 * Internal implementation of @ref lv_obj_get_style_prop
 * Does not do any argument checking
 */
lv_style_ಮೌಲ್ಯ_t lv_obj_get_style_prop_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop);


/**
 * Internal implementation of @ref lv_obj_style_apply_color_filter
 * Does not do any argument checking
 */
lv_style_ಮೌಲ್ಯ_t lv_obj_style_apply_color_filter_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_part_t part, lv_style_ಮೌಲ್ಯ_t v);

/**
 * Update the cached blur status of a widget based on its current styles and
 * state, and maintain the global count of widgets with blur (blur_obj_cnt in
 * lv_global_t). The result will be stored in `obj->has_blur`.
 * @param obj       the object whose blur status should be updated
 */
ಶೂನ್ಯ lv_obj_update_blur_ಸ್ಥಿತಿ(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_OBJ_STYLE_PRIVATE_H*/
