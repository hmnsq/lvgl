/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "lv_global.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

प्रकारव्याख्या संरचना _class_info_t {
    स्थिरांक lv_obj_class_t * class_p;
    uint32_t obj_मोजणी;
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

#जर LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

शून्य lv_obj_assign_id(स्थिरांक lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    LV_CHECK_ARG(class_p != रिक्त, परत);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = रिक्त;

    LV_ASSERT_NULL(global);

    obj->id = रिक्त;

    साठी(i = 0; i < global->objid_मोजणी; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        जर(class_p == info->class_p) थांबा;
    }

    /*Resize array*/
    जर(i == global->objid_मोजणी) {
        शून्य * array = lv_realloc(global->objid_array, आकार(class_info_t) * (global->objid_मोजणी + 1));
        LV_ASSERT_MALLOC(array);
        जर(array == रिक्त) परत;
        global->objid_array = array;
        global->objid_मोजणी ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_मोजणी = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_मोजणी;

    obj->id = (शून्य *)(lv_uintptr_t)id;
}

शून्य lv_obj_set_id(lv_obj_t * obj, शून्य * id)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    जर(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

शून्य lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, परत);
    obj->id = रिक्त;
}

स्थिरांक अक्षर * lv_obj_stringify_id(lv_obj_t * obj, अक्षर * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != रिक्त, परत रिक्त);
    LV_CHECK_ARG(obj->class_p != रिक्त, परत रिक्त);
    LV_CHECK_ARG(buf != रिक्त, परत रिक्त);
    LV_CHECK_ARG(len > 0, परत रिक्त);

    स्थिरांक अक्षर * name = obj->class_p->name;
    जर(name == रिक्त) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    परत buf;
}

शून्य lv_objid_builtin_destroy(शून्य)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    जर(global == रिक्त) परत;

    lv_free(global->objid_array);
    global->objid_array = रिक्त;
    global->objid_मोजणी = 0;
}

पूर्णांक lv_obj_id_compare(स्थिरांक शून्य * id1, स्थिरांक शून्य * id2)
{
    परत id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
