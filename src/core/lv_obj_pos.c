/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "../misc/lv_area_private.h"
#شامل "../lvgl_public.h"
#شامل "../layouts/lv_layout_private.h"
#شامل "lv_obj_event_private.h"
#شامل "lv_obj_draw_private.h"
#شامل "lv_obj_style_private.h"
#شامل "lv_obj_private.h"
#شامل "../display/lv_display_private.h"
#شامل "lv_refr_private.h"
#شامل "../core/lv_global.h"
#شامل "lv_obj_class_private.h"
#شامل "lv_obj_style_internal.h"
#شامل "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)
#تعریف_کرو update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#اگر_غیر_تعریف_شدہ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #تعریف_کرو LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#اگر_ختم

#اگر LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #غلطی "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#اگر_ختم

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساکن int32_t calc_content_چوڑائی(lv_obj_t * obj);
ساکن int32_t calc_content_اونچائی(lv_obj_t * obj);
ساکن خالی layout_update_core(lv_obj_t * obj);
ساکن خالی transform_نقطہ_array(ثابت lv_obj_t * obj, lv_نقطہ_t * p, سائز_t p_گنتی, bool inv);
ساکن bool is_transformed(ثابت lv_obj_t * obj);
ساکن lv_نتیجہ_t invalidate_area_core(ثابت lv_obj_t * obj, lv_area_t * area_tmp);
ساکن lv_نتیجہ_t obj_invalidate_area_اندرونی(ثابت lv_obj_t * obj, ثابت lv_area_t * area);
ساکن int32_t calc_dynamic_چوڑائی(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_چوڑائی);
ساکن int32_t calc_dynamic_اونچائی(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_اونچائی);
ساکن bool سائز_اندر_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t سائز_style,
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

خالی lv_obj_سیٹ_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_سیٹ_x(obj, x);
    lv_obj_سیٹ_y(obj, y);
}

خالی lv_obj_سیٹ_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_style_res_t res_x;
    lv_style_قدر_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    اگر((res_x == LV_STYLE_RES_ملا && v_x.عدد != x) || res_x == LV_STYLE_RES_NOT_ملا) {
        lv_obj_سیٹ_style_x(obj, x, 0);
    }
}

خالی lv_obj_سیٹ_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_style_res_t res_y;
    lv_style_قدر_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    اگر((res_y == LV_STYLE_RES_ملا && v_y.عدد != y) || res_y == LV_STYLE_RES_NOT_ملا) {
        lv_obj_سیٹ_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_چوڑائی(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);
    LV_CHECK_ARG(prop == LV_STYLE_چوڑائی || prop == LV_STYLE_MIN_چوڑائی || prop == LV_STYLE_MAX_چوڑائی, واپس 0);

    واپس calc_dynamic_چوڑائی(obj, prop, تہی);
}

int32_t lv_obj_calc_dynamic_اونچائی(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);
    LV_CHECK_ARG(prop == LV_STYLE_اونچائی || prop == LV_STYLE_MIN_اونچائی || prop == LV_STYLE_MAX_اونچائی, واپس 0);

    واپس calc_dynamic_اونچائی(obj, prop, تہی);
}

bool lv_obj_refr_سائز(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    /*If the width or height is set by a layout do not modify them*/
    اگر(obj->w_layout && obj->h_layout) واپس false;

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد == تہی) واپس false;

    int32_t w;
    اگر(obj->w_layout) {
        w = lv_obj_get_چوڑائی(obj);
    }
    ورنہ {
        int32_t content_چوڑائی = -1;
        int32_t unclamped_w = calc_dynamic_چوڑائی(obj, LV_STYLE_چوڑائی, &content_چوڑائی);
        int32_t minw = calc_dynamic_چوڑائی(obj, LV_STYLE_MIN_چوڑائی, &content_چوڑائی);
        int32_t maxw = calc_dynamic_چوڑائی(obj, LV_STYLE_MAX_چوڑائی, &content_چوڑائی);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = سائز_اندر_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_چوڑائی_اندرونی(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_چوڑائی_اندرونی(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_چوڑائی_اندرونی(obj, LV_PART_MAIN));
        obj->w_ignore_سائز =
            (w_pct && والد->w_layout == 0 && lv_obj_get_style_چوڑائی_اندرونی(والد, 0) == LV_سائز_CONTENT);
    }

    int32_t h;
    اگر(obj->h_layout) {
        h = lv_obj_get_اونچائی(obj);
    }
    ورنہ {
        int32_t content_اونچائی = -1;
        int32_t unclamped_h = calc_dynamic_اونچائی(obj, LV_STYLE_اونچائی, &content_اونچائی);
        int32_t minh = calc_dynamic_اونچائی(obj, LV_STYLE_MIN_اونچائی, &content_اونچائی);
        int32_t maxh = calc_dynamic_اونچائی(obj, LV_STYLE_MAX_اونچائی, &content_اونچائی);
        h = LV_CLAMP(minh, unclamped_h, maxh);

        /**
         * If the height in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `h_ignore_size` excludes this object from the
         * parent's content height calculation. See the width branch above.
         */
        bool h_pct = سائز_اندر_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_اونچائی_اندرونی(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_اونچائی_اندرونی(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_اونچائی_اندرونی(obj, LV_PART_MAIN));

        obj->h_ignore_سائز = (h_pct && والد->h_layout == 0 &&
                              lv_obj_get_style_اونچائی_اندرونی(والد, 0) == LV_سائز_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    اگر(lv_obj_get_چوڑائی(obj) == w && lv_obj_get_اونچائی(obj) == h)
        واپس false;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t والد_fit_area;
    lv_obj_get_content_coords(والد, &والد_fit_area);

    /*If the object is already out of the parent and its position is changes
     *surely the scrollbars also changes so invalidate them*/
    bool on1 = lv_area_is_اندر(&ori, &والد_fit_area, 0);
    اگر(!on1)
        lv_obj_scrollbar_invalidate(والد);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) == LV_بنیاد_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    ورنہ {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_سائز_بدلا, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_اندر(&obj->coords, &والد_fit_area, 0);
    اگر(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(والد);

    lv_obj_refresh_ext_draw_سائز(obj);

    واپس true;
}

خالی lv_obj_سیٹ_سائز(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_سیٹ_چوڑائی(obj, w);
    lv_obj_سیٹ_اونچائی(obj, h);
}

خالی lv_obj_سیٹ_چوڑائی(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    lv_style_res_t res_w;
    lv_style_قدر_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_چوڑائی, &v_w, 0);

    اگر((res_w == LV_STYLE_RES_ملا && v_w.عدد != w) || res_w == LV_STYLE_RES_NOT_ملا) {
        lv_obj_سیٹ_style_چوڑائی(obj, w, 0);
    }
}

خالی lv_obj_سیٹ_اونچائی(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    lv_style_res_t res_h;
    lv_style_قدر_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_اونچائی, &v_h, 0);

    اگر((res_h == LV_STYLE_RES_ملا && v_h.عدد != h) || res_h == LV_STYLE_RES_NOT_ملا) {
        lv_obj_سیٹ_style_اونچائی(obj, h, 0);
    }
}

خالی lv_obj_سیٹ_content_چوڑائی(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    int32_t بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    lv_obj_سیٹ_چوڑائی(obj, w + بایاں + دایاں);
}

خالی lv_obj_سیٹ_content_اونچائی(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    int32_t چوٹی = lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);
    lv_obj_سیٹ_اونچائی(obj, h + چوٹی + bottom);
}

خالی lv_obj_سیٹ_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_سیٹ_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    اگر(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) واپس false;

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد == تہی) واپس false;

    uint32_t layout = lv_obj_get_style_layout_اندرونی(والد, LV_PART_MAIN);
    اگر(layout) واپس true;
    ورنہ واپس false;
}

خالی lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, تہی);
}

خالی lv_obj_update_layout(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        واپس;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    جب(scr->scr_layout_inv) {
        اگر(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_پیغام(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (خالی *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            توڑو;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, تہی);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_انتہا;
}

خالی lv_obj_سیٹ_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_سیٹ_style_align(obj, align, 0);
}

خالی lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    lv_obj_سیٹ_style_align(obj, align, 0);
    lv_obj_سیٹ_pos(obj, x_ofs, y_ofs);
}

خالی lv_obj_align_مقصد(lv_obj_t * obj, ثابت lv_obj_t * بنیاد, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_update_layout(obj);
    اگر(بنیاد == تہی) بنیاد = lv_obj_get_والد(obj);

    اگر(بنیاد == تہی) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        واپس;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد == تہی) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        واپس;
    }

    int32_t pleft = lv_obj_get_style_space_بایاں_اندرونی(والد, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_چوٹی_اندرونی(والد, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_بایاں_اندرونی(بنیاد, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_چوٹی_اندرونی(بنیاد, LV_PART_MAIN);

    اگر(align == LV_ALIGN_DEFAULT) {
        اگر(lv_obj_get_style_بنیاد_dir_اندرونی(بنیاد, LV_PART_MAIN) == LV_بنیاد_DIR_RTL) align = LV_ALIGN_چوٹی_دایاں;
        ورنہ align = LV_ALIGN_چوٹی_بایاں;
    }

    انتخاب(align) {
        حالت LV_ALIGN_CENTER:
            x = lv_obj_get_content_چوڑائی(بنیاد) / 2 - lv_obj_get_چوڑائی(obj) / 2 + bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) / 2 - lv_obj_get_اونچائی(obj) / 2 + btop;
            توڑو;

        حالت LV_ALIGN_چوٹی_بایاں:
            x = bleft;
            y = btop;
            توڑو;

        حالت LV_ALIGN_چوٹی_MID:
            x = lv_obj_get_content_چوڑائی(بنیاد) / 2 - lv_obj_get_چوڑائی(obj) / 2 + bleft;
            y = btop;
            توڑو;

        حالت LV_ALIGN_چوٹی_دایاں:
            x = lv_obj_get_content_چوڑائی(بنیاد) - lv_obj_get_چوڑائی(obj) + bleft;
            y = btop;
            توڑو;

        حالت LV_ALIGN_BOTTOM_بایاں:
            x = bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) - lv_obj_get_اونچائی(obj) + btop;
            توڑو;
        حالت LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_چوڑائی(بنیاد) / 2 - lv_obj_get_چوڑائی(obj) / 2 + bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) - lv_obj_get_اونچائی(obj) + btop;
            توڑو;

        حالت LV_ALIGN_BOTTOM_دایاں:
            x = lv_obj_get_content_چوڑائی(بنیاد) - lv_obj_get_چوڑائی(obj) + bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) - lv_obj_get_اونچائی(obj) + btop;
            توڑو;

        حالت LV_ALIGN_بایاں_MID:
            x = bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) / 2 - lv_obj_get_اونچائی(obj) / 2 + btop;
            توڑو;

        حالت LV_ALIGN_دایاں_MID:
            x = lv_obj_get_content_چوڑائی(بنیاد) - lv_obj_get_چوڑائی(obj) + bleft;
            y = lv_obj_get_content_اونچائی(بنیاد) / 2 - lv_obj_get_اونچائی(obj) / 2 + btop;
            توڑو;

        حالت LV_ALIGN_برآمد_چوٹی_بایاں:
            x = 0;
            y = -lv_obj_get_اونچائی(obj);
            توڑو;

        حالت LV_ALIGN_برآمد_چوٹی_MID:
            x = lv_obj_get_چوڑائی(بنیاد) / 2 - lv_obj_get_چوڑائی(obj) / 2;
            y = -lv_obj_get_اونچائی(obj);
            توڑو;

        حالت LV_ALIGN_برآمد_چوٹی_دایاں:
            x = lv_obj_get_چوڑائی(بنیاد) - lv_obj_get_چوڑائی(obj);
            y = -lv_obj_get_اونچائی(obj);
            توڑو;

        حالت LV_ALIGN_برآمد_BOTTOM_بایاں:
            x = 0;
            y = lv_obj_get_اونچائی(بنیاد);
            توڑو;

        حالت LV_ALIGN_برآمد_BOTTOM_MID:
            x = lv_obj_get_چوڑائی(بنیاد) / 2 - lv_obj_get_چوڑائی(obj) / 2;
            y = lv_obj_get_اونچائی(بنیاد);
            توڑو;

        حالت LV_ALIGN_برآمد_BOTTOM_دایاں:
            x = lv_obj_get_چوڑائی(بنیاد) - lv_obj_get_چوڑائی(obj);
            y = lv_obj_get_اونچائی(بنیاد);
            توڑو;

        حالت LV_ALIGN_برآمد_بایاں_چوٹی:
            x = -lv_obj_get_چوڑائی(obj);
            y = 0;
            توڑو;

        حالت LV_ALIGN_برآمد_بایاں_MID:
            x = -lv_obj_get_چوڑائی(obj);
            y = lv_obj_get_اونچائی(بنیاد) / 2 - lv_obj_get_اونچائی(obj) / 2;
            توڑو;

        حالت LV_ALIGN_برآمد_بایاں_BOTTOM:
            x = -lv_obj_get_چوڑائی(obj);
            y = lv_obj_get_اونچائی(بنیاد) - lv_obj_get_اونچائی(obj);
            توڑو;

        حالت LV_ALIGN_برآمد_دایاں_چوٹی:
            x = lv_obj_get_چوڑائی(بنیاد);
            y = 0;
            توڑو;

        حالت LV_ALIGN_برآمد_دایاں_MID:
            x = lv_obj_get_چوڑائی(بنیاد);
            y = lv_obj_get_اونچائی(بنیاد) / 2 - lv_obj_get_اونچائی(obj) / 2;
            توڑو;

        حالت LV_ALIGN_برآمد_دایاں_BOTTOM:
            x = lv_obj_get_چوڑائی(بنیاد);
            y = lv_obj_get_اونچائی(بنیاد) - lv_obj_get_اونچائی(obj);
            توڑو;

        حالت LV_ALIGN_DEFAULT:
            توڑو;
    }

    اگر(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_چوڑائی(بنیاد) * LV_COORD_GET_PCT(x_ofs)) / 100;
    اگر(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_اونچائی(بنیاد) * LV_COORD_GET_PCT(y_ofs)) / 100;
    اگر(lv_obj_get_style_بنیاد_dir_اندرونی(والد, LV_PART_MAIN) == LV_بنیاد_DIR_RTL) {
        x += x_ofs + بنیاد->coords.x1 - والد->coords.x1 + lv_obj_get_scroll_دایاں(والد) - pleft;
    }
    ورنہ {
        x += x_ofs + بنیاد->coords.x1 - والد->coords.x1 + lv_obj_get_scroll_بایاں(والد) - pleft;
    }
    y += y_ofs + بنیاد->coords.y1 - والد->coords.y1 + lv_obj_get_scroll_چوٹی(والد) - ptop;
    lv_obj_سیٹ_style_align(obj, LV_ALIGN_چوٹی_بایاں, 0);
    lv_obj_سیٹ_pos(obj, x, y);

}

خالی lv_obj_get_coords(ثابت lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != تہی, واپس);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t rel_x;
    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد) {
        rel_x  = obj->coords.x1 - والد->coords.x1;
        rel_x += lv_obj_get_scroll_x(والد);
        rel_x -= lv_obj_get_style_space_بایاں_اندرونی(والد, LV_PART_MAIN);
    }
    ورنہ {
        rel_x = obj->coords.x1;
    }
    واپس rel_x;
}

int32_t lv_obj_get_x2(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_obj_get_x(obj) + lv_obj_get_چوڑائی(obj);
}

int32_t lv_obj_get_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t rel_y;
    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد) {
        rel_y = obj->coords.y1 - والد->coords.y1;
        rel_y += lv_obj_get_scroll_y(والد);
        rel_y -= lv_obj_get_style_space_چوٹی_اندرونی(والد, LV_PART_MAIN);
    }
    ورنہ {
        rel_y = obj->coords.y1;
    }
    واپس rel_y;
}

int32_t lv_obj_get_y2(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_obj_get_y(obj) + lv_obj_get_اونچائی(obj);
}

int32_t lv_obj_get_x_aligned(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_obj_get_style_x_اندرونی(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_obj_get_style_y_اندرونی(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_چوڑائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_area_get_چوڑائی(&obj->coords);
}

int32_t lv_obj_get_اونچائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    واپس lv_area_get_اونچائی(&obj->coords);
}

int32_t lv_obj_get_content_چوڑائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);

    واپس lv_obj_get_چوڑائی(obj) - بایاں - دایاں;
}

int32_t lv_obj_get_content_اونچائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t چوٹی = lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);

    واپس lv_obj_get_اونچائی(obj) - چوٹی - bottom;
}

خالی lv_obj_get_content_coords(ثابت lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != تہی, واپس);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_چوڑائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    lv_نقطہ_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_سائز, &p);
    واپس p.x;
}

int32_t lv_obj_get_self_اونچائی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    lv_نقطہ_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_سائز, &p);
    واپس p.y;
}

int32_t lv_obj_get_style_clamped_چوڑائی(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t content_چوڑائی = -1;
    int32_t w = calc_dynamic_چوڑائی(obj, LV_STYLE_چوڑائی, &content_چوڑائی);
    int32_t minw = calc_dynamic_چوڑائی(obj, LV_STYLE_MIN_چوڑائی, &content_چوڑائی);
    int32_t maxw = calc_dynamic_چوڑائی(obj, LV_STYLE_MAX_چوڑائی, &content_چوڑائی);
    اگر(w <= minw) {
        w = lv_obj_get_style_min_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    }
    ورنہ اگر(w >= maxw) {
        w = lv_obj_get_style_max_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    }
    ورنہ {
        w = lv_obj_get_style_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    }
    واپس w;
}

int32_t lv_obj_get_style_clamped_اونچائی(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t content_اونچائی = -1;
    int32_t h = calc_dynamic_اونچائی(obj, LV_STYLE_اونچائی, &content_اونچائی);
    int32_t minh = calc_dynamic_اونچائی(obj, LV_STYLE_MIN_اونچائی, &content_اونچائی);
    int32_t maxh = calc_dynamic_اونچائی(obj, LV_STYLE_MAX_اونچائی, &content_اونچائی);
    اگر(h <= minh) {
        h = lv_obj_get_style_min_اونچائی_اندرونی(obj, LV_PART_MAIN);
    }
    ورنہ اگر(h >= maxh) {
        h = lv_obj_get_style_max_اونچائی_اندرونی(obj, LV_PART_MAIN);
    }
    ورنہ {
        h = lv_obj_get_style_اونچائی_اندرونی(obj, LV_PART_MAIN);
    }
    واپس h;
}

bool lv_obj_is_style_any_چوڑائی_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t w = lv_obj_get_style_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_چوڑائی_اندرونی(obj, LV_PART_MAIN);
    واپس (w == LV_سائز_CONTENT || minw == LV_سائز_CONTENT || maxw == LV_سائز_CONTENT);
}

bool lv_obj_is_style_any_اونچائی_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t h = lv_obj_get_style_اونچائی_اندرونی(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_اونچائی_اندرونی(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_اونچائی_اندرونی(obj, LV_PART_MAIN);
    واپس (h == LV_سائز_CONTENT || minh == LV_سائز_CONTENT || maxh == LV_سائز_CONTENT);
}

bool lv_obj_is_چوڑائی_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t minw = lv_obj_calc_dynamic_چوڑائی(obj, LV_STYLE_MIN_چوڑائی);
    int32_t w = lv_obj_get_چوڑائی(obj);
    واپس w == minw;
}

bool lv_obj_is_اونچائی_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t minh = lv_obj_calc_dynamic_اونچائی(obj, LV_STYLE_MIN_اونچائی);
    int32_t h = lv_obj_get_اونچائی(obj);
    واپس h == minh;
}

bool lv_obj_is_چوڑائی_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t maxw = lv_obj_calc_dynamic_چوڑائی(obj, LV_STYLE_MAX_چوڑائی);
    int32_t w = lv_obj_get_چوڑائی(obj);
    واپس w == maxw;
}

bool lv_obj_is_اونچائی_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    int32_t maxh = lv_obj_calc_dynamic_اونچائی(obj, LV_STYLE_MAX_اونچائی);
    int32_t h = lv_obj_get_اونچائی(obj);
    واپس h == maxh;
}

bool lv_obj_refresh_self_سائز(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    اگر(!lv_obj_is_style_any_چوڑائی_content(obj) && !lv_obj_is_style_any_اونچائی_content(obj))
        واپس false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد != تہی) {
        والد->w_layout = 0;
        والد->h_layout = 0;
        lv_obj_mark_layout_as_dirty(والد);
    }
    lv_obj_mark_layout_as_dirty(obj);
    واپس true;
}

خالی lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(lv_obj_is_layout_positioned(obj)) واپس;

    lv_obj_t * والد = lv_obj_get_والد(obj);
    int32_t x = lv_obj_get_style_x_اندرونی(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_اندرونی(obj, LV_PART_MAIN);

    اگر(والد == تہی) {
        lv_obj_move_مقصد(obj, x, y);
        واپس;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_چوڑائی(والد);
    int32_t ph = lv_obj_get_content_اونچائی(والد);
    اگر(LV_COORD_IS_PCT(x)) {
        اگر(lv_obj_get_style_چوڑائی_اندرونی(والد, LV_PART_MAIN) == LV_سائز_CONTENT) x = 0; /*Avoid circular dependency*/
        ورنہ x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    اگر(LV_COORD_IS_PCT(y)) {
        اگر(lv_obj_get_style_اونچائی_اندرونی(والد, LV_PART_MAIN) == LV_سائز_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_اندرونی(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_اندرونی(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_چوڑائی(obj);
    int32_t h = lv_obj_get_اونچائی(obj);
    اگر(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    اگر(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_اندرونی(obj, LV_PART_MAIN);

    اگر(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_چوٹی_بایاں;
    }

    bool rtl = lv_obj_get_style_بنیاد_dir_اندرونی(والد, LV_PART_MAIN) == LV_بنیاد_DIR_RTL;

    اگر(rtl) {
        انتخاب(align) {
            حالت LV_ALIGN_چوٹی_بایاں:
                align = LV_ALIGN_چوٹی_دایاں;
                توڑو;
            حالت LV_ALIGN_چوٹی_دایاں:
                align = LV_ALIGN_چوٹی_بایاں;
                توڑو;
            حالت LV_ALIGN_بایاں_MID:
                align = LV_ALIGN_دایاں_MID;
                توڑو;
            حالت LV_ALIGN_دایاں_MID:
                align = LV_ALIGN_بایاں_MID;
                توڑو;
            حالت LV_ALIGN_BOTTOM_بایاں:
                align = LV_ALIGN_BOTTOM_دایاں;
                توڑو;
            حالت LV_ALIGN_BOTTOM_دایاں:
                align = LV_ALIGN_BOTTOM_بایاں;
                توڑو;
            طےشدہ:
                توڑو;
        }
    }

    انتخاب(align) {
        حالت LV_ALIGN_چوٹی_بایاں:
            توڑو;
        حالت LV_ALIGN_چوٹی_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            توڑو;
        حالت LV_ALIGN_چوٹی_دایاں:
            x = rtl ? pw - w - x : pw - w + x;
            توڑو;
        حالت LV_ALIGN_بایاں_MID:
            y += ph / 2 - h / 2;
            توڑو;
        حالت LV_ALIGN_BOTTOM_بایاں:
            y += ph - h;
            توڑو;
        حالت LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            توڑو;
        حالت LV_ALIGN_BOTTOM_دایاں:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            توڑو;
        حالت LV_ALIGN_دایاں_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            توڑو;
        حالت LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            توڑو;
        طےشدہ:
            توڑو;
    }

    lv_obj_move_مقصد(obj, x, y);
}

خالی lv_obj_move_مقصد(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * والد = obj->والد;

    اگر(والد) {
        اگر(lv_obj_is_floating(obj)) {
            x += والد->coords.x1;
            y += والد->coords.y1;
        }
        ورنہ {
            x += والد->coords.x1 - lv_obj_get_scroll_x(والد);
            y += والد->coords.y1 - lv_obj_get_scroll_y(والد);
        }

        x += lv_obj_get_style_space_بایاں_اندرونی(والد, LV_PART_MAIN);
        y += lv_obj_get_style_space_چوٹی_اندرونی(والد, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_نقطہ_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    اگر(diff.x == 0 && diff.y == 0) واپس;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t والد_fit_area;
    bool on1 = false;
    اگر(والد) {
        lv_obj_get_content_coords(والد, &والد_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_اندر(&ori, &والد_fit_area, 0);
        اگر(!on1) lv_obj_scrollbar_invalidate(والد);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    اگر(والد) lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    اگر(والد) {
        bool on2 = lv_area_is_اندر(&obj->coords, &والد_fit_area, 0);
        اگر(!on1 || !on2) lv_obj_scrollbar_invalidate(والد);
    }
}

خالی lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    برائے(i = 0; i < بچہ_cnt; i++) {
        lv_obj_t * بچہ = obj->spec_attr->children[i];
        اگر(ignore_floating && lv_obj_is_floating(بچہ)) جاری;
        بچہ->coords.x1 += x_diff;
        بچہ->coords.y1 += y_diff;
        بچہ->coords.x2 += x_diff;
        بچہ->coords.y2 += y_diff;

        lv_obj_move_children_by(بچہ, x_diff, y_diff, false);
    }
}

خالی lv_obj_transform_نقطہ(ثابت lv_obj_t * obj, lv_نقطہ_t * p, lv_obj_نقطہ_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(p != تہی, واپس);

    lv_obj_transform_نقطہ_array(obj, p, 1, flags);
}

خالی lv_obj_transform_نقطہ_array(ثابت lv_obj_t * obj, lv_نقطہ_t points[], سائز_t گنتی,
                                  lv_obj_نقطہ_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(points != تہی || گنتی == 0, واپس);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_نقطہ_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_نقطہ_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(inverse) {
        اگر(recursive && والد) lv_obj_transform_نقطہ_array(والد, points, گنتی, flags);
        اگر(do_tranf) transform_نقطہ_array(obj, points, گنتی, inverse);
    }
    ورنہ {
        اگر(do_tranf) transform_نقطہ_array(obj, points, گنتی, inverse);
        اگر(recursive && والد) lv_obj_transform_نقطہ_array(والد, points, گنتی, flags);
    }
}

خالی lv_obj_get_transformed_area(ثابت lv_obj_t * obj, lv_area_t * area, lv_obj_نقطہ_transform_flag_t flags)
{
    LV_CHECK_ARG(area != تہی, واپس);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_نقطہ_t p[4] = {
        {area->x1, area->y1},
        {area->x1, area->y2 + 1},
        {area->x2 + 1, area->y1},
        {area->x2 + 1, area->y2 + 1},
    };

    lv_obj_transform_نقطہ_array(obj, p, 4, flags);

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

ساکن lv_obj_درخت_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, خالی * user_ڈیٹا)
{
    اگر(!obj->has_blur) واپس LV_OBJ_درخت_WALK_اگلا;

    /*invalidate_area_core() expects untransformed coordinates and applies the
     *transform itself*/
    lv_area_t obj_coords;
    int32_t ext_سائز = lv_obj_get_ext_draw_سائز(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_سائز, ext_سائز);

    /*The invalidated areas are in screen coordinates so check the overlap
     *against the transformed area*/
    lv_area_t scr_coords;
    scr_coords = obj_coords;
    اگر(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_نقطہ_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_ڈیٹا;
    uint32_t i;
    برائے(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        اگر(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            واپس LV_OBJ_درخت_WALK_SKIP_CHILDREN;
        }
    }

    واپس LV_OBJ_درخت_WALK_اگلا;
}

خالی lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    اگر(disp->inv_p == 0) واپس;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    اگر(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) واپس;

    uint32_t پچھلا_inv_p;
    کرو {
        پچھلا_inv_p = disp->inv_p;

        lv_obj_درخت_walk(disp->act_scr, blur_expand_walk_cb, disp);
        اگر(disp->پچھلا_scr) lv_obj_درخت_walk(disp->پچھلا_scr, blur_expand_walk_cb, disp);
        lv_obj_درخت_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_درخت_walk(disp->چوٹی_layer, blur_expand_walk_cb, disp);
        lv_obj_درخت_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } جب(disp->inv_p > پچھلا_inv_p);
}

lv_نتیجہ_t lv_obj_invalidate_area(ثابت lv_obj_t * obj, ثابت lv_area_t * area)
{
    LV_CHECK_ARG(area != تہی, واپس LV_نتیجہ_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_نتیجہ_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    اگر(!lv_display_is_invalidation_enabled(disp)) واپس LV_نتیجہ_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    اگر(obj->has_blur) واپس lv_obj_invalidate(obj);
    ورنہ واپس obj_invalidate_area_اندرونی(obj, area);
}


lv_نتیجہ_t lv_obj_invalidate(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_نتیجہ_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    اگر(!lv_display_is_invalidation_enabled(disp)) واپس LV_نتیجہ_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_سائز = lv_obj_get_ext_draw_سائز(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_سائز;
    obj_coords.y1 -= ext_سائز;
    obj_coords.x2 += ext_سائز;
    obj_coords.y2 += ext_سائز;

    lv_نتیجہ_t res = obj_invalidate_area_اندرونی(obj, &obj_coords);

    واپس res;
}

bool lv_obj_area_is_visible(ثابت lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != تہی, واپس false);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    اگر(lv_obj_is_hidden(obj)) واپس false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    اگر(obj_scr != lv_display_get_screen_فعال(disp) &&
       obj_scr != lv_display_get_screen_پچھلا(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_چوٹی(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        واپس false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_سائز = lv_obj_get_ext_draw_سائز(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_سائز, ext_سائز);

    /*The area is not on the object*/
    اگر(!lv_area_intersect(area, area, &obj_coords)) واپس false;

    اگر(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_نقطہ_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * والد = lv_obj_get_والد(obj);
    جب(والد != تہی) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        اگر(lv_obj_is_hidden(والد)) واپس false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t والد_coords = والد->coords;
        اگر(lv_obj_is_حد_سے_تجاوز_visible(والد)) {
            int32_t والد_ext_سائز = lv_obj_get_ext_draw_سائز(والد);
            lv_area_increase(&والد_coords, والد_ext_سائز, والد_ext_سائز);
        }

        اگر(is_transformed(والد)) {
            lv_obj_get_transformed_area(والد, &والد_coords, LV_OBJ_نقطہ_TRANSFORM_FLAG_RECURSIVE);
        }
        اگر(!lv_area_intersect(area, area, &والد_coords)) واپس false;

        والد = lv_obj_get_والد(والد);
    }

    واپس true;
}

bool lv_obj_is_visible(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    lv_area_t obj_coords;
    int32_t ext_سائز = lv_obj_get_ext_draw_سائز(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_سائز;
    obj_coords.y1 -= ext_سائز;
    obj_coords.x2 += ext_سائز;
    obj_coords.y2 += ext_سائز;

    واپس lv_obj_area_is_visible(obj, &obj_coords);
}

خالی lv_obj_سیٹ_ext_click_area(lv_obj_t * obj, int32_t سائز)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }
    obj->spec_attr->ext_click_pad = سائز;
}

خالی lv_obj_get_click_area(ثابت lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != تہی, واپس);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    *area = obj->coords;
    اگر(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_کامیابی_test(lv_obj_t * obj, ثابت lv_نقطہ_t * نقطہ)
{
    LV_CHECK_ARG(نقطہ != تہی, واپس false);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    اگر(!lv_obj_is_clickable(obj)) واپس false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_نقطہ_on(&a, نقطہ, 0);
    اگر(res == false) واپس false;

    اگر(lv_obj_is_adv_hittest(obj)) {
        lv_کامیابی_test_info_t کامیابی_info;
        کامیابی_info.نقطہ = نقطہ;
        کامیابی_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_کامیابی_TEST, &کامیابی_info);
        واپس کامیابی_info.res;
    }

    واپس res;
}

int32_t lv_clamp_چوڑائی(int32_t چوڑائی, int32_t min_چوڑائی, int32_t max_چوڑائی, int32_t ref_چوڑائی)
{
    اگر(LV_COORD_IS_PCT(min_چوڑائی)) min_چوڑائی = (ref_چوڑائی * LV_COORD_GET_PCT(min_چوڑائی)) / 100;
    اگر(LV_COORD_IS_PCT(max_چوڑائی)) max_چوڑائی = (ref_چوڑائی * LV_COORD_GET_PCT(max_چوڑائی)) / 100;
    واپس LV_CLAMP(min_چوڑائی, چوڑائی, max_چوڑائی);
}

int32_t lv_clamp_اونچائی(int32_t اونچائی, int32_t min_اونچائی, int32_t max_اونچائی, int32_t ref_اونچائی)
{
    اگر(LV_COORD_IS_PCT(min_اونچائی)) min_اونچائی = (ref_اونچائی * LV_COORD_GET_PCT(min_اونچائی)) / 100;
    اگر(LV_COORD_IS_PCT(max_اونچائی)) max_اونچائی = (ref_اونچائی * LV_COORD_GET_PCT(max_اونچائی)) / 100;
    واپس LV_CLAMP(min_اونچائی, اونچائی, max_اونچائی);
}

خالی lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

خالی lv_obj_سیٹ_transform(lv_obj_t * obj, ثابت lv_میٹرکس_t * میٹرکس)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
    اگر(!میٹرکس) {
        lv_obj_reset_transform(obj);
        واپس;
    }

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }

    اگر(!obj->spec_attr->میٹرکس) {
        obj->spec_attr->میٹرکس = lv_malloc(حجم(lv_میٹرکس_t));
        LV_ASSERT_MALLOC(obj->spec_attr->میٹرکس);
        اگر(obj->spec_attr->میٹرکس == تہی) واپس;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Copy the matrix */
    *obj->spec_attr->میٹرکس = *میٹرکس;

    /* Matrix is set. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ورنہ
    LV_غیر_مستعمل(obj);
    LV_غیر_مستعمل(میٹرکس);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#اگر_ختم
}

خالی lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
    اگر(!obj->spec_attr) {
        واپس;
    }

    اگر(!obj->spec_attr->میٹرکس) {
        واپس;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->میٹرکس);
    obj->spec_attr->میٹرکس = تہی;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#ورنہ
    LV_غیر_مستعمل(obj);
#اگر_ختم
}

ثابت lv_میٹرکس_t * lv_obj_get_transform(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
    اگر(obj->spec_attr) {
        واپس obj->spec_attr->میٹرکس;
    }
#ورنہ
    LV_غیر_مستعمل(obj);
#اگر_ختم
    واپس تہی;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن lv_نتیجہ_t obj_invalidate_area_اندرونی(ثابت lv_obj_t * obj, ثابت lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    واپس invalidate_area_core(obj, &area_tmp);
}

ساکن bool is_transformed(ثابت lv_obj_t * obj)
{
    جب(obj) {
        اگر(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) واپس true;
        obj = obj->والد;
    }
    واپس false;
}

ساکن int32_t calc_content_چوڑائی(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    اگر(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t space_بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_چوڑائی(obj) + space_بایاں + space_دایاں;

    int32_t بچہ_res = LV_COORD_MIN;

    اگر(!lv_layout_get_min_سائز(obj, &بچہ_res, true)) {
        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        /*With RTL find the left most coordinate*/
        اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) == LV_بنیاد_DIR_RTL) {
            برائے(i = 0; i < بچہ_cnt; i++) {
                int32_t بچہ_res_tmp = LV_COORD_MIN;
                lv_obj_t * بچہ = obj->spec_attr->children[i];
                اگر(بچہ->hidden || بچہ->floating)
                    جاری;

                اگر(بچہ->w_ignore_سائز)
                    جاری;

                اگر(!lv_obj_is_layout_positioned(بچہ)) {
                    lv_align_t align = lv_obj_get_style_align_اندرونی(بچہ, LV_PART_MAIN);
                    انتخاب(align) {
                        حالت LV_ALIGN_DEFAULT:
                        حالت LV_ALIGN_چوٹی_دایاں:
                        حالت LV_ALIGN_BOTTOM_دایاں:
                        حالت LV_ALIGN_دایاں_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            بچہ_res_tmp = obj->coords.x2 - بچہ->coords.x1 + 1;
                            توڑو;
                        طےشدہ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            اگر(lv_obj_get_style_x_اندرونی(بچہ, LV_PART_MAIN) == 0) {
                                بچہ_res_tmp = lv_area_get_چوڑائی(&بچہ->coords) + space_دایاں;
                                بچہ_res_tmp += lv_obj_get_style_margin_بایاں_اندرونی(بچہ, LV_PART_MAIN);
                            }
                            توڑو;
                    }
                }
                ورنہ {
                    بچہ_res_tmp = obj->coords.x2 - بچہ->coords.x1 + 1;
                }
                بچہ_res = LV_MAX(بچہ_res, بچہ_res_tmp + lv_obj_get_style_margin_بایاں_اندرونی(بچہ, LV_PART_MAIN));
            }
            اگر(بچہ_res != LV_COORD_MIN) {
                بچہ_res += space_بایاں;
            }
        }
        /*Else find the right most coordinate*/
        ورنہ {
            برائے(i = 0; i < بچہ_cnt; i++) {
                int32_t بچہ_res_tmp = LV_COORD_MIN;
                lv_obj_t * بچہ = obj->spec_attr->children[i];
                اگر(بچہ->hidden || بچہ->floating) جاری;

                اگر(بچہ->w_ignore_سائز)
                    جاری;

                اگر(!lv_obj_is_layout_positioned(بچہ)) {
                    lv_align_t align = lv_obj_get_style_align_اندرونی(بچہ, LV_PART_MAIN);
                    انتخاب(align) {
                        حالت LV_ALIGN_DEFAULT:
                        حالت LV_ALIGN_چوٹی_بایاں:
                        حالت LV_ALIGN_BOTTOM_بایاں:
                        حالت LV_ALIGN_بایاں_MID:
                            /*Normal left aligns.*/
                            بچہ_res_tmp = بچہ->coords.x2 - obj->coords.x1 + 1;
                            توڑو;
                        طےشدہ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            اگر(lv_obj_get_style_x_اندرونی(بچہ, LV_PART_MAIN) == 0) {
                                بچہ_res_tmp = lv_area_get_چوڑائی(&بچہ->coords) + space_بایاں;
                                بچہ_res_tmp += lv_obj_get_style_margin_دایاں_اندرونی(بچہ, LV_PART_MAIN);
                            }
                            توڑو;
                    }
                }
                ورنہ {
                    بچہ_res_tmp = بچہ->coords.x2 - obj->coords.x1 + 1;
                }

                بچہ_res = LV_MAX(بچہ_res, بچہ_res_tmp + lv_obj_get_style_margin_دایاں_اندرونی(بچہ, LV_PART_MAIN));
            }

            اگر(بچہ_res != LV_COORD_MIN) {
                بچہ_res += space_دایاں;
            }
        }
    }

    اگر(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    اگر(بچہ_res == LV_COORD_MIN) {
        واپس self_w;
    }

    واپس LV_MAX(بچہ_res, self_w);
}

ساکن int32_t calc_content_اونچائی(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    اگر(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_چوٹی = lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_اونچائی(obj) + space_چوٹی + space_bottom;

    int32_t بچہ_res = LV_COORD_MIN;

    اگر(!lv_layout_get_min_سائز(obj, &بچہ_res, false)) {
        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        برائے(i = 0; i < بچہ_cnt; i++) {
            int32_t بچہ_res_tmp = LV_COORD_MIN;
            lv_obj_t * بچہ = obj->spec_attr->children[i];
            اگر(بچہ->hidden || بچہ->floating) جاری;

            اگر(بچہ->h_ignore_سائز)
                جاری;

            اگر(!lv_obj_is_layout_positioned(بچہ)) {
                lv_align_t align = lv_obj_get_style_align_اندرونی(بچہ, LV_PART_MAIN);
                انتخاب(align) {
                    حالت LV_ALIGN_DEFAULT:
                    حالت LV_ALIGN_چوٹی_دایاں:
                    حالت LV_ALIGN_چوٹی_MID:
                    حالت LV_ALIGN_چوٹی_بایاں:
                        /*Normal top aligns. */
                        بچہ_res_tmp = بچہ->coords.y2 - obj->coords.y1 + 1;
                        توڑو;
                    طےشدہ:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        اگر(lv_obj_get_style_y_اندرونی(بچہ, LV_PART_MAIN) == 0) {
                            بچہ_res_tmp = lv_area_get_اونچائی(&بچہ->coords) + space_چوٹی;
                            بچہ_res_tmp += lv_obj_get_style_margin_چوٹی_اندرونی(بچہ, LV_PART_MAIN);
                        }
                        توڑو;
                }
            }
            ورنہ {
                بچہ_res_tmp = بچہ->coords.y2 - obj->coords.y1 + 1;
            }

            بچہ_res = LV_MAX(بچہ_res, بچہ_res_tmp + lv_obj_get_style_margin_bottom_اندرونی(بچہ, LV_PART_MAIN));
        }

        اگر(بچہ_res != LV_COORD_MIN) {
            بچہ_res += space_bottom;
        }
    }

    اگر(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    اگر(بچہ_res == LV_COORD_MIN) {
        واپس self_h;
    }

    واپس LV_MAX(self_h, بچہ_res);
}

ساکن خالی layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    برائے(i = 0; i < بچہ_cnt; i++) {
        lv_obj_t * بچہ = obj->spec_attr->children[i];
        layout_update_core(بچہ);
    }

    اگر(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_سائز(obj);
        lv_obj_refr_pos(obj);

        اگر(بچہ_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    اگر(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

ساکن خالی transform_نقطہ_array(ثابت lv_obj_t * obj, lv_نقطہ_t * p, سائز_t p_گنتی, bool inv)
{
#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
    ثابت lv_میٹرکس_t * obj_میٹرکس = lv_obj_get_transform(obj);
    اگر(obj_میٹرکس) {
        lv_میٹرکس_t m;
        lv_میٹرکس_شناختی_عنصر(&m);
        lv_میٹرکس_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_میٹرکس_multiply(&m, obj_میٹرکس);
        lv_میٹرکس_translate(&m, -obj->coords.x1, -obj->coords.y1);

        اگر(inv) {
            lv_میٹرکس_t inv_m;
            lv_میٹرکس_inverse(&inv_m, &m);
            m = inv_m;
        }

        برائے(سائز_t i = 0; i < p_گنتی; i++) {
            lv_نقطہ_precise_t p_precise = lv_نقطہ_مقصد_precise(&p[i]);
            lv_نقطہ_precise_t res = lv_میٹرکس_transform_precise_نقطہ(&m, &p_precise);
            p[i] = lv_نقطہ_from_precise(&res);
        }

        واپس;
    }
#اگر_ختم /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_اندرونی(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_محفوظ_اندرونی(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_محفوظ_اندرونی(obj, LV_PART_MAIN);
    اگر(scale_x == 0) scale_x = 1;
    اگر(scale_y == 0) scale_y = 1;

    اگر(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) واپس;

    lv_نقطہ_t محور = {
        .x = lv_obj_get_style_transform_محور_x_اندرونی(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_محور_y_اندرونی(obj, LV_PART_MAIN)
    };

    اگر(LV_COORD_IS_PCT(محور.x)) {
        محور.x = (LV_COORD_GET_PCT(محور.x) * lv_area_get_چوڑائی(&obj->coords)) / 100;
    }
    اگر(LV_COORD_IS_PCT(محور.y)) {
        محور.y = (LV_COORD_GET_PCT(محور.y) * lv_area_get_اونچائی(&obj->coords)) / 100;
    }

    محور.x = obj->coords.x1 + محور.x;
    محور.y = obj->coords.y1 + محور.y;

    اگر(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_نقطہ_array_transform(p, p_گنتی, angle, scale_x, scale_y, &محور, !inv);
}

ساکن lv_نتیجہ_t invalidate_area_core(ثابت lv_obj_t * obj, lv_area_t * area_tmp)
{
    اگر(!lv_obj_area_is_visible(obj, area_tmp)) واپس LV_نتیجہ_INVALID;
#اگر LV_DRAW_TRANSFORM_USE_میٹرکس
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#ورنہ
    اگر(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#اگر_ختم

    lv_نتیجہ_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    واپس res;
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
ساکن int32_t calc_dynamic_چوڑائی(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ثابت content_چوڑائی)
{
    LV_ASSERT(prop == LV_STYLE_چوڑائی || prop == LV_STYLE_MIN_چوڑائی || prop == LV_STYLE_MAX_چوڑائی);

    int32_t چوڑائی = lv_obj_get_style_prop(obj, 0, prop).عدد;

    اگر(چوڑائی == LV_سائز_CONTENT) {
        اگر(content_چوڑائی == تہی) {
            چوڑائی = calc_content_چوڑائی(obj);
        }
        ورنہ {
            اگر(*content_چوڑائی < 0) {
                *content_چوڑائی = calc_content_چوڑائی(obj);
            }
            چوڑائی = *content_چوڑائی;
        }
    }
    ورنہ اگر(LV_COORD_IS_PCT(چوڑائی)) {
        lv_obj_t * والد = lv_obj_get_والد(obj);
        int32_t والد_w = lv_obj_get_content_چوڑائی(والد);
        چوڑائی = (LV_COORD_GET_PCT(چوڑائی) * والد_w) / 100;
        چوڑائی -= lv_obj_get_style_margin_بایاں_اندرونی(obj, LV_PART_MAIN) + lv_obj_get_style_margin_دایاں_اندرونی(obj,
                                                                                                                   LV_PART_MAIN);
    }
    واپس چوڑائی;
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
ساکن int32_t calc_dynamic_اونچائی(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ثابت content_اونچائی)
{
    LV_ASSERT(prop == LV_STYLE_اونچائی || prop == LV_STYLE_MIN_اونچائی || prop == LV_STYLE_MAX_اونچائی);

    int32_t اونچائی = lv_obj_get_style_prop(obj, 0, prop).عدد;

    اگر(اونچائی == LV_سائز_CONTENT) {
        اگر(content_اونچائی == تہی) {
            اونچائی = calc_content_اونچائی(obj);
        }
        ورنہ {
            اگر(*content_اونچائی < 0) {
                *content_اونچائی = calc_content_اونچائی(obj);
            }
            اونچائی = *content_اونچائی;
        }
    }
    ورنہ اگر(LV_COORD_IS_PCT(اونچائی)) {
        lv_obj_t * والد = lv_obj_get_والد(obj);
        int32_t والد_h = lv_obj_get_content_اونچائی(والد);
        اونچائی = (LV_COORD_GET_PCT(اونچائی) * والد_h) / 100;
        اونچائی -= lv_obj_get_style_margin_چوٹی_اندرونی(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_اندرونی(obj,
                                                                                                                    LV_PART_MAIN);
    }
    واپس اونچائی;
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
ساکن bool سائز_اندر_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t سائز_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    اگر(min > max) واپس LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    اگر(unclamped < min) واپس LV_COORD_IS_PCT(min_style);
    اگر(unclamped > max) واپس LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(سائز_style);
    اگر(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    اگر(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    واپس is_pct;
}
