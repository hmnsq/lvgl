/**
 * @file lv_obj_class_private.h
 *
 */

#که_نه_تعریف_شوی LV_OBJ_CLASS_شخصي_H
#تعریف_کړه LV_OBJ_CLASS_شخصي_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/*********************
 *      INCLUDES
 *********************/

#ضمن "../lvgl_public.h"

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
بنية _lv_obj_class_t {
    ثابت lv_obj_class_t * بنسټ_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    فراغ (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    فراغ (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    فراغ (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#إذا LV_USE_OBJ_PROPERTY
    uint32_t prop_شاخص_پیل;
    uint32_t prop_شاخص_پای_ټکی;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_شمېر;

#إذا LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_نومونه;
    uint32_t نومونه_شمېر;
#د_که_پای
#د_که_پای

    فراغ * user_معلومات;
    ثابت حرف * name;
    int32_t پلنوالی_def;
    int32_t لوړوالی_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t ډله_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_اندازه : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

فراغ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_OBJ_CLASS_PRIVATE_H*/
