/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

#ਜੇ LV_USE_OBJ_PROPERTY

#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HANDLE_PROPERTY_TYPE(type, ਖੇਤਰ) \
    ਜੇ(!ਸੈੱਟ) { \
        ਮੁੱਲ->ਖੇਤਰ = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ਨਹੀਂਤਾਂ { \
        ਚੋਣ(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            ਹਾਲਤ LV_PROPERTY_ID_INVALID: \
                ((lv_property_ਸੈੱਟ##type##_t)(prop->setter))(obj, ਮੁੱਲ->ਖੇਤਰ); \
                ਰੋਕੋ; \
            ਹਾਲਤ LV_PROPERTY_TYPE_INT: \
                ((lv_property_ਸੈੱਟ##type##_integer_t)(prop->setter))(obj, ਮੁੱਲ->arg1.ਖੇਤਰ, ਮੁੱਲ->arg2.ਅੰਕ); \
                ਰੋਕੋ; \
            ਹਾਲਤ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_ਸੈੱਟ##type##_boolean_t)(prop->setter))(obj, ਮੁੱਲ->arg1.ਖੇਤਰ, ਮੁੱਲ->arg2.enable); \
                ਰੋਕੋ; \
            ਹਾਲਤ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_ਸੈੱਟ##type##_precise_t)(prop->setter))(obj, ਮੁੱਲ->arg1.ਖੇਤਰ, ਮੁੱਲ->arg2.precise); \
                ਰੋਕੋ; \
            ਹਾਲਤ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_ਸੈੱਟ##type##_color_t)(prop->setter))(obj, ਮੁੱਲ->arg1.ਖੇਤਰ, ਮੁੱਲ->arg2.color); \
                ਰੋਕੋ; \
            ਹਾਲਤ LV_PROPERTY_TYPE_ਸੂਚਕ: \
            ਹਾਲਤ LV_PROPERTY_TYPE_IMGSRC: \
            ਹਾਲਤ LV_PROPERTY_TYPE_ਪਾਠ: \
            ਹਾਲਤ LV_PROPERTY_TYPE_OBJ: \
            ਹਾਲਤ LV_PROPERTY_TYPE_DISPLAY: \
            ਹਾਲਤ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_ਸੈੱਟ##type##_pointer_t)(prop->setter))(obj, ਮੁੱਲ->arg1.ਖੇਤਰ, ਮੁੱਲ->arg2.ptr); \
                ਰੋਕੋ; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ int32_t integer;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ bool boolean;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_ਮੁੱਲ_precise_t precise;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_color_t color;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ ਖਾਲੀ * ਸੂਚਕ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_PROPERTY_SETTER_TYPES(type) \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_t)(lv_obj_t *, type); \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_boolean_t)(lv_obj_t *, type, bool); \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_precise_t)(lv_obj_t *, type, lv_ਮੁੱਲ_precise_t); \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ##type##_pointer_t)(lv_obj_t *, type, ਅਚਲ ਖਾਲੀ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(ਸੂਚਕ);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*lv_property_ਸੈੱਟ_ਬਿੰਦੂ_t)(lv_obj_t *, lv_ਬਿੰਦੂ_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_ਨਤੀਜਾ_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ਅਚਲ lv_property_t *);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ integer(*lv_property_get_integer_t)(ਅਚਲ lv_obj_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ bool (*lv_property_get_boolean_t)(ਅਚਲ lv_obj_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_ਮੁੱਲ_precise_t (*lv_property_get_precise_t)(ਅਚਲ lv_obj_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_color_t (*lv_property_get_color_t)(ਅਚਲ lv_obj_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ * (*lv_property_get_ਸੂਚਕ_t)(ਅਚਲ lv_obj_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_ਬਿੰਦੂ_t (*lv_property_get_ਬਿੰਦੂ_t)(lv_obj_t *);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ lv_ਨਤੀਜਾ_t (*lv_property_getter_t)(ਅਚਲ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ਸਥਿਰ lv_ਨਤੀਜਾ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ਮੁੱਲ, bool ਸੈੱਟ);
ਸਥਿਰ ਪੂਰਨ property_name_compare(ਅਚਲ ਖਾਲੀ * ref, ਅਚਲ ਖਾਲੀ * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ਨਤੀਜਾ_t lv_obj_ਸੈੱਟ_property(lv_obj_t * obj, ਅਚਲ lv_property_t * ਮੁੱਲ)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);
    LV_CHECK_ARG(ਮੁੱਲ != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    uint32_t ਸੂਚਕਾਂਕ = LV_PROPERTY_ID_ਸੂਚਕਾਂਕ(ਮੁੱਲ->id);
    ਜੇ(ਮੁੱਲ->id == LV_PROPERTY_ID_INVALID || ਸੂਚਕਾਂਕ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }

    ਜੇ(ਸੂਚਕਾਂਕ < LV_PROPERTY_ID_ਸ਼ੁਰੂ) {
        lv_obj_ਸੈੱਟ_local_style_prop(obj, ਸੂਚਕਾਂਕ, ਮੁੱਲ->style, ਮੁੱਲ->selector);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }

    ਵਾਪਸ obj_property(obj, ਮੁੱਲ->id, (lv_property_t *)ਮੁੱਲ, true);
}

lv_ਨਤੀਜਾ_t lv_obj_ਸੈੱਟ_properties(lv_obj_t * obj, ਅਚਲ lv_property_t * ਮੁੱਲ, uint32_t ਗਿਣਤੀ_2)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);
    LV_CHECK_ARG(ਮੁੱਲ != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    ਲਈ(uint32_t i = 0; i < ਗਿਣਤੀ_2; i++) {
        lv_ਨਤੀਜਾ_t ਨਤੀਜਾ = lv_obj_ਸੈੱਟ_property(obj, &ਮੁੱਲ[i]);
        ਜੇ(ਨਤੀਜਾ != LV_ਨਤੀਜਾ_OK) {
            ਵਾਪਸ ਨਤੀਜਾ;
        }
    }

    ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_ਨਤੀਜਾ_t ਨਤੀਜਾ;
    lv_property_t ਮੁੱਲ = { 0 };

    uint32_t ਸੂਚਕਾਂਕ = LV_PROPERTY_ID_ਸੂਚਕਾਂਕ(id);
    ਜੇ(id == LV_PROPERTY_ID_INVALID || ਸੂਚਕਾਂਕ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        ਮੁੱਲ.id = LV_PROPERTY_ID_INVALID;
        ਮੁੱਲ.ਅੰਕ = 0;
        ਵਾਪਸ ਮੁੱਲ;
    }

    ਜੇ(ਸੂਚਕਾਂਕ < LV_PROPERTY_ID_ਸ਼ੁਰੂ) {
        ਮੁੱਲ.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, ਸੂਚਕਾਂਕ);
        ਮੁੱਲ.id = id;
        ਮੁੱਲ.selector = LV_PART_MAIN | obj->state;
        ਵਾਪਸ ਮੁੱਲ;
    }

    ਨਤੀਜਾ = obj_property(obj, id, &ਮੁੱਲ, false);
    ਜੇ(ਨਤੀਜਾ != LV_ਨਤੀਜਾ_OK)
        ਮੁੱਲ.id = LV_PROPERTY_ID_INVALID;

    ਵਾਪਸ ਮੁੱਲ;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t ਮੁੱਲ;
    uint32_t ਸੂਚਕਾਂਕ = LV_PROPERTY_ID_ਸੂਚਕਾਂਕ(id);

    ਜੇ(ਸੂਚਕਾਂਕ == LV_PROPERTY_ID_INVALID || ਸੂਚਕਾਂਕ >= LV_PROPERTY_ID_ਸ਼ੁਰੂ) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        ਮੁੱਲ.id = LV_PROPERTY_ID_INVALID;
        ਮੁੱਲ.ਅੰਕ = 0;
        ਵਾਪਸ ਮੁੱਲ;
    }

    ਮੁੱਲ.style = lv_obj_get_style_prop(obj, part, ਸੂਚਕਾਂਕ);
    ਮੁੱਲ.id = id;
    ਮੁੱਲ.selector = part | obj->state;
    ਵਾਪਸ ਮੁੱਲ;
}

lv_prop_id_t lv_style_property_get_id(ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_ARG(name != ਨਿਰਾ, ਵਾਪਸ LV_PROPERTY_ID_INVALID);
#ਜੇ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * ਮਿਲਿਆ;
    /*Check style property*/
    ਮਿਲਿਆ = lv_utils_bsearch(name, lv_style_property_ਨਾਂ, ਆਕਾਰ(lv_style_property_ਨਾਂ) / ਆਕਾਰ(lv_property_name_t),
                             ਆਕਾਰ(lv_property_name_t), property_name_compare);
    ਜੇ(ਮਿਲਿਆ) ਵਾਪਸ ਮਿਲਿਆ->id;
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(name);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ਅਚਲ lv_obj_class_t * clz, ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_ARG(clz != ਨਿਰਾ, ਵਾਪਸ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ਨਿਰਾ, ਵਾਪਸ LV_PROPERTY_ID_INVALID);
#ਜੇ LV_USE_OBJ_PROPERTY_NAME
    ਅਚਲ lv_property_name_t * ਨਾਂ;
    lv_property_name_t * ਮਿਲਿਆ;

    ਨਾਂ = clz->property_ਨਾਂ;
    ਜੇ(ਨਾਂ == ਨਿਰਾ) {
        /* try base class*/
        ਵਾਪਸ LV_PROPERTY_ID_INVALID;
    }

    ਮਿਲਿਆ = lv_utils_bsearch(name, ਨਾਂ, clz->ਨਾਂ_ਗਿਣਤੀ, ਆਕਾਰ(lv_property_name_t), property_name_compare);
    ਜੇ(ਮਿਲਿਆ) ਵਾਪਸ ਮਿਲਿਆ->id;
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(clz);
    LV_ਅਣਵਰਤਿਆ(name);
    LV_ਅਣਵਰਤਿਆ(property_name_compare);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ਅਚਲ lv_obj_t * obj, ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ਨਿਰਾ, ਵਾਪਸ LV_PROPERTY_ID_INVALID);
#ਜੇ LV_USE_OBJ_PROPERTY_NAME

    ਅਚਲ lv_obj_class_t * clz;
    lv_prop_id_t id;

    ਲਈ(clz = obj->class_p; clz; clz = clz->ਅਧਾਰ_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ਜੇ(id != LV_PROPERTY_ID_INVALID) ਵਾਪਸ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ਜੇ(id != LV_PROPERTY_ID_INVALID) ਵਾਪਸ id;
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_ਅਣਵਰਤਿਆ(name);
    LV_ਅਣਵਰਤਿਆ(property_name_compare);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ਸਥਿਰ lv_ਨਤੀਜਾ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ਮੁੱਲ, bool ਸੈੱਟ)
{
    ਅਚਲ lv_property_ops_t * properties;
    ਅਚਲ lv_property_ops_t * prop;

    ਅਚਲ lv_obj_class_t * clz;
    uint32_t ਸੂਚਕਾਂਕ = LV_PROPERTY_ID_ਸੂਚਕਾਂਕ(id);

    ਲਈ(clz = obj->class_p ; clz; clz = clz->ਅਧਾਰ_class) {
        properties = clz->properties;
        ਜੇ(properties == ਨਿਰਾ) {
            /* try base class*/
            ਜਾਰੀ;
        }

        ਜੇ(id != LV_PROPERTY_ID_ANY && (ਸੂਚਕਾਂਕ < clz->prop_ਸੂਚਕਾਂਕ_ਸ਼ੁਰੂ || ਸੂਚਕਾਂਕ > clz->prop_ਸੂਚਕਾਂਕ_ਸਿਰਾ)) {
            /* try base class*/
            ਜਾਰੀ;
        }

        /*Check if there's setter available for this class*/
        ਲਈ(uint32_t i = 0; i < clz->properties_ਗਿਣਤੀ; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ਜੇ(prop->id == LV_PROPERTY_ID_ANY) {
                ਮੁੱਲ->id = prop->id;
                ਜੇ(ਸੈੱਟ) ਵਾਪਸ ((lv_property_setter_t)prop->setter)(obj, id, ਮੁੱਲ);
                ਨਹੀਂਤਾਂ ਵਾਪਸ ((lv_property_getter_t)prop->getter)(obj, id, ਮੁੱਲ);
            }

            /*Not this id, check next*/
            ਜੇ(prop->id != id)
                ਜਾਰੀ;

            /*id matched but we got null pointer to functions*/
            ਜੇ(ਸੈੱਟ ? prop->setter == ਨਿਰਾ : prop->getter == ਨਿਰਾ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, ਸੈੱਟ ? "setter" : "getter", id);
                ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
            }

            /*Update value id if it's a read*/
            ਜੇ(!ਸੈੱਟ) ਮੁੱਲ->id = prop->id;

            ਚੋਣ(LV_PROPERTY_ID_TYPE(prop->id)) {
                ਹਾਲਤ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, ਅੰਕ);
                    ਰੋਕੋ;
                ਹਾਲਤ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ਰੋਕੋ;
                ਹਾਲਤ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ਰੋਕੋ;
                ਹਾਲਤ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ਰੋਕੋ;
                ਹਾਲਤ LV_PROPERTY_TYPE_ਸੂਚਕ:
                ਹਾਲਤ LV_PROPERTY_TYPE_IMGSRC:
                ਹਾਲਤ LV_PROPERTY_TYPE_ਪਾਠ:
                ਹਾਲਤ LV_PROPERTY_TYPE_OBJ:
                ਹਾਲਤ LV_PROPERTY_TYPE_DISPLAY:
                ਹਾਲਤ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(ਸੂਚਕ, ptr);
                    ਰੋਕੋ;
                ਹਾਲਤ LV_PROPERTY_TYPE_ਬਿੰਦੂ: {
                        lv_ਬਿੰਦੂ_t * ਬਿੰਦੂ = &ਮੁੱਲ->ਬਿੰਦੂ;
                        ਜੇ(ਸੈੱਟ)((lv_property_ਸੈੱਟ_ਬਿੰਦੂ_t)(prop->setter))(obj, ਬਿੰਦੂ);
                        ਨਹੀਂਤਾਂ *ਬਿੰਦੂ = ((lv_property_get_ਬਿੰਦੂ_t)(prop->getter))(obj);
                        ਰੋਕੋ;
                    }
                ਮੂਲ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
                    }
            }

            ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
}

ਸਥਿਰ ਪੂਰਨ property_name_compare(ਅਚਲ ਖਾਲੀ * ref, ਅਚਲ ਖਾਲੀ * element)
{
    ਅਚਲ lv_property_name_t * prop = element;
    ਵਾਪਸ lv_strcmp(ref, prop->name);
}

#ਜੇ_ਅੰਤ /*LV_USE_OBJ_PROPERTY*/
