/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_class_private.h"
#شامل "../lvgl_public.h"
#شامل "lv_obj_private.h"
#شامل "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساکن خالی lv_obj_construct(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساکن uint32_t get_instance_سائز(ثابت lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(ثابت lv_obj_class_t * class_p, lv_obj_t * والد)
{
    LV_CHECK_ARG(class_p != تہی, واپس تہی);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (خالی *)class_p, (خالی *)والد);
    uint32_t s = get_instance_سائز(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    اگر(obj == تہی) واپس تہی;
    obj->class_p = class_p;
    obj->والد = والد;

    /*Create a screen*/
    اگر(والد == تہی) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        اگر(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            واپس تہی;
        }

        اگر(disp->screens == تہی) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, حجم(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        اگر(screens == تہی) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            واپس تہی;
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
    ورنہ {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(والد, MY_CLASS, واپس تہی);

        اگر(!lv_obj_allocate_spec_attr(والد)) {
            lv_free(obj);
            واپس تہی;
        }
        اگر(lv_obj_جوڑو_بچہ(والد, obj) != LV_نتیجہ_OK) {
            lv_free(obj);
            واپس تہی;
        }
    }

    واپس obj;
}

خالی lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_سائز(obj);

    lv_گروہ_t * def_گروہ = lv_گروہ_get_default();
    اگر(def_گروہ && lv_obj_is_گروہ_def(obj)) {
        lv_گروہ_جوڑو_obj(def_گروہ, obj);
    }

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, obj);
        lv_obj_send_event(والد, LV_EVENT_بچہ_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

خالی lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس);
    LV_CHECK_ARG(obj->class_p != تہی, واپس);

#اگر LV_USE_EXT_ڈیٹا
    اگر(obj->ext_ڈیٹا.free_cb) {
        obj->ext_ڈیٹا.free_cb(obj->ext_ڈیٹا.ڈیٹا);
        obj->ext_ڈیٹا.ڈیٹا = تہی;
    }
#اگر_ختم

    اگر(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    اگر(obj->class_p->بنیاد_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->بنیاد_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس false);

    ثابت lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    جب(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->بنیاد_class;

    اگر(class_p == تہی) واپس false;

    واپس class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_گروہ_def(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != تہی, واپس false);

    ثابت lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    جب(class_p && class_p->گروہ_def == LV_OBJ_CLASS_گروہ_DEF_INHERIT) class_p = class_p->بنیاد_class;

    اگر(class_p == تہی) واپس false;

    واپس class_p->گروہ_def == LV_OBJ_CLASS_گروہ_DEF_TRUE;
}

#اگر LV_USE_EXT_ڈیٹا
خالی lv_obj_سیٹ_external_ڈیٹا(lv_obj_t * obj, خالی * ڈیٹا, خالی (* free_cb)(خالی * ڈیٹا))
{
    LV_CHECK_ARG_پیغام(obj != تہی, واپس, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_ڈیٹا.ڈیٹا = ڈیٹا;
    obj->ext_ڈیٹا.free_cb = free_cb;
}
#اگر_ختم

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن خالی lv_obj_construct(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#اگر LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#اگر_ختم

#اگر LV_USE_EXT_ڈیٹا
    obj->ext_ڈیٹا.free_cb = تہی;
    obj->ext_ڈیٹا.ڈیٹا = تہی;
#اگر_ختم

    اگر(obj->class_p->بنیاد_class) {
        ثابت lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->بنیاد_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    اگر(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

ساکن uint32_t get_instance_سائز(ثابت lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    جب(class_p && class_p->instance_سائز == 0) class_p = class_p->بنیاد_class;

    LV_ASSERT(class_p != تہی); /*Never happens: set at least in `lv_obj` class*/

    واپس class_p->instance_سائز;
}
