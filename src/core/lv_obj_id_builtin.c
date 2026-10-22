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
    अचर lv_obj_class_t * class_p;
    uint32_t obj_गन्ती;
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

#यदि LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

शून्य lv_obj_assign_id(अचर lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);
    LV_CHECK_ARG(class_p != रिक्त, फर्काऊ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = रिक्त;

    LV_ASSERT_NULL(global);

    obj->id = रिक्त;

    लागि(i = 0; i < global->objid_गन्ती; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        यदि(class_p == info->class_p) रोक;
    }

    /*Resize array*/
    यदि(i == global->objid_गन्ती) {
        शून्य * array = lv_realloc(global->objid_array, परिमाण(class_info_t) * (global->objid_गन्ती + 1));
        LV_ASSERT_MALLOC(array);
        यदि(array == रिक्त) फर्काऊ;
        global->objid_array = array;
        global->objid_गन्ती ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_गन्ती = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_गन्ती;

    obj->id = (शून्य *)(lv_uintptr_t)id;
}

शून्य lv_obj_set_id(lv_obj_t * obj, शून्य * id)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);
    यदि(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

शून्य lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);
    obj->id = रिक्त;
}

अचर वर्ण * lv_obj_stringify_id(lv_obj_t * obj, वर्ण * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ रिक्त);
    LV_CHECK_ARG(obj->class_p != रिक्त, फर्काऊ रिक्त);
    LV_CHECK_ARG(buf != रिक्त, फर्काऊ रिक्त);
    LV_CHECK_ARG(len > 0, फर्काऊ रिक्त);

    अचर वर्ण * name = obj->class_p->name;
    यदि(name == रिक्त) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    फर्काऊ buf;
}

शून्य lv_objid_builtin_destroy(शून्य)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    यदि(global == रिक्त) फर्काऊ;

    lv_free(global->objid_array);
    global->objid_array = रिक्त;
    global->objid_गन्ती = 0;
}

पूर्णाङ्क lv_obj_id_compare(अचर शून्य * id1, अचर शून्य * id2)
{
    फर्काऊ id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
