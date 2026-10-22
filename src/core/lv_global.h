/**
 * @file lv_global.h
 *
 */

#ifndef LV_GLOBAL_H
#define LV_GLOBAL_H

#ifdef __cplusplus
બાહ્ય "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "../lvgl_public.h"

#જો LV_USE_DRAW_SW
#અન્તર્ભાવ "../draw/sw/lv_draw_sw.h"
#endif
#અન્તર્ભાવ "../osal/lv_os_private.h"
#અન્તર્ભાવ "../stdlib/builtin/lv_tlsf.h"

#જો LV_USE_FONT_COMPRESSED
#અન્તર્ભાવ "../font/fmt_txt/lv_font_fmt_txt_private.h"
#endif

#અન્તર્ભાવ "../misc/lv_timer_private.h"
#અન્તર્ભાવ "../misc/lv_anim_private.h"
#અન્તર્ભાવ "../tick/lv_tick_private.h"
#અન્તર્ભાવ "../draw/lv_draw_buf_private.h"
#અન્તર્ભાવ "../draw/lv_draw_private.h"
#અન્તર્ભાવ "../draw/sw/lv_draw_sw_private.h"
#અન્તર્ભાવ "../draw/sw/lv_draw_sw_mask_private.h"
#અન્તર્ભાવ "../stdlib/builtin/lv_tlsf_private.h"
#અન્તર્ભાવ "../debugging/sysmon/lv_sysmon_private.h"
#અન્તર્ભાવ "../debugging/test/lv_test_private.h"
#અન્તર્ભાવ "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#define ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#જો LV_USE_SPAN != 0
સંરચના snippet_ઢગલો;
#endif

#જો LV_USE_FREETYPE
સંરચના _lv_freetype_context_t;
#endif

#જો LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
સંરચના _lv_profiler_builtin_ctx_t;
#endif

#જો LV_USE_NUTTX
સંરચના _lv_nuttx_ctx_t;
#endif

પ્રકારવ્યાખ્યા સંરચના _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    શૂન્ય * user_data;

    bool inited;
    bool deinit_in_progress;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_table_size;
    uint32_t style_last_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_table;

    lv_ll_t group_ll;
    lv_group_t * group_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_active;
    lv_obj_t * indev_obj_active;

    uint32_t layout_ગણતરી;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t memory_zero;
    uint32_t math_rand_seed;

    lv_event_t * event_header;
    uint32_t event_last_register_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#જો LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_lock;
    lv_mutex_t img_decoder_open_lock;
#endif

    lv_cache_t * img_cache;
    lv_cache_t * img_header_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#જો LV_USE_OBSERVER
    lv_ll_t subject_ll;
#endif

#જો defined(LV_DRAW_SW_SHADOW_CACHE_SIZE) && LV_DRAW_SW_SHADOW_CACHE_SIZE > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#endif
#જો LV_DRAW_SW_COMPLEX
    lv_draw_sw_mask_radius_circle_dsc_arr_t sw_circle_cache;
#endif

#જો LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#endif

#જો LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_last_log_time;
#endif

#જો LV_USE_THEME_SIMPLE
    શૂન્ય * theme_simple;
#endif

#જો LV_USE_THEME_DEFAULT
    શૂન્ય * theme_default;
#endif

#જો LV_USE_THEME_MONO
    શૂન્ય * theme_mono;
#endif

#જો LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#endif

    lv_ll_t fsdrv_ll;
#જો LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#endif
#જો LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#endif

#જો LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#endif

#જો LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#endif

#જો LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#endif

#જો LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#endif

#જો LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#endif

#જો LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#endif

#જો LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#endif

#જો LV_USE_FREETYPE
    સંરચના _lv_freetype_context_t * ft_context;
#endif

#જો LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#endif

#જો LV_USE_SPAN != 0
    સંરચના snippet_ઢગલો * span_snippet_ઢગલો;
#endif

#જો LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    સંરચના _lv_profiler_builtin_ctx_t * profiler_context;
#endif


#જો LV_USE_MEM_MONITOR
    lv_sysmon_backend_data_t sysmon_mem;
#endif

#જો LV_USE_IME_PINYIN != 0
    size_t ime_cand_len;
#endif

#જો LV_USE_OBJ_ID_BUILTIN
    શૂન્ય * objid_array;
    uint32_t objid_ગણતરી;
#endif

#જો LV_USE_TEST
    lv_test_state_t test_state;
#endif

#જો LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    અચલ અક્ષર * translation_selected_lang;
#endif

#જો LV_USE_NUTTX
    સંરચના _lv_nuttx_ctx_t * nuttx_ctx;
#endif

#જો LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#endif

#જો defined(__linux__)
    lv_linux_proc_stat_t linux_last_proc_stat;
#જો LV_SYSMON_PROC_IDLE_AVAILABLE
    uint64_t linux_last_self_proc_time_ticks;
    lv_linux_proc_stat_t linux_last_system_total_ticks_stat;
#endif
#endif

#જો LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_time_sum;
    uint32_t freertos_non_idle_time_sum;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#endif

#જો LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#endif

#જો LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#endif
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#જો LV_ENABLE_GLOBAL_CUSTOM
#જો LV_GLOBAL_USE_CUSTOM_INCLUDE
#અન્તર્ભાવ LV_GLOBAL_CUSTOM_INCLUDE
#endif /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#ifndef LV_GLOBAL_CUSTOM
#define LV_GLOBAL_CUSTOM() lv_global_default()
#endif
#define LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#નહીંતર
LV_ATTRIBUTE_EXTERN_DATA બાહ્ય lv_global_t lv_global;
#define LV_GLOBAL_DEFAULT() (&lv_global)
#endif /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#જો LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(શૂન્ય);
#endif
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_GLOBAL_H*/
