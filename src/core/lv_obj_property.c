/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "../lvgl_public.h"

#যদি LV_USE_OBJ_PROPERTY

#অন্তর্ভাব "lv_obj_style_internal.h"
#অন্তর্ভাব "lv_obj_private.h"
#অন্তর্ভাব "../misc/lv_utils.h"
#অন্তর্ভাব "lv_obj_class_private.h"
#অন্তর্ভাব "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    যদি(!set) { \
        মান->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } অন্যথা { \
        নির্বাচন(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            ক্ষেত্র LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, মান->field); \
                বিরতি; \
            ক্ষেত্র LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.অঙ্ক); \
                বিরতি; \
            ক্ষেত্র LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.enable); \
                বিরতি; \
            ক্ষেত্র LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.precise); \
                বিরতি; \
            ক্ষেত্র LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.color); \
                বিরতি; \
            ক্ষেত্র LV_PROPERTY_TYPE_POINTER: \
            ক্ষেত্র LV_PROPERTY_TYPE_IMGSRC: \
            ক্ষেত্র LV_PROPERTY_TYPE_TEXT: \
            ক্ষেত্র LV_PROPERTY_TYPE_OBJ: \
            ক্ষেত্র LV_PROPERTY_TYPE_DISPLAY: \
            ক্ষেত্র LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, মান->arg1.field, মান->arg2.ptr); \
                বিরতি; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

প্রকারলেখ int32_t integer;
প্রকারলেখ bool boolean;
প্রকারলেখ lv_মান_precise_t precise;
প্রকারলেখ lv_color_t color;
প্রকারলেখ ধ্রুবক শূন্য * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_মান_precise_t); \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    প্রকারলেখ শূন্য (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ধ্রুবক শূন্য *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

প্রকারলেখ শূন্য (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
প্রকারলেখ lv_ফলাফল_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ধ্রুবক lv_property_t *);

প্রকারলেখ integer(*lv_property_get_integer_t)(ধ্রুবক lv_obj_t *);
প্রকারলেখ bool (*lv_property_get_boolean_t)(ধ্রুবক lv_obj_t *);
প্রকারলেখ lv_মান_precise_t (*lv_property_get_precise_t)(ধ্রুবক lv_obj_t *);
প্রকারলেখ lv_color_t (*lv_property_get_color_t)(ধ্রুবক lv_obj_t *);
প্রকারলেখ শূন্য * (*lv_property_get_pointer_t)(ধ্রুবক lv_obj_t *);
প্রকারলেখ lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

প্রকারলেখ lv_ফলাফল_t (*lv_property_getter_t)(ধ্রুবক lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

স্থির lv_ফলাফল_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * মান, bool set);
স্থির পূর্ণসংখ্যা property_name_compare(ধ্রুবক শূন্য * ref, ধ্রুবক শূন্য * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ফলাফল_t lv_obj_set_property(lv_obj_t * obj, ধ্রুবক lv_property_t * মান)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত LV_ফলাফল_INVALID);
    LV_CHECK_ARG(মান != ফাঁকা, ফেরত LV_ফলাফল_INVALID);

    uint32_t সূচক = LV_PROPERTY_ID_সূচক(মান->id);
    যদি(মান->id == LV_PROPERTY_ID_INVALID || সূচক > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ফেরত LV_ফলাফল_INVALID;
    }

    যদি(সূচক < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, সূচক, মান->style, মান->selector);
        ফেরত LV_ফলাফল_OK;
    }

    ফেরত obj_property(obj, মান->id, (lv_property_t *)মান, true);
}

lv_ফলাফল_t lv_obj_set_properties(lv_obj_t * obj, ধ্রুবক lv_property_t * মান, uint32_t গুনতি)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত LV_ফলাফল_INVALID);
    LV_CHECK_ARG(মান != ফাঁকা, ফেরত LV_ফলাফল_INVALID);

    জন্য(uint32_t i = 0; i < গুনতি; i++) {
        lv_ফলাফল_t ফলাফল = lv_obj_set_property(obj, &মান[i]);
        যদি(ফলাফল != LV_ফলাফল_OK) {
            ফেরত ফলাফল;
        }
    }

    ফেরত LV_ফলাফল_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_ফলাফল_t ফলাফল;
    lv_property_t মান = { 0 };

    uint32_t সূচক = LV_PROPERTY_ID_সূচক(id);
    যদি(id == LV_PROPERTY_ID_INVALID || সূচক > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        মান.id = LV_PROPERTY_ID_INVALID;
        মান.অঙ্ক = 0;
        ফেরত মান;
    }

    যদি(সূচক < LV_PROPERTY_ID_START) {
        মান.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, সূচক);
        মান.id = id;
        মান.selector = LV_PART_MAIN | obj->state;
        ফেরত মান;
    }

    ফলাফল = obj_property(obj, id, &মান, false);
    যদি(ফলাফল != LV_ফলাফল_OK)
        মান.id = LV_PROPERTY_ID_INVALID;

    ফেরত মান;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t মান;
    uint32_t সূচক = LV_PROPERTY_ID_সূচক(id);

    যদি(সূচক == LV_PROPERTY_ID_INVALID || সূচক >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        মান.id = LV_PROPERTY_ID_INVALID;
        মান.অঙ্ক = 0;
        ফেরত মান;
    }

    মান.style = lv_obj_get_style_prop(obj, part, সূচক);
    মান.id = id;
    মান.selector = part | obj->state;
    ফেরত মান;
}

lv_prop_id_t lv_style_property_get_id(ধ্রুবক অক্ষর * name)
{
    LV_CHECK_ARG(name != ফাঁকা, ফেরত LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, আকার(lv_style_property_names) / আকার(lv_property_name_t),
                             আকার(lv_property_name_t), property_name_compare);
    যদি(found) ফেরত found->id;
#অন্যথা
    LV_UNUSED(name);
#endif
    ফেরত LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ধ্রুবক lv_obj_class_t * clz, ধ্রুবক অক্ষর * name)
{
    LV_CHECK_ARG(clz != ফাঁকা, ফেরত LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ফাঁকা, ফেরত LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME
    ধ্রুবক lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    যদি(names == ফাঁকা) {
        /* try base class*/
        ফেরত LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_গুনতি, আকার(lv_property_name_t), property_name_compare);
    যদি(found) ফেরত found->id;
#অন্যথা
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ফেরত LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ধ্রুবক lv_obj_t * obj, ধ্রুবক অক্ষর * name)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ফাঁকা, ফেরত LV_PROPERTY_ID_INVALID);
#যদি LV_USE_OBJ_PROPERTY_NAME

    ধ্রুবক lv_obj_class_t * clz;
    lv_prop_id_t id;

    জন্য(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        যদি(id != LV_PROPERTY_ID_INVALID) ফেরত id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    যদি(id != LV_PROPERTY_ID_INVALID) ফেরত id;
#অন্যথা
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ফেরত LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

স্থির lv_ফলাফল_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * মান, bool set)
{
    ধ্রুবক lv_property_ops_t * properties;
    ধ্রুবক lv_property_ops_t * prop;

    ধ্রুবক lv_obj_class_t * clz;
    uint32_t সূচক = LV_PROPERTY_ID_সূচক(id);

    জন্য(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        যদি(properties == ফাঁকা) {
            /* try base class*/
            চালু;
        }

        যদি(id != LV_PROPERTY_ID_ANY && (সূচক < clz->prop_সূচক_start || সূচক > clz->prop_সূচক_end)) {
            /* try base class*/
            চালু;
        }

        /*Check if there's setter available for this class*/
        জন্য(uint32_t i = 0; i < clz->properties_গুনতি; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            যদি(prop->id == LV_PROPERTY_ID_ANY) {
                মান->id = prop->id;
                যদি(set) ফেরত ((lv_property_setter_t)prop->setter)(obj, id, মান);
                অন্যথা ফেরত ((lv_property_getter_t)prop->getter)(obj, id, মান);
            }

            /*Not this id, check next*/
            যদি(prop->id != id)
                চালু;

            /*id matched but we got null pointer to functions*/
            যদি(set ? prop->setter == ফাঁকা : prop->getter == ফাঁকা) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ফেরত LV_ফলাফল_INVALID;
            }

            /*Update value id if it's a read*/
            যদি(!set) মান->id = prop->id;

            নির্বাচন(LV_PROPERTY_ID_TYPE(prop->id)) {
                ক্ষেত্র LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, অঙ্ক);
                    বিরতি;
                ক্ষেত্র LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    বিরতি;
                ক্ষেত্র LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    বিরতি;
                ক্ষেত্র LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    বিরতি;
                ক্ষেত্র LV_PROPERTY_TYPE_POINTER:
                ক্ষেত্র LV_PROPERTY_TYPE_IMGSRC:
                ক্ষেত্র LV_PROPERTY_TYPE_TEXT:
                ক্ষেত্র LV_PROPERTY_TYPE_OBJ:
                ক্ষেত্র LV_PROPERTY_TYPE_DISPLAY:
                ক্ষেত্র LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    বিরতি;
                ক্ষেত্র LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &মান->point;
                        যদি(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        অন্যথা *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        বিরতি;
                    }
                পূর্বনির্ধারিত: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ফেরত LV_ফলাফল_INVALID;
                    }
            }

            ফেরত LV_ফলাফল_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ফেরত LV_ফলাফল_INVALID;
}

স্থির পূর্ণসংখ্যা property_name_compare(ধ্রুবক শূন্য * ref, ধ্রুবক শূন্য * element)
{
    ধ্রুবক lv_property_name_t * prop = element;
    ফেরত lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
