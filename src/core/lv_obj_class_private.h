/**
 * @file lv_obj_class_private.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_CLASS_ਨਿੱਜੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_CLASS_ਨਿੱਜੀ_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/

#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

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
ਬਣਤਰ _lv_obj_class_t {
    ਅਚਲ lv_obj_class_t * ਅਧਾਰ_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    ਖਾਲੀ (*constructor_cb)(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj);
    ਖਾਲੀ (*destructor_cb)(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    ਖਾਲੀ (*event_cb)(ਅਚਲ lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#ਜੇ LV_USE_OBJ_PROPERTY
    uint32_t prop_ਸੂਚਕਾਂਕ_ਸ਼ੁਰੂ;
    uint32_t prop_ਸੂਚਕਾਂਕ_ਸਿਰਾ;
    ਅਚਲ lv_property_ops_t * properties;
    uint32_t properties_ਗਿਣਤੀ;

#ਜੇ LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ਅਚਲ lv_property_name_t * property_ਨਾਂ;
    uint32_t ਨਾਂ_ਗਿਣਤੀ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    ਖਾਲੀ * user_ਅੰਕੜੇ;
    ਅਚਲ ਅੱਖਰ * name;
    int32_t ਚੌੜਾਈ_def;
    int32_t ਉਚਾਈ_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t ਸਮੂਹ_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_ਮਾਪ : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

ਖਾਲੀ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_OBJ_CLASS_PRIVATE_H*/
