/**
 * @file lv_obj_class_private.h
 *
 */

#ifndef LV_OBJ_CLASS_PRIVATE_H
#define LV_OBJ_CLASS_PRIVATE_H

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

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
ಸಂರಚನಾ _lv_obj_class_t {
    ಧ್ರುವ lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    ಶೂನ್ಯ (*constructor_cb)(ಧ್ರುವ lv_obj_class_t * class_p, lv_obj_t * obj);
    ಶೂನ್ಯ (*destructor_cb)(ಧ್ರುವ lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    ಶೂನ್ಯ (*event_cb)(ಧ್ರುವ lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#ಯದಿ LV_USE_OBJ_PROPERTY
    uint32_t prop_index_start;
    uint32_t prop_index_end;
    ಧ್ರುವ lv_property_ops_t * properties;
    uint32_t properties_ಎಣಿಕೆ;

#ಯದಿ LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ಧ್ರುವ lv_property_name_t * property_names;
    uint32_t names_ಎಣಿಕೆ;
#endif
#endif

    ಶೂನ್ಯ * user_data;
    ಧ್ರುವ ವರ್ಣ * name;
    int32_t width_def;
    int32_t height_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t group_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_size : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

ಶೂನ್ಯ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_CLASS_PRIVATE_H*/
