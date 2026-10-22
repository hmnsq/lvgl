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
        نيلاي->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, نيلاي->field); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, نيلاي->arg1.field, نيلاي->arg2.اڠک); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, نيلاي->arg1.field, نيلاي->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, نيلاي->arg1.field, نيلاي->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, نيلاي->arg1.field, نيلاي->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_POINTER: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_TEXT: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, نيلاي->arg1.field, نيلاي->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_نيلاي_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_نيلاي_precise_t); \
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
تعريفنوع lv_نيلاي_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_pointer_t)(ثابت lv_obj_t *);
تعريفنوع lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

تعريفنوع lv_result_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * نيلاي, bool set);
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

lv_result_t lv_obj_set_property(lv_obj_t * obj, ثابت lv_property_t * نيلاي)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_RESULT_INVALID);
    LV_CHECK_ARG(نيلاي != عدم, أرجع LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(نيلاي->id);
    إذا(نيلاي->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        أرجع LV_RESULT_INVALID;
    }

    إذا(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, نيلاي->style, نيلاي->selector);
        أرجع LV_RESULT_OK;
    }

    أرجع obj_property(obj, نيلاي->id, (lv_property_t *)نيلاي, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ثابت lv_property_t * نيلاي, uint32_t کيراءن)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_RESULT_INVALID);
    LV_CHECK_ARG(نيلاي != عدم, أرجع LV_RESULT_INVALID);

    لكل(uint32_t i = 0; i < کيراءن; i++) {
        lv_result_t result = lv_obj_set_property(obj, &نيلاي[i]);
        إذا(result != LV_RESULT_OK) {
            أرجع result;
        }
    }

    أرجع LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t نيلاي = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    إذا(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        نيلاي.id = LV_PROPERTY_ID_INVALID;
        نيلاي.اڠک = 0;
        أرجع نيلاي;
    }

    إذا(index < LV_PROPERTY_ID_START) {
        نيلاي.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        نيلاي.id = id;
        نيلاي.selector = LV_PART_MAIN | obj->state;
        أرجع نيلاي;
    }

    result = obj_property(obj, id, &نيلاي, false);
    إذا(result != LV_RESULT_OK)
        نيلاي.id = LV_PROPERTY_ID_INVALID;

    أرجع نيلاي;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t نيلاي;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    إذا(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        نيلاي.id = LV_PROPERTY_ID_INVALID;
        نيلاي.اڠک = 0;
        أرجع نيلاي;
    }

    نيلاي.style = lv_obj_get_style_prop(obj, part, index);
    نيلاي.id = id;
    نيلاي.selector = part | obj->state;
    أرجع نيلاي;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, حجم(lv_style_property_names) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(found) أرجع found->id;
#وإلا
    LV_UNUSED(name);
#endif
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    إذا(names == عدم) {
        /* try base class*/
        أرجع LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_کيراءن, حجم(lv_property_name_t), property_name_compare);
    إذا(found) أرجع found->id;
#وإلا
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
#وإلا
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    أرجع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * نيلاي, bool set)
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
        لكل(uint32_t i = 0; i < clz->properties_کيراءن; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                نيلاي->id = prop->id;
                إذا(set) أرجع ((lv_property_setter_t)prop->setter)(obj, id, نيلاي);
                وإلا أرجع ((lv_property_getter_t)prop->getter)(obj, id, نيلاي);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(set ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                أرجع LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!set) نيلاي->id = prop->id;

            بدل(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالة LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, اڠک);
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
                        lv_point_t * point = &نيلاي->point;
                        إذا(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        وإلا *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        أرجع LV_RESULT_INVALID;
                    }
            }

            أرجع LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    أرجع LV_RESULT_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    أرجع lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
