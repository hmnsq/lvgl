/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_refr_private.h"
#అన్తర్భావ "lv_obj_draw_private.h"
#అన్తర్భావ "../misc/lv_area_private.h"
#అన్తర్భావ "../draw/sw/lv_draw_sw_mask_private.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "lv_obj_event_private.h"
#అన్తర్భావ "../display/lv_display_private.h"
#అన్తర్భావ "../misc/lv_timer_private.h"
#అన్తర్భావ "../draw/lv_draw_private.h"
#అన్తర్భావ "lv_global.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/

/*Display being refreshed*/
#నిర్వచించు disp_refr LV_GLOBAL_DEFAULT()->disp_refresh

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర శూన్య lv_refr_join_area(శూన్య);
స్థిర శూన్య refr_invalid_areas(శూన్య);
స్థిర శూన్య refr_sync_areas(శూన్య);
స్థిర శూన్య refr_area(స్థిరరాశి lv_area_t * area_p, int32_t y_స్థానభ్రంశం);
స్థిర శూన్య refr_configured_layer(lv_layer_t * layer);
స్థిర శూన్య refr_obj_and_children(lv_layer_t * layer, lv_obj_t * శిఖరం_obj);
స్థిర uint32_t get_max_అడ్డువరుస(lv_display_t * disp, int32_t area_w, int32_t area_h);
స్థిర శూన్య draw_buf_flush(lv_display_t * disp);
స్థిర శూన్య call_flush_cb(lv_display_t * disp, స్థిరరాశి lv_area_t * area, uint8_t * px_map);
స్థిర శూన్య wait_for_flushing(lv_display_t * disp);
స్థిర శూన్య call_sync_cb(lv_display_t * disp, స్థిరరాశి lv_area_t * area);
స్థిర శూన్య wait_for_syncing(lv_display_t * disp);
స్థిర lv_ఫలితం_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_బహిర్గతం, lv_area_t * obj_draw_కొలత_బహిర్గతం);
స్థిర bool alpha_test_area_on_obj(lv_obj_t * obj, స్థిరరాశి lv_area_t * area);
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    స్థిర bool refr_check_obj_clip_అధికప్రవాహం(lv_layer_t * layer, lv_obj_t * obj);
    స్థిర శూన్య refr_obj_మాతృక(lv_layer_t * layer, lv_obj_t * obj);
#అయితే_ముగింపు

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ఒకవేళ LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #నిర్వచించు LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#లేకపోతే
    #నిర్వచించు LV_TRACE_REFR(...)
#అయితే_ముగింపు

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
శూన్య lv_refr_init(శూన్య)
{
}

శూన్య lv_refr_deinit(శూన్య)
{
}

శూన్య lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    ఒకవేళ(disp) {
        ఒకవేళ(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    లేకపోతే {
        lv_display_t * d;
        d = lv_display_get_తదుపరి(ఖాళీ);
        అయితే(d) {
            ఒకవేళ(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_తదుపరి(d);
        }
    }
}

శూన్య lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    LV_PROFILER_REFR_BEGIN;
    lv_area_t clip_area_ori = layer->_clip_area;
    lv_area_t clip_coords_for_obj;

    /*The widget will be rendered.
     *So setters from now should use animations. */
    obj->rendered = 1;

    /*Truncate the clip area to `obj size + ext size` area*/
    lv_area_t obj_coords_ext;
    lv_obj_get_coords(obj, &obj_coords_ext);
    int32_t ext_draw_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    lv_area_increase(&obj_coords_ext, ext_draw_కొలత, ext_draw_కొలత);

    ఒకవేళ(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_చివర, layer);
#ఒకవేళ LV_USE_REFR_DEBUG
    lv_color_t debug_color = lv_color_make(lv_rand(0, 0xFF), lv_rand(0, 0xFF), lv_rand(0, 0xFF));
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = debug_color;
    draw_dsc.bg_opa = LV_OPA_20;
    draw_dsc.border_వెడల్పు = 1;
    draw_dsc.border_opa = LV_OPA_30;
    draw_dsc.border_color = debug_color;
    lv_draw_rect(layer, &draw_dsc, &obj_coords_ext);
#అయితే_ముగింపు

    స్థిరరాశి lv_area_t * obj_coords;
    ఒకవేళ(lv_obj_is_అధికప్రవాహం_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    లేకపోతే {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    ఒకవేళ(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    ఒకవేళ(refr_children) {
        uint32_t i;
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
        ఒకవేళ(బిడ్డ_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_చివర, layer);
        }
        లేకపోతే {
            layer->_clip_area = clip_coords_for_children;
            bool clip_మూల = lv_obj_get_style_clip_మూల_అంతర్గత(obj, LV_PART_MAIN);

            int32_t radius = 0;
            ఒకవేళ(clip_మూల) {
                radius = lv_obj_get_style_radius_అంతర్గత(obj, LV_PART_MAIN);
                ఒకవేళ(radius == 0) clip_మూల = false;
            }

            ఒకవేళ(clip_మూల == false) {
                కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                    lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                    lv_obj_refr(layer, బిడ్డ);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_చివర, layer);
            }
            లేకపోతే {
                lv_layer_t * layer_children;
                lv_draw_ముసుగు_rect_dsc_t ముసుగు_draw_dsc;
                lv_draw_ముసుగు_rect_dsc_init(&ముసుగు_draw_dsc);
                ముసుగు_draw_dsc.radius = radius;
                ముసుగు_draw_dsc.area = obj->coords;

                lv_draw_image_dsc_t img_draw_dsc;
                lv_draw_image_dsc_init(&img_draw_dsc);

                int32_t short_side = LV_MIN(lv_area_get_వెడల్పు(&obj->coords), lv_area_get_ఎత్తు(&obj->coords));
                int32_t rout = LV_MIN(radius, short_side >> 1);

                lv_area_t bottom = obj->coords;
                bottom.y1 = bottom.y2 - rout + 1;
                ఒకవేళ(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, బిడ్డ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_చివర, layer_children);

                    lv_draw_ముసుగు_rect(layer_children, &ముసుగు_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &bottom);
                }

                lv_area_t శిఖరం = obj->coords;
                శిఖరం.y2 = శిఖరం.y1 + rout - 1;
                ఒకవేళ(lv_area_intersect(&శిఖరం, &శిఖరం, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &శిఖరం);

                    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, బిడ్డ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_చివర, layer_children);

                    lv_draw_ముసుగు_rect(layer_children, &ముసుగు_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &శిఖరం);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                ఒకవేళ(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
                        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
                        lv_obj_refr(layer, బిడ్డ);
                    }

                    /*If all the children are redrawn make 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_చివర, layer);

                }

            }
        }
    }

    layer->_clip_area = clip_area_ori;

    LV_PROFILER_REFR_చివర;
}

lv_ఫలితం_t lv_inv_area(lv_display_t * disp, స్థిరరాశి lv_area_t * area_p)
{
    ఒకవేళ(!disp) తిరిగి LV_ఫలితం_INVALID;
    ఒకవేళ(!lv_display_is_invalidation_enabled(disp)) తిరిగి LV_ఫలితం_INVALID;

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
    LV_ASSERT_సందేశం(!disp->rendering_లోపల_పురోగతి, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    ఒకవేళ(area_p == ఖాళీ) {
        disp->inv_p = 0;
        తిరిగి LV_ఫలితం_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    ఒకవేళ(suc == false)  తిరిగి LV_ఫలితం_INVALID; /*Out of the screen*/

    ఒకవేళ(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    ఒకవేళ(disp->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_నిండిన) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ఖాళీ);
        తిరిగి LV_ఫలితం_OK;
    }

    lv_ఫలితం_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి LV_ఫలితం_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    కొరకు(i = 0; i < disp->inv_p; i++) {
        ఒకవేళ(lv_area_is_లోపల(&com_area, &disp->inv_areas[i], 0) != false) తిరిగి LV_ఫలితం_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    ఒకవేళ(disp->inv_p >= LV_INV_BUF_కొలత) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ఖాళీ);

    తిరిగి LV_ఫలితం_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(శూన్య)
{
    తిరిగి disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
శూన్య lv_refr_సమితి_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

శూన్య lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    ఒకవేళ(timer) {
        disp_refr = timer->user_దత్తాంశం;
    }
    లేకపోతే {
        disp_refr = lv_display_get_default();
    }

    ఒకవేళ(disp_refr == ఖాళీ) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    ఒకవేళ(!(buf_act && buf_act->దత్తాంశం && buf_act->దత్తాంశం_కొలత)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#ఒకవేళ !LV_USE_PERF_MONITOR
    ఒకవేళ(timer) lv_timer_pause(timer);
#అయితే_ముగింపు

    lv_ఫలితం_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_ప్రారంభం, ఖాళీ);
    ఒకవేళ(res == LV_ఫలితం_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_ట్యాగ్("layout");
    ఒకవేళ(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    ఒకవేళ(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->శిఖరం_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_చివర_ట్యాగ్("layout");

    /*Do nothing if there is no active screen*/
    ఒకవేళ(disp_refr->act_scr == ఖాళీ) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        వెళ్ళు refr_ముగింపు;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    ఒకవేళ(disp_refr->inv_p == 0) వెళ్ళు refr_ముగింపు;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    ఒకవేళ((lv_display_is_double_buffered(disp_refr) && disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_ప్రత్యక్ష) ||
       disp_refr->sync_cb) {
        uint32_t i;
        కొరకు(i = 0; i < disp_refr->inv_p; i++) {
            ఒకవేళ(disp_refr->inv_area_joined[i])
                కొనసాగు;

            lv_area_t * sync_area = lv_ll_ins_తోక(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, పరిమాణం(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, పరిమాణం(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_ముగింపు:

#ఒకవేళ LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_ముసుగు_cleanup();
#అయితే_ముగింపు

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, ఖాళీ);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_చివర;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_శిఖరం_obj(స్థిరరాశి lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి ఖాళీ);

    lv_obj_t * దొరికింది_p = ఖాళీ;

    ఒకవేళ(lv_area_is_లోపల(area_p, &obj->coords, 0) == false) తిరిగి ఖాళీ;
    ఒకవేళ(lv_obj_is_hidden(obj)) తిరిగి ఖాళీ;
    ఒకవేళ(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) తిరిగి ఖాళీ;
    ఒకవేళ(lv_obj_get_style_opa_అంతర్గత(obj, LV_PART_MAIN) < LV_OPA_MAX) తిరిగి ఖాళీ;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ఒకవేళ(info.res == LV_COVER_RES_MASKED) తిరిగి ఖాళీ;

    int32_t i;
    int32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = బిడ్డ_cnt - 1; i >= 0; i--) {
        lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        దొరికింది_p = lv_refr_get_శిఖరం_obj(area_p, బిడ్డ);

        /*If a children is ok then break*/
        ఒకవేళ(దొరికింది_p != ఖాళీ) {
            ఆపు;
        }
    }

    /*If no better children use this object*/
    ఒకవేళ(దొరికింది_p == ఖాళీ && info.res == LV_COVER_RES_COVER) {
        దొరికింది_p = obj;
    }

    తిరిగి దొరికింది_p;
}


శూన్య lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(obj != ఖాళీ, తిరిగి);

    ఒకవేళ(lv_obj_is_hidden(obj)) తిరిగి;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    స్థిరరాశి lv_opa_t opa_layered = lv_obj_get_style_opa_layered_అంతర్గత(obj, LV_PART_MAIN);
    ఒకవేళ(opa_layered <= LV_OPA_MIN) తిరిగి;

    ఒకవేళ(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) తిరిగి;
    ఒకవేళ(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) తిరిగి;

    స్థిరరాశి lv_opa_t layer_opa_ori = layer->opa;
    స్థిరరాశి lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    స్థిరరాశి lv_opa_t opa_main = lv_obj_get_style_opa_అంతర్గత(obj, LV_PART_MAIN);
    ఒకవేళ(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    ఒకవేళ(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    /*If the layer opa is full then use the matrix transform*/
    లేకపోతే ఒకవేళ(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_అధికప్రవాహం(layer, obj)) {
        refr_obj_మాతృక(layer, obj);
    }
#అయితే_ముగింపు /* LV_DRAW_TRANSFORM_USE_MATRIX */
    లేకపోతే {
        lv_area_t layer_area_నిండిన;
        lv_area_t obj_draw_కొలత;
        lv_ఫలితం_t res = layer_get_area(layer, obj, layer_type, &layer_area_నిండిన, &obj_draw_కొలత);
        ఒకవేళ(res != LV_ఫలితం_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            తిరిగి;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_అడ్డువరుస_ఎత్తు = lv_area_get_ఎత్తు(&layer_area_నిండిన);
        uint32_t max_argb_అడ్డువరుస_ఎత్తు = lv_area_get_ఎత్తు(&layer_area_నిండిన);
        ఒకవేళ(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_వెడల్పు(&layer_area_నిండిన);
            uint8_t px_కొలత = lv_color_format_get_కొలత(disp_refr->color_format);
            max_rgb_అడ్డువరుస_ఎత్తు = LV_DRAW_LAYER_SIMPLE_BUF_కొలత / w / px_కొలత;
            max_argb_అడ్డువరుస_ఎత్తు = LV_DRAW_LAYER_SIMPLE_BUF_కొలత / w / పరిమాణం(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_నిండిన.x1;
        layer_area_act.x2 = layer_area_నిండిన.x2;
        layer_area_act.y1 = layer_area_నిండిన.y1;
        layer_area_act.y2 = layer_area_నిండిన.y1;

        అయితే(layer_area_act.y2 < layer_area_నిండిన.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_అడ్డువరుస_ఎత్తు - 1;
            ఒకవేళ(layer_area_act.y2 > layer_area_నిండిన.y2) layer_area_act.y2 = layer_area_నిండిన.y2;

            స్థిరరాశి శూన్య * bitmap_ముసుగు_src = lv_obj_get_style_bitmap_ముసుగు_src_అంతర్గత(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_ముసుగు_src || alpha_test_area_on_obj(obj, &layer_area_act);

            ఒకవేళ(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_అడ్డువరుస_ఎత్తు - 1;
                ఒకవేళ(layer_area_act.y2 > layer_area_నిండిన.y2) layer_area_act.y2 = layer_area_నిండిన.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

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

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.ఇరుసు.x = obj->coords.x1 + ఇరుసు.x - new_layer->buf_area.x1;
            layer_draw_dsc.ఇరుసు.y = obj->coords.y1 + ఇరుసు.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_అంతర్గత(obj, LV_PART_MAIN);
            అయితే(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            అయితే(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
            layer_draw_dsc.scale_x = lv_obj_get_style_transform_scale_x_అంతర్గత(obj, LV_PART_MAIN);
            layer_draw_dsc.scale_y = lv_obj_get_style_transform_scale_y_అంతర్గత(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_x = lv_obj_get_style_transform_skew_x_అంతర్గత(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_y = lv_obj_get_style_transform_skew_y_అంతర్గత(obj, LV_PART_MAIN);
            layer_draw_dsc.blend_బహుళకం = lv_obj_get_style_blend_బహుళకం_అంతర్గత(obj, LV_PART_MAIN);
            layer_draw_dsc.antialias = disp_refr->antialiasing;
            layer_draw_dsc.bitmap_ముసుగు_src = bitmap_ముసుగు_src;
            layer_draw_dsc.image_area = obj_draw_కొలత;
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
స్థిర శూన్య lv_refr_join_area(శూన్య)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_లోపల;
    lv_area_t joined_area;
    కొరకు(join_లోపల = 0; join_లోపల < disp_refr->inv_p; join_లోపల++) {
        ఒకవేళ(disp_refr->inv_area_joined[join_లోపల] != 0) కొనసాగు;

        /*Check all areas to join them in 'join_in'*/
        కొరకు(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            ఒకవేళ(disp_refr->inv_area_joined[join_from] != 0 || join_లోపల == join_from) {
                కొనసాగు;
            }

            /*Check if the areas are on each other*/
            ఒకవేళ(lv_area_is_on(&disp_refr->inv_areas[join_లోపల], &disp_refr->inv_areas[join_from]) == false) {
                కొనసాగు;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_లోపల], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            ఒకవేళ(lv_area_get_కొలత(&joined_area) < (lv_area_get_కొలత(&disp_refr->inv_areas[join_లోపల]) +
                                                 lv_area_get_కొలత(&disp_refr->inv_areas[join_from]))) {
                disp_refr->inv_areas[join_లోపల] = joined_area;

                /*Mark 'join_form' is joined into 'join_in'*/
                disp_refr->inv_area_joined[join_from] = 1;
            }
        }
    }
    LV_PROFILER_REFR_చివర;
}

/**
 * Refresh the sync areas
 */
స్థిర శూన్య refr_sync_areas(శూన్య)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    స్థిరరాశి bool auto_sync = disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_ప్రత్యక్ష &&
                           lv_display_is_double_buffered(disp_refr);
    స్థిరరాశి bool user_sync = disp_refr->sync_cb != ఖాళీ;
    ఒకవేళ(!auto_sync && !user_sync) తిరిగి;

    /*Do not sync if no sync areas*/
    ఒకవేళ(lv_ll_is_ఖాళీలు(&disp_refr->sync_areas)) తిరిగి;

    LV_PROFILER_REFR_BEGIN;
    /*With double buffered direct mode synchronize the rendered areas to the other buffer*/
    /*We need to wait for ready here to not mess up the active screen*/
    wait_for_flushing(disp_refr);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    uint16_t i;
    int8_t j;
    lv_area_t res[4] = {0};
    int8_t res_c;
    lv_area_t * sync_area, * new_area, * తదుపరి_area;
    కొరకు(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        ఒకవేళ(disp_refr->inv_area_joined[i]) కొనసాగు;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_తల(&disp_refr->sync_areas);
        అయితే(sync_area != ఖాళీ) {
            /*Get next sync area*/
            తదుపరి_area = lv_ll_get_తదుపరి(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            ఒకవేళ(res_c != -1) {
                /*Replace old sync area with new areas*/
                కొరకు(j = 0; j < res_c; j++) {
                    new_area = lv_ll_ins_prev(&disp_refr->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp_refr->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = తదుపరి_area;
        }
    }

    /*The buffers are already swapped.
     *So the active buffer is the off screen buffer where LVGL will render*/
    lv_draw_buf_t * off_screen = disp_refr->buf_act;
    /*Triple buffer sync buffer for off-screen2 updates.*/
    lv_draw_buf_t * off_screen2 = ఖాళీ;
    lv_draw_buf_t * on_screen = ఖాళీ;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    ఒకవేళ(auto_sync) {
        ఒకవేళ(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        లేకపోతే ఒకవేళ(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        లేకపోతే {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    కొరకు(sync_area = lv_ll_get_తల(&disp_refr->sync_areas); sync_area != ఖాళీ;
        sync_area = lv_ll_get_తదుపరి(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        ఒకవేళ(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            కొనసాగు;
        }
#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
        ఒకవేళ(lv_display_get_మాతృక_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#అయితే_ముగింపు /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        ఒకవేళ(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_చివరి = lv_ll_get_తోక(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        లేకపోతే {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            ఒకవేళ(off_screen2 != on_screen)
                lv_draw_buf_copy(off_screen2, sync_area, on_screen, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp_refr->sync_areas);
    LV_PROFILER_REFR_చివర;
}

/**
 * Refresh the joined areas
 */
స్థిర శూన్య refr_invalid_areas(శూన్య)
{
    ఒకవేళ(disp_refr->inv_p == 0) తిరిగి;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_ప్రారంభం, ఖాళీ);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t చివరి_i = 0;
    కొరకు(i = disp_refr->inv_p - 1; i >= 0; i--) {
        ఒకవేళ(disp_refr->inv_area_joined[i] == 0) {
            చివరి_i = i;
            ఆపు;
        }
    }

    disp_refr->చివరి_area = 0;
    disp_refr->చివరి_part = 0;
    disp_refr->rendering_లోపల_పురోగతి = true;

    కొరకు(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        ఒకవేళ(disp_refr->inv_area_joined[i]) కొనసాగు;

        ఒకవేళ(i == చివరి_i) disp_refr->చివరి_area = 1;
        disp_refr->చివరి_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        ఒకవేళ(disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_PARTIAL) {
            /*Calculate the max row num*/
            int32_t w = lv_area_get_వెడల్పు(&inv_a);
            int32_t h = lv_area_get_ఎత్తు(&inv_a);

            int32_t max_అడ్డువరుస = get_max_అడ్డువరుస(disp_refr, w, h);

            int32_t అడ్డువరుస;
            int32_t అడ్డువరుస_చివరి = 0;
            lv_area_t sub_area;
            sub_area.x1 = inv_a.x1;
            sub_area.x2 = inv_a.x2;
            int32_t y_off = 0;
            కొరకు(అడ్డువరుస = inv_a.y1; అడ్డువరుస + max_అడ్డువరుస - 1 <= inv_a.y2; అడ్డువరుస += max_అడ్డువరుస) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = అడ్డువరుస;
                sub_area.y2 = అడ్డువరుస + max_అడ్డువరుస - 1;
                ఒకవేళ(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                అడ్డువరుస_చివరి = sub_area.y2;
                ఒకవేళ(inv_a.y2 == అడ్డువరుస_చివరి) disp_refr->చివరి_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ఎత్తు(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            ఒకవేళ(inv_a.y2 != అడ్డువరుస_చివరి) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = అడ్డువరుస;
                sub_area.y2 = inv_a.y2;
                disp_refr->చివరి_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ఎత్తు(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        లేకపోతే ఒకవేళ(disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_నిండిన ||
                disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_ప్రత్యక్ష) {
            disp_refr->చివరి_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, ఖాళీ);
    disp_refr->rendering_లోపల_పురోగతి = false;
    LV_PROFILER_REFR_చివర;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
స్థిర శూన్య layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
{
    lv_draw_buf_t * ret = lv_draw_buf_reshape(
                              layer->draw_buf,
                              layer->color_format,
                              lv_area_get_వెడల్పు(&layer->buf_area),
                              lv_area_get_ఎత్తు(&layer->buf_area),
                              stride);
    LV_వాడని(ret);
    LV_ASSERT_NULL(ret);
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
స్థిర శూన్య refr_area(స్థిరరాశి lv_area_t * area_p, int32_t y_స్థానభ్రంశం)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_తల;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_స్థానభ్రంశం = y_స్థానభ్రంశం;
    layer->all_tasks_added = false;

    ఒకవేళ(disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    లేకపోతే ఒకవేళ(disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_ప్రత్యక్ష ||
            disp_refr->render_బహుళకం == LV_DISPLAY_RENDER_బహుళకం_నిండిన) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        ఒకవేళ(LV_DRAW_TRANSFORM_USE_మాతృక && lv_display_get_మాతృక_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        లేకపోతే {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->శీర్షిక.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_ఎత్తు(area_p);
    ఒకవేళ(LV_COLOR_FORMAT_IS_సూచీకృత(layer->color_format) == false) {
        /* Assume that the buffer size (can be screen sized or smaller in case of partial mode)
         * and max tile size are the optimal scenario. From this calculate the ideal tile size
         * and set the tile count and tile height accordingly.
         */
        uint32_t max_tile_cnt = disp_refr->tile_cnt;
        uint32_t మొత్తం_buf_కొలత = layer->draw_buf->దత్తాంశం_కొలత;
        uint32_t ideal_tile_కొలత = మొత్తం_buf_కొలత / max_tile_cnt;
        uint32_t area_buf_కొలత = lv_area_get_కొలత(area_p) * lv_color_format_get_కొలత(layer->color_format);

        tile_cnt = (area_buf_కొలత + (ideal_tile_కొలత - 1)) / ideal_tile_కొలత; /*Round up*/
        tile_h = lv_area_get_ఎత్తు(area_p) / tile_cnt;
    }

    ఒకవేళ(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    లేకపోతే {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * పరిమాణం(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        ఒకవేళ(tile_layers == ఖాళీ) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_చివర;
            తిరిగి;
        }
        uint32_t i;
        కొరకు(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_సమితి(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            ఒకవేళ(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, ఖాళీ, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        కొరకు(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            అయితే(tile_layer->draw_task_తల) {
                lv_draw_dispatch_wait_for_request();
                lv_draw_dispatch();
            }
            lv_draw_layer_deinit(tile_layer);
        }
        lv_free(tile_layers);

        layer->all_tasks_added = true;
    }

    disp_refr->refreshed_area = *area_p;
    LV_PROFILER_REFR_చివర;
}

స్థిర శూన్య refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక
    ఒకవేళ(lv_display_get_మాతృక_rotation(disp_refr)) {
        స్థిరరాశి lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        ఒకవేళ(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            ఎంపిక(rotation) {
                స్థితి LV_ROTATION_90:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 270);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->ver_res, 0);
                     */
                    layer->మాతృక.m[0][0] = 0;
                    layer->మాతృక.m[0][1] = 1;
                    layer->మాతృక.m[0][2] = 0;
                    layer->మాతృక.m[1][0] = -1;
                    layer->మాతృక.m[1][1] = 0;
                    layer->మాతృక.m[1][2] = disp_refr->ver_res;
                    ఆపు;

                స్థితి LV_ROTATION_180:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 180);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->hor_res, -disp_refr->ver_res);
                     */
                    layer->మాతృక.m[0][0] = -1;
                    layer->మాతృక.m[0][1] = 0;
                    layer->మాతృక.m[0][2] = disp_refr->hor_res;
                    layer->మాతృక.m[1][0] = 0;
                    layer->మాతృక.m[1][1] = -1;
                    layer->మాతృక.m[1][2] = disp_refr->ver_res;
                    ఆపు;

                స్థితి LV_ROTATION_270:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 90);
                     * lv_matrix_translate(&layer->matrix, 0, -disp_refr->hor_res);
                     */
                    layer->మాతృక.m[0][0] = 0;
                    layer->మాతృక.m[0][1] = -1;
                    layer->మాతృక.m[0][2] = disp_refr->hor_res;
                    layer->మాతృక.m[1][0] = 1;
                    layer->మాతృక.m[1][1] = 0;
                    layer->మాతృక.m[1][2] = 0;
                    ఆపు;

                అప్రమేయ:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    ఆపు;
            }
        }
    }
#అయితే_ముగింపు /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    ఒకవేళ(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    ఒకవేళ(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * శిఖరం_act_scr = ఖాళీ;
    lv_obj_t * శిఖరం_prev_scr = ఖాళీ;

    /*Get the most top object which is not covered by others*/
    శిఖరం_act_scr = lv_refr_get_శిఖరం_obj(&layer->_clip_area, lv_display_get_screen_చురుకైన(disp_refr));
    ఒకవేళ(disp_refr->prev_scr) {
        శిఖరం_prev_scr = lv_refr_get_శిఖరం_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    ఒకవేళ(శిఖరం_act_scr == ఖాళీ && శిఖరం_prev_scr == ఖాళీ) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    ఒకవేళ(disp_refr->draw_prev_over_act) {
        ఒకవేళ(శిఖరం_act_scr == ఖాళీ) శిఖరం_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, శిఖరం_act_scr);

        /*Refresh the previous screen if any*/
        ఒకవేళ(disp_refr->prev_scr) {
            ఒకవేళ(శిఖరం_prev_scr == ఖాళీ) శిఖరం_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, శిఖరం_prev_scr);
        }
    }
    లేకపోతే {
        /*Refresh the previous screen if any*/
        ఒకవేళ(disp_refr->prev_scr) {
            ఒకవేళ(శిఖరం_prev_scr == ఖాళీ) శిఖరం_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, శిఖరం_prev_scr);
        }

        ఒకవేళ(శిఖరం_act_scr == ఖాళీ) శిఖరం_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, శిఖరం_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_శిఖరం(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_చివర;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
స్థిర శూన్య refr_obj_and_children(lv_layer_t * layer, lv_obj_t * శిఖరం_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    ఒకవేళ(శిఖరం_obj == ఖాళీ) శిఖరం_obj = lv_display_get_screen_చురుకైన(disp_refr);
    ఒకవేళ(శిఖరం_obj == ఖాళీ) తిరిగి;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * మాతృ;
    lv_obj_t * border_p = శిఖరం_obj;

    మాతృ = lv_obj_get_మాతృ(శిఖరం_obj);

    స్థిరరాశి lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    ఒకవేళ(మాతృ) {
        layer->recolor = lv_obj_get_style_recolor_recursive(మాతృ, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, శిఖరం_obj);

    /*Do until not reach the screen*/
    అయితే(మాతృ != ఖాళీ) {
        bool go = false;
        uint32_t i;
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
        కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
            lv_obj_t * బిడ్డ = మాతృ->spec_attr->children[i];
            ఒకవేళ(!go) {
                ఒకవేళ(బిడ్డ == border_p) go = true;
            }
            లేకపోతే {
                /*Refresh the objects*/
                lv_obj_refr(layer, బిడ్డ);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(మాతృ, LV_EVENT_DRAW_POST_BEGIN, (శూన్య *)layer);
        lv_obj_send_event(మాతృ, LV_EVENT_DRAW_POST, (శూన్య *)layer);
        lv_obj_send_event(మాతృ, LV_EVENT_DRAW_POST_చివర, (శూన్య *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = మాతృ;
        /*Go a level deeper*/
        మాతృ = lv_obj_get_మాతృ(మాతృ);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_చివర;
}

స్థిర lv_ఫలితం_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_బహిర్గతం, lv_area_t * obj_draw_కొలత_బహిర్గతం)
{
    int32_t ext_draw_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    lv_obj_get_coords(obj, obj_draw_కొలత_బహిర్గతం);
    lv_area_increase(obj_draw_కొలత_బహిర్గతం, ext_draw_కొలత, ext_draw_కొలత);

    ఒకవేళ(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_కొలత_బహిర్గతం;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_బిందువు_TRANSFORM_FLAG_NONE);
        ఒకవేళ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            తిరిగి LV_ఫలితం_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_బిందువు_TRANSFORM_FLAG_INVERSE);
        ఒకవేళ(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_కొలత_బహిర్గతం)) {
            తిరిగి LV_ఫలితం_INVALID;
        }

        *layer_area_బహిర్గతం = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_బహిర్గతం, 5, 5); /*To avoid rounding error*/
    }
    లేకపోతే ఒకవేళ(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        ఒకవేళ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_కొలత_బహిర్గతం)) {
            తిరిగి LV_ఫలితం_INVALID;
        }
        *layer_area_బహిర్గతం = clip_coords_for_obj;
    }
    లేకపోతే {
        LV_LOG_WARN("Unhandled layer type");
        తిరిగి LV_ఫలితం_INVALID;
    }

    తిరిగి LV_ఫలితం_OK;
}

స్థిర bool alpha_test_area_on_obj(lv_obj_t * obj, స్థిరరాశి lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    ఒకవేళ(!lv_area_is_on(area, &obj->coords)) తిరిగి true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ఒకవేళ(info.res == LV_COVER_RES_COVER) తిరిగి false;
    లేకపోతే తిరిగి true;
}

#ఒకవేళ LV_DRAW_TRANSFORM_USE_మాతృక

స్థిర bool obj_get_మాతృక(lv_obj_t * obj, lv_మాతృక_t * మాతృక)
{
    lv_మాతృక_తటస్థం(మాతృక);

    స్థిరరాశి lv_మాతృక_t * obj_మాతృక = lv_obj_get_transform(obj);
    ఒకవేళ(obj_మాతృక) {
        lv_మాతృక_translate(మాతృక, obj->coords.x1, obj->coords.y1);
        lv_మాతృక_multiply(మాతృక, obj_మాతృక);
        lv_మాతృక_translate(మాతృక, -obj->coords.x1, -obj->coords.y1);
        తిరిగి true;
    }

    lv_బిందువు_t ఇరుసు = {
        .x = lv_obj_get_style_transform_ఇరుసు_x_అంతర్గత(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ఇరుసు_y_అంతర్గత(obj, LV_PART_MAIN)
    };

    ఇరుసు.x = obj->coords.x1 + lv_pct_గమ్యం_px(ఇరుసు.x, lv_area_get_వెడల్పు(&obj->coords));
    ఇరుసు.y = obj->coords.y1 + lv_pct_గమ్యం_px(ఇరుసు.y, lv_area_get_ఎత్తు(&obj->coords));

    int32_t rotation = lv_obj_get_style_transform_rotation_అంతర్గత(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_అంతర్గత(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_అంతర్గత(obj, LV_PART_MAIN);
    int32_t skew_x = lv_obj_get_style_transform_skew_x_అంతర్గత(obj, LV_PART_MAIN);
    int32_t skew_y = lv_obj_get_style_transform_skew_y_అంతర్గత(obj, LV_PART_MAIN);

    /* Checked by caller */
    LV_ASSERT(scale_x > 0 && scale_y > 0);

    /* generate the obj matrix */
    lv_మాతృక_translate(మాతృక, ఇరుసు.x, ఇరుసు.y);
    ఒకవేళ(rotation != 0) {
        lv_మాతృక_rotate(మాతృక, rotation * 0.1f);
    }

    ఒకవేళ(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_మాతృక_scale(
            మాతృక,
            (దశాంశం)scale_x / LV_SCALE_NONE,
            (దశాంశం)scale_y / LV_SCALE_NONE
        );
    }

    ఒకవేళ(skew_x != 0 || skew_y != 0) {
        lv_మాతృక_skew(మాతృక, skew_x, skew_y);
    }

    lv_మాతృక_translate(మాతృక, -ఇరుసు.x, -ఇరుసు.y);
    తిరిగి true;
}

స్థిర శూన్య refr_obj_మాతృక(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_మాతృక_t obj_మాతృక;
    ఒకవేళ(!obj_get_మాతృక(obj, &obj_మాతృక)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }

    lv_మాతృక_t మాతృక_inv;
    ఒకవేళ(!lv_మాతృక_inverse(&మాతృక_inv, &obj_మాతృక)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_చివర;
        తిరిగి;
    }

    /* save original matrix */
    lv_మాతృక_t ori_మాతృక = layer->మాతృక;

    /* apply the obj matrix */
    lv_మాతృక_multiply(&layer->మాతృక, &obj_మాతృక);

    /* calculate clip area without transform */
    lv_area_t clip_area = layer->_clip_area;
    lv_area_t clip_area_ori = layer->_clip_area;
    clip_area = lv_మాతృక_transform_area(&మాతృక_inv, &clip_area);

    /* increase the clip area by 1 pixel to avoid rounding errors */
    ఒకవేళ(!lv_మాతృక_is_తటస్థం_or_translation(&obj_మాతృక)) {
        lv_area_increase(&clip_area, 1, 1);
    }

    layer->_clip_area = clip_area;

    /* redraw obj */
    lv_obj_redraw(layer, obj);

    /* restore original matrix */
    layer->మాతృక = ori_మాతృక;
    /* restore clip area */
    layer->_clip_area = clip_area_ori;
    LV_PROFILER_REFR_చివర;
}

స్థిర bool refr_check_obj_clip_అధికప్రవాహం(lv_layer_t * layer, lv_obj_t * obj)
{
    ఒకవేళ(lv_obj_get_style_transform_rotation_అంతర్గత(obj, LV_PART_MAIN) == 0) {
        తిరిగి false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_కొలత = lv_obj_get_ext_draw_కొలత(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_కొలత, ext_కొలత);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_బిందువు_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    ఒకవేళ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        తిరిగి true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, పరిమాణం(lv_area_t)) != 0;
    తిరిగి has_clip;
}

#అయితే_ముగింపు /* LV_DRAW_TRANSFORM_USE_MATRIX */

స్థిర uint32_t get_max_అడ్డువరుస(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_వెడల్పు_గమ్యం_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_సూచీకృత_PALETTE_కొలత(cf) * పరిమాణం(lv_color32_t);

    ఒకవేళ(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        తిరిగి 0;
    }

    int32_t max_అడ్డువరుస = (uint32_t)(disp->buf_act->దత్తాంశం_కొలత - overhead) / stride;

    ఒకవేళ(max_అడ్డువరుస > area_h) max_అడ్డువరుస = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_అడ్డువరుస;
    చేయి {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        ఒకవేళ(lv_area_get_ఎత్తు(&tmp) <= max_అడ్డువరుస) ఆపు;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } అయితే(h_tmp > 0);

    ఒకవేళ(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        తిరిగి 0;
    }
    లేకపోతే {
        max_అడ్డువరుస = tmp.y2 + 1;
    }

    తిరిగి max_అడ్డువరుస;
}

/**
 * Flush the content of the draw buffer
 */
స్థిర శూన్య draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_తల;

    అయితే(layer->draw_task_తల) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_సందేశం(layer->తదుపరి == ఖాళీ, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    ఒకవేళ(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    ఒకవేళ(disp->చివరి_area && disp->చివరి_part) disp->flushing_చివరి = 1;
    లేకపోతే disp->flushing_చివరి = 0;

    bool flushing_చివరి = disp->flushing_చివరి;

    ఒకవేళ(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->దత్తాంశం);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    ఒకవేళ(lv_display_is_double_buffered(disp) && (disp->render_బహుళకం != LV_DISPLAY_RENDER_బహుళకం_ప్రత్యక్ష || flushing_చివరి)) {
        ఒకవేళ(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        లేకపోతే ఒకవేళ(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        లేకపోతే {
            disp->buf_act = disp->buf_1;
        }
    }
}

స్థిర శూన్య call_flush_cb(lv_display_t * disp, స్థిరరాశి lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (పూర్ణాంకం)area->x1, (పూర్ణాంకం)area->y1, (పూర్ణాంకం)area->x2, (పూర్ణాంకం)area->y2, (శూన్య *)px_map);

    lv_area_t స్థానభ్రంశం_area = {
        .x1 = area->x1 + disp->స్థానభ్రంశం_x,
        .y1 = area->y1 + disp->స్థానభ్రంశం_y,
        .x2 = area->x2 + disp->స్థానభ్రంశం_x,
        .y2 = area->y2 + disp->స్థానభ్రంశం_y
    };

    lv_display_send_event(disp, LV_EVENT_FLUSH_ప్రారంభం, &స్థానభ్రంశం_area);

    disp->flush_cb(disp, &స్థానభ్రంశం_area, px_map);
    lv_display_send_event(disp, LV_EVENT_FLUSH_ముగింపు, &స్థానభ్రంశం_area);

    LV_PROFILER_REFR_చివర;
}

స్థిర శూన్య wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ప్రారంభం, ఖాళీ);

    ఒకవేళ(disp->flush_wait_cb) {
        ఒకవేళ(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    లేకపోతే {
        అయితే(disp->flushing);
    }
    disp->flushing_చివరి = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ముగింపు, ఖాళీ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_చివర;
}

స్థిర శూన్య call_sync_cb(lv_display_t * disp, స్థిరరాశి lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t స్థానభ్రంశం_area = *area;
    స్థానభ్రంశం_area.x1 += disp->స్థానభ్రంశం_x;
    స్థానభ్రంశం_area.x2 += disp->స్థానభ్రంశం_x;
    స్థానభ్రంశం_area.y1 += disp->స్థానభ్రంశం_y;
    స్థానభ్రంశం_area.y2 += disp->స్థానభ్రంశం_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (పూర్ణాంకం)స్థానభ్రంశం_area.x1, (పూర్ణాంకం)స్థానభ్రంశం_area.y1,
                  (పూర్ణాంకం)స్థానభ్రంశం_area.x2, (పూర్ణాంకం)స్థానభ్రంశం_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_ప్రారంభం, (శూన్య *)&స్థానభ్రంశం_area);

    disp->sync_cb(disp, &స్థానభ్రంశం_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_ముగింపు, (శూన్య *)&స్థానభ్రంశం_area);

    LV_PROFILER_REFR_చివర;
}

స్థిర శూన్య wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ప్రారంభం, ఖాళీ);

    ఒకవేళ(disp->sync_wait_cb) {
        ఒకవేళ(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    లేకపోతే {
        అయితే(disp->syncing);
    }
    disp->syncing_చివరి = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ముగింపు, ఖాళీ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_చివర;
}
