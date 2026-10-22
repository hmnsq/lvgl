/**
 * @file lv_obj_id_builtin.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "lv_obj_class_private.h"
#અન્તર્ભાવ "lv_obj_private.h"
#અન્તર્ભાવ "lv_global.h"
#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "../osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

પ્રકારવ્યાખ્યા સંરચના _class_info_t {
    અચલ lv_obj_class_t * class_p;
    uint32_t obj_ગણતરી;
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

#જો LV_USE_OBJ_ID && LV_USE_OBJ_ID_BUILTIN

શૂન્ય lv_obj_assign_id(અચલ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    LV_CHECK_ARG(class_p != ખાલી, પાછા);

    uint32_t i;
    uint32_t id = 0;
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    class_info_t * info = ખાલી;

    LV_ASSERT_NULL(global);

    obj->id = ખાલી;

    માટે(i = 0; i < global->objid_ગણતરી; i ++) {
        info = ((class_info_t *)global->objid_array) + i;
        જો(class_p == info->class_p) તોડો;
    }

    /*Resize array*/
    જો(i == global->objid_ગણતરી) {
        શૂન્ય * array = lv_realloc(global->objid_array, કદ(class_info_t) * (global->objid_ગણતરી + 1));
        LV_ASSERT_MALLOC(array);
        જો(array == ખાલી) પાછા;
        global->objid_array = array;
        global->objid_ગણતરી ++;
        info = ((class_info_t *)global->objid_array) + i;
        info->obj_ગણતરી = 0;
        info->class_p = class_p;
    }

    id = ++info->obj_ગણતરી;

    obj->id = (શૂન્ય *)(lv_uintptr_t)id;
}

શૂન્ય lv_obj_set_id(lv_obj_t * obj, શૂન્ય * id)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    જો(obj->id) lv_obj_free_id(obj);
    obj->id = id;
}

શૂન્ય lv_obj_free_id(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);
    obj->id = ખાલી;
}

અચલ અક્ષર * lv_obj_stringify_id(lv_obj_t * obj, અક્ષર * buf, uint32_t len)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(obj->class_p != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(buf != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(len > 0, પાછા ખાલી);

    અચલ અક્ષર * name = obj->class_p->name;
    જો(name == ખાલી) name = "nameless";

    lv_snprintf(buf, len, "%s%" LV_PRIu32 "", name, (uint32_t)(lv_uintptr_t)obj->id);
    પાછા buf;
}

શૂન્ય lv_objid_builtin_destroy(શૂન્ય)
{
    lv_global_t * global = LV_GLOBAL_DEFAULT();
    જો(global == ખાલી) પાછા;

    lv_free(global->objid_array);
    global->objid_array = ખાલી;
    global->objid_ગણતરી = 0;
}

પૂર્ણાંક lv_obj_id_compare(અચલ શૂન્ય * id1, અચલ શૂન્ય * id2)
{
    પાછા id1 == id2 ? 0 : 1;
}

#endif /*LV_USE_OBJ_ID_BUILTIN*/
