/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl_public.h"

#якщо LV_USE_OBJ_PROPERTY

#include "lv_obj_style_internal.h"
#include "lv_obj_private.h"
#include "../misc/lv_utils.h"
#include "lv_obj_class_private.h"
#include "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    якщо(!set) { \
        value->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } інакше { \
        switch(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            case LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, value->field); \
                break; \
            case LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, value->arg1.field, value->arg2.num); \
                break; \
            case LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, value->arg1.field, value->arg2.enable); \
                break; \
            case LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, value->arg1.field, value->arg2.precise); \
                break; \
            case LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, value->arg1.field, value->arg2.color); \
                break; \
            case LV_PROPERTY_TYPE_POINTER: \
            case LV_PROPERTY_TYPE_IMGSRC: \
            case LV_PROPERTY_TYPE_TEXT: \
            case LV_PROPERTY_TYPE_OBJ: \
            case LV_PROPERTY_TYPE_DISPLAY: \
            case LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, value->arg1.field, value->arg2.ptr); \
                break; \
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
static ціле property_name_compare(const void * ref, const void * element);

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
    LV_CHECK_ARG(obj != NULL, повернути LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, повернути LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(value->id);
    якщо(value->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        повернути LV_RESULT_INVALID;
    }

    якщо(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, value->style, value->selector);
        повернути LV_RESULT_OK;
    }

    повернути obj_property(obj, value->id, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, const lv_property_t * value, uint32_t count)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_RESULT_INVALID);
    LV_CHECK_ARG(value != NULL, повернути LV_RESULT_INVALID);

    для(uint32_t i = 0; i < count; i++) {
        lv_result_t result = lv_obj_set_property(obj, &value[i]);
        якщо(result != LV_RESULT_OK) {
            повернути result;
        }
    }

    повернути LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != NULL, повернути (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t value = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    якщо(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        повернути value;
    }

    якщо(index < LV_PROPERTY_ID_START) {
        value.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        value.id = id;
        value.selector = LV_PART_MAIN | obj->state;
        повернути value;
    }

    result = obj_property(obj, id, &value, false);
    якщо(result != LV_RESULT_OK)
        value.id = LV_PROPERTY_ID_INVALID;

    повернути value;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != NULL, повернути (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t value;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    якщо(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        повернути value;
    }

    value.style = lv_obj_get_style_prop(obj, part, index);
    value.id = id;
    value.selector = part | obj->state;
    повернути value;
}

lv_prop_id_t lv_style_property_get_id(const char * name)
{
    LV_CHECK_ARG(name != NULL, повернути LV_PROPERTY_ID_INVALID);
#якщо LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, sizeof(lv_style_property_names) / sizeof(lv_property_name_t),
                             sizeof(lv_property_name_t), property_name_compare);
    якщо(found) повернути found->id;
#інакше
    LV_UNUSED(name);
#endif
    повернути LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(const lv_obj_class_t * clz, const char * name)
{
    LV_CHECK_ARG(clz != NULL, повернути LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, повернути LV_PROPERTY_ID_INVALID);
#якщо LV_USE_OBJ_PROPERTY_NAME
    const lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    якщо(names == NULL) {
        /* try base class*/
        повернути LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_count, sizeof(lv_property_name_t), property_name_compare);
    якщо(found) повернути found->id;
#інакше
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    повернути LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(const lv_obj_t * obj, const char * name)
{
    LV_CHECK_ARG(obj != NULL, повернути LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != NULL, повернути LV_PROPERTY_ID_INVALID);
#якщо LV_USE_OBJ_PROPERTY_NAME

    const lv_obj_class_t * clz;
    lv_prop_id_t id;

    для(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        якщо(id != LV_PROPERTY_ID_INVALID) повернути id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    якщо(id != LV_PROPERTY_ID_INVALID) повернути id;
#інакше
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    повернути LV_PROPERTY_ID_INVALID;
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

    для(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        якщо(properties == NULL) {
            /* try base class*/
            continue;
        }

        якщо(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            continue;
        }

        /*Check if there's setter available for this class*/
        для(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            якщо(prop->id == LV_PROPERTY_ID_ANY) {
                value->id = prop->id;
                якщо(set) повернути ((lv_property_setter_t)prop->setter)(obj, id, value);
                інакше повернути ((lv_property_getter_t)prop->getter)(obj, id, value);
            }

            /*Not this id, check next*/
            якщо(prop->id != id)
                continue;

            /*id matched but we got null pointer to functions*/
            якщо(set ? prop->setter == NULL : prop->getter == NULL) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                повернути LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            якщо(!set) value->id = prop->id;

            switch(LV_PROPERTY_ID_TYPE(prop->id)) {
                case LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, num);
                    break;
                case LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    break;
                case LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    break;
                case LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    break;
                case LV_PROPERTY_TYPE_POINTER:
                case LV_PROPERTY_TYPE_IMGSRC:
                case LV_PROPERTY_TYPE_TEXT:
                case LV_PROPERTY_TYPE_OBJ:
                case LV_PROPERTY_TYPE_DISPLAY:
                case LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    break;
                case LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &value->point;
                        якщо(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        інакше *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        break;
                    }
                default: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        повернути LV_RESULT_INVALID;
                    }
            }

            повернути LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    повернути LV_RESULT_INVALID;
}

static ціле property_name_compare(const void * ref, const void * element)
{
    const lv_property_name_t * prop = element;
    повернути lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
