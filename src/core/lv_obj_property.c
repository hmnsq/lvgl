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

#وصف_ڪريو HANDLE_PROPERTY_TYPE(type, ميدان) \
    إذا(!سيٽ) { \
        قدر->ميدان = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_سيٽ##type##_t)(prop->setter))(obj, قدر->ميدان); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_سيٽ##type##_integer_t)(prop->setter))(obj, قدر->arg1.ميدان, قدر->arg2.انگ); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_سيٽ##type##_boolean_t)(prop->setter))(obj, قدر->arg1.ميدان, قدر->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_سيٽ##type##_precise_t)(prop->setter))(obj, قدر->arg1.ميدان, قدر->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_سيٽ##type##_color_t)(prop->setter))(obj, قدر->arg1.ميدان, قدر->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_اشارو: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_متن: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_سيٽ##type##_pointer_t)(prop->setter))(obj, قدر->arg1.ميدان, قدر->arg2.ptr); \
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
تعريفنوع ثابت فراغ * اشارو;

#وصف_ڪريو DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_precise_t)(lv_obj_t *, type, lv_قدر_precise_t); \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعريفنوع فراغ (*lv_property_سيٽ##type##_pointer_t)(lv_obj_t *, type, ثابت فراغ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(اشارو);

تعريفنوع فراغ (*lv_property_سيٽ_نقطو_t)(lv_obj_t *, lv_نقطو_t *);
تعريفنوع lv_نتيجو_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعريفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعريفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعريفنوع lv_قدر_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_اشارو_t)(ثابت lv_obj_t *);
تعريفنوع lv_نقطو_t (*lv_property_get_نقطو_t)(lv_obj_t *);

تعريفنوع lv_نتيجو_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_نتيجو_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سيٽ);
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

lv_نتيجو_t lv_obj_سيٽ_property(lv_obj_t * obj, ثابت lv_property_t * قدر)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتيجو_INVALID);
    LV_CHECK_ARG(قدر != عدم, أرجع LV_نتيجو_INVALID);

    uint32_t اشاريو = LV_PROPERTY_ID_اشاريو(قدر->id);
    إذا(قدر->id == LV_PROPERTY_ID_INVALID || اشاريو > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        أرجع LV_نتيجو_INVALID;
    }

    إذا(اشاريو < LV_PROPERTY_ID_شروعات) {
        lv_obj_سيٽ_local_style_prop(obj, اشاريو, قدر->style, قدر->selector);
        أرجع LV_نتيجو_OK;
    }

    أرجع obj_property(obj, قدر->id, (lv_property_t *)قدر, true);
}

lv_نتيجو_t lv_obj_سيٽ_properties(lv_obj_t * obj, ثابت lv_property_t * قدر, uint32_t ڳڻپ)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتيجو_INVALID);
    LV_CHECK_ARG(قدر != عدم, أرجع LV_نتيجو_INVALID);

    لكل(uint32_t i = 0; i < ڳڻپ; i++) {
        lv_نتيجو_t نتيجو = lv_obj_سيٽ_property(obj, &قدر[i]);
        إذا(نتيجو != LV_نتيجو_OK) {
            أرجع نتيجو;
        }
    }

    أرجع LV_نتيجو_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_نتيجو_t نتيجو;
    lv_property_t قدر = { 0 };

    uint32_t اشاريو = LV_PROPERTY_ID_اشاريو(id);
    إذا(id == LV_PROPERTY_ID_INVALID || اشاريو > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.انگ = 0;
        أرجع قدر;
    }

    إذا(اشاريو < LV_PROPERTY_ID_شروعات) {
        قدر.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, اشاريو);
        قدر.id = id;
        قدر.selector = LV_PART_MAIN | obj->state;
        أرجع قدر;
    }

    نتيجو = obj_property(obj, id, &قدر, false);
    إذا(نتيجو != LV_نتيجو_OK)
        قدر.id = LV_PROPERTY_ID_INVALID;

    أرجع قدر;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t قدر;
    uint32_t اشاريو = LV_PROPERTY_ID_اشاريو(id);

    إذا(اشاريو == LV_PROPERTY_ID_INVALID || اشاريو >= LV_PROPERTY_ID_شروعات) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        قدر.id = LV_PROPERTY_ID_INVALID;
        قدر.انگ = 0;
        أرجع قدر;
    }

    قدر.style = lv_obj_get_style_prop(obj, part, اشاريو);
    قدر.id = id;
    قدر.selector = part | obj->state;
    أرجع قدر;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * مليو;
    /*Check style property*/
    مليو = lv_utils_bsearch(name, lv_style_property_نالا, حجم(lv_style_property_نالا) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(مليو) أرجع مليو->id;
#وإلا
    LV_اڻ_استعمال_ٿيل(name);
#جيڪڏهن_پورو
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * نالا;
    lv_property_name_t * مليو;

    نالا = clz->property_نالا;
    إذا(نالا == عدم) {
        /* try base class*/
        أرجع LV_PROPERTY_ID_INVALID;
    }

    مليو = lv_utils_bsearch(name, نالا, clz->نالا_ڳڻپ, حجم(lv_property_name_t), property_name_compare);
    إذا(مليو) أرجع مليو->id;
#وإلا
    LV_اڻ_استعمال_ٿيل(clz);
    LV_اڻ_استعمال_ٿيل(name);
    LV_اڻ_استعمال_ٿيل(property_name_compare);
#جيڪڏهن_پورو
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->بنياد_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
#وإلا
    LV_اڻ_استعمال_ٿيل(obj);
    LV_اڻ_استعمال_ٿيل(name);
    LV_اڻ_استعمال_ٿيل(property_name_compare);
#جيڪڏهن_پورو
    أرجع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_نتيجو_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * قدر, bool سيٽ)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t اشاريو = LV_PROPERTY_ID_اشاريو(id);

    لكل(clz = obj->class_p ; clz; clz = clz->بنياد_class) {
        properties = clz->properties;
        إذا(properties == عدم) {
            /* try base class*/
            استمر;
        }

        إذا(id != LV_PROPERTY_ID_ANY && (اشاريو < clz->prop_اشاريو_شروعات || اشاريو > clz->prop_اشاريو_پڇاڙي)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        لكل(uint32_t i = 0; i < clz->properties_ڳڻپ; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                قدر->id = prop->id;
                إذا(سيٽ) أرجع ((lv_property_setter_t)prop->setter)(obj, id, قدر);
                وإلا أرجع ((lv_property_getter_t)prop->getter)(obj, id, قدر);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(سيٽ ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, سيٽ ? "setter" : "getter", id);
                أرجع LV_نتيجو_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!سيٽ) قدر->id = prop->id;

            بدل(LV_PROPERTY_ID_TYPE(prop->id)) {
                حالة LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, انگ);
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
                حالة LV_PROPERTY_TYPE_اشارو:
                حالة LV_PROPERTY_TYPE_IMGSRC:
                حالة LV_PROPERTY_TYPE_متن:
                حالة LV_PROPERTY_TYPE_OBJ:
                حالة LV_PROPERTY_TYPE_DISPLAY:
                حالة LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(اشارو, ptr);
                    اكسر;
                حالة LV_PROPERTY_TYPE_نقطو: {
                        lv_نقطو_t * نقطو = &قدر->نقطو;
                        إذا(سيٽ)((lv_property_سيٽ_نقطو_t)(prop->setter))(obj, نقطو);
                        وإلا *نقطو = ((lv_property_get_نقطو_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        أرجع LV_نتيجو_INVALID;
                    }
            }

            أرجع LV_نتيجو_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    أرجع LV_نتيجو_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    أرجع lv_strcmp(ref, prop->name);
}

#جيڪڏهن_پورو /*LV_USE_OBJ_PROPERTY*/
