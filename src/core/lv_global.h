/**
 * @file lv_global.h
 *
 */

#جيڪڏهن_وصف_نه_ٿيل LV_GLOBAL_H
#وصف_ڪريو LV_GLOBAL_H

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

#إذا LV_USE_DRAW_SW
#ضمن "../draw/sw/lv_draw_sw.h"
#جيڪڏهن_پورو
#ضمن "../osal/lv_os_private.h"
#ضمن "../stdlib/builtin/lv_tlsf.h"

#إذا LV_USE_FONT_COMPRESSED
#ضمن "../font/fmt_txt/lv_font_fmt_txt_private.h"
#جيڪڏهن_پورو

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
#وصف_ڪريو ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#إذا LV_USE_وسعت != 0
بنية snippet_ڍير;
#جيڪڏهن_پورو

#إذا LV_USE_FREETYPE
بنية _lv_freetype_context_t;
#جيڪڏهن_پورو

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
بنية _lv_profiler_builtin_ctx_t;
#جيڪڏهن_پورو

#إذا LV_USE_NUTTX
بنية _lv_nuttx_ctx_t;
#جيڪڏهن_پورو

تعريفنوع بنية _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    فراغ * user_معلومات;

    bool inited;
    bool deinit_اندريون_ترقي;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_جدول_سائيز;
    uint32_t style_آخري_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_جدول;

    lv_ll_t گروهه_ll;
    lv_گروهه_t * گروهه_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_سرگرم;
    lv_obj_t * indev_obj_سرگرم;

    uint32_t layout_ڳڻپ;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t يادگيري_zero;
    uint32_t math_rand_ٻج;

    lv_event_t * event_سرو;
    uint32_t event_آخري_رجسٽر_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_تالو;
    lv_mutex_t img_decoder_open_تالو;
#جيڪڏهن_پورو

    lv_cache_t * img_cache;
    lv_cache_t * img_سرو_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#إذا LV_USE_OBSERVER
    lv_ll_t subject_ll;
#جيڪڏهن_پورو

#إذا defined(LV_DRAW_SW_SHADOW_CACHE_سائيز) && LV_DRAW_SW_SHADOW_CACHE_سائيز > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#جيڪڏهن_پورو
#إذا LV_DRAW_SW_COMPLEX
    lv_draw_sw_نقاب_radius_circle_dsc_arr_t sw_circle_cache;
#جيڪڏهن_پورو

#إذا LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#جيڪڏهن_پورو

#إذا LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_آخري_log_وقت;
#جيڪڏهن_پورو

#إذا LV_USE_THEME_SIMPLE
    فراغ * theme_simple;
#جيڪڏهن_پورو

#إذا LV_USE_THEME_DEFAULT
    فراغ * theme_default;
#جيڪڏهن_پورو

#إذا LV_USE_THEME_MONO
    فراغ * theme_mono;
#جيڪڏهن_پورو

#إذا LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#جيڪڏهن_پورو

    lv_ll_t fsdrv_ll;
#إذا LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#جيڪڏهن_پورو
#إذا LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#جيڪڏهن_پورو

#إذا LV_USE_FREETYPE
    بنية _lv_freetype_context_t * ft_context;
#جيڪڏهن_پورو

#إذا LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#جيڪڏهن_پورو

#إذا LV_USE_وسعت != 0
    بنية snippet_ڍير * وسعت_snippet_ڍير;
#جيڪڏهن_پورو

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    بنية _lv_profiler_builtin_ctx_t * profiler_context;
#جيڪڏهن_پورو


#إذا LV_USE_MEM_MONITOR
    lv_sysmon_backend_معلومات_t sysmon_mem;
#جيڪڏهن_پورو

#إذا LV_USE_IME_PINYIN != 0
    سائيز_t ime_cand_len;
#جيڪڏهن_پورو

#إذا LV_USE_OBJ_ID_BUILTIN
    فراغ * objid_array;
    uint32_t objid_ڳڻپ;
#جيڪڏهن_پورو

#إذا LV_USE_TEST
    lv_test_state_t test_state;
#جيڪڏهن_پورو

#إذا LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ثابت حرف * translation_selected_lang;
#جيڪڏهن_پورو

#إذا LV_USE_NUTTX
    بنية _lv_nuttx_ctx_t * nuttx_ctx;
#جيڪڏهن_پورو

#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#جيڪڏهن_پورو

#إذا defined(__linux__)
    lv_linux_proc_stat_t linux_آخري_proc_stat;
#إذا LV_SYSMON_PROC_IDLE_موجود
    uint64_t linux_آخري_self_proc_وقت_ticks;
    lv_linux_proc_stat_t linux_آخري_system_ڪل_ticks_stat;
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_وقت_جوڙ;
    uint32_t freertos_non_idle_وقت_جوڙ;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#جيڪڏهن_پورو

#إذا LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#جيڪڏهن_پورو

#إذا LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#جيڪڏهن_پورو
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#إذا LV_ENABLE_GLOBAL_CUSTOM
#إذا LV_GLOBAL_USE_CUSTOM_INCLUDE
#ضمن LV_GLOBAL_CUSTOM_INCLUDE
#جيڪڏهن_پورو /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#جيڪڏهن_وصف_نه_ٿيل LV_GLOBAL_CUSTOM
#وصف_ڪريو LV_GLOBAL_CUSTOM() lv_global_default()
#جيڪڏهن_پورو
#وصف_ڪريو LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#وإلا
LV_ATTRIBUTE_EXTERN_معلومات خارجي lv_global_t lv_global;
#وصف_ڪريو LV_GLOBAL_DEFAULT() (&lv_global)
#جيڪڏهن_پورو /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#إذا LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(فراغ);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل __cplusplus
} /*extern "C"*/
#جيڪڏهن_پورو

#جيڪڏهن_پورو /*LV_GLOBAL_H*/
