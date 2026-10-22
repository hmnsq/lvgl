/**
 * @file lv_obj_property.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "../lvgl_public.h"

#જો LV_USE_OBJ_PROPERTY

#અન્તર્ભાવ "lv_obj_style_internal.h"
#અન્તર્ભાવ "lv_obj_private.h"
#અન્તર્ભાવ "../misc/lv_utils.h"
#અન્તર્ભાવ "lv_obj_class_private.h"
#અન્તર્ભાવ "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/

#define HANDLE_PROPERTY_TYPE(type, field) \
    જો(!set) { \
        કિંમત->field = ((lv_property_get_##type##_t)(prop->getter))(obj); \
    } નહીંતર { \
        પસંદગી(LV_PROPERTY_ID_TYPE2(prop->id)) { \
            સ્થિતિ LV_PROPERTY_ID_INVALID: \
                ((lv_property_set_##type##_t)(prop->setter))(obj, કિંમત->field); \
                તોડો; \
            સ્થિતિ LV_PROPERTY_TYPE_INT: \
                ((lv_property_set_##type##_integer_t)(prop->setter))(obj, કિંમત->arg1.field, કિંમત->arg2.અંક); \
                તોડો; \
            સ્થિતિ LV_PROPERTY_TYPE_BOOL: \
                ((lv_property_set_##type##_boolean_t)(prop->setter))(obj, કિંમત->arg1.field, કિંમત->arg2.enable); \
                તોડો; \
            સ્થિતિ LV_PROPERTY_TYPE_PRECISE: \
                ((lv_property_set_##type##_precise_t)(prop->setter))(obj, કિંમત->arg1.field, કિંમત->arg2.precise); \
                તોડો; \
            સ્થિતિ LV_PROPERTY_TYPE_COLOR: \
                ((lv_property_set_##type##_color_t)(prop->setter))(obj, કિંમત->arg1.field, કિંમત->arg2.color); \
                તોડો; \
            સ્થિતિ LV_PROPERTY_TYPE_POINTER: \
            સ્થિતિ LV_PROPERTY_TYPE_IMGSRC: \
            સ્થિતિ LV_PROPERTY_TYPE_TEXT: \
            સ્થિતિ LV_PROPERTY_TYPE_OBJ: \
            સ્થિતિ LV_PROPERTY_TYPE_DISPLAY: \
            સ્થિતિ LV_PROPERTY_TYPE_FONT: \
                ((lv_property_set_##type##_pointer_t)(prop->setter))(obj, કિંમત->arg1.field, કિંમત->arg2.ptr); \
                તોડો; \
        } \
    }


/**********************
 *      TYPEDEFS
 **********************/

પ્રકારવ્યાખ્યા int32_t integer;
પ્રકારવ્યાખ્યા bool boolean;
પ્રકારવ્યાખ્યા lv_કિંમત_precise_t precise;
પ્રકારવ્યાખ્યા lv_color_t color;
પ્રકારવ્યાખ્યા અચલ શૂન્ય * pointer;

#define DEFINE_PROPERTY_SETTER_TYPES(type) \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_t)(lv_obj_t *, type); \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_integer_t)(lv_obj_t *, type, int32_t); \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_boolean_t)(lv_obj_t *, type, bool); \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_precise_t)(lv_obj_t *, type, lv_કિંમત_precise_t); \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_color_t)(lv_obj_t *, type, lv_color_t); \
    પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_##type##_pointer_t)(lv_obj_t *, type, અચલ શૂન્ય *)

DEFINE_PROPERTY_SETTER_TYPES(integer);
DEFINE_PROPERTY_SETTER_TYPES(boolean);
DEFINE_PROPERTY_SETTER_TYPES(precise);
DEFINE_PROPERTY_SETTER_TYPES(color);
DEFINE_PROPERTY_SETTER_TYPES(pointer);

પ્રકારવ્યાખ્યા શૂન્ય (*lv_property_set_point_t)(lv_obj_t *, lv_point_t *);
પ્રકારવ્યાખ્યા lv_result_t (*lv_property_setter_t)(lv_obj_t *, lv_prop_id_t, અચલ lv_property_t *);

પ્રકારવ્યાખ્યા integer(*lv_property_get_integer_t)(અચલ lv_obj_t *);
પ્રકારવ્યાખ્યા bool (*lv_property_get_boolean_t)(અચલ lv_obj_t *);
પ્રકારવ્યાખ્યા lv_કિંમત_precise_t (*lv_property_get_precise_t)(અચલ lv_obj_t *);
પ્રકારવ્યાખ્યા lv_color_t (*lv_property_get_color_t)(અચલ lv_obj_t *);
પ્રકારવ્યાખ્યા શૂન્ય * (*lv_property_get_pointer_t)(અચલ lv_obj_t *);
પ્રકારવ્યાખ્યા lv_point_t (*lv_property_get_point_t)(lv_obj_t *);

પ્રકારવ્યાખ્યા lv_result_t (*lv_property_getter_t)(અચલ lv_obj_t *, lv_prop_id_t, lv_property_t *);

/**********************
 *  STATIC PROTOTYPES
 **********************/

સ્થિર lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * કિંમત, bool set);
સ્થિર પૂર્ણાંક property_name_compare(અચલ શૂન્ય * ref, અચલ શૂન્ય * element);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_result_t lv_obj_set_property(lv_obj_t * obj, અચલ lv_property_t * કિંમત)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_RESULT_INVALID);
    LV_CHECK_ARG(કિંમત != ખાલી, પાછા LV_RESULT_INVALID);

    uint32_t index = LV_PROPERTY_ID_INDEX(કિંમત->id);
    જો(કિંમત->id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id set to %p", obj);
        પાછા LV_RESULT_INVALID;
    }

    જો(index < LV_PROPERTY_ID_START) {
        lv_obj_set_local_style_prop(obj, index, કિંમત->style, કિંમત->selector);
        પાછા LV_RESULT_OK;
    }

    પાછા obj_property(obj, કિંમત->id, (lv_property_t *)કિંમત, true);
}

lv_result_t lv_obj_set_properties(lv_obj_t * obj, અચલ lv_property_t * કિંમત, uint32_t ગણતરી)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_RESULT_INVALID);
    LV_CHECK_ARG(કિંમત != ખાલી, પાછા LV_RESULT_INVALID);

    માટે(uint32_t i = 0; i < ગણતરી; i++) {
        lv_result_t result = lv_obj_set_property(obj, &કિંમત[i]);
        જો(result != LV_RESULT_OK) {
            પાછા result;
        }
    }

    પાછા LV_RESULT_OK;
}

lv_property_t lv_obj_get_property(lv_obj_t * obj, lv_prop_id_t id)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_result_t result;
    lv_property_t કિંમત = { 0 };

    uint32_t index = LV_PROPERTY_ID_INDEX(id);
    જો(id == LV_PROPERTY_ID_INVALID || index > LV_PROPERTY_ID_ANY) {
        LV_LOG_WARN("Invalid property id to get from %p", obj);
        કિંમત.id = LV_PROPERTY_ID_INVALID;
        કિંમત.અંક = 0;
        પાછા કિંમત;
    }

    જો(index < LV_PROPERTY_ID_START) {
        કિંમત.style = lv_obj_get_style_prop(obj, LV_PART_MAIN, index);
        કિંમત.id = id;
        કિંમત.selector = LV_PART_MAIN | obj->state;
        પાછા કિંમત;
    }

    result = obj_property(obj, id, &કિંમત, false);
    જો(result != LV_RESULT_OK)
        કિંમત.id = LV_PROPERTY_ID_INVALID;

    પાછા કિંમત;
}

lv_property_t lv_obj_get_style_property(lv_obj_t * obj, lv_prop_id_t id, lv_part_t part)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા (lv_property_t) {
        .id = LV_PROPERTY_ID_INVALID
    });

    lv_property_t કિંમત;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    જો(index == LV_PROPERTY_ID_INVALID || index >= LV_PROPERTY_ID_START) {
        LV_LOG_WARN("invalid style property id 0x%" LV_PRIx32, id);
        કિંમત.id = LV_PROPERTY_ID_INVALID;
        કિંમત.અંક = 0;
        પાછા કિંમત;
    }

    કિંમત.style = lv_obj_get_style_prop(obj, part, index);
    કિંમત.id = id;
    કિંમત.selector = part | obj->state;
    પાછા કિંમત;
}

lv_prop_id_t lv_style_property_get_id(અચલ અક્ષર * name)
{
    LV_CHECK_ARG(name != ખાલી, પાછા LV_PROPERTY_ID_INVALID);
#જો LV_USE_OBJ_PROPERTY_NAME
    lv_property_name_t * found;
    /*Check style property*/
    found = lv_utils_bsearch(name, lv_style_property_names, કદ(lv_style_property_names) / કદ(lv_property_name_t),
                             કદ(lv_property_name_t), property_name_compare);
    જો(found) પાછા found->id;
#નહીંતર
    LV_UNUSED(name);
#endif
    પાછા LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_class_property_get_id(અચલ lv_obj_class_t * clz, અચલ અક્ષર * name)
{
    LV_CHECK_ARG(clz != ખાલી, પાછા LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ખાલી, પાછા LV_PROPERTY_ID_INVALID);
#જો LV_USE_OBJ_PROPERTY_NAME
    અચલ lv_property_name_t * names;
    lv_property_name_t * found;

    names = clz->property_names;
    જો(names == ખાલી) {
        /* try base class*/
        પાછા LV_PROPERTY_ID_INVALID;
    }

    found = lv_utils_bsearch(name, names, clz->names_ગણતરી, કદ(lv_property_name_t), property_name_compare);
    જો(found) પાછા found->id;
#નહીંતર
    LV_UNUSED(clz);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    પાછા LV_PROPERTY_ID_INVALID;
}

lv_prop_id_t lv_obj_property_get_id(અચલ lv_obj_t * obj, અચલ અક્ષર * name)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા LV_PROPERTY_ID_INVALID);
    LV_CHECK_ARG(name != ખાલી, પાછા LV_PROPERTY_ID_INVALID);
#જો LV_USE_OBJ_PROPERTY_NAME

    અચલ lv_obj_class_t * clz;
    lv_prop_id_t id;

    માટે(clz = obj->class_p; clz; clz = clz->base_class) {
        id = lv_obj_class_property_get_id(clz, name);
        જો(id != LV_PROPERTY_ID_INVALID) પાછા id;
    }

    /*Check style property*/
    id = lv_style_property_get_id(name);
    જો(id != LV_PROPERTY_ID_INVALID) પાછા id;
#નહીંતર
    LV_UNUSED(obj);
    LV_UNUSED(name);
    LV_UNUSED(property_name_compare);
#endif
    પાછા LV_PROPERTY_ID_INVALID;
}

/**********************
 *  STATIC FUNCTIONS
 **********************/

સ્થિર lv_result_t obj_property(lv_obj_t * obj, lv_prop_id_t id, lv_property_t * કિંમત, bool set)
{
    અચલ lv_property_ops_t * properties;
    અચલ lv_property_ops_t * prop;

    અચલ lv_obj_class_t * clz;
    uint32_t index = LV_PROPERTY_ID_INDEX(id);

    માટે(clz = obj->class_p ; clz; clz = clz->base_class) {
        properties = clz->properties;
        જો(properties == ખાલી) {
            /* try base class*/
            ચાલુ;
        }

        જો(id != LV_PROPERTY_ID_ANY && (index < clz->prop_index_start || index > clz->prop_index_end)) {
            /* try base class*/
            ચાલુ;
        }

        /*Check if there's setter available for this class*/
        માટે(uint32_t i = 0; i < clz->properties_ગણતરી; i++) {
            prop = &properties[i];

            /*pass id and value directly to widget's property method*/
            જો(prop->id == LV_PROPERTY_ID_ANY) {
                કિંમત->id = prop->id;
                જો(set) પાછા ((lv_property_setter_t)prop->setter)(obj, id, કિંમત);
                નહીંતર પાછા ((lv_property_getter_t)prop->getter)(obj, id, કિંમત);
            }

            /*Not this id, check next*/
            જો(prop->id != id)
                ચાલુ;

            /*id matched but we got null pointer to functions*/
            જો(set ? prop->setter == ખાલી : prop->getter == ખાલી) {
                LV_LOG_WARN("NULL %s provided, id: 0x%" LV_PRIx32, set ? "setter" : "getter", id);
                પાછા LV_RESULT_INVALID;
            }

            /*Update value id if it's a read*/
            જો(!set) કિંમત->id = prop->id;

            પસંદગી(LV_PROPERTY_ID_TYPE(prop->id)) {
                સ્થિતિ LV_PROPERTY_TYPE_INT:
                    HANDLE_PROPERTY_TYPE(integer, અંક);
                    તોડો;
                સ્થિતિ LV_PROPERTY_TYPE_BOOL:
                    HANDLE_PROPERTY_TYPE(boolean, enable);
                    તોડો;
                સ્થિતિ LV_PROPERTY_TYPE_PRECISE:
                    HANDLE_PROPERTY_TYPE(precise, precise);
                    તોડો;
                સ્થિતિ LV_PROPERTY_TYPE_COLOR:
                    HANDLE_PROPERTY_TYPE(color, color);
                    તોડો;
                સ્થિતિ LV_PROPERTY_TYPE_POINTER:
                સ્થિતિ LV_PROPERTY_TYPE_IMGSRC:
                સ્થિતિ LV_PROPERTY_TYPE_TEXT:
                સ્થિતિ LV_PROPERTY_TYPE_OBJ:
                સ્થિતિ LV_PROPERTY_TYPE_DISPLAY:
                સ્થિતિ LV_PROPERTY_TYPE_FONT:
                    HANDLE_PROPERTY_TYPE(pointer, ptr);
                    તોડો;
                સ્થિતિ LV_PROPERTY_TYPE_POINT: {
                        lv_point_t * point = &કિંમત->point;
                        જો(set)((lv_property_set_point_t)(prop->setter))(obj, point);
                        નહીંતર *point = ((lv_property_get_point_t)(prop->getter))(obj);
                        તોડો;
                    }
                મૂળભૂત: {
                        LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, prop->id);
                        પાછા LV_RESULT_INVALID;
                    }
            }

            પાછા LV_RESULT_OK;
        }

        /*If no setter found, try base class then*/
    }

    LV_LOG_WARN("Unknown property id: 0x%08" LV_PRIx32, id);
    પાછા LV_RESULT_INVALID;
}

સ્થિર પૂર્ણાંક property_name_compare(અચલ શૂન્ય * ref, અચલ શૂન્ય * element)
{
    અચલ lv_property_name_t * prop = element;
    પાછા lv_strcmp(ref, prop->name);
}

#endif /*LV_USE_OBJ_PROPERTY*/
