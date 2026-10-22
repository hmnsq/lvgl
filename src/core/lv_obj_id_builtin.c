/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "lv_global.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

రకనిర్వచనం నిర్మాణం _class_info_t {
    స్థిరరాశి lv_obj_class_t * class_p;
    uint32_t obj_లెక్క;
} class_info_t;

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#ఒకవేళ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

శూన్య lv_obj_కేటాయించు_id(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ఖాళీ;

    LV_ASSERT_NULL(global);

    obj->id = ఖాళీ;

    కొరకు(i = 0; i < global->objid_లెక్క; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        ఒకవేళ(class_p == info->class_p) ఆపు;
    }

    /*Resize array*/
    ఒకవేళ(i == global->objid_లెక్క) {
        శూన్య * array = lv_realloc(global->objid_array, పరిమాణం(class_info_t) * (global->objid_లెక్క + 1));
        LV_ASSERT_MALLOC(array);
        ఒకవేళ(array == ఖాళీ) తిరిగి;
        global->objid_array = array;
        global->objid_లెక్క ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_లెక్క = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_లెక్క;

    obj->id = (శూన్య *)(lv_uintptr_t)id;
}

శూన్య lv_obj_సమితి_id(lv_obj_t * obj, శూన్య * id)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    ఒకవేళ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

శూన్య lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    obj->id = ఖాళీ;
}

స్థిరరాశి అక్షరం * lv_obj_stringify_id(lv_obj_t * obj, అక్షరం * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(obj->class_p != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(buf != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(len > 0, తిరిగి ఖాళీ);

    స్థిరరాశి అక్షరం * name = obj->class_p->name;
    ఒకవేళ(name == ఖాళీ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    తిరిగి buf;
}

శూన్య lv_objid_builtin_destroy(శూన్య)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    ఒకవేళ(global == ఖాళీ) తిరిగి;

    lv_free(global->objid_array);
    global->objid_array = ఖాళీ;
    global->objid_లెక్క = 0;
}

పూర్ణాంకం lv_obj_id_compare(స్థిరరాశి శూన్య * id1, స్థిరరాశి శూన్య * id2)
{
    తిరిగి id1 == id2 ? 0 : 1;
}

#అయితే_ముగింపు /*LV_USE_OBJ_ID_BUILTIN*/
