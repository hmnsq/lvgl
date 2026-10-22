/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাৱ "../lvgl_public.h"

#যদি LV_USE_OBJ_PROPERTY

#অন্তর্ভাৱ "lv_obj_style_internal.h"
#অন্তর্ভাৱ "lv_obj_private.h"
#অন্তর্ভাৱ "../misc/lv_utils.h"
#অন্তর্ভাৱ "lv_obj_class_private.h"
#অন্তর্ভাৱ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    যদি(!set) { \
        মান->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } অন্যথা { \
        ৱিকল্প(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            পক্ষ LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, মান->field); \
                ভঙ্গ; \
            পক্ষ LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.অঙ্ক); \
                ভঙ্গ; \
            পক্ষ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.enable); \
                ভঙ্গ; \
            পক্ষ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.precise); \
                ভঙ্গ; \
            পক্ষ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.color); \
                ভঙ্গ; \
            পক্ষ LV_PROPERTY_TYPE_POINTER: \
            পক্ষ LV_PROPERTY_TYPE_IMGSRC: \
            পক্ষ LV_PROPERTY_TYPE_TEXT: \
            পক্ষ LV_PROPERTY_TYPE_OBJ: \
            পক্ষ LV_PROPERTY_TYPE_DISPLAY: \
            পক্ষ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.ptr); \
                ভঙ্গ; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

প্রকারনির্দেশ int32_t integer;
প্রকারনির্দেশ bool boolean;
প্রকারনির্দেশ lv_মান_precise_t precise;
প্রকারনির্দেশ lv_color_t color;
প্রকারনির্দেশ ধ্রুৱ শূন্য * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_মান_precise_t); \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    প্রকারনির্দেশ শূন্য (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ধ্রুৱ শূন্য *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

প্রকারনির্দেশ শূন্য (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
প্রকারনির্দেশ lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ধ্রুৱ lv_property_t *);

প্রকারনির্দেশ integer(*lv_property_get_integer_t)(ধ্রুৱ lv_obj_t *);
প্রকারনির্দেশ bool (*lv_property_get_boolean_t)(ধ্রুৱ lv_obj_t *);
প্রকারনির্দেশ lv_মান_precise_t (*lv_property_get_precise_t)(ধ্রুৱ lv_obj_t *);
প্রকারনির্দেশ lv_color_t (*lv_property_get_color_t)(ধ্রুৱ lv_obj_t *);
প্রকারনির্দেশ শূন্য * (*lv_property_get_pointer_t)(ধ্রুৱ lv_obj_t *);
প্রকারনির্দেশ lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

প্রকারনির্দেশ lv_result_t (*lv_property_getter_t)(ধ্রুৱ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

স্থির lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * মান, bool set);
স্থির পূর্ণাঙ্ক property_name_compare(ধ্রুৱ শূন্য * ref, ধ্রুৱ শূন্য * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, ধ্রুৱ lv_property_t * মান)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই LV_RESULT_INVALID);
    LV_CHECK_ARG(মান != রিক্ত, ঘূৰাই LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(মান->id);
    যদি(মান->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ঘূৰাই LV_RESULT_INVALID;
    }

    যদি(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, মান->style, মান->selector);
        ঘূৰাই LV_RESULT_OK;
    }

    ঘূৰাই obj_property(obj, মান->id, (lv_property_t *)মান, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ধ্রুৱ lv_property_t * মান, uint32_t গুনতি)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই LV_RESULT_INVALID);
    LV_CHECK_ARG(মান != রিক্ত, ঘূৰাই LV_RESULT_INVALID);

    বাবে(uint32_t i = 0; i < গুনতি; i++) {
        lv_result_t result = lv_obj_set_property(obj, &মান[i]);
        যদি(result != LV_RESULT_OK) {
            ঘূৰাই result;
        }
    }

    ঘূৰাই LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t মান = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    যদি(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        মান.id = LV_PROPERTY_ID_INVALID;
        মান.অঙ্ক = 0;
        ঘূৰাই মান;
    }

    যদি(index < LV_PROPERTY_ID_START) {
        মান.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        মান.id = id;
        মান.selector = LV_PART_MAIN | obj->state;
        ঘূৰাই মান;
    }

    result = obj_property(obj, id, &মান, false);
    যদি(result != LV_RESULT_OK)
        মান.id = LV_PROPERTY_ID_INVALID;

    ঘূৰাই মান;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t মান;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    যদি(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        মান.id = LV_PROPERTY_ID_INVALID;
        মান.অঙ্ক = 0;
        ঘূৰাই মান;
    }

    মান.style = lv_obj_get_style_prop(obj, part, index);
    মান.id = id;
    মান.selector = part | obj->state;
    ঘূৰাই মান;
}

lv_prop_id_t lv_style_property_get_id(ধ্রুৱ ৱর্ণ * name)
{
    LV_CHECK_ARG(name != রিক্ত, ঘূৰাই LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, পরিমাণ(lv_style_property_names) / পরিমাণ(lv_property_name_t),
                             পরিমাণ(lv_property_name_t), property_name_compare);
    যদি(found) ঘূৰাই found->id;
#অন্যথা
    LV_UNUSED(name);
#endif
    ঘূৰাই LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ধ্রুৱ lv_obj_class_t * clz, ধ্রুৱ ৱর্ণ * name)
{
    LV_CHECK_ARG(clz != রিক্ত, ঘূৰাই LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != রিক্ত, ঘূৰাই LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME
    ধ্রুৱ lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    যদি(names == রিক্ত) {
        /* try base class*/
        ঘূৰাই LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_গুনতি, পরিমাণ(lv_property_name_t), property_name_compare);
    যদি(found) ঘূৰাই found->id;
#অন্যথা
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ঘূৰাই LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ধ্রুৱ lv_obj_t * obj, ধ্রুৱ ৱর্ণ * name)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != রিক্ত, ঘূৰাই LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME

    ধ্রুৱ lv_obj_class_t * clz;
    lv_prop_id_t id;

    বাবে(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        যদি(id != LV_PROPERTY_ID_INVALID) ঘূৰাই id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    যদি(id != LV_PROPERTY_ID_INVALID) ঘূৰাই id;
#অন্যথা
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ঘূৰাই LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

স্থির lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * মান, bool set)
{
    ধ্রুৱ lv_property_ops_t * properties;
    ধ্রুৱ lv_property_ops_t * prop;

    ধ্রুৱ lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    বাবে(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        যদি(properties == রিক্ত) {
            /* try base class*/
            অনুৱর্ত;
        }

        যদি(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            অনুৱর্ত;
        }

        /*Check if there's setter available for this class*/
        বাবে(uint32_t i = 0; i < clz->properties_গুনতি; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            যদি(prop->id == LV_PROPERTY_ID_ANY) {
                মান->id = prop->id;
                যদি(set) ঘূৰাই ((lv_property_setter_t)prop->setter)(obj, id, মান);
                অন্যথা ঘূৰাই ((lv_property_getter_t)prop->getter)(obj, id, মান);
            }

            /*Not this id, check next*/
            যদি(prop->id != id)
                অনুৱর্ত;

            /*id matched but we got null pointer to functions*/
            যদি(set ? prop->setter == রিক্ত : prop->getter == রিক্ত) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ঘূৰাই LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            যদি(!set) মান->id = prop->id;

            ৱিকল্প(LV_PROPERTY_ID_TYPE(prop->id)) {
                পক্ষ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, অঙ্ক);
                    ভঙ্গ;
                পক্ষ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ভঙ্গ;
                পক্ষ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ভঙ্গ;
                পক্ষ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ভঙ্গ;
                পক্ষ LV_PROPERTY_TYPE_POINTER:
                পক্ষ LV_PROPERTY_TYPE_IMGSRC:
                পক্ষ LV_PROPERTY_TYPE_TEXT:
                পক্ষ LV_PROPERTY_TYPE_OBJ:
                পক্ষ LV_PROPERTY_TYPE_DISPLAY:
                পক্ষ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    ভঙ্গ;
                পক্ষ LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &মান->point;
                        যদি(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        অন্যথা *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        ভঙ্গ;
                    }
                স্ৱভাৱ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ঘূৰাই LV_RESULT_INVALID;
                    }
            }

            ঘূৰাই LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ঘূৰাই LV_RESULT_INVALID;
}

স্থির পূর্ণাঙ্ক property_name_compare(ধ্রুৱ শূন্য * ref, ধ্রুৱ শূন্য * element)
{
    ধ্রুৱ lv_property_name_t * prop = element;
    ঘূৰাই lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
