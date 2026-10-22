/**
 * @file lv_obj_class_private.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_OBJ_CLASS_نجی_H
#تعریف_کرو LV_OBJ_CLASS_نجی_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

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
ساخت _lv_obj_class_t {
    ثابت lv_obj_class_t * بنیاد_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    خالی (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    خالی (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    خالی (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#اگر LV_USE_OBJ_PROPERTY
    uint32_t prop_اشاریہ_آغاز;
    uint32_t prop_اشاریہ_انتہا;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_گنتی;

#اگر LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_نام;
    uint32_t نام_گنتی;
#اگر_ختم
#اگر_ختم

    خالی * user_ڈیٹا;
    ثابت حرف * name;
    int32_t چوڑائی_def;
    int32_t اونچائی_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t گروہ_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_سائز : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

خالی lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_OBJ_CLASS_PRIVATE_H*/
