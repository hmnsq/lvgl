/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_obj_scroll_private.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"
#ಅನ್ತರ್ಭಾವ "../misc/lv_anim_private.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../indev/lv_indev_scroll.h"
#ಅನ್ತರ್ಭಾವ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ವ್ಯಾಖ್ಯಾನಿಸು MY_CLASS (&lv_obj_class)
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCROLL_ANIM_ಸಮಯ_MIN
    #ವ್ಯಾಖ್ಯಾನಿಸು SCROLL_ANIM_ಸಮಯ_MIN    200    /*ms*/
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCROLL_ANIM_ಸಮಯ_MAX
    #ವ್ಯಾಖ್ಯಾನಿಸು SCROLL_ANIM_ಸಮಯ_MAX    400    /*ms*/
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು SCROLLBAR_MIN_ಗಾತ್ರ      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t mul_div(int32_t v, int32_t ಗುಣಕ, int32_t ಭಾಜಕ);
ಸ್ಥಿರ ಶೂನ್ಯ scroll_x_anim(ಶೂನ್ಯ * obj, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ scroll_y_anim(ಶೂನ್ಯ * obj, int32_t v);
ಸ್ಥಿರ ಶೂನ್ಯ scroll_ಕೊನೆ_cb(lv_anim_t * a);
ಸ್ಥಿರ ಶೂನ್ಯ scroll_area_into_view(ಸ್ಥಿರಾಂಕ lv_area_t * area, lv_obj_t * ಮಗು, lv_ಬಿಂದು_t * scroll_ಮೌಲ್ಯ,
                                  lv_anim_enable_t anim_en);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*=====================
 * Setter functions
 *====================*/

ಶೂನ್ಯ lv_obj_ಗಣ_scrollbar_ಬಹುಲಕ(lv_obj_t * obj, lv_scrollbar_ಬಹುಲಕ_t ಬಹುಲಕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(obj->spec_attr->scrollbar_ಬಹುಲಕ == ಬಹುಲಕ) ಹಿಂತಿರುಗಿ;
    obj->spec_attr->scrollbar_ಬಹುಲಕ = ಬಹುಲಕ;
    lv_obj_invalidate(obj);
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }
    obj->spec_attr->scroll_snap_x = align;
}

ಶೂನ್ಯ lv_obj_ಗಣ_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_ಬಹುಲಕ_t lv_obj_get_scrollbar_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_SCROLLBAR_ಬಹುಲಕ_OFF);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ (lv_scrollbar_ಬಹುಲಕ_t) obj->spec_attr->scrollbar_ಬಹುಲಕ;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_SCROLLBAR_ಬಹುಲಕ_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_DIR_NONE);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ (lv_dir_t) obj->spec_attr->scroll_dir;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_SCROLL_SNAP_NONE);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_SCROLL_SNAP_NONE);

    ಆದರೆ(obj->spec_attr) ಹಿಂತಿರುಗಿ (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_ತುದಿ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    int32_t ಮಗು_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        ಸ್ಥಿರಾಂಕ lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        ಆದರೆ((lv_obj_is_hidden(ಮಗು) || lv_obj_is_floating(ಮಗು))) ಮುಂದುವರಿ;

        int32_t tmp_y = ಮಗು->coords.y2 + lv_obj_get_style_margin_bottom_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
        ಮಗು_res = LV_MAX(ಮಗು_res, tmp_y);
    }

    int32_t space_ತುದಿ = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಆದರೆ(ಮಗು_res != LV_COORD_MIN) {
        ಮಗು_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_ಎತ್ತರ(obj);
    self_h = self_h - (lv_obj_get_ಎತ್ತರ(obj) - space_ತುದಿ - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    ಹಿಂತಿರುಗಿ LV_MAX(ಮಗು_res, self_h);
}

int32_t lv_obj_get_scroll_ಎಡ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_ತಳ_DIR_RTL) {
        ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
        ಹಿಂತಿರುಗಿ -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_ಬಲ = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t space_ಎಡ = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    int32_t ಮಗು_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        ಸ್ಥಿರಾಂಕ lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        ಆದರೆ((lv_obj_is_hidden(ಮಗು) || lv_obj_is_floating(ಮಗು))) ಮುಂದುವರಿ;

        int32_t tmp_x = ಮಗು->coords.x1 - lv_obj_get_style_margin_ಎಡ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    ಆದರೆ(x1 != LV_COORD_MAX) {
        ಮಗು_res = x1;
        ಮಗು_res = (obj->coords.x1 + space_ಎಡ) - ಮಗು_res;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಮಗು_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_ಅಗಲ(obj);
    self_w = self_w - (lv_obj_get_ಅಗಲ(obj) - space_ಬಲ - space_ಎಡ);
    self_w += lv_obj_get_scroll_x(obj);

    ಹಿಂತಿರುಗಿ LV_MAX(ಮಗು_res, self_w);
}

int32_t lv_obj_get_scroll_ಬಲ(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) == LV_ತಳ_DIR_RTL) {
        ಆದರೆ(obj->spec_attr == ಖಾಲಿ) ಹಿಂತಿರುಗಿ 0;
        ಹಿಂತಿರುಗಿ obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t ಮಗು_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ಮಗು_cnt = lv_obj_get_ಮಗು_ಎಣಿಕೆ(obj);
    ಗಾಗಿ(i = 0; i < ಮಗು_cnt; i++) {
        ಸ್ಥಿರಾಂಕ lv_obj_t * ಮಗು = obj->spec_attr->children[i];
        ಆದರೆ((lv_obj_is_hidden(ಮಗು) || lv_obj_is_floating(ಮಗು))) ಮುಂದುವರಿ;

        int32_t tmp_x = ಮಗು->coords.x2 + lv_obj_get_style_margin_ಬಲ_ಆಂತರಿಕ(ಮಗು, LV_PART_MAIN);
        ಮಗು_res = LV_MAX(ಮಗು_res, tmp_x);
    }

    int32_t space_ಬಲ = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_MAIN);
    int32_t space_ಎಡ = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_MAIN);

    ಆದರೆ(ಮಗು_res != LV_COORD_MIN) {
        ಮಗು_res -= (obj->coords.x2 - space_ಬಲ);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_ಅಗಲ(obj);
    self_w = self_w - (lv_obj_get_ಅಗಲ(obj) - space_ಬಲ - space_ಎಡ);
    self_w -= lv_obj_get_scroll_x(obj);
    ಹಿಂತಿರುಗಿ LV_MAX(ಮಗು_res, self_w);
}

ಶೂನ್ಯ lv_obj_get_scroll_ಕೊನೆ(lv_obj_t * obj, lv_ಬಿಂದು_t * ಕೊನೆ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(ಕೊನೆ != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    ಕೊನೆ->x = a ? -a->ಕೊನೆ_ಮೌಲ್ಯ : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    ಕೊನೆ->y = a ? -a->ಕೊನೆ_ಮೌಲ್ಯ : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

ಶೂನ್ಯ lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(dx == 0 && dy == 0) ಹಿಂತಿರುಗಿ;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_ತಳ_DIR_RTL) {
        ಆದರೆ(x_bounded > 0) x_bounded = 0;
        ಆದರೆ(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ಎಡ(obj) + lv_obj_get_scroll_ಬಲ(obj);
            ಆದರೆ(scroll_max < 0) scroll_max = 0;

            ಆದರೆ(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ(x_bounded < 0) x_bounded = 0;
        ಆದರೆ(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ಎಡ(obj) + lv_obj_get_scroll_ಬಲ(obj);
            ಆದರೆ(scroll_max < 0) scroll_max = 0;

            ಆದರೆ(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    ಆದರೆ(y_bounded > 0) y_bounded = 0;
    ಆದರೆ(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_ತುದಿ(obj) + lv_obj_get_scroll_bottom(obj);
        ಆದರೆ(scroll_max < 0) scroll_max = 0;
        ಆದರೆ(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    ಆದರೆ(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

ಶೂನ್ಯ lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    ಆದರೆ(dx == 0 && dy == 0) ಹಿಂತಿರುಗಿ;
    ಆದರೆ(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_ಗಣ_var(&a, obj);
        lv_anim_ಗಣ_deleted_cb(&a, scroll_ಕೊನೆ_cb);

        ಆದರೆ(dx) {
            uint32_t t = lv_anim_ವೇಗ_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_ಸಮಯ_MIN,
                                               SCROLL_ANIM_ಸಮಯ_MAX);
            lv_anim_ಗಣ_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_ಗಣ_ಮೌಲ್ಯಗಳು(&a, -sx, -sx + dx);
            lv_anim_ಗಣ_exec_cb(&a, scroll_x_anim);
            lv_anim_ಗಣ_ಮಾರ್ಗ_cb(&a, lv_anim_ಮಾರ್ಗ_ease_ಹೊರಹೋಗು);

            lv_ಫಲಿತಾಂಶ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
            lv_anim_ಆರಂಭ(&a);
        }

        ಆದರೆ(dy) {
            uint32_t t = lv_anim_ವೇಗ_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_ಸಮಯ_MIN,
                                               SCROLL_ANIM_ಸಮಯ_MAX);
            lv_anim_ಗಣ_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_ಗಣ_ಮೌಲ್ಯಗಳು(&a, -sy, -sy + dy);
            lv_anim_ಗಣ_exec_cb(&a,  scroll_y_anim);
            lv_anim_ಗಣ_ಮಾರ್ಗ_cb(&a, lv_anim_ಮಾರ್ಗ_ease_ಹೊರಹೋಗು);

            lv_ಫಲಿತಾಂಶ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
            lv_anim_ಆರಂಭ(&a);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_ಫಲಿತಾಂಶ_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, ಖಾಲಿ);
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_ಕೊನೆ, ಖಾಲಿ);
        ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ;
    }
}

ಶೂನ್ಯ lv_obj_scroll_ಗಮ್ಯ(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_scroll_ಗಮ್ಯ_x(obj, x, anim_en);
    lv_obj_scroll_ಗಮ್ಯ_y(obj, y, anim_en);
}

ಶೂನ್ಯ lv_obj_scroll_ಗಮ್ಯ_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

ಶೂನ್ಯ lv_obj_scroll_ಗಮ್ಯ_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

ಶೂನ್ಯ lv_obj_scroll_ಗಮ್ಯ_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ಬಿಂದು_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

ಶೂನ್ಯ lv_obj_scroll_ಗಮ್ಯ_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ಬಿಂದು_t p = {0, 0};
    lv_obj_t * ಮಗು = obj;
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಮಗು);
    ಆಗ(ಪೋಷಕ) {
        scroll_area_into_view(&obj->coords, ಮಗು, &p, anim_en);
        ಮಗು = ಪೋಷಕ;
        ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಪೋಷಕ);
    }
}

lv_ಫಲಿತಾಂಶ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID);

    ಆದರೆ(x == 0 && y == 0) ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;

    ಆದರೆ(!lv_obj_allocate_spec_attr(obj)) {
        ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_ಫಲಿತಾಂಶ_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, ಖಾಲಿ);
    ಆದರೆ(res != LV_ಫಲಿತಾಂಶ_OK) ಹಿಂತಿರುಗಿ res;
    lv_obj_invalidate(obj);
    ಹಿಂತಿರುಗಿ LV_ಫಲಿತಾಂಶ_OK;
}

bool lv_obj_is_scrolling(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ false);

    lv_indev_t * indev = lv_indev_get_ಮುಂದಿನ(ಖಾಲಿ);
    ಆಗ(indev) {
        ಆದರೆ(lv_indev_get_scroll_obj(indev) == obj) ಹಿಂತಿರುಗಿ true;
        indev = lv_indev_get_ಮುಂದಿನ(indev);
    }

    ಆದರೆ(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != ಖಾಲಿ ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ true;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಶೂನ್ಯ lv_obj_stop_scroll_anim(ಸ್ಥಿರಾಂಕ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

ಶೂನ್ಯ lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_obj_update_layout(obj);
    lv_ಬಿಂದು_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    ಆದರೆ(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    ಆದರೆ(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

ಶೂನ್ಯ lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(hor_area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);
    LV_CHECK_ARG(ver_area != ಖಾಲಿ, ಹಿಂತಿರುಗಿ);

    lv_area_ಗಣ(hor_area, 0, 0, -1, -1);
    lv_area_ಗಣ(ver_area, 0, 0, -1, -1);

    ಆದರೆ(lv_obj_is_scrollable(obj) == false) ಹಿಂತಿರುಗಿ;

    lv_scrollbar_ಬಹುಲಕ_t sm = lv_obj_get_scrollbar_ಬಹುಲಕ(obj);
    ಆದರೆ(sm == LV_SCROLLBAR_ಬಹುಲಕ_OFF)  ಹಿಂತಿರುಗಿ;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_ಮುಂದಿನ(ಖಾಲಿ);
    ಆದರೆ(sm == LV_SCROLLBAR_ಬಹುಲಕ_ಸಕ್ರಿಯ) {
        ಆಗ(indev) {
            ಆದರೆ(lv_indev_get_scroll_obj(indev) == obj) ನಿಲ್ಲಿಸು;
            indev = lv_indev_get_ಮುಂದಿನ(indev);
        }
        ಆದರೆ(indev == ಖಾಲಿ)  ಹಿಂತಿರುಗಿ;
    }

    int32_t st = lv_obj_get_scroll_ತುದಿ(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_ಎಡ(obj);
    int32_t sr = lv_obj_get_scroll_ಬಲ(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    ಆದರೆ((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_ಬಹುಲಕ_ON) ||
        (sm == LV_SCROLLBAR_ಬಹುಲಕ_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_ಬಹುಲಕ_ಸಕ್ರಿಯ && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    ಆದರೆ((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_ಬಹುಲಕ_ON) ||
        (sm == LV_SCROLLBAR_ಬಹುಲಕ_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_ಬಹುಲಕ_ಸಕ್ರಿಯ && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    ಆದರೆ(!hor_draw && !ver_draw) ಹಿಂತಿರುಗಿ;

    bool rtl = lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR) == LV_ತಳ_DIR_RTL;

    int32_t ತುದಿ_space = lv_obj_get_style_pad_ತುದಿ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    int32_t ಎಡ_space = lv_obj_get_style_pad_ಎಡ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    int32_t ಬಲ_space = lv_obj_get_style_pad_ಬಲ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_ಅಗಲ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);
    int32_t ಉದ್ದಳತೆ = lv_obj_get_style_ಉದ್ದಳತೆ_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_ಎತ್ತರ(obj);
    int32_t obj_w = lv_obj_get_ಅಗಲ(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    ಆದರೆ(lv_obj_get_style_bg_opa_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_ಆಂತರಿಕ(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        ಹಿಂತಿರುಗಿ;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    ಆದರೆ(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        ಆದರೆ(rtl) {
            ver_area->x1 = obj->coords.x1 + ಎಡ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ver_area->x2 = obj->coords.x2 - ಬಲ_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - ತುದಿ_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(ಉದ್ದಳತೆ > 0 ? ಉದ್ದಳತೆ : sb_h, SCROLLBAR_MIN_ಗಾತ್ರ); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - ತುದಿ_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        ಆದರೆ(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + ತುದಿ_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + ತುದಿ_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            ಆದರೆ(ver_area->y1 < obj->coords.y1 + ತುದಿ_space) {
                ver_area->y1 = obj->coords.y1 + ತುದಿ_space;
                ಆದರೆ(ver_area->y1 + SCROLLBAR_MIN_ಗಾತ್ರ > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_ಗಾತ್ರ;
                }
            }
            ಆದರೆ(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                ಆದರೆ(ver_area->y2 - SCROLLBAR_MIN_ಗಾತ್ರ < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_ಗಾತ್ರ;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    ಆದರೆ(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - ಎಡ_space - ಬಲ_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(ಉದ್ದಳತೆ > 0 ? ಉದ್ದಳತೆ : sb_w, SCROLLBAR_MIN_ಗಾತ್ರ); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - ಎಡ_space - ಬಲ_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        ಆದರೆ(scroll_w <= 0) {
            ಆದರೆ(rtl) {
                hor_area->x1 = obj->coords.x1 + ಎಡ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - ಬಲ_space;
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                hor_area->x1 = obj->coords.x1 + ಎಡ_space;
                hor_area->x2 = obj->coords.x2 - ಬಲ_space - ver_reg_space - 1;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            ಆದರೆ(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + ಎಡ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ಆದರೆ(hor_area->x1 < obj->coords.x1 + ಎಡ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + ಎಡ_space + ver_reg_space;
                    ಆದರೆ(hor_area->x1 + SCROLLBAR_MIN_ಗಾತ್ರ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ಗಾತ್ರ;
                    }
                }
                ಆದರೆ(hor_area->x2 > obj->coords.x2 - ಬಲ_space) {
                    hor_area->x2 = obj->coords.x2 - ಬಲ_space;
                    ಆದರೆ(hor_area->x2 - SCROLLBAR_MIN_ಗಾತ್ರ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ಗಾತ್ರ;
                    }
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                hor_area->x1 = obj->coords.x1 + sb_x + ಎಡ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ಆದರೆ(hor_area->x1 < obj->coords.x1 + ಎಡ_space) {
                    hor_area->x1 = obj->coords.x1 + ಎಡ_space;
                    ಆದರೆ(hor_area->x1 + SCROLLBAR_MIN_ಗಾತ್ರ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ಗಾತ್ರ;
                    }
                }
                ಆದರೆ(hor_area->x2 > obj->coords.x2 - ver_reg_space - ಬಲ_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - ಬಲ_space;
                    ಆದರೆ(hor_area->x2 - SCROLLBAR_MIN_ಗಾತ್ರ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ಗಾತ್ರ;
                    }
                }
            }
        }
    }
}

ಶೂನ್ಯ lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&hor_area) <= 0 && lv_area_get_ಗಾತ್ರ(&ver_area) <= 0) ಹಿಂತಿರುಗಿ;

    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    ಆದರೆ(lv_area_get_ಗಾತ್ರ(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

ಶೂನ್ಯ lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ಹಿಂತಿರುಗಿ);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    ಆದರೆ(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_ತುದಿ(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        ಆದರೆ(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    ಆದರೆ(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_ಎಡ(obj);
        int32_t sr = lv_obj_get_scroll_ಬಲ(obj);
        ಆದರೆ(lv_obj_get_style_ತಳ_dir_ಆಂತರಿಕ(obj, LV_PART_MAIN) != LV_ತಳ_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            ಆದರೆ(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            /*Be sure the right side is not remains scrolled in*/
            ಆದರೆ(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int32_t mul_div(int32_t v, int32_t ಗುಣಕ, int32_t ಭಾಜಕ)
{
    ಹಿಂತಿರುಗಿ ((int64_t)v * ಗುಣಕ) / ಭಾಜಕ;
}

ಸ್ಥಿರ ಶೂನ್ಯ scroll_x_anim(ಶೂನ್ಯ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ scroll_y_anim(ಶೂನ್ಯ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ಸ್ಥಿರ ಶೂನ್ಯ scroll_ಕೊನೆ_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    ಆದರೆ(a->ಆರಂಭ_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_ಕೊನೆ, ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ scroll_area_into_view(ಸ್ಥಿರಾಂಕ lv_area_t * area, lv_obj_t * ಮಗು, lv_ಬಿಂದು_t * scroll_ಮೌಲ್ಯ,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * ಪೋಷಕ = lv_obj_get_ಪೋಷಕ(ಮಗು);
    ಆದರೆ(!lv_obj_is_scrollable(ಪೋಷಕ)) ಹಿಂತಿರುಗಿ;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(ಪೋಷಕ);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ಸ್ಥಿರಾಂಕ lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(ಪೋಷಕ);
    ಆದರೆ(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &ಮಗು->coords;
    ಇಲ್ಲದಿದ್ದರೆ area_tmp = area;

    int32_t stop = lv_obj_get_style_space_ತುದಿ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    int32_t ತುದಿ_diff = ಪೋಷಕ->coords.y1 + stop - area_tmp->y1 - scroll_ಮೌಲ್ಯ->y;
    int32_t bottom_diff = -(ಪೋಷಕ->coords.y2 - sbottom - area_tmp->y2 - scroll_ಮೌಲ್ಯ->y);
    int32_t ಪೋಷಕ_h = lv_obj_get_ಎತ್ತರ(ಪೋಷಕ) - stop - sbottom;
    ಆದರೆ((ತುದಿ_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ತುದಿ_diff > 0) {
        y_scroll = ತುದಿ_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_ತುದಿ(ಪೋಷಕ);
        ಆದರೆ(st - y_scroll < 0) y_scroll = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(ಪೋಷಕ);
        ಆದರೆ(sb + y_scroll < 0) y_scroll = 0;
    }

    ಆಯ್ಕೆ(snap_y) {
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_ಆರಂಭ:
            snap_goal = ಪೋಷಕ->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_ಕೊನೆ:
            snap_goal = ಪೋಷಕ->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_CENTER:
            snap_goal = ಪೋಷಕ->coords.y1 + stop + ಪೋಷಕ_h / 2;
            act = lv_area_get_ಎತ್ತರ(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_NONE:
            ನಿಲ್ಲಿಸು;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(ಪೋಷಕ);
    ಆದರೆ(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &ಮಗು->coords;
    ಇಲ್ಲದಿದ್ದರೆ area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_ಎಡ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_ಬಲ_ಆಂತರಿಕ(ಪೋಷಕ, LV_PART_MAIN);
    int32_t ಎಡ_diff = ಪೋಷಕ->coords.x1 + sleft - area_tmp->x1 - scroll_ಮೌಲ್ಯ->x;
    int32_t ಬಲ_diff = -(ಪೋಷಕ->coords.x2 - sright - area_tmp->x2 - scroll_ಮೌಲ್ಯ->x);
    ಆದರೆ((ಎಡ_diff >= 0 && ಬಲ_diff >= 0)) x_scroll = 0;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಎಡ_diff > 0) {
        x_scroll = ಎಡ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_ಎಡ(ಪೋಷಕ);
        ಆದರೆ(sl - x_scroll < 0) x_scroll = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಬಲ_diff > 0) {
        x_scroll = -ಬಲ_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_ಬಲ(ಪೋಷಕ);
        ಆದರೆ(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t ಪೋಷಕ_w = lv_obj_get_ಅಗಲ(ಪೋಷಕ) - sleft - sright;
    ಆಯ್ಕೆ(snap_x) {
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_ಆರಂಭ:
            snap_goal = ಪೋಷಕ->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_ಕೊನೆ:
            snap_goal = ಪೋಷಕ->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_CENTER:
            snap_goal = ಪೋಷಕ->coords.x1 + sleft + ಪೋಷಕ_w / 2;
            act = lv_area_get_ಅಗಲ(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LV_SCROLL_SNAP_NONE:
            ನಿಲ್ಲಿಸು;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(ಪೋಷಕ, scroll_y_anim);
    lv_anim_delete(ಪೋಷಕ, scroll_x_anim);

    ಆದರೆ((scroll_dir & LV_DIR_ಎಡ) == 0 && x_scroll < 0) x_scroll = 0;
    ಆದರೆ((scroll_dir & LV_DIR_ಬಲ) == 0 && x_scroll > 0) x_scroll = 0;
    ಆದರೆ((scroll_dir & LV_DIR_ತುದಿ) == 0 && y_scroll < 0) y_scroll = 0;
    ಆದರೆ((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_ಮೌಲ್ಯ->x += anim_en ? x_scroll : 0;
    scroll_ಮೌಲ್ಯ->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(ಪೋಷಕ, x_scroll, y_scroll, anim_en);
}
