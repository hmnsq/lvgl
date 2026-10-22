/**
 * @file lv_refr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_refr_private.h"
#अन्तर्भाव "lv_obj_draw_private.h"
#अन्तर्भाव "../misc/lv_area_private.h"
#अन्तर्भाव "../draw/sw/lv_draw_sw_mask_private.h"
#अन्तर्भाव "lv_obj_private.h"
#अन्तर्भाव "lv_obj_event_private.h"
#अन्तर्भाव "../display/lv_display_private.h"
#अन्तर्भाव "../misc/lv_timer_private.h"
#अन्तर्भाव "../draw/lv_draw_private.h"
#अन्तर्भाव "lv_global.h"
#अन्तर्भाव "../lvgl_public.h"
#अन्तर्भाव "lv_obj_style_internal.h"

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
स्थिर शून्य lv_refr_join_area(शून्य);
स्थिर शून्य refr_invalid_areas(शून्य);
स्थिर शून्य refr_sync_areas(शून्य);
स्थिर शून्य refr_area(अचर lv_area_t * area_p, int32_t y_offset);
स्थिर शून्य refr_configured_layer(lv_layer_t * layer);
स्थिर शून्य refr_obj_and_children(lv_layer_t * layer, lv_obj_t * शीर्ष_obj);
स्थिर uint32_t get_max_row(lv_display_t * disp, int32_t area_w, int32_t area_h);
स्थिर शून्य draw_buf_flush(lv_display_t * disp);
स्थिर शून्य call_flush_cb(lv_display_t * disp, अचर lv_area_t * area, uint8_t * px_map);
स्थिर शून्य wait_for_flushing(lv_display_t * disp);
स्थिर शून्य call_sync_cb(lv_display_t * disp, अचर lv_area_t * area);
स्थिर शून्य wait_for_syncing(lv_display_t * disp);
स्थिर lv_result_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_out, lv_area_t * obj_draw_size_out);
स्थिर bool alpha_test_area_on_obj(lv_obj_t * obj, अचर lv_area_t * area);
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    स्थिर bool refr_check_obj_clip_overflow(lv_layer_t * layer, lv_obj_t * obj);
    स्थिर शून्य refr_obj_matrix(lv_layer_t * layer, lv_obj_t * obj);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
#यदि LV_USE_LOG && LV_LOG_TRACE_DISP_REFR
    #define LV_TRACE_REFR(...) LV_LOG_TRACE(__VA_ARGS__)
#अन्यथा
    #define LV_TRACE_REFR(...)
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the screen refresh subsystem
 */
शून्य lv_refr_init(शून्य)
{
}

शून्य lv_refr_deinit(शून्य)
{
}

शून्य lv_refr_now(lv_display_t * disp)
{
    lv_anim_refr_now();

    यदि(disp) {
        यदि(disp->refr_timer) lv_display_refr_timer(disp->refr_timer);
    }
    अन्यथा {
        lv_display_t * d;
        d = lv_display_get_next(रिक्त);
        जबसम्म(d) {
            यदि(d->refr_timer) lv_display_refr_timer(d->refr_timer);
            d = lv_display_get_next(d);
        }
    }
}

शून्य lv_obj_redraw(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != रिक्त, फर्काऊ);
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);

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

    यदि(!lv_area_intersect(&clip_coords_for_obj, &clip_area_ori, &obj_coords_ext)) {
        LV_PROFILER_REFR_END;
        फर्काऊ;
    }
    /*If the object is visible on the current clip area*/
    layer->_clip_area = clip_coords_for_obj;

    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_BEGIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN, layer);
    lv_obj_send_event(obj, LV_EVENT_DRAW_MAIN_END, layer);
#यदि LV_USE_REFR_DEBUG
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

    अचर lv_area_t * obj_coords;
    यदि(lv_obj_is_overflow_visible(obj)) {
        obj_coords = &obj_coords_ext;
    }
    अन्यथा {
        obj_coords = &obj->coords;
    }
    lv_area_t clip_coords_for_children;
    bool refr_children = true;
    यदि(!lv_area_intersect(&clip_coords_for_children, &layer->_clip_area, obj_coords) || layer->opa <= LV_OPA_MIN) {
        refr_children = false;
    }

    यदि(refr_children) {
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गन्ती(obj);
        यदि(child_cnt == 0) {
            /*If the object was visible on the clip area call the post draw events too*/
            /*If all the children are redrawn make 'post draw' draw*/
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
            lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer);
        }
        अन्यथा {
            layer->_clip_area = clip_coords_for_children;
            bool clip_corner = lv_obj_get_style_clip_corner_internal(obj, LV_PART_MAIN);

            int32_t radius = 0;
            यदि(clip_corner) {
                radius = lv_obj_get_style_radius_internal(obj, LV_PART_MAIN);
                यदि(radius == 0) clip_corner = false;
            }

            यदि(clip_corner == false) {
                लागि(i = 0; i < child_cnt; i++) {
                    lv_obj_t * child = obj->spec_attr->children[i];
                    lv_obj_refr(layer, child);
                }

                /*If the object was visible on the clip area call the post draw events too*/
                /*If all the children are redrawn make 'post draw' draw*/
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer);
                lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer);
            }
            अन्यथा {
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
                यदि(lv_area_intersect(&bottom, &bottom, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &bottom);

                    लागि(i = 0; i < child_cnt; i++) {
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

                lv_area_t शीर्ष = obj->coords;
                शीर्ष.y2 = शीर्ष.y1 + rout - 1;
                यदि(lv_area_intersect(&शीर्ष, &शीर्ष, &layer->_clip_area)) {
                    layer_children = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, &शीर्ष);

                    लागि(i = 0; i < child_cnt; i++) {
                        lv_obj_t * child = obj->spec_attr->children[i];
                        lv_obj_refr(layer_children, child);
                    }

                    /*If all the children are redrawn send 'post draw' draw*/
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_BEGIN, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST, layer_children);
                    lv_obj_send_event(obj, LV_EVENT_DRAW_POST_END, layer_children);

                    lv_draw_mask_rect(layer_children, &mask_draw_dsc);

                    img_draw_dsc.src = layer_children;
                    lv_draw_layer(layer, &img_draw_dsc, &शीर्ष);

                }

                lv_area_t mid = obj->coords;
                mid.y1 += rout;
                mid.y2 -= rout;
                यदि(lv_area_intersect(&mid, &mid, &layer->_clip_area)) {
                    layer->_clip_area = mid;
                    लागि(i = 0; i < child_cnt; i++) {
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

lv_result_t lv_inv_area(lv_display_t * disp, अचर lv_area_t * area_p)
{
    यदि(!disp) फर्काऊ LV_RESULT_INVALID;
    यदि(!lv_display_is_invalidation_enabled(disp)) फर्काऊ LV_RESULT_INVALID;

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
    LV_ASSERT_सन्देश(!disp->rendering_in_progress, "Invalidate area is not allowed during rendering.");

    /*Clear the invalidate buffer if the parameter is NULL*/
    यदि(area_p == रिक्त) {
        disp->inv_p = 0;
        फर्काऊ LV_RESULT_OK;
    }

    lv_area_t scr_area;
    scr_area.x1 = 0;
    scr_area.y1 = 0;
    scr_area.x2 = lv_display_get_horizontal_resolution(disp) - 1;
    scr_area.y2 = lv_display_get_vertical_resolution(disp) - 1;

    lv_area_t com_area;
    bool suc;

    suc = lv_area_intersect(&com_area, area_p, &scr_area);
    यदि(suc == false)  फर्काऊ LV_RESULT_INVALID; /*Out of the screen*/

    यदि(disp->color_format == LV_COLOR_FORMAT_I1) {
        /*Make sure that the X coordinates start and end on byte boundary.
         *E.g. convert 11;27 to 8;31*/
        com_area.x1 &= ~0x7; /*Round down: Nx8*/
        com_area.x2 |= 0x7;    /*Round up: Nx8 - 1*/
    }

    /*If there were at least 1 invalid area in full refresh mode, redraw the whole screen*/
    यदि(disp->render_mode == LV_DISPLAY_RENDER_MODE_FULL) {
        disp->inv_areas[0] = scr_area;
        disp->inv_p = 1;
        lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, रिक्त);
        फर्काऊ LV_RESULT_OK;
    }

    lv_result_t res = lv_display_send_event(disp, LV_EVENT_INVALIDATE_AREA, &com_area);
    यदि(res != LV_RESULT_OK) फर्काऊ LV_RESULT_INVALID;

    /*Save only if this area is not in one of the saved areas*/
    uint16_t i;
    लागि(i = 0; i < disp->inv_p; i++) {
        यदि(lv_area_is_in(&com_area, &disp->inv_areas[i], 0) != false) फर्काऊ LV_RESULT_OK;
    }

    /*Save the area*/
    lv_area_t * tmp_area_p = &com_area;
    यदि(disp->inv_p >= LV_INV_BUF_SIZE) { /*If no place for the area add the screen*/
        disp->inv_p = 0;
        tmp_area_p = &scr_area;
    }
    disp->inv_areas[disp->inv_p] = *tmp_area_p;
    disp->inv_p++;

    lv_display_send_event(disp, LV_EVENT_REFR_REQUEST, रिक्त);

    फर्काऊ LV_RESULT_OK;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
lv_display_t * lv_refr_get_disp_refreshing(शून्य)
{
    फर्काऊ disp_refr;
}

/**
 * Get the display which is being refreshed
 * @return the display being refreshed
 */
शून्य lv_refr_set_disp_refreshing(lv_display_t * disp)
{
    disp_refr = disp;
}

शून्य lv_display_refr_timer(lv_timer_t * timer)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("begin");

    यदि(timer) {
        disp_refr = timer->user_data;
    }
    अन्यथा {
        disp_refr = lv_display_get_default();
    }

    यदि(disp_refr == रिक्त) {
        LV_LOG_WARN("No display registered");
        LV_PROFILER_REFR_END;
        फर्काऊ;
    }

    lv_draw_buf_t * buf_act = disp_refr->buf_act;
    यदि(!(buf_act && buf_act->data && buf_act->data_size)) {
        LV_LOG_WARN("No draw buffer");
        LV_PROFILER_REFR_END;
        फर्काऊ;
    }

    /* Ensure the timer does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     * However if the performance monitor is enabled keep the timer running to count the FPS.
     * Pause here where the draw buffer exists otherwise lv_display_refr_timer() may not be
     * called again which would yield a blank screen */
#यदि !LV_USE_PERF_MONITOR
    यदि(timer) lv_timer_pause(timer);
#endif

    lv_result_t res = lv_display_send_event(disp_refr, LV_EVENT_REFR_START, रिक्त);
    यदि(res == LV_RESULT_INVALID) {
        LV_TRACE_REFR("deleted");
        LV_PROFILER_REFR_END;
        फर्काऊ;
    }

    /*Refresh the screen's layout if required*/
    LV_PROFILER_LAYOUT_BEGIN_TAG("layout");
    यदि(disp_refr->act_scr) lv_obj_update_layout(disp_refr->act_scr);
    यदि(disp_refr->prev_scr) lv_obj_update_layout(disp_refr->prev_scr);

    lv_obj_update_layout(disp_refr->bottom_layer);
    lv_obj_update_layout(disp_refr->शीर्ष_layer);
    lv_obj_update_layout(disp_refr->sys_layer);
    LV_PROFILER_LAYOUT_END_TAG("layout");

    /*Do nothing if there is no active screen*/
    यदि(disp_refr->act_scr == रिक्त) {
        disp_refr->inv_p = 0;
        LV_LOG_WARN("there is no active screen");
        गच्छ refr_finish;
    }

    /*Expand the invalidated areas for blur objects before joining them*/
    lv_obj_invalidate_expand_blur(disp_refr);
    lv_refr_join_area();
    refr_sync_areas();
    refr_invalid_areas();

    यदि(disp_refr->inv_p == 0) गच्छ refr_finish;
    /*In double buffered direct mode or if sync callback is set, save the updated areas.
     *They will be used on the next call to synchronize the buffers.*/
    यदि((lv_display_is_double_buffered(disp_refr) && disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT) ||
       disp_refr->sync_cb) {
        uint32_t i;
        लागि(i = 0; i < disp_refr->inv_p; i++) {
            यदि(disp_refr->inv_area_joined[i])
                जारी;

            lv_area_t * sync_area = lv_ll_ins_tail(&disp_refr->sync_areas);
            *sync_area = disp_refr->inv_areas[i];
        }
    }

    lv_memzero(disp_refr->inv_areas, परिमाण(disp_refr->inv_areas));
    lv_memzero(disp_refr->inv_area_joined, परिमाण(disp_refr->inv_area_joined));
    disp_refr->inv_p = 0;

refr_finish:

#यदि LV_DRAW_SW_COMPLEX == 1
    lv_draw_sw_mask_cleanup();
#endif

    lv_display_send_event(disp_refr, LV_EVENT_REFR_READY, रिक्त);

    LV_TRACE_REFR("finished");
    LV_PROFILER_REFR_END;
}

/**
 * Search the most top object which fully covers an area
 * @param area_p pointer to an area
 * @param obj the first object to start the searching (typically a screen)
 * @return
 */
lv_obj_t * lv_refr_get_शीर्ष_obj(अचर lv_area_t * area_p, lv_obj_t * obj)
{
    LV_CHECK_ARG(area_p != रिक्त, फर्काऊ रिक्त);
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ रिक्त);

    lv_obj_t * found_p = रिक्त;

    यदि(lv_area_is_in(area_p, &obj->coords, 0) == false) फर्काऊ रिक्त;
    यदि(lv_obj_is_hidden(obj)) फर्काऊ रिक्त;
    यदि(lv_obj_get_layer_type(obj) != LV_LAYER_TYPE_NONE) फर्काऊ रिक्त;
    यदि(lv_obj_get_style_opa_internal(obj, LV_PART_MAIN) < LV_OPA_MAX) फर्काऊ रिक्त;

    /*If this object is fully cover the draw area then check the children too*/
    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area_p;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    यदि(info.res == LV_COVER_RES_MASKED) फर्काऊ रिक्त;

    int32_t i;
    int32_t child_cnt = lv_obj_get_child_गन्ती(obj);
    लागि(i = child_cnt - 1; i >= 0; i--) {
        lv_obj_t * child = obj->spec_attr->children[i];
        found_p = lv_refr_get_शीर्ष_obj(area_p, child);

        /*If a children is ok then break*/
        यदि(found_p != रिक्त) {
            रोक;
        }
    }

    /*If no better children use this object*/
    यदि(found_p == रिक्त && info.res == LV_COVER_RES_COVER) {
        found_p = obj;
    }

    फर्काऊ found_p;
}


शून्य lv_obj_refr(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_CHECK_ARG(layer != रिक्त, फर्काऊ);
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);

    यदि(lv_obj_is_hidden(obj)) फर्काऊ;

    /*If `opa_layered != LV_OPA_COVER` draw the widget on a new layer and blend that layer with the given opacity.*/
    अचर lv_opa_t opa_layered = lv_obj_get_style_opa_layered_internal(obj, LV_PART_MAIN);
    यदि(opa_layered <= LV_OPA_MIN) फर्काऊ;

    यदि(lv_obj_get_style_transform_scale_x(obj, LV_PART_MAIN) <= 0) फर्काऊ;
    यदि(lv_obj_get_style_transform_scale_y(obj, LV_PART_MAIN) <= 0) फर्काऊ;

    अचर lv_opa_t layer_opa_ori = layer->opa;
    अचर lv_color32_t layer_recolor = layer->recolor;

    /*Normal `opa` (not layered) will just scale down `bg_opa`, `text_opa`, etc, in the upcoming drawings.*/
    अचर lv_opa_t opa_main = lv_obj_get_style_opa_internal(obj, LV_PART_MAIN);
    यदि(opa_main < LV_OPA_MAX) {
        layer->opa = LV_OPA_MIX2(layer_opa_ori, opa_main);
    }

    layer->recolor = lv_obj_style_apply_recolor(obj, LV_PART_MAIN, layer->recolor);

    lv_layer_type_t layer_type = lv_obj_get_layer_type(obj);
    यदि(layer_type == LV_LAYER_TYPE_NONE) {
        lv_obj_redraw(layer, obj);
    }
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    /*If the layer opa is full then use the matrix transform*/
    अन्यथा यदि(opa_layered >= LV_OPA_MAX && !refr_check_obj_clip_overflow(layer, obj)) {
        refr_obj_matrix(layer, obj);
    }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */
    अन्यथा {
        lv_area_t layer_area_full;
        lv_area_t obj_draw_size;
        lv_result_t res = layer_get_area(layer, obj, layer_type, &layer_area_full, &obj_draw_size);
        यदि(res != LV_RESULT_OK) {
            layer->opa = layer_opa_ori;
            layer->recolor = layer_recolor;
            फर्काऊ;
        }

        /*Simple layers can be subdivided into smaller layers*/
        uint32_t max_rgb_row_height = lv_area_get_height(&layer_area_full);
        uint32_t max_argb_row_height = lv_area_get_height(&layer_area_full);
        यदि(layer_type == LV_LAYER_TYPE_SIMPLE) {
            int32_t w = lv_area_get_width(&layer_area_full);
            uint8_t px_size = lv_color_format_get_size(disp_refr->color_format);
            max_rgb_row_height = LV_DRAW_LAYER_SIMPLE_BUF_SIZE / w / px_size;
            max_argb_row_height = LV_DRAW_LAYER_SIMPLE_BUF_SIZE / w / परिमाण(lv_color32_t);
        }

        lv_area_t layer_area_act;
        layer_area_act.x1 = layer_area_full.x1;
        layer_area_act.x2 = layer_area_full.x2;
        layer_area_act.y1 = layer_area_full.y1;
        layer_area_act.y2 = layer_area_full.y1;

        जबसम्म(layer_area_act.y2 < layer_area_full.y2) {
            /* Test with an RGB layer size (which is larger than the ARGB layer size)
             * If it really doesn't need alpha use it. Else switch to the ARGB size*/
            layer_area_act.y2 = layer_area_act.y1 + max_rgb_row_height - 1;
            यदि(layer_area_act.y2 > layer_area_full.y2) layer_area_act.y2 = layer_area_full.y2;

            अचर शून्य * bitmap_mask_src = lv_obj_get_style_bitmap_mask_src_internal(obj, LV_PART_MAIN);
            bool area_need_alpha = bitmap_mask_src || alpha_test_area_on_obj(obj, &layer_area_act);

            यदि(area_need_alpha) {
                layer_area_act.y2 = layer_area_act.y1 + max_argb_row_height - 1;
                यदि(layer_area_act.y2 > layer_area_full.y2) layer_area_act.y2 = layer_area_full.y2;
            }

            lv_layer_t * new_layer = lv_draw_layer_create(layer,
                                                          area_need_alpha ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_DEFAULT, &layer_area_act);
            lv_obj_redraw(new_layer, obj);

            lv_point_t pivot = {
                .x = lv_obj_get_style_transform_pivot_x_internal(obj, LV_PART_MAIN),
                .y = lv_obj_get_style_transform_pivot_y_internal(obj, LV_PART_MAIN)
            };

            यदि(LV_COORD_IS_PCT(pivot.x)) {
                pivot.x = (LV_COORD_GET_PCT(pivot.x) * lv_area_get_width(&obj->coords)) / 100;
            }
            यदि(LV_COORD_IS_PCT(pivot.y)) {
                pivot.y = (LV_COORD_GET_PCT(pivot.y) * lv_area_get_height(&obj->coords)) / 100;
            }

            lv_draw_image_dsc_t layer_draw_dsc;
            lv_draw_image_dsc_init(&layer_draw_dsc);
            layer_draw_dsc.pivot.x = obj->coords.x1 + pivot.x - new_layer->buf_area.x1;
            layer_draw_dsc.pivot.y = obj->coords.y1 + pivot.y - new_layer->buf_area.y1;

            layer_draw_dsc.opa = opa_layered;
            layer_draw_dsc.rotation = lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN);
            जबसम्म(layer_draw_dsc.rotation > 3600) layer_draw_dsc.rotation -= 3600;
            जबसम्म(layer_draw_dsc.rotation < 0) layer_draw_dsc.rotation += 3600;
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
स्थिर शून्य lv_refr_join_area(शून्य)
{
    LV_PROFILER_REFR_BEGIN;
    uint32_t join_from;
    uint32_t join_in;
    lv_area_t joined_area;
    लागि(join_in = 0; join_in < disp_refr->inv_p; join_in++) {
        यदि(disp_refr->inv_area_joined[join_in] != 0) जारी;

        /*Check all areas to join them in 'join_in'*/
        लागि(join_from = 0; join_from < disp_refr->inv_p; join_from++) {
            /*Handle only unjoined areas and ignore itself*/
            यदि(disp_refr->inv_area_joined[join_from] != 0 || join_in == join_from) {
                जारी;
            }

            /*Check if the areas are on each other*/
            यदि(lv_area_is_on(&disp_refr->inv_areas[join_in], &disp_refr->inv_areas[join_from]) == false) {
                जारी;
            }

            lv_area_join(&joined_area, &disp_refr->inv_areas[join_in], &disp_refr->inv_areas[join_from]);

            /*Join two area only if the joined area size is smaller*/
            यदि(lv_area_get_size(&joined_area) < (lv_area_get_size(&disp_refr->inv_areas[join_in]) +
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
स्थिर शून्य refr_sync_areas(शून्य)
{
    /*Do not sync if not direct double buffered and no sync callback set*/
    अचर bool auto_sync = disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT &&
                           lv_display_is_double_buffered(disp_refr);
    अचर bool user_sync = disp_refr->sync_cb != रिक्त;
    यदि(!auto_sync && !user_sync) फर्काऊ;

    /*Do not sync if no sync areas*/
    यदि(lv_ll_is_empty(&disp_refr->sync_areas)) फर्काऊ;

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
    लागि(i = 0; i < disp_refr->inv_p; i++) {
        /*Skip joined areas*/
        यदि(disp_refr->inv_area_joined[i]) जारी;

        /*Iterate over sync areas*/
        sync_area = lv_ll_get_head(&disp_refr->sync_areas);
        जबसम्म(sync_area != रिक्त) {
            /*Get next sync area*/
            next_area = lv_ll_get_next(&disp_refr->sync_areas, sync_area);

            /*Remove intersect of redraw area from sync area and get remaining areas*/
            res_c = lv_area_diff(res, sync_area, &disp_refr->inv_areas[i]);

            /*New sub areas created after removing intersect*/
            यदि(res_c != -1) {
                /*Replace old sync area with new areas*/
                लागि(j = 0; j < res_c; j++) {
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
    lv_draw_buf_t * off_screen2 = रिक्त;
    lv_draw_buf_t * on_screen = रिक्त;

    /* Only compute buffer relationships when auto_sync (direct double-buffered) is used.
     * When only user_sync is active, these pointers are not needed. */
    यदि(auto_sync) {
        यदि(disp_refr->buf_act == disp_refr->buf_1) {
            off_screen2 = disp_refr->buf_2;
            on_screen = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_2;
        }
        अन्यथा यदि(disp_refr->buf_act == disp_refr->buf_2) {
            off_screen2 = disp_refr->buf_3 ? disp_refr->buf_3 : disp_refr->buf_1;
            on_screen = disp_refr->buf_1;
        }
        अन्यथा {
            off_screen2 = disp_refr->buf_1;
            on_screen = disp_refr->buf_2;
        }
    }

    uint32_t hor_res = lv_display_get_horizontal_resolution(disp_refr);
    uint32_t ver_res = lv_display_get_vertical_resolution(disp_refr);

    lv_area_t disp_area = {0, 0, (int32_t)hor_res - 1, (int32_t)ver_res - 1};
    /*Copy sync areas (if any remaining)*/
    लागि(sync_area = lv_ll_get_head(&disp_refr->sync_areas); sync_area != रिक्त;
        sync_area = lv_ll_get_next(&disp_refr->sync_areas, sync_area)) {
        /**
         * @todo Resize SDL window will trigger crash because of sync_area is larger than disp_area
         */
        यदि(!lv_area_intersect(sync_area, sync_area, &disp_area)) {
            जारी;
        }
#यदि LV_DRAW_TRANSFORM_USE_MATRIX
        यदि(lv_display_get_matrix_rotation(disp_refr)) {
            lv_display_rotate_area(disp_refr, sync_area);
        }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */
        /*Call sync callback (if set)*/
        यदि(disp_refr->sync_cb) {
            /*Set syncing flags*/
            disp_refr->syncing = true;
            disp_refr->syncing_last = lv_ll_get_tail(&disp_refr->sync_areas) == sync_area;

            /*Call sync callback and wait for sync to complete*/
            call_sync_cb(disp_refr, sync_area);
            wait_for_syncing(disp_refr);
        }
        /*Fallback to internal double buffered direct mode sync*/
        अन्यथा {
            lv_draw_buf_copy(off_screen, sync_area, on_screen, sync_area);
            यदि(off_screen2 != on_screen)
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
स्थिर शून्य refr_invalid_areas(शून्य)
{
    यदि(disp_refr->inv_p == 0) फर्काऊ;
    LV_PROFILER_REFR_BEGIN;

    /*Notify the display driven rendering has started*/
    lv_display_send_event(disp_refr, LV_EVENT_RENDER_START, रिक्त);

    /*Find the last area which will be drawn*/
    int32_t i;
    int32_t last_i = 0;
    लागि(i = disp_refr->inv_p - 1; i >= 0; i--) {
        यदि(disp_refr->inv_area_joined[i] == 0) {
            last_i = i;
            रोक;
        }
    }

    disp_refr->last_area = 0;
    disp_refr->last_part = 0;
    disp_refr->rendering_in_progress = true;

    लागि(i = 0; i < (int32_t)disp_refr->inv_p; i++) {
        /*Refresh the unjoined areas*/
        यदि(disp_refr->inv_area_joined[i]) जारी;

        यदि(i == last_i) disp_refr->last_area = 1;
        disp_refr->last_part = 0;

        lv_area_t inv_a = disp_refr->inv_areas[i];
        यदि(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_PARTIAL) {
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
            लागि(row = inv_a.y1; row + max_row - 1 <= inv_a.y2; row += max_row) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = row;
                sub_area.y2 = row + max_row - 1;
                यदि(sub_area.y2 > inv_a.y2) sub_area.y2 = inv_a.y2;
                row_last = sub_area.y2;
                यदि(inv_a.y2 == row_last) disp_refr->last_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_height(&sub_area);
                draw_buf_flush(disp_refr);
            }

            /*If the last y coordinates are not handled yet ...*/
            यदि(inv_a.y2 != row_last) {
                /*Calc. the next y coordinates of draw_buf*/
                sub_area.y1 = row;
                sub_area.y2 = inv_a.y2;
                disp_refr->last_part = 1;
                refr_area(&sub_area, y_off);
                y_off += lv_area_get_height(&sub_area);
                draw_buf_flush(disp_refr);
            }
        }
        अन्यथा यदि(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_FULL ||
                disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT) {
            disp_refr->last_part = 1;
            refr_area(&disp_refr->inv_areas[i], 0);
            draw_buf_flush(disp_refr);
        }
    }

    lv_display_send_event(disp_refr, LV_EVENT_RENDER_READY, रिक्त);
    disp_refr->rendering_in_progress = false;
    LV_PROFILER_REFR_END;
}

/**
 * Reshape the draw buffer if required
 * @param layer  pointer to a layer which will be drawn
 */
स्थिर शून्य layer_reshape_draw_buf(lv_layer_t * layer, uint32_t stride)
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
स्थिर शून्य refr_area(अचर lv_area_t * area_p, int32_t y_offset)
{
    LV_PROFILER_REFR_BEGIN;
    lv_layer_t * layer = disp_refr->layer_head;
    layer->draw_buf = disp_refr->buf_act;
    layer->_clip_area = *area_p;
    layer->phy_clip_area = *area_p;
    layer->partial_y_offset = y_offset;
    layer->all_tasks_added = false;

    यदि(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_PARTIAL) {
        /*In partial mode render this area to the buffer*/
        layer->buf_area = *area_p;
        layer_reshape_draw_buf(layer, LV_STRIDE_AUTO);
    }
    अन्यथा यदि(disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_DIRECT ||
            disp_refr->render_mode == LV_DISPLAY_RENDER_MODE_FULL) {
        /*In direct mode and full mode the buffer area is always the whole screen, not considering rotation*/
        layer->buf_area.x1 = 0;
        layer->buf_area.y1 = 0;

        यदि(LV_DRAW_TRANSFORM_USE_MATRIX && lv_display_get_matrix_rotation(disp_refr)) {
            layer->buf_area.x2 = lv_display_get_original_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_original_vertical_resolution(disp_refr) - 1;
        }
        अन्यथा {
            layer->buf_area.x2 = lv_display_get_horizontal_resolution(disp_refr) - 1;
            layer->buf_area.y2 = lv_display_get_vertical_resolution(disp_refr) - 1;
        }
        layer_reshape_draw_buf(layer, disp_refr->stride_is_auto ? LV_STRIDE_AUTO : layer->draw_buf->header.stride);
    }

    /*Try to divide the area to smaller tiles*/
    uint32_t tile_cnt = 1;
    int32_t tile_h = lv_area_get_height(area_p);
    यदि(LV_COLOR_FORMAT_IS_INDEXED(layer->color_format) == false) {
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

    यदि(tile_cnt == 1) {
        refr_configured_layer(layer);
        layer->all_tasks_added = true;
    }
    अन्यथा {
        /* Don't draw to the layers buffer of the display but create smaller dummy layers which are using the
         * display's layer buffer. These will be the tiles. By using tiles it's more likely that there will
         * be independent areas for each draw unit. */
        lv_layer_t * tile_layers = lv_malloc(tile_cnt * परिमाण(lv_layer_t));
        LV_ASSERT_MALLOC(tile_layers);
        यदि(tile_layers == रिक्त) {
            disp_refr->refreshed_area = *area_p;
            LV_PROFILER_REFR_END;
            फर्काऊ;
        }
        uint32_t i;
        लागि(i = 0; i < tile_cnt; i++) {
            lv_area_t tile_area;
            lv_area_set(&tile_area, area_p->x1, area_p->y1 + i * tile_h,
                        area_p->x2, area_p->y1 + (i + 1) * tile_h - 1);

            यदि(i == tile_cnt - 1) {
                tile_area.y2 = area_p->y2;
            }

            lv_layer_t * tile_layer = &tile_layers[i];
            lv_draw_layer_init(tile_layer, रिक्त, layer->color_format, &tile_area);
            tile_layer->buf_area = layer->buf_area; /*the buffer is still large*/
            tile_layer->draw_buf = layer->draw_buf;
            refr_configured_layer(tile_layer);
            tile_layer->all_tasks_added = true;
        }


        /*Wait until all tiles are ready and destroy remove them*/
        लागि(i = 0; i < tile_cnt; i++) {
            lv_layer_t * tile_layer = &tile_layers[i];
            जबसम्म(tile_layer->draw_task_head) {
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

स्थिर शून्य refr_configured_layer(lv_layer_t * layer)
{
    LV_PROFILER_REFR_BEGIN;

    lv_layer_reset(layer);

#यदि LV_DRAW_TRANSFORM_USE_MATRIX
    यदि(lv_display_get_matrix_rotation(disp_refr)) {
        अचर lv_rotation_t rotation = lv_display_get_rotation(disp_refr);
        यदि(rotation != LV_ROTATION_0) {
            lv_display_rotate_area(disp_refr, &layer->phy_clip_area);

            /**
             * The screen rotation direction defined by LVGL is opposite to the drawing angle.
             * Use direct matrix assignment to reduce precision loss and improve efficiency.
             */
            विकल्प(rotation) {
                पक्ष LV_ROTATION_90:
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
                    रोक;

                पक्ष LV_ROTATION_180:
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
                    रोक;

                पक्ष LV_ROTATION_270:
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
                    रोक;

                स्वभाव:
                    LV_LOG_WARN("Invalid rotation: %d", rotation);
                    रोक;
            }
        }
    }
#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */

    /* In single buffered mode wait here until the buffer is freed.
     * Else we would draw into the buffer while it's still being transferred to the display*/
    यदि(!lv_display_is_double_buffered(disp_refr)) {
        wait_for_flushing(disp_refr);
    }
    /*If the screen is transparent initialize it when the flushing is ready*/
    यदि(lv_color_format_has_alpha(disp_refr->color_format)) {
        lv_area_t clear_area = layer->_clip_area;
        lv_area_move(&clear_area, -layer->buf_area.x1, -layer->buf_area.y1);
        lv_draw_buf_clear_ex(layer->draw_buf, &clear_area, layer);
    }

    lv_obj_t * शीर्ष_act_scr = रिक्त;
    lv_obj_t * शीर्ष_prev_scr = रिक्त;

    /*Get the most top object which is not covered by others*/
    शीर्ष_act_scr = lv_refr_get_शीर्ष_obj(&layer->_clip_area, lv_display_get_screen_active(disp_refr));
    यदि(disp_refr->prev_scr) {
        शीर्ष_prev_scr = lv_refr_get_शीर्ष_obj(&layer->_clip_area, disp_refr->prev_scr);
    }

    /*Draw a bottom layer background if there is no top object*/
    यदि(शीर्ष_act_scr == रिक्त && शीर्ष_prev_scr == रिक्त) {
        refr_obj_and_children(layer, lv_display_get_layer_bottom(disp_refr));
    }

    यदि(disp_refr->draw_prev_over_act) {
        यदि(शीर्ष_act_scr == रिक्त) शीर्ष_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, शीर्ष_act_scr);

        /*Refresh the previous screen if any*/
        यदि(disp_refr->prev_scr) {
            यदि(शीर्ष_prev_scr == रिक्त) शीर्ष_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, शीर्ष_prev_scr);
        }
    }
    अन्यथा {
        /*Refresh the previous screen if any*/
        यदि(disp_refr->prev_scr) {
            यदि(शीर्ष_prev_scr == रिक्त) शीर्ष_prev_scr = disp_refr->prev_scr;
            refr_obj_and_children(layer, शीर्ष_prev_scr);
        }

        यदि(शीर्ष_act_scr == रिक्त) शीर्ष_act_scr = disp_refr->act_scr;
        refr_obj_and_children(layer, शीर्ष_act_scr);
    }

    /*Also refresh top and sys layer unconditionally*/
    refr_obj_and_children(layer, lv_display_get_layer_शीर्ष(disp_refr));
    refr_obj_and_children(layer, lv_display_get_layer_sys(disp_refr));

    LV_PROFILER_REFR_END;
}

/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param layer pointer to the layer to draw into
 * @param top_obj pointer to an object. Start the drawing from it.
 */
स्थिर शून्य refr_obj_and_children(lv_layer_t * layer, lv_obj_t * शीर्ष_obj)
{
    /*Normally always will be a top_obj (at least the screen)
     *but in special cases (e.g. if the screen has alpha) it won't.
     *In this case use the screen directly*/
    यदि(शीर्ष_obj == रिक्त) शीर्ष_obj = lv_display_get_screen_active(disp_refr);
    यदि(शीर्ष_obj == रिक्त) फर्काऊ;  /*Shouldn't happen*/

    LV_PROFILER_REFR_BEGIN;
    /*Draw the 'younger' sibling objects because they can be on top_obj*/
    lv_obj_t * parent;
    lv_obj_t * border_p = शीर्ष_obj;

    parent = lv_obj_get_parent(शीर्ष_obj);

    अचर lv_color32_t layer_recolor = layer->recolor;

    /*Calculate the recolor before the parent*/
    यदि(parent) {
        layer->recolor = lv_obj_get_style_recolor_recursive(parent, LV_PART_MAIN);
    }

    /*Refresh the top object and its children*/
    lv_obj_refr(layer, शीर्ष_obj);

    /*Do until not reach the screen*/
    जबसम्म(parent != रिक्त) {
        bool go = false;
        uint32_t i;
        uint32_t child_cnt = lv_obj_get_child_गन्ती(parent);
        लागि(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = parent->spec_attr->children[i];
            यदि(!go) {
                यदि(child == border_p) go = true;
            }
            अन्यथा {
                /*Refresh the objects*/
                lv_obj_refr(layer, child);
            }
        }

        /*Call the post draw function of the parents of the to object*/
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST_BEGIN, (शून्य *)layer);
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST, (शून्य *)layer);
        lv_obj_send_event(parent, LV_EVENT_DRAW_POST_END, (शून्य *)layer);

        /*The new border will be the last parents,
         *so the 'younger' brothers of parent will be refreshed*/
        border_p = parent;
        /*Go a level deeper*/
        parent = lv_obj_get_parent(parent);
    }

    layer->recolor = layer_recolor;
    LV_PROFILER_REFR_END;
}

स्थिर lv_result_t layer_get_area(lv_layer_t * layer, lv_obj_t * obj, lv_layer_type_t layer_type,
                                  lv_area_t * layer_area_out, lv_area_t * obj_draw_size_out)
{
    int32_t ext_draw_size = lv_obj_get_ext_draw_size(obj);
    lv_obj_get_coords(obj, obj_draw_size_out);
    lv_area_increase(obj_draw_size_out, ext_draw_size, ext_draw_size);

    यदि(layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Get the transformed area and clip it to the current clip area.
         *This area needs to be updated on the screen.*/
        lv_area_t clip_coords_for_obj;
        lv_area_t tranf_coords = *obj_draw_size_out;
        lv_obj_get_transformed_area(obj, &tranf_coords, LV_OBJ_POINT_TRANSFORM_FLAG_NONE);
        यदि(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &tranf_coords)) {
            फर्काऊ LV_RESULT_INVALID;
        }

        /*Transform back (inverse) the transformed area.
         *It will tell which area of the non-transformed widget needs to be redrawn
         *in order to cover transformed area after transformation.*/
        lv_area_t inverse_clip_coords_for_obj = clip_coords_for_obj;
        lv_obj_get_transformed_area(obj, &inverse_clip_coords_for_obj, LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE);
        यदि(!lv_area_intersect(&inverse_clip_coords_for_obj, &inverse_clip_coords_for_obj, obj_draw_size_out)) {
            फर्काऊ LV_RESULT_INVALID;
        }

        *layer_area_out = inverse_clip_coords_for_obj;
        lv_area_increase(layer_area_out, 5, 5); /*To avoid rounding error*/
    }
    अन्यथा यदि(layer_type == LV_LAYER_TYPE_SIMPLE) {
        lv_area_t clip_coords_for_obj;
        यदि(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, obj_draw_size_out)) {
            फर्काऊ LV_RESULT_INVALID;
        }
        *layer_area_out = clip_coords_for_obj;
    }
    अन्यथा {
        LV_LOG_WARN("Unhandled layer type");
        फर्काऊ LV_RESULT_INVALID;
    }

    फर्काऊ LV_RESULT_OK;
}

स्थिर bool alpha_test_area_on_obj(lv_obj_t * obj, अचर lv_area_t * area)
{
    /*Test for alpha by assuming there is no alpha. If it fails, fall back to rendering with alpha*/
    /*If the layer area is not fully on the object, it can't fully cover it*/
    यदि(!lv_area_is_on(area, &obj->coords)) फर्काऊ true;

    lv_cover_check_info_t info;
    info.res = LV_COVER_RES_COVER;
    info.area = area;
    lv_obj_send_event(obj, LV_EVENT_COVER_CHECK, &info);
    यदि(info.res == LV_COVER_RES_COVER) फर्काऊ false;
    अन्यथा फर्काऊ true;
}

#यदि LV_DRAW_TRANSFORM_USE_MATRIX

स्थिर bool obj_get_matrix(lv_obj_t * obj, lv_matrix_t * matrix)
{
    lv_matrix_identity(matrix);

    अचर lv_matrix_t * obj_matrix = lv_obj_get_transform(obj);
    यदि(obj_matrix) {
        lv_matrix_translate(matrix, obj->coords.x1, obj->coords.y1);
        lv_matrix_multiply(matrix, obj_matrix);
        lv_matrix_translate(matrix, -obj->coords.x1, -obj->coords.y1);
        फर्काऊ true;
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
    यदि(rotation != 0) {
        lv_matrix_rotate(matrix, rotation * 0.1f);
    }

    यदि(scale_x != LV_SCALE_NONE || scale_y != LV_SCALE_NONE) {
        lv_matrix_scale(
            matrix,
            (भिन्न)scale_x / LV_SCALE_NONE,
            (भिन्न)scale_y / LV_SCALE_NONE
        );
    }

    यदि(skew_x != 0 || skew_y != 0) {
        lv_matrix_skew(matrix, skew_x, skew_y);
    }

    lv_matrix_translate(matrix, -pivot.x, -pivot.y);
    फर्काऊ true;
}

स्थिर शून्य refr_obj_matrix(lv_layer_t * layer, lv_obj_t * obj)
{
    LV_PROFILER_REFR_BEGIN;
    lv_matrix_t obj_matrix;
    यदि(!obj_get_matrix(obj, &obj_matrix)) {
        /* NOT draw if obj matrix is not available */
        LV_PROFILER_REFR_END;
        फर्काऊ;
    }

    lv_matrix_t matrix_inv;
    यदि(!lv_matrix_inverse(&matrix_inv, &obj_matrix)) {
        /* NOT draw if matrix is not invertible */
        LV_PROFILER_REFR_END;
        फर्काऊ;
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
    यदि(!lv_matrix_is_identity_or_translation(&obj_matrix)) {
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

स्थिर bool refr_check_obj_clip_overflow(lv_layer_t * layer, lv_obj_t * obj)
{
    यदि(lv_obj_get_style_transform_rotation_internal(obj, LV_PART_MAIN) == 0) {
        फर्काऊ false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_size = lv_obj_get_ext_draw_size(obj);
    obj_coords = obj->coords;
    lv_area_increase(&obj_coords, ext_size, ext_size);

    lv_obj_get_transformed_area(obj, &obj_coords, LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);

    lv_area_t clip_coords_for_obj;
    यदि(!lv_area_intersect(&clip_coords_for_obj, &layer->_clip_area, &obj_coords)) {
        फर्काऊ true;
    }

    bool has_clip = lv_memcmp(&clip_coords_for_obj, &obj_coords, परिमाण(lv_area_t)) != 0;
    फर्काऊ has_clip;
}

#endif /* LV_DRAW_TRANSFORM_USE_MATRIX */

स्थिर uint32_t get_max_row(lv_display_t * disp, int32_t area_w, int32_t area_h)
{
    lv_color_format_t cf = disp->color_format;
    uint32_t stride = lv_draw_buf_width_to_stride(area_w, cf);
    uint32_t overhead = LV_COLOR_INDEXED_PALETTE_SIZE(cf) * परिमाण(lv_color32_t);

    यदि(stride == 0) {
        LV_LOG_WARN("Invalid stride. Value is 0");
        फर्काऊ 0;
    }

    int32_t max_row = (uint32_t)(disp->buf_act->data_size - overhead) / stride;

    यदि(max_row > area_h) max_row = area_h;

    /*Round down the lines of draw_buf if rounding is added*/
    lv_area_t tmp;
    tmp.x1 = 0;
    tmp.x2 = 0;
    tmp.y1 = 0;

    int32_t h_tmp = max_row;
    गर {
        tmp.y2 = h_tmp - 1;
        lv_display_send_event(disp_refr, LV_EVENT_INVALIDATE_AREA, &tmp);

        /*If this height fits into `max_row` then fine*/
        यदि(lv_area_get_height(&tmp) <= max_row) रोक;

        /*Decrement the height of the area until it fits into `max_row` after rounding*/
        h_tmp--;
    } जबसम्म(h_tmp > 0);

    यदि(h_tmp <= 0) {
        LV_LOG_WARN("Can't set draw_buf height using the round function. (Wrong round_cb or too "
                    "small draw_buf)");
        फर्काऊ 0;
    }
    अन्यथा {
        max_row = tmp.y2 + 1;
    }

    फर्काऊ max_row;
}

/**
 * Flush the content of the draw buffer
 */
स्थिर शून्य draw_buf_flush(lv_display_t * disp)
{
    /*Flush the rendered content to the display*/
    lv_layer_t * layer = disp->layer_head;

    जबसम्म(layer->draw_task_head) {
        lv_draw_dispatch_wait_for_request();
        lv_draw_dispatch();
    }

    /* Every layer must've been drawn*/
    LV_ASSERT_सन्देश(layer->next == रिक्त, "Expected every layer to have been drawn at this point");

    /* In double buffered mode wait until the other buffer is freed
     * and driver is ready to receive the new buffer.
     * If we need to wait here it means that the content of one buffer is being sent to display
     * and other buffer already contains the new rendered image. */
    यदि(lv_display_is_double_buffered(disp)) {
        wait_for_flushing(disp_refr);
    }

    disp->flushing = 1;

    यदि(disp->last_area && disp->last_part) disp->flushing_last = 1;
    अन्यथा disp->flushing_last = 0;

    bool flushing_last = disp->flushing_last;

    यदि(disp->flush_cb) {
        call_flush_cb(disp, &disp->refreshed_area, layer->draw_buf->data);
    }
    /*If there are 2 buffers swap them. With direct mode swap only on the last area*/
    यदि(lv_display_is_double_buffered(disp) && (disp->render_mode != LV_DISPLAY_RENDER_MODE_DIRECT || flushing_last)) {
        यदि(disp->buf_act == disp->buf_1) {
            disp->buf_act = disp->buf_2;
        }
        अन्यथा यदि(disp->buf_act == disp->buf_2) {
            disp->buf_act = disp->buf_3 ? disp->buf_3 : disp->buf_1;
        }
        अन्यथा {
            disp->buf_act = disp->buf_1;
        }
    }
}

स्थिर शून्य call_flush_cb(lv_display_t * disp, अचर lv_area_t * area, uint8_t * px_map)
{
    LV_PROFILER_REFR_BEGIN;
    LV_TRACE_REFR("Calling flush_cb on (%d;%d)(%d;%d) area with %p image pointer",
                  (पूर्णाङ्क)area->x1, (पूर्णाङ्क)area->y1, (पूर्णाङ्क)area->x2, (पूर्णाङ्क)area->y2, (शून्य *)px_map);

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

स्थिर शून्य wait_for_flushing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_START, रिक्त);

    यदि(disp->flush_wait_cb) {
        यदि(disp->flushing) {
            disp->flush_wait_cb(disp);
            disp->flushing = 0;
        }
    }
    अन्यथा {
        जबसम्म(disp->flushing);
    }
    disp->flushing_last = 0;

    lv_display_send_event(disp, LV_EVENT_FLUSH_WAIT_FINISH, रिक्त);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_END;
}

स्थिर शून्य call_sync_cb(lv_display_t * disp, अचर lv_area_t * area)
{
    LV_PROFILER_REFR_BEGIN;

    /* Apply display offsets to the sync area for consistency with flush_cb */
    lv_area_t offset_area = *area;
    offset_area.x1 += disp->offset_x;
    offset_area.x2 += disp->offset_x;
    offset_area.y1 += disp->offset_y;
    offset_area.y2 += disp->offset_y;

    LV_TRACE_REFR("Calling sync_cb on (%d;%d)(%d;%d) area",
                  (पूर्णाङ्क)offset_area.x1, (पूर्णाङ्क)offset_area.y1,
                  (पूर्णाङ्क)offset_area.x2, (पूर्णाङ्क)offset_area.y2);

    lv_display_send_event(disp, LV_EVENT_SYNC_START, (शून्य *)&offset_area);

    disp->sync_cb(disp, &offset_area);

    lv_display_send_event(disp, LV_EVENT_SYNC_FINISH, (शून्य *)&offset_area);

    LV_PROFILER_REFR_END;
}

स्थिर शून्य wait_for_syncing(lv_display_t * disp)
{
    LV_PROFILER_REFR_BEGIN;
    LV_LOG_TRACE("begin");

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_START, रिक्त);

    यदि(disp->sync_wait_cb) {
        यदि(disp->syncing) {
            disp->sync_wait_cb(disp);
            disp->syncing = 0;
        }
    }
    अन्यथा {
        जबसम्म(disp->syncing);
    }
    disp->syncing_last = 0;

    lv_display_send_event(disp, LV_EVENT_SYNC_WAIT_FINISH, रिक्त);

    LV_LOG_TRACE("end");
    LV_PROFILER_REFR_END;
}
