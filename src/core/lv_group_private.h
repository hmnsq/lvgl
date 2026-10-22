/**
 * @file lv_group_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_సమూహం_వ్యక్తిగత_H
#నిర్వచించు LV_సమూహం_వ్యక్తిగత_H

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

/**
 * Groups can be used to logically hold objects so that they can be individually focused.
 * They are NOT for laying out objects on a screen (try layouts for that).
 */
నిర్మాణం lv_సమూహం_t_2 {
#ఒకవేళ LV_USE_EXT_దత్తాంశం
    lv_ext_దత్తాంశం_t ext_దత్తాంశం;
#అయితే_ముగింపు
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_సమూహం_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_సమూహం_అంచు_cb_t  అంచు_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    శూన్య * user_దత్తాంశం;

    uint8_t frozen : 1;         /**< 1: can't focus to new object*/
    uint8_t editing : 1;        /**< 1: Edit mode, 0: Navigate mode*/
    uint8_t refocus_policy : 1; /**< 1: Focus prev if focused on deletion. 0: Focus next if focused on
                                   deletion.*/
    uint8_t wrap : 1;           /**< 1: Focus next/prev can wrap at end of list. 0: Focus next/prev stops at end
                                   of list.*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Init the group module
 * @remarks Internal function, do not call directly.
 */
శూన్య lv_సమూహం_init(శూన్య);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
శూన్య lv_సమూహం_deinit(శూన్య);

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_GROUP_PRIVATE_H*/
