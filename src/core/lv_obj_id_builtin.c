/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_class_private.h"
#ضمن "lv_obj_private.h"
#ضمن "lv_global.h"
#ضمن "../lvgl_public.h"
#ضمن "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

تعريفنوع بنية _class_info_t {
    ثابت lv_obj_class_t * class_p;
    uint32_t obj_کيراءن;
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

#إذا LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

فراغ lv_obj_assign_id(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(class_p != عدم, أرجع);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = عدم;

    LV_ASSERT_NULL(global);

    obj->id = عدم;

    لكل(i = 0; i < global->objid_کيراءن; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        إذا(class_p == info->class_p) اكسر;
    }

    /*Resize array*/
    إذا(i == global->objid_کيراءن) {
        فراغ * array = lv_realloc(global->objid_array, حجم(class_info_t) * (global->objid_کيراءن + 1));
        LV_ASSERT_MALLOC(array);
        إذا(array == عدم) أرجع;
        global->objid_array = array;
        global->objid_کيراءن ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_کيراءن = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_کيراءن;

    obj->id = (فراغ *)(lv_uintptr_t)id;
}

فراغ lv_obj_set_id(lv_obj_t * obj, فراغ * id)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    إذا(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

فراغ lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    obj->id = عدم;
}

ثابت حرف * lv_obj_stringify_id(lv_obj_t * obj, حرف * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != عدم, أرجع عدم);
    LV_CHECK_ARG(obj->class_p != عدم, أرجع عدم);
    LV_CHECK_ARG(buf != عدم, أرجع عدم);
    LV_CHECK_ARG(len > 0, أرجع عدم);

    ثابت حرف * name = obj->class_p->name;
    إذا(name == عدم) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    أرجع buf;
}

فراغ lv_objid_builtin_destroy(فراغ)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    إذا(global == عدم) أرجع;

    lv_free(global->objid_array);
    global->objid_array = عدم;
    global->objid_کيراءن = 0;
}

صحيح lv_obj_id_compare(ثابت فراغ * id1, ثابت فراغ * id2)
{
    أرجع id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
