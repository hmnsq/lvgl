/**
 * @file lv_group_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_ਸਮੂਹ_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_ਸਮੂਹ_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

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
ਬਣਤਰ lv_ਸਮੂਹ_t_2 {
#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    lv_ext_ਅੰਕੜੇ_t ext_ਅੰਕੜੇ;
#ਜੇ_ਅੰਤ
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_ਸਮੂਹ_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_ਸਮੂਹ_ਕਿਨਾਰਾ_cb_t  ਕਿਨਾਰਾ_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    ਖਾਲੀ * user_ਅੰਕੜੇ;

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
ਖਾਲੀ lv_ਸਮੂਹ_init(ਖਾਲੀ);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
ਖਾਲੀ lv_ਸਮੂਹ_deinit(ਖਾਲੀ);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_GROUP_PRIVATE_H*/
