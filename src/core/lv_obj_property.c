/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#அந்தர்பாவ "../lvgl_public.h"

#எனில் LV_USE_OBJ_PROPERTY

#அந்தர்பாவ "lv_obj_style_internal.h"
#அந்தர்பாவ "lv_obj_private.h"
#அந்தர்பாவ "../misc/lv_utils.h"
#அந்தர்பாவ "lv_obj_class_private.h"
#அந்தர்பாவ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    எனில்(!set) { \
        மதிப்பு->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } இல்லையெனில் { \
        தேர்வு(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            நிலை LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, மதிப்பு->field); \
                நிறுத்து; \
            நிலை LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, மதிப்பு->arg1.field, மதிப்பு->arg2.இலக்கம்); \
                நிறுத்து; \
            நிலை LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, மதிப்பு->arg1.field, மதிப்பு->arg2.enable); \
                நிறுத்து; \
            நிலை LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, மதிப்பு->arg1.field, மதிப்பு->arg2.precise); \
                நிறுத்து; \
            நிலை LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, மதிப்பு->arg1.field, மதிப்பு->arg2.color); \
                நிறுத்து; \
            நிலை LV_PROPERTY_TYPE_POINTER: \
            நிலை LV_PROPERTY_TYPE_IMGSRC: \
            நிலை LV_PROPERTY_TYPE_TEXT: \
            நிலை LV_PROPERTY_TYPE_OBJ: \
            நிலை LV_PROPERTY_TYPE_DISPLAY: \
            நிலை LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, மதிப்பு->arg1.field, மதிப்பு->arg2.ptr); \
                நிறுத்து; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

வகைவரை int32_t integer;
வகைவரை bool boolean;
வகைவரை lv_மதிப்பு_precise_t precise;
வகைவரை lv_color_t color;
வகைவரை மாறிலி வெறுமை * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    வகைவரை வெறுமை (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    வகைவரை வெறுமை (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    வகைவரை வெறுமை (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    வகைவரை வெறுமை (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_மதிப்பு_precise_t); \
    வகைவரை வெறுமை (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    வகைவரை வெறுமை (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, மாறிலி வெறுமை *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

வகைவரை வெறுமை (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
வகைவரை lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, மாறிலி lv_property_t *);

வகைவரை integer(*lv_property_get_integer_t)(மாறிலி lv_obj_t *);
வகைவரை bool (*lv_property_get_boolean_t)(மாறிலி lv_obj_t *);
வகைவரை lv_மதிப்பு_precise_t (*lv_property_get_precise_t)(மாறிலி lv_obj_t *);
வகைவரை lv_color_t (*lv_property_get_color_t)(மாறிலி lv_obj_t *);
வகைவரை வெறுமை * (*lv_property_get_pointer_t)(மாறிலி lv_obj_t *);
வகைவரை lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

வகைவரை lv_result_t (*lv_property_getter_t)(மாறிலி lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

நிலையான lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * மதிப்பு, bool set);
நிலையான முழு property_name_compare(மாறிலி வெறுமை * ref, மாறிலி வெறுமை * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, மாறிலி lv_property_t * மதிப்பு)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு LV_RESULT_INVALID);
    LV_CHECK_ARG(மதிப்பு != வெற்று, திரும்பு LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(மதிப்பு->id);
    எனில்(மதிப்பு->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        திரும்பு LV_RESULT_INVALID;
    }

    எனில்(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, மதிப்பு->style, மதிப்பு->selector);
        திரும்பு LV_RESULT_OK;
    }

    திரும்பு obj_property(obj, மதிப்பு->id, (lv_property_t *)மதிப்பு, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, மாறிலி lv_property_t * மதிப்பு, uint32_t மொத்தம்)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு LV_RESULT_INVALID);
    LV_CHECK_ARG(மதிப்பு != வெற்று, திரும்பு LV_RESULT_INVALID);

    க்கு(uint32_t i = 0; i < மொத்தம்; i++) {
        lv_result_t result = lv_obj_set_property(obj, &மதிப்பு[i]);
        எனில்(result != LV_RESULT_OK) {
            திரும்பு result;
        }
    }

    திரும்பு LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t மதிப்பு = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    எனில்(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        மதிப்பு.id = LV_PROPERTY_ID_INVALID;
        மதிப்பு.இலக்கம் = 0;
        திரும்பு மதிப்பு;
    }

    எனில்(index < LV_PROPERTY_ID_START) {
        மதிப்பு.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        மதிப்பு.id = id;
        மதிப்பு.selector = LV_PART_MAIN | obj->state;
        திரும்பு மதிப்பு;
    }

    result = obj_property(obj, id, &மதிப்பு, false);
    எனில்(result != LV_RESULT_OK)
        மதிப்பு.id = LV_PROPERTY_ID_INVALID;

    திரும்பு மதிப்பு;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t மதிப்பு;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    எனில்(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        மதிப்பு.id = LV_PROPERTY_ID_INVALID;
        மதிப்பு.இலக்கம் = 0;
        திரும்பு மதிப்பு;
    }

    மதிப்பு.style = lv_obj_get_style_prop(obj, part, index);
    மதிப்பு.id = id;
    மதிப்பு.selector = part | obj->state;
    திரும்பு மதிப்பு;
}

lv_prop_id_t lv_style_property_get_id(மாறிலி எழுத்து * name)
{
    LV_CHECK_ARG(name != வெற்று, திரும்பு LV_PROPERTY_ID_INVALID);
#எனில் LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, அளவு(lv_style_property_names) / அளவு(lv_property_name_t),
                             அளவு(lv_property_name_t), property_name_compare);
    எனில்(found) திரும்பு found->id;
#இல்லையெனில்
    LV_UNUSED(name);
#endif
    திரும்பு LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(மாறிலி lv_obj_class_t * clz, மாறிலி எழுத்து * name)
{
    LV_CHECK_ARG(clz != வெற்று, திரும்பு LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != வெற்று, திரும்பு LV_PROPERTY_ID_INVALID);
#எனில் LV_USE_OBJ_PROPERTY_NAME
    மாறிலி lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    எனில்(names == வெற்று) {
        /* try base class*/
        திரும்பு LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_மொத்தம், அளவு(lv_property_name_t), property_name_compare);
    எனில்(found) திரும்பு found->id;
#இல்லையெனில்
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    திரும்பு LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(மாறிலி lv_obj_t * obj, மாறிலி எழுத்து * name)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != வெற்று, திரும்பு LV_PROPERTY_ID_INVALID);
#எனில் LV_USE_OBJ_PROPERTY_NAME

    மாறிலி lv_obj_class_t * clz;
    lv_prop_id_t id;

    க்கு(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        எனில்(id != LV_PROPERTY_ID_INVALID) திரும்பு id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    எனில்(id != LV_PROPERTY_ID_INVALID) திரும்பு id;
#இல்லையெனில்
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    திரும்பு LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

நிலையான lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * மதிப்பு, bool set)
{
    மாறிலி lv_property_ops_t * properties;
    மாறிலி lv_property_ops_t * prop;

    மாறிலி lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    க்கு(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        எனில்(properties == வெற்று) {
            /* try base class*/
            தொடர்;
        }

        எனில்(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            தொடர்;
        }

        /*Check if there's setter available for this class*/
        க்கு(uint32_t i = 0; i < clz->properties_மொத்தம்; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            எனில்(prop->id == LV_PROPERTY_ID_ANY) {
                மதிப்பு->id = prop->id;
                எனில்(set) திரும்பு ((lv_property_setter_t)prop->setter)(obj, id, மதிப்பு);
                இல்லையெனில் திரும்பு ((lv_property_getter_t)prop->getter)(obj, id, மதிப்பு);
            }

            /*Not this id, check next*/
            எனில்(prop->id != id)
                தொடர்;

            /*id matched but we got null pointer to functions*/
            எனில்(set ? prop->setter == வெற்று : prop->getter == வெற்று) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                திரும்பு LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            எனில்(!set) மதிப்பு->id = prop->id;

            தேர்வு(LV_PROPERTY_ID_TYPE(prop->id)) {
                நிலை LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, இலக்கம்);
                    நிறுத்து;
                நிலை LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    நிறுத்து;
                நிலை LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    நிறுத்து;
                நிலை LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    நிறுத்து;
                நிலை LV_PROPERTY_TYPE_POINTER:
                நிலை LV_PROPERTY_TYPE_IMGSRC:
                நிலை LV_PROPERTY_TYPE_TEXT:
                நிலை LV_PROPERTY_TYPE_OBJ:
                நிலை LV_PROPERTY_TYPE_DISPLAY:
                நிலை LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    நிறுத்து;
                நிலை LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &மதிப்பு->point;
                        எனில்(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        இல்லையெனில் *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        நிறுத்து;
                    }
                இயல்பு: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        திரும்பு LV_RESULT_INVALID;
                    }
            }

            திரும்பு LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    திரும்பு LV_RESULT_INVALID;
}

நிலையான முழு property_name_compare(மாறிலி வெறுமை * ref, மாறிலி வெறுமை * element)
{
    மாறிலி lv_property_name_t * prop = element;
    திரும்பு lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
