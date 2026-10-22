/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../lvgl_public.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lv_obj_class_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../indev/lv_indev_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../display/lv_display_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../misc/lv_anim_private.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MY_CLASS (&lv_obj_class)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OBJ_DUMP_STRING_LEN 128
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ਸਥਿਰ ਖਾਲੀ lv_obj_delete_async_cb(ਖਾਲੀ * obj);
ਸਥਿਰ ਖਾਲੀ obj_delete_core(lv_obj_t * obj);
ਸਥਿਰ lv_obj_ਰੁੱਖ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ਰੁੱਖ_walk_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ);
ਸਥਿਰ ਖਾਲੀ dump_ਰੁੱਖ_core(lv_obj_t * obj, int32_t ਡੂੰਘਾਈ);
ਸਥਿਰ lv_obj_t * lv_obj_get_ਪਹਿਲਾ_not_deleting_ਬੱਚਾ(lv_obj_t * obj);
#ਜੇ LV_USE_OBJ_NAME
    ਸਥਿਰ lv_obj_t * ਲੱਭੋ_by_name_ਸਿੱਧਾ(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ ਅੱਖਰ * name, ਮਾਪ_t len);
#ਜੇ_ਅੰਤ /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ਖਾਲੀ lv_obj_delete(lv_obj_t * obj)
{
    ਜੇ(obj == ਨਿਰਾ) ਵਾਪਸ;
    ਜੇ(obj->is_deleting) ਵਾਪਸ;

    LV_LOG_TRACE("begin (delete %p)", (ਖਾਲੀ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_ਮਾਪਾ(obj);

    lv_display_t * disp = ਨਿਰਾ;
    bool act_screen_del = false;
    ਜੇ(par == ਨਿਰਾ) {
        disp = lv_obj_get_display(obj);
        ਜੇ(!disp) ਵਾਪਸ;   /*Shouldn't happen*/
        ਜੇ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ਜੇ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, ਨਿਰਾ);
        lv_obj_send_event(par, LV_EVENT_ਬੱਚਾ_DELETED, ਨਿਰਾ);
    }

    /*Handle if the active screen was deleted*/
    ਜੇ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ਨਿਰਾ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (ਖਾਲੀ *)obj);
}

ਖਾਲੀ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (ਖਾਲੀ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj);
    lv_obj_t * ਬੱਚਾ = lv_obj_get_ਪਹਿਲਾ_not_deleting_ਬੱਚਾ(obj);
    ਜਦੋਂ(ਬੱਚਾ) {
        obj_delete_core(ਬੱਚਾ);
        ਬੱਚਾ = lv_obj_get_ਪਹਿਲਾ_not_deleting_ਬੱਚਾ(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_ਮੰਜ਼ਿਲ(obj, 0, 0, LV_ANIM_OFF);
    ਜੇ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ਜੇ(lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, ਨਿਰਾ);
        lv_obj_send_event(obj, LV_EVENT_ਬੱਚਾ_DELETED, ਨਿਰਾ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (ਖਾਲੀ *)obj);
}

ਖਾਲੀ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ਜੇ(obj == ਨਿਰਾ) ਵਾਪਸ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_ਸੈੱਟ_var(&a, obj);
    lv_anim_ਸੈੱਟ_exec_cb(&a, ਨਿਰਾ);
    lv_anim_ਸੈੱਟ_duration(&a, 1);
    lv_anim_ਸੈੱਟ_delay(&a, delay_ms);
    lv_anim_ਸੈੱਟ_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_ਸ਼ੁਰੂ(&a);
}

ਖਾਲੀ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ਨਿਰਾ, ਵਾਪਸ);

    lv_obj_delete(a->var);
}

ਖਾਲੀ lv_obj_delete_async(lv_obj_t * obj)
{
    ਜੇ(obj == ਨਿਰਾ) ਵਾਪਸ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_ਮਾਪਾ(lv_obj_t * obj, lv_obj_t * ਮਾਪਾ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_OBJ(ਮਾਪਾ, MY_CLASS, ਵਾਪਸ);

    ਜੇ(obj->ਮਾਪਾ == ਨਿਰਾ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ਵਾਪਸ;
    }

    ਜੇ(ਮਾਪਾ == obj->ਮਾਪਾ) {
        ਵਾਪਸ;
    }

    ਜੇ(!lv_obj_allocate_spec_attr(ਮਾਪਾ)) {
        ਵਾਪਸ;
    }

    /*Add the object to the new parents list first*/
    ਜੇ(lv_obj_ਜੋੜੋ_ਬੱਚਾ(ਮਾਪਾ, obj) != LV_ਨਤੀਜਾ_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ਵਾਪਸ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_ਬੱਚਾ(obj->ਮਾਪਾ, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->ਮਾਪਾ);
    lv_obj_send_event(obj->ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj);
    lv_obj_send_event(obj->ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_DELETED, ਨਿਰਾ);
    obj->ਮਾਪਾ = ਮਾਪਾ;

    /*Notify the new parent about the child*/
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj);
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_CREATED, ਨਿਰਾ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

ਖਾਲੀ lv_obj_move_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(lv_obj_t * obj, int32_t ਸੂਚਕਾਂਕ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    /* Check parent validity */
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(!ਮਾਪਾ) {
        LV_LOG_WARN("parent is NULL");
        ਵਾਪਸ;
    }

    ਅਚਲ uint32_t ਮਾਪਾ_ਬੱਚਾ_ਗਿਣਤੀ = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ਅਚਲ int32_t old_ਸੂਚਕਾਂਕ = lv_obj_get_ਸੂਚਕਾਂਕ(obj);
    LV_ASSERT(0 <= old_ਸੂਚਕਾਂਕ);

    ਜੇ(ਸੂਚਕਾਂਕ < 0) {
        ਸੂਚਕਾਂਕ += ਮਾਪਾ_ਬੱਚਾ_ਗਿਣਤੀ;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ਜੇ((ਸੂਚਕਾਂਕ < 0)
       /* Index is same or bigger than parent child count */
       || (ਸੂਚਕਾਂਕ >= (int32_t) ਮਾਪਾ_ਬੱਚਾ_ਗਿਣਤੀ)
       /* If both previous and new index are the same */
       || (ਸੂਚਕਾਂਕ == old_ਸੂਚਕਾਂਕ)) {

        ਵਾਪਸ;
    }

    int32_t i = old_ਸੂਚਕਾਂਕ;
    ਜੇ(ਸੂਚਕਾਂਕ < old_ਸੂਚਕਾਂਕ) {
        ਜਦੋਂ(i > ਸੂਚਕਾਂਕ)  {
            ਮਾਪਾ->spec_attr->children[i] = ਮਾਪਾ->spec_attr->children[i - 1];
            i--;
        }
    }
    ਨਹੀਂਤਾਂ {
        ਜਦੋਂ(i < ਸੂਚਕਾਂਕ) {
            ਮਾਪਾ->spec_attr->children[i] = ਮਾਪਾ->spec_attr->children[i + 1];
            i++;
        }
    }

    ਮਾਪਾ->spec_attr->children[ਸੂਚਕਾਂਕ] = obj;
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, ਨਿਰਾ);
    lv_obj_invalidate(ਮਾਪਾ);
}

ਖਾਲੀ lv_obj_ਅਦਲਾ_ਬਦਲੀ(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_OBJ(obj2, MY_CLASS, ਵਾਪਸ);

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj1);
    lv_obj_t * parent2 = lv_obj_get_ਮਾਪਾ(obj2);

    uint_fast32_t index1 = lv_obj_get_ਸੂਚਕਾਂਕ(obj1);
    uint_fast32_t index2 = lv_obj_get_ਸੂਚਕਾਂਕ(obj2);

    lv_obj_send_event(parent2, LV_EVENT_ਬੱਚਾ_DELETED, obj2);
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_DELETED, obj1);

    ਮਾਪਾ->spec_attr->children[index1] = obj2;
    obj2->ਮਾਪਾ = ਮਾਪਾ;

    parent2->spec_attr->children[index2] = obj1;
    obj1->ਮਾਪਾ = parent2;

    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj2);
    lv_obj_send_event(ਮਾਪਾ, LV_EVENT_ਬੱਚਾ_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_ਬੱਚਾ_ਬਦਲਿਆ, obj1);
    lv_obj_send_event(parent2, LV_EVENT_ਬੱਚਾ_CREATED, obj1);

    lv_obj_invalidate(ਮਾਪਾ);

    ਜੇ(ਮਾਪਾ != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_ਸਮੂਹ_ਅਦਲਾ_ਬਦਲੀ_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਅਚਲ lv_obj_t * par = obj;
    ਅਚਲ lv_obj_t * act_par;

    ਕਰੋ {
        act_par = par;
        par = lv_obj_get_ਮਾਪਾ(act_par);
    } ਜਦੋਂ(par != ਨਿਰਾ);

    ਵਾਪਸ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਅਚਲ lv_obj_t * scr;

    ਜੇ(obj->ਮਾਪਾ == ਨਿਰਾ) scr = obj;  /*`obj` is a screen*/
    ਨਹੀਂਤਾਂ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_ਸਿਰ = disp_ll_p;
    LV_LL_READ(disp_ਸਿਰ, d) {
        uint32_t i;
        ਲਈ(i = 0; i < d->screen_cnt; i++) {
            ਜੇ(d->screens[i] == scr) ਵਾਪਸ d;
        }
    }

    LV_LOG_WARN("No screen found");
    ਵਾਪਸ ਨਿਰਾ;
}

lv_obj_t * lv_obj_get_ਮਾਪਾ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਵਾਪਸ obj->ਮਾਪਾ;
}

lv_obj_t * lv_obj_get_ਬੱਚਾ(ਅਚਲ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    ਜੇ(idx < 0) {
        idx = obj->spec_attr->ਬੱਚਾ_cnt + idx;
        ਜੇ(idx < 0) ਵਾਪਸ ਨਿਰਾ;
    }

    uint32_t idu = (uint32_t) idx;

    ਜੇ(idu < obj->spec_attr->ਬੱਚਾ_cnt) ਵਾਪਸ obj->spec_attr->children[idu];
    ਨਹੀਂਤਾਂ ਵਾਪਸ ਨਿਰਾ;
}

lv_obj_t * lv_obj_get_ਬੱਚਾ_by_type(ਅਚਲ lv_obj_t * obj, int32_t idx, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ਬੱਚਾ_cnt;
    ਜੇ(idx >= 0) {
        ਲਈ(i = 0; i < cnt; i++) {
            ਜੇ(obj->spec_attr->children[i]->class_p == class_p) {
                ਜੇ(idx == 0) ਵਾਪਸ obj->spec_attr->children[i];
                ਨਹੀਂਤਾਂ idx--;
            }
        }
    }
    ਨਹੀਂਤਾਂ {
        idx++;   /*-1 means the first child*/
        ਲਈ(i = cnt - 1; i >= 0; i--) {
            ਜੇ(obj->spec_attr->children[i]->class_p == class_p) {
                ਜੇ(idx == 0) ਵਾਪਸ obj->spec_attr->children[i];
                ਨਹੀਂਤਾਂ idx++;
            }
        }
    }
    ਵਾਪਸ ਨਿਰਾ;
}

lv_obj_t * lv_obj_get_sibling(ਅਚਲ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(obj->ਮਾਪਾ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_ਸੂਚਕਾਂਕ(obj) + idx;
    ਜੇ(sibling_idx < 0) ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ lv_obj_get_ਬੱਚਾ(ਮਾਪਾ, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ਅਚਲ lv_obj_t * obj, int32_t idx, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(obj->ਮਾਪਾ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_ਸੂਚਕਾਂਕ_by_type(obj, class_p) + idx;
    ਜੇ(sibling_idx < 0) ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ lv_obj_get_ਬੱਚਾ_by_type(ਮਾਪਾ, sibling_idx, class_p);
}

uint32_t lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);
    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;
    ਵਾਪਸ obj->spec_attr->ਬੱਚਾ_cnt;
}

uint32_t lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ_by_type(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ 0);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ 0);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ 0;

    uint32_t i;
    uint32_t cnt = 0;
    ਲਈ(i = 0; i < obj->spec_attr->ਬੱਚਾ_cnt; i++) {
        ਜੇ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ਵਾਪਸ cnt;
}

#ਜੇ LV_USE_OBJ_NAME

ਖਾਲੀ lv_obj_ਸੈੱਟ_name(lv_obj_t * obj, ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }

    ਜੇ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ਖਾਲੀ *)obj->spec_attr->name);

    ਜੇ(name == ਨਿਰਾ) {
        obj->spec_attr->name = ਨਿਰਾ;
        obj->spec_attr->name_static = 1;
    }
    ਨਹੀਂਤਾਂ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

ਖਾਲੀ lv_obj_ਸੈੱਟ_name_static(lv_obj_t * obj, ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(name != ਨਿਰਾ, ਵਾਪਸ);

    ਜੇ(!lv_obj_allocate_spec_attr(obj)) {
        ਵਾਪਸ;
    }
    ਜੇ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ਖਾਲੀ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ਅਚਲ ਅੱਖਰ * lv_obj_get_name(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;
    ਨਹੀਂਤਾਂ ਵਾਪਸ obj->spec_attr->name;
}

ਖਾਲੀ lv_obj_get_name_resolved(ਅਚਲ lv_obj_t * obj, ਅੱਖਰ buf[], ਮਾਪ_t buf_ਮਾਪ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ);
    LV_CHECK_ARG(buf != ਨਿਰਾ && buf_ਮਾਪ > 0, ਵਾਪਸ);

    ਅਚਲ ਅੱਖਰ * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    ਅੱਖਰ name_buf[LV_OBJ_NAME_MAX_LEN];
    ਜੇ(name == ਨਿਰਾ) {
        lv_snprintf(name_buf, ਆਕਾਰ(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    ਮਾਪ_t name_len = lv_strlen(name);
    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ਜੇ(ਮਾਪਾ && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
        uint32_t cnt = 0;
        uint32_t i;
        ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
            lv_obj_t * ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(ਮਾਪਾ, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ਜੇ(ਬੱਚਾ == obj) {
                ਅੱਖਰ ਅੰਕ_buf[8];
                ਮਾਪ_t ਅੰਕ_len;
                ਅੰਕ_len = lv_snprintf(ਅੰਕ_buf, ਆਕਾਰ(ਅੰਕ_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ਜੇ(buf_ਮਾਪ > name_len + ਅੰਕ_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], ਅੰਕ_buf);
                }
                ਨਹੀਂਤਾਂ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_ਮਾਪ);
                }
                ਰੋਕੋ;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ਨਹੀਂਤਾਂ {
                ਅਚਲ ਅੱਖਰ * ਬੱਚਾ_name = lv_obj_get_name(ਬੱਚਾ);
                ਜੇ(ਬੱਚਾ_name == ਨਿਰਾ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    ਮਾਪ_t class_name_len = lv_strlen(ਬੱਚਾ->class_p->name);
                    ਜੇ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(ਬੱਚਾ->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ਨਹੀਂਤਾਂ {
                    ਜੇ(lv_strcmp(ਬੱਚਾ->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ਨਹੀਂਤਾਂ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_ਮਾਪ);
    }
}

lv_obj_t * lv_obj_get_ਬੱਚਾ_by_name(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ ਅੱਖਰ * ਰਸਤਾ)
{
    LV_CHECK_OBJ(ਮਾਪਾ, MY_CLASS, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਰਸਤਾ != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);
    LV_CHECK_ARG(ਮਾਪਾ->spec_attr != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਜਦੋਂ(*ਰਸਤਾ) {
        ਅਚਲ ਅੱਖਰ * segment = ਰਸਤਾ;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ਜਦੋਂ(ਰਸਤਾ[len] && ਰਸਤਾ[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * ਬੱਚਾ = ਲੱਭੋ_by_name_ਸਿੱਧਾ(ਮਾਪਾ, segment, len);
        ਜੇ(!ਬੱਚਾ) ਵਾਪਸ ਨਿਰਾ; /*Segment not found*/

        /* Advance to the next segment */
        ਰਸਤਾ += len;
        ਜੇ(*ਰਸਤਾ == '/') ਰਸਤਾ++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ਜੇ(*ਰਸਤਾ == '\0') ਵਾਪਸ ਬੱਚਾ;

        ਮਾਪਾ = ਬੱਚਾ;
    }

    ਵਾਪਸ ਨਿਰਾ;

}

lv_obj_t * lv_obj_ਲੱਭੋ_by_name(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ ਅੱਖਰ * name)
{
    LV_CHECK_ARG(name != ਨਿਰਾ, ਵਾਪਸ ਨਿਰਾ);

    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਮਾਪਾ = lv_display_get_screen_ਸਰਗਰਮ(lv_display_get_default());
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    lv_obj_t * ਬੱਚਾ = ਲੱਭੋ_by_name_ਸਿੱਧਾ(ਮਾਪਾ, name, UINT16_MAX);
    ਜੇ(ਬੱਚਾ) ਵਾਪਸ ਬੱਚਾ;

    /*Search children recursively*/
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
    uint32_t i;
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        ਬੱਚਾ = ਮਾਪਾ->spec_attr->children[i];
        lv_obj_t * ਮਿਲਿਆ = lv_obj_ਲੱਭੋ_by_name(ਬੱਚਾ, name);
        ਜੇ(ਮਿਲਿਆ != ਨਿਰਾ) ਵਾਪਸ ਮਿਲਿਆ;
    }

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_ਸੂਚਕਾਂਕ(ਅਚਲ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ -1);

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਵਾਪਸ -1;

    uint32_t i = 0;
    ਲਈ(i = 0; i < ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt; i++) {
        ਜੇ(ਮਾਪਾ->spec_attr->children[i] == obj) ਵਾਪਸ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ਵਾਪਸ -1;
}

int32_t lv_obj_get_ਸੂਚਕਾਂਕ_by_type(ਅਚਲ lv_obj_t * obj, ਅਚਲ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ਵਾਪਸ -1);
    LV_CHECK_ARG(class_p != ਨਿਰਾ, ਵਾਪਸ -1);

    lv_obj_t * ਮਾਪਾ = lv_obj_get_ਮਾਪਾ(obj);
    ਜੇ(ਮਾਪਾ == ਨਿਰਾ) ਵਾਪਸ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    ਲਈ(i = 0; i < ਮਾਪਾ->spec_attr->ਬੱਚਾ_cnt; i++) {
        lv_obj_t * ਬੱਚਾ = ਮਾਪਾ->spec_attr->children[i];
        ਜੇ(ਬੱਚਾ->class_p == class_p) {
            ਜੇ(ਬੱਚਾ == obj) ਵਾਪਸ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ਵਾਪਸ -1;
}

ਖਾਲੀ lv_obj_ਰੁੱਖ_walk(lv_obj_t * ਸ਼ੁਰੂ_obj, lv_obj_ਰੁੱਖ_walk_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    LV_CHECK_ARG(cb != ਨਿਰਾ, ਵਾਪਸ);

    walk_core(ਸ਼ੁਰੂ_obj, cb, user_ਅੰਕੜੇ);
}

ਖਾਲੀ lv_obj_dump_ਰੁੱਖ(lv_obj_t * ਸ਼ੁਰੂ_obj)
{
    ਜੇ(ਸ਼ੁਰੂ_obj == ਨਿਰਾ) {
        lv_display_t * disp = lv_display_get_ਅਗਲਾ(ਨਿਰਾ);
        ਜਦੋਂ(disp) {
            uint32_t i;
            ਲਈ(i = 0; i < disp->screen_cnt; i++) {
                dump_ਰੁੱਖ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_ਅਗਲਾ(disp);
        }
    }
    ਨਹੀਂਤਾਂ {
        dump_ਰੁੱਖ_core(ਸ਼ੁਰੂ_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ਸਥਿਰ ਖਾਲੀ lv_obj_delete_async_cb(ਖਾਲੀ * obj)
{
    lv_obj_delete(obj);
}

ਸਥਿਰ ਖਾਲੀ obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    ਜੇ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ਸਥਿਰ ਖਾਲੀ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ਨਿਰਾ);
    ਜੇ(obj->is_deleting)
        ਵਾਪਸ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_ਨਤੀਜਾ_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ਨਿਰਾ);
    ਜੇ(res == LV_ਨਤੀਜਾ_INVALID) {
        obj->is_deleting = false;
        ਵਾਪਸ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ਜੇ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ਜੇ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(obj, 0);
    ਜਦੋਂ(ਬੱਚਾ) {
        obj_delete_core(ਬੱਚਾ);
        ਬੱਚਾ = lv_obj_get_ਬੱਚਾ(obj, 0);
    }

    lv_ਸਮੂਹ_t * ਸਮੂਹ = lv_obj_get_ਸਮੂਹ(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_ਅਗਲਾ(ਨਿਰਾ);
    ਜਦੋਂ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ਜੇ(indev_type == LV_INDEV_TYPE_ਸੂਚਕ || indev_type == LV_INDEV_TYPE_BUTTON) {
            ਜੇ(indev->ਸੂਚਕ.act_obj == obj || indev->ਸੂਚਕ.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ਜੇ(indev->ਸੂਚਕ.ਆਖਰੀ_pressed == obj) {
                indev->ਸੂਚਕ.ਆਖਰੀ_pressed = ਨਿਰਾ;
            }
            ਜੇ(indev->ਸੂਚਕ.ਆਖਰੀ_hovered == obj) {
                indev->ਸੂਚਕ.ਆਖਰੀ_hovered = ਨਿਰਾ;
            }
        }

        ਜੇ(indev->ਸਮੂਹ == ਸਮੂਹ && obj == lv_indev_get_ਸਰਗਰਮ_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_ਅਗਲਾ(indev);
    }

    /*Delete all pending async del-s*/
    lv_ਨਤੀਜਾ_t async_cancel_res = LV_ਨਤੀਜਾ_OK;
    ਜਦੋਂ(async_cancel_res == LV_ਨਤੀਜਾ_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ਜੇ(obj->ਮਾਪਾ == ਨਿਰਾ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        ਲਈ(i = 0; i < disp->screen_cnt; i++) {
            ਜੇ(disp->screens[i] == obj) ਰੋਕੋ;
        }

        uint32_t id = i;
        ਲਈ(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * ਆਕਾਰ(lv_obj_t *));
        ਜੇ(new_screens != ਨਿਰਾ) {
            disp->screens = new_screens;
        }
        ਨਹੀਂਤਾਂ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ਨਹੀਂਤਾਂ {
        lv_obj_remove_ਬੱਚਾ(obj->ਮਾਪਾ, obj);
    }

    obj->ਮਾਪਾ = ਨਿਰਾ;

    /*Free the object itself*/
    lv_free(obj);
}

ਸਥਿਰ lv_obj_ਰੁੱਖ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ਰੁੱਖ_walk_cb_t cb, ਖਾਲੀ * user_ਅੰਕੜੇ)
{
    lv_obj_ਰੁੱਖ_walk_res_t res = LV_OBJ_ਰੁੱਖ_WALK_ਅਗਲਾ;

    ਜੇ(obj == ਨਿਰਾ) {
        lv_display_t * disp = lv_display_get_ਅਗਲਾ(ਨਿਰਾ);
        ਜਦੋਂ(disp) {
            uint32_t i;
            ਲਈ(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_ਅੰਕੜੇ);
            }
            disp = lv_display_get_ਅਗਲਾ(disp);
        }
        ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਸਿਰਾ;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_ਅੰਕੜੇ);

    ਜੇ(res == LV_OBJ_ਰੁੱਖ_WALK_ਸਿਰਾ) ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਸਿਰਾ;

    ਜੇ(res != LV_OBJ_ਰੁੱਖ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        ਲਈ(i = 0; i < lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(obj); i++) {
            res = walk_core(lv_obj_get_ਬੱਚਾ(obj, i), cb, user_ਅੰਕੜੇ);
            ਜੇ(res == LV_OBJ_ਰੁੱਖ_WALK_ਸਿਰਾ) ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਸਿਰਾ;
        }
    }
    ਵਾਪਸ LV_OBJ_ਰੁੱਖ_WALK_ਅਗਲਾ;
}

ਸਥਿਰ ਖਾਲੀ dump_ਰੁੱਖ_core(lv_obj_t * obj, int32_t ਡੂੰਘਾਈ)
{
    LV_ASSERT(obj != ਨਿਰਾ);
#ਜੇ LV_USE_LOG
    ਅਚਲ ਅੱਖਰ * id;

#ਜੇ LV_USE_OBJ_ID
    ਅੱਖਰ buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, ਆਕਾਰ(buf));
    ਜੇ(id == ਨਿਰਾ) id = "obj0";
#ਨਹੀਂਤਾਂ
    id = "obj0";
#ਜੇ_ਅੰਤ

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (ਪੂਰਨ)(2 * ਡੂੰਘਾਈ), "", (ਖਾਲੀ *)obj, id);
#ਜੇ_ਅੰਤ /*LV_USE_LOG*/

    ਜੇ(obj->spec_attr && obj->spec_attr->ਬੱਚਾ_cnt) {
        ਲਈ(uint32_t i = 0; i < obj->spec_attr->ਬੱਚਾ_cnt; i++) {
            dump_ਰੁੱਖ_core(lv_obj_get_ਬੱਚਾ(obj, i), ਡੂੰਘਾਈ + 1);
        }
    }
}

ਸਥਿਰ lv_obj_t * lv_obj_get_ਪਹਿਲਾ_not_deleting_ਬੱਚਾ(lv_obj_t * obj)
{
    LV_ASSERT(obj != ਨਿਰਾ);

    ਜੇ(obj->spec_attr == ਨਿਰਾ) ਵਾਪਸ ਨਿਰਾ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->ਬੱਚਾ_cnt;
    ਲਈ(i = 0; i < cnt; i++) {
        ਜੇ(!obj->spec_attr->children[i]->is_deleting) {
            ਵਾਪਸ obj->spec_attr->children[i];
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ LV_USE_OBJ_NAME

ਸਥਿਰ lv_obj_t * ਲੱਭੋ_by_name_ਸਿੱਧਾ(ਅਚਲ lv_obj_t * ਮਾਪਾ, ਅਚਲ ਅੱਖਰ * name, ਮਾਪ_t len)
{
    uint32_t i;
    uint32_t ਬੱਚਾ_cnt = lv_obj_get_ਬੱਚਾ_ਗਿਣਤੀ(ਮਾਪਾ);
    ਲਈ(i = 0; i < ਬੱਚਾ_cnt; i++) {
        lv_obj_t * ਬੱਚਾ = ਮਾਪਾ->spec_attr->children[i];

        ਅੱਖਰ ਬੱਚਾ_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(ਬੱਚਾ, ਬੱਚਾ_name_resolved, ਆਕਾਰ(ਬੱਚਾ_name_resolved));

        ਜੇ(len == UINT16_MAX) {
            ਜੇ(lv_strcmp(ਬੱਚਾ_name_resolved, name) == 0) ਵਾਪਸ ਬੱਚਾ;
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ(lv_strncmp(ਬੱਚਾ_name_resolved, name, len) == 0 &&
               ਬੱਚਾ_name_resolved[len] == '\0') {
                ਵਾਪਸ ਬੱਚਾ;
            }
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ /*LV_USE_OBJ_NAME*/
