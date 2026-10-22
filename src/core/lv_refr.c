/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "lv_refr_private.h"
#അന്തര്ഭാവ "lv_obj_draw_private.h"
#അന്തര്ഭാവ "../misc/lv_area_private.h"
#അന്തര്ഭാവ "../draw/sw/lv_draw_sw_mask_private.h"
#അന്തര്ഭാവ "lv_obj_private.h"
#അന്തര്ഭാവ "lv_obj_event_private.h"
#അന്തര്ഭാവ "../display/lv_display_private.h"
#അന്തര്ഭാവ "../misc/lv_timer_private.h"
#അന്തര്ഭാവ "../draw/lv_draw_private.h"
#അന്തര്ഭാവ "lv_global.h"
#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/

/*Display being refreshed*/
#define disp_refr LV_GLOBAL_DEFAULT()->disp_refresh

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
സ്ഥിതം ശൂന്യം lv_refr_join_area(ശൂന്യം);
സ്ഥിതം ശൂന്യം refr_invalid_areas(ശൂന്യം);
സ്ഥിതം ശൂന്യം refr_sync_areas(ശൂന്യം);
സ്ഥിതം ശൂന്യം refr_area(സ്ഥിരം lv_area_t * area_p, int32_t y_offset);
സ്ഥിതം ശൂന്യം refr_configured_layer(lv_layer_t * layer);
സ്ഥിതം ശൂന്യം refr_obj_and_children(lv_layer_t * layer, lv_obj_t * മുകൾ_obj);
സ്ഥിതം uint32_t get_max_row(lv_display_t * disp, int32_t area_w, int32_t area_h);
സ്ഥിതം ശൂന്യം draw_buf_flush(lv_display_t * disp);
സ്ഥിതം ശൂന്യം call_flush_cb(lv_display_t * disp, സ്ഥിരം lv_area_t * area, uint8_t * px_map);
സ്ഥിതം ശൂന്യം wait_for_flushing(lv_display_t * disp);
സ്ഥിതം ശൂന്യം call_sync_cb(lv_display_t * disp, സ്ഥിരം lv_area_t * area);
സ്ഥിതം ശൂന്യം wait_for_syncing(lv_display_t * disp);
സ്ഥിതം lv_result_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_out, lv_area_t * obj_draw_size_out);
സ്ഥിതം bool alpha_test_area_on_obj(lv_obj_t * obj, സ്ഥിരം lv_area_t * area);
#എങ്കിൽ LV_DRAW_TRANSFORM_USE_MATRIX
    സ്ഥിതം bool refr_check_obj_clip_overflow(lv_layer_t * layer, lv_obj_t * obj);
    സ്ഥിതം ശൂന്യം refr_obj_matrix(lv_layer_t * layer, lv_obj_t * obj);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#എങ്കിൽ LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #define LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#അല്ലെങ്കിൽ
    #define LV_TRACE_REFR(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
ശൂന്യം lv_refr_init(ശൂന്യം)
{
}

ശൂന്യം lv_refr_deinit(ശൂന്യം)
{
}

ശൂന്യം lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    എങ്കിൽ(disp) {
        എങ്കിൽ(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    അല്ലെങ്കിൽ {
        lv_display_t * d;
        d = lv_display_get_next(ശൂന്യ);
        ആകുമ്പോൾ(d) {
            എങ്കിൽ(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_next(d);
        }
    }
}

ശൂന്യം lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);

    LV_PROFILER_REFR_BEGIN;
    lv_area_t clip_area_ori = layer->_clip_area;
    lv_area_t clip_coords_for_obj;

    /*The widget will be rendered.
     *So setters from now should use animations. */
    obj->rendered = 1;

    /*Truncate the clip area to `obj size + ext size` area*/
    lv_area_t obj_coords_ext;
    lv_obj_get_coords(obj, &obj_coords_ext);
    int32_t ext_draw_size = lv_obj_get_ext_draw_size(obj);
    lv_area_increase(&obj_coords_ext, ext_draw_size, ext_draw_size);

    എങ്കിൽ(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_END;
        തിരികെ;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_END, layer);
#എങ്കിൽ LV_USE_REFR_DEBUG
    lv_color_t debug_color = lv_color_make(lv_rand(0, 0xFF), lv_rand(0, 0xFF), lv_rand(0, 0xFF));
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = debug_color;
    draw_dsc.bg_opa = LV_OPA_20;
    draw_dsc.border_width = 1;
    draw_dsc.border_opa = LV_OPA_30;
    draw_dsc.border_color = debug_color;
    lv_draw_rect(layer, &draw_dsc, &obj_coords_ext);
#endif

    സ്ഥിരം lv_area_t * obj_coords;
    എങ്കിൽ(lv_obj_is_overflow_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    അല്ലെങ്കിൽ {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    എങ്കിൽ(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    എങ്കിൽ(refr_children) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_എണ്ണം(obj);
        എങ്കിൽ(child_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer);
        }
        അല്ലെങ്കിൽ {
            layer->_clip_area = clip_coords_for_children;
            bool clip_corner = lv_obj_get_style_clip_corner_internal(obj, LV_PART_MAIN);

            int32_t radius = 0;
            എങ്കിൽ(clip_corner) {
                radius = lv_obj_get_style_radius_internal(obj, LV_PART_MAIN);
                എങ്കിൽ(radius == 0) clip_corner = false;
            }

            എങ്കിൽ(clip_corner == false) {
                വേണ്ടി(i = 0; i < child_cnt; i++) {
                    lv_obj_t * child = obj->spec_attr->children[i];
                    lv_obj_refr(layer, child);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer);
            }
            അല്ലെങ്കിൽ {
                lv_layer_t * layer_children;
                lv_draw_mask_rect_dsc_t mask_draw_dsc;
                lv_draw_mask_rect_dsc_init(&mask_draw_dsc);
                mask_draw_dsc.radius = radius;
                mask_draw_dsc.area = obj->coords;

                lv_draw_image_dsc_t img_draw_dsc;
                lv_draw_image_dsc_init(&img_draw_dsc);

                int32_t short_side = LV_MIN(lv_area_get_width(&obj->coords), lv_area_get_height(&obj->coords));
                int32_t rout = LV_MIN(radius, short_side >> 1);

                lv_area_t bottom = obj->coords;
                bottom.y1 = bottom.y2 - rout + 1;
                എങ്കിൽ(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    വേണ്ടി(i = 0; i < child_cnt; i++) {
                        lv_obj_t * child = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, child);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer_children);

                    lv_draw_mask_rect(layer_children, &mask_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &bottom);
                }

                lv_area_t മുകൾ = obj->coords;
                മുകൾ.y2 = മുകൾ.y1 + rout - 1;
                എങ്കിൽ(lv_area_intersect(&മുകൾ, &മുകൾ, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &മുകൾ);

                    വേണ്ടി(i = 0; i < child_cnt; i++) {
                        lv_obj_t * child = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, child);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer_children);

                    lv_draw_mask_rect(layer_children, &mask_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &മുകൾ);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                എങ്കിൽ(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    വേണ്ടി(i = 0; i < child_cnt; i++) {
                        lv_obj_t * child = obj->spec_attr->children[i];
                        lv_obj_refr(layer, child);
                    }

                    /*If all the children are redrawn make 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer);

                }

            }
        }
    }

    layer->_clip_area = clip_area_ori;

    LV_PROFILER_REFR_END;
}

lv_result_t lv_inv_area(lv_display_t * disp, സ്ഥിരം lv_area_t * area_p)
{
    എങ്കിൽ(!disp) തിരികെ LV_RESULT_INVALID;
    എങ്കിൽ(!lv_display_is_invalidation_enabled(disp)) തിരികെ LV_RESULT_INVALID;

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
    LV_ASSERT_സന്ദേശം(!disp->rendering_in_progress, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    എങ്കിൽ(area_p == ശൂന്യ) {
        disp->inv_p = 0;
        തിരികെ LV_RESULT_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    എങ്കിൽ(suc == false)  തിരികെ LV_RESULT_INVALID; /*Out of the screen*/

    എങ്കിൽ(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    എങ്കിൽ(disp->render_mode == LV_DISPLAY_RENDER_MODE_FULL) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ശൂന്യ);
        തിരികെ LV_RESULT_OK;
    }

    lv_result_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    എങ്കിൽ(res != LV_RESULT_OK) തിരികെ LV_RESULT_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    വേണ്ടി(i = 0; i < disp->inv_p; i++) {
        എങ്കിൽ(lv_area_is_in(&com_area, &disp->inv_areas[i], 0) != false) തിരികെ LV_RESULT_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    എങ്കിൽ(disp->inv_p >= LV_INV_BUF_SIZE) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ശൂന്യ);

    തിരികെ LV_RESULT_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(ശൂന്യം)
{
    തിരികെ disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
ശൂന്യം lv_refr_set_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

ശൂന്യം lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    എങ്കിൽ(timer) {
        disp_refr = timer->user_data;
    }
    അല്ലെങ്കിൽ {
        disp_refr = lv_display_get_default();
    }

    എങ്കിൽ(disp_refr == ശൂന്യ) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_END;
        തിരികെ;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    എങ്കിൽ(!(buf_act && buf_act->data && buf_act->data_size)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_END;
        തിരികെ;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#എങ്കിൽ !LV_USE_PERF_MONITOR
    എങ്കിൽ(timer) lv_timer_pause(timer);
#endif

    lv_result_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_START, ശൂന്യ);
    എങ്കിൽ(res == LV_RESULT_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_END;
        തിരികെ;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_TAG("layout");
    എങ്കിൽ(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    എങ്കിൽ(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->മുകൾ_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_END_TAG("layout");

    /*Do nothing if there is no active screen*/
    എങ്കിൽ(disp_refr->act_scr == ശൂന്യ) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        പോകുക refr_finish;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    എങ്കിൽ(disp_refr->inv_p == 0) പോകുക refr_finish;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    എങ്കിൽ((lv_display_is_double_buffered(disp_refr) && disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT) ||
       disp_refr->sync_cb) {
        uint32_t i;
        വേണ്ടി(i = 0; i < disp_refr->inv_p; i++) {
            എങ്കിൽ(disp_refr->inv_area_joined[i])
                തുടരുക;

            lv_area_t * sync_area = lv_ll_ins_tail(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, വലുപ്പം(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, വലുപ്പം(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_finish:

#എങ്കിൽ LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_mask_cleanup();
#endif

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, ശൂന്യ);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_END;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_മുകൾ_obj(സ്ഥിരം lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ ശൂന്യ);

    lv_obj_t * found_p = ശൂന്യ;

    എങ്കിൽ(lv_area_is_in(area_p, &obj->coords, 0) == false) തിരികെ ശൂന്യ;
    എങ്കിൽ(lv_obj_is_hidden(obj)) തിരികെ ശൂന്യ;
    എങ്കിൽ(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) തിരികെ ശൂന്യ;
    എങ്കിൽ(lv_obj_get_style_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) തിരികെ ശൂന്യ;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    എങ്കിൽ(info.res == LV_COVER_RES_MASKED) തിരികെ ശൂന്യ;

    int32_t i;
    int32_t child_cnt = lv_obj_get_child_എണ്ണം(obj);
    വേണ്ടി(i = child_cnt - 1; i >= 0; i--) {
        lv_obj_t * child = obj->spec_attr->children[i];
        found_p = lv_refr_get_മുകൾ_obj(area_p, child);

        /*If a children is ok then break*/
        എങ്കിൽ(found_p != ശൂന്യ) {
            നിർത്തുക;
        }
    }

    /*If no better children use this object*/
    എങ്കിൽ(found_p == ശൂന്യ && info.res == LV_COVER_RES_COVER) {
        found_p = obj;
    }

    തിരികെ found_p;
}


ശൂന്യം lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);

    എങ്കിൽ(lv_obj_is_hidden(obj)) തിരികെ;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    സ്ഥിരം lv_opa_t opa_layered = lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN);
    എങ്കിൽ(opa_layered <= LV_OPA_MIN) തിരികെ;

    എങ്കിൽ(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) തിരികെ;
    എങ്കിൽ(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) തിരികെ;

    സ്ഥിരം lv_opa_t layer_opa_ori = layer->opa;
    സ്ഥിരം lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    സ്ഥിരം lv_opa_t opa_main = lv_obj_get_style_opa_internal(obj, LV_PART_MAIN);
    എങ്കിൽ(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    എങ്കിൽ(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#എങ്കിൽ LV_DRAW_TRANSFORM_USE_MATRIX
    /*If the layer opa is full then use the matrix transform*/
    അല്ലെങ്കിൽ എങ്കിൽ(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_overflow(layer, obj)) {
        refr_obj_matrix(layer, obj);
    }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */
    അല്ലെങ്കിൽ {
        lv_area_t layer_area_full;
        lv_area_t obj_draw_size;
        lv_result_t res = layer_get_area(layer, obj, layer_type, &layer_area_full, &obj_draw_size);
        എങ്കിൽ(res != LV_RESULT_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            തിരികെ;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_row_height = lv_area_get_height(&layer_area_full);
        uint32_t max_argb_row_height = lv_area_get_height(&layer_area_full);
        എങ്കിൽ(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_width(&layer_area_full);
            uint8_t px_size = lv_color_format_get_size(disp_refr->color_format);
            max_rgb_row_height = LV_DRAW_LAYER_SIMPLE_BUF_SIZE / w / px_size;
            max_argb_row_height = LV_DRAW_LAYER_SIMPLE_BUF_SIZE / w / വലുപ്പം(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_full.x1;
        layer_area_act.x2 = layer_area_full.x2;
        layer_area_act.y1 = layer_area_full.y1;
        layer_area_act.y2 = layer_area_full.y1;

        ആകുമ്പോൾ(layer_area_act.y2 < layer_area_full.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_row_height - 1;
            എങ്കിൽ(layer_area_act.y2 > layer_area_full.y2) layer_area_act.y2 = layer_area_full.y2;

            സ്ഥിരം ശൂന്യം * bitmap_mask_src = lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_mask_src || alpha_test_area_on_obj(obj, &layer_area_act);

            എങ്കിൽ(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_row_height - 1;
                എങ്കിൽ(layer_area_act.y2 > layer_area_full.y2) layer_area_act.y2 = layer_area_full.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

            lv_point_t pivot = {
                .x = lv_obj_get_style_transform_pivot_x_internal(obj, LV_PART_MAIN),
                .y = lv_obj_get_style_transform_pivot_y_internal(obj, LV_PART_MAIN)
            };

            എങ്കിൽ(LV_COORD_IS_PCT(pivot.x)) {
                pivot.x = (LV_COORD_GET_PCT(pivot.x) * lv_area_get_width(&obj->coords)) / 100;
            }
            എങ്കിൽ(LV_COORD_IS_PCT(pivot.y)) {
                pivot.y = (LV_COORD_GET_PCT(pivot.y) * lv_area_get_height(&obj->coords)) / 100;
            }

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.pivot.x = obj->coords.x1 + pivot.x - new_layer->buf_area.x1;
            layer_draw_dsc.pivot.y = obj->coords.y1 + pivot.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN);
            ആകുമ്പോൾ(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            ആകുമ്പോൾ(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
            layer_draw_dsc.scale_x = lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN);
            layer_draw_dsc.scale_y = lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_x = lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_y = lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN);
            layer_draw_dsc.blend_mode = lv_obj_get_style_blend_mode_internal(obj, LV_PART_MAIN);
            layer_draw_dsc.antialias = disp_refr->antialiasing;
            layer_draw_dsc.bitmap_mask_src = bitmap_mask_src;
            layer_draw_dsc.image_area = obj_draw_size;
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
സ്ഥിതം ശൂന്യം lv_refr_join_area(ശൂന്യം)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_in;
    lv_area_t joined_area;
    വേണ്ടി(join_in = 0; join_in < disp_refr->inv_p; join_in++) {
        എങ്കിൽ(disp_refr->inv_area_joined[join_in] != 0) തുടരുക;

        /*Check all areas to join them in 'join_in'*/
        വേണ്ടി(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            എങ്കിൽ(disp_refr->inv_area_joined[join_from] != 0 || join_in == join_from) {
                തുടരുക;
            }

            /*Check if the areas are on each other*/
            എങ്കിൽ(lv_area_is_on(&disp_refr->inv_areas[join_in], &disp_refr->inv_areas[join_from]) == false) {
                തുടരുക;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_in], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            എങ്കിൽ(lv_area_get_size(&joined_area) < (lv_area_get_size(&disp_refr->inv_areas[join_in]) +
                                                 lv_area_get_size(&disp_refr->inv_areas[join_from]))) {
                disp_refr->inv_areas[join_in] = joined_area;

                /*Mark 'join_form' is joined into 'join_in'*/
                disp_refr->inv_area_joined[join_from] = 1;
            }
        }
    }
    LV_PROFILER_REFR_END;
}

/**
 * Refresh the sync areas
 */
സ്ഥിതം ശൂന്യം refr_sync_areas(ശൂന്യം)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    സ്ഥിരം bool auto_sync = disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT &&
                           lv_display_is_double_buffered(disp_refr);
    സ്ഥിരം bool user_sync = disp_refr->sync_cb != ശൂന്യ;
    എങ്കിൽ(!auto_sync && !user_sync) തിരികെ;

    /*Do not sync if no sync areas*/
    എങ്കിൽ(lv_ll_is_empty(&disp_refr->sync_areas)) തിരികെ;

    LV_PROFILER_REFR_BEGIN;
    /*With double buffered direct mode synchronize the rendered areas to the other buffer*/
    /*We need to wait for ready here to not mess up the active screen*/
    wait_for_flushing(disp_refr);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    uint16_t i;
    int8_t j;
    lv_area_t res[4] = {0};
    int8_t res_c;
    lv_area_t * sync_area, * new_area, * next_area;
    വേണ്ടി(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        എങ്കിൽ(disp_refr->inv_area_joined[i]) തുടരുക;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_head(&disp_refr->sync_areas);
        ആകുമ്പോൾ(sync_area != ശൂന്യ) {
            /*Get next sync area*/
            next_area = lv_ll_get_next(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            എങ്കിൽ(res_c != -1) {
                /*Replace old sync area with new areas*/
                വേണ്ടി(j = 0; j < res_c; j++) {
                    new_area = lv_ll_ins_prev(&disp_refr->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp_refr->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = next_area;
        }
    }

    /*The buffers are already swapped.
     *So the active buffer is the off screen buffer where LVGL will render*/
    lv_draw_buf_t * off_screen = disp_refr->buf_act;
    /*Triple buffer sync buffer for off-screen2 updates.*/
    lv_draw_buf_t * off_screen2 = ശൂന്യ;
    lv_draw_buf_t * on_screen = ശൂന്യ;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    എങ്കിൽ(auto_sync) {
        എങ്കിൽ(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        അല്ലെങ്കിൽ {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    വേണ്ടി(sync_area = lv_ll_get_head(&disp_refr->sync_areas); sync_area != ശൂന്യ;
        sync_area = lv_ll_get_next(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        എങ്കിൽ(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            തുടരുക;
        }
#എങ്കിൽ LV_DRAW_TRANSFORM_USE_MATRIX
        എങ്കിൽ(lv_display_get_matrix_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        എങ്കിൽ(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_last = lv_ll_get_tail(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        അല്ലെങ്കിൽ {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            എങ്കിൽ(off_screen2 != on_screen)
                lv_draw_buf_copy(off_screen2, sync_area, on_screen, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp_refr->sync_areas);
    LV_PROFILER_REFR_END;
}

/**
 * Refresh the joined areas
 */
സ്ഥിതം ശൂന്യം refr_invalid_areas(ശൂന്യം)
{
    എങ്കിൽ(disp_refr->inv_p == 0) തിരികെ;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_START, ശൂന്യ);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t last_i = 0;
    വേണ്ടി(i = disp_refr->inv_p - 1; i >= 0; i--) {
        എങ്കിൽ(disp_refr->inv_area_joined[i] == 0) {
            last_i = i;
            നിർത്തുക;
        }
    }

    disp_refr->last_area = 0;
    disp_refr->last_part = 0;
    disp_refr->rendering_in_progress = true;

    വേണ്ടി(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        എങ്കിൽ(disp_refr->inv_area_joined[i]) തുടരുക;

        എങ്കിൽ(i == last_i) disp_refr->last_area = 1;
        disp_refr->last_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        എങ്കിൽ(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_PARTIAL) {
            /*Calculate the max row num*/
            int32_t w = lv_area_get_width(&inv_a);
            int32_t h = lv_area_get_height(&inv_a);

            int32_t max_row = get_max_row(disp_refr, w, h);

            int32_t row;
            int32_t row_last = 0;
            lv_area_t sub_area;
            sub_area.x1 = inv_a.x1;
            sub_area.x2 = inv_a.x2;
            int32_t y_off = 0;
            വേണ്ടി(row = inv_a.y1; row + max_row - 1 <= inv_a.y2; row += max_row) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = row;
                sub_area.y2 = row + max_row - 1;
                എങ്കിൽ(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                row_last = sub_area.y2;
                എങ്കിൽ(inv_a.y2 == row_last) disp_refr->last_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_height(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            എങ്കിൽ(inv_a.y2 != row_last) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = row;
                sub_area.y2 = inv_a.y2;
                disp_refr->last_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_height(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_FULL ||
                disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT) {
            disp_refr->last_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, ശൂന്യ);
    disp_refr->rendering_in_progress = false;
    LV_PROFILER_REFR_END;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
സ്ഥിതം ശൂന്യം layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
{
    lv_draw_buf_t * ret = lv_draw_buf_reshape(
                              layer->draw_buf,
                              layer->color_format,
                              lv_area_get_width(&layer->buf_area),
                              lv_area_get_height(&layer->buf_area),
                              stride);
    LV_UNUSED(ret);
    LV_ASSERT_NULL(ret);
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
സ്ഥിതം ശൂന്യം refr_area(സ്ഥിരം lv_area_t * area_p, int32_t y_offset)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_head;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_offset = y_offset;
    layer->all_tasks_added = false;

    എങ്കിൽ(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    അല്ലെങ്കിൽ എങ്കിൽ(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT ||
            disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_FULL) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        എങ്കിൽ(LV_DRAW_TRANSFORM_USE_MATRIX && lv_display_get_matrix_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        അല്ലെങ്കിൽ {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->header.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_height(area_p);
    എങ്കിൽ(LV_COLOR_FORMAT_IS_INDEXED(layer->color_format) == false) {
        /* Assume that the buffer size (can be screen sized or smaller in case of partial mode)
         * and max tile size are the optimal scenario. From this calculate the ideal tile size
         * and set the tile count and tile height accordingly.
         */
        uint32_t max_tile_cnt = disp_refr->tile_cnt;
        uint32_t total_buf_size = layer->draw_buf->data_size;
        uint32_t ideal_tile_size = total_buf_size / max_tile_cnt;
        uint32_t area_buf_size = lv_area_get_size(area_p) * lv_color_format_get_size(layer->color_format);

        tile_cnt = (area_buf_size + (ideal_tile_size - 1)) / ideal_tile_size; /*Round up*/
        tile_h = lv_area_get_height(area_p) / tile_cnt;
    }

    എങ്കിൽ(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    അല്ലെങ്കിൽ {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * വലുപ്പം(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        എങ്കിൽ(tile_layers == ശൂന്യ) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_END;
            തിരികെ;
        }
        uint32_t i;
        വേണ്ടി(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_set(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            എങ്കിൽ(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, ശൂന്യ, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        വേണ്ടി(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            ആകുമ്പോൾ(tile_layer->draw_task_head) {
                lv_draw_dispatch_wait_for_request();
                lv_draw_dispatch();
            }
            lv_draw_layer_deinit(tile_layer);
        }
        lv_free(tile_layers);

        layer->all_tasks_added = true;
    }

    disp_refr->refreshed_area = *area_p;
    LV_PROFILER_REFR_END;
}

സ്ഥിതം ശൂന്യം refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#എങ്കിൽ LV_DRAW_TRANSFORM_USE_MATRIX
    എങ്കിൽ(lv_display_get_matrix_rotation(disp_refr)) {
        സ്ഥിരം lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        എങ്കിൽ(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            തിരഞ്ഞെടുപ്പ്(rotation) {
                അവസ്ഥ LV_ROTATION_90:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 270);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->ver_res, 0);
                     */
                    layer->matrix.m[0][0] = 0;
                    layer->matrix.m[0][1] = 1;
                    layer->matrix.m[0][2] = 0;
                    layer->matrix.m[1][0] = -1;
                    layer->matrix.m[1][1] = 0;
                    layer->matrix.m[1][2] = disp_refr->ver_res;
                    നിർത്തുക;

                അവസ്ഥ LV_ROTATION_180:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 180);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->hor_res, -disp_refr->ver_res);
                     */
                    layer->matrix.m[0][0] = -1;
                    layer->matrix.m[0][1] = 0;
                    layer->matrix.m[0][2] = disp_refr->hor_res;
                    layer->matrix.m[1][0] = 0;
                    layer->matrix.m[1][1] = -1;
                    layer->matrix.m[1][2] = disp_refr->ver_res;
                    നിർത്തുക;

                അവസ്ഥ LV_ROTATION_270:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 90);
                     * lv_matrix_translate(&layer->matrix, 0, -disp_refr->hor_res);
                     */
                    layer->matrix.m[0][0] = 0;
                    layer->matrix.m[0][1] = -1;
                    layer->matrix.m[0][2] = disp_refr->hor_res;
                    layer->matrix.m[1][0] = 1;
                    layer->matrix.m[1][1] = 0;
                    layer->matrix.m[1][2] = 0;
                    നിർത്തുക;

                സ്ഥിരസ്ഥിതി:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    നിർത്തുക;
            }
        }
    }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    എങ്കിൽ(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    എങ്കിൽ(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * മുകൾ_act_scr = ശൂന്യ;
    lv_obj_t * മുകൾ_prev_scr = ശൂന്യ;

    /*Get the most top object which is not covered by others*/
    മുകൾ_act_scr = lv_refr_get_മുകൾ_obj(&layer->_clip_area, lv_display_get_screen_active(disp_refr));
    എങ്കിൽ(disp_refr->prev_scr) {
        മുകൾ_prev_scr = lv_refr_get_മുകൾ_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    എങ്കിൽ(മുകൾ_act_scr == ശൂന്യ && മുകൾ_prev_scr == ശൂന്യ) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    എങ്കിൽ(disp_refr->draw_prev_over_act) {
        എങ്കിൽ(മുകൾ_act_scr == ശൂന്യ) മുകൾ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, മുകൾ_act_scr);

        /*Refresh the previous screen if any*/
        എങ്കിൽ(disp_refr->prev_scr) {
            എങ്കിൽ(മുകൾ_prev_scr == ശൂന്യ) മുകൾ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, മുകൾ_prev_scr);
        }
    }
    അല്ലെങ്കിൽ {
        /*Refresh the previous screen if any*/
        എങ്കിൽ(disp_refr->prev_scr) {
            എങ്കിൽ(മുകൾ_prev_scr == ശൂന്യ) മുകൾ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, മുകൾ_prev_scr);
        }

        എങ്കിൽ(മുകൾ_act_scr == ശൂന്യ) മുകൾ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, മുകൾ_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_മുകൾ(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_END;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
സ്ഥിതം ശൂന്യം refr_obj_and_children(lv_layer_t * layer, lv_obj_t * മുകൾ_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    എങ്കിൽ(മുകൾ_obj == ശൂന്യ) മുകൾ_obj = lv_display_get_screen_active(disp_refr);
    എങ്കിൽ(മുകൾ_obj == ശൂന്യ) തിരികെ;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * parent;
    lv_obj_t * border_p = മുകൾ_obj;

    parent = lv_obj_get_parent(മുകൾ_obj);

    സ്ഥിരം lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    എങ്കിൽ(parent) {
        layer->recolor = lv_obj_get_style_recolor_recursive(parent, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, മുകൾ_obj);

    /*Do until not reach the screen*/
    ആകുമ്പോൾ(parent != ശൂന്യ) {
        bool go = false;
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_എണ്ണം(parent);
        വേണ്ടി(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = parent->spec_attr->children[i];
            എങ്കിൽ(!go) {
                എങ്കിൽ(child == border_p) go = true;
            }
            അല്ലെങ്കിൽ {
                /*Refresh the objects*/
                lv_obj_refr(layer, child);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST_BEGIN, (ശൂന്യം *)layer);
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST, (ശൂന്യം *)layer);
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST_END, (ശൂന്യം *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = parent;
        /*Go a level deeper*/
        parent = lv_obj_get_parent(parent);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_END;
}

സ്ഥിതം lv_result_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_out, lv_area_t * obj_draw_size_out)
{
    int32_t ext_draw_size = lv_obj_get_ext_draw_size(obj);
    lv_obj_get_coords(obj, obj_draw_size_out);
    lv_area_increase(obj_draw_size_out, ext_draw_size, ext_draw_size);

    എങ്കിൽ(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_size_out;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_POINT_TRANSFORM_FLAG_NONE);
        എങ്കിൽ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            തിരികെ LV_RESULT_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE);
        എങ്കിൽ(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_size_out)) {
            തിരികെ LV_RESULT_INVALID;
        }

        *layer_area_out = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_out, 5, 5); /*To avoid rounding error*/
    }
    അല്ലെങ്കിൽ എങ്കിൽ(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        എങ്കിൽ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_size_out)) {
            തിരികെ LV_RESULT_INVALID;
        }
        *layer_area_out = clip_coords_for_obj;
    }
    അല്ലെങ്കിൽ {
        LV_LOG_WARN("Unhandled layer type");
        തിരികെ LV_RESULT_INVALID;
    }

    തിരികെ LV_RESULT_OK;
}

സ്ഥിതം bool alpha_test_area_on_obj(lv_obj_t * obj, സ്ഥിരം lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    എങ്കിൽ(!lv_area_is_on(area, &obj->coords)) തിരികെ true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    എങ്കിൽ(info.res == LV_COVER_RES_COVER) തിരികെ false;
    അല്ലെങ്കിൽ തിരികെ true;
}

#എങ്കിൽ LV_DRAW_TRANSFORM_USE_MATRIX

സ്ഥിതം bool obj_get_matrix(lv_obj_t * obj, lv_matrix_t * matrix)
{
    lv_matrix_identity(matrix);

    സ്ഥിരം lv_matrix_t * obj_matrix = lv_obj_get_transform(obj);
    എങ്കിൽ(obj_matrix) {
        lv_matrix_translate(matrix, obj->coords.x1, obj->coords.y1);
        lv_matrix_multiply(matrix, obj_matrix);
        lv_matrix_translate(matrix, -obj->coords.x1, -obj->coords.y1);
        തിരികെ true;
    }

    lv_point_t pivot = {
        .x = lv_obj_get_style_transform_pivot_x_internal(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_pivot_y_internal(obj, LV_PART_MAIN)
    };

    pivot.x = obj->coords.x1 + lv_pct_to_px(pivot.x, lv_area_get_width(&obj->coords));
    pivot.y = obj->coords.y1 + lv_pct_to_px(pivot.y, lv_area_get_height(&obj->coords));

    int32_t rotation = lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_internal(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_internal(obj, LV_PART_MAIN);
    int32_t skew_x = lv_obj_get_style_transform_skew_x_internal(obj, LV_PART_MAIN);
    int32_t skew_y = lv_obj_get_style_transform_skew_y_internal(obj, LV_PART_MAIN);

    /* Checked by caller */
    LV_ASSERT(scale_x > 0 && scale_y > 0);

    /* generate the obj matrix */
    lv_matrix_translate(matrix, pivot.x, pivot.y);
    എങ്കിൽ(rotation != 0) {
        lv_matrix_rotate(matrix, rotation * 0.1f);
    }

    എങ്കിൽ(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_matrix_scale(
            matrix,
            (ദശാംശം)scale_x / LV_SCALE_NONE,
            (ദശാംശം)scale_y / LV_SCALE_NONE
        );
    }

    എങ്കിൽ(skew_x != 0 || skew_y != 0) {
        lv_matrix_skew(matrix, skew_x, skew_y);
    }

    lv_matrix_translate(matrix, -pivot.x, -pivot.y);
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം refr_obj_matrix(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_matrix_t obj_matrix;
    എങ്കിൽ(!obj_get_matrix(obj, &obj_matrix)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_END;
        തിരികെ;
    }

    lv_matrix_t matrix_inv;
    എങ്കിൽ(!lv_matrix_inverse(&matrix_inv, &obj_matrix)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_END;
        തിരികെ;
    }

    /* save original matrix */
    lv_matrix_t ori_matrix = layer->matrix;

    /* apply the obj matrix */
    lv_matrix_multiply(&layer->matrix, &obj_matrix);

    /* calculate clip area without transform */
    lv_area_t clip_area = layer->_clip_area;
    lv_area_t clip_area_ori = layer->_clip_area;
    clip_area = lv_matrix_transform_area(&matrix_inv, &clip_area);

    /* increase the clip area by 1 pixel to avoid rounding errors */
    എങ്കിൽ(!lv_matrix_is_identity_or_translation(&obj_matrix)) {
        lv_area_increase(&clip_area, 1, 1);
    }

    layer->_clip_area = clip_area;

    /* redraw obj */
    lv_obj_redraw(layer, obj);

    /* restore original matrix */
    layer->matrix = ori_matrix;
    /* restore clip area */
    layer->_clip_area = clip_area_ori;
    LV_PROFILER_REFR_END;
}

സ്ഥിതം bool refr_check_obj_clip_overflow(lv_layer_t * layer, lv_obj_t * obj)
{
    എങ്കിൽ(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) == 0) {
        തിരികെ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_size = lv_obj_get_ext_draw_size(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_size, ext_size);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    എങ്കിൽ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        തിരികെ true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, വലുപ്പം(lv_area_t)) != 0;
    തിരികെ has_clip;
}

#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */

സ്ഥിതം uint32_t get_max_row(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_width_to_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_INDEXED_PALETTE_SIZE(cf) * വലുപ്പം(lv_color32_t);

    എങ്കിൽ(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        തിരികെ 0;
    }

    int32_t max_row = (uint32_t)(disp->buf_act->data_size - overhead) / stride;

    എങ്കിൽ(max_row > area_h) max_row = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_row;
    ചെയ്യുക {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        എങ്കിൽ(lv_area_get_height(&tmp) <= max_row) നിർത്തുക;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } ആകുമ്പോൾ(h_tmp > 0);

    എങ്കിൽ(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        തിരികെ 0;
    }
    അല്ലെങ്കിൽ {
        max_row = tmp.y2 + 1;
    }

    തിരികെ max_row;
}

/**
 * Flush the content of the draw buffer
 */
സ്ഥിതം ശൂന്യം draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_head;

    ആകുമ്പോൾ(layer->draw_task_head) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_സന്ദേശം(layer->next == ശൂന്യ, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    എങ്കിൽ(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    എങ്കിൽ(disp->last_area && disp->last_part) disp->flushing_last = 1;
    അല്ലെങ്കിൽ disp->flushing_last = 0;

    bool flushing_last = disp->flushing_last;

    എങ്കിൽ(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->data);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    എങ്കിൽ(lv_display_is_double_buffered(disp) && (disp->render_mode != LV_DISPLAY_RENDER_MODE_DIRECT || flushing_last)) {
        എങ്കിൽ(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        അല്ലെങ്കിൽ {
            disp->buf_act = disp->buf_1;
        }
    }
}

സ്ഥിതം ശൂന്യം call_flush_cb(lv_display_t * disp, സ്ഥിരം lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (പൂർണ്ണസംഖ്യ)area->x1, (പൂർണ്ണസംഖ്യ)area->y1, (പൂർണ്ണസംഖ്യ)area->x2, (പൂർണ്ണസംഖ്യ)area->y2, (ശൂന്യം *)px_map);

    lv_area_t offset_area = {
        .x1 = area->x1 + disp->offset_x,
        .y1 = area->y1 + disp->offset_y,
        .x2 = area->x2 + disp->offset_x,
        .y2 = area->y2 + disp->offset_y
    };

    lv_display_send_event(disp, LV_EVENT_FLUSH_START, &offset_area);

    disp->flush_cb(disp, &offset_area, px_map);
    lv_display_send_event(disp, LV_EVENT_FLUSH_FINISH, &offset_area);

    LV_PROFILER_REFR_END;
}

സ്ഥിതം ശൂന്യം wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_START, ശൂന്യ);

    എങ്കിൽ(disp->flush_wait_cb) {
        എങ്കിൽ(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    അല്ലെങ്കിൽ {
        ആകുമ്പോൾ(disp->flushing);
    }
    disp->flushing_last = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_FINISH, ശൂന്യ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_END;
}

സ്ഥിതം ശൂന്യം call_sync_cb(lv_display_t * disp, സ്ഥിരം lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t offset_area = *area;
    offset_area.x1 += disp->offset_x;
    offset_area.x2 += disp->offset_x;
    offset_area.y1 += disp->offset_y;
    offset_area.y2 += disp->offset_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (പൂർണ്ണസംഖ്യ)offset_area.x1, (പൂർണ്ണസംഖ്യ)offset_area.y1,
                  (പൂർണ്ണസംഖ്യ)offset_area.x2, (പൂർണ്ണസംഖ്യ)offset_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_START, (ശൂന്യം *)&offset_area);

    disp->sync_cb(disp, &offset_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_FINISH, (ശൂന്യം *)&offset_area);

    LV_PROFILER_REFR_END;
}

സ്ഥിതം ശൂന്യം wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_START, ശൂന്യ);

    എങ്കിൽ(disp->sync_wait_cb) {
        എങ്കിൽ(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    അല്ലെങ്കിൽ {
        ആകുമ്പോൾ(disp->syncing);
    }
    disp->syncing_last = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_FINISH, ശൂന്യ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_END;
}
