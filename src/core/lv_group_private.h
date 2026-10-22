/**
 * @file lv_group_private.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_گروہ_نجی_H
#تعریف_کرو LV_گروہ_نجی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*********************
 *      INCLUDES
 *********************/
#شامل "../lvgl_public.h"

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
ساخت lv_گروہ_t_2 {
#اگر LV_USE_EXT_ڈیٹا
    lv_ext_ڈیٹا_t ext_ڈیٹا;
#اگر_ختم
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_گروہ_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_گروہ_کنارہ_cb_t  کنارہ_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    خالی * user_ڈیٹا;

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
خالی lv_گروہ_init(خالی);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
خالی lv_گروہ_deinit(خالی);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_GROUP_PRIVATE_H*/
