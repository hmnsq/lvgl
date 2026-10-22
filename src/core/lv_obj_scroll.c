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
#تعریف_کرو MY_CLASS (&lv_obj_class)
#جے_بنا_تعریف SCROLL_ANIM_ویلا_MIN
    #تعریف_کرو SCROLL_ANIM_ویلا_MIN    200    /*ms*/
#جے_انت
#جے_بنا_تعریف SCROLL_ANIM_ویلا_MAX
    #تعریف_کرو SCROLL_ANIM_ویلا_MAX    400    /*ms*/
#جے_انت
#تعریف_کرو SCROLLBAR_MIN_ناپ      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن مضمن int32_t mul_div(int32_t v, int32_t ضارب, int32_t مقسوم_علیہ);
ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v);
ساكن فراغ scroll_سرا_cb(lv_anim_t * a);
ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * بچہ, lv_نقطہ_t * scroll_قدر,
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

فراغ lv_obj_سیٹ_scrollbar_منوال(lv_obj_t * obj, lv_scrollbar_منوال_t منوال)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(obj->spec_attr->scrollbar_منوال == منوال) أرجع;
    obj->spec_attr->scrollbar_منوال = منوال;
    lv_obj_invalidate(obj);
}

فراغ lv_obj_سیٹ_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

فراغ lv_obj_سیٹ_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    obj->spec_attr->scroll_snap_x = align;
}

فراغ lv_obj_سیٹ_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
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

lv_scrollbar_منوال_t lv_obj_get_scrollbar_منوال(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_SCROLLBAR_منوال_OFF);

    إذا(obj->spec_attr) أرجع (lv_scrollbar_منوال_t) obj->spec_attr->scrollbar_منوال;
    وإلا أرجع LV_SCROLLBAR_منوال_AUTO;
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

int32_t lv_obj_get_scroll_چوٹی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    int32_t بچہ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    لكل(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) استمر;

        int32_t tmp_y = بچہ->coords.y2 + lv_obj_get_style_margin_bottom_اندرونی(بچہ, LV_PART_MAIN);
        بچہ_res = LV_MAX(بچہ_res, tmp_y);
    }

    int32_t space_چوٹی = lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);

    إذا(بچہ_res != LV_COORD_MIN) {
        بچہ_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_اچائی(obj);
    self_h = self_h - (lv_obj_get_اچائی(obj) - space_چوٹی - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    أرجع LV_MAX(بچہ_res, self_h);
}

int32_t lv_obj_get_scroll_بایاں(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    إذا(lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_MAIN) != LV_ادھار_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t space_بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);

    int32_t بچہ_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    لكل(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) استمر;

        int32_t tmp_x = بچہ->coords.x1 - lv_obj_get_style_margin_بایاں_اندرونی(بچہ, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    إذا(x1 != LV_COORD_MAX) {
        بچہ_res = x1;
        بچہ_res = (obj->coords.x1 + space_بایاں) - بچہ_res;
    }
    وإلا {
        بچہ_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_چوڑائی(obj);
    self_w = self_w - (lv_obj_get_چوڑائی(obj) - space_دایاں - space_بایاں);
    self_w += lv_obj_get_scroll_x(obj);

    أرجع LV_MAX(بچہ_res, self_w);
}

int32_t lv_obj_get_scroll_دایاں(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    إذا(lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_MAIN) == LV_ادھار_DIR_RTL) {
        إذا(obj->spec_attr == عدم) أرجع 0;
        أرجع obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t بچہ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    لكل(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        إذا((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) استمر;

        int32_t tmp_x = بچہ->coords.x2 + lv_obj_get_style_margin_دایاں_اندرونی(بچہ, LV_PART_MAIN);
        بچہ_res = LV_MAX(بچہ_res, tmp_x);
    }

    int32_t space_دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t space_بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);

    إذا(بچہ_res != LV_COORD_MIN) {
        بچہ_res -= (obj->coords.x2 - space_دایاں);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_چوڑائی(obj);
    self_w = self_w - (lv_obj_get_چوڑائی(obj) - space_دایاں - space_بایاں);
    self_w -= lv_obj_get_scroll_x(obj);
    أرجع LV_MAX(بچہ_res, self_w);
}

فراغ lv_obj_get_scroll_سرا(lv_obj_t * obj, lv_نقطہ_t * سرا)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(سرا != عدم, أرجع);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    سرا->x = a ? -a->سرا_قدر : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    سرا->y = a ? -a->سرا_قدر : lv_obj_get_scroll_y(obj);
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

    إذا(lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_MAIN) != LV_ادھار_DIR_RTL) {
        إذا(x_bounded > 0) x_bounded = 0;
        إذا(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_بایاں(obj) + lv_obj_get_scroll_دایاں(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    وإلا {
        إذا(x_bounded < 0) x_bounded = 0;
        إذا(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_بایاں(obj) + lv_obj_get_scroll_دایاں(obj);
            إذا(scroll_max < 0) scroll_max = 0;

            إذا(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    إذا(y_bounded > 0) y_bounded = 0;
    إذا(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_چوٹی(obj) + lv_obj_get_scroll_bottom(obj);
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
        lv_anim_سیٹ_var(&a, obj);
        lv_anim_سیٹ_deleted_cb(&a, scroll_سرا_cb);

        إذا(dx) {
            uint32_t t = lv_anim_رفتار_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_ویلا_MIN,
                                               SCROLL_ANIM_ویلا_MAX);
            lv_anim_سیٹ_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_سیٹ_اقدار(&a, -sx, -sx + dx);
            lv_anim_سیٹ_exec_cb(&a, scroll_x_anim);
            lv_anim_سیٹ_رستہ_cb(&a, lv_anim_رستہ_ease_باہرلا);

            lv_نتیجہ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_نتیجہ_OK) أرجع;
            lv_anim_شروع(&a);
        }

        إذا(dy) {
            uint32_t t = lv_anim_رفتار_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_ویلا_MIN,
                                               SCROLL_ANIM_ویلا_MAX);
            lv_anim_سیٹ_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_سیٹ_اقدار(&a, -sy, -sy + dy);
            lv_anim_سیٹ_exec_cb(&a,  scroll_y_anim);
            lv_anim_سیٹ_رستہ_cb(&a, lv_anim_رستہ_ease_باہرلا);

            lv_نتیجہ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            إذا(res != LV_نتیجہ_OK) أرجع;
            lv_anim_شروع(&a);
        }
    }
    وإلا {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_نتیجہ_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, عدم);
        إذا(res != LV_نتیجہ_OK) أرجع;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        إذا(res != LV_نتیجہ_OK) أرجع;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_سرا, عدم);
        إذا(res != LV_نتیجہ_OK) أرجع;
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

    lv_نقطہ_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

فراغ lv_obj_scroll_منزل_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_نقطہ_t p = {0, 0};
    lv_obj_t * بچہ = obj;
    lv_obj_t * ماپا = lv_obj_get_ماپا(بچہ);
    طالما(ماپا) {
        scroll_area_into_view(&obj->coords, بچہ, &p, anim_en);
        بچہ = ماپا;
        ماپا = lv_obj_get_ماپا(ماپا);
    }
}

lv_نتیجہ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع LV_نتیجہ_INVALID);

    إذا(x == 0 && y == 0) أرجع LV_نتیجہ_OK;

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع LV_نتیجہ_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, عدم);
    إذا(res != LV_نتیجہ_OK) أرجع res;
    lv_obj_invalidate(obj);
    أرجع LV_نتیجہ_OK;
}

bool lv_obj_is_scrolling(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع false);

    lv_indev_t * indev = lv_indev_get_اگلا(عدم);
    طالما(indev) {
        إذا(lv_indev_get_scroll_obj(indev) == obj) أرجع true;
        indev = lv_indev_get_اگلا(indev);
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
    lv_نقطہ_t p;
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

    lv_area_سیٹ(hor_area, 0, 0, -1, -1);
    lv_area_سیٹ(ver_area, 0, 0, -1, -1);

    إذا(lv_obj_is_scrollable(obj) == false) أرجع;

    lv_scrollbar_منوال_t sm = lv_obj_get_scrollbar_منوال(obj);
    إذا(sm == LV_SCROLLBAR_منوال_OFF)  أرجع;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_اگلا(عدم);
    إذا(sm == LV_SCROLLBAR_منوال_سرگرم) {
        طالما(indev) {
            إذا(lv_indev_get_scroll_obj(indev) == obj) اكسر;
            indev = lv_indev_get_اگلا(indev);
        }
        إذا(indev == عدم)  أرجع;
    }

    int32_t st = lv_obj_get_scroll_چوٹی(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_بایاں(obj);
    int32_t sr = lv_obj_get_scroll_دایاں(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    إذا((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_منوال_ON) ||
        (sm == LV_SCROLLBAR_منوال_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_منوال_سرگرم && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    إذا((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_منوال_ON) ||
        (sm == LV_SCROLLBAR_منوال_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_منوال_سرگرم && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    إذا(!hor_draw && !ver_draw) أرجع;

    bool rtl = lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_SCROLLBAR) == LV_ادھار_DIR_RTL;

    int32_t چوٹی_space = lv_obj_get_style_pad_چوٹی_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t بایاں_space = lv_obj_get_style_pad_بایاں_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t دایاں_space = lv_obj_get_style_pad_دایاں_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t لمائی = lv_obj_get_style_لمائی_اندرونی(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_اچائی(obj);
    int32_t obj_w = lv_obj_get_چوڑائی(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    إذا(lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_اندرونی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        أرجع;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    إذا(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        إذا(rtl) {
            ver_area->x1 = obj->coords.x1 + بایاں_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        وإلا {
            ver_area->x2 = obj->coords.x2 - دایاں_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - چوٹی_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(لمائی > 0 ? لمائی : sb_h, SCROLLBAR_MIN_ناپ); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - چوٹی_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        إذا(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + چوٹی_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        وإلا {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + چوٹی_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            إذا(ver_area->y1 < obj->coords.y1 + چوٹی_space) {
                ver_area->y1 = obj->coords.y1 + چوٹی_space;
                إذا(ver_area->y1 + SCROLLBAR_MIN_ناپ > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_ناپ;
                }
            }
            إذا(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                إذا(ver_area->y2 - SCROLLBAR_MIN_ناپ < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_ناپ;
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

        int32_t sb_w = mul_div(obj_w - بایاں_space - دایاں_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(لمائی > 0 ? لمائی : sb_w, SCROLLBAR_MIN_ناپ); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - بایاں_space - دایاں_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        إذا(scroll_w <= 0) {
            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + بایاں_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - دایاں_space;
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + بایاں_space;
                hor_area->x2 = obj->coords.x2 - دایاں_space - ver_reg_space - 1;
            }
        }
        وإلا {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            إذا(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + بایاں_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + بایاں_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + بایاں_space + ver_reg_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_ناپ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ناپ;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - دایاں_space) {
                    hor_area->x2 = obj->coords.x2 - دایاں_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_ناپ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ناپ;
                    }
                }
            }
            وإلا {
                hor_area->x1 = obj->coords.x1 + sb_x + بایاں_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                إذا(hor_area->x1 < obj->coords.x1 + بایاں_space) {
                    hor_area->x1 = obj->coords.x1 + بایاں_space;
                    إذا(hor_area->x1 + SCROLLBAR_MIN_ناپ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ناپ;
                    }
                }
                إذا(hor_area->x2 > obj->coords.x2 - ver_reg_space - دایاں_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - دایاں_space;
                    إذا(hor_area->x2 - SCROLLBAR_MIN_ناپ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ناپ;
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

    إذا(lv_area_get_ناپ(&hor_area) <= 0 && lv_area_get_ناپ(&ver_area) <= 0) أرجع;

    إذا(lv_area_get_ناپ(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    إذا(lv_area_get_ناپ(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

فراغ lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    إذا(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_چوٹی(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        إذا(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    إذا(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_بایاں(obj);
        int32_t sr = lv_obj_get_scroll_دایاں(obj);
        إذا(lv_obj_get_style_ادھار_dir_اندرونی(obj, LV_PART_MAIN) != LV_ادھار_DIR_RTL) {
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

ساكن مضمن int32_t mul_div(int32_t v, int32_t ضارب, int32_t مقسوم_علیہ)
{
    أرجع ((int64_t)v * ضارب) / مقسوم_علیہ;
}

ساكن فراغ scroll_x_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ساكن فراغ scroll_y_anim(فراغ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ساكن فراغ scroll_سرا_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    إذا(a->شروع_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_سرا, عدم);
}

ساكن فراغ scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * بچہ, lv_نقطہ_t * scroll_قدر,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * ماپا = lv_obj_get_ماپا(بچہ);
    إذا(!lv_obj_is_scrollable(ماپا)) أرجع;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(ماپا);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ثابت lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(ماپا);
    إذا(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &بچہ->coords;
    وإلا area_tmp = area;

    int32_t stop = lv_obj_get_style_space_چوٹی_اندرونی(ماپا, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_اندرونی(ماپا, LV_PART_MAIN);
    int32_t چوٹی_diff = ماپا->coords.y1 + stop - area_tmp->y1 - scroll_قدر->y;
    int32_t bottom_diff = -(ماپا->coords.y2 - sbottom - area_tmp->y2 - scroll_قدر->y);
    int32_t ماپا_h = lv_obj_get_اچائی(ماپا) - stop - sbottom;
    إذا((چوٹی_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    وإلا إذا(چوٹی_diff > 0) {
        y_scroll = چوٹی_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_چوٹی(ماپا);
        إذا(st - y_scroll < 0) y_scroll = 0;
    }
    وإلا إذا(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(ماپا);
        إذا(sb + y_scroll < 0) y_scroll = 0;
    }

    بدل(snap_y) {
        حالة LV_SCROLL_SNAP_شروع:
            snap_goal = ماپا->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_سرا:
            snap_goal = ماپا->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = ماپا->coords.y1 + stop + ماپا_h / 2;
            act = lv_area_get_اچائی(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(ماپا);
    إذا(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &بچہ->coords;
    وإلا area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_بایاں_اندرونی(ماپا, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_دایاں_اندرونی(ماپا, LV_PART_MAIN);
    int32_t بایاں_diff = ماپا->coords.x1 + sleft - area_tmp->x1 - scroll_قدر->x;
    int32_t دایاں_diff = -(ماپا->coords.x2 - sright - area_tmp->x2 - scroll_قدر->x);
    إذا((بایاں_diff >= 0 && دایاں_diff >= 0)) x_scroll = 0;
    وإلا إذا(بایاں_diff > 0) {
        x_scroll = بایاں_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_بایاں(ماپا);
        إذا(sl - x_scroll < 0) x_scroll = 0;
    }
    وإلا إذا(دایاں_diff > 0) {
        x_scroll = -دایاں_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_دایاں(ماپا);
        إذا(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t ماپا_w = lv_obj_get_چوڑائی(ماپا) - sleft - sright;
    بدل(snap_x) {
        حالة LV_SCROLL_SNAP_شروع:
            snap_goal = ماپا->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_سرا:
            snap_goal = ماپا->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_CENTER:
            snap_goal = ماپا->coords.x1 + sleft + ماپا_w / 2;
            act = lv_area_get_چوڑائی(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            اكسر;
        حالة LV_SCROLL_SNAP_NONE:
            اكسر;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(ماپا, scroll_y_anim);
    lv_anim_delete(ماپا, scroll_x_anim);

    إذا((scroll_dir & LV_DIR_بایاں) == 0 && x_scroll < 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_دایاں) == 0 && x_scroll > 0) x_scroll = 0;
    إذا((scroll_dir & LV_DIR_چوٹی) == 0 && y_scroll < 0) y_scroll = 0;
    إذا((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_قدر->x += anim_en ? x_scroll : 0;
    scroll_قدر->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(ماپا, x_scroll, y_scroll, anim_en);
}
