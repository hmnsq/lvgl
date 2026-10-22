/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "lv_obj_class_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "lv_global.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା _class_info_t {
    ଅଚଳ lv_obj_class_t * class_p;
    uint32_t obj_ଗଣନା;
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

#ଯଦି LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

ଶୂନ୍ୟ lv_obj_assign_id(ଅଚଳ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(class_p != ଖାଲି, ଫେରନ୍ତୁ);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ଖାଲି;

    LV_ASSERT_NULL(global);

    obj->id = ଖାଲି;

    ପାଇଁ(i = 0; i < global->objid_ଗଣନା; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        ଯଦି(class_p == info->class_p) ଭାଙ୍ଗ;
    }

    /*Resize array*/
    ଯଦି(i == global->objid_ଗଣନା) {
        ଶୂନ୍ୟ * array = lv_realloc(global->objid_array, ଆକାର(class_info_t) * (global->objid_ଗଣନା + 1));
        LV_ASSERT_MALLOC(array);
        ଯଦି(array == ଖାଲି) ଫେରନ୍ତୁ;
        global->objid_array = array;
        global->objid_ଗଣନା ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_ଗଣନା = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_ଗଣନା;

    obj->id = (ଶୂନ୍ୟ *)(lv_uintptr_t)id;
}

ଶୂନ୍ୟ lv_obj_set_id(lv_obj_t * obj, ଶୂନ୍ୟ * id)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    ଯଦି(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

ଶୂନ୍ୟ lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);
    obj->id = ଖାଲି;
}

ଅଚଳ ଅକ୍ଷର * lv_obj_stringify_id(lv_obj_t * obj, ଅକ୍ଷର * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(obj->class_p != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(buf != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(len > 0, ଫେରନ୍ତୁ ଖାଲି);

    ଅଚଳ ଅକ୍ଷର * name = obj->class_p->name;
    ଯଦି(name == ଖାଲି) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    ଫେରନ୍ତୁ buf;
}

ଶୂନ୍ୟ lv_objid_builtin_destroy(ଶୂନ୍ୟ)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    ଯଦି(global == ଖାଲି) ଫେରନ୍ତୁ;

    lv_free(global->objid_array);
    global->objid_array = ଖାଲି;
    global->objid_ଗଣନା = 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା lv_obj_id_compare(ଅଚଳ ଶୂନ୍ୟ * id1, ଅଚଳ ଶୂନ୍ୟ * id2)
{
    ଫେରନ୍ତୁ id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
