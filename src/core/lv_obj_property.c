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

#تعریف_کرو HANDLE_PROPERTY_TYPE(type, میدان) \
    إذا(!سیٹ) { \
        قدر->میدان = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_سیٹ##type##_t)(prop->setter))(obj, قدر->میدان); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_سیٹ##type##_integer_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.عدد); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_سیٹ##type##_boolean_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_سیٹ##type##_precise_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_سیٹ##type##_color_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_اشارہ_گر: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_متن: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_سیٹ##type##_pointer_t)(prop->setter))(obj, قدر->arg1.میدان, قدر->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_قدر_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * اشارہ_گر;

#تعریف_کرو DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_precise_t)(lv_obj_t *, type, lv_قدر_precise_t); \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعريفنوع فراغ (*lv_property_سیٹ##type##_pointer_t)(lv_obj_t *, type, ثابت فراغ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(اشارہ_گر);

تعريفنوع فراغ (*lv_property_سیٹ_نقطہ_t)(lv_obj_t *, lv_نقطہ_t *);
تعريفنوع lv_نتیجہ_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعريفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعريفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعريفنوع lv_قدر_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_اشارہ_گر_t)(ثابت lv_obj_t *);
تعريفنوع lv_نقطہ_t (*lv_property_get_نقطہ_t)(lv_obj_t *);

تعريفنوع lv_نتیجہ_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_نتیجہ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سیٹ);
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

lv_نتیجہ_t lv_obj_سیٹ_property(lv_obj_t * obj, ثابت lv_property_t * قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتیجہ_INVALID);
    LV_CHECK_ARG(قدر != عدم, أرجع LV_نتیجہ_INVALID);

    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(قدر->id);
    إذا(قدر->id == LV_PROPERTY_ID_INVALID || اشاریہ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        أرجع LV_نتیجہ_INVALID;
    }

    إذا(اشاریہ < LV_PROPERTY_ID_شروع) {
        lv_obj_سیٹ_local_style_prop(obj, اشاریہ, قدر->style, قدر->selector);
        أرجع LV_نتیجہ_OK;
    }

    أرجع obj_property(obj, قدر->id, (lv_property_t *)قدر, true);
}

lv_نتیجہ_t lv_obj_سیٹ_properties(lv_obj_t * obj, ثابت lv_property_t * قدر, uint32_t گنتی)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتیجہ_INVALID);
    LV_CHECK_ARG(قدر != عدم, أرجع LV_نتیجہ_INVALID);

    لكل(uint32_t i = 0; i < گنتی; i++) {
        lv_نتیجہ_t نتیجہ = lv_obj_سیٹ_property(obj, &قدر[i]);
        إذا(نتیجہ != LV_نتیجہ_OK) {
            أرجع نتیجہ;
        }
    }

    أرجع LV_نتیجہ_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_نتیجہ_t نتیجہ;
    lv_property_t قدر = { 0 };

    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);
    إذا(id == LV_PROPERTY_ID_INVALID || اشاریہ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.عدد = 0;
        أرجع قدر;
    }

    إذا(اشاریہ < LV_PROPERTY_ID_شروع) {
        قدر.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, اشاریہ);
        قدر.id = id;
        قدر.selector = LV_PART_MAIN | obj->state;
        أرجع قدر;
    }

    نتیجہ = obj_property(obj, id, &قدر, false);
    إذا(نتیجہ != LV_نتیجہ_OK)
        قدر.id = LV_PROPERTY_ID_INVALID;

    أرجع قدر;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t قدر;
    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);

    إذا(اشاریہ == LV_PROPERTY_ID_INVALID || اشاریہ >= LV_PROPERTY_ID_شروع) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.عدد = 0;
        أرجع قدر;
    }

    قدر.style = lv_obj_get_style_prop(obj, part, اشاریہ);
    قدر.id = id;
    قدر.selector = part | obj->state;
    أرجع قدر;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * لبھیا;
    /*Check style property*/
    لبھیا = lv_utils_bsearch(name, lv_style_property_ناں, حجم(lv_style_property_ناں) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(لبھیا) أرجع لبھیا->id;
#وإلا
    LV_بن_ورتیا(name);
#جے_انت
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * ناں;
    lv_property_name_t * لبھیا;

    ناں = clz->property_ناں;
    إذا(ناں == عدم) {
        /* try base class*/
        أرجع LV_PROPERTY_ID_INVALID;
    }

    لبھیا = lv_utils_bsearch(name, ناں, clz->ناں_گنتی, حجم(lv_property_name_t), property_name_compare);
    إذا(لبھیا) أرجع لبھیا->id;
#وإلا
    LV_بن_ورتیا(clz);
    LV_بن_ورتیا(name);
    LV_بن_ورتیا(property_name_compare);
#جے_انت
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->ادھار_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
#وإلا
    LV_بن_ورتیا(obj);
    LV_بن_ورتیا(name);
    LV_بن_ورتیا(property_name_compare);
#جے_انت
    أرجع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_نتیجہ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سیٹ)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t اشاریہ = LV_PROPERTY_ID_اشاریہ(id);

    لكل(clz = obj->class_p ; clz; clz = clz->ادھار_class) {
        properties = clz->properties;
        إذا(properties == عدم) {
            /* try base class*/
            استمر;
        }

        إذا(id != LV_PROPERTY_ID_ANY && (اشاریہ < clz->prop_اشاریہ_شروع || اشاریہ > clz->prop_اشاریہ_سرا)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        لكل(uint32_t i = 0; i < clz->properties_گنتی; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                قدر->id = prop->id;
                إذا(سیٹ) أرجع ((lv_property_setter_t)prop->setter)(obj, id, قدر);
                وإلا أرجع ((lv_property_getter_t)prop->getter)(obj, id, قدر);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(سیٹ ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, سیٹ ? "setter" : "getter", id);
                أرجع LV_نتیجہ_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!سیٹ) قدر->id = prop->id;

            بدل(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالة LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, عدد);
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
                حالة LV_PROPERTY_TYPE_اشارہ_گر:
                حالة LV_PROPERTY_TYPE_IMGSRC:
                حالة LV_PROPERTY_TYPE_متن:
                حالة LV_PROPERTY_TYPE_OBJ:
                حالة LV_PROPERTY_TYPE_DISPLAY:
                حالة LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(اشارہ_گر, ptr);
                    اكسر;
                حالة LV_PROPERTY_TYPE_نقطہ: {
                        lv_نقطہ_t * نقطہ = &قدر->نقطہ;
                        إذا(سیٹ)((lv_property_سیٹ_نقطہ_t)(prop->setter))(obj, نقطہ);
                        وإلا *نقطہ = ((lv_property_get_نقطہ_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        أرجع LV_نتیجہ_INVALID;
                    }
            }

            أرجع LV_نتیجہ_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    أرجع LV_نتیجہ_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    أرجع lv_strcmp(ref, prop->name);
}

#جے_انت /*LV_USE_OBJ_PROPERTY*/
