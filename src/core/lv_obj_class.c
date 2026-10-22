/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাৱ "lv_obj_class_private.h"
#অন্তর্ভাৱ "../lvgl_public.h"
#অন্তর্ভাৱ "lv_obj_private.h"
#অন্তর্ভাৱ "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
স্থির শূন্য lv_obj_construct(ধ্রুৱ lv_obj_class_t * class_p, lv_obj_t * obj);
স্থির uint32_t get_instance_size(ধ্রুৱ lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(ধ্রুৱ lv_obj_class_t * class_p, lv_obj_t * parent)
{
    LV_CHECK_ARG(class_p != রিক্ত, ঘূৰাই রিক্ত);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (শূন্য *)class_p, (শূন্য *)parent);
    uint32_t s = get_instance_size(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    যদি(obj == রিক্ত) ঘূৰাই রিক্ত;
    obj->class_p = class_p;
    obj->parent = parent;

    /*Create a screen*/
    যদি(parent == রিক্ত) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        যদি(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            ঘূৰাই রিক্ত;
        }

        যদি(disp->screens == রিক্ত) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, পরিমাণ(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        যদি(screens == রিক্ত) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            ঘূৰাই রিক্ত;
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
    অন্যথা {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(parent, MY_CLASS, ঘূৰাই রিক্ত);

        যদি(!lv_obj_allocate_spec_attr(parent)) {
            lv_free(obj);
            ঘূৰাই রিক্ত;
        }
        যদি(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
            lv_free(obj);
            ঘূৰাই রিক্ত;
        }
    }

    ঘূৰাই obj;
}

শূন্য lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_size(obj);

    lv_group_t * def_group = lv_group_get_default();
    যদি(def_group && lv_obj_is_group_def(obj)) {
        lv_group_add_obj(def_group, obj);
    }

    lv_obj_t * parent = lv_obj_get_parent(obj);
    যদি(parent) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
        lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

শূন্য lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই);
    LV_CHECK_ARG(obj->class_p != রিক্ত, ঘূৰাই);

#যদি LV_USE_EXT_DATA
    যদি(obj->ext_data.free_cb) {
        obj->ext_data.free_cb(obj->ext_data.data);
        obj->ext_data.data = রিক্ত;
    }
#endif

    যদি(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    যদি(obj->class_p->base_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->base_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ধ্রুৱ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই false);

    ধ্রুৱ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    যেতিয়ালৈ(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->base_class;

    যদি(class_p == রিক্ত) ঘূৰাই false;

    ঘূৰাই class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_group_def(ধ্রুৱ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই false);

    ধ্রুৱ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    যেতিয়ালৈ(class_p && class_p->group_def == LV_OBJ_CLASS_GROUP_DEF_INHERIT) class_p = class_p->base_class;

    যদি(class_p == রিক্ত) ঘূৰাই false;

    ঘূৰাই class_p->group_def == LV_OBJ_CLASS_GROUP_DEF_TRUE;
}

#যদি LV_USE_EXT_DATA
শূন্য lv_obj_set_external_data(lv_obj_t * obj, শূন্য * data, শূন্য (* free_cb)(শূন্য * data))
{
    LV_CHECK_ARG_বাৰ্তা(obj != রিক্ত, ঘূৰাই, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_data.data = data;
    obj->ext_data.free_cb = free_cb;
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির শূন্য lv_obj_construct(ধ্রুৱ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#যদি LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#endif

#যদি LV_USE_EXT_DATA
    obj->ext_data.free_cb = রিক্ত;
    obj->ext_data.data = রিক্ত;
#endif

    যদি(obj->class_p->base_class) {
        ধ্রুৱ lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->base_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    যদি(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

স্থির uint32_t get_instance_size(ধ্রুৱ lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    যেতিয়ালৈ(class_p && class_p->instance_size == 0) class_p = class_p->base_class;

    LV_ASSERT(class_p != রিক্ত); /*Never happens: set at least in `lv_obj` class*/

    ঘূৰাই class_p->instance_size;
}
