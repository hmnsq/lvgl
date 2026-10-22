/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_obj_private.h"
#شامل "../lvgl_public.h"
#شامل "lv_obj_class_private.h"
#شامل "../indev/lv_indev_private.h"
#شامل "../display/lv_display_private.h"
#شامل "../misc/lv_anim_private.h"
#شامل "../core/lv_global.h"

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
ساکن خالی lv_obj_delete_async_cb(خالی * obj);
ساکن خالی obj_delete_core(lv_obj_t * obj);
ساکن lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, خالی * user_ڈیٹا);
ساکن خالی dump_درخت_core(lv_obj_t * obj, int32_t گہرائی);
ساکن lv_obj_t * lv_obj_get_پہلا_not_deleting_بچہ(lv_obj_t * obj);
#اگر LV_USE_OBJ_NAME
    ساکن lv_obj_t * ڈھونڈو_by_name_براہ_راست(ثابت lv_obj_t * والد, ثابت حرف * name, سائز_t len);
#اگر_ختم /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

خالی lv_obj_delete(lv_obj_t * obj)
{
    اگر(obj == تہی) واپس;
    اگر(obj->is_deleting) واپس;

    LV_LOG_TRACE("begin (delete %p)", (خالی *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_والد(obj);

    lv_display_t * disp = تہی;
    bool act_screen_del = false;
    اگر(par == تہی) {
        disp = lv_obj_get_display(obj);
        اگر(!disp) واپس;   /*Shouldn't happen*/
        اگر(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    اگر(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_بچہ_بدلا, تہی);
        lv_obj_send_event(par, LV_EVENT_بچہ_DELETED, تہی);
    }

    /*Handle if the active screen was deleted*/
    اگر(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = تہی;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (خالی *)obj);
}

خالی lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (خالی *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_بچہ_گنتی(obj);
    lv_obj_t * بچہ = lv_obj_get_پہلا_not_deleting_بچہ(obj);
    جب(بچہ) {
        obj_delete_core(بچہ);
        بچہ = lv_obj_get_پہلا_not_deleting_بچہ(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_مقصد(obj, 0, 0, LV_ANIM_OFF);
    اگر(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    اگر(lv_obj_get_بچہ_گنتی(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_بچہ_بدلا, تہی);
        lv_obj_send_event(obj, LV_EVENT_بچہ_DELETED, تہی);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (خالی *)obj);
}

خالی lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    اگر(obj == تہی) واپس;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_سیٹ_var(&a, obj);
    lv_anim_سیٹ_exec_cb(&a, تہی);
    lv_anim_سیٹ_duration(&a, 1);
    lv_anim_سیٹ_delay(&a, delay_ms);
    lv_anim_سیٹ_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_آغاز(&a);
}

خالی lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != تہی, واپس);

    lv_obj_delete(a->var);
}

خالی lv_obj_delete_async(lv_obj_t * obj)
{
    اگر(obj == تہی) واپس;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

خالی lv_obj_سیٹ_والد(lv_obj_t * obj, lv_obj_t * والد)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_OBJ(والد, MY_CLASS, واپس);

    اگر(obj->والد == تہی) {
        LV_LOG_WARN("Can't set the parent of a screen");
        واپس;
    }

    اگر(والد == obj->والد) {
        واپس;
    }

    اگر(!lv_obj_allocate_spec_attr(والد)) {
        واپس;
    }

    /*Add the object to the new parents list first*/
    اگر(lv_obj_جوڑو_بچہ(والد, obj) != LV_نتیجہ_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        واپس;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_بچہ(obj->والد, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->والد);
    lv_obj_send_event(obj->والد, LV_EVENT_بچہ_بدلا, obj);
    lv_obj_send_event(obj->والد, LV_EVENT_بچہ_DELETED, تہی);
    obj->والد = والد;

    /*Notify the new parent about the child*/
    lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, obj);
    lv_obj_send_event(والد, LV_EVENT_بچہ_CREATED, تہی);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

خالی lv_obj_move_مقصد_اشاریہ(lv_obj_t * obj, int32_t اشاریہ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    /* Check parent validity */
    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(!والد) {
        LV_LOG_WARN("parent is NULL");
        واپس;
    }

    ثابت uint32_t والد_بچہ_گنتی = lv_obj_get_بچہ_گنتی(والد);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ثابت int32_t old_اشاریہ = lv_obj_get_اشاریہ(obj);
    LV_ASSERT(0 <= old_اشاریہ);

    اگر(اشاریہ < 0) {
        اشاریہ += والد_بچہ_گنتی;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    اگر((اشاریہ < 0)
       /* Index is same or bigger than parent child count */
       || (اشاریہ >= (int32_t) والد_بچہ_گنتی)
       /* If both previous and new index are the same */
       || (اشاریہ == old_اشاریہ)) {

        واپس;
    }

    int32_t i = old_اشاریہ;
    اگر(اشاریہ < old_اشاریہ) {
        جب(i > اشاریہ)  {
            والد->spec_attr->children[i] = والد->spec_attr->children[i - 1];
            i--;
        }
    }
    ورنہ {
        جب(i < اشاریہ) {
            والد->spec_attr->children[i] = والد->spec_attr->children[i + 1];
            i++;
        }
    }

    والد->spec_attr->children[اشاریہ] = obj;
    lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, تہی);
    lv_obj_invalidate(والد);
}

خالی lv_obj_تبادلہ(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, واپس);
    LV_CHECK_OBJ(obj2, MY_CLASS, واپس);

    lv_obj_t * والد = lv_obj_get_والد(obj1);
    lv_obj_t * parent2 = lv_obj_get_والد(obj2);

    uint_fast32_t index1 = lv_obj_get_اشاریہ(obj1);
    uint_fast32_t index2 = lv_obj_get_اشاریہ(obj2);

    lv_obj_send_event(parent2, LV_EVENT_بچہ_DELETED, obj2);
    lv_obj_send_event(والد, LV_EVENT_بچہ_DELETED, obj1);

    والد->spec_attr->children[index1] = obj2;
    obj2->والد = والد;

    parent2->spec_attr->children[index2] = obj1;
    obj1->والد = parent2;

    lv_obj_send_event(والد, LV_EVENT_بچہ_بدلا, obj2);
    lv_obj_send_event(والد, LV_EVENT_بچہ_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_بچہ_بدلا, obj1);
    lv_obj_send_event(parent2, LV_EVENT_بچہ_CREATED, obj1);

    lv_obj_invalidate(والد);

    اگر(والد != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_گروہ_تبادلہ_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    ثابت lv_obj_t * par = obj;
    ثابت lv_obj_t * act_par;

    کرو {
        act_par = par;
        par = lv_obj_get_والد(act_par);
    } جب(par != تہی);

    واپس (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    ثابت lv_obj_t * scr;

    اگر(obj->والد == تہی) scr = obj;  /*`obj` is a screen*/
    ورنہ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_سر = disp_ll_p;
    LV_LL_READ(disp_سر, d) {
        uint32_t i;
        برائے(i = 0; i < d->screen_cnt; i++) {
            اگر(d->screens[i] == scr) واپس d;
        }
    }

    LV_LOG_WARN("No screen found");
    واپس تہی;
}

lv_obj_t * lv_obj_get_والد(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    واپس obj->والد;
}

lv_obj_t * lv_obj_get_بچہ(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    اگر(obj->spec_attr == تہی) واپس تہی;

    اگر(idx < 0) {
        idx = obj->spec_attr->بچہ_cnt + idx;
        اگر(idx < 0) واپس تہی;
    }

    uint32_t idu = (uint32_t) idx;

    اگر(idu < obj->spec_attr->بچہ_cnt) واپس obj->spec_attr->children[idu];
    ورنہ واپس تہی;
}

lv_obj_t * lv_obj_get_بچہ_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);
    LV_CHECK_ARG(class_p != تہی, واپس تہی);

    اگر(obj->spec_attr == تہی) واپس تہی;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->بچہ_cnt;
    اگر(idx >= 0) {
        برائے(i = 0; i < cnt; i++) {
            اگر(obj->spec_attr->children[i]->class_p == class_p) {
                اگر(idx == 0) واپس obj->spec_attr->children[i];
                ورنہ idx--;
            }
        }
    }
    ورنہ {
        idx++;   /*-1 means the first child*/
        برائے(i = cnt - 1; i >= 0; i--) {
            اگر(obj->spec_attr->children[i]->class_p == class_p) {
                اگر(idx == 0) واپس obj->spec_attr->children[i];
                ورنہ idx++;
            }
        }
    }
    واپس تہی;
}

lv_obj_t * lv_obj_get_sibling(ثابت lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);
    LV_CHECK_ARG(obj->والد != تہی, واپس تہی);

    lv_obj_t * والد = lv_obj_get_والد(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_اشاریہ(obj) + idx;
    اگر(sibling_idx < 0) واپس تہی;

    واپس lv_obj_get_بچہ(والد, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ثابت lv_obj_t * obj, int32_t idx, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);
    LV_CHECK_ARG(obj->والد != تہی, واپس تہی);
    LV_CHECK_ARG(class_p != تہی, واپس تہی);

    lv_obj_t * والد = lv_obj_get_والد(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_اشاریہ_by_type(obj, class_p) + idx;
    اگر(sibling_idx < 0) واپس تہی;

    واپس lv_obj_get_بچہ_by_type(والد, sibling_idx, class_p);
}

uint32_t lv_obj_get_بچہ_گنتی(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);
    اگر(obj->spec_attr == تہی) واپس 0;
    واپس obj->spec_attr->بچہ_cnt;
}

uint32_t lv_obj_get_بچہ_گنتی_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس 0);
    LV_CHECK_ARG(class_p != تہی, واپس 0);

    اگر(obj->spec_attr == تہی) واپس 0;

    uint32_t i;
    uint32_t cnt = 0;
    برائے(i = 0; i < obj->spec_attr->بچہ_cnt; i++) {
        اگر(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    واپس cnt;
}

#اگر LV_USE_OBJ_NAME

خالی lv_obj_سیٹ_name(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }

    اگر(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((خالی *)obj->spec_attr->name);

    اگر(name == تہی) {
        obj->spec_attr->name = تہی;
        obj->spec_attr->name_static = 1;
    }
    ورنہ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

خالی lv_obj_سیٹ_name_static(lv_obj_t * obj, ثابت حرف * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(name != تہی, واپس);

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        واپس;
    }
    اگر(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((خالی *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ثابت حرف * lv_obj_get_name(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس تہی);

    اگر(obj->spec_attr == تہی) واپس تہی;
    ورنہ واپس obj->spec_attr->name;
}

خالی lv_obj_get_name_resolved(ثابت lv_obj_t * obj, حرف buf[], سائز_t buf_سائز)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس);
    LV_CHECK_ARG(buf != تہی && buf_سائز > 0, واپس);

    ثابت حرف * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    حرف name_buf[LV_OBJ_NAME_MAX_LEN];
    اگر(name == تہی) {
        lv_snprintf(name_buf, حجم(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    سائز_t name_len = lv_strlen(name);
    lv_obj_t * والد = lv_obj_get_والد(obj);

    /*If the last character is # automatically index the children with the same name start*/
    اگر(والد && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(والد);
        uint32_t cnt = 0;
        uint32_t i;
        برائے(i = 0; i < بچہ_cnt; i++) {
            lv_obj_t * بچہ = lv_obj_get_بچہ(والد, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            اگر(بچہ == obj) {
                حرف عدد_buf[8];
                سائز_t عدد_len;
                عدد_len = lv_snprintf(عدد_buf, حجم(عدد_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                اگر(buf_سائز > name_len + عدد_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], عدد_buf);
                }
                ورنہ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_سائز);
                }
                توڑو;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ورنہ {
                ثابت حرف * بچہ_name = lv_obj_get_name(بچہ);
                اگر(بچہ_name == تہی) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    سائز_t class_name_len = lv_strlen(بچہ->class_p->name);
                    اگر(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(بچہ->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ورنہ {
                    اگر(lv_strcmp(بچہ->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ورنہ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_سائز);
    }
}

lv_obj_t * lv_obj_get_بچہ_by_name(ثابت lv_obj_t * والد, ثابت حرف * راستہ)
{
    LV_CHECK_OBJ(والد, MY_CLASS, واپس تہی);
    LV_CHECK_ARG(راستہ != تہی, واپس تہی);
    LV_CHECK_ARG(والد->spec_attr != تہی, واپس تہی);

    جب(*راستہ) {
        ثابت حرف * segment = راستہ;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        جب(راستہ[len] && راستہ[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * بچہ = ڈھونڈو_by_name_براہ_راست(والد, segment, len);
        اگر(!بچہ) واپس تہی; /*Segment not found*/

        /* Advance to the next segment */
        راستہ += len;
        اگر(*راستہ == '/') راستہ++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        اگر(*راستہ == '\0') واپس بچہ;

        والد = بچہ;
    }

    واپس تہی;

}

lv_obj_t * lv_obj_ڈھونڈو_by_name(ثابت lv_obj_t * والد, ثابت حرف * name)
{
    LV_CHECK_ARG(name != تہی, واپس تہی);

    اگر(والد == تہی) والد = lv_display_get_screen_فعال(lv_display_get_default());
    اگر(والد == تہی) واپس تہی;

    lv_obj_t * بچہ = ڈھونڈو_by_name_براہ_راست(والد, name, UINT16_MAX);
    اگر(بچہ) واپس بچہ;

    /*Search children recursively*/
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(والد);
    uint32_t i;
    برائے(i = 0; i < بچہ_cnt; i++) {
        بچہ = والد->spec_attr->children[i];
        lv_obj_t * ملا = lv_obj_ڈھونڈو_by_name(بچہ, name);
        اگر(ملا != تہی) واپس ملا;
    }

    واپس تہی;
}

#اگر_ختم /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_اشاریہ(ثابت lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس -1);

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد == تہی) واپس -1;

    uint32_t i = 0;
    برائے(i = 0; i < والد->spec_attr->بچہ_cnt; i++) {
        اگر(والد->spec_attr->children[i] == obj) واپس (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    واپس -1;
}

int32_t lv_obj_get_اشاریہ_by_type(ثابت lv_obj_t * obj, ثابت lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, واپس -1);
    LV_CHECK_ARG(class_p != تہی, واپس -1);

    lv_obj_t * والد = lv_obj_get_والد(obj);
    اگر(والد == تہی) واپس 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    برائے(i = 0; i < والد->spec_attr->بچہ_cnt; i++) {
        lv_obj_t * بچہ = والد->spec_attr->children[i];
        اگر(بچہ->class_p == class_p) {
            اگر(بچہ == obj) واپس idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    واپس -1;
}

خالی lv_obj_درخت_walk(lv_obj_t * آغاز_obj, lv_obj_درخت_walk_cb_t cb, خالی * user_ڈیٹا)
{
    LV_CHECK_ARG(cb != تہی, واپس);

    walk_core(آغاز_obj, cb, user_ڈیٹا);
}

خالی lv_obj_dump_درخت(lv_obj_t * آغاز_obj)
{
    اگر(آغاز_obj == تہی) {
        lv_display_t * disp = lv_display_get_اگلا(تہی);
        جب(disp) {
            uint32_t i;
            برائے(i = 0; i < disp->screen_cnt; i++) {
                dump_درخت_core(disp->screens[i], 0);
            }
            disp = lv_display_get_اگلا(disp);
        }
    }
    ورنہ {
        dump_درخت_core(آغاز_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساکن خالی lv_obj_delete_async_cb(خالی * obj)
{
    lv_obj_delete(obj);
}

ساکن خالی obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    اگر(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ساکن خالی obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != تہی);
    اگر(obj->is_deleting)
        واپس;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_نتیجہ_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, تہی);
    اگر(res == LV_نتیجہ_INVALID) {
        obj->is_deleting = false;
        واپس;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    اگر(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    اگر(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * بچہ = lv_obj_get_بچہ(obj, 0);
    جب(بچہ) {
        obj_delete_core(بچہ);
        بچہ = lv_obj_get_بچہ(obj, 0);
    }

    lv_گروہ_t * گروہ = lv_obj_get_گروہ(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_اگلا(تہی);
    جب(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        اگر(indev_type == LV_INDEV_TYPE_اشارہ_گر || indev_type == LV_INDEV_TYPE_BUTTON) {
            اگر(indev->اشارہ_گر.act_obj == obj || indev->اشارہ_گر.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            اگر(indev->اشارہ_گر.آخری_pressed == obj) {
                indev->اشارہ_گر.آخری_pressed = تہی;
            }
            اگر(indev->اشارہ_گر.آخری_hovered == obj) {
                indev->اشارہ_گر.آخری_hovered = تہی;
            }
        }

        اگر(indev->گروہ == گروہ && obj == lv_indev_get_فعال_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_اگلا(indev);
    }

    /*Delete all pending async del-s*/
    lv_نتیجہ_t async_cancel_res = LV_نتیجہ_OK;
    جب(async_cancel_res == LV_نتیجہ_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    اگر(obj->والد == تہی) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        برائے(i = 0; i < disp->screen_cnt; i++) {
            اگر(disp->screens[i] == obj) توڑو;
        }

        uint32_t id = i;
        برائے(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * حجم(lv_obj_t *));
        اگر(new_screens != تہی) {
            disp->screens = new_screens;
        }
        ورنہ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ورنہ {
        lv_obj_remove_بچہ(obj->والد, obj);
    }

    obj->والد = تہی;

    /*Free the object itself*/
    lv_free(obj);
}

ساکن lv_obj_درخت_walk_res_t walk_core(lv_obj_t * obj, lv_obj_درخت_walk_cb_t cb, خالی * user_ڈیٹا)
{
    lv_obj_درخت_walk_res_t res = LV_OBJ_درخت_WALK_اگلا;

    اگر(obj == تہی) {
        lv_display_t * disp = lv_display_get_اگلا(تہی);
        جب(disp) {
            uint32_t i;
            برائے(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_ڈیٹا);
            }
            disp = lv_display_get_اگلا(disp);
        }
        واپس LV_OBJ_درخت_WALK_انتہا;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_ڈیٹا);

    اگر(res == LV_OBJ_درخت_WALK_انتہا) واپس LV_OBJ_درخت_WALK_انتہا;

    اگر(res != LV_OBJ_درخت_WALK_SKIP_CHILDREN) {
        uint32_t i;
        برائے(i = 0; i < lv_obj_get_بچہ_گنتی(obj); i++) {
            res = walk_core(lv_obj_get_بچہ(obj, i), cb, user_ڈیٹا);
            اگر(res == LV_OBJ_درخت_WALK_انتہا) واپس LV_OBJ_درخت_WALK_انتہا;
        }
    }
    واپس LV_OBJ_درخت_WALK_اگلا;
}

ساکن خالی dump_درخت_core(lv_obj_t * obj, int32_t گہرائی)
{
    LV_ASSERT(obj != تہی);
#اگر LV_USE_LOG
    ثابت حرف * id;

#اگر LV_USE_OBJ_ID
    حرف buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, حجم(buf));
    اگر(id == تہی) id = "obj0";
#ورنہ
    id = "obj0";
#اگر_ختم

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (صحیح)(2 * گہرائی), "", (خالی *)obj, id);
#اگر_ختم /*LV_USE_LOG*/

    اگر(obj->spec_attr && obj->spec_attr->بچہ_cnt) {
        برائے(uint32_t i = 0; i < obj->spec_attr->بچہ_cnt; i++) {
            dump_درخت_core(lv_obj_get_بچہ(obj, i), گہرائی + 1);
        }
    }
}

ساکن lv_obj_t * lv_obj_get_پہلا_not_deleting_بچہ(lv_obj_t * obj)
{
    LV_ASSERT(obj != تہی);

    اگر(obj->spec_attr == تہی) واپس تہی;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->بچہ_cnt;
    برائے(i = 0; i < cnt; i++) {
        اگر(!obj->spec_attr->children[i]->is_deleting) {
            واپس obj->spec_attr->children[i];
        }
    }

    واپس تہی;
}

#اگر LV_USE_OBJ_NAME

ساکن lv_obj_t * ڈھونڈو_by_name_براہ_راست(ثابت lv_obj_t * والد, ثابت حرف * name, سائز_t len)
{
    uint32_t i;
    uint32_t بچہ_cnt = lv_obj_get_بچہ_گنتی(والد);
    برائے(i = 0; i < بچہ_cnt; i++) {
        lv_obj_t * بچہ = والد->spec_attr->children[i];

        حرف بچہ_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(بچہ, بچہ_name_resolved, حجم(بچہ_name_resolved));

        اگر(len == UINT16_MAX) {
            اگر(lv_strcmp(بچہ_name_resolved, name) == 0) واپس بچہ;
        }
        ورنہ {
            اگر(lv_strncmp(بچہ_name_resolved, name, len) == 0 &&
               بچہ_name_resolved[len] == '\0') {
                واپس بچہ;
            }
        }
    }

    واپس تہی;
}

#اگر_ختم /*LV_USE_OBJ_NAME*/
