/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj_class_private.h"
#include "lv_obj_private.h"
#include "lv_global.h"
#include "../lvgl_public.h"
#include "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

typedef struct _class_info_t {
    const lv_obj_class_t * class_p;
    uint32_t obj_count;
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

#якщо LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

void lv_obj_assign_id(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    LV_CHECK_ARG(class_p != NULL, повернути);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = NULL;

    LV_ASSERT_NULL(global);

    obj->id = NULL;

    для(i = 0; i < global->objid_count; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        якщо(class_p == info->class_p) break;
    }

    /*Resize array*/
    якщо(i == global->objid_count) {
        void * array = lv_realloc(global->objid_array, sizeof(class_info_t) * (global->objid_count + 1));
        LV_ASSERT_MALLOC(array);
        якщо(array == NULL) повернути;
        global->objid_array = array;
        global->objid_count ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_count = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_count;

    obj->id = (void *)(lv_uintptr_t)id;
}

void lv_obj_set_id(lv_obj_t * obj, void * id)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    якщо(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

void lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, повернути);
    obj->id = NULL;
}

const char * lv_obj_stringify_id(lv_obj_t * obj, char * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != NULL, повернути NULL);
    LV_CHECK_ARG(obj->class_p != NULL, повернути NULL);
    LV_CHECK_ARG(buf != NULL, повернути NULL);
    LV_CHECK_ARG(len > 0, повернути NULL);

    const char * name = obj->class_p->name;
    якщо(name == NULL) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    повернути buf;
}

void lv_objid_builtin_destroy(void)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    якщо(global == NULL) повернути;

    lv_free(global->objid_array);
    global->objid_array = NULL;
    global->objid_count = 0;
}

ціле lv_obj_id_compare(const void * id1, const void * id2)
{
    повернути id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
