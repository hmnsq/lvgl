/**
 * @file lv_global.h
 *
 */

#यदि_अपरिभाषितम् LV_GLOBAL_H
#परिभाषय LV_GLOBAL_H

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "../lvgl_public.h"

#यदि LV_USE_DRAW_SW
#अन्तर्भाव "../draw/sw/lv_draw_sw.h"
#यदि_अन्तः
#अन्तर्भाव "../osal/lv_os_private.h"
#अन्तर्भाव "../stdlib/builtin/lv_tlsf.h"

#यदि LV_USE_FONT_COMPRESSED
#अन्तर्भाव "../font/fmt_txt/lv_font_fmt_txt_private.h"
#यदि_अन्तः

#अन्तर्भाव "../misc/lv_timer_private.h"
#अन्तर्भाव "../misc/lv_anim_private.h"
#अन्तर्भाव "../tick/lv_tick_private.h"
#अन्तर्भाव "../draw/lv_draw_buf_private.h"
#अन्तर्भाव "../draw/lv_draw_private.h"
#अन्तर्भाव "../draw/sw/lv_draw_sw_private.h"
#अन्तर्भाव "../draw/sw/lv_draw_sw_mask_private.h"
#अन्तर्भाव "../stdlib/builtin/lv_tlsf_private.h"
#अन्तर्भाव "../debugging/sysmon/lv_sysmon_private.h"
#अन्तर्भाव "../debugging/test/lv_test_private.h"
#अन्तर्भाव "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#परिभाषय ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#यदि LV_USE_विस्तृतिः != 0
संरचना snippet_राशिः;
#यदि_अन्तः

#यदि LV_USE_FREETYPE
संरचना _lv_freetype_context_t;
#यदि_अन्तः

#यदि LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
संरचना _lv_profiler_builtin_ctx_t;
#यदि_अन्तः

#यदि LV_USE_NUTTX
संरचना _lv_nuttx_ctx_t;
#यदि_अन्तः

प्रकारनिर्देश संरचना _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    शून्य * user_दत्त;

    bool inited;
    bool deinit_अन्तर्_प्रगतिः;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_सारणी_आकार;
    uint32_t style_चरमः_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_सारणी;

    lv_ll_t गणः_ll;
    lv_गणः_t * गणः_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_सक्रिय;
    lv_obj_t * indev_obj_सक्रिय;

    uint32_t layout_गणनम्;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t स्मृतिः_zero;
    uint32_t math_rand_बीजम्;

    lv_event_t * event_शीर्षकम्;
    uint32_t event_चरमः_पञ्जिकाकोशः_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#यदि LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_तालकम्;
    lv_mutex_t img_decoder_open_तालकम्;
#यदि_अन्तः

    lv_cache_t * img_cache;
    lv_cache_t * img_शीर्षकम्_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#यदि LV_USE_OBSERVER
    lv_ll_t subject_ll;
#यदि_अन्तः

#यदि defined(LV_DRAW_SW_SHADOW_CACHE_आकार) && LV_DRAW_SW_SHADOW_CACHE_आकार > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#यदि_अन्तः
#यदि LV_DRAW_SW_COMPLEX
    lv_draw_sw_आवरणम्_radius_circle_dsc_arr_t sw_circle_cache;
#यदि_अन्तः

#यदि LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#यदि_अन्तः

#यदि LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_चरमः_log_time;
#यदि_अन्तः

#यदि LV_USE_THEME_SIMPLE
    शून्य * theme_simple;
#यदि_अन्तः

#यदि LV_USE_THEME_DEFAULT
    शून्य * theme_default;
#यदि_अन्तः

#यदि LV_USE_THEME_MONO
    शून्य * theme_mono;
#यदि_अन्तः

#यदि LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#यदि_अन्तः

    lv_ll_t fsdrv_ll;
#यदि LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#यदि_अन्तः
#यदि LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#यदि_अन्तः

#यदि LV_USE_FREETYPE
    संरचना _lv_freetype_context_t * ft_context;
#यदि_अन्तः

#यदि LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#यदि_अन्तः

#यदि LV_USE_विस्तृतिः != 0
    संरचना snippet_राशिः * विस्तृतिः_snippet_राशिः;
#यदि_अन्तः

#यदि LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    संरचना _lv_profiler_builtin_ctx_t * profiler_context;
#यदि_अन्तः


#यदि LV_USE_MEM_MONITOR
    lv_sysmon_backend_दत्त_t sysmon_mem;
#यदि_अन्तः

#यदि LV_USE_IME_PINYIN != 0
    आकार_t ime_cand_len;
#यदि_अन्तः

#यदि LV_USE_OBJ_ID_BUILTIN
    शून्य * objid_array;
    uint32_t objid_गणनम्;
#यदि_अन्तः

#यदि LV_USE_TEST
    lv_test_state_t test_state;
#यदि_अन्तः

#यदि LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ध्रुव वर्ण * translation_selected_lang;
#यदि_अन्तः

#यदि LV_USE_NUTTX
    संरचना _lv_nuttx_ctx_t * nuttx_ctx;
#यदि_अन्तः

#यदि LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#यदि_अन्तः

#यदि defined(__linux__)
    lv_linux_proc_stat_t linux_चरमः_proc_stat;
#यदि LV_SYSMON_PROC_IDLE_उपलब्धम्
    uint64_t linux_चरमः_self_proc_time_ticks;
    lv_linux_proc_stat_t linux_चरमः_system_योग_ticks_stat;
#यदि_अन्तः
#यदि_अन्तः

#यदि LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_time_योग;
    uint32_t freertos_non_idle_time_योग;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#यदि_अन्तः

#यदि LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#यदि_अन्तः

#यदि LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#यदि_अन्तः
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#यदि LV_ENABLE_GLOBAL_CUSTOM
#यदि LV_GLOBAL_USE_CUSTOM_INCLUDE
#अन्तर्भाव LV_GLOBAL_CUSTOM_INCLUDE
#यदि_अन्तः /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#यदि_अपरिभाषितम् LV_GLOBAL_CUSTOM
#परिभाषय LV_GLOBAL_CUSTOM() lv_global_default()
#यदि_अन्तः
#परिभाषय LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#अन्यथा
LV_ATTRIBUTE_EXTERN_दत्त बाह्य lv_global_t lv_global;
#परिभाषय LV_GLOBAL_DEFAULT() (&lv_global)
#यदि_अन्तः /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#यदि LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(शून्य);
#यदि_अन्तः
#यदि_परिभाषितम् __cplusplus
} /*extern "C"*/
#यदि_अन्तः

#यदि_अन्तः /*LV_GLOBAL_H*/
