/**
 * @file lv_obj_class_private.h
 *
 */

#यदि_अपरिभाषितम् LV_OBJ_CLASS_वैयक्तिकम्_H
#परिभाषय LV_OBJ_CLASS_वैयक्तिकम्_H

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
 * Describe the common methods of every object.
 * Similar to a C++ class.
 */
संरचना _lv_obj_class_t {
    ध्रुव lv_obj_class_t * आधारः_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    शून्य (*constructor_cb)(ध्रुव lv_obj_class_t * class_p, lv_obj_t * obj);
    शून्य (*destructor_cb)(ध्रुव lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    शून्य (*event_cb)(ध्रुव lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#यदि LV_USE_OBJ_PROPERTY
    uint32_t prop_सूचकांक_start;
    uint32_t prop_सूचकांक_अन्तः;
    ध्रुव lv_property_ops_t * properties;
    uint32_t properties_गणनम्;

#यदि LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ध्रुव lv_property_name_t * property_नामानि;
    uint32_t नामानि_गणनम्;
#यदि_अन्तः
#यदि_अन्तः

    शून्य * user_दत्त;
    ध्रुव वर्ण * name;
    int32_t विस्तारः_def;
    int32_t height_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t गणः_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_आकार : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

शून्य lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_OBJ_CLASS_PRIVATE_H*/
