/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj_scroll_private.h"
#include "../lvgl_public.h"
#include "../misc/lv_anim_private.h"
#include "lv_obj_private.h"
#include "../indev/lv_indev_scroll.h"
#include "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#ifndef SCROLL_ANIM_TIME_MIN
    #define SCROLL_ANIM_TIME_MIN    200    /*ms*/
#endif
#ifndef SCROLL_ANIM_TIME_MAX
    #define SCROLL_ANIM_TIME_MAX    400    /*ms*/
#endif
#define SCROLLBAR_MIN_SIZE      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static anta_hstha int32_t mul_div(int32_t v, int32_t factor, int32_t divisor);
static void scroll_x_anim(void * obj, int32_t v);
static void scroll_y_anim(void * obj, int32_t v);
static void scroll_end_cb(lv_anim_t * a);
static void scroll_area_into_view(const lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_value,
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

void lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(!lv_obj_allocate_spec_attr(obj)) {
        ආපසු;
    }

    නම්(obj->spec_attr->scrollbar_mode == mode) ආපසු;
    obj->spec_attr->scrollbar_mode = mode;
    lv_obj_invalidate(obj);
}

void lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(!lv_obj_allocate_spec_attr(obj)) {
        ආපසු;
    }

    නම්(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

void lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(!lv_obj_allocate_spec_attr(obj)) {
        ආපසු;
    }
    obj->spec_attr->scroll_snap_x = align;
}

void lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(!lv_obj_allocate_spec_attr(obj)) {
        ආපසු;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු LV_SCROLLBAR_MODE_OFF);

    නම්(obj->spec_attr) ආපසු (lv_scrollbar_mode_t) obj->spec_attr->scrollbar_mode;
    නැතහොත් ආපසු LV_SCROLLBAR_MODE_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු LV_DIR_NONE);

    නම්(obj->spec_attr) ආපසු (lv_dir_t) obj->spec_attr->scroll_dir;
    නැතහොත් ආපසු LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු LV_SCROLL_SNAP_NONE);

    නම්(obj->spec_attr) ආපසු (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    නැතහොත් ආපසු LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු LV_SCROLL_SNAP_NONE);

    නම්(obj->spec_attr) ආපසු (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    නැතහොත් ආපසු LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    නම්(obj->spec_attr == NULL) ආපසු 0;
    ආපසු -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    නම්(obj->spec_attr == NULL) ආපසු 0;
    ආපසු -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_top(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    නම්(obj->spec_attr == NULL) ආපසු 0;
    ආපසු -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    සඳහා(i = 0; i < child_cnt; i++) {
        const lv_obj_t * child = obj->spec_attr->children[i];
        නම්((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) continue;

        int32_t tmp_y = child->coords.y2 + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_y);
    }

    int32_t space_top = lv_obj_get_style_space_top_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    නම්(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_height(obj);
    self_h = self_h - (lv_obj_get_height(obj) - space_top - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    ආපසු LV_MAX(child_res, self_h);
}

int32_t lv_obj_get_scroll_left(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    නම්(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        නම්(obj->spec_attr == NULL) ආපසු 0;
        ආපසු -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_right = lv_obj_get_style_space_right_internal(obj, LV_PART_MAIN);
    int32_t space_left = lv_obj_get_style_space_left_internal(obj, LV_PART_MAIN);

    int32_t child_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    සඳහා(i = 0; i < child_cnt; i++) {
        const lv_obj_t * child = obj->spec_attr->children[i];
        නම්((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) continue;

        int32_t tmp_x = child->coords.x1 - lv_obj_get_style_margin_left_internal(child, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    නම්(x1 != LV_COORD_MAX) {
        child_res = x1;
        child_res = (obj->coords.x1 + space_left) - child_res;
    }
    නැතහොත් {
        child_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_right - space_left);
    self_w += lv_obj_get_scroll_x(obj);

    ආපසු LV_MAX(child_res, self_w);
}

int32_t lv_obj_get_scroll_right(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    නම්(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        නම්(obj->spec_attr == NULL) ආපසු 0;
        ආපසු obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(obj);
    සඳහා(i = 0; i < child_cnt; i++) {
        const lv_obj_t * child = obj->spec_attr->children[i];
        නම්((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) continue;

        int32_t tmp_x = child->coords.x2 + lv_obj_get_style_margin_right_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_x);
    }

    int32_t space_right = lv_obj_get_style_space_right_internal(obj, LV_PART_MAIN);
    int32_t space_left = lv_obj_get_style_space_left_internal(obj, LV_PART_MAIN);

    නම්(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.x2 - space_right);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_right - space_left);
    self_w -= lv_obj_get_scroll_x(obj);
    ආපසු LV_MAX(child_res, self_w);
}

void lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);
    LV_CHECK_ARG(end != NULL, ආපසු);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    end->x = a ? -a->end_value : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    end->y = a ? -a->end_value : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

void lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(dx == 0 && dy == 0) ආපසු;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    නම්(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        නම්(x_bounded > 0) x_bounded = 0;
        නම්(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_left(obj) + lv_obj_get_scroll_right(obj);
            නම්(scroll_max < 0) scroll_max = 0;

            නම්(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    නැතහොත් {
        නම්(x_bounded < 0) x_bounded = 0;
        නම්(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_left(obj) + lv_obj_get_scroll_right(obj);
            නම්(scroll_max < 0) scroll_max = 0;

            නම්(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    නම්(y_bounded > 0) y_bounded = 0;
    නම්(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_top(obj) + lv_obj_get_scroll_bottom(obj);
        නම්(scroll_max < 0) scroll_max = 0;
        නම්(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    නම්(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

void lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    නම්(dx == 0 && dy == 0) ආපසු;
    නම්(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_deleted_cb(&a, scroll_end_cb);

        නම්(dx) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_set_values(&a, -sx, -sx + dx);
            lv_anim_set_exec_cb(&a, scroll_x_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            නම්(res != LV_RESULT_OK) ආපසු;
            lv_anim_start(&a);
        }

        නම්(dy) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_set_values(&a, -sy, -sy + dy);
            lv_anim_set_exec_cb(&a,  scroll_y_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            නම්(res != LV_RESULT_OK) ආපසු;
            lv_anim_start(&a);
        }
    }
    නැතහොත් {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_result_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, NULL);
        නම්(res != LV_RESULT_OK) ආපසු;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        නම්(res != LV_RESULT_OK) ආපසු;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_END, NULL);
        නම්(res != LV_RESULT_OK) ආපසු;
    }
}

void lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_obj_scroll_to_x(obj, x, anim_en);
    lv_obj_scroll_to_y(obj, y, anim_en);
}

void lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

void lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

void lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

void lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    lv_obj_t * child = obj;
    lv_obj_t * parent = lv_obj_get_parent(child);
    අතරතුර(parent) {
        scroll_area_into_view(&obj->coords, child, &p, anim_en);
        child = parent;
        parent = lv_obj_get_parent(parent);
    }
}

lv_result_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු LV_RESULT_INVALID);

    නම්(x == 0 && y == 0) ආපසු LV_RESULT_OK;

    නම්(!lv_obj_allocate_spec_attr(obj)) {
        ආපසු LV_RESULT_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, NULL);
    නම්(res != LV_RESULT_OK) ආපසු res;
    lv_obj_invalidate(obj);
    ආපසු LV_RESULT_OK;
}

bool lv_obj_is_scrolling(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු false);

    lv_indev_t * indev = lv_indev_get_next(NULL);
    අතරතුර(indev) {
        නම්(lv_indev_get_scroll_obj(indev) == obj) ආපසු true;
        indev = lv_indev_get_next(indev);
    }

    නම්(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != NULL ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != NULL) {
        ආපසු true;
    }

    ආපසු false;
}

void lv_obj_stop_scroll_anim(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

void lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_obj_update_layout(obj);
    lv_point_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    නම්(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    නම්(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

void lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);
    LV_CHECK_ARG(hor_area != NULL, ආපසු);
    LV_CHECK_ARG(ver_area != NULL, ආපසු);

    lv_area_set(hor_area, 0, 0, -1, -1);
    lv_area_set(ver_area, 0, 0, -1, -1);

    නම්(lv_obj_is_scrollable(obj) == false) ආපසු;

    lv_scrollbar_mode_t sm = lv_obj_get_scrollbar_mode(obj);
    නම්(sm == LV_SCROLLBAR_MODE_OFF)  ආපසු;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_next(NULL);
    නම්(sm == LV_SCROLLBAR_MODE_ACTIVE) {
        අතරතුර(indev) {
            නම්(lv_indev_get_scroll_obj(indev) == obj) bha_nga;
            indev = lv_indev_get_next(indev);
        }
        නම්(indev == NULL)  ආපසු;
    }

    int32_t st = lv_obj_get_scroll_top(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_left(obj);
    int32_t sr = lv_obj_get_scroll_right(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    නම්((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    නම්((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    නම්(!hor_draw && !ver_draw) ආපසු;

    bool rtl = lv_obj_get_style_base_dir_internal(obj, LV_PART_SCROLLBAR) == LV_BASE_DIR_RTL;

    int32_t top_space = lv_obj_get_style_pad_top_internal(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_internal(obj, LV_PART_SCROLLBAR);
    int32_t left_space = lv_obj_get_style_pad_left_internal(obj, LV_PART_SCROLLBAR);
    int32_t right_space = lv_obj_get_style_pad_right_internal(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_width_internal(obj, LV_PART_SCROLLBAR);
    int32_t length = lv_obj_get_style_length_internal(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_height(obj);
    int32_t obj_w = lv_obj_get_width(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    නම්(lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        ආපසු;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    නම්(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        නම්(rtl) {
            ver_area->x1 = obj->coords.x1 + left_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        නැතහොත් {
            ver_area->x2 = obj->coords.x2 - right_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - top_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(length > 0 ? length : sb_h, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - top_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        නම්(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + top_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        නැතහොත් {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + top_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            නම්(ver_area->y1 < obj->coords.y1 + top_space) {
                ver_area->y1 = obj->coords.y1 + top_space;
                නම්(ver_area->y1 + SCROLLBAR_MIN_SIZE > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_SIZE;
                }
            }
            නම්(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                නම්(ver_area->y2 - SCROLLBAR_MIN_SIZE < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_SIZE;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    නම්(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - left_space - right_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(length > 0 ? length : sb_w, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - left_space - right_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        නම්(scroll_w <= 0) {
            නම්(rtl) {
                hor_area->x1 = obj->coords.x1 + left_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - right_space;
            }
            නැතහොත් {
                hor_area->x1 = obj->coords.x1 + left_space;
                hor_area->x2 = obj->coords.x2 - right_space - ver_reg_space - 1;
            }
        }
        නැතහොත් {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            නම්(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + left_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                නම්(hor_area->x1 < obj->coords.x1 + left_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + left_space + ver_reg_space;
                    නම්(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                නම්(hor_area->x2 > obj->coords.x2 - right_space) {
                    hor_area->x2 = obj->coords.x2 - right_space;
                    නම්(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
            නැතහොත් {
                hor_area->x1 = obj->coords.x1 + sb_x + left_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                නම්(hor_area->x1 < obj->coords.x1 + left_space) {
                    hor_area->x1 = obj->coords.x1 + left_space;
                    නම්(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                නම්(hor_area->x2 > obj->coords.x2 - ver_reg_space - right_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - right_space;
                    නම්(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
        }
    }
}

void lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    නම්(lv_area_get_size(&hor_area) <= 0 && lv_area_get_size(&ver_area) <= 0) ආපසු;

    නම්(lv_area_get_size(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    නම්(lv_area_get_size(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

void lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ආපසු);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    නම්(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_top(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        නම්(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    නම්(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_left(obj);
        int32_t sr = lv_obj_get_scroll_right(obj);
        නම්(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            නම්(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        නැතහොත් {
            /*Be sure the right side is not remains scrolled in*/
            නම්(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static anta_hstha int32_t mul_div(int32_t v, int32_t factor, int32_t divisor)
{
    ආපසු ((int64_t)v * factor) / divisor;
}

static void scroll_x_anim(void * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

static void scroll_y_anim(void * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

static void scroll_end_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    නම්(a->start_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_END, NULL);
}

static void scroll_area_into_view(const lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_value,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * parent = lv_obj_get_parent(child);
    නම්(!lv_obj_is_scrollable(parent)) ආපසු;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(parent);
    int32_t snap_goal = 0;
    int32_t act = 0;
    const lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(parent);
    නම්(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    නැතහොත් area_tmp = area;

    int32_t stop = lv_obj_get_style_space_top_internal(parent, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_internal(parent, LV_PART_MAIN);
    int32_t top_diff = parent->coords.y1 + stop - area_tmp->y1 - scroll_value->y;
    int32_t bottom_diff = -(parent->coords.y2 - sbottom - area_tmp->y2 - scroll_value->y);
    int32_t parent_h = lv_obj_get_height(parent) - stop - sbottom;
    නම්((top_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    නැතහොත් නම්(top_diff > 0) {
        y_scroll = top_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_top(parent);
        නම්(st - y_scroll < 0) y_scroll = 0;
    }
    නැතහොත් නම්(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(parent);
        නම්(sb + y_scroll < 0) y_scroll = 0;
    }

    switch(snap_y) {
        pak_sa LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.y1 + stop + parent_h / 2;
            act = lv_area_get_height(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_NONE:
            bha_nga;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(parent);
    නම්(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    නැතහොත් area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_left_internal(parent, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_right_internal(parent, LV_PART_MAIN);
    int32_t left_diff = parent->coords.x1 + sleft - area_tmp->x1 - scroll_value->x;
    int32_t right_diff = -(parent->coords.x2 - sright - area_tmp->x2 - scroll_value->x);
    නම්((left_diff >= 0 && right_diff >= 0)) x_scroll = 0;
    නැතහොත් නම්(left_diff > 0) {
        x_scroll = left_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_left(parent);
        නම්(sl - x_scroll < 0) x_scroll = 0;
    }
    නැතහොත් නම්(right_diff > 0) {
        x_scroll = -right_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_right(parent);
        නම්(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t parent_w = lv_obj_get_width(parent) - sleft - sright;
    switch(snap_x) {
        pak_sa LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.x1 + sleft + parent_w / 2;
            act = lv_area_get_width(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            bha_nga;
        pak_sa LV_SCROLL_SNAP_NONE:
            bha_nga;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(parent, scroll_y_anim);
    lv_anim_delete(parent, scroll_x_anim);

    නම්((scroll_dir & LV_DIR_LEFT) == 0 && x_scroll < 0) x_scroll = 0;
    නම්((scroll_dir & LV_DIR_RIGHT) == 0 && x_scroll > 0) x_scroll = 0;
    නම්((scroll_dir & LV_DIR_TOP) == 0 && y_scroll < 0) y_scroll = 0;
    නම්((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_value->x += anim_en ? x_scroll : 0;
    scroll_value->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(parent, x_scroll, y_scroll, anim_en);
}
