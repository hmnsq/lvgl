/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../lvgl_public.h"

#जर LV_USE_OBJ_PROPERTY

#अन्तर्भाव "lv_obj_style_internal.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../misc/lv_utils.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    जर(!set) { \
        मूल्य->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } अन्यथा { \
        निवड(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            स्थिती LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, मूल्य->field); \
                थांबा; \
            स्थिती LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.अंक); \
                थांबा; \
            स्थिती LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.enable); \
                थांबा; \
            स्थिती LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.precise); \
                थांबा; \
            स्थिती LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.color); \
                थांबा; \
            स्थिती LV_PROPERTY_TYPE_POINTER: \
            स्थिती LV_PROPERTY_TYPE_IMGSRC: \
            स्थिती LV_PROPERTY_TYPE_TEXT: \
            स्थिती LV_PROPERTY_TYPE_OBJ: \
            स्थिती LV_PROPERTY_TYPE_DISPLAY: \
            स्थिती LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, मूल्य->arg1.field, मूल्य->arg2.ptr); \
                थांबा; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

प्रकारव्याख्या int32_t integer;
प्रकारव्याख्या bool boolean;
प्रकारव्याख्या lv_मूल्य_precise_t precise;
प्रकारव्याख्या lv_color_t color;
प्रकारव्याख्या स्थिरांक शून्य * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_मूल्य_precise_t); \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    प्रकारव्याख्या शून्य (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, स्थिरांक शून्य *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

प्रकारव्याख्या शून्य (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
प्रकारव्याख्या lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, स्थिरांक lv_property_t *);

प्रकारव्याख्या integer(*lv_property_get_integer_t)(स्थिरांक lv_obj_t *);
प्रकारव्याख्या bool (*lv_property_get_boolean_t)(स्थिरांक lv_obj_t *);
प्रकारव्याख्या lv_मूल्य_precise_t (*lv_property_get_precise_t)(स्थिरांक lv_obj_t *);
प्रकारव्याख्या lv_color_t (*lv_property_get_color_t)(स्थिरांक lv_obj_t *);
प्रकारव्याख्या शून्य * (*lv_property_get_pointer_t)(स्थिरांक lv_obj_t *);
प्रकारव्याख्या lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

प्रकारव्याख्या lv_result_t (*lv_property_getter_t)(स्थिरांक lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

स्थिर lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मूल्य, bool set);
स्थिर पूर्णांक property_name_compare(स्थिरांक शून्य * ref, स्थिरांक शून्य * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, स्थिरांक lv_property_t * मूल्य)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_RESULT_INVALID);
    LV_CHECK_ARG(मूल्य != रिक्त, परत LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(मूल्य->id);
    जर(मूल्य->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        परत LV_RESULT_INVALID;
    }

    जर(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, मूल्य->style, मूल्य->selector);
        परत LV_RESULT_OK;
    }

    परत obj_property(obj, मूल्य->id, (lv_property_t *)मूल्य, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, स्थिरांक lv_property_t * मूल्य, uint32_t मोजणी)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_RESULT_INVALID);
    LV_CHECK_ARG(मूल्य != रिक्त, परत LV_RESULT_INVALID);

    साठी(uint32_t i = 0; i < मोजणी; i++) {
        lv_result_t result = lv_obj_set_property(obj, &मूल्य[i]);
        जर(result != LV_RESULT_OK) {
            परत result;
        }
    }

    परत LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != रिक्त, परत (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t मूल्य = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    जर(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        मूल्य.id = LV_PROPERTY_ID_INVALID;
        मूल्य.अंक = 0;
        परत मूल्य;
    }

    जर(index < LV_PROPERTY_ID_START) {
        मूल्य.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        मूल्य.id = id;
        मूल्य.selector = LV_PART_MAIN | obj->state;
        परत मूल्य;
    }

    result = obj_property(obj, id, &मूल्य, false);
    जर(result != LV_RESULT_OK)
        मूल्य.id = LV_PROPERTY_ID_INVALID;

    परत मूल्य;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != रिक्त, परत (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t मूल्य;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    जर(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        मूल्य.id = LV_PROPERTY_ID_INVALID;
        मूल्य.अंक = 0;
        परत मूल्य;
    }

    मूल्य.style = lv_obj_get_style_prop(obj, part, index);
    मूल्य.id = id;
    मूल्य.selector = part | obj->state;
    परत मूल्य;
}

lv_prop_id_t lv_style_property_get_id(स्थिरांक अक्षर * name)
{
    LV_CHECK_ARG(name != रिक्त, परत LV_PROPERTY_ID_INVALID);
#जर LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, आकार(lv_style_property_names) / आकार(lv_property_name_t),
                             आकार(lv_property_name_t), property_name_compare);
    जर(found) परत found->id;
#अन्यथा
    LV_UNUSED(name);
#endif
    परत LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(स्थिरांक lv_obj_class_t * clz, स्थिरांक अक्षर * name)
{
    LV_CHECK_ARG(clz != रिक्त, परत LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, परत LV_PROPERTY_ID_INVALID);
#जर LV_USE_OBJ_PROPERTY_NAME
    स्थिरांक lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    जर(names == रिक्त) {
        /* try base class*/
        परत LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_मोजणी, आकार(lv_property_name_t), property_name_compare);
    जर(found) परत found->id;
#अन्यथा
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    परत LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(स्थिरांक lv_obj_t * obj, स्थिरांक अक्षर * name)
{
    LV_CHECK_ARG(obj != रिक्त, परत LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != रिक्त, परत LV_PROPERTY_ID_INVALID);
#जर LV_USE_OBJ_PROPERTY_NAME

    स्थिरांक lv_obj_class_t * clz;
    lv_prop_id_t id;

    साठी(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        जर(id != LV_PROPERTY_ID_INVALID) परत id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    जर(id != LV_PROPERTY_ID_INVALID) परत id;
#अन्यथा
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    परत LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

स्थिर lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * मूल्य, bool set)
{
    स्थिरांक lv_property_ops_t * properties;
    स्थिरांक lv_property_ops_t * prop;

    स्थिरांक lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    साठी(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        जर(properties == रिक्त) {
            /* try base class*/
            सुरूठेवा;
        }

        जर(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            सुरूठेवा;
        }

        /*Check if there's setter available for this class*/
        साठी(uint32_t i = 0; i < clz->properties_मोजणी; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            जर(prop->id == LV_PROPERTY_ID_ANY) {
                मूल्य->id = prop->id;
                जर(set) परत ((lv_property_setter_t)prop->setter)(obj, id, मूल्य);
                अन्यथा परत ((lv_property_getter_t)prop->getter)(obj, id, मूल्य);
            }

            /*Not this id, check next*/
            जर(prop->id != id)
                सुरूठेवा;

            /*id matched but we got null pointer to functions*/
            जर(set ? prop->setter == रिक्त : prop->getter == रिक्त) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                परत LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            जर(!set) मूल्य->id = prop->id;

            निवड(LV_PROPERTY_ID_TYPE(prop->id)) {
                स्थिती LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, अंक);
                    थांबा;
                स्थिती LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    थांबा;
                स्थिती LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    थांबा;
                स्थिती LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    थांबा;
                स्थिती LV_PROPERTY_TYPE_POINTER:
                स्थिती LV_PROPERTY_TYPE_IMGSRC:
                स्थिती LV_PROPERTY_TYPE_TEXT:
                स्थिती LV_PROPERTY_TYPE_OBJ:
                स्थिती LV_PROPERTY_TYPE_DISPLAY:
                स्थिती LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    थांबा;
                स्थिती LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &मूल्य->point;
                        जर(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        अन्यथा *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        थांबा;
                    }
                पूर्वनिर्धारित: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        परत LV_RESULT_INVALID;
                    }
            }

            परत LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    परत LV_RESULT_INVALID;
}

स्थिर पूर्णांक property_name_compare(स्थिरांक शून्य * ref, स्थिरांक शून्य * element)
{
    स्थिरांक lv_property_name_t * prop = element;
    परत lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
