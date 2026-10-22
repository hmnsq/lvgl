/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "../misc/lv_area_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../layouts/lv_layout_private.h"
#అన్తర్భావ "lv_obj_event_private.h"
#అన్తర్భావ "lv_obj_draw_private.h"
#అన్తర్భావ "lv_obj_style_private.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../display/lv_display_private.h"
#అన్తర్భావ "lv_refr_private.h"
#అన్తర్భావ "../core/lv_global.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "lv_obj_style_internal.h"
#అన్తర్భావ "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)
#నిర్వచించు update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#నిర్వచితం_కాకపోతే LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #నిర్వచించు LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#అయితే_ముగింపు

#ఒకవేళ LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #దోషం "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#అయితే_ముగింపు

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర int32_t calc_content_వెడల్పు(lv_obj_t * obj);
స్థిర int32_t calc_content_ఎత్తు(lv_obj_t * obj);
స్థిర శూన్య layout_update_core(lv_obj_t * obj);
స్థిర శూన్య transform_బిందువు_array(స్థిరరాశి lv_obj_t * obj, lv_బిందువు_t * p, కొలత_t p_లెక్క, bool inv);
స్థిర bool is_transformed(స్థిరరాశి lv_obj_t * obj);
స్థిర lv_ఫలితం_t invalidate_area_core(స్థిరరాశి lv_obj_t * obj, lv_area_t * area_tmp);
స్థిర lv_ఫలితం_t obj_invalidate_area_అంతర్గత(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_area_t * area);
స్థిర int32_t calc_dynamic_వెడల్పు(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_వెడల్పు);
స్థిర int32_t calc_dynamic_ఎత్తు(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_ఎత్తు);
స్థిర bool కొలత_లోపల_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t కొలత_style,
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

శూన్య lv_obj_సమితి_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_సమితి_x(obj, x);
    lv_obj_సమితి_y(obj, y);
}

శూన్య lv_obj_సమితి_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_style_res_t res_x;
    lv_style_విలువ_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    ఒకవేళ((res_x == LV_STYLE_RES_దొరికింది && v_x.అంకె != x) || res_x == LV_STYLE_RES_NOT_దొరికింది) {
        lv_obj_సమితి_style_x(obj, x, 0);
    }
}

శూన్య lv_obj_సమితి_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_style_res_t res_y;
    lv_style_విలువ_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    ఒకవేళ((res_y == LV_STYLE_RES_దొరికింది && v_y.అంకె != y) || res_y == LV_STYLE_RES_NOT_దొరికింది) {
        lv_obj_సమితి_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_వెడల్పు(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);
    LV_CHECK_ARG(prop == LV_STYLE_వెడల్పు || prop == LV_STYLE_MIN_వెడల్పు || prop == LV_STYLE_MAX_వెడల్పు, తిరిగి 0);

    తిరిగి calc_dynamic_వెడల్పు(obj, prop, ఖాళీ);
}

int32_t lv_obj_calc_dynamic_ఎత్తు(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);
    LV_CHECK_ARG(prop == LV_STYLE_ఎత్తు || prop == LV_STYLE_MIN_ఎత్తు || prop == LV_STYLE_MAX_ఎత్తు, తిరిగి 0);

    తిరిగి calc_dynamic_ఎత్తు(obj, prop, ఖాళీ);
}

bool lv_obj_refr_కొలత(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    /*If the width or height is set by a layout do not modify them*/
    ఒకవేళ(obj->w_layout && obj->h_layout) తిరిగి false;

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ == ఖాళీ) తిరిగి false;

    int32_t w;
    ఒకవేళ(obj->w_layout) {
        w = lv_obj_get_వెడల్పు(obj);
    }
    లేకపోతే {
        int32_t content_వెడల్పు = -1;
        int32_t unclamped_w = calc_dynamic_వెడల్పు(obj, LV_STYLE_వెడల్పు, &content_వెడల్పు);
        int32_t minw = calc_dynamic_వెడల్పు(obj, LV_STYLE_MIN_వెడల్పు, &content_వెడల్పు);
        int32_t maxw = calc_dynamic_వెడల్పు(obj, LV_STYLE_MAX_వెడల్పు, &content_వెడల్పు);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = కొలత_లోపల_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN));
        obj->w_ignore_కొలత =
            (w_pct && మాతృ->w_layout == 0 && lv_obj_get_style_వెడల్పు_అంతర్గత(మాతృ, 0) == LV_కొలత_CONTENT);
    }

    int32_t h;
    ఒకవేళ(obj->h_layout) {
        h = lv_obj_get_ఎత్తు(obj);
    }
    లేకపోతే {
        int32_t content_ఎత్తు = -1;
        int32_t unclamped_h = calc_dynamic_ఎత్తు(obj, LV_STYLE_ఎత్తు, &content_ఎత్తు);
        int32_t minh = calc_dynamic_ఎత్తు(obj, LV_STYLE_MIN_ఎత్తు, &content_ఎత్తు);
        int32_t maxh = calc_dynamic_ఎత్తు(obj, LV_STYLE_MAX_ఎత్తు, &content_ఎత్తు);
        h = LV_CLAMP(minh, unclamped_h, maxh);

        /**
         * If the height in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `h_ignore_size` excludes this object from the
         * parent's content height calculation. See the width branch above.
         */
        bool h_pct = కొలత_లోపల_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN));

        obj->h_ignore_కొలత = (h_pct && మాతృ->h_layout == 0 &&
                              lv_obj_get_style_ఎత్తు_అంతర్గత(మాతృ, 0) == LV_కొలత_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    ఒకవేళ(lv_obj_get_వెడల్పు(obj) == w && lv_obj_get_ఎత్తు(obj) == h)
        తిరిగి false;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t మాతృ_fit_area;
    lv_obj_get_content_coords(మాతృ, &మాతృ_fit_area);

    /*If the object is already out of the parent and its position is changes
     *surely the scrollbars also changes so invalidate them*/
    bool on1 = lv_area_is_లోపల(&ori, &మాతృ_fit_area, 0);
    ఒకవేళ(!on1)
        lv_obj_scrollbar_invalidate(మాతృ);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) == LV_పునాది_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    లేకపోతే {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_కొలత_మారింది, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_లోపల(&obj->coords, &మాతృ_fit_area, 0);
    ఒకవేళ(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(మాతృ);

    lv_obj_refresh_ext_draw_కొలత(obj);

    తిరిగి true;
}

శూన్య lv_obj_సమితి_కొలత(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_సమితి_వెడల్పు(obj, w);
    lv_obj_సమితి_ఎత్తు(obj, h);
}

శూన్య lv_obj_సమితి_వెడల్పు(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    lv_style_res_t res_w;
    lv_style_విలువ_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_వెడల్పు, &v_w, 0);

    ఒకవేళ((res_w == LV_STYLE_RES_దొరికింది && v_w.అంకె != w) || res_w == LV_STYLE_RES_NOT_దొరికింది) {
        lv_obj_సమితి_style_వెడల్పు(obj, w, 0);
    }
}

శూన్య lv_obj_సమితి_ఎత్తు(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    lv_style_res_t res_h;
    lv_style_విలువ_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_ఎత్తు, &v_h, 0);

    ఒకవేళ((res_h == LV_STYLE_RES_దొరికింది && v_h.అంకె != h) || res_h == LV_STYLE_RES_NOT_దొరికింది) {
        lv_obj_సమితి_style_ఎత్తు(obj, h, 0);
    }
}

శూన్య lv_obj_సమితి_content_వెడల్పు(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    int32_t ఎడమ = lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);
    int32_t కుడి = lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);
    lv_obj_సమితి_వెడల్పు(obj, w + ఎడమ + కుడి);
}

శూన్య lv_obj_సమితి_content_ఎత్తు(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    int32_t శిఖరం = lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_అంతర్గత(obj, LV_PART_MAIN);
    lv_obj_సమితి_ఎత్తు(obj, h + శిఖరం + bottom);
}

శూన్య lv_obj_సమితి_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_సమితి_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    ఒకవేళ(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) తిరిగి false;

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ == ఖాళీ) తిరిగి false;

    uint32_t layout = lv_obj_get_style_layout_అంతర్గత(మాతృ, LV_PART_MAIN);
    ఒకవేళ(layout) తిరిగి true;
    లేకపోతే తిరిగి false;
}

శూన్య lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ఖాళీ);
}

శూన్య lv_obj_update_layout(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        తిరిగి;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    అయితే(scr->scr_layout_inv) {
        ఒకవేళ(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_సందేశం(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (శూన్య *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            ఆపు;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, ఖాళీ);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_చివర;
}

శూన్య lv_obj_సమితి_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_సమితి_style_align(obj, align, 0);
}

శూన్య lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    lv_obj_సమితి_style_align(obj, align, 0);
    lv_obj_సమితి_pos(obj, x_ofs, y_ofs);
}

శూన్య lv_obj_align_గమ్యం(lv_obj_t * obj, స్థిరరాశి lv_obj_t * పునాది, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_update_layout(obj);
    ఒకవేళ(పునాది == ఖాళీ) పునాది = lv_obj_get_మాతృ(obj);

    ఒకవేళ(పునాది == ఖాళీ) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        తిరిగి;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ == ఖాళీ) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        తిరిగి;
    }

    int32_t pleft = lv_obj_get_style_space_ఎడమ_అంతర్గత(మాతృ, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_శిఖరం_అంతర్గత(మాతృ, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_ఎడమ_అంతర్గత(పునాది, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_శిఖరం_అంతర్గత(పునాది, LV_PART_MAIN);

    ఒకవేళ(align == LV_ALIGN_DEFAULT) {
        ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(పునాది, LV_PART_MAIN) == LV_పునాది_DIR_RTL) align = LV_ALIGN_శిఖరం_కుడి;
        లేకపోతే align = LV_ALIGN_శిఖరం_ఎడమ;
    }

    ఎంపిక(align) {
        స్థితి LV_ALIGN_CENTER:
            x = lv_obj_get_content_వెడల్పు(పునాది) / 2 - lv_obj_get_వెడల్పు(obj) / 2 + bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) / 2 - lv_obj_get_ఎత్తు(obj) / 2 + btop;
            ఆపు;

        స్థితి LV_ALIGN_శిఖరం_ఎడమ:
            x = bleft;
            y = btop;
            ఆపు;

        స్థితి LV_ALIGN_శిఖరం_MID:
            x = lv_obj_get_content_వెడల్పు(పునాది) / 2 - lv_obj_get_వెడల్పు(obj) / 2 + bleft;
            y = btop;
            ఆపు;

        స్థితి LV_ALIGN_శిఖరం_కుడి:
            x = lv_obj_get_content_వెడల్పు(పునాది) - lv_obj_get_వెడల్పు(obj) + bleft;
            y = btop;
            ఆపు;

        స్థితి LV_ALIGN_BOTTOM_ఎడమ:
            x = bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) - lv_obj_get_ఎత్తు(obj) + btop;
            ఆపు;
        స్థితి LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_వెడల్పు(పునాది) / 2 - lv_obj_get_వెడల్పు(obj) / 2 + bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) - lv_obj_get_ఎత్తు(obj) + btop;
            ఆపు;

        స్థితి LV_ALIGN_BOTTOM_కుడి:
            x = lv_obj_get_content_వెడల్పు(పునాది) - lv_obj_get_వెడల్పు(obj) + bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) - lv_obj_get_ఎత్తు(obj) + btop;
            ఆపు;

        స్థితి LV_ALIGN_ఎడమ_MID:
            x = bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) / 2 - lv_obj_get_ఎత్తు(obj) / 2 + btop;
            ఆపు;

        స్థితి LV_ALIGN_కుడి_MID:
            x = lv_obj_get_content_వెడల్పు(పునాది) - lv_obj_get_వెడల్పు(obj) + bleft;
            y = lv_obj_get_content_ఎత్తు(పునాది) / 2 - lv_obj_get_ఎత్తు(obj) / 2 + btop;
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_శిఖరం_ఎడమ:
            x = 0;
            y = -lv_obj_get_ఎత్తు(obj);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_శిఖరం_MID:
            x = lv_obj_get_వెడల్పు(పునాది) / 2 - lv_obj_get_వెడల్పు(obj) / 2;
            y = -lv_obj_get_ఎత్తు(obj);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_శిఖరం_కుడి:
            x = lv_obj_get_వెడల్పు(పునాది) - lv_obj_get_వెడల్పు(obj);
            y = -lv_obj_get_ఎత్తు(obj);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_BOTTOM_ఎడమ:
            x = 0;
            y = lv_obj_get_ఎత్తు(పునాది);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_BOTTOM_MID:
            x = lv_obj_get_వెడల్పు(పునాది) / 2 - lv_obj_get_వెడల్పు(obj) / 2;
            y = lv_obj_get_ఎత్తు(పునాది);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_BOTTOM_కుడి:
            x = lv_obj_get_వెడల్పు(పునాది) - lv_obj_get_వెడల్పు(obj);
            y = lv_obj_get_ఎత్తు(పునాది);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_ఎడమ_శిఖరం:
            x = -lv_obj_get_వెడల్పు(obj);
            y = 0;
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_ఎడమ_MID:
            x = -lv_obj_get_వెడల్పు(obj);
            y = lv_obj_get_ఎత్తు(పునాది) / 2 - lv_obj_get_ఎత్తు(obj) / 2;
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_ఎడమ_BOTTOM:
            x = -lv_obj_get_వెడల్పు(obj);
            y = lv_obj_get_ఎత్తు(పునాది) - lv_obj_get_ఎత్తు(obj);
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_కుడి_శిఖరం:
            x = lv_obj_get_వెడల్పు(పునాది);
            y = 0;
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_కుడి_MID:
            x = lv_obj_get_వెడల్పు(పునాది);
            y = lv_obj_get_ఎత్తు(పునాది) / 2 - lv_obj_get_ఎత్తు(obj) / 2;
            ఆపు;

        స్థితి LV_ALIGN_బహిర్గతం_కుడి_BOTTOM:
            x = lv_obj_get_వెడల్పు(పునాది);
            y = lv_obj_get_ఎత్తు(పునాది) - lv_obj_get_ఎత్తు(obj);
            ఆపు;

        స్థితి LV_ALIGN_DEFAULT:
            ఆపు;
    }

    ఒకవేళ(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_వెడల్పు(పునాది) * LV_COORD_GET_PCT(x_ofs)) / 100;
    ఒకవేళ(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_ఎత్తు(పునాది) * LV_COORD_GET_PCT(y_ofs)) / 100;
    ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(మాతృ, LV_PART_MAIN) == LV_పునాది_DIR_RTL) {
        x += x_ofs + పునాది->coords.x1 - మాతృ->coords.x1 + lv_obj_get_scroll_కుడి(మాతృ) - pleft;
    }
    లేకపోతే {
        x += x_ofs + పునాది->coords.x1 - మాతృ->coords.x1 + lv_obj_get_scroll_ఎడమ(మాతృ) - pleft;
    }
    y += y_ofs + పునాది->coords.y1 - మాతృ->coords.y1 + lv_obj_get_scroll_శిఖరం(మాతృ) - ptop;
    lv_obj_సమితి_style_align(obj, LV_ALIGN_శిఖరం_ఎడమ, 0);
    lv_obj_సమితి_pos(obj, x, y);

}

శూన్య lv_obj_get_coords(స్థిరరాశి lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != ఖాళీ, తిరిగి);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t rel_x;
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ) {
        rel_x  = obj->coords.x1 - మాతృ->coords.x1;
        rel_x += lv_obj_get_scroll_x(మాతృ);
        rel_x -= lv_obj_get_style_space_ఎడమ_అంతర్గత(మాతృ, LV_PART_MAIN);
    }
    లేకపోతే {
        rel_x = obj->coords.x1;
    }
    తిరిగి rel_x;
}

int32_t lv_obj_get_x2(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_obj_get_x(obj) + lv_obj_get_వెడల్పు(obj);
}

int32_t lv_obj_get_y(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t rel_y;
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ) {
        rel_y = obj->coords.y1 - మాతృ->coords.y1;
        rel_y += lv_obj_get_scroll_y(మాతృ);
        rel_y -= lv_obj_get_style_space_శిఖరం_అంతర్గత(మాతృ, LV_PART_MAIN);
    }
    లేకపోతే {
        rel_y = obj->coords.y1;
    }
    తిరిగి rel_y;
}

int32_t lv_obj_get_y2(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_obj_get_y(obj) + lv_obj_get_ఎత్తు(obj);
}

int32_t lv_obj_get_x_aligned(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_obj_get_style_x_అంతర్గత(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_obj_get_style_y_అంతర్గత(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_వెడల్పు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_area_get_వెడల్పు(&obj->coords);
}

int32_t lv_obj_get_ఎత్తు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    తిరిగి lv_area_get_ఎత్తు(&obj->coords);
}

int32_t lv_obj_get_content_వెడల్పు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t ఎడమ = lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);
    int32_t కుడి = lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);

    తిరిగి lv_obj_get_వెడల్పు(obj) - ఎడమ - కుడి;
}

int32_t lv_obj_get_content_ఎత్తు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t శిఖరం = lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_అంతర్గత(obj, LV_PART_MAIN);

    తిరిగి lv_obj_get_ఎత్తు(obj) - శిఖరం - bottom;
}

శూన్య lv_obj_get_content_coords(స్థిరరాశి lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ఖాళీ, తిరిగి);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_అంతర్గత(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_వెడల్పు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    lv_బిందువు_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_కొలత, &p);
    తిరిగి p.x;
}

int32_t lv_obj_get_self_ఎత్తు(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    lv_బిందువు_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_కొలత, &p);
    తిరిగి p.y;
}

int32_t lv_obj_get_style_clamped_వెడల్పు(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t content_వెడల్పు = -1;
    int32_t w = calc_dynamic_వెడల్పు(obj, LV_STYLE_వెడల్పు, &content_వెడల్పు);
    int32_t minw = calc_dynamic_వెడల్పు(obj, LV_STYLE_MIN_వెడల్పు, &content_వెడల్పు);
    int32_t maxw = calc_dynamic_వెడల్పు(obj, LV_STYLE_MAX_వెడల్పు, &content_వెడల్పు);
    ఒకవేళ(w <= minw) {
        w = lv_obj_get_style_min_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    }
    లేకపోతే ఒకవేళ(w >= maxw) {
        w = lv_obj_get_style_max_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    }
    లేకపోతే {
        w = lv_obj_get_style_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    }
    తిరిగి w;
}

int32_t lv_obj_get_style_clamped_ఎత్తు(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t content_ఎత్తు = -1;
    int32_t h = calc_dynamic_ఎత్తు(obj, LV_STYLE_ఎత్తు, &content_ఎత్తు);
    int32_t minh = calc_dynamic_ఎత్తు(obj, LV_STYLE_MIN_ఎత్తు, &content_ఎత్తు);
    int32_t maxh = calc_dynamic_ఎత్తు(obj, LV_STYLE_MAX_ఎత్తు, &content_ఎత్తు);
    ఒకవేళ(h <= minh) {
        h = lv_obj_get_style_min_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    }
    లేకపోతే ఒకవేళ(h >= maxh) {
        h = lv_obj_get_style_max_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    }
    లేకపోతే {
        h = lv_obj_get_style_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    }
    తిరిగి h;
}

bool lv_obj_is_style_any_వెడల్పు_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t w = lv_obj_get_style_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_వెడల్పు_అంతర్గత(obj, LV_PART_MAIN);
    తిరిగి (w == LV_కొలత_CONTENT || minw == LV_కొలత_CONTENT || maxw == LV_కొలత_CONTENT);
}

bool lv_obj_is_style_any_ఎత్తు_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t h = lv_obj_get_style_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_ఎత్తు_అంతర్గత(obj, LV_PART_MAIN);
    తిరిగి (h == LV_కొలత_CONTENT || minh == LV_కొలత_CONTENT || maxh == LV_కొలత_CONTENT);
}

bool lv_obj_is_వెడల్పు_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t minw = lv_obj_calc_dynamic_వెడల్పు(obj, LV_STYLE_MIN_వెడల్పు);
    int32_t w = lv_obj_get_వెడల్పు(obj);
    తిరిగి w == minw;
}

bool lv_obj_is_ఎత్తు_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t minh = lv_obj_calc_dynamic_ఎత్తు(obj, LV_STYLE_MIN_ఎత్తు);
    int32_t h = lv_obj_get_ఎత్తు(obj);
    తిరిగి h == minh;
}

bool lv_obj_is_వెడల్పు_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t maxw = lv_obj_calc_dynamic_వెడల్పు(obj, LV_STYLE_MAX_వెడల్పు);
    int32_t w = lv_obj_get_వెడల్పు(obj);
    తిరిగి w == maxw;
}

bool lv_obj_is_ఎత్తు_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    int32_t maxh = lv_obj_calc_dynamic_ఎత్తు(obj, LV_STYLE_MAX_ఎత్తు);
    int32_t h = lv_obj_get_ఎత్తు(obj);
    తిరిగి h == maxh;
}

bool lv_obj_refresh_self_కొలత(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    ఒకవేళ(!lv_obj_is_style_any_వెడల్పు_content(obj) && !lv_obj_is_style_any_ఎత్తు_content(obj))
        తిరిగి false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ != ఖాళీ) {
        మాతృ->w_layout = 0;
        మాతృ->h_layout = 0;
        lv_obj_mark_layout_as_dirty(మాతృ);
    }
    lv_obj_mark_layout_as_dirty(obj);
    తిరిగి true;
}

శూన్య lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(lv_obj_is_layout_positioned(obj)) తిరిగి;

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    int32_t x = lv_obj_get_style_x_అంతర్గత(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_అంతర్గత(obj, LV_PART_MAIN);

    ఒకవేళ(మాతృ == ఖాళీ) {
        lv_obj_move_గమ్యం(obj, x, y);
        తిరిగి;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_వెడల్పు(మాతృ);
    int32_t ph = lv_obj_get_content_ఎత్తు(మాతృ);
    ఒకవేళ(LV_COORD_IS_PCT(x)) {
        ఒకవేళ(lv_obj_get_style_వెడల్పు_అంతర్గత(మాతృ, LV_PART_MAIN) == LV_కొలత_CONTENT) x = 0; /*Avoid circular dependency*/
        లేకపోతే x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    ఒకవేళ(LV_COORD_IS_PCT(y)) {
        ఒకవేళ(lv_obj_get_style_ఎత్తు_అంతర్గత(మాతృ, LV_PART_MAIN) == LV_కొలత_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_అంతర్గత(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_అంతర్గత(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_వెడల్పు(obj);
    int32_t h = lv_obj_get_ఎత్తు(obj);
    ఒకవేళ(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    ఒకవేళ(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_అంతర్గత(obj, LV_PART_MAIN);

    ఒకవేళ(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_శిఖరం_ఎడమ;
    }

    bool rtl = lv_obj_get_style_పునాది_dir_అంతర్గత(మాతృ, LV_PART_MAIN) == LV_పునాది_DIR_RTL;

    ఒకవేళ(rtl) {
        ఎంపిక(align) {
            స్థితి LV_ALIGN_శిఖరం_ఎడమ:
                align = LV_ALIGN_శిఖరం_కుడి;
                ఆపు;
            స్థితి LV_ALIGN_శిఖరం_కుడి:
                align = LV_ALIGN_శిఖరం_ఎడమ;
                ఆపు;
            స్థితి LV_ALIGN_ఎడమ_MID:
                align = LV_ALIGN_కుడి_MID;
                ఆపు;
            స్థితి LV_ALIGN_కుడి_MID:
                align = LV_ALIGN_ఎడమ_MID;
                ఆపు;
            స్థితి LV_ALIGN_BOTTOM_ఎడమ:
                align = LV_ALIGN_BOTTOM_కుడి;
                ఆపు;
            స్థితి LV_ALIGN_BOTTOM_కుడి:
                align = LV_ALIGN_BOTTOM_ఎడమ;
                ఆపు;
            అప్రమేయ:
                ఆపు;
        }
    }

    ఎంపిక(align) {
        స్థితి LV_ALIGN_శిఖరం_ఎడమ:
            ఆపు;
        స్థితి LV_ALIGN_శిఖరం_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            ఆపు;
        స్థితి LV_ALIGN_శిఖరం_కుడి:
            x = rtl ? pw - w - x : pw - w + x;
            ఆపు;
        స్థితి LV_ALIGN_ఎడమ_MID:
            y += ph / 2 - h / 2;
            ఆపు;
        స్థితి LV_ALIGN_BOTTOM_ఎడమ:
            y += ph - h;
            ఆపు;
        స్థితి LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            ఆపు;
        స్థితి LV_ALIGN_BOTTOM_కుడి:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            ఆపు;
        స్థితి LV_ALIGN_కుడి_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            ఆపు;
        స్థితి LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            ఆపు;
        అప్రమేయ:
            ఆపు;
    }

    lv_obj_move_గమ్యం(obj, x, y);
}

శూన్య lv_obj_move_గమ్యం(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * మాతృ = obj->మాతృ;

    ఒకవేళ(మాతృ) {
        ఒకవేళ(lv_obj_is_floating(obj)) {
            x += మాతృ->coords.x1;
            y += మాతృ->coords.y1;
        }
        లేకపోతే {
            x += మాతృ->coords.x1 - lv_obj_get_scroll_x(మాతృ);
            y += మాతృ->coords.y1 - lv_obj_get_scroll_y(మాతృ);
        }

        x += lv_obj_get_style_space_ఎడమ_అంతర్గత(మాతృ, LV_PART_MAIN);
        y += lv_obj_get_style_space_శిఖరం_అంతర్గత(మాతృ, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_బిందువు_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    ఒకవేళ(diff.x == 0 && diff.y == 0) తిరిగి;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t మాతృ_fit_area;
    bool on1 = false;
    ఒకవేళ(మాతృ) {
        lv_obj_get_content_coords(మాతృ, &మాతృ_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_లోపల(&ori, &మాతృ_fit_area, 0);
        ఒకవేళ(!on1) lv_obj_scrollbar_invalidate(మాతృ);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    ఒకవేళ(మాతృ) lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    ఒకవేళ(మాతృ) {
        bool on2 = lv_area_is_లోపల(&obj->coords, &మాతృ_fit_area, 0);
        ఒకవేళ(!on1 || !on2) lv_obj_scrollbar_invalidate(మాతృ);
    }
}

శూన్య lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        ఒకవేళ(ignore_floating && lv_obj_is_floating(బిడ్డ)) కొనసాగు;
        బిడ్డ->coords.x1 += x_diff;
        బిడ్డ->coords.y1 += y_diff;
        బిడ్డ->coords.x2 += x_diff;
        బిడ్డ->coords.y2 += y_diff;

        lv_obj_move_children_by(బిడ్డ, x_diff, y_diff, false);
    }
}

శూన్య lv_obj_transform_బిందువు(స్థిరరాశి lv_obj_t * obj, lv_బిందువు_t * p, lv_obj_బిందువు_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(p != ఖాళీ, తిరిగి);

    lv_obj_transform_బిందువు_array(obj, p, 1, flags);
}

శూన్య lv_obj_transform_బిందువు_array(స్థిరరాశి lv_obj_t * obj, lv_బిందువు_t points[], కొలత_t లెక్క,
                                  lv_obj_బిందువు_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(points != ఖాళీ || లెక్క == 0, తిరిగి);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_బిందువు_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_బిందువు_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(inverse) {
        ఒకవేళ(recursive && మాతృ) lv_obj_transform_బిందువు_array(మాతృ, points, లెక్క, flags);
        ఒకవేళ(do_tranf) transform_బిందువు_array(obj, points, లెక్క, inverse);
    }
    లేకపోతే {
        ఒకవేళ(do_tranf) transform_బిందువు_array(obj, points, లెక్క, inverse);
        ఒకవేళ(recursive && మాతృ) lv_obj_transform_బిందువు_array(మాతృ, points, లెక్క, flags);
    }
}

శూన్య lv_obj_get_transformed_area(స్థిరరాశి lv_obj_t * obj, lv_area_t * area, lv_obj_బిందువు_transform_flag_t flags)
{
    LV_CHECK_ARG(area != ఖాళీ, తిరిగి);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_బిందువు_t p[4] = {
        {area->x1, area->y1},
        {area->x1, area->y2 + 1},
        {area->x2 + 1, area->y1},
        {area->x2 + 1, area->y2 + 1},
    };

    lv_obj_transform_బిందువు_array(obj, p, 4, flags);

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

స్థిర lv_obj_వృక్షం_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, శూన్య * user_దత్తాంశం)
{
    ఒకవేళ(!obj->has_blur) తిరిగి LV_OBJ_వృక్షం_WALK_తదుపరి;

    /*invalidate_area_core() expects untransformed coordinates and applies the
     *transform itself*/
    lv_area_t obj_coords;
    int32_t ext_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_కొలత, ext_కొలత);

    /*The invalidated areas are in screen coordinates so check the overlap
     *against the transformed area*/
    lv_area_t scr_coords;
    scr_coords = obj_coords;
    ఒకవేళ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_బిందువు_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_దత్తాంశం;
    uint32_t i;
    కొరకు(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        ఒకవేళ(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            తిరిగి LV_OBJ_వృక్షం_WALK_SKIP_CHILDREN;
        }
    }

    తిరిగి LV_OBJ_వృక్షం_WALK_తదుపరి;
}

శూన్య lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    ఒకవేళ(disp->inv_p == 0) తిరిగి;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    ఒకవేళ(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) తిరిగి;

    uint32_t prev_inv_p;
    చేయి {
        prev_inv_p = disp->inv_p;

        lv_obj_వృక్షం_walk(disp->act_scr, blur_expand_walk_cb, disp);
        ఒకవేళ(disp->prev_scr) lv_obj_వృక్షం_walk(disp->prev_scr, blur_expand_walk_cb, disp);
        lv_obj_వృక్షం_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_వృక్షం_walk(disp->శిఖరం_layer, blur_expand_walk_cb, disp);
        lv_obj_వృక్షం_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } అయితే(disp->inv_p > prev_inv_p);
}

lv_ఫలితం_t lv_obj_invalidate_area(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_area_t * area)
{
    LV_CHECK_ARG(area != ఖాళీ, తిరిగి LV_ఫలితం_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_ఫలితం_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    ఒకవేళ(!lv_display_is_invalidation_enabled(disp)) తిరిగి LV_ఫలితం_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    ఒకవేళ(obj->has_blur) తిరిగి lv_obj_invalidate(obj);
    లేకపోతే తిరిగి obj_invalidate_area_అంతర్గత(obj, area);
}


lv_ఫలితం_t lv_obj_invalidate(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_ఫలితం_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    ఒకవేళ(!lv_display_is_invalidation_enabled(disp)) తిరిగి LV_ఫలితం_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_కొలత;
    obj_coords.y1 -= ext_కొలత;
    obj_coords.x2 += ext_కొలత;
    obj_coords.y2 += ext_కొలత;

    lv_ఫలితం_t res = obj_invalidate_area_అంతర్గత(obj, &obj_coords);

    తిరిగి res;
}

bool lv_obj_area_is_visible(స్థిరరాశి lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ఖాళీ, తిరిగి false);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    ఒకవేళ(lv_obj_is_hidden(obj)) తిరిగి false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    ఒకవేళ(obj_scr != lv_display_get_screen_చురుకైన(disp) &&
       obj_scr != lv_display_get_screen_prev(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_శిఖరం(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        తిరిగి false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_కొలత, ext_కొలత);

    /*The area is not on the object*/
    ఒకవేళ(!lv_area_intersect(area, area, &obj_coords)) తిరిగి false;

    ఒకవేళ(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_బిందువు_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    అయితే(మాతృ != ఖాళీ) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        ఒకవేళ(lv_obj_is_hidden(మాతృ)) తిరిగి false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t మాతృ_coords = మాతృ->coords;
        ఒకవేళ(lv_obj_is_అధికప్రవాహం_visible(మాతృ)) {
            int32_t మాతృ_ext_కొలత = lv_obj_get_ext_draw_కొలత(మాతృ);
            lv_area_increase(&మాతృ_coords, మాతృ_ext_కొలత, మాతృ_ext_కొలత);
        }

        ఒకవేళ(is_transformed(మాతృ)) {
            lv_obj_get_transformed_area(మాతృ, &మాతృ_coords, LV_OBJ_బిందువు_TRANSFORM_FLAG_RECURSIVE);
        }
        ఒకవేళ(!lv_area_intersect(area, area, &మాతృ_coords)) తిరిగి false;

        మాతృ = lv_obj_get_మాతృ(మాతృ);
    }

    తిరిగి true;
}

bool lv_obj_is_visible(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    lv_area_t obj_coords;
    int32_t ext_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_కొలత;
    obj_coords.y1 -= ext_కొలత;
    obj_coords.x2 += ext_కొలత;
    obj_coords.y2 += ext_కొలత;

    తిరిగి lv_obj_area_is_visible(obj, &obj_coords);
}

శూన్య lv_obj_సమితి_ext_click_area(lv_obj_t * obj, int32_t కొలత)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }
    obj->spec_attr->ext_click_pad = కొలత;
}

శూన్య lv_obj_get_click_area(స్థిరరాశి lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != ఖాళీ, తిరిగి);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    *area = obj->coords;
    ఒకవేళ(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_తగిలింది_test(lv_obj_t * obj, స్థిరరాశి lv_బిందువు_t * బిందువు)
{
    LV_CHECK_ARG(బిందువు != ఖాళీ, తిరిగి false);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    ఒకవేళ(!lv_obj_is_clickable(obj)) తిరిగి false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_బిందువు_on(&a, బిందువు, 0);
    ఒకవేళ(res == false) తిరిగి false;

    ఒకవేళ(lv_obj_is_adv_hittest(obj)) {
        lv_తగిలింది_test_info_t తగిలింది_info;
        తగిలింది_info.బిందువు = బిందువు;
        తగిలింది_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_తగిలింది_TEST, &తగిలింది_info);
        తిరిగి తగిలింది_info.res;
    }

    తిరిగి res;
}

int32_t lv_clamp_వెడల్పు(int32_t వెడల్పు, int32_t min_వెడల్పు, int32_t max_వెడల్పు, int32_t ref_వెడల్పు)
{
    ఒకవేళ(LV_COORD_IS_PCT(min_వెడల్పు)) min_వెడల్పు = (ref_వెడల్పు * LV_COORD_GET_PCT(min_వెడల్పు)) / 100;
    ఒకవేళ(LV_COORD_IS_PCT(max_వెడల్పు)) max_వెడల్పు = (ref_వెడల్పు * LV_COORD_GET_PCT(max_వెడల్పు)) / 100;
    తిరిగి LV_CLAMP(min_వెడల్పు, వెడల్పు, max_వెడల్పు);
}

int32_t lv_clamp_ఎత్తు(int32_t ఎత్తు, int32_t min_ఎత్తు, int32_t max_ఎత్తు, int32_t ref_ఎత్తు)
{
    ఒకవేళ(LV_COORD_IS_PCT(min_ఎత్తు)) min_ఎత్తు = (ref_ఎత్తు * LV_COORD_GET_PCT(min_ఎత్తు)) / 100;
    ఒకవేళ(LV_COORD_IS_PCT(max_ఎత్తు)) max_ఎత్తు = (ref_ఎత్తు * LV_COORD_GET_PCT(max_ఎత్తు)) / 100;
    తిరిగి LV_CLAMP(min_ఎత్తు, ఎత్తు, max_ఎత్తు);
}

శూన్య lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

శూన్య lv_obj_సమితి_transform(lv_obj_t * obj, స్థిరరాశి lv_మాతృక_t * మాతృక)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    ఒకవేళ(!మాతృక) {
        lv_obj_reset_transform(obj);
        తిరిగి;
    }

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }

    ఒకవేళ(!obj->spec_attr->మాతృక) {
        obj->spec_attr->మాతృక = lv_malloc(పరిమాణం(lv_మాతృక_t));
        LV_ASSERT_MALLOC(obj->spec_attr->మాతృక);
        ఒకవేళ(obj->spec_attr->మాతృక == ఖాళీ) తిరిగి;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Copy the matrix */
    *obj->spec_attr->మాతృక = *మాతృక;

    /* Matrix is set. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#లేకపోతే
    LV_వాడని(obj);
    LV_వాడని(మాతృక);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#అయితే_ముగింపు
}

శూన్య lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    ఒకవేళ(!obj->spec_attr) {
        తిరిగి;
    }

    ఒకవేళ(!obj->spec_attr->మాతృక) {
        తిరిగి;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->మాతృక);
    obj->spec_attr->మాతృక = ఖాళీ;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#లేకపోతే
    LV_వాడని(obj);
#అయితే_ముగింపు
}

స్థిరరాశి lv_మాతృక_t * lv_obj_get_transform(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    ఒకవేళ(obj->spec_attr) {
        తిరిగి obj->spec_attr->మాతృక;
    }
#లేకపోతే
    LV_వాడని(obj);
#అయితే_ముగింపు
    తిరిగి ఖాళీ;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర lv_ఫలితం_t obj_invalidate_area_అంతర్గత(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    తిరిగి invalidate_area_core(obj, &area_tmp);
}

స్థిర bool is_transformed(స్థిరరాశి lv_obj_t * obj)
{
    అయితే(obj) {
        ఒకవేళ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) తిరిగి true;
        obj = obj->మాతృ;
    }
    తిరిగి false;
}

స్థిర int32_t calc_content_వెడల్పు(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    ఒకవేళ(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_కుడి = lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);
    int32_t space_ఎడమ = lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_వెడల్పు(obj) + space_ఎడమ + space_కుడి;

    int32_t బిడ్డ_res = LV_COORD_MIN;

    ఒకవేళ(!lv_layout_get_min_కొలత(obj, &బిడ్డ_res, true)) {
        uint32_t i;
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
        /*With RTL find the left most coordinate*/
        ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) == LV_పునాది_DIR_RTL) {
            కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                int32_t బిడ్డ_res_tmp = LV_COORD_MIN;
                lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                ఒకవేళ(బిడ్డ->hidden || బిడ్డ->floating)
                    కొనసాగు;

                ఒకవేళ(బిడ్డ->w_ignore_కొలత)
                    కొనసాగు;

                ఒకవేళ(!lv_obj_is_layout_positioned(బిడ్డ)) {
                    lv_align_t align = lv_obj_get_style_align_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                    ఎంపిక(align) {
                        స్థితి LV_ALIGN_DEFAULT:
                        స్థితి LV_ALIGN_శిఖరం_కుడి:
                        స్థితి LV_ALIGN_BOTTOM_కుడి:
                        స్థితి LV_ALIGN_కుడి_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            బిడ్డ_res_tmp = obj->coords.x2 - బిడ్డ->coords.x1 + 1;
                            ఆపు;
                        అప్రమేయ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ఒకవేళ(lv_obj_get_style_x_అంతర్గత(బిడ్డ, LV_PART_MAIN) == 0) {
                                బిడ్డ_res_tmp = lv_area_get_వెడల్పు(&బిడ్డ->coords) + space_కుడి;
                                బిడ్డ_res_tmp += lv_obj_get_style_margin_ఎడమ_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                            }
                            ఆపు;
                    }
                }
                లేకపోతే {
                    బిడ్డ_res_tmp = obj->coords.x2 - బిడ్డ->coords.x1 + 1;
                }
                బిడ్డ_res = LV_MAX(బిడ్డ_res, బిడ్డ_res_tmp + lv_obj_get_style_margin_ఎడమ_అంతర్గత(బిడ్డ, LV_PART_MAIN));
            }
            ఒకవేళ(బిడ్డ_res != LV_COORD_MIN) {
                బిడ్డ_res += space_ఎడమ;
            }
        }
        /*Else find the right most coordinate*/
        లేకపోతే {
            కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                int32_t బిడ్డ_res_tmp = LV_COORD_MIN;
                lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                ఒకవేళ(బిడ్డ->hidden || బిడ్డ->floating) కొనసాగు;

                ఒకవేళ(బిడ్డ->w_ignore_కొలత)
                    కొనసాగు;

                ఒకవేళ(!lv_obj_is_layout_positioned(బిడ్డ)) {
                    lv_align_t align = lv_obj_get_style_align_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                    ఎంపిక(align) {
                        స్థితి LV_ALIGN_DEFAULT:
                        స్థితి LV_ALIGN_శిఖరం_ఎడమ:
                        స్థితి LV_ALIGN_BOTTOM_ఎడమ:
                        స్థితి LV_ALIGN_ఎడమ_MID:
                            /*Normal left aligns.*/
                            బిడ్డ_res_tmp = బిడ్డ->coords.x2 - obj->coords.x1 + 1;
                            ఆపు;
                        అప్రమేయ:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            ఒకవేళ(lv_obj_get_style_x_అంతర్గత(బిడ్డ, LV_PART_MAIN) == 0) {
                                బిడ్డ_res_tmp = lv_area_get_వెడల్పు(&బిడ్డ->coords) + space_ఎడమ;
                                బిడ్డ_res_tmp += lv_obj_get_style_margin_కుడి_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                            }
                            ఆపు;
                    }
                }
                లేకపోతే {
                    బిడ్డ_res_tmp = బిడ్డ->coords.x2 - obj->coords.x1 + 1;
                }

                బిడ్డ_res = LV_MAX(బిడ్డ_res, బిడ్డ_res_tmp + lv_obj_get_style_margin_కుడి_అంతర్గత(బిడ్డ, LV_PART_MAIN));
            }

            ఒకవేళ(బిడ్డ_res != LV_COORD_MIN) {
                బిడ్డ_res += space_కుడి;
            }
        }
    }

    ఒకవేళ(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    ఒకవేళ(బిడ్డ_res == LV_COORD_MIN) {
        తిరిగి self_w;
    }

    తిరిగి LV_MAX(బిడ్డ_res, self_w);
}

స్థిర int32_t calc_content_ఎత్తు(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    ఒకవేళ(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_శిఖరం = lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_అంతర్గత(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_ఎత్తు(obj) + space_శిఖరం + space_bottom;

    int32_t బిడ్డ_res = LV_COORD_MIN;

    ఒకవేళ(!lv_layout_get_min_కొలత(obj, &బిడ్డ_res, false)) {
        uint32_t i;
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
        కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
            int32_t బిడ్డ_res_tmp = LV_COORD_MIN;
            lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
            ఒకవేళ(బిడ్డ->hidden || బిడ్డ->floating) కొనసాగు;

            ఒకవేళ(బిడ్డ->h_ignore_కొలత)
                కొనసాగు;

            ఒకవేళ(!lv_obj_is_layout_positioned(బిడ్డ)) {
                lv_align_t align = lv_obj_get_style_align_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                ఎంపిక(align) {
                    స్థితి LV_ALIGN_DEFAULT:
                    స్థితి LV_ALIGN_శిఖరం_కుడి:
                    స్థితి LV_ALIGN_శిఖరం_MID:
                    స్థితి LV_ALIGN_శిఖరం_ఎడమ:
                        /*Normal top aligns. */
                        బిడ్డ_res_tmp = బిడ్డ->coords.y2 - obj->coords.y1 + 1;
                        ఆపు;
                    అప్రమేయ:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        ఒకవేళ(lv_obj_get_style_y_అంతర్గత(బిడ్డ, LV_PART_MAIN) == 0) {
                            బిడ్డ_res_tmp = lv_area_get_ఎత్తు(&బిడ్డ->coords) + space_శిఖరం;
                            బిడ్డ_res_tmp += lv_obj_get_style_margin_శిఖరం_అంతర్గత(బిడ్డ, LV_PART_MAIN);
                        }
                        ఆపు;
                }
            }
            లేకపోతే {
                బిడ్డ_res_tmp = బిడ్డ->coords.y2 - obj->coords.y1 + 1;
            }

            బిడ్డ_res = LV_MAX(బిడ్డ_res, బిడ్డ_res_tmp + lv_obj_get_style_margin_bottom_అంతర్గత(బిడ్డ, LV_PART_MAIN));
        }

        ఒకవేళ(బిడ్డ_res != LV_COORD_MIN) {
            బిడ్డ_res += space_bottom;
        }
    }

    ఒకవేళ(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    ఒకవేళ(బిడ్డ_res == LV_COORD_MIN) {
        తిరిగి self_h;
    }

    తిరిగి LV_MAX(self_h, బిడ్డ_res);
}

స్థిర శూన్య layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        layout_update_core(బిడ్డ);
    }

    ఒకవేళ(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_కొలత(obj);
        lv_obj_refr_pos(obj);

        ఒకవేళ(బిడ్డ_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    ఒకవేళ(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

స్థిర శూన్య transform_బిందువు_array(స్థిరరాశి lv_obj_t * obj, lv_బిందువు_t * p, కొలత_t p_లెక్క, bool inv)
{
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    స్థిరరాశి lv_మాతృక_t * obj_మాతృక = lv_obj_get_transform(obj);
    ఒకవేళ(obj_మాతృక) {
        lv_మాతృక_t m;
        lv_మాతృక_తటస్థం(&m);
        lv_మాతృక_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_మాతృక_multiply(&m, obj_మాతృక);
        lv_మాతృక_translate(&m, -obj->coords.x1, -obj->coords.y1);

        ఒకవేళ(inv) {
            lv_మాతృక_t inv_m;
            lv_మాతృక_inverse(&inv_m, &m);
            m = inv_m;
        }

        కొరకు(కొలత_t i = 0; i < p_లెక్క; i++) {
            lv_బిందువు_precise_t p_precise = lv_బిందువు_గమ్యం_precise(&p[i]);
            lv_బిందువు_precise_t res = lv_మాతృక_transform_precise_బిందువు(&m, &p_precise);
            p[i] = lv_బిందువు_from_precise(&res);
        }

        తిరిగి;
    }
#అయితే_ముగింపు /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_అంతర్గత(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_సురక్షిత_అంతర్గత(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_సురక్షిత_అంతర్గత(obj, LV_PART_MAIN);
    ఒకవేళ(scale_x == 0) scale_x = 1;
    ఒకవేళ(scale_y == 0) scale_y = 1;

    ఒకవేళ(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) తిరిగి;

    lv_బిందువు_t ఇరుసు = {
        .x = lv_obj_get_style_transform_ఇరుసు_x_అంతర్గత(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ఇరుసు_y_అంతర్గత(obj, LV_PART_MAIN)
    };

    ఒకవేళ(LV_COORD_IS_PCT(ఇరుసు.x)) {
        ఇరుసు.x = (LV_COORD_GET_PCT(ఇరుసు.x) * lv_area_get_వెడల్పు(&obj->coords)) / 100;
    }
    ఒకవేళ(LV_COORD_IS_PCT(ఇరుసు.y)) {
        ఇరుసు.y = (LV_COORD_GET_PCT(ఇరుసు.y) * lv_area_get_ఎత్తు(&obj->coords)) / 100;
    }

    ఇరుసు.x = obj->coords.x1 + ఇరుసు.x;
    ఇరుసు.y = obj->coords.y1 + ఇరుసు.y;

    ఒకవేళ(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_బిందువు_array_transform(p, p_లెక్క, angle, scale_x, scale_y, &ఇరుసు, !inv);
}

స్థిర lv_ఫలితం_t invalidate_area_core(స్థిరరాశి lv_obj_t * obj, lv_area_t * area_tmp)
{
    ఒకవేళ(!lv_obj_area_is_visible(obj, area_tmp)) తిరిగి LV_ఫలితం_INVALID;
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#లేకపోతే
    ఒకవేళ(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#అయితే_ముగింపు

    lv_ఫలితం_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    తిరిగి res;
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
స్థిర int32_t calc_dynamic_వెడల్పు(lv_obj_t * obj, lv_style_prop_t prop, int32_t * స్థిరరాశి content_వెడల్పు)
{
    LV_ASSERT(prop == LV_STYLE_వెడల్పు || prop == LV_STYLE_MIN_వెడల్పు || prop == LV_STYLE_MAX_వెడల్పు);

    int32_t వెడల్పు = lv_obj_get_style_prop(obj, 0, prop).అంకె;

    ఒకవేళ(వెడల్పు == LV_కొలత_CONTENT) {
        ఒకవేళ(content_వెడల్పు == ఖాళీ) {
            వెడల్పు = calc_content_వెడల్పు(obj);
        }
        లేకపోతే {
            ఒకవేళ(*content_వెడల్పు < 0) {
                *content_వెడల్పు = calc_content_వెడల్పు(obj);
            }
            వెడల్పు = *content_వెడల్పు;
        }
    }
    లేకపోతే ఒకవేళ(LV_COORD_IS_PCT(వెడల్పు)) {
        lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
        int32_t మాతృ_w = lv_obj_get_content_వెడల్పు(మాతృ);
        వెడల్పు = (LV_COORD_GET_PCT(వెడల్పు) * మాతృ_w) / 100;
        వెడల్పు -= lv_obj_get_style_margin_ఎడమ_అంతర్గత(obj, LV_PART_MAIN) + lv_obj_get_style_margin_కుడి_అంతర్గత(obj,
                                                                                                                   LV_PART_MAIN);
    }
    తిరిగి వెడల్పు;
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
స్థిర int32_t calc_dynamic_ఎత్తు(lv_obj_t * obj, lv_style_prop_t prop, int32_t * స్థిరరాశి content_ఎత్తు)
{
    LV_ASSERT(prop == LV_STYLE_ఎత్తు || prop == LV_STYLE_MIN_ఎత్తు || prop == LV_STYLE_MAX_ఎత్తు);

    int32_t ఎత్తు = lv_obj_get_style_prop(obj, 0, prop).అంకె;

    ఒకవేళ(ఎత్తు == LV_కొలత_CONTENT) {
        ఒకవేళ(content_ఎత్తు == ఖాళీ) {
            ఎత్తు = calc_content_ఎత్తు(obj);
        }
        లేకపోతే {
            ఒకవేళ(*content_ఎత్తు < 0) {
                *content_ఎత్తు = calc_content_ఎత్తు(obj);
            }
            ఎత్తు = *content_ఎత్తు;
        }
    }
    లేకపోతే ఒకవేళ(LV_COORD_IS_PCT(ఎత్తు)) {
        lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
        int32_t మాతృ_h = lv_obj_get_content_ఎత్తు(మాతృ);
        ఎత్తు = (LV_COORD_GET_PCT(ఎత్తు) * మాతృ_h) / 100;
        ఎత్తు -= lv_obj_get_style_margin_శిఖరం_అంతర్గత(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_అంతర్గత(obj,
                                                                                                                    LV_PART_MAIN);
    }
    తిరిగి ఎత్తు;
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
స్థిర bool కొలత_లోపల_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t కొలత_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    ఒకవేళ(min > max) తిరిగి LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    ఒకవేళ(unclamped < min) తిరిగి LV_COORD_IS_PCT(min_style);
    ఒకవేళ(unclamped > max) తిరిగి LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(కొలత_style);
    ఒకవేళ(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    ఒకవేళ(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    తిరిగి is_pct;
}
