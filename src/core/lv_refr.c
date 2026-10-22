/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_refr_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_area_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/sw/lv_draw_sw_mask_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_event_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_timer_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/lv_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_global.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/

/*Display being refreshed*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ disp_refr LV_GLOBAL_DEFAULT()->disp_refresh

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ ਖਾਲੀ lv_refr_join_area(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ refr_invalid_areas(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ refr_sync_areas(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ refr_area(ਅਚਲ lv_area_t * area_p, int32_t y_ਵਿੱਥ);
ਸਥਿਰ ਖਾਲੀ refr_configured_layer(lv_layer_t * layer);
ਸਥਿਰ ਖਾਲੀ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * ਸਿਖਰ_obj);
ਸਥਿਰ uint32_t get_max_ਪੰਕਤੀ(lv_display_t * disp, int32_t area_w, int32_t area_h);
ਸਥਿਰ ਖਾਲੀ draw_buf_flush(lv_display_t * disp);
ਸਥਿਰ ਖਾਲੀ call_flush_cb(lv_display_t * disp, ਅਚਲ lv_area_t * area, uint8_t * px_map);
ਸਥਿਰ ਖਾਲੀ wait_for_flushing(lv_display_t * disp);
ਸਥਿਰ ਖਾਲੀ call_sync_cb(lv_display_t * disp, ਅਚਲ lv_area_t * area);
ਸਥਿਰ ਖਾਲੀ wait_for_syncing(lv_display_t * disp);
ਸਥਿਰ lv_ਨਤੀਜਾ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_ਬਾਹਰਲਾ, lv_area_t * obj_draw_ਮਾਪ_ਬਾਹਰਲਾ);
ਸਥਿਰ bool alpha_test_area_on_obj(lv_obj_t * obj, ਅਚਲ lv_area_t * area);
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਸਥਿਰ bool refr_check_obj_clip_ਉਛਾਲ(lv_layer_t * layer, lv_obj_t * obj);
    ਸਥਿਰ ਖਾਲੀ refr_obj_ਮੈਟ੍ਰਿਕਸ(lv_layer_t * layer, lv_obj_t * obj);
#ਜੇ_ਅੰਤ

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ਜੇ LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_TRACE_REFR(...)
#ਜੇ_ਅੰਤ

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
ਖਾਲੀ lv_refr_init(ਖਾਲੀ)
{
}

ਖਾਲੀ lv_refr_deinit(ਖਾਲੀ)
{
}

ਖਾਲੀ lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    ਜੇ(disp) {
        ਜੇ(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    ਨਹੀਂਤਾਂ {
        lv_display_t * d;
        d = lv_display_get_ਅਗਲਾ(ਨਿਰਾ);
        ਜਦੋਂ(d) {
            ਜੇ(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_ਅਗਲਾ(d);
        }
    }
}

ਖਾਲੀ lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    LV_PROFILER_REFR_BEGIN;
    lv_area_t clip_area_ori = layer->_clip_area;
    lv_area_t clip_coords_for_obj;

    /*The widget will be rendered.
     *So setters from now should use animations. */
    obj->rendered = 1;

    /*Truncate the clip area to `obj size + ext size` area*/
    lv_area_t obj_coords_ext;
    lv_obj_get_coords(obj, &obj_coords_ext);
    int32_t ext_draw_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    lv_area_increase(&obj_coords_ext, ext_draw_ਮਾਪ, ext_draw_ਮਾਪ);

    ਜੇ(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_ਸਿਰਾ, layer);
#ਜੇ LV_USE_REFR_DEBUG
    lv_color_t debug_color = lv_color_make(lv_rand(0, 0xFF), lv_rand(0, 0xFF), lv_rand(0, 0xFF));
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = debug_color;
    draw_dsc.bg_opa = LV_OPA_20;
    draw_dsc.border_ਚੌੜਾਈ = 1;
    draw_dsc.border_opa = LV_OPA_30;
    draw_dsc.border_color = debug_color;
    lv_draw_rect(layer, &draw_dsc, &obj_coords_ext);
#ਜੇ_ਅੰਤ

    ਅਚਲ lv_area_t * obj_coords;
    ਜੇ(lv_obj_is_ਉਛਾਲ_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    ਨਹੀਂਤਾਂ {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    ਜੇ(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    ਜੇ(refr_children) {
        uint32_t i;
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
        ਜੇ(ਬੱਚਾ_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ਸਿਰਾ, layer);
        }
        ਨਹੀਂਤਾਂ {
            layer->_clip_area = clip_coords_for_children;
            bool clip_ਕੋਨਾ = lv_obj_get_style_clip_ਕੋਨਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

            int32_t radius = 0;
            ਜੇ(clip_ਕੋਨਾ) {
                radius = lv_obj_get_style_radius_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
                ਜੇ(radius == 0) clip_ਕੋਨਾ = false;
            }

            ਜੇ(clip_ਕੋਨਾ == false) {
                ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                    lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                    lv_obj_refr(layer, ਬੱਚਾ);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ਸਿਰਾ, layer);
            }
            ਨਹੀਂਤਾਂ {
                lv_layer_t * layer_children;
                lv_draw_ਨਕਾਬ_rect_dsc_t ਨਕਾਬ_draw_dsc;
                lv_draw_ਨਕਾਬ_rect_dsc_init(&ਨਕਾਬ_draw_dsc);
                ਨਕਾਬ_draw_dsc.radius = radius;
                ਨਕਾਬ_draw_dsc.area = obj->coords;

                lv_draw_image_dsc_t img_draw_dsc;
                lv_draw_image_dsc_init(&img_draw_dsc);

                int32_t short_side = LV_MIN(lv_area_get_ਚੌੜਾਈ(&obj->coords), lv_area_get_ਉਚਾਈ(&obj->coords));
                int32_t rout = LV_MIN(radius, short_side >> 1);

                lv_area_t bottom = obj->coords;
                bottom.y1 = bottom.y2 - rout + 1;
                ਜੇ(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, ਬੱਚਾ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ਸਿਰਾ, layer_children);

                    lv_draw_ਨਕਾਬ_rect(layer_children, &ਨਕਾਬ_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &bottom);
                }

                lv_area_t ਸਿਖਰ = obj->coords;
                ਸਿਖਰ.y2 = ਸਿਖਰ.y1 + rout - 1;
                ਜੇ(lv_area_intersect(&ਸਿਖਰ, &ਸਿਖਰ, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &ਸਿਖਰ);

                    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, ਬੱਚਾ);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ਸਿਰਾ, layer_children);

                    lv_draw_ਨਕਾਬ_rect(layer_children, &ਨਕਾਬ_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &ਸਿਖਰ);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                ਜੇ(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
                        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
                        lv_obj_refr(layer, ਬੱਚਾ);
                    }

                    /*If all the children are redrawn make 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ਸਿਰਾ, layer);

                }

            }
        }
    }

    layer->_clip_area = clip_area_ori;

    LV_PROFILER_REFR_ਸਿਰਾ;
}

lv_ਨਤੀਜਾ_t lv_inv_area(lv_display_t * disp, ਅਚਲ lv_area_t * area_p)
{
    ਜੇ(!disp) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    ਜੇ(!lv_display_is_invalidation_enabled(disp)) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;

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
    LV_ASSERT_ਸੁਨੇਹਾ(!disp->rendering_ਵਿੱਚ_ਤਰੱਕੀ, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    ਜੇ(area_p == ਨਿਰਾ) {
        disp->inv_p = 0;
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    ਜੇ(suc == false)  ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID; /*Out of the screen*/

    ਜੇ(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    ਜੇ(disp->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਭਰਿਆ) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ਨਿਰਾ);
        ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }

    lv_ਨਤੀਜਾ_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    ਲਈ(i = 0; i < disp->inv_p; i++) {
        ਜੇ(lv_area_is_ਵਿੱਚ(&com_area, &disp->inv_areas[i], 0) != false) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    ਜੇ(disp->inv_p >= LV_INV_BUF_ਮਾਪ) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ਨਿਰਾ);

    ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(ਖਾਲੀ)
{
    ਵਾਪਸ disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
ਖਾਲੀ lv_refr_ਸੈੱਟ_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

ਖਾਲੀ lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    ਜੇ(timer) {
        disp_refr = timer->user_ਅੰਕੜੇ;
    }
    ਨਹੀਂਤਾਂ {
        disp_refr = lv_display_get_default();
    }

    ਜੇ(disp_refr == ਨਿਰਾ) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    ਜੇ(!(buf_act && buf_act->ਅੰਕੜੇ && buf_act->ਅੰਕੜੇ_ਮਾਪ)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#ਜੇ !LV_USE_PERF_MONITOR
    ਜੇ(timer) lv_timer_pause(timer);
#ਜੇ_ਅੰਤ

    lv_ਨਤੀਜਾ_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_ਸ਼ੁਰੂ, ਨਿਰਾ);
    ਜੇ(res == LV_ਨਤੀਜਾ_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_ਟੈਗ("layout");
    ਜੇ(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    ਜੇ(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->ਸਿਖਰ_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_ਸਿਰਾ_ਟੈਗ("layout");

    /*Do nothing if there is no active screen*/
    ਜੇ(disp_refr->act_scr == ਨਿਰਾ) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        ਜਾਓ refr_ਅੰਤ;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    ਜੇ(disp_refr->inv_p == 0) ਜਾਓ refr_ਅੰਤ;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    ਜੇ((lv_display_is_double_buffered(disp_refr) && disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਸਿੱਧਾ) ||
       disp_refr->sync_cb) {
        uint32_t i;
        ਲਈ(i = 0; i < disp_refr->inv_p; i++) {
            ਜੇ(disp_refr->inv_area_joined[i])
                ਜਾਰੀ;

            lv_area_t * sync_area = lv_ll_ins_ਪੂਛ(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, ਆਕਾਰ(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, ਆਕਾਰ(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_ਅੰਤ:

#ਜੇ LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_ਨਕਾਬ_cleanup();
#ਜੇ_ਅੰਤ

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, ਨਿਰਾ);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_ਸਿਰਾ;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_ਸਿਖਰ_obj(ਅਚਲ lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    lv_obj_t * ਮਿਲਿਆ_p = ਨਿਰਾ;

    ਜੇ(lv_area_is_ਵਿੱਚ(area_p, &obj->coords, 0) == false) ਵਾਪਸ ਨਿਰਾ;
    ਜੇ(lv_obj_is_hidden(obj)) ਵਾਪਸ ਨਿਰਾ;
    ਜੇ(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) ਵਾਪਸ ਨਿਰਾ;
    ਜੇ(lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) < LV_OPA_MAX) ਵਾਪਸ ਨਿਰਾ;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ਜੇ(info.res == LV_COVER_RES_MASKED) ਵਾਪਸ ਨਿਰਾ;

    int32_t i;
    int32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = ਬੱਚਾ_cnt - 1; i >= 0; i--) {
        lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        ਮਿਲਿਆ_p = lv_refr_get_ਸਿਖਰ_obj(area_p, ਬੱਚਾ);

        /*If a children is ok then break*/
        ਜੇ(ਮਿਲਿਆ_p != ਨਿਰਾ) {
            ਰੋਕੋ;
        }
    }

    /*If no better children use this object*/
    ਜੇ(ਮਿਲਿਆ_p == ਨਿਰਾ && info.res == LV_COVER_RES_COVER) {
        ਮਿਲਿਆ_p = obj;
    }

    ਵਾਪਸ ਮਿਲਿਆ_p;
}


ਖਾਲੀ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(obj != ਨਿਰਾ, ਵਾਪਸ);

    ਜੇ(lv_obj_is_hidden(obj)) ਵਾਪਸ;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    ਅਚਲ lv_opa_t opa_layered = lv_obj_get_style_opa_layered_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    ਜੇ(opa_layered <= LV_OPA_MIN) ਵਾਪਸ;

    ਜੇ(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) ਵਾਪਸ;
    ਜੇ(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) ਵਾਪਸ;

    ਅਚਲ lv_opa_t layer_opa_ori = layer->opa;
    ਅਚਲ lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    ਅਚਲ lv_opa_t opa_main = lv_obj_get_style_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    ਜੇ(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    ਜੇ(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    /*If the layer opa is full then use the matrix transform*/
    ਨਹੀਂਤਾਂ ਜੇ(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_ਉਛਾਲ(layer, obj)) {
        refr_obj_ਮੈਟ੍ਰਿਕਸ(layer, obj);
    }
#ਜੇ_ਅੰਤ /* LV_DRAW_TRANSFORM_USE_MATRIX */
    ਨਹੀਂਤਾਂ {
        lv_area_t layer_area_ਭਰਿਆ;
        lv_area_t obj_draw_ਮਾਪ;
        lv_ਨਤੀਜਾ_t res = layer_get_area(layer, obj, layer_type, &layer_area_ਭਰਿਆ, &obj_draw_ਮਾਪ);
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            ਵਾਪਸ;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_ਪੰਕਤੀ_ਉਚਾਈ = lv_area_get_ਉਚਾਈ(&layer_area_ਭਰਿਆ);
        uint32_t max_argb_ਪੰਕਤੀ_ਉਚਾਈ = lv_area_get_ਉਚਾਈ(&layer_area_ਭਰਿਆ);
        ਜੇ(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_ਚੌੜਾਈ(&layer_area_ਭਰਿਆ);
            uint8_t px_ਮਾਪ = lv_color_format_get_ਮਾਪ(disp_refr->color_format);
            max_rgb_ਪੰਕਤੀ_ਉਚਾਈ = LV_DRAW_LAYER_SIMPLE_BUF_ਮਾਪ / w / px_ਮਾਪ;
            max_argb_ਪੰਕਤੀ_ਉਚਾਈ = LV_DRAW_LAYER_SIMPLE_BUF_ਮਾਪ / w / ਆਕਾਰ(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_ਭਰਿਆ.x1;
        layer_area_act.x2 = layer_area_ਭਰਿਆ.x2;
        layer_area_act.y1 = layer_area_ਭਰਿਆ.y1;
        layer_area_act.y2 = layer_area_ਭਰਿਆ.y1;

        ਜਦੋਂ(layer_area_act.y2 < layer_area_ਭਰਿਆ.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_ਪੰਕਤੀ_ਉਚਾਈ - 1;
            ਜੇ(layer_area_act.y2 > layer_area_ਭਰਿਆ.y2) layer_area_act.y2 = layer_area_ਭਰਿਆ.y2;

            ਅਚਲ ਖਾਲੀ * bitmap_ਨਕਾਬ_src = lv_obj_get_style_bitmap_ਨਕਾਬ_src_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_ਨਕਾਬ_src || alpha_test_area_on_obj(obj, &layer_area_act);

            ਜੇ(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_ਪੰਕਤੀ_ਉਚਾਈ - 1;
                ਜੇ(layer_area_act.y2 > layer_area_ਭਰਿਆ.y2) layer_area_act.y2 = layer_area_ਭਰਿਆ.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

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

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.ਧੁਰਾ.x = obj->coords.x1 + ਧੁਰਾ.x - new_layer->buf_area.x1;
            layer_draw_dsc.ਧੁਰਾ.y = obj->coords.y1 + ਧੁਰਾ.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            ਜਦੋਂ(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            ਜਦੋਂ(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
            layer_draw_dsc.scale_x = lv_obj_get_style_transform_scale_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            layer_draw_dsc.scale_y = lv_obj_get_style_transform_scale_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_x = lv_obj_get_style_transform_skew_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_y = lv_obj_get_style_transform_skew_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            layer_draw_dsc.blend_ਬਹੁਲਕ = lv_obj_get_style_blend_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
            layer_draw_dsc.antialias = disp_refr->antialiasing;
            layer_draw_dsc.bitmap_ਨਕਾਬ_src = bitmap_ਨਕਾਬ_src;
            layer_draw_dsc.image_area = obj_draw_ਮਾਪ;
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
ਸਥਿਰ ਖਾਲੀ lv_refr_join_area(ਖਾਲੀ)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_ਵਿੱਚ;
    lv_area_t joined_area;
    ਲਈ(join_ਵਿੱਚ = 0; join_ਵਿੱਚ < disp_refr->inv_p; join_ਵਿੱਚ++) {
        ਜੇ(disp_refr->inv_area_joined[join_ਵਿੱਚ] != 0) ਜਾਰੀ;

        /*Check all areas to join them in 'join_in'*/
        ਲਈ(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            ਜੇ(disp_refr->inv_area_joined[join_from] != 0 || join_ਵਿੱਚ == join_from) {
                ਜਾਰੀ;
            }

            /*Check if the areas are on each other*/
            ਜੇ(lv_area_is_on(&disp_refr->inv_areas[join_ਵਿੱਚ], &disp_refr->inv_areas[join_from]) == false) {
                ਜਾਰੀ;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_ਵਿੱਚ], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            ਜੇ(lv_area_get_ਮਾਪ(&joined_area) < (lv_area_get_ਮਾਪ(&disp_refr->inv_areas[join_ਵਿੱਚ]) +
                                                 lv_area_get_ਮਾਪ(&disp_refr->inv_areas[join_from]))) {
                disp_refr->inv_areas[join_ਵਿੱਚ] = joined_area;

                /*Mark 'join_form' is joined into 'join_in'*/
                disp_refr->inv_area_joined[join_from] = 1;
            }
        }
    }
    LV_PROFILER_REFR_ਸਿਰਾ;
}

/**
 * Refresh the sync areas
 */
ਸਥਿਰ ਖਾਲੀ refr_sync_areas(ਖਾਲੀ)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    ਅਚਲ bool auto_sync = disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਸਿੱਧਾ &&
                           lv_display_is_double_buffered(disp_refr);
    ਅਚਲ bool user_sync = disp_refr->sync_cb != ਨਿਰਾ;
    ਜੇ(!auto_sync && !user_sync) ਵਾਪਸ;

    /*Do not sync if no sync areas*/
    ਜੇ(lv_ll_is_ਖਾਲੀ_ਥਾਵਾਂ(&disp_refr->sync_areas)) ਵਾਪਸ;

    LV_PROFILER_REFR_BEGIN;
    /*With double buffered direct mode synchronize the rendered areas to the other buffer*/
    /*We need to wait for ready here to not mess up the active screen*/
    wait_for_flushing(disp_refr);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    uint16_t i;
    int8_t j;
    lv_area_t res[4] = {0};
    int8_t res_c;
    lv_area_t * sync_area, * new_area, * ਅਗਲਾ_area;
    ਲਈ(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        ਜੇ(disp_refr->inv_area_joined[i]) ਜਾਰੀ;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_ਸਿਰ(&disp_refr->sync_areas);
        ਜਦੋਂ(sync_area != ਨਿਰਾ) {
            /*Get next sync area*/
            ਅਗਲਾ_area = lv_ll_get_ਅਗਲਾ(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            ਜੇ(res_c != -1) {
                /*Replace old sync area with new areas*/
                ਲਈ(j = 0; j < res_c; j++) {
                    new_area = lv_ll_ins_prev(&disp_refr->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp_refr->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = ਅਗਲਾ_area;
        }
    }

    /*The buffers are already swapped.
     *So the active buffer is the off screen buffer where LVGL will render*/
    lv_draw_buf_t * off_screen = disp_refr->buf_act;
    /*Triple buffer sync buffer for off-screen2 updates.*/
    lv_draw_buf_t * off_screen2 = ਨਿਰਾ;
    lv_draw_buf_t * on_screen = ਨਿਰਾ;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    ਜੇ(auto_sync) {
        ਜੇ(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        ਨਹੀਂਤਾਂ ਜੇ(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        ਨਹੀਂਤਾਂ {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    ਲਈ(sync_area = lv_ll_get_ਸਿਰ(&disp_refr->sync_areas); sync_area != ਨਿਰਾ;
        sync_area = lv_ll_get_ਅਗਲਾ(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        ਜੇ(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            ਜਾਰੀ;
        }
#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
        ਜੇ(lv_display_get_ਮੈਟ੍ਰਿਕਸ_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#ਜੇ_ਅੰਤ /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        ਜੇ(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_ਆਖਰੀ = lv_ll_get_ਪੂਛ(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        ਨਹੀਂਤਾਂ {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            ਜੇ(off_screen2 != on_screen)
                lv_draw_buf_copy(off_screen2, sync_area, on_screen, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp_refr->sync_areas);
    LV_PROFILER_REFR_ਸਿਰਾ;
}

/**
 * Refresh the joined areas
 */
ਸਥਿਰ ਖਾਲੀ refr_invalid_areas(ਖਾਲੀ)
{
    ਜੇ(disp_refr->inv_p == 0) ਵਾਪਸ;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_ਸ਼ੁਰੂ, ਨਿਰਾ);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t ਆਖਰੀ_i = 0;
    ਲਈ(i = disp_refr->inv_p - 1; i >= 0; i--) {
        ਜੇ(disp_refr->inv_area_joined[i] == 0) {
            ਆਖਰੀ_i = i;
            ਰੋਕੋ;
        }
    }

    disp_refr->ਆਖਰੀ_area = 0;
    disp_refr->ਆਖਰੀ_part = 0;
    disp_refr->rendering_ਵਿੱਚ_ਤਰੱਕੀ = true;

    ਲਈ(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        ਜੇ(disp_refr->inv_area_joined[i]) ਜਾਰੀ;

        ਜੇ(i == ਆਖਰੀ_i) disp_refr->ਆਖਰੀ_area = 1;
        disp_refr->ਆਖਰੀ_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        ਜੇ(disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_PARTIAL) {
            /*Calculate the max row num*/
            int32_t w = lv_area_get_ਚੌੜਾਈ(&inv_a);
            int32_t h = lv_area_get_ਉਚਾਈ(&inv_a);

            int32_t max_ਪੰਕਤੀ = get_max_ਪੰਕਤੀ(disp_refr, w, h);

            int32_t ਪੰਕਤੀ;
            int32_t ਪੰਕਤੀ_ਆਖਰੀ = 0;
            lv_area_t sub_area;
            sub_area.x1 = inv_a.x1;
            sub_area.x2 = inv_a.x2;
            int32_t y_off = 0;
            ਲਈ(ਪੰਕਤੀ = inv_a.y1; ਪੰਕਤੀ + max_ਪੰਕਤੀ - 1 <= inv_a.y2; ਪੰਕਤੀ += max_ਪੰਕਤੀ) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = ਪੰਕਤੀ;
                sub_area.y2 = ਪੰਕਤੀ + max_ਪੰਕਤੀ - 1;
                ਜੇ(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                ਪੰਕਤੀ_ਆਖਰੀ = sub_area.y2;
                ਜੇ(inv_a.y2 == ਪੰਕਤੀ_ਆਖਰੀ) disp_refr->ਆਖਰੀ_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ਉਚਾਈ(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            ਜੇ(inv_a.y2 != ਪੰਕਤੀ_ਆਖਰੀ) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = ਪੰਕਤੀ;
                sub_area.y2 = inv_a.y2;
                disp_refr->ਆਖਰੀ_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ਉਚਾਈ(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ(disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਭਰਿਆ ||
                disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਸਿੱਧਾ) {
            disp_refr->ਆਖਰੀ_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, ਨਿਰਾ);
    disp_refr->rendering_ਵਿੱਚ_ਤਰੱਕੀ = false;
    LV_PROFILER_REFR_ਸਿਰਾ;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
ਸਥਿਰ ਖਾਲੀ layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
{
    lv_draw_buf_t * ret = lv_draw_buf_reshape(
                              layer->draw_buf,
                              layer->color_format,
                              lv_area_get_ਚੌੜਾਈ(&layer->buf_area),
                              lv_area_get_ਉਚਾਈ(&layer->buf_area),
                              stride);
    LV_ਅਣਵਰਤਿਆ(ret);
    LV_ASSERT_NULL(ret);
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
ਸਥਿਰ ਖਾਲੀ refr_area(ਅਚਲ lv_area_t * area_p, int32_t y_ਵਿੱਥ)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_ਸਿਰ;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_ਵਿੱਥ = y_ਵਿੱਥ;
    layer->all_tasks_added = false;

    ਜੇ(disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    ਨਹੀਂਤਾਂ ਜੇ(disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਸਿੱਧਾ ||
            disp_refr->render_ਬਹੁਲਕ == LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਭਰਿਆ) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        ਜੇ(LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ && lv_display_get_ਮੈਟ੍ਰਿਕਸ_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        ਨਹੀਂਤਾਂ {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->ਸਿਰਲੇਖ.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_ਉਚਾਈ(area_p);
    ਜੇ(LV_COLOR_FORMAT_IS_ਸੂਚਕਾਂਕਿਤ(layer->color_format) == false) {
        /* Assume that the buffer size (can be screen sized or smaller in case of partial mode)
         * and max tile size are the optimal scenario. From this calculate the ideal tile size
         * and set the tile count and tile height accordingly.
         */
        uint32_t max_tile_cnt = disp_refr->tile_cnt;
        uint32_t ਕੁੱਲ_buf_ਮਾਪ = layer->draw_buf->ਅੰਕੜੇ_ਮਾਪ;
        uint32_t ideal_tile_ਮਾਪ = ਕੁੱਲ_buf_ਮਾਪ / max_tile_cnt;
        uint32_t area_buf_ਮਾਪ = lv_area_get_ਮਾਪ(area_p) * lv_color_format_get_ਮਾਪ(layer->color_format);

        tile_cnt = (area_buf_ਮਾਪ + (ideal_tile_ਮਾਪ - 1)) / ideal_tile_ਮਾਪ; /*Round up*/
        tile_h = lv_area_get_ਉਚਾਈ(area_p) / tile_cnt;
    }

    ਜੇ(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    ਨਹੀਂਤਾਂ {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * ਆਕਾਰ(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        ਜੇ(tile_layers == ਨਿਰਾ) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_ਸਿਰਾ;
            ਵਾਪਸ;
        }
        uint32_t i;
        ਲਈ(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_ਸੈੱਟ(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            ਜੇ(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, ਨਿਰਾ, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        ਲਈ(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            ਜਦੋਂ(tile_layer->draw_task_ਸਿਰ) {
                lv_draw_dispatch_wait_for_request();
                lv_draw_dispatch();
            }
            lv_draw_layer_deinit(tile_layer);
        }
        lv_free(tile_layers);

        layer->all_tasks_added = true;
    }

    disp_refr->refreshed_area = *area_p;
    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ
    ਜੇ(lv_display_get_ਮੈਟ੍ਰਿਕਸ_rotation(disp_refr)) {
        ਅਚਲ lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        ਜੇ(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            ਚੋਣ(rotation) {
                ਹਾਲਤ LV_ROTATION_90:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 270);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->ver_res, 0);
                     */
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][0] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][1] = 1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][2] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][0] = -1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][1] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][2] = disp_refr->ver_res;
                    ਰੋਕੋ;

                ਹਾਲਤ LV_ROTATION_180:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 180);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->hor_res, -disp_refr->ver_res);
                     */
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][0] = -1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][1] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][2] = disp_refr->hor_res;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][0] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][1] = -1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][2] = disp_refr->ver_res;
                    ਰੋਕੋ;

                ਹਾਲਤ LV_ROTATION_270:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 90);
                     * lv_matrix_translate(&layer->matrix, 0, -disp_refr->hor_res);
                     */
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][0] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][1] = -1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[0][2] = disp_refr->hor_res;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][0] = 1;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][1] = 0;
                    layer->ਮੈਟ੍ਰਿਕਸ.m[1][2] = 0;
                    ਰੋਕੋ;

                ਮੂਲ:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    ਰੋਕੋ;
            }
        }
    }
#ਜੇ_ਅੰਤ /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    ਜੇ(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    ਜੇ(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * ਸਿਖਰ_act_scr = ਨਿਰਾ;
    lv_obj_t * ਸਿਖਰ_prev_scr = ਨਿਰਾ;

    /*Get the most top object which is not covered by others*/
    ਸਿਖਰ_act_scr = lv_refr_get_ਸਿਖਰ_obj(&layer->_clip_area, lv_display_get_screen_ਸਰਗਰਮ(disp_refr));
    ਜੇ(disp_refr->prev_scr) {
        ਸਿਖਰ_prev_scr = lv_refr_get_ਸਿਖਰ_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    ਜੇ(ਸਿਖਰ_act_scr == ਨਿਰਾ && ਸਿਖਰ_prev_scr == ਨਿਰਾ) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    ਜੇ(disp_refr->draw_prev_over_act) {
        ਜੇ(ਸਿਖਰ_act_scr == ਨਿਰਾ) ਸਿਖਰ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, ਸਿਖਰ_act_scr);

        /*Refresh the previous screen if any*/
        ਜੇ(disp_refr->prev_scr) {
            ਜੇ(ਸਿਖਰ_prev_scr == ਨਿਰਾ) ਸਿਖਰ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, ਸਿਖਰ_prev_scr);
        }
    }
    ਨਹੀਂਤਾਂ {
        /*Refresh the previous screen if any*/
        ਜੇ(disp_refr->prev_scr) {
            ਜੇ(ਸਿਖਰ_prev_scr == ਨਿਰਾ) ਸਿਖਰ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, ਸਿਖਰ_prev_scr);
        }

        ਜੇ(ਸਿਖਰ_act_scr == ਨਿਰਾ) ਸਿਖਰ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, ਸਿਖਰ_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_ਸਿਖਰ(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_ਸਿਰਾ;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
ਸਥਿਰ ਖਾਲੀ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * ਸਿਖਰ_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    ਜੇ(ਸਿਖਰ_obj == ਨਿਰਾ) ਸਿਖਰ_obj = lv_display_get_screen_ਸਰਗਰਮ(disp_refr);
    ਜੇ(ਸਿਖਰ_obj == ਨਿਰਾ) ਵਾਪਸ;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * ਮਾਪਾ;
    lv_obj_t * border_p = ਸਿਖਰ_obj;

    ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਸਿਖਰ_obj);

    ਅਚਲ lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    ਜੇ(ਮਾਪਾ) {
        layer->recolor = lv_obj_get_style_recolor_recursive(ਮਾਪਾ, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, ਸਿਖਰ_obj);

    /*Do until not reach the screen*/
    ਜਦੋਂ(ਮਾਪਾ != ਨਿਰਾ) {
        bool go = false;
        uint32_t i;
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
        ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
            lv_obj_t * ਬੱਚਾ = ਮਾਪਾ->spec_attr->children[i];
            ਜੇ(!go) {
                ਜੇ(ਬੱਚਾ == border_p) go = true;
            }
            ਨਹੀਂਤਾਂ {
                /*Refresh the objects*/
                lv_obj_refr(layer, ਬੱਚਾ);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(ਮਾਪਾ, LV_EVENT_DRAW_POST_BEGIN, (ਖਾਲੀ *)layer);
        lv_obj_send_event(ਮਾਪਾ, LV_EVENT_DRAW_POST, (ਖਾਲੀ *)layer);
        lv_obj_send_event(ਮਾਪਾ, LV_EVENT_DRAW_POST_ਸਿਰਾ, (ਖਾਲੀ *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = ਮਾਪਾ;
        /*Go a level deeper*/
        ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਮਾਪਾ);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ lv_ਨਤੀਜਾ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_ਬਾਹਰਲਾ, lv_area_t * obj_draw_ਮਾਪ_ਬਾਹਰਲਾ)
{
    int32_t ext_draw_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    lv_obj_get_coords(obj, obj_draw_ਮਾਪ_ਬਾਹਰਲਾ);
    lv_area_increase(obj_draw_ਮਾਪ_ਬਾਹਰਲਾ, ext_draw_ਮਾਪ, ext_draw_ਮਾਪ);

    ਜੇ(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_ਮਾਪ_ਬਾਹਰਲਾ;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_NONE);
        ਜੇ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_INVERSE);
        ਜੇ(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_ਮਾਪ_ਬਾਹਰਲਾ)) {
            ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
        }

        *layer_area_ਬਾਹਰਲਾ = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_ਬਾਹਰਲਾ, 5, 5); /*To avoid rounding error*/
    }
    ਨਹੀਂਤਾਂ ਜੇ(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        ਜੇ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_ਮਾਪ_ਬਾਹਰਲਾ)) {
            ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
        }
        *layer_area_ਬਾਹਰਲਾ = clip_coords_for_obj;
    }
    ਨਹੀਂਤਾਂ {
        LV_LOG_WARN("Unhandled layer type");
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }

    ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
}

ਸਥਿਰ bool alpha_test_area_on_obj(lv_obj_t * obj, ਅਚਲ lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    ਜੇ(!lv_area_is_on(area, &obj->coords)) ਵਾਪਸ true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ਜੇ(info.res == LV_COVER_RES_COVER) ਵਾਪਸ false;
    ਨਹੀਂਤਾਂ ਵਾਪਸ true;
}

#ਜੇ LV_DRAW_TRANSFORM_USE_ਮੈਟ੍ਰਿਕਸ

ਸਥਿਰ bool obj_get_ਮੈਟ੍ਰਿਕਸ(lv_obj_t * obj, lv_ਮੈਟ੍ਰਿਕਸ_t * ਮੈਟ੍ਰਿਕਸ)
{
    lv_ਮੈਟ੍ਰਿਕਸ_ਪਛਾਣ_ਤੱਤ(ਮੈਟ੍ਰਿਕਸ);

    ਅਚਲ lv_ਮੈਟ੍ਰਿਕਸ_t * obj_ਮੈਟ੍ਰਿਕਸ = lv_obj_get_transform(obj);
    ਜੇ(obj_ਮੈਟ੍ਰਿਕਸ) {
        lv_ਮੈਟ੍ਰਿਕਸ_translate(ਮੈਟ੍ਰਿਕਸ, obj->coords.x1, obj->coords.y1);
        lv_ਮੈਟ੍ਰਿਕਸ_multiply(ਮੈਟ੍ਰਿਕਸ, obj_ਮੈਟ੍ਰਿਕਸ);
        lv_ਮੈਟ੍ਰਿਕਸ_translate(ਮੈਟ੍ਰਿਕਸ, -obj->coords.x1, -obj->coords.y1);
        ਵਾਪਸ true;
    }

    lv_ਬਿੰਦੂ_t ਧੁਰਾ = {
        .x = lv_obj_get_style_transform_ਧੁਰਾ_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ਧੁਰਾ_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN)
    };

    ਧੁਰਾ.x = obj->coords.x1 + lv_pct_ਮੰਜ਼ਿਲ_px(ਧੁਰਾ.x, lv_area_get_ਚੌੜਾਈ(&obj->coords));
    ਧੁਰਾ.y = obj->coords.y1 + lv_pct_ਮੰਜ਼ਿਲ_px(ਧੁਰਾ.y, lv_area_get_ਉਚਾਈ(&obj->coords));

    int32_t rotation = lv_obj_get_style_transform_rotation_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t skew_x = lv_obj_get_style_transform_skew_x_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t skew_y = lv_obj_get_style_transform_skew_y_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    /* Checked by caller */
    LV_ASSERT(scale_x > 0 && scale_y > 0);

    /* generate the obj matrix */
    lv_ਮੈਟ੍ਰਿਕਸ_translate(ਮੈਟ੍ਰਿਕਸ, ਧੁਰਾ.x, ਧੁਰਾ.y);
    ਜੇ(rotation != 0) {
        lv_ਮੈਟ੍ਰਿਕਸ_rotate(ਮੈਟ੍ਰਿਕਸ, rotation * 0.1f);
    }

    ਜੇ(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_ਮੈਟ੍ਰਿਕਸ_scale(
            ਮੈਟ੍ਰਿਕਸ,
            (ਦਸ਼ਮਲਵ)scale_x / LV_SCALE_NONE,
            (ਦਸ਼ਮਲਵ)scale_y / LV_SCALE_NONE
        );
    }

    ਜੇ(skew_x != 0 || skew_y != 0) {
        lv_ਮੈਟ੍ਰਿਕਸ_skew(ਮੈਟ੍ਰਿਕਸ, skew_x, skew_y);
    }

    lv_ਮੈਟ੍ਰਿਕਸ_translate(ਮੈਟ੍ਰਿਕਸ, -ਧੁਰਾ.x, -ਧੁਰਾ.y);
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ refr_obj_ਮੈਟ੍ਰਿਕਸ(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_ਮੈਟ੍ਰਿਕਸ_t obj_ਮੈਟ੍ਰਿਕਸ;
    ਜੇ(!obj_get_ਮੈਟ੍ਰਿਕਸ(obj, &obj_ਮੈਟ੍ਰਿਕਸ)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    lv_ਮੈਟ੍ਰਿਕਸ_t ਮੈਟ੍ਰਿਕਸ_inv;
    ਜੇ(!lv_ਮੈਟ੍ਰਿਕਸ_inverse(&ਮੈਟ੍ਰਿਕਸ_inv, &obj_ਮੈਟ੍ਰਿਕਸ)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_ਸਿਰਾ;
        ਵਾਪਸ;
    }

    /* save original matrix */
    lv_ਮੈਟ੍ਰਿਕਸ_t ori_ਮੈਟ੍ਰਿਕਸ = layer->ਮੈਟ੍ਰਿਕਸ;

    /* apply the obj matrix */
    lv_ਮੈਟ੍ਰਿਕਸ_multiply(&layer->ਮੈਟ੍ਰਿਕਸ, &obj_ਮੈਟ੍ਰਿਕਸ);

    /* calculate clip area without transform */
    lv_area_t clip_area = layer->_clip_area;
    lv_area_t clip_area_ori = layer->_clip_area;
    clip_area = lv_ਮੈਟ੍ਰਿਕਸ_transform_area(&ਮੈਟ੍ਰਿਕਸ_inv, &clip_area);

    /* increase the clip area by 1 pixel to avoid rounding errors */
    ਜੇ(!lv_ਮੈਟ੍ਰਿਕਸ_is_ਪਛਾਣ_ਤੱਤ_or_translation(&obj_ਮੈਟ੍ਰਿਕਸ)) {
        lv_area_increase(&clip_area, 1, 1);
    }

    layer->_clip_area = clip_area;

    /* redraw obj */
    lv_obj_redraw(layer, obj);

    /* restore original matrix */
    layer->ਮੈਟ੍ਰਿਕਸ = ori_ਮੈਟ੍ਰਿਕਸ;
    /* restore clip area */
    layer->_clip_area = clip_area_ori;
    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ bool refr_check_obj_clip_ਉਛਾਲ(lv_layer_t * layer, lv_obj_t * obj)
{
    ਜੇ(lv_obj_get_style_transform_rotation_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == 0) {
        ਵਾਪਸ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ਮਾਪ = lv_obj_get_ext_draw_ਮਾਪ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ਮਾਪ, ext_ਮਾਪ);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_ਬਿੰਦੂ_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    ਜੇ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        ਵਾਪਸ true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, ਆਕਾਰ(lv_area_t)) != 0;
    ਵਾਪਸ has_clip;
}

#ਜੇ_ਅੰਤ /* LV_DRAW_TRANSFORM_USE_MATRIX */

ਸਥਿਰ uint32_t get_max_ਪੰਕਤੀ(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_ਚੌੜਾਈ_ਮੰਜ਼ਿਲ_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_ਸੂਚਕਾਂਕਿਤ_PALETTE_ਮਾਪ(cf) * ਆਕਾਰ(lv_color32_t);

    ਜੇ(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        ਵਾਪਸ 0;
    }

    int32_t max_ਪੰਕਤੀ = (uint32_t)(disp->buf_act->ਅੰਕੜੇ_ਮਾਪ - overhead) / stride;

    ਜੇ(max_ਪੰਕਤੀ > area_h) max_ਪੰਕਤੀ = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_ਪੰਕਤੀ;
    ਕਰੋ {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        ਜੇ(lv_area_get_ਉਚਾਈ(&tmp) <= max_ਪੰਕਤੀ) ਰੋਕੋ;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } ਜਦੋਂ(h_tmp > 0);

    ਜੇ(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        ਵਾਪਸ 0;
    }
    ਨਹੀਂਤਾਂ {
        max_ਪੰਕਤੀ = tmp.y2 + 1;
    }

    ਵਾਪਸ max_ਪੰਕਤੀ;
}

/**
 * Flush the content of the draw buffer
 */
ਸਥਿਰ ਖਾਲੀ draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_ਸਿਰ;

    ਜਦੋਂ(layer->draw_task_ਸਿਰ) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_ਸੁਨੇਹਾ(layer->ਅਗਲਾ == ਨਿਰਾ, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    ਜੇ(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    ਜੇ(disp->ਆਖਰੀ_area && disp->ਆਖਰੀ_part) disp->flushing_ਆਖਰੀ = 1;
    ਨਹੀਂਤਾਂ disp->flushing_ਆਖਰੀ = 0;

    bool flushing_ਆਖਰੀ = disp->flushing_ਆਖਰੀ;

    ਜੇ(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->ਅੰਕੜੇ);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    ਜੇ(lv_display_is_double_buffered(disp) && (disp->render_ਬਹੁਲਕ != LV_DISPLAY_RENDER_ਬਹੁਲਕ_ਸਿੱਧਾ || flushing_ਆਖਰੀ)) {
        ਜੇ(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        ਨਹੀਂਤਾਂ ਜੇ(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        ਨਹੀਂਤਾਂ {
            disp->buf_act = disp->buf_1;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ call_flush_cb(lv_display_t * disp, ਅਚਲ lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (ਪੂਰਨ)area->x1, (ਪੂਰਨ)area->y1, (ਪੂਰਨ)area->x2, (ਪੂਰਨ)area->y2, (ਖਾਲੀ *)px_map);

    lv_area_t ਵਿੱਥ_area = {
        .x1 = area->x1 + disp->ਵਿੱਥ_x,
        .y1 = area->y1 + disp->ਵਿੱਥ_y,
        .x2 = area->x2 + disp->ਵਿੱਥ_x,
        .y2 = area->y2 + disp->ਵਿੱਥ_y
    };

    lv_display_send_event(disp, LV_EVENT_FLUSH_ਸ਼ੁਰੂ, &ਵਿੱਥ_area);

    disp->flush_cb(disp, &ਵਿੱਥ_area, px_map);
    lv_display_send_event(disp, LV_EVENT_FLUSH_ਅੰਤ, &ਵਿੱਥ_area);

    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ਸ਼ੁਰੂ, ਨਿਰਾ);

    ਜੇ(disp->flush_wait_cb) {
        ਜੇ(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    ਨਹੀਂਤਾਂ {
        ਜਦੋਂ(disp->flushing);
    }
    disp->flushing_ਆਖਰੀ = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ਅੰਤ, ਨਿਰਾ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ call_sync_cb(lv_display_t * disp, ਅਚਲ lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t ਵਿੱਥ_area = *area;
    ਵਿੱਥ_area.x1 += disp->ਵਿੱਥ_x;
    ਵਿੱਥ_area.x2 += disp->ਵਿੱਥ_x;
    ਵਿੱਥ_area.y1 += disp->ਵਿੱਥ_y;
    ਵਿੱਥ_area.y2 += disp->ਵਿੱਥ_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (ਪੂਰਨ)ਵਿੱਥ_area.x1, (ਪੂਰਨ)ਵਿੱਥ_area.y1,
                  (ਪੂਰਨ)ਵਿੱਥ_area.x2, (ਪੂਰਨ)ਵਿੱਥ_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_ਸ਼ੁਰੂ, (ਖਾਲੀ *)&ਵਿੱਥ_area);

    disp->sync_cb(disp, &ਵਿੱਥ_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_ਅੰਤ, (ਖਾਲੀ *)&ਵਿੱਥ_area);

    LV_PROFILER_REFR_ਸਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ਸ਼ੁਰੂ, ਨਿਰਾ);

    ਜੇ(disp->sync_wait_cb) {
        ਜੇ(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    ਨਹੀਂਤਾਂ {
        ਜਦੋਂ(disp->syncing);
    }
    disp->syncing_ਆਖਰੀ = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ਅੰਤ, ਨਿਰਾ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_ਸਿਰਾ;
}
