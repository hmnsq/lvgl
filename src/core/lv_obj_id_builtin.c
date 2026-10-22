/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "lv_obj_class_private.h"
#समावेश "lv_obj_private.h"
#समावेश "lv_global.h"
#समावेश "../lvgl_public.h"
#समावेश "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

प्रकार काष्ठा _class_info_t {
    स्थिर lv_obj_class_t * class_p;
    uint32_t obj_गिनती;
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

#अगर LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

व्योम lv_obj_assign_id(स्थिर lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    LV_CHECK_ARG(class_p != शून्य, वापस);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = शून्य;

    LV_ASSERT_NULL(global);

    obj->id = शून्य;

    क्रम(i = 0; i < global->objid_गिनती; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        अगर(class_p == info->class_p) अवरोध;
    }

    /*Resize array*/
    अगर(i == global->objid_गिनती) {
        व्योम * array = lv_realloc(global->objid_array, माप(class_info_t) * (global->objid_गिनती + 1));
        LV_ASSERT_MALLOC(array);
        अगर(array == शून्य) वापस;
        global->objid_array = array;
        global->objid_गिनती ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_गिनती = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_गिनती;

    obj->id = (व्योम *)(lv_uintptr_t)id;
}

व्योम lv_obj_set_id(lv_obj_t * obj, व्योम * id)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    अगर(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

व्योम lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);
    obj->id = शून्य;
}

स्थिर अक्षर * lv_obj_stringify_id(lv_obj_t * obj, अक्षर * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != शून्य, वापस शून्य);
    LV_CHECK_ARG(obj->class_p != शून्य, वापस शून्य);
    LV_CHECK_ARG(buf != शून्य, वापस शून्य);
    LV_CHECK_ARG(len > 0, वापस शून्य);

    स्थिर अक्षर * name = obj->class_p->name;
    अगर(name == शून्य) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    वापस buf;
}

व्योम lv_objid_builtin_destroy(व्योम)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    अगर(global == शून्य) वापस;

    lv_free(global->objid_array);
    global->objid_array = शून्य;
    global->objid_गिनती = 0;
}

पूर्णांक lv_obj_id_compare(स्थिर व्योम * id1, स्थिर व्योम * id2)
{
    वापस id1 == id2 ? 0 : 1;
}

#पूर्ण_अगर /*LV_USE_OBJ_ID_BUILTIN*/
