/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ _class_info_t {
    ਅਚਲ lv_obj_class_t * class_p;
    uint32_t obj_ਗਿਣਤੀ;
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

#ਜੇ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

ਖਾਲੀ lv_obj_ਸੌਂਪੋ_id(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ਨਿਰਾ;

    LV_ASSERT_NULL(global);

    obj->id = ਨਿਰਾ;

    ਲਈ(i = 0; i < global->objid_ਗਿਣਤੀ; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        ਜੇ(class_p == info->class_p) ਰੋਕੋ;
    }

    /*Resize array*/
    ਜੇ(i == global->objid_ਗਿਣਤੀ) {
        ਖਾਲੀ * array = lv_realloc(global->objid_array, ਆਕਾਰ(class_info_t) * (global->objid_ਗਿਣਤੀ + 1));
        LV_ASSERT_MALLOC(array);
        ਜੇ(array == ਨਿਰਾ) ਵਾਪਸ;
        global->objid_array = array;
        global->objid_ਗਿਣਤੀ ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_ਗਿਣਤੀ = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_ਗਿਣਤੀ;

    obj->id = (ਖਾਲੀ *)(lv_uintptr_t)id;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_id(lv_obj_t * obj, ਖਾਲੀ * id)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    ਜੇ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

ਖਾਲੀ lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    obj->id = ਨਿਰਾ;
}

ਅਚਲ ਅੱਖਰ * lv_obj_stringify_id(lv_obj_t * obj, ਅੱਖਰ * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(obj->class_p != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(buf != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(len > 0, ਵਾਪਸ ਨਿਰਾ);

    ਅਚਲ ਅੱਖਰ * name = obj->class_p->name;
    ਜੇ(name == ਨਿਰਾ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ਵਾਪਸ buf;
}

ਖਾਲੀ lv_objid_builtin_destroy(ਖਾਲੀ)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    ਜੇ(global == ਨਿਰਾ) ਵਾਪਸ;

    lv_free(global->objid_array);
    global->objid_array = ਨਿਰਾ;
    global->objid_ਗਿਣਤੀ = 0;
}

ਪੂਰਨ lv_obj_id_compare(ਅਚਲ ਖਾਲੀ * id1, ਅਚਲ ਖਾਲੀ * id2)
{
    ਵਾਪਸ id1 == id2 ? 0 : 1;
}

#ਜੇ_ਅੰਤ /*LV_USE_OBJ_ID_BUILTIN*/
