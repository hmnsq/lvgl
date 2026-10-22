/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_obj_private.h"
#ضمن "../lvgl_public.h"
#ضمن "lv_obj_class_private.h"
#ضمن "../indev/lv_indev_private.h"
#ضمن "../display/lv_display_private.h"
#ضمن "../misc/lv_anim_private.h"
#ضمن "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#تعریف_کرو MY_CLASS (&lv_obj_class)
#تعریف_کرو disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#تعریف_کرو OBJ_DUMP_STRING_LEN 128
#تعریف_کرو LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_obj_delete_async_cb(فراغ * obj);
ساكن فراغ obj_delete_core(lv_obj_t * obj);
ساكن lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, فراغ * user_انکڑے);
ساكن فراغ dump_درخت_core(lv_obj_t * obj, int32_t ڈونگھائی);
ساكن lv_obj_t * lv_obj_get_پہلا_not_deleting_بچہ(lv_obj_t * obj);
#إذا LV_USE_OBJ_NAME
    ساكن lv_obj_t * لبھو_by_name_سدھا(ثابت lv_obj_t * ماپا, ثابت حرف * name, ناپ_t len);
#جے_انت /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_obj_delete(lv_obj_t * obj)
{
    إذا(obj == عدم) أرجع;
    إذا(obj->is_deleting) أرجع;

    LV_LOG_TRACE("begin (delete %p)", (فراغ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_ماپا(obj);

    lv_display_t * disp = عدم;
    bool act_screen_del = false;
    إذا(par == عدم) {
        disp = lv_obj_get_display(obj);
        إذا(!disp) أرجع;   /*Shouldn't happen*/
        إذا(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    إذا(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_بچہ_بدلیا, عدم);
        lv_obj_send_event(par, LV_EVENT_بچہ_DELETED, عدم);
    }

    /*Handle if the active screen was deleted*/
    إذا(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = عدم;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (فراغ *)obj);
}

فراغ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (فراغ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_بچہ_گنتی(obj);
    lv_obj_t * بچہ = lv_obj_get_پہلا_not_deleting_بچہ(obj);
    طالما(بچہ) {
        obj_delete_core(بچہ);
        بچہ = lv_obj_get_پہلا_not_deleting_بچہ(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_منزل(obj, 0, 0, LV_ANIM_OFF);
    إذا(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    إذا(lv_obj_get_بچہ_گنتی(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_بچہ_بدلیا, عدم);
        lv_obj_send_event(obj, LV_EVENT_بچہ_DELETED, عدم);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (فراغ *)obj);
}

فراغ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    إذا(obj == عدم) أرجع;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سیٹ_var(&a, obj);
    lv_anim_سیٹ_exec_cb(&a, عدم);
    lv_anim_سیٹ_duration(&a, 1);
    lv_anim_سیٹ_delay(&a, delay_ms);
    lv_anim_سیٹ_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_شروع(&a);
}

فراغ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != عدم, أرجع);

    lv_obj_delete(a->var);
}

فراغ lv_obj_delete_async(lv_obj_t * obj)
{
    إذا(obj == عدم) أرجع;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

فراغ lv_obj_سیٹ_ماپا(lv_obj_t * obj, lv_obj_t * ماپا)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_OBJ(ماپا, MY_CLASS, أرجع);

    إذا(obj->ماپا == عدم) {
        LV_LOG_WARN("Can't set the parent of a screen");
        أرجع;
    }

    إذا(ماپا == obj->ماپا) {
        أرجع;
    }

    إذا(!lv_obj_allocate_spec_attr(ماپا)) {
        أرجع;
    }

    /*Add the object to the new parents list first*/
    إذا(lv_obj_جوڑو_بچہ(ماپا, obj) != LV_نتیجہ_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        أرجع;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_بچہ(obj->ماپا, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->ماپا);
    lv_obj_send_event(obj->ماپا, LV_EVENT_بچہ_بدلیا, obj);
    lv_obj_send_event(obj->ماپا, LV_EVENT_بچہ_DELETED, عدم);
    obj->ماپا = ماپا;

    /*Notify the new parent about the child*/
    lv_obj_send_event(ماپا, LV_EVENT_بچہ_بدلیا, obj);
    lv_obj_send_event(ماپا, LV_EVENT_بچہ_CREATED, عدم);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

فراغ lv_obj_move_منزل_اشاریہ(lv_obj_t * obj, int32_t اشاریہ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /* Check parent validity */
    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
    إذا(!ماپا) {
        LV_LOG_WARN("parent is NULL");
        أرجع;
    }

    ثابت uint32_t ماپا_بچہ_گنتی = lv_obj_get_بچہ_گنتی(ماپا);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_اشاریہ = lv_obj_get_اشاریہ(obj);
    LV_ASSERT(0 <= old_اشاریہ);

    إذا(اشاریہ < 0) {
        اشاریہ += ماپا_بچہ_گنتی;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    إذا((اشاریہ < 0)
       /* Index is same or bigger than parent child count */
       || (اشاریہ >= (int32_t) ماپا_بچہ_گنتی)
       /* If both previous and new index are the same */
       || (اشاریہ == old_اشاریہ)) {

        أرجع;
    }

    int32_t i = old_اشاریہ;
    إذا(اشاریہ < old_اشاریہ) {
        طالما(i > اشاریہ)  {
            ماپا->spec_attr->children[i] = ماپا->spec_attr->children[i - 1];
            i--;
        }
    }
    وإلا {
        طالما(i < اشاریہ) {
            ماپا->spec_attr->children[i] = ماپا->spec_attr->children[i + 1];
            i++;
        }
    }

    ماپا->spec_attr->children[اشاریہ] = obj;
    lv_obj_send_event(ماپا, LV_EVENT_بچہ_بدلیا, عدم);
    lv_obj_invalidate(ماپا);
}

فراغ lv_obj_وٹاندرا(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, أرجع);
    LV_CHECK_OBJ(obj2, MY_CLASS, أرجع);

    lv_obj_t * ماپا = lv_obj_get_ماپا(obj1);
    lv_obj_t * parent2 = lv_obj_get_ماپا(obj2);

    uint_fast32_t index1 = lv_obj_get_اشاریہ(obj1);
    uint_fast32_t index2 = lv_obj_get_اشاریہ(obj2);

    lv_obj_send_event(parent2, LV_EVENT_بچہ_DELETED, obj2);
    lv_obj_send_event(ماپا, LV_EVENT_بچہ_DELETED, obj1);

    ماپا->spec_attr->children[index1] = obj2;
    obj2->ماپا = ماپا;

    parent2->spec_attr->children[index2] = obj1;
    obj1->ماپا = parent2;

    lv_obj_send_event(ماپا, LV_EVENT_بچہ_بدلیا, obj2);
    lv_obj_send_event(ماپا, LV_EVENT_بچہ_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_بچہ_بدلیا, obj1);
    lv_obj_send_event(parent2, LV_EVENT_بچہ_CREATED, obj1);

    lv_obj_invalidate(ماپا);

    إذا(ماپا != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_گروہ_وٹاندرا_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    افعل {
        act_par = par;
        par = lv_obj_get_ماپا(act_par);
    } طالما(par != عدم);

    أرجع (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * scr;

    إذا(obj->ماپا == عدم) scr = obj;  /*`obj` is a screen*/
    وإلا scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_سر = disp_ll_p;
    LV_LL_READ(disp_سر, d) {
        uint32_t i;
        لكل(i = 0; i < d->screen_cnt; i++) {
            إذا(d->screens[i] == scr) أرجع d;
        }
    }

    LV_LOG_WARN("No screen found");
    أرجع عدم;
}

lv_obj_t * lv_obj_get_ماپا(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    أرجع obj->ماپا;
}

lv_obj_t * lv_obj_get_بچہ(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    إذا(idx < 0) {
        idx = obj->spec_attr->بچہ_cnt + idx;
        إذا(idx < 0) أرجع عدم;
    }

    uint32_t idu = (uint32_t) idx;

    إذا(idu < obj->spec_attr->بچہ_cnt) أرجع obj->spec_attr->children[idu];
    وإلا أرجع عدم;
}

lv_obj_t * lv_obj_get_بچہ_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->بچہ_cnt;
    إذا(idx >= 0) {
        لكل(i = 0; i < cnt; i++) {
            إذا(obj->spec_attr->children[i]->class_p == class_p) {
                إذا(idx == 0) أرجع obj->spec_attr->children[i];
                وإلا idx--;
            }
        }
    }
    وإلا {
        idx++;   /*-1 means the first child*/
        لكل(i = cnt - 1; i >= 0; i--) {
            إذا(obj->spec_attr->children[i]->class_p == class_p) {
                إذا(idx == 0) أرجع obj->spec_attr->children[i];
                وإلا idx++;
            }
        }
    }
    أرجع عدم;
}

lv_obj_t * lv_obj_get_sibling(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(obj->ماپا != عدم, أرجع عدم);

    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_اشاریہ(obj) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_بچہ(ماپا, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(obj->ماپا != عدم, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_اشاریہ_by_type(obj, class_p) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_بچہ_by_type(ماپا, sibling_idx, class_p);
}

uint32_t lv_obj_get_بچہ_گنتی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع obj->spec_attr->بچہ_cnt;
}

uint32_t lv_obj_get_بچہ_گنتی_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    LV_CHECK_ARG(class_p != عدم, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;

    uint32_t i;
    uint32_t cnt = 0;
    لكل(i = 0; i < obj->spec_attr->بچہ_cnt; i++) {
        إذا(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    أرجع cnt;
}

#إذا LV_USE_OBJ_NAME

فراغ lv_obj_سیٹ_name(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    إذا(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    إذا(name == عدم) {
        obj->spec_attr->name = عدم;
        obj->spec_attr->name_static = 1;
    }
    وإلا {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

فراغ lv_obj_سیٹ_name_static(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(name != عدم, أرجع);

    إذا(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }
    إذا(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((فراغ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ثابت حرف * lv_obj_get_name(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;
    وإلا أرجع obj->spec_attr->name;
}

فراغ lv_obj_get_name_resolved(ثابت lv_obj_t * obj, حرف buf[], ناپ_t buf_ناپ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(buf != عدم && buf_ناپ > 0, أرجع);

    ثابت حرف * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    حرف name_buf[LV_OBJ_NAME_MAX_LEN];
    إذا(name == عدم) {
        lv_snprintf(name_buf, حجم(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    ناپ_t name_len = lv_strlen(name);
    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);

    /*If the last character is # automatically index the children with the same name start*/
    إذا(ماپا && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(ماپا);
        uint32_t cnt = 0;
        uint32_t i;
        لكل(i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = lv_obj_get_بچہ(ماپا, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            إذا(بچہ == obj) {
                حرف عدد_buf[8];
                ناپ_t عدد_len;
                عدد_len = lv_snprintf(عدد_buf, حجم(عدد_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                إذا(buf_ناپ > name_len + عدد_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], عدد_buf);
                }
                وإلا {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_ناپ);
                }
                اكسر;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            وإلا {
                ثابت حرف * بچہ_name = lv_obj_get_name(بچہ);
                إذا(بچہ_name == عدم) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    ناپ_t class_name_len = lv_strlen(بچہ->class_p->name);
                    إذا(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(بچہ->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                وإلا {
                    إذا(lv_strcmp(بچہ->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    وإلا {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_ناپ);
    }
}

lv_obj_t * lv_obj_get_بچہ_by_name(ثابت lv_obj_t * ماپا, ثابت حرف * رستہ)
{
    LV_CHECK_OBJ(ماپا, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(رستہ != عدم, أرجع عدم);
    LV_CHECK_ARG(ماپا->spec_attr != عدم, أرجع عدم);

    طالما(*رستہ) {
        ثابت حرف * segment = رستہ;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        طالما(رستہ[len] && رستہ[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * بچہ = لبھو_by_name_سدھا(ماپا, segment, len);
        إذا(!بچہ) أرجع عدم; /*Segment not found*/

        /* Advance to the next segment */
        رستہ += len;
        إذا(*رستہ == '/') رستہ++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        إذا(*رستہ == '\0') أرجع بچہ;

        ماپا = بچہ;
    }

    أرجع عدم;

}

lv_obj_t * lv_obj_لبھو_by_name(ثابت lv_obj_t * ماپا, ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع عدم);

    إذا(ماپا == عدم) ماپا = lv_display_get_screen_سرگرم(lv_display_get_default());
    إذا(ماپا == عدم) أرجع عدم;

    lv_obj_t * بچہ = لبھو_by_name_سدھا(ماپا, name, UINT16_MAX);
    إذا(بچہ) أرجع بچہ;

    /*Search children recursively*/
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(ماپا);
    uint32_t i;
    لكل(i = 0; i < بچہ_cnt; i++) {
        بچہ = ماپا->spec_attr->children[i];
        lv_obj_t * لبھیا = lv_obj_لبھو_by_name(بچہ, name);
        إذا(لبھیا != عدم) أرجع لبھیا;
    }

    أرجع عدم;
}

#جے_انت /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_اشاریہ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);

    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
    إذا(ماپا == عدم) أرجع -1;

    uint32_t i = 0;
    لكل(i = 0; i < ماپا->spec_attr->بچہ_cnt; i++) {
        إذا(ماپا->spec_attr->children[i] == obj) أرجع (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    أرجع -1;
}

int32_t lv_obj_get_اشاریہ_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);
    LV_CHECK_ARG(class_p != عدم, أرجع -1);

    lv_obj_t * ماپا = lv_obj_get_ماپا(obj);
    إذا(ماپا == عدم) أرجع 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    لكل(i = 0; i < ماپا->spec_attr->بچہ_cnt; i++) {
        lv_obj_t * بچہ = ماپا->spec_attr->children[i];
        إذا(بچہ->class_p == class_p) {
            إذا(بچہ == obj) أرجع idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    أرجع -1;
}

فراغ lv_obj_درخت_walk(lv_obj_t * شروع_obj, lv_obj_درخت_walk_cb_t cb, فراغ * user_انکڑے)
{
    LV_CHECK_ARG(cb != عدم, أرجع);

    walk_core(شروع_obj, cb, user_انکڑے);
}

فراغ lv_obj_dump_درخت(lv_obj_t * شروع_obj)
{
    إذا(شروع_obj == عدم) {
        lv_display_t * disp = lv_display_get_اگلا(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                dump_درخت_core(disp->screens[i], 0);
            }
            disp = lv_display_get_اگلا(disp);
        }
    }
    وإلا {
        dump_درخت_core(شروع_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_obj_delete_async_cb(فراغ * obj)
{
    lv_obj_delete(obj);
}

ساكن فراغ obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    إذا(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ساكن فراغ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);
    إذا(obj->is_deleting)
        أرجع;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, عدم);
    إذا(res == LV_نتیجہ_INVALID) {
        obj->is_deleting = false;
        أرجع;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    إذا(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    إذا(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * بچہ = lv_obj_get_بچہ(obj, 0);
    طالما(بچہ) {
        obj_delete_core(بچہ);
        بچہ = lv_obj_get_بچہ(obj, 0);
    }

    lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_اگلا(عدم);
    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        إذا(indev_type == LV_INDEV_TYPE_اشارہ_گر || indev_type == LV_INDEV_TYPE_BUTTON) {
            إذا(indev->اشارہ_گر.act_obj == obj || indev->اشارہ_گر.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            إذا(indev->اشارہ_گر.آخری_pressed == obj) {
                indev->اشارہ_گر.آخری_pressed = عدم;
            }
            إذا(indev->اشارہ_گر.آخری_hovered == obj) {
                indev->اشارہ_گر.آخری_hovered = عدم;
            }
        }

        إذا(indev->گروہ == گروہ && obj == lv_indev_get_سرگرم_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_اگلا(indev);
    }

    /*Delete all pending async del-s*/
    lv_نتیجہ_t async_cancel_res = LV_نتیجہ_OK;
    طالما(async_cancel_res == LV_نتیجہ_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    إذا(obj->ماپا == عدم) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        لكل(i = 0; i < disp->screen_cnt; i++) {
            إذا(disp->screens[i] == obj) اكسر;
        }

        uint32_t id = i;
        لكل(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * حجم(lv_obj_t *));
        إذا(new_screens != عدم) {
            disp->screens = new_screens;
        }
        وإلا {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    وإلا {
        lv_obj_remove_بچہ(obj->ماپا, obj);
    }

    obj->ماپا = عدم;

    /*Free the object itself*/
    lv_free(obj);
}

ساكن lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, فراغ * user_انکڑے)
{
    lv_obj_درخت_walk_res_t res = LV_OBJ_درخت_WALK_اگلا;

    إذا(obj == عدم) {
        lv_display_t * disp = lv_display_get_اگلا(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_انکڑے);
            }
            disp = lv_display_get_اگلا(disp);
        }
        أرجع LV_OBJ_درخت_WALK_سرا;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_انکڑے);

    إذا(res == LV_OBJ_درخت_WALK_سرا) أرجع LV_OBJ_درخت_WALK_سرا;

    إذا(res != LV_OBJ_درخت_WALK_SKIP_CHILDREN) {
        uint32_t i;
        لكل(i = 0; i < lv_obj_get_بچہ_گنتی(obj); i++) {
            res = walk_core(lv_obj_get_بچہ(obj, i), cb, user_انکڑے);
            إذا(res == LV_OBJ_درخت_WALK_سرا) أرجع LV_OBJ_درخت_WALK_سرا;
        }
    }
    أرجع LV_OBJ_درخت_WALK_اگلا;
}

ساكن فراغ dump_درخت_core(lv_obj_t * obj, int32_t ڈونگھائی)
{
    LV_ASSERT(obj != عدم);
#إذا LV_USE_LOG
    ثابت حرف * id;

#إذا LV_USE_OBJ_ID
    حرف buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, حجم(buf));
    إذا(id == عدم) id = "obj0";
#وإلا
    id = "obj0";
#جے_انت

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحيح)(2 * ڈونگھائی), "", (فراغ *)obj, id);
#جے_انت /*LV_USE_LOG*/

    إذا(obj->spec_attr && obj->spec_attr->بچہ_cnt) {
        لكل(uint32_t i = 0; i < obj->spec_attr->بچہ_cnt; i++) {
            dump_درخت_core(lv_obj_get_بچہ(obj, i), ڈونگھائی + 1);
        }
    }
}

ساكن lv_obj_t * lv_obj_get_پہلا_not_deleting_بچہ(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->بچہ_cnt;
    لكل(i = 0; i < cnt; i++) {
        إذا(!obj->spec_attr->children[i]->is_deleting) {
            أرجع obj->spec_attr->children[i];
        }
    }

    أرجع عدم;
}

#إذا LV_USE_OBJ_NAME

ساكن lv_obj_t * لبھو_by_name_سدھا(ثابت lv_obj_t * ماپا, ثابت حرف * name, ناپ_t len)
{
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(ماپا);
    لكل(i = 0; i < بچہ_cnt; i++) {
        lv_obj_t * بچہ = ماپا->spec_attr->children[i];

        حرف بچہ_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(بچہ, بچہ_name_resolved, حجم(بچہ_name_resolved));

        إذا(len == UINT16_MAX) {
            إذا(lv_strcmp(بچہ_name_resolved, name) == 0) أرجع بچہ;
        }
        وإلا {
            إذا(lv_strncmp(بچہ_name_resolved, name, len) == 0 &&
               بچہ_name_resolved[len] == '\0') {
                أرجع بچہ;
            }
        }
    }

    أرجع عدم;
}

#جے_انت /*LV_USE_OBJ_NAME*/
