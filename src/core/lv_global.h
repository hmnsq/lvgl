/**
 * @file lv_global.h
 *
 */

#اگر_تعریف_نشده LV_GLOBAL_H
#تعریف_کن LV_GLOBAL_H

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

#إذا LV_USE_DRAW_SW
#ضمن "../draw/sw/lv_draw_sw.h"
#پایان_اگر
#ضمن "../osal/lv_os_private.h"
#ضمن "../stdlib/builtin/lv_tlsf.h"

#إذا LV_USE_FONT_COMPRESSED
#ضمن "../font/fmt_txt/lv_font_fmt_txt_private.h"
#پایان_اگر

#ضمن "../misc/lv_timer_private.h"
#ضمن "../misc/lv_anim_private.h"
#ضمن "../tick/lv_tick_private.h"
#ضمن "../draw/lv_draw_buf_private.h"
#ضمن "../draw/lv_draw_private.h"
#ضمن "../draw/sw/lv_draw_sw_private.h"
#ضمن "../draw/sw/lv_draw_sw_mask_private.h"
#ضمن "../stdlib/builtin/lv_tlsf_private.h"
#ضمن "../debugging/sysmon/lv_sysmon_private.h"
#ضمن "../debugging/test/lv_test_private.h"
#ضمن "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کن ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#إذا LV_USE_بازه != 0
بنية snippet_پشته;
#پایان_اگر

#إذا LV_USE_FREETYPE
بنية _lv_freetype_context_t;
#پایان_اگر

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
بنية _lv_profiler_builtin_ctx_t;
#پایان_اگر

#إذا LV_USE_NUTTX
بنية _lv_nuttx_ctx_t;
#پایان_اگر

تعريفنوع بنية _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    فراغ * user_داده;

    bool inited;
    bool deinit_درون_پیشرفت;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_جدول_اندازه;
    uint32_t style_آخرین_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_جدول;

    lv_ll_t گروه_ll;
    lv_گروه_t * گروه_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_فعال;
    lv_obj_t * indev_obj_فعال;

    uint32_t layout_شمار;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t حافظه_zero;
    uint32_t math_rand_دانه;

    lv_event_t * event_سرآیند;
    uint32_t event_آخرین_ثبات_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_قفل;
    lv_mutex_t img_decoder_open_قفل;
#پایان_اگر

    lv_cache_t * img_cache;
    lv_cache_t * img_سرآیند_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#إذا LV_USE_OBSERVER
    lv_ll_t subject_ll;
#پایان_اگر

#إذا defined(LV_DRAW_SW_SHADOW_CACHE_اندازه) && LV_DRAW_SW_SHADOW_CACHE_اندازه > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#پایان_اگر
#إذا LV_DRAW_SW_COMPLEX
    lv_draw_sw_ماسک_radius_circle_dsc_arr_t sw_circle_cache;
#پایان_اگر

#إذا LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#پایان_اگر

#إذا LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_آخرین_log_زمان;
#پایان_اگر

#إذا LV_USE_THEME_SIMPLE
    فراغ * theme_simple;
#پایان_اگر

#إذا LV_USE_THEME_DEFAULT
    فراغ * theme_default;
#پایان_اگر

#إذا LV_USE_THEME_MONO
    فراغ * theme_mono;
#پایان_اگر

#إذا LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#پایان_اگر

    lv_ll_t fsdrv_ll;
#إذا LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#پایان_اگر
#إذا LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#پایان_اگر

#إذا LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#پایان_اگر

#إذا LV_USE_FREETYPE
    بنية _lv_freetype_context_t * ft_context;
#پایان_اگر

#إذا LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#پایان_اگر

#إذا LV_USE_بازه != 0
    بنية snippet_پشته * بازه_snippet_پشته;
#پایان_اگر

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    بنية _lv_profiler_builtin_ctx_t * profiler_context;
#پایان_اگر


#إذا LV_USE_MEM_MONITOR
    lv_sysmon_backend_داده_t sysmon_mem;
#پایان_اگر

#إذا LV_USE_IME_PINYIN != 0
    اندازه_t ime_cand_len;
#پایان_اگر

#إذا LV_USE_OBJ_ID_BUILTIN
    فراغ * objid_array;
    uint32_t objid_شمار;
#پایان_اگر

#إذا LV_USE_TEST
    lv_test_state_t test_state;
#پایان_اگر

#إذا LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ثابت حرف * translation_selected_lang;
#پایان_اگر

#إذا LV_USE_NUTTX
    بنية _lv_nuttx_ctx_t * nuttx_ctx;
#پایان_اگر

#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#پایان_اگر

#إذا defined(__linux__)
    lv_linux_proc_stat_t linux_آخرین_proc_stat;
#إذا LV_SYSMON_PROC_IDLE_موجود
    uint64_t linux_آخرین_self_proc_زمان_ticks;
    lv_linux_proc_stat_t linux_آخرین_system_مجموع_ticks_stat;
#پایان_اگر
#پایان_اگر

#إذا LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_زمان_جمع;
    uint32_t freertos_non_idle_زمان_جمع;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#پایان_اگر

#إذا LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#پایان_اگر

#إذا LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#پایان_اگر
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#إذا LV_ENABLE_GLOBAL_CUSTOM
#إذا LV_GLOBAL_USE_CUSTOM_INCLUDE
#ضمن LV_GLOBAL_CUSTOM_INCLUDE
#پایان_اگر /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#اگر_تعریف_نشده LV_GLOBAL_CUSTOM
#تعریف_کن LV_GLOBAL_CUSTOM() lv_global_default()
#پایان_اگر
#تعریف_کن LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#وإلا
LV_ATTRIBUTE_EXTERN_داده خارجي lv_global_t lv_global;
#تعریف_کن LV_GLOBAL_DEFAULT() (&lv_global)
#پایان_اگر /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#إذا LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(فراغ);
#پایان_اگر
#اگر_تعریف_شده __cplusplus
} /*extern "C"*/
#پایان_اگر

#پایان_اگر /*LV_GLOBAL_H*/
