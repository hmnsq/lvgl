/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../misc/lv_area_private.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "../layouts/lv_layout_private.h"
#अन्तर्भाव "lv_obj_event_private.h"
#अन्तर्भाव "lv_obj_draw_private.h"
#अन्तर्भाव "lv_obj_style_private.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "../display/lv_display_private.h"
#अन्तर्भाव "lv_refr_private.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "lv_obj_class_private.h"
#अन्तर्भाव "lv_obj_style_internal.h"
#अन्तर्भाव "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#परिभाषय MY_CLASS (&lv_obj_class)
#परिभाषय update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#यदि_अपरिभाषितम् LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #परिभाषय LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#यदि_अन्तः

#यदि LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #दोषः "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#यदि_अन्तः

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
स्थिर int32_t calc_content_विस्तारः(lv_obj_t * obj);
स्थिर int32_t calc_content_height(lv_obj_t * obj);
स्थिर शून्य layout_update_core(lv_obj_t * obj);
स्थिर शून्य transform_point_array(ध्रुव lv_obj_t * obj, lv_point_t * p, आकार_t p_गणनम्, bool inv);
स्थिर bool is_transformed(ध्रुव lv_obj_t * obj);
स्थिर lv_परिणाम_t invalidate_area_core(ध्रुव lv_obj_t * obj, lv_area_t * area_tmp);
स्थिर lv_परिणाम_t obj_invalidate_area_आन्तरिकम्(ध्रुव lv_obj_t * obj, ध्रुव lv_area_t * area);
स्थिर int32_t calc_dynamic_विस्तारः(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_विस्तारः);
स्थिर int32_t calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_height);
स्थिर bool आकार_अन्तर्_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t आकार_style,
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

शून्य lv_obj_समुच्चयः_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_समुच्चयः_x(obj, x);
    lv_obj_समुच्चयः_y(obj, y);
}

शून्य lv_obj_समुच्चयः_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_style_res_t res_x;
    lv_style_मानम्_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    यदि((res_x == LV_STYLE_RES_प्राप्तम् && v_x.अङ्कः != x) || res_x == LV_STYLE_RES_NOT_प्राप्तम्) {
        lv_obj_समुच्चयः_style_x(obj, x, 0);
    }
}

शून्य lv_obj_समुच्चयः_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_style_res_t res_y;
    lv_style_मानम्_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    यदि((res_y == LV_STYLE_RES_प्राप्तम् && v_y.अङ्कः != y) || res_y == LV_STYLE_RES_NOT_प्राप्तम्) {
        lv_obj_समुच्चयः_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_विस्तारः(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);
    LV_CHECK_ARG(prop == LV_STYLE_विस्तारः || prop == LV_STYLE_MIN_विस्तारः || prop == LV_STYLE_MAX_विस्तारः, प्रत्यागम 0);

    प्रत्यागम calc_dynamic_विस्तारः(obj, prop, रिक्त);
}

int32_t lv_obj_calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);
    LV_CHECK_ARG(prop == LV_STYLE_HEIGHT || prop == LV_STYLE_MIN_HEIGHT || prop == LV_STYLE_MAX_HEIGHT, प्रत्यागम 0);

    प्रत्यागम calc_dynamic_height(obj, prop, रिक्त);
}

bool lv_obj_refr_आकार(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    /*If the width or height is set by a layout do not modify them*/
    यदि(obj->w_layout && obj->h_layout) प्रत्यागम false;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent == रिक्त) प्रत्यागम false;

    int32_t w;
    यदि(obj->w_layout) {
        w = lv_obj_get_विस्तारः(obj);
    }
    अन्यथा {
        int32_t content_विस्तारः = -1;
        int32_t unclamped_w = calc_dynamic_विस्तारः(obj, LV_STYLE_विस्तारः, &content_विस्तारः);
        int32_t minw = calc_dynamic_विस्तारः(obj, LV_STYLE_MIN_विस्तारः, &content_विस्तारः);
        int32_t maxw = calc_dynamic_विस्तारः(obj, LV_STYLE_MAX_विस्तारः, &content_विस्तारः);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = आकार_अन्तर्_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN));
        obj->w_ignore_आकार =
            (w_pct && parent->w_layout == 0 && lv_obj_get_style_विस्तारः_आन्तरिकम्(parent, 0) == LV_आकार_CONTENT);
    }

    int32_t h;
    यदि(obj->h_layout) {
        h = lv_obj_get_height(obj);
    }
    अन्यथा {
        int32_t content_height = -1;
        int32_t unclamped_h = calc_dynamic_height(obj, LV_STYLE_HEIGHT, &content_height);
        int32_t minh = calc_dynamic_height(obj, LV_STYLE_MIN_HEIGHT, &content_height);
        int32_t maxh = calc_dynamic_height(obj, LV_STYLE_MAX_HEIGHT, &content_height);
        h = LV_CLAMP(minh, unclamped_h, maxh);

        /**
         * If the height in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `h_ignore_size` excludes this object from the
         * parent's content height calculation. See the width branch above.
         */
        bool h_pct = आकार_अन्तर्_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_height_आन्तरिकम्(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_height_आन्तरिकम्(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_height_आन्तरिकम्(obj, LV_PART_MAIN));

        obj->h_ignore_आकार = (h_pct && parent->h_layout == 0 &&
                              lv_obj_get_style_height_आन्तरिकम्(parent, 0) == LV_आकार_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    यदि(lv_obj_get_विस्तारः(obj) == w && lv_obj_get_height(obj) == h)
        प्रत्यागम false;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t parent_fit_area;
    lv_obj_get_content_coords(parent, &parent_fit_area);

    /*If the object is already out of the parent and its position is changes
     *surely the scrollbars also changes so invalidate them*/
    bool on1 = lv_area_is_अन्तर्(&ori, &parent_fit_area, 0);
    यदि(!on1)
        lv_obj_scrollbar_invalidate(parent);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    यदि(lv_obj_get_style_आधारः_dir_आन्तरिकम्(obj, LV_PART_MAIN) == LV_आधारः_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    अन्यथा {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_आकार_परिवर्तितम्, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_परिवर्तितम्, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_अन्तर्(&obj->coords, &parent_fit_area, 0);
    यदि(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(parent);

    lv_obj_refresh_ext_draw_आकार(obj);

    प्रत्यागम true;
}

शून्य lv_obj_समुच्चयः_आकार(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_समुच्चयः_विस्तारः(obj, w);
    lv_obj_समुच्चयः_height(obj, h);
}

शून्य lv_obj_समुच्चयः_विस्तारः(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    lv_style_res_t res_w;
    lv_style_मानम्_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_विस्तारः, &v_w, 0);

    यदि((res_w == LV_STYLE_RES_प्राप्तम् && v_w.अङ्कः != w) || res_w == LV_STYLE_RES_NOT_प्राप्तम्) {
        lv_obj_समुच्चयः_style_विस्तारः(obj, w, 0);
    }
}

शून्य lv_obj_समुच्चयः_height(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    lv_style_res_t res_h;
    lv_style_मानम्_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_HEIGHT, &v_h, 0);

    यदि((res_h == LV_STYLE_RES_प्राप्तम् && v_h.अङ्कः != h) || res_h == LV_STYLE_RES_NOT_प्राप्तम्) {
        lv_obj_समुच्चयः_style_height(obj, h, 0);
    }
}

शून्य lv_obj_समुच्चयः_content_विस्तारः(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    int32_t वामः = lv_obj_get_style_space_वामः_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t दक्षिणः = lv_obj_get_style_space_दक्षिणः_आन्तरिकम्(obj, LV_PART_MAIN);
    lv_obj_समुच्चयः_विस्तारः(obj, w + वामः + दक्षिणः);
}

शून्य lv_obj_समुच्चयः_content_height(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    int32_t शिखरम् = lv_obj_get_style_space_शिखरम्_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_आन्तरिकम्(obj, LV_PART_MAIN);
    lv_obj_समुच्चयः_height(obj, h + शिखरम् + bottom);
}

शून्य lv_obj_समुच्चयः_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_समुच्चयः_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    यदि(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) प्रत्यागम false;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent == रिक्त) प्रत्यागम false;

    uint32_t layout = lv_obj_get_style_layout_आन्तरिकम्(parent, LV_PART_MAIN);
    यदि(layout) प्रत्यागम true;
    अन्यथा प्रत्यागम false;
}

शून्य lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, रिक्त);
}

शून्य lv_obj_update_layout(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    यदि(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        प्रत्यागम;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    यावत्(scr->scr_layout_inv) {
        यदि(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_सन्देशः(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (शून्य *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            भङ्ग;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, रिक्त);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_अन्तः;
}

शून्य lv_obj_समुच्चयः_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_समुच्चयः_style_align(obj, align, 0);
}

शून्य lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    lv_obj_समुच्चयः_style_align(obj, align, 0);
    lv_obj_समुच्चयः_pos(obj, x_ofs, y_ofs);
}

शून्य lv_obj_align_गन्तव्यम्(lv_obj_t * obj, ध्रुव lv_obj_t * आधारः, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_update_layout(obj);
    यदि(आधारः == रिक्त) आधारः = lv_obj_get_parent(obj);

    यदि(आधारः == रिक्त) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        प्रत्यागम;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent == रिक्त) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        प्रत्यागम;
    }

    int32_t pleft = lv_obj_get_style_space_वामः_आन्तरिकम्(parent, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_शिखरम्_आन्तरिकम्(parent, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_वामः_आन्तरिकम्(आधारः, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_शिखरम्_आन्तरिकम्(आधारः, LV_PART_MAIN);

    यदि(align == LV_ALIGN_DEFAULT) {
        यदि(lv_obj_get_style_आधारः_dir_आन्तरिकम्(आधारः, LV_PART_MAIN) == LV_आधारः_DIR_RTL) align = LV_ALIGN_शिखरम्_दक्षिणः;
        अन्यथा align = LV_ALIGN_शिखरम्_वामः;
    }

    विकल्प(align) {
        पक्ष LV_ALIGN_CENTER:
            x = lv_obj_get_content_विस्तारः(आधारः) / 2 - lv_obj_get_विस्तारः(obj) / 2 + bleft;
            y = lv_obj_get_content_height(आधारः) / 2 - lv_obj_get_height(obj) / 2 + btop;
            भङ्ग;

        पक्ष LV_ALIGN_शिखरम्_वामः:
            x = bleft;
            y = btop;
            भङ्ग;

        पक्ष LV_ALIGN_शिखरम्_MID:
            x = lv_obj_get_content_विस्तारः(आधारः) / 2 - lv_obj_get_विस्तारः(obj) / 2 + bleft;
            y = btop;
            भङ्ग;

        पक्ष LV_ALIGN_शिखरम्_दक्षिणः:
            x = lv_obj_get_content_विस्तारः(आधारः) - lv_obj_get_विस्तारः(obj) + bleft;
            y = btop;
            भङ्ग;

        पक्ष LV_ALIGN_BOTTOM_वामः:
            x = bleft;
            y = lv_obj_get_content_height(आधारः) - lv_obj_get_height(obj) + btop;
            भङ्ग;
        पक्ष LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_विस्तारः(आधारः) / 2 - lv_obj_get_विस्तारः(obj) / 2 + bleft;
            y = lv_obj_get_content_height(आधारः) - lv_obj_get_height(obj) + btop;
            भङ्ग;

        पक्ष LV_ALIGN_BOTTOM_दक्षिणः:
            x = lv_obj_get_content_विस्तारः(आधारः) - lv_obj_get_विस्तारः(obj) + bleft;
            y = lv_obj_get_content_height(आधारः) - lv_obj_get_height(obj) + btop;
            भङ्ग;

        पक्ष LV_ALIGN_वामः_MID:
            x = bleft;
            y = lv_obj_get_content_height(आधारः) / 2 - lv_obj_get_height(obj) / 2 + btop;
            भङ्ग;

        पक्ष LV_ALIGN_दक्षिणः_MID:
            x = lv_obj_get_content_विस्तारः(आधारः) - lv_obj_get_विस्तारः(obj) + bleft;
            y = lv_obj_get_content_height(आधारः) / 2 - lv_obj_get_height(obj) / 2 + btop;
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_शिखरम्_वामः:
            x = 0;
            y = -lv_obj_get_height(obj);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_शिखरम्_MID:
            x = lv_obj_get_विस्तारः(आधारः) / 2 - lv_obj_get_विस्तारः(obj) / 2;
            y = -lv_obj_get_height(obj);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_शिखरम्_दक्षिणः:
            x = lv_obj_get_विस्तारः(आधारः) - lv_obj_get_विस्तारः(obj);
            y = -lv_obj_get_height(obj);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_BOTTOM_वामः:
            x = 0;
            y = lv_obj_get_height(आधारः);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_BOTTOM_MID:
            x = lv_obj_get_विस्तारः(आधारः) / 2 - lv_obj_get_विस्तारः(obj) / 2;
            y = lv_obj_get_height(आधारः);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_BOTTOM_दक्षिणः:
            x = lv_obj_get_विस्तारः(आधारः) - lv_obj_get_विस्तारः(obj);
            y = lv_obj_get_height(आधारः);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_वामः_शिखरम्:
            x = -lv_obj_get_विस्तारः(obj);
            y = 0;
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_वामः_MID:
            x = -lv_obj_get_विस्तारः(obj);
            y = lv_obj_get_height(आधारः) / 2 - lv_obj_get_height(obj) / 2;
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_वामः_BOTTOM:
            x = -lv_obj_get_विस्तारः(obj);
            y = lv_obj_get_height(आधारः) - lv_obj_get_height(obj);
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_दक्षिणः_शिखरम्:
            x = lv_obj_get_विस्तारः(आधारः);
            y = 0;
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_दक्षिणः_MID:
            x = lv_obj_get_विस्तारः(आधारः);
            y = lv_obj_get_height(आधारः) / 2 - lv_obj_get_height(obj) / 2;
            भङ्ग;

        पक्ष LV_ALIGN_बहिर्गमनम्_दक्षिणः_BOTTOM:
            x = lv_obj_get_विस्तारः(आधारः);
            y = lv_obj_get_height(आधारः) - lv_obj_get_height(obj);
            भङ्ग;

        पक्ष LV_ALIGN_DEFAULT:
            भङ्ग;
    }

    यदि(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_विस्तारः(आधारः) * LV_COORD_GET_PCT(x_ofs)) / 100;
    यदि(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_height(आधारः) * LV_COORD_GET_PCT(y_ofs)) / 100;
    यदि(lv_obj_get_style_आधारः_dir_आन्तरिकम्(parent, LV_PART_MAIN) == LV_आधारः_DIR_RTL) {
        x += x_ofs + आधारः->coords.x1 - parent->coords.x1 + lv_obj_get_scroll_दक्षिणः(parent) - pleft;
    }
    अन्यथा {
        x += x_ofs + आधारः->coords.x1 - parent->coords.x1 + lv_obj_get_scroll_वामः(parent) - pleft;
    }
    y += y_ofs + आधारः->coords.y1 - parent->coords.y1 + lv_obj_get_scroll_शिखरम्(parent) - ptop;
    lv_obj_समुच्चयः_style_align(obj, LV_ALIGN_शिखरम्_वामः, 0);
    lv_obj_समुच्चयः_pos(obj, x, y);

}

शून्य lv_obj_get_coords(ध्रुव lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != रिक्त, प्रत्यागम);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t rel_x;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent) {
        rel_x  = obj->coords.x1 - parent->coords.x1;
        rel_x += lv_obj_get_scroll_x(parent);
        rel_x -= lv_obj_get_style_space_वामः_आन्तरिकम्(parent, LV_PART_MAIN);
    }
    अन्यथा {
        rel_x = obj->coords.x1;
    }
    प्रत्यागम rel_x;
}

int32_t lv_obj_get_x2(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_obj_get_x(obj) + lv_obj_get_विस्तारः(obj);
}

int32_t lv_obj_get_y(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t rel_y;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent) {
        rel_y = obj->coords.y1 - parent->coords.y1;
        rel_y += lv_obj_get_scroll_y(parent);
        rel_y -= lv_obj_get_style_space_शिखरम्_आन्तरिकम्(parent, LV_PART_MAIN);
    }
    अन्यथा {
        rel_y = obj->coords.y1;
    }
    प्रत्यागम rel_y;
}

int32_t lv_obj_get_y2(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_obj_get_y(obj) + lv_obj_get_height(obj);
}

int32_t lv_obj_get_x_aligned(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_obj_get_style_x_आन्तरिकम्(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_obj_get_style_y_आन्तरिकम्(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_विस्तारः(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_area_get_विस्तारः(&obj->coords);
}

int32_t lv_obj_get_height(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    प्रत्यागम lv_area_get_height(&obj->coords);
}

int32_t lv_obj_get_content_विस्तारः(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t वामः = lv_obj_get_style_space_वामः_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t दक्षिणः = lv_obj_get_style_space_दक्षिणः_आन्तरिकम्(obj, LV_PART_MAIN);

    प्रत्यागम lv_obj_get_विस्तारः(obj) - वामः - दक्षिणः;
}

int32_t lv_obj_get_content_height(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t शिखरम् = lv_obj_get_style_space_शिखरम्_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_आन्तरिकम्(obj, LV_PART_MAIN);

    प्रत्यागम lv_obj_get_height(obj) - शिखरम् - bottom;
}

शून्य lv_obj_get_content_coords(ध्रुव lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != रिक्त, प्रत्यागम);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_वामः_आन्तरिकम्(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_दक्षिणः_आन्तरिकम्(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_शिखरम्_आन्तरिकम्(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_आन्तरिकम्(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_विस्तारः(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    lv_point_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_आकार, &p);
    प्रत्यागम p.x;
}

int32_t lv_obj_get_self_height(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    lv_point_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_आकार, &p);
    प्रत्यागम p.y;
}

int32_t lv_obj_get_style_clamped_विस्तारः(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t content_विस्तारः = -1;
    int32_t w = calc_dynamic_विस्तारः(obj, LV_STYLE_विस्तारः, &content_विस्तारः);
    int32_t minw = calc_dynamic_विस्तारः(obj, LV_STYLE_MIN_विस्तारः, &content_विस्तारः);
    int32_t maxw = calc_dynamic_विस्तारः(obj, LV_STYLE_MAX_विस्तारः, &content_विस्तारः);
    यदि(w <= minw) {
        w = lv_obj_get_style_min_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    अन्यथा यदि(w >= maxw) {
        w = lv_obj_get_style_max_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    अन्यथा {
        w = lv_obj_get_style_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    प्रत्यागम w;
}

int32_t lv_obj_get_style_clamped_height(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम 0);

    int32_t content_height = -1;
    int32_t h = calc_dynamic_height(obj, LV_STYLE_HEIGHT, &content_height);
    int32_t minh = calc_dynamic_height(obj, LV_STYLE_MIN_HEIGHT, &content_height);
    int32_t maxh = calc_dynamic_height(obj, LV_STYLE_MAX_HEIGHT, &content_height);
    यदि(h <= minh) {
        h = lv_obj_get_style_min_height_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    अन्यथा यदि(h >= maxh) {
        h = lv_obj_get_style_max_height_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    अन्यथा {
        h = lv_obj_get_style_height_आन्तरिकम्(obj, LV_PART_MAIN);
    }
    प्रत्यागम h;
}

bool lv_obj_is_style_any_विस्तारः_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t w = lv_obj_get_style_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_विस्तारः_आन्तरिकम्(obj, LV_PART_MAIN);
    प्रत्यागम (w == LV_आकार_CONTENT || minw == LV_आकार_CONTENT || maxw == LV_आकार_CONTENT);
}

bool lv_obj_is_style_any_height_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t h = lv_obj_get_style_height_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_height_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_height_आन्तरिकम्(obj, LV_PART_MAIN);
    प्रत्यागम (h == LV_आकार_CONTENT || minh == LV_आकार_CONTENT || maxh == LV_आकार_CONTENT);
}

bool lv_obj_is_विस्तारः_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t minw = lv_obj_calc_dynamic_विस्तारः(obj, LV_STYLE_MIN_विस्तारः);
    int32_t w = lv_obj_get_विस्तारः(obj);
    प्रत्यागम w == minw;
}

bool lv_obj_is_height_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t minh = lv_obj_calc_dynamic_height(obj, LV_STYLE_MIN_HEIGHT);
    int32_t h = lv_obj_get_height(obj);
    प्रत्यागम h == minh;
}

bool lv_obj_is_विस्तारः_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t maxw = lv_obj_calc_dynamic_विस्तारः(obj, LV_STYLE_MAX_विस्तारः);
    int32_t w = lv_obj_get_विस्तारः(obj);
    प्रत्यागम w == maxw;
}

bool lv_obj_is_height_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    int32_t maxh = lv_obj_calc_dynamic_height(obj, LV_STYLE_MAX_HEIGHT);
    int32_t h = lv_obj_get_height(obj);
    प्रत्यागम h == maxh;
}

bool lv_obj_refresh_self_आकार(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    यदि(!lv_obj_is_style_any_विस्तारः_content(obj) && !lv_obj_is_style_any_height_content(obj))
        प्रत्यागम false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(parent != रिक्त) {
        parent->w_layout = 0;
        parent->h_layout = 0;
        lv_obj_mark_layout_as_dirty(parent);
    }
    lv_obj_mark_layout_as_dirty(obj);
    प्रत्यागम true;
}

शून्य lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    यदि(lv_obj_is_layout_positioned(obj)) प्रत्यागम;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t x = lv_obj_get_style_x_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_आन्तरिकम्(obj, LV_PART_MAIN);

    यदि(parent == रिक्त) {
        lv_obj_move_गन्तव्यम्(obj, x, y);
        प्रत्यागम;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_विस्तारः(parent);
    int32_t ph = lv_obj_get_content_height(parent);
    यदि(LV_COORD_IS_PCT(x)) {
        यदि(lv_obj_get_style_विस्तारः_आन्तरिकम्(parent, LV_PART_MAIN) == LV_आकार_CONTENT) x = 0; /*Avoid circular dependency*/
        अन्यथा x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    यदि(LV_COORD_IS_PCT(y)) {
        यदि(lv_obj_get_style_height_आन्तरिकम्(parent, LV_PART_MAIN) == LV_आकार_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_विस्तारः(obj);
    int32_t h = lv_obj_get_height(obj);
    यदि(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    यदि(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_आन्तरिकम्(obj, LV_PART_MAIN);

    यदि(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_शिखरम्_वामः;
    }

    bool rtl = lv_obj_get_style_आधारः_dir_आन्तरिकम्(parent, LV_PART_MAIN) == LV_आधारः_DIR_RTL;

    यदि(rtl) {
        विकल्प(align) {
            पक्ष LV_ALIGN_शिखरम्_वामः:
                align = LV_ALIGN_शिखरम्_दक्षिणः;
                भङ्ग;
            पक्ष LV_ALIGN_शिखरम्_दक्षिणः:
                align = LV_ALIGN_शिखरम्_वामः;
                भङ्ग;
            पक्ष LV_ALIGN_वामः_MID:
                align = LV_ALIGN_दक्षिणः_MID;
                भङ्ग;
            पक्ष LV_ALIGN_दक्षिणः_MID:
                align = LV_ALIGN_वामः_MID;
                भङ्ग;
            पक्ष LV_ALIGN_BOTTOM_वामः:
                align = LV_ALIGN_BOTTOM_दक्षिणः;
                भङ्ग;
            पक्ष LV_ALIGN_BOTTOM_दक्षिणः:
                align = LV_ALIGN_BOTTOM_वामः;
                भङ्ग;
            स्वभाव:
                भङ्ग;
        }
    }

    विकल्प(align) {
        पक्ष LV_ALIGN_शिखरम्_वामः:
            भङ्ग;
        पक्ष LV_ALIGN_शिखरम्_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            भङ्ग;
        पक्ष LV_ALIGN_शिखरम्_दक्षिणः:
            x = rtl ? pw - w - x : pw - w + x;
            भङ्ग;
        पक्ष LV_ALIGN_वामः_MID:
            y += ph / 2 - h / 2;
            भङ्ग;
        पक्ष LV_ALIGN_BOTTOM_वामः:
            y += ph - h;
            भङ्ग;
        पक्ष LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            भङ्ग;
        पक्ष LV_ALIGN_BOTTOM_दक्षिणः:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            भङ्ग;
        पक्ष LV_ALIGN_दक्षिणः_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            भङ्ग;
        पक्ष LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            भङ्ग;
        स्वभाव:
            भङ्ग;
    }

    lv_obj_move_गन्तव्यम्(obj, x, y);
}

शून्य lv_obj_move_गन्तव्यम्(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * parent = obj->parent;

    यदि(parent) {
        यदि(lv_obj_is_floating(obj)) {
            x += parent->coords.x1;
            y += parent->coords.y1;
        }
        अन्यथा {
            x += parent->coords.x1 - lv_obj_get_scroll_x(parent);
            y += parent->coords.y1 - lv_obj_get_scroll_y(parent);
        }

        x += lv_obj_get_style_space_वामः_आन्तरिकम्(parent, LV_PART_MAIN);
        y += lv_obj_get_style_space_शिखरम्_आन्तरिकम्(parent, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_point_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    यदि(diff.x == 0 && diff.y == 0) प्रत्यागम;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t parent_fit_area;
    bool on1 = false;
    यदि(parent) {
        lv_obj_get_content_coords(parent, &parent_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_अन्तर्(&ori, &parent_fit_area, 0);
        यदि(!on1) lv_obj_scrollbar_invalidate(parent);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    यदि(parent) lv_obj_send_event(parent, LV_EVENT_CHILD_परिवर्तितम्, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    यदि(parent) {
        bool on2 = lv_area_is_अन्तर्(&obj->coords, &parent_fit_area, 0);
        यदि(!on1 || !on2) lv_obj_scrollbar_invalidate(parent);
    }
}

शून्य lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गणनम्(obj);
    प्रति(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        यदि(ignore_floating && lv_obj_is_floating(child)) अनुवर्त;
        child->coords.x1 += x_diff;
        child->coords.y1 += y_diff;
        child->coords.x2 += x_diff;
        child->coords.y2 += y_diff;

        lv_obj_move_children_by(child, x_diff, y_diff, false);
    }
}

शून्य lv_obj_transform_point(ध्रुव lv_obj_t * obj, lv_point_t * p, lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(p != रिक्त, प्रत्यागम);

    lv_obj_transform_point_array(obj, p, 1, flags);
}

शून्य lv_obj_transform_point_array(ध्रुव lv_obj_t * obj, lv_point_t points[], आकार_t गणनम्,
                                  lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);
    LV_CHECK_ARG(points != रिक्त || गणनम् == 0, प्रत्यागम);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    यदि(inverse) {
        यदि(recursive && parent) lv_obj_transform_point_array(parent, points, गणनम्, flags);
        यदि(do_tranf) transform_point_array(obj, points, गणनम्, inverse);
    }
    अन्यथा {
        यदि(do_tranf) transform_point_array(obj, points, गणनम्, inverse);
        यदि(recursive && parent) lv_obj_transform_point_array(parent, points, गणनम्, flags);
    }
}

शून्य lv_obj_get_transformed_area(ध्रुव lv_obj_t * obj, lv_area_t * area, lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_ARG(area != रिक्त, प्रत्यागम);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_point_t p[4] = {
        {area->x1, area->y1},
        {area->x1, area->y2 + 1},
        {area->x2 + 1, area->y1},
        {area->x2 + 1, area->y2 + 1},
    };

    lv_obj_transform_point_array(obj, p, 4, flags);

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

स्थिर lv_obj_वृक्षः_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, शून्य * user_दत्त)
{
    यदि(!obj->has_blur) प्रत्यागम LV_OBJ_वृक्षः_WALK_अग्र;

    /*invalidate_area_core() expects untransformed coordinates and applies the
     *transform itself*/
    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_आकार, ext_आकार);

    /*The invalidated areas are in screen coordinates so check the overlap
     *against the transformed area*/
    lv_area_t scr_coords;
    scr_coords = obj_coords;
    यदि(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_दत्त;
    uint32_t i;
    प्रति(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        यदि(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            प्रत्यागम LV_OBJ_वृक्षः_WALK_SKIP_CHILDREN;
        }
    }

    प्रत्यागम LV_OBJ_वृक्षः_WALK_अग्र;
}

शून्य lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    यदि(disp->inv_p == 0) प्रत्यागम;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    यदि(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) प्रत्यागम;

    uint32_t पूर्व_inv_p;
    कुरु {
        पूर्व_inv_p = disp->inv_p;

        lv_obj_वृक्षः_walk(disp->act_scr, blur_expand_walk_cb, disp);
        यदि(disp->पूर्व_scr) lv_obj_वृक्षः_walk(disp->पूर्व_scr, blur_expand_walk_cb, disp);
        lv_obj_वृक्षः_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_वृक्षः_walk(disp->शिखरम्_layer, blur_expand_walk_cb, disp);
        lv_obj_वृक्षः_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } यावत्(disp->inv_p > पूर्व_inv_p);
}

lv_परिणाम_t lv_obj_invalidate_area(ध्रुव lv_obj_t * obj, ध्रुव lv_area_t * area)
{
    LV_CHECK_ARG(area != रिक्त, प्रत्यागम LV_परिणाम_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम LV_परिणाम_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    यदि(!lv_display_is_invalidation_enabled(disp)) प्रत्यागम LV_परिणाम_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    यदि(obj->has_blur) प्रत्यागम lv_obj_invalidate(obj);
    अन्यथा प्रत्यागम obj_invalidate_area_आन्तरिकम्(obj, area);
}


lv_परिणाम_t lv_obj_invalidate(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम LV_परिणाम_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    यदि(!lv_display_is_invalidation_enabled(disp)) प्रत्यागम LV_परिणाम_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_आकार;
    obj_coords.y1 -= ext_आकार;
    obj_coords.x2 += ext_आकार;
    obj_coords.y2 += ext_आकार;

    lv_परिणाम_t res = obj_invalidate_area_आन्तरिकम्(obj, &obj_coords);

    प्रत्यागम res;
}

bool lv_obj_area_is_visible(ध्रुव lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != रिक्त, प्रत्यागम false);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    यदि(lv_obj_is_hidden(obj)) प्रत्यागम false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    यदि(obj_scr != lv_display_get_screen_सक्रिय(disp) &&
       obj_scr != lv_display_get_screen_पूर्व(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_शिखरम्(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        प्रत्यागम false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_आकार, ext_आकार);

    /*The area is not on the object*/
    यदि(!lv_area_intersect(area, area, &obj_coords)) प्रत्यागम false;

    यदि(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * parent = lv_obj_get_parent(obj);
    यावत्(parent != रिक्त) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        यदि(lv_obj_is_hidden(parent)) प्रत्यागम false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t parent_coords = parent->coords;
        यदि(lv_obj_is_अतिप्रवाहः_visible(parent)) {
            int32_t parent_ext_आकार = lv_obj_get_ext_draw_आकार(parent);
            lv_area_increase(&parent_coords, parent_ext_आकार, parent_ext_आकार);
        }

        यदि(is_transformed(parent)) {
            lv_obj_get_transformed_area(parent, &parent_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
        }
        यदि(!lv_area_intersect(area, area, &parent_coords)) प्रत्यागम false;

        parent = lv_obj_get_parent(parent);
    }

    प्रत्यागम true;
}

bool lv_obj_is_visible(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_आकार;
    obj_coords.y1 -= ext_आकार;
    obj_coords.x2 += ext_आकार;
    obj_coords.y2 += ext_आकार;

    प्रत्यागम lv_obj_area_is_visible(obj, &obj_coords);
}

शून्य lv_obj_समुच्चयः_ext_click_area(lv_obj_t * obj, int32_t आकार)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    यदि(!lv_obj_allocate_spec_attr(obj)) {
        प्रत्यागम;
    }
    obj->spec_attr->ext_click_pad = आकार;
}

शून्य lv_obj_get_click_area(ध्रुव lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != रिक्त, प्रत्यागम);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    *area = obj->coords;
    यदि(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_लब्धम्_test(lv_obj_t * obj, ध्रुव lv_point_t * point)
{
    LV_CHECK_ARG(point != रिक्त, प्रत्यागम false);
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम false);

    यदि(!lv_obj_is_clickable(obj)) प्रत्यागम false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_point_on(&a, point, 0);
    यदि(res == false) प्रत्यागम false;

    यदि(lv_obj_is_adv_hittest(obj)) {
        lv_लब्धम्_test_info_t लब्धम्_info;
        लब्धम्_info.point = point;
        लब्धम्_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_लब्धम्_TEST, &लब्धम्_info);
        प्रत्यागम लब्धम्_info.res;
    }

    प्रत्यागम res;
}

int32_t lv_clamp_विस्तारः(int32_t विस्तारः, int32_t min_विस्तारः, int32_t max_विस्तारः, int32_t ref_विस्तारः)
{
    यदि(LV_COORD_IS_PCT(min_विस्तारः)) min_विस्तारः = (ref_विस्तारः * LV_COORD_GET_PCT(min_विस्तारः)) / 100;
    यदि(LV_COORD_IS_PCT(max_विस्तारः)) max_विस्तारः = (ref_विस्तारः * LV_COORD_GET_PCT(max_विस्तारः)) / 100;
    प्रत्यागम LV_CLAMP(min_विस्तारः, विस्तारः, max_विस्तारः);
}

int32_t lv_clamp_height(int32_t height, int32_t min_height, int32_t max_height, int32_t ref_height)
{
    यदि(LV_COORD_IS_PCT(min_height)) min_height = (ref_height * LV_COORD_GET_PCT(min_height)) / 100;
    यदि(LV_COORD_IS_PCT(max_height)) max_height = (ref_height * LV_COORD_GET_PCT(max_height)) / 100;
    प्रत्यागम LV_CLAMP(min_height, height, max_height);
}

शून्य lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

शून्य lv_obj_समुच्चयः_transform(lv_obj_t * obj, ध्रुव lv_matrix_t * matrix)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    यदि(!matrix) {
        lv_obj_reset_transform(obj);
        प्रत्यागम;
    }

    यदि(!lv_obj_allocate_spec_attr(obj)) {
        प्रत्यागम;
    }

    यदि(!obj->spec_attr->matrix) {
        obj->spec_attr->matrix = lv_malloc(परिमाण(lv_matrix_t));
        LV_ASSERT_MALLOC(obj->spec_attr->matrix);
        यदि(obj->spec_attr->matrix == रिक्त) प्रत्यागम;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Copy the matrix */
    *obj->spec_attr->matrix = *matrix;

    /* Matrix is set. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#अन्यथा
    LV_अप्रयुक्तम्(obj);
    LV_अप्रयुक्तम्(matrix);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#यदि_अन्तः
}

शून्य lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम);

#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    यदि(!obj->spec_attr) {
        प्रत्यागम;
    }

    यदि(!obj->spec_attr->matrix) {
        प्रत्यागम;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->matrix);
    obj->spec_attr->matrix = रिक्त;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#अन्यथा
    LV_अप्रयुक्तम्(obj);
#यदि_अन्तः
}

ध्रुव lv_matrix_t * lv_obj_get_transform(ध्रुव lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, प्रत्यागम रिक्त);

#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    यदि(obj->spec_attr) {
        प्रत्यागम obj->spec_attr->matrix;
    }
#अन्यथा
    LV_अप्रयुक्तम्(obj);
#यदि_अन्तः
    प्रत्यागम रिक्त;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर lv_परिणाम_t obj_invalidate_area_आन्तरिकम्(ध्रुव lv_obj_t * obj, ध्रुव lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    प्रत्यागम invalidate_area_core(obj, &area_tmp);
}

स्थिर bool is_transformed(ध्रुव lv_obj_t * obj)
{
    यावत्(obj) {
        यदि(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) प्रत्यागम true;
        obj = obj->parent;
    }
    प्रत्यागम false;
}

स्थिर int32_t calc_content_विस्तारः(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    यदि(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_दक्षिणः = lv_obj_get_style_space_दक्षिणः_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t space_वामः = lv_obj_get_style_space_वामः_आन्तरिकम्(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_विस्तारः(obj) + space_वामः + space_दक्षिणः;

    int32_t child_res = LV_COORD_MIN;

    यदि(!lv_layout_get_min_आकार(obj, &child_res, true)) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गणनम्(obj);
        /*With RTL find the left most coordinate*/
        यदि(lv_obj_get_style_आधारः_dir_आन्तरिकम्(obj, LV_PART_MAIN) == LV_आधारः_DIR_RTL) {
            प्रति(i = 0; i < child_cnt; i++) {
                int32_t child_res_tmp = LV_COORD_MIN;
                lv_obj_t * child = obj->spec_attr->children[i];
                यदि(child->hidden || child->floating)
                    अनुवर्त;

                यदि(child->w_ignore_आकार)
                    अनुवर्त;

                यदि(!lv_obj_is_layout_positioned(child)) {
                    lv_align_t align = lv_obj_get_style_align_आन्तरिकम्(child, LV_PART_MAIN);
                    विकल्प(align) {
                        पक्ष LV_ALIGN_DEFAULT:
                        पक्ष LV_ALIGN_शिखरम्_दक्षिणः:
                        पक्ष LV_ALIGN_BOTTOM_दक्षिणः:
                        पक्ष LV_ALIGN_दक्षिणः_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            child_res_tmp = obj->coords.x2 - child->coords.x1 + 1;
                            भङ्ग;
                        स्वभाव:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            यदि(lv_obj_get_style_x_आन्तरिकम्(child, LV_PART_MAIN) == 0) {
                                child_res_tmp = lv_area_get_विस्तारः(&child->coords) + space_दक्षिणः;
                                child_res_tmp += lv_obj_get_style_margin_वामः_आन्तरिकम्(child, LV_PART_MAIN);
                            }
                            भङ्ग;
                    }
                }
                अन्यथा {
                    child_res_tmp = obj->coords.x2 - child->coords.x1 + 1;
                }
                child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_वामः_आन्तरिकम्(child, LV_PART_MAIN));
            }
            यदि(child_res != LV_COORD_MIN) {
                child_res += space_वामः;
            }
        }
        /*Else find the right most coordinate*/
        अन्यथा {
            प्रति(i = 0; i < child_cnt; i++) {
                int32_t child_res_tmp = LV_COORD_MIN;
                lv_obj_t * child = obj->spec_attr->children[i];
                यदि(child->hidden || child->floating) अनुवर्त;

                यदि(child->w_ignore_आकार)
                    अनुवर्त;

                यदि(!lv_obj_is_layout_positioned(child)) {
                    lv_align_t align = lv_obj_get_style_align_आन्तरिकम्(child, LV_PART_MAIN);
                    विकल्प(align) {
                        पक्ष LV_ALIGN_DEFAULT:
                        पक्ष LV_ALIGN_शिखरम्_वामः:
                        पक्ष LV_ALIGN_BOTTOM_वामः:
                        पक्ष LV_ALIGN_वामः_MID:
                            /*Normal left aligns.*/
                            child_res_tmp = child->coords.x2 - obj->coords.x1 + 1;
                            भङ्ग;
                        स्वभाव:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            यदि(lv_obj_get_style_x_आन्तरिकम्(child, LV_PART_MAIN) == 0) {
                                child_res_tmp = lv_area_get_विस्तारः(&child->coords) + space_वामः;
                                child_res_tmp += lv_obj_get_style_margin_दक्षिणः_आन्तरिकम्(child, LV_PART_MAIN);
                            }
                            भङ्ग;
                    }
                }
                अन्यथा {
                    child_res_tmp = child->coords.x2 - obj->coords.x1 + 1;
                }

                child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_दक्षिणः_आन्तरिकम्(child, LV_PART_MAIN));
            }

            यदि(child_res != LV_COORD_MIN) {
                child_res += space_दक्षिणः;
            }
        }
    }

    यदि(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    यदि(child_res == LV_COORD_MIN) {
        प्रत्यागम self_w;
    }

    प्रत्यागम LV_MAX(child_res, self_w);
}

स्थिर int32_t calc_content_height(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    यदि(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_शिखरम् = lv_obj_get_style_space_शिखरम्_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_आन्तरिकम्(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_height(obj) + space_शिखरम् + space_bottom;

    int32_t child_res = LV_COORD_MIN;

    यदि(!lv_layout_get_min_आकार(obj, &child_res, false)) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गणनम्(obj);
        प्रति(i = 0; i < child_cnt; i++) {
            int32_t child_res_tmp = LV_COORD_MIN;
            lv_obj_t * child = obj->spec_attr->children[i];
            यदि(child->hidden || child->floating) अनुवर्त;

            यदि(child->h_ignore_आकार)
                अनुवर्त;

            यदि(!lv_obj_is_layout_positioned(child)) {
                lv_align_t align = lv_obj_get_style_align_आन्तरिकम्(child, LV_PART_MAIN);
                विकल्प(align) {
                    पक्ष LV_ALIGN_DEFAULT:
                    पक्ष LV_ALIGN_शिखरम्_दक्षिणः:
                    पक्ष LV_ALIGN_शिखरम्_MID:
                    पक्ष LV_ALIGN_शिखरम्_वामः:
                        /*Normal top aligns. */
                        child_res_tmp = child->coords.y2 - obj->coords.y1 + 1;
                        भङ्ग;
                    स्वभाव:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        यदि(lv_obj_get_style_y_आन्तरिकम्(child, LV_PART_MAIN) == 0) {
                            child_res_tmp = lv_area_get_height(&child->coords) + space_शिखरम्;
                            child_res_tmp += lv_obj_get_style_margin_शिखरम्_आन्तरिकम्(child, LV_PART_MAIN);
                        }
                        भङ्ग;
                }
            }
            अन्यथा {
                child_res_tmp = child->coords.y2 - obj->coords.y1 + 1;
            }

            child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_bottom_आन्तरिकम्(child, LV_PART_MAIN));
        }

        यदि(child_res != LV_COORD_MIN) {
            child_res += space_bottom;
        }
    }

    यदि(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    यदि(child_res == LV_COORD_MIN) {
        प्रत्यागम self_h;
    }

    प्रत्यागम LV_MAX(self_h, child_res);
}

स्थिर शून्य layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गणनम्(obj);
    प्रति(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        layout_update_core(child);
    }

    यदि(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_आकार(obj);
        lv_obj_refr_pos(obj);

        यदि(child_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    यदि(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

स्थिर शून्य transform_point_array(ध्रुव lv_obj_t * obj, lv_point_t * p, आकार_t p_गणनम्, bool inv)
{
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    ध्रुव lv_matrix_t * obj_matrix = lv_obj_get_transform(obj);
    यदि(obj_matrix) {
        lv_matrix_t m;
        lv_matrix_तटस्थः(&m);
        lv_matrix_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_matrix_multiply(&m, obj_matrix);
        lv_matrix_translate(&m, -obj->coords.x1, -obj->coords.y1);

        यदि(inv) {
            lv_matrix_t inv_m;
            lv_matrix_inverse(&inv_m, &m);
            m = inv_m;
        }

        प्रति(आकार_t i = 0; i < p_गणनम्; i++) {
            lv_point_precise_t p_precise = lv_point_गन्तव्यम्_precise(&p[i]);
            lv_point_precise_t res = lv_matrix_transform_precise_point(&m, &p_precise);
            p[i] = lv_point_from_precise(&res);
        }

        प्रत्यागम;
    }
#यदि_अन्तः /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_safe_आन्तरिकम्(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_safe_आन्तरिकम्(obj, LV_PART_MAIN);
    यदि(scale_x == 0) scale_x = 1;
    यदि(scale_y == 0) scale_y = 1;

    यदि(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) प्रत्यागम;

    lv_point_t pivot = {
        .x = lv_obj_get_style_transform_pivot_x_आन्तरिकम्(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_pivot_y_आन्तरिकम्(obj, LV_PART_MAIN)
    };

    यदि(LV_COORD_IS_PCT(pivot.x)) {
        pivot.x = (LV_COORD_GET_PCT(pivot.x) * lv_area_get_विस्तारः(&obj->coords)) / 100;
    }
    यदि(LV_COORD_IS_PCT(pivot.y)) {
        pivot.y = (LV_COORD_GET_PCT(pivot.y) * lv_area_get_height(&obj->coords)) / 100;
    }

    pivot.x = obj->coords.x1 + pivot.x;
    pivot.y = obj->coords.y1 + pivot.y;

    यदि(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_point_array_transform(p, p_गणनम्, angle, scale_x, scale_y, &pivot, !inv);
}

स्थिर lv_परिणाम_t invalidate_area_core(ध्रुव lv_obj_t * obj, lv_area_t * area_tmp)
{
    यदि(!lv_obj_area_is_visible(obj, area_tmp)) प्रत्यागम LV_परिणाम_INVALID;
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#अन्यथा
    यदि(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#यदि_अन्तः

    lv_परिणाम_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    प्रत्यागम res;
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
स्थिर int32_t calc_dynamic_विस्तारः(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ध्रुव content_विस्तारः)
{
    LV_ASSERT(prop == LV_STYLE_विस्तारः || prop == LV_STYLE_MIN_विस्तारः || prop == LV_STYLE_MAX_विस्तारः);

    int32_t विस्तारः = lv_obj_get_style_prop(obj, 0, prop).अङ्कः;

    यदि(विस्तारः == LV_आकार_CONTENT) {
        यदि(content_विस्तारः == रिक्त) {
            विस्तारः = calc_content_विस्तारः(obj);
        }
        अन्यथा {
            यदि(*content_विस्तारः < 0) {
                *content_विस्तारः = calc_content_विस्तारः(obj);
            }
            विस्तारः = *content_विस्तारः;
        }
    }
    अन्यथा यदि(LV_COORD_IS_PCT(विस्तारः)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        int32_t parent_w = lv_obj_get_content_विस्तारः(parent);
        विस्तारः = (LV_COORD_GET_PCT(विस्तारः) * parent_w) / 100;
        विस्तारः -= lv_obj_get_style_margin_वामः_आन्तरिकम्(obj, LV_PART_MAIN) + lv_obj_get_style_margin_दक्षिणः_आन्तरिकम्(obj,
                                                                                                                   LV_PART_MAIN);
    }
    प्रत्यागम विस्तारः;
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
स्थिर int32_t calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop, int32_t * ध्रुव content_height)
{
    LV_ASSERT(prop == LV_STYLE_HEIGHT || prop == LV_STYLE_MIN_HEIGHT || prop == LV_STYLE_MAX_HEIGHT);

    int32_t height = lv_obj_get_style_prop(obj, 0, prop).अङ्कः;

    यदि(height == LV_आकार_CONTENT) {
        यदि(content_height == रिक्त) {
            height = calc_content_height(obj);
        }
        अन्यथा {
            यदि(*content_height < 0) {
                *content_height = calc_content_height(obj);
            }
            height = *content_height;
        }
    }
    अन्यथा यदि(LV_COORD_IS_PCT(height)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        int32_t parent_h = lv_obj_get_content_height(parent);
        height = (LV_COORD_GET_PCT(height) * parent_h) / 100;
        height -= lv_obj_get_style_margin_शिखरम्_आन्तरिकम्(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_आन्तरिकम्(obj,
                                                                                                                    LV_PART_MAIN);
    }
    प्रत्यागम height;
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
स्थिर bool आकार_अन्तर्_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t आकार_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    यदि(min > max) प्रत्यागम LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    यदि(unclamped < min) प्रत्यागम LV_COORD_IS_PCT(min_style);
    यदि(unclamped > max) प्रत्यागम LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(आकार_style);
    यदि(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    यदि(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    प्रत्यागम is_pct;
}
