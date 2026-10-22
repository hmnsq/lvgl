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
#تعریف_کړه MY_CLASS (&lv_obj_class)
#تعریف_کړه disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#تعریف_کړه OBJ_DUMP_STRING_LEN 128
#تعریف_کړه LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ساكن فراغ lv_obj_delete_async_cb(فراغ * obj);
ساكن فراغ obj_delete_core(lv_obj_t * obj);
ساكن lv_obj_ونه_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ونه_walk_cb_t cb, فراغ * user_معلومات);
ساكن فراغ dump_ونه_core(lv_obj_t * obj, int32_t ژوروالی);
ساكن lv_obj_t * lv_obj_get_لومړی_not_deleting_ماشوم(lv_obj_t * obj);
#إذا LV_USE_OBJ_NAME
    ساكن lv_obj_t * پیدا_کړه_by_name_مستقیم(ثابت lv_obj_t * پلار, ثابت حرف * name, اندازه_t len);
#د_که_پای /*LV_USE_OBJ_NAME*/

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

    lv_obj_t * par = lv_obj_get_پلار(obj);

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
        lv_obj_send_event(par, LV_EVENT_ماشوم_بدل_شوی, عدم);
        lv_obj_send_event(par, LV_EVENT_ماشوم_DELETED, عدم);
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

    uint32_t cnt = lv_obj_get_ماشوم_شمېر(obj);
    lv_obj_t * ماشوم = lv_obj_get_لومړی_not_deleting_ماشوم(obj);
    طالما(ماشوم) {
        obj_delete_core(ماشوم);
        ماشوم = lv_obj_get_لومړی_not_deleting_ماشوم(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_منزل(obj, 0, 0, LV_ANIM_OFF);
    إذا(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    إذا(lv_obj_get_ماشوم_شمېر(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_ماشوم_بدل_شوی, عدم);
        lv_obj_send_event(obj, LV_EVENT_ماشوم_DELETED, عدم);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (فراغ *)obj);
}

فراغ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    إذا(obj == عدم) أرجع;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سېټ_var(&a, obj);
    lv_anim_سېټ_exec_cb(&a, عدم);
    lv_anim_سېټ_duration(&a, 1);
    lv_anim_سېټ_delay(&a, delay_ms);
    lv_anim_سېټ_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_پیل(&a);
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

فراغ lv_obj_سېټ_پلار(lv_obj_t * obj, lv_obj_t * پلار)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_OBJ(پلار, MY_CLASS, أرجع);

    إذا(obj->پلار == عدم) {
        LV_LOG_WARN("Can't set the parent of a screen");
        أرجع;
    }

    إذا(پلار == obj->پلار) {
        أرجع;
    }

    إذا(!lv_obj_allocate_spec_attr(پلار)) {
        أرجع;
    }

    /*Add the object to the new parents list first*/
    إذا(lv_obj_جمع_کړه_ماشوم(پلار, obj) != LV_پایله_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        أرجع;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_ماشوم(obj->پلار, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->پلار);
    lv_obj_send_event(obj->پلار, LV_EVENT_ماشوم_بدل_شوی, obj);
    lv_obj_send_event(obj->پلار, LV_EVENT_ماشوم_DELETED, عدم);
    obj->پلار = پلار;

    /*Notify the new parent about the child*/
    lv_obj_send_event(پلار, LV_EVENT_ماشوم_بدل_شوی, obj);
    lv_obj_send_event(پلار, LV_EVENT_ماشوم_CREATED, عدم);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

فراغ lv_obj_move_منزل_شاخص(lv_obj_t * obj, int32_t شاخص)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);

    /* Check parent validity */
    lv_obj_t * پلار = lv_obj_get_پلار(obj);
    إذا(!پلار) {
        LV_LOG_WARN("parent is NULL");
        أرجع;
    }

    ثابت uint32_t پلار_ماشوم_شمېر = lv_obj_get_ماشوم_شمېر(پلار);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_شاخص = lv_obj_get_شاخص(obj);
    LV_ASSERT(0 <= old_شاخص);

    إذا(شاخص < 0) {
        شاخص += پلار_ماشوم_شمېر;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    إذا((شاخص < 0)
       /* Index is same or bigger than parent child count */
       || (شاخص >= (int32_t) پلار_ماشوم_شمېر)
       /* If both previous and new index are the same */
       || (شاخص == old_شاخص)) {

        أرجع;
    }

    int32_t i = old_شاخص;
    إذا(شاخص < old_شاخص) {
        طالما(i > شاخص)  {
            پلار->spec_attr->children[i] = پلار->spec_attr->children[i - 1];
            i--;
        }
    }
    وإلا {
        طالما(i < شاخص) {
            پلار->spec_attr->children[i] = پلار->spec_attr->children[i + 1];
            i++;
        }
    }

    پلار->spec_attr->children[شاخص] = obj;
    lv_obj_send_event(پلار, LV_EVENT_ماشوم_بدل_شوی, عدم);
    lv_obj_invalidate(پلار);
}

فراغ lv_obj_بدلون(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, أرجع);
    LV_CHECK_OBJ(obj2, MY_CLASS, أرجع);

    lv_obj_t * پلار = lv_obj_get_پلار(obj1);
    lv_obj_t * parent2 = lv_obj_get_پلار(obj2);

    uint_fast32_t index1 = lv_obj_get_شاخص(obj1);
    uint_fast32_t index2 = lv_obj_get_شاخص(obj2);

    lv_obj_send_event(parent2, LV_EVENT_ماشوم_DELETED, obj2);
    lv_obj_send_event(پلار, LV_EVENT_ماشوم_DELETED, obj1);

    پلار->spec_attr->children[index1] = obj2;
    obj2->پلار = پلار;

    parent2->spec_attr->children[index2] = obj1;
    obj1->پلار = parent2;

    lv_obj_send_event(پلار, LV_EVENT_ماشوم_بدل_شوی, obj2);
    lv_obj_send_event(پلار, LV_EVENT_ماشوم_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_ماشوم_بدل_شوی, obj1);
    lv_obj_send_event(parent2, LV_EVENT_ماشوم_CREATED, obj1);

    lv_obj_invalidate(پلار);

    إذا(پلار != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_ډله_بدلون_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    افعل {
        act_par = par;
        par = lv_obj_get_پلار(act_par);
    } طالما(par != عدم);

    أرجع (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    ثابت lv_obj_t * scr;

    إذا(obj->پلار == عدم) scr = obj;  /*`obj` is a screen*/
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

lv_obj_t * lv_obj_get_پلار(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    أرجع obj->پلار;
}

lv_obj_t * lv_obj_get_ماشوم(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    إذا(idx < 0) {
        idx = obj->spec_attr->ماشوم_cnt + idx;
        إذا(idx < 0) أرجع عدم;
    }

    uint32_t idu = (uint32_t) idx;

    إذا(idu < obj->spec_attr->ماشوم_cnt) أرجع obj->spec_attr->children[idu];
    وإلا أرجع عدم;
}

lv_obj_t * lv_obj_get_ماشوم_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ماشوم_cnt;
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
    LV_CHECK_ARG(obj->پلار != عدم, أرجع عدم);

    lv_obj_t * پلار = lv_obj_get_پلار(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_شاخص(obj) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_ماشوم(پلار, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(obj->پلار != عدم, أرجع عدم);
    LV_CHECK_ARG(class_p != عدم, أرجع عدم);

    lv_obj_t * پلار = lv_obj_get_پلار(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_شاخص_by_type(obj, class_p) + idx;
    إذا(sibling_idx < 0) أرجع عدم;

    أرجع lv_obj_get_ماشوم_by_type(پلار, sibling_idx, class_p);
}

uint32_t lv_obj_get_ماشوم_شمېر(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    إذا(obj->spec_attr == عدم) أرجع 0;
    أرجع obj->spec_attr->ماشوم_cnt;
}

uint32_t lv_obj_get_ماشوم_شمېر_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع 0);
    LV_CHECK_ARG(class_p != عدم, أرجع 0);

    إذا(obj->spec_attr == عدم) أرجع 0;

    uint32_t i;
    uint32_t cnt = 0;
    لكل(i = 0; i < obj->spec_attr->ماشوم_cnt; i++) {
        إذا(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    أرجع cnt;
}

#إذا LV_USE_OBJ_NAME

فراغ lv_obj_سېټ_name(lv_obj_t * obj, ثابت حرف * name)
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

فراغ lv_obj_سېټ_name_static(lv_obj_t * obj, ثابت حرف * name)
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

فراغ lv_obj_get_name_resolved(ثابت lv_obj_t * obj, حرف buf[], اندازه_t buf_اندازه)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع);
    LV_CHECK_ARG(buf != عدم && buf_اندازه > 0, أرجع);

    ثابت حرف * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    حرف name_buf[LV_OBJ_NAME_MAX_LEN];
    إذا(name == عدم) {
        lv_snprintf(name_buf, حجم(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    اندازه_t name_len = lv_strlen(name);
    lv_obj_t * پلار = lv_obj_get_پلار(obj);

    /*If the last character is # automatically index the children with the same name start*/
    إذا(پلار && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(پلار);
        uint32_t cnt = 0;
        uint32_t i;
        لكل(i = 0; i < ماشوم_cnt; i++) {
            lv_obj_t * ماشوم = lv_obj_get_ماشوم(پلار, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            إذا(ماشوم == obj) {
                حرف عدد_buf[8];
                اندازه_t عدد_len;
                عدد_len = lv_snprintf(عدد_buf, حجم(عدد_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                إذا(buf_اندازه > name_len + عدد_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], عدد_buf);
                }
                وإلا {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_اندازه);
                }
                اكسر;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            وإلا {
                ثابت حرف * ماشوم_name = lv_obj_get_name(ماشوم);
                إذا(ماشوم_name == عدم) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    اندازه_t class_name_len = lv_strlen(ماشوم->class_p->name);
                    إذا(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(ماشوم->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                وإلا {
                    إذا(lv_strcmp(ماشوم->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    وإلا {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_اندازه);
    }
}

lv_obj_t * lv_obj_get_ماشوم_by_name(ثابت lv_obj_t * پلار, ثابت حرف * لار)
{
    LV_CHECK_OBJ(پلار, MY_CLASS, أرجع عدم);
    LV_CHECK_ARG(لار != عدم, أرجع عدم);
    LV_CHECK_ARG(پلار->spec_attr != عدم, أرجع عدم);

    طالما(*لار) {
        ثابت حرف * segment = لار;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        طالما(لار[len] && لار[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * ماشوم = پیدا_کړه_by_name_مستقیم(پلار, segment, len);
        إذا(!ماشوم) أرجع عدم; /*Segment not found*/

        /* Advance to the next segment */
        لار += len;
        إذا(*لار == '/') لار++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        إذا(*لار == '\0') أرجع ماشوم;

        پلار = ماشوم;
    }

    أرجع عدم;

}

lv_obj_t * lv_obj_پیدا_کړه_by_name(ثابت lv_obj_t * پلار, ثابت حرف * name)
{
    LV_CHECK_ARG(name != عدم, أرجع عدم);

    إذا(پلار == عدم) پلار = lv_display_get_screen_فعال(lv_display_get_default());
    إذا(پلار == عدم) أرجع عدم;

    lv_obj_t * ماشوم = پیدا_کړه_by_name_مستقیم(پلار, name, UINT16_MAX);
    إذا(ماشوم) أرجع ماشوم;

    /*Search children recursively*/
    uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(پلار);
    uint32_t i;
    لكل(i = 0; i < ماشوم_cnt; i++) {
        ماشوم = پلار->spec_attr->children[i];
        lv_obj_t * وموندل = lv_obj_پیدا_کړه_by_name(ماشوم, name);
        إذا(وموندل != عدم) أرجع وموندل;
    }

    أرجع عدم;
}

#د_که_پای /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_شاخص(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);

    lv_obj_t * پلار = lv_obj_get_پلار(obj);
    إذا(پلار == عدم) أرجع -1;

    uint32_t i = 0;
    لكل(i = 0; i < پلار->spec_attr->ماشوم_cnt; i++) {
        إذا(پلار->spec_attr->children[i] == obj) أرجع (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    أرجع -1;
}

int32_t lv_obj_get_شاخص_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, أرجع -1);
    LV_CHECK_ARG(class_p != عدم, أرجع -1);

    lv_obj_t * پلار = lv_obj_get_پلار(obj);
    إذا(پلار == عدم) أرجع 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    لكل(i = 0; i < پلار->spec_attr->ماشوم_cnt; i++) {
        lv_obj_t * ماشوم = پلار->spec_attr->children[i];
        إذا(ماشوم->class_p == class_p) {
            إذا(ماشوم == obj) أرجع idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    أرجع -1;
}

فراغ lv_obj_ونه_walk(lv_obj_t * پیل_obj, lv_obj_ونه_walk_cb_t cb, فراغ * user_معلومات)
{
    LV_CHECK_ARG(cb != عدم, أرجع);

    walk_core(پیل_obj, cb, user_معلومات);
}

فراغ lv_obj_dump_ونه(lv_obj_t * پیل_obj)
{
    إذا(پیل_obj == عدم) {
        lv_display_t * disp = lv_display_get_بل(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                dump_ونه_core(disp->screens[i], 0);
            }
            disp = lv_display_get_بل(disp);
        }
    }
    وإلا {
        dump_ونه_core(پیل_obj, 0);
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
    lv_پایله_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, عدم);
    إذا(res == LV_پایله_INVALID) {
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
    lv_obj_t * ماشوم = lv_obj_get_ماشوم(obj, 0);
    طالما(ماشوم) {
        obj_delete_core(ماشوم);
        ماشوم = lv_obj_get_ماشوم(obj, 0);
    }

    lv_ډله_t * ډله = lv_obj_get_ډله(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_بل(عدم);
    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        إذا(indev_type == LV_INDEV_TYPE_اشاره_ګر || indev_type == LV_INDEV_TYPE_BUTTON) {
            إذا(indev->اشاره_ګر.act_obj == obj || indev->اشاره_ګر.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            إذا(indev->اشاره_ګر.وروستی_pressed == obj) {
                indev->اشاره_ګر.وروستی_pressed = عدم;
            }
            إذا(indev->اشاره_ګر.وروستی_hovered == obj) {
                indev->اشاره_ګر.وروستی_hovered = عدم;
            }
        }

        إذا(indev->ډله == ډله && obj == lv_indev_get_فعال_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_بل(indev);
    }

    /*Delete all pending async del-s*/
    lv_پایله_t async_cancel_res = LV_پایله_OK;
    طالما(async_cancel_res == LV_پایله_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    إذا(obj->پلار == عدم) {
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
        lv_obj_remove_ماشوم(obj->پلار, obj);
    }

    obj->پلار = عدم;

    /*Free the object itself*/
    lv_free(obj);
}

ساكن lv_obj_ونه_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ونه_walk_cb_t cb, فراغ * user_معلومات)
{
    lv_obj_ونه_walk_res_t res = LV_OBJ_ونه_WALK_بل;

    إذا(obj == عدم) {
        lv_display_t * disp = lv_display_get_بل(عدم);
        طالما(disp) {
            uint32_t i;
            لكل(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_معلومات);
            }
            disp = lv_display_get_بل(disp);
        }
        أرجع LV_OBJ_ونه_WALK_پای_ټکی;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_معلومات);

    إذا(res == LV_OBJ_ونه_WALK_پای_ټکی) أرجع LV_OBJ_ونه_WALK_پای_ټکی;

    إذا(res != LV_OBJ_ونه_WALK_SKIP_CHILDREN) {
        uint32_t i;
        لكل(i = 0; i < lv_obj_get_ماشوم_شمېر(obj); i++) {
            res = walk_core(lv_obj_get_ماشوم(obj, i), cb, user_معلومات);
            إذا(res == LV_OBJ_ونه_WALK_پای_ټکی) أرجع LV_OBJ_ونه_WALK_پای_ټکی;
        }
    }
    أرجع LV_OBJ_ونه_WALK_بل;
}

ساكن فراغ dump_ونه_core(lv_obj_t * obj, int32_t ژوروالی)
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
#د_که_پای

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحيح)(2 * ژوروالی), "", (فراغ *)obj, id);
#د_که_پای /*LV_USE_LOG*/

    إذا(obj->spec_attr && obj->spec_attr->ماشوم_cnt) {
        لكل(uint32_t i = 0; i < obj->spec_attr->ماشوم_cnt; i++) {
            dump_ونه_core(lv_obj_get_ماشوم(obj, i), ژوروالی + 1);
        }
    }
}

ساكن lv_obj_t * lv_obj_get_لومړی_not_deleting_ماشوم(lv_obj_t * obj)
{
    LV_ASSERT(obj != عدم);

    إذا(obj->spec_attr == عدم) أرجع عدم;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ماشوم_cnt;
    لكل(i = 0; i < cnt; i++) {
        إذا(!obj->spec_attr->children[i]->is_deleting) {
            أرجع obj->spec_attr->children[i];
        }
    }

    أرجع عدم;
}

#إذا LV_USE_OBJ_NAME

ساكن lv_obj_t * پیدا_کړه_by_name_مستقیم(ثابت lv_obj_t * پلار, ثابت حرف * name, اندازه_t len)
{
    uint32_t i;
    uint32_t ماشوم_cnt = lv_obj_get_ماشوم_شمېر(پلار);
    لكل(i = 0; i < ماشوم_cnt; i++) {
        lv_obj_t * ماشوم = پلار->spec_attr->children[i];

        حرف ماشوم_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(ماشوم, ماشوم_name_resolved, حجم(ماشوم_name_resolved));

        إذا(len == UINT16_MAX) {
            إذا(lv_strcmp(ماشوم_name_resolved, name) == 0) أرجع ماشوم;
        }
        وإلا {
            إذا(lv_strncmp(ماشوم_name_resolved, name, len) == 0 &&
               ماشوم_name_resolved[len] == '\0') {
                أرجع ماشوم;
            }
        }
    }

    أرجع عدم;
}

#د_که_پای /*LV_USE_OBJ_NAME*/
