/**
 * @file lv_group_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_ಗುಂಪು_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_ಗುಂಪು_ಖಾಸಗಿ_H

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

/**
 * Groups can be used to logically hold objects so that they can be individually focused.
 * They are NOT for laying out objects on a screen (try layouts for that).
 */
ರಚನೆ lv_ಗುಂಪು_t_2 {
#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    lv_ext_ದತ್ತಾಂಶ_t ext_ದತ್ತಾಂಶ;
#ಆದರೆ_ಅಂತ್ಯ
    lv_ll_t obj_ll;        /**< Linked list to store the objects in the group*/
    lv_obj_t ** obj_focus; /**< The object in focus*/

    lv_ಗುಂಪು_focus_cb_t focus_cb;              /**< A function to call when a new object is focused (optional)*/
    lv_ಗುಂಪು_ಅಂಚು_cb_t  ಅಂಚು_cb;               /**< A function to call when an edge is reached, no more focus
                                                    targets are available in this direction (to allow edge feedback
                                                    like a sound or a scroll bounce) */

    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;

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
ಶೂನ್ಯ lv_ಗುಂಪು_init(ಶೂನ್ಯ);

/**
 * Deinit the group module
 * @remarks Internal function, do not call directly.
 */
ಶೂನ್ಯ lv_ಗುಂಪು_deinit(ಶೂನ್ಯ);

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_GROUP_PRIVATE_H*/
