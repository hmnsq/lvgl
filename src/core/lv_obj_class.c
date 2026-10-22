/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_class_private.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_private.h"
#ضمن "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کن MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_obj_construct(ثابت lv_obj_class_t * class_p, lv_obj_t * obj);
ساكن uint32_t get_instance_اندازه(ثابت lv_obj_class_t * class_p);

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
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (فراغ *)class_p, (فراغ *)والد);
    uint32_t s = get_instance_اندازه(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    إذا(obj == عدم) أرجع عدم;
    obj->class_p = class_p;
    obj->والد = والد;

    /*Create a screen*/
    إذا(والد == عدم) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        إذا(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            أرجع عدم;
        }

        إذا(disp->screens == عدم) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, حجم(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        إذا(screens == عدم) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            أرجع عدم;
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
    وإلا {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(والد, MY_CLASS, أرجع عدم);

        إذا(!lv_obj_allocate_spec_attr(والد)) {
            lv_free(obj);
            أرجع عدم;
        }
        إذا(lv_obj_جمع_کن_فرزند(والد, obj) != LV_نتیجه_OK) {
            lv_free(obj);
            أرجع عدم;
        }
    }

    أرجع obj;
}

فراغ lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_اندازه(obj);

    lv_گروه_t * def_گروه = lv_گروه_get_default();
    إذا(def_گروه && lv_obj_is_گروه_def(obj)) {
        lv_گروه_جمع_کن_obj(def_گروه, obj);
    }

    lv_obj_t * والد = lv_obj_get_والد(obj);
    إذا(والد) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(والد, LV_EVENT_فرزند_تغییریافته, obj);
        lv_obj_send_event(والد, LV_EVENT_فرزند_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

فراغ lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);
    LV_CHECK_ARG(obj->class_p != عدم, أرجع);

#إذا LV_USE_EXT_داده
    إذا(obj->ext_داده.free_cb) {
        obj->ext_داده.free_cb(obj->ext_داده.داده);
        obj->ext_داده.داده = عدم;
    }
#پایان_اگر

    إذا(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    إذا(obj->class_p->پایه_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->پایه_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);

    ثابت lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    طالما(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->پایه_class;

    إذا(class_p == عدم) أرجع false;

    أرجع class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_گروه_def(ثابت lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع false);

    ثابت lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    طالما(class_p && class_p->گروه_def == LV_OBJ_CLASS_گروه_DEF_INHERIT) class_p = class_p->پایه_class;

    إذا(class_p == عدم) أرجع false;

    أرجع class_p->گروه_def == LV_OBJ_CLASS_گروه_DEF_TRUE;
}

#إذا LV_USE_EXT_داده
فراغ lv_obj_مجموعه_external_داده(lv_obj_t * obj, فراغ * داده, فراغ (* free_cb)(فراغ * داده))
{
    LV_CHECK_ARG_پیام(obj != عدم, أرجع, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_داده.داده = داده;
    obj->ext_داده.free_cb = free_cb;
}
#پایان_اگر

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_obj_construct(ثابت lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#إذا LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#پایان_اگر

#إذا LV_USE_EXT_داده
    obj->ext_داده.free_cb = عدم;
    obj->ext_داده.داده = عدم;
#پایان_اگر

    إذا(obj->class_p->پایه_class) {
        ثابت lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->پایه_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    إذا(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

ساكن uint32_t get_instance_اندازه(ثابت lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    طالما(class_p && class_p->instance_اندازه == 0) class_p = class_p->پایه_class;

    LV_ASSERT(class_p != عدم); /*Never happens: set at least in `lv_obj` class*/

    أرجع class_p->instance_اندازه;
}
