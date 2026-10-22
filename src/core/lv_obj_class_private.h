/**
 * @file lv_obj_class_private.h
 *
 */

#ifndef LV_OBJ_CLASS_PRIVATE_H
#define LV_OBJ_CLASS_PRIVATE_H

#ifdef __cplusplus
بیرونی "C" {
#endif

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
 * Describe the common methods of every object.
 * Similar to a C++ class.
 */
ساختار _lv_obj_class_t {
    ثابت lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    تهی (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    تهی (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    تهی (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#اگر LV_USE_OBJ_PROPERTY
    uint32_t prop_index_start;
    uint32_t prop_index_end;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_شمار;

#اگر LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_names;
    uint32_t names_شمار;
#endif
#endif

    تهی * user_data;
    ثابت نویسه * name;
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

تهی lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_CLASS_PRIVATE_H*/
