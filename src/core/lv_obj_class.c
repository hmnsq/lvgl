/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#परिभाषय MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
स्थिर शून्य lv_obj_construct(ध्रुव lv_obj_class_t * class_p, lv_obj_t * obj);
स्थिर uint32_t get_instance_आकार(ध्रुव lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(ध्रुव lv_obj_class_t * class_p, lv_obj_t * parent)
{
    LV_CHECK_ARG(class_p != रिक्त, प्रत्यागम रिक्त);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (शून्य *)class_p, (शून्य *)parent);
    uint32_t s = get_instance_आकार(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    यदि(obj == रिक्त) प्रत्यागम रिक्त;
    obj->class_p = class_p;
    obj->parent = parent;

    /*Create a screen*/
    यदि(parent == रिक्त) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        यदि(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            प्रत्यागम रिक्त;
        }

        यदि(disp->screens == रिक्त) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, परिमाण(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        यदि(screens == रिक्त) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            प्रत्यागम रिक्त;
        }

        disp->screen_cnt++;
        disp->screens = screens;
        disp->screens[disp->screen_cnt - 1] = obj;

        /*Set coordinates to full screen size*/
        obj->coords.x1 = 0;
        obj->coords.y1 = 0;
        obj->coords.x2 = lv_display_get_horizontal_resolution(disp) - 1;
        obj->coords.y2 = lv_display_get_vertical_resolution(disp) - 1;
    }
    /*Create a normal object*/
    अन्यथा {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(parent, MY_CLASS, प्रत्यागम रिक्त);

        यदि(!lv_obj_allocate_spec_attr(parent)) {
            lv_free(obj);
            प्रत्यागम रिक्त;
        }
        यदि(lv_obj_सङ्कलय_child(parent, obj) != LV_परिणाम_OK) {
            lv_free(obj);
            प्रत्यागम रिक्त;
        }
    }

    प्रत्यागम obj;
}

शून्य lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_आकार(obj);

    lv_गणः_t * def_गणः = lv_गणः_get_default();
    यदि(def_गणः && lv_obj_is_गणः_def(obj)) {
        lv_गणः_सङ्कलय_obj(def_गणः, obj);
    }

    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(parent, LV_EVENT_CHILD_परिवर्तितम्, obj);
        lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

शून्य lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(obj->class_p != रिक्त, प्रत्यागम);

#यदि LV_USE_EXT_दत्त
    यदि(obj->ext_दत्त.free_cb) {
        obj->ext_दत्त.free_cb(obj->ext_दत्त.दत्त);
        obj->ext_दत्त.दत्त = रिक्त;
    }
#यदि_अन्तः

    यदि(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    यदि(obj->class_p->आधारः_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->आधारः_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);

    ध्रुव lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    यावत्(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->आधारः_class;

    यदि(class_p == रिक्त) प्रत्यागम false;

    प्रत्यागम class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_गणः_def(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम false);

    ध्रुव lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    यावत्(class_p && class_p->गणः_def == LV_OBJ_CLASS_गणः_DEF_INHERIT) class_p = class_p->आधारः_class;

    यदि(class_p == रिक्त) प्रत्यागम false;

    प्रत्यागम class_p->गणः_def == LV_OBJ_CLASS_गणः_DEF_TRUE;
}

#यदि LV_USE_EXT_दत्त
शून्य lv_obj_समुच्चयः_external_दत्त(lv_obj_t * obj, शून्य * दत्त, शून्य (* free_cb)(शून्य * दत्त))
{
    LV_CHECK_ARG_सन्देशः(obj != रिक्त, प्रत्यागम, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_दत्त.दत्त = दत्त;
    obj->ext_दत्त.free_cb = free_cb;
}
#यदि_अन्तः

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर शून्य lv_obj_construct(ध्रुव lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#यदि LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#यदि_अन्तः

#यदि LV_USE_EXT_दत्त
    obj->ext_दत्त.free_cb = रिक्त;
    obj->ext_दत्त.दत्त = रिक्त;
#यदि_अन्तः

    यदि(obj->class_p->आधारः_class) {
        ध्रुव lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->आधारः_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    यदि(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

स्थिर uint32_t get_instance_आकार(ध्रुव lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    यावत्(class_p && class_p->instance_आकार == 0) class_p = class_p->आधारः_class;

    LV_ASSERT(class_p != रिक्त); /*Never happens: set at least in `lv_obj` class*/

    प्रत्यागम class_p->instance_आकार;
}
