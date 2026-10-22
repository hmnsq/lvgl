/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "lv_obj_private.h"
#അന്തര്ഭാവ "../lvgl_public.h"
#അന്തര്ഭാവ "lv_obj_class_private.h"
#അന്തര്ഭാവ "../indev/lv_indev_private.h"
#അന്തര്ഭാവ "../display/lv_display_private.h"
#അന്തര്ഭാവ "../misc/lv_anim_private.h"
#അന്തര്ഭാവ "../core/lv_global.h"

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
സ്ഥിതം ശൂന്യം lv_obj_delete_async_cb(ശൂന്യം * obj);
സ്ഥിതം ശൂന്യം obj_delete_core(lv_obj_t * obj);
സ്ഥിതം lv_obj_വൃക്ഷം_walk_res_t walk_core(lv_obj_t * obj, lv_obj_വൃക്ഷം_walk_cb_t cb, ശൂന്യം * user_data);
സ്ഥിതം ശൂന്യം dump_വൃക്ഷം_core(lv_obj_t * obj, int32_t depth);
സ്ഥിതം lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#എങ്കിൽ LV_USE_OBJ_NAME
    സ്ഥിതം lv_obj_t * find_by_name_direct(സ്ഥിരം lv_obj_t * parent, സ്ഥിരം അക്ഷരം * name, size_t len);
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

ശൂന്യം lv_obj_delete(lv_obj_t * obj)
{
    എങ്കിൽ(obj == ശൂന്യ) തിരികെ;
    എങ്കിൽ(obj->is_deleting) തിരികെ;

    LV_LOG_TRACE("begin (delete %p)", (ശൂന്യം *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = ശൂന്യ;
    bool act_screen_del = false;
    എങ്കിൽ(par == ശൂന്യ) {
        disp = lv_obj_get_display(obj);
        എങ്കിൽ(!disp) തിരികെ;   /*Shouldn't happen*/
        എങ്കിൽ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    എങ്കിൽ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, ശൂന്യ);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, ശൂന്യ);
    }

    /*Handle if the active screen was deleted*/
    എങ്കിൽ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ശൂന്യ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (ശൂന്യം *)obj);
}

ശൂന്യം lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (ശൂന്യം *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_എണ്ണം(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    ആകുമ്പോൾ(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    എങ്കിൽ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    എങ്കിൽ(lv_obj_get_child_എണ്ണം(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, ശൂന്യ);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, ശൂന്യ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (ശൂന്യം *)obj);
}

ശൂന്യം lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    എങ്കിൽ(obj == ശൂന്യ) തിരികെ;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, ശൂന്യ);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

ശൂന്യം lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ശൂന്യ, തിരികെ);

    lv_obj_delete(a->var);
}

ശൂന്യം lv_obj_delete_async(lv_obj_t * obj)
{
    എങ്കിൽ(obj == ശൂന്യ) തിരികെ;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

ശൂന്യം lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);
    LV_CHECK_OBJ(parent, MY_CLASS, തിരികെ);

    എങ്കിൽ(obj->parent == ശൂന്യ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        തിരികെ;
    }

    എങ്കിൽ(parent == obj->parent) {
        തിരികെ;
    }

    എങ്കിൽ(!lv_obj_allocate_spec_attr(parent)) {
        തിരികെ;
    }

    /*Add the object to the new parents list first*/
    എങ്കിൽ(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        തിരികെ;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, ശൂന്യ);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, ശൂന്യ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

ശൂന്യം lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    എങ്കിൽ(!parent) {
        LV_LOG_WARN("parent is NULL");
        തിരികെ;
    }

    സ്ഥിരം uint32_t parent_child_എണ്ണം = lv_obj_get_child_എണ്ണം(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    സ്ഥിരം int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    എങ്കിൽ(index < 0) {
        index += parent_child_എണ്ണം;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    എങ്കിൽ((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_എണ്ണം)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        തിരികെ;
    }

    int32_t i = old_index;
    എങ്കിൽ(index < old_index) {
        ആകുമ്പോൾ(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    അല്ലെങ്കിൽ {
        ആകുമ്പോൾ(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, ശൂന്യ);
    lv_obj_invalidate(parent);
}

ശൂന്യം lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, തിരികെ);
    LV_CHECK_OBJ(obj2, MY_CLASS, തിരികെ);

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

    എങ്കിൽ(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);

    സ്ഥിരം lv_obj_t * par = obj;
    സ്ഥിരം lv_obj_t * act_par;

    ചെയ്യുക {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } ആകുമ്പോൾ(par != ശൂന്യ);

    തിരികെ (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);

    സ്ഥിരം lv_obj_t * scr;

    എങ്കിൽ(obj->parent == ശൂന്യ) scr = obj;  /*`obj` is a screen*/
    അല്ലെങ്കിൽ scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        വേണ്ടി(i = 0; i < d->screen_cnt; i++) {
            എങ്കിൽ(d->screens[i] == scr) തിരികെ d;
        }
    }

    LV_LOG_WARN("No screen found");
    തിരികെ ശൂന്യ;
}

lv_obj_t * lv_obj_get_parent(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);

    തിരികെ obj->parent;
}

lv_obj_t * lv_obj_get_child(സ്ഥിരം lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ ശൂന്യ;

    എങ്കിൽ(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        എങ്കിൽ(idx < 0) തിരികെ ശൂന്യ;
    }

    uint32_t idu = (uint32_t) idx;

    എങ്കിൽ(idu < obj->spec_attr->child_cnt) തിരികെ obj->spec_attr->children[idu];
    അല്ലെങ്കിൽ തിരികെ ശൂന്യ;
}

lv_obj_t * lv_obj_get_child_by_type(സ്ഥിരം lv_obj_t * obj, int32_t idx, സ്ഥിരം lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(class_p != ശൂന്യ, തിരികെ ശൂന്യ);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ ശൂന്യ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    എങ്കിൽ(idx >= 0) {
        വേണ്ടി(i = 0; i < cnt; i++) {
            എങ്കിൽ(obj->spec_attr->children[i]->class_p == class_p) {
                എങ്കിൽ(idx == 0) തിരികെ obj->spec_attr->children[i];
                അല്ലെങ്കിൽ idx--;
            }
        }
    }
    അല്ലെങ്കിൽ {
        idx++;   /*-1 means the first child*/
        വേണ്ടി(i = cnt - 1; i >= 0; i--) {
            എങ്കിൽ(obj->spec_attr->children[i]->class_p == class_p) {
                എങ്കിൽ(idx == 0) തിരികെ obj->spec_attr->children[i];
                അല്ലെങ്കിൽ idx++;
            }
        }
    }
    തിരികെ ശൂന്യ;
}

lv_obj_t * lv_obj_get_sibling(സ്ഥിരം lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(obj->parent != ശൂന്യ, തിരികെ ശൂന്യ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    എങ്കിൽ(sibling_idx < 0) തിരികെ ശൂന്യ;

    തിരികെ lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(സ്ഥിരം lv_obj_t * obj, int32_t idx, സ്ഥിരം lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(obj->parent != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(class_p != ശൂന്യ, തിരികെ ശൂന്യ);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    എങ്കിൽ(sibling_idx < 0) തിരികെ ശൂന്യ;

    തിരികെ lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_എണ്ണം(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ 0);
    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ 0;
    തിരികെ obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_എണ്ണം_by_type(സ്ഥിരം lv_obj_t * obj, സ്ഥിരം lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ 0);
    LV_CHECK_ARG(class_p != ശൂന്യ, തിരികെ 0);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ 0;

    uint32_t i;
    uint32_t cnt = 0;
    വേണ്ടി(i = 0; i < obj->spec_attr->child_cnt; i++) {
        എങ്കിൽ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    തിരികെ cnt;
}

#എങ്കിൽ LV_USE_OBJ_NAME

ശൂന്യം lv_obj_set_name(lv_obj_t * obj, സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);

    എങ്കിൽ(!lv_obj_allocate_spec_attr(obj)) {
        തിരികെ;
    }

    എങ്കിൽ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ശൂന്യം *)obj->spec_attr->name);

    എങ്കിൽ(name == ശൂന്യ) {
        obj->spec_attr->name = ശൂന്യ;
        obj->spec_attr->name_static = 1;
    }
    അല്ലെങ്കിൽ {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

ശൂന്യം lv_obj_set_name_static(lv_obj_t * obj, സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);
    LV_CHECK_ARG(name != ശൂന്യ, തിരികെ);

    എങ്കിൽ(!lv_obj_allocate_spec_attr(obj)) {
        തിരികെ;
    }
    എങ്കിൽ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((ശൂന്യം *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

സ്ഥിരം അക്ഷരം * lv_obj_get_name(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ ശൂന്യ);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ ശൂന്യ;
    അല്ലെങ്കിൽ തിരികെ obj->spec_attr->name;
}

ശൂന്യം lv_obj_get_name_resolved(സ്ഥിരം lv_obj_t * obj, അക്ഷരം buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ);
    LV_CHECK_ARG(buf != ശൂന്യ && buf_size > 0, തിരികെ);

    സ്ഥിരം അക്ഷരം * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    അക്ഷരം name_buf[LV_OBJ_NAME_MAX_LEN];
    എങ്കിൽ(name == ശൂന്യ) {
        lv_snprintf(name_buf, വലുപ്പം(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    എങ്കിൽ(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_എണ്ണം(parent);
        uint32_t cnt = 0;
        uint32_t i;
        വേണ്ടി(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            എങ്കിൽ(child == obj) {
                അക്ഷരം അക്കം_buf[8];
                size_t അക്കം_len;
                അക്കം_len = lv_snprintf(അക്കം_buf, വലുപ്പം(അക്കം_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                എങ്കിൽ(buf_size > name_len + അക്കം_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], അക്കം_buf);
                }
                അല്ലെങ്കിൽ {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                നിർത്തുക;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            അല്ലെങ്കിൽ {
                സ്ഥിരം അക്ഷരം * child_name = lv_obj_get_name(child);
                എങ്കിൽ(child_name == ശൂന്യ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    എങ്കിൽ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                അല്ലെങ്കിൽ {
                    എങ്കിൽ(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    അല്ലെങ്കിൽ {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(സ്ഥിരം lv_obj_t * parent, സ്ഥിരം അക്ഷരം * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(path != ശൂന്യ, തിരികെ ശൂന്യ);
    LV_CHECK_ARG(parent->spec_attr != ശൂന്യ, തിരികെ ശൂന്യ);

    ആകുമ്പോൾ(*path) {
        സ്ഥിരം അക്ഷരം * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        ആകുമ്പോൾ(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        എങ്കിൽ(!child) തിരികെ ശൂന്യ; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        എങ്കിൽ(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        എങ്കിൽ(*path == '\0') തിരികെ child;

        parent = child;
    }

    തിരികെ ശൂന്യ;

}

lv_obj_t * lv_obj_find_by_name(സ്ഥിരം lv_obj_t * parent, സ്ഥിരം അക്ഷരം * name)
{
    LV_CHECK_ARG(name != ശൂന്യ, തിരികെ ശൂന്യ);

    എങ്കിൽ(parent == ശൂന്യ) parent = lv_display_get_screen_active(lv_display_get_default());
    എങ്കിൽ(parent == ശൂന്യ) തിരികെ ശൂന്യ;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    എങ്കിൽ(child) തിരികെ child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_എണ്ണം(parent);
    uint32_t i;
    വേണ്ടി(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        എങ്കിൽ(found != ശൂന്യ) തിരികെ found;
    }

    തിരികെ ശൂന്യ;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(സ്ഥിരം lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    എങ്കിൽ(parent == ശൂന്യ) തിരികെ -1;

    uint32_t i = 0;
    വേണ്ടി(i = 0; i < parent->spec_attr->child_cnt; i++) {
        എങ്കിൽ(parent->spec_attr->children[i] == obj) തിരികെ (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    തിരികെ -1;
}

int32_t lv_obj_get_index_by_type(സ്ഥിരം lv_obj_t * obj, സ്ഥിരം lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, തിരികെ -1);
    LV_CHECK_ARG(class_p != ശൂന്യ, തിരികെ -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    എങ്കിൽ(parent == ശൂന്യ) തിരികെ 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    വേണ്ടി(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        എങ്കിൽ(child->class_p == class_p) {
            എങ്കിൽ(child == obj) തിരികെ idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    തിരികെ -1;
}

ശൂന്യം lv_obj_വൃക്ഷം_walk(lv_obj_t * start_obj, lv_obj_വൃക്ഷം_walk_cb_t cb, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(cb != ശൂന്യ, തിരികെ);

    walk_core(start_obj, cb, user_data);
}

ശൂന്യം lv_obj_dump_വൃക്ഷം(lv_obj_t * start_obj)
{
    എങ്കിൽ(start_obj == ശൂന്യ) {
        lv_display_t * disp = lv_display_get_next(ശൂന്യ);
        ആകുമ്പോൾ(disp) {
            uint32_t i;
            വേണ്ടി(i = 0; i < disp->screen_cnt; i++) {
                dump_വൃക്ഷം_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    അല്ലെങ്കിൽ {
        dump_വൃക്ഷം_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

സ്ഥിതം ശൂന്യം lv_obj_delete_async_cb(ശൂന്യം * obj)
{
    lv_obj_delete(obj);
}

സ്ഥിതം ശൂന്യം obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    എങ്കിൽ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

സ്ഥിതം ശൂന്യം obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ശൂന്യ);
    എങ്കിൽ(obj->is_deleting)
        തിരികെ;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ശൂന്യ);
    എങ്കിൽ(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        തിരികെ;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    എങ്കിൽ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    എങ്കിൽ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    ആകുമ്പോൾ(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(ശൂന്യ);
    ആകുമ്പോൾ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        എങ്കിൽ(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            എങ്കിൽ(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            എങ്കിൽ(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = ശൂന്യ;
            }
            എങ്കിൽ(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = ശൂന്യ;
            }
        }

        എങ്കിൽ(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    ആകുമ്പോൾ(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    എങ്കിൽ(obj->parent == ശൂന്യ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        വേണ്ടി(i = 0; i < disp->screen_cnt; i++) {
            എങ്കിൽ(disp->screens[i] == obj) നിർത്തുക;
        }

        uint32_t id = i;
        വേണ്ടി(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * വലുപ്പം(lv_obj_t *));
        എങ്കിൽ(new_screens != ശൂന്യ) {
            disp->screens = new_screens;
        }
        അല്ലെങ്കിൽ {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    അല്ലെങ്കിൽ {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = ശൂന്യ;

    /*Free the object itself*/
    lv_free(obj);
}

സ്ഥിതം lv_obj_വൃക്ഷം_walk_res_t walk_core(lv_obj_t * obj, lv_obj_വൃക്ഷം_walk_cb_t cb, ശൂന്യം * user_data)
{
    lv_obj_വൃക്ഷം_walk_res_t res = LV_OBJ_വൃക്ഷം_WALK_NEXT;

    എങ്കിൽ(obj == ശൂന്യ) {
        lv_display_t * disp = lv_display_get_next(ശൂന്യ);
        ആകുമ്പോൾ(disp) {
            uint32_t i;
            വേണ്ടി(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        തിരികെ LV_OBJ_വൃക്ഷം_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    എങ്കിൽ(res == LV_OBJ_വൃക്ഷം_WALK_END) തിരികെ LV_OBJ_വൃക്ഷം_WALK_END;

    എങ്കിൽ(res != LV_OBJ_വൃക്ഷം_WALK_SKIP_CHILDREN) {
        uint32_t i;
        വേണ്ടി(i = 0; i < lv_obj_get_child_എണ്ണം(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            എങ്കിൽ(res == LV_OBJ_വൃക്ഷം_WALK_END) തിരികെ LV_OBJ_വൃക്ഷം_WALK_END;
        }
    }
    തിരികെ LV_OBJ_വൃക്ഷം_WALK_NEXT;
}

സ്ഥിതം ശൂന്യം dump_വൃക്ഷം_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != ശൂന്യ);
#എങ്കിൽ LV_USE_LOG
    സ്ഥിരം അക്ഷരം * id;

#എങ്കിൽ LV_USE_OBJ_ID
    അക്ഷരം buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, വലുപ്പം(buf));
    എങ്കിൽ(id == ശൂന്യ) id = "obj0";
#അല്ലെങ്കിൽ
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (പൂർണ്ണസംഖ്യ)(2 * depth), "", (ശൂന്യം *)obj, id);
#endif /*LV_USE_LOG*/

    എങ്കിൽ(obj->spec_attr && obj->spec_attr->child_cnt) {
        വേണ്ടി(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_വൃക്ഷം_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

സ്ഥിതം lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != ശൂന്യ);

    എങ്കിൽ(obj->spec_attr == ശൂന്യ) തിരികെ ശൂന്യ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    വേണ്ടി(i = 0; i < cnt; i++) {
        എങ്കിൽ(!obj->spec_attr->children[i]->is_deleting) {
            തിരികെ obj->spec_attr->children[i];
        }
    }

    തിരികെ ശൂന്യ;
}

#എങ്കിൽ LV_USE_OBJ_NAME

സ്ഥിതം lv_obj_t * find_by_name_direct(സ്ഥിരം lv_obj_t * parent, സ്ഥിരം അക്ഷരം * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_എണ്ണം(parent);
    വേണ്ടി(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        അക്ഷരം child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, വലുപ്പം(child_name_resolved));

        എങ്കിൽ(len == UINT16_MAX) {
            എങ്കിൽ(lv_strcmp(child_name_resolved, name) == 0) തിരികെ child;
        }
        അല്ലെങ്കിൽ {
            എങ്കിൽ(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                തിരികെ child;
            }
        }
    }

    തിരികെ ശൂന്യ;
}

#endif /*LV_USE_OBJ_NAME*/
