/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ ਖਾਲੀ lv_obj_construct(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj);
ਸਥਿਰ uint32_t get_instance_ਮਾਪ(ਅਚਲ lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * ਮਾਪਾ)
{
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (ਖਾਲੀ *)class_p, (ਖਾਲੀ *)ਮਾਪਾ);
    uint32_t s = get_instance_ਮਾਪ(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    ਜੇ(obj == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
    obj->class_p = class_p;
    obj->ਮਾਪਾ = ਮਾਪਾ;

    /*Create a screen*/
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        ਜੇ(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            ਵਾਪਸ ਨਿਰਾ;
        }

        ਜੇ(disp->screens == ਨਿਰਾ) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, ਆਕਾਰ(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        ਜੇ(screens == ਨਿਰਾ) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            ਵਾਪਸ ਨਿਰਾ;
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
    ਨਹੀਂਤਾਂ {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(ਮਾਪਾ, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

        ਜੇ(!lv_obj_allocate_spec_attr(ਮਾਪਾ)) {
            lv_free(obj);
            ਵਾਪਸ ਨਿਰਾ;
        }
        ਜੇ(lv_obj_ਜੋੜੋ_ਬੱਚਾ(ਮਾਪਾ, obj) != LV_ਨਤੀਜਾ_OK) {
            lv_free(obj);
            ਵਾਪਸ ਨਿਰਾ;
        }
    }

    ਵਾਪਸ obj;
}

ਖਾਲੀ lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_ਮਾਪ(obj);

    lv_ਸਮੂਹ_t * def_ਸਮੂਹ = lv_ਸਮੂਹ_get_default();
    ਜੇ(def_ਸਮੂਹ && lv_obj_is_ਸਮੂਹ_def(obj)) {
        lv_ਸਮੂਹ_ਜੋੜੋ_obj(def_ਸਮੂਹ, obj);
    }

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj);
        lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

ਖਾਲੀ lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(obj->class_p != ਨਿਰਾ, ਵਾਪਸ);

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    ਜੇ(obj->ext_ਅੰਕੜੇ.free_cb) {
        obj->ext_ਅੰਕੜੇ.free_cb(obj->ext_ਅੰਕੜੇ.ਅੰਕੜੇ);
        obj->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਨਿਰਾ;
    }
#ਜੇ_ਅੰਤ

    ਜੇ(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    ਜੇ(obj->class_p->ਅਧਾਰ_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->ਅਧਾਰ_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);

    ਅਚਲ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    ਜਦੋਂ(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->ਅਧਾਰ_class;

    ਜੇ(class_p == ਨਿਰਾ) ਵਾਪਸ false;

    ਵਾਪਸ class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_ਸਮੂਹ_def(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ false);

    ਅਚਲ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    ਜਦੋਂ(class_p && class_p->ਸਮੂਹ_def == LV_OBJ_CLASS_ਸਮੂਹ_DEF_INHERIT) class_p = class_p->ਅਧਾਰ_class;

    ਜੇ(class_p == ਨਿਰਾ) ਵਾਪਸ false;

    ਵਾਪਸ class_p->ਸਮੂਹ_def == LV_OBJ_CLASS_ਸਮੂਹ_DEF_TRUE;
}

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
ਖਾਲੀ lv_obj_ਸੈੱਟ_external_ਅੰਕੜੇ(lv_obj_t * obj, ਖਾਲੀ * ਅੰਕੜੇ, ਖਾਲੀ (* free_cb)(ਖਾਲੀ * ਅੰਕੜੇ))
{
    LV_CHECK_ARG_ਸੁਨੇਹਾ(obj != ਨਿਰਾ, ਵਾਪਸ, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਅੰਕੜੇ;
    obj->ext_ਅੰਕੜੇ.free_cb = free_cb;
}
#ਜੇ_ਅੰਤ

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਖਾਲੀ lv_obj_construct(ਅਚਲ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#ਜੇ LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_EXT_ਅੰਕੜੇ
    obj->ext_ਅੰਕੜੇ.free_cb = ਨਿਰਾ;
    obj->ext_ਅੰਕੜੇ.ਅੰਕੜੇ = ਨਿਰਾ;
#ਜੇ_ਅੰਤ

    ਜੇ(obj->class_p->ਅਧਾਰ_class) {
        ਅਚਲ lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->ਅਧਾਰ_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    ਜੇ(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

ਸਥਿਰ uint32_t get_instance_ਮਾਪ(ਅਚਲ lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    ਜਦੋਂ(class_p && class_p->instance_ਮਾਪ == 0) class_p = class_p->ਅਧਾਰ_class;

    LV_ASSERT(class_p != ਨਿਰਾ); /*Never happens: set at least in `lv_obj` class*/

    ਵਾਪਸ class_p->instance_ਮਾਪ;
}
