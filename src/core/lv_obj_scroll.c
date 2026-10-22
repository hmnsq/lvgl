/**
 * @file lv_obj_scroll.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "lv_obj_scroll_private.h"
#समावेश "../lvgl_public.h"
#समावेश "../misc/lv_anim_private.h"
#समावेश "lv_obj_private.h"
#समावेश "../indev/lv_indev_scroll.h"
#समावेश "lv_obj_style_internal.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा MY_CLASS (&lv_obj_class)
#अगर_अघोषित SCROLL_ANIM_TIME_MIN
    #घोषणा SCROLL_ANIM_TIME_MIN    200    /*ms*/
#पूर्ण_अगर
#अगर_अघोषित SCROLL_ANIM_TIME_MAX
    #घोषणा SCROLL_ANIM_TIME_MAX    400    /*ms*/
#पूर्ण_अगर
#घोषणा SCROLLBAR_MIN_आकार      (LV_DPX(10))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल अंतरभूत int32_t mul_div(int32_t v, int32_t factor, int32_t भाजक);
अटल व्योम scroll_x_anim(व्योम * obj, int32_t v);
अटल व्योम scroll_y_anim(व्योम * obj, int32_t v);
अटल व्योम scroll_end_cb(lv_anim_t * a);
अटल व्योम scroll_area_into_view(स्थिर lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_मूल्य,
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

व्योम lv_obj_set_scrollbar_mode(lv_obj_t * obj, lv_scrollbar_mode_t mode)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }

    अगर(obj->spec_attr->scrollbar_mode == mode) वापस;
    obj->spec_attr->scrollbar_mode = mode;
    lv_obj_invalidate(obj);
}

व्योम lv_obj_set_scroll_dir(lv_obj_t * obj, lv_dir_t dir)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }

    अगर(dir != obj->spec_attr->scroll_dir) {
        obj->spec_attr->scroll_dir = dir;
    }
}

व्योम lv_obj_set_scroll_snap_x(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }
    obj->spec_attr->scroll_snap_x = align;
}

व्योम lv_obj_set_scroll_snap_y(lv_obj_t * obj, lv_scroll_snap_t align)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }
    obj->spec_attr->scroll_snap_y = align;
}

/*=====================
 * Getter functions
 *====================*/

lv_scrollbar_mode_t lv_obj_get_scrollbar_mode(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_SCROLLBAR_MODE_OFF);

    अगर(obj->spec_attr) वापस (lv_scrollbar_mode_t) obj->spec_attr->scrollbar_mode;
    अन्यथा वापस LV_SCROLLBAR_MODE_AUTO;
}

lv_dir_t lv_obj_get_scroll_dir(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_DIR_NONE);

    अगर(obj->spec_attr) वापस (lv_dir_t) obj->spec_attr->scroll_dir;
    अन्यथा वापस LV_DIR_ALL;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_x(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_SCROLL_SNAP_NONE);

    अगर(obj->spec_attr) वापस (lv_scroll_snap_t) obj->spec_attr->scroll_snap_x;
    अन्यथा वापस LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t lv_obj_get_scroll_snap_y(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_SCROLL_SNAP_NONE);

    अगर(obj->spec_attr) वापस (lv_scroll_snap_t) obj->spec_attr->scroll_snap_y;
    अन्यथा वापस LV_SCROLL_SNAP_NONE;
}

int32_t lv_obj_get_scroll_x(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    अगर(obj->spec_attr == शून्य) वापस 0;
    वापस -obj->spec_attr->scroll.x;
}

int32_t lv_obj_get_scroll_y(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    अगर(obj->spec_attr == शून्य) वापस 0;
    वापस -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_शीर्ष(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    अगर(obj->spec_attr == शून्य) वापस 0;
    वापस -obj->spec_attr->scroll.y;
}

int32_t lv_obj_get_scroll_bottom(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        स्थिर lv_obj_t * child = obj->spec_attr->children[i];
        अगर((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) जारी;

        int32_t tmp_y = child->coords.y2 + lv_obj_get_style_margin_bottom_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_y);
    }

    int32_t space_शीर्ष = lv_obj_get_style_space_शीर्ष_internal(obj, LV_PART_MAIN);
    int32_t space_bottom = lv_obj_get_style_space_bottom_internal(obj, LV_PART_MAIN);

    अगर(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.y2 - space_bottom);
    }

    int32_t self_h = lv_obj_get_self_height(obj);
    self_h = self_h - (lv_obj_get_height(obj) - space_शीर्ष - space_bottom);
    self_h -= lv_obj_get_scroll_y(obj);
    वापस LV_MAX(child_res, self_h);
}

int32_t lv_obj_get_scroll_बायाँ(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    /*Normally can't scroll the object out on the left.
     *So simply use the current scroll position as "left size"*/
    अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        अगर(obj->spec_attr == शून्य) वापस 0;
        वापस -obj->spec_attr->scroll.x;
    }

    /*With RTL base direction scrolling the left is normal so find the left most coordinate*/
    int32_t space_दायाँ = lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);
    int32_t space_बायाँ = lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);

    int32_t child_res = 0;

    uint32_t i;
    int32_t x1 = LV_COORD_MAX;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        स्थिर lv_obj_t * child = obj->spec_attr->children[i];
        अगर((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) जारी;

        int32_t tmp_x = child->coords.x1 - lv_obj_get_style_margin_बायाँ_internal(child, LV_PART_MAIN);
        x1 = LV_MIN(x1, tmp_x);
    }

    अगर(x1 != LV_COORD_MAX) {
        child_res = x1;
        child_res = (obj->coords.x1 + space_बायाँ) - child_res;
    }
    अन्यथा {
        child_res = LV_COORD_MIN;
    }

    int32_t self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_दायाँ - space_बायाँ);
    self_w += lv_obj_get_scroll_x(obj);

    वापस LV_MAX(child_res, self_w);
}

int32_t lv_obj_get_scroll_दायाँ(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);

    /*With RTL base dir can't scroll to the object out on the right.
     *So simply use the current scroll position as "right size"*/
    अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) == LV_BASE_DIR_RTL) {
        अगर(obj->spec_attr == शून्य) वापस 0;
        वापस obj->spec_attr->scroll.x;
    }

    /*With other base direction (LTR) scrolling to the right is normal so find the right most coordinate*/
    int32_t child_res = LV_COORD_MIN;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(obj);
    क्रम(i = 0; i < child_cnt; i++) {
        स्थिर lv_obj_t * child = obj->spec_attr->children[i];
        अगर((lv_obj_is_hidden(child) || lv_obj_is_floating(child))) जारी;

        int32_t tmp_x = child->coords.x2 + lv_obj_get_style_margin_दायाँ_internal(child, LV_PART_MAIN);
        child_res = LV_MAX(child_res, tmp_x);
    }

    int32_t space_दायाँ = lv_obj_get_style_space_दायाँ_internal(obj, LV_PART_MAIN);
    int32_t space_बायाँ = lv_obj_get_style_space_बायाँ_internal(obj, LV_PART_MAIN);

    अगर(child_res != LV_COORD_MIN) {
        child_res -= (obj->coords.x2 - space_दायाँ);
    }

    int32_t self_w;
    self_w = lv_obj_get_self_width(obj);
    self_w = self_w - (lv_obj_get_width(obj) - space_दायाँ - space_बायाँ);
    self_w -= lv_obj_get_scroll_x(obj);
    वापस LV_MAX(child_res, self_w);
}

व्योम lv_obj_get_scroll_end(lv_obj_t * obj, lv_point_t * end)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(end != शून्य, वापस);

    lv_anim_t * a;
    a = lv_anim_get(obj, scroll_x_anim);
    end->x = a ? -a->end_मूल्य : lv_obj_get_scroll_x(obj);

    a = lv_anim_get(obj, scroll_y_anim);
    end->y = a ? -a->end_मूल्य : lv_obj_get_scroll_y(obj);
}

/*=====================
 * Other functions
 *====================*/

व्योम lv_obj_scroll_by_bounded(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(dx == 0 && dy == 0) वापस;

    /*We need to know the final sizes for bound check*/
    lv_obj_update_layout(obj);

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t x_current = -lv_obj_get_scroll_x(obj);
    int32_t x_bounded = x_current + dx;

    अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
        अगर(x_bounded > 0) x_bounded = 0;
        अगर(x_bounded < 0) {
            int32_t  scroll_max = lv_obj_get_scroll_बायाँ(obj) + lv_obj_get_scroll_दायाँ(obj);
            अगर(scroll_max < 0) scroll_max = 0;

            अगर(x_bounded < -scroll_max) x_bounded = -scroll_max;
        }
    }
    अन्यथा {
        अगर(x_bounded < 0) x_bounded = 0;
        अगर(x_bounded > 0) {
            int32_t  scroll_max = lv_obj_get_scroll_बायाँ(obj) + lv_obj_get_scroll_दायाँ(obj);
            अगर(scroll_max < 0) scroll_max = 0;

            अगर(x_bounded > scroll_max) x_bounded = scroll_max;
        }
    }

    /*Don't let scroll more than naturally possible by the size of the content*/
    int32_t y_current = -lv_obj_get_scroll_y(obj);
    int32_t y_bounded = y_current + dy;

    अगर(y_bounded > 0) y_bounded = 0;
    अगर(y_bounded < 0) {
        int32_t  scroll_max = lv_obj_get_scroll_शीर्ष(obj) + lv_obj_get_scroll_bottom(obj);
        अगर(scroll_max < 0) scroll_max = 0;
        अगर(y_bounded < -scroll_max) y_bounded = -scroll_max;
    }

    dx = x_bounded - x_current;
    dy = y_bounded - y_current;
    अगर(dx || dy) {
        lv_obj_scroll_by(obj, dx, dy, anim_en);
    }
}

व्योम lv_obj_scroll_by(lv_obj_t * obj, int32_t dx, int32_t dy, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(dx == 0 && dy == 0) वापस;
    अगर(anim_en) {
        lv_display_t * d = lv_obj_get_display(obj);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_deleted_cb(&a, scroll_end_cb);

        अगर(dx) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_horizontal_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sx = lv_obj_get_scroll_x(obj);
            lv_anim_set_मूल्यावली(&a, -sx, -sx + dx);
            lv_anim_set_exec_cb(&a, scroll_x_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_परिणाम_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            अगर(res != LV_परिणाम_OK) वापस;
            lv_anim_start(&a);
        }

        अगर(dy) {
            uint32_t t = lv_anim_speed_clamped((lv_display_get_vertical_resolution(d)) >> 1, SCROLL_ANIM_TIME_MIN,
                                               SCROLL_ANIM_TIME_MAX);
            lv_anim_set_duration(&a, t);
            int32_t sy = lv_obj_get_scroll_y(obj);
            lv_anim_set_मूल्यावली(&a, -sy, -sy + dy);
            lv_anim_set_exec_cb(&a,  scroll_y_anim);
            lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

            lv_परिणाम_t res;
            res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, &a);
            अगर(res != LV_परिणाम_OK) वापस;
            lv_anim_start(&a);
        }
    }
    अन्यथा {
        /*Remove pending animations*/
        lv_anim_delete(obj, scroll_y_anim);
        lv_anim_delete(obj, scroll_x_anim);

        lv_परिणाम_t res;
        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_BEGIN, शून्य);
        अगर(res != LV_परिणाम_OK) वापस;

        res = lv_obj_scroll_by_raw(obj, dx, dy);
        अगर(res != LV_परिणाम_OK) वापस;

        res = lv_obj_send_event(obj, LV_EVENT_SCROLL_END, शून्य);
        अगर(res != LV_परिणाम_OK) वापस;
    }
}

व्योम lv_obj_scroll_to(lv_obj_t * obj, int32_t x, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_scroll_to_x(obj, x, anim_en);
    lv_obj_scroll_to_y(obj, y, anim_en);
}

व्योम lv_obj_scroll_to_x(lv_obj_t * obj, int32_t x, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_anim_delete(obj, scroll_x_anim);

    int32_t scroll_x = lv_obj_get_scroll_x(obj);
    int32_t diff = -x + scroll_x;

    lv_obj_scroll_by_bounded(obj, diff, 0, anim_en);
}

व्योम lv_obj_scroll_to_y(lv_obj_t * obj, int32_t y, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_anim_delete(obj, scroll_y_anim);

    int32_t scroll_y = lv_obj_get_scroll_y(obj);
    int32_t diff = -y + scroll_y;

    lv_obj_scroll_by_bounded(obj, 0, diff, anim_en);
}

व्योम lv_obj_scroll_to_view(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    scroll_area_into_view(&obj->coords, obj, &p, anim_en);
}

व्योम lv_obj_scroll_to_view_recursive(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    /*Be sure the screens layout is correct*/
    lv_obj_update_layout(obj);

    lv_point_t p = {0, 0};
    lv_obj_t * child = obj;
    lv_obj_t * parent = lv_obj_get_parent(child);
    जबतक(parent) {
        scroll_area_into_view(&obj->coords, child, &p, anim_en);
        child = parent;
        parent = lv_obj_get_parent(parent);
    }
}

lv_परिणाम_t lv_obj_scroll_by_raw(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस LV_परिणाम_INVALID);

    अगर(x == 0 && y == 0) वापस LV_परिणाम_OK;

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस LV_परिणाम_INVALID;
    }

    obj->spec_attr->scroll.x += x;
    obj->spec_attr->scroll.y += y;

    lv_obj_move_children_by(obj, x, y, true);
    lv_परिणाम_t res = lv_obj_send_event(obj, LV_EVENT_SCROLL, शून्य);
    अगर(res != LV_परिणाम_OK) वापस res;
    lv_obj_invalidate(obj);
    वापस LV_परिणाम_OK;
}

bool lv_obj_is_scrolling(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस false);

    lv_indev_t * indev = lv_indev_get_अग्र(शून्य);
    जबतक(indev) {
        अगर(lv_indev_get_scroll_obj(indev) == obj) वापस true;
        indev = lv_indev_get_अग्र(indev);
    }

    अगर(lv_anim_get((lv_obj_t *)obj, scroll_x_anim) != शून्य ||
       lv_anim_get((lv_obj_t *)obj, scroll_y_anim) != शून्य) {
        वापस true;
    }

    वापस false;
}

व्योम lv_obj_stop_scroll_anim(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_anim_delete((lv_obj_t *)obj, scroll_y_anim);
    lv_anim_delete((lv_obj_t *)obj, scroll_x_anim);
}

व्योम lv_obj_update_snap(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_update_layout(obj);
    lv_point_t p;
    lv_indev_scroll_get_snap_dist(obj, &p);
    अगर(p.x == LV_COORD_MAX || p.x == LV_COORD_MIN) p.x = 0;
    अगर(p.y == LV_COORD_MAX || p.y == LV_COORD_MIN) p.y = 0;
    lv_obj_scroll_by(obj, p.x, p.y, anim_en);
}

व्योम lv_obj_get_scrollbar_area(lv_obj_t * obj, lv_area_t * hor_area, lv_area_t * ver_area)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(hor_area != शून्य, वापस);
    LV_CHECK_ARG(ver_area != शून्य, वापस);

    lv_area_set(hor_area, 0, 0, -1, -1);
    lv_area_set(ver_area, 0, 0, -1, -1);

    अगर(lv_obj_is_scrollable(obj) == false) वापस;

    lv_scrollbar_mode_t sm = lv_obj_get_scrollbar_mode(obj);
    अगर(sm == LV_SCROLLBAR_MODE_OFF)  वापस;

    /*If there is no indev scrolling this object but `mode==active` return*/
    lv_indev_t * indev = lv_indev_get_अग्र(शून्य);
    अगर(sm == LV_SCROLLBAR_MODE_ACTIVE) {
        जबतक(indev) {
            अगर(lv_indev_get_scroll_obj(indev) == obj) अवरोध;
            indev = lv_indev_get_अग्र(indev);
        }
        अगर(indev == शून्य)  वापस;
    }

    int32_t st = lv_obj_get_scroll_शीर्ष(obj);
    int32_t sb = lv_obj_get_scroll_bottom(obj);
    int32_t sl = lv_obj_get_scroll_बायाँ(obj);
    int32_t sr = lv_obj_get_scroll_दायाँ(obj);

    lv_dir_t dir = lv_obj_get_scroll_dir(obj);

    bool ver_draw = false;
    अगर((dir & LV_DIR_VER) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (st > 0 || sb > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_VER))) {
        ver_draw = true;
    }

    bool hor_draw = false;
    अगर((dir & LV_DIR_HOR) &&
       ((sm == LV_SCROLLBAR_MODE_ON) ||
        (sm == LV_SCROLLBAR_MODE_AUTO && (sl > 0 || sr > 0)) ||
        (sm == LV_SCROLLBAR_MODE_ACTIVE && lv_indev_get_scroll_dir(indev) == LV_DIR_HOR))) {
        hor_draw = true;
    }

    अगर(!hor_draw && !ver_draw) वापस;

    bool rtl = lv_obj_get_style_base_dir_internal(obj, LV_PART_SCROLLBAR) == LV_BASE_DIR_RTL;

    int32_t शीर्ष_space = lv_obj_get_style_pad_शीर्ष_internal(obj, LV_PART_SCROLLBAR);
    int32_t bottom_space = lv_obj_get_style_pad_bottom_internal(obj, LV_PART_SCROLLBAR);
    int32_t बायाँ_space = lv_obj_get_style_pad_बायाँ_internal(obj, LV_PART_SCROLLBAR);
    int32_t दायाँ_space = lv_obj_get_style_pad_दायाँ_internal(obj, LV_PART_SCROLLBAR);
    int32_t thickness = lv_obj_get_style_width_internal(obj, LV_PART_SCROLLBAR);
    int32_t लंबाई = lv_obj_get_style_लंबाई_internal(obj, LV_PART_SCROLLBAR);

    int32_t obj_h = lv_obj_get_height(obj);
    int32_t obj_w = lv_obj_get_width(obj);

    /*Space required for the vertical and horizontal scrollbars*/
    int32_t ver_reg_space = ver_draw ? thickness : 0;
    int32_t hor_req_space = hor_draw ? thickness : 0;
    int32_t rem;

    अगर(lv_obj_get_style_bg_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN &&
       lv_obj_get_style_border_opa_internal(obj, LV_PART_SCROLLBAR) <= LV_OPA_MIN) {
        वापस;
    }

    /*Draw vertical scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_h = obj_h + st + sb;
    अगर(ver_draw && content_h) {
        ver_area->y1 = obj->coords.y1;
        ver_area->y2 = obj->coords.y2;
        अगर(rtl) {
            ver_area->x1 = obj->coords.x1 + बायाँ_space;
            ver_area->x2 = ver_area->x1 + thickness - 1;
        }
        अन्यथा {
            ver_area->x2 = obj->coords.x2 - दायाँ_space;
            ver_area->x1 = ver_area->x2 - thickness + 1;
        }

        int32_t sb_h = mul_div(obj_h - शीर्ष_space - bottom_space - hor_req_space, obj_h, content_h);
        sb_h = LV_MAX(लंबाई > 0 ? लंबाई : sb_h, SCROLLBAR_MIN_आकार); /*Style-defined size, calculated size, or minimum size*/
        sb_h = LV_MIN(sb_h, obj_h); /*Limit scrollbar length to parent height*/
        rem = (obj_h - शीर्ष_space - bottom_space - hor_req_space) -
              sb_h;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_h = content_h - obj_h; /*The size of the content which can be really scrolled*/
        अगर(scroll_h <= 0) {
            ver_area->y1 = obj->coords.y1 + शीर्ष_space;
            ver_area->y2 = obj->coords.y2 - bottom_space - hor_req_space - 1;
        }
        अन्यथा {
            int32_t sb_y = mul_div(rem, sb, scroll_h);
            sb_y = rem - sb_y;

            ver_area->y1 = obj->coords.y1 + sb_y + शीर्ष_space;
            ver_area->y2 = ver_area->y1 + sb_h - 1;
            अगर(ver_area->y1 < obj->coords.y1 + शीर्ष_space) {
                ver_area->y1 = obj->coords.y1 + शीर्ष_space;
                अगर(ver_area->y1 + SCROLLBAR_MIN_आकार > ver_area->y2) {
                    ver_area->y2 = ver_area->y1 + SCROLLBAR_MIN_आकार;
                }
            }
            अगर(ver_area->y2 > obj->coords.y2 - hor_req_space - bottom_space) {
                ver_area->y2 = obj->coords.y2 - hor_req_space - bottom_space;
                अगर(ver_area->y2 - SCROLLBAR_MIN_आकार < ver_area->y1) {
                    ver_area->y1 = ver_area->y2 - SCROLLBAR_MIN_आकार;
                }
            }
        }
    }

    /*Draw horizontal scrollbar if the mode is ON or can be scrolled in this direction*/
    int32_t content_w = obj_w + sl + sr;
    अगर(hor_draw && content_w) {
        hor_area->y2 = obj->coords.y2 - bottom_space;
        hor_area->y1 = hor_area->y2 - thickness + 1;
        hor_area->x1 = obj->coords.x1;
        hor_area->x2 = obj->coords.x2;

        int32_t sb_w = mul_div(obj_w - बायाँ_space - दायाँ_space - ver_reg_space, obj_w, content_w);
        sb_w = LV_MAX(लंबाई > 0 ? लंबाई : sb_w, SCROLLBAR_MIN_आकार); /*Style-defined size, calculated size, or minimum size*/
        sb_w = LV_MIN(sb_w, obj_w); /*Limit scrollbar length to parent width*/
        rem = (obj_w - बायाँ_space - दायाँ_space - ver_reg_space) -
              sb_w;  /*Remaining size from the scrollbar track that is not the scrollbar itself*/
        int32_t scroll_w = content_w - obj_w; /*The size of the content which can be really scrolled*/
        अगर(scroll_w <= 0) {
            अगर(rtl) {
                hor_area->x1 = obj->coords.x1 + बायाँ_space + ver_reg_space - 1;
                hor_area->x2 = obj->coords.x2 - दायाँ_space;
            }
            अन्यथा {
                hor_area->x1 = obj->coords.x1 + बायाँ_space;
                hor_area->x2 = obj->coords.x2 - दायाँ_space - ver_reg_space - 1;
            }
        }
        अन्यथा {
            int32_t sb_x = mul_div(rem, sr, scroll_w);
            sb_x = rem - sb_x;

            अगर(rtl) {
                hor_area->x1 = obj->coords.x1 + sb_x + बायाँ_space + ver_reg_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                अगर(hor_area->x1 < obj->coords.x1 + बायाँ_space + ver_reg_space) {
                    hor_area->x1 = obj->coords.x1 + बायाँ_space + ver_reg_space;
                    अगर(hor_area->x1 + SCROLLBAR_MIN_आकार > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_आकार;
                    }
                }
                अगर(hor_area->x2 > obj->coords.x2 - दायाँ_space) {
                    hor_area->x2 = obj->coords.x2 - दायाँ_space;
                    अगर(hor_area->x2 - SCROLLBAR_MIN_आकार < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_आकार;
                    }
                }
            }
            अन्यथा {
                hor_area->x1 = obj->coords.x1 + sb_x + बायाँ_space;
                hor_area->x2 = hor_area->x1 + sb_w - 1;
                अगर(hor_area->x1 < obj->coords.x1 + बायाँ_space) {
                    hor_area->x1 = obj->coords.x1 + बायाँ_space;
                    अगर(hor_area->x1 + SCROLLBAR_MIN_आकार > hor_area->x2) {
                        hor_area->x2 = hor_area->x1 + SCROLLBAR_MIN_आकार;
                    }
                }
                अगर(hor_area->x2 > obj->coords.x2 - ver_reg_space - दायाँ_space) {
                    hor_area->x2 = obj->coords.x2 - ver_reg_space - दायाँ_space;
                    अगर(hor_area->x2 - SCROLLBAR_MIN_आकार < hor_area->x1) {
                        hor_area->x1 = hor_area->x2 - SCROLLBAR_MIN_आकार;
                    }
                }
            }
        }
    }
}

व्योम lv_obj_scrollbar_invalidate(lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_area_t hor_area;
    lv_area_t ver_area;
    lv_obj_get_scrollbar_area(obj, &hor_area, &ver_area);

    अगर(lv_area_get_आकार(&hor_area) <= 0 && lv_area_get_आकार(&ver_area) <= 0) वापस;

    अगर(lv_area_get_आकार(&hor_area) > 0) lv_obj_invalidate_area(obj, &hor_area);
    अगर(lv_area_get_आकार(&ver_area) > 0) lv_obj_invalidate_area(obj, &ver_area);
}

व्योम lv_obj_readjust_scroll(lv_obj_t * obj, lv_anim_enable_t anim_en)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    /*Be sure the bottom side is not remains scrolled in*/
    /*With snapping the content can't be scrolled in*/
    अगर(lv_obj_get_scroll_snap_y(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t st = lv_obj_get_scroll_शीर्ष(obj);
        int32_t sb = lv_obj_get_scroll_bottom(obj);
        अगर(sb < 0 && st > 0) {
            sb = LV_MIN(st, -sb);
            lv_obj_scroll_by(obj, 0, sb, anim_en);
        }
    }

    अगर(lv_obj_get_scroll_snap_x(obj) == LV_SCROLL_SNAP_NONE) {
        int32_t sl = lv_obj_get_scroll_बायाँ(obj);
        int32_t sr = lv_obj_get_scroll_दायाँ(obj);
        अगर(lv_obj_get_style_base_dir_internal(obj, LV_PART_MAIN) != LV_BASE_DIR_RTL) {
            /*Be sure the left side is not remains scrolled in*/
            अगर(sr < 0 && sl > 0) {
                sr = LV_MIN(sl, -sr);
                lv_obj_scroll_by(obj, sr, 0, anim_en);
            }
        }
        अन्यथा {
            /*Be sure the right side is not remains scrolled in*/
            अगर(sl < 0 && sr > 0) {
                sr = LV_MIN(sr, -sl);
                lv_obj_scroll_by(obj, sl, 0, anim_en);
            }
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल अंतरभूत int32_t mul_div(int32_t v, int32_t factor, int32_t भाजक)
{
    वापस ((int64_t)v * factor) / भाजक;
}

अटल व्योम scroll_x_anim(व्योम * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, v + lv_obj_get_scroll_x(obj), 0);
}

अटल व्योम scroll_y_anim(व्योम * obj, int32_t v)
{
    lv_obj_scroll_by_raw(obj, 0, v + lv_obj_get_scroll_y(obj));
}

अटल व्योम scroll_end_cb(lv_anim_t * a)
{
    /*Do not sent END event if there wasn't a BEGIN*/
    अगर(a->start_cb_called) lv_obj_send_event(a->var, LV_EVENT_SCROLL_END, शून्य);
}

अटल व्योम scroll_area_into_view(स्थिर lv_area_t * area, lv_obj_t * child, lv_point_t * scroll_मूल्य,
                                  lv_anim_enable_t anim_en)
{
    lv_obj_t * parent = lv_obj_get_parent(child);
    अगर(!lv_obj_is_scrollable(parent)) वापस;

    lv_dir_t scroll_dir = lv_obj_get_scroll_dir(parent);
    int32_t snap_goal = 0;
    int32_t act = 0;
    स्थिर lv_area_t * area_tmp;

    int32_t y_scroll = 0;
    lv_scroll_snap_t snap_y = lv_obj_get_scroll_snap_y(parent);
    अगर(snap_y != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    अन्यथा area_tmp = area;

    int32_t stop = lv_obj_get_style_space_शीर्ष_internal(parent, LV_PART_MAIN);
    int32_t sbottom = lv_obj_get_style_space_bottom_internal(parent, LV_PART_MAIN);
    int32_t शीर्ष_diff = parent->coords.y1 + stop - area_tmp->y1 - scroll_मूल्य->y;
    int32_t bottom_diff = -(parent->coords.y2 - sbottom - area_tmp->y2 - scroll_मूल्य->y);
    int32_t parent_h = lv_obj_get_height(parent) - stop - sbottom;
    अगर((शीर्ष_diff >= 0 && bottom_diff >= 0)) y_scroll = 0;
    अन्यथा अगर(शीर्ष_diff > 0) {
        y_scroll = शीर्ष_diff;
        /*Do not let scrolling in*/
        int32_t st = lv_obj_get_scroll_शीर्ष(parent);
        अगर(st - y_scroll < 0) y_scroll = 0;
    }
    अन्यथा अगर(bottom_diff > 0) {
        y_scroll = -bottom_diff;
        /*Do not let scrolling in*/
        int32_t sb = lv_obj_get_scroll_bottom(parent);
        अगर(sb + y_scroll < 0) y_scroll = 0;
    }

    चयन(snap_y) {
        हाल LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.y1 + stop;
            act = area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.y2 - sbottom;
            act = area_tmp->y2 + y_scroll;
            y_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.y1 + stop + parent_h / 2;
            act = lv_area_get_height(area_tmp) / 2 + area_tmp->y1 + y_scroll;
            y_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_NONE:
            अवरोध;
    }

    int32_t x_scroll = 0;
    lv_scroll_snap_t snap_x = lv_obj_get_scroll_snap_x(parent);
    अगर(snap_x != LV_SCROLL_SNAP_NONE) area_tmp = &child->coords;
    अन्यथा area_tmp = area;

    int32_t sleft = lv_obj_get_style_space_बायाँ_internal(parent, LV_PART_MAIN);
    int32_t sright = lv_obj_get_style_space_दायाँ_internal(parent, LV_PART_MAIN);
    int32_t बायाँ_diff = parent->coords.x1 + sleft - area_tmp->x1 - scroll_मूल्य->x;
    int32_t दायाँ_diff = -(parent->coords.x2 - sright - area_tmp->x2 - scroll_मूल्य->x);
    अगर((बायाँ_diff >= 0 && दायाँ_diff >= 0)) x_scroll = 0;
    अन्यथा अगर(बायाँ_diff > 0) {
        x_scroll = बायाँ_diff;
        /*Do not let scrolling in*/
        int32_t sl = lv_obj_get_scroll_बायाँ(parent);
        अगर(sl - x_scroll < 0) x_scroll = 0;
    }
    अन्यथा अगर(दायाँ_diff > 0) {
        x_scroll = -दायाँ_diff;
        /*Do not let scrolling in*/
        int32_t sr = lv_obj_get_scroll_दायाँ(parent);
        अगर(sr + x_scroll < 0) x_scroll = 0;
    }

    int32_t parent_w = lv_obj_get_width(parent) - sleft - sright;
    चयन(snap_x) {
        हाल LV_SCROLL_SNAP_START:
            snap_goal = parent->coords.x1 + sleft;
            act = area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_END:
            snap_goal = parent->coords.x2 - sright;
            act = area_tmp->x2 + x_scroll;
            x_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_CENTER:
            snap_goal = parent->coords.x1 + sleft + parent_w / 2;
            act = lv_area_get_width(area_tmp) / 2 + area_tmp->x1 + x_scroll;
            x_scroll += snap_goal - act;
            अवरोध;
        हाल LV_SCROLL_SNAP_NONE:
            अवरोध;
    }

    /*Remove any pending scroll animations.*/
    lv_anim_delete(parent, scroll_y_anim);
    lv_anim_delete(parent, scroll_x_anim);

    अगर((scroll_dir & LV_DIR_बायाँ) == 0 && x_scroll < 0) x_scroll = 0;
    अगर((scroll_dir & LV_DIR_दायाँ) == 0 && x_scroll > 0) x_scroll = 0;
    अगर((scroll_dir & LV_DIR_शीर्ष) == 0 && y_scroll < 0) y_scroll = 0;
    अगर((scroll_dir & LV_DIR_BOTTOM) == 0 && y_scroll > 0) y_scroll = 0;

    scroll_मूल्य->x += anim_en ? x_scroll : 0;
    scroll_मूल्य->y += anim_en ? y_scroll : 0;
    lv_obj_scroll_by(parent, x_scroll, y_scroll, anim_en);
}
