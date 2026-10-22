/**
 * @file lv_group_private.h
 *
 */

#यदि_अपरिभाषितम् LV_गणः_वैयक्तिकम्_H
#परिभाषय LV_गणः_वैयक्तिकम्_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../lvgl_public.h"

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
संरचना lv_गणः_t_2 {
#यदि LV_USE_EXT_दत्त
    lv_ext_दत्त_t ext_दत्त;
#यदि_अन्तः
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_गणः_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_गणः_edge_cb_t  edge_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    शून्य * user_दत्त;

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
शून्य lv_गणः_init(शून्य);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
शून्य lv_गणः_deinit(शून्य);

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_GROUP_PRIVATE_H*/
