/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_scroll_private.h"
#ضمن "../lvgl_public.h"
#ضمن "../misc/lv_anim_private.h"
#ضمن "lv_obj_private.h"
#ضمن "../indev/lv_indev_scroll.h"
#ضمن "lv_obj_style_internal.h"

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
ساكن مضمن int32_t mul_div(int32_t v, int32_t factor, int32_t مي_ربو);
ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_end_cb(lv_anim_t * a);
ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_درج,
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

فراغ lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(obj->spec_attr->scrollbar_mode == mode) أرجع;
    obj->spec_attr->scrollbar_mode = mode;
    lv_obj_invalidate(obj);
}

فراغ lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

فراغ lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    obj->spec_attr->scroll_snap_x = align;
}

فراغ lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_SCROLLBAR_MODE_OFF);

    إذا(obj->spec_attr) أرجع (lv_scrollbar_mode_t) obj->spec_attr->scrollbar_mode;
    وإلا أرجع LV_SCROLLBAR_MODE_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_DIR_NONE);

    إذا(obj->spec_attr) أرجع (lv_dir_t) obj->spec_attr->scroll_dir;
    وإلا أرجع LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_SCROLL_SNAP_NONE);

    إذا(obj->spec_attr) أرجع (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    وإلا أرجع LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_SCROLL_SNAP_NONE);

    إذا(obj->spec_attr) أرجع (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    وإلا أرجع LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_كولي(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_كدي(obj);
    لكل(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) استمر;

        int32_t tmp_y = child->coords.y2 + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_y);
    }

    int32_t space_كولي = lv_obj_get_style_space_كولي_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    إذا(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_height(obj);
    self_h = self_h - (lv_obj_get_height(obj) - space_كولي - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    أرجع LV_MAX(child_res, self_h);
}

int32_t lv_obj_get_scroll_هغو(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    إذا(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_دام = lv_obj_get_style_space_دام_internal(obj, LV_PART_MAIN);
    int32_t space_هغو = lv_obj_get_style_space_هغو_internal(obj, LV_PART_MAIN);

    int32_t child_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t child_cnt = lv_obj_get_child_كدي(obj);
    لكل(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) استمر;

        int32_t tmp_x = child->coords.x1 - lv_obj_get_style_margin_هغو_internal(child, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    إذا(x1 != LV_COORD_MAX) {
        child_res = x1;
        child_res = (obj->coords.x1 + space_هغو) - child_res;
    }
    وإلا {
        child_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_دام - space_هغو);
    self_w += lv_obj_get_scroll_x(obj);

    أرجع LV_MAX(child_res, self_w);
}

int32_t lv_obj_get_scroll_دام(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    إذا(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_كدي(obj);
    لكل(i = 0; i < child_cnt; i++) {
        ثابت lv_obj_t * child = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) استمر;

        int32_t tmp_x = child->coords.x2 + lv_obj_get_style_margin_دام_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_x);
    }

    int32_t space_دام = lv_obj_get_style_space_دام_internal(obj, LV_PART_MAIN);
    int32_t space_هغو = lv_obj_get_style_space_هغو_internal(obj, LV_PART_MAIN);

    إذا(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.x2 - space_دام);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_دام - space_هغو);
    self_w -= lv_obj_get_scroll_x(obj);
    أرجع LV_MAX(child_res, self_w);
}

فراغ lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(end != عدم, أرجع);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    end->x = a ? -a->end_درج : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    end->y = a ? -a->end_درج : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

فراغ lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(dx == 0 && dy == 0) أرجع;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    إذا(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        إذا(x_bounded > 0) x_bounded = 0;
        إذا(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_هغو(obj) + lv_obj_get_scroll_دام(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    وإلا {
        إذا(x_bounded < 0) x_bounded = 0;
        إذا(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_هغو(obj) + lv_obj_get_scroll_دام(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    إذا(y_bounded > 0) y_bounded = 0;
    إذا(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_كولي(obj) + lv_obj_get_scroll_bottom(obj);
        إذا(scroll_max < 0) scroll_max = 0;
        إذا(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    إذا(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

فراغ lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(dx == 0 && dy == 0) أرجع;
    إذا(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_deleted_cb(&a, scroll_end_cb);

        إذا(dx) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_set_درجوجي(&a, -sx, -sx + dx);
            lv_anim_set_exec_cb(&a, scroll_x_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_RESULT_OK) أرجع;
            lv_anim_start(&a);
        }

        إذا(dy) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_set_درجوجي(&a, -sy, -sy + dy);
            lv_anim_set_exec_cb(&a,  scroll_y_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_result_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_RESULT_OK) أرجع;
            lv_anim_start(&a);
        }
    }
    وإلا {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_result_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, عدم);
        إذا(res != LV_RESULT_OK) أرجع;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        إذا(res != LV_RESULT_OK) أرجع;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_END, عدم);
        إذا(res != LV_RESULT_OK) أرجع;
    }
}

فراغ lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_scroll_to_x(obj, x, anim_en);
    lv_obj_scroll_to_y(obj, y, anim_en);
}

فراغ lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

فراغ lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

فراغ lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

فراغ lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    lv_obj_t * child = obj;
    lv_obj_t * parent = lv_obj_get_parent(child);
    طالما(parent) {
        scroll_area_into_view(&obj->coords, child, &p, anim_en);
        child = parent;
        parent = lv_obj_get_parent(parent);
    }
}

lv_result_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_RESULT_INVALID);

    إذا(x == 0 && y == 0) أرجع LV_RESULT_OK;

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع LV_RESULT_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, عدم);
    إذا(res != LV_RESULT_OK) أرجع res;
    lv_obj_invalidate(obj);
    أرجع LV_RESULT_OK;
}

bool lv_obj_is_scrolling(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);

    lv_indev_t * indev = lv_indev_get_next(عدم);
    طالما(indev) {
        إذا(lv_indev_get_scroll_obj(indev) == obj) أرجع true;
        indev = lv_indev_get_next(indev);
    }

    إذا(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != عدم ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != عدم) {
        أرجع true;
    }

    أرجع false;
}

فراغ lv_obj_stop_scroll_anim(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

فراغ lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_update_layout(obj);
    lv_point_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    إذا(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    إذا(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

فراغ lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(hor_area != عدم, أرجع);
    LV_CHECK_ARG(ver_area != عدم, أرجع);

    lv_area_set(hor_area, 0, 0, -1, -1);
    lv_area_set(ver_area, 0, 0, -1, -1);

    إذا(lv_obj_is_scrollable(obj) == false) أرجع;

    lv_scrollbar_mode_t sm = lv_obj_get_scrollbar_mode(obj);
    إذا(sm == LV_SCROLLBAR_MODE_OFF)  أرجع;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_next(عدم);
    إذا(sm == LV_SCROLLBAR_MODE_ACTIVE) {
        طالما(indev) {
            إذا(lv_indev_get_scroll_obj(indev) == obj) اكسر;
            indev = lv_indev_get_next(indev);
        }
        إذا(indev == عدم)  أرجع;
    }

    int32_t st = lv_obj_get_scroll_كولي(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_هغو(obj);
    int32_t sr = lv_obj_get_scroll_دام(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    إذا((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    إذا((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    إذا(!hor_draw && !ver_draw) أرجع;

    bool rtl = lv_obj_get_style_base_dir_internal(obj, LV_PART_SCROLLBAR) == LV_BASE_DIR_RTL;

    int32_t كولي_space = lv_obj_get_style_pad_كولي_internal(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_internal(obj, LV_PART_SCROLLBAR);
    int32_t هغو_space = lv_obj_get_style_pad_هغو_internal(obj, LV_PART_SCROLLBAR);
    int32_t دام_space = lv_obj_get_style_pad_دام_internal(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_width_internal(obj, LV_PART_SCROLLBAR);
    int32_t length = lv_obj_get_style_length_internal(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_height(obj);
    int32_t obj_w = lv_obj_get_width(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    إذا(lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        أرجع;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    إذا(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        إذا(rtl) {
            ver_area->x1 = obj->coords.x1 + هغو_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        وإلا {
            ver_area->x2 = obj->coords.x2 - دام_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - كولي_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(length > 0 ? length : sb_h, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - كولي_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        إذا(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + كولي_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        وإلا {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + كولي_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            إذا(ver_area->y1 < obj->coords.y1 + كولي_space) {
                ver_area->y1 = obj->coords.y1 + كولي_space;
                إذا(ver_area->y1 + SCROLLBAR_MIN_SIZE > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_SIZE;
                }
            }
            إذا(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                إذا(ver_area->y2 - SCROLLBAR_MIN_SIZE < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_SIZE;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    إذا(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - هغو_space - دام_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(length > 0 ? length : sb_w, SCROLLBAR_MIN_SIZE); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - هغو_space - دام_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        إذا(scroll_w <= 0) {
            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + هغو_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - دام_space;
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + هغو_space;
                hor_area->x2 = obj->coords.x2 - دام_space - ver_reg_space - 1;
            }
        }
        وإلا {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + هغو_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + هغو_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + هغو_space + ver_reg_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - دام_space) {
                    hor_area->x2 = obj->coords.x2 - دام_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + sb_x + هغو_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + هغو_space) {
                    hor_area->x1 = obj->coords.x1 + هغو_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_SIZE > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_SIZE;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - ver_reg_space - دام_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - دام_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_SIZE < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_SIZE;
                    }
                }
            }
        }
    }
}

فراغ lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    إذا(lv_area_get_size(&hor_area) <= 0 && lv_area_get_size(&ver_area) <= 0) أرجع;

    إذا(lv_area_get_size(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    إذا(lv_area_get_size(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

فراغ lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    إذا(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_كولي(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        إذا(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    إذا(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_هغو(obj);
        int32_t sr = lv_obj_get_scroll_دام(obj);
        إذا(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            إذا(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        وإلا {
            /*Be sure the right side is not remains scrolled in*/
            إذا(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن مضمن int32_t mul_div(int32_t v, int32_t factor, int32_t مي_ربو)
{
    أرجع ((int64_t)v * factor) / مي_ربو;
}

ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ساكن فراغ scroll_end_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    إذا(a->start_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_END, عدم);
}

ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_درج,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * parent = lv_obj_get_parent(child);
    إذا(!lv_obj_is_scrollable(parent)) أرجع;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(parent);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ثابت lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(parent);
    إذا(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    وإلا area_tmp = area;

    int32_t stop = lv_obj_get_style_space_كولي_internal(parent, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_internal(parent, LV_PART_MAIN);
    int32_t كولي_diff = parent->coords.y1 + stop - area_tmp->y1 - scroll_درج->y;
    int32_t bottom_diff = -(parent->coords.y2 - sbottom - area_tmp->y2 - scroll_درج->y);
    int32_t parent_h = lv_obj_get_height(parent) - stop - sbottom;
    إذا((كولي_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    وإلا إذا(كولي_diff > 0) {
        y_scroll = كولي_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_كولي(parent);
        إذا(st - y_scroll < 0) y_scroll = 0;
    }
    وإلا إذا(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(parent);
        إذا(sb + y_scroll < 0) y_scroll = 0;
    }

    بدل(snap_y) {
        حالة LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.y1 + stop + parent_h / 2;
            act = lv_area_get_height(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(parent);
    إذا(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    وإلا area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_هغو_internal(parent, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_دام_internal(parent, LV_PART_MAIN);
    int32_t هغو_diff = parent->coords.x1 + sleft - area_tmp->x1 - scroll_درج->x;
    int32_t دام_diff = -(parent->coords.x2 - sright - area_tmp->x2 - scroll_درج->x);
    إذا((هغو_diff >= 0 && دام_diff >= 0)) x_scroll = 0;
    وإلا إذا(هغو_diff > 0) {
        x_scroll = هغو_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_هغو(parent);
        إذا(sl - x_scroll < 0) x_scroll = 0;
    }
    وإلا إذا(دام_diff > 0) {
        x_scroll = -دام_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_دام(parent);
        إذا(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t parent_w = lv_obj_get_width(parent) - sleft - sright;
    بدل(snap_x) {
        حالة LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.x1 + sleft + parent_w / 2;
            act = lv_area_get_width(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(parent, scroll_y_anim);
    lv_anim_delete(parent, scroll_x_anim);

    إذا((scroll_dir & LV_DIR_هغو) == 0 && x_scroll < 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_دام) == 0 && x_scroll > 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_كولي) == 0 && y_scroll < 0) y_scroll = 0;
    إذا((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_درج->x += anim_en ? x_scroll : 0;
    scroll_درج->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(parent, x_scroll, y_scroll, anim_en);
}
