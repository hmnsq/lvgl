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

#تعریف_کن HANDLE_PROPERTY_TYPE(type, فیلد) \
    إذا(!مجموعه) { \
        مقدار->فیلد = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_مجموعه##type##_t)(prop->setter))(obj, مقدار->فیلد); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_مجموعه##type##_integer_t)(prop->setter))(obj, مقدار->arg1.فیلد, مقدار->arg2.رقم); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_مجموعه##type##_boolean_t)(prop->setter))(obj, مقدار->arg1.فیلد, مقدار->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_مجموعه##type##_precise_t)(prop->setter))(obj, مقدار->arg1.فیلد, مقدار->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_مجموعه##type##_color_t)(prop->setter))(obj, مقدار->arg1.فیلد, مقدار->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_اشاره_گر: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_متن: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_مجموعه##type##_pointer_t)(prop->setter))(obj, مقدار->arg1.فیلد, مقدار->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_مقدار_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * اشاره_گر;

#تعریف_کن DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_precise_t)(lv_obj_t *, type, lv_مقدار_precise_t); \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعريفنوع فراغ (*lv_property_مجموعه##type##_pointer_t)(lv_obj_t *, type, ثابت فراغ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(اشاره_گر);

تعريفنوع فراغ (*lv_property_مجموعه_نقطه_t)(lv_obj_t *, lv_نقطه_t *);
تعريفنوع lv_نتیجه_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعريفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعريفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعريفنوع lv_مقدار_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_اشاره_گر_t)(ثابت lv_obj_t *);
تعريفنوع lv_نقطه_t (*lv_property_get_نقطه_t)(lv_obj_t *);

تعريفنوع lv_نتیجه_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_نتیجه_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * مقدار, bool مجموعه);
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

lv_نتیجه_t lv_obj_مجموعه_property(lv_obj_t * obj, ثابت lv_property_t * مقدار)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتیجه_INVALID);
    LV_CHECK_ARG(مقدار != عدم, أرجع LV_نتیجه_INVALID);

    uint32_t شاخص = LV_PROPERTY_ID_شاخص(مقدار->id);
    إذا(مقدار->id == LV_PROPERTY_ID_INVALID || شاخص > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        أرجع LV_نتیجه_INVALID;
    }

    إذا(شاخص < LV_PROPERTY_ID_شروع) {
        lv_obj_مجموعه_local_style_prop(obj, شاخص, مقدار->style, مقدار->selector);
        أرجع LV_نتیجه_OK;
    }

    أرجع obj_property(obj, مقدار->id, (lv_property_t *)مقدار, true);
}

lv_نتیجه_t lv_obj_مجموعه_properties(lv_obj_t * obj, ثابت lv_property_t * مقدار, uint32_t شمار)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_نتیجه_INVALID);
    LV_CHECK_ARG(مقدار != عدم, أرجع LV_نتیجه_INVALID);

    لكل(uint32_t i = 0; i < شمار; i++) {
        lv_نتیجه_t نتیجه = lv_obj_مجموعه_property(obj, &مقدار[i]);
        إذا(نتیجه != LV_نتیجه_OK) {
            أرجع نتیجه;
        }
    }

    أرجع LV_نتیجه_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_نتیجه_t نتیجه;
    lv_property_t مقدار = { 0 };

    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);
    إذا(id == LV_PROPERTY_ID_INVALID || شاخص > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        مقدار.id = LV_PROPERTY_ID_INVALID;
        مقدار.رقم = 0;
        أرجع مقدار;
    }

    إذا(شاخص < LV_PROPERTY_ID_شروع) {
        مقدار.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, شاخص);
        مقدار.id = id;
        مقدار.selector = LV_PART_MAIN | obj->state;
        أرجع مقدار;
    }

    نتیجه = obj_property(obj, id, &مقدار, false);
    إذا(نتیجه != LV_نتیجه_OK)
        مقدار.id = LV_PROPERTY_ID_INVALID;

    أرجع مقدار;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t مقدار;
    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);

    إذا(شاخص == LV_PROPERTY_ID_INVALID || شاخص >= LV_PROPERTY_ID_شروع) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        مقدار.id = LV_PROPERTY_ID_INVALID;
        مقدار.رقم = 0;
        أرجع مقدار;
    }

    مقدار.style = lv_obj_get_style_prop(obj, part, شاخص);
    مقدار.id = id;
    مقدار.selector = part | obj->state;
    أرجع مقدار;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * یافت;
    /*Check style property*/
    یافت = lv_utils_bsearch(name, lv_style_property_نامها, حجم(lv_style_property_نامها) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(یافت) أرجع یافت->id;
#وإلا
    LV_استفادهنشده(name);
#پایان_اگر
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * نامها;
    lv_property_name_t * یافت;

    نامها = clz->property_نامها;
    إذا(نامها == عدم) {
        /* try base class*/
        أرجع LV_PROPERTY_ID_INVALID;
    }

    یافت = lv_utils_bsearch(name, نامها, clz->نامها_شمار, حجم(lv_property_name_t), property_name_compare);
    إذا(یافت) أرجع یافت->id;
#وإلا
    LV_استفادهنشده(clz);
    LV_استفادهنشده(name);
    LV_استفادهنشده(property_name_compare);
#پایان_اگر
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->پایه_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
#وإلا
    LV_استفادهنشده(obj);
    LV_استفادهنشده(name);
    LV_استفادهنشده(property_name_compare);
#پایان_اگر
    أرجع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_نتیجه_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * مقدار, bool مجموعه)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);

    لكل(clz = obj->class_p ; clz; clz = clz->پایه_class) {
        properties = clz->properties;
        إذا(properties == عدم) {
            /* try base class*/
            استمر;
        }

        إذا(id != LV_PROPERTY_ID_ANY && (شاخص < clz->prop_شاخص_شروع || شاخص > clz->prop_شاخص_انتها)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        لكل(uint32_t i = 0; i < clz->properties_شمار; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                مقدار->id = prop->id;
                إذا(مجموعه) أرجع ((lv_property_setter_t)prop->setter)(obj, id, مقدار);
                وإلا أرجع ((lv_property_getter_t)prop->getter)(obj, id, مقدار);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(مجموعه ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, مجموعه ? "setter" : "getter", id);
                أرجع LV_نتیجه_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!مجموعه) مقدار->id = prop->id;

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
                حالة LV_PROPERTY_TYPE_اشاره_گر:
                حالة LV_PROPERTY_TYPE_IMGSRC:
                حالة LV_PROPERTY_TYPE_متن:
                حالة LV_PROPERTY_TYPE_OBJ:
                حالة LV_PROPERTY_TYPE_DISPLAY:
                حالة LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(اشاره_گر, ptr);
                    اكسر;
                حالة LV_PROPERTY_TYPE_نقطه: {
                        lv_نقطه_t * نقطه = &مقدار->نقطه;
                        إذا(مجموعه)((lv_property_مجموعه_نقطه_t)(prop->setter))(obj, نقطه);
                        وإلا *نقطه = ((lv_property_get_نقطه_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        أرجع LV_نتیجه_INVALID;
                    }
            }

            أرجع LV_نتیجه_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    أرجع LV_نتیجه_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    أرجع lv_strcmp(ref, prop->name);
}

#پایان_اگر /*LV_USE_OBJ_PROPERTY*/
