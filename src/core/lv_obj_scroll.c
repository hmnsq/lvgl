/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "lv_obj_scroll_private.h"
#অন্তর্ভাব "../lvgl_public.h"
#অন্তর্ভাব "../misc/lv_anim_private.h"
#অন্তর্ভাব "lv_obj_private.h"
#অন্তর্ভাব "../indev/lv_indev_scroll.h"
#অন্তর্ভাব "lv_obj_style_internal.h"

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
#define SCROLLBAR_MIN_আকার      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
স্থির অন্তঃরেখ int32_t mul_div(int32_t v, int32_t factor, int32_t ভাজক);
স্থির শূন্য scroll_x_anim(শূন্য * obj, int32_t v);
স্থির শূন্য scroll_y_anim(শূন্য * obj, int32_t v);
স্থির শূন্য scroll_end_cb(lv_anim_t * a);
স্থির শূন্য scroll_area_into_view(ধ্রুবক lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_মান,
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

শূন্য lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }

    যদি(obj->spec_attr->scrollbar_mode == mode) ফেরত;
    obj->spec_attr->scrollbar_mode = mode;
    lv_obj_invalidate(obj);
}

শূন্য lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }

    যদি(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

শূন্য lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }
    obj->spec_attr->scroll_snap_x = align;
}

শূন্য lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত LV_SCROLLBAR_MODE_OFF);

    যদি(obj->spec_attr) ফেরত (lv_scrollbar_mode_t) obj->spec_attr->scrollbar_mode;
    অন্যথা ফেরত LV_SCROLLBAR_MODE_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত LV_DIR_NONE);

    যদি(obj->spec_attr) ফেরত (lv_dir_t) obj->spec_attr->scroll_dir;
    অন্যথা ফেরত LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত LV_SCROLL_SNAP_NONE);

    যদি(obj->spec_attr) ফেরত (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    অন্যথা ফেরত LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত LV_SCROLL_SNAP_NONE);

    যদি(obj->spec_attr) ফেরত (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    অন্যথা ফেরত LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
    ফেরত -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
    ফেরত -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_শীর্ষ(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
    ফেরত -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(obj);
    জন্য(i = 0; i < child_cnt; i++) {
        ধ্রুবক lv_obj_t * child = obj->spec_attr->children[i];
        যদি((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) চালু;

        int32_t tmp_y = child->coords.y2 + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_y);
    }

    int32_t space_শীর্ষ = lv_obj_get_style_space_শীর্ষ_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    যদি(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_height(obj);
    self_h = self_h - (lv_obj_get_height(obj) - space_শীর্ষ - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    ফেরত LV_MAX(child_res, self_h);
}

int32_t lv_obj_get_scroll_বাম(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    যদি(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
        ফেরত -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_ডান = lv_obj_get_style_space_ডান_internal(obj, LV_PART_MAIN);
    int32_t space_বাম = lv_obj_get_style_space_বাম_internal(obj, LV_PART_MAIN);

    int32_t child_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(obj);
    জন্য(i = 0; i < child_cnt; i++) {
        ধ্রুবক lv_obj_t * child = obj->spec_attr->children[i];
        যদি((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) চালু;

        int32_t tmp_x = child->coords.x1 - lv_obj_get_style_margin_বাম_internal(child, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    যদি(x1 != LV_COORD_MAX) {
        child_res = x1;
        child_res = (obj->coords.x1 + space_বাম) - child_res;
    }
    অন্যথা {
        child_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_ডান - space_বাম);
    self_w += lv_obj_get_scroll_x(obj);

    ফেরত LV_MAX(child_res, self_w);
}

int32_t lv_obj_get_scroll_ডান(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    যদি(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        যদি(obj->spec_attr == ফাঁকা) ফেরত 0;
        ফেরত obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_গুনতি(obj);
    জন্য(i = 0; i < child_cnt; i++) {
        ধ্রুবক lv_obj_t * child = obj->spec_attr->children[i];
        যদি((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) চালু;

        int32_t tmp_x = child->coords.x2 + lv_obj_get_style_margin_ডান_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_x);
    }

    int32_t space_ডান = lv_obj_get_style_space_ডান_internal(obj, LV_PART_MAIN);
    int32_t space_বাম = lv_obj_get_style_space_বাম_internal(obj, LV_PART_MAIN);

    যদি(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.x2 - space_ডান);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_ডান - space_বাম);
    self_w -= lv_obj_get_scroll_x(obj);
    ফেরত LV_MAX(child_res, self_w);
}

শূন্য lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);
    LV_CHECK_ARG(end != ফাঁকা, ফেরত);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    end->x = a ? -a->end_মান : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    end->y = a ? -a->end_মান : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

শূন্য lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(dx == 0 && dy == 0) ফেরত;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    যদি(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        যদি(x_bounded > 0) x_bounded = 0;
        যদি(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_বাম(obj) + lv_obj_get_scroll_ডান(obj);
            যদি(scroll_max < 0) scroll_max = 0;

            যদি(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    অন্যথা {
        যদি(x_bounded < 0) x_bounded = 0;
        যদি(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_বাম(obj) + lv_obj_get_scroll_ডান(obj);
            যদি(scroll_max < 0) scroll_max = 0;

            যদি(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    যদি(y_bounded > 0) y_bounded = 0;
    যদি(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_শীর্ষ(obj) + lv_obj_get_scroll_bottom(obj);
        যদি(scroll_max < 0) scroll_max = 0;
        যদি(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    যদি(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

শূন্য lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    যদি(dx == 0 && dy == 0) ফেরত;
    যদি(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_deleted_cb(&a, scroll_end_cb);

        যদি(dx) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_set_মানসমূহ(&a, -sx, -sx + dx);
            lv_anim_set_exec_cb(&a, scroll_x_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_ফলাফল_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            যদি(res != LV_ফলাফল_OK) ফেরত;
            lv_anim_start(&a);
        }

        যদি(dy) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_set_মানসমূহ(&a, -sy, -sy + dy);
            lv_anim_set_exec_cb(&a,  scroll_y_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_ফলাফল_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            যদি(res != LV_ফলাফল_OK) ফেরত;
            lv_anim_start(&a);
        }
    }
    অন্যথা {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_ফলাফল_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, ফাঁকা);
        যদি(res != LV_ফলাফল_OK) ফেরত;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        যদি(res != LV_ফলাফল_OK) ফেরত;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_END, ফাঁকা);
        যদি(res != LV_ফলাফল_OK) ফেরত;
    }
}

শূন্য lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_obj_scroll_to_x(obj, x, anim_en);
    lv_obj_scroll_to_y(obj, y, anim_en);
}

শূন্য lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

শূন্য lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

শূন্য lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

শূন্য lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    lv_obj_t * child = obj;
    lv_obj_t * parent = lv_obj_get_parent(child);
    যখন(parent) {
        scroll_area_into_view(&obj->coords, child, &p, anim_en);
        child = parent;
        parent = lv_obj_get_parent(parent);
    }
}

lv_ফলাফল_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত LV_ফলাফল_INVALID);

    যদি(x == 0 && y == 0) ফেরত LV_ফলাফল_OK;

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত LV_ফলাফল_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_ফলাফল_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, ফাঁকা);
    যদি(res != LV_ফলাফল_OK) ফেরত res;
    lv_obj_invalidate(obj);
    ফেরত LV_ফলাফল_OK;
}

bool lv_obj_is_scrolling(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত false);

    lv_indev_t * indev = lv_indev_get_পরবর্তী(ফাঁকা);
    যখন(indev) {
        যদি(lv_indev_get_scroll_obj(indev) == obj) ফেরত true;
        indev = lv_indev_get_পরবর্তী(indev);
    }

    যদি(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != ফাঁকা ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != ফাঁকা) {
        ফেরত true;
    }

    ফেরত false;
}

শূন্য lv_obj_stop_scroll_anim(ধ্রুবক lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

শূন্য lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_obj_update_layout(obj);
    lv_point_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    যদি(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    যদি(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

শূন্য lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);
    LV_CHECK_ARG(hor_area != ফাঁকা, ফেরত);
    LV_CHECK_ARG(ver_area != ফাঁকা, ফেরত);

    lv_area_set(hor_area, 0, 0, -1, -1);
    lv_area_set(ver_area, 0, 0, -1, -1);

    যদি(lv_obj_is_scrollable(obj) == false) ফেরত;

    lv_scrollbar_mode_t sm = lv_obj_get_scrollbar_mode(obj);
    যদি(sm == LV_SCROLLBAR_MODE_OFF)  ফেরত;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_পরবর্তী(ফাঁকা);
    যদি(sm == LV_SCROLLBAR_MODE_ACTIVE) {
        যখন(indev) {
            যদি(lv_indev_get_scroll_obj(indev) == obj) বিরতি;
            indev = lv_indev_get_পরবর্তী(indev);
        }
        যদি(indev == ফাঁকা)  ফেরত;
    }

    int32_t st = lv_obj_get_scroll_শীর্ষ(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_বাম(obj);
    int32_t sr = lv_obj_get_scroll_ডান(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    যদি((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    যদি((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    যদি(!hor_draw && !ver_draw) ফেরত;

    bool rtl = lv_obj_get_style_base_dir_internal(obj, LV_PART_SCROLLBAR) == LV_BASE_DIR_RTL;

    int32_t শীর্ষ_space = lv_obj_get_style_pad_শীর্ষ_internal(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_internal(obj, LV_PART_SCROLLBAR);
    int32_t বাম_space = lv_obj_get_style_pad_বাম_internal(obj, LV_PART_SCROLLBAR);
    int32_t ডান_space = lv_obj_get_style_pad_ডান_internal(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_width_internal(obj, LV_PART_SCROLLBAR);
    int32_t দৈর্ঘ্য = lv_obj_get_style_দৈর্ঘ্য_internal(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_height(obj);
    int32_t obj_w = lv_obj_get_width(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    যদি(lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        ফেরত;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    যদি(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        যদি(rtl) {
            ver_area->x1 = obj->coords.x1 + বাম_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        অন্যথা {
            ver_area->x2 = obj->coords.x2 - ডান_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - শীর্ষ_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(দৈর্ঘ্য > 0 ? দৈর্ঘ্য : sb_h, SCROLLBAR_MIN_আকার); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - শীর্ষ_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        যদি(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + শীর্ষ_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        অন্যথা {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + শীর্ষ_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            যদি(ver_area->y1 < obj->coords.y1 + শীর্ষ_space) {
                ver_area->y1 = obj->coords.y1 + শীর্ষ_space;
                যদি(ver_area->y1 + SCROLLBAR_MIN_আকার > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_আকার;
                }
            }
            যদি(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                যদি(ver_area->y2 - SCROLLBAR_MIN_আকার < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_আকার;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    যদি(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - বাম_space - ডান_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(দৈর্ঘ্য > 0 ? দৈর্ঘ্য : sb_w, SCROLLBAR_MIN_আকার); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - বাম_space - ডান_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        যদি(scroll_w <= 0) {
            যদি(rtl) {
                hor_area->x1 = obj->coords.x1 + বাম_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - ডান_space;
            }
            অন্যথা {
                hor_area->x1 = obj->coords.x1 + বাম_space;
                hor_area->x2 = obj->coords.x2 - ডান_space - ver_reg_space - 1;
            }
        }
        অন্যথা {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            যদি(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + বাম_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                যদি(hor_area->x1 < obj->coords.x1 + বাম_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + বাম_space + ver_reg_space;
                    যদি(hor_area->x1 + SCROLLBAR_MIN_আকার > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_আকার;
                    }
                }
                যদি(hor_area->x2 > obj->coords.x2 - ডান_space) {
                    hor_area->x2 = obj->coords.x2 - ডান_space;
                    যদি(hor_area->x2 - SCROLLBAR_MIN_আকার < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_আকার;
                    }
                }
            }
            অন্যথা {
                hor_area->x1 = obj->coords.x1 + sb_x + বাম_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                যদি(hor_area->x1 < obj->coords.x1 + বাম_space) {
                    hor_area->x1 = obj->coords.x1 + বাম_space;
                    যদি(hor_area->x1 + SCROLLBAR_MIN_আকার > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_আকার;
                    }
                }
                যদি(hor_area->x2 > obj->coords.x2 - ver_reg_space - ডান_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - ডান_space;
                    যদি(hor_area->x2 - SCROLLBAR_MIN_আকার < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_আকার;
                    }
                }
            }
        }
    }
}

শূন্য lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    যদি(lv_area_get_আকার(&hor_area) <= 0 && lv_area_get_আকার(&ver_area) <= 0) ফেরত;

    যদি(lv_area_get_আকার(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    যদি(lv_area_get_আকার(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

শূন্য lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ফেরত);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    যদি(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_শীর্ষ(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        যদি(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    যদি(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_বাম(obj);
        int32_t sr = lv_obj_get_scroll_ডান(obj);
        যদি(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            যদি(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        অন্যথা {
            /*Be sure the right side is not remains scrolled in*/
            যদি(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির অন্তঃরেখ int32_t mul_div(int32_t v, int32_t factor, int32_t ভাজক)
{
    ফেরত ((int64_t)v * factor) / ভাজক;
}

স্থির শূন্য scroll_x_anim(শূন্য * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

স্থির শূন্য scroll_y_anim(শূন্য * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

স্থির শূন্য scroll_end_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    যদি(a->start_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_END, ফাঁকা);
}

স্থির শূন্য scroll_area_into_view(ধ্রুবক lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_মান,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * parent = lv_obj_get_parent(child);
    যদি(!lv_obj_is_scrollable(parent)) ফেরত;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(parent);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ধ্রুবক lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(parent);
    যদি(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    অন্যথা area_tmp = area;

    int32_t stop = lv_obj_get_style_space_শীর্ষ_internal(parent, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_internal(parent, LV_PART_MAIN);
    int32_t শীর্ষ_diff = parent->coords.y1 + stop - area_tmp->y1 - scroll_মান->y;
    int32_t bottom_diff = -(parent->coords.y2 - sbottom - area_tmp->y2 - scroll_মান->y);
    int32_t parent_h = lv_obj_get_height(parent) - stop - sbottom;
    যদি((শীর্ষ_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    অন্যথা যদি(শীর্ষ_diff > 0) {
        y_scroll = শীর্ষ_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_শীর্ষ(parent);
        যদি(st - y_scroll < 0) y_scroll = 0;
    }
    অন্যথা যদি(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(parent);
        যদি(sb + y_scroll < 0) y_scroll = 0;
    }

    নির্বাচন(snap_y) {
        ক্ষেত্র LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.y1 + stop + parent_h / 2;
            act = lv_area_get_height(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_NONE:
            বিরতি;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(parent);
    যদি(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    অন্যথা area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_বাম_internal(parent, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_ডান_internal(parent, LV_PART_MAIN);
    int32_t বাম_diff = parent->coords.x1 + sleft - area_tmp->x1 - scroll_মান->x;
    int32_t ডান_diff = -(parent->coords.x2 - sright - area_tmp->x2 - scroll_মান->x);
    যদি((বাম_diff >= 0 && ডান_diff >= 0)) x_scroll = 0;
    অন্যথা যদি(বাম_diff > 0) {
        x_scroll = বাম_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_বাম(parent);
        যদি(sl - x_scroll < 0) x_scroll = 0;
    }
    অন্যথা যদি(ডান_diff > 0) {
        x_scroll = -ডান_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_ডান(parent);
        যদি(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t parent_w = lv_obj_get_width(parent) - sleft - sright;
    নির্বাচন(snap_x) {
        ক্ষেত্র LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.x1 + sleft + parent_w / 2;
            act = lv_area_get_width(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            বিরতি;
        ক্ষেত্র LV_SCROLL_SNAP_NONE:
            বিরতি;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(parent, scroll_y_anim);
    lv_anim_delete(parent, scroll_x_anim);

    যদি((scroll_dir & LV_DIR_বাম) == 0 && x_scroll < 0) x_scroll = 0;
    যদি((scroll_dir & LV_DIR_ডান) == 0 && x_scroll > 0) x_scroll = 0;
    যদি((scroll_dir & LV_DIR_শীর্ষ) == 0 && y_scroll < 0) y_scroll = 0;
    যদি((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_মান->x += anim_en ? x_scroll : 0;
    scroll_মান->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(parent, x_scroll, y_scroll, anim_en);
}
