/**
 * @file lv_obj_class_private.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_OBJ_CLASS_ذاتي_H
#وصف_ڪريو LV_OBJ_CLASS_ذاتي_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

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
    ثابت lv_obj_class_t * بنياد_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    فراغ (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    فراغ (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    فراغ (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#إذا LV_USE_OBJ_PROPERTY
    uint32_t prop_اشاريو_شروعات;
    uint32_t prop_اشاريو_پڇاڙي;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_ڳڻپ;

#إذا LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_نالا;
    uint32_t نالا_ڳڻپ;
#جيڪڏهن_پورو
#جيڪڏهن_پورو

    فراغ * user_معلومات;
    ثابت حرف * name;
    int32_t ويڪر_def;
    int32_t اوچائي_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t گروهه_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_سائيز : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

فراغ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_OBJ_CLASS_PRIVATE_H*/
