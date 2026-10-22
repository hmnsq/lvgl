/**
 * @file lv_obj_class_private.h
 *
 */

#నిర్వచితం_కాకపోతే LV_OBJ_CLASS_వ్యక్తిగత_H
#నిర్వచించు LV_OBJ_CLASS_వ్యక్తిగత_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/

#అన్తర్భావ "../lvgl_public.h"

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
నిర్మాణం _lv_obj_class_t {
    స్థిరరాశి lv_obj_class_t * పునాది_class;
    /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
    శూన్య (*constructor_cb)(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj);
    శూన్య (*destructor_cb)(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj);

    /** class_p is the class in which event is being processed. */
    శూన్య (*event_cb)(స్థిరరాశి lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

#ఒకవేళ LV_USE_OBJ_PROPERTY
    uint32_t prop_సూచీ_ప్రారంభం;
    uint32_t prop_సూచీ_చివర;
    స్థిరరాశి lv_property_ops_t * properties;
    uint32_t properties_లెక్క;

#ఒకవేళ LV_USE_OBJ_PROPERTY_NAME
    /* An array of property ID and name */
    స్థిరరాశి lv_property_name_t * property_పేర్లు;
    uint32_t పేర్లు_లెక్క;
#అయితే_ముగింపు
#అయితే_ముగింపు

    శూన్య * user_దత్తాంశం;
    స్థిరరాశి అక్షరం * name;
    int32_t వెడల్పు_def;
    int32_t ఎత్తు_def;
    uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
    uint32_t సమూహం_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
    uint32_t instance_కొలత : 16;
    uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
};


/**********************
 * GLOBAL PROTOTYPES
 **********************/

శూన్య lv_obj_destruct(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_OBJ_CLASS_PRIVATE_H*/
