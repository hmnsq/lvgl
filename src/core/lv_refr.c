/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_refr_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_draw_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_area_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/sw/lv_draw_sw_mask_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_event_private.h"
#ಅನ್ತರ್ಭಾವ "../display/lv_display_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_timer_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/lv_draw_private.h"
#ಅನ್ತರ್ಭಾವ "lv_global.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/

/*Display being refreshed*/
#ವ್ಯಾಖ್ಯಾನಿಸು disp_refr LV_GLOBAL_DEFAULT()->disp_refresh

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ ಶೂನ್ಯ lv_refr_join_area(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ refr_invalid_areas(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ refr_sync_areas(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ refr_area(ಸ್ಥಿರಾಂಕ lv_area_t * area_p, int32_t y_ಸ್ಥಾನಾಂತರ);
ಸ್ಥಿರ ಶೂನ್ಯ refr_configured_layer(lv_layer_t * layer);
ಸ್ಥಿರ ಶೂನ್ಯ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * ತುದಿ_obj);
ಸ್ಥಿರ uint32_t get_max_ಅಡ್ಡಸಾಲು(lv_display_t * disp, int32_t area_w, int32_t area_h);
ಸ್ಥಿರ ಶೂನ್ಯ draw_buf_flush(lv_display_t * disp);
ಸ್ಥಿರ ಶೂನ್ಯ call_flush_cb(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area, uint8_t * px_map);
ಸ್ಥಿರ ಶೂನ್ಯ wait_for_flushing(lv_display_t * disp);
ಸ್ಥಿರ ಶೂನ್ಯ call_sync_cb(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area);
ಸ್ಥಿರ ಶೂನ್ಯ wait_for_syncing(lv_display_t * disp);
ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_ಹೊರಹೋಗು, lv_area_t * obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು);
ಸ್ಥಿರ bool alpha_test_area_on_obj(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_area_t * area);
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಸ್ಥಿರ bool refr_check_obj_clip_ಅತಿಪ್ರವಾಹ(lv_layer_t * layer, lv_obj_t * obj);
    ಸ್ಥಿರ ಶೂನ್ಯ refr_obj_ಮಾತೃಕೆ(lv_layer_t * layer, lv_obj_t * obj);
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#ಆದರೆ LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #ವ್ಯಾಖ್ಯಾನಿಸು LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು LV_TRACE_REFR(...)
#ಆದರೆ_ಅಂತ್ಯ

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
ಶೂನ್ಯ lv_refr_init(ಶೂನ್ಯ)
{
}

ಶೂನ್ಯ lv_refr_deinit(ಶೂನ್ಯ)
{
}

ಶೂನ್ಯ lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    ಆದರೆ(disp) {
        ಆದರೆ(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_display_t * d;
        d = lv_display_get_ಮುಂದಿನ(ಖಾಲಿ);
        ಆಗ(d) {
            ಆದರೆ(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_ಮುಂದಿನ(d);
        }
    }
}

ಶೂನ್ಯ lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    LV_PROFILER_REFR_BEGIN;
    lv_area_t clip_area_ori = layer->_clip_area;
    lv_area_t clip_coords_for_obj;

    /*The widget will be rendered.
     *So setters from now should use animations. */
    obj->rendered = 1;

    /*Truncate the clip area to `obj size + ext size` area*/
    lv_area_t obj_coords_ext;
    lv_obj_get_coords(obj, &obj_coords_ext);
    int32_t ext_draw_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    lv_area_increase(&obj_coords_ext, ext_draw_ಗಾತ್ರ, ext_draw_ಗಾತ್ರ);

    ಆದರೆ(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_ಕೊನೆ, layer);
#ಆದರೆ LV_USE_REFR_DEBUG
    lv_color_t debug_color = lv_color_make(lv_rand(0, 0xFF), lv_rand(0, 0xFF), lv_rand(0, 0xFF));
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = debug_color;
    draw_dsc.bg_opa = LV_OPA_20;
    draw_dsc.border_ಅಗಲ = 1;
    draw_dsc.border_opa = LV_OPA_30;
    draw_dsc.border_color = debug_color;
    lv_draw_rect(layer, &draw_dsc, &obj_coords_ext);
#ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರಾಂಕ lv_area_t * obj_coords;
    ಆದರೆ(lv_obj_is_ಅತಿಪ್ರವಾಹ_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    ಆದರೆ(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    ಆದರೆ(refr_children) {
        uint32_t i;
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
        ಆದರೆ(ಮಗು_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ಕೊನೆ, layer);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            layer->_clip_area = clip_coords_for_children;
            bool clip_ಮೂಲೆ = lv_obj_get_style_clip_ಮೂಲೆ_ಆಂತರಿಕ(obj, LV_PART_MAIN);

            int32_t radius = 0;
            ಆದರೆ(clip_ಮೂಲೆ) {
                radius = lv_obj_get_style_radius_ಆಂತರಿಕ(obj, LV_PART_MAIN);
                ಆದರೆ(radius == 0) clip_ಮೂಲೆ = false;
            }

            ಆದರೆ(clip_ಮೂಲೆ == false) {
                ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                    lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                    lv_obj_refr(layer, ಮಗು);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ಕೊನೆ, layer);
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                lv_layer_t * layer_children;
                lv_draw_ಮುಖವಾಡ_rect_dsc_t ಮುಖವಾಡ_draw_dsc;
                lv_draw_ಮುಖವಾಡ_rect_dsc_init(&ಮುಖವಾಡ_draw_dsc);
                ಮುಖವಾಡ_draw_dsc.radius = radius;
                ಮುಖವಾಡ_draw_dsc.area = obj->coords;

                lv_draw_image_dsc_t img_draw_dsc;
                lv_draw_image_dsc_init(&img_draw_dsc);

                int32_t short_side = LV_MIN(lv_area_get_ಅಗಲ(&obj->coords), lv_area_get_ಎತ್ತರ(&obj->coords));
                int32_t rout = LV_MIN(radius, short_side >> 1);

                lv_area_t bottom = obj->coords;
                bottom.y1 = bottom.y2 - rout + 1;
                ಆದರೆ(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, ಮಗು);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ಕೊನೆ, layer_children);

                    lv_draw_ಮುಖವಾಡ_rect(layer_children, &ಮುಖವಾಡ_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &bottom);
                }

                lv_area_t ತುದಿ = obj->coords;
                ತುದಿ.y2 = ತುದಿ.y1 + rout - 1;
                ಆದರೆ(lv_area_intersect(&ತುದಿ, &ತುದಿ, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &ತುದಿ);

                    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, ಮಗು);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ಕೊನೆ, layer_children);

                    lv_draw_ಮುಖವಾಡ_rect(layer_children, &ಮುಖವಾಡ_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &ತುದಿ);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                ಆದರೆ(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
                        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
                        lv_obj_refr(layer, ಮಗು);
                    }

                    /*If all the children are redrawn make 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_ಕೊನೆ, layer);

                }

            }
        }
    }

    layer->_clip_area = clip_area_ori;

    LV_PROFILER_REFR_ಕೊನೆ;
}

lv_ಫಲಿತಾಂಶ_t lv_inv_area(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area_p)
{
    ಆದರೆ(!disp) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    ಆದರೆ(!lv_display_is_invalidation_enabled(disp)) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;

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
    LV_ASSERT_ಸಂದೇಶ(!disp->rendering_ಒಳ_ಪ್ರಗತಿ, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    ಆದರೆ(area_p == ಖಾಲಿ) {
        disp->inv_p = 0;
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    ಆದರೆ(suc == false)  ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID; /*Out of the screen*/

    ಆದರೆ(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    ಆದರೆ(disp->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ತುಂಬಿದ) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ಖಾಲಿ);
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }

    lv_ಫಲಿತಾಂಶ_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    ಗಾಗಿ(i = 0; i < disp->inv_p; i++) {
        ಆದರೆ(lv_area_is_ಒಳ(&com_area, &disp->inv_areas[i], 0) != false) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    ಆದರೆ(disp->inv_p >= LV_INV_BUF_ಗಾತ್ರ) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
ಶೂನ್ಯ lv_refr_ಗಣ_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

ಶೂನ್ಯ lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    ಆದರೆ(timer) {
        disp_refr = timer->user_ದತ್ತಾಂಶ;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        disp_refr = lv_display_get_default();
    }

    ಆದರೆ(disp_refr == ಖಾಲಿ) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    ಆದರೆ(!(buf_act && buf_act->ದತ್ತಾಂಶ && buf_act->ದತ್ತಾಂಶ_ಗಾತ್ರ)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#ಆದರೆ !LV_USE_PERF_MONITOR
    ಆದರೆ(timer) lv_timer_pause(timer);
#ಆದರೆ_ಅಂತ್ಯ

    lv_ಫಲಿತಾಂಶ_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_ಆರಂಭ, ಖಾಲಿ);
    ಆದರೆ(res == LV_ಫಲಿತಾಂಶ_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_ಟ್ಯಾಗ್("layout");
    ಆದರೆ(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    ಆದರೆ(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->ತುದಿ_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_ಕೊನೆ_ಟ್ಯಾಗ್("layout");

    /*Do nothing if there is no active screen*/
    ಆದರೆ(disp_refr->act_scr == ಖಾಲಿ) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        ಹೋಗು refr_ಮುಕ್ತಾಯ;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    ಆದರೆ(disp_refr->inv_p == 0) ಹೋಗು refr_ಮುಕ್ತಾಯ;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    ಆದರೆ((lv_display_is_double_buffered(disp_refr) && disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ನೇರ) ||
       disp_refr->sync_cb) {
        uint32_t i;
        ಗಾಗಿ(i = 0; i < disp_refr->inv_p; i++) {
            ಆದರೆ(disp_refr->inv_area_joined[i])
                ಮುಂದುವರಿ;

            lv_area_t * sync_area = lv_ll_ins_ಬಾಲ(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, ಗಾತ್ರ(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, ಗಾತ್ರ(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_ಮುಕ್ತಾಯ:

#ಆದರೆ LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_ಮುಖವಾಡ_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, ಖಾಲಿ);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_ಕೊನೆ;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_ತುದಿ_obj(ಸ್ಥಿರಾಂಕ lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ ಖಾಲಿ);

    lv_obj_t * ಸಿಕ್ಕಿತು_p = ಖಾಲಿ;

    ಆದರೆ(lv_area_is_ಒಳ(area_p, &obj->coords, 0) == false) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಆದರೆ(lv_obj_is_hidden(obj)) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಆದರೆ(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಆದರೆ(lv_obj_get_style_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN) < LV_OPA_MAX) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ಆದರೆ(info.res == LV_COVER_RES_MASKED) ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    int32_t i;
    int32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = ಮಗು_cnt - 1; i >= 0; i--) {
        lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        ಸಿಕ್ಕಿತು_p = lv_refr_get_ತುದಿ_obj(area_p, ಮಗು);

        /*If a children is ok then break*/
        ಆದರೆ(ಸಿಕ್ಕಿತು_p != ಖಾಲಿ) {
            ನಿಲ್ಲಿಸು;
        }
    }

    /*If no better children use this object*/
    ಆದರೆ(ಸಿಕ್ಕಿತು_p == ಖಾಲಿ && info.res == LV_COVER_RES_COVER) {
        ಸಿಕ್ಕಿತು_p = obj;
    }

    ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_p;
}


ಶೂನ್ಯ lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(obj != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(lv_obj_is_hidden(obj)) ಹಿಂತಿರುಗಿ;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    ಸ್ಥಿರಾಂಕ lv_opa_t opa_layered = lv_obj_get_style_opa_layered_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    ಆದರೆ(opa_layered <= LV_OPA_MIN) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿರಾಂಕ lv_opa_t layer_opa_ori = layer->opa;
    ಸ್ಥಿರಾಂಕ lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    ಸ್ಥಿರಾಂಕ lv_opa_t opa_main = lv_obj_get_style_opa_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    ಆದರೆ(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    ಆದರೆ(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    /*If the layer opa is full then use the matrix transform*/
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_ಅತಿಪ್ರವಾಹ(layer, obj)) {
        refr_obj_ಮಾತೃಕೆ(layer, obj);
    }
#ಆದರೆ_ಅಂತ್ಯ /* LV_DRAW_TRANSFORM_USE_MATRIX */
    ಇಲ್ಲದಿದ್ದರೆ {
        lv_area_t layer_area_ತುಂಬಿದ;
        lv_area_t obj_draw_ಗಾತ್ರ;
        lv_ಫಲಿತಾಂಶ_t res = layer_get_area(layer, obj, layer_type, &layer_area_ತುಂಬಿದ, &obj_draw_ಗಾತ್ರ);
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            ಹಿಂತಿರುಗಿ;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ = lv_area_get_ಎತ್ತರ(&layer_area_ತುಂಬಿದ);
        uint32_t max_argb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ = lv_area_get_ಎತ್ತರ(&layer_area_ತುಂಬಿದ);
        ಆದರೆ(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_ಅಗಲ(&layer_area_ತುಂಬಿದ);
            uint8_t px_ಗಾತ್ರ = lv_color_format_get_ಗಾತ್ರ(disp_refr->color_format);
            max_rgb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ = LV_DRAW_LAYER_SIMPLE_BUF_ಗಾತ್ರ / w / px_ಗಾತ್ರ;
            max_argb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ = LV_DRAW_LAYER_SIMPLE_BUF_ಗಾತ್ರ / w / ಗಾತ್ರ(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_ತುಂಬಿದ.x1;
        layer_area_act.x2 = layer_area_ತುಂಬಿದ.x2;
        layer_area_act.y1 = layer_area_ತುಂಬಿದ.y1;
        layer_area_act.y2 = layer_area_ತುಂಬಿದ.y1;

        ಆಗ(layer_area_act.y2 < layer_area_ತುಂಬಿದ.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ - 1;
            ಆದರೆ(layer_area_act.y2 > layer_area_ತುಂಬಿದ.y2) layer_area_act.y2 = layer_area_ತುಂಬಿದ.y2;

            ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * bitmap_ಮುಖವಾಡ_src = lv_obj_get_style_bitmap_ಮುಖವಾಡ_src_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_ಮುಖವಾಡ_src || alpha_test_area_on_obj(obj, &layer_area_act);

            ಆದರೆ(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_ಅಡ್ಡಸಾಲು_ಎತ್ತರ - 1;
                ಆದರೆ(layer_area_act.y2 > layer_area_ತುಂಬಿದ.y2) layer_area_act.y2 = layer_area_ತುಂಬಿದ.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

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

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.ಅಚ್ಚು.x = obj->coords.x1 + ಅಚ್ಚು.x - new_layer->buf_area.x1;
            layer_draw_dsc.ಅಚ್ಚು.y = obj->coords.y1 + ಅಚ್ಚು.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            ಆಗ(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            ಆಗ(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
            layer_draw_dsc.scale_x = lv_obj_get_style_transform_scale_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            layer_draw_dsc.scale_y = lv_obj_get_style_transform_scale_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_x = lv_obj_get_style_transform_skew_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            layer_draw_dsc.skew_y = lv_obj_get_style_transform_skew_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            layer_draw_dsc.blend_ಬಹುಲಕ = lv_obj_get_style_blend_ಬಹುಲಕ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
            layer_draw_dsc.antialias = disp_refr->antialiasing;
            layer_draw_dsc.bitmap_ಮುಖವಾಡ_src = bitmap_ಮುಖವಾಡ_src;
            layer_draw_dsc.image_area = obj_draw_ಗಾತ್ರ;
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
ಸ್ಥಿರ ಶೂನ್ಯ lv_refr_join_area(ಶೂನ್ಯ)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_ಒಳ;
    lv_area_t joined_area;
    ಗಾಗಿ(join_ಒಳ = 0; join_ಒಳ < disp_refr->inv_p; join_ಒಳ++) {
        ಆದರೆ(disp_refr->inv_area_joined[join_ಒಳ] != 0) ಮುಂದುವರಿ;

        /*Check all areas to join them in 'join_in'*/
        ಗಾಗಿ(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            ಆದರೆ(disp_refr->inv_area_joined[join_from] != 0 || join_ಒಳ == join_from) {
                ಮುಂದುವರಿ;
            }

            /*Check if the areas are on each other*/
            ಆದರೆ(lv_area_is_on(&disp_refr->inv_areas[join_ಒಳ], &disp_refr->inv_areas[join_from]) == false) {
                ಮುಂದುವರಿ;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_ಒಳ], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            ಆದರೆ(lv_area_get_ಗಾತ್ರ(&joined_area) < (lv_area_get_ಗಾತ್ರ(&disp_refr->inv_areas[join_ಒಳ]) +
                                                 lv_area_get_ಗಾತ್ರ(&disp_refr->inv_areas[join_from]))) {
                disp_refr->inv_areas[join_ಒಳ] = joined_area;

                /*Mark 'join_form' is joined into 'join_in'*/
                disp_refr->inv_area_joined[join_from] = 1;
            }
        }
    }
    LV_PROFILER_REFR_ಕೊನೆ;
}

/**
 * Refresh the sync areas
 */
ಸ್ಥಿರ ಶೂನ್ಯ refr_sync_areas(ಶೂನ್ಯ)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    ಸ್ಥಿರಾಂಕ bool auto_sync = disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ನೇರ &&
                           lv_display_is_double_buffered(disp_refr);
    ಸ್ಥಿರಾಂಕ bool user_sync = disp_refr->sync_cb != ಖಾಲಿ;
    ಆದರೆ(!auto_sync && !user_sync) ಹಿಂತಿರುಗಿ;

    /*Do not sync if no sync areas*/
    ಆದರೆ(lv_ll_is_ಖಾಲಿಗಳು(&disp_refr->sync_areas)) ಹಿಂತಿರುಗಿ;

    LV_PROFILER_REFR_BEGIN;
    /*With double buffered direct mode synchronize the rendered areas to the other buffer*/
    /*We need to wait for ready here to not mess up the active screen*/
    wait_for_flushing(disp_refr);

    /*Iterate through invalidated areas to see if sync area should be copied*/
    uint16_t i;
    int8_t j;
    lv_area_t res[4] = {0};
    int8_t res_c;
    lv_area_t * sync_area, * new_area, * ಮುಂದಿನ_area;
    ಗಾಗಿ(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        ಆದರೆ(disp_refr->inv_area_joined[i]) ಮುಂದುವರಿ;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_ತಲೆ(&disp_refr->sync_areas);
        ಆಗ(sync_area != ಖಾಲಿ) {
            /*Get next sync area*/
            ಮುಂದಿನ_area = lv_ll_get_ಮುಂದಿನ(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            ಆದರೆ(res_c != -1) {
                /*Replace old sync area with new areas*/
                ಗಾಗಿ(j = 0; j < res_c; j++) {
                    new_area = lv_ll_ins_prev(&disp_refr->sync_areas, sync_area);
                    *new_area = res[j];
                }
                lv_ll_remove(&disp_refr->sync_areas, sync_area);
                lv_free(sync_area);
            }

            /*Move on to next sync area*/
            sync_area = ಮುಂದಿನ_area;
        }
    }

    /*The buffers are already swapped.
     *So the active buffer is the off screen buffer where LVGL will render*/
    lv_draw_buf_t * off_screen = disp_refr->buf_act;
    /*Triple buffer sync buffer for off-screen2 updates.*/
    lv_draw_buf_t * off_screen2 = ಖಾಲಿ;
    lv_draw_buf_t * on_screen = ಖಾಲಿ;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    ಆದರೆ(auto_sync) {
        ಆದರೆ(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    ಗಾಗಿ(sync_area = lv_ll_get_ತಲೆ(&disp_refr->sync_areas); sync_area != ಖಾಲಿ;
        sync_area = lv_ll_get_ಮುಂದಿನ(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        ಆದರೆ(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            ಮುಂದುವರಿ;
        }
#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
        ಆದರೆ(lv_display_get_ಮಾತೃಕೆ_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#ಆದರೆ_ಅಂತ್ಯ /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        ಆದರೆ(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_ಕೊನೆಯ = lv_ll_get_ಬಾಲ(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        ಇಲ್ಲದಿದ್ದರೆ {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            ಆದರೆ(off_screen2 != on_screen)
                lv_draw_buf_copy(off_screen2, sync_area, on_screen, sync_area);
        }
    }

    /*Clear sync areas*/
    lv_ll_clear(&disp_refr->sync_areas);
    LV_PROFILER_REFR_ಕೊನೆ;
}

/**
 * Refresh the joined areas
 */
ಸ್ಥಿರ ಶೂನ್ಯ refr_invalid_areas(ಶೂನ್ಯ)
{
    ಆದರೆ(disp_refr->inv_p == 0) ಹಿಂತಿರುಗಿ;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_ಆರಂಭ, ಖಾಲಿ);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t ಕೊನೆಯ_i = 0;
    ಗಾಗಿ(i = disp_refr->inv_p - 1; i >= 0; i--) {
        ಆದರೆ(disp_refr->inv_area_joined[i] == 0) {
            ಕೊನೆಯ_i = i;
            ನಿಲ್ಲಿಸು;
        }
    }

    disp_refr->ಕೊನೆಯ_area = 0;
    disp_refr->ಕೊನೆಯ_part = 0;
    disp_refr->rendering_ಒಳ_ಪ್ರಗತಿ = true;

    ಗಾಗಿ(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        ಆದರೆ(disp_refr->inv_area_joined[i]) ಮುಂದುವರಿ;

        ಆದರೆ(i == ಕೊನೆಯ_i) disp_refr->ಕೊನೆಯ_area = 1;
        disp_refr->ಕೊನೆಯ_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        ಆದರೆ(disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_PARTIAL) {
            /*Calculate the max row num*/
            int32_t w = lv_area_get_ಅಗಲ(&inv_a);
            int32_t h = lv_area_get_ಎತ್ತರ(&inv_a);

            int32_t max_ಅಡ್ಡಸಾಲು = get_max_ಅಡ್ಡಸಾಲು(disp_refr, w, h);

            int32_t ಅಡ್ಡಸಾಲು;
            int32_t ಅಡ್ಡಸಾಲು_ಕೊನೆಯ = 0;
            lv_area_t sub_area;
            sub_area.x1 = inv_a.x1;
            sub_area.x2 = inv_a.x2;
            int32_t y_off = 0;
            ಗಾಗಿ(ಅಡ್ಡಸಾಲು = inv_a.y1; ಅಡ್ಡಸಾಲು + max_ಅಡ್ಡಸಾಲು - 1 <= inv_a.y2; ಅಡ್ಡಸಾಲು += max_ಅಡ್ಡಸಾಲು) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = ಅಡ್ಡಸಾಲು;
                sub_area.y2 = ಅಡ್ಡಸಾಲು + max_ಅಡ್ಡಸಾಲು - 1;
                ಆದರೆ(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                ಅಡ್ಡಸಾಲು_ಕೊನೆಯ = sub_area.y2;
                ಆದರೆ(inv_a.y2 == ಅಡ್ಡಸಾಲು_ಕೊನೆಯ) disp_refr->ಕೊನೆಯ_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ಎತ್ತರ(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            ಆದರೆ(inv_a.y2 != ಅಡ್ಡಸಾಲು_ಕೊನೆಯ) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = ಅಡ್ಡಸಾಲು;
                sub_area.y2 = inv_a.y2;
                disp_refr->ಕೊನೆಯ_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_ಎತ್ತರ(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ತುಂಬಿದ ||
                disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ನೇರ) {
            disp_refr->ಕೊನೆಯ_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, ಖಾಲಿ);
    disp_refr->rendering_ಒಳ_ಪ್ರಗತಿ = false;
    LV_PROFILER_REFR_ಕೊನೆ;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
ಸ್ಥಿರ ಶೂನ್ಯ layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
{
    lv_draw_buf_t * ret = lv_draw_buf_reshape(
                              layer->draw_buf,
                              layer->color_format,
                              lv_area_get_ಅಗಲ(&layer->buf_area),
                              lv_area_get_ಎತ್ತರ(&layer->buf_area),
                              stride);
    LV_ಬಳಸದ(ret);
    LV_ASSERT_NULL(ret);
}

/**
 * Refresh an area if there is Virtual Display Buffer
 * @param area_p  pointer to an area to refresh
 */
ಸ್ಥಿರ ಶೂನ್ಯ refr_area(ಸ್ಥಿರಾಂಕ lv_area_t * area_p, int32_t y_ಸ್ಥಾನಾಂತರ)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_ತಲೆ;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_ಸ್ಥಾನಾಂತರ = y_ಸ್ಥಾನಾಂತರ;
    layer->all_tasks_added = false;

    ಆದರೆ(disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ನೇರ ||
            disp_refr->render_ಬಹುಲಕ == LV_DISPLAY_RENDER_ಬಹುಲಕ_ತುಂಬಿದ) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        ಆದರೆ(LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ && lv_display_get_ಮಾತೃಕೆ_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->ಶೀರ್ಷಿಕೆ.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_ಎತ್ತರ(area_p);
    ಆದರೆ(LV_COLOR_FORMAT_IS_ಸೂಚ್ಯಂಕಿತ(layer->color_format) == false) {
        /* Assume that the buffer size (can be screen sized or smaller in case of partial mode)
         * and max tile size are the optimal scenario. From this calculate the ideal tile size
         * and set the tile count and tile height accordingly.
         */
        uint32_t max_tile_cnt = disp_refr->tile_cnt;
        uint32_t ಒಟ್ಟು_buf_ಗಾತ್ರ = layer->draw_buf->ದತ್ತಾಂಶ_ಗಾತ್ರ;
        uint32_t ideal_tile_ಗಾತ್ರ = ಒಟ್ಟು_buf_ಗಾತ್ರ / max_tile_cnt;
        uint32_t area_buf_ಗಾತ್ರ = lv_area_get_ಗಾತ್ರ(area_p) * lv_color_format_get_ಗಾತ್ರ(layer->color_format);

        tile_cnt = (area_buf_ಗಾತ್ರ + (ideal_tile_ಗಾತ್ರ - 1)) / ideal_tile_ಗಾತ್ರ; /*Round up*/
        tile_h = lv_area_get_ಎತ್ತರ(area_p) / tile_cnt;
    }

    ಆದರೆ(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * ಗಾತ್ರ(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        ಆದರೆ(tile_layers == ಖಾಲಿ) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_ಕೊನೆ;
            ಹಿಂತಿರುಗಿ;
        }
        uint32_t i;
        ಗಾಗಿ(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_ಗಣ(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            ಆದರೆ(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, ಖಾಲಿ, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        ಗಾಗಿ(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            ಆಗ(tile_layer->draw_task_ತಲೆ) {
                lv_draw_dispatch_wait_for_request();
                lv_draw_dispatch();
            }
            lv_draw_layer_deinit(tile_layer);
        }
        lv_free(tile_layers);

        layer->all_tasks_added = true;
    }

    disp_refr->refreshed_area = *area_p;
    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ
    ಆದರೆ(lv_display_get_ಮಾತೃಕೆ_rotation(disp_refr)) {
        ಸ್ಥಿರಾಂಕ lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        ಆದರೆ(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            ಆಯ್ಕೆ(rotation) {
                ಸ್ಥಿತಿ LV_ROTATION_90:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 270);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->ver_res, 0);
                     */
                    layer->ಮಾತೃಕೆ.m[0][0] = 0;
                    layer->ಮಾತೃಕೆ.m[0][1] = 1;
                    layer->ಮಾತೃಕೆ.m[0][2] = 0;
                    layer->ಮಾತೃಕೆ.m[1][0] = -1;
                    layer->ಮಾತೃಕೆ.m[1][1] = 0;
                    layer->ಮಾತೃಕೆ.m[1][2] = disp_refr->ver_res;
                    ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ LV_ROTATION_180:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 180);
                     * lv_matrix_translate(&layer->matrix, -disp_refr->hor_res, -disp_refr->ver_res);
                     */
                    layer->ಮಾತೃಕೆ.m[0][0] = -1;
                    layer->ಮಾತೃಕೆ.m[0][1] = 0;
                    layer->ಮಾತೃಕೆ.m[0][2] = disp_refr->hor_res;
                    layer->ಮಾತೃಕೆ.m[1][0] = 0;
                    layer->ಮಾತೃಕೆ.m[1][1] = -1;
                    layer->ಮಾತೃಕೆ.m[1][2] = disp_refr->ver_res;
                    ನಿಲ್ಲಿಸು;

                ಸ್ಥಿತಿ LV_ROTATION_270:
                    /**
                     * lv_matrix_rotate(&layer->matrix, 90);
                     * lv_matrix_translate(&layer->matrix, 0, -disp_refr->hor_res);
                     */
                    layer->ಮಾತೃಕೆ.m[0][0] = 0;
                    layer->ಮಾತೃಕೆ.m[0][1] = -1;
                    layer->ಮಾತೃಕೆ.m[0][2] = disp_refr->hor_res;
                    layer->ಮಾತೃಕೆ.m[1][0] = 1;
                    layer->ಮಾತೃಕೆ.m[1][1] = 0;
                    layer->ಮಾತೃಕೆ.m[1][2] = 0;
                    ನಿಲ್ಲಿಸು;

                ಪೂರ್ವನಿಯೋಜಿತ:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    ನಿಲ್ಲಿಸು;
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    ಆದರೆ(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    ಆದರೆ(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * ತುದಿ_act_scr = ಖಾಲಿ;
    lv_obj_t * ತುದಿ_prev_scr = ಖಾಲಿ;

    /*Get the most top object which is not covered by others*/
    ತುದಿ_act_scr = lv_refr_get_ತುದಿ_obj(&layer->_clip_area, lv_display_get_screen_ಸಕ್ರಿಯ(disp_refr));
    ಆದರೆ(disp_refr->prev_scr) {
        ತುದಿ_prev_scr = lv_refr_get_ತುದಿ_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    ಆದರೆ(ತುದಿ_act_scr == ಖಾಲಿ && ತುದಿ_prev_scr == ಖಾಲಿ) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    ಆದರೆ(disp_refr->draw_prev_over_act) {
        ಆದರೆ(ತುದಿ_act_scr == ಖಾಲಿ) ತುದಿ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, ತುದಿ_act_scr);

        /*Refresh the previous screen if any*/
        ಆದರೆ(disp_refr->prev_scr) {
            ಆದರೆ(ತುದಿ_prev_scr == ಖಾಲಿ) ತುದಿ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, ತುದಿ_prev_scr);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /*Refresh the previous screen if any*/
        ಆದರೆ(disp_refr->prev_scr) {
            ಆದರೆ(ತುದಿ_prev_scr == ಖಾಲಿ) ತುದಿ_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, ತುದಿ_prev_scr);
        }

        ಆದರೆ(ತುದಿ_act_scr == ಖಾಲಿ) ತುದಿ_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, ತುದಿ_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_ತುದಿ(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_ಕೊನೆ;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
ಸ್ಥಿರ ಶೂನ್ಯ refr_obj_and_children(lv_layer_t * layer, lv_obj_t * ತುದಿ_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    ಆದರೆ(ತುದಿ_obj == ಖಾಲಿ) ತುದಿ_obj = lv_display_get_screen_ಸಕ್ರಿಯ(disp_refr);
    ಆದರೆ(ತುದಿ_obj == ಖಾಲಿ) ಹಿಂತಿರುಗಿ;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * ಪೋಷಕ;
    lv_obj_t * border_p = ತುದಿ_obj;

    ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ತುದಿ_obj);

    ಸ್ಥಿರಾಂಕ lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    ಆದರೆ(ಪೋಷಕ) {
        layer->recolor = lv_obj_get_style_recolor_recursive(ಪೋಷಕ, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, ತುದಿ_obj);

    /*Do until not reach the screen*/
    ಆಗ(ಪೋಷಕ != ಖಾಲಿ) {
        bool go = false;
        uint32_t i;
        uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(ಪೋಷಕ);
        ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
            lv_obj_t * ಮಗು = ಪೋಷಕ->spec_attr->children[i];
            ಆದರೆ(!go) {
                ಆದರೆ(ಮಗು == border_p) go = true;
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                /*Refresh the objects*/
                lv_obj_refr(layer, ಮಗು);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(ಪೋಷಕ, LV_EVENT_DRAW_POST_BEGIN, (ಶೂನ್ಯ *)layer);
        lv_obj_send_event(ಪೋಷಕ, LV_EVENT_DRAW_POST, (ಶೂನ್ಯ *)layer);
        lv_obj_send_event(ಪೋಷಕ, LV_EVENT_DRAW_POST_ಕೊನೆ, (ಶೂನ್ಯ *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = ಪೋಷಕ;
        /*Go a level deeper*/
        ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಪೋಷಕ);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ lv_ಫಲಿತಾಂಶ_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_ಹೊರಹೋಗು, lv_area_t * obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು)
{
    int32_t ext_draw_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    lv_obj_get_coords(obj, obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು);
    lv_area_increase(obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು, ext_draw_ಗಾತ್ರ, ext_draw_ಗಾತ್ರ);

    ಆದರೆ(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_NONE);
        ಆದರೆ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_INVERSE);
        ಆದರೆ(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು)) {
            ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
        }

        *layer_area_ಹೊರಹೋಗು = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_ಹೊರಹೋಗು, 5, 5); /*To avoid rounding error*/
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        ಆದರೆ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_ಗಾತ್ರ_ಹೊರಹೋಗು)) {
            ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
        }
        *layer_area_ಹೊರಹೋಗು = clip_coords_for_obj;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        LV_LOG_WARN("Unhandled layer type");
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }

    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
}

ಸ್ಥಿರ bool alpha_test_area_on_obj(lv_obj_t * obj, ಸ್ಥಿರಾಂಕ lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    ಆದರೆ(!lv_area_is_on(area, &obj->coords)) ಹಿಂತಿರುಗಿ true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    ಆದರೆ(info.res == LV_COVER_RES_COVER) ಹಿಂತಿರುಗಿ false;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ true;
}

#ಆದರೆ LV_DRAW_TRANSFORM_USE_ಮಾತೃಕೆ

ಸ್ಥಿರ bool obj_get_ಮಾತೃಕೆ(lv_obj_t * obj, lv_ಮಾತೃಕೆ_t * ಮಾತೃಕೆ)
{
    lv_ಮಾತೃಕೆ_ತಟಸ್ಥ(ಮಾತೃಕೆ);

    ಸ್ಥಿರಾಂಕ lv_ಮಾತೃಕೆ_t * obj_ಮಾತೃಕೆ = lv_obj_get_transform(obj);
    ಆದರೆ(obj_ಮಾತೃಕೆ) {
        lv_ಮಾತೃಕೆ_translate(ಮಾತೃಕೆ, obj->coords.x1, obj->coords.y1);
        lv_ಮಾತೃಕೆ_multiply(ಮಾತೃಕೆ, obj_ಮಾತೃಕೆ);
        lv_ಮಾತೃಕೆ_translate(ಮಾತೃಕೆ, -obj->coords.x1, -obj->coords.y1);
        ಹಿಂತಿರುಗಿ true;
    }

    lv_ಬಿಂದು_t ಅಚ್ಚು = {
        .x = lv_obj_get_style_transform_ಅಚ್ಚು_x_ಆಂತರಿಕ(obj, LV_PART_MAIN),
        .y = lv_obj_get_style_transform_ಅಚ್ಚು_y_ಆಂತರಿಕ(obj, LV_PART_MAIN)
    };

    ಅಚ್ಚು.x = obj->coords.x1 + lv_pct_ಗಮ್ಯ_px(ಅಚ್ಚು.x, lv_area_get_ಅಗಲ(&obj->coords));
    ಅಚ್ಚು.y = obj->coords.y1 + lv_pct_ಗಮ್ಯ_px(ಅಚ್ಚು.y, lv_area_get_ಎತ್ತರ(&obj->coords));

    int32_t rotation = lv_obj_get_style_transform_rotation_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t scale_x = lv_obj_get_style_transform_scale_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t scale_y = lv_obj_get_style_transform_scale_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t skew_x = lv_obj_get_style_transform_skew_x_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t skew_y = lv_obj_get_style_transform_skew_y_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    /* Checked by caller */
    LV_ASSERT(scale_x > 0 && scale_y > 0);

    /* generate the obj matrix */
    lv_ಮಾತೃಕೆ_translate(ಮಾತೃಕೆ, ಅಚ್ಚು.x, ಅಚ್ಚು.y);
    ಆದರೆ(rotation != 0) {
        lv_ಮಾತೃಕೆ_rotate(ಮಾತೃಕೆ, rotation * 0.1f);
    }

    ಆದರೆ(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_ಮಾತೃಕೆ_scale(
            ಮಾತೃಕೆ,
            (ದಶಮಾಂಶ)scale_x / LV_SCALE_NONE,
            (ದಶಮಾಂಶ)scale_y / LV_SCALE_NONE
        );
    }

    ಆದರೆ(skew_x != 0 || skew_y != 0) {
        lv_ಮಾತೃಕೆ_skew(ಮಾತೃಕೆ, skew_x, skew_y);
    }

    lv_ಮಾತೃಕೆ_translate(ಮಾತೃಕೆ, -ಅಚ್ಚು.x, -ಅಚ್ಚು.y);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ refr_obj_ಮಾತೃಕೆ(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_ಮಾತೃಕೆ_t obj_ಮಾತೃಕೆ;
    ಆದರೆ(!obj_get_ಮಾತೃಕೆ(obj, &obj_ಮಾತೃಕೆ)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    lv_ಮಾತೃಕೆ_t ಮಾತೃಕೆ_inv;
    ಆದರೆ(!lv_ಮಾತೃಕೆ_inverse(&ಮಾತೃಕೆ_inv, &obj_ಮಾತೃಕೆ)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_ಕೊನೆ;
        ಹಿಂತಿರುಗಿ;
    }

    /* save original matrix */
    lv_ಮಾತೃಕೆ_t ori_ಮಾತೃಕೆ = layer->ಮಾತೃಕೆ;

    /* apply the obj matrix */
    lv_ಮಾತೃಕೆ_multiply(&layer->ಮಾತೃಕೆ, &obj_ಮಾತೃಕೆ);

    /* calculate clip area without transform */
    lv_area_t clip_area = layer->_clip_area;
    lv_area_t clip_area_ori = layer->_clip_area;
    clip_area = lv_ಮಾತೃಕೆ_transform_area(&ಮಾತೃಕೆ_inv, &clip_area);

    /* increase the clip area by 1 pixel to avoid rounding errors */
    ಆದರೆ(!lv_ಮಾತೃಕೆ_is_ತಟಸ್ಥ_or_translation(&obj_ಮಾತೃಕೆ)) {
        lv_area_increase(&clip_area, 1, 1);
    }

    layer->_clip_area = clip_area;

    /* redraw obj */
    lv_obj_redraw(layer, obj);

    /* restore original matrix */
    layer->ಮಾತೃಕೆ = ori_ಮಾತೃಕೆ;
    /* restore clip area */
    layer->_clip_area = clip_area_ori;
    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ bool refr_check_obj_clip_ಅತಿಪ್ರವಾಹ(lv_layer_t * layer, lv_obj_t * obj)
{
    ಆದರೆ(lv_obj_get_style_transform_rotation_ಆಂತರಿಕ(obj, LV_PART_MAIN) == 0) {
        ಹಿಂತಿರುಗಿ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_ಗಾತ್ರ = lv_obj_get_ext_draw_ಗಾತ್ರ(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_ಗಾತ್ರ, ext_ಗಾತ್ರ);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_ಬಿಂದು_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    ಆದರೆ(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        ಹಿಂತಿರುಗಿ true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, ಗಾತ್ರ(lv_area_t)) != 0;
    ಹಿಂತಿರುಗಿ has_clip;
}

#ಆದರೆ_ಅಂತ್ಯ /* LV_DRAW_TRANSFORM_USE_MATRIX */

ಸ್ಥಿರ uint32_t get_max_ಅಡ್ಡಸಾಲು(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_ಅಗಲ_ಗಮ್ಯ_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_ಸೂಚ್ಯಂಕಿತ_PALETTE_ಗಾತ್ರ(cf) * ಗಾತ್ರ(lv_color32_t);

    ಆದರೆ(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        ಹಿಂತಿರುಗಿ 0;
    }

    int32_t max_ಅಡ್ಡಸಾಲು = (uint32_t)(disp->buf_act->ದತ್ತಾಂಶ_ಗಾತ್ರ - overhead) / stride;

    ಆದರೆ(max_ಅಡ್ಡಸಾಲು > area_h) max_ಅಡ್ಡಸಾಲು = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_ಅಡ್ಡಸಾಲು;
    ಮಾಡು {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        ಆದರೆ(lv_area_get_ಎತ್ತರ(&tmp) <= max_ಅಡ್ಡಸಾಲು) ನಿಲ್ಲಿಸು;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } ಆಗ(h_tmp > 0);

    ಆದರೆ(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        ಹಿಂತಿರುಗಿ 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        max_ಅಡ್ಡಸಾಲು = tmp.y2 + 1;
    }

    ಹಿಂತಿರುಗಿ max_ಅಡ್ಡಸಾಲು;
}

/**
 * Flush the content of the draw buffer
 */
ಸ್ಥಿರ ಶೂನ್ಯ draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_ತಲೆ;

    ಆಗ(layer->draw_task_ತಲೆ) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_ಸಂದೇಶ(layer->ಮುಂದಿನ == ಖಾಲಿ, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    ಆದರೆ(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    ಆದರೆ(disp->ಕೊನೆಯ_area && disp->ಕೊನೆಯ_part) disp->flushing_ಕೊನೆಯ = 1;
    ಇಲ್ಲದಿದ್ದರೆ disp->flushing_ಕೊನೆಯ = 0;

    bool flushing_ಕೊನೆಯ = disp->flushing_ಕೊನೆಯ;

    ಆದರೆ(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->ದತ್ತಾಂಶ);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    ಆದರೆ(lv_display_is_double_buffered(disp) && (disp->render_ಬಹುಲಕ != LV_DISPLAY_RENDER_ಬಹುಲಕ_ನೇರ || flushing_ಕೊನೆಯ)) {
        ಆದರೆ(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            disp->buf_act = disp->buf_1;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ call_flush_cb(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (ಪೂರ್ಣಾಂಕ)area->x1, (ಪೂರ್ಣಾಂಕ)area->y1, (ಪೂರ್ಣಾಂಕ)area->x2, (ಪೂರ್ಣಾಂಕ)area->y2, (ಶೂನ್ಯ *)px_map);

    lv_area_t ಸ್ಥಾನಾಂತರ_area = {
        .x1 = area->x1 + disp->ಸ್ಥಾನಾಂತರ_x,
        .y1 = area->y1 + disp->ಸ್ಥಾನಾಂತರ_y,
        .x2 = area->x2 + disp->ಸ್ಥಾನಾಂತರ_x,
        .y2 = area->y2 + disp->ಸ್ಥಾನಾಂತರ_y
    };

    lv_display_send_event(disp, LV_EVENT_FLUSH_ಆರಂಭ, &ಸ್ಥಾನಾಂತರ_area);

    disp->flush_cb(disp, &ಸ್ಥಾನಾಂತರ_area, px_map);
    lv_display_send_event(disp, LV_EVENT_FLUSH_ಮುಕ್ತಾಯ, &ಸ್ಥಾನಾಂತರ_area);

    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ಆರಂಭ, ಖಾಲಿ);

    ಆದರೆ(disp->flush_wait_cb) {
        ಆದರೆ(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆಗ(disp->flushing);
    }
    disp->flushing_ಕೊನೆಯ = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_ಮುಕ್ತಾಯ, ಖಾಲಿ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ call_sync_cb(lv_display_t * disp, ಸ್ಥಿರಾಂಕ lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t ಸ್ಥಾನಾಂತರ_area = *area;
    ಸ್ಥಾನಾಂತರ_area.x1 += disp->ಸ್ಥಾನಾಂತರ_x;
    ಸ್ಥಾನಾಂತರ_area.x2 += disp->ಸ್ಥಾನಾಂತರ_x;
    ಸ್ಥಾನಾಂತರ_area.y1 += disp->ಸ್ಥಾನಾಂತರ_y;
    ಸ್ಥಾನಾಂತರ_area.y2 += disp->ಸ್ಥಾನಾಂತರ_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನಾಂತರ_area.x1, (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನಾಂತರ_area.y1,
                  (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನಾಂತರ_area.x2, (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನಾಂತರ_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_ಆರಂಭ, (ಶೂನ್ಯ *)&ಸ್ಥಾನಾಂತರ_area);

    disp->sync_cb(disp, &ಸ್ಥಾನಾಂತರ_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_ಮುಕ್ತಾಯ, (ಶೂನ್ಯ *)&ಸ್ಥಾನಾಂತರ_area);

    LV_PROFILER_REFR_ಕೊನೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ಆರಂಭ, ಖಾಲಿ);

    ಆದರೆ(disp->sync_wait_cb) {
        ಆದರೆ(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆಗ(disp->syncing);
    }
    disp->syncing_ಕೊನೆಯ = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_ಮುಕ್ತಾಯ, ಖಾಲಿ);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_ಕೊನೆ;
}
