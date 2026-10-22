/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"

#ଯଦି LV_USE_OBJ_PROPERTY

#ଅନ୍ତର୍ଭାଵ "lv_obj_style_internal.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_utils.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_class_private.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    ଯଦି(!set) { \
        ମୂଲ୍ୟ->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } ଅନ୍ୟଥା { \
        ବାଛ(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            ସ୍ଥିତି LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, ମୂଲ୍ୟ->field); \
                ଭାଙ୍ଗ; \
            ସ୍ଥିତି LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, ମୂଲ୍ୟ->arg1.field, ମୂଲ୍ୟ->arg2.ଅଙ୍କ); \
                ଭାଙ୍ଗ; \
            ସ୍ଥିତି LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, ମୂଲ୍ୟ->arg1.field, ମୂଲ୍ୟ->arg2.enable); \
                ଭାଙ୍ଗ; \
            ସ୍ଥିତି LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, ମୂଲ୍ୟ->arg1.field, ମୂଲ୍ୟ->arg2.precise); \
                ଭାଙ୍ଗ; \
            ସ୍ଥିତି LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, ମୂଲ୍ୟ->arg1.field, ମୂଲ୍ୟ->arg2.color); \
                ଭାଙ୍ଗ; \
            ସ୍ଥିତି LV_PROPERTY_TYPE_POINTER: \
            ସ୍ଥିତି LV_PROPERTY_TYPE_IMGSRC: \
            ସ୍ଥିତି LV_PROPERTY_TYPE_TEXT: \
            ସ୍ଥିତି LV_PROPERTY_TYPE_OBJ: \
            ସ୍ଥିତି LV_PROPERTY_TYPE_DISPLAY: \
            ସ୍ଥିତି LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, ମୂଲ୍ୟ->arg1.field, ମୂଲ୍ୟ->arg2.ptr); \
                ଭାଙ୍ଗ; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

ପ୍ରକାରବ୍ୟାଖ୍ୟା int32_t integer;
ପ୍ରକାରବ୍ୟାଖ୍ୟା bool boolean;
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_ମୂଲ୍ୟ_precise_t precise;
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_color_t color;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଚଳ ଶୂନ୍ୟ * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_ମୂଲ୍ୟ_precise_t); \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, ଅଚଳ ଶୂନ୍ୟ *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, ଅଚଳ lv_property_t *);

ପ୍ରକାରବ୍ୟାଖ୍ୟା integer(*lv_property_get_integer_t)(ଅଚଳ lv_obj_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା bool (*lv_property_get_boolean_t)(ଅଚଳ lv_obj_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_ମୂଲ୍ୟ_precise_t (*lv_property_get_precise_t)(ଅଚଳ lv_obj_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_color_t (*lv_property_get_color_t)(ଅଚଳ lv_obj_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ * (*lv_property_get_pointer_t)(ଅଚଳ lv_obj_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

ପ୍ରକାରବ୍ୟାଖ୍ୟା lv_result_t (*lv_property_getter_t)(ଅଚଳ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

ସ୍ଥିର lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ମୂଲ୍ୟ, bool set);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା property_name_compare(ଅଚଳ ଶୂନ୍ୟ * ref, ଅଚଳ ଶୂନ୍ୟ * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, ଅଚଳ lv_property_t * ମୂଲ୍ୟ)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);
    LV_CHECK_ARG(ମୂଲ୍ୟ != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(ମୂଲ୍ୟ->id);
    ଯଦି(ମୂଲ୍ୟ->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        ଫେରନ୍ତୁ LV_RESULT_INVALID;
    }

    ଯଦି(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, ମୂଲ୍ୟ->style, ମୂଲ୍ୟ->selector);
        ଫେରନ୍ତୁ LV_RESULT_OK;
    }

    ଫେରନ୍ତୁ obj_property(obj, ମୂଲ୍ୟ->id, (lv_property_t *)ମୂଲ୍ୟ, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, ଅଚଳ lv_property_t * ମୂଲ୍ୟ, uint32_t ଗଣନା_2)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);
    LV_CHECK_ARG(ମୂଲ୍ୟ != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);

    ପାଇଁ(uint32_t i = 0; i < ଗଣନା_2; i++) {
        lv_result_t result = lv_obj_set_property(obj, &ମୂଲ୍ୟ[i]);
        ଯଦି(result != LV_RESULT_OK) {
            ଫେରନ୍ତୁ result;
        }
    }

    ଫେରନ୍ତୁ LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t ମୂଲ୍ୟ = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    ଯଦି(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        ମୂଲ୍ୟ.id = LV_PROPERTY_ID_INVALID;
        ମୂଲ୍ୟ.ଅଙ୍କ = 0;
        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

    ଯଦି(index < LV_PROPERTY_ID_START) {
        ମୂଲ୍ୟ.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        ମୂଲ୍ୟ.id = id;
        ମୂଲ୍ୟ.selector = LV_PART_MAIN | obj->state;
        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

    result = obj_property(obj, id, &ମୂଲ୍ୟ, false);
    ଯଦି(result != LV_RESULT_OK)
        ମୂଲ୍ୟ.id = LV_PROPERTY_ID_INVALID;

    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t ମୂଲ୍ୟ;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    ଯଦି(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        ମୂଲ୍ୟ.id = LV_PROPERTY_ID_INVALID;
        ମୂଲ୍ୟ.ଅଙ୍କ = 0;
        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
    }

    ମୂଲ୍ୟ.style = lv_obj_get_style_prop(obj, part, index);
    ମୂଲ୍ୟ.id = id;
    ମୂଲ୍ୟ.selector = part | obj->state;
    ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

lv_prop_id_t lv_style_property_get_id(ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_ARG(name != ଖାଲି, ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID);
#ଯଦି LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, ଆକାର(lv_style_property_names) / ଆକାର(lv_property_name_t),
                             ଆକାର(lv_property_name_t), property_name_compare);
    ଯଦି(found) ଫେରନ୍ତୁ found->id;
#ଅନ୍ୟଥା
    LV_UNUSED(name);
#endif
    ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(ଅଚଳ lv_obj_class_t * clz, ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_ARG(clz != ଖାଲି, ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ଖାଲି, ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID);
#ଯଦି LV_USE_OBJ_PROPERTY_NAME
    ଅଚଳ lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    ଯଦି(names == ଖାଲି) {
        /* try base class*/
        ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_ଗଣନା, ଆକାର(lv_property_name_t), property_name_compare);
    ଯଦି(found) ଫେରନ୍ତୁ found->id;
#ଅନ୍ୟଥା
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(ଅଚଳ lv_obj_t * obj, ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ଖାଲି, ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID);
#ଯଦି LV_USE_OBJ_PROPERTY_NAME

    ଅଚଳ lv_obj_class_t * clz;
    lv_prop_id_t id;

    ପାଇଁ(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ଯଦି(id != LV_PROPERTY_ID_INVALID) ଫେରନ୍ତୁ id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ଯଦି(id != LV_PROPERTY_ID_INVALID) ଫେରନ୍ତୁ id;
#ଅନ୍ୟଥା
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    ଫେରନ୍ତୁ LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

ସ୍ଥିର lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * ମୂଲ୍ୟ, bool set)
{
    ଅଚଳ lv_property_ops_t * properties;
    ଅଚଳ lv_property_ops_t * prop;

    ଅଚଳ lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    ପାଇଁ(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        ଯଦି(properties == ଖାଲି) {
            /* try base class*/
            ଜାରି;
        }

        ଯଦି(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            ଜାରି;
        }

        /*Check if there's setter available for this class*/
        ପାଇଁ(uint32_t i = 0; i < clz->properties_ଗଣନା; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ଯଦି(prop->id == LV_PROPERTY_ID_ANY) {
                ମୂଲ୍ୟ->id = prop->id;
                ଯଦି(set) ଫେରନ୍ତୁ ((lv_property_setter_t)prop->setter)(obj, id, ମୂଲ୍ୟ);
                ଅନ୍ୟଥା ଫେରନ୍ତୁ ((lv_property_getter_t)prop->getter)(obj, id, ମୂଲ୍ୟ);
            }

            /*Not this id, check next*/
            ଯଦି(prop->id != id)
                ଜାରି;

            /*id matched but we got null pointer to functions*/
            ଯଦି(set ? prop->setter == ଖାଲି : prop->getter == ଖାଲି) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                ଫେରନ୍ତୁ LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            ଯଦି(!set) ମୂଲ୍ୟ->id = prop->id;

            ବାଛ(LV_PROPERTY_ID_TYPE(prop->id)) {
                ସ୍ଥିତି LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, ଅଙ୍କ);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି LV_PROPERTY_TYPE_POINTER:
                ସ୍ଥିତି LV_PROPERTY_TYPE_IMGSRC:
                ସ୍ଥିତି LV_PROPERTY_TYPE_TEXT:
                ସ୍ଥିତି LV_PROPERTY_TYPE_OBJ:
                ସ୍ଥିତି LV_PROPERTY_TYPE_DISPLAY:
                ସ୍ଥିତି LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &ମୂଲ୍ୟ->point;
                        ଯଦି(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        ଅନ୍ୟଥା *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        ଭାଙ୍ଗ;
                    }
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        ଫେରନ୍ତୁ LV_RESULT_INVALID;
                    }
            }

            ଫେରନ୍ତୁ LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    ଫେରନ୍ତୁ LV_RESULT_INVALID;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା property_name_compare(ଅଚଳ ଶୂନ୍ୟ * ref, ଅଚଳ ଶୂନ୍ୟ * element)
{
    ଅଚଳ lv_property_name_t * prop = element;
    ଫେରନ୍ତୁ lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
