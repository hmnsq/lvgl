/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "../lvgl_public.h"

#اگر LV_USE_OBJ_PROPERTY

#شامل "lv_obj_style_internal.h"
#شامل "lv_obj_private.h"
#شامل "../misc/lv_utils.h"
#شامل "lv_obj_class_private.h"
#شامل "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    اگر(!set) { \
        مقدار->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وگرنه { \
        انتخاب(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالت LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, مقدار->field); \
                بشکن; \
            حالت LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, مقدار->arg1.field, مقدار->arg2.رقم); \
                بشکن; \
            حالت LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, مقدار->arg1.field, مقدار->arg2.enable); \
                بشکن; \
            حالت LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, مقدار->arg1.field, مقدار->arg2.precise); \
                بشکن; \
            حالت LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, مقدار->arg1.field, مقدار->arg2.color); \
                بشکن; \
            حالت LV_PROPERTY_TYPE_POINTER: \
            حالت LV_PROPERTY_TYPE_IMGSRC: \
            حالت LV_PROPERTY_TYPE_TEXT: \
            حالت LV_PROPERTY_TYPE_OBJ: \
            حالت LV_PROPERTY_TYPE_DISPLAY: \
            حالت LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, مقدار->arg1.field, مقدار->arg2.ptr); \
                بشکن; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعریفنوع int32_t integer;
تعریفنوع bool boolean;
تعریفنوع lv_مقدار_precise_t precise;
تعریفنوع lv_color_t color;
تعریفنوع ثابت تهی * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعریفنوع تهی (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    تعریفنوع تهی (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعریفنوع تهی (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعریفنوع تهی (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_مقدار_precise_t); \
    تعریفنوع تهی (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعریفنوع تهی (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ثابت تهی *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

تعریفنوع تهی (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
تعریفنوع lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعریفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعریفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعریفنوع lv_مقدار_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعریفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعریفنوع تهی * (*lv_property_get_pointer_t)(ثابت lv_obj_t *);
تعریفنوع lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

تعریفنوع lv_result_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ایستا lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * مقدار, bool set);
ایستا صحیح property_name_compare(ثابت تهی * ref, ثابت تهی * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, ثابت lv_property_t * مقدار)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت LV_RESULT_INVALID);
    LV_CHECK_ARG(مقدار != پوچ, بازگشت LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(مقدار->id);
    اگر(مقدار->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        بازگشت LV_RESULT_INVALID;
    }

    اگر(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, مقدار->style, مقدار->selector);
        بازگشت LV_RESULT_OK;
    }

    بازگشت obj_property(obj, مقدار->id, (lv_property_t *)مقدار, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ثابت lv_property_t * مقدار, uint32_t شمار)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت LV_RESULT_INVALID);
    LV_CHECK_ARG(مقدار != پوچ, بازگشت LV_RESULT_INVALID);

    برای(uint32_t i = 0; i < شمار; i++) {
        lv_result_t result = lv_obj_set_property(obj, &مقدار[i]);
        اگر(result != LV_RESULT_OK) {
            بازگشت result;
        }
    }

    بازگشت LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t مقدار = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    اگر(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        مقدار.id = LV_PROPERTY_ID_INVALID;
        مقدار.رقم = 0;
        بازگشت مقدار;
    }

    اگر(index < LV_PROPERTY_ID_START) {
        مقدار.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        مقدار.id = id;
        مقدار.selector = LV_PART_MAIN | obj->state;
        بازگشت مقدار;
    }

    result = obj_property(obj, id, &مقدار, false);
    اگر(result != LV_RESULT_OK)
        مقدار.id = LV_PROPERTY_ID_INVALID;

    بازگشت مقدار;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t مقدار;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    اگر(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        مقدار.id = LV_PROPERTY_ID_INVALID;
        مقدار.رقم = 0;
        بازگشت مقدار;
    }

    مقدار.style = lv_obj_get_style_prop(obj, part, index);
    مقدار.id = id;
    مقدار.selector = part | obj->state;
    بازگشت مقدار;
}

lv_prop_id_t lv_style_property_get_id(ثابت نویسه * name)
{
    LV_CHECK_ARG(name != پوچ, بازگشت LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, اندازه(lv_style_property_names) / اندازه(lv_property_name_t),
                             اندازه(lv_property_name_t), property_name_compare);
    اگر(found) بازگشت found->id;
#وگرنه
    LV_UNUSED(name);
#endif
    بازگشت LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت نویسه * name)
{
    LV_CHECK_ARG(clz != پوچ, بازگشت LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != پوچ, بازگشت LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    اگر(names == پوچ) {
        /* try base class*/
        بازگشت LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_شمار, اندازه(lv_property_name_t), property_name_compare);
    اگر(found) بازگشت found->id;
#وگرنه
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    بازگشت LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت نویسه * name)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != پوچ, بازگشت LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    برای(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        اگر(id != LV_PROPERTY_ID_INVALID) بازگشت id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    اگر(id != LV_PROPERTY_ID_INVALID) بازگشت id;
#وگرنه
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    بازگشت LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ایستا lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * مقدار, bool set)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    برای(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        اگر(properties == پوچ) {
            /* try base class*/
            ادامه;
        }

        اگر(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            ادامه;
        }

        /*Check if there's setter available for this class*/
        برای(uint32_t i = 0; i < clz->properties_شمار; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            اگر(prop->id == LV_PROPERTY_ID_ANY) {
                مقدار->id = prop->id;
                اگر(set) بازگشت ((lv_property_setter_t)prop->setter)(obj, id, مقدار);
                وگرنه بازگشت ((lv_property_getter_t)prop->getter)(obj, id, مقدار);
            }

            /*Not this id, check next*/
            اگر(prop->id != id)
                ادامه;

            /*id matched but we got null pointer to functions*/
            اگر(set ? prop->setter == پوچ : prop->getter == پوچ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                بازگشت LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            اگر(!set) مقدار->id = prop->id;

            انتخاب(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالت LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, رقم);
                    بشکن;
                حالت LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    بشکن;
                حالت LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    بشکن;
                حالت LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    بشکن;
                حالت LV_PROPERTY_TYPE_POINTER:
                حالت LV_PROPERTY_TYPE_IMGSRC:
                حالت LV_PROPERTY_TYPE_TEXT:
                حالت LV_PROPERTY_TYPE_OBJ:
                حالت LV_PROPERTY_TYPE_DISPLAY:
                حالت LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    بشکن;
                حالت LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &مقدار->point;
                        اگر(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        وگرنه *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        بشکن;
                    }
                پیشفرض: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        بازگشت LV_RESULT_INVALID;
                    }
            }

            بازگشت LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    بازگشت LV_RESULT_INVALID;
}

ایستا صحیح property_name_compare(ثابت تهی * ref, ثابت تهی * element)
{
    ثابت lv_property_name_t * prop = element;
    بازگشت lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
