/**
 * @file lv_obj_class.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../display/lv_display_private.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర శూన్య lv_obj_construct(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj);
స్థిర uint32_t get_instance_కొలత(స్థిరరాశి lv_obj_class_t * class_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_obj_class_create_obj(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * మాతృ)
{
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి ఖాళీ);

    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (శూన్య *)class_p, (శూన్య *)మాతృ);
    uint32_t s = get_instance_కొలత(class_p);
    lv_obj_t * obj = lv_malloc_zeroed(s);
    LV_ASSERT_MALLOC(obj);
    ఒకవేళ(obj == ఖాళీ) తిరిగి ఖాళీ;
    obj->class_p = class_p;
    obj->మాతృ = మాతృ;

    /*Create a screen*/
    ఒకవేళ(మాతృ == ఖాళీ) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_display_t * disp = lv_display_get_default();
        ఒకవేళ(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_free(obj);
            తిరిగి ఖాళీ;
        }

        ఒకవేళ(disp->screens == ఖాళీ) {
            disp->screen_cnt = 0;
        }

        lv_obj_t ** screens = lv_realloc(disp->screens, పరిమాణం(lv_obj_t *) * (disp->screen_cnt + 1));
        LV_ASSERT_MALLOC(screens);
        ఒకవేళ(screens == ఖాళీ) {
            LV_LOG_WARN("Failed to expand memory for screen array");
            lv_free(obj);
            తిరిగి ఖాళీ;
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
    లేకపోతే {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_CHECK_OBJ(మాతృ, MY_CLASS, తిరిగి ఖాళీ);

        ఒకవేళ(!lv_obj_allocate_spec_attr(మాతృ)) {
            lv_free(obj);
            తిరిగి ఖాళీ;
        }
        ఒకవేళ(lv_obj_కూడు_బిడ్డ(మాతృ, obj) != LV_ఫలితం_OK) {
            lv_free(obj);
            తిరిగి ఖాళీ;
        }
    }

    తిరిగి obj;
}

శూన్య lv_obj_class_init_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    lv_obj_mark_layout_as_dirty(obj);
    lv_obj_enable_style_refresh(false);

    lv_theme_apply(obj);
    lv_obj_construct(obj->class_p, obj);

    lv_obj_enable_style_refresh(true);
    lv_obj_refresh_style(obj, LV_PART_ANY, LV_STYLE_PROP_ANY);

    lv_obj_refresh_self_కొలత(obj);

    lv_సమూహం_t * def_సమూహం = lv_సమూహం_get_default();
    ఒకవేళ(def_సమూహం && lv_obj_is_సమూహం_def(obj)) {
        lv_సమూహం_కూడు_obj(def_సమూహం, obj);
    }

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ) {
        /*Call the ancestor's event handler to the parent to notify it about the new child.
         *Also triggers layout update*/
        lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, obj);
        lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_CREATED, obj);

        /*Invalidate the area if not screen created*/
        lv_obj_invalidate(obj);
    }
}

శూన్య lv_obj_destruct(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(obj->class_p != ఖాళీ, తిరిగి);

#ఒకవేళ LV_USE_EXT_దత్తాంశం
    ఒకవేళ(obj->ext_దత్తాంశం.free_cb) {
        obj->ext_దత్తాంశం.free_cb(obj->ext_దత్తాంశం.దత్తాంశం);
        obj->ext_దత్తాంశం.దత్తాంశం = ఖాళీ;
    }
#అయితే_ముగింపు

    ఒకవేళ(obj->class_p->destructor_cb) obj->class_p->destructor_cb(obj->class_p, obj);

    ఒకవేళ(obj->class_p->పునాది_class) {
        /*Don't let the descendant methods run during destructing the ancestor type*/
        obj->class_p = obj->class_p->పునాది_class;

        /*Call the base class's destructor too*/
        lv_obj_destruct(obj);
    }
}

bool lv_obj_is_editable(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);

    స్థిరరాశి lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which editable is set*/
    అయితే(class_p && class_p->editable == LV_OBJ_CLASS_EDITABLE_INHERIT) class_p = class_p->పునాది_class;

    ఒకవేళ(class_p == ఖాళీ) తిరిగి false;

    తిరిగి class_p->editable == LV_OBJ_CLASS_EDITABLE_TRUE;
}

bool lv_obj_is_సమూహం_def(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి false);

    స్థిరరాశి lv_obj_class_t * class_p = obj->class_p;

    /*Find a base in which group_def is set*/
    అయితే(class_p && class_p->సమూహం_def == LV_OBJ_CLASS_సమూహం_DEF_INHERIT) class_p = class_p->పునాది_class;

    ఒకవేళ(class_p == ఖాళీ) తిరిగి false;

    తిరిగి class_p->సమూహం_def == LV_OBJ_CLASS_సమూహం_DEF_TRUE;
}

#ఒకవేళ LV_USE_EXT_దత్తాంశం
శూన్య lv_obj_సమితి_external_దత్తాంశం(lv_obj_t * obj, శూన్య * దత్తాంశం, శూన్య (* free_cb)(శూన్య * దత్తాంశం))
{
    LV_CHECK_ARG_సందేశం(obj != ఖాళీ, తిరిగి, "Can't attach external user data and destructor callback to a NULL object");

    obj->ext_దత్తాంశం.దత్తాంశం = దత్తాంశం;
    obj->ext_దత్తాంశం.free_cb = free_cb;
}
#అయితే_ముగింపు

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర శూన్య lv_obj_construct(స్థిరరాశి lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_ASSERT_NULL(class_p);
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(obj->class_p);

#ఒకవేళ LV_USE_OBJ_NAME
    LV_ASSERT_NULL(class_p->name);
#అయితే_ముగింపు

#ఒకవేళ LV_USE_EXT_దత్తాంశం
    obj->ext_దత్తాంశం.free_cb = ఖాళీ;
    obj->ext_దత్తాంశం.దత్తాంశం = ఖాళీ;
#అయితే_ముగింపు

    ఒకవేళ(obj->class_p->పునాది_class) {
        స్థిరరాశి lv_obj_class_t * original_class_p = obj->class_p;

        /*Don't let the descendant methods run during constructing the ancestor type*/
        obj->class_p = obj->class_p->పునాది_class;

        /*Construct the base first*/
        lv_obj_construct(class_p, obj);

        /*Restore the original class*/
        obj->class_p = original_class_p;
    }

    ఒకవేళ(obj->class_p->constructor_cb) obj->class_p->constructor_cb(class_p, obj);
}

స్థిర uint32_t get_instance_కొలత(స్థిరరాశి lv_obj_class_t * class_p)
{
    /*Find a base in which instance size is set*/
    అయితే(class_p && class_p->instance_కొలత == 0) class_p = class_p->పునాది_class;

    LV_ASSERT(class_p != ఖాళీ); /*Never happens: set at least in `lv_obj` class*/

    తిరిగి class_p->instance_కొలత;
}
