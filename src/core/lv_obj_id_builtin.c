/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_class_private.h"
#شامل "lv_obj_private.h"
#شامل "lv_global.h"
#شامل "../lvgl_public.h"
#شامل "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

قسمتعریف ساخت _class_info_t {
    ثابت lv_obj_class_t * class_p;
    uint32_t obj_گنتی;
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

#اگر LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

خالی lv_obj_تفویض_id(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(class_p != تہی, واپس);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = تہی;

    LV_ASSERT_NULL(global);

    obj->id = تہی;

    برائے(i = 0; i < global->objid_گنتی; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        اگر(class_p == info->class_p) توڑو;
    }

    /*Resize array*/
    اگر(i == global->objid_گنتی) {
        خالی * array = lv_realloc(global->objid_array, حجم(class_info_t) * (global->objid_گنتی + 1));
        LV_ASSERT_MALLOC(array);
        اگر(array == تہی) واپس;
        global->objid_array = array;
        global->objid_گنتی ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_گنتی = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_گنتی;

    obj->id = (خالی *)(lv_uintptr_t)id;
}

خالی lv_obj_سیٹ_id(lv_obj_t * obj, خالی * id)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    اگر(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

خالی lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    obj->id = تہی;
}

ثابت حرف * lv_obj_stringify_id(lv_obj_t * obj, حرف * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != تہی, واپس تہی);
    LV_CHECK_ARG(obj->class_p != تہی, واپس تہی);
    LV_CHECK_ARG(buf != تہی, واپس تہی);
    LV_CHECK_ARG(len > 0, واپس تہی);

    ثابت حرف * name = obj->class_p->name;
    اگر(name == تہی) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    واپس buf;
}

خالی lv_objid_builtin_destroy(خالی)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    اگر(global == تہی) واپس;

    lv_free(global->objid_array);
    global->objid_array = تہی;
    global->objid_گنتی = 0;
}

صحیح lv_obj_id_compare(ثابت خالی * id1, ثابت خالی * id2)
{
    واپس id1 == id2 ? 0 : 1;
}

#اگر_ختم /*LV_USE_OBJ_ID_BUILTIN*/
