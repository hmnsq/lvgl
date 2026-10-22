/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_scroll_private.h"
#شامل "../lvgl_public.h"
#شامل "../misc/lv_anim_private.h"
#شامل "lv_obj_private.h"
#شامل "../indev/lv_indev_scroll.h"
#شامل "lv_obj_style_internal.h"

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
ایستا درونخطی int32_t mul_div(int32_t v, int32_t factor, int32_t مقسوم_علیه);
ایستا تهی scroll_x_anim(تهی * obj, int32_t v);
ایستا تهی scroll_y_anim(تهی * obj, int32_t v);
ایستا تهی scroll_end_cb(lv_anim_t * a);
ایستا تهی scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_مقدار,
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

تهی lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }

    اگر(obj->spec_attr->scrollbar_mode == mode) بازگشت;
    obj->spec_attr->scrollbar_mode = mode;
    lv_obj_invalidate(obj);
}

تهی lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }

    اگر(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

تهی lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }
    obj->spec_attr->scroll_snap_x = align;
}

تهی lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت LV_SCROLLBAR_MODE_OFF);

    اگر(obj->spec_attr) بازگشت (lv_scrollbar_mode_t) obj->spec_attr->scrollbar_mode;
    وگرنه بازگشت LV_SCROLLBAR_MODE_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت LV_DIR_NONE);

    اگر(obj->spec_attr) بازگشت (lv_dir_t) obj->spec_attr->scroll_dir;
    وگرنه بازگشت LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت LV_SCROLL_SNAP_NONE);

    اگر(obj->spec_attr) بازگشت (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    وگرنه بازگشت LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت LV_SCROLL_SNAP_NONE);

    اگر(obj->spec_attr) بازگشت (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    وگرنه بازگشت LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    اگر(obj->spec_attr == پوچ) بازگشت 0;
    بازگشت -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    اگر(obj->spec_attr == پوچ) بازگشت 0;
    بازگشت -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_بالاترین(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    اگر(obj->spec_attr == پوچ) بازگشت 0;
    بازگشت -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_شمار(obj);
    برای(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) ادامه;

        int32_t tmp_y = child->coords.y2 + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_y);
    }

    int32_t space_بالاترین = lv_obj_get_style_space_بالاترین_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    اگر(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_height(obj);
    self_h = self_h - (lv_obj_get_height(obj) - space_بالاترین - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    بازگشت LV_MAX(child_res, self_h);
}

int32_t lv_obj_get_scroll_چپ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    اگر(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        اگر(obj->spec_attr == پوچ) بازگشت 0;
        بازگشت -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_راست = lv_obj_get_style_space_راست_internal(obj, LV_PART_MAIN);
    int32_t space_چپ = lv_obj_get_style_space_چپ_internal(obj, LV_PART_MAIN);

    int32_t child_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t child_cnt = lv_obj_get_child_شمار(obj);
    برای(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) ادامه;

        int32_t tmp_x = child->coords.x1 - lv_obj_get_style_margin_چپ_internal(child, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    اگر(x1 != LV_COORD_MAX) {
        child_res = x1;
        child_res = (obj->coords.x1 + space_چپ) - child_res;
    }
    وگرنه {
        child_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_راست - space_چپ);
    self_w += lv_obj_get_scroll_x(obj);

    بازگشت LV_MAX(child_res, self_w);
}

int32_t lv_obj_get_scroll_راست(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    اگر(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        اگر(obj->spec_attr == پوچ) بازگشت 0;
        بازگشت obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_شمار(obj);
    برای(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) ادامه;

        int32_t tmp_x = child->coords.x2 + lv_obj_get_style_margin_راست_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_x);
    }

    int32_t space_راست = lv_obj_get_style_space_راست_internal(obj, LV_PART_MAIN);
    int32_t space_چپ = lv_obj_get_style_space_چپ_internal(obj, LV_PART_MAIN);

    اگر(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.x2 - space_راست);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_راست - space_چپ);
    self_w -= lv_obj_get_scroll_x(obj);
    بازگشت LV_MAX(child_res, self_w);
}

تهی lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);
    LV_CHECK_ARG(end != پوچ, بازگشت);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    end->x = a ? -a->end_مقدار : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    end->y = a ? -a->end_مقدار : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

تهی lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(dx == 0 && dy == 0) بازگشت;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    اگر(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        اگر(x_bounded > 0) x_bounded = 0;
        اگر(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_چپ(obj) + lv_obj_get_scroll_راست(obj);
            اگر(scroll_max < 0) scroll_max = 0;

            اگر(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    وگرنه {
        اگر(x_bounded < 0) x_bounded = 0;
        اگر(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_چپ(obj) + lv_obj_get_scroll_راست(obj);
            اگر(scroll_max < 0) scroll_max = 0;

            اگر(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    اگر(y_bounded > 0) y_bounded = 0;
    اگر(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_بالاترین(obj) + lv_obj_get_scroll_bottom(obj);
        اگر(scroll_max < 0) scroll_max = 0;
        اگر(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    اگر(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

تهی lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    اگر(dx == 0 && dy == 0) بازگشت;
    اگر(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_deleted_cb(&a, scroll_end_cb);

        اگر(dx) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_set_مقادیر(&a, -sx, -sx + dx);
            lv_anim_set_exec_cb(&a, scroll_x_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            اگر(res != LV_RESULT_OK) بازگشت;
            lv_anim_start(&a);
        }

        اگر(dy) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_set_مقادیر(&a, -sy, -sy + dy);
            lv_anim_set_exec_cb(&a,  scroll_y_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            اگر(res != LV_RESULT_OK) بازگشت;
            lv_anim_start(&a);
        }
    }
    وگرنه {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_result_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, پوچ);
        اگر(res != LV_RESULT_OK) بازگشت;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        اگر(res != LV_RESULT_OK) بازگشت;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_END, پوچ);
        اگر(res != LV_RESULT_OK) بازگشت;
    }
}

تهی lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_obj_scroll_to_x(obj, x, anim_en);
    lv_obj_scroll_to_y(obj, y, anim_en);
}

تهی lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

تهی lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

تهی lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

تهی lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    lv_obj_t * child = obj;
    lv_obj_t * parent = lv_obj_get_parent(child);
    وقتی(parent) {
        scroll_area_into_view(&obj->coords, child, &p, anim_en);
        child = parent;
        parent = lv_obj_get_parent(parent);
    }
}

lv_result_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت LV_RESULT_INVALID);

    اگر(x == 0 && y == 0) بازگشت LV_RESULT_OK;

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت LV_RESULT_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, پوچ);
    اگر(res != LV_RESULT_OK) بازگشت res;
    lv_obj_invalidate(obj);
    بازگشت LV_RESULT_OK;
}

bool lv_obj_is_scrolling(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت false);

    lv_indev_t * indev = lv_indev_get_next(پوچ);
    وقتی(indev) {
        اگر(lv_indev_get_scroll_obj(indev) == obj) بازگشت true;
        indev = lv_indev_get_next(indev);
    }

    اگر(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != پوچ ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != پوچ) {
        بازگشت true;
    }

    بازگشت false;
}

تهی lv_obj_stop_scroll_anim(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

تهی lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_obj_update_layout(obj);
    lv_point_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    اگر(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    اگر(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

تهی lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);
    LV_CHECK_ARG(hor_area != پوچ, بازگشت);
    LV_CHECK_ARG(ver_area != پوچ, بازگشت);

    lv_area_set(hor_area, 0, 0, -1, -1);
    lv_area_set(ver_area, 0, 0, -1, -1);

    اگر(lv_obj_is_scrollable(obj) == false) بازگشت;

    lv_scrollbar_mode_t sm = lv_obj_get_scrollbar_mode(obj);
    اگر(sm == LV_SCROLLBAR_MODE_OFF)  بازگشت;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_next(پوچ);
    اگر(sm == LV_SCROLLBAR_MODE_ACTIVE) {
        وقتی(indev) {
            اگر(lv_indev_get_scroll_obj(indev) == obj) بشکن;
            indev = lv_indev_get_next(indev);
        }
        اگر(indev == پوچ)  بازگشت;
    }

    int32_t st = lv_obj_get_scroll_بالاترین(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_چپ(obj);
    int32_t sr = lv_obj_get_scroll_راست(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    اگر((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    اگر((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    اگر(!hor_draw && !ver_draw) بازگشت;

    bool rtl = lv_obj_get_style_base_dir_internal(obj, LV_PART_SCROLLBAR) == LV_BASE_DIR_RTL;

    int32_t بالاترین_space = lv_obj_get_style_pad_بالاترین_internal(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_internal(obj, LV_PART_SCROLLBAR);
    int32_t چپ_space = lv_obj_get_style_pad_چپ_internal(obj, LV_PART_SCROLLBAR);
    int32_t راست_space = lv_obj_get_style_pad_راست_internal(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_width_internal(obj, LV_PART_SCROLLBAR);
    int32_t length = lv_obj_get_style_length_internal(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_height(obj);
    int32_t obj_w = lv_obj_get_width(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    اگر(lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        بازگشت;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    اگر(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        اگر(rtl) {
            ver_area->x1 = obj->coords.x1 + چپ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        وگرنه {
            ver_area->x2 = obj->coords.x2 - راست_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - بالاترین_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(length > 0 ? length : sb_h, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - بالاترین_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        اگر(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + بالاترین_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        وگرنه {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + بالاترین_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            اگر(ver_area->y1 < obj->coords.y1 + بالاترین_space) {
                ver_area->y1 = obj->coords.y1 + بالاترین_space;
                اگر(ver_area->y1 + SCROLLBAR_MIN_SIZE > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_SIZE;
                }
            }
            اگر(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                اگر(ver_area->y2 - SCROLLBAR_MIN_SIZE < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_SIZE;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    اگر(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - چپ_space - راست_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(length > 0 ? length : sb_w, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - چپ_space - راست_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        اگر(scroll_w <= 0) {
            اگر(rtl) {
                hor_area->x1 = obj->coords.x1 + چپ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - راست_space;
            }
            وگرنه {
                hor_area->x1 = obj->coords.x1 + چپ_space;
                hor_area->x2 = obj->coords.x2 - راست_space - ver_reg_space - 1;
            }
        }
        وگرنه {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            اگر(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + چپ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                اگر(hor_area->x1 < obj->coords.x1 + چپ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + چپ_space + ver_reg_space;
                    اگر(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                اگر(hor_area->x2 > obj->coords.x2 - راست_space) {
                    hor_area->x2 = obj->coords.x2 - راست_space;
                    اگر(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
            وگرنه {
                hor_area->x1 = obj->coords.x1 + sb_x + چپ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                اگر(hor_area->x1 < obj->coords.x1 + چپ_space) {
                    hor_area->x1 = obj->coords.x1 + چپ_space;
                    اگر(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                اگر(hor_area->x2 > obj->coords.x2 - ver_reg_space - راست_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - راست_space;
                    اگر(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
        }
    }
}

تهی lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    اگر(lv_area_get_size(&hor_area) <= 0 && lv_area_get_size(&ver_area) <= 0) بازگشت;

    اگر(lv_area_get_size(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    اگر(lv_area_get_size(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

تهی lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, بازگشت);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    اگر(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_بالاترین(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        اگر(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    اگر(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_چپ(obj);
        int32_t sr = lv_obj_get_scroll_راست(obj);
        اگر(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            اگر(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        وگرنه {
            /*Be sure the right side is not remains scrolled in*/
            اگر(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ایستا درونخطی int32_t mul_div(int32_t v, int32_t factor, int32_t مقسوم_علیه)
{
    بازگشت ((int64_t)v * factor) / مقسوم_علیه;
}

ایستا تهی scroll_x_anim(تهی * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ایستا تهی scroll_y_anim(تهی * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ایستا تهی scroll_end_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    اگر(a->start_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_END, پوچ);
}

ایستا تهی scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_مقدار,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * parent = lv_obj_get_parent(child);
    اگر(!lv_obj_is_scrollable(parent)) بازگشت;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(parent);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ثابت lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(parent);
    اگر(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    وگرنه area_tmp = area;

    int32_t stop = lv_obj_get_style_space_بالاترین_internal(parent, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_internal(parent, LV_PART_MAIN);
    int32_t بالاترین_diff = parent->coords.y1 + stop - area_tmp->y1 - scroll_مقدار->y;
    int32_t bottom_diff = -(parent->coords.y2 - sbottom - area_tmp->y2 - scroll_مقدار->y);
    int32_t parent_h = lv_obj_get_height(parent) - stop - sbottom;
    اگر((بالاترین_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    وگرنه اگر(بالاترین_diff > 0) {
        y_scroll = بالاترین_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_بالاترین(parent);
        اگر(st - y_scroll < 0) y_scroll = 0;
    }
    وگرنه اگر(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(parent);
        اگر(sb + y_scroll < 0) y_scroll = 0;
    }

    انتخاب(snap_y) {
        حالت LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.y1 + stop + parent_h / 2;
            act = lv_area_get_height(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_NONE:
            بشکن;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(parent);
    اگر(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    وگرنه area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_چپ_internal(parent, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_راست_internal(parent, LV_PART_MAIN);
    int32_t چپ_diff = parent->coords.x1 + sleft - area_tmp->x1 - scroll_مقدار->x;
    int32_t راست_diff = -(parent->coords.x2 - sright - area_tmp->x2 - scroll_مقدار->x);
    اگر((چپ_diff >= 0 && راست_diff >= 0)) x_scroll = 0;
    وگرنه اگر(چپ_diff > 0) {
        x_scroll = چپ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_چپ(parent);
        اگر(sl - x_scroll < 0) x_scroll = 0;
    }
    وگرنه اگر(راست_diff > 0) {
        x_scroll = -راست_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_راست(parent);
        اگر(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t parent_w = lv_obj_get_width(parent) - sleft - sright;
    انتخاب(snap_x) {
        حالت LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.x1 + sleft + parent_w / 2;
            act = lv_area_get_width(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            بشکن;
        حالت LV_SCROLL_SNAP_NONE:
            بشکن;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(parent, scroll_y_anim);
    lv_anim_delete(parent, scroll_x_anim);

    اگر((scroll_dir & LV_DIR_چپ) == 0 && x_scroll < 0) x_scroll = 0;
    اگر((scroll_dir & LV_DIR_راست) == 0 && x_scroll > 0) x_scroll = 0;
    اگر((scroll_dir & LV_DIR_بالاترین) == 0 && y_scroll < 0) y_scroll = 0;
    اگر((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_مقدار->x += anim_en ? x_scroll : 0;
    scroll_مقدار->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(parent, x_scroll, y_scroll, anim_en);
}
