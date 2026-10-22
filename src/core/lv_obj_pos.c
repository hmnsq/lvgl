/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "../misc/lv_area_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../layouts/lv_layout_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_event_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_draw_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"
#ಅನ್ತರ್ಭಾವ "lv_refr_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_class_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)
#ವ್ಯಾಖ್ಯಾನಿಸು update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #ವ್ಯಾಖ್ಯಾನಿಸು LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #ದೋಷ "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ int32_t calc_content_ಅಗಲ(lv_obj_t * obj);
ಸ್ಥಿರ int32_t calc_content_ಎತ್ತರ(lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ layout_update_core(lv_obj_t * obj);
ಸ್ಥಿರ ಶೂನ್ಯ transform_ಬಿಂದು_array(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_ಬಿಂದು_t * p, ಗಾತ್ರ_t p_ಎಣಿಕೆ, bool inv);
ಸ್ಥಿರ bool is_transformed(ಸ್ಥಿರಾಂಕ lv_obj_t * obj);
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t invalidate_area_core(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area_tmp);
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t obj_invalidate_area_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_area_t * area);
ಸ್ಥಿರ int32_t calc_dynamic_ಅಗಲ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_ಅಗಲ);
ಸ್ಥಿರ int32_t calc_dynamic_ಎತ್ತರ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_ಎತ್ತರ);
ಸ್ಥಿರ bool ಗಾತ್ರ_ಒಳ_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t ಗಾತ್ರ_style,
                                  int32_t min_style, int32_t max_style);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_obj_ಗಣ_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_ಗಣ_x(obj, x);
    lv_obj_ಗಣ_y(obj, y);
}

ಶೂನ್ಯ lv_obj_ಗಣ_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_style_res_t res_x;
    lv_style_ಮೌಲ್ಯ_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    ಆದರೆ((res_x == LV_STYLE_RES_ಸಿಕ್ಕಿತು && v_x.ಅಂಕೆ != x) || res_x == LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು) {
        lv_obj_ಗಣ_style_x(obj, x, 0);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_style_res_t res_y;
    lv_style_ಮೌಲ್ಯ_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    ಆದರೆ((res_y == LV_STYLE_RES_ಸಿಕ್ಕಿತು && v_y.ಅಂಕೆ != y) || res_y == LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು) {
        lv_obj_ಗಣ_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_ಅಗಲ(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG(prop == LV_STYLE_ಅಗಲ || prop == LV_STYLE_MIN_ಅಗಲ || prop == LV_STYLE_MAX_ಅಗಲ, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ calc_dynamic_ಅಗಲ(obj, prop, ಖಾಲಿ);
}

int32_t lv_obj_calc_dynamic_ಎತ್ತರ(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);
    LV_CHECK_ARG(prop == LV_STYLE_ಎತ್ತರ || prop == LV_STYLE_MIN_ಎತ್ತರ || prop == LV_STYLE_MAX_ಎತ್ತರ, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ calc_dynamic_ಎತ್ತರ(obj, prop, ಖಾಲಿ);
}

bool lv_obj_refr_ಗಾತ್ರ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    /*If the width or height is set by a layout do not modify them*/
    ಆದರೆ(obj->w_layout && obj->h_layout) ಹಿಂತಿರುಗಿ false;

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    int32_t w;
    ಆದರೆ(obj->w_layout) {
        w = lv_obj_get_ಅಗಲ(obj);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        int32_t content_ಅಗಲ = -1;
        int32_t unclamped_w = calc_dynamic_ಅಗಲ(obj, LV_STYLE_ಅಗಲ, &content_ಅಗಲ);
        int32_t minw = calc_dynamic_ಅಗಲ(obj, LV_STYLE_MIN_ಅಗಲ, &content_ಅಗಲ);
        int32_t maxw = calc_dynamic_ಅಗಲ(obj, LV_STYLE_MAX_ಅಗಲ, &content_ಅಗಲ);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = ಗಾತ್ರ_ಒಳ_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN));
        obj->w_ignore_ಗಾತ್ರ =
            (w_pct && ಪೋಷಕ->w_layout == 0 && lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(ಪೋಷಕ, 0) == LV_ಗಾತ್ರ_CONTENT);
    }

    int32_t h;
    ಆದರೆ(obj->h_layout) {
        h = lv_obj_get_ಎತ್ತರ(obj);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        int32_t content_ಎತ್ತರ = -1;
        int32_t unclamped_h = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_ಎತ್ತರ, &content_ಎತ್ತರ);
        int32_t minh = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MIN_ಎತ್ತರ, &content_ಎತ್ತರ);
        int32_t maxh = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MAX_ಎತ್ತರ, &content_ಎತ್ತರ);
        h = LV_CLAMP(minh, unclamped_h, maxh);

        /**
         * If the height in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `h_ignore_size` excludes this object from the
         * parent's content height calculation. See the width branch above.
         */
        bool h_pct = ಗಾತ್ರ_ಒಳ_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN));

        obj->h_ignore_ಗಾತ್ರ = (h_pct && ಪೋಷಕ->h_layout == 0 &&
                              lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(ಪೋಷಕ, 0) == LV_ಗಾತ್ರ_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    ಆದರೆ(lv_obj_get_ಅಗಲ(obj) == w && lv_obj_get_ಎತ್ತರ(obj) == h)
        ಹಿಂತಿರುಗಿ false;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t ಪೋಷಕ_fit_area;
    lv_obj_get_content_coords(ಪೋಷಕ, &ಪೋಷಕ_fit_area);

    /*If the object is already out of the parent and its position is changes
     *surely the scrollbars also changes so invalidate them*/
    bool on1 = lv_area_is_ಒಳ(&ori, &ಪೋಷಕ_fit_area, 0);
    ಆದರೆ(!on1)
        lv_obj_scrollbar_invalidate(ಪೋಷಕ);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) == LV_ತಳ_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_ಗಾತ್ರ_ಬದಲಾಯಿತು, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_ಒಳ(&obj->coords, &ಪೋಷಕ_fit_area, 0);
    ಆದರೆ(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(ಪೋಷಕ);

    lv_obj_refresh_ext_draw_ಗಾತ್ರ(obj);

    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ lv_obj_ಗಣ_ಗಾತ್ರ(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_ಗಣ_ಅಗಲ(obj, w);
    lv_obj_ಗಣ_ಎತ್ತರ(obj, h);
}

ಶೂನ್ಯ lv_obj_ಗಣ_ಅಗಲ(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    lv_style_res_t res_w;
    lv_style_ಮೌಲ್ಯ_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_ಅಗಲ, &v_w, 0);

    ಆದರೆ((res_w == LV_STYLE_RES_ಸಿಕ್ಕಿತು && v_w.ಅಂಕೆ != w) || res_w == LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು) {
        lv_obj_ಗಣ_style_ಅಗಲ(obj, w, 0);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_ಎತ್ತರ(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    lv_style_res_t res_h;
    lv_style_ಮೌಲ್ಯ_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_ಎತ್ತರ, &v_h, 0);

    ಆದರೆ((res_h == LV_STYLE_RES_ಸಿಕ್ಕಿತು && v_h.ಅಂಕೆ != h) || res_h == LV_STYLE_RES_NOT_ಸಿಕ್ಕಿತು) {
        lv_obj_ಗಣ_style_ಎತ್ತರ(obj, h, 0);
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_content_ಅಗಲ(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    int32_t ಎಡ = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t ಬಲ = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    lv_obj_ಗಣ_ಅಗಲ(obj, w + ಎಡ + ಬಲ);
}

ಶೂನ್ಯ lv_obj_ಗಣ_content_ಎತ್ತರ(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    int32_t ತುದಿ = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    lv_obj_ಗಣ_ಎತ್ತರ(obj, h + ತುದಿ + bottom);
}

ಶೂನ್ಯ lv_obj_ಗಣ_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_ಗಣ_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) ಹಿಂತಿರುಗಿ false;

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ false;

    uint32_t layout = lv_obj_get_style_layout_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    ಆದರೆ(layout) ಹಿಂತಿರುಗಿ true;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ false;
}

ಶೂನ್ಯ lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ಖಾಲಿ);
}

ಶೂನ್ಯ lv_obj_update_layout(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        ಹಿಂತಿರುಗಿ;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    ಆಗ(scr->scr_layout_inv) {
        ಆದರೆ(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_ಸಂದೇಶ(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (ಶೂನ್ಯ *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            ನಿಲ್ಲಿಸು;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, ಖಾಲಿ);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_ಕೊನೆ;
}

ಶೂನ್ಯ lv_obj_ಗಣ_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_ಗಣ_style_align(obj, align, 0);
}

ಶೂನ್ಯ lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    lv_obj_ಗಣ_style_align(obj, align, 0);
    lv_obj_ಗಣ_pos(obj, x_ofs, y_ofs);
}

ಶೂನ್ಯ lv_obj_align_ಗಮ್ಯ(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_obj_t * ತಳ, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_update_layout(obj);
    ಆದರೆ(ತಳ == ಖಾಲಿ) ತಳ = lv_obj_get_ಪೋಷಕ(obj);

    ಆದರೆ(ತಳ == ಖಾಲಿ) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        ಹಿಂತಿರುಗಿ;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        ಹಿಂತಿರುಗಿ;
    }

    int32_t pleft = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ತಳ, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ತಳ, LV_PART_MAIN);

    ಆದರೆ(align == LV_ALIGN_DEFAULT) {
        ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(ತಳ, LV_PART_MAIN) == LV_ತಳ_DIR_RTL) align = LV_ALIGN_ತುದಿ_ಬಲ;
        ಇಲ್ಲದಿದ್ದರೆ align = LV_ALIGN_ತುದಿ_ಎಡ;
    }

    ಆಯ್ಕೆ(align) {
        ಸ್ಥಿತಿ LV_ALIGN_CENTER:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) / 2 - lv_obj_get_ಅಗಲ(obj) / 2 + bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) / 2 - lv_obj_get_ಎತ್ತರ(obj) / 2 + btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಎಡ:
            x = bleft;
            y = btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_MID:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) / 2 - lv_obj_get_ಅಗಲ(obj) / 2 + bleft;
            y = btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಬಲ:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) - lv_obj_get_ಅಗಲ(obj) + bleft;
            y = btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಎಡ:
            x = bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) - lv_obj_get_ಎತ್ತರ(obj) + btop;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) / 2 - lv_obj_get_ಅಗಲ(obj) / 2 + bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) - lv_obj_get_ಎತ್ತರ(obj) + btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಬಲ:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) - lv_obj_get_ಅಗಲ(obj) + bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) - lv_obj_get_ಎತ್ತರ(obj) + btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಎಡ_MID:
            x = bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) / 2 - lv_obj_get_ಎತ್ತರ(obj) / 2 + btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಬಲ_MID:
            x = lv_obj_get_content_ಅಗಲ(ತಳ) - lv_obj_get_ಅಗಲ(obj) + bleft;
            y = lv_obj_get_content_ಎತ್ತರ(ತಳ) / 2 - lv_obj_get_ಎತ್ತರ(obj) / 2 + btop;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ತುದಿ_ಎಡ:
            x = 0;
            y = -lv_obj_get_ಎತ್ತರ(obj);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ತುದಿ_MID:
            x = lv_obj_get_ಅಗಲ(ತಳ) / 2 - lv_obj_get_ಅಗಲ(obj) / 2;
            y = -lv_obj_get_ಎತ್ತರ(obj);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ತುದಿ_ಬಲ:
            x = lv_obj_get_ಅಗಲ(ತಳ) - lv_obj_get_ಅಗಲ(obj);
            y = -lv_obj_get_ಎತ್ತರ(obj);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_BOTTOM_ಎಡ:
            x = 0;
            y = lv_obj_get_ಎತ್ತರ(ತಳ);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_BOTTOM_MID:
            x = lv_obj_get_ಅಗಲ(ತಳ) / 2 - lv_obj_get_ಅಗಲ(obj) / 2;
            y = lv_obj_get_ಎತ್ತರ(ತಳ);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_BOTTOM_ಬಲ:
            x = lv_obj_get_ಅಗಲ(ತಳ) - lv_obj_get_ಅಗಲ(obj);
            y = lv_obj_get_ಎತ್ತರ(ತಳ);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಎಡ_ತುದಿ:
            x = -lv_obj_get_ಅಗಲ(obj);
            y = 0;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಎಡ_MID:
            x = -lv_obj_get_ಅಗಲ(obj);
            y = lv_obj_get_ಎತ್ತರ(ತಳ) / 2 - lv_obj_get_ಎತ್ತರ(obj) / 2;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಎಡ_BOTTOM:
            x = -lv_obj_get_ಅಗಲ(obj);
            y = lv_obj_get_ಎತ್ತರ(ತಳ) - lv_obj_get_ಎತ್ತರ(obj);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಬಲ_ತುದಿ:
            x = lv_obj_get_ಅಗಲ(ತಳ);
            y = 0;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಬಲ_MID:
            x = lv_obj_get_ಅಗಲ(ತಳ);
            y = lv_obj_get_ಎತ್ತರ(ತಳ) / 2 - lv_obj_get_ಎತ್ತರ(obj) / 2;
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_ಹೊರಹೋಗು_ಬಲ_BOTTOM:
            x = lv_obj_get_ಅಗಲ(ತಳ);
            y = lv_obj_get_ಎತ್ತರ(ತಳ) - lv_obj_get_ಎತ್ತರ(obj);
            ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LV_ALIGN_DEFAULT:
            ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_ಅಗಲ(ತಳ) * LV_COORD_GET_PCT(x_ofs)) / 100;
    ಆದರೆ(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_ಎತ್ತರ(ತಳ) * LV_COORD_GET_PCT(y_ofs)) / 100;
    ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) == LV_ತಳ_DIR_RTL) {
        x += x_ofs + ತಳ->coords.x1 - ಪೋಷಕ->coords.x1 + lv_obj_get_scroll_ಬಲ(ಪೋಷಕ) - pleft;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        x += x_ofs + ತಳ->coords.x1 - ಪೋಷಕ->coords.x1 + lv_obj_get_scroll_ಎಡ(ಪೋಷಕ) - pleft;
    }
    y += y_ofs + ತಳ->coords.y1 - ಪೋಷಕ->coords.y1 + lv_obj_get_scroll_ತುದಿ(ಪೋಷಕ) - ptop;
    lv_obj_ಗಣ_style_align(obj, LV_ALIGN_ತುದಿ_ಎಡ, 0);
    lv_obj_ಗಣ_pos(obj, x, y);

}

ಶೂನ್ಯ lv_obj_get_coords(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t rel_x;
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ) {
        rel_x  = obj->coords.x1 - ಪೋಷಕ->coords.x1;
        rel_x += lv_obj_get_scroll_x(ಪೋಷಕ);
        rel_x -= lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        rel_x = obj->coords.x1;
    }
    ಹಿಂತಿರುಗಿ rel_x;
}

int32_t lv_obj_get_x2(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_obj_get_x(obj) + lv_obj_get_ಅಗಲ(obj);
}

int32_t lv_obj_get_y(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t rel_y;
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ) {
        rel_y = obj->coords.y1 - ಪೋಷಕ->coords.y1;
        rel_y += lv_obj_get_scroll_y(ಪೋಷಕ);
        rel_y -= lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        rel_y = obj->coords.y1;
    }
    ಹಿಂತಿರುಗಿ rel_y;
}

int32_t lv_obj_get_y2(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_obj_get_y(obj) + lv_obj_get_ಎತ್ತರ(obj);
}

int32_t lv_obj_get_x_aligned(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_obj_get_style_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_obj_get_style_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_ಅಗಲ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_area_get_ಅಗಲ(&obj->coords);
}

int32_t lv_obj_get_ಎತ್ತರ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಹಿಂತಿರುಗಿ lv_area_get_ಎತ್ತರ(&obj->coords);
}

int32_t lv_obj_get_content_ಅಗಲ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t ಎಡ = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t ಬಲ = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಹಿಂತಿರುಗಿ lv_obj_get_ಅಗಲ(obj) - ಎಡ - ಬಲ;
}

int32_t lv_obj_get_content_ಎತ್ತರ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t ತುದಿ = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಹಿಂತಿರುಗಿ lv_obj_get_ಎತ್ತರ(obj) - ತುದಿ - bottom;
}

ಶೂನ್ಯ lv_obj_get_content_coords(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_ಅಗಲ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    lv_ಬಿಂದು_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_ಗಾತ್ರ, &p);
    ಹಿಂತಿರುಗಿ p.x;
}

int32_t lv_obj_get_self_ಎತ್ತರ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    lv_ಬಿಂದು_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_ಗಾತ್ರ, &p);
    ಹಿಂತಿರುಗಿ p.y;
}

int32_t lv_obj_get_style_clamped_ಅಗಲ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t content_ಅಗಲ = -1;
    int32_t w = calc_dynamic_ಅಗಲ(obj, LV_STYLE_ಅಗಲ, &content_ಅಗಲ);
    int32_t minw = calc_dynamic_ಅಗಲ(obj, LV_STYLE_MIN_ಅಗಲ, &content_ಅಗಲ);
    int32_t maxw = calc_dynamic_ಅಗಲ(obj, LV_STYLE_MAX_ಅಗಲ, &content_ಅಗಲ);
    ಆದರೆ(w <= minw) {
        w = lv_obj_get_style_min_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(w >= maxw) {
        w = lv_obj_get_style_max_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        w = lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಹಿಂತಿರುಗಿ w;
}

int32_t lv_obj_get_style_clamped_ಎತ್ತರ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t content_ಎತ್ತರ = -1;
    int32_t h = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_ಎತ್ತರ, &content_ಎತ್ತರ);
    int32_t minh = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MIN_ಎತ್ತರ, &content_ಎತ್ತರ);
    int32_t maxh = calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MAX_ಎತ್ತರ, &content_ಎತ್ತರ);
    ಆದರೆ(h <= minh) {
        h = lv_obj_get_style_min_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(h >= maxh) {
        h = lv_obj_get_style_max_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        h = lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    }
    ಹಿಂತಿರುಗಿ h;
}

bool lv_obj_is_style_any_ಅಗಲ_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t w = lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    ಹಿಂತಿರುಗಿ (w == LV_ಗಾತ್ರ_CONTENT || minw == LV_ಗಾತ್ರ_CONTENT || maxw == LV_ಗಾತ್ರ_CONTENT);
}

bool lv_obj_is_style_any_ಎತ್ತರ_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t h = lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_ಎತ್ತರ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    ಹಿಂತಿರುಗಿ (h == LV_ಗಾತ್ರ_CONTENT || minh == LV_ಗಾತ್ರ_CONTENT || maxh == LV_ಗಾತ್ರ_CONTENT);
}

bool lv_obj_is_ಅಗಲ_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t minw = lv_obj_calc_dynamic_ಅಗಲ(obj, LV_STYLE_MIN_ಅಗಲ);
    int32_t w = lv_obj_get_ಅಗಲ(obj);
    ಹಿಂತಿರುಗಿ w == minw;
}

bool lv_obj_is_ಎತ್ತರ_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t minh = lv_obj_calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MIN_ಎತ್ತರ);
    int32_t h = lv_obj_get_ಎತ್ತರ(obj);
    ಹಿಂತಿರುಗಿ h == minh;
}

bool lv_obj_is_ಅಗಲ_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t maxw = lv_obj_calc_dynamic_ಅಗಲ(obj, LV_STYLE_MAX_ಅಗಲ);
    int32_t w = lv_obj_get_ಅಗಲ(obj);
    ಹಿಂತಿರುಗಿ w == maxw;
}

bool lv_obj_is_ಎತ್ತರ_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    int32_t maxh = lv_obj_calc_dynamic_ಎತ್ತರ(obj, LV_STYLE_MAX_ಎತ್ತರ);
    int32_t h = lv_obj_get_ಎತ್ತರ(obj);
    ಹಿಂತಿರುಗಿ h == maxh;
}

bool lv_obj_refresh_self_ಗಾತ್ರ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(!lv_obj_is_style_any_ಅಗಲ_content(obj) && !lv_obj_is_style_any_ಎತ್ತರ_content(obj))
        ಹಿಂತಿರುಗಿ false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(ಪೋಷಕ != ಖಾಲಿ) {
        ಪೋಷಕ->w_layout = 0;
        ಪೋಷಕ->h_layout = 0;
        lv_obj_mark_layout_as_dirty(ಪೋಷಕ);
    }
    lv_obj_mark_layout_as_dirty(obj);
    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(lv_obj_is_layout_positioned(obj)) ಹಿಂತಿರುಗಿ;

    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    int32_t x = lv_obj_get_style_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಆದರೆ(ಪೋಷಕ == ಖಾಲಿ) {
        lv_obj_move_ಗಮ್ಯ(obj, x, y);
        ಹಿಂತಿರುಗಿ;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_ಅಗಲ(ಪೋಷಕ);
    int32_t ph = lv_obj_get_content_ಎತ್ತರ(ಪೋಷಕ);
    ಆದರೆ(LV_COORD_IS_PCT(x)) {
        ಆದರೆ(lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) == LV_ಗಾತ್ರ_CONTENT) x = 0; /*Avoid circular dependency*/
        ಇಲ್ಲದಿದ್ದರೆ x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    ಆದರೆ(LV_COORD_IS_PCT(y)) {
        ಆದರೆ(lv_obj_get_style_ಎತ್ತರ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) == LV_ಗಾತ್ರ_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_ಅಗಲ(obj);
    int32_t h = lv_obj_get_ಎತ್ತರ(obj);
    ಆದರೆ(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    ಆದರೆ(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಆದರೆ(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_ತುದಿ_ಎಡ;
    }

    bool rtl = lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN) == LV_ತಳ_DIR_RTL;

    ಆದರೆ(rtl) {
        ಆಯ್ಕೆ(align) {
            ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಎಡ:
                align = LV_ALIGN_ತುದಿ_ಬಲ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಬಲ:
                align = LV_ALIGN_ತುದಿ_ಎಡ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_ALIGN_ಎಡ_MID:
                align = LV_ALIGN_ಬಲ_MID;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_ALIGN_ಬಲ_MID:
                align = LV_ALIGN_ಎಡ_MID;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಎಡ:
                align = LV_ALIGN_BOTTOM_ಬಲ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಬಲ:
                align = LV_ALIGN_BOTTOM_ಎಡ;
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                ನಿಲ್ಲಿಸು;
        }
    }

    ಆಯ್ಕೆ(align) {
        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಎಡ:
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಬಲ:
            x = rtl ? pw - w - x : pw - w + x;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_ಎಡ_MID:
            y += ph / 2 - h / 2;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಎಡ:
            y += ph - h;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಬಲ:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_ಬಲ_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
    }

    lv_obj_move_ಗಮ್ಯ(obj, x, y);
}

ಶೂನ್ಯ lv_obj_move_ಗಮ್ಯ(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * ಪೋಷಕ = obj->ಪೋಷಕ;

    ಆದರೆ(ಪೋಷಕ) {
        ಆದರೆ(lv_obj_is_floating(obj)) {
            x += ಪೋಷಕ->coords.x1;
            y += ಪೋಷಕ->coords.y1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            x += ಪೋಷಕ->coords.x1 - lv_obj_get_scroll_x(ಪೋಷಕ);
            y += ಪೋಷಕ->coords.y1 - lv_obj_get_scroll_y(ಪೋಷಕ);
        }

        x += lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
        y += lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_ಬಿಂದು_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    ಆದರೆ(diff.x == 0 && diff.y == 0) ಹಿಂತಿರುಗಿ;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t ಪೋಷಕ_fit_area;
    bool on1 = false;
    ಆದರೆ(ಪೋಷಕ) {
        lv_obj_get_content_coords(ಪೋಷಕ, &ಪೋಷಕ_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_ಒಳ(&ori, &ಪೋಷಕ_fit_area, 0);
        ಆದರೆ(!on1) lv_obj_scrollbar_invalidate(ಪೋಷಕ);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    ಆದರೆ(ಪೋಷಕ) lv_obj_send_event(ಪೋಷಕ, LV_EVENT_ಮಗು_ಬದಲಾಯಿತು, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    ಆದರೆ(ಪೋಷಕ) {
        bool on2 = lv_area_is_ಒಳ(&obj->coords, &ಪೋಷಕ_fit_area, 0);
        ಆದರೆ(!on1 || !on2) lv_obj_scrollbar_invalidate(ಪೋಷಕ);
    }
}

ಶೂನ್ಯ lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    uint32_t i;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        ಆದರೆ(ignore_floating && lv_obj_is_floating(ಮಗು)) ಮುಂದುವರಿ;
        ಮಗು->coords.x1 += x_diff;
        ಮಗು->coords.y1 += y_diff;
        ಮಗು->coords.x2 += x_diff;
        ಮಗು->coords.y2 += y_diff;

        lv_obj_move_children_by(ಮಗು, x_diff, y_diff, false);
    }
}

ಶೂನ್ಯ lv_obj_transform_ಬಿಂದು(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_ಬಿಂದು_t * p, lv_obj_ಬಿಂದು_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_obj_transform_ಬಿಂದು_array(obj, p, 1, flags);
}

ಶೂನ್ಯ lv_obj_transform_ಬಿಂದು_array(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_ಬಿಂದು_t points[], ಗಾತ್ರ_t ಎಣಿಕೆ_2,
                                  lv_obj_ಬಿಂದು_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(points != ಖಾಲಿ || ಎಣಿಕೆ_2 == 0, ಹಿಂತಿರುಗಿ);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆದರೆ(inverse) {
        ಆದರೆ(recursive && ಪೋಷಕ) lv_obj_transform_ಬಿಂದು_array(ಪೋಷಕ, points, ಎಣಿಕೆ_2, flags);
        ಆದರೆ(do_tranf) transform_ಬಿಂದು_array(obj, points, ಎಣಿಕೆ_2, inverse);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ(do_tranf) transform_ಬಿಂದು_array(obj, points, ಎಣಿಕೆ_2, inverse);
        ಆದರೆ(recursive && ಪೋಷಕ) lv_obj_transform_ಬಿಂದು_array(ಪೋಷಕ, points, ಎಣಿಕೆ_2, flags);
    }
}

ಶೂನ್ಯ lv_obj_get_transformed_area(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area, lv_obj_ಬಿಂದು_transform_flag_t flags)
{
    LV_CHECK_ARG(area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_ಬಿಂದು_t p[4] = {
        {area->x1, area->y1},
        {area->x1, area->y2 + 1},
        {area->x2 + 1, area->y1},
        {area->x2 + 1, area->y2 + 1},
    };

    lv_obj_transform_ಬಿಂದು_array(obj, p, 4, flags);

    area->x1 = LV_MIN4(p[0].x, p[1].x, p[2].x, p[3].x);
    area->x2 = LV_MAX4(p[0].x, p[1].x, p[2].x, p[3].x);
    area->y1 = LV_MIN4(p[0].y, p[1].y, p[2].y, p[3].y);
    area->y2 = LV_MAX4(p[0].y, p[1].y, p[2].y, p[3].y);
}

/*
 * Deferred blur invalidation expansion. A blur object samples the pixels
 * behind it, so when anything behind it changes the blur object must be
 * redrawn too. Once per frame, after all invalidations are collected, walk
 * the tree and add the full extent of any blur object overlapping an
 * invalidated area. The walk repeats until a pass adds no new areas, which
 * catches transitive cases (a blur object overlapping another blur object
 * that overlaps an invalidated area).
 *
 * Each widget's blur status is cached in obj->has_blur and counted globally in
 * blur_obj_cnt (see lv_obj_update_blur_status()), so the walk is skipped
 * entirely while no widget has blur and costs one bit test per widget when it
 * does run.
 */

ಸ್ಥಿರ lv_obj_ವೃಕ್ಷ_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, ಶೂನ್ಯ * user_ದತ್ತಾಂಶ)
{
    ಆದರೆ(!obj->has_blur) ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಮುಂದಿನ;

    /*invalidate_area_core() expects untransformed coordinates and applies the
     *transform itself*/
    lv_area_t obj_coords;
    int32_t ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ಗಾತ್ರ, ext_ಗಾತ್ರ);

    /*The invalidated areas are in screen coordinates so check the overlap
     *against the transformed area*/
    lv_area_t scr_coords;
    scr_coords = obj_coords;
    ಆದರೆ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_ದತ್ತಾಂಶ;
    uint32_t i;
    ಗಾಗಿ(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        ಆದರೆ(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_SKIP_CHILDREN;
        }
    }

    ಹಿಂತಿರುಗಿ LV_OBJ_ವೃಕ್ಷ_WALK_ಮುಂದಿನ;
}

ಶೂನ್ಯ lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    ಆದರೆ(disp->inv_p == 0) ಹಿಂತಿರುಗಿ;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    ಆದರೆ(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) ಹಿಂತಿರುಗಿ;

    uint32_t prev_inv_p;
    ಮಾಡು {
        prev_inv_p = disp->inv_p;

        lv_obj_ವೃಕ್ಷ_walk(disp->act_scr, blur_expand_walk_cb, disp);
        ಆದರೆ(disp->prev_scr) lv_obj_ವೃಕ್ಷ_walk(disp->prev_scr, blur_expand_walk_cb, disp);
        lv_obj_ವೃಕ್ಷ_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_ವೃಕ್ಷ_walk(disp->ತುದಿ_layer, blur_expand_walk_cb, disp);
        lv_obj_ವೃಕ್ಷ_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } ಆಗ(disp->inv_p > prev_inv_p);
}

lv_ಫಲಿತಾಂಶ_t lv_obj_invalidate_area(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_area_t * area)
{
    LV_CHECK_ARG(area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    ಆದರೆ(!lv_display_is_invalidation_enabled(disp)) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    ಆದರೆ(obj->has_blur) ಹಿಂತಿರುಗಿ lv_obj_invalidate(obj);
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ obj_invalidate_area_ಆಂತರಿಕ(obj, area);
}


lv_ಫಲಿತಾಂಶ_t lv_obj_invalidate(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    ಆದರೆ(!lv_display_is_invalidation_enabled(disp)) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_ಗಾತ್ರ;
    obj_coords.y1 -= ext_ಗಾತ್ರ;
    obj_coords.x2 += ext_ಗಾತ್ರ;
    obj_coords.y2 += ext_ಗಾತ್ರ;

    lv_ಫಲಿತಾಂಶ_t res = obj_invalidate_area_ಆಂತರಿಕ(obj, &obj_coords);

    ಹಿಂತಿರುಗಿ res;
}

bool lv_obj_area_is_visible(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(lv_obj_is_hidden(obj)) ಹಿಂತಿರುಗಿ false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    ಆದರೆ(obj_scr != lv_display_get_screen_ಸಕ್ರಿಯ(disp) &&
       obj_scr != lv_display_get_screen_prev(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_ತುದಿ(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        ಹಿಂತಿರುಗಿ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ಗಾತ್ರ, ext_ಗಾತ್ರ);

    /*The area is not on the object*/
    ಆದರೆ(!lv_area_intersect(area, area, &obj_coords)) ಹಿಂತಿರುಗಿ false;

    ಆದರೆ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
    ಆಗ(ಪೋಷಕ != ಖಾಲಿ) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        ಆದರೆ(lv_obj_is_hidden(ಪೋಷಕ)) ಹಿಂತಿರುಗಿ false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t ಪೋಷಕ_coords = ಪೋಷಕ->coords;
        ಆದರೆ(lv_obj_is_ಅತಿಪ್ರವಾಹ_visible(ಪೋಷಕ)) {
            int32_t ಪೋಷಕ_ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(ಪೋಷಕ);
            lv_area_increase(&ಪೋಷಕ_coords, ಪೋಷಕ_ext_ಗಾತ್ರ, ಪೋಷಕ_ext_ಗಾತ್ರ);
        }

        ಆದರೆ(is_transformed(ಪೋಷಕ)) {
            lv_obj_get_transformed_area(ಪೋಷಕ, &ಪೋಷಕ_coords, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_RECURSIVE);
        }
        ಆದರೆ(!lv_area_intersect(area, area, &ಪೋಷಕ_coords)) ಹಿಂತಿರುಗಿ false;

        ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಪೋಷಕ);
    }

    ಹಿಂತಿರುಗಿ true;
}

bool lv_obj_is_visible(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    lv_area_t obj_coords;
    int32_t ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_ಗಾತ್ರ;
    obj_coords.y1 -= ext_ಗಾತ್ರ;
    obj_coords.x2 += ext_ಗಾತ್ರ;
    obj_coords.y2 += ext_ಗಾತ್ರ;

    ಹಿಂತಿರುಗಿ lv_obj_area_is_visible(obj, &obj_coords);
}

ಶೂನ್ಯ lv_obj_ಗಣ_ext_click_area(lv_obj_t * obj, int32_t ಗಾತ್ರ_2)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }
    obj->spec_attr->ext_click_pad = ಗಾತ್ರ_2;
}

ಶೂನ್ಯ lv_obj_get_click_area(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    *area = obj->coords;
    ಆದರೆ(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_ದೊರಕಿತು_test(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_ಬಿಂದು_t * ಬಿಂದು)
{
    LV_CHECK_ARG(ಬಿಂದು != ಖಾಲಿ, ಹಿಂತಿರುಗಿ false);
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    ಆದರೆ(!lv_obj_is_clickable(obj)) ಹಿಂತಿರುಗಿ false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_ಬಿಂದು_on(&a, ಬಿಂದು, 0);
    ಆದರೆ(res == false) ಹಿಂತಿರುಗಿ false;

    ಆದರೆ(lv_obj_is_adv_hittest(obj)) {
        lv_ದೊರಕಿತು_test_info_t ದೊರಕಿತು_info;
        ದೊರಕಿತು_info.ಬಿಂದು = ಬಿಂದು;
        ದೊರಕಿತು_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_ದೊರಕಿತು_TEST, &ದೊರಕಿತು_info);
        ಹಿಂತಿರುಗಿ ದೊರಕಿತು_info.res;
    }

    ಹಿಂತಿರುಗಿ res;
}

int32_t lv_clamp_ಅಗಲ(int32_t ಅಗಲ, int32_t min_ಅಗಲ, int32_t max_ಅಗಲ, int32_t ref_ಅಗಲ)
{
    ಆದರೆ(LV_COORD_IS_PCT(min_ಅಗಲ)) min_ಅಗಲ = (ref_ಅಗಲ * LV_COORD_GET_PCT(min_ಅಗಲ)) / 100;
    ಆದರೆ(LV_COORD_IS_PCT(max_ಅಗಲ)) max_ಅಗಲ = (ref_ಅಗಲ * LV_COORD_GET_PCT(max_ಅಗಲ)) / 100;
    ಹಿಂತಿರುಗಿ LV_CLAMP(min_ಅಗಲ, ಅಗಲ, max_ಅಗಲ);
}

int32_t lv_clamp_ಎತ್ತರ(int32_t ಎತ್ತರ, int32_t min_ಎತ್ತರ, int32_t max_ಎತ್ತರ, int32_t ref_ಎತ್ತರ)
{
    ಆದರೆ(LV_COORD_IS_PCT(min_ಎತ್ತರ)) min_ಎತ್ತರ = (ref_ಎತ್ತರ * LV_COORD_GET_PCT(min_ಎತ್ತರ)) / 100;
    ಆದರೆ(LV_COORD_IS_PCT(max_ಎತ್ತರ)) max_ಎತ್ತರ = (ref_ಎತ್ತರ * LV_COORD_GET_PCT(max_ಎತ್ತರ)) / 100;
    ಹಿಂತಿರುಗಿ LV_CLAMP(min_ಎತ್ತರ, ಎತ್ತರ, max_ಎತ್ತರ);
}

ಶೂನ್ಯ lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

ಶೂನ್ಯ lv_obj_ಗಣ_transform(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_ಮಾತೃಕೆ_t * ಮಾತೃಕೆ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಆದರೆ(!ಮಾತೃಕೆ) {
        lv_obj_reset_transform(obj);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(!obj->spec_attr->ಮಾತೃಕೆ) {
        obj->spec_attr->ಮಾತೃಕೆ = lv_malloc(ಗಾತ್ರ(lv_ಮಾತೃಕೆ_t));
        LV_ASSERT_MALLOC(obj->spec_attr->ಮಾತೃಕೆ);
        ಆದರೆ(obj->spec_attr->ಮಾತೃಕೆ == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Copy the matrix */
    *obj->spec_attr->ಮಾತೃಕೆ = *ಮಾತೃಕೆ;

    /* Matrix is set. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
    LV_ಬಳಸದ(ಮಾತೃಕೆ);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಆದರೆ(!obj->spec_attr) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(!obj->spec_attr->ಮಾತೃಕೆ) {
        ಹಿಂತಿರುಗಿ;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->ಮಾತೃಕೆ);
    obj->spec_attr->ಮಾತೃಕೆ = ಖಾಲಿ;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರಾಂಕ lv_ಮಾತೃಕೆ_t * lv_obj_get_transform(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಆದರೆ(obj->spec_attr) {
        ಹಿಂತಿರುಗಿ obj->spec_attr->ಮಾತೃಕೆ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    LV_ಬಳಸದ(obj);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t obj_invalidate_area_ಆಂತರಿಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    ಹಿಂತಿರುಗಿ invalidate_area_core(obj, &area_tmp);
}

ಸ್ಥಿರ bool is_transformed(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    ಆಗ(obj) {
        ಆದರೆ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) ಹಿಂತಿರುಗಿ true;
        obj = obj->ಪೋಷಕ;
    }
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ int32_t calc_content_ಅಗಲ(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    ಆದರೆ(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_ಬಲ = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t space_ಎಡ = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_ಅಗಲ(obj) + space_ಎಡ + space_ಬಲ;

    int32_t ಮಗು_res = LV_COORD_MIN;

    ಆದರೆ(!lv_layout_get_min_ಗಾತ್ರ(obj, &ಮಗು_res, true)) {
        uint32_t i;
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
        /*With RTL find the left most coordinate*/
        ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) == LV_ತಳ_DIR_RTL) {
            ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                int32_t ಮಗು_res_tmp = LV_COORD_MIN;
                lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                ಆದರೆ(ಮಗು->hidden || ಮಗು->floating)
                    ಮುಂದುವರಿ;

                ಆದರೆ(ಮಗು->w_ignore_ಗಾತ್ರ)
                    ಮುಂದುವರಿ;

                ಆದರೆ(!lv_obj_is_layout_positioned(ಮಗು)) {
                    lv_align_t align = lv_obj_get_style_align_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                    ಆಯ್ಕೆ(align) {
                        ಸ್ಥಿತಿ LV_ALIGN_DEFAULT:
                        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಬಲ:
                        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಬಲ:
                        ಸ್ಥಿತಿ LV_ALIGN_ಬಲ_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            ಮಗು_res_tmp = obj->coords.x2 - ಮಗು->coords.x1 + 1;
                            ನಿಲ್ಲಿಸು;
                        ಪೂರ್ವನಿಯೋಜಿತ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ಆದರೆ(lv_obj_get_style_x_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN) == 0) {
                                ಮಗು_res_tmp = lv_area_get_ಅಗಲ(&ಮಗು->coords) + space_ಬಲ;
                                ಮಗು_res_tmp += lv_obj_get_style_margin_ಎಡ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                            }
                            ನಿಲ್ಲಿಸು;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    ಮಗು_res_tmp = obj->coords.x2 - ಮಗು->coords.x1 + 1;
                }
                ಮಗು_res = LV_MAX(ಮಗು_res, ಮಗು_res_tmp + lv_obj_get_style_margin_ಎಡ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN));
            }
            ಆದರೆ(ಮಗು_res != LV_COORD_MIN) {
                ಮಗು_res += space_ಎಡ;
            }
        }
        /*Else find the right most coordinate*/
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                int32_t ಮಗು_res_tmp = LV_COORD_MIN;
                lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                ಆದರೆ(ಮಗು->hidden || ಮಗು->floating) ಮುಂದುವರಿ;

                ಆದರೆ(ಮಗು->w_ignore_ಗಾತ್ರ)
                    ಮುಂದುವರಿ;

                ಆದರೆ(!lv_obj_is_layout_positioned(ಮಗು)) {
                    lv_align_t align = lv_obj_get_style_align_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                    ಆಯ್ಕೆ(align) {
                        ಸ್ಥಿತಿ LV_ALIGN_DEFAULT:
                        ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಎಡ:
                        ಸ್ಥಿತಿ LV_ALIGN_BOTTOM_ಎಡ:
                        ಸ್ಥಿತಿ LV_ALIGN_ಎಡ_MID:
                            /*Normal left aligns.*/
                            ಮಗು_res_tmp = ಮಗು->coords.x2 - obj->coords.x1 + 1;
                            ನಿಲ್ಲಿಸು;
                        ಪೂರ್ವನಿಯೋಜಿತ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ಆದರೆ(lv_obj_get_style_x_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN) == 0) {
                                ಮಗು_res_tmp = lv_area_get_ಅಗಲ(&ಮಗು->coords) + space_ಎಡ;
                                ಮಗು_res_tmp += lv_obj_get_style_margin_ಬಲ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                            }
                            ನಿಲ್ಲಿಸು;
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    ಮಗು_res_tmp = ಮಗು->coords.x2 - obj->coords.x1 + 1;
                }

                ಮಗು_res = LV_MAX(ಮಗು_res, ಮಗು_res_tmp + lv_obj_get_style_margin_ಬಲ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN));
            }

            ಆದರೆ(ಮಗು_res != LV_COORD_MIN) {
                ಮಗು_res += space_ಬಲ;
            }
        }
    }

    ಆದರೆ(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    ಆದರೆ(ಮಗು_res == LV_COORD_MIN) {
        ಹಿಂತಿರುಗಿ self_w;
    }

    ಹಿಂತಿರುಗಿ LV_MAX(ಮಗು_res, self_w);
}

ಸ್ಥಿರ int32_t calc_content_ಎತ್ತರ(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    ಆದರೆ(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_ತುದಿ = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_ಎತ್ತರ(obj) + space_ತುದಿ + space_bottom;

    int32_t ಮಗು_res = LV_COORD_MIN;

    ಆದರೆ(!lv_layout_get_min_ಗಾತ್ರ(obj, &ಮಗು_res, false)) {
        uint32_t i;
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
        ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
            int32_t ಮಗು_res_tmp = LV_COORD_MIN;
            lv_obj_t * ಮಗು = obj->spec_attr->children[i];
            ಆದರೆ(ಮಗು->hidden || ಮಗು->floating) ಮುಂದುವರಿ;

            ಆದರೆ(ಮಗು->h_ignore_ಗಾತ್ರ)
                ಮುಂದುವರಿ;

            ಆದರೆ(!lv_obj_is_layout_positioned(ಮಗು)) {
                lv_align_t align = lv_obj_get_style_align_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                ಆಯ್ಕೆ(align) {
                    ಸ್ಥಿತಿ LV_ALIGN_DEFAULT:
                    ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಬಲ:
                    ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_MID:
                    ಸ್ಥಿತಿ LV_ALIGN_ತುದಿ_ಎಡ:
                        /*Normal top aligns. */
                        ಮಗು_res_tmp = ಮಗು->coords.y2 - obj->coords.y1 + 1;
                        ನಿಲ್ಲಿಸು;
                    ಪೂರ್ವನಿಯೋಜಿತ:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        ಆದರೆ(lv_obj_get_style_y_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN) == 0) {
                            ಮಗು_res_tmp = lv_area_get_ಎತ್ತರ(&ಮಗು->coords) + space_ತುದಿ;
                            ಮಗು_res_tmp += lv_obj_get_style_margin_ತುದಿ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
                        }
                        ನಿಲ್ಲಿಸು;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                ಮಗು_res_tmp = ಮಗು->coords.y2 - obj->coords.y1 + 1;
            }

            ಮಗು_res = LV_MAX(ಮಗು_res, ಮಗು_res_tmp + lv_obj_get_style_margin_bottom_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN));
        }

        ಆದರೆ(ಮಗು_res != LV_COORD_MIN) {
            ಮಗು_res += space_bottom;
        }
    }

    ಆದರೆ(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    ಆದರೆ(ಮಗು_res == LV_COORD_MIN) {
        ಹಿಂತಿರುಗಿ self_h;
    }

    ಹಿಂತಿರುಗಿ LV_MAX(self_h, ಮಗು_res);
}

ಸ್ಥಿರ ಶೂನ್ಯ layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        layout_update_core(ಮಗು);
    }

    ಆದರೆ(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_ಗಾತ್ರ(obj);
        lv_obj_refr_pos(obj);

        ಆದರೆ(ಮಗು_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    ಆದರೆ(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ transform_ಬಿಂದು_array(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_ಬಿಂದು_t * p, ಗಾತ್ರ_t p_ಎಣಿಕೆ, bool inv)
{
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಸ್ಥಿರಾಂಕ lv_ಮಾತೃಕೆ_t * obj_ಮಾತೃಕೆ = lv_obj_get_transform(obj);
    ಆದರೆ(obj_ಮಾತೃಕೆ) {
        lv_ಮಾತೃಕೆ_t m;
        lv_ಮಾತೃಕೆ_ತಟಸ್ಥ(&m);
        lv_ಮಾತೃಕೆ_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_ಮಾತೃಕೆ_multiply(&m, obj_ಮಾತೃಕೆ);
        lv_ಮಾತೃಕೆ_translate(&m, -obj->coords.x1, -obj->coords.y1);

        ಆದರೆ(inv) {
            lv_ಮಾತೃಕೆ_t inv_m;
            lv_ಮಾತೃಕೆ_inverse(&inv_m, &m);
            m = inv_m;
        }

        ಗಾಗಿ(ಗಾತ್ರ_t i = 0; i < p_ಎಣಿಕೆ; i++) {
            lv_ಬಿಂದು_precise_t p_precise = lv_ಬಿಂದು_ಗಮ್ಯ_precise(&p[i]);
            lv_ಬಿಂದು_precise_t res = lv_ಮಾತೃಕೆ_transform_precise_ಬಿಂದು(&m, &p_precise);
            p[i] = lv_ಬಿಂದು_from_precise(&res);
        }

        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_ಸುರಕ್ಷಿತ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    ಆದರೆ(scale_x == 0) scale_x = 1;
    ಆದರೆ(scale_y == 0) scale_y = 1;

    ಆದರೆ(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) ಹಿಂತಿರುಗಿ;

    lv_ಬಿಂದು_t ಅಚ್ಚು = {
        .x = lv_obj_get_style_transform_ಅಚ್ಚು_x_ಆಂತರಿಕ(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ಅಚ್ಚು_y_ಆಂತರಿಕ(obj, LV_PART_MAIN)
    };

    ಆದರೆ(LV_COORD_IS_PCT(ಅಚ್ಚು.x)) {
        ಅಚ್ಚು.x = (LV_COORD_GET_PCT(ಅಚ್ಚು.x) * lv_area_get_ಅಗಲ(&obj->coords)) / 100;
    }
    ಆದರೆ(LV_COORD_IS_PCT(ಅಚ್ಚು.y)) {
        ಅಚ್ಚು.y = (LV_COORD_GET_PCT(ಅಚ್ಚು.y) * lv_area_get_ಎತ್ತರ(&obj->coords)) / 100;
    }

    ಅಚ್ಚು.x = obj->coords.x1 + ಅಚ್ಚು.x;
    ಅಚ್ಚು.y = obj->coords.y1 + ಅಚ್ಚು.y;

    ಆದರೆ(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_ಬಿಂದು_array_transform(p, p_ಎಣಿಕೆ, angle, scale_x, scale_y, &ಅಚ್ಚು, !inv);
}

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t invalidate_area_core(ಸ್ಥಿರಾಂಕ lv_obj_t * obj, lv_area_t * area_tmp)
{
    ಆದರೆ(!lv_obj_area_is_visible(obj, area_tmp)) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#ಆದರೆ_ಅಂತ್ಯ

    lv_ಫಲಿತಾಂಶ_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    ಹಿಂತಿರುಗಿ res;
}

/**
 * @brief Calculates the width in pixels of an LVGL object based on its style and parent for a given width `prop`.
 * @param obj Pointer to the LVGL object whose width is being calculated.
 * @param prop Which style width to calculate for. Valid values are: LV_STYLE_WIDTH, LV_STYLE_MIN_WIDTH, or
 * LV_STYLE_MAX_WIDTH.
 * @param content_width Pointer to an integer storing the object's content width to prevent unnecessary recalculation.
 * If negative or NULL and width is `LV_SIZE_CONTENT`, it will be calculated.
 * @return The computed width for the object:
 * @note If the style width is a fixed value, that value is returned.
 * @note If the style width is `LV_SIZE_CONTENT`, the content width is calculated and returned.
 * @note If the style width is a `LV_PCT()`, the percentage is applied to the parent's width.
 */
ಸ್ಥಿರ int32_t calc_dynamic_ಅಗಲ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ಸ್ಥಿರಾಂಕ content_ಅಗಲ)
{
    LV_ASSERT(prop == LV_STYLE_ಅಗಲ || prop == LV_STYLE_MIN_ಅಗಲ || prop == LV_STYLE_MAX_ಅಗಲ);

    int32_t ಅಗಲ = lv_obj_get_style_prop(obj, 0, prop).ಅಂಕೆ;

    ಆದರೆ(ಅಗಲ == LV_ಗಾತ್ರ_CONTENT) {
        ಆದರೆ(content_ಅಗಲ == ಖಾಲಿ) {
            ಅಗಲ = calc_content_ಅಗಲ(obj);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ(*content_ಅಗಲ < 0) {
                *content_ಅಗಲ = calc_content_ಅಗಲ(obj);
            }
            ಅಗಲ = *content_ಅಗಲ;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(LV_COORD_IS_PCT(ಅಗಲ)) {
        lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
        int32_t ಪೋಷಕ_w = lv_obj_get_content_ಅಗಲ(ಪೋಷಕ);
        ಅಗಲ = (LV_COORD_GET_PCT(ಅಗಲ) * ಪೋಷಕ_w) / 100;
        ಅಗಲ -= lv_obj_get_style_margin_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN) + lv_obj_get_style_margin_ಬಲ_ಆಂತರಿಕ(obj,
                                                                                                                   LV_PART_MAIN);
    }
    ಹಿಂತಿರುಗಿ ಅಗಲ;
}

/**
 * @brief Calculates the height in pixels of an LVGL object based on its style and parent for a given height `prop`.
 * @param obj Pointer to the LVGL object whose height is being calculated.
 * @param prop Which style height to calculate for. Valid values are: LV_STYLE_HEIGHT, LV_STYLE_MIN_HEIGHT, or
 * LV_STYLE_MAX_HEIGHT.
 * @param content_height Pointer to an integer storing the object's content height to prevent unnecessary recalculation.
 * If negative or NULL and height is `LV_SIZE_CONTENT`, it will be calculated.
 * @return The computed height for the object:
 * @note If the style height is a fixed value, that value is returned.
 * @note If the style height is `LV_SIZE_CONTENT`, the content height is calculated and returned.
 * @note If the style height is a `LV_PCT()`, the percentage is applied to the parent's height.
 */
ಸ್ಥಿರ int32_t calc_dynamic_ಎತ್ತರ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ಸ್ಥಿರಾಂಕ content_ಎತ್ತರ)
{
    LV_ASSERT(prop == LV_STYLE_ಎತ್ತರ || prop == LV_STYLE_MIN_ಎತ್ತರ || prop == LV_STYLE_MAX_ಎತ್ತರ);

    int32_t ಎತ್ತರ = lv_obj_get_style_prop(obj, 0, prop).ಅಂಕೆ;

    ಆದರೆ(ಎತ್ತರ == LV_ಗಾತ್ರ_CONTENT) {
        ಆದರೆ(content_ಎತ್ತರ == ಖಾಲಿ) {
            ಎತ್ತರ = calc_content_ಎತ್ತರ(obj);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ(*content_ಎತ್ತರ < 0) {
                *content_ಎತ್ತರ = calc_content_ಎತ್ತರ(obj);
            }
            ಎತ್ತರ = *content_ಎತ್ತರ;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(LV_COORD_IS_PCT(ಎತ್ತರ)) {
        lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(obj);
        int32_t ಪೋಷಕ_h = lv_obj_get_content_ಎತ್ತರ(ಪೋಷಕ);
        ಎತ್ತರ = (LV_COORD_GET_PCT(ಎತ್ತರ) * ಪೋಷಕ_h) / 100;
        ಎತ್ತರ -= lv_obj_get_style_margin_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_ಆಂತರಿಕ(obj,
                                                                                                                    LV_PART_MAIN);
    }
    ಹಿಂತಿರುಗಿ ಎತ್ತರ;
}

/**
 * Tell whether the style which sets the final size is a percentage.
 * @param unclamped     the size from the size style, before clamping
 * @param min           the min size after resolving its style
 * @param max           the max size after resolving its style
 * @param size_style    the size style value, e.g. LV_PCT(100) or LV_SIZE_CONTENT
 * @param min_style     the min size style value
 * @param max_style     the max size style value
 * @return              true: a percentage style sets the size
 */
ಸ್ಥಿರ bool ಗಾತ್ರ_ಒಳ_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t ಗಾತ್ರ_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    ಆದರೆ(min > max) ಹಿಂತಿರುಗಿ LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    ಆದರೆ(unclamped < min) ಹಿಂತಿರುಗಿ LV_COORD_IS_PCT(min_style);
    ಆದರೆ(unclamped > max) ಹಿಂತಿರುಗಿ LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(ಗಾತ್ರ_style);
    ಆದರೆ(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    ಆದರೆ(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    ಹಿಂತಿರುಗಿ is_pct;
}
