/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

#אם LV_USE_OBJ_PROPERTY

#ضمن "lv_obj_style_internal.h"
#ضمن "lv_obj_private.h"
#ضمن "../misc/lv_utils.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    אם(!set) { \
        value->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } אחרת { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, value->field); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, value->arg1.field, value->arg2.num); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, value->arg1.field, value->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, value->arg1.field, value->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, value->arg1.field, value->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_POINTER: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_TEXT: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, value->arg1.field, value->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_value_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_value_precise_t); \
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
تعريفنوع lv_value_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_pointer_t)(ثابت lv_obj_t *);
تعريفنوع lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

تعريفنوع lv_result_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set);
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

lv_result_t lv_obj_set_property(lv_obj_t * obj, ثابت lv_property_t * value)
{
    LV_CHECK_ARG(obj != عدم, החזר LV_RESULT_INVALID);
    LV_CHECK_ARG(value != عدم, החזר LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(value->id);
    אם(value->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        החזר LV_RESULT_INVALID;
    }

    אם(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, value->style, value->selector);
        החזר LV_RESULT_OK;
    }

    החזר obj_property(obj, value->id, (lv_property_t *)value, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ثابت lv_property_t * value, uint32_t count)
{
    LV_CHECK_ARG(obj != عدم, החזר LV_RESULT_INVALID);
    LV_CHECK_ARG(value != عدم, החזר LV_RESULT_INVALID);

    עבור(uint32_t i = 0; i < count; i++) {
        lv_result_t result = lv_obj_set_property(obj, &value[i]);
        אם(result != LV_RESULT_OK) {
            החזר result;
        }
    }

    החזר LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, החזר (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t value = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    אם(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        החזר value;
    }

    אם(index < LV_PROPERTY_ID_START) {
        value.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        value.id = id;
        value.selector = LV_PART_MAIN | obj->state;
        החזר value;
    }

    result = obj_property(obj, id, &value, false);
    אם(result != LV_RESULT_OK)
        value.id = LV_PROPERTY_ID_INVALID;

    החזר value;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, החזר (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t value;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    אם(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        value.id = LV_PROPERTY_ID_INVALID;
        value.num = 0;
        החזר value;
    }

    value.style = lv_obj_get_style_prop(obj, part, index);
    value.id = id;
    value.selector = part | obj->state;
    החזר value;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, החזר LV_PROPERTY_ID_INVALID);
#אם LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, حجم(lv_style_property_names) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    אם(found) החזר found->id;
#אחרת
    LV_UNUSED(name);
#endif
    החזר LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, החזר LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, החזר LV_PROPERTY_ID_INVALID);
#אם LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    אם(names == عدم) {
        /* try base class*/
        החזר LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_count, حجم(lv_property_name_t), property_name_compare);
    אם(found) החזר found->id;
#אחרת
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    החזר LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, החזר LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, החזר LV_PROPERTY_ID_INVALID);
#אם LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    עבור(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        אם(id != LV_PROPERTY_ID_INVALID) החזר id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    אם(id != LV_PROPERTY_ID_INVALID) החזר id;
#אחרת
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    החזר LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * value, bool set)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    עבור(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        אם(properties == عدم) {
            /* try base class*/
            استمر;
        }

        אם(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        עבור(uint32_t i = 0; i < clz->properties_count; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            אם(prop->id == LV_PROPERTY_ID_ANY) {
                value->id = prop->id;
                אם(set) החזר ((lv_property_setter_t)prop->setter)(obj, id, value);
                אחרת החזר ((lv_property_getter_t)prop->getter)(obj, id, value);
            }

            /*Not this id, check next*/
            אם(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            אם(set ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                החזר LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            אם(!set) value->id = prop->id;

            بدل(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالة LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, num);
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
                        lv_point_t * point = &value->point;
                        אם(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        אחרת *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        החזר LV_RESULT_INVALID;
                    }
            }

            החזר LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    החזר LV_RESULT_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    החזר lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
