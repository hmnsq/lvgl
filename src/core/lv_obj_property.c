/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "../lvgl_public.h"

#ఒకవేళ LV_USE_OBJ_PROPERTY

#అన్తర్భావ "lv_obj_style_internal.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../misc/lv_utils.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#నిర్వచించు HANDLE_PROPERTY_TYPE(type, క్షేత్రం) \
    ఒకవేళ(!సమితి) { \
        విలువ->క్షేత్రం = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } లేకపోతే { \
        ఎంపిక(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            స్థితి LV_PROPERTY_ID_INVALID: \
                ((lv_property_సమితి##type##_t)(prop->setter))(obj, విలువ->క్షేత్రం); \
                ఆపు; \
            స్థితి LV_PROPERTY_TYPE_INT: \
                ((lv_property_సమితి##type##_integer_t)(prop->setter))(obj, విలువ->arg1.క్షేత్రం, విలువ->arg2.అంకె); \
                ఆపు; \
            స్థితి LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_సమితి##type##_boolean_t)(prop->setter))(obj, విలువ->arg1.క్షేత్రం, విలువ->arg2.enable); \
                ఆపు; \
            స్థితి LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_సమితి##type##_precise_t)(prop->setter))(obj, విలువ->arg1.క్షేత్రం, విలువ->arg2.precise); \
                ఆపు; \
            స్థితి LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_సమితి##type##_color_t)(prop->setter))(obj, విలువ->arg1.క్షేత్రం, విలువ->arg2.color); \
                ఆపు; \
            స్థితి LV_PROPERTY_TYPE_సూచిక: \
            స్థితి LV_PROPERTY_TYPE_IMGSRC: \
            స్థితి LV_PROPERTY_TYPE_పాఠ్యం: \
            స్థితి LV_PROPERTY_TYPE_OBJ: \
            స్థితి LV_PROPERTY_TYPE_DISPLAY: \
            స్థితి LV_PROPERTY_TYPE_FONT: \
                ((lv_property_సమితి##type##_pointer_t)(prop->setter))(obj, విలువ->arg1.క్షేత్రం, విలువ->arg2.ptr); \
                ఆపు; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

రకనిర్వచనం int32_t integer;
రకనిర్వచనం bool boolean;
రకనిర్వచనం lv_విలువ_precise_t precise;
రకనిర్వచనం lv_color_t color;
రకనిర్వచనం స్థిరరాశి శూన్య * సూచిక;

#నిర్వచించు DEFINE_PROPERTY_SETTER_TYPES(type) \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_t)(lv_obj_t *, type); \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_integer_t)(lv_obj_t *, type, int32_t); \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_boolean_t)(lv_obj_t *, type, bool); \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_precise_t)(lv_obj_t *, type, lv_విలువ_precise_t); \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    రకనిర్వచనం శూన్య (*lv_property_సమితి##type##_pointer_t)(lv_obj_t *, type, స్థిరరాశి శూన్య *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(సూచిక);

రకనిర్వచనం శూన్య (*lv_property_సమితి_బిందువు_t)(lv_obj_t *, lv_బిందువు_t *);
రకనిర్వచనం lv_ఫలితం_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, స్థిరరాశి lv_property_t *);

రకనిర్వచనం integer(*lv_property_get_integer_t)(స్థిరరాశి lv_obj_t *);
రకనిర్వచనం bool (*lv_property_get_boolean_t)(స్థిరరాశి lv_obj_t *);
రకనిర్వచనం lv_విలువ_precise_t (*lv_property_get_precise_t)(స్థిరరాశి lv_obj_t *);
రకనిర్వచనం lv_color_t (*lv_property_get_color_t)(స్థిరరాశి lv_obj_t *);
రకనిర్వచనం శూన్య * (*lv_property_get_సూచిక_t)(స్థిరరాశి lv_obj_t *);
రకనిర్వచనం lv_బిందువు_t (*lv_property_get_బిందువు_t)(lv_obj_t *);

రకనిర్వచనం lv_ఫలితం_t (*lv_property_getter_t)(స్థిరరాశి lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

స్థిర lv_ఫలితం_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * విలువ, bool సమితి);
స్థిర పూర్ణాంకం property_name_compare(స్థిరరాశి శూన్య * ref, స్థిరరాశి శూన్య * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_ఫలితం_t lv_obj_సమితి_property(lv_obj_t * obj, స్థిరరాశి lv_property_t * విలువ)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);
    LV_CHECK_ARG(విలువ != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);

    uint32_t సూచీ = LV_PROPERTY_ID_సూచీ(విలువ->id);
    ఒకవేళ(విలువ->id == LV_PROPERTY_ID_INVALID || సూచీ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        తిరిగి LV_ఫలితం_INVALID;
    }

    ఒకవేళ(సూచీ < LV_PROPERTY_ID_ప్రారంభం) {
        lv_obj_సమితి_local_style_prop(obj, సూచీ, విలువ->style, విలువ->selector);
        తిరిగి LV_ఫలితం_OK;
    }

    తిరిగి obj_property(obj, విలువ->id, (lv_property_t *)విలువ, true);
}

lv_ఫలితం_t lv_obj_సమితి_properties(lv_obj_t * obj, స్థిరరాశి lv_property_t * విలువ, uint32_t లెక్క)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);
    LV_CHECK_ARG(విలువ != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);

    కొరకు(uint32_t i = 0; i < లెక్క; i++) {
        lv_ఫలితం_t ఫలితం = lv_obj_సమితి_property(obj, &విలువ[i]);
        ఒకవేళ(ఫలితం != LV_ఫలితం_OK) {
            తిరిగి ఫలితం;
        }
    }

    తిరిగి LV_ఫలితం_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_ఫలితం_t ఫలితం;
    lv_property_t విలువ = { 0 };

    uint32_t సూచీ = LV_PROPERTY_ID_సూచీ(id);
    ఒకవేళ(id == LV_PROPERTY_ID_INVALID || సూచీ > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        విలువ.id = LV_PROPERTY_ID_INVALID;
        విలువ.అంకె = 0;
        తిరిగి విలువ;
    }

    ఒకవేళ(సూచీ < LV_PROPERTY_ID_ప్రారంభం) {
        విలువ.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, సూచీ);
        విలువ.id = id;
        విలువ.selector = LV_PART_MAIN | obj->state;
        తిరిగి విలువ;
    }

    ఫలితం = obj_property(obj, id, &విలువ, false);
    ఒకవేళ(ఫలితం != LV_ఫలితం_OK)
        విలువ.id = LV_PROPERTY_ID_INVALID;

    తిరిగి విలువ;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t విలువ;
    uint32_t సూచీ = LV_PROPERTY_ID_సూచీ(id);

    ఒకవేళ(సూచీ == LV_PROPERTY_ID_INVALID || సూచీ >= LV_PROPERTY_ID_ప్రారంభం) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        విలువ.id = LV_PROPERTY_ID_INVALID;
        విలువ.అంకె = 0;
        తిరిగి విలువ;
    }

    విలువ.style = lv_obj_get_style_prop(obj, part, సూచీ);
    విలువ.id = id;
    విలువ.selector = part | obj->state;
    తిరిగి విలువ;
}

lv_prop_id_t lv_style_property_get_id(స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_ARG(name != ఖాళీ, తిరిగి LV_PROPERTY_ID_INVALID);
#ఒకవేళ LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * దొరికింది;
    /*Check style property*/
    దొరికింది = lv_utils_bsearch(name, lv_style_property_పేర్లు, పరిమాణం(lv_style_property_పేర్లు) / పరిమాణం(lv_property_name_t),
                             పరిమాణం(lv_property_name_t), property_name_compare);
    ఒకవేళ(దొరికింది) తిరిగి దొరికింది->id;
#లేకపోతే
    LV_వాడని(name);
#అయితే_ముగింపు
    తిరిగి LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(స్థిరరాశి lv_obj_class_t * clz, స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_ARG(clz != ఖాళీ, తిరిగి LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ఖాళీ, తిరిగి LV_PROPERTY_ID_INVALID);
#ఒకవేళ LV_USE_OBJ_PROPERTY_NAME
    స్థిరరాశి lv_property_name_t * పేర్లు;
    lv_property_name_t * దొరికింది;

    పేర్లు = clz->property_పేర్లు;
    ఒకవేళ(పేర్లు == ఖాళీ) {
        /* try base class*/
        తిరిగి LV_PROPERTY_ID_INVALID;
    }

    దొరికింది = lv_utils_bsearch(name, పేర్లు, clz->పేర్లు_లెక్క, పరిమాణం(lv_property_name_t), property_name_compare);
    ఒకవేళ(దొరికింది) తిరిగి దొరికింది->id;
#లేకపోతే
    LV_వాడని(clz);
    LV_వాడని(name);
    LV_వాడని(property_name_compare);
#అయితే_ముగింపు
    తిరిగి LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ఖాళీ, తిరిగి LV_PROPERTY_ID_INVALID);
#ఒకవేళ LV_USE_OBJ_PROPERTY_NAME

    స్థిరరాశి lv_obj_class_t * clz;
    lv_prop_id_t id;

    కొరకు(clz = obj->class_p; clz; clz = clz->పునాది_class) {
        id = lv_obj_class_property_get_id(clz, name);
        ఒకవేళ(id != LV_PROPERTY_ID_INVALID) తిరిగి id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    ఒకవేళ(id != LV_PROPERTY_ID_INVALID) తిరిగి id;
#లేకపోతే
    LV_వాడని(obj);
    LV_వాడని(name);
    LV_వాడని(property_name_compare);
#అయితే_ముగింపు
    తిరిగి LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

స్థిర lv_ఫలితం_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * విలువ, bool సమితి)
{
    స్థిరరాశి lv_property_ops_t * properties;
    స్థిరరాశి lv_property_ops_t * prop;

    స్థిరరాశి lv_obj_class_t * clz;
    uint32_t సూచీ = LV_PROPERTY_ID_సూచీ(id);

    కొరకు(clz = obj->class_p ; clz; clz = clz->పునాది_class) {
        properties = clz->properties;
        ఒకవేళ(properties == ఖాళీ) {
            /* try base class*/
            కొనసాగు;
        }

        ఒకవేళ(id != LV_PROPERTY_ID_ANY && (సూచీ < clz->prop_సూచీ_ప్రారంభం || సూచీ > clz->prop_సూచీ_చివర)) {
            /* try base class*/
            కొనసాగు;
        }

        /*Check if there's setter available for this class*/
        కొరకు(uint32_t i = 0; i < clz->properties_లెక్క; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            ఒకవేళ(prop->id == LV_PROPERTY_ID_ANY) {
                విలువ->id = prop->id;
                ఒకవేళ(సమితి) తిరిగి ((lv_property_setter_t)prop->setter)(obj, id, విలువ);
                లేకపోతే తిరిగి ((lv_property_getter_t)prop->getter)(obj, id, విలువ);
            }

            /*Not this id, check next*/
            ఒకవేళ(prop->id != id)
                కొనసాగు;

            /*id matched but we got null pointer to functions*/
            ఒకవేళ(సమితి ? prop->setter == ఖాళీ : prop->getter == ఖాళీ) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, సమితి ? "setter" : "getter", id);
                తిరిగి LV_ఫలితం_INVALID;
            }

            /*Update value id if it's a read*/
            ఒకవేళ(!సమితి) విలువ->id = prop->id;

            ఎంపిక(LV_PROPERTY_ID_TYPE(prop->id)) {
                స్థితి LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, అంకె);
                    ఆపు;
                స్థితి LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    ఆపు;
                స్థితి LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    ఆపు;
                స్థితి LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    ఆపు;
                స్థితి LV_PROPERTY_TYPE_సూచిక:
                స్థితి LV_PROPERTY_TYPE_IMGSRC:
                స్థితి LV_PROPERTY_TYPE_పాఠ్యం:
                స్థితి LV_PROPERTY_TYPE_OBJ:
                స్థితి LV_PROPERTY_TYPE_DISPLAY:
                స్థితి LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(సూచిక, ptr);
                    ఆపు;
                స్థితి LV_PROPERTY_TYPE_బిందువు: {
                        lv_బిందువు_t * బిందువు = &విలువ->బిందువు;
                        ఒకవేళ(సమితి)((lv_property_సమితి_బిందువు_t)(prop->setter))(obj, బిందువు);
                        లేకపోతే *బిందువు = ((lv_property_get_బిందువు_t)(prop->getter))(obj);
                        ఆపు;
                    }
                అప్రమేయ: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        తిరిగి LV_ఫలితం_INVALID;
                    }
            }

            తిరిగి LV_ఫలితం_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    తిరిగి LV_ఫలితం_INVALID;
}

స్థిర పూర్ణాంకం property_name_compare(స్థిరరాశి శూన్య * ref, స్థిరరాశి శూన్య * element)
{
    స్థిరరాశి lv_property_name_t * prop = element;
    తిరిగి lv_strcmp(ref, prop->name);
}

#అయితే_ముగింపు /*LV_USE_OBJ_PROPERTY*/
