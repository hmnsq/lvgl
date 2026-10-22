/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_scroll_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "../misc/lv_anim_private.h"
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../indev/lv_indev_scroll.h"
#అన్తర్భావ "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)
#నిర్వచితం_కాకపోతే SCROLL_ANIM_సమయం_MIN
    #నిర్వచించు SCROLL_ANIM_సమయం_MIN    200    /*ms*/
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCROLL_ANIM_సమయం_MAX
    #నిర్వచించు SCROLL_ANIM_సమయం_MAX    400    /*ms*/
#అయితే_ముగింపు
#నిర్వచించు SCROLLBAR_MIN_కొలత      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర అంతఃవరుస int32_t mul_div(int32_t v, int32_t గుణకం, int32_t భాజకం);
స్థిర శూన్య scroll_x_anim(శూన్య * obj, int32_t v);
స్థిర శూన్య scroll_y_anim(శూన్య * obj, int32_t v);
స్థిర శూన్య scroll_చివర_cb(lv_anim_t * a);
స్థిర శూన్య scroll_area_into_view(స్థిరరాశి lv_area_t * area, lv_obj_t * బిడ్డ, lv_బిందువు_t * scroll_విలువ,
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

శూన్య lv_obj_సమితి_scrollbar_బహుళకం(lv_obj_t * obj, lv_scrollbar_బహుళకం_t బహుళకం)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }

    ఒకవేళ(obj->spec_attr->scrollbar_బహుళకం == బహుళకం) తిరిగి;
    obj->spec_attr->scrollbar_బహుళకం = బహుళకం;
    lv_obj_invalidate(obj);
}

శూన్య lv_obj_సమితి_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }

    ఒకవేళ(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

శూన్య lv_obj_సమితి_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }
    obj->spec_attr->scroll_snap_x = align;
}

శూన్య lv_obj_సమితి_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_బహుళకం_t lv_obj_get_scrollbar_బహుళకం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_SCROLLBAR_బహుళకం_OFF);

    ఒకవేళ(obj->spec_attr) తిరిగి (lv_scrollbar_బహుళకం_t) obj->spec_attr->scrollbar_బహుళకం;
    లేకపోతే తిరిగి LV_SCROLLBAR_బహుళకం_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_DIR_NONE);

    ఒకవేళ(obj->spec_attr) తిరిగి (lv_dir_t) obj->spec_attr->scroll_dir;
    లేకపోతే తిరిగి LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_SCROLL_SNAP_NONE);

    ఒకవేళ(obj->spec_attr) తిరిగి (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    లేకపోతే తిరిగి LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_SCROLL_SNAP_NONE);

    ఒకవేళ(obj->spec_attr) తిరిగి (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    లేకపోతే తిరిగి LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
    తిరిగి -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
    తిరిగి -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_శిఖరం(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
    తిరిగి -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    int32_t బిడ్డ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        స్థిరరాశి lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        ఒకవేళ((lv_obj_is_hidden(బిడ్డ) || lv_obj_is_floating(బిడ్డ))) కొనసాగు;

        int32_t tmp_y = బిడ్డ->coords.y2 + lv_obj_get_style_margin_bottom_అంతర్గత(బిడ్డ, LV_PART_MAIN);
        బిడ్డ_res = LV_MAX(బిడ్డ_res, tmp_y);
    }

    int32_t space_శిఖరం = lv_obj_get_style_space_శిఖరం_అంతర్గత(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_అంతర్గత(obj, LV_PART_MAIN);

    ఒకవేళ(బిడ్డ_res != LV_COORD_MIN) {
        బిడ్డ_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_ఎత్తు(obj);
    self_h = self_h - (lv_obj_get_ఎత్తు(obj) - space_శిఖరం - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    తిరిగి LV_MAX(బిడ్డ_res, self_h);
}

int32_t lv_obj_get_scroll_ఎడమ(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) != LV_పునాది_DIR_RTL) {
        ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
        తిరిగి -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_కుడి = lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);
    int32_t space_ఎడమ = lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);

    int32_t బిడ్డ_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        స్థిరరాశి lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        ఒకవేళ((lv_obj_is_hidden(బిడ్డ) || lv_obj_is_floating(బిడ్డ))) కొనసాగు;

        int32_t tmp_x = బిడ్డ->coords.x1 - lv_obj_get_style_margin_ఎడమ_అంతర్గత(బిడ్డ, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    ఒకవేళ(x1 != LV_COORD_MAX) {
        బిడ్డ_res = x1;
        బిడ్డ_res = (obj->coords.x1 + space_ఎడమ) - బిడ్డ_res;
    }
    లేకపోతే {
        బిడ్డ_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_వెడల్పు(obj);
    self_w = self_w - (lv_obj_get_వెడల్పు(obj) - space_కుడి - space_ఎడమ);
    self_w += lv_obj_get_scroll_x(obj);

    తిరిగి LV_MAX(బిడ్డ_res, self_w);
}

int32_t lv_obj_get_scroll_కుడి(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) == LV_పునాది_DIR_RTL) {
        ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
        తిరిగి obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t బిడ్డ_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        స్థిరరాశి lv_obj_t * బిడ్డ = obj->spec_attr->children[i];
        ఒకవేళ((lv_obj_is_hidden(బిడ్డ) || lv_obj_is_floating(బిడ్డ))) కొనసాగు;

        int32_t tmp_x = బిడ్డ->coords.x2 + lv_obj_get_style_margin_కుడి_అంతర్గత(బిడ్డ, LV_PART_MAIN);
        బిడ్డ_res = LV_MAX(బిడ్డ_res, tmp_x);
    }

    int32_t space_కుడి = lv_obj_get_style_space_కుడి_అంతర్గత(obj, LV_PART_MAIN);
    int32_t space_ఎడమ = lv_obj_get_style_space_ఎడమ_అంతర్గత(obj, LV_PART_MAIN);

    ఒకవేళ(బిడ్డ_res != LV_COORD_MIN) {
        బిడ్డ_res -= (obj->coords.x2 - space_కుడి);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_వెడల్పు(obj);
    self_w = self_w - (lv_obj_get_వెడల్పు(obj) - space_కుడి - space_ఎడమ);
    self_w -= lv_obj_get_scroll_x(obj);
    తిరిగి LV_MAX(బిడ్డ_res, self_w);
}

శూన్య lv_obj_get_scroll_చివర(lv_obj_t * obj, lv_బిందువు_t * చివర)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(చివర != ఖాళీ, తిరిగి);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    చివర->x = a ? -a->చివర_విలువ : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    చివర->y = a ? -a->చివర_విలువ : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

శూన్య lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(dx == 0 && dy == 0) తిరిగి;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) != LV_పునాది_DIR_RTL) {
        ఒకవేళ(x_bounded > 0) x_bounded = 0;
        ఒకవేళ(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ఎడమ(obj) + lv_obj_get_scroll_కుడి(obj);
            ఒకవేళ(scroll_max < 0) scroll_max = 0;

            ఒకవేళ(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    లేకపోతే {
        ఒకవేళ(x_bounded < 0) x_bounded = 0;
        ఒకవేళ(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_ఎడమ(obj) + lv_obj_get_scroll_కుడి(obj);
            ఒకవేళ(scroll_max < 0) scroll_max = 0;

            ఒకవేళ(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    ఒకవేళ(y_bounded > 0) y_bounded = 0;
    ఒకవేళ(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_శిఖరం(obj) + lv_obj_get_scroll_bottom(obj);
        ఒకవేళ(scroll_max < 0) scroll_max = 0;
        ఒకవేళ(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    ఒకవేళ(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

శూన్య lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(dx == 0 && dy == 0) తిరిగి;
    ఒకవేళ(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_సమితి_var(&a, obj);
        lv_anim_సమితి_deleted_cb(&a, scroll_చివర_cb);

        ఒకవేళ(dx) {
            uint32_t t = lv_anim_వేగం_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_సమయం_MIN,
                                               SCROLL_ANIM_సమయం_MAX);
            lv_anim_సమితి_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_సమితి_విలువలు(&a, -sx, -sx + dx);
            lv_anim_సమితి_exec_cb(&a, scroll_x_anim);
            lv_anim_సమితి_మార్గం_cb(&a, lv_anim_మార్గం_ease_బహిర్గతం);

            lv_ఫలితం_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
            lv_anim_ప్రారంభం(&a);
        }

        ఒకవేళ(dy) {
            uint32_t t = lv_anim_వేగం_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_సమయం_MIN,
                                               SCROLL_ANIM_సమయం_MAX);
            lv_anim_సమితి_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_సమితి_విలువలు(&a, -sy, -sy + dy);
            lv_anim_సమితి_exec_cb(&a,  scroll_y_anim);
            lv_anim_సమితి_మార్గం_cb(&a, lv_anim_మార్గం_ease_బహిర్గతం);

            lv_ఫలితం_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
            lv_anim_ప్రారంభం(&a);
        }
    }
    లేకపోతే {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_ఫలితం_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, ఖాళీ);
        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_చివర, ఖాళీ);
        ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి;
    }
}

శూన్య lv_obj_scroll_గమ్యం(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_scroll_గమ్యం_x(obj, x, anim_en);
    lv_obj_scroll_గమ్యం_y(obj, y, anim_en);
}

శూన్య lv_obj_scroll_గమ్యం_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

శూన్య lv_obj_scroll_గమ్యం_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

శూన్య lv_obj_scroll_గమ్యం_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_బిందువు_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

శూన్య lv_obj_scroll_గమ్యం_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_బిందువు_t p = {0, 0};
    lv_obj_t * బిడ్డ = obj;
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(బిడ్డ);
    అయితే(మాతృ) {
        scroll_area_into_view(&obj->coords, బిడ్డ, &p, anim_en);
        బిడ్డ = మాతృ;
        మాతృ = lv_obj_get_మాతృ(మాతృ);
    }
}

lv_ఫలితం_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి LV_ఫలితం_INVALID);

    ఒకవేళ(x == 0 && y == 0) తిరిగి LV_ఫలితం_OK;

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి LV_ఫలితం_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_ఫలితం_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, ఖాళీ);
    ఒకవేళ(res != LV_ఫలితం_OK) తిరిగి res;
    lv_obj_invalidate(obj);
    తిరిగి LV_ఫలితం_OK;
}

bool lv_obj_is_scrolling(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి false);

    lv_indev_t * indev = lv_indev_get_తదుపరి(ఖాళీ);
    అయితే(indev) {
        ఒకవేళ(lv_indev_get_scroll_obj(indev) == obj) తిరిగి true;
        indev = lv_indev_get_తదుపరి(indev);
    }

    ఒకవేళ(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != ఖాళీ ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != ఖాళీ) {
        తిరిగి true;
    }

    తిరిగి false;
}

శూన్య lv_obj_stop_scroll_anim(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

శూన్య lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_update_layout(obj);
    lv_బిందువు_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    ఒకవేళ(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    ఒకవేళ(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

శూన్య lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(hor_area != ఖాళీ, తిరిగి);
    LV_CHECK_ARG(ver_area != ఖాళీ, తిరిగి);

    lv_area_సమితి(hor_area, 0, 0, -1, -1);
    lv_area_సమితి(ver_area, 0, 0, -1, -1);

    ఒకవేళ(lv_obj_is_scrollable(obj) == false) తిరిగి;

    lv_scrollbar_బహుళకం_t sm = lv_obj_get_scrollbar_బహుళకం(obj);
    ఒకవేళ(sm == LV_SCROLLBAR_బహుళకం_OFF)  తిరిగి;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_తదుపరి(ఖాళీ);
    ఒకవేళ(sm == LV_SCROLLBAR_బహుళకం_చురుకైన) {
        అయితే(indev) {
            ఒకవేళ(lv_indev_get_scroll_obj(indev) == obj) ఆపు;
            indev = lv_indev_get_తదుపరి(indev);
        }
        ఒకవేళ(indev == ఖాళీ)  తిరిగి;
    }

    int32_t st = lv_obj_get_scroll_శిఖరం(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_ఎడమ(obj);
    int32_t sr = lv_obj_get_scroll_కుడి(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    ఒకవేళ((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_బహుళకం_ON) ||
        (sm == LV_SCROLLBAR_బహుళకం_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_బహుళకం_చురుకైన && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    ఒకవేళ((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_బహుళకం_ON) ||
        (sm == LV_SCROLLBAR_బహుళకం_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_బహుళకం_చురుకైన && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    ఒకవేళ(!hor_draw && !ver_draw) తిరిగి;

    bool rtl = lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_SCROLLBAR) == LV_పునాది_DIR_RTL;

    int32_t శిఖరం_space = lv_obj_get_style_pad_శిఖరం_అంతర్గత(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_అంతర్గత(obj, LV_PART_SCROLLBAR);
    int32_t ఎడమ_space = lv_obj_get_style_pad_ఎడమ_అంతర్గత(obj, LV_PART_SCROLLBAR);
    int32_t కుడి_space = lv_obj_get_style_pad_కుడి_అంతర్గత(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_వెడల్పు_అంతర్గత(obj, LV_PART_SCROLLBAR);
    int32_t నిడివి = lv_obj_get_style_నిడివి_అంతర్గత(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_ఎత్తు(obj);
    int32_t obj_w = lv_obj_get_వెడల్పు(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    ఒకవేళ(lv_obj_get_style_bg_opa_అంతర్గత(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_అంతర్గత(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        తిరిగి;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    ఒకవేళ(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        ఒకవేళ(rtl) {
            ver_area->x1 = obj->coords.x1 + ఎడమ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        లేకపోతే {
            ver_area->x2 = obj->coords.x2 - కుడి_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - శిఖరం_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(నిడివి > 0 ? నిడివి : sb_h, SCROLLBAR_MIN_కొలత); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - శిఖరం_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        ఒకవేళ(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + శిఖరం_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        లేకపోతే {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + శిఖరం_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            ఒకవేళ(ver_area->y1 < obj->coords.y1 + శిఖరం_space) {
                ver_area->y1 = obj->coords.y1 + శిఖరం_space;
                ఒకవేళ(ver_area->y1 + SCROLLBAR_MIN_కొలత > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_కొలత;
                }
            }
            ఒకవేళ(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                ఒకవేళ(ver_area->y2 - SCROLLBAR_MIN_కొలత < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_కొలత;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    ఒకవేళ(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - ఎడమ_space - కుడి_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(నిడివి > 0 ? నిడివి : sb_w, SCROLLBAR_MIN_కొలత); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - ఎడమ_space - కుడి_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        ఒకవేళ(scroll_w <= 0) {
            ఒకవేళ(rtl) {
                hor_area->x1 = obj->coords.x1 + ఎడమ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - కుడి_space;
            }
            లేకపోతే {
                hor_area->x1 = obj->coords.x1 + ఎడమ_space;
                hor_area->x2 = obj->coords.x2 - కుడి_space - ver_reg_space - 1;
            }
        }
        లేకపోతే {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            ఒకవేళ(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + ఎడమ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ఒకవేళ(hor_area->x1 < obj->coords.x1 + ఎడమ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + ఎడమ_space + ver_reg_space;
                    ఒకవేళ(hor_area->x1 + SCROLLBAR_MIN_కొలత > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_కొలత;
                    }
                }
                ఒకవేళ(hor_area->x2 > obj->coords.x2 - కుడి_space) {
                    hor_area->x2 = obj->coords.x2 - కుడి_space;
                    ఒకవేళ(hor_area->x2 - SCROLLBAR_MIN_కొలత < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_కొలత;
                    }
                }
            }
            లేకపోతే {
                hor_area->x1 = obj->coords.x1 + sb_x + ఎడమ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                ఒకవేళ(hor_area->x1 < obj->coords.x1 + ఎడమ_space) {
                    hor_area->x1 = obj->coords.x1 + ఎడమ_space;
                    ఒకవేళ(hor_area->x1 + SCROLLBAR_MIN_కొలత > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_కొలత;
                    }
                }
                ఒకవేళ(hor_area->x2 > obj->coords.x2 - ver_reg_space - కుడి_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - కుడి_space;
                    ఒకవేళ(hor_area->x2 - SCROLLBAR_MIN_కొలత < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_కొలత;
                    }
                }
            }
        }
    }
}

శూన్య lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    ఒకవేళ(lv_area_get_కొలత(&hor_area) <= 0 && lv_area_get_కొలత(&ver_area) <= 0) తిరిగి;

    ఒకవేళ(lv_area_get_కొలత(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    ఒకవేళ(lv_area_get_కొలత(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

శూన్య lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    ఒకవేళ(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_శిఖరం(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        ఒకవేళ(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    ఒకవేళ(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_ఎడమ(obj);
        int32_t sr = lv_obj_get_scroll_కుడి(obj);
        ఒకవేళ(lv_obj_get_style_పునాది_dir_అంతర్గత(obj, LV_PART_MAIN) != LV_పునాది_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            ఒకవేళ(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        లేకపోతే {
            /*Be sure the right side is not remains scrolled in*/
            ఒకవేళ(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర అంతఃవరుస int32_t mul_div(int32_t v, int32_t గుణకం, int32_t భాజకం)
{
    తిరిగి ((int64_t)v * గుణకం) / భాజకం;
}

స్థిర శూన్య scroll_x_anim(శూన్య * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

స్థిర శూన్య scroll_y_anim(శూన్య * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

స్థిర శూన్య scroll_చివర_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    ఒకవేళ(a->ప్రారంభం_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_చివర, ఖాళీ);
}

స్థిర శూన్య scroll_area_into_view(స్థిరరాశి lv_area_t * area, lv_obj_t * బిడ్డ, lv_బిందువు_t * scroll_విలువ,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(బిడ్డ);
    ఒకవేళ(!lv_obj_is_scrollable(మాతృ)) తిరిగి;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(మాతృ);
    int32_t snap_goal = 0;
    int32_t act = 0;
    స్థిరరాశి lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(మాతృ);
    ఒకవేళ(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &బిడ్డ->coords;
    లేకపోతే area_tmp = area;

    int32_t stop = lv_obj_get_style_space_శిఖరం_అంతర్గత(మాతృ, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_అంతర్గత(మాతృ, LV_PART_MAIN);
    int32_t శిఖరం_diff = మాతృ->coords.y1 + stop - area_tmp->y1 - scroll_విలువ->y;
    int32_t bottom_diff = -(మాతృ->coords.y2 - sbottom - area_tmp->y2 - scroll_విలువ->y);
    int32_t మాతృ_h = lv_obj_get_ఎత్తు(మాతృ) - stop - sbottom;
    ఒకవేళ((శిఖరం_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    లేకపోతే ఒకవేళ(శిఖరం_diff > 0) {
        y_scroll = శిఖరం_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_శిఖరం(మాతృ);
        ఒకవేళ(st - y_scroll < 0) y_scroll = 0;
    }
    లేకపోతే ఒకవేళ(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(మాతృ);
        ఒకవేళ(sb + y_scroll < 0) y_scroll = 0;
    }

    ఎంపిక(snap_y) {
        స్థితి LV_SCROLL_SNAP_ప్రారంభం:
            snap_goal = మాతృ->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_చివర:
            snap_goal = మాతృ->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_CENTER:
            snap_goal = మాతృ->coords.y1 + stop + మాతృ_h / 2;
            act = lv_area_get_ఎత్తు(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_NONE:
            ఆపు;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(మాతృ);
    ఒకవేళ(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &బిడ్డ->coords;
    లేకపోతే area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_ఎడమ_అంతర్గత(మాతృ, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_కుడి_అంతర్గత(మాతృ, LV_PART_MAIN);
    int32_t ఎడమ_diff = మాతృ->coords.x1 + sleft - area_tmp->x1 - scroll_విలువ->x;
    int32_t కుడి_diff = -(మాతృ->coords.x2 - sright - area_tmp->x2 - scroll_విలువ->x);
    ఒకవేళ((ఎడమ_diff >= 0 && కుడి_diff >= 0)) x_scroll = 0;
    లేకపోతే ఒకవేళ(ఎడమ_diff > 0) {
        x_scroll = ఎడమ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_ఎడమ(మాతృ);
        ఒకవేళ(sl - x_scroll < 0) x_scroll = 0;
    }
    లేకపోతే ఒకవేళ(కుడి_diff > 0) {
        x_scroll = -కుడి_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_కుడి(మాతృ);
        ఒకవేళ(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t మాతృ_w = lv_obj_get_వెడల్పు(మాతృ) - sleft - sright;
    ఎంపిక(snap_x) {
        స్థితి LV_SCROLL_SNAP_ప్రారంభం:
            snap_goal = మాతృ->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_చివర:
            snap_goal = మాతృ->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_CENTER:
            snap_goal = మాతృ->coords.x1 + sleft + మాతృ_w / 2;
            act = lv_area_get_వెడల్పు(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            ఆపు;
        స్థితి LV_SCROLL_SNAP_NONE:
            ఆపు;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(మాతృ, scroll_y_anim);
    lv_anim_delete(మాతృ, scroll_x_anim);

    ఒకవేళ((scroll_dir & LV_DIR_ఎడమ) == 0 && x_scroll < 0) x_scroll = 0;
    ఒకవేళ((scroll_dir & LV_DIR_కుడి) == 0 && x_scroll > 0) x_scroll = 0;
    ఒకవేళ((scroll_dir & LV_DIR_శిఖరం) == 0 && y_scroll < 0) y_scroll = 0;
    ఒకవేళ((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_విలువ->x += anim_en ? x_scroll : 0;
    scroll_విలువ->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(మాతృ, x_scroll, y_scroll, anim_en);
}
