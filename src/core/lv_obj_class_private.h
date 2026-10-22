/**
 * @file lv_obj_class_private.h
 *
 */

#ifndef LV_OBJ_CLASS_PRIVATE_H
#define LV_OBJ_CLASS_PRIVATE_H

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"

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
ସଂରଚନା _lv_obj_class_t {
    ଅଚଳ lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    ଶୂନ୍ୟ (*constructor_cb)(ଅଚଳ lv_obj_class_t * class_p, lv_obj_t * obj);
    ଶୂନ୍ୟ (*destructor_cb)(ଅଚଳ lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    ଶୂନ୍ୟ (*event_cb)(ଅଚଳ lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#ଯଦି LV_USE_OBJ_PROPERTY
    uint32_t prop_index_start;
    uint32_t prop_index_end;
    ଅଚଳ lv_property_ops_t * properties;
    uint32_t properties_ଗଣନା;

#ଯଦି LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ଅଚଳ lv_property_name_t * property_names;
    uint32_t names_ଗଣନା;
#endif
#endif

    ଶୂନ୍ୟ * user_data;
    ଅଚଳ ଅକ୍ଷର * name;
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

ଶୂନ୍ୟ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_CLASS_PRIVATE_H*/
