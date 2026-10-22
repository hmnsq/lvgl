/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "lv_obj_class_private.h"
#അന്തര്ഭാവ "lv_obj_private.h"
#അന്തര്ഭാവ "lv_global.h"
#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

തരനിർവചനം ഘടന _class_info_t {
    സ്ഥിരം lv_obj_class_t * class_p;
    uint32_t obj_എണ്ണം;
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

#എങ്കിൽ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

ശൂന്യം lv_obj_assign_id(സ്ഥിരം lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(class_p != ശൂന്യ, തിരികെ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ശൂന്യ;

    LV_ASSERT_NULL(global);

    obj->id = ശൂന്യ;

    വേണ്ടി(i = 0; i < global->objid_എണ്ണം; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        എങ്കിൽ(class_p == info->class_p) നിർത്തുക;
    }

    /*Resize array*/
    എങ്കിൽ(i == global->objid_എണ്ണം) {
        ശൂന്യം * array = lv_realloc(global->objid_array, വലുപ്പം(class_info_t) * (global->objid_എണ്ണം + 1));
        LV_ASSERT_MALLOC(array);
        എങ്കിൽ(array == ശൂന്യ) തിരികെ;
        global->objid_array = array;
        global->objid_എണ്ണം ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_എണ്ണം = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_എണ്ണം;

    obj->id = (ശൂന്യം *)(lv_uintptr_t)id;
}

ശൂന്യം lv_obj_set_id(lv_obj_t * obj, ശൂന്യം * id)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    എങ്കിൽ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

ശൂന്യം lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);
    obj->id = ശൂന്യ;
}

സ്ഥിരം അക്ഷരം * lv_obj_stringify_id(lv_obj_t * obj, അക്ഷരം * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(obj->class_p != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(buf != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(len > 0, തിരികെ ശൂന്യ);

    സ്ഥിരം അക്ഷരം * name = obj->class_p->name;
    എങ്കിൽ(name == ശൂന്യ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    തിരികെ buf;
}

ശൂന്യം lv_objid_builtin_destroy(ശൂന്യം)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    എങ്കിൽ(global == ശൂന്യ) തിരികെ;

    lv_free(global->objid_array);
    global->objid_array = ശൂന്യ;
    global->objid_എണ്ണം = 0;
}

പൂർണ്ണസംഖ്യ lv_obj_id_compare(സ്ഥിരം ശൂന്യം * id1, സ്ഥിരം ശൂന്യം * id2)
{
    തിരികെ id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
