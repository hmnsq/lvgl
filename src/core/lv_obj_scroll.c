/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_scroll_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_anim_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../indev/lv_indev_scroll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCROLL_ANIM_ਸਮਾਂ_MIN
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCROLL_ANIM_ਸਮਾਂ_MIN    200    /*ms*/
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCROLL_ANIM_ਸਮਾਂ_MAX
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCROLL_ANIM_ਸਮਾਂ_MAX    400    /*ms*/
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCROLLBAR_MIN_ਮਾਪ      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t mul_div(int32_t v, int32_t ਗੁਣਕ, int32_t ਭਾਜਕ);
ਸਥਿਰ ਖਾਲੀ scroll_x_anim(ਖਾਲੀ * obj, int32_t v);
ਸਥਿਰ ਖਾਲੀ scroll_y_anim(ਖਾਲੀ * obj, int32_t v);
ਸਥਿਰ ਖਾਲੀ scroll_ਸਿਰਾ_cb(lv_anim_t * a);
ਸਥਿਰ ਖਾਲੀ scroll_area_into_view(ਅਚਲ lv_area_t * area, lv_obj_t * ਬੱਚਾ, lv_ਬਿੰਦੂ_t * scroll_ਮੁੱਲ,
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

ਖਾਲੀ lv_obj_ਸੈੱਟ_scrollbar_ਬਹੁਲਕ(lv_obj_t * obj, lv_scrollbar_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }

    ਜੇ(obj->spec_attr->scrollbar_ਬਹੁਲਕ == ਬਹੁਲਕ) ਵਾਪਸ;
    obj->spec_attr->scrollbar_ਬਹੁਲਕ = ਬਹੁਲਕ;
    lv_obj_invalidate(obj);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }

    ਜੇ(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }
    obj->spec_attr->scroll_snap_x = align;
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_ਬਹੁਲਕ_t lv_obj_get_scrollbar_ਬਹੁਲਕ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_SCROLLBAR_ਬਹੁਲਕ_OFF);

    ਜੇ(obj->spec_attr) ਵਾਪਸ (lv_scrollbar_ਬਹੁਲਕ_t) obj->spec_attr->scrollbar_ਬਹੁਲਕ;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_SCROLLBAR_ਬਹੁਲਕ_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_DIR_NONE);

    ਜੇ(obj->spec_attr) ਵਾਪਸ (lv_dir_t) obj->spec_attr->scroll_dir;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_SCROLL_SNAP_NONE);

    ਜੇ(obj->spec_attr) ਵਾਪਸ (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_SCROLL_SNAP_NONE);

    ਜੇ(obj->spec_attr) ਵਾਪਸ (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    ਨਹੀਂਤਾਂ ਵਾਪਸ LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
    ਵਾਪਸ -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
    ਵਾਪਸ -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_ਸਿਖਰ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
    ਵਾਪਸ -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    int32_t ਬੱਚਾ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        ਅਚਲ lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        ਜੇ((lv_obj_is_hidden(ਬੱਚਾ) || lv_obj_is_floating(ਬੱਚਾ))) ਜਾਰੀ;

        int32_t tmp_y = ਬੱਚਾ->coords.y2 + lv_obj_get_style_margin_bottom_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
        ਬੱਚਾ_res = LV_MAX(ਬੱਚਾ_res, tmp_y);
    }

    int32_t space_ਸਿਖਰ = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਜੇ(ਬੱਚਾ_res != LV_COORD_MIN) {
        ਬੱਚਾ_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_ਉਚਾਈ(obj);
    self_h = self_h - (lv_obj_get_ਉਚਾਈ(obj) - space_ਸਿਖਰ - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    ਵਾਪਸ LV_MAX(ਬੱਚਾ_res, self_h);
}

int32_t lv_obj_get_scroll_ਖੱਬਾ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_ਅਧਾਰ_DIR_RTL) {
        ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
        ਵਾਪਸ -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_ਸੱਜਾ = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t space_ਖੱਬਾ = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    int32_t ਬੱਚਾ_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        ਅਚਲ lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        ਜੇ((lv_obj_is_hidden(ਬੱਚਾ) || lv_obj_is_floating(ਬੱਚਾ))) ਜਾਰੀ;

        int32_t tmp_x = ਬੱਚਾ->coords.x1 - lv_obj_get_style_margin_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    ਜੇ(x1 != LV_COORD_MAX) {
        ਬੱਚਾ_res = x1;
        ਬੱਚਾ_res = (obj->coords.x1 + space_ਖੱਬਾ) - ਬੱਚਾ_res;
    }
    ਨਹੀਂਤਾਂ {
        ਬੱਚਾ_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_ਚੌੜਾਈ(obj);
    self_w = self_w - (lv_obj_get_ਚੌੜਾਈ(obj) - space_ਸੱਜਾ - space_ਖੱਬਾ);
    self_w += lv_obj_get_scroll_x(obj);

    ਵਾਪਸ LV_MAX(ਬੱਚਾ_res, self_w);
}

int32_t lv_obj_get_scroll_ਸੱਜਾ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) == LV_ਅਧਾਰ_DIR_RTL) {
        ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
        ਵਾਪਸ obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t ਬੱਚਾ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        ਅਚਲ lv_obj_t * ਬੱਚਾ = obj->spec_attr->children[i];
        ਜੇ((lv_obj_is_hidden(ਬੱਚਾ) || lv_obj_is_floating(ਬੱਚਾ))) ਜਾਰੀ;

        int32_t tmp_x = ਬੱਚਾ->coords.x2 + lv_obj_get_style_margin_ਸੱਜਾ_ਅੰਦਰੂਨੀ(ਬੱਚਾ, LV_PART_MAIN);
        ਬੱਚਾ_res = LV_MAX(ਬੱਚਾ_res, tmp_x);
    }

    int32_t space_ਸੱਜਾ = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);
    int32_t space_ਖੱਬਾ = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN);

    ਜੇ(ਬੱਚਾ_res != LV_COORD_MIN) {
        ਬੱਚਾ_res -= (obj->coords.x2 - space_ਸੱਜਾ);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_ਚੌੜਾਈ(obj);
    self_w = self_w - (lv_obj_get_ਚੌੜਾਈ(obj) - space_ਸੱਜਾ - space_ਖੱਬਾ);
    self_w -= lv_obj_get_scroll_x(obj);
    ਵਾਪਸ LV_MAX(ਬੱਚਾ_res, self_w);
}

ਖਾਲੀ lv_obj_get_scroll_ਸਿਰਾ(lv_obj_t * obj, lv_ਬਿੰਦੂ_t * ਸਿਰਾ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(ਸਿਰਾ != ਨਿਰਾ, ਵਾਪਸ);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    ਸਿਰਾ->x = a ? -a->ਸਿਰਾ_ਮੁੱਲ : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    ਸਿਰਾ->y = a ? -a->ਸਿਰਾ_ਮੁੱਲ : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

ਖਾਲੀ lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(dx == 0 && dy == 0) ਵਾਪਸ;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_ਅਧਾਰ_DIR_RTL) {
        ਜੇ(x_bounded > 0) x_bounded = 0;
        ਜੇ(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ਖੱਬਾ(obj) + lv_obj_get_scroll_ਸੱਜਾ(obj);
            ਜੇ(scroll_max < 0) scroll_max = 0;

            ਜੇ(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    ਨਹੀਂਤਾਂ {
        ਜੇ(x_bounded < 0) x_bounded = 0;
        ਜੇ(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ਖੱਬਾ(obj) + lv_obj_get_scroll_ਸੱਜਾ(obj);
            ਜੇ(scroll_max < 0) scroll_max = 0;

            ਜੇ(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    ਜੇ(y_bounded > 0) y_bounded = 0;
    ਜੇ(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_ਸਿਖਰ(obj) + lv_obj_get_scroll_bottom(obj);
        ਜੇ(scroll_max < 0) scroll_max = 0;
        ਜੇ(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    ਜੇ(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

ਖਾਲੀ lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(dx == 0 && dy == 0) ਵਾਪਸ;
    ਜੇ(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_ਸੈੱਟ_var(&a, obj);
        lv_anim_ਸੈੱਟ_deleted_cb(&a, scroll_ਸਿਰਾ_cb);

        ਜੇ(dx) {
            uint32_t t = lv_anim_ਰਫਤਾਰ_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_ਸਮਾਂ_MIN,
                                               SCROLL_ANIM_ਸਮਾਂ_MAX);
            lv_anim_ਸੈੱਟ_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_ਸੈੱਟ_ਮੁੱਲਾਂ(&a, -sx, -sx + dx);
            lv_anim_ਸੈੱਟ_exec_cb(&a, scroll_x_anim);
            lv_anim_ਸੈੱਟ_ਰਸਤਾ_cb(&a, lv_anim_ਰਸਤਾ_ease_ਬਾਹਰਲਾ);

            lv_ਨਤੀਜਾ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
            lv_anim_ਸ਼ੁਰੂ(&a);
        }

        ਜੇ(dy) {
            uint32_t t = lv_anim_ਰਫਤਾਰ_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_ਸਮਾਂ_MIN,
                                               SCROLL_ANIM_ਸਮਾਂ_MAX);
            lv_anim_ਸੈੱਟ_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_ਸੈੱਟ_ਮੁੱਲਾਂ(&a, -sy, -sy + dy);
            lv_anim_ਸੈੱਟ_exec_cb(&a,  scroll_y_anim);
            lv_anim_ਸੈੱਟ_ਰਸਤਾ_cb(&a, lv_anim_ਰਸਤਾ_ease_ਬਾਹਰਲਾ);

            lv_ਨਤੀਜਾ_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
            lv_anim_ਸ਼ੁਰੂ(&a);
        }
    }
    ਨਹੀਂਤਾਂ {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_ਨਤੀਜਾ_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, ਨਿਰਾ);
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_ਸਿਰਾ, ਨਿਰਾ);
        ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ;
    }
}

ਖਾਲੀ lv_obj_scroll_ਮੰਜ਼ਿਲ(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_scroll_ਮੰਜ਼ਿਲ_x(obj, x, anim_en);
    lv_obj_scroll_ਮੰਜ਼ਿਲ_y(obj, y, anim_en);
}

ਖਾਲੀ lv_obj_scroll_ਮੰਜ਼ਿਲ_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

ਖਾਲੀ lv_obj_scroll_ਮੰਜ਼ਿਲ_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

ਖਾਲੀ lv_obj_scroll_ਮੰਜ਼ਿਲ_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ਬਿੰਦੂ_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

ਖਾਲੀ lv_obj_scroll_ਮੰਜ਼ਿਲ_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_ਬਿੰਦੂ_t p = {0, 0};
    lv_obj_t * ਬੱਚਾ = obj;
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਬੱਚਾ);
    ਜਦੋਂ(ਮਾਪਾ) {
        scroll_area_into_view(&obj->coords, ਬੱਚਾ, &p, anim_en);
        ਬੱਚਾ = ਮਾਪਾ;
        ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਮਾਪਾ);
    }
}

lv_ਨਤੀਜਾ_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID);

    ਜੇ(x == 0 && y == 0) ਵਾਪਸ LV_ਨਤੀਜਾ_OK;

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ LV_ਨਤੀਜਾ_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_ਨਤੀਜਾ_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, ਨਿਰਾ);
    ਜੇ(res != LV_ਨਤੀਜਾ_OK) ਵਾਪਸ res;
    lv_obj_invalidate(obj);
    ਵਾਪਸ LV_ਨਤੀਜਾ_OK;
}

bool lv_obj_is_scrolling(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ false);

    lv_indev_t * indev = lv_indev_get_ਅਗਲਾ(ਨਿਰਾ);
    ਜਦੋਂ(indev) {
        ਜੇ(lv_indev_get_scroll_obj(indev) == obj) ਵਾਪਸ true;
        indev = lv_indev_get_ਅਗਲਾ(indev);
    }

    ਜੇ(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != ਨਿਰਾ ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != ਨਿਰਾ) {
        ਵਾਪਸ true;
    }

    ਵਾਪਸ false;
}

ਖਾਲੀ lv_obj_stop_scroll_anim(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

ਖਾਲੀ lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_update_layout(obj);
    lv_ਬਿੰਦੂ_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    ਜੇ(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    ਜੇ(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

ਖਾਲੀ lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(hor_area != ਨਿਰਾ, ਵਾਪਸ);
    LV_CHECK_ARG(ver_area != ਨਿਰਾ, ਵਾਪਸ);

    lv_area_ਸੈੱਟ(hor_area, 0, 0, -1, -1);
    lv_area_ਸੈੱਟ(ver_area, 0, 0, -1, -1);

    ਜੇ(lv_obj_is_scrollable(obj) == false) ਵਾਪਸ;

    lv_scrollbar_ਬਹੁਲਕ_t sm = lv_obj_get_scrollbar_ਬਹੁਲਕ(obj);
    ਜੇ(sm == LV_SCROLLBAR_ਬਹੁਲਕ_OFF)  ਵਾਪਸ;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_ਅਗਲਾ(ਨਿਰਾ);
    ਜੇ(sm == LV_SCROLLBAR_ਬਹੁਲਕ_ਸਰਗਰਮ) {
        ਜਦੋਂ(indev) {
            ਜੇ(lv_indev_get_scroll_obj(indev) == obj) ਰੋਕੋ;
            indev = lv_indev_get_ਅਗਲਾ(indev);
        }
        ਜੇ(indev == ਨਿਰਾ)  ਵਾਪਸ;
    }

    int32_t st = lv_obj_get_scroll_ਸਿਖਰ(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_ਖੱਬਾ(obj);
    int32_t sr = lv_obj_get_scroll_ਸੱਜਾ(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    ਜੇ((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_ਬਹੁਲਕ_ON) ||
        (sm == LV_SCROLLBAR_ਬਹੁਲਕ_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_ਬਹੁਲਕ_ਸਰਗਰਮ && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    ਜੇ((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_ਬਹੁਲਕ_ON) ||
        (sm == LV_SCROLLBAR_ਬਹੁਲਕ_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_ਬਹੁਲਕ_ਸਰਗਰਮ && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    ਜੇ(!hor_draw && !ver_draw) ਵਾਪਸ;

    bool rtl = lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR) == LV_ਅਧਾਰ_DIR_RTL;

    int32_t ਸਿਖਰ_space = lv_obj_get_style_pad_ਸਿਖਰ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    int32_t ਖੱਬਾ_space = lv_obj_get_style_pad_ਖੱਬਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    int32_t ਸੱਜਾ_space = lv_obj_get_style_pad_ਸੱਜਾ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_ਚੌੜਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);
    int32_t ਲੰਮਾਈ = lv_obj_get_style_ਲੰਮਾਈ_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_ਉਚਾਈ(obj);
    int32_t obj_w = lv_obj_get_ਚੌੜਾਈ(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    ਜੇ(lv_obj_get_style_bg_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_ਅੰਦਰੂਨੀ(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        ਵਾਪਸ;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    ਜੇ(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        ਜੇ(rtl) {
            ver_area->x1 = obj->coords.x1 + ਖੱਬਾ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        ਨਹੀਂਤਾਂ {
            ver_area->x2 = obj->coords.x2 - ਸੱਜਾ_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - ਸਿਖਰ_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(ਲੰਮਾਈ > 0 ? ਲੰਮਾਈ : sb_h, SCROLLBAR_MIN_ਮਾਪ); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - ਸਿਖਰ_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        ਜੇ(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + ਸਿਖਰ_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        ਨਹੀਂਤਾਂ {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + ਸਿਖਰ_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            ਜੇ(ver_area->y1 < obj->coords.y1 + ਸਿਖਰ_space) {
                ver_area->y1 = obj->coords.y1 + ਸਿਖਰ_space;
                ਜੇ(ver_area->y1 + SCROLLBAR_MIN_ਮਾਪ > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_ਮਾਪ;
                }
            }
            ਜੇ(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                ਜੇ(ver_area->y2 - SCROLLBAR_MIN_ਮਾਪ < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_ਮਾਪ;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    ਜੇ(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - ਖੱਬਾ_space - ਸੱਜਾ_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(ਲੰਮਾਈ > 0 ? ਲੰਮਾਈ : sb_w, SCROLLBAR_MIN_ਮਾਪ); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - ਖੱਬਾ_space - ਸੱਜਾ_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        ਜੇ(scroll_w <= 0) {
            ਜੇ(rtl) {
                hor_area->x1 = obj->coords.x1 + ਖੱਬਾ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - ਸੱਜਾ_space;
            }
            ਨਹੀਂਤਾਂ {
                hor_area->x1 = obj->coords.x1 + ਖੱਬਾ_space;
                hor_area->x2 = obj->coords.x2 - ਸੱਜਾ_space - ver_reg_space - 1;
            }
        }
        ਨਹੀਂਤਾਂ {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            ਜੇ(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + ਖੱਬਾ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ਜੇ(hor_area->x1 < obj->coords.x1 + ਖੱਬਾ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + ਖੱਬਾ_space + ver_reg_space;
                    ਜੇ(hor_area->x1 + SCROLLBAR_MIN_ਮਾਪ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ਮਾਪ;
                    }
                }
                ਜੇ(hor_area->x2 > obj->coords.x2 - ਸੱਜਾ_space) {
                    hor_area->x2 = obj->coords.x2 - ਸੱਜਾ_space;
                    ਜੇ(hor_area->x2 - SCROLLBAR_MIN_ਮਾਪ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ਮਾਪ;
                    }
                }
            }
            ਨਹੀਂਤਾਂ {
                hor_area->x1 = obj->coords.x1 + sb_x + ਖੱਬਾ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ਜੇ(hor_area->x1 < obj->coords.x1 + ਖੱਬਾ_space) {
                    hor_area->x1 = obj->coords.x1 + ਖੱਬਾ_space;
                    ਜੇ(hor_area->x1 + SCROLLBAR_MIN_ਮਾਪ > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_ਮਾਪ;
                    }
                }
                ਜੇ(hor_area->x2 > obj->coords.x2 - ver_reg_space - ਸੱਜਾ_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - ਸੱਜਾ_space;
                    ਜੇ(hor_area->x2 - SCROLLBAR_MIN_ਮਾਪ < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_ਮਾਪ;
                    }
                }
            }
        }
    }
}

ਖਾਲੀ lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ਜੇ(lv_area_get_ਮਾਪ(&hor_area) <= 0 && lv_area_get_ਮਾਪ(&ver_area) <= 0) ਵਾਪਸ;

    ਜੇ(lv_area_get_ਮਾਪ(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    ਜੇ(lv_area_get_ਮਾਪ(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

ਖਾਲੀ lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    ਜੇ(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_ਸਿਖਰ(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        ਜੇ(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    ਜੇ(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_ਖੱਬਾ(obj);
        int32_t sr = lv_obj_get_scroll_ਸੱਜਾ(obj);
        ਜੇ(lv_obj_get_style_ਅਧਾਰ_dir_ਅੰਦਰੂਨੀ(obj, LV_PART_MAIN) != LV_ਅਧਾਰ_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            ਜੇ(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        ਨਹੀਂਤਾਂ {
            /*Be sure the right side is not remains scrolled in*/
            ਜੇ(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਸਤਰਅੰਦਰ int32_t mul_div(int32_t v, int32_t ਗੁਣਕ, int32_t ਭਾਜਕ)
{
    ਵਾਪਸ ((int64_t)v * ਗੁਣਕ) / ਭਾਜਕ;
}

ਸਥਿਰ ਖਾਲੀ scroll_x_anim(ਖਾਲੀ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

ਸਥਿਰ ਖਾਲੀ scroll_y_anim(ਖਾਲੀ * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

ਸਥਿਰ ਖਾਲੀ scroll_ਸਿਰਾ_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    ਜੇ(a->ਸ਼ੁਰੂ_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_ਸਿਰਾ, ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ scroll_area_into_view(ਅਚਲ lv_area_t * area, lv_obj_t * ਬੱਚਾ, lv_ਬਿੰਦੂ_t * scroll_ਮੁੱਲ,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(ਬੱਚਾ);
    ਜੇ(!lv_obj_is_scrollable(ਮਾਪਾ)) ਵਾਪਸ;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(ਮਾਪਾ);
    int32_t snap_goal = 0;
    int32_t act = 0;
    ਅਚਲ lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(ਮਾਪਾ);
    ਜੇ(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &ਬੱਚਾ->coords;
    ਨਹੀਂਤਾਂ area_tmp = area;

    int32_t stop = lv_obj_get_style_space_ਸਿਖਰ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    int32_t ਸਿਖਰ_diff = ਮਾਪਾ->coords.y1 + stop - area_tmp->y1 - scroll_ਮੁੱਲ->y;
    int32_t bottom_diff = -(ਮਾਪਾ->coords.y2 - sbottom - area_tmp->y2 - scroll_ਮੁੱਲ->y);
    int32_t ਮਾਪਾ_h = lv_obj_get_ਉਚਾਈ(ਮਾਪਾ) - stop - sbottom;
    ਜੇ((ਸਿਖਰ_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    ਨਹੀਂਤਾਂ ਜੇ(ਸਿਖਰ_diff > 0) {
        y_scroll = ਸਿਖਰ_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_ਸਿਖਰ(ਮਾਪਾ);
        ਜੇ(st - y_scroll < 0) y_scroll = 0;
    }
    ਨਹੀਂਤਾਂ ਜੇ(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(ਮਾਪਾ);
        ਜੇ(sb + y_scroll < 0) y_scroll = 0;
    }

    ਚੋਣ(snap_y) {
        ਹਾਲਤ LV_SCROLL_SNAP_ਸ਼ੁਰੂ:
            snap_goal = ਮਾਪਾ->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_ਸਿਰਾ:
            snap_goal = ਮਾਪਾ->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_CENTER:
            snap_goal = ਮਾਪਾ->coords.y1 + stop + ਮਾਪਾ_h / 2;
            act = lv_area_get_ਉਚਾਈ(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_NONE:
            ਰੋਕੋ;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(ਮਾਪਾ);
    ਜੇ(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &ਬੱਚਾ->coords;
    ਨਹੀਂਤਾਂ area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_ਖੱਬਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_ਸੱਜਾ_ਅੰਦਰੂਨੀ(ਮਾਪਾ, LV_PART_MAIN);
    int32_t ਖੱਬਾ_diff = ਮਾਪਾ->coords.x1 + sleft - area_tmp->x1 - scroll_ਮੁੱਲ->x;
    int32_t ਸੱਜਾ_diff = -(ਮਾਪਾ->coords.x2 - sright - area_tmp->x2 - scroll_ਮੁੱਲ->x);
    ਜੇ((ਖੱਬਾ_diff >= 0 && ਸੱਜਾ_diff >= 0)) x_scroll = 0;
    ਨਹੀਂਤਾਂ ਜੇ(ਖੱਬਾ_diff > 0) {
        x_scroll = ਖੱਬਾ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_ਖੱਬਾ(ਮਾਪਾ);
        ਜੇ(sl - x_scroll < 0) x_scroll = 0;
    }
    ਨਹੀਂਤਾਂ ਜੇ(ਸੱਜਾ_diff > 0) {
        x_scroll = -ਸੱਜਾ_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_ਸੱਜਾ(ਮਾਪਾ);
        ਜੇ(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t ਮਾਪਾ_w = lv_obj_get_ਚੌੜਾਈ(ਮਾਪਾ) - sleft - sright;
    ਚੋਣ(snap_x) {
        ਹਾਲਤ LV_SCROLL_SNAP_ਸ਼ੁਰੂ:
            snap_goal = ਮਾਪਾ->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_ਸਿਰਾ:
            snap_goal = ਮਾਪਾ->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_CENTER:
            snap_goal = ਮਾਪਾ->coords.x1 + sleft + ਮਾਪਾ_w / 2;
            act = lv_area_get_ਚੌੜਾਈ(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ਰੋਕੋ;
        ਹਾਲਤ LV_SCROLL_SNAP_NONE:
            ਰੋਕੋ;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(ਮਾਪਾ, scroll_y_anim);
    lv_anim_delete(ਮਾਪਾ, scroll_x_anim);

    ਜੇ((scroll_dir & LV_DIR_ਖੱਬਾ) == 0 && x_scroll < 0) x_scroll = 0;
    ਜੇ((scroll_dir & LV_DIR_ਸੱਜਾ) == 0 && x_scroll > 0) x_scroll = 0;
    ਜੇ((scroll_dir & LV_DIR_ਸਿਖਰ) == 0 && y_scroll < 0) y_scroll = 0;
    ਜੇ((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_ਮੁੱਲ->x += anim_en ? x_scroll : 0;
    scroll_ਮੁੱਲ->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(ਮਾਪਾ, x_scroll, y_scroll, anim_en);
}
