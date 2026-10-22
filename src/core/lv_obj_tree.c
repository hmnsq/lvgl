/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"
#ଅନ୍ତର୍ଭାଵ "lv_obj_class_private.h"
#ଅନ୍ତର୍ଭାଵ "../indev/lv_indev_private.h"
#ଅନ୍ତର୍ଭାଵ "../display/lv_display_private.h"
#ଅନ୍ତର୍ଭାଵ "../misc/lv_anim_private.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#define disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#define OBJ_DUMP_STRING_LEN 128
#define LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
ସ୍ଥିର ଶୂନ୍ୟ lv_obj_delete_async_cb(ଶୂନ୍ୟ * obj);
ସ୍ଥିର ଶୂନ୍ୟ obj_delete_core(lv_obj_t * obj);
ସ୍ଥିର lv_obj_ବୃକ୍ଷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ବୃକ୍ଷ_walk_cb_t cb, ଶୂନ୍ୟ * user_data);
ସ୍ଥିର ଶୂନ୍ୟ dump_ବୃକ୍ଷ_core(lv_obj_t * obj, int32_t depth);
ସ୍ଥିର lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#ଯଦି LV_USE_OBJ_NAME
    ସ୍ଥିର lv_obj_t * find_by_name_direct(ଅଚଳ lv_obj_t * parent, ଅଚଳ ଅକ୍ଷର * name, size_t len);
#endif /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ଶୂନ୍ୟ lv_obj_delete(lv_obj_t * obj)
{
    ଯଦି(obj == ଖାଲି) ଫେରନ୍ତୁ;
    ଯଦି(obj->is_deleting) ଫେରନ୍ତୁ;

    LV_LOG_TRACE("begin (delete %p)", (ଶୂନ୍ୟ *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = ଖାଲି;
    bool act_screen_del = false;
    ଯଦି(par == ଖାଲି) {
        disp = lv_obj_get_display(obj);
        ଯଦି(!disp) ଫେରନ୍ତୁ;   /*Shouldn't happen*/
        ଯଦି(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ଯଦି(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, ଖାଲି);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, ଖାଲି);
    }

    /*Handle if the active screen was deleted*/
    ଯଦି(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ଖାଲି;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (ଶୂନ୍ୟ *)obj);
}

ଶୂନ୍ୟ lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (ଶୂନ୍ୟ *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_ଗଣନା(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    ଯେତେବେଳେ(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    ଯଦି(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ଯଦି(lv_obj_get_child_ଗଣନା(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, ଖାଲି);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, ଖାଲି);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (ଶୂନ୍ୟ *)obj);
}

ଶୂନ୍ୟ lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ଯଦି(obj == ଖାଲି) ଫେରନ୍ତୁ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, ଖାଲି);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

ଶୂନ୍ୟ lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ଖାଲି, ଫେରନ୍ତୁ);

    lv_obj_delete(a->var);
}

ଶୂନ୍ୟ lv_obj_delete_async(lv_obj_t * obj)
{
    ଯଦି(obj == ଖାଲି) ଫେରନ୍ତୁ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

ଶୂନ୍ୟ lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);
    LV_CHECK_OBJ(parent, MY_CLASS, ଫେରନ୍ତୁ);

    ଯଦି(obj->parent == ଖାଲି) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ଫେରନ୍ତୁ;
    }

    ଯଦି(parent == obj->parent) {
        ଫେରନ୍ତୁ;
    }

    ଯଦି(!lv_obj_allocate_spec_attr(parent)) {
        ଫେରନ୍ତୁ;
    }

    /*Add the object to the new parents list first*/
    ଯଦି(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ଫେରନ୍ତୁ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, ଖାଲି);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, ଖାଲି);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

ଶୂନ୍ୟ lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    ଯଦି(!parent) {
        LV_LOG_WARN("parent is NULL");
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ uint32_t parent_child_ଗଣନା = lv_obj_get_child_ଗଣନା(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    ଅଚଳ int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    ଯଦି(index < 0) {
        index += parent_child_ଗଣନା;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ଯଦି((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_ଗଣନା)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        ଫେରନ୍ତୁ;
    }

    int32_t i = old_index;
    ଯଦି(index < old_index) {
        ଯେତେବେଳେ(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    ଅନ୍ୟଥା {
        ଯେତେବେଳେ(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, ଖାଲି);
    lv_obj_invalidate(parent);
}

ଶୂନ୍ୟ lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ଫେରନ୍ତୁ);
    LV_CHECK_OBJ(obj2, MY_CLASS, ଫେରନ୍ତୁ);

    lv_obj_t * parent = lv_obj_get_parent(obj1);
    lv_obj_t * parent2 = lv_obj_get_parent(obj2);

    uint_fast32_t index1 = lv_obj_get_index(obj1);
    uint_fast32_t index2 = lv_obj_get_index(obj2);

    lv_obj_send_event(parent2, LV_EVENT_CHILD_DELETED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_DELETED, obj1);

    parent->spec_attr->children[index1] = obj2;
    obj2->parent = parent;

    parent2->spec_attr->children[index2] = obj1;
    obj1->parent = parent2;

    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CHANGED, obj1);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CREATED, obj1);

    lv_obj_invalidate(parent);

    ଯଦି(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);

    ଅଚଳ lv_obj_t * par = obj;
    ଅଚଳ lv_obj_t * act_par;

    କର {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } ଯେତେବେଳେ(par != ଖାଲି);

    ଫେରନ୍ତୁ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);

    ଅଚଳ lv_obj_t * scr;

    ଯଦି(obj->parent == ଖାଲି) scr = obj;  /*`obj` is a screen*/
    ଅନ୍ୟଥା scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        ପାଇଁ(i = 0; i < d->screen_cnt; i++) {
            ଯଦି(d->screens[i] == scr) ଫେରନ୍ତୁ d;
        }
    }

    LV_LOG_WARN("No screen found");
    ଫେରନ୍ତୁ ଖାଲି;
}

lv_obj_t * lv_obj_get_parent(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);

    ଫେରନ୍ତୁ obj->parent;
}

lv_obj_t * lv_obj_get_child(ଅଚଳ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        ଯଦି(idx < 0) ଫେରନ୍ତୁ ଖାଲି;
    }

    uint32_t idu = (uint32_t) idx;

    ଯଦି(idu < obj->spec_attr->child_cnt) ଫେରନ୍ତୁ obj->spec_attr->children[idu];
    ଅନ୍ୟଥା ଫେରନ୍ତୁ ଖାଲି;
}

lv_obj_t * lv_obj_get_child_by_type(ଅଚଳ lv_obj_t * obj, int32_t idx, ଅଚଳ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(class_p != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ଯଦି(idx >= 0) {
        ପାଇଁ(i = 0; i < cnt; i++) {
            ଯଦି(obj->spec_attr->children[i]->class_p == class_p) {
                ଯଦି(idx == 0) ଫେରନ୍ତୁ obj->spec_attr->children[i];
                ଅନ୍ୟଥା idx--;
            }
        }
    }
    ଅନ୍ୟଥା {
        idx++;   /*-1 means the first child*/
        ପାଇଁ(i = cnt - 1; i >= 0; i--) {
            ଯଦି(obj->spec_attr->children[i]->class_p == class_p) {
                ଯଦି(idx == 0) ଫେରନ୍ତୁ obj->spec_attr->children[i];
                ଅନ୍ୟଥା idx++;
            }
        }
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

lv_obj_t * lv_obj_get_sibling(ଅଚଳ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(obj->parent != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    ଯଦି(sibling_idx < 0) ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(ଅଚଳ lv_obj_t * obj, int32_t idx, ଅଚଳ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(obj->parent != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(class_p != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    ଯଦି(sibling_idx < 0) ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_ଗଣନା(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ 0);
    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_ଗଣନା_by_type(ଅଚଳ lv_obj_t * obj, ଅଚଳ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ 0);
    LV_CHECK_ARG(class_p != ଖାଲି, ଫେରନ୍ତୁ 0);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ 0;

    uint32_t i;
    uint32_t cnt = 0;
    ପାଇଁ(i = 0; i < obj->spec_attr->child_cnt; i++) {
        ଯଦି(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ଫେରନ୍ତୁ cnt;
}

#ଯଦି LV_USE_OBJ_NAME

ଶୂନ୍ୟ lv_obj_set_name(lv_obj_t * obj, ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);

    ଯଦି(!lv_obj_allocate_spec_attr(obj)) {
        ଫେରନ୍ତୁ;
    }

    ଯଦି(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ଶୂନ୍ୟ *)obj->spec_attr->name);

    ଯଦି(name == ଖାଲି) {
        obj->spec_attr->name = ଖାଲି;
        obj->spec_attr->name_static = 1;
    }
    ଅନ୍ୟଥା {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

ଶୂନ୍ୟ lv_obj_set_name_static(lv_obj_t * obj, ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(name != ଖାଲି, ଫେରନ୍ତୁ);

    ଯଦି(!lv_obj_allocate_spec_attr(obj)) {
        ଫେରନ୍ତୁ;
    }
    ଯଦି(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ଶୂନ୍ୟ *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

ଅଚଳ ଅକ୍ଷର * lv_obj_get_name(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;
    ଅନ୍ୟଥା ଫେରନ୍ତୁ obj->spec_attr->name;
}

ଶୂନ୍ୟ lv_obj_get_name_resolved(ଅଚଳ lv_obj_t * obj, ଅକ୍ଷର buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(buf != ଖାଲି && buf_size > 0, ଫେରନ୍ତୁ);

    ଅଚଳ ଅକ୍ଷର * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    ଅକ୍ଷର name_buf[LV_OBJ_NAME_MAX_LEN];
    ଯଦି(name == ଖାଲି) {
        lv_snprintf(name_buf, ଆକାର(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ଯଦି(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_ଗଣନା(parent);
        uint32_t cnt = 0;
        uint32_t i;
        ପାଇଁ(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ଯଦି(child == obj) {
                ଅକ୍ଷର ଅଙ୍କ_buf[8];
                size_t ଅଙ୍କ_len;
                ଅଙ୍କ_len = lv_snprintf(ଅଙ୍କ_buf, ଆକାର(ଅଙ୍କ_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ଯଦି(buf_size > name_len + ଅଙ୍କ_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], ଅଙ୍କ_buf);
                }
                ଅନ୍ୟଥା {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                ଭାଙ୍ଗ;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            ଅନ୍ୟଥା {
                ଅଚଳ ଅକ୍ଷର * child_name = lv_obj_get_name(child);
                ଯଦି(child_name == ଖାଲି) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    ଯଦି(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                ଅନ୍ୟଥା {
                    ଯଦି(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    ଅନ୍ୟଥା {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(ଅଚଳ lv_obj_t * parent, ଅଚଳ ଅକ୍ଷର * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(path != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    LV_CHECK_ARG(parent->spec_attr != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଯେତେବେଳେ(*path) {
        ଅଚଳ ଅକ୍ଷର * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ଯେତେବେଳେ(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        ଯଦି(!child) ଫେରନ୍ତୁ ଖାଲି; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        ଯଦି(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ଯଦି(*path == '\0') ଫେରନ୍ତୁ child;

        parent = child;
    }

    ଫେରନ୍ତୁ ଖାଲି;

}

lv_obj_t * lv_obj_find_by_name(ଅଚଳ lv_obj_t * parent, ଅଚଳ ଅକ୍ଷର * name)
{
    LV_CHECK_ARG(name != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);

    ଯଦି(parent == ଖାଲି) parent = lv_display_get_screen_active(lv_display_get_default());
    ଯଦି(parent == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    ଯଦି(child) ଫେରନ୍ତୁ child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_ଗଣନା(parent);
    uint32_t i;
    ପାଇଁ(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        ଯଦି(found != ଖାଲି) ଫେରନ୍ତୁ found;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(ଅଚଳ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ଯଦି(parent == ଖାଲି) ଫେରନ୍ତୁ -1;

    uint32_t i = 0;
    ପାଇଁ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        ଯଦି(parent->spec_attr->children[i] == obj) ଫେରନ୍ତୁ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ଫେରନ୍ତୁ -1;
}

int32_t lv_obj_get_index_by_type(ଅଚଳ lv_obj_t * obj, ଅଚଳ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ଫେରନ୍ତୁ -1);
    LV_CHECK_ARG(class_p != ଖାଲି, ଫେରନ୍ତୁ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    ଯଦି(parent == ଖାଲି) ଫେରନ୍ତୁ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    ପାଇଁ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        ଯଦି(child->class_p == class_p) {
            ଯଦି(child == obj) ଫେରନ୍ତୁ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ଫେରନ୍ତୁ -1;
}

ଶୂନ୍ୟ lv_obj_ବୃକ୍ଷ_walk(lv_obj_t * start_obj, lv_obj_ବୃକ୍ଷ_walk_cb_t cb, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(cb != ଖାଲି, ଫେରନ୍ତୁ);

    walk_core(start_obj, cb, user_data);
}

ଶୂନ୍ୟ lv_obj_dump_ବୃକ୍ଷ(lv_obj_t * start_obj)
{
    ଯଦି(start_obj == ଖାଲି) {
        lv_display_t * disp = lv_display_get_next(ଖାଲି);
        ଯେତେବେଳେ(disp) {
            uint32_t i;
            ପାଇଁ(i = 0; i < disp->screen_cnt; i++) {
                dump_ବୃକ୍ଷ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    ଅନ୍ୟଥା {
        dump_ବୃକ୍ଷ_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ସ୍ଥିର ଶୂନ୍ୟ lv_obj_delete_async_cb(ଶୂନ୍ୟ * obj)
{
    lv_obj_delete(obj);
}

ସ୍ଥିର ଶୂନ୍ୟ obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    ଯଦି(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

ସ୍ଥିର ଶୂନ୍ୟ obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ଖାଲି);
    ଯଦି(obj->is_deleting)
        ଫେରନ୍ତୁ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ଖାଲି);
    ଯଦି(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        ଫେରନ୍ତୁ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ଯଦି(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ଯଦି(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    ଯେତେବେଳେ(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(ଖାଲି);
    ଯେତେବେଳେ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ଯଦି(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            ଯଦି(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ଯଦି(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = ଖାଲି;
            }
            ଯଦି(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = ଖାଲି;
            }
        }

        ଯଦି(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    ଯେତେବେଳେ(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ଯଦି(obj->parent == ଖାଲି) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        ପାଇଁ(i = 0; i < disp->screen_cnt; i++) {
            ଯଦି(disp->screens[i] == obj) ଭାଙ୍ଗ;
        }

        uint32_t id = i;
        ପାଇଁ(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * ଆକାର(lv_obj_t *));
        ଯଦି(new_screens != ଖାଲି) {
            disp->screens = new_screens;
        }
        ଅନ୍ୟଥା {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    ଅନ୍ୟଥା {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = ଖାଲି;

    /*Free the object itself*/
    lv_free(obj);
}

ସ୍ଥିର lv_obj_ବୃକ୍ଷ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_ବୃକ୍ଷ_walk_cb_t cb, ଶୂନ୍ୟ * user_data)
{
    lv_obj_ବୃକ୍ଷ_walk_res_t res = LV_OBJ_ବୃକ୍ଷ_WALK_NEXT;

    ଯଦି(obj == ଖାଲି) {
        lv_display_t * disp = lv_display_get_next(ଖାଲି);
        ଯେତେବେଳେ(disp) {
            uint32_t i;
            ପାଇଁ(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        ଫେରନ୍ତୁ LV_OBJ_ବୃକ୍ଷ_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    ଯଦି(res == LV_OBJ_ବୃକ୍ଷ_WALK_END) ଫେରନ୍ତୁ LV_OBJ_ବୃକ୍ଷ_WALK_END;

    ଯଦି(res != LV_OBJ_ବୃକ୍ଷ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        ପାଇଁ(i = 0; i < lv_obj_get_child_ଗଣନା(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            ଯଦି(res == LV_OBJ_ବୃକ୍ଷ_WALK_END) ଫେରନ୍ତୁ LV_OBJ_ବୃକ୍ଷ_WALK_END;
        }
    }
    ଫେରନ୍ତୁ LV_OBJ_ବୃକ୍ଷ_WALK_NEXT;
}

ସ୍ଥିର ଶୂନ୍ୟ dump_ବୃକ୍ଷ_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != ଖାଲି);
#ଯଦି LV_USE_LOG
    ଅଚଳ ଅକ୍ଷର * id;

#ଯଦି LV_USE_OBJ_ID
    ଅକ୍ଷର buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, ଆକାର(buf));
    ଯଦି(id == ଖାଲି) id = "obj0";
#ଅନ୍ୟଥା
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(2 * depth), "", (ଶୂନ୍ୟ *)obj, id);
#endif /*LV_USE_LOG*/

    ଯଦି(obj->spec_attr && obj->spec_attr->child_cnt) {
        ପାଇଁ(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_ବୃକ୍ଷ_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

ସ୍ଥିର lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != ଖାଲି);

    ଯଦି(obj->spec_attr == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ପାଇଁ(i = 0; i < cnt; i++) {
        ଯଦି(!obj->spec_attr->children[i]->is_deleting) {
            ଫେରନ୍ତୁ obj->spec_attr->children[i];
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

#ଯଦି LV_USE_OBJ_NAME

ସ୍ଥିର lv_obj_t * find_by_name_direct(ଅଚଳ lv_obj_t * parent, ଅଚଳ ଅକ୍ଷର * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ଗଣନା(parent);
    ପାଇଁ(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        ଅକ୍ଷର child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, ଆକାର(child_name_resolved));

        ଯଦି(len == UINT16_MAX) {
            ଯଦି(lv_strcmp(child_name_resolved, name) == 0) ଫେରନ୍ତୁ child;
        }
        ଅନ୍ୟଥା {
            ଯଦି(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                ଫେରନ୍ତୁ child;
            }
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

#endif /*LV_USE_OBJ_NAME*/
