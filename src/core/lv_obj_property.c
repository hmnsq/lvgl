/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

#ಆದರೆ LV_USE_OBJ_PROPERTY

#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_utils.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#ವ್ಯಾಖ್ಯಾನಿಸು HANDLE_PROPERTY_TYPE(type, ಕ್ಷೇತ್ರ) \
    ಆದರೆ(!ಗಣ) { \
        ಮೌಲ್ಯ->ಕ್ಷೇತ್ರ = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ಇಲ್ಲದಿದ್ದರೆ { \
        ಆಯ್ಕೆ(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            ಸ್ಥಿತಿ LV_PROPERTY_ID_INVALID: \
                ((lv_property_ಗಣ##type##_t)(prop->setter))(obj, ಮೌಲ್ಯ->ಕ್ಷೇತ್ರ); \
                ನಿಲ್ಲಿಸು; \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_INT: \
                ((lv_property_ಗಣ##type##_integer_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.ಕ್ಷೇತ್ರ, ಮೌಲ್ಯ->arg2.ಅಂಕೆ); \
                ನಿಲ್ಲಿಸು; \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_ಗಣ##type##_boolean_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.ಕ್ಷೇತ್ರ, ಮೌಲ್ಯ->arg2.enable); \
                ನಿಲ್ಲಿಸು; \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_ಗಣ##type##_precise_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.ಕ್ಷೇತ್ರ, ಮೌಲ್ಯ->arg2.precise); \
                ನಿಲ್ಲಿಸು; \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_ಗಣ##type##_color_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.ಕ್ಷೇತ್ರ, ಮೌಲ್ಯ->arg2.color); \
                ನಿಲ್ಲಿಸು; \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_ಸೂಚಕ: \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_IMGSRC: \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_ಪಠ್ಯ: \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_OBJ: \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_DISPLAY: \
            ಸ್ಥಿತಿ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_ಗಣ##type##_pointer_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.ಕ್ಷೇತ್ರ, ಮೌಲ್ಯ->arg2.ptr); \
                ನಿಲ್ಲಿಸು; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int32_t integer;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ bool boolean;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_ಮೌಲ್ಯ_precise_t precise;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_color_t color;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ಸೂಚಕ;

#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_PROPERTY_SETTER_TYPES(type) \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_t)(lv_obj_t *, type); \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_integer_t)(lv_obj_t *, type, int32_t); \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_boolean_t)(lv_obj_t *, type, bool); \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_precise_t)(lv_obj_t *, type, lv_ಮೌಲ್ಯ_precise_t); \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ##type##_pointer_t)(lv_obj_t *, type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(ಸೂಚಕ);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*lv_property_ಗಣ_ಬಿಂದು_t)(lv_obj_t *, lv_ಬಿಂದು_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_ಫಲಿತಾಂಶ_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ಸ್ಥಿರಾಂಕ lv_property_t *);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ integer(*lv_property_get_integer_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ bool (*lv_property_get_boolean_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_ಮೌಲ್ಯ_precise_t (*lv_property_get_precise_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_color_t (*lv_property_get_color_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ * (*lv_property_get_ಸೂಚಕ_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_ಬಿಂದು_t (*lv_property_get_ಬಿಂದು_t)(lv_obj_t *);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ lv_ಫಲಿತಾಂಶ_t (*lv_property_getter_t)(ಸ್ಥಿರಾಂಕ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ಮೌಲ್ಯ, bool ಗಣ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ property_name_compare(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ref, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ಫಲಿತಾಂಶ_t lv_obj_ಗಣ_property(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_property_t * ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    uint32_t ಸೂಚ್ಯಂಕ = LV_PROPERTY_ID_ಸೂಚ್ಯಂಕ(ಮೌಲ್ಯ->id);
    ಆದರೆ(ಮೌಲ್ಯ->id == LV_PROPERTY_ID_INVALID || ಸೂಚ್ಯಂಕ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }

    ಆದರೆ(ಸೂಚ್ಯಂಕ < LV_PROPERTY_ID_ಆರಂಭ) {
        lv_obj_ಗಣ_local_style_prop(obj, ಸೂಚ್ಯಂಕ, ಮೌಲ್ಯ->style, ಮೌಲ್ಯ->selector);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }

    ಹಿಂತಿರುಗಿ obj_property(obj, ಮೌಲ್ಯ->id, (lv_property_t *)ಮೌಲ್ಯ, true);
}

lv_ಫಲಿತಾಂಶ_t lv_obj_ಗಣ_properties(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_property_t * ಮೌಲ್ಯ, uint32_t ಎಣಿಕೆ_2)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    ಗಾಗಿ(uint32_t i = 0; i < ಎಣಿಕೆ_2; i++) {
        lv_ಫಲಿತಾಂಶ_t ಫಲಿತಾಂಶ = lv_obj_ಗಣ_property(obj, &ಮೌಲ್ಯ[i]);
        ಆದರೆ(ಫಲಿತಾಂಶ != LV_ಫಲಿತಾಂಶ_OK) {
            ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
        }
    }

    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_ಫಲಿತಾಂಶ_t ಫಲಿತಾಂಶ;
    lv_property_t ಮೌಲ್ಯ = { 0 };

    uint32_t ಸೂಚ್ಯಂಕ = LV_PROPERTY_ID_ಸೂಚ್ಯಂಕ(id);
    ಆದರೆ(id == LV_PROPERTY_ID_INVALID || ಸೂಚ್ಯಂಕ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;
        ಮೌಲ್ಯ.ಅಂಕೆ = 0;
        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
    }

    ಆದರೆ(ಸೂಚ್ಯಂಕ < LV_PROPERTY_ID_ಆರಂಭ) {
        ಮೌಲ್ಯ.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, ಸೂಚ್ಯಂಕ);
        ಮೌಲ್ಯ.id = id;
        ಮೌಲ್ಯ.selector = LV_PART_MAIN | obj->state;
        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
    }

    ಫಲಿತಾಂಶ = obj_property(obj, id, &ಮೌಲ್ಯ, false);
    ಆದರೆ(ಫಲಿತಾಂಶ != LV_ಫಲಿತಾಂಶ_OK)
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;

    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t ಮೌಲ್ಯ;
    uint32_t ಸೂಚ್ಯಂಕ = LV_PROPERTY_ID_ಸೂಚ್ಯಂಕ(id);

    ಆದರೆ(ಸೂಚ್ಯಂಕ == LV_PROPERTY_ID_INVALID || ಸೂಚ್ಯಂಕ >= LV_PROPERTY_ID_ಆರಂಭ) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;
        ಮೌಲ್ಯ.ಅಂಕೆ = 0;
        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
    }

    ಮೌಲ್ಯ.style = lv_obj_get_style_prop(obj, part, ಸೂಚ್ಯಂಕ);
    ಮೌಲ್ಯ.id = id;
    ಮೌಲ್ಯ.selector = part | obj->state;
    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

lv_prop_id_t lv_style_property_get_id(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_ARG(name != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID);
#ಆದರೆ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * ಸಿಕ್ಕಿತು;
    /*Check style property*/
    ಸಿಕ್ಕಿತು = lv_utils_bsearch(name, lv_style_property_ಹೆಸರುಗಳು, ಗಾತ್ರ(lv_style_property_ಹೆಸರುಗಳು) / ಗಾತ್ರ(lv_property_name_t),
                             ಗಾತ್ರ(lv_property_name_t), property_name_compare);
    ಆದರೆ(ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು->id;
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(name);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ಸ್ಥಿರಾಂಕ lv_obj_class_t * clz, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_ARG(clz != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID);
#ಆದರೆ LV_USE_OBJ_PROPERTY_NAME
    ಸ್ಥಿರಾಂಕ lv_property_name_t * ಹೆಸರುಗಳು;
    lv_property_name_t * ಸಿಕ್ಕಿತು;

    ಹೆಸರುಗಳು = clz->property_ಹೆಸರುಗಳು;
    ಆದರೆ(ಹೆಸರುಗಳು == ಖಾಲಿ) {
        /* try base class*/
        ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID;
    }

    ಸಿಕ್ಕಿತು = lv_utils_bsearch(name, ಹೆಸರುಗಳು, clz->ಹೆಸರುಗಳು_ಎಣಿಕೆ, ಗಾತ್ರ(lv_property_name_t), property_name_compare);
    ಆದರೆ(ಸಿಕ್ಕಿತು) ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು->id;
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(clz);
    LV_ಬಳಸದ(name);
    LV_ಬಳಸದ(property_name_compare);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID);
#ಆದರೆ LV_USE_OBJ_PROPERTY_NAME

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * clz;
    lv_prop_id_t id;

    ಗಾಗಿ(clz = obj->class_p; clz; clz = clz->ತಳ_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ಆದರೆ(id != LV_PROPERTY_ID_INVALID) ಹಿಂತಿರುಗಿ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ಆದರೆ(id != LV_PROPERTY_ID_INVALID) ಹಿಂತಿರುಗಿ id;
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
    LV_ಬಳಸದ(name);
    LV_ಬಳಸದ(property_name_compare);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ಮೌಲ್ಯ, bool ಗಣ)
{
    ಸ್ಥಿರಾಂಕ lv_property_ops_t * properties;
    ಸ್ಥಿರಾಂಕ lv_property_ops_t * prop;

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * clz;
    uint32_t ಸೂಚ್ಯಂಕ = LV_PROPERTY_ID_ಸೂಚ್ಯಂಕ(id);

    ಗಾಗಿ(clz = obj->class_p ; clz; clz = clz->ತಳ_class) {
        properties = clz->properties;
        ಆದರೆ(properties == ಖಾಲಿ) {
            /* try base class*/
            ಮುಂದುವರಿ;
        }

        ಆದರೆ(id != LV_PROPERTY_ID_ANY && (ಸೂಚ್ಯಂಕ < clz->prop_ಸೂಚ್ಯಂಕ_ಆರಂಭ || ಸೂಚ್ಯಂಕ > clz->prop_ಸೂಚ್ಯಂಕ_ಕೊನೆ)) {
            /* try base class*/
            ಮುಂದುವರಿ;
        }

        /*Check if there's setter available for this class*/
        ಗಾಗಿ(uint32_t i = 0; i < clz->properties_ಎಣಿಕೆ; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ಆದರೆ(prop->id == LV_PROPERTY_ID_ANY) {
                ಮೌಲ್ಯ->id = prop->id;
                ಆದರೆ(ಗಣ) ಹಿಂತಿರುಗಿ ((lv_property_setter_t)prop->setter)(obj, id, ಮೌಲ್ಯ);
                ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ ((lv_property_getter_t)prop->getter)(obj, id, ಮೌಲ್ಯ);
            }

            /*Not this id, check next*/
            ಆದರೆ(prop->id != id)
                ಮುಂದುವರಿ;

            /*id matched but we got null pointer to functions*/
            ಆದರೆ(ಗಣ ? prop->setter == ಖಾಲಿ : prop->getter == ಖಾಲಿ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, ಗಣ ? "setter" : "getter", id);
                ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
            }

            /*Update value id if it's a read*/
            ಆದರೆ(!ಗಣ) ಮೌಲ್ಯ->id = prop->id;

            ಆಯ್ಕೆ(LV_PROPERTY_ID_TYPE(prop->id)) {
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, ಅಂಕೆ);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_ಸೂಚಕ:
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_IMGSRC:
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_ಪಠ್ಯ:
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_OBJ:
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_DISPLAY:
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(ಸೂಚಕ, ptr);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ LV_PROPERTY_TYPE_ಬಿಂದು: {
                        lv_ಬಿಂದು_t * ಬಿಂದು = &ಮೌಲ್ಯ->ಬಿಂದು;
                        ಆದರೆ(ಗಣ)((lv_property_ಗಣ_ಬಿಂದು_t)(prop->setter))(obj, ಬಿಂದು);
                        ಇಲ್ಲದಿದ್ದರೆ *ಬಿಂದು = ((lv_property_get_ಬಿಂದು_t)(prop->getter))(obj);
                        ನಿಲ್ಲಿಸು;
                    }
                ಪೂರ್ವನಿಯೋಜಿತ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
                    }
            }

            ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ property_name_compare(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ref, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * element)
{
    ಸ್ಥಿರಾಂಕ lv_property_name_t * prop = element;
    ಹಿಂತಿರುಗಿ lv_strcmp(ref, prop->name);
}

#ಆದರೆ_ಅಂತ್ಯ /*LV_USE_OBJ_PROPERTY*/
