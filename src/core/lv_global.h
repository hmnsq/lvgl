/**
 * @file lv_global.h
 *
 */

#اگر_غیر_تعریف_شدہ LV_GLOBAL_H
#تعریف_کرو LV_GLOBAL_H

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/*********************
 *      INCLUDES
 *********************/
#شامل "../lvgl_public.h"

#اگر LV_USE_DRAW_SW
#شامل "../draw/sw/lv_draw_sw.h"
#اگر_ختم
#شامل "../osal/lv_os_private.h"
#شامل "../stdlib/builtin/lv_tlsf.h"

#اگر LV_USE_FONT_COMPRESSED
#شامل "../font/fmt_txt/lv_font_fmt_txt_private.h"
#اگر_ختم

#شامل "../misc/lv_timer_private.h"
#شامل "../misc/lv_anim_private.h"
#شامل "../tick/lv_tick_private.h"
#شامل "../draw/lv_draw_buf_private.h"
#شامل "../draw/lv_draw_private.h"
#شامل "../draw/sw/lv_draw_sw_private.h"
#شامل "../draw/sw/lv_draw_sw_mask_private.h"
#شامل "../stdlib/builtin/lv_tlsf_private.h"
#شامل "../debugging/sysmon/lv_sysmon_private.h"
#شامل "../debugging/test/lv_test_private.h"
#شامل "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#اگر LV_USE_وسعت != 0
ساخت snippet_ڈھیر;
#اگر_ختم

#اگر LV_USE_FREETYPE
ساخت _lv_freetype_context_t;
#اگر_ختم

#اگر LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
ساخت _lv_profiler_builtin_ctx_t;
#اگر_ختم

#اگر LV_USE_NUTTX
ساخت _lv_nuttx_ctx_t;
#اگر_ختم

قسمتعریف ساخت _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    خالی * user_ڈیٹا;

    bool inited;
    bool deinit_اندر_پیش_رفت;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_جدول_سائز;
    uint32_t style_آخری_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_جدول;

    lv_ll_t گروہ_ll;
    lv_گروہ_t * گروہ_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_فعال;
    lv_obj_t * indev_obj_فعال;

    uint32_t layout_گنتی;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t حافظہ_zero;
    uint32_t math_rand_بیج;

    lv_event_t * event_سرخی;
    uint32_t event_آخری_رجسٹر_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#اگر LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_تالا;
    lv_mutex_t img_decoder_open_تالا;
#اگر_ختم

    lv_cache_t * img_cache;
    lv_cache_t * img_سرخی_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#اگر LV_USE_OBSERVER
    lv_ll_t subject_ll;
#اگر_ختم

#اگر defined(LV_DRAW_SW_SHADOW_CACHE_سائز) && LV_DRAW_SW_SHADOW_CACHE_سائز > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#اگر_ختم
#اگر LV_DRAW_SW_COMPLEX
    lv_draw_sw_نقاب_radius_circle_dsc_arr_t sw_circle_cache;
#اگر_ختم

#اگر LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#اگر_ختم

#اگر LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_آخری_log_وقت;
#اگر_ختم

#اگر LV_USE_THEME_SIMPLE
    خالی * theme_simple;
#اگر_ختم

#اگر LV_USE_THEME_DEFAULT
    خالی * theme_default;
#اگر_ختم

#اگر LV_USE_THEME_MONO
    خالی * theme_mono;
#اگر_ختم

#اگر LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#اگر_ختم

    lv_ll_t fsdrv_ll;
#اگر LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#اگر_ختم
#اگر LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#اگر_ختم

#اگر LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#اگر_ختم

#اگر LV_USE_FREETYPE
    ساخت _lv_freetype_context_t * ft_context;
#اگر_ختم

#اگر LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#اگر_ختم

#اگر LV_USE_وسعت != 0
    ساخت snippet_ڈھیر * وسعت_snippet_ڈھیر;
#اگر_ختم

#اگر LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    ساخت _lv_profiler_builtin_ctx_t * profiler_context;
#اگر_ختم


#اگر LV_USE_MEM_MONITOR
    lv_sysmon_backend_ڈیٹا_t sysmon_mem;
#اگر_ختم

#اگر LV_USE_IME_PINYIN != 0
    سائز_t ime_cand_len;
#اگر_ختم

#اگر LV_USE_OBJ_ID_BUILTIN
    خالی * objid_array;
    uint32_t objid_گنتی;
#اگر_ختم

#اگر LV_USE_TEST
    lv_test_state_t test_state;
#اگر_ختم

#اگر LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ثابت حرف * translation_selected_lang;
#اگر_ختم

#اگر LV_USE_NUTTX
    ساخت _lv_nuttx_ctx_t * nuttx_ctx;
#اگر_ختم

#اگر LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#اگر_ختم

#اگر defined(__linux__)
    lv_linux_proc_stat_t linux_آخری_proc_stat;
#اگر LV_SYSMON_PROC_IDLE_دستیاب
    uint64_t linux_آخری_self_proc_وقت_ticks;
    lv_linux_proc_stat_t linux_آخری_system_کل_ticks_stat;
#اگر_ختم
#اگر_ختم

#اگر LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_وقت_مجموع;
    uint32_t freertos_non_idle_وقت_مجموع;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#اگر_ختم

#اگر LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#اگر_ختم

#اگر LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#اگر_ختم
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#اگر LV_ENABLE_GLOBAL_CUSTOM
#اگر LV_GLOBAL_USE_CUSTOM_INCLUDE
#شامل LV_GLOBAL_CUSTOM_INCLUDE
#اگر_ختم /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#اگر_غیر_تعریف_شدہ LV_GLOBAL_CUSTOM
#تعریف_کرو LV_GLOBAL_CUSTOM() lv_global_default()
#اگر_ختم
#تعریف_کرو LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#ورنہ
LV_ATTRIBUTE_EXTERN_ڈیٹا خارجی lv_global_t lv_global;
#تعریف_کرو LV_GLOBAL_DEFAULT() (&lv_global)
#اگر_ختم /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#اگر LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(خالی);
#اگر_ختم
#اگر_تعریف_شدہ __cplusplus
} /*extern "C"*/
#اگر_ختم

#اگر_ختم /*LV_GLOBAL_H*/
