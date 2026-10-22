/**
 * @file lv_obj_class_private.h
 *
 */

#ifndef LV_OBJ_CLASS_PRIVATE_H
#define LV_OBJ_CLASS_PRIVATE_H

#ifdef __cplusplus
बाह्य "C" {
#endif

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
 * Describe the common methods of every object.
 * Similar to a C++ class.
 */
संरचना _lv_obj_class_t {
    अचर lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    शून्य (*constructor_cb)(अचर lv_obj_class_t * class_p, lv_obj_t * obj);
    शून्य (*destructor_cb)(अचर lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    शून्य (*event_cb)(अचर lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#यदि LV_USE_OBJ_PROPERTY
    uint32_t prop_index_start;
    uint32_t prop_index_end;
    अचर lv_property_ops_t * properties;
    uint32_t properties_गन्ती;

#यदि LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    अचर lv_property_name_t * property_names;
    uint32_t names_गन्ती;
#endif
#endif

    शून्य * user_data;
    अचर वर्ण * name;
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

शून्य lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_CLASS_PRIVATE_H*/
