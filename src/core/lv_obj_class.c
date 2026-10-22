/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_construct(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj);
ಸ್ಥಿರ uint32_t get_instance_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * ಪೋಷಕ)
{
    LV_CHECK_ARG(class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (ಶೂನ್ಯ *)class_p, (ಶೂನ್ಯ *)ಪೋಷಕ);
    uint32_t s = get_instance_ಗಾತ್ರ(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    ಆದರೆ(obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    obj->class_p = class_p;
    obj->ಪೋಷಕ = ಪೋಷಕ;

    /*Create a screen*/
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        ಆದರೆ(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        ಆದರೆ(disp->screens == ಖಾಲಿ) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, ಗಾತ್ರ(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        ಆದರೆ(screens == ಖಾಲಿ) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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
    ಇಲ್ಲದಿದ್ದರೆ {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(ಪೋಷಕ, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

        ಆದರೆ(!lv_obj_allocate_spec_attr(ಪೋಷಕ)) {
            lv_free(obj);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
        ಆದರೆ(lv_obj_ಕೂಡಿಸು_ಮಗು(ಪೋಷಕ, obj) != LV_ಫಲಿತಾಂಶ_OK) {
            lv_free(obj);
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }

    ಹಿಂತಿರುಗಿ obj;
}

ಶೂನ್ಯ lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_ಗಾತ್ರ(obj);

    lv_ಗುಂಪು_t * def_ಗುಂಪು = lv_ಗುಂಪು_get_default();
    ಆದರೆ(def_ಗುಂಪು && lv_obj_is_ಗುಂಪು_def(obj)) {
        lv_ಗುಂಪು_ಕೂಡಿಸು_obj(def_ಗುಂಪು, obj);
    }

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj);
        lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

ಶೂನ್ಯ lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(obj->class_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    ಆದರೆ(obj->ext_ದತ್ತಾಂಶ.free_cb) {
        obj->ext_ದತ್ತಾಂಶ.free_cb(obj->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ);
        obj->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ಖಾಲಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    ಆದರೆ(obj->class_p->ತಳ_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->ತಳ_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    ಆಗ(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->ತಳ_class;

    ಆದರೆ(class_p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    ಹಿಂತಿರುಗಿ class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_ಗುಂಪು_def(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);

    ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    ಆಗ(class_p && class_p->ಗುಂಪು_def == LV_OBJ_CLASS_ಗುಂಪು_DEF_INHERIT) class_p = class_p->ತಳ_class;

    ಆದರೆ(class_p == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    ಹಿಂತಿರುಗಿ class_p->ಗುಂಪು_def == LV_OBJ_CLASS_ಗುಂಪು_DEF_TRUE;
}

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
ಶೂನ್ಯ lv_obj_ಗಣ_external_ದತ್ತಾಂಶ(lv_obj_t * obj, ಶೂನ್ಯ * ದತ್ತಾಂಶ, ಶೂನ್ಯ (* free_cb)(ಶೂನ್ಯ * ದತ್ತಾಂಶ))
{
    LV_CHECK_ARG_ಸಂದೇಶ(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    obj->ext_ದತ್ತಾಂಶ.free_cb = free_cb;
}
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ lv_obj_construct(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#ಆದರೆ LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_EXT_ದತ್ತಾಂಶ
    obj->ext_ದತ್ತಾಂಶ.free_cb = ಖಾಲಿ;
    obj->ext_ದತ್ತಾಂಶ.ದತ್ತಾಂಶ = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ(obj->class_p->ತಳ_class) {
        ಸ್ಥಿರಾಂಕ lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->ತಳ_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    ಆದರೆ(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

ಸ್ಥಿರ uint32_t get_instance_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    ಆಗ(class_p && class_p->instance_ಗಾತ್ರ == 0) class_p = class_p->ತಳ_class;

    LV_ASSERT(class_p != ಖಾಲಿ); /*Never happens: set at least in `lv_obj` class*/

    ಹಿಂತಿರುಗಿ class_p->instance_ಗಾತ್ರ;
}
