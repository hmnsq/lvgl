/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "../misc/lv_area_private.h"
#समावेश "../lvgl_public.h"
#समावेश "../layouts/lv_layout_private.h"
#समावेश "lv_obj_event_private.h"
#समावेश "lv_obj_draw_private.h"
#समावेश "lv_obj_style_private.h"
#समावेश "lv_obj_private.h"
#समावेश "../display/lv_display_private.h"
#समावेश "lv_refr_private.h"
#समावेश "../core/lv_global.h"
#समावेश "lv_obj_class_private.h"
#समावेश "lv_obj_style_internal.h"
#समावेश "../misc/lv_style_private.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा MY_CLASS (&lv_obj_class)
#घोषणा update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

#अगर_अघोषित LV_OBJ_LAYOUT_UPDATE_MAX_PASSES
    #घोषणा LV_OBJ_LAYOUT_UPDATE_MAX_PASSES 100
#पूर्ण_अगर

#अगर LV_OBJ_LAYOUT_UPDATE_MAX_PASSES <= 0
    #त्रुटि "LV_OBJ_LAYOUT_UPDATE_MAX_PASSES needs to be at least 1, otherwise no object layout is calculated"
#पूर्ण_अगर

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल int32_t calc_content_width(lv_obj_t * obj);
अटल int32_t calc_content_height(lv_obj_t * obj);
अटल व्योम layout_update_core(lv_obj_t * obj);
अटल व्योम transform_point_array(स्थिर lv_obj_t * obj, lv_point_t * p, माप_प्रकार p_गिनती, bool inv);
अटल bool is_transformed(स्थिर lv_obj_t * obj);
अटल lv_परिणाम_t invalidate_area_core(स्थिर lv_obj_t * obj, lv_area_t * area_tmp);
अटल lv_परिणाम_t obj_invalidate_area_internal(स्थिर lv_obj_t * obj, स्थिर lv_area_t * area);
अटल int32_t calc_dynamic_width(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_width);
अटल int32_t calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop, int32_t * content_height);
अटल bool आकार_in_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t आकार_style,
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

व्योम lv_obj_set_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_set_x(obj, x);
    lv_obj_set_y(obj, y);
}

व्योम lv_obj_set_x(lv_obj_t * obj, int32_t x)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_style_res_t res_x;
    lv_style_मूल्य_t v_x;

    res_x = lv_obj_get_local_style_prop(obj, LV_STYLE_X, &v_x, 0);

    अगर((res_x == LV_STYLE_RES_FOUND && v_x.अंक != x) || res_x == LV_STYLE_RES_NOT_FOUND) {
        lv_obj_set_style_x(obj, x, 0);
    }
}

व्योम lv_obj_set_y(lv_obj_t * obj, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_style_res_t res_y;
    lv_style_मूल्य_t v_y;

    res_y = lv_obj_get_local_style_prop(obj, LV_STYLE_Y, &v_y, 0);

    अगर((res_y == LV_STYLE_RES_FOUND && v_y.अंक != y) || res_y == LV_STYLE_RES_NOT_FOUND) {
        lv_obj_set_style_y(obj, y, 0);
    }
}

int32_t lv_obj_calc_dynamic_width(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);
    LV_CHECK_ARG(prop == LV_STYLE_WIDTH || prop == LV_STYLE_MIN_WIDTH || prop == LV_STYLE_MAX_WIDTH, वापस 0);

    वापस calc_dynamic_width(obj, prop, शून्य);
}

int32_t lv_obj_calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);
    LV_CHECK_ARG(prop == LV_STYLE_HEIGHT || prop == LV_STYLE_MIN_HEIGHT || prop == LV_STYLE_MAX_HEIGHT, वापस 0);

    वापस calc_dynamic_height(obj, prop, शून्य);
}

bool lv_obj_refr_आकार(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    /*If the width or height is set by a layout do not modify them*/
    अगर(obj->w_layout && obj->h_layout) वापस false;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent == शून्य) वापस false;

    int32_t w;
    अगर(obj->w_layout) {
        w = lv_obj_get_width(obj);
    }
    अन्यथा {
        int32_t content_width = -1;
        int32_t unclamped_w = calc_dynamic_width(obj, LV_STYLE_WIDTH, &content_width);
        int32_t minw = calc_dynamic_width(obj, LV_STYLE_MIN_WIDTH, &content_width);
        int32_t maxw = calc_dynamic_width(obj, LV_STYLE_MAX_WIDTH, &content_width);
        w = LV_CLAMP(minw, unclamped_w, maxw);

        /**
         * If the width in effect is a percentage of a parent that is itself LV_SIZE_CONTENT and not managed by
         * a layout, the two sizes would depend on each other, so `w_ignore_size` excludes this object from the
         * parent's content width calculation.
         */
        bool w_pct = आकार_in_effect_is_pct(unclamped_w, minw, maxw,
                                           lv_obj_get_style_width_internal(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_width_internal(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_width_internal(obj, LV_PART_MAIN));
        obj->w_ignore_आकार =
            (w_pct && parent->w_layout == 0 && lv_obj_get_style_width_internal(parent, 0) == LV_आकार_CONTENT);
    }

    int32_t h;
    अगर(obj->h_layout) {
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
        bool h_pct = आकार_in_effect_is_pct(unclamped_h, minh, maxh,
                                           lv_obj_get_style_height_internal(obj, LV_PART_MAIN),
                                           lv_obj_get_style_min_height_internal(obj, LV_PART_MAIN),
                                           lv_obj_get_style_max_height_internal(obj, LV_PART_MAIN));

        obj->h_ignore_आकार = (h_pct && parent->h_layout == 0 &&
                              lv_obj_get_style_height_internal(parent, 0) == LV_आकार_CONTENT);
    }

    /*Do nothing if the size is not changed*/
    /*It is very important else recursive resizing can occur without size change*/
    अगर(lv_obj_get_width(obj) == w && lv_obj_get_height(obj) == h)
        वापस false;

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
    bool on1 = lv_area_is_in(&ori, &parent_fit_area, 0);
    अगर(!on1)
        lv_obj_scrollbar_invalidate(parent);

    /*Set the length and height
     *Be sure the content is not scrolled in an invalid position on the new size*/
    obj->coords.y2 = obj->coords.y1 + h - 1;
    अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        obj->coords.x1 = obj->coords.x2 - w + 1;
    }
    अन्यथा {
        obj->coords.x2 = obj->coords.x1 + w - 1;
    }

    /*Call the ancestor's event handler to the object with its new coordinates*/
    lv_obj_send_event(obj, LV_EVENT_आकार_CHANGED, &ori);

    /*Call the ancestor's event handler to the parent too*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    obj->readjust_scroll_after_layout = 1;

    /*If the object was out of the parent invalidate the new scrollbar area too.
     *If it wasn't out of the parent but out now, also invalidate the scrollbars*/
    bool on2 = lv_area_is_in(&obj->coords, &parent_fit_area, 0);
    अगर(on1 || (!on1 && on2))
        lv_obj_scrollbar_invalidate(parent);

    lv_obj_refresh_ext_draw_आकार(obj);

    वापस true;
}

व्योम lv_obj_set_आकार(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_set_width(obj, w);
    lv_obj_set_height(obj, h);
}

व्योम lv_obj_set_width(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    lv_style_res_t res_w;
    lv_style_मूल्य_t v_w;

    res_w = lv_obj_get_local_style_prop(obj, LV_STYLE_WIDTH, &v_w, 0);

    अगर((res_w == LV_STYLE_RES_FOUND && v_w.अंक != w) || res_w == LV_STYLE_RES_NOT_FOUND) {
        lv_obj_set_style_width(obj, w, 0);
    }
}

व्योम lv_obj_set_height(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    lv_style_res_t res_h;
    lv_style_मूल्य_t v_h;

    res_h = lv_obj_get_local_style_prop(obj, LV_STYLE_HEIGHT, &v_h, 0);

    अगर((res_h == LV_STYLE_RES_FOUND && v_h.अंक != h) || res_h == LV_STYLE_RES_NOT_FOUND) {
        lv_obj_set_style_height(obj, h, 0);
    }
}

व्योम lv_obj_set_content_width(lv_obj_t * obj, int32_t w)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    int32_t बायाँ = lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);
    int32_t दायाँ = lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);
    lv_obj_set_width(obj, w + बायाँ + दायाँ);
}

व्योम lv_obj_set_content_height(lv_obj_t * obj, int32_t h)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    int32_t शीर्ष = lv_obj_get_style_space_शीर्ष_internal(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);
    lv_obj_set_height(obj, h + शीर्ष + bottom);
}

व्योम lv_obj_set_layout(lv_obj_t * obj, uint32_t layout)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_set_style_layout(obj, layout, 0);

    lv_obj_mark_layout_as_dirty(obj);
}

bool lv_obj_is_layout_positioned(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    अगर(lv_obj_is_hidden(obj) || lv_obj_is_ignore_layout(obj) || lv_obj_is_floating(obj)) वापस false;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent == शून्य) वापस false;

    uint32_t layout = lv_obj_get_style_layout_internal(parent, LV_PART_MAIN);
    अगर(layout) वापस true;
    अन्यथा वापस false;
}

व्योम lv_obj_mark_layout_as_dirty(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    obj->layout_inv = 1;

    /*Mark the screen as dirty too to mark that there is something to do on this screen*/
    lv_obj_t * scr = lv_obj_get_screen(obj);
    scr->scr_layout_inv = 1;

    /*Make the display refreshing*/
    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, शून्य);
}

व्योम lv_obj_update_layout(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(update_layout_mutex) {
        LV_LOG_TRACE("Already running, returning");
        वापस;
    }
    LV_PROFILER_LAYOUT_BEGIN;
    update_layout_mutex = true;

    lv_obj_t * scr = lv_obj_get_screen(obj);
    /*Repeat until there are no more layout invalidations*/
    uint32_t pass_cnt = 0;
    जबतक(scr->scr_layout_inv) {
        अगर(pass_cnt >= LV_OBJ_LAYOUT_UPDATE_MAX_PASSES) {
            LV_ASSERT_FORMAT_संदेश(false,
                                 "Layout of screen %p (class: '%s') didn't settle in %d passes, giving up. Some "
                                 "sizes probably depend on each other circularly. Please report it on github",
                                 (व्योम *)scr, scr->class_p->name, LV_OBJ_LAYOUT_UPDATE_MAX_PASSES);
            /*Reached only if the assert handler returns*/
            scr->scr_layout_inv = 0;
            अवरोध;
        }
        pass_cnt++;
        LV_LOG_TRACE("Layout update begin");
        scr->scr_layout_inv = 0;
        layout_update_core(scr);
        LV_LOG_TRACE("Layout update end");
    }

    lv_display_t * disp = lv_obj_get_display(scr);
    lv_display_send_event(disp, LV_EVENT_UPDATE_LAYOUT_COMPLETED, शून्य);
    update_layout_mutex = false;
    LV_PROFILER_LAYOUT_END;
}

व्योम lv_obj_set_align(lv_obj_t * obj, lv_align_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_set_style_align(obj, align, 0);
}

व्योम lv_obj_align(lv_obj_t * obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    lv_obj_set_style_align(obj, align, 0);
    lv_obj_set_pos(obj, x_ofs, y_ofs);
}

व्योम lv_obj_align_to(lv_obj_t * obj, स्थिर lv_obj_t * base, lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_update_layout(obj);
    अगर(base == शून्य) base = lv_obj_get_parent(obj);

    अगर(base == शून्य) {
        LV_LOG_WARN("lv_obj_align_to: base is NULL");
        वापस;
    }

    int32_t x = 0;
    int32_t y = 0;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent == शून्य) {
        LV_LOG_WARN("lv_obj_align_to: parent is NULL");
        वापस;
    }

    int32_t pleft = lv_obj_get_style_space_बायाँ_internal(parent, LV_PART_MAIN);
    int32_t ptop = lv_obj_get_style_space_शीर्ष_internal(parent, LV_PART_MAIN);

    int32_t bleft = lv_obj_get_style_space_बायाँ_internal(base, LV_PART_MAIN);
    int32_t btop = lv_obj_get_style_space_शीर्ष_internal(base, LV_PART_MAIN);

    अगर(align == LV_ALIGN_DEFAULT) {
        अगर(lv_obj_get_style_base_dir_internal(base, LV_PART_MAIN) == LV_BASE_DIR_RTL) align = LV_ALIGN_शीर्ष_दायाँ;
        अन्यथा align = LV_ALIGN_शीर्ष_बायाँ;
    }

    चयन(align) {
        हाल LV_ALIGN_CENTER:
            x = lv_obj_get_content_width(base) / 2 - lv_obj_get_width(obj) / 2 + bleft;
            y = lv_obj_get_content_height(base) / 2 - lv_obj_get_height(obj) / 2 + btop;
            अवरोध;

        हाल LV_ALIGN_शीर्ष_बायाँ:
            x = bleft;
            y = btop;
            अवरोध;

        हाल LV_ALIGN_शीर्ष_MID:
            x = lv_obj_get_content_width(base) / 2 - lv_obj_get_width(obj) / 2 + bleft;
            y = btop;
            अवरोध;

        हाल LV_ALIGN_शीर्ष_दायाँ:
            x = lv_obj_get_content_width(base) - lv_obj_get_width(obj) + bleft;
            y = btop;
            अवरोध;

        हाल LV_ALIGN_BOTTOM_बायाँ:
            x = bleft;
            y = lv_obj_get_content_height(base) - lv_obj_get_height(obj) + btop;
            अवरोध;
        हाल LV_ALIGN_BOTTOM_MID:
            x = lv_obj_get_content_width(base) / 2 - lv_obj_get_width(obj) / 2 + bleft;
            y = lv_obj_get_content_height(base) - lv_obj_get_height(obj) + btop;
            अवरोध;

        हाल LV_ALIGN_BOTTOM_दायाँ:
            x = lv_obj_get_content_width(base) - lv_obj_get_width(obj) + bleft;
            y = lv_obj_get_content_height(base) - lv_obj_get_height(obj) + btop;
            अवरोध;

        हाल LV_ALIGN_बायाँ_MID:
            x = bleft;
            y = lv_obj_get_content_height(base) / 2 - lv_obj_get_height(obj) / 2 + btop;
            अवरोध;

        हाल LV_ALIGN_दायाँ_MID:
            x = lv_obj_get_content_width(base) - lv_obj_get_width(obj) + bleft;
            y = lv_obj_get_content_height(base) / 2 - lv_obj_get_height(obj) / 2 + btop;
            अवरोध;

        हाल LV_ALIGN_OUT_शीर्ष_बायाँ:
            x = 0;
            y = -lv_obj_get_height(obj);
            अवरोध;

        हाल LV_ALIGN_OUT_शीर्ष_MID:
            x = lv_obj_get_width(base) / 2 - lv_obj_get_width(obj) / 2;
            y = -lv_obj_get_height(obj);
            अवरोध;

        हाल LV_ALIGN_OUT_शीर्ष_दायाँ:
            x = lv_obj_get_width(base) - lv_obj_get_width(obj);
            y = -lv_obj_get_height(obj);
            अवरोध;

        हाल LV_ALIGN_OUT_BOTTOM_बायाँ:
            x = 0;
            y = lv_obj_get_height(base);
            अवरोध;

        हाल LV_ALIGN_OUT_BOTTOM_MID:
            x = lv_obj_get_width(base) / 2 - lv_obj_get_width(obj) / 2;
            y = lv_obj_get_height(base);
            अवरोध;

        हाल LV_ALIGN_OUT_BOTTOM_दायाँ:
            x = lv_obj_get_width(base) - lv_obj_get_width(obj);
            y = lv_obj_get_height(base);
            अवरोध;

        हाल LV_ALIGN_OUT_बायाँ_शीर्ष:
            x = -lv_obj_get_width(obj);
            y = 0;
            अवरोध;

        हाल LV_ALIGN_OUT_बायाँ_MID:
            x = -lv_obj_get_width(obj);
            y = lv_obj_get_height(base) / 2 - lv_obj_get_height(obj) / 2;
            अवरोध;

        हाल LV_ALIGN_OUT_बायाँ_BOTTOM:
            x = -lv_obj_get_width(obj);
            y = lv_obj_get_height(base) - lv_obj_get_height(obj);
            अवरोध;

        हाल LV_ALIGN_OUT_दायाँ_शीर्ष:
            x = lv_obj_get_width(base);
            y = 0;
            अवरोध;

        हाल LV_ALIGN_OUT_दायाँ_MID:
            x = lv_obj_get_width(base);
            y = lv_obj_get_height(base) / 2 - lv_obj_get_height(obj) / 2;
            अवरोध;

        हाल LV_ALIGN_OUT_दायाँ_BOTTOM:
            x = lv_obj_get_width(base);
            y = lv_obj_get_height(base) - lv_obj_get_height(obj);
            अवरोध;

        हाल LV_ALIGN_DEFAULT:
            अवरोध;
    }

    अगर(LV_COORD_IS_PCT(x_ofs)) x_ofs = (lv_obj_get_width(base) * LV_COORD_GET_PCT(x_ofs)) / 100;
    अगर(LV_COORD_IS_PCT(y_ofs)) y_ofs = (lv_obj_get_height(base) * LV_COORD_GET_PCT(y_ofs)) / 100;
    अगर(lv_obj_get_style_base_dir_internal(parent, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        x += x_ofs + base->coords.x1 - parent->coords.x1 + lv_obj_get_scroll_दायाँ(parent) - pleft;
    }
    अन्यथा {
        x += x_ofs + base->coords.x1 - parent->coords.x1 + lv_obj_get_scroll_बायाँ(parent) - pleft;
    }
    y += y_ofs + base->coords.y1 - parent->coords.y1 + lv_obj_get_scroll_शीर्ष(parent) - ptop;
    lv_obj_set_style_align(obj, LV_ALIGN_शीर्ष_बायाँ, 0);
    lv_obj_set_pos(obj, x, y);

}

व्योम lv_obj_get_coords(स्थिर lv_obj_t * obj, lv_area_t * coords)
{
    LV_CHECK_ARG(coords != शून्य, वापस);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    *coords = obj->coords;
}

int32_t lv_obj_get_x(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t rel_x;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent) {
        rel_x  = obj->coords.x1 - parent->coords.x1;
        rel_x += lv_obj_get_scroll_x(parent);
        rel_x -= lv_obj_get_style_space_बायाँ_internal(parent, LV_PART_MAIN);
    }
    अन्यथा {
        rel_x = obj->coords.x1;
    }
    वापस rel_x;
}

int32_t lv_obj_get_x2(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_obj_get_x(obj) + lv_obj_get_width(obj);
}

int32_t lv_obj_get_y(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t rel_y;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent) {
        rel_y = obj->coords.y1 - parent->coords.y1;
        rel_y += lv_obj_get_scroll_y(parent);
        rel_y -= lv_obj_get_style_space_शीर्ष_internal(parent, LV_PART_MAIN);
    }
    अन्यथा {
        rel_y = obj->coords.y1;
    }
    वापस rel_y;
}

int32_t lv_obj_get_y2(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_obj_get_y(obj) + lv_obj_get_height(obj);
}

int32_t lv_obj_get_x_aligned(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_obj_get_style_x_internal(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_y_aligned(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_obj_get_style_y_internal(obj, LV_PART_MAIN);
}

int32_t lv_obj_get_width(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_area_get_width(&obj->coords);
}

int32_t lv_obj_get_height(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    वापस lv_area_get_height(&obj->coords);
}

int32_t lv_obj_get_content_width(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t बायाँ = lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);
    int32_t दायाँ = lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);

    वापस lv_obj_get_width(obj) - बायाँ - दायाँ;
}

int32_t lv_obj_get_content_height(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t शीर्ष = lv_obj_get_style_space_शीर्ष_internal(obj, LV_PART_MAIN);
    int32_t bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    वापस lv_obj_get_height(obj) - शीर्ष - bottom;
}

व्योम lv_obj_get_content_coords(स्थिर lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != शून्य, वापस);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_get_coords(obj, area);
    area->x1 += lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);
    area->x2 -= lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);
    area->y1 += lv_obj_get_style_space_शीर्ष_internal(obj, LV_PART_MAIN);
    area->y2 -= lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

}

int32_t lv_obj_get_self_width(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    lv_point_t p = {0, LV_COORD_MIN};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_आकार, &p);
    वापस p.x;
}

int32_t lv_obj_get_self_height(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    lv_point_t p = {LV_COORD_MIN, 0};
    lv_obj_send_event((lv_obj_t *)obj, LV_EVENT_GET_SELF_आकार, &p);
    वापस p.y;
}

int32_t lv_obj_get_style_clamped_width(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t content_width = -1;
    int32_t w = calc_dynamic_width(obj, LV_STYLE_WIDTH, &content_width);
    int32_t minw = calc_dynamic_width(obj, LV_STYLE_MIN_WIDTH, &content_width);
    int32_t maxw = calc_dynamic_width(obj, LV_STYLE_MAX_WIDTH, &content_width);
    अगर(w <= minw) {
        w = lv_obj_get_style_min_width_internal(obj, LV_PART_MAIN);
    }
    अन्यथा अगर(w >= maxw) {
        w = lv_obj_get_style_max_width_internal(obj, LV_PART_MAIN);
    }
    अन्यथा {
        w = lv_obj_get_style_width_internal(obj, LV_PART_MAIN);
    }
    वापस w;
}

int32_t lv_obj_get_style_clamped_height(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t content_height = -1;
    int32_t h = calc_dynamic_height(obj, LV_STYLE_HEIGHT, &content_height);
    int32_t minh = calc_dynamic_height(obj, LV_STYLE_MIN_HEIGHT, &content_height);
    int32_t maxh = calc_dynamic_height(obj, LV_STYLE_MAX_HEIGHT, &content_height);
    अगर(h <= minh) {
        h = lv_obj_get_style_min_height_internal(obj, LV_PART_MAIN);
    }
    अन्यथा अगर(h >= maxh) {
        h = lv_obj_get_style_max_height_internal(obj, LV_PART_MAIN);
    }
    अन्यथा {
        h = lv_obj_get_style_height_internal(obj, LV_PART_MAIN);
    }
    वापस h;
}

bool lv_obj_is_style_any_width_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t w = lv_obj_get_style_width_internal(obj, LV_PART_MAIN);
    int32_t minw = lv_obj_get_style_min_width_internal(obj, LV_PART_MAIN);
    int32_t maxw = lv_obj_get_style_max_width_internal(obj, LV_PART_MAIN);
    वापस (w == LV_आकार_CONTENT || minw == LV_आकार_CONTENT || maxw == LV_आकार_CONTENT);
}

bool lv_obj_is_style_any_height_content(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t h = lv_obj_get_style_height_internal(obj, LV_PART_MAIN);
    int32_t minh = lv_obj_get_style_min_height_internal(obj, LV_PART_MAIN);
    int32_t maxh = lv_obj_get_style_max_height_internal(obj, LV_PART_MAIN);
    वापस (h == LV_आकार_CONTENT || minh == LV_आकार_CONTENT || maxh == LV_आकार_CONTENT);
}

bool lv_obj_is_width_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t minw = lv_obj_calc_dynamic_width(obj, LV_STYLE_MIN_WIDTH);
    int32_t w = lv_obj_get_width(obj);
    वापस w == minw;
}

bool lv_obj_is_height_min(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t minh = lv_obj_calc_dynamic_height(obj, LV_STYLE_MIN_HEIGHT);
    int32_t h = lv_obj_get_height(obj);
    वापस h == minh;
}

bool lv_obj_is_width_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t maxw = lv_obj_calc_dynamic_width(obj, LV_STYLE_MAX_WIDTH);
    int32_t w = lv_obj_get_width(obj);
    वापस w == maxw;
}

bool lv_obj_is_height_max(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    int32_t maxh = lv_obj_calc_dynamic_height(obj, LV_STYLE_MAX_HEIGHT);
    int32_t h = lv_obj_get_height(obj);
    वापस h == maxh;
}

bool lv_obj_refresh_self_आकार(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    अगर(!lv_obj_is_style_any_width_content(obj) && !lv_obj_is_style_any_height_content(obj))
        वापस false;

    /**
     * Refresh the parent's layout, because the children size is in some way dependent on its contents we need to force a
     * recalculation of the parents layout
     */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent != शून्य) {
        parent->w_layout = 0;
        parent->h_layout = 0;
        lv_obj_mark_layout_as_dirty(parent);
    }
    lv_obj_mark_layout_as_dirty(obj);
    वापस true;
}

व्योम lv_obj_refr_pos(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(lv_obj_is_layout_positioned(obj)) वापस;

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t x = lv_obj_get_style_x_internal(obj, LV_PART_MAIN);
    int32_t y = lv_obj_get_style_y_internal(obj, LV_PART_MAIN);

    अगर(parent == शून्य) {
        lv_obj_move_to(obj, x, y);
        वापस;
    }

    /*Handle percentage value*/
    int32_t pw = lv_obj_get_content_width(parent);
    int32_t ph = lv_obj_get_content_height(parent);
    अगर(LV_COORD_IS_PCT(x)) {
        अगर(lv_obj_get_style_width_internal(parent, LV_PART_MAIN) == LV_आकार_CONTENT) x = 0; /*Avoid circular dependency*/
        अन्यथा x = (pw * LV_COORD_GET_PCT(x)) / 100;
    }

    अगर(LV_COORD_IS_PCT(y)) {
        अगर(lv_obj_get_style_height_internal(parent, LV_PART_MAIN) == LV_आकार_CONTENT) y = 0; /*Avoid circular dependency*/
        y = (ph * LV_COORD_GET_PCT(y)) / 100;
    }

    /*Handle percentage value of translate*/
    int32_t tr_x = lv_obj_get_style_translate_x_internal(obj, LV_PART_MAIN);
    int32_t tr_y = lv_obj_get_style_translate_y_internal(obj, LV_PART_MAIN);
    int32_t w = lv_obj_get_width(obj);
    int32_t h = lv_obj_get_height(obj);
    अगर(LV_COORD_IS_PCT(tr_x)) tr_x = (w * LV_COORD_GET_PCT(tr_x)) / 100;
    अगर(LV_COORD_IS_PCT(tr_y)) tr_y = (h * LV_COORD_GET_PCT(tr_y)) / 100;

    /*Use the translation*/
    x += tr_x;
    y += tr_y;

    lv_align_t align = lv_obj_get_style_align_internal(obj, LV_PART_MAIN);

    अगर(align == LV_ALIGN_DEFAULT) {
        align = LV_ALIGN_शीर्ष_बायाँ;
    }

    bool rtl = lv_obj_get_style_base_dir_internal(parent, LV_PART_MAIN) == LV_BASE_DIR_RTL;

    अगर(rtl) {
        चयन(align) {
            हाल LV_ALIGN_शीर्ष_बायाँ:
                align = LV_ALIGN_शीर्ष_दायाँ;
                अवरोध;
            हाल LV_ALIGN_शीर्ष_दायाँ:
                align = LV_ALIGN_शीर्ष_बायाँ;
                अवरोध;
            हाल LV_ALIGN_बायाँ_MID:
                align = LV_ALIGN_दायाँ_MID;
                अवरोध;
            हाल LV_ALIGN_दायाँ_MID:
                align = LV_ALIGN_बायाँ_MID;
                अवरोध;
            हाल LV_ALIGN_BOTTOM_बायाँ:
                align = LV_ALIGN_BOTTOM_दायाँ;
                अवरोध;
            हाल LV_ALIGN_BOTTOM_दायाँ:
                align = LV_ALIGN_BOTTOM_बायाँ;
                अवरोध;
            शेष:
                अवरोध;
        }
    }

    चयन(align) {
        हाल LV_ALIGN_शीर्ष_बायाँ:
            अवरोध;
        हाल LV_ALIGN_शीर्ष_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            अवरोध;
        हाल LV_ALIGN_शीर्ष_दायाँ:
            x = rtl ? pw - w - x : pw - w + x;
            अवरोध;
        हाल LV_ALIGN_बायाँ_MID:
            y += ph / 2 - h / 2;
            अवरोध;
        हाल LV_ALIGN_BOTTOM_बायाँ:
            y += ph - h;
            अवरोध;
        हाल LV_ALIGN_BOTTOM_MID:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph - h;
            अवरोध;
        हाल LV_ALIGN_BOTTOM_दायाँ:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph - h;
            अवरोध;
        हाल LV_ALIGN_दायाँ_MID:
            x = rtl ? pw - w - x : pw - w + x;
            y += ph / 2 - h / 2;
            अवरोध;
        हाल LV_ALIGN_CENTER:
            x = rtl ? pw / 2 - w / 2 - x : pw / 2 - w / 2 + x;
            y += ph / 2 - h / 2;
            अवरोध;
        शेष:
            अवरोध;
    }

    lv_obj_move_to(obj, x, y);
}

व्योम lv_obj_move_to(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    /*Convert x and y to absolute coordinates*/
    lv_obj_t * parent = obj->parent;

    अगर(parent) {
        अगर(lv_obj_is_floating(obj)) {
            x += parent->coords.x1;
            y += parent->coords.y1;
        }
        अन्यथा {
            x += parent->coords.x1 - lv_obj_get_scroll_x(parent);
            y += parent->coords.y1 - lv_obj_get_scroll_y(parent);
        }

        x += lv_obj_get_style_space_बायाँ_internal(parent, LV_PART_MAIN);
        y += lv_obj_get_style_space_शीर्ष_internal(parent, LV_PART_MAIN);
    }

    /*Calculate and set the movement*/
    lv_point_t diff;
    diff.x = x - obj->coords.x1;
    diff.y = y - obj->coords.y1;

    /*Do nothing if the position is not changed*/
    /*It is very important else recursive positioning can
     *occur without position change*/
    अगर(diff.x == 0 && diff.y == 0) वापस;

    /*Invalidate the original area*/
    lv_obj_invalidate(obj);

    /*Save the original coordinates*/
    lv_area_t ori;
    lv_obj_get_coords(obj, &ori);

    /*Check if the object inside the parent or not*/
    lv_area_t parent_fit_area;
    bool on1 = false;
    अगर(parent) {
        lv_obj_get_content_coords(parent, &parent_fit_area);

        /*If the object is already out of the parent and its position is changes
         *surely the scrollbars also changes so invalidate them*/
        on1 = lv_area_is_in(&ori, &parent_fit_area, 0);
        अगर(!on1) lv_obj_scrollbar_invalidate(parent);
    }

    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    lv_obj_move_children_by(obj, diff.x, diff.y, false);

    /*Call the ancestor's event handler to the parent too*/
    अगर(parent) lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);

    /*Invalidate the new area*/
    lv_obj_invalidate(obj);

    /* Invalidate if the object wasn't inside the parent before the move
    * or isn't inside it now. If it stayed inside, the scrollbars cannot change. */
    अगर(parent) {
        bool on2 = lv_area_is_in(&obj->coords, &parent_fit_area, 0);
        अगर(!on1 || !on2) lv_obj_scrollbar_invalidate(parent);
    }
}

व्योम lv_obj_move_children_by(lv_obj_t * obj, int32_t x_diff, int32_t y_diff, bool ignore_floating)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        अगर(ignore_floating && lv_obj_is_floating(child)) जारी;
        child->coords.x1 += x_diff;
        child->coords.y1 += y_diff;
        child->coords.x2 += x_diff;
        child->coords.y2 += y_diff;

        lv_obj_move_children_by(child, x_diff, y_diff, false);
    }
}

व्योम lv_obj_transform_point(स्थिर lv_obj_t * obj, lv_point_t * p, lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(p != शून्य, वापस);

    lv_obj_transform_point_array(obj, p, 1, flags);
}

व्योम lv_obj_transform_point_array(स्थिर lv_obj_t * obj, lv_point_t points[], माप_प्रकार गिनती,
                                  lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(points != शून्य || गिनती == 0, वापस);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    bool do_tranf = layer_type == LV_LAYER_TYPE_TRANSFORM;
    bool recursive = flags & LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE;
    bool inverse = flags & LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE;
    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(inverse) {
        अगर(recursive && parent) lv_obj_transform_point_array(parent, points, गिनती, flags);
        अगर(do_tranf) transform_point_array(obj, points, गिनती, inverse);
    }
    अन्यथा {
        अगर(do_tranf) transform_point_array(obj, points, गिनती, inverse);
        अगर(recursive && parent) lv_obj_transform_point_array(parent, points, गिनती, flags);
    }
}

व्योम lv_obj_get_transformed_area(स्थिर lv_obj_t * obj, lv_area_t * area, lv_obj_point_transform_flag_t flags)
{
    LV_CHECK_ARG(area != शून्य, वापस);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

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

अटल lv_obj_वृक्ष_walk_res_t blur_expand_walk_cb(lv_obj_t * obj, व्योम * user_तथ्य)
{
    अगर(!obj->has_blur) वापस LV_OBJ_वृक्ष_WALK_अग्र;

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
    अगर(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, &scr_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    }

    lv_display_t * disp = user_तथ्य;
    uint32_t i;
    क्रम(i = 0; i < disp->inv_p; i++) {
        /*The join pass runs after this and its state is reset each frame*/
        LV_ASSERT(!disp->inv_area_joined[i]);
        अगर(lv_area_is_on(&disp->inv_areas[i], &scr_coords)) {
            invalidate_area_core(obj, &obj_coords);

            /*No need to check the children as the widget is already invalidated
             *which will redraw the children too*/
            वापस LV_OBJ_वृक्ष_WALK_SKIP_CHILDREN;
        }
    }

    वापस LV_OBJ_वृक्ष_WALK_अग्र;
}

व्योम lv_obj_invalidate_expand_blur(lv_display_t * disp)
{
    अगर(disp->inv_p == 0) वापस;

    /*There is nothing to expand if no widget has blur or drop shadow*/
    अगर(LV_GLOBAL_DEFAULT()->blur_obj_cnt == 0) वापस;

    uint32_t पूर्व_inv_p;
    करो {
        पूर्व_inv_p = disp->inv_p;

        lv_obj_वृक्ष_walk(disp->act_scr, blur_expand_walk_cb, disp);
        अगर(disp->पूर्व_scr) lv_obj_वृक्ष_walk(disp->पूर्व_scr, blur_expand_walk_cb, disp);
        lv_obj_वृक्ष_walk(disp->sys_layer, blur_expand_walk_cb, disp);
        lv_obj_वृक्ष_walk(disp->शीर्ष_layer, blur_expand_walk_cb, disp);
        lv_obj_वृक्ष_walk(disp->bottom_layer, blur_expand_walk_cb, disp);

        /*Repeat while new areas keep being added. An overflow in lv_inv_area()
         *can instead shrink inv_p (it collapses to a single whole-screen area)
         *and the loop exits, which is correct because the whole screen covers
         *every blur object.*/
    } जबतक(disp->inv_p > पूर्व_inv_p);
}

lv_परिणाम_t lv_obj_invalidate_area(स्थिर lv_obj_t * obj, स्थिर lv_area_t * area)
{
    LV_CHECK_ARG(area != शून्य, वापस LV_परिणाम_INVALID);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_परिणाम_INVALID);

    lv_display_t * disp   = lv_obj_get_display(obj);
    अगर(!lv_display_is_invalidation_enabled(disp)) वापस LV_परिणाम_INVALID;

    /*If there are blurred or drop-shadow parts the whole widget needs to be invalidated
     *as these can't be calculated partially. */
    अगर(obj->has_blur) वापस lv_obj_invalidate(obj);
    अन्यथा वापस obj_invalidate_area_internal(obj, area);
}


lv_परिणाम_t lv_obj_invalidate(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_परिणाम_INVALID);

    lv_display_t * disp = lv_obj_get_display(obj);
    अगर(!lv_display_is_invalidation_enabled(disp)) वापस LV_परिणाम_INVALID;

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_आकार;
    obj_coords.y1 -= ext_आकार;
    obj_coords.x2 += ext_आकार;
    obj_coords.y2 += ext_आकार;

    lv_परिणाम_t res = obj_invalidate_area_internal(obj, &obj_coords);

    वापस res;
}

bool lv_obj_area_is_visible(स्थिर lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != शून्य, वापस false);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    अगर(lv_obj_is_hidden(obj)) वापस false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    अगर(obj_scr != lv_display_get_screen_active(disp) &&
       obj_scr != lv_display_get_screen_पूर्व(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_शीर्ष(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        वापस false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_आकार, ext_आकार);

    /*The area is not on the object*/
    अगर(!lv_area_intersect(area, area, &obj_coords)) वापस false;

    अगर(is_transformed(obj)) {
        lv_obj_get_transformed_area(obj, area, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    }

    /*Truncate recursively to the parents*/
    lv_obj_t * parent = lv_obj_get_parent(obj);
    जबतक(parent != शून्य) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        अगर(lv_obj_is_hidden(parent)) वापस false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t parent_coords = parent->coords;
        अगर(lv_obj_is_overflow_visible(parent)) {
            int32_t parent_ext_आकार = lv_obj_get_ext_draw_आकार(parent);
            lv_area_increase(&parent_coords, parent_ext_आकार, parent_ext_आकार);
        }

        अगर(is_transformed(parent)) {
            lv_obj_get_transformed_area(parent, &parent_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
        }
        अगर(!lv_area_intersect(area, area, &parent_coords)) वापस false;

        parent = lv_obj_get_parent(parent);
    }

    वापस true;
}

bool lv_obj_is_visible(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    lv_area_t obj_coords;
    int32_t ext_आकार = lv_obj_get_ext_draw_आकार(obj);
    obj_coords = obj->coords;
    obj_coords.x1 -= ext_आकार;
    obj_coords.y1 -= ext_आकार;
    obj_coords.x2 += ext_आकार;
    obj_coords.y2 += ext_आकार;

    वापस lv_obj_area_is_visible(obj, &obj_coords);
}

व्योम lv_obj_set_ext_click_area(lv_obj_t * obj, int32_t आकार)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }
    obj->spec_attr->ext_click_pad = आकार;
}

व्योम lv_obj_get_click_area(स्थिर lv_obj_t * obj, lv_area_t * area)
{
    LV_CHECK_ARG(area != शून्य, वापस);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    *area = obj->coords;
    अगर(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_hit_test(lv_obj_t * obj, स्थिर lv_point_t * point)
{
    LV_CHECK_ARG(point != शून्य, वापस false);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    अगर(!lv_obj_is_clickable(obj)) वापस false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_point_on(&a, point, 0);
    अगर(res == false) वापस false;

    अगर(lv_obj_is_adv_hittest(obj)) {
        lv_hit_test_info_t hit_info;
        hit_info.point = point;
        hit_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_HIT_TEST, &hit_info);
        वापस hit_info.res;
    }

    वापस res;
}

int32_t lv_clamp_width(int32_t width, int32_t min_width, int32_t max_width, int32_t ref_width)
{
    अगर(LV_COORD_IS_PCT(min_width)) min_width = (ref_width * LV_COORD_GET_PCT(min_width)) / 100;
    अगर(LV_COORD_IS_PCT(max_width)) max_width = (ref_width * LV_COORD_GET_PCT(max_width)) / 100;
    वापस LV_CLAMP(min_width, width, max_width);
}

int32_t lv_clamp_height(int32_t height, int32_t min_height, int32_t max_height, int32_t ref_height)
{
    अगर(LV_COORD_IS_PCT(min_height)) min_height = (ref_height * LV_COORD_GET_PCT(min_height)) / 100;
    अगर(LV_COORD_IS_PCT(max_height)) max_height = (ref_height * LV_COORD_GET_PCT(max_height)) / 100;
    वापस LV_CLAMP(min_height, height, max_height);
}

व्योम lv_obj_center(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
}

व्योम lv_obj_set_transform(lv_obj_t * obj, स्थिर lv_matrix_t * matrix)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    अगर(!matrix) {
        lv_obj_reset_transform(obj);
        वापस;
    }

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }

    अगर(!obj->spec_attr->matrix) {
        obj->spec_attr->matrix = lv_malloc(माप(lv_matrix_t));
        LV_ASSERT_MALLOC(obj->spec_attr->matrix);
        अगर(obj->spec_attr->matrix == शून्य) वापस;
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
    LV_UNUSED(obj);
    LV_UNUSED(matrix);
    LV_LOG_WARN("Transform matrix is not used because LV_DRAW_TRANSFORM_USE_MATRIX is disabled");
#पूर्ण_अगर
}

व्योम lv_obj_reset_transform(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    अगर(!obj->spec_attr) {
        वापस;
    }

    अगर(!obj->spec_attr->matrix) {
        वापस;
    }

    /* Invalidate the old area */
    lv_obj_invalidate(obj);

    /* Free the matrix */
    lv_free(obj->spec_attr->matrix);
    obj->spec_attr->matrix = शून्य;

    /* Matrix is cleared. Update the layer type */
    lv_obj_update_layer_type(obj);

    /* Invalidate the new area */
    lv_obj_invalidate(obj);
#अन्यथा
    LV_UNUSED(obj);
#पूर्ण_अगर
}

स्थिर lv_matrix_t * lv_obj_get_transform(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    अगर(obj->spec_attr) {
        वापस obj->spec_attr->matrix;
    }
#अन्यथा
    LV_UNUSED(obj);
#पूर्ण_अगर
    वापस शून्य;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल lv_परिणाम_t obj_invalidate_area_internal(स्थिर lv_obj_t * obj, स्थिर lv_area_t * area)
{
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(area);

    lv_area_t area_tmp;
    area_tmp = *area;

    वापस invalidate_area_core(obj, &area_tmp);
}

अटल bool is_transformed(स्थिर lv_obj_t * obj)
{
    जबतक(obj) {
        अगर(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) वापस true;
        obj = obj->parent;
    }
    वापस false;
}

अटल int32_t calc_content_width(lv_obj_t * obj)
{
    int32_t scroll_x_tmp = lv_obj_get_scroll_x(obj);
    अगर(obj->spec_attr) obj->spec_attr->scroll.x = 0;

    int32_t space_दायाँ = lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);
    int32_t space_बायाँ = lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj) + space_बायाँ + space_दायाँ;

    int32_t child_res = LV_COORD_MIN;

    अगर(!lv_layout_get_min_आकार(obj, &child_res, true)) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
        /*With RTL find the left most coordinate*/
        अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
            क्रम(i = 0; i < child_cnt; i++) {
                int32_t child_res_tmp = LV_COORD_MIN;
                lv_obj_t * child = obj->spec_attr->children[i];
                अगर(child->hidden || child->floating)
                    जारी;

                अगर(child->w_ignore_आकार)
                    जारी;

                अगर(!lv_obj_is_layout_positioned(child)) {
                    lv_align_t align = lv_obj_get_style_align_internal(child, LV_PART_MAIN);
                    चयन(align) {
                        हाल LV_ALIGN_DEFAULT:
                        हाल LV_ALIGN_शीर्ष_दायाँ:
                        हाल LV_ALIGN_BOTTOM_दायाँ:
                        हाल LV_ALIGN_दायाँ_MID:
                            /*Normal right aligns. Other are ignored due to possible circular dependencies*/
                            child_res_tmp = obj->coords.x2 - child->coords.x1 + 1;
                            अवरोध;
                        शेष:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            अगर(lv_obj_get_style_x_internal(child, LV_PART_MAIN) == 0) {
                                child_res_tmp = lv_area_get_width(&child->coords) + space_दायाँ;
                                child_res_tmp += lv_obj_get_style_margin_बायाँ_internal(child, LV_PART_MAIN);
                            }
                            अवरोध;
                    }
                }
                अन्यथा {
                    child_res_tmp = obj->coords.x2 - child->coords.x1 + 1;
                }
                child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_बायाँ_internal(child, LV_PART_MAIN));
            }
            अगर(child_res != LV_COORD_MIN) {
                child_res += space_बायाँ;
            }
        }
        /*Else find the right most coordinate*/
        अन्यथा {
            क्रम(i = 0; i < child_cnt; i++) {
                int32_t child_res_tmp = LV_COORD_MIN;
                lv_obj_t * child = obj->spec_attr->children[i];
                अगर(child->hidden || child->floating) जारी;

                अगर(child->w_ignore_आकार)
                    जारी;

                अगर(!lv_obj_is_layout_positioned(child)) {
                    lv_align_t align = lv_obj_get_style_align_internal(child, LV_PART_MAIN);
                    चयन(align) {
                        हाल LV_ALIGN_DEFAULT:
                        हाल LV_ALIGN_शीर्ष_बायाँ:
                        हाल LV_ALIGN_BOTTOM_बायाँ:
                        हाल LV_ALIGN_बायाँ_MID:
                            /*Normal left aligns.*/
                            child_res_tmp = child->coords.x2 - obj->coords.x1 + 1;
                            अवरोध;
                        शेष:
                            /* Consider other cases only if x=0 and use the width of the object.
                             * With x!=0 circular dependency could occur. */
                            अगर(lv_obj_get_style_x_internal(child, LV_PART_MAIN) == 0) {
                                child_res_tmp = lv_area_get_width(&child->coords) + space_बायाँ;
                                child_res_tmp += lv_obj_get_style_margin_दायाँ_internal(child, LV_PART_MAIN);
                            }
                            अवरोध;
                    }
                }
                अन्यथा {
                    child_res_tmp = child->coords.x2 - obj->coords.x1 + 1;
                }

                child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_दायाँ_internal(child, LV_PART_MAIN));
            }

            अगर(child_res != LV_COORD_MIN) {
                child_res += space_दायाँ;
            }
        }
    }

    अगर(obj->spec_attr) {
        obj->spec_attr->scroll.x = -scroll_x_tmp;
    }

    अगर(child_res == LV_COORD_MIN) {
        वापस self_w;
    }

    वापस LV_MAX(child_res, self_w);
}

अटल int32_t calc_content_height(lv_obj_t * obj)
{
    int32_t scroll_y_tmp = lv_obj_get_scroll_y(obj);
    अगर(obj->spec_attr) obj->spec_attr->scroll.y = 0;

    int32_t space_शीर्ष = lv_obj_get_style_space_शीर्ष_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    int32_t self_h;
    self_h = lv_obj_get_self_height(obj) + space_शीर्ष + space_bottom;

    int32_t child_res = LV_COORD_MIN;

    अगर(!lv_layout_get_min_आकार(obj, &child_res, false)) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
        क्रम(i = 0; i < child_cnt; i++) {
            int32_t child_res_tmp = LV_COORD_MIN;
            lv_obj_t * child = obj->spec_attr->children[i];
            अगर(child->hidden || child->floating) जारी;

            अगर(child->h_ignore_आकार)
                जारी;

            अगर(!lv_obj_is_layout_positioned(child)) {
                lv_align_t align = lv_obj_get_style_align_internal(child, LV_PART_MAIN);
                चयन(align) {
                    हाल LV_ALIGN_DEFAULT:
                    हाल LV_ALIGN_शीर्ष_दायाँ:
                    हाल LV_ALIGN_शीर्ष_MID:
                    हाल LV_ALIGN_शीर्ष_बायाँ:
                        /*Normal top aligns. */
                        child_res_tmp = child->coords.y2 - obj->coords.y1 + 1;
                        अवरोध;
                    शेष:
                        /* Consider other cases only if y=0 and use the height of the object.
                         * With y!=0 circular dependency could occur. */
                        अगर(lv_obj_get_style_y_internal(child, LV_PART_MAIN) == 0) {
                            child_res_tmp = lv_area_get_height(&child->coords) + space_शीर्ष;
                            child_res_tmp += lv_obj_get_style_margin_शीर्ष_internal(child, LV_PART_MAIN);
                        }
                        अवरोध;
                }
            }
            अन्यथा {
                child_res_tmp = child->coords.y2 - obj->coords.y1 + 1;
            }

            child_res = LV_MAX(child_res, child_res_tmp + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN));
        }

        अगर(child_res != LV_COORD_MIN) {
            child_res += space_bottom;
        }
    }

    अगर(obj->spec_attr) {
        obj->spec_attr->scroll.y = -scroll_y_tmp;
    }

    अगर(child_res == LV_COORD_MIN) {
        वापस self_h;
    }

    वापस LV_MAX(self_h, child_res);
}

अटल व्योम layout_update_core(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        layout_update_core(child);
    }

    अगर(obj->layout_inv) {
        obj->layout_inv = 0;
        lv_obj_refr_आकार(obj);
        lv_obj_refr_pos(obj);

        अगर(child_cnt > 0) {
            lv_layout_apply(obj);
        }
    }

    अगर(obj->readjust_scroll_after_layout) {
        obj->readjust_scroll_after_layout = 0;
        lv_obj_readjust_scroll(obj, LV_ANIM_OFF);
    }
}

अटल व्योम transform_point_array(स्थिर lv_obj_t * obj, lv_point_t * p, माप_प्रकार p_गिनती, bool inv)
{
#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    स्थिर lv_matrix_t * obj_matrix = lv_obj_get_transform(obj);
    अगर(obj_matrix) {
        lv_matrix_t m;
        lv_matrix_identity(&m);
        lv_matrix_translate(&m, obj->coords.x1, obj->coords.y1);
        lv_matrix_multiply(&m, obj_matrix);
        lv_matrix_translate(&m, -obj->coords.x1, -obj->coords.y1);

        अगर(inv) {
            lv_matrix_t inv_m;
            lv_matrix_inverse(&inv_m, &m);
            m = inv_m;
        }

        क्रम(माप_प्रकार i = 0; i < p_गिनती; i++) {
            lv_point_precise_t p_precise = lv_point_to_precise(&p[i]);
            lv_point_precise_t res = lv_matrix_transform_precise_point(&m, &p_precise);
            p[i] = lv_point_from_precise(&res);
        }

        वापस;
    }
#पूर्ण_अगर /* LV_DRAW_TRANSFORM_USE_MATRIX */

    int32_t angle = lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_safe_internal(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_safe_internal(obj, LV_PART_MAIN);
    अगर(scale_x == 0) scale_x = 1;
    अगर(scale_y == 0) scale_y = 1;

    अगर(angle == 0 && scale_x == LV_SCALE_NONE && scale_y == LV_SCALE_NONE) वापस;

    lv_point_t pivot = {
        .x = lv_obj_get_style_transform_pivot_x_internal(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_pivot_y_internal(obj, LV_PART_MAIN)
    };

    अगर(LV_COORD_IS_PCT(pivot.x)) {
        pivot.x = (LV_COORD_GET_PCT(pivot.x) * lv_area_get_width(&obj->coords)) / 100;
    }
    अगर(LV_COORD_IS_PCT(pivot.y)) {
        pivot.y = (LV_COORD_GET_PCT(pivot.y) * lv_area_get_height(&obj->coords)) / 100;
    }

    pivot.x = obj->coords.x1 + pivot.x;
    pivot.y = obj->coords.y1 + pivot.y;

    अगर(inv) {
        angle = -angle;
        scale_x = (256 * 256 + scale_x - 1) / scale_x;
        scale_y = (256 * 256 + scale_y - 1) / scale_y;
    }

    lv_point_array_transform(p, p_गिनती, angle, scale_x, scale_y, &pivot, !inv);
}

अटल lv_परिणाम_t invalidate_area_core(स्थिर lv_obj_t * obj, lv_area_t * area_tmp)
{
    अगर(!lv_obj_area_is_visible(obj, area_tmp)) वापस LV_परिणाम_INVALID;
#अगर LV_DRAW_TRANSFORM_USE_MATRIX
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(area_tmp, 5, 5);
#अन्यथा
    अगर(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(area_tmp, 5, 5);
    }
#पूर्ण_अगर

    lv_परिणाम_t res = lv_inv_area(lv_obj_get_display(obj), area_tmp);
    वापस res;
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
अटल int32_t calc_dynamic_width(lv_obj_t * obj, lv_style_prop_t prop, int32_t * स्थिर content_width)
{
    LV_ASSERT(prop == LV_STYLE_WIDTH || prop == LV_STYLE_MIN_WIDTH || prop == LV_STYLE_MAX_WIDTH);

    int32_t width = lv_obj_get_style_prop(obj, 0, prop).अंक;

    अगर(width == LV_आकार_CONTENT) {
        अगर(content_width == शून्य) {
            width = calc_content_width(obj);
        }
        अन्यथा {
            अगर(*content_width < 0) {
                *content_width = calc_content_width(obj);
            }
            width = *content_width;
        }
    }
    अन्यथा अगर(LV_COORD_IS_PCT(width)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        int32_t parent_w = lv_obj_get_content_width(parent);
        width = (LV_COORD_GET_PCT(width) * parent_w) / 100;
        width -= lv_obj_get_style_margin_बायाँ_internal(obj, LV_PART_MAIN) + lv_obj_get_style_margin_दायाँ_internal(obj,
                                                                                                                   LV_PART_MAIN);
    }
    वापस width;
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
अटल int32_t calc_dynamic_height(lv_obj_t * obj, lv_style_prop_t prop, int32_t * स्थिर content_height)
{
    LV_ASSERT(prop == LV_STYLE_HEIGHT || prop == LV_STYLE_MIN_HEIGHT || prop == LV_STYLE_MAX_HEIGHT);

    int32_t height = lv_obj_get_style_prop(obj, 0, prop).अंक;

    अगर(height == LV_आकार_CONTENT) {
        अगर(content_height == शून्य) {
            height = calc_content_height(obj);
        }
        अन्यथा {
            अगर(*content_height < 0) {
                *content_height = calc_content_height(obj);
            }
            height = *content_height;
        }
    }
    अन्यथा अगर(LV_COORD_IS_PCT(height)) {
        lv_obj_t * parent = lv_obj_get_parent(obj);
        int32_t parent_h = lv_obj_get_content_height(parent);
        height = (LV_COORD_GET_PCT(height) * parent_h) / 100;
        height -= lv_obj_get_style_margin_शीर्ष_internal(obj, LV_PART_MAIN) + lv_obj_get_style_margin_bottom_internal(obj,
                                                                                                                    LV_PART_MAIN);
    }
    वापस height;
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
अटल bool आकार_in_effect_is_pct(int32_t unclamped, int32_t min, int32_t max, int32_t आकार_style,
                                  int32_t min_style, int32_t max_style)
{
    /*If the bounds are inverted the clamp always returns the min size*/
    अगर(min > max) वापस LV_COORD_IS_PCT(min_style);

    /*The size is set by the bound which clamped it*/
    अगर(unclamped < min) वापस LV_COORD_IS_PCT(min_style);
    अगर(unclamped > max) वापस LV_COORD_IS_PCT(max_style);

    /*Else the size is set by the size style. If it's equal to a bound, that bound sets the same
     *size, so use the style which is not a percentage. Percentages are left out of the parent's
     *content size, so choosing a different style in each pass would make the layout never settle.*/
    bool is_pct = LV_COORD_IS_PCT(आकार_style);
    अगर(is_pct && unclamped == min) is_pct = LV_COORD_IS_PCT(min_style);
    अगर(is_pct && unclamped == max) is_pct = LV_COORD_IS_PCT(max_style);

    वापस is_pct;
}
