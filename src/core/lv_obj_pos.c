/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_area_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../layouts/lv_layout_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_refr_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#ਜੇ_ਅੰਤ

#ਜੇ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #ਗਲਤੀ "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#ਜੇ_ਅੰਤ

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ int32_t calc_content_ਚੌੜਾਈ(lv_obj_t * obj);
ਸਥਿਰ int32_t calc_content_ਉਚਾਈ(lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ layout_update_core(lv_obj_t * obj);
ਸਥਿਰ ਖਾਲੀ transform_ਬਿੰਦੂ_array(ਅਚਲ lv_obj_t * obj, lv_ਬਿੰਦੂ_t * p, ਮਾਪ_t p_ਗਿਣਤੀ, bool inv);
ਸਥਿਰ bool is_transformed(ਅਚਲ lv_obj_t * obj);
ਸਥਿਰ lv_ਨਤੀਜਾ_t invalidate_area_core(ਅਚਲ lv_obj_t * obj, lv_area_t * area_tmp);
ਸਥਿਰ lv_ਨਤੀਜਾ_t obj_invalidate_area_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_area_t * area);
ਸਥਿਰ int32_t calc_dynamic_ਚੌੜਾਈ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_ਚੌੜਾਈ);
ਸਥਿਰ int32_t calc_dynamic_ਉਚਾਈ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_ਉਚਾਈ);
ਸਥਿਰ bool ਮਾਪ_ਵਿੱਚ_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t ਮਾਪ_style,
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

ਖਾਲੀ lv_obj_ਸੈੱਟ_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_ਸੈੱਟ_x(obj, x);
    lv_obj_ਸੈੱਟ_y(obj, y);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_style_res_t res_x;
    lv_style_ਮੁੱਲ_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    ਜੇ((res_x == LV_STYLE_RES_ਮਿਲਿਆ && v_x.ਅੰਕ != x) || res_x == LV_STYLE_RES_NOT_ਮਿਲਿਆ) {
        lv_obj_ਸੈੱਟ_style_x(obj, x, 0);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_style_res_t res_y;
    lv_style_ਮੁੱਲ_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    ਜੇ((res_y == LV_STYLE_RES_ਮਿਲਿਆ && v_y.ਅੰਕ != y) || res_y == LV_STYLE_RES_NOT_ਮਿਲਿਆ) {
        lv_obj_ਸੈੱਟ_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_ਚੌੜਾਈ(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);
    LV_CHECK_ARG(prop == LV_STYLE_ਚੌੜਾਈ || prop == LV_STYLE_MIN_ਚੌੜਾਈ || prop == LV_STYLE_MAX_ਚੌੜਾਈ, ਵਾਪਸ 0);

    ਵਾਪਸ calc_dynamic_ਚੌੜਾਈ(obj, prop, ਨਿਰਾ);
}

int32_t lv_obj_calc_dynamic_ਉਚਾਈ(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);
    LV_CHECK_ARG(prop == LV_STYLE_ਉਚਾਈ || prop == LV_STYLE_MIN_ਉਚਾਈ || prop == LV_STYLE_MAX_ਉਚਾਈ, ਵਾਪਸ 0);

    ਵਾਪਸ calc_dynamic_ਉਚਾਈ(obj, prop, ਨਿਰਾ);
}

bool lv_obj_refr_ਮਾਪ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    /*If the width or height is set by a layout do not modify them*/
    ਜੇ(obj->w_layout && obj->h_layout) ਵਾਪਸ false;

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਵਾਪਸ false;

    int32_t w;
    ਜੇ(obj->w_layout) {
        w = lv_obj_get_ਚੌੜਾਈ(obj);
    }
    ਨਹੀਂਤਾਂ {
        int32_t content_ਚੌੜਾਈ = -1;
        int32_t unclamped_w = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
        int32_t minw = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MIN_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
        int32_t maxw = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MAX_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = ਮਾਪ_ਵਿੱਚ_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN));
        obj->w_ignore_ਮਾਪ =
            (w_pct && ਮਾਪਾ->w_layout == 0 && lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, 0) == LV_ਮਾਪ_CONTENT);
    }

    int32_t h;
    ਜੇ(obj->h_layout) {
        h = lv_obj_get_ਉਚਾਈ(obj);
    }
    ਨਹੀਂਤਾਂ {
        int32_t content_ਉਚਾਈ = -1;
        int32_t unclamped_h = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_ਉਚਾਈ, &content_ਉਚਾਈ);
        int32_t minh = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MIN_ਉਚਾਈ, &content_ਉਚਾਈ);
        int32_t maxh = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MAX_ਉਚਾਈ, &content_ਉਚਾਈ);
        h = LV_CLAMP(minh, unclamped_h, maxh);

        /**
         * If the height in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `h_ignore_size` excludes this object from the
         * parent's content height calculation. See the width branch above.
         */
        bool h_pct = ਮਾਪ_ਵਿੱਚ_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN));

        obj->h_ignore_ਮਾਪ = (h_pct && ਮਾਪਾ->h_layout == 0 &&
                              lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, 0) == LV_ਮਾਪ_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    ਜੇ(lv_obj_get_ਚੌੜਾਈ(obj) == w && lv_obj_get_ਉਚਾਈ(obj) == h)
        ਵਾਪਸ false;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t ਮਾਪਾ_fit_area;
    lv_obj_get_content_coords(ਮਾਪਾ, &ਮਾਪਾ_fit_area);

    /*If the object is already out of the parent and its position is changes
     *surely the scrollbars also changes so invalidate them*/
    bool on1 = lv_area_is_ਵਿੱਚ(&ori, &ਮਾਪਾ_fit_area, 0);
    ਜੇ(!on1)
        lv_obj_scrollbar_invalidate(ਮਾਪਾ);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    ਨਹੀਂਤਾਂ {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_ਮਾਪ_ਬਦਲਿਆ, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_ਵਿੱਚ(&obj->coords, &ਮਾਪਾ_fit_area, 0);
    ਜੇ(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(ਮਾਪਾ);

    lv_obj_refresh_ext_draw_ਮਾਪ(obj);

    ਵਾਪਸ true;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ਮਾਪ(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_ਸੈੱਟ_ਚੌੜਾਈ(obj, w);
    lv_obj_ਸੈੱਟ_ਉਚਾਈ(obj, h);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ਚੌੜਾਈ(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    lv_style_res_t res_w;
    lv_style_ਮੁੱਲ_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_ਚੌੜਾਈ, &v_w, 0);

    ਜੇ((res_w == LV_STYLE_RES_ਮਿਲਿਆ && v_w.ਅੰਕ != w) || res_w == LV_STYLE_RES_NOT_ਮਿਲਿਆ) {
        lv_obj_ਸੈੱਟ_style_ਚੌੜਾਈ(obj, w, 0);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ਉਚਾਈ(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    lv_style_res_t res_h;
    lv_style_ਮੁੱਲ_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_ਉਚਾਈ, &v_h, 0);

    ਜੇ((res_h == LV_STYLE_RES_ਮਿਲਿਆ && v_h.ਅੰਕ != h) || res_h == LV_STYLE_RES_NOT_ਮਿਲਿਆ) {
        lv_obj_ਸੈੱਟ_style_ਉਚਾਈ(obj, h, 0);
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_content_ਚੌੜਾਈ(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    int32_t ਖੱਬਾ = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t ਸੱਜਾ = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    lv_obj_ਸੈੱਟ_ਚੌੜਾਈ(obj, w + ਖੱਬਾ + ਸੱਜਾ);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_content_ਉਚਾਈ(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    int32_t ਸਿਖਰ = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    lv_obj_ਸੈੱਟ_ਉਚਾਈ(obj, h + ਸਿਖਰ + bottom);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_ਸੈੱਟ_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    ਜੇ(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) ਵਾਪਸ false;

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਵਾਪਸ false;

    uint32_t layout = lv_obj_get_style_layout_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    ਜੇ(layout) ਵਾਪਸ true;
    ਨਹੀਂਤਾਂ ਵਾਪਸ false;
}

ਖਾਲੀ lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ਨਿਰਾ);
}

ਖਾਲੀ lv_obj_update_layout(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        ਵਾਪਸ;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    ਜਦੋਂ(scr->scr_layout_inv) {
        ਜੇ(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_ਸੁਨੇਹਾ(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (ਖਾਲੀ *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            ਰੋਕੋ;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, ਨਿਰਾ);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_ਸਿਰਾ;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_ਸੈੱਟ_style_align(obj, align, 0);
}

ਖਾਲੀ lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    lv_obj_ਸੈੱਟ_style_align(obj, align, 0);
    lv_obj_ਸੈੱਟ_pos(obj, x_ofs, y_ofs);
}

ਖਾਲੀ lv_obj_align_ਮੰਜ਼ਿਲ(lv_obj_t * obj, ਅਚਲ lv_obj_t * ਅਧਾਰ, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_update_layout(obj);
    ਜੇ(ਅਧਾਰ == ਨਿਰਾ) ਅਧਾਰ = lv_obj_get_ਮਾਪਾ(obj);

    ਜੇ(ਅਧਾਰ == ਨਿਰਾ) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        ਵਾਪਸ;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        ਵਾਪਸ;
    }

    int32_t pleft = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਅਧਾਰ, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਅਧਾਰ, LV_PART_MAIN);

    ਜੇ(align == LV_ALIGN_DEFAULT) {
        ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(ਅਧਾਰ, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL) align = LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ;
        ਨਹੀਂਤਾਂ align = LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ;
    }

    ਚੋਣ(align) {
        ਹਾਲਤ LV_ALIGN_CENTER:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਚੌੜਾਈ(obj) / 2 + bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਉਚਾਈ(obj) / 2 + btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ:
            x = bleft;
            y = btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_MID:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਚੌੜਾਈ(obj) / 2 + bleft;
            y = btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) - lv_obj_get_ਚੌੜਾਈ(obj) + bleft;
            y = btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_BOTTOM_ਖੱਬਾ:
            x = bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) - lv_obj_get_ਉਚਾਈ(obj) + btop;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਚੌੜਾਈ(obj) / 2 + bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) - lv_obj_get_ਉਚਾਈ(obj) + btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_BOTTOM_ਸੱਜਾ:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) - lv_obj_get_ਚੌੜਾਈ(obj) + bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) - lv_obj_get_ਉਚਾਈ(obj) + btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਖੱਬਾ_MID:
            x = bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਉਚਾਈ(obj) / 2 + btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਸੱਜਾ_MID:
            x = lv_obj_get_content_ਚੌੜਾਈ(ਅਧਾਰ) - lv_obj_get_ਚੌੜਾਈ(obj) + bleft;
            y = lv_obj_get_content_ਉਚਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਉਚਾਈ(obj) / 2 + btop;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸਿਖਰ_ਖੱਬਾ:
            x = 0;
            y = -lv_obj_get_ਉਚਾਈ(obj);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸਿਖਰ_MID:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਚੌੜਾਈ(obj) / 2;
            y = -lv_obj_get_ਉਚਾਈ(obj);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸਿਖਰ_ਸੱਜਾ:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ) - lv_obj_get_ਚੌੜਾਈ(obj);
            y = -lv_obj_get_ਉਚਾਈ(obj);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_BOTTOM_ਖੱਬਾ:
            x = 0;
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_BOTTOM_MID:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਚੌੜਾਈ(obj) / 2;
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_BOTTOM_ਸੱਜਾ:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ) - lv_obj_get_ਚੌੜਾਈ(obj);
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਖੱਬਾ_ਸਿਖਰ:
            x = -lv_obj_get_ਚੌੜਾਈ(obj);
            y = 0;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਖੱਬਾ_MID:
            x = -lv_obj_get_ਚੌੜਾਈ(obj);
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਉਚਾਈ(obj) / 2;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਖੱਬਾ_BOTTOM:
            x = -lv_obj_get_ਚੌੜਾਈ(obj);
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ) - lv_obj_get_ਉਚਾਈ(obj);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸੱਜਾ_ਸਿਖਰ:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ);
            y = 0;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸੱਜਾ_MID:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ);
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ) / 2 - lv_obj_get_ਉਚਾਈ(obj) / 2;
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_ਬਾਹਰਲਾ_ਸੱਜਾ_BOTTOM:
            x = lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ);
            y = lv_obj_get_ਉਚਾਈ(ਅਧਾਰ) - lv_obj_get_ਉਚਾਈ(obj);
            ਰੋਕੋ;

        ਹਾਲਤ LV_ALIGN_DEFAULT:
            ਰੋਕੋ;
    }

    ਜੇ(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_ਚੌੜਾਈ(ਅਧਾਰ) * LV_COORD_GET_PCT(x_ofs)) / 100;
    ਜੇ(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_ਉਚਾਈ(ਅਧਾਰ) * LV_COORD_GET_PCT(y_ofs)) / 100;
    ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL) {
        x += x_ofs + ਅਧਾਰ->coords.x1 - ਮਾਪਾ->coords.x1 + lv_obj_get_scroll_ਸੱਜਾ(ਮਾਪਾ) - pleft;
    }
    ਨਹੀਂਤਾਂ {
        x += x_ofs + ਅਧਾਰ->coords.x1 - ਮਾਪਾ->coords.x1 + lv_obj_get_scroll_ਖੱਬਾ(ਮਾਪਾ) - pleft;
    }
    y += y_ofs + ਅਧਾਰ->coords.y1 - ਮਾਪਾ->coords.y1 + lv_obj_get_scroll_ਸਿਖਰ(ਮਾਪਾ) - ptop;
    lv_obj_ਸੈੱਟ_style_align(obj, LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ, 0);
    lv_obj_ਸੈੱਟ_pos(obj, x, y);

}

ਖਾਲੀ lv_obj_get_coords(ਅਚਲ lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t rel_x;
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ) {
        rel_x  = obj->coords.x1 - ਮਾਪਾ->coords.x1;
        rel_x += lv_obj_get_scroll_x(ਮਾਪਾ);
        rel_x -= lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ {
        rel_x = obj->coords.x1;
    }
    ਵਾਪਸ rel_x;
}

int32_t lv_obj_get_x2(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_obj_get_x(obj) + lv_obj_get_ਚੌੜਾਈ(obj);
}

int32_t lv_obj_get_y(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t rel_y;
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ) {
        rel_y = obj->coords.y1 - ਮਾਪਾ->coords.y1;
        rel_y += lv_obj_get_scroll_y(ਮਾਪਾ);
        rel_y -= lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ {
        rel_y = obj->coords.y1;
    }
    ਵਾਪਸ rel_y;
}

int32_t lv_obj_get_y2(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_obj_get_y(obj) + lv_obj_get_ਉਚਾਈ(obj);
}

int32_t lv_obj_get_x_aligned(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_obj_get_style_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_obj_get_style_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_ਚੌੜਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_area_get_ਚੌੜਾਈ(&obj->coords);
}

int32_t lv_obj_get_ਉਚਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਵਾਪਸ lv_area_get_ਉਚਾਈ(&obj->coords);
}

int32_t lv_obj_get_content_ਚੌੜਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t ਖੱਬਾ = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t ਸੱਜਾ = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਵਾਪਸ lv_obj_get_ਚੌੜਾਈ(obj) - ਖੱਬਾ - ਸੱਜਾ;
}

int32_t lv_obj_get_content_ਉਚਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t ਸਿਖਰ = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਵਾਪਸ lv_obj_get_ਉਚਾਈ(obj) - ਸਿਖਰ - bottom;
}

ਖਾਲੀ lv_obj_get_content_coords(ਅਚਲ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_ਚੌੜਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    lv_ਬਿੰਦੂ_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_ਮਾਪ, &p);
    ਵਾਪਸ p.x;
}

int32_t lv_obj_get_self_ਉਚਾਈ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    lv_ਬਿੰਦੂ_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_ਮਾਪ, &p);
    ਵਾਪਸ p.y;
}

int32_t lv_obj_get_style_clamped_ਚੌੜਾਈ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t content_ਚੌੜਾਈ = -1;
    int32_t w = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
    int32_t minw = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MIN_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
    int32_t maxw = calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MAX_ਚੌੜਾਈ, &content_ਚੌੜਾਈ);
    ਜੇ(w <= minw) {
        w = lv_obj_get_style_min_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ ਜੇ(w >= maxw) {
        w = lv_obj_get_style_max_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ {
        w = lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਵਾਪਸ w;
}

int32_t lv_obj_get_style_clamped_ਉਚਾਈ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t content_ਉਚਾਈ = -1;
    int32_t h = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_ਉਚਾਈ, &content_ਉਚਾਈ);
    int32_t minh = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MIN_ਉਚਾਈ, &content_ਉਚਾਈ);
    int32_t maxh = calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MAX_ਉਚਾਈ, &content_ਉਚਾਈ);
    ਜੇ(h <= minh) {
        h = lv_obj_get_style_min_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ ਜੇ(h >= maxh) {
        h = lv_obj_get_style_max_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਨਹੀਂਤਾਂ {
        h = lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    }
    ਵਾਪਸ h;
}

bool lv_obj_is_style_any_ਚੌੜਾਈ_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t w = lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    ਵਾਪਸ (w == LV_ਮਾਪ_CONTENT || minw == LV_ਮਾਪ_CONTENT || maxw == LV_ਮਾਪ_CONTENT);
}

bool lv_obj_is_style_any_ਉਚਾਈ_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t h = lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_ਉਚਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    ਵਾਪਸ (h == LV_ਮਾਪ_CONTENT || minh == LV_ਮਾਪ_CONTENT || maxh == LV_ਮਾਪ_CONTENT);
}

bool lv_obj_is_ਚੌੜਾਈ_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t minw = lv_obj_calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MIN_ਚੌੜਾਈ);
    int32_t w = lv_obj_get_ਚੌੜਾਈ(obj);
    ਵਾਪਸ w == minw;
}

bool lv_obj_is_ਉਚਾਈ_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t minh = lv_obj_calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MIN_ਉਚਾਈ);
    int32_t h = lv_obj_get_ਉਚਾਈ(obj);
    ਵਾਪਸ h == minh;
}

bool lv_obj_is_ਚੌੜਾਈ_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t maxw = lv_obj_calc_dynamic_ਚੌੜਾਈ(obj, LV_STYLE_MAX_ਚੌੜਾਈ);
    int32_t w = lv_obj_get_ਚੌੜਾਈ(obj);
    ਵਾਪਸ w == maxw;
}

bool lv_obj_is_ਉਚਾਈ_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    int32_t maxh = lv_obj_calc_dynamic_ਉਚਾਈ(obj, LV_STYLE_MAX_ਉਚਾਈ);
    int32_t h = lv_obj_get_ਉਚਾਈ(obj);
    ਵਾਪਸ h == maxh;
}

bool lv_obj_refresh_self_ਮਾਪ(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    ਜੇ(!lv_obj_is_style_any_ਚੌੜਾਈ_content(obj) && !lv_obj_is_style_any_ਉਚਾਈ_content(obj))
        ਵਾਪਸ false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ != ਨਿਰਾ) {
        ਮਾਪਾ->w_layout = 0;
        ਮਾਪਾ->h_layout = 0;
        lv_obj_mark_layout_as_dirty(ਮਾਪਾ);
    }
    lv_obj_mark_layout_as_dirty(obj);
    ਵਾਪਸ true;
}

ਖਾਲੀ lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(lv_obj_is_layout_positioned(obj)) ਵਾਪਸ;

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    int32_t x = lv_obj_get_style_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) {
        lv_obj_move_ਮੰਜ਼ਿਲ(obj, x, y);
        ਵਾਪਸ;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_ਚੌੜਾਈ(ਮਾਪਾ);
    int32_t ph = lv_obj_get_content_ਉਚਾਈ(ਮਾਪਾ);
    ਜੇ(LV_COORD_IS_PCT(x)) {
        ਜੇ(lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) == LV_ਮਾਪ_CONTENT) x = 0; /*Avoid circular dependency*/
        ਨਹੀਂਤਾਂ x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    ਜੇ(LV_COORD_IS_PCT(y)) {
        ਜੇ(lv_obj_get_style_ਉਚਾਈ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) == LV_ਮਾਪ_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_ਚੌੜਾਈ(obj);
    int32_t h = lv_obj_get_ਉਚਾਈ(obj);
    ਜੇ(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    ਜੇ(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਜੇ(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ;
    }

    bool rtl = lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL;

    ਜੇ(rtl) {
        ਚੋਣ(align) {
            ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ:
                align = LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ;
                ਰੋਕੋ;
            ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ:
                align = LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ;
                ਰੋਕੋ;
            ਹਾਲਤ LV_ALIGN_ਖੱਬਾ_MID:
                align = LV_ALIGN_ਸੱਜਾ_MID;
                ਰੋਕੋ;
            ਹਾਲਤ LV_ALIGN_ਸੱਜਾ_MID:
                align = LV_ALIGN_ਖੱਬਾ_MID;
                ਰੋਕੋ;
            ਹਾਲਤ LV_ALIGN_BOTTOM_ਖੱਬਾ:
                align = LV_ALIGN_BOTTOM_ਸੱਜਾ;
                ਰੋਕੋ;
            ਹਾਲਤ LV_ALIGN_BOTTOM_ਸੱਜਾ:
                align = LV_ALIGN_BOTTOM_ਖੱਬਾ;
                ਰੋਕੋ;
            ਮੂਲ:
                ਰੋਕੋ;
        }
    }

    ਚੋਣ(align) {
        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ:
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ:
            x = rtl ? pw - w - x : pw - w + x;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_ਖੱਬਾ_MID:
            y += ph / 2 - h / 2;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_BOTTOM_ਖੱਬਾ:
            y += ph - h;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_BOTTOM_ਸੱਜਾ:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_ਸੱਜਾ_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            ਰੋਕੋ;
        ਹਾਲਤ LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            ਰੋਕੋ;
        ਮੂਲ:
            ਰੋਕੋ;
    }

    lv_obj_move_ਮੰਜ਼ਿਲ(obj, x, y);
}

ਖਾਲੀ lv_obj_move_ਮੰਜ਼ਿਲ(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * ਮਾਪਾ = obj->ਮਾਪਾ;

    ਜੇ(ਮਾਪਾ) {
        ਜੇ(lv_obj_is_floating(obj)) {
            x += ਮਾਪਾ->coords.x1;
            y += ਮਾਪਾ->coords.y1;
        }
        ਨਹੀਂਤਾਂ {
            x += ਮਾਪਾ->coords.x1 - lv_obj_get_scroll_x(ਮਾਪਾ);
            y += ਮਾਪਾ->coords.y1 - lv_obj_get_scroll_y(ਮਾਪਾ);
        }

        x += lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
        y += lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_ਬਿੰਦੂ_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    ਜੇ(diff.x == 0 && diff.y == 0) ਵਾਪਸ;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t ਮਾਪਾ_fit_area;
    bool on1 = false;
    ਜੇ(ਮਾਪਾ) {
        lv_obj_get_content_coords(ਮਾਪਾ, &ਮਾਪਾ_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_ਵਿੱਚ(&ori, &ਮਾਪਾ_fit_area, 0);
        ਜੇ(!on1) lv_obj_scrollbar_invalidate(ਮਾਪਾ);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    ਜੇ(ਮਾਪਾ) lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    ਜੇ(ਮਾਪਾ) {
        bool on2 = lv_area_is_ਵਿੱਚ(&obj->coords, &ਮਾਪਾ_fit_area, 0);
        ਜੇ(!on1 || !on2) lv_obj_scrollbar_invalidate(ਮਾਪਾ);
    }
}

ਖਾਲੀ lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        ਜੇ(ignore_floating && lv_obj_is_floating(ਬੱਚਾ)) ਜਾਰੀ;
        ਬੱਚਾ->coords.x1 += x_diff;
        ਬੱਚਾ->coords.y1 += y_diff;
        ਬੱਚਾ->coords.x2 += x_diff;
        ਬੱਚਾ->coords.y2 += y_diff;

        lv_obj_move_children_by(ਬੱਚਾ, x_diff, y_diff, false);
    }
}

ਖਾਲੀ lv_obj_transform_ਬਿੰਦੂ(ਅਚਲ lv_obj_t * obj, lv_ਬਿੰਦੂ_t * p, lv_obj_ਬਿੰਦੂ_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(p != ਨਿਰਾ, ਵਾਪਸ);

    lv_obj_transform_ਬਿੰਦੂ_array(obj, p, 1, flags);
}

ਖਾਲੀ lv_obj_transform_ਬਿੰਦੂ_array(ਅਚਲ lv_obj_t * obj, lv_ਬਿੰਦੂ_t points[], ਮਾਪ_t ਗਿਣਤੀ_2,
                                  lv_obj_ਬਿੰਦੂ_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(points != ਨਿਰਾ || ਗਿਣਤੀ_2 == 0, ਵਾਪਸ);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(inverse) {
        ਜੇ(recursive && ਮਾਪਾ) lv_obj_transform_ਬਿੰਦੂ_array(ਮਾਪਾ, points, ਗਿਣਤੀ_2, flags);
        ਜੇ(do_tranf) transform_ਬਿੰਦੂ_array(obj, points, ਗਿਣਤੀ_2, inverse);
    }
    ਨਹੀਂਤਾਂ {
        ਜੇ(do_tranf) transform_ਬਿੰਦੂ_array(obj, points, ਗਿਣਤੀ_2, inverse);
        ਜੇ(recursive && ਮਾਪਾ) lv_obj_transform_ਬਿੰਦੂ_array(ਮਾਪਾ, points, ਗਿਣਤੀ_2, flags);
    }
}

ਖਾਲੀ lv_obj_get_transformed_area(ਅਚਲ lv_obj_t * obj, lv_area_t * area, lv_obj_ਬਿੰਦੂ_transform_flag_t flags)
{
    LV_CHECK_ARG(area != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_ਬਿੰਦੂ_t p[4] = {
        {area->x1, area->y1},
        {area->x1, area->y2 + 1},
        {area->x2 + 1, area->y1},
        {area->x2 + 1, area->y2 + 1},
    };

    lv_obj_transform_ਬਿੰਦੂ_array(obj, p, 4, flags);

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

ਸਥਿਰ lv_obj_ਰੁੱਖ_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    ਜੇ(!obj->has_blur) ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਅਗਲਾ;

    /*invalidate_area_core() expects untransformed coordinates and applies the
     *transform itself*/
    lv_area_t obj_coords;
    int32_t ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ਮਾਪ, ext_ਮਾਪ);

    /*The invalidated areas are in screen coordinates so check the overlap
     *against the transformed area*/
    lv_area_t scr_coords;
    scr_coords = obj_coords;
    ਜੇ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_ਅੰਕੜੇ;
    uint32_t i;
    ਲਈ(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        ਜੇ(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_SKIP_CHILDREN;
        }
    }

    ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਅਗਲਾ;
}

ਖਾਲੀ lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    ਜੇ(disp->inv_p == 0) ਵਾਪਸ;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    ਜੇ(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) ਵਾਪਸ;

    uint32_t prev_inv_p;
    ਕਰੋ {
        prev_inv_p = disp->inv_p;

        lv_obj_ਰੁੱਖ_walk(disp->act_scr, blur_expand_walk_cb, disp);
        ਜੇ(disp->prev_scr) lv_obj_ਰੁੱਖ_walk(disp->prev_scr, blur_expand_walk_cb, disp);
        lv_obj_ਰੁੱਖ_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_ਰੁੱਖ_walk(disp->ਸਿਖਰ_layer, blur_expand_walk_cb, disp);
        lv_obj_ਰੁੱਖ_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } ਜਦੋਂ(disp->inv_p > prev_inv_p);
}

lv_ਨਤੀਜਾ_t lv_obj_invalidate_area(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_area_t * area)
{
    LV_CHECK_ARG(area != ਨਿਰਾ, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    ਜੇ(!lv_display_is_invalidation_enabled(disp)) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    ਜੇ(obj->has_blur) ਵਾਪਸ lv_obj_invalidate(obj);
    ਨਹੀਂਤਾਂ ਵਾਪਸ obj_invalidate_area_ਅੰਦਰੂਨੀ(obj, area);
}


lv_ਨਤੀਜਾ_t lv_obj_invalidate(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    ਜੇ(!lv_display_is_invalidation_enabled(disp)) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_ਮਾਪ;
    obj_coords.y1 -= ext_ਮਾਪ;
    obj_coords.x2 += ext_ਮਾਪ;
    obj_coords.y2 += ext_ਮਾਪ;

    lv_ਨਤੀਜਾ_t res = obj_invalidate_area_ਅੰਦਰੂਨੀ(obj, &obj_coords);

    ਵਾਪਸ res;
}

bool lv_obj_area_is_visible(ਅਚਲ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    ਜੇ(lv_obj_is_hidden(obj)) ਵਾਪਸ false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    ਜੇ(obj_scr != lv_display_get_screen_ਸਰਗਰਮ(disp) &&
       obj_scr != lv_display_get_screen_prev(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_ਸਿਖਰ(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        ਵਾਪਸ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ਮਾਪ, ext_ਮਾਪ);

    /*The area is not on the object*/
    ਜੇ(!lv_area_intersect(area, area, &obj_coords)) ਵਾਪਸ false;

    ਜੇ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜਦੋਂ(ਮਾਪਾ != ਨਿਰਾ) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        ਜੇ(lv_obj_is_hidden(ਮਾਪਾ)) ਵਾਪਸ false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t ਮਾਪਾ_coords = ਮਾਪਾ->coords;
        ਜੇ(lv_obj_is_ਉਛਾਲ_visible(ਮਾਪਾ)) {
            int32_t ਮਾਪਾ_ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(ਮਾਪਾ);
            lv_area_increase(&ਮਾਪਾ_coords, ਮਾਪਾ_ext_ਮਾਪ, ਮਾਪਾ_ext_ਮਾਪ);
        }

        ਜੇ(is_transformed(ਮਾਪਾ)) {
            lv_obj_get_transformed_area(ਮਾਪਾ, &ਮਾਪਾ_coords, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_RECURSIVE);
        }
        ਜੇ(!lv_area_intersect(area, area, &ਮਾਪਾ_coords)) ਵਾਪਸ false;

        ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਮਾਪਾ);
    }

    ਵਾਪਸ true;
}

bool lv_obj_is_visible(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    lv_area_t obj_coords;
    int32_t ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_ਮਾਪ;
    obj_coords.y1 -= ext_ਮਾਪ;
    obj_coords.x2 += ext_ਮਾਪ;
    obj_coords.y2 += ext_ਮਾਪ;

    ਵਾਪਸ lv_obj_area_is_visible(obj, &obj_coords);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ext_click_area(lv_obj_t * obj, int32_t ਮਾਪ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }
    obj->spec_attr->ext_click_pad = ਮਾਪ;
}

ਖਾਲੀ lv_obj_get_click_area(ਅਚਲ lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    *area = obj->coords;
    ਜੇ(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_ਪ੍ਰਾਪਤੀ_test(lv_obj_t * obj, ਅਚਲ lv_ਬਿੰਦੂ_t * ਬਿੰਦੂ)
{
    LV_CHECK_ARG(ਬਿੰਦੂ != ਨਿਰਾ, ਵਾਪਸ false);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    ਜੇ(!lv_obj_is_clickable(obj)) ਵਾਪਸ false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_ਬਿੰਦੂ_on(&a, ਬਿੰਦੂ, 0);
    ਜੇ(res == false) ਵਾਪਸ false;

    ਜੇ(lv_obj_is_adv_hittest(obj)) {
        lv_ਪ੍ਰਾਪਤੀ_test_info_t ਪ੍ਰਾਪਤੀ_info;
        ਪ੍ਰਾਪਤੀ_info.ਬਿੰਦੂ = ਬਿੰਦੂ;
        ਪ੍ਰਾਪਤੀ_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_ਪ੍ਰਾਪਤੀ_TEST, &ਪ੍ਰਾਪਤੀ_info);
        ਵਾਪਸ ਪ੍ਰਾਪਤੀ_info.res;
    }

    ਵਾਪਸ res;
}

int32_t lv_clamp_ਚੌੜਾਈ(int32_t ਚੌੜਾਈ, int32_t min_ਚੌੜਾਈ, int32_t max_ਚੌੜਾਈ, int32_t ref_ਚੌੜਾਈ)
{
    ਜੇ(LV_COORD_IS_PCT(min_ਚੌੜਾਈ)) min_ਚੌੜਾਈ = (ref_ਚੌੜਾਈ * LV_COORD_GET_PCT(min_ਚੌੜਾਈ)) / 100;
    ਜੇ(LV_COORD_IS_PCT(max_ਚੌੜਾਈ)) max_ਚੌੜਾਈ = (ref_ਚੌੜਾਈ * LV_COORD_GET_PCT(max_ਚੌੜਾਈ)) / 100;
    ਵਾਪਸ LV_CLAMP(min_ਚੌੜਾਈ, ਚੌੜਾਈ, max_ਚੌੜਾਈ);
}

int32_t lv_clamp_ਉਚਾਈ(int32_t ਉਚਾਈ, int32_t min_ਉਚਾਈ, int32_t max_ਉਚਾਈ, int32_t ref_ਉਚਾਈ)
{
    ਜੇ(LV_COORD_IS_PCT(min_ਉਚਾਈ)) min_ਉਚਾਈ = (ref_ਉਚਾਈ * LV_COORD_GET_PCT(min_ਉਚਾਈ)) / 100;
    ਜੇ(LV_COORD_IS_PCT(max_ਉਚਾਈ)) max_ਉਚਾਈ = (ref_ਉਚਾਈ * LV_COORD_GET_PCT(max_ਉਚਾਈ)) / 100;
    ਵਾਪਸ LV_CLAMP(min_ਉਚਾਈ, ਉਚਾਈ, max_ਉਚਾਈ);
}

ਖਾਲੀ lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_transform(lv_obj_t * obj, ਅਚਲ lv_ਮੈਟ੍ਰਿਕਸ_t * ਮੈਟ੍ਰਿਕਸ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਜੇ(!ਮੈਟ੍ਰਿਕਸ) {
        lv_obj_reset_transform(obj);
        ਵਾਪਸ;
    }

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }

    ਜੇ(!obj->spec_attr->ਮੈਟ੍ਰਿਕਸ) {
        obj->spec_attr->ਮੈਟ੍ਰਿਕਸ = lv_malloc(ਆਕਾਰ(lv_ਮੈਟ੍ਰਿਕਸ_t));
        LV_ASSERT_MALLOC(obj->spec_attr->ਮੈਟ੍ਰਿਕਸ);
        ਜੇ(obj->spec_attr->ਮੈਟ੍ਰਿਕਸ == ਨਿਰਾ) ਵਾਪਸ;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Copy the matrix */
    *obj->spec_attr->ਮੈਟ੍ਰਿਕਸ = *ਮੈਟ੍ਰਿਕਸ;

    /* Matrix is set. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
    LV_ਅਣਵਰਤਿਆ(ਮੈਟ੍ਰਿਕਸ);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਜੇ(!obj->spec_attr) {
        ਵਾਪਸ;
    }

    ਜੇ(!obj->spec_attr->ਮੈਟ੍ਰਿਕਸ) {
        ਵਾਪਸ;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->ਮੈਟ੍ਰਿਕਸ);
    obj->spec_attr->ਮੈਟ੍ਰਿਕਸ = ਨਿਰਾ;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
#ਜੇ_ਅੰਤ
}

ਅਚਲ lv_ਮੈਟ੍ਰਿਕਸ_t * lv_obj_get_transform(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਜੇ(obj->spec_attr) {
        ਵਾਪਸ obj->spec_attr->ਮੈਟ੍ਰਿਕਸ;
    }
#ਨਹੀਂਤਾਂ
    LV_ਅਣਵਰਤਿਆ(obj);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਨਿਰਾ;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ lv_ਨਤੀਜਾ_t obj_invalidate_area_ਅੰਦਰੂਨੀ(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    ਵਾਪਸ invalidate_area_core(obj, &area_tmp);
}

ਸਥਿਰ bool is_transformed(ਅਚਲ lv_obj_t * obj)
{
    ਜਦੋਂ(obj) {
        ਜੇ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) ਵਾਪਸ true;
        obj = obj->ਮਾਪਾ;
    }
    ਵਾਪਸ false;
}

ਸਥਿਰ int32_t calc_content_ਚੌੜਾਈ(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    ਜੇ(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_ਸੱਜਾ = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t space_ਖੱਬਾ = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_ਚੌੜਾਈ(obj) + space_ਖੱਬਾ + space_ਸੱਜਾ;

    int32_t ਬੱਚਾ_res = LV_COORD_MIN;

    ਜੇ(!lv_layout_get_min_ਮਾਪ(obj, &ਬੱਚਾ_res, true)) {
        uint32_t i;
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
        /*With RTL find the left most coordinate*/
        ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL) {
            ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                int32_t ਬੱਚਾ_res_tmp = LV_COORD_MIN;
                lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                ਜੇ(ਬੱਚਾ->hidden || ਬੱਚਾ->floating)
                    ਜਾਰੀ;

                ਜੇ(ਬੱਚਾ->w_ignore_ਮਾਪ)
                    ਜਾਰੀ;

                ਜੇ(!lv_obj_is_layout_positioned(ਬੱਚਾ)) {
                    lv_align_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                    ਚੋਣ(align) {
                        ਹਾਲਤ LV_ALIGN_DEFAULT:
                        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ:
                        ਹਾਲਤ LV_ALIGN_BOTTOM_ਸੱਜਾ:
                        ਹਾਲਤ LV_ALIGN_ਸੱਜਾ_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            ਬੱਚਾ_res_tmp = obj->coords.x2 - ਬੱਚਾ->coords.x1 + 1;
                            ਰੋਕੋ;
                        ਮੂਲ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ਜੇ(lv_obj_get_style_x_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN) == 0) {
                                ਬੱਚਾ_res_tmp = lv_area_get_ਚੌੜਾਈ(&ਬੱਚਾ->coords) + space_ਸੱਜਾ;
                                ਬੱਚਾ_res_tmp += lv_obj_get_style_margin_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                            }
                            ਰੋਕੋ;
                    }
                }
                ਨਹੀਂਤਾਂ {
                    ਬੱਚਾ_res_tmp = obj->coords.x2 - ਬੱਚਾ->coords.x1 + 1;
                }
                ਬੱਚਾ_res = LV_MAX(ਬੱਚਾ_res, ਬੱਚਾ_res_tmp + lv_obj_get_style_margin_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN));
            }
            ਜੇ(ਬੱਚਾ_res != LV_COORD_MIN) {
                ਬੱਚਾ_res += space_ਖੱਬਾ;
            }
        }
        /*Else find the right most coordinate*/
        ਨਹੀਂਤਾਂ {
            ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                int32_t ਬੱਚਾ_res_tmp = LV_COORD_MIN;
                lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                ਜੇ(ਬੱਚਾ->hidden || ਬੱਚਾ->floating) ਜਾਰੀ;

                ਜੇ(ਬੱਚਾ->w_ignore_ਮਾਪ)
                    ਜਾਰੀ;

                ਜੇ(!lv_obj_is_layout_positioned(ਬੱਚਾ)) {
                    lv_align_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                    ਚੋਣ(align) {
                        ਹਾਲਤ LV_ALIGN_DEFAULT:
                        ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ:
                        ਹਾਲਤ LV_ALIGN_BOTTOM_ਖੱਬਾ:
                        ਹਾਲਤ LV_ALIGN_ਖੱਬਾ_MID:
                            /*Normal left aligns.*/
                            ਬੱਚਾ_res_tmp = ਬੱਚਾ->coords.x2 - obj->coords.x1 + 1;
                            ਰੋਕੋ;
                        ਮੂਲ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ਜੇ(lv_obj_get_style_x_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN) == 0) {
                                ਬੱਚਾ_res_tmp = lv_area_get_ਚੌੜਾਈ(&ਬੱਚਾ->coords) + space_ਖੱਬਾ;
                                ਬੱਚਾ_res_tmp += lv_obj_get_style_margin_ਸੱਜਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                            }
                            ਰੋਕੋ;
                    }
                }
                ਨਹੀਂਤਾਂ {
                    ਬੱਚਾ_res_tmp = ਬੱਚਾ->coords.x2 - obj->coords.x1 + 1;
                }

                ਬੱਚਾ_res = LV_MAX(ਬੱਚਾ_res, ਬੱਚਾ_res_tmp + lv_obj_get_style_margin_ਸੱਜਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN));
            }

            ਜੇ(ਬੱਚਾ_res != LV_COORD_MIN) {
                ਬੱਚਾ_res += space_ਸੱਜਾ;
            }
        }
    }

    ਜੇ(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    ਜੇ(ਬੱਚਾ_res == LV_COORD_MIN) {
        ਵਾਪਸ self_w;
    }

    ਵਾਪਸ LV_MAX(ਬੱਚਾ_res, self_w);
}

ਸਥਿਰ int32_t calc_content_ਉਚਾਈ(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    ਜੇ(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_ਸਿਖਰ = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_ਉਚਾਈ(obj) + space_ਸਿਖਰ + space_bottom;

    int32_t ਬੱਚਾ_res = LV_COORD_MIN;

    ਜੇ(!lv_layout_get_min_ਮਾਪ(obj, &ਬੱਚਾ_res, false)) {
        uint32_t i;
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
        ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
            int32_t ਬੱਚਾ_res_tmp = LV_COORD_MIN;
            lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
            ਜੇ(ਬੱਚਾ->hidden || ਬੱਚਾ->floating) ਜਾਰੀ;

            ਜੇ(ਬੱਚਾ->h_ignore_ਮਾਪ)
                ਜਾਰੀ;

            ਜੇ(!lv_obj_is_layout_positioned(ਬੱਚਾ)) {
                lv_align_t align = lv_obj_get_style_align_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                ਚੋਣ(align) {
                    ਹਾਲਤ LV_ALIGN_DEFAULT:
                    ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਸੱਜਾ:
                    ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_MID:
                    ਹਾਲਤ LV_ALIGN_ਸਿਖਰ_ਖੱਬਾ:
                        /*Normal top aligns. */
                        ਬੱਚਾ_res_tmp = ਬੱਚਾ->coords.y2 - obj->coords.y1 + 1;
                        ਰੋਕੋ;
                    ਮੂਲ:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        ਜੇ(lv_obj_get_style_y_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN) == 0) {
                            ਬੱਚਾ_res_tmp = lv_area_get_ਉਚਾਈ(&ਬੱਚਾ->coords) + space_ਸਿਖਰ;
                            ਬੱਚਾ_res_tmp += lv_obj_get_style_margin_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
                        }
                        ਰੋਕੋ;
                }
            }
            ਨਹੀਂਤਾਂ {
                ਬੱਚਾ_res_tmp = ਬੱਚਾ->coords.y2 - obj->coords.y1 + 1;
            }

            ਬੱਚਾ_res = LV_MAX(ਬੱਚਾ_res, ਬੱਚਾ_res_tmp + lv_obj_get_style_margin_bottom_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN));
        }

        ਜੇ(ਬੱਚਾ_res != LV_COORD_MIN) {
            ਬੱਚਾ_res += space_bottom;
        }
    }

    ਜੇ(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    ਜੇ(ਬੱਚਾ_res == LV_COORD_MIN) {
        ਵਾਪਸ self_h;
    }

    ਵਾਪਸ LV_MAX(self_h, ਬੱਚਾ_res);
}

ਸਥਿਰ ਖਾਲੀ layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        layout_update_core(ਬੱਚਾ);
    }

    ਜੇ(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_ਮਾਪ(obj);
        lv_obj_refr_pos(obj);

        ਜੇ(ਬੱਚਾ_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    ਜੇ(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

ਸਥਿਰ ਖਾਲੀ transform_ਬਿੰਦੂ_array(ਅਚਲ lv_obj_t * obj, lv_ਬਿੰਦੂ_t * p, ਮਾਪ_t p_ਗਿਣਤੀ, bool inv)
{
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਅਚਲ lv_ਮੈਟ੍ਰਿਕਸ_t * obj_ਮੈਟ੍ਰਿਕਸ = lv_obj_get_transform(obj);
    ਜੇ(obj_ਮੈਟ੍ਰਿਕਸ) {
        lv_ਮੈਟ੍ਰਿਕਸ_t m;
        lv_ਮੈਟ੍ਰਿਕਸ_ਪਛਾਣ_ਤੱਤ(&m);
        lv_ਮੈਟ੍ਰਿਕਸ_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_ਮੈਟ੍ਰਿਕਸ_multiply(&m, obj_ਮੈਟ੍ਰਿਕਸ);
        lv_ਮੈਟ੍ਰਿਕਸ_translate(&m, -obj->coords.x1, -obj->coords.y1);

        ਜੇ(inv) {
            lv_ਮੈਟ੍ਰਿਕਸ_t inv_m;
            lv_ਮੈਟ੍ਰਿਕਸ_inverse(&inv_m, &m);
            m = inv_m;
        }

        ਲਈ(ਮਾਪ_t i = 0; i < p_ਗਿਣਤੀ; i++) {
            lv_ਬਿੰਦੂ_precise_t p_precise = lv_ਬਿੰਦੂ_ਮੰਜ਼ਿਲ_precise(&p[i]);
            lv_ਬਿੰਦੂ_precise_t res = lv_ਮੈਟ੍ਰਿਕਸ_transform_precise_ਬਿੰਦੂ(&m, &p_precise);
            p[i] = lv_ਬਿੰਦੂ_from_precise(&res);
        }

        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_ਸੁਰੱਖਿਅਤ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    ਜੇ(scale_x == 0) scale_x = 1;
    ਜੇ(scale_y == 0) scale_y = 1;

    ਜੇ(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) ਵਾਪਸ;

    lv_ਬਿੰਦੂ_t ਧੁਰਾ = {
        .x = lv_obj_get_style_transform_ਧੁਰਾ_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ਧੁਰਾ_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN)
    };

    ਜੇ(LV_COORD_IS_PCT(ਧੁਰਾ.x)) {
        ਧੁਰਾ.x = (LV_COORD_GET_PCT(ਧੁਰਾ.x) * lv_area_get_ਚੌੜਾਈ(&obj->coords)) / 100;
    }
    ਜੇ(LV_COORD_IS_PCT(ਧੁਰਾ.y)) {
        ਧੁਰਾ.y = (LV_COORD_GET_PCT(ਧੁਰਾ.y) * lv_area_get_ਉਚਾਈ(&obj->coords)) / 100;
    }

    ਧੁਰਾ.x = obj->coords.x1 + ਧੁਰਾ.x;
    ਧੁਰਾ.y = obj->coords.y1 + ਧੁਰਾ.y;

    ਜੇ(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_ਬਿੰਦੂ_array_transform(p, p_ਗਿਣਤੀ, angle, scale_x, scale_y, &ਧੁਰਾ, !inv);
}

ਸਥਿਰ lv_ਨਤੀਜਾ_t invalidate_area_core(ਅਚਲ lv_obj_t * obj, lv_area_t * area_tmp)
{
    ਜੇ(!lv_obj_area_is_visible(obj, area_tmp)) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#ਨਹੀਂਤਾਂ
    ਜੇ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#ਜੇ_ਅੰਤ

    lv_ਨਤੀਜਾ_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    ਵਾਪਸ res;
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
ਸਥਿਰ int32_t calc_dynamic_ਚੌੜਾਈ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ਅਚਲ content_ਚੌੜਾਈ)
{
    LV_ASSERT(prop == LV_STYLE_ਚੌੜਾਈ || prop == LV_STYLE_MIN_ਚੌੜਾਈ || prop == LV_STYLE_MAX_ਚੌੜਾਈ);

    int32_t ਚੌੜਾਈ = lv_obj_get_style_prop(obj, 0, prop).ਅੰਕ;

    ਜੇ(ਚੌੜਾਈ == LV_ਮਾਪ_CONTENT) {
        ਜੇ(content_ਚੌੜਾਈ == ਨਿਰਾ) {
            ਚੌੜਾਈ = calc_content_ਚੌੜਾਈ(obj);
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ(*content_ਚੌੜਾਈ < 0) {
                *content_ਚੌੜਾਈ = calc_content_ਚੌੜਾਈ(obj);
            }
            ਚੌੜਾਈ = *content_ਚੌੜਾਈ;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(LV_COORD_IS_PCT(ਚੌੜਾਈ)) {
        lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
        int32_t ਮਾਪਾ_w = lv_obj_get_content_ਚੌੜਾਈ(ਮਾਪਾ);
        ਚੌੜਾਈ = (LV_COORD_GET_PCT(ਚੌੜਾਈ) * ਮਾਪਾ_w) / 100;
        ਚੌੜਾਈ -= lv_obj_get_style_margin_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) + lv_obj_get_style_margin_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj,
                                                                                                                   LV_PART_MAIN);
    }
    ਵਾਪਸ ਚੌੜਾਈ;
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
ਸਥਿਰ int32_t calc_dynamic_ਉਚਾਈ(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ਅਚਲ content_ਉਚਾਈ)
{
    LV_ASSERT(prop == LV_STYLE_ਉਚਾਈ || prop == LV_STYLE_MIN_ਉਚਾਈ || prop == LV_STYLE_MAX_ਉਚਾਈ);

    int32_t ਉਚਾਈ = lv_obj_get_style_prop(obj, 0, prop).ਅੰਕ;

    ਜੇ(ਉਚਾਈ == LV_ਮਾਪ_CONTENT) {
        ਜੇ(content_ਉਚਾਈ == ਨਿਰਾ) {
            ਉਚਾਈ = calc_content_ਉਚਾਈ(obj);
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ(*content_ਉਚਾਈ < 0) {
                *content_ਉਚਾਈ = calc_content_ਉਚਾਈ(obj);
            }
            ਉਚਾਈ = *content_ਉਚਾਈ;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(LV_COORD_IS_PCT(ਉਚਾਈ)) {
        lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
        int32_t ਮਾਪਾ_h = lv_obj_get_content_ਉਚਾਈ(ਮਾਪਾ);
        ਉਚਾਈ = (LV_COORD_GET_PCT(ਉਚਾਈ) * ਮਾਪਾ_h) / 100;
        ਉਚਾਈ -= lv_obj_get_style_margin_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_ਅੰਦਰੂਨੀ(obj,
                                                                                                                    LV_PART_MAIN);
    }
    ਵਾਪਸ ਉਚਾਈ;
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
ਸਥਿਰ bool ਮਾਪ_ਵਿੱਚ_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t ਮਾਪ_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    ਜੇ(min > max) ਵਾਪਸ LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    ਜੇ(unclamped < min) ਵਾਪਸ LV_COORD_IS_PCT(min_style);
    ਜੇ(unclamped > max) ਵਾਪਸ LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(ਮਾਪ_style);
    ਜੇ(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    ਜੇ(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    ਵਾਪਸ is_pct;
}
