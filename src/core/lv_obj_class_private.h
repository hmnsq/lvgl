/**
 * @file lv_obj_class_private.h
 *
 */

#اگر_تعریف_نشده LV_OBJ_CLASS_خصوصی_H
#تعریف_کن LV_OBJ_CLASS_خصوصی_H

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

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
    ثابت lv_obj_class_t * پایه_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    فراغ (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    فراغ (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    فراغ (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#إذا LV_USE_OBJ_PROPERTY
    uint32_t prop_شاخص_شروع;
    uint32_t prop_شاخص_انتها;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_شمار;

#إذا LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_نامها;
    uint32_t نامها_شمار;
#پایان_اگر
#پایان_اگر

    فراغ * user_داده;
    ثابت حرف * name;
    int32_t عرض_def;
    int32_t ارتفاع_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t گروه_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
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

#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_OBJ_CLASS_PRIVATE_H*/
