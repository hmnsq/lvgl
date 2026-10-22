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
#تعریف_کرو MY_CLASS (&lv_obj_class)
#اگر_غیر_تعریف_شدہ SCROLL_ANIM_وقت_MIN
    #تعریف_کرو SCROLL_ANIM_وقت_MIN    200    /*ms*/
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCROLL_ANIM_وقت_MAX
    #تعریف_کرو SCROLL_ANIM_وقت_MAX    400    /*ms*/
#اگر_ختم
#تعریف_کرو SCROLLBAR_MIN_سائز      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساکن اندرونی int32_t mul_div(int32_t v, int32_t ضربی_عدد, int32_t مقسوم_علیہ);
ساکن خالی scroll_x_anim(خالی * obj, int32_t v);
ساکن خالی scroll_y_anim(خالی * obj, int32_t v);
ساکن خالی scroll_انتہا_cb(lv_anim_t * a);
ساکن خالی scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * بچہ, lv_نقطہ_t * scroll_قدر,
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

خالی lv_obj_سیٹ_scrollbar_منوال(lv_obj_t * obj, lv_scrollbar_منوال_t منوال)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }

    اگر(obj->spec_attr->scrollbar_منوال == منوال) واپس;
    obj->spec_attr->scrollbar_منوال = منوال;
    lv_obj_invalidate(obj);
}

خالی lv_obj_سیٹ_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }

    اگر(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

خالی lv_obj_سیٹ_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }
    obj->spec_attr->scroll_snap_x = align;
}

خالی lv_obj_سیٹ_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_منوال_t lv_obj_get_scrollbar_منوال(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_SCROLLBAR_منوال_OFF);

    اگر(obj->spec_attr) واپس (lv_scrollbar_منوال_t) obj->spec_attr->scrollbar_منوال;
    ورنہ واپس LV_SCROLLBAR_منوال_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_DIR_NONE);

    اگر(obj->spec_attr) واپس (lv_dir_t) obj->spec_attr->scroll_dir;
    ورنہ واپس LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_SCROLL_SNAP_NONE);

    اگر(obj->spec_attr) واپس (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    ورنہ واپس LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_SCROLL_SNAP_NONE);

    اگر(obj->spec_attr) واپس (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    ورنہ واپس LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    اگر(obj->spec_attr == تہی) واپس 0;
    واپس -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    اگر(obj->spec_attr == تہی) واپس 0;
    واپس -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_چوٹی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    اگر(obj->spec_attr == تہی) واپس 0;
    واپس -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    int32_t بچہ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    برائے(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) جاری;

        int32_t tmp_y = بچہ->coords.y2 + lv_obj_get_style_margin_bottom_اندرونی(بچہ, LV_PART_MAIN);
        بچہ_res = LV_MAX(بچہ_res, tmp_y);
    }

    int32_t space_چوٹی = lv_obj_get_style_space_چوٹی_اندرونی(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_اندرونی(obj, LV_PART_MAIN);

    اگر(بچہ_res != LV_COORD_MIN) {
        بچہ_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_اونچائی(obj);
    self_h = self_h - (lv_obj_get_اونچائی(obj) - space_چوٹی - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    واپس LV_MAX(بچہ_res, self_h);
}

int32_t lv_obj_get_scroll_بایاں(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) != LV_بنیاد_DIR_RTL) {
        اگر(obj->spec_attr == تہی) واپس 0;
        واپس -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t space_بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);

    int32_t بچہ_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    برائے(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) جاری;

        int32_t tmp_x = بچہ->coords.x1 - lv_obj_get_style_margin_بایاں_اندرونی(بچہ, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    اگر(x1 != LV_COORD_MAX) {
        بچہ_res = x1;
        بچہ_res = (obj->coords.x1 + space_بایاں) - بچہ_res;
    }
    ورنہ {
        بچہ_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_چوڑائی(obj);
    self_w = self_w - (lv_obj_get_چوڑائی(obj) - space_دایاں - space_بایاں);
    self_w += lv_obj_get_scroll_x(obj);

    واپس LV_MAX(بچہ_res, self_w);
}

int32_t lv_obj_get_scroll_دایاں(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) == LV_بنیاد_DIR_RTL) {
        اگر(obj->spec_attr == تہی) واپس 0;
        واپس obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t بچہ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(obj);
    برائے(i = 0; i < بچہ_cnt; i++) {
        ثابت lv_obj_t * بچہ = obj->spec_attr->children[i];
        اگر((lv_obj_is_hidden(بچہ) || lv_obj_is_floating(بچہ))) جاری;

        int32_t tmp_x = بچہ->coords.x2 + lv_obj_get_style_margin_دایاں_اندرونی(بچہ, LV_PART_MAIN);
        بچہ_res = LV_MAX(بچہ_res, tmp_x);
    }

    int32_t space_دایاں = lv_obj_get_style_space_دایاں_اندرونی(obj, LV_PART_MAIN);
    int32_t space_بایاں = lv_obj_get_style_space_بایاں_اندرونی(obj, LV_PART_MAIN);

    اگر(بچہ_res != LV_COORD_MIN) {
        بچہ_res -= (obj->coords.x2 - space_دایاں);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_چوڑائی(obj);
    self_w = self_w - (lv_obj_get_چوڑائی(obj) - space_دایاں - space_بایاں);
    self_w -= lv_obj_get_scroll_x(obj);
    واپس LV_MAX(بچہ_res, self_w);
}

خالی lv_obj_get_scroll_انتہا(lv_obj_t * obj, lv_نقطہ_t * انتہا)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(انتہا != تہی, واپس);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    انتہا->x = a ? -a->انتہا_قدر : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    انتہا->y = a ? -a->انتہا_قدر : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

خالی lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(dx == 0 && dy == 0) واپس;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) != LV_بنیاد_DIR_RTL) {
        اگر(x_bounded > 0) x_bounded = 0;
        اگر(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_بایاں(obj) + lv_obj_get_scroll_دایاں(obj);
            اگر(scroll_max < 0) scroll_max = 0;

            اگر(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    ورنہ {
        اگر(x_bounded < 0) x_bounded = 0;
        اگر(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_بایاں(obj) + lv_obj_get_scroll_دایاں(obj);
            اگر(scroll_max < 0) scroll_max = 0;

            اگر(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    اگر(y_bounded > 0) y_bounded = 0;
    اگر(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_چوٹی(obj) + lv_obj_get_scroll_bottom(obj);
        اگر(scroll_max < 0) scroll_max = 0;
        اگر(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    اگر(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

خالی lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(dx == 0 && dy == 0) واپس;
    اگر(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_سیٹ_var(&a, obj);
        lv_anim_سیٹ_deleted_cb(&a, scroll_انتہا_cb);

        اگر(dx) {
            uint32_t t = lv_anim_رفتار_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_وقت_MIN,
                                               SCROLL_ANIM_وقت_MAX);
            lv_anim_سیٹ_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_سیٹ_اقدار(&a, -sx, -sx + dx);
            lv_anim_سیٹ_exec_cb(&a, scroll_x_anim);
            lv_anim_سیٹ_راستہ_cb(&a, lv_anim_راستہ_ease_برآمد);

            lv_نتیجہ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            اگر(res != LV_نتیجہ_OK) واپس;
            lv_anim_آغاز(&a);
        }

        اگر(dy) {
            uint32_t t = lv_anim_رفتار_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_وقت_MIN,
                                               SCROLL_ANIM_وقت_MAX);
            lv_anim_سیٹ_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_سیٹ_اقدار(&a, -sy, -sy + dy);
            lv_anim_سیٹ_exec_cb(&a,  scroll_y_anim);
            lv_anim_سیٹ_راستہ_cb(&a, lv_anim_راستہ_ease_برآمد);

            lv_نتیجہ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            اگر(res != LV_نتیجہ_OK) واپس;
            lv_anim_آغاز(&a);
        }
    }
    ورنہ {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_نتیجہ_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, تہی);
        اگر(res != LV_نتیجہ_OK) واپس;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        اگر(res != LV_نتیجہ_OK) واپس;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_انتہا, تہی);
        اگر(res != LV_نتیجہ_OK) واپس;
    }
}

خالی lv_obj_scroll_مقصد(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_scroll_مقصد_x(obj, x, anim_en);
    lv_obj_scroll_مقصد_y(obj, y, anim_en);
}

خالی lv_obj_scroll_مقصد_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

خالی lv_obj_scroll_مقصد_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

خالی lv_obj_scroll_مقصد_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_نقطہ_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

خالی lv_obj_scroll_مقصد_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_نقطہ_t p = {0, 0};
    lv_obj_t * بچہ = obj;
    lv_obj_t * والد = lv_obj_get_والد(بچہ);
    جب(والد) {
        scroll_area_into_view(&obj->coords, بچہ, &p, anim_en);
        بچہ = والد;
        والد = lv_obj_get_والد(والد);
    }
}

lv_نتیجہ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس LV_نتیجہ_INVALID);

    اگر(x == 0 && y == 0) واپس LV_نتیجہ_OK;

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس LV_نتیجہ_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, تہی);
    اگر(res != LV_نتیجہ_OK) واپس res;
    lv_obj_invalidate(obj);
    واپس LV_نتیجہ_OK;
}

bool lv_obj_is_scrolling(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس false);

    lv_indev_t * indev = lv_indev_get_اگلا(تہی);
    جب(indev) {
        اگر(lv_indev_get_scroll_obj(indev) == obj) واپس true;
        indev = lv_indev_get_اگلا(indev);
    }

    اگر(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != تہی ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != تہی) {
        واپس true;
    }

    واپس false;
}

خالی lv_obj_stop_scroll_anim(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

خالی lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_update_layout(obj);
    lv_نقطہ_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    اگر(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    اگر(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

خالی lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(hor_area != تہی, واپس);
    LV_CHECK_ARG(ver_area != تہی, واپس);

    lv_area_سیٹ(hor_area, 0, 0, -1, -1);
    lv_area_سیٹ(ver_area, 0, 0, -1, -1);

    اگر(lv_obj_is_scrollable(obj) == false) واپس;

    lv_scrollbar_منوال_t sm = lv_obj_get_scrollbar_منوال(obj);
    اگر(sm == LV_SCROLLBAR_منوال_OFF)  واپس;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_اگلا(تہی);
    اگر(sm == LV_SCROLLBAR_منوال_فعال) {
        جب(indev) {
            اگر(lv_indev_get_scroll_obj(indev) == obj) توڑو;
            indev = lv_indev_get_اگلا(indev);
        }
        اگر(indev == تہی)  واپس;
    }

    int32_t st = lv_obj_get_scroll_چوٹی(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_بایاں(obj);
    int32_t sr = lv_obj_get_scroll_دایاں(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    اگر((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_منوال_ON) ||
        (sm == LV_SCROLLBAR_منوال_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_منوال_فعال && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    اگر((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_منوال_ON) ||
        (sm == LV_SCROLLBAR_منوال_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_منوال_فعال && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    اگر(!hor_draw && !ver_draw) واپس;

    bool rtl = lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_SCROLLBAR) == LV_بنیاد_DIR_RTL;

    int32_t چوٹی_space = lv_obj_get_style_pad_چوٹی_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t بایاں_space = lv_obj_get_style_pad_بایاں_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t دایاں_space = lv_obj_get_style_pad_دایاں_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_چوڑائی_اندرونی(obj, LV_PART_SCROLLBAR);
    int32_t لمبائی = lv_obj_get_style_لمبائی_اندرونی(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_اونچائی(obj);
    int32_t obj_w = lv_obj_get_چوڑائی(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    اگر(lv_obj_get_style_bg_opa_اندرونی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_اندرونی(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        واپس;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    اگر(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        اگر(rtl) {
            ver_area->x1 = obj->coords.x1 + بایاں_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        ورنہ {
            ver_area->x2 = obj->coords.x2 - دایاں_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - چوٹی_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(لمبائی > 0 ? لمبائی : sb_h, SCROLLBAR_MIN_سائز); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - چوٹی_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        اگر(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + چوٹی_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        ورنہ {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + چوٹی_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            اگر(ver_area->y1 < obj->coords.y1 + چوٹی_space) {
                ver_area->y1 = obj->coords.y1 + چوٹی_space;
                اگر(ver_area->y1 + SCROLLBAR_MIN_سائز > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_سائز;
                }
            }
            اگر(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                اگر(ver_area->y2 - SCROLLBAR_MIN_سائز < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_سائز;
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

        int32_t sb_w = mul_div(obj_w - بایاں_space - دایاں_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(لمبائی > 0 ? لمبائی : sb_w, SCROLLBAR_MIN_سائز); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - بایاں_space - دایاں_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        اگر(scroll_w <= 0) {
            اگر(rtl) {
                hor_area->x1 = obj->coords.x1 + بایاں_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - دایاں_space;
            }
            ورنہ {
                hor_area->x1 = obj->coords.x1 + بایاں_space;
                hor_area->x2 = obj->coords.x2 - دایاں_space - ver_reg_space - 1;
            }
        }
        ورنہ {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            اگر(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + بایاں_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                اگر(hor_area->x1 < obj->coords.x1 + بایاں_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + بایاں_space + ver_reg_space;
                    اگر(hor_area->x1 + SCROLLBAR_MIN_سائز > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_سائز;
                    }
                }
                اگر(hor_area->x2 > obj->coords.x2 - دایاں_space) {
                    hor_area->x2 = obj->coords.x2 - دایاں_space;
                    اگر(hor_area->x2 - SCROLLBAR_MIN_سائز < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_سائز;
                    }
                }
            }
            ورنہ {
                hor_area->x1 = obj->coords.x1 + sb_x + بایاں_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                اگر(hor_area->x1 < obj->coords.x1 + بایاں_space) {
                    hor_area->x1 = obj->coords.x1 + بایاں_space;
                    اگر(hor_area->x1 + SCROLLBAR_MIN_سائز > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_سائز;
                    }
                }
                اگر(hor_area->x2 > obj->coords.x2 - ver_reg_space - دایاں_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - دایاں_space;
                    اگر(hor_area->x2 - SCROLLBAR_MIN_سائز < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_سائز;
                    }
                }
            }
        }
    }
}

خالی lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    اگر(lv_area_get_سائز(&hor_area) <= 0 && lv_area_get_سائز(&ver_area) <= 0) واپس;

    اگر(lv_area_get_سائز(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    اگر(lv_area_get_سائز(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

خالی lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    اگر(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_چوٹی(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        اگر(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    اگر(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_بایاں(obj);
        int32_t sr = lv_obj_get_scroll_دایاں(obj);
        اگر(lv_obj_get_style_بنیاد_dir_اندرونی(obj, LV_PART_MAIN) != LV_بنیاد_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            اگر(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        ورنہ {
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

ساکن اندرونی int32_t mul_div(int32_t v, int32_t ضربی_عدد, int32_t مقسوم_علیہ)
{
    واپس ((int64_t)v * ضربی_عدد) / مقسوم_علیہ;
}

ساکن خالی scroll_x_anim(خالی * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ساکن خالی scroll_y_anim(خالی * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ساکن خالی scroll_انتہا_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    اگر(a->آغاز_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_انتہا, تہی);
}

ساکن خالی scroll_area_into_view(ثابت lv_area_t * area, lv_obj_t * بچہ, lv_نقطہ_t * scroll_قدر,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * والد = lv_obj_get_والد(بچہ);
    اگر(!lv_obj_is_scrollable(والد)) واپس;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(والد);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ثابت lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(والد);
    اگر(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &بچہ->coords;
    ورنہ area_tmp = area;

    int32_t stop = lv_obj_get_style_space_چوٹی_اندرونی(والد, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_اندرونی(والد, LV_PART_MAIN);
    int32_t چوٹی_diff = والد->coords.y1 + stop - area_tmp->y1 - scroll_قدر->y;
    int32_t bottom_diff = -(والد->coords.y2 - sbottom - area_tmp->y2 - scroll_قدر->y);
    int32_t والد_h = lv_obj_get_اونچائی(والد) - stop - sbottom;
    اگر((چوٹی_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    ورنہ اگر(چوٹی_diff > 0) {
        y_scroll = چوٹی_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_چوٹی(والد);
        اگر(st - y_scroll < 0) y_scroll = 0;
    }
    ورنہ اگر(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(والد);
        اگر(sb + y_scroll < 0) y_scroll = 0;
    }

    انتخاب(snap_y) {
        حالت LV_SCROLL_SNAP_آغاز:
            snap_goal = والد->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_انتہا:
            snap_goal = والد->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_CENTER:
            snap_goal = والد->coords.y1 + stop + والد_h / 2;
            act = lv_area_get_اونچائی(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_NONE:
            توڑو;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(والد);
    اگر(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &بچہ->coords;
    ورنہ area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_بایاں_اندرونی(والد, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_دایاں_اندرونی(والد, LV_PART_MAIN);
    int32_t بایاں_diff = والد->coords.x1 + sleft - area_tmp->x1 - scroll_قدر->x;
    int32_t دایاں_diff = -(والد->coords.x2 - sright - area_tmp->x2 - scroll_قدر->x);
    اگر((بایاں_diff >= 0 && دایاں_diff >= 0)) x_scroll = 0;
    ورنہ اگر(بایاں_diff > 0) {
        x_scroll = بایاں_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_بایاں(والد);
        اگر(sl - x_scroll < 0) x_scroll = 0;
    }
    ورنہ اگر(دایاں_diff > 0) {
        x_scroll = -دایاں_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_دایاں(والد);
        اگر(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t والد_w = lv_obj_get_چوڑائی(والد) - sleft - sright;
    انتخاب(snap_x) {
        حالت LV_SCROLL_SNAP_آغاز:
            snap_goal = والد->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_انتہا:
            snap_goal = والد->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_CENTER:
            snap_goal = والد->coords.x1 + sleft + والد_w / 2;
            act = lv_area_get_چوڑائی(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            توڑو;
        حالت LV_SCROLL_SNAP_NONE:
            توڑو;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(والد, scroll_y_anim);
    lv_anim_delete(والد, scroll_x_anim);

    اگر((scroll_dir & LV_DIR_بایاں) == 0 && x_scroll < 0) x_scroll = 0;
    اگر((scroll_dir & LV_DIR_دایاں) == 0 && x_scroll > 0) x_scroll = 0;
    اگر((scroll_dir & LV_DIR_چوٹی) == 0 && y_scroll < 0) y_scroll = 0;
    اگر((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_قدر->x += anim_en ? x_scroll : 0;
    scroll_قدر->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(والد, x_scroll, y_scroll, anim_en);
}
