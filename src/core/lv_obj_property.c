/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl_public.h"

#གལ་ཏེ LV_USE_OBJ_PROPERTY

#include "lv_obj_style_internal.h"
#include "lv_obj_private.h"
#include "../misc/lv_utils.h"
#include "lv_obj_class_private.h"
#include "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    གལ་ཏེ(!set) { \
        value->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } དེ་མིན { \
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
static ཧྲིལ property_name_compare(const void * ref, const void * element);

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
    LV_CHECK_ARG(obj != NULL, ཕྱིར LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, ཕྱིར LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(value->id);
    གལ་ཏེ(value->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ཕྱིར LV_RESULT_INVALID;
    }

    གལ་ཏེ(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, value->style, value->selector);
        ཕྱིར LV_RESULT_OK;
    }

    ཕྱིར obj_property(obj, value->id, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)
{
    LV_CHECK_ARG(obj != NULL, ཕྱིར LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, ཕྱིར LV_RESULT_INVALID);

    ཆེད་དུ(uint32_t i = 0; i < count; i++) {
        lv_result_t result = lv_obj_set_property(obj, &value[i]);
        གལ་ཏེ(result != LV_RESULT_OK) {
            ཕྱིར result;
        }
    }

    ཕྱིར LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != NULL, ཕྱིར (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t value = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    གལ་ཏེ(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        ཕྱིར value;
    }

    གལ་ཏེ(index < LV_PROPERTY_ID_START) {
        value.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        value.id = id;
        value.selector = LV_PART_MAIN | obj->state;
        ཕྱིར value;
    }

    result = obj_property(obj, id, &value, false);
    གལ་ཏེ(result != LV_RESULT_OK)
        value.id = LV_PROPERTY_ID_INVALID;

    ཕྱིར value;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != NULL, ཕྱིར (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t value;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    གལ་ཏེ(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        ཕྱིར value;
    }

    value.style = lv_obj_get_style_prop(obj, part, index);
    value.id = id;
    value.selector = part | obj->state;
    ཕྱིར value;
}

lv_prop_id_t lv_style_property_get_id(const var_na * name)
{
    LV_CHECK_ARG(name != NULL, ཕྱིར LV_PROPERTY_ID_INVALID);
#གལ་ཏེ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, parimaa_na(lv_style_property_names) / parimaa_na(lv_property_name_t),
                             parimaa_na(lv_property_name_t), property_name_compare);
    གལ་ཏེ(found) ཕྱིར found->id;
#དེ་མིན
    LV_UNUSED(name);
#endif
    ཕྱིར LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(const lv_obj_class_t * clz, const var_na * name)
{
    LV_CHECK_ARG(clz != NULL, ཕྱིར LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, ཕྱིར LV_PROPERTY_ID_INVALID);
#གལ་ཏེ LV_USE_OBJ_PROPERTY_NAME
    const lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    གལ་ཏེ(names == NULL) {
        /* try base class*/
        ཕྱིར LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_count, parimaa_na(lv_property_name_t), property_name_compare);
    གལ་ཏེ(found) ཕྱིར found->id;
#དེ་མིན
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ཕྱིར LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(const lv_obj_t * obj, const var_na * name)
{
    LV_CHECK_ARG(obj != NULL, ཕྱིར LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, ཕྱིར LV_PROPERTY_ID_INVALID);
#གལ་ཏེ LV_USE_OBJ_PROPERTY_NAME

    const lv_obj_class_t * clz;
    lv_prop_id_t id;

    ཆེད་དུ(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        གལ་ཏེ(id != LV_PROPERTY_ID_INVALID) ཕྱིར id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    གལ་ཏེ(id != LV_PROPERTY_ID_INVALID) ཕྱིར id;
#དེ་མིན
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ཕྱིར LV_PROPERTY_ID_INVALID;
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

    ཆེད་དུ(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        གལ་ཏེ(properties == NULL) {
            /* try base class*/
            continue;
        }

        གལ་ཏེ(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            continue;
        }

        /*Check if there's setter available for this class*/
        ཆེད་དུ(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            གལ་ཏེ(prop->id == LV_PROPERTY_ID_ANY) {
                value->id = prop->id;
                གལ་ཏེ(set) ཕྱིར ((lv_property_setter_t)prop->setter)(obj, id, value);
                དེ་མིན ཕྱིར ((lv_property_getter_t)prop->getter)(obj, id, value);
            }

            /*Not this id, check next*/
            གལ་ཏེ(prop->id != id)
                continue;

            /*id matched but we got null pointer to functions*/
            གལ་ཏེ(set ? prop->setter == NULL : prop->getter == NULL) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ཕྱིར LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            གལ་ཏེ(!set) value->id = prop->id;

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
                        གལ་ཏེ(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        དེ་མིན *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        bha_nga;
                    }
                default: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ཕྱིར LV_RESULT_INVALID;
                    }
            }

            ཕྱིར LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ཕྱིར LV_RESULT_INVALID;
}

static ཧྲིལ property_name_compare(const void * ref, const void * element)
{
    const lv_property_name_t * prop = element;
    ཕྱིར lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
