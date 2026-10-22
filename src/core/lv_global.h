/**
 * @file lv_global.h
 *
 */

#که_نه_تعریف_شوی LV_GLOBAL_H
#تعریف_کړه LV_GLOBAL_H

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/*********************
 *      INCLUDES
 *********************/
#ضمن "../lvgl_public.h"

#إذا LV_USE_DRAW_SW
#ضمن "../draw/sw/lv_draw_sw.h"
#د_که_پای
#ضمن "../osal/lv_os_private.h"
#ضمن "../stdlib/builtin/lv_tlsf.h"

#إذا LV_USE_FONT_COMPRESSED
#ضمن "../font/fmt_txt/lv_font_fmt_txt_private.h"
#د_که_پای

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
#تعریف_کړه ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#إذا LV_USE_پراختیا != 0
بنية snippet_ډېری;
#د_که_پای

#إذا LV_USE_FREETYPE
بنية _lv_freetype_context_t;
#د_که_پای

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
بنية _lv_profiler_builtin_ctx_t;
#د_که_پای

#إذا LV_USE_NUTTX
بنية _lv_nuttx_ctx_t;
#د_که_پای

تعريفنوع بنية _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    فراغ * user_معلومات;

    bool inited;
    bool deinit_دننه_پرمختګ;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_جدول_اندازه;
    uint32_t style_وروستی_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_جدول;

    lv_ll_t ډله_ll;
    lv_ډله_t * ډله_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_فعال;
    lv_obj_t * indev_obj_فعال;

    uint32_t layout_شمېر;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t حافظه_zero;
    uint32_t math_rand_تخم;

    lv_event_t * event_سرلیک;
    uint32_t event_وروستی_ثبتوونکی_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_قلف;
    lv_mutex_t img_decoder_open_قلف;
#د_که_پای

    lv_cache_t * img_cache;
    lv_cache_t * img_سرلیک_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#إذا LV_USE_OBSERVER
    lv_ll_t subject_ll;
#د_که_پای

#إذا defined(LV_DRAW_SW_SHADOW_CACHE_اندازه) && LV_DRAW_SW_SHADOW_CACHE_اندازه > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#د_که_پای
#إذا LV_DRAW_SW_COMPLEX
    lv_draw_sw_ماسک_radius_circle_dsc_arr_t sw_circle_cache;
#د_که_پای

#إذا LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#د_که_پای

#إذا LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_وروستی_log_وخت;
#د_که_پای

#إذا LV_USE_THEME_SIMPLE
    فراغ * theme_simple;
#د_که_پای

#إذا LV_USE_THEME_DEFAULT
    فراغ * theme_default;
#د_که_پای

#إذا LV_USE_THEME_MONO
    فراغ * theme_mono;
#د_که_پای

#إذا LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#د_که_پای

    lv_ll_t fsdrv_ll;
#إذا LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#د_که_پای
#إذا LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#د_که_پای

#إذا LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#د_که_پای

#إذا LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#د_که_پای

#إذا LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#د_که_پای

#إذا LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#د_که_پای

#إذا LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#د_که_پای

#إذا LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#د_که_پای

#إذا LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#د_که_پای

#إذا LV_USE_FREETYPE
    بنية _lv_freetype_context_t * ft_context;
#د_که_پای

#إذا LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#د_که_پای

#إذا LV_USE_پراختیا != 0
    بنية snippet_ډېری * پراختیا_snippet_ډېری;
#د_که_پای

#إذا LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    بنية _lv_profiler_builtin_ctx_t * profiler_context;
#د_که_پای


#إذا LV_USE_MEM_MONITOR
    lv_sysmon_backend_معلومات_t sysmon_mem;
#د_که_پای

#إذا LV_USE_IME_PINYIN != 0
    اندازه_t ime_cand_len;
#د_که_پای

#إذا LV_USE_OBJ_ID_BUILTIN
    فراغ * objid_array;
    uint32_t objid_شمېر;
#د_که_پای

#إذا LV_USE_TEST
    lv_test_state_t test_state;
#د_که_پای

#إذا LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ثابت حرف * translation_selected_lang;
#د_که_پای

#إذا LV_USE_NUTTX
    بنية _lv_nuttx_ctx_t * nuttx_ctx;
#د_که_پای

#إذا LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#د_که_پای

#إذا defined(__linux__)
    lv_linux_proc_stat_t linux_وروستی_proc_stat;
#إذا LV_SYSMON_PROC_IDLE_شته
    uint64_t linux_وروستی_self_proc_وخت_ticks;
    lv_linux_proc_stat_t linux_وروستی_system_ټول_ticks_stat;
#د_که_پای
#د_که_پای

#إذا LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_وخت_مجموعه;
    uint32_t freertos_non_idle_وخت_مجموعه;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#د_که_پای

#إذا LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#د_که_پای

#إذا LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#د_که_پای
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#إذا LV_ENABLE_GLOBAL_CUSTOM
#إذا LV_GLOBAL_USE_CUSTOM_INCLUDE
#ضمن LV_GLOBAL_CUSTOM_INCLUDE
#د_که_پای /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#که_نه_تعریف_شوی LV_GLOBAL_CUSTOM
#تعریف_کړه LV_GLOBAL_CUSTOM() lv_global_default()
#د_که_پای
#تعریف_کړه LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#وإلا
LV_ATTRIBUTE_EXTERN_معلومات خارجي lv_global_t lv_global;
#تعریف_کړه LV_GLOBAL_DEFAULT() (&lv_global)
#د_که_پای /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#إذا LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(فراغ);
#د_که_پای
#که_تعریف_شوی __cplusplus
} /*extern "C"*/
#د_که_پای

#د_که_پای /*LV_GLOBAL_H*/
