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

تعریفنوع ساختار _class_info_t {
    ثابت lv_obj_class_t * class_p;
    uint32_t obj_شمار;
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

تهی lv_obj_assign_id(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    LV_CHECK_ARG(class_p != پوچ, بازگشت);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = پوچ;

    LV_ASSERT_NULL(global);

    obj->id = پوچ;

    برای(i = 0; i < global->objid_شمار; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        اگر(class_p == info->class_p) بشکن;
    }

    /*Resize array*/
    اگر(i == global->objid_شمار) {
        تهی * array = lv_realloc(global->objid_array, اندازه(class_info_t) * (global->objid_شمار + 1));
        LV_ASSERT_MALLOC(array);
        اگر(array == پوچ) بازگشت;
        global->objid_array = array;
        global->objid_شمار ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_شمار = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_شمار;

    obj->id = (تهی *)(lv_uintptr_t)id;
}

تهی lv_obj_set_id(lv_obj_t * obj, تهی * id)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    اگر(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

تهی lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);
    obj->id = پوچ;
}

ثابت نویسه * lv_obj_stringify_id(lv_obj_t * obj, نویسه * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(obj->class_p != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(buf != پوچ, بازگشت پوچ);
    LV_CHECK_ARG(len > 0, بازگشت پوچ);

    ثابت نویسه * name = obj->class_p->name;
    اگر(name == پوچ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    بازگشت buf;
}

تهی lv_objid_builtin_destroy(تهی)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    اگر(global == پوچ) بازگشت;

    lv_free(global->objid_array);
    global->objid_array = پوچ;
    global->objid_شمار = 0;
}

صحیح lv_obj_id_compare(ثابت تهی * id1, ثابت تهی * id2)
{
    بازگشت id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
