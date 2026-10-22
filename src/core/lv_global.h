/**
 * @file lv_global.h
 *
 */

#अगर_अघोषित LV_GLOBAL_H
#घोषणा LV_GLOBAL_H

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/*********************
 *      INCLUDES
 *********************/
#समावेश "../lvgl_public.h"

#अगर LV_USE_DRAW_SW
#समावेश "../draw/sw/lv_draw_sw.h"
#पूर्ण_अगर
#समावेश "../osal/lv_os_private.h"
#समावेश "../stdlib/builtin/lv_tlsf.h"

#अगर LV_USE_FONT_COMPRESSED
#समावेश "../font/fmt_txt/lv_font_fmt_txt_private.h"
#पूर्ण_अगर

#समावेश "../misc/lv_timer_private.h"
#समावेश "../misc/lv_anim_private.h"
#समावेश "../tick/lv_tick_private.h"
#समावेश "../draw/lv_draw_buf_private.h"
#समावेश "../draw/lv_draw_private.h"
#समावेश "../draw/sw/lv_draw_sw_private.h"
#समावेश "../draw/sw/lv_draw_sw_mask_private.h"
#समावेश "../stdlib/builtin/lv_tlsf_private.h"
#समावेश "../debugging/sysmon/lv_sysmon_private.h"
#समावेश "../debugging/test/lv_test_private.h"
#समावेश "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#अगर LV_USE_SPAN != 0
काष्ठा snippet_ढेर;
#पूर्ण_अगर

#अगर LV_USE_FREETYPE
काष्ठा _lv_freetype_context_t;
#पूर्ण_अगर

#अगर LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
काष्ठा _lv_profiler_builtin_ctx_t;
#पूर्ण_अगर

#अगर LV_USE_NUTTX
काष्ठा _lv_nuttx_ctx_t;
#पूर्ण_अगर

प्रकार काष्ठा _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    व्योम * user_तथ्य;

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
    uint32_t style_custom_table_आकार;
    uint32_t style_last_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_table;

    lv_ll_t group_ll;
    lv_group_t * group_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_active;
    lv_obj_t * indev_obj_active;

    uint32_t layout_गिनती;
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
#अगर LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_lock;
    lv_mutex_t img_decoder_open_lock;
#पूर्ण_अगर

    lv_cache_t * img_cache;
    lv_cache_t * img_header_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#अगर LV_USE_OBSERVER
    lv_ll_t subject_ll;
#पूर्ण_अगर

#अगर defined(LV_DRAW_SW_SHADOW_CACHE_आकार) && LV_DRAW_SW_SHADOW_CACHE_आकार > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#पूर्ण_अगर
#अगर LV_DRAW_SW_COMPLEX
    lv_draw_sw_mask_radius_circle_dsc_arr_t sw_circle_cache;
#पूर्ण_अगर

#अगर LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#पूर्ण_अगर

#अगर LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_last_log_time;
#पूर्ण_अगर

#अगर LV_USE_THEME_SIMPLE
    व्योम * theme_simple;
#पूर्ण_अगर

#अगर LV_USE_THEME_DEFAULT
    व्योम * theme_default;
#पूर्ण_अगर

#अगर LV_USE_THEME_MONO
    व्योम * theme_mono;
#पूर्ण_अगर

#अगर LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#पूर्ण_अगर

    lv_ll_t fsdrv_ll;
#अगर LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#पूर्ण_अगर
#अगर LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#पूर्ण_अगर

#अगर LV_USE_FREETYPE
    काष्ठा _lv_freetype_context_t * ft_context;
#पूर्ण_अगर

#अगर LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#पूर्ण_अगर

#अगर LV_USE_SPAN != 0
    काष्ठा snippet_ढेर * span_snippet_ढेर;
#पूर्ण_अगर

#अगर LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    काष्ठा _lv_profiler_builtin_ctx_t * profiler_context;
#पूर्ण_अगर


#अगर LV_USE_MEM_MONITOR
    lv_sysmon_backend_तथ्य_t sysmon_mem;
#पूर्ण_अगर

#अगर LV_USE_IME_PINYIN != 0
    माप_प्रकार ime_cand_len;
#पूर्ण_अगर

#अगर LV_USE_OBJ_ID_BUILTIN
    व्योम * objid_array;
    uint32_t objid_गिनती;
#पूर्ण_अगर

#अगर LV_USE_TEST
    lv_test_state_t test_state;
#पूर्ण_अगर

#अगर LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    स्थिर अक्षर * translation_selected_lang;
#पूर्ण_अगर

#अगर LV_USE_NUTTX
    काष्ठा _lv_nuttx_ctx_t * nuttx_ctx;
#पूर्ण_अगर

#अगर LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#पूर्ण_अगर

#अगर defined(__linux__)
    lv_linux_proc_stat_t linux_last_proc_stat;
#अगर LV_SYSMON_PROC_IDLE_AVAILABLE
    uint64_t linux_last_self_proc_time_ticks;
    lv_linux_proc_stat_t linux_last_system_योग_ticks_stat;
#पूर्ण_अगर
#पूर्ण_अगर

#अगर LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_time_योग;
    uint32_t freertos_non_idle_time_योग;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#पूर्ण_अगर

#अगर LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#पूर्ण_अगर

#अगर LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#पूर्ण_अगर
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#अगर LV_ENABLE_GLOBAL_CUSTOM
#अगर LV_GLOBAL_USE_CUSTOM_INCLUDE
#समावेश LV_GLOBAL_CUSTOM_INCLUDE
#पूर्ण_अगर /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#अगर_अघोषित LV_GLOBAL_CUSTOM
#घोषणा LV_GLOBAL_CUSTOM() lv_global_default()
#पूर्ण_अगर
#घोषणा LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#अन्यथा
LV_ATTRIBUTE_EXTERN_तथ्य बाह्य lv_global_t lv_global;
#घोषणा LV_GLOBAL_DEFAULT() (&lv_global)
#पूर्ण_अगर /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#अगर LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(व्योम);
#पूर्ण_अगर
#अगर_घोषित __cplusplus
} /*extern "C"*/
#पूर्ण_अगर

#पूर्ण_अगर /*LV_GLOBAL_H*/
