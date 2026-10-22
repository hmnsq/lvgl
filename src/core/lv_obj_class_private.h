/**
 * @file lv_obj_class_private.h
 *
 */

#ifndef LV_OBJ_CLASS_PRIVATE_H
#define LV_OBJ_CLASS_PRIVATE_H

#ifdef __cplusplus
બાહ્ય "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#અન્તર્ભાવ "../lvgl_public.h"

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
સંરચના _lv_obj_class_t {
    અચલ lv_obj_class_t * base_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    શૂન્ય (*constructor_cb)(અચલ lv_obj_class_t * class_p, lv_obj_t * obj);
    શૂન્ય (*destructor_cb)(અચલ lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    શૂન્ય (*event_cb)(અચલ lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#જો LV_USE_OBJ_PROPERTY
    uint32_t prop_index_start;
    uint32_t prop_index_end;
    અચલ lv_property_ops_t * properties;
    uint32_t properties_ગણતરી;

#જો LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    અચલ lv_property_name_t * property_names;
    uint32_t names_ગણતરી;
#endif
#endif

    શૂન્ય * user_data;
    અચલ અક્ષર * name;
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

શૂન્ય lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBJ_CLASS_PRIVATE_H*/
