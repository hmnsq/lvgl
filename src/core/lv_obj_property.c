/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "../lvgl_public.h"

#എങ്കിൽ LV_USE_OBJ_PROPERTY

#അന്തര്ഭാവ "lv_obj_style_internal.h"
#അന്തര്ഭാവ "lv_obj_private.h"
#അന്തര്ഭാവ "../misc/lv_utils.h"
#അന്തര്ഭാവ "lv_obj_class_private.h"
#അന്തര്ഭാവ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    എങ്കിൽ(!set) { \
        മൂല്യം->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } അല്ലെങ്കിൽ { \
        തിരഞ്ഞെടുപ്പ്(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            അവസ്ഥ LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, മൂല്യം->field); \
                നിർത്തുക; \
            അവസ്ഥ LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, മൂല്യം->arg1.field, മൂല്യം->arg2.അക്കം); \
                നിർത്തുക; \
            അവസ്ഥ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, മൂല്യം->arg1.field, മൂല്യം->arg2.enable); \
                നിർത്തുക; \
            അവസ്ഥ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, മൂല്യം->arg1.field, മൂല്യം->arg2.precise); \
                നിർത്തുക; \
            അവസ്ഥ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, മൂല്യം->arg1.field, മൂല്യം->arg2.color); \
                നിർത്തുക; \
            അവസ്ഥ LV_PROPERTY_TYPE_POINTER: \
            അവസ്ഥ LV_PROPERTY_TYPE_IMGSRC: \
            അവസ്ഥ LV_PROPERTY_TYPE_TEXT: \
            അവസ്ഥ LV_PROPERTY_TYPE_OBJ: \
            അവസ്ഥ LV_PROPERTY_TYPE_DISPLAY: \
            അവസ്ഥ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, മൂല്യം->arg1.field, മൂല്യം->arg2.ptr); \
                നിർത്തുക; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

തരനിർവചനം int32_t integer;
തരനിർവചനം bool boolean;
തരനിർവചനം lv_മൂല്യം_precise_t precise;
തരനിർവചനം lv_color_t color;
തരനിർവചനം സ്ഥിരം ശൂന്യം * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_മൂല്യം_precise_t); \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    തരനിർവചനം ശൂന്യം (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, സ്ഥിരം ശൂന്യം *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

തരനിർവചനം ശൂന്യം (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
തരനിർവചനം lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, സ്ഥിരം lv_property_t *);

തരനിർവചനം integer(*lv_property_get_integer_t)(സ്ഥിരം lv_obj_t *);
തരനിർവചനം bool (*lv_property_get_boolean_t)(സ്ഥിരം lv_obj_t *);
തരനിർവചനം lv_മൂല്യം_precise_t (*lv_property_get_precise_t)(സ്ഥിരം lv_obj_t *);
തരനിർവചനം lv_color_t (*lv_property_get_color_t)(സ്ഥിരം lv_obj_t *);
തരനിർവചനം ശൂന്യം * (*lv_property_get_pointer_t)(സ്ഥിരം lv_obj_t *);
തരനിർവചനം lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

തരനിർവചനം lv_result_t (*lv_property_getter_t)(സ്ഥിരം lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

സ്ഥിതം lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * മൂല്യം, bool set);
സ്ഥിതം പൂർണ്ണസംഖ്യ property_name_compare(സ്ഥിരം ശൂന്യം * ref, സ്ഥിരം ശൂന്യം * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, സ്ഥിരം lv_property_t * മൂല്യം)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ LV_RESULT_INVALID);
    LV_CHECK_ARG(മൂല്യം != ശൂന്യ, തിരികെ LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(മൂല്യം->id);
    എങ്കിൽ(മൂല്യം->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        തിരികെ LV_RESULT_INVALID;
    }

    എങ്കിൽ(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, മൂല്യം->style, മൂല്യം->selector);
        തിരികെ LV_RESULT_OK;
    }

    തിരികെ obj_property(obj, മൂല്യം->id, (lv_property_t *)മൂല്യം, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, സ്ഥിരം lv_property_t * മൂല്യം, uint32_t എണ്ണം)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ LV_RESULT_INVALID);
    LV_CHECK_ARG(മൂല്യം != ശൂന്യ, തിരികെ LV_RESULT_INVALID);

    വേണ്ടി(uint32_t i = 0; i < എണ്ണം; i++) {
        lv_result_t result = lv_obj_set_property(obj, &മൂല്യം[i]);
        എങ്കിൽ(result != LV_RESULT_OK) {
            തിരികെ result;
        }
    }

    തിരികെ LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t മൂല്യം = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    എങ്കിൽ(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        മൂല്യം.id = LV_PROPERTY_ID_INVALID;
        മൂല്യം.അക്കം = 0;
        തിരികെ മൂല്യം;
    }

    എങ്കിൽ(index < LV_PROPERTY_ID_START) {
        മൂല്യം.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        മൂല്യം.id = id;
        മൂല്യം.selector = LV_PART_MAIN | obj->state;
        തിരികെ മൂല്യം;
    }

    result = obj_property(obj, id, &മൂല്യം, false);
    എങ്കിൽ(result != LV_RESULT_OK)
        മൂല്യം.id = LV_PROPERTY_ID_INVALID;

    തിരികെ മൂല്യം;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t മൂല്യം;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    എങ്കിൽ(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        മൂല്യം.id = LV_PROPERTY_ID_INVALID;
        മൂല്യം.അക്കം = 0;
        തിരികെ മൂല്യം;
    }

    മൂല്യം.style = lv_obj_get_style_prop(obj, part, index);
    മൂല്യം.id = id;
    മൂല്യം.selector = part | obj->state;
    തിരികെ മൂല്യം;
}

lv_prop_id_t lv_style_property_get_id(സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_ARG(name != ശൂന്യ, തിരികെ LV_PROPERTY_ID_INVALID);
#എങ്കിൽ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, വലുപ്പം(lv_style_property_names) / വലുപ്പം(lv_property_name_t),
                             വലുപ്പം(lv_property_name_t), property_name_compare);
    എങ്കിൽ(found) തിരികെ found->id;
#അല്ലെങ്കിൽ
    LV_UNUSED(name);
#endif
    തിരികെ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(സ്ഥിരം lv_obj_class_t * clz, സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_ARG(clz != ശൂന്യ, തിരികെ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ശൂന്യ, തിരികെ LV_PROPERTY_ID_INVALID);
#എങ്കിൽ LV_USE_OBJ_PROPERTY_NAME
    സ്ഥിരം lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    എങ്കിൽ(names == ശൂന്യ) {
        /* try base class*/
        തിരികെ LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_എണ്ണം, വലുപ്പം(lv_property_name_t), property_name_compare);
    എങ്കിൽ(found) തിരികെ found->id;
#അല്ലെങ്കിൽ
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    തിരികെ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(സ്ഥിരം lv_obj_t * obj, സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ശൂന്യ, തിരികെ LV_PROPERTY_ID_INVALID);
#എങ്കിൽ LV_USE_OBJ_PROPERTY_NAME

    സ്ഥിരം lv_obj_class_t * clz;
    lv_prop_id_t id;

    വേണ്ടി(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        എങ്കിൽ(id != LV_PROPERTY_ID_INVALID) തിരികെ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    എങ്കിൽ(id != LV_PROPERTY_ID_INVALID) തിരികെ id;
#അല്ലെങ്കിൽ
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    തിരികെ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

സ്ഥിതം lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * മൂല്യം, bool set)
{
    സ്ഥിരം lv_property_ops_t * properties;
    സ്ഥിരം lv_property_ops_t * prop;

    സ്ഥിരം lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    വേണ്ടി(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        എങ്കിൽ(properties == ശൂന്യ) {
            /* try base class*/
            തുടരുക;
        }

        എങ്കിൽ(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            തുടരുക;
        }

        /*Check if there's setter available for this class*/
        വേണ്ടി(uint32_t i = 0; i < clz->properties_എണ്ണം; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            എങ്കിൽ(prop->id == LV_PROPERTY_ID_ANY) {
                മൂല്യം->id = prop->id;
                എങ്കിൽ(set) തിരികെ ((lv_property_setter_t)prop->setter)(obj, id, മൂല്യം);
                അല്ലെങ്കിൽ തിരികെ ((lv_property_getter_t)prop->getter)(obj, id, മൂല്യം);
            }

            /*Not this id, check next*/
            എങ്കിൽ(prop->id != id)
                തുടരുക;

            /*id matched but we got null pointer to functions*/
            എങ്കിൽ(set ? prop->setter == ശൂന്യ : prop->getter == ശൂന്യ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                തിരികെ LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            എങ്കിൽ(!set) മൂല്യം->id = prop->id;

            തിരഞ്ഞെടുപ്പ്(LV_PROPERTY_ID_TYPE(prop->id)) {
                അവസ്ഥ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, അക്കം);
                    നിർത്തുക;
                അവസ്ഥ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    നിർത്തുക;
                അവസ്ഥ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    നിർത്തുക;
                അവസ്ഥ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    നിർത്തുക;
                അവസ്ഥ LV_PROPERTY_TYPE_POINTER:
                അവസ്ഥ LV_PROPERTY_TYPE_IMGSRC:
                അവസ്ഥ LV_PROPERTY_TYPE_TEXT:
                അവസ്ഥ LV_PROPERTY_TYPE_OBJ:
                അവസ്ഥ LV_PROPERTY_TYPE_DISPLAY:
                അവസ്ഥ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    നിർത്തുക;
                അവസ്ഥ LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &മൂല്യം->point;
                        എങ്കിൽ(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        അല്ലെങ്കിൽ *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        നിർത്തുക;
                    }
                സ്ഥിരസ്ഥിതി: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        തിരികെ LV_RESULT_INVALID;
                    }
            }

            തിരികെ LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    തിരികെ LV_RESULT_INVALID;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ property_name_compare(സ്ഥിരം ശൂന്യം * ref, സ്ഥിരം ശൂന്യം * element)
{
    സ്ഥിരം lv_property_name_t * prop = element;
    തിരികെ lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
