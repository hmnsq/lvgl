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
#تعریف_کړه MY_CLASS (&lv_obj_class)
#که_نه_تعریف_شوی SCROLL_ANIM_وخت_MIN
    #تعریف_کړه SCROLL_ANIM_وخت_MIN    200    /*ms*/
#د_که_پای
#که_نه_تعریف_شوی SCROLL_ANIM_وخت_MAX
    #تعریف_کړه SCROLL_ANIM_وخت_MAX    400    /*ms*/
#د_که_پای
#تعریف_کړه SCROLLBAR_MIN_اندازه      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن مضمن int32_t mul_div(int32_t v, int32_t ضریب, int32_t وېشونکی);
ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_پای_ټکی_cb(lv_anim_t * a);
ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * ماشوم, lv_ټکی_t * scroll_ارزښت,
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

فراغ lv_obj_سېټ_scrollbar_ډېرپېښ(lv_obj_t * obj, lv_scrollbar_ډېرپېښ_t ډېرپېښ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(obj->spec_attr->scrollbar_ډېرپېښ == ډېرپېښ) أرجع;
    obj->spec_attr->scrollbar_ډېرپېښ = ډېرپېښ;
    lv_obj_invalidate(obj);
}

فراغ lv_obj_سېټ_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

فراغ lv_obj_سېټ_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    obj->spec_attr->scroll_snap_x = align;
}

فراغ lv_obj_سېټ_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
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

lv_scrollbar_ډېرپېښ_t lv_obj_get_scrollbar_ډېرپېښ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_SCROLLBAR_ډېرپېښ_OFF);

    إذا(obj->spec_attr) أرجع (lv_scrollbar_ډېرپېښ_t) obj->spec_attr->scrollbar_ډېرپېښ;
    وإلا أرجع LV_SCROLLBAR_ډېرپېښ_AUTO;
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

int32_t lv_obj_get_scroll_سر(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    int32_t ماشوم_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(obj);
    لكل(i = 0; i < ماشوم_cnt; i++) {
        ثابت lv_obj_t * ماشوم = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(ماشوم) || lv_obj_is_floating(ماشوم))) استمر;

        int32_t tmp_y = ماشوم->coords.y2 + lv_obj_get_style_margin_bottom_دننی(ماشوم, LV_PART_MAIN);
        ماشوم_res = LV_MAX(ماشوم_res, tmp_y);
    }

    int32_t space_سر = lv_obj_get_style_space_سر_دننی(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_دننی(obj, LV_PART_MAIN);

    إذا(ماشوم_res != LV_COORD_MIN) {
        ماشوم_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_لوړوالی(obj);
    self_h = self_h - (lv_obj_get_لوړوالی(obj) - space_سر - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    أرجع LV_MAX(ماشوم_res, self_h);
}

int32_t lv_obj_get_scroll_کیڼ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    إذا(lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_MAIN) != LV_بنسټ_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_ښی = lv_obj_get_style_space_ښی_دننی(obj, LV_PART_MAIN);
    int32_t space_کیڼ = lv_obj_get_style_space_کیڼ_دننی(obj, LV_PART_MAIN);

    int32_t ماشوم_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(obj);
    لكل(i = 0; i < ماشوم_cnt; i++) {
        ثابت lv_obj_t * ماشوم = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(ماشوم) || lv_obj_is_floating(ماشوم))) استمر;

        int32_t tmp_x = ماشوم->coords.x1 - lv_obj_get_style_margin_کیڼ_دننی(ماشوم, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    إذا(x1 != LV_COORD_MAX) {
        ماشوم_res = x1;
        ماشوم_res = (obj->coords.x1 + space_کیڼ) - ماشوم_res;
    }
    وإلا {
        ماشوم_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_پلنوالی(obj);
    self_w = self_w - (lv_obj_get_پلنوالی(obj) - space_ښی - space_کیڼ);
    self_w += lv_obj_get_scroll_x(obj);

    أرجع LV_MAX(ماشوم_res, self_w);
}

int32_t lv_obj_get_scroll_ښی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    إذا(lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_MAIN) == LV_بنسټ_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t ماشوم_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(obj);
    لكل(i = 0; i < ماشوم_cnt; i++) {
        ثابت lv_obj_t * ماشوم = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(ماشوم) || lv_obj_is_floating(ماشوم))) استمر;

        int32_t tmp_x = ماشوم->coords.x2 + lv_obj_get_style_margin_ښی_دننی(ماشوم, LV_PART_MAIN);
        ماشوم_res = LV_MAX(ماشوم_res, tmp_x);
    }

    int32_t space_ښی = lv_obj_get_style_space_ښی_دننی(obj, LV_PART_MAIN);
    int32_t space_کیڼ = lv_obj_get_style_space_کیڼ_دننی(obj, LV_PART_MAIN);

    إذا(ماشوم_res != LV_COORD_MIN) {
        ماشوم_res -= (obj->coords.x2 - space_ښی);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_پلنوالی(obj);
    self_w = self_w - (lv_obj_get_پلنوالی(obj) - space_ښی - space_کیڼ);
    self_w -= lv_obj_get_scroll_x(obj);
    أرجع LV_MAX(ماشوم_res, self_w);
}

فراغ lv_obj_get_scroll_پای_ټکی(lv_obj_t * obj, lv_ټکی_t * پای_ټکی)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(پای_ټکی != عدم, أرجع);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    پای_ټکی->x = a ? -a->پای_ټکی_ارزښت : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    پای_ټکی->y = a ? -a->پای_ټکی_ارزښت : lv_obj_get_scroll_y(obj);
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

    إذا(lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_MAIN) != LV_بنسټ_DIR_RTL) {
        إذا(x_bounded > 0) x_bounded = 0;
        إذا(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_کیڼ(obj) + lv_obj_get_scroll_ښی(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    وإلا {
        إذا(x_bounded < 0) x_bounded = 0;
        إذا(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_کیڼ(obj) + lv_obj_get_scroll_ښی(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    إذا(y_bounded > 0) y_bounded = 0;
    إذا(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_سر(obj) + lv_obj_get_scroll_bottom(obj);
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
        lv_anim_سېټ_var(&a, obj);
        lv_anim_سېټ_deleted_cb(&a, scroll_پای_ټکی_cb);

        إذا(dx) {
            uint32_t t = lv_anim_چټکتیا_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_وخت_MIN,
                                               SCROLL_ANIM_وخت_MAX);
            lv_anim_سېټ_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_سېټ_ارزښتونه(&a, -sx, -sx + dx);
            lv_anim_سېټ_exec_cb(&a, scroll_x_anim);
            lv_anim_سېټ_لار_cb(&a, lv_anim_لار_ease_بهر);

            lv_پایله_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_پایله_OK) أرجع;
            lv_anim_پیل(&a);
        }

        إذا(dy) {
            uint32_t t = lv_anim_چټکتیا_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_وخت_MIN,
                                               SCROLL_ANIM_وخت_MAX);
            lv_anim_سېټ_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_سېټ_ارزښتونه(&a, -sy, -sy + dy);
            lv_anim_سېټ_exec_cb(&a,  scroll_y_anim);
            lv_anim_سېټ_لار_cb(&a, lv_anim_لار_ease_بهر);

            lv_پایله_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_پایله_OK) أرجع;
            lv_anim_پیل(&a);
        }
    }
    وإلا {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_پایله_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, عدم);
        إذا(res != LV_پایله_OK) أرجع;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        إذا(res != LV_پایله_OK) أرجع;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_پای_ټکی, عدم);
        إذا(res != LV_پایله_OK) أرجع;
    }
}

فراغ lv_obj_scroll_منزل(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_scroll_منزل_x(obj, x, anim_en);
    lv_obj_scroll_منزل_y(obj, y, anim_en);
}

فراغ lv_obj_scroll_منزل_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

فراغ lv_obj_scroll_منزل_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

فراغ lv_obj_scroll_منزل_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ټکی_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

فراغ lv_obj_scroll_منزل_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ټکی_t p = {0, 0};
    lv_obj_t * ماشوم = obj;
    lv_obj_t * پلار = lv_obj_get_پلار(ماشوم);
    طالما(پلار) {
        scroll_area_into_view(&obj->coords, ماشوم, &p, anim_en);
        ماشوم = پلار;
        پلار = lv_obj_get_پلار(پلار);
    }
}

lv_پایله_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_پایله_INVALID);

    إذا(x == 0 && y == 0) أرجع LV_پایله_OK;

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع LV_پایله_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_پایله_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, عدم);
    إذا(res != LV_پایله_OK) أرجع res;
    lv_obj_invalidate(obj);
    أرجع LV_پایله_OK;
}

bool lv_obj_is_scrolling(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);

    lv_indev_t * indev = lv_indev_get_بل(عدم);
    طالما(indev) {
        إذا(lv_indev_get_scroll_obj(indev) == obj) أرجع true;
        indev = lv_indev_get_بل(indev);
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
    lv_ټکی_t p;
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

    lv_area_سېټ(hor_area, 0, 0, -1, -1);
    lv_area_سېټ(ver_area, 0, 0, -1, -1);

    إذا(lv_obj_is_scrollable(obj) == false) أرجع;

    lv_scrollbar_ډېرپېښ_t sm = lv_obj_get_scrollbar_ډېرپېښ(obj);
    إذا(sm == LV_SCROLLBAR_ډېرپېښ_OFF)  أرجع;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_بل(عدم);
    إذا(sm == LV_SCROLLBAR_ډېرپېښ_فعال) {
        طالما(indev) {
            إذا(lv_indev_get_scroll_obj(indev) == obj) اكسر;
            indev = lv_indev_get_بل(indev);
        }
        إذا(indev == عدم)  أرجع;
    }

    int32_t st = lv_obj_get_scroll_سر(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_کیڼ(obj);
    int32_t sr = lv_obj_get_scroll_ښی(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    إذا((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_ډېرپېښ_ON) ||
        (sm == LV_SCROLLBAR_ډېرپېښ_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_ډېرپېښ_فعال && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    إذا((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_ډېرپېښ_ON) ||
        (sm == LV_SCROLLBAR_ډېرپېښ_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_ډېرپېښ_فعال && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    إذا(!hor_draw && !ver_draw) أرجع;

    bool rtl = lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_SCROLLBAR) == LV_بنسټ_DIR_RTL;

    int32_t سر_space = lv_obj_get_style_pad_سر_دننی(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_دننی(obj, LV_PART_SCROLLBAR);
    int32_t کیڼ_space = lv_obj_get_style_pad_کیڼ_دننی(obj, LV_PART_SCROLLBAR);
    int32_t ښی_space = lv_obj_get_style_pad_ښی_دننی(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_پلنوالی_دننی(obj, LV_PART_SCROLLBAR);
    int32_t اوږدتیا = lv_obj_get_style_اوږدتیا_دننی(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_لوړوالی(obj);
    int32_t obj_w = lv_obj_get_پلنوالی(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    إذا(lv_obj_get_style_bg_opa_دننی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_دننی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        أرجع;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    إذا(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        إذا(rtl) {
            ver_area->x1 = obj->coords.x1 + کیڼ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        وإلا {
            ver_area->x2 = obj->coords.x2 - ښی_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - سر_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(اوږدتیا > 0 ? اوږدتیا : sb_h, SCROLLBAR_MIN_اندازه); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - سر_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        إذا(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + سر_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        وإلا {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + سر_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            إذا(ver_area->y1 < obj->coords.y1 + سر_space) {
                ver_area->y1 = obj->coords.y1 + سر_space;
                إذا(ver_area->y1 + SCROLLBAR_MIN_اندازه > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_اندازه;
                }
            }
            إذا(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                إذا(ver_area->y2 - SCROLLBAR_MIN_اندازه < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_اندازه;
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

        int32_t sb_w = mul_div(obj_w - کیڼ_space - ښی_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(اوږدتیا > 0 ? اوږدتیا : sb_w, SCROLLBAR_MIN_اندازه); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - کیڼ_space - ښی_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        إذا(scroll_w <= 0) {
            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + کیڼ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - ښی_space;
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + کیڼ_space;
                hor_area->x2 = obj->coords.x2 - ښی_space - ver_reg_space - 1;
            }
        }
        وإلا {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + کیڼ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + کیڼ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + کیڼ_space + ver_reg_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_اندازه > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_اندازه;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - ښی_space) {
                    hor_area->x2 = obj->coords.x2 - ښی_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_اندازه < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_اندازه;
                    }
                }
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + sb_x + کیڼ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + کیڼ_space) {
                    hor_area->x1 = obj->coords.x1 + کیڼ_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_اندازه > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_اندازه;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - ver_reg_space - ښی_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - ښی_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_اندازه < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_اندازه;
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

    إذا(lv_area_get_اندازه(&hor_area) <= 0 && lv_area_get_اندازه(&ver_area) <= 0) أرجع;

    إذا(lv_area_get_اندازه(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    إذا(lv_area_get_اندازه(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

فراغ lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    إذا(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_سر(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        إذا(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    إذا(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_کیڼ(obj);
        int32_t sr = lv_obj_get_scroll_ښی(obj);
        إذا(lv_obj_get_style_بنسټ_dir_دننی(obj, LV_PART_MAIN) != LV_بنسټ_DIR_RTL) {
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

ساكن مضمن int32_t mul_div(int32_t v, int32_t ضریب, int32_t وېشونکی)
{
    أرجع ((int64_t)v * ضریب) / وېشونکی;
}

ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ساكن فراغ scroll_پای_ټکی_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    إذا(a->پیل_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_پای_ټکی, عدم);
}

ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * ماشوم, lv_ټکی_t * scroll_ارزښت,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * پلار = lv_obj_get_پلار(ماشوم);
    إذا(!lv_obj_is_scrollable(پلار)) أرجع;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(پلار);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ثابت lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(پلار);
    إذا(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &ماشوم->coords;
    وإلا area_tmp = area;

    int32_t stop = lv_obj_get_style_space_سر_دننی(پلار, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_دننی(پلار, LV_PART_MAIN);
    int32_t سر_diff = پلار->coords.y1 + stop - area_tmp->y1 - scroll_ارزښت->y;
    int32_t bottom_diff = -(پلار->coords.y2 - sbottom - area_tmp->y2 - scroll_ارزښت->y);
    int32_t پلار_h = lv_obj_get_لوړوالی(پلار) - stop - sbottom;
    إذا((سر_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    وإلا إذا(سر_diff > 0) {
        y_scroll = سر_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_سر(پلار);
        إذا(st - y_scroll < 0) y_scroll = 0;
    }
    وإلا إذا(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(پلار);
        إذا(sb + y_scroll < 0) y_scroll = 0;
    }

    بدل(snap_y) {
        حالة LV_SCROLL_SNAP_پیل:
            snap_goal = پلار->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_پای_ټکی:
            snap_goal = پلار->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = پلار->coords.y1 + stop + پلار_h / 2;
            act = lv_area_get_لوړوالی(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(پلار);
    إذا(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &ماشوم->coords;
    وإلا area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_کیڼ_دننی(پلار, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_ښی_دننی(پلار, LV_PART_MAIN);
    int32_t کیڼ_diff = پلار->coords.x1 + sleft - area_tmp->x1 - scroll_ارزښت->x;
    int32_t ښی_diff = -(پلار->coords.x2 - sright - area_tmp->x2 - scroll_ارزښت->x);
    إذا((کیڼ_diff >= 0 && ښی_diff >= 0)) x_scroll = 0;
    وإلا إذا(کیڼ_diff > 0) {
        x_scroll = کیڼ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_کیڼ(پلار);
        إذا(sl - x_scroll < 0) x_scroll = 0;
    }
    وإلا إذا(ښی_diff > 0) {
        x_scroll = -ښی_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_ښی(پلار);
        إذا(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t پلار_w = lv_obj_get_پلنوالی(پلار) - sleft - sright;
    بدل(snap_x) {
        حالة LV_SCROLL_SNAP_پیل:
            snap_goal = پلار->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_پای_ټکی:
            snap_goal = پلار->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = پلار->coords.x1 + sleft + پلار_w / 2;
            act = lv_area_get_پلنوالی(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(پلار, scroll_y_anim);
    lv_anim_delete(پلار, scroll_x_anim);

    إذا((scroll_dir & LV_DIR_کیڼ) == 0 && x_scroll < 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_ښی) == 0 && x_scroll > 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_سر) == 0 && y_scroll < 0) y_scroll = 0;
    إذا((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_ارزښت->x += anim_en ? x_scroll : 0;
    scroll_ارزښت->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(پلار, x_scroll, y_scroll, anim_en);
}
