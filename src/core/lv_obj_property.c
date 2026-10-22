/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../lvgl_public.h"

#यदि LV_USE_OBJ_PROPERTY

#अन्तर्भाव "lv_obj_style_internal.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../misc/lv_utils.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    यदि(!set) { \
        मान->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } अन्यथा { \
        विकल्प(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            पक्ष LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, मान->field); \
                रोक; \
            पक्ष LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, मान->arg1.field, मान->arg2.अङ्क); \
                रोक; \
            पक्ष LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, मान->arg1.field, मान->arg2.enable); \
                रोक; \
            पक्ष LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, मान->arg1.field, मान->arg2.precise); \
                रोक; \
            पक्ष LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, मान->arg1.field, मान->arg2.color); \
                रोक; \
            पक्ष LV_PROPERTY_TYPE_POINTER: \
            पक्ष LV_PROPERTY_TYPE_IMGSRC: \
            पक्ष LV_PROPERTY_TYPE_TEXT: \
            पक्ष LV_PROPERTY_TYPE_OBJ: \
            पक्ष LV_PROPERTY_TYPE_DISPLAY: \
            पक्ष LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, मान->arg1.field, मान->arg2.ptr); \
                रोक; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

प्रकारनिर्देश int32_t integer;
प्रकारनिर्देश bool boolean;
प्रकारनिर्देश lv_मान_precise_t precise;
प्रकारनिर्देश lv_color_t color;
प्रकारनिर्देश अचर शून्य * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_मान_precise_t); \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    प्रकारनिर्देश शून्य (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, अचर शून्य *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

प्रकारनिर्देश शून्य (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
प्रकारनिर्देश lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, अचर lv_property_t *);

प्रकारनिर्देश integer(*lv_property_get_integer_t)(अचर lv_obj_t *);
प्रकारनिर्देश bool (*lv_property_get_boolean_t)(अचर lv_obj_t *);
प्रकारनिर्देश lv_मान_precise_t (*lv_property_get_precise_t)(अचर lv_obj_t *);
प्रकारनिर्देश lv_color_t (*lv_property_get_color_t)(अचर lv_obj_t *);
प्रकारनिर्देश शून्य * (*lv_property_get_pointer_t)(अचर lv_obj_t *);
प्रकारनिर्देश lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

प्रकारनिर्देश lv_result_t (*lv_property_getter_t)(अचर lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मान, bool set);
स्थिर पूर्णाङ्क property_name_compare(अचर शून्य * ref, अचर शून्य * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, अचर lv_property_t * मान)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ LV_RESULT_INVALID);
    LV_CHECK_ARG(मान != रिक्त, फर्काऊ LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(मान->id);
    यदि(मान->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        फर्काऊ LV_RESULT_INVALID;
    }

    यदि(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, मान->style, मान->selector);
        फर्काऊ LV_RESULT_OK;
    }

    फर्काऊ obj_property(obj, मान->id, (lv_property_t *)मान, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, अचर lv_property_t * मान, uint32_t गन्ती)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ LV_RESULT_INVALID);
    LV_CHECK_ARG(मान != रिक्त, फर्काऊ LV_RESULT_INVALID);

    लागि(uint32_t i = 0; i < गन्ती; i++) {
        lv_result_t result = lv_obj_set_property(obj, &मान[i]);
        यदि(result != LV_RESULT_OK) {
            फर्काऊ result;
        }
    }

    फर्काऊ LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t मान = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    यदि(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        मान.id = LV_PROPERTY_ID_INVALID;
        मान.अङ्क = 0;
        फर्काऊ मान;
    }

    यदि(index < LV_PROPERTY_ID_START) {
        मान.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        मान.id = id;
        मान.selector = LV_PART_MAIN | obj->state;
        फर्काऊ मान;
    }

    result = obj_property(obj, id, &मान, false);
    यदि(result != LV_RESULT_OK)
        मान.id = LV_PROPERTY_ID_INVALID;

    फर्काऊ मान;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t मान;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    यदि(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        मान.id = LV_PROPERTY_ID_INVALID;
        मान.अङ्क = 0;
        फर्काऊ मान;
    }

    मान.style = lv_obj_get_style_prop(obj, part, index);
    मान.id = id;
    मान.selector = part | obj->state;
    फर्काऊ मान;
}

lv_prop_id_t lv_style_property_get_id(अचर वर्ण * name)
{
    LV_CHECK_ARG(name != रिक्त, फर्काऊ LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, परिमाण(lv_style_property_names) / परिमाण(lv_property_name_t),
                             परिमाण(lv_property_name_t), property_name_compare);
    यदि(found) फर्काऊ found->id;
#अन्यथा
    LV_UNUSED(name);
#endif
    फर्काऊ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(अचर lv_obj_class_t * clz, अचर वर्ण * name)
{
    LV_CHECK_ARG(clz != रिक्त, फर्काऊ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, फर्काऊ LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME
    अचर lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    यदि(names == रिक्त) {
        /* try base class*/
        फर्काऊ LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_गन्ती, परिमाण(lv_property_name_t), property_name_compare);
    यदि(found) फर्काऊ found->id;
#अन्यथा
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    फर्काऊ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(अचर lv_obj_t * obj, अचर वर्ण * name)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, फर्काऊ LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME

    अचर lv_obj_class_t * clz;
    lv_prop_id_t id;

    लागि(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        यदि(id != LV_PROPERTY_ID_INVALID) फर्काऊ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    यदि(id != LV_PROPERTY_ID_INVALID) फर्काऊ id;
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    फर्काऊ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

स्थिर lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मान, bool set)
{
    अचर lv_property_ops_t * properties;
    अचर lv_property_ops_t * prop;

    अचर lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    लागि(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        यदि(properties == रिक्त) {
            /* try base class*/
            जारी;
        }

        यदि(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            जारी;
        }

        /*Check if there's setter available for this class*/
        लागि(uint32_t i = 0; i < clz->properties_गन्ती; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            यदि(prop->id == LV_PROPERTY_ID_ANY) {
                मान->id = prop->id;
                यदि(set) फर्काऊ ((lv_property_setter_t)prop->setter)(obj, id, मान);
                अन्यथा फर्काऊ ((lv_property_getter_t)prop->getter)(obj, id, मान);
            }

            /*Not this id, check next*/
            यदि(prop->id != id)
                जारी;

            /*id matched but we got null pointer to functions*/
            यदि(set ? prop->setter == रिक्त : prop->getter == रिक्त) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                फर्काऊ LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            यदि(!set) मान->id = prop->id;

            विकल्प(LV_PROPERTY_ID_TYPE(prop->id)) {
                पक्ष LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, अङ्क);
                    रोक;
                पक्ष LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    रोक;
                पक्ष LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    रोक;
                पक्ष LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    रोक;
                पक्ष LV_PROPERTY_TYPE_POINTER:
                पक्ष LV_PROPERTY_TYPE_IMGSRC:
                पक्ष LV_PROPERTY_TYPE_TEXT:
                पक्ष LV_PROPERTY_TYPE_OBJ:
                पक्ष LV_PROPERTY_TYPE_DISPLAY:
                पक्ष LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    रोक;
                पक्ष LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &मान->point;
                        यदि(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        अन्यथा *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        रोक;
                    }
                स्वभाव: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        फर्काऊ LV_RESULT_INVALID;
                    }
            }

            फर्काऊ LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    फर्काऊ LV_RESULT_INVALID;
}

स्थिर पूर्णाङ्क property_name_compare(अचर शून्य * ref, अचर शून्य * element)
{
    अचर lv_property_name_t * prop = element;
    फर्काऊ lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
