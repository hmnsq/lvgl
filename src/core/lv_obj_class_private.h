/**
 * @file lv_obj_class_private.h
 *
 */

#جے_بنا_تعریف LV_OBJ_CLASS_نجی_H
#تعریف_کرو LV_OBJ_CLASS_نجی_H

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

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
    ثابت lv_obj_class_t * ادھار_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    فراغ (*constructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
    فراغ (*destructor_cb)(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    فراغ (*event_cb)(ثابت lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#إذا LV_USE_OBJ_PROPERTY
    uint32_t prop_اشاریہ_شروع;
    uint32_t prop_اشاریہ_سرا;
    ثابت lv_property_ops_t * properties;
    uint32_t properties_گنتی;

#إذا LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    ثابت lv_property_name_t * property_ناں;
    uint32_t ناں_گنتی;
#جے_انت
#جے_انت

    فراغ * user_انکڑے;
    ثابت حرف * name;
    int32_t چوڑائی_def;
    int32_t اچائی_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t گروہ_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_ناپ : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

فراغ lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#جے_تعریف_شدہ __cplusplus
} /*extern "C"*/
#جے_انت

#جے_انت /*LV_OBJ_CLASS_PRIVATE_H*/
