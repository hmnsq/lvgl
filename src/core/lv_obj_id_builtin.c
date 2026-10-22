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

প্রকারনির্দেশ সংরচনা _class_info_t {
    ধ্রুব lv_obj_class_t * class_p;
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

শূন্য lv_obj_assign_id(ধ্রুব lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    LV_CHECK_ARG(class_p != রিক্ত, প্রত্যাগম);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = রিক্ত;

    LV_ASSERT_NULL(global);

    obj->id = রিক্ত;

    প্রতি(i = 0; i < global->objid_গুনতি; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        যদি(class_p == info->class_p) ভঙ্গ;
    }

    /*Resize array*/
    যদি(i == global->objid_গুনতি) {
        শূন্য * array = lv_realloc(global->objid_array, পরিমাণ(class_info_t) * (global->objid_গুনতি + 1));
        LV_ASSERT_MALLOC(array);
        যদি(array == রিক্ত) প্রত্যাগম;
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
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    যদি(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

শূন্য lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম);
    obj->id = রিক্ত;
}

ধ্রুব বর্ণ * lv_obj_stringify_id(lv_obj_t * obj, বর্ণ * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(obj->class_p != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(buf != রিক্ত, প্রত্যাগম রিক্ত);
    LV_CHECK_ARG(len > 0, প্রত্যাগম রিক্ত);

    ধ্রুব বর্ণ * name = obj->class_p->name;
    যদি(name == রিক্ত) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    প্রত্যাগম buf;
}

শূন্য lv_objid_builtin_destroy(শূন্য)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    যদি(global == রিক্ত) প্রত্যাগম;

    lv_free(global->objid_array);
    global->objid_array = রিক্ত;
    global->objid_গুনতি = 0;
}

পূর্ণাঙ্ক lv_obj_id_compare(ধ্রুব শূন্য * id1, ধ্রুব শূন্য * id2)
{
    প্রত্যাগম id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
