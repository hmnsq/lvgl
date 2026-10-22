/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "lv_obj_class_private.h"
#অন্তর্ভাব "lv_obj_private.h"
#অন্তর্ভাব "lv_global.h"
#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

প্রকারলেখ কাঠামো _class_info_t {
    ধ্রুবক lv_obj_class_t * class_p;
    uint32_t obj_গুনতি;
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

#যদি LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

শূন্য lv_obj_assign_id(ধ্রুবক lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);
    LV_CHECK_ARG(class_p != ফাঁকা, ফেরত);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ফাঁকা;

    LV_ASSERT_NULL(global);

    obj->id = ফাঁকা;

    জন্য(i = 0; i < global->objid_গুনতি; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        যদি(class_p == info->class_p) বিরতি;
    }

    /*Resize array*/
    যদি(i == global->objid_গুনতি) {
        শূন্য * array = lv_realloc(global->objid_array, আকার(class_info_t) * (global->objid_গুনতি + 1));
        LV_ASSERT_MALLOC(array);
        যদি(array == ফাঁকা) ফেরত;
        global->objid_array = array;
        global->objid_গুনতি ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_গুনতি = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_গুনতি;

    obj->id = (শূন্য *)(lv_uintptr_t)id;
}

শূন্য lv_obj_set_id(lv_obj_t * obj, শূন্য * id)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);
    যদি(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

শূন্য lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);
    obj->id = ফাঁকা;
}

ধ্রুবক অক্ষর * lv_obj_stringify_id(lv_obj_t * obj, অক্ষর * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত ফাঁকা);
    LV_CHECK_ARG(obj->class_p != ফাঁকা, ফেরত ফাঁকা);
    LV_CHECK_ARG(buf != ফাঁকা, ফেরত ফাঁকা);
    LV_CHECK_ARG(len > 0, ফেরত ফাঁকা);

    ধ্রুবক অক্ষর * name = obj->class_p->name;
    যদি(name == ফাঁকা) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ফেরত buf;
}

শূন্য lv_objid_builtin_destroy(শূন্য)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    যদি(global == ফাঁকা) ফেরত;

    lv_free(global->objid_array);
    global->objid_array = ফাঁকা;
    global->objid_গুনতি = 0;
}

পূর্ণসংখ্যা lv_obj_id_compare(ধ্রুবক শূন্য * id1, ধ্রুবক শূন্য * id2)
{
    ফেরত id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
