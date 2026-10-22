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

#تعریف_کړه HANDLE_PROPERTY_TYPE(type, ډګر) \
    إذا(!سېټ) { \
        ارزښت->ډګر = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } وإلا { \
        بدل(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            حالة LV_PROPERTY_ID_INVALID: \
                ((lv_property_سېټ##type##_t)(prop->setter))(obj, ارزښت->ډګر); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_INT: \
                ((lv_property_سېټ##type##_integer_t)(prop->setter))(obj, ارزښت->arg1.ډګر, ارزښت->arg2.عدد); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_سېټ##type##_boolean_t)(prop->setter))(obj, ارزښت->arg1.ډګر, ارزښت->arg2.enable); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_سېټ##type##_precise_t)(prop->setter))(obj, ارزښت->arg1.ډګر, ارزښت->arg2.precise); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_سېټ##type##_color_t)(prop->setter))(obj, ارزښت->arg1.ډګر, ارزښت->arg2.color); \
                اكسر; \
            حالة LV_PROPERTY_TYPE_اشاره_ګر: \
            حالة LV_PROPERTY_TYPE_IMGSRC: \
            حالة LV_PROPERTY_TYPE_متن: \
            حالة LV_PROPERTY_TYPE_OBJ: \
            حالة LV_PROPERTY_TYPE_DISPLAY: \
            حالة LV_PROPERTY_TYPE_FONT: \
                ((lv_property_سېټ##type##_pointer_t)(prop->setter))(obj, ارزښت->arg1.ډګر, ارزښت->arg2.ptr); \
                اكسر; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

تعريفنوع int32_t integer;
تعريفنوع bool boolean;
تعريفنوع lv_ارزښت_precise_t precise;
تعريفنوع lv_color_t color;
تعريفنوع ثابت فراغ * اشاره_ګر;

#تعریف_کړه DEFINE_PROPERTY_SETTER_TYPES(type) \
    تعريفنوع فراغ (*lv_property_سېټ##type##_t)(lv_obj_t *, type); \
    تعريفنوع فراغ (*lv_property_سېټ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    تعريفنوع فراغ (*lv_property_سېټ##type##_boolean_t)(lv_obj_t *, type, bool); \
    تعريفنوع فراغ (*lv_property_سېټ##type##_precise_t)(lv_obj_t *, type, lv_ارزښت_precise_t); \
    تعريفنوع فراغ (*lv_property_سېټ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    تعريفنوع فراغ (*lv_property_سېټ##type##_pointer_t)(lv_obj_t *, type, ثابت فراغ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(اشاره_ګر);

تعريفنوع فراغ (*lv_property_سېټ_ټکی_t)(lv_obj_t *, lv_ټکی_t *);
تعريفنوع lv_پایله_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ثابت lv_property_t *);

تعريفنوع integer(*lv_property_get_integer_t)(ثابت lv_obj_t *);
تعريفنوع bool (*lv_property_get_boolean_t)(ثابت lv_obj_t *);
تعريفنوع lv_ارزښت_precise_t (*lv_property_get_precise_t)(ثابت lv_obj_t *);
تعريفنوع lv_color_t (*lv_property_get_color_t)(ثابت lv_obj_t *);
تعريفنوع فراغ * (*lv_property_get_اشاره_ګر_t)(ثابت lv_obj_t *);
تعريفنوع lv_ټکی_t (*lv_property_get_ټکی_t)(lv_obj_t *);

تعريفنوع lv_پایله_t (*lv_property_getter_t)(ثابت lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ساكن lv_پایله_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ارزښت, bool سېټ);
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

lv_پایله_t lv_obj_سېټ_property(lv_obj_t * obj, ثابت lv_property_t * ارزښت)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_پایله_INVALID);
    LV_CHECK_ARG(ارزښت != عدم, أرجع LV_پایله_INVALID);

    uint32_t شاخص = LV_PROPERTY_ID_شاخص(ارزښت->id);
    إذا(ارزښت->id == LV_PROPERTY_ID_INVALID || شاخص > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        أرجع LV_پایله_INVALID;
    }

    إذا(شاخص < LV_PROPERTY_ID_پیل) {
        lv_obj_سېټ_local_style_prop(obj, شاخص, ارزښت->style, ارزښت->selector);
        أرجع LV_پایله_OK;
    }

    أرجع obj_property(obj, ارزښت->id, (lv_property_t *)ارزښت, true);
}

lv_پایله_t lv_obj_سېټ_properties(lv_obj_t * obj, ثابت lv_property_t * ارزښت, uint32_t شمېر)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_پایله_INVALID);
    LV_CHECK_ARG(ارزښت != عدم, أرجع LV_پایله_INVALID);

    لكل(uint32_t i = 0; i < شمېر; i++) {
        lv_پایله_t پایله = lv_obj_سېټ_property(obj, &ارزښت[i]);
        إذا(پایله != LV_پایله_OK) {
            أرجع پایله;
        }
    }

    أرجع LV_پایله_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_پایله_t پایله;
    lv_property_t ارزښت = { 0 };

    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);
    إذا(id == LV_PROPERTY_ID_INVALID || شاخص > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        ارزښت.id = LV_PROPERTY_ID_INVALID;
        ارزښت.عدد = 0;
        أرجع ارزښت;
    }

    إذا(شاخص < LV_PROPERTY_ID_پیل) {
        ارزښت.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, شاخص);
        ارزښت.id = id;
        ارزښت.selector = LV_PART_MAIN | obj->state;
        أرجع ارزښت;
    }

    پایله = obj_property(obj, id, &ارزښت, false);
    إذا(پایله != LV_پایله_OK)
        ارزښت.id = LV_PROPERTY_ID_INVALID;

    أرجع ارزښت;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != عدم, أرجع (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t ارزښت;
    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);

    إذا(شاخص == LV_PROPERTY_ID_INVALID || شاخص >= LV_PROPERTY_ID_پیل) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        ارزښت.id = LV_PROPERTY_ID_INVALID;
        ارزښت.عدد = 0;
        أرجع ارزښت;
    }

    ارزښت.style = lv_obj_get_style_prop(obj, part, شاخص);
    ارزښت.id = id;
    ارزښت.selector = part | obj->state;
    أرجع ارزښت;
}

lv_prop_id_t lv_style_property_get_id(ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * وموندل;
    /*Check style property*/
    وموندل = lv_utils_bsearch(name, lv_style_property_نومونه, حجم(lv_style_property_نومونه) / حجم(lv_property_name_t),
                             حجم(lv_property_name_t), property_name_compare);
    إذا(وموندل) أرجع وموندل->id;
#وإلا
    LV_ناکارول_شوی(name);
#د_که_پای
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ثابت lv_obj_class_t * clz, ثابت حرف * name)
{
    LV_CHECK_ARG(clz != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME
    ثابت lv_property_name_t * نومونه;
    lv_property_name_t * وموندل;

    نومونه = clz->property_نومونه;
    إذا(نومونه == عدم) {
        /* try base class*/
        أرجع LV_PROPERTY_ID_INVALID;
    }

    وموندل = lv_utils_bsearch(name, نومونه, clz->نومونه_شمېر, حجم(lv_property_name_t), property_name_compare);
    إذا(وموندل) أرجع وموندل->id;
#وإلا
    LV_ناکارول_شوی(clz);
    LV_ناکارول_شوی(name);
    LV_ناکارول_شوی(property_name_compare);
#د_که_پای
    أرجع LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ثابت lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_ARG(obj != عدم, أرجع LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != عدم, أرجع LV_PROPERTY_ID_INVALID);
#إذا LV_USE_OBJ_PROPERTY_NAME

    ثابت lv_obj_class_t * clz;
    lv_prop_id_t id;

    لكل(clz = obj->class_p; clz; clz = clz->بنسټ_class) {
        id = lv_obj_class_property_get_id(clz, name);
        إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    إذا(id != LV_PROPERTY_ID_INVALID) أرجع id;
#وإلا
    LV_ناکارول_شوی(obj);
    LV_ناکارول_شوی(name);
    LV_ناکارول_شوی(property_name_compare);
#د_که_پای
    أرجع LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ساكن lv_پایله_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ارزښت, bool سېټ)
{
    ثابت lv_property_ops_t * properties;
    ثابت lv_property_ops_t * prop;

    ثابت lv_obj_class_t * clz;
    uint32_t شاخص = LV_PROPERTY_ID_شاخص(id);

    لكل(clz = obj->class_p ; clz; clz = clz->بنسټ_class) {
        properties = clz->properties;
        إذا(properties == عدم) {
            /* try base class*/
            استمر;
        }

        إذا(id != LV_PROPERTY_ID_ANY && (شاخص < clz->prop_شاخص_پیل || شاخص > clz->prop_شاخص_پای_ټکی)) {
            /* try base class*/
            استمر;
        }

        /*Check if there's setter available for this class*/
        لكل(uint32_t i = 0; i < clz->properties_شمېر; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            إذا(prop->id == LV_PROPERTY_ID_ANY) {
                ارزښت->id = prop->id;
                إذا(سېټ) أرجع ((lv_property_setter_t)prop->setter)(obj, id, ارزښت);
                وإلا أرجع ((lv_property_getter_t)prop->getter)(obj, id, ارزښت);
            }

            /*Not this id, check next*/
            إذا(prop->id != id)
                استمر;

            /*id matched but we got null pointer to functions*/
            إذا(سېټ ? prop->setter == عدم : prop->getter == عدم) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, سېټ ? "setter" : "getter", id);
                أرجع LV_پایله_INVALID;
            }

            /*Update value id if it's a read*/
            إذا(!سېټ) ارزښت->id = prop->id;

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
                حالة LV_PROPERTY_TYPE_اشاره_ګر:
                حالة LV_PROPERTY_TYPE_IMGSRC:
                حالة LV_PROPERTY_TYPE_متن:
                حالة LV_PROPERTY_TYPE_OBJ:
                حالة LV_PROPERTY_TYPE_DISPLAY:
                حالة LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(اشاره_ګر, ptr);
                    اكسر;
                حالة LV_PROPERTY_TYPE_ټکی: {
                        lv_ټکی_t * ټکی = &ارزښت->ټکی;
                        إذا(سېټ)((lv_property_سېټ_ټکی_t)(prop->setter))(obj, ټکی);
                        وإلا *ټکی = ((lv_property_get_ټکی_t)(prop->getter))(obj);
                        اكسر;
                    }
                افتراضي: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        أرجع LV_پایله_INVALID;
                    }
            }

            أرجع LV_پایله_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    أرجع LV_پایله_INVALID;
}

ساكن صحيح property_name_compare(ثابت فراغ * ref, ثابت فراغ * element)
{
    ثابت lv_property_name_t * prop = element;
    أرجع lv_strcmp(ref, prop->name);
}

#د_که_پای /*LV_USE_OBJ_PROPERTY*/
