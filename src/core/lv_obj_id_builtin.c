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

typedef sa_nracanaa _class_info_t {
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

#නම් LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

void lv_obj_assign_id(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, ආපසු);
    LV_CHECK_ARG(class_p != NULL, ආපසු);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = NULL;

    LV_ASSERT_NULL(global);

    obj->id = NULL;

    සඳහා(i = 0; i < global->objid_count; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        නම්(class_p == info->class_p) bha_nga;
    }

    /*Resize array*/
    නම්(i == global->objid_count) {
        void * array = lv_realloc(global->objid_array, parimaa_na(class_info_t) * (global->objid_count + 1));
        LV_ASSERT_MALLOC(array);
        නම්(array == NULL) ආපසු;
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
    LV_CHECK_ARG(obj != NULL, ආපසු);
    නම්(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

void lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, ආපසු);
    obj->id = NULL;
}

const var_na * lv_obj_stringify_id(lv_obj_t * obj, var_na * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != NULL, ආපසු NULL);
    LV_CHECK_ARG(obj->class_p != NULL, ආපසු NULL);
    LV_CHECK_ARG(buf != NULL, ආපසු NULL);
    LV_CHECK_ARG(len > 0, ආපසු NULL);

    const var_na * name = obj->class_p->name;
    නම්(name == NULL) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ආපසු buf;
}

void lv_objid_builtin_destroy(void)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    නම්(global == NULL) ආපසු;

    lv_free(global->objid_array);
    global->objid_array = NULL;
    global->objid_count = 0;
}

පූර්ණ_සංඛ්‍යා lv_obj_id_compare(const void * id1, const void * id2)
{
    ආපසු id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
