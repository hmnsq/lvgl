/**
 * @file lv_obj_style_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_STYLE_వ్యక్తిగత_H
#నిర్వచించు LV_OBJ_STYLE_వ్యక్తిగత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

నిర్మాణం _lv_obj_style_t {
    స్థిరరాశి lv_style_t * style;
    uint32_t selector : 24;
    uint32_t is_local : 1;
    uint32_t is_trans : 1;
    uint32_t is_disabled : 1;
    uint32_t is_theme : 1;  /**< The style is added by a theme */
};

నిర్మాణం _lv_obj_style_transition_dsc_t {
    lv_anim_మార్గం_cb_t మార్గం_cb;
    శూన్య * user_దత్తాంశం;
    lv_style_selector_t selector;
    uint16_t సమయం;
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
శూన్య lv_obj_style_init(శూన్య);

/**
 * Deinitialize the object related style manager module.
 * Called by LVGL in `lv_deinit()`
 */
శూన్య lv_obj_style_deinit(శూన్య);

/**
 * Used internally to create a style transition
 * @param obj
 * @param part
 * @param prev_state
 * @param new_state
 * @param tr_dsc
 */
శూన్య lv_obj_style_create_transition(lv_obj_t * obj, lv_part_t part, lv_state_t prev_state,
                                    lv_state_t new_state, స్థిరరాశి lv_obj_style_transition_dsc_t * tr_dsc);

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
                                                lv_part_t * మారింది_part);

/**
 * Update the layer type of a widget bayed on its current styles.
 * The result will be stored in `obj->spec_attr->layer_type`
 * @param obj       the object whose layer should be updated
 */
శూన్య lv_obj_update_layer_type(lv_obj_t * obj);

/**
 * Internal implementation of @ref lv_obj_get_style_prop
 * Does not do any argument checking
 */
lv_style_విలువ_t lv_obj_get_style_prop_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop);


/**
 * Internal implementation of @ref lv_obj_style_apply_color_filter
 * Does not do any argument checking
 */
lv_style_విలువ_t lv_obj_style_apply_color_filter_అంతర్గత(స్థిరరాశి lv_obj_t * obj, lv_part_t part, lv_style_విలువ_t v);

/**
 * Update the cached blur status of a widget based on its current styles and
 * state, and maintain the global count of widgets with blur (blur_obj_cnt in
 * lv_global_t). The result will be stored in `obj->has_blur`.
 * @param obj       the object whose blur status should be updated
 */
శూన్య lv_obj_update_blur_స్థితి(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_STYLE_PRIVATE_H*/
