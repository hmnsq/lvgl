/**
 * @file lv_global.h
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_GLOBAL_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_GLOBAL_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"

#ਜੇ LV_USE_DRAW_SW
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/sw/lv_draw_sw.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "../osal/lv_os_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../stdlib/builtin/lv_tlsf.h"

#ਜੇ LV_USE_FONT_COMPRESSED
#ਅਨ੍ਤਰ੍ਭਾਵ "../font/fmt_txt/lv_font_fmt_txt_private.h"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_timer_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_anim_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../tick/lv_tick_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/lv_draw_buf_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/lv_draw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/sw/lv_draw_sw_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../draw/sw/lv_draw_sw_mask_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../stdlib/builtin/lv_tlsf_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../debugging/sysmon/lv_sysmon_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../debugging/test/lv_test_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#ਜੇ LV_USE_ਵਿਸਤਾਰ != 0
ਬਣਤਰ snippet_ਢੇਰ;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FREETYPE
ਬਣਤਰ _lv_freetype_context_t;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
ਬਣਤਰ _lv_profiler_builtin_ctx_t;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_NUTTX
ਬਣਤਰ _lv_nuttx_ctx_t;
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    ਖਾਲੀ * user_ਅੰਕੜੇ;

    bool inited;
    bool deinit_ਵਿੱਚ_ਤਰੱਕੀ;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_ਸਾਰਣੀ_ਮਾਪ;
    uint32_t style_ਆਖਰੀ_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_ਸਾਰਣੀ;

    lv_ll_t ਸਮੂਹ_ll;
    lv_ਸਮੂਹ_t * ਸਮੂਹ_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_ਸਰਗਰਮ;
    lv_obj_t * indev_obj_ਸਰਗਰਮ;

    uint32_t layout_ਗਿਣਤੀ;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t ਸਮ੍ਰਿਤੀ_zero;
    uint32_t math_rand_ਬੀਜ;

    lv_event_t * event_ਸਿਰਲੇਖ;
    uint32_t event_ਆਖਰੀ_ਰਜਿਸਟਰ_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#ਜੇ LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_ਤਾਲਾ;
    lv_mutex_t img_decoder_open_ਤਾਲਾ;
#ਜੇ_ਅੰਤ

    lv_cache_t * img_cache;
    lv_cache_t * img_ਸਿਰਲੇਖ_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#ਜੇ LV_USE_OBSERVER
    lv_ll_t subject_ll;
#ਜੇ_ਅੰਤ

#ਜੇ defined(LV_DRAW_SW_SHADOW_CACHE_ਮਾਪ) && LV_DRAW_SW_SHADOW_CACHE_ਮਾਪ > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#ਜੇ_ਅੰਤ
#ਜੇ LV_DRAW_SW_COMPLEX
    lv_draw_sw_ਨਕਾਬ_radius_circle_dsc_arr_t sw_circle_cache;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_ਆਖਰੀ_log_ਸਮਾਂ;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_THEME_SIMPLE
    ਖਾਲੀ * theme_simple;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_THEME_DEFAULT
    ਖਾਲੀ * theme_default;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_THEME_MONO
    ਖਾਲੀ * theme_mono;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#ਜੇ_ਅੰਤ

    lv_ll_t fsdrv_ll;
#ਜੇ LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#ਜੇ_ਅੰਤ
#ਜੇ LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FREETYPE
    ਬਣਤਰ _lv_freetype_context_t * ft_context;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_ਵਿਸਤਾਰ != 0
    ਬਣਤਰ snippet_ਢੇਰ * ਵਿਸਤਾਰ_snippet_ਢੇਰ;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    ਬਣਤਰ _lv_profiler_builtin_ctx_t * profiler_context;
#ਜੇ_ਅੰਤ


#ਜੇ LV_USE_MEM_MONITOR
    lv_sysmon_backend_ਅੰਕੜੇ_t sysmon_mem;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_IME_PINYIN != 0
    ਮਾਪ_t ime_cand_len;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_OBJ_ID_BUILTIN
    ਖਾਲੀ * objid_array;
    uint32_t objid_ਗਿਣਤੀ;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_TEST
    lv_test_state_t test_state;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ਅਚਲ ਅੱਖਰ * translation_selected_lang;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_NUTTX
    ਬਣਤਰ _lv_nuttx_ctx_t * nuttx_ctx;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__)
    lv_linux_proc_stat_t linux_ਆਖਰੀ_proc_stat;
#ਜੇ LV_SYSMON_PROC_IDLE_ਉਪਲਬਧ
    uint64_t linux_ਆਖਰੀ_self_proc_ਸਮਾਂ_ticks;
    lv_linux_proc_stat_t linux_ਆਖਰੀ_system_ਕੁੱਲ_ticks_stat;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_ਸਮਾਂ_ਜੋੜ;
    uint32_t freertos_non_idle_ਸਮਾਂ_ਜੋੜ;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#ਜੇ_ਅੰਤ

#ਜੇ LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#ਜੇ_ਅੰਤ
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#ਜੇ LV_ENABLE_GLOBAL_CUSTOM
#ਜੇ LV_GLOBAL_USE_CUSTOM_INCLUDE
#ਅਨ੍ਤਰ੍ਭਾਵ LV_GLOBAL_CUSTOM_INCLUDE
#ਜੇ_ਅੰਤ /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LV_GLOBAL_CUSTOM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_GLOBAL_CUSTOM() lv_global_default()
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#ਨਹੀਂਤਾਂ
LV_ATTRIBUTE_EXTERN_ਅੰਕੜੇ ਬਾਹਰੀ lv_global_t lv_global;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_GLOBAL_DEFAULT() (&lv_global)
#ਜੇ_ਅੰਤ /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#ਜੇ LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(ਖਾਲੀ);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
} /*extern "C"*/
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /*LV_GLOBAL_H*/
