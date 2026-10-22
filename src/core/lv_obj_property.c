/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl_public.h"

#ᰀᰧ LV_USE_OBJ_PROPERTY

#include "lv_obj_style_internal.h"
#include "lv_obj_private.h"
#include "../misc/lv_utils.h"
#include "lv_obj_class_private.h"
#include "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    ᰀᰧ(!set) { \
        value->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ᰀᰧᰀ { \
        switch(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            pak_sa LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, value->field); \
                bha_nga; \
            pak_sa LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, value->arg1.field, value->arg2.num); \
                bha_nga; \
            pak_sa LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, value->arg1.field, value->arg2.enable); \
                bha_nga; \
            pak_sa LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, value->arg1.field, value->arg2.precise); \
                bha_nga; \
            pak_sa LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, value->arg1.field, value->arg2.color); \
                bha_nga; \
            pak_sa LV_PROPERTY_TYPE_POINTER: \
            pak_sa LV_PROPERTY_TYPE_IMGSRC: \
            pak_sa LV_PROPERTY_TYPE_TEXT: \
            pak_sa LV_PROPERTY_TYPE_OBJ: \
            pak_sa LV_PROPERTY_TYPE_DISPLAY: \
            pak_sa LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, value->arg1.field, value->arg2.ptr); \
                bha_nga; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

typedef int32_t integer;
typedef bool boolean;
typedef lv_value_precise_t precise;
typedef lv_color_t color;
typedef const void * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    typedef void (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    typedef void (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    typedef void (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    typedef void (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_value_precise_t); \
    typedef void (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    typedef void (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, const void *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

typedef void (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
typedef lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, const lv_property_t *);

typedef integer(*lv_property_get_integer_t)(const lv_obj_t *);
typedef bool (*lv_property_get_boolean_t)(const lv_obj_t *);
typedef lv_value_precise_t (*lv_property_get_precise_t)(const lv_obj_t *);
typedef lv_color_t (*lv_property_get_color_t)(const lv_obj_t *);
typedef void * (*lv_property_get_pointer_t)(const lv_obj_t *);
typedef lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

typedef lv_result_t (*lv_property_getter_t)(const lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set);
static puur_naa_nka property_name_compare(const void * ref, const void * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, const lv_property_t * value)
{
    LV_CHECK_ARG(obj != NULL, ᰀᰧᰌ LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, ᰀᰧᰌ LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(value->id);
    ᰀᰧ(value->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ᰀᰧᰌ LV_RESULT_INVALID;
    }

    ᰀᰧ(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, value->style, value->selector);
        ᰀᰧᰌ LV_RESULT_OK;
    }

    ᰀᰧᰌ obj_property(obj, value->id, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)
{
    LV_CHECK_ARG(obj != NULL, ᰀᰧᰌ LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, ᰀᰧᰌ LV_RESULT_INVALID);

    for(uint32_t i = 0; i < count; i++) {
        lv_result_t result = lv_obj_set_property(obj, &value[i]);
        ᰀᰧ(result != LV_RESULT_OK) {
            ᰀᰧᰌ result;
        }
    }

    ᰀᰧᰌ LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != NULL, ᰀᰧᰌ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t value = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    ᰀᰧ(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        ᰀᰧᰌ value;
    }

    ᰀᰧ(index < LV_PROPERTY_ID_START) {
        value.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        value.id = id;
        value.selector = LV_PART_MAIN | obj->state;
        ᰀᰧᰌ value;
    }

    result = obj_property(obj, id, &value, false);
    ᰀᰧ(result != LV_RESULT_OK)
        value.id = LV_PROPERTY_ID_INVALID;

    ᰀᰧᰌ value;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != NULL, ᰀᰧᰌ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t value;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    ᰀᰧ(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        ᰀᰧᰌ value;
    }

    value.style = lv_obj_get_style_prop(obj, part, index);
    value.id = id;
    value.selector = part | obj->state;
    ᰀᰧᰌ value;
}

lv_prop_id_t lv_style_property_get_id(const var_na * name)
{
    LV_CHECK_ARG(name != NULL, ᰀᰧᰌ LV_PROPERTY_ID_INVALID);
#ᰀᰧ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, parimaa_na(lv_style_property_names) / parimaa_na(lv_property_name_t),
                             parimaa_na(lv_property_name_t), property_name_compare);
    ᰀᰧ(found) ᰀᰧᰌ found->id;
#ᰀᰧᰀ
    LV_UNUSED(name);
#endif
    ᰀᰧᰌ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(const lv_obj_class_t * clz, const var_na * name)
{
    LV_CHECK_ARG(clz != NULL, ᰀᰧᰌ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, ᰀᰧᰌ LV_PROPERTY_ID_INVALID);
#ᰀᰧ LV_USE_OBJ_PROPERTY_NAME
    const lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    ᰀᰧ(names == NULL) {
        /* try base class*/
        ᰀᰧᰌ LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_count, parimaa_na(lv_property_name_t), property_name_compare);
    ᰀᰧ(found) ᰀᰧᰌ found->id;
#ᰀᰧᰀ
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ᰀᰧᰌ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(const lv_obj_t * obj, const var_na * name)
{
    LV_CHECK_ARG(obj != NULL, ᰀᰧᰌ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, ᰀᰧᰌ LV_PROPERTY_ID_INVALID);
#ᰀᰧ LV_USE_OBJ_PROPERTY_NAME

    const lv_obj_class_t * clz;
    lv_prop_id_t id;

    for(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ᰀᰧ(id != LV_PROPERTY_ID_INVALID) ᰀᰧᰌ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ᰀᰧ(id != LV_PROPERTY_ID_INVALID) ᰀᰧᰌ id;
#ᰀᰧᰀ
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ᰀᰧᰌ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

static lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set)
{
    const lv_property_ops_t * properties;
    const lv_property_ops_t * prop;

    const lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    for(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        ᰀᰧ(properties == NULL) {
            /* try base class*/
            continue;
        }

        ᰀᰧ(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            continue;
        }

        /*Check if there's setter available for this class*/
        for(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ᰀᰧ(prop->id == LV_PROPERTY_ID_ANY) {
                value->id = prop->id;
                ᰀᰧ(set) ᰀᰧᰌ ((lv_property_setter_t)prop->setter)(obj, id, value);
                ᰀᰧᰀ ᰀᰧᰌ ((lv_property_getter_t)prop->getter)(obj, id, value);
            }

            /*Not this id, check next*/
            ᰀᰧ(prop->id != id)
                continue;

            /*id matched but we got null pointer to functions*/
            ᰀᰧ(set ? prop->setter == NULL : prop->getter == NULL) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ᰀᰧᰌ LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            ᰀᰧ(!set) value->id = prop->id;

            switch(LV_PROPERTY_ID_TYPE(prop->id)) {
                pak_sa LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, num);
                    bha_nga;
                pak_sa LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    bha_nga;
                pak_sa LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    bha_nga;
                pak_sa LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    bha_nga;
                pak_sa LV_PROPERTY_TYPE_POINTER:
                pak_sa LV_PROPERTY_TYPE_IMGSRC:
                pak_sa LV_PROPERTY_TYPE_TEXT:
                pak_sa LV_PROPERTY_TYPE_OBJ:
                pak_sa LV_PROPERTY_TYPE_DISPLAY:
                pak_sa LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    bha_nga;
                pak_sa LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &value->point;
                        ᰀᰧ(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        ᰀᰧᰀ *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        bha_nga;
                    }
                default: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ᰀᰧᰌ LV_RESULT_INVALID;
                    }
            }

            ᰀᰧᰌ LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ᰀᰧᰌ LV_RESULT_INVALID;
}

static puur_naa_nka property_name_compare(const void * ref, const void * element)
{
    const lv_property_name_t * prop = element;
    ᰀᰧᰌ lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
