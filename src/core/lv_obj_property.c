/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

#إذا LV_USE_OBJ_PROPERTY

#ضمن "lv_obj_style_internal.h"
#ضمن "lv_obj_private.h"
#ضمن "../misc/lv_utils.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    إذا(!set) { \
        قيمة->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, قيمة->field); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, قيمة->arg1.field, قيمة->arg2.رقم); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, قيمة->arg1.field, قيمة->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, قيمة->arg1.field, قيمة->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, قيمة->arg1.field, قيمة->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_POINTER: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_TEXT: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, قيمة->arg1.field, قيمة->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_قيمة_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_قيمة_precise_t); \
    تعريفنوع فراغ (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعريفنوع فراغ (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ثابت فراغ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

تعريفنوع فراغ (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
تعريفنوع lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعريفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعريفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعريفنوع lv_قيمة_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_pointer_t)(ثابت lv_obj_t *);
تعريفنوع lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

تعريفنوع lv_result_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قيمة, bool set);
ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, ثابت lv_property_t * قيمة)
{
    LV_CHECK_ARG(obj != عدم, إرجاع LV_RESULT_INVALID);
    LV_CHECK_ARG(قيمة != عدم, إرجاع LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(قيمة->id);
    إذا(قيمة->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        إرجاع LV_RESULT_INVALID;
    }

    إذا(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, قيمة->style, قيمة->selector);
        إرجاع LV_RESULT_OK;
    }

    إرجاع obj_property(obj, قيمة->id, (lv_property_t *)قيمة, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ثابت lv_property_t * قيمة, uint32_t تعداد_2)
{
    LV_CHECK_ARG(obj != عدم, إرجاع LV_RESULT_INVALID);
    LV_CHECK_ARG(قيمة != عدم, إرجاع LV_RESULT_INVALID);

    لكل(uint32_t i = 0; i < تعداد_2; i++) {
        lv_result_t result = lv_obj_set_property(obj, &قيمة[i]);
        إذا(result != LV_RESULT_OK) {
            إرجاع result;
        }
    }

    إرجاع LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, إرجاع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t قيمة = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    إذا(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        قيمة.id = LV_PROPERTY_ID_INVALID;
        قيمة.رقم = 0;
        إرجاع قيمة;
    }

    إذا(index < LV_PROPERTY_ID_START) {
        قيمة.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        قيمة.id = id;
        قيمة.selector = LV_PART_MAIN | obj->state;
        إرجاع قيمة;
    }

    result = obj_property(obj, id, &قيمة, false);
    إذا(result != LV_RESULT_OK)
        قيمة.id = LV_PROPERTY_ID_INVALID;

    إرجاع قيمة;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, إرجاع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t قيمة;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    إذا(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        قيمة.id = LV_PROPERTY_ID_INVALID;
        قيمة.رقم = 0;
        إرجاع قيمة;
    }

    قيمة.style = lv_obj_get_style_prop(obj, part, index);
    قيمة.id = id;
    قيمة.selector = part | obj->state;
    إرجاع قيمة;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, إرجاع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, حجم(lv_style_property_names) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(found) إرجاع found->id;
#وإلا
    LV_UNUSED(name);
#endif
    إرجاع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, إرجاع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, إرجاع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    إذا(names == عدم) {
        /* try base class*/
        إرجاع LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_تعداد, حجم(lv_property_name_t), property_name_compare);
    إذا(found) إرجاع found->id;
#وإلا
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    إرجاع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, إرجاع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, إرجاع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) إرجاع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) إرجاع id;
#وإلا
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    إرجاع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قيمة, bool set)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    لكل(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        إذا(properties == عدم) {
            /* try base class*/
            استمر;
        }

        إذا(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        لكل(uint32_t i = 0; i < clz->properties_تعداد; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                قيمة->id = prop->id;
                إذا(set) إرجاع ((lv_property_setter_t)prop->setter)(obj, id, قيمة);
                وإلا إرجاع ((lv_property_getter_t)prop->getter)(obj, id, قيمة);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(set ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                إرجاع LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!set) قيمة->id = prop->id;

            بدل(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالة LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, رقم);
                    اكسر;
                حالة LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    اكسر;
                حالة LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    اكسر;
                حالة LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    اكسر;
                حالة LV_PROPERTY_TYPE_POINTER:
                حالة LV_PROPERTY_TYPE_IMGSRC:
                حالة LV_PROPERTY_TYPE_TEXT:
                حالة LV_PROPERTY_TYPE_OBJ:
                حالة LV_PROPERTY_TYPE_DISPLAY:
                حالة LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    اكسر;
                حالة LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &قيمة->point;
                        إذا(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        وإلا *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        إرجاع LV_RESULT_INVALID;
                    }
            }

            إرجاع LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    إرجاع LV_RESULT_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    إرجاع lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
