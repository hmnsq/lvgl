/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#அந்தர்பாவ "lv_obj_class_private.h"
#அந்தர்பாவ "lv_obj_private.h"
#அந்தர்பாவ "lv_global.h"
#அந்தர்பாவ "../lvgl_public.h"
#அந்தர்பாவ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

வகைவரை கட்டமைப்பு _class_info_t {
    மாறிலி lv_obj_class_t * class_p;
    uint32_t obj_மொத்தம்;
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

#எனில் LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

வெறுமை lv_obj_assign_id(மாறிலி lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    LV_CHECK_ARG(class_p != வெற்று, திரும்பு);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = வெற்று;

    LV_ASSERT_NULL(global);

    obj->id = வெற்று;

    க்கு(i = 0; i < global->objid_மொத்தம்; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        எனில்(class_p == info->class_p) நிறுத்து;
    }

    /*Resize array*/
    எனில்(i == global->objid_மொத்தம்) {
        வெறுமை * array = lv_realloc(global->objid_array, அளவு(class_info_t) * (global->objid_மொத்தம் + 1));
        LV_ASSERT_MALLOC(array);
        எனில்(array == வெற்று) திரும்பு;
        global->objid_array = array;
        global->objid_மொத்தம் ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_மொத்தம் = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_மொத்தம்;

    obj->id = (வெறுமை *)(lv_uintptr_t)id;
}

வெறுமை lv_obj_set_id(lv_obj_t * obj, வெறுமை * id)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    எனில்(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

வெறுமை lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);
    obj->id = வெற்று;
}

மாறிலி எழுத்து * lv_obj_stringify_id(lv_obj_t * obj, எழுத்து * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(obj->class_p != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(buf != வெற்று, திரும்பு வெற்று);
    LV_CHECK_ARG(len > 0, திரும்பு வெற்று);

    மாறிலி எழுத்து * name = obj->class_p->name;
    எனில்(name == வெற்று) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    திரும்பு buf;
}

வெறுமை lv_objid_builtin_destroy(வெறுமை)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    எனில்(global == வெற்று) திரும்பு;

    lv_free(global->objid_array);
    global->objid_array = வெற்று;
    global->objid_மொத்தம் = 0;
}

முழு lv_obj_id_compare(மாறிலி வெறுமை * id1, மாறிலி வெறுமை * id2)
{
    திரும்பு id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
