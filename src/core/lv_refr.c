/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_refr_private.h"
#ضمن "lv_obj_draw_private.h"
#ضمن "../misc/lv_area_private.h"
#ضمن "../draw/sw/lv_draw_sw_mask_private.h"
#ضمن "lv_obj_private.h"
#ضمن "lv_obj_event_private.h"
#ضمن "../display/lv_display_private.h"
#ضمن "../misc/lv_timer_private.h"
#ضمن "../draw/lv_draw_private.h"
#ضمن "lv_global.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/

/*Display being refreshed*/
#تعریف_کرو disp_refr LV_GLOBAL_DEFAULT()->disp_refresh

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_refr_join_area(فراغ);
ساكن فراغ refr_invalid_areas(فراغ);
ساكن فراغ refr_sync_areas(فراغ);
ساكن فراغ refr_area(ثابت lv_area_t * area_p, int32_t y_وتھ);
ساكن فراغ refr_configured_layer(lv_layer_t * layer);
ساكن فراغ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * چوٹی_obj);
ساكن uint32_t get_max_صف(lv_display_t * disp, int32_t area_w, int32_t area_h);
ساكن فراغ draw_buf_flush(lv_display_t * disp);
ساكن فراغ call_flush_cb(lv_display_t * disp, ثابت lv_area_t * area, uint8_t * px_map);
ساكن فراغ wait_for_flushing(lv_display_t * disp);
ساكن فراغ call_sync_cb(lv_display_t * disp, ثابت lv_area_t * area);
ساكن فراغ wait_for_syncing(lv_display_t * disp);
ساكن lv_نتیجہ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_باہرلا, lv_area_t * obj_draw_ناپ_باہرلا);
ساكن bool alpha_test_area_on_obj(lv_obj_t * obj, ثابت lv_area_t * area);
#إذا LV_DRAW_TRANSFORM_USE_میٹرکس
    ساكن bool refr_check_obj_clip_حد_توں_ودھ(lv_layer_t * layer, lv_obj_t * obj);
    ساكن فراغ refr_obj_میٹرکس(lv_layer_t * layer, lv_obj_t * obj);
#جے_انت

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#إذا LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #تعریف_کرو LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#وإلا
    #تعریف_کرو LV_TRACE_REFR(...)
#جے_انت

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
فراغ lv_refr_init(فراغ)
{
}

فراغ lv_refr_deinit(فراغ)
{
}

فراغ lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    إذا(disp) {
        إذا(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    وإلا {
        lv_display_t * d;
        d = lv_display_get_اگلا(عدم);
        طالما(d) {
            إذا(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_اگلا(d);
        }
    }
}

فراغ lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_PROFILER_REFR_BEGIN;
    lv_area_t clip_area_ori = layer->_clip_area;
    lv_area_t clip_coords_for_obj;

    /*The widget will be rendered.
     *So setters from now should use animations. */
    obj->rendered = 1;

    /*Truncate the clip area to `obj size + ext size` area*/
    lv_area_t obj_coords_ext;
    lv_obj_get_coords(obj, &obj_coords_ext);
    int32_t ext_draw_ناپ = lv_obj_get_ext_draw_ناپ(obj);
    lv_area_increase(&obj_coords_ext, ext_draw_ناپ, ext_draw_ناپ);

    إذا(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_سرا;
        أرجع;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_سرا, layer);
#إذا LV_USE_REFR_DEBUG
    lv_color_t debug_color = lv_color_make(lv_rand(0, 0xFF), lv_rand(0, 0xFF), lv_rand(0, 0xFF));
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = debug_color;
    draw_dsc.bg_opa = LV_OPA_20;
    draw_dsc.border_چوڑائی = 1;
    draw_dsc.border_opa = LV_OPA_30;
    draw_dsc.border_color = debug_color;
    lv_draw_rect(layer, &draw_dsc, &obj_coords_ext);
#جے_انت

    ثابت lv_area_t * obj_coords;
    إذا(lv_obj_is_حد_توں_ودھ_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    وإلا {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    إذا(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    إذا(refr_children) {
        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
        إذا(بچہ_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_سرا, layer);
        }
        وإلا {
            layer->_clip_area = clip_coords_for_children;
            bool clip_کونا = lv_obj_get_style_clip_کونا_اندرونی(obj, LV_PART_MAIN);

            int32_t radius = 0;
            إذا(clip_کونا) {
                radius = lv_obj_get_style_radius_اندرونی(obj, LV_PART_MAIN);
                إذا(radius == 0) clip_کونا = false;
            }

            إذا(clip_کونا == false) {
                لكل(i = 0; i < بچہ_cnt; i++) {
                    lv_obj_t * بچہ = obj->spec_attr->children[i];
                    lv_obj_refr(layer, بچہ);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_سرا, layer);
            }
            وإلا {
                lv_layer_t * layer_children;
                lv_draw_نقاب_rect_dsc_t نقاب_draw_dsc;
                lv_draw_نقاب_rect_dsc_init(&نقاب_draw_dsc);
                نقاب_draw_dsc.radius = radius;
                نقاب_draw_dsc.area = obj->coords;

                lv_draw_image_dsc_t img_draw_dsc;
                lv_draw_image_dsc_init(&img_draw_dsc);

                int32_t short_side = LV_MIN(lv_area_get_چوڑائی(&obj->coords), lv_area_get_اچائی(&obj->coords));
                int32_t rout = LV_MIN(radius, short_side >> 1);

                lv_area_t bottom = obj->coords;
                bottom.y1 = bottom.y2 - rout + 1;
                إذا(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    لكل(i = 0; i < بچہ_cnt; i++) {
                        lv_obj_t * بچہ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, بچہ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_سرا, layer_children);

                    lv_draw_نقاب_rect(layer_children, &نقاب_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &bottom);
                }

                lv_area_t چوٹی = obj->coords;
                چوٹی.y2 = چوٹی.y1 + rout - 1;
                إذا(lv_area_intersect(&چوٹی, &چوٹی, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &چوٹی);

                    لكل(i = 0; i < بچہ_cnt; i++) {
                        lv_obj_t * بچہ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, بچہ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_سرا, layer_children);

                    lv_draw_نقاب_rect(layer_children, &نقاب_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &چوٹی);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                إذا(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    لكل(i = 0; i < بچہ_cnt; i++) {
                        lv_obj_t * بچہ = obj->spec_attr->children[i];
                        lv_obj_refr(layer, بچہ);
                    }

                    /*If all the children are redrawn make 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_سرا, layer);

                }

            }
        }
    }

    layer->_clip_area = clip_area_ori;

    LV_PROFILER_REFR_سرا;
}

lv_نتیجہ_t lv_inv_area(lv_display_t * disp, ثابت lv_area_t * area_p)
{
    إذا(!disp) أرجع LV_نتیجہ_INVALID;
    إذا(!lv_display_is_invalidation_enabled(disp)) أرجع LV_نتیجہ_INVALID;

    /**
     * There are two reasons for this issue:
     *  1.LVGL API is being used across threads, such as modifying widget properties in another thread
     *    or within an interrupt handler during the main thread rendering process.
     *  2.User-customized widget modify widget properties/styles again within the DRAW event.
     *
     * Therefore, ensure that LVGL is used in a single-threaded manner, or refer to
     * documentation: https://docs.lvgl.io/master/porting/os.html for proper locking mechanisms.
     * Additionally, ensure that only drawing-related tasks are performed within the DRAW event,
     * and move widget property/style modifications to other events.
     */
    LV_ASSERT_پیغام(!disp->rendering_وچ_ترقی, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    إذا(area_p == عدم) {
        disp->inv_p = 0;
        أرجع LV_نتیجہ_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    إذا(suc == false)  أرجع LV_نتیجہ_INVALID; /*Out of the screen*/

    إذا(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    إذا(disp->render_منوال == LV_DISPLAY_RENDER_منوال_بھریا) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, عدم);
        أرجع LV_نتیجہ_OK;
    }

    lv_نتیجہ_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    إذا(res != LV_نتیجہ_OK) أرجع LV_نتیجہ_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    لكل(i = 0; i < disp->inv_p; i++) {
        إذا(lv_area_is_وچ(&com_area, &disp->inv_areas[i], 0) != false) أرجع LV_نتیجہ_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    إذا(disp->inv_p >= LV_INV_BUF_ناپ) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, عدم);

    أرجع LV_نتیجہ_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(فراغ)
{
    أرجع disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
فراغ lv_refr_سیٹ_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

فراغ lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    إذا(timer) {
        disp_refr = timer->user_انکڑے;
    }
    وإلا {
        disp_refr = lv_display_get_default();
    }

    إذا(disp_refr == عدم) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_سرا;
        أرجع;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    إذا(!(buf_act && buf_act->انکڑے && buf_act->انکڑے_ناپ)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_سرا;
        أرجع;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#إذا !LV_USE_PERF_MONITOR
    إذا(timer) lv_timer_pause(timer);
#جے_انت

    lv_نتیجہ_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_شروع, عدم);
    إذا(res == LV_نتیجہ_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_سرا;
        أرجع;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_ٹیگ("layout");
    إذا(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    إذا(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->چوٹی_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_سرا_ٹیگ("layout");

    /*Do nothing if there is no active screen*/
    إذا(disp_refr->act_scr == عدم) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        اذهب refr_انت;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    إذا(disp_refr->inv_p == 0) اذهب refr_انت;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    إذا((lv_display_is_double_buffered(disp_refr) && disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_سدھا) ||
       disp_refr->sync_cb) {
        uint32_t i;
        لكل(i = 0; i < disp_refr->inv_p; i++) {
            إذا(disp_refr->inv_area_joined[i])
                استمر;

            lv_area_t * sync_area = lv_ll_ins_پوچھ(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, حجم(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, حجم(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_انت:

#إذا LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_نقاب_cleanup();
#جے_انت

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, عدم);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_سرا;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_چوٹی_obj(ثابت lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != عدم, أرجع عدم);
    LV_CHECK_ARG(obj != عدم, أرجع عدم);

    lv_obj_t * لبھیا_p = عدم;

    إذا(lv_area_is_وچ(area_p, &obj->coords, 0) == false) أرجع عدم;
    إذا(lv_obj_is_hidden(obj)) أرجع عدم;
    إذا(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) أرجع عدم;
    إذا(lv_obj_get_style_opa_اندرونی(obj, LV_PART_MAIN) < LV_OPA_MAX) أرجع عدم;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    إذا(info.res == LV_COVER_RES_MASKED) أرجع عدم;

    int32_t i;
    int32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    لكل(i = بچہ_cnt - 1; i >= 0; i--) {
        lv_obj_t * بچہ = obj->spec_attr->children[i];
        لبھیا_p = lv_refr_get_چوٹی_obj(area_p, بچہ);

        /*If a children is ok then break*/
        إذا(لبھیا_p != عدم) {
            اكسر;
        }
    }

    /*If no better children use this object*/
    إذا(لبھیا_p == عدم && info.res == LV_COVER_RES_COVER) {
        لبھیا_p = obj;
    }

    أرجع لبھیا_p;
}


فراغ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    إذا(lv_obj_is_hidden(obj)) أرجع;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    ثابت lv_opa_t opa_layered = lv_obj_get_style_opa_layered_اندرونی(obj, LV_PART_MAIN);
    إذا(opa_layered <= LV_OPA_MIN) أرجع;

    إذا(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) أرجع;
    إذا(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) أرجع;

    ثابت lv_opa_t layer_opa_ori = layer->opa;
    ثابت lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    ثابت lv_opa_t opa_main = lv_obj_get_style_opa_اندرونی(obj, LV_PART_MAIN);
    إذا(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    إذا(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#إذا LV_DRAW_TRANSFORM_USE_میٹرکس
    /*If the layer opa is full then use the matrix transform*/
    وإلا إذا(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_حد_توں_ودھ(layer, obj)) {
        refr_obj_میٹرکس(layer, obj);
    }
#جے_انت /* LV_DRAW_TRANSFORM_USE_MATRIX */
    وإلا {
        lv_area_t layer_area_بھریا;
        lv_area_t obj_draw_ناپ;
        lv_نتیجہ_t res = layer_get_area(layer, obj, layer_type, &layer_area_بھریا, &obj_draw_ناپ);
        إذا(res != LV_نتیجہ_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            أرجع;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_صف_اچائی = lv_area_get_اچائی(&layer_area_بھریا);
        uint32_t max_argb_صف_اچائی = lv_area_get_اچائی(&layer_area_بھریا);
        إذا(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_چوڑائی(&layer_area_بھریا);
            uint8_t px_ناپ = lv_color_format_get_ناپ(disp_refr->color_format);
            max_rgb_صف_اچائی = LV_DRAW_LAYER_SIMPLE_BUF_ناپ / w / px_ناپ;
            max_argb_صف_اچائی = LV_DRAW_LAYER_SIMPLE_BUF_ناپ / w / حجم(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_بھریا.x1;
        layer_area_act.x2 = layer_area_بھریا.x2;
        layer_area_act.y1 = layer_area_بھریا.y1;
        layer_area_act.y2 = layer_area_بھریا.y1;

        طالما(layer_area_act.y2 < layer_area_بھریا.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_صف_اچائی - 1;
            إذا(layer_area_act.y2 > layer_area_بھریا.y2) layer_area_act.y2 = layer_area_بھریا.y2;

            ثابت فراغ * bitmap_نقاب_src = lv_obj_get_style_bitmap_نقاب_src_اندرونی(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_نقاب_src || alpha_test_area_on_obj(obj, &layer_area_act);

            إذا(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_صف_اچائی - 1;
                إذا(layer_area_act.y2 > layer_area_بھریا.y2) layer_area_act.y2 = layer_area_بھریا.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

            lv_نقطہ_t دھرا = {
                .x = lv_obj_get_style_transform_دھرا_x_اندرونی(obj, LV_PART_MAIN),
                .y = lv_obj_get_style_transform_دھرا_y_اندرونی(obj, LV_PART_MAIN)
            };

            إذا(LV_COORD_IS_PCT(دھرا.x)) {
                دھرا.x = (LV_COORD_GET_PCT(دھرا.x) * lv_area_get_چوڑائی(&obj->coords)) / 100;
            }
            إذا(LV_COORD_IS_PCT(دھرا.y)) {
                دھرا.y = (LV_COORD_GET_PCT(دھرا.y) * lv_area_get_اچائی(&obj->coords)) / 100;
            }

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.دھرا.x = obj->coords.x1 + دھرا.x - new_layer->buf_area.x1;
            layer_draw_dsc.دھرا.y = obj->coords.y1 + دھرا.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_اندرونی(obj, LV_PART_MAIN);
            طالما(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            طالما(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
            layer_draw_dsc.scale_x = lv_obj_get_style_transform_scale_x_اندرونی(obj, LV_PART_MAIN);
            layer_draw_dsc.scale_y = lv_obj_get_style_transform_scale_y_اندرونی(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_x = lv_obj_get_style_transform_skew_x_اندرونی(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_y = lv_obj_get_style_transform_skew_y_اندرونی(obj, LV_PART_MAIN);
            layer_draw_dsc.blend_منوال = lv_obj_get_style_blend_منوال_اندرونی(obj, LV_PART_MAIN);
            layer_draw_dsc.antialias = disp_refr->antialiasing;
            layer_draw_dsc.bitmap_نقاب_src = bitmap_نقاب_src;
            layer_draw_dsc.image_area = obj_draw_ناپ;
            layer_draw_dsc.src = new_layer;

            lv_draw_layer(layer, &layer_draw_dsc, &layer_area_act);

            layer_area_act.y1 = layer_area_act.y2 + 1;
        }
    }

    /* Restore the original layer opa and recolor */
    layer->opa = layer_opa_ori;
    layer->recolor = layer_recolor;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Join the areas which has got common parts
 */
ساكن فراغ lv_refr_join_area(فراغ)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_وچ;
    lv_area_t joined_area;
    لكل(join_وچ = 0; join_وچ < disp_refr->inv_p; join_وچ++) {
        إذا(disp_refr->inv_area_joined[join_وچ] != 0) استمر;

        /*Check all areas to join them in 'join_in'*/
        لكل(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            إذا(disp_refr->inv_area_joined[join_from] != 0 || join_وچ == join_from) {
                استمر;
            }

            /*Check if the areas are on each other*/
            إذا(lv_area_is_on(&disp_refr->inv_areas[join_وچ], &disp_refr->inv_areas[join_from]) == false) {
                استمر;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_وچ], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            إذا(lv_area_get_ناپ(&joined_area) < (lv_area_get_ناپ(&disp_refr->inv_areas[join_وچ]) +
                                                 lv_area_get_ناپ(&disp_refr->inv_areas[join_from]))) {
                disp_refr->inv_areas[join_وچ] = joined_area;

                /*Mark 'join_form' is joined into 'join_in'*/
                disp_refr->inv_area_joined[join_from] = 1;
            }
        }
    }
    LV_PROFILER_REFR_سرا;
}

/**
 * Refresh the sync areas
 */
ساكن فراغ refr_sync_areas(فراغ)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    ثابت bool auto_sync = disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_سدھا &&
                           lv_display_is_double_buffered(disp_refr);
    ثابت bool user_sync = disp_refr->sync_cb != عدم;
    إذا(!auto_sync && !user_sync) أرجع;

    /*Do not sync if no sync areas*/
    إذا(lv_ll_is_خالی_تھاواں(&disp_refr->sync_areas)) أرجع;

    LV_PROFILER_REFR_BEGIN;
    /*With double buffered direct mode synchronize the rendered areas to the other buffer*/
    /*We need to wait for ready here to not mess up the active screen*/
    wait_for_flushing(disp_refr);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    uint16_t i;
    int8_t j;
    lv_area_t res[4] = {0};
    int8_t res_c;
    lv_area_t * sync_area, * new_area, * اگلا_area;
    لكل(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        إذا(disp_refr->inv_area_joined[i]) استمر;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_سر(&disp_refr->sync_areas);
        طالما(sync_area != عدم) {
            /*Get next sync area*/
            اگلا_area = lv_ll_get_اگلا(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            إذا(res_c != -1) {
                /*Replace old sync area with new areas*/
                لكل(j = 0; j < res_c; j++) {
                    new_area = lv_ll_ins_prev(&disp_refr->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp_refr->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = اگلا_area;
        }
    }

    /*The buffers are already swapped.
     *So the active buffer is the off screen buffer where LVGL will render*/
    lv_draw_buf_t * off_screen = disp_refr->buf_act;
    /*Triple buffer sync buffer for off-screen2 updates.*/
    lv_draw_buf_t * off_screen2 = عدم;
    lv_draw_buf_t * on_screen = عدم;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    إذا(auto_sync) {
        إذا(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        وإلا إذا(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        وإلا {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    لكل(sync_area = lv_ll_get_سر(&disp_refr->sync_areas); sync_area != عدم;
        sync_area = lv_ll_get_اگلا(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        إذا(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            استمر;
        }
#إذا LV_DRAW_TRANSFORM_USE_میٹرکس
        إذا(lv_display_get_میٹرکس_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#جے_انت /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        إذا(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_آخری = lv_ll_get_پوچھ(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        وإلا {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            إذا(off_screen2 != on_screen)
                lv_draw_buf_copy(off_screen2, sync_area, on_screen, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp_refr->sync_areas);
    LV_PROFILER_REFR_سرا;
}

/**
 * Refresh the joined areas
 */
ساكن فراغ refr_invalid_areas(فراغ)
{
    إذا(disp_refr->inv_p == 0) أرجع;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_شروع, عدم);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t آخری_i = 0;
    لكل(i = disp_refr->inv_p - 1; i >= 0; i--) {
        إذا(disp_refr->inv_area_joined[i] == 0) {
            آخری_i = i;
            اكسر;
        }
    }

    disp_refr->آخری_area = 0;
    disp_refr->آخری_part = 0;
    disp_refr->rendering_وچ_ترقی = true;

    لكل(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        إذا(disp_refr->inv_area_joined[i]) استمر;

        إذا(i == آخری_i) disp_refr->آخری_area = 1;
        disp_refr->آخری_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        إذا(disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_PARTIAL) {
            /*Calculate the max row num*/
            int32_t w = lv_area_get_چوڑائی(&inv_a);
            int32_t h = lv_area_get_اچائی(&inv_a);

            int32_t max_صف = get_max_صف(disp_refr, w, h);

            int32_t صف;
            int32_t صف_آخری = 0;
            lv_area_t sub_area;
            sub_area.x1 = inv_a.x1;
            sub_area.x2 = inv_a.x2;
            int32_t y_off = 0;
            لكل(صف = inv_a.y1; صف + max_صف - 1 <= inv_a.y2; صف += max_صف) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = صف;
                sub_area.y2 = صف + max_صف - 1;
                إذا(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                صف_آخری = sub_area.y2;
                إذا(inv_a.y2 == صف_آخری) disp_refr->آخری_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_اچائی(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            إذا(inv_a.y2 != صف_آخری) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = صف;
                sub_area.y2 = inv_a.y2;
                disp_refr->آخری_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_اچائی(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        وإلا إذا(disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_بھریا ||
                disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_سدھا) {
            disp_refr->آخری_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, عدم);
    disp_refr->rendering_وچ_ترقی = false;
    LV_PROFILER_REFR_سرا;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
ساكن فراغ layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
{
    lv_draw_buf_t * ret = lv_draw_buf_reshape(
                              layer->draw_buf,
                              layer->color_format,
                              lv_area_get_چوڑائی(&layer->buf_area),
                              lv_area_get_اچائی(&layer->buf_area),
                              stride);
    LV_بن_ورتیا(ret);
    LV_ASSERT_NULL(ret);
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
ساكن فراغ refr_area(ثابت lv_area_t * area_p, int32_t y_وتھ)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_سر;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_وتھ = y_وتھ;
    layer->all_tasks_added = false;

    إذا(disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    وإلا إذا(disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_سدھا ||
            disp_refr->render_منوال == LV_DISPLAY_RENDER_منوال_بھریا) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        إذا(LV_DRAW_TRANSFORM_USE_میٹرکس && lv_display_get_میٹرکس_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        وإلا {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->سرخی.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_اچائی(area_p);
    إذا(LV_COLOR_FORMAT_IS_اشاریہ_دار(layer->color_format) == false) {
        /* Assume that the buffer size (can be screen sized or smaller in case of partial mode)
         * and max tile size are the optimal scenario. From this calculate the ideal tile size
         * and set the tile count and tile height accordingly.
         */
        uint32_t max_tile_cnt = disp_refr->tile_cnt;
        uint32_t کل_buf_ناپ = layer->draw_buf->انکڑے_ناپ;
        uint32_t ideal_tile_ناپ = کل_buf_ناپ / max_tile_cnt;
        uint32_t area_buf_ناپ = lv_area_get_ناپ(area_p) * lv_color_format_get_ناپ(layer->color_format);

        tile_cnt = (area_buf_ناپ + (ideal_tile_ناپ - 1)) / ideal_tile_ناپ; /*Round up*/
        tile_h = lv_area_get_اچائی(area_p) / tile_cnt;
    }

    إذا(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    وإلا {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * حجم(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        إذا(tile_layers == عدم) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_سرا;
            أرجع;
        }
        uint32_t i;
        لكل(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_سیٹ(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            إذا(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, عدم, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        لكل(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            طالما(tile_layer->draw_task_سر) {
                lv_draw_dispatch_wait_for_request();
                lv_draw_dispatch();
            }
            lv_draw_layer_deinit(tile_layer);
        }
        lv_free(tile_layers);

        layer->all_tasks_added = true;
    }

    disp_refr->refreshed_area = *area_p;
    LV_PROFILER_REFR_سرا;
}

ساكن فراغ refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#إذا LV_DRAW_TRANSFORM_USE_میٹرکس
    إذا(lv_display_get_میٹرکس_rotation(disp_refr)) {
        ثابت lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        إذا(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            بدل(rotation) {
                حالة LV_ROTATION_90:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 270);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->ver_res, 0);
                     */
                    layer->میٹرکس.m[0][0] = 0;
                    layer->میٹرکس.m[0][1] = 1;
                    layer->میٹرکس.m[0][2] = 0;
                    layer->میٹرکس.m[1][0] = -1;
                    layer->میٹرکس.m[1][1] = 0;
                    layer->میٹرکس.m[1][2] = disp_refr->ver_res;
                    اكسر;

                حالة LV_ROTATION_180:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 180);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->hor_res, -disp_refr->ver_res);
                     */
                    layer->میٹرکس.m[0][0] = -1;
                    layer->میٹرکس.m[0][1] = 0;
                    layer->میٹرکس.m[0][2] = disp_refr->hor_res;
                    layer->میٹرکس.m[1][0] = 0;
                    layer->میٹرکس.m[1][1] = -1;
                    layer->میٹرکس.m[1][2] = disp_refr->ver_res;
                    اكسر;

                حالة LV_ROTATION_270:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 90);
                     * lv_matrix_translate(&layer->matrix, 0, -disp_refr->hor_res);
                     */
                    layer->میٹرکس.m[0][0] = 0;
                    layer->میٹرکس.m[0][1] = -1;
                    layer->میٹرکس.m[0][2] = disp_refr->hor_res;
                    layer->میٹرکس.m[1][0] = 1;
                    layer->میٹرکس.m[1][1] = 0;
                    layer->میٹرکس.m[1][2] = 0;
                    اكسر;

                افتراضي:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    اكسر;
            }
        }
    }
#جے_انت /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    إذا(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    إذا(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * چوٹی_act_scr = عدم;
    lv_obj_t * چوٹی_prev_scr = عدم;

    /*Get the most top object which is not covered by others*/
    چوٹی_act_scr = lv_refr_get_چوٹی_obj(&layer->_clip_area, lv_display_get_screen_سرگرم(disp_refr));
    إذا(disp_refr->prev_scr) {
        چوٹی_prev_scr = lv_refr_get_چوٹی_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    إذا(چوٹی_act_scr == عدم && چوٹی_prev_scr == عدم) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    إذا(disp_refr->draw_prev_over_act) {
        إذا(چوٹی_act_scr == عدم) چوٹی_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, چوٹی_act_scr);

        /*Refresh the previous screen if any*/
        إذا(disp_refr->prev_scr) {
            إذا(چوٹی_prev_scr == عدم) چوٹی_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, چوٹی_prev_scr);
        }
    }
    وإلا {
        /*Refresh the previous screen if any*/
        إذا(disp_refr->prev_scr) {
            إذا(چوٹی_prev_scr == عدم) چوٹی_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, چوٹی_prev_scr);
        }

        إذا(چوٹی_act_scr == عدم) چوٹی_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, چوٹی_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_چوٹی(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_سرا;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
ساكن فراغ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * چوٹی_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    إذا(چوٹی_obj == عدم) چوٹی_obj = lv_display_get_screen_سرگرم(disp_refr);
    إذا(چوٹی_obj == عدم) أرجع;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * ماپا;
    lv_obj_t * border_p = چوٹی_obj;

    ماپا = lv_obj_get_ماپا(چوٹی_obj);

    ثابت lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    إذا(ماپا) {
        layer->recolor = lv_obj_get_style_recolor_recursive(ماپا, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, چوٹی_obj);

    /*Do until not reach the screen*/
    طالما(ماپا != عدم) {
        bool go = false;
        uint32_t i;
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(ماپا);
        لكل(i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = ماپا->spec_attr->children[i];
            إذا(!go) {
                إذا(بچہ == border_p) go = true;
            }
            وإلا {
                /*Refresh the objects*/
                lv_obj_refr(layer, بچہ);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(ماپا, LV_EVENT_DRAW_POST_BEGIN, (فراغ *)layer);
        lv_obj_send_event(ماپا, LV_EVENT_DRAW_POST, (فراغ *)layer);
        lv_obj_send_event(ماپا, LV_EVENT_DRAW_POST_سرا, (فراغ *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = ماپا;
        /*Go a level deeper*/
        ماپا = lv_obj_get_ماپا(ماپا);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_سرا;
}

ساكن lv_نتیجہ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_باہرلا, lv_area_t * obj_draw_ناپ_باہرلا)
{
    int32_t ext_draw_ناپ = lv_obj_get_ext_draw_ناپ(obj);
    lv_obj_get_coords(obj, obj_draw_ناپ_باہرلا);
    lv_area_increase(obj_draw_ناپ_باہرلا, ext_draw_ناپ, ext_draw_ناپ);

    إذا(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_ناپ_باہرلا;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_نقطہ_TRANSFORM_FLAG_NONE);
        إذا(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            أرجع LV_نتیجہ_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_نقطہ_TRANSFORM_FLAG_INVERSE);
        إذا(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_ناپ_باہرلا)) {
            أرجع LV_نتیجہ_INVALID;
        }

        *layer_area_باہرلا = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_باہرلا, 5, 5); /*To avoid rounding error*/
    }
    وإلا إذا(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        إذا(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_ناپ_باہرلا)) {
            أرجع LV_نتیجہ_INVALID;
        }
        *layer_area_باہرلا = clip_coords_for_obj;
    }
    وإلا {
        LV_LOG_WARN("Unhandled layer type");
        أرجع LV_نتیجہ_INVALID;
    }

    أرجع LV_نتیجہ_OK;
}

ساكن bool alpha_test_area_on_obj(lv_obj_t * obj, ثابت lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    إذا(!lv_area_is_on(area, &obj->coords)) أرجع true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    إذا(info.res == LV_COVER_RES_COVER) أرجع false;
    وإلا أرجع true;
}

#إذا LV_DRAW_TRANSFORM_USE_میٹرکس

ساكن bool obj_get_میٹرکس(lv_obj_t * obj, lv_میٹرکس_t * میٹرکس)
{
    lv_میٹرکس_پچھان_تت(میٹرکس);

    ثابت lv_میٹرکس_t * obj_میٹرکس = lv_obj_get_transform(obj);
    إذا(obj_میٹرکس) {
        lv_میٹرکس_translate(میٹرکس, obj->coords.x1, obj->coords.y1);
        lv_میٹرکس_multiply(میٹرکس, obj_میٹرکس);
        lv_میٹرکس_translate(میٹرکس, -obj->coords.x1, -obj->coords.y1);
        أرجع true;
    }

    lv_نقطہ_t دھرا = {
        .x = lv_obj_get_style_transform_دھرا_x_اندرونی(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_دھرا_y_اندرونی(obj, LV_PART_MAIN)
    };

    دھرا.x = obj->coords.x1 + lv_pct_منزل_px(دھرا.x, lv_area_get_چوڑائی(&obj->coords));
    دھرا.y = obj->coords.y1 + lv_pct_منزل_px(دھرا.y, lv_area_get_اچائی(&obj->coords));

    int32_t rotation = lv_obj_get_style_transform_rotation_اندرونی(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_اندرونی(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_اندرونی(obj, LV_PART_MAIN);
    int32_t skew_x = lv_obj_get_style_transform_skew_x_اندرونی(obj, LV_PART_MAIN);
    int32_t skew_y = lv_obj_get_style_transform_skew_y_اندرونی(obj, LV_PART_MAIN);

    /* Checked by caller */
    LV_ASSERT(scale_x > 0 && scale_y > 0);

    /* generate the obj matrix */
    lv_میٹرکس_translate(میٹرکس, دھرا.x, دھرا.y);
    إذا(rotation != 0) {
        lv_میٹرکس_rotate(میٹرکس, rotation * 0.1f);
    }

    إذا(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_میٹرکس_scale(
            میٹرکس,
            (عائم)scale_x / LV_SCALE_NONE,
            (عائم)scale_y / LV_SCALE_NONE
        );
    }

    إذا(skew_x != 0 || skew_y != 0) {
        lv_میٹرکس_skew(میٹرکس, skew_x, skew_y);
    }

    lv_میٹرکس_translate(میٹرکس, -دھرا.x, -دھرا.y);
    أرجع true;
}

ساكن فراغ refr_obj_میٹرکس(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_میٹرکس_t obj_میٹرکس;
    إذا(!obj_get_میٹرکس(obj, &obj_میٹرکس)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_سرا;
        أرجع;
    }

    lv_میٹرکس_t میٹرکس_inv;
    إذا(!lv_میٹرکس_inverse(&میٹرکس_inv, &obj_میٹرکس)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_سرا;
        أرجع;
    }

    /* save original matrix */
    lv_میٹرکس_t ori_میٹرکس = layer->میٹرکس;

    /* apply the obj matrix */
    lv_میٹرکس_multiply(&layer->میٹرکس, &obj_میٹرکس);

    /* calculate clip area without transform */
    lv_area_t clip_area = layer->_clip_area;
    lv_area_t clip_area_ori = layer->_clip_area;
    clip_area = lv_میٹرکس_transform_area(&میٹرکس_inv, &clip_area);

    /* increase the clip area by 1 pixel to avoid rounding errors */
    إذا(!lv_میٹرکس_is_پچھان_تت_or_translation(&obj_میٹرکس)) {
        lv_area_increase(&clip_area, 1, 1);
    }

    layer->_clip_area = clip_area;

    /* redraw obj */
    lv_obj_redraw(layer, obj);

    /* restore original matrix */
    layer->میٹرکس = ori_میٹرکس;
    /* restore clip area */
    layer->_clip_area = clip_area_ori;
    LV_PROFILER_REFR_سرا;
}

ساكن bool refr_check_obj_clip_حد_توں_ودھ(lv_layer_t * layer, lv_obj_t * obj)
{
    إذا(lv_obj_get_style_transform_rotation_اندرونی(obj, LV_PART_MAIN) == 0) {
        أرجع false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ناپ = lv_obj_get_ext_draw_ناپ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ناپ, ext_ناپ);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_نقطہ_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    إذا(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        أرجع true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, حجم(lv_area_t)) != 0;
    أرجع has_clip;
}

#جے_انت /* LV_DRAW_TRANSFORM_USE_MATRIX */

ساكن uint32_t get_max_صف(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_چوڑائی_منزل_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_اشاریہ_دار_PALETTE_ناپ(cf) * حجم(lv_color32_t);

    إذا(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        أرجع 0;
    }

    int32_t max_صف = (uint32_t)(disp->buf_act->انکڑے_ناپ - overhead) / stride;

    إذا(max_صف > area_h) max_صف = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_صف;
    افعل {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        إذا(lv_area_get_اچائی(&tmp) <= max_صف) اكسر;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } طالما(h_tmp > 0);

    إذا(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        أرجع 0;
    }
    وإلا {
        max_صف = tmp.y2 + 1;
    }

    أرجع max_صف;
}

/**
 * Flush the content of the draw buffer
 */
ساكن فراغ draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_سر;

    طالما(layer->draw_task_سر) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_پیغام(layer->اگلا == عدم, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    إذا(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    إذا(disp->آخری_area && disp->آخری_part) disp->flushing_آخری = 1;
    وإلا disp->flushing_آخری = 0;

    bool flushing_آخری = disp->flushing_آخری;

    إذا(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->انکڑے);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    إذا(lv_display_is_double_buffered(disp) && (disp->render_منوال != LV_DISPLAY_RENDER_منوال_سدھا || flushing_آخری)) {
        إذا(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        وإلا إذا(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        وإلا {
            disp->buf_act = disp->buf_1;
        }
    }
}

ساكن فراغ call_flush_cb(lv_display_t * disp, ثابت lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (صحيح)area->x1, (صحيح)area->y1, (صحيح)area->x2, (صحيح)area->y2, (فراغ *)px_map);

    lv_area_t وتھ_area = {
        .x1 = area->x1 + disp->وتھ_x,
        .y1 = area->y1 + disp->وتھ_y,
        .x2 = area->x2 + disp->وتھ_x,
        .y2 = area->y2 + disp->وتھ_y
    };

    lv_display_send_event(disp, LV_EVENT_FLUSH_شروع, &وتھ_area);

    disp->flush_cb(disp, &وتھ_area, px_map);
    lv_display_send_event(disp, LV_EVENT_FLUSH_انت, &وتھ_area);

    LV_PROFILER_REFR_سرا;
}

ساكن فراغ wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_شروع, عدم);

    إذا(disp->flush_wait_cb) {
        إذا(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    وإلا {
        طالما(disp->flushing);
    }
    disp->flushing_آخری = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_انت, عدم);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_سرا;
}

ساكن فراغ call_sync_cb(lv_display_t * disp, ثابت lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t وتھ_area = *area;
    وتھ_area.x1 += disp->وتھ_x;
    وتھ_area.x2 += disp->وتھ_x;
    وتھ_area.y1 += disp->وتھ_y;
    وتھ_area.y2 += disp->وتھ_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (صحيح)وتھ_area.x1, (صحيح)وتھ_area.y1,
                  (صحيح)وتھ_area.x2, (صحيح)وتھ_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_شروع, (فراغ *)&وتھ_area);

    disp->sync_cb(disp, &وتھ_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_انت, (فراغ *)&وتھ_area);

    LV_PROFILER_REFR_سرا;
}

ساكن فراغ wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_شروع, عدم);

    إذا(disp->sync_wait_cb) {
        إذا(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    وإلا {
        طالما(disp->syncing);
    }
    disp->syncing_آخری = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_انت, عدم);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_سرا;
}
