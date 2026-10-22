/**
 * @file lv_obj_class_private.h
 *
 */

#अगर_अघोषित LV_OBJ_CLASS_PRIVATE_H
#घोषणा LV_OBJ_CLASS_PRIVATE_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*********************
 *      INCLUDES
 *********************/

#समावेश "../lvgl_public.h"

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
काष्ठा _lv_obj_class_t {
    स्थिर lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    व्योम (*constructor_cb)(स्थिर lv_obj_class_t * class_p, lv_obj_t * obj);
    व्योम (*destructor_cb)(स्थिर lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    व्योम (*event_cb)(स्थिर lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#अगर LV_USE_OBJ_PROPERTY
    uint32_t prop_सूचकांक_start;
    uint32_t prop_सूचकांक_end;
    स्थिर lv_property_ops_t * properties;
    uint32_t properties_गिनती;

#अगर LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    स्थिर lv_property_name_t * property_names;
    uint32_t names_गिनती;
#पूर्ण_अगर
#पूर्ण_अगर

    व्योम * user_तथ्य;
    स्थिर अक्षर * name;
    int32_t width_def;
    int32_t height_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t group_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_आकार : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

व्योम lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_OBJ_CLASS_PRIVATE_H*/
