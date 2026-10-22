/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "lv_global.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ _class_info_t {
    ಧ್ರುವ lv_obj_class_t * class_p;
    uint32_t obj_ಎಣಿಕೆ;
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

#ಯದಿ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

ಶೂನ್ಯ lv_obj_assign_id(ಧ್ರುವ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ರಿಕ್ತ;

    LV_ASSERT_NULL(global);

    obj->id = ರಿಕ್ತ;

    ಪ್ರತಿ(i = 0; i < global->objid_ಎಣಿಕೆ; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        ಯದಿ(class_p == info->class_p) ಭಙ್ಗ;
    }

    /*Resize array*/
    ಯದಿ(i == global->objid_ಎಣಿಕೆ) {
        ಶೂನ್ಯ * array = lv_realloc(global->objid_array, ಪರಿಮಾಣ(class_info_t) * (global->objid_ಎಣಿಕೆ + 1));
        LV_ASSERT_MALLOC(array);
        ಯದಿ(array == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;
        global->objid_array = array;
        global->objid_ಎಣಿಕೆ ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_ಎಣಿಕೆ = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_ಎಣಿಕೆ;

    obj->id = (ಶೂನ್ಯ *)(lv_uintptr_t)id;
}

ಶೂನ್ಯ lv_obj_set_id(lv_obj_t * obj, ಶೂನ್ಯ * id)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    ಯದಿ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

ಶೂನ್ಯ lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    obj->id = ರಿಕ್ತ;
}

ಧ್ರುವ ವರ್ಣ * lv_obj_stringify_id(lv_obj_t * obj, ವರ್ಣ * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(obj->class_p != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(buf != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    LV_CHECK_ARG(len > 0, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);

    ಧ್ರುವ ವರ್ಣ * name = obj->class_p->name;
    ಯದಿ(name == ರಿಕ್ತ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ಪ್ರತ್ಯಾಗಮ buf;
}

ಶೂನ್ಯ lv_objid_builtin_destroy(ಶೂನ್ಯ)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    ಯದಿ(global == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    lv_free(global->objid_array);
    global->objid_array = ರಿಕ್ತ;
    global->objid_ಎಣಿಕೆ = 0;
}

ಪೂರ್ಣಾಙ್ಕ lv_obj_id_compare(ಧ್ರುವ ಶೂನ್ಯ * id1, ಧ್ರುವ ಶೂನ್ಯ * id2)
{
    ಪ್ರತ್ಯಾಗಮ id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
