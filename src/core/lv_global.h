/**
 * @file lv_global.h
 *
 */

#నిర్వచితం_కాకపోతే LV_GLOBAL_H
#నిర్వచించు LV_GLOBAL_H

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "../lvgl_public.h"

#ఒకవేళ LV_USE_DRAW_SW
#అన్తర్భావ "../draw/sw/lv_draw_sw.h"
#అయితే_ముగింపు
#అన్తర్భావ "../osal/lv_os_private.h"
#అన్తర్భావ "../stdlib/builtin/lv_tlsf.h"

#ఒకవేళ LV_USE_FONT_COMPRESSED
#అన్తర్భావ "../font/fmt_txt/lv_font_fmt_txt_private.h"
#అయితే_ముగింపు

#అన్తర్భావ "../misc/lv_timer_private.h"
#అన్తర్భావ "../misc/lv_anim_private.h"
#అన్తర్భావ "../tick/lv_tick_private.h"
#అన్తర్భావ "../draw/lv_draw_buf_private.h"
#అన్తర్భావ "../draw/lv_draw_private.h"
#అన్తర్భావ "../draw/sw/lv_draw_sw_private.h"
#అన్తర్భావ "../draw/sw/lv_draw_sw_mask_private.h"
#అన్తర్భావ "../stdlib/builtin/lv_tlsf_private.h"
#అన్తర్భావ "../debugging/sysmon/lv_sysmon_private.h"
#అన్తర్భావ "../debugging/test/lv_test_private.h"
#అన్తర్భావ "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#ఒకవేళ LV_USE_పరిధి != 0
నిర్మాణం snippet_దొంతర;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FREETYPE
నిర్మాణం _lv_freetype_context_t;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
నిర్మాణం _lv_profiler_builtin_ctx_t;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_NUTTX
నిర్మాణం _lv_nuttx_ctx_t;
#అయితే_ముగింపు

రకనిర్వచనం నిర్మాణం _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    శూన్య * user_దత్తాంశం;

    bool inited;
    bool deinit_లోపల_పురోగతి;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_పట్టిక_కొలత;
    uint32_t style_చివరి_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_పట్టిక;

    lv_ll_t సమూహం_ll;
    lv_సమూహం_t * సమూహం_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_చురుకైన;
    lv_obj_t * indev_obj_చురుకైన;

    uint32_t layout_లెక్క;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t జ్ఞాపకం_zero;
    uint32_t math_rand_విత్తనం;

    lv_event_t * event_శీర్షిక;
    uint32_t event_చివరి_రిజిస్టరు_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#ఒకవేళ LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_తాళం;
    lv_mutex_t img_decoder_open_తాళం;
#అయితే_ముగింపు

    lv_cache_t * img_cache;
    lv_cache_t * img_శీర్షిక_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#ఒకవేళ LV_USE_OBSERVER
    lv_ll_t subject_ll;
#అయితే_ముగింపు

#ఒకవేళ defined(LV_DRAW_SW_SHADOW_CACHE_కొలత) && LV_DRAW_SW_SHADOW_CACHE_కొలత > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#అయితే_ముగింపు
#ఒకవేళ LV_DRAW_SW_COMPLEX
    lv_draw_sw_ముసుగు_radius_circle_dsc_arr_t sw_circle_cache;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_చివరి_log_సమయం;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_THEME_SIMPLE
    శూన్య * theme_simple;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_THEME_DEFAULT
    శూన్య * theme_default;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_THEME_MONO
    శూన్య * theme_mono;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#అయితే_ముగింపు

    lv_ll_t fsdrv_ll;
#ఒకవేళ LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#అయితే_ముగింపు
#ఒకవేళ LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FREETYPE
    నిర్మాణం _lv_freetype_context_t * ft_context;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_పరిధి != 0
    నిర్మాణం snippet_దొంతర * పరిధి_snippet_దొంతర;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    నిర్మాణం _lv_profiler_builtin_ctx_t * profiler_context;
#అయితే_ముగింపు


#ఒకవేళ LV_USE_MEM_MONITOR
    lv_sysmon_backend_దత్తాంశం_t sysmon_mem;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_IME_PINYIN != 0
    కొలత_t ime_cand_len;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_OBJ_ID_BUILTIN
    శూన్య * objid_array;
    uint32_t objid_లెక్క;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_TEST
    lv_test_state_t test_state;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    స్థిరరాశి అక్షరం * translation_selected_lang;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_NUTTX
    నిర్మాణం _lv_nuttx_ctx_t * nuttx_ctx;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__)
    lv_linux_proc_stat_t linux_చివరి_proc_stat;
#ఒకవేళ LV_SYSMON_PROC_IDLE_అందుబాటు
    uint64_t linux_చివరి_self_proc_సమయం_ticks;
    lv_linux_proc_stat_t linux_చివరి_system_మొత్తం_ticks_stat;
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_సమయం_కూడిక;
    uint32_t freertos_non_idle_సమయం_కూడిక;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#అయితే_ముగింపు

#ఒకవేళ LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#అయితే_ముగింపు
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#ఒకవేళ LV_ENABLE_GLOBAL_CUSTOM
#ఒకవేళ LV_GLOBAL_USE_CUSTOM_INCLUDE
#అన్తర్భావ LV_GLOBAL_CUSTOM_INCLUDE
#అయితే_ముగింపు /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#నిర్వచితం_కాకపోతే LV_GLOBAL_CUSTOM
#నిర్వచించు LV_GLOBAL_CUSTOM() lv_global_default()
#అయితే_ముగింపు
#నిర్వచించు LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#లేకపోతే
LV_ATTRIBUTE_EXTERN_దత్తాంశం బాహ్య lv_global_t lv_global;
#నిర్వచించు LV_GLOBAL_DEFAULT() (&lv_global)
#అయితే_ముగింపు /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#ఒకవేళ LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(శూన్య);
#అయితే_ముగింపు
#నిర్వచితమైతే __cplusplus
} /*extern "C"*/
#అయితే_ముగింపు

#అయితే_ముగింపు /*LV_GLOBAL_H*/
