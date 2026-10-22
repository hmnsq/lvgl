/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

#ಯದಿ LV_USE_OBJ_PROPERTY

#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_utils.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    ಯದಿ(!set) { \
        ಮೌಲ್ಯ->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ಅನ್ಯಥಾ { \
        ವಿಕಲ್ಪ(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            ಪಕ್ಷ LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, ಮೌಲ್ಯ->field); \
                ಭಙ್ಗ; \
            ಪಕ್ಷ LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.field, ಮೌಲ್ಯ->arg2.ಅಂಕೆ); \
                ಭಙ್ಗ; \
            ಪಕ್ಷ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.field, ಮೌಲ್ಯ->arg2.enable); \
                ಭಙ್ಗ; \
            ಪಕ್ಷ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.field, ಮೌಲ್ಯ->arg2.precise); \
                ಭಙ್ಗ; \
            ಪಕ್ಷ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.field, ಮೌಲ್ಯ->arg2.color); \
                ಭಙ್ಗ; \
            ಪಕ್ಷ LV_PROPERTY_TYPE_POINTER: \
            ಪಕ್ಷ LV_PROPERTY_TYPE_IMGSRC: \
            ಪಕ್ಷ LV_PROPERTY_TYPE_TEXT: \
            ಪಕ್ಷ LV_PROPERTY_TYPE_OBJ: \
            ಪಕ್ಷ LV_PROPERTY_TYPE_DISPLAY: \
            ಪಕ್ಷ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, ಮೌಲ್ಯ->arg1.field, ಮೌಲ್ಯ->arg2.ptr); \
                ಭಙ್ಗ; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

ಪ್ರಕಾರನಿರ್ದೇಶ int32_t integer;
ಪ್ರಕಾರನಿರ್ದೇಶ bool boolean;
ಪ್ರಕಾರನಿರ್ದೇಶ lv_ಮೌಲ್ಯ_precise_t precise;
ಪ್ರಕಾರನಿರ್ದೇಶ lv_color_t color;
ಪ್ರಕಾರನಿರ್ದೇಶ ಧ್ರುವ ಶೂನ್ಯ * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_ಮೌಲ್ಯ_precise_t); \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ಧ್ರುವ ಶೂನ್ಯ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ಧ್ರುವ lv_property_t *);

ಪ್ರಕಾರನಿರ್ದೇಶ integer(*lv_property_get_integer_t)(ಧ್ರುವ lv_obj_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ bool (*lv_property_get_boolean_t)(ಧ್ರುವ lv_obj_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ lv_ಮೌಲ್ಯ_precise_t (*lv_property_get_precise_t)(ಧ್ರುವ lv_obj_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ lv_color_t (*lv_property_get_color_t)(ಧ್ರುವ lv_obj_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ * (*lv_property_get_pointer_t)(ಧ್ರುವ lv_obj_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

ಪ್ರಕಾರನಿರ್ದೇಶ lv_result_t (*lv_property_getter_t)(ಧ್ರುವ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ಸ್ಥಿರ lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ಮೌಲ್ಯ, bool set);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ property_name_compare(ಧ್ರುವ ಶೂನ್ಯ * ref, ಧ್ರುವ ಶೂನ್ಯ * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, ಧ್ರುವ lv_property_t * ಮೌಲ್ಯ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(ಮೌಲ್ಯ->id);
    ಯದಿ(ಮೌಲ್ಯ->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID;
    }

    ಯದಿ(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, ಮೌಲ್ಯ->style, ಮೌಲ್ಯ->selector);
        ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;
    }

    ಪ್ರತ್ಯಾಗಮ obj_property(obj, ಮೌಲ್ಯ->id, (lv_property_t *)ಮೌಲ್ಯ, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ಧ್ರುವ lv_property_t * ಮೌಲ್ಯ, uint32_t ಎಣಿಕೆ)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);
    LV_CHECK_ARG(ಮೌಲ್ಯ != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);

    ಪ್ರತಿ(uint32_t i = 0; i < ಎಣಿಕೆ; i++) {
        lv_result_t result = lv_obj_set_property(obj, &ಮೌಲ್ಯ[i]);
        ಯದಿ(result != LV_RESULT_OK) {
            ಪ್ರತ್ಯಾಗಮ result;
        }
    }

    ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t ಮೌಲ್ಯ = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    ಯದಿ(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;
        ಮೌಲ್ಯ.ಅಂಕೆ = 0;
        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

    ಯದಿ(index < LV_PROPERTY_ID_START) {
        ಮೌಲ್ಯ.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        ಮೌಲ್ಯ.id = id;
        ಮೌಲ್ಯ.selector = LV_PART_MAIN | obj->state;
        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

    result = obj_property(obj, id, &ಮೌಲ್ಯ, false);
    ಯದಿ(result != LV_RESULT_OK)
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;

    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t ಮೌಲ್ಯ;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    ಯದಿ(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        ಮೌಲ್ಯ.id = LV_PROPERTY_ID_INVALID;
        ಮೌಲ್ಯ.ಅಂಕೆ = 0;
        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
    }

    ಮೌಲ್ಯ.style = lv_obj_get_style_prop(obj, part, index);
    ಮೌಲ್ಯ.id = id;
    ಮೌಲ್ಯ.selector = part | obj->state;
    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}

lv_prop_id_t lv_style_property_get_id(ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_ARG(name != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID);
#ಯದಿ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, ಪರಿಮಾಣ(lv_style_property_names) / ಪರಿಮಾಣ(lv_property_name_t),
                             ಪರಿಮಾಣ(lv_property_name_t), property_name_compare);
    ಯದಿ(found) ಪ್ರತ್ಯಾಗಮ found->id;
#ಅನ್ಯಥಾ
    LV_UNUSED(name);
#endif
    ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ಧ್ರುವ lv_obj_class_t * clz, ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_ARG(clz != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID);
#ಯದಿ LV_USE_OBJ_PROPERTY_NAME
    ಧ್ರುವ lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    ಯದಿ(names == ರಿಕ್ತ) {
        /* try base class*/
        ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_ಎಣಿಕೆ, ಪರಿಮಾಣ(lv_property_name_t), property_name_compare);
    ಯದಿ(found) ಪ್ರತ್ಯಾಗಮ found->id;
#ಅನ್ಯಥಾ
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ಧ್ರುವ lv_obj_t * obj, ಧ್ರುವ ವರ್ಣ * name)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID);
#ಯದಿ LV_USE_OBJ_PROPERTY_NAME

    ಧ್ರುವ lv_obj_class_t * clz;
    lv_prop_id_t id;

    ಪ್ರತಿ(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ಯದಿ(id != LV_PROPERTY_ID_INVALID) ಪ್ರತ್ಯಾಗಮ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ಯದಿ(id != LV_PROPERTY_ID_INVALID) ಪ್ರತ್ಯಾಗಮ id;
#ಅನ್ಯಥಾ
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ಪ್ರತ್ಯಾಗಮ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ಮೌಲ್ಯ, bool set)
{
    ಧ್ರುವ lv_property_ops_t * properties;
    ಧ್ರುವ lv_property_ops_t * prop;

    ಧ್ರುವ lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    ಪ್ರತಿ(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        ಯದಿ(properties == ರಿಕ್ತ) {
            /* try base class*/
            ಅನುವರ್ತ;
        }

        ಯದಿ(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            ಅನುವರ್ತ;
        }

        /*Check if there's setter available for this class*/
        ಪ್ರತಿ(uint32_t i = 0; i < clz->properties_ಎಣಿಕೆ; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ಯದಿ(prop->id == LV_PROPERTY_ID_ANY) {
                ಮೌಲ್ಯ->id = prop->id;
                ಯದಿ(set) ಪ್ರತ್ಯಾಗಮ ((lv_property_setter_t)prop->setter)(obj, id, ಮೌಲ್ಯ);
                ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ ((lv_property_getter_t)prop->getter)(obj, id, ಮೌಲ್ಯ);
            }

            /*Not this id, check next*/
            ಯದಿ(prop->id != id)
                ಅನುವರ್ತ;

            /*id matched but we got null pointer to functions*/
            ಯದಿ(set ? prop->setter == ರಿಕ್ತ : prop->getter == ರಿಕ್ತ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            ಯದಿ(!set) ಮೌಲ್ಯ->id = prop->id;

            ವಿಕಲ್ಪ(LV_PROPERTY_ID_TYPE(prop->id)) {
                ಪಕ್ಷ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, ಅಂಕೆ);
                    ಭಙ್ಗ;
                ಪಕ್ಷ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ಭಙ್ಗ;
                ಪಕ್ಷ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ಭಙ್ಗ;
                ಪಕ್ಷ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ಭಙ್ಗ;
                ಪಕ್ಷ LV_PROPERTY_TYPE_POINTER:
                ಪಕ್ಷ LV_PROPERTY_TYPE_IMGSRC:
                ಪಕ್ಷ LV_PROPERTY_TYPE_TEXT:
                ಪಕ್ಷ LV_PROPERTY_TYPE_OBJ:
                ಪಕ್ಷ LV_PROPERTY_TYPE_DISPLAY:
                ಪಕ್ಷ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    ಭಙ್ಗ;
                ಪಕ್ಷ LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &ಮೌಲ್ಯ->point;
                        ಯದಿ(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        ಅನ್ಯಥಾ *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        ಭಙ್ಗ;
                    }
                ಸ್ವಭಾವ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID;
                    }
            }

            ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ property_name_compare(ಧ್ರುವ ಶೂನ್ಯ * ref, ಧ್ರುವ ಶೂನ್ಯ * element)
{
    ಧ್ರುವ lv_property_name_t * prop = element;
    ಪ್ರತ್ಯಾಗಮ lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
