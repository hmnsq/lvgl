/**
 * @file lv_global.h
 *
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_GLOBAL_H
#ವ್ಯಾಖ್ಯಾನಿಸು LV_GLOBAL_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

#ಆದರೆ LV_USE_DRAW_SW
#ಅನ್ತರ್ಭಾವ "../draw/sw/lv_draw_sw.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "../osal/lv_os_private.h"
#ಅನ್ತರ್ಭಾವ "../stdlib/builtin/lv_tlsf.h"

#ಆದರೆ LV_USE_FONT_COMPRESSED
#ಅನ್ತರ್ಭಾವ "../font/fmt_txt/lv_font_fmt_txt_private.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "../misc/lv_timer_private.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_anim_private.h"
#ಅನ್ತರ್ಭಾವ "../tick/lv_tick_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/lv_draw_buf_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/lv_draw_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/sw/lv_draw_sw_private.h"
#ಅನ್ತರ್ಭಾವ "../draw/sw/lv_draw_sw_mask_private.h"
#ಅನ್ತರ್ಭಾವ "../stdlib/builtin/lv_tlsf_private.h"
#ಅನ್ತರ್ಭಾವ "../debugging/sysmon/lv_sysmon_private.h"
#ಅನ್ತರ್ಭಾವ "../debugging/test/lv_test_private.h"
#ಅನ್ತರ್ಭಾವ "../layouts/lv_layout_private.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು ZERO_MEM_SENTINEL  0xa1b2c3d4

/**********************
 *      TYPEDEFS
 **********************/

#ಆದರೆ LV_USE_ಹರವು != 0
ರಚನೆ snippet_ರಾಶಿ;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FREETYPE
ರಚನೆ _lv_freetype_context_t;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
ರಚನೆ _lv_profiler_builtin_ctx_t;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_NUTTX
ರಚನೆ _lv_nuttx_ctx_t;
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ _lv_global_t {
    /**
     * User data for the LVGL library. Move from the bottom of the struct
     * to avoid breaking the ABI. E.g., if the user data is used by a
     * closed-source library, this can help to avoid re-compiling the library
     * when the lvgl-related configs are changed.
     */
    ಶೂನ್ಯ * user_ದತ್ತಾಂಶ;

    bool inited;
    bool deinit_ಒಳ_ಪ್ರಗತಿ;     /**< Can be used e.g. in the LV_EVENT_DELETE to deinit the drivers too */

    lv_ll_t disp_ll;
    lv_display_t * disp_refresh;
    lv_display_t * disp_default;

    lv_ll_t style_trans_ll;
    bool style_refresh;
    /** Number of widgets that currently have blur or a drop shadow (see
     * lv_obj_t::has_blur). If it is zero lv_obj_invalidate_expand_blur() is skipped. */
    uint32_t blur_obj_cnt;
    uint32_t style_custom_ಕೋಷ್ಟಕ_ಗಾತ್ರ;
    uint32_t style_ಕೊನೆಯ_custom_prop_id;
    uint8_t * style_custom_prop_flag_lookup_ಕೋಷ್ಟಕ;

    lv_ll_t ಗುಂಪು_ll;
    lv_ಗುಂಪು_t * ಗುಂಪು_default;

    lv_ll_t indev_ll;
    lv_indev_t * indev_ಸಕ್ರಿಯ;
    lv_obj_t * indev_obj_ಸಕ್ರಿಯ;

    uint32_t layout_ಎಣಿಕೆ;
    lv_layout_dsc_t * layout_list;
    bool layout_update_mutex;

    uint32_t ಸ್ಮರಣೆ_zero;
    uint32_t math_rand_ಬೀಜ;

    lv_event_t * event_ಶೀರ್ಷಿಕೆ;
    uint32_t event_ಕೊನೆಯ_ರಿಜಿಸ್ಟರ್_id;

    lv_timer_state_t timer_state;
    lv_anim_state_t anim_state;
    lv_tick_state_t tick_state;

    lv_draw_buf_handlers_t draw_buf_handlers;
    lv_draw_buf_handlers_t font_draw_buf_handlers;
    lv_draw_buf_handlers_t image_cache_draw_buf_handlers;  /**< Ensure that all assigned draw buffers
                                                            * can be managed by image cache. */

    lv_ll_t img_decoder_ll;
#ಆದರೆ LV_USE_OS != LV_OS_NONE
    lv_mutex_t img_decoder_info_ಬೀಗ;
    lv_mutex_t img_decoder_open_ಬೀಗ;
#ಆದರೆ_ಅಂತ್ಯ

    lv_cache_t * img_cache;
    lv_cache_t * img_ಶೀರ್ಷಿಕೆ_cache;

    lv_draw_global_info_t draw_info;
    lv_ll_t draw_sw_blend_handler_ll;

#ಆದರೆ LV_USE_OBSERVER
    lv_ll_t subject_ll;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(LV_DRAW_SW_SHADOW_CACHE_ಗಾತ್ರ) && LV_DRAW_SW_SHADOW_CACHE_ಗಾತ್ರ > 0
    lv_draw_sw_shadow_cache_t sw_shadow_cache;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ LV_DRAW_SW_COMPLEX
    lv_draw_sw_ಮುಖವಾಡ_radius_circle_dsc_arr_t sw_circle_cache;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_LOG
    lv_log_print_g_cb_t custom_log_print_cb;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_LOG && LV_LOG_USE_TIMESTAMP
    uint32_t log_ಕೊನೆಯ_log_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_THEME_SIMPLE
    ಶೂನ್ಯ * theme_simple;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_THEME_DEFAULT
    ಶೂನ್ಯ * theme_default;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_THEME_MONO
    ಶೂನ್ಯ * theme_mono;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    lv_tlsf_state_t tlsf_state;
#ಆದರೆ_ಅಂತ್ಯ

    lv_ll_t fsdrv_ll;
#ಆದರೆ LV_USE_FS_STDIO != '\0'
    lv_fs_drv_t stdio_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ LV_USE_FS_POSIX
    lv_fs_drv_t posix_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_FATFS
    lv_fs_drv_t fatfs_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_WIN32 != '\0'
    lv_fs_drv_t win32_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_UEFI
    lv_fs_drv_t uefi_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_LITTLEFS
    lv_fs_drv_t littlefs_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_ARDUINO_ESP_LITTLEFS
    lv_fs_drv_t arduino_esp_littlefs_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_ARDUINO_SD
    lv_fs_drv_t arduino_sd_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FS_FROGFS
    lv_fs_drv_t frogfs_fs_drv;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FREETYPE
    ರಚನೆ _lv_freetype_context_t * ft_context;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_FONT_COMPRESSED
    lv_font_fmt_rle_t font_fmt_rle;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_ಹರವು != 0
    ರಚನೆ snippet_ರಾಶಿ * ಹರವು_snippet_ರಾಶಿ;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_PROFILER && LV_USE_PROFILER_BUILTIN
    ರಚನೆ _lv_profiler_builtin_ctx_t * profiler_context;
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ LV_USE_MEM_MONITOR
    lv_sysmon_backend_ದತ್ತಾಂಶ_t sysmon_mem;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_IME_PINYIN != 0
    ಗಾತ್ರ_t ime_cand_len;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_OBJ_ID_BUILTIN
    ಶೂನ್ಯ * objid_array;
    uint32_t objid_ಎಣಿಕೆ;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_TEST
    lv_test_state_t test_state;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_TRANSLATION
    lv_ll_t translation_packs_ll;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * translation_selected_lang;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_NUTTX
    ರಚನೆ _lv_nuttx_ctx_t * nuttx_ctx;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_OS != LV_OS_NONE
    lv_mutex_t lv_general_mutex;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__)
    lv_linux_proc_stat_t linux_ಕೊನೆಯ_proc_stat;
#ಆದರೆ LV_SYSMON_PROC_IDLE_ಲಭ್ಯ
    uint64_t linux_ಕೊನೆಯ_self_proc_ಸಮಯ_ticks;
    lv_linux_proc_stat_t linux_ಕೊನೆಯ_system_ಒಟ್ಟು_ticks_stat;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_OS == LV_OS_FREERTOS
    uint32_t freertos_idle_ಸಮಯ_ಮೊತ್ತ;
    uint32_t freertos_non_idle_ಸಮಯ_ಮೊತ್ತ;
    uint32_t freertos_task_switch_timestamp;
    bool freertos_idle_task_running;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_EVDEV
    lv_evdev_discovery_t * evdev_discovery;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ LV_USE_DRAW_EVE
    lv_draw_eve_unit_t * draw_eve_unit;
#ಆದರೆ_ಅಂತ್ಯ
} lv_global_t;

/**********************
 *      MACROS
 **********************/

#ಆದರೆ LV_ENABLE_GLOBAL_CUSTOM
#ಆದರೆ LV_GLOBAL_USE_CUSTOM_INCLUDE
#ಅನ್ತರ್ಭಾವ LV_GLOBAL_CUSTOM_INCLUDE
#ಆದರೆ_ಅಂತ್ಯ /*LV_GLOBAL_USE_CUSTOM_INCLUDE*/

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LV_GLOBAL_CUSTOM
#ವ್ಯಾಖ್ಯಾನಿಸು LV_GLOBAL_CUSTOM() lv_global_default()
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು LV_GLOBAL_DEFAULT() LV_GLOBAL_CUSTOM()
#ಇಲ್ಲದಿದ್ದರೆ
LV_ATTRIBUTE_EXTERN_ದತ್ತಾಂಶ ಬಾಹ್ಯ lv_global_t lv_global;
#ವ್ಯಾಖ್ಯಾನಿಸು LV_GLOBAL_DEFAULT() (&lv_global)
#ಆದರೆ_ಅಂತ್ಯ /*LV_ENABLE_GLOBAL_CUSTOM*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#ಆದರೆ LV_ENABLE_GLOBAL_CUSTOM
/**
 * Get the default global object for current thread
 * @return  pointer to the default global object
 */
lv_global_t * lv_global_default(ಶೂನ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
} /*extern "C"*/
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /*LV_GLOBAL_H*/
