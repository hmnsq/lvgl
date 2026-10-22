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

प्रकारनिर्देश संरचना _class_info_t {
    ध्रुव lv_obj_class_t * class_p;
    uint32_t obj_गणनम्;
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

#जइ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

शून्य lv_obj_assign_id(ध्रुव lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(class_p != रिक्त, प्रत्यागम);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = रिक्त;

    LV_ASSERT_NULL(global);

    obj->id = रिक्त;

    प्रति(i = 0; i < global->objid_गणनम्; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        जइ(class_p == info->class_p) भङ्ग;
    }

    /*Resize array*/
    जइ(i == global->objid_गणनम्) {
        शून्य * array = lv_realloc(global->objid_array, परिमाण(class_info_t) * (global->objid_गणनम् + 1));
        LV_ASSERT_MALLOC(array);
        जइ(array == रिक्त) प्रत्यागम;
        global->objid_array = array;
        global->objid_गणनम् ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_गणनम् = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_गणनम्;

    obj->id = (शून्य *)(lv_uintptr_t)id;
}

शून्य lv_obj_set_id(lv_obj_t * obj, शून्य * id)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    जइ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

शून्य lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    obj->id = रिक्त;
}

ध्रुव वर्ण * lv_obj_stringify_id(lv_obj_t * obj, वर्ण * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(obj->class_p != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(buf != रिक्त, प्रत्यागम रिक्त);
    LV_CHECK_ARG(len > 0, प्रत्यागम रिक्त);

    ध्रुव वर्ण * name = obj->class_p->name;
    जइ(name == रिक्त) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    प्रत्यागम buf;
}

शून्य lv_objid_builtin_destroy(शून्य)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    जइ(global == रिक्त) प्रत्यागम;

    lv_free(global->objid_array);
    global->objid_array = रिक्त;
    global->objid_गणनम् = 0;
}

पूर्णाङ्क lv_obj_id_compare(ध्रुव शून्य * id1, ध्रुव शून्य * id2)
{
    प्रत्यागम id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
