/**
 * @file lv_group_private.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_گروهه_ذاتي_H
#وصف_ڪريو LV_گروهه_ذاتي_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

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
بنية lv_گروهه_t_2 {
#إذا LV_USE_EXT_معلومات
    lv_ext_معلومات_t ext_معلومات;
#جيڪڏهن_پورو
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_گروهه_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_گروهه_ڪنارو_cb_t  ڪنارو_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    فراغ * user_معلومات;

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
فراغ lv_گروهه_init(فراغ);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
فراغ lv_گروهه_deinit(فراغ);

/**********************
 *      MACROS
 **********************/

#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_GROUP_PRIVATE_H*/
