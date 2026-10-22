/**
 * @file lv_obj_class_private.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBJ_CLASS_ಖಾಸಗಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_CLASS_ಖಾಸಗಿ_H

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
 * Describe the common methods of every object.
 * Similar to a C++ class.
 */
ರಚನೆ _lv_obj_class_t {
    ಸ್ಥಿರಾಂಕ lv_obj_class_t * ತಳ_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    ಶೂನ್ಯ (*constructor_cb)(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj);
    ಶೂನ್ಯ (*destructor_cb)(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    ಶೂನ್ಯ (*event_cb)(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#ಆದರೆ LV_USE_OBJ_PROPERTY
    uint32_t prop_ಸೂಚ್ಯಂಕ_ಆರಂಭ;
    uint32_t prop_ಸೂಚ್ಯಂಕ_ಕೊನೆ;
    ಸ್ಥಿರಾಂಕ lv_property_ops_t * properties;
    uint32_t properties_ಎಣಿಕೆ;

#ಆದರೆ LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ಸ್ಥಿರಾಂಕ lv_property_name_t * property_ಹೆಸರುಗಳು;
    uint32_t ಹೆಸರುಗಳು_ಎಣಿಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name;
    int32_t ಅಗಲ_def;
    int32_t ಎತ್ತರ_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t ಗುಂಪು_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_ಗಾತ್ರ : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

ಶೂನ್ಯ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_OBJ_CLASS_PRIVATE_H*/
