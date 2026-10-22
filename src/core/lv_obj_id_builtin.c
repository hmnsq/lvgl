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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ _class_info_t {
    ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p;
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

#ಆದರೆ LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

ಶೂನ್ಯ lv_obj_ನಿಯೋಜಿಸು_id(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ಖಾಲಿ;

    LV_ASSERT_NULL(global);

    obj->id = ಖಾಲಿ;

    ಗಾಗಿ(i = 0; i < global->objid_ಎಣಿಕೆ; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        ಆದರೆ(class_p == info->class_p) ನಿಲ್ಲಿಸು;
    }

    /*Resize array*/
    ಆದರೆ(i == global->objid_ಎಣಿಕೆ) {
        ಶೂನ್ಯ * array = lv_realloc(global->objid_array, ಗಾತ್ರ(class_info_t) * (global->objid_ಎಣಿಕೆ + 1));
        LV_ASSERT_MALLOC(array);
        ಆದರೆ(array == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
        global->objid_array = array;
        global->objid_ಎಣಿಕೆ ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_ಎಣಿಕೆ = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_ಎಣಿಕೆ;

    obj->id = (ಶೂನ್ಯ *)(lv_uintptr_t)id;
}

ಶೂನ್ಯ lv_obj_ಗಣ_id(lv_obj_t * obj, ಶೂನ್ಯ * id)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    ಆದರೆ(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

ಶೂನ್ಯ lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    obj->id = ಖಾಲಿ;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * lv_obj_stringify_id(lv_obj_t * obj, ಅಕ್ಷರ * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(obj->class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(buf != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(len > 0, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name = obj->class_p->name;
    ಆದರೆ(name == ಖಾಲಿ) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ಹಿಂತಿರುಗಿ buf;
}

ಶೂನ್ಯ lv_objid_builtin_destroy(ಶೂನ್ಯ)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    ಆದರೆ(global == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;

    lv_free(global->objid_array);
    global->objid_array = ಖಾಲಿ;
    global->objid_ಎಣಿಕೆ = 0;
}

ಪೂರ್ಣಾಂಕ lv_obj_id_compare(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * id1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * id2)
{
    ಹಿಂತಿರುಗಿ id1 == id2 ? 0 : 1;
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_ID_BUILTIN*/
