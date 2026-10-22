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

#परिभाषय HANDLE_PROPERTY_TYPE(type, क्षेत्रम्) \
    यदि(!समुच्चयः) { \
        मानम्->क्षेत्रम् = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } अन्यथा { \
        विकल्प(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            पक्ष LV_PROPERTY_ID_INVALID: \
                ((lv_property_समुच्चयः##type##_t)(prop->setter))(obj, मानम्->क्षेत्रम्); \
                भङ्ग; \
            पक्ष LV_PROPERTY_TYPE_INT: \
                ((lv_property_समुच्चयः##type##_integer_t)(prop->setter))(obj, मानम्->arg1.क्षेत्रम्, मानम्->arg2.अङ्कः); \
                भङ्ग; \
            पक्ष LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_समुच्चयः##type##_boolean_t)(prop->setter))(obj, मानम्->arg1.क्षेत्रम्, मानम्->arg2.enable); \
                भङ्ग; \
            पक्ष LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_समुच्चयः##type##_precise_t)(prop->setter))(obj, मानम्->arg1.क्षेत्रम्, मानम्->arg2.precise); \
                भङ्ग; \
            पक्ष LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_समुच्चयः##type##_color_t)(prop->setter))(obj, मानम्->arg1.क्षेत्रम्, मानम्->arg2.color); \
                भङ्ग; \
            पक्ष LV_PROPERTY_TYPE_POINTER: \
            पक्ष LV_PROPERTY_TYPE_IMGSRC: \
            पक्ष LV_PROPERTY_TYPE_TEXT: \
            पक्ष LV_PROPERTY_TYPE_OBJ: \
            पक्ष LV_PROPERTY_TYPE_DISPLAY: \
            पक्ष LV_PROPERTY_TYPE_FONT: \
                ((lv_property_समुच्चयः##type##_pointer_t)(prop->setter))(obj, मानम्->arg1.क्षेत्रम्, मानम्->arg2.ptr); \
                भङ्ग; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

प्रकारनिर्देश int32_t integer;
प्रकारनिर्देश bool boolean;
प्रकारनिर्देश lv_मानम्_precise_t precise;
प्रकारनिर्देश lv_color_t color;
प्रकारनिर्देश ध्रुव शून्य * pointer;

#परिभाषय DEFINE_PROPERTY_SETTER_TYPES(type) \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_t)(lv_obj_t *, type); \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_integer_t)(lv_obj_t *, type, int32_t); \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_boolean_t)(lv_obj_t *, type, bool); \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_precise_t)(lv_obj_t *, type, lv_मानम्_precise_t); \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    प्रकारनिर्देश शून्य (*lv_property_समुच्चयः##type##_pointer_t)(lv_obj_t *, type, ध्रुव शून्य *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

प्रकारनिर्देश शून्य (*lv_property_समुच्चयः_point_t)(lv_obj_t *, lv_point_t *);
प्रकारनिर्देश lv_परिणाम_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ध्रुव lv_property_t *);

प्रकारनिर्देश integer(*lv_property_get_integer_t)(ध्रुव lv_obj_t *);
प्रकारनिर्देश bool (*lv_property_get_boolean_t)(ध्रुव lv_obj_t *);
प्रकारनिर्देश lv_मानम्_precise_t (*lv_property_get_precise_t)(ध्रुव lv_obj_t *);
प्रकारनिर्देश lv_color_t (*lv_property_get_color_t)(ध्रुव lv_obj_t *);
प्रकारनिर्देश शून्य * (*lv_property_get_pointer_t)(ध्रुव lv_obj_t *);
प्रकारनिर्देश lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

प्रकारनिर्देश lv_परिणाम_t (*lv_property_getter_t)(ध्रुव lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर lv_परिणाम_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मानम्, bool समुच्चयः);
स्थिर पूर्णाङ्क property_name_compare(ध्रुव शून्य * ref, ध्रुव शून्य * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_परिणाम_t lv_obj_समुच्चयः_property(lv_obj_t * obj, ध्रुव lv_property_t * मानम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_परिणाम_INVALID);
    LV_CHECK_ARG(मानम् != रिक्त, प्रत्यागम LV_परिणाम_INVALID);

    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(मानम्->id);
    यदि(मानम्->id == LV_PROPERTY_ID_INVALID || सूचकांक > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        प्रत्यागम LV_परिणाम_INVALID;
    }

    यदि(सूचकांक < LV_PROPERTY_ID_START) {
        lv_obj_समुच्चयः_local_style_prop(obj, सूचकांक, मानम्->style, मानम्->selector);
        प्रत्यागम LV_परिणाम_OK;
    }

    प्रत्यागम obj_property(obj, मानम्->id, (lv_property_t *)मानम्, true);
}

lv_परिणाम_t lv_obj_समुच्चयः_properties(lv_obj_t * obj, ध्रुव lv_property_t * मानम्, uint32_t गणनम्)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_परिणाम_INVALID);
    LV_CHECK_ARG(मानम् != रिक्त, प्रत्यागम LV_परिणाम_INVALID);

    प्रति(uint32_t i = 0; i < गणनम्; i++) {
        lv_परिणाम_t परिणाम = lv_obj_समुच्चयः_property(obj, &मानम्[i]);
        यदि(परिणाम != LV_परिणाम_OK) {
            प्रत्यागम परिणाम;
        }
    }

    प्रत्यागम LV_परिणाम_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_परिणाम_t परिणाम;
    lv_property_t मानम् = { 0 };

    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);
    यदि(id == LV_PROPERTY_ID_INVALID || सूचकांक > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        मानम्.id = LV_PROPERTY_ID_INVALID;
        मानम्.अङ्कः = 0;
        प्रत्यागम मानम्;
    }

    यदि(सूचकांक < LV_PROPERTY_ID_START) {
        मानम्.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, सूचकांक);
        मानम्.id = id;
        मानम्.selector = LV_PART_MAIN | obj->state;
        प्रत्यागम मानम्;
    }

    परिणाम = obj_property(obj, id, &मानम्, false);
    यदि(परिणाम != LV_परिणाम_OK)
        मानम्.id = LV_PROPERTY_ID_INVALID;

    प्रत्यागम मानम्;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t मानम्;
    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);

    यदि(सूचकांक == LV_PROPERTY_ID_INVALID || सूचकांक >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        मानम्.id = LV_PROPERTY_ID_INVALID;
        मानम्.अङ्कः = 0;
        प्रत्यागम मानम्;
    }

    मानम्.style = lv_obj_get_style_prop(obj, part, सूचकांक);
    मानम्.id = id;
    मानम्.selector = part | obj->state;
    प्रत्यागम मानम्;
}

lv_prop_id_t lv_style_property_get_id(ध्रुव वर्ण * name)
{
    LV_CHECK_ARG(name != रिक्त, प्रत्यागम LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * प्राप्तम्;
    /*Check style property*/
    प्राप्तम् = lv_utils_bsearch(name, lv_style_property_नामानि, परिमाण(lv_style_property_नामानि) / परिमाण(lv_property_name_t),
                             परिमाण(lv_property_name_t), property_name_compare);
    यदि(प्राप्तम्) प्रत्यागम प्राप्तम्->id;
#अन्यथा
    LV_अप्रयुक्तम्(name);
#यदि_अन्तः
    प्रत्यागम LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ध्रुव lv_obj_class_t * clz, ध्रुव वर्ण * name)
{
    LV_CHECK_ARG(clz != रिक्त, प्रत्यागम LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, प्रत्यागम LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME
    ध्रुव lv_property_name_t * नामानि;
    lv_property_name_t * प्राप्तम्;

    नामानि = clz->property_नामानि;
    यदि(नामानि == रिक्त) {
        /* try base class*/
        प्रत्यागम LV_PROPERTY_ID_INVALID;
    }

    प्राप्तम् = lv_utils_bsearch(name, नामानि, clz->नामानि_गणनम्, परिमाण(lv_property_name_t), property_name_compare);
    यदि(प्राप्तम्) प्रत्यागम प्राप्तम्->id;
#अन्यथा
    LV_अप्रयुक्तम्(clz);
    LV_अप्रयुक्तम्(name);
    LV_अप्रयुक्तम्(property_name_compare);
#यदि_अन्तः
    प्रत्यागम LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ध्रुव lv_obj_t * obj, ध्रुव वर्ण * name)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, प्रत्यागम LV_PROPERTY_ID_INVALID);
#यदि LV_USE_OBJ_PROPERTY_NAME

    ध्रुव lv_obj_class_t * clz;
    lv_prop_id_t id;

    प्रति(clz = obj->class_p; clz; clz = clz->आधारः_class) {
        id = lv_obj_class_property_get_id(clz, name);
        यदि(id != LV_PROPERTY_ID_INVALID) प्रत्यागम id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    यदि(id != LV_PROPERTY_ID_INVALID) प्रत्यागम id;
#अन्यथा
    LV_अप्रयुक्तम्(obj);
    LV_अप्रयुक्तम्(name);
    LV_अप्रयुक्तम्(property_name_compare);
#यदि_अन्तः
    प्रत्यागम LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

स्थिर lv_परिणाम_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मानम्, bool समुच्चयः)
{
    ध्रुव lv_property_ops_t * properties;
    ध्रुव lv_property_ops_t * prop;

    ध्रुव lv_obj_class_t * clz;
    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);

    प्रति(clz = obj->class_p ; clz; clz = clz->आधारः_class) {
        properties = clz->properties;
        यदि(properties == रिक्त) {
            /* try base class*/
            अनुवर्त;
        }

        यदि(id != LV_PROPERTY_ID_ANY && (सूचकांक < clz->prop_सूचकांक_start || सूचकांक > clz->prop_सूचकांक_अन्तः)) {
            /* try base class*/
            अनुवर्त;
        }

        /*Check if there's setter available for this class*/
        प्रति(uint32_t i = 0; i < clz->properties_गणनम्; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            यदि(prop->id == LV_PROPERTY_ID_ANY) {
                मानम्->id = prop->id;
                यदि(समुच्चयः) प्रत्यागम ((lv_property_setter_t)prop->setter)(obj, id, मानम्);
                अन्यथा प्रत्यागम ((lv_property_getter_t)prop->getter)(obj, id, मानम्);
            }

            /*Not this id, check next*/
            यदि(prop->id != id)
                अनुवर्त;

            /*id matched but we got null pointer to functions*/
            यदि(समुच्चयः ? prop->setter == रिक्त : prop->getter == रिक्त) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, समुच्चयः ? "setter" : "getter", id);
                प्रत्यागम LV_परिणाम_INVALID;
            }

            /*Update value id if it's a read*/
            यदि(!समुच्चयः) मानम्->id = prop->id;

            विकल्प(LV_PROPERTY_ID_TYPE(prop->id)) {
                पक्ष LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, अङ्कः);
                    भङ्ग;
                पक्ष LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    भङ्ग;
                पक्ष LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    भङ्ग;
                पक्ष LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    भङ्ग;
                पक्ष LV_PROPERTY_TYPE_POINTER:
                पक्ष LV_PROPERTY_TYPE_IMGSRC:
                पक्ष LV_PROPERTY_TYPE_TEXT:
                पक्ष LV_PROPERTY_TYPE_OBJ:
                पक्ष LV_PROPERTY_TYPE_DISPLAY:
                पक्ष LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    भङ्ग;
                पक्ष LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &मानम्->point;
                        यदि(समुच्चयः)((lv_property_समुच्चयः_point_t)(prop->setter))(obj, point);
                        अन्यथा *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        भङ्ग;
                    }
                स्वभाव: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        प्रत्यागम LV_परिणाम_INVALID;
                    }
            }

            प्रत्यागम LV_परिणाम_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    प्रत्यागम LV_परिणाम_INVALID;
}

स्थिर पूर्णाङ्क property_name_compare(ध्रुव शून्य * ref, ध्रुव शून्य * element)
{
    ध्रुव lv_property_name_t * prop = element;
    प्रत्यागम lv_strcmp(ref, prop->name);
}

#यदि_अन्तः /*LV_USE_OBJ_PROPERTY*/
