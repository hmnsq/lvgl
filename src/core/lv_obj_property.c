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

#تعریف_کرو HANDLE_PROPERTY_TYPE(type, میدان) \
    اگر(!سیٹ) { \
        قدر->میدان = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ورنہ { \
        انتخاب(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالت LV_PROPERTY_ID_INVALID: \
                ((lv_property_سیٹ##type##_t)(prop->setter))(obj, قدر->میدان); \
                توڑو; \
            حالت LV_PROPERTY_TYPE_INT: \
                ((lv_property_سیٹ##type##_integer_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.عدد); \
                توڑو; \
            حالت LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_سیٹ##type##_boolean_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.enable); \
                توڑو; \
            حالت LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_سیٹ##type##_precise_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.precise); \
                توڑو; \
            حالت LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_سیٹ##type##_color_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.color); \
                توڑو; \
            حالت LV_PROPERTY_TYPE_اشارہ_گر: \
            حالت LV_PROPERTY_TYPE_IMGSRC: \
            حالت LV_PROPERTY_TYPE_متن: \
            حالت LV_PROPERTY_TYPE_OBJ: \
            حالت LV_PROPERTY_TYPE_DISPLAY: \
            حالت LV_PROPERTY_TYPE_FONT: \
                ((lv_property_سیٹ##type##_pointer_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.ptr); \
                توڑو; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

قسمتعریف int32_t integer;
قسمتعریف bool boolean;
قسمتعریف lv_قدر_precise_t precise;
قسمتعریف lv_color_t color;
قسمتعریف ثابت خالی * اشارہ_گر;

#تعریف_کرو DEFINE_PROPERTY_SETTER_TYPES(type) \
    قسمتعریف خالی (*lv_property_سیٹ##type##_t)(lv_obj_t *, type); \
    قسمتعریف خالی (*lv_property_سیٹ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    قسمتعریف خالی (*lv_property_سیٹ##type##_boolean_t)(lv_obj_t *, type, bool); \
    قسمتعریف خالی (*lv_property_سیٹ##type##_precise_t)(lv_obj_t *, type, lv_قدر_precise_t); \
    قسمتعریف خالی (*lv_property_سیٹ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    قسمتعریف خالی (*lv_property_سیٹ##type##_pointer_t)(lv_obj_t *, type, ثابت خالی *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(اشارہ_گر);

قسمتعریف خالی (*lv_property_سیٹ_نقطہ_t)(lv_obj_t *, lv_نقطہ_t *);
قسمتعریف lv_نتیجہ_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

قسمتعریف integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
قسمتعریف bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
قسمتعریف lv_قدر_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
قسمتعریف lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
قسمتعریف خالی * (*lv_property_get_اشارہ_گر_t)(ثابت lv_obj_t *);
قسمتعریف lv_نقطہ_t (*lv_property_get_نقطہ_t)(lv_obj_t *);

قسمتعریف lv_نتیجہ_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساکن lv_نتیجہ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سیٹ);
ساکن صحیح property_name_compare(ثابت خالی * ref, ثابت خالی * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_نتیجہ_t lv_obj_سیٹ_property(lv_obj_t * obj, ثابت lv_property_t * قدر)
{
    LV_CHECK_ARG(obj != تہی, واپس LV_نتیجہ_INVALID);
    LV_CHECK_ARG(قدر != تہی, واپس LV_نتیجہ_INVALID);

    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(قدر->id);
    اگر(قدر->id == LV_PROPERTY_ID_INVALID || اشاریہ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        واپس LV_نتیجہ_INVALID;
    }

    اگر(اشاریہ < LV_PROPERTY_ID_آغاز) {
        lv_obj_سیٹ_local_style_prop(obj, اشاریہ, قدر->style, قدر->selector);
        واپس LV_نتیجہ_OK;
    }

    واپس obj_property(obj, قدر->id, (lv_property_t *)قدر, true);
}

lv_نتیجہ_t lv_obj_سیٹ_properties(lv_obj_t * obj, ثابت lv_property_t * قدر, uint32_t گنتی)
{
    LV_CHECK_ARG(obj != تہی, واپس LV_نتیجہ_INVALID);
    LV_CHECK_ARG(قدر != تہی, واپس LV_نتیجہ_INVALID);

    برائے(uint32_t i = 0; i < گنتی; i++) {
        lv_نتیجہ_t نتیجہ = lv_obj_سیٹ_property(obj, &قدر[i]);
        اگر(نتیجہ != LV_نتیجہ_OK) {
            واپس نتیجہ;
        }
    }

    واپس LV_نتیجہ_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != تہی, واپس (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_نتیجہ_t نتیجہ;
    lv_property_t قدر = { 0 };

    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);
    اگر(id == LV_PROPERTY_ID_INVALID || اشاریہ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.عدد = 0;
        واپس قدر;
    }

    اگر(اشاریہ < LV_PROPERTY_ID_آغاز) {
        قدر.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, اشاریہ);
        قدر.id = id;
        قدر.selector = LV_PART_MAIN | obj->state;
        واپس قدر;
    }

    نتیجہ = obj_property(obj, id, &قدر, false);
    اگر(نتیجہ != LV_نتیجہ_OK)
        قدر.id = LV_PROPERTY_ID_INVALID;

    واپس قدر;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != تہی, واپس (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t قدر;
    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);

    اگر(اشاریہ == LV_PROPERTY_ID_INVALID || اشاریہ >= LV_PROPERTY_ID_آغاز) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.عدد = 0;
        واپس قدر;
    }

    قدر.style = lv_obj_get_style_prop(obj, part, اشاریہ);
    قدر.id = id;
    قدر.selector = part | obj->state;
    واپس قدر;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != تہی, واپس LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * ملا;
    /*Check style property*/
    ملا = lv_utils_bsearch(name, lv_style_property_نام, حجم(lv_style_property_نام) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    اگر(ملا) واپس ملا->id;
#ورنہ
    LV_غیر_مستعمل(name);
#اگر_ختم
    واپس LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != تہی, واپس LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != تہی, واپس LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * نام;
    lv_property_name_t * ملا;

    نام = clz->property_نام;
    اگر(نام == تہی) {
        /* try base class*/
        واپس LV_PROPERTY_ID_INVALID;
    }

    ملا = lv_utils_bsearch(name, نام, clz->نام_گنتی, حجم(lv_property_name_t), property_name_compare);
    اگر(ملا) واپس ملا->id;
#ورنہ
    LV_غیر_مستعمل(clz);
    LV_غیر_مستعمل(name);
    LV_غیر_مستعمل(property_name_compare);
#اگر_ختم
    واپس LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != تہی, واپس LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != تہی, واپس LV_PROPERTY_ID_INVALID);
#اگر LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    برائے(clz = obj->class_p; clz; clz = clz->بنیاد_class) {
        id = lv_obj_class_property_get_id(clz, name);
        اگر(id != LV_PROPERTY_ID_INVALID) واپس id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    اگر(id != LV_PROPERTY_ID_INVALID) واپس id;
#ورنہ
    LV_غیر_مستعمل(obj);
    LV_غیر_مستعمل(name);
    LV_غیر_مستعمل(property_name_compare);
#اگر_ختم
    واپس LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساکن lv_نتیجہ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سیٹ)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);

    برائے(clz = obj->class_p ; clz; clz = clz->بنیاد_class) {
        properties = clz->properties;
        اگر(properties == تہی) {
            /* try base class*/
            جاری;
        }

        اگر(id != LV_PROPERTY_ID_ANY && (اشاریہ < clz->prop_اشاریہ_آغاز || اشاریہ > clz->prop_اشاریہ_انتہا)) {
            /* try base class*/
            جاری;
        }

        /*Check if there's setter available for this class*/
        برائے(uint32_t i = 0; i < clz->properties_گنتی; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            اگر(prop->id == LV_PROPERTY_ID_ANY) {
                قدر->id = prop->id;
                اگر(سیٹ) واپس ((lv_property_setter_t)prop->setter)(obj, id, قدر);
                ورنہ واپس ((lv_property_getter_t)prop->getter)(obj, id, قدر);
            }

            /*Not this id, check next*/
            اگر(prop->id != id)
                جاری;

            /*id matched but we got null pointer to functions*/
            اگر(سیٹ ? prop->setter == تہی : prop->getter == تہی) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, سیٹ ? "setter" : "getter", id);
                واپس LV_نتیجہ_INVALID;
            }

            /*Update value id if it's a read*/
            اگر(!سیٹ) قدر->id = prop->id;

            انتخاب(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالت LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, عدد);
                    توڑو;
                حالت LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    توڑو;
                حالت LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    توڑو;
                حالت LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    توڑو;
                حالت LV_PROPERTY_TYPE_اشارہ_گر:
                حالت LV_PROPERTY_TYPE_IMGSRC:
                حالت LV_PROPERTY_TYPE_متن:
                حالت LV_PROPERTY_TYPE_OBJ:
                حالت LV_PROPERTY_TYPE_DISPLAY:
                حالت LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(اشارہ_گر, ptr);
                    توڑو;
                حالت LV_PROPERTY_TYPE_نقطہ: {
                        lv_نقطہ_t * نقطہ = &قدر->نقطہ;
                        اگر(سیٹ)((lv_property_سیٹ_نقطہ_t)(prop->setter))(obj, نقطہ);
                        ورنہ *نقطہ = ((lv_property_get_نقطہ_t)(prop->getter))(obj);
                        توڑو;
                    }
                طےشدہ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        واپس LV_نتیجہ_INVALID;
                    }
            }

            واپس LV_نتیجہ_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    واپس LV_نتیجہ_INVALID;
}

ساکن صحیح property_name_compare(ثابت خالی * ref, ثابت خالی * element)
{
    ثابت lv_property_name_t * prop = element;
    واپس lv_strcmp(ref, prop->name);
}

#اگر_ختم /*LV_USE_OBJ_PROPERTY*/
