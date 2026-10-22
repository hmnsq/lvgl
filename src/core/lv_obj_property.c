/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "../lvgl_public.h"

#अगर LV_USE_OBJ_PROPERTY

#समावेश "lv_obj_style_internal.h"
#समावेश "lv_obj_private.h"
#समावेश "../misc/lv_utils.h"
#समावेश "lv_obj_class_private.h"
#समावेश "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#घोषणा HANDLE_PROPERTY_TYPE(type, field) \
    अगर(!set) { \
        मूल्य->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } अन्यथा { \
        चयन(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            हाल LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, मूल्य->field); \
                अवरोध; \
            हाल LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.अंक); \
                अवरोध; \
            हाल LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.enable); \
                अवरोध; \
            हाल LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.precise); \
                अवरोध; \
            हाल LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.color); \
                अवरोध; \
            हाल LV_PROPERTY_TYPE_POINTER: \
            हाल LV_PROPERTY_TYPE_IMGSRC: \
            हाल LV_PROPERTY_TYPE_TEXT: \
            हाल LV_PROPERTY_TYPE_OBJ: \
            हाल LV_PROPERTY_TYPE_DISPLAY: \
            हाल LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.ptr); \
                अवरोध; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

प्रकार int32_t integer;
प्रकार bool boolean;
प्रकार lv_मूल्य_precise_t precise;
प्रकार lv_color_t color;
प्रकार स्थिर व्योम * pointer;

#घोषणा DEFINE_PROPERTY_SETTER_TYPES(type) \
    प्रकार व्योम (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    प्रकार व्योम (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    प्रकार व्योम (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    प्रकार व्योम (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_मूल्य_precise_t); \
    प्रकार व्योम (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    प्रकार व्योम (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, स्थिर व्योम *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

प्रकार व्योम (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
प्रकार lv_परिणाम_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, स्थिर lv_property_t *);

प्रकार integer(*lv_property_get_integer_t)(स्थिर lv_obj_t *);
प्रकार bool (*lv_property_get_boolean_t)(स्थिर lv_obj_t *);
प्रकार lv_मूल्य_precise_t (*lv_property_get_precise_t)(स्थिर lv_obj_t *);
प्रकार lv_color_t (*lv_property_get_color_t)(स्थिर lv_obj_t *);
प्रकार व्योम * (*lv_property_get_pointer_t)(स्थिर lv_obj_t *);
प्रकार lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

प्रकार lv_परिणाम_t (*lv_property_getter_t)(स्थिर lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

अटल lv_परिणाम_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मूल्य, bool set);
अटल पूर्णांक property_name_compare(स्थिर व्योम * ref, स्थिर व्योम * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_परिणाम_t lv_obj_set_property(lv_obj_t * obj, स्थिर lv_property_t * मूल्य)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_परिणाम_INVALID);
    LV_CHECK_ARG(मूल्य != शून्य, वापस LV_परिणाम_INVALID);

    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(मूल्य->id);
    अगर(मूल्य->id == LV_PROPERTY_ID_INVALID || सूचकांक > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        वापस LV_परिणाम_INVALID;
    }

    अगर(सूचकांक < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, सूचकांक, मूल्य->style, मूल्य->selector);
        वापस LV_परिणाम_OK;
    }

    वापस obj_property(obj, मूल्य->id, (lv_property_t *)मूल्य, true);
}

lv_परिणाम_t lv_obj_set_properties(lv_obj_t * obj, स्थिर lv_property_t * मूल्य, uint32_t गिनती)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_परिणाम_INVALID);
    LV_CHECK_ARG(मूल्य != शून्य, वापस LV_परिणाम_INVALID);

    क्रम(uint32_t i = 0; i < गिनती; i++) {
        lv_परिणाम_t परिणाम = lv_obj_set_property(obj, &मूल्य[i]);
        अगर(परिणाम != LV_परिणाम_OK) {
            वापस परिणाम;
        }
    }

    वापस LV_परिणाम_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != शून्य, वापस (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_परिणाम_t परिणाम;
    lv_property_t मूल्य = { 0 };

    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);
    अगर(id == LV_PROPERTY_ID_INVALID || सूचकांक > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        मूल्य.id = LV_PROPERTY_ID_INVALID;
        मूल्य.अंक = 0;
        वापस मूल्य;
    }

    अगर(सूचकांक < LV_PROPERTY_ID_START) {
        मूल्य.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, सूचकांक);
        मूल्य.id = id;
        मूल्य.selector = LV_PART_MAIN | obj->state;
        वापस मूल्य;
    }

    परिणाम = obj_property(obj, id, &मूल्य, false);
    अगर(परिणाम != LV_परिणाम_OK)
        मूल्य.id = LV_PROPERTY_ID_INVALID;

    वापस मूल्य;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != शून्य, वापस (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t मूल्य;
    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);

    अगर(सूचकांक == LV_PROPERTY_ID_INVALID || सूचकांक >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        मूल्य.id = LV_PROPERTY_ID_INVALID;
        मूल्य.अंक = 0;
        वापस मूल्य;
    }

    मूल्य.style = lv_obj_get_style_prop(obj, part, सूचकांक);
    मूल्य.id = id;
    मूल्य.selector = part | obj->state;
    वापस मूल्य;
}

lv_prop_id_t lv_style_property_get_id(स्थिर अक्षर * name)
{
    LV_CHECK_ARG(name != शून्य, वापस LV_PROPERTY_ID_INVALID);
#अगर LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, माप(lv_style_property_names) / माप(lv_property_name_t),
                             माप(lv_property_name_t), property_name_compare);
    अगर(found) वापस found->id;
#अन्यथा
    LV_UNUSED(name);
#पूर्ण_अगर
    वापस LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(स्थिर lv_obj_class_t * clz, स्थिर अक्षर * name)
{
    LV_CHECK_ARG(clz != शून्य, वापस LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != शून्य, वापस LV_PROPERTY_ID_INVALID);
#अगर LV_USE_OBJ_PROPERTY_NAME
    स्थिर lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    अगर(names == शून्य) {
        /* try base class*/
        वापस LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_गिनती, माप(lv_property_name_t), property_name_compare);
    अगर(found) वापस found->id;
#अन्यथा
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#पूर्ण_अगर
    वापस LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(स्थिर lv_obj_t * obj, स्थिर अक्षर * name)
{
    LV_CHECK_ARG(obj != शून्य, वापस LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != शून्य, वापस LV_PROPERTY_ID_INVALID);
#अगर LV_USE_OBJ_PROPERTY_NAME

    स्थिर lv_obj_class_t * clz;
    lv_prop_id_t id;

    क्रम(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        अगर(id != LV_PROPERTY_ID_INVALID) वापस id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    अगर(id != LV_PROPERTY_ID_INVALID) वापस id;
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#पूर्ण_अगर
    वापस LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

अटल lv_परिणाम_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मूल्य, bool set)
{
    स्थिर lv_property_ops_t * properties;
    स्थिर lv_property_ops_t * prop;

    स्थिर lv_obj_class_t * clz;
    uint32_t सूचकांक = LV_PROPERTY_ID_सूचकांक(id);

    क्रम(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        अगर(properties == शून्य) {
            /* try base class*/
            जारी;
        }

        अगर(id != LV_PROPERTY_ID_ANY && (सूचकांक < clz->prop_सूचकांक_start || सूचकांक > clz->prop_सूचकांक_end)) {
            /* try base class*/
            जारी;
        }

        /*Check if there's setter available for this class*/
        क्रम(uint32_t i = 0; i < clz->properties_गिनती; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            अगर(prop->id == LV_PROPERTY_ID_ANY) {
                मूल्य->id = prop->id;
                अगर(set) वापस ((lv_property_setter_t)prop->setter)(obj, id, मूल्य);
                अन्यथा वापस ((lv_property_getter_t)prop->getter)(obj, id, मूल्य);
            }

            /*Not this id, check next*/
            अगर(prop->id != id)
                जारी;

            /*id matched but we got null pointer to functions*/
            अगर(set ? prop->setter == शून्य : prop->getter == शून्य) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                वापस LV_परिणाम_INVALID;
            }

            /*Update value id if it's a read*/
            अगर(!set) मूल्य->id = prop->id;

            चयन(LV_PROPERTY_ID_TYPE(prop->id)) {
                हाल LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, अंक);
                    अवरोध;
                हाल LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    अवरोध;
                हाल LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    अवरोध;
                हाल LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    अवरोध;
                हाल LV_PROPERTY_TYPE_POINTER:
                हाल LV_PROPERTY_TYPE_IMGSRC:
                हाल LV_PROPERTY_TYPE_TEXT:
                हाल LV_PROPERTY_TYPE_OBJ:
                हाल LV_PROPERTY_TYPE_DISPLAY:
                हाल LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    अवरोध;
                हाल LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &मूल्य->point;
                        अगर(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        अन्यथा *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        अवरोध;
                    }
                शेष: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        वापस LV_परिणाम_INVALID;
                    }
            }

            वापस LV_परिणाम_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    वापस LV_परिणाम_INVALID;
}

अटल पूर्णांक property_name_compare(स्थिर व्योम * ref, स्थिर व्योम * element)
{
    स्थिर lv_property_name_t * prop = element;
    वापस lv_strcmp(ref, prop->name);
}

#पूर्ण_अगर /*LV_USE_OBJ_PROPERTY*/
