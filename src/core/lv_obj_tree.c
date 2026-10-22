/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "lv_obj_private.h"
#અન્તર્ભાવ "../lvgl_public.h"
#અન્તર્ભાવ "lv_obj_class_private.h"
#અન્તર્ભાવ "../indev/lv_indev_private.h"
#અન્તર્ભાવ "../display/lv_display_private.h"
#અન્તર્ભાવ "../misc/lv_anim_private.h"
#અન્તર્ભાવ "../core/lv_global.h"

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
સ્થિર શૂન્ય lv_obj_delete_async_cb(શૂન્ય * obj);
સ્થિર શૂન્ય obj_delete_core(lv_obj_t * obj);
સ્થિર lv_obj_વૃક્ષ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_વૃક્ષ_walk_cb_t cb, શૂન્ય * user_data);
સ્થિર શૂન્ય dump_વૃક્ષ_core(lv_obj_t * obj, int32_t depth);
સ્થિર lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#જો LV_USE_OBJ_NAME
    સ્થિર lv_obj_t * find_by_name_direct(અચલ lv_obj_t * parent, અચલ અક્ષર * name, size_t len);
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

શૂન્ય lv_obj_delete(lv_obj_t * obj)
{
    જો(obj == ખાલી) પાછા;
    જો(obj->is_deleting) પાછા;

    LV_LOG_TRACE("begin (delete %p)", (શૂન્ય *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = ખાલી;
    bool act_screen_del = false;
    જો(par == ખાલી) {
        disp = lv_obj_get_display(obj);
        જો(!disp) પાછા;   /*Shouldn't happen*/
        જો(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    જો(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, ખાલી);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, ખાલી);
    }

    /*Handle if the active screen was deleted*/
    જો(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ખાલી;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (શૂન્ય *)obj);
}

શૂન્ય lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (શૂન્ય *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_ગણતરી(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    જ્યારે(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    જો(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    જો(lv_obj_get_child_ગણતરી(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, ખાલી);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, ખાલી);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (શૂન્ય *)obj);
}

શૂન્ય lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    જો(obj == ખાલી) પાછા;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, ખાલી);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

શૂન્ય lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ખાલી, પાછા);

    lv_obj_delete(a->var);
}

શૂન્ય lv_obj_delete_async(lv_obj_t * obj)
{
    જો(obj == ખાલી) પાછા;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

શૂન્ય lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_OBJ(parent, MY_CLASS, પાછા);

    જો(obj->parent == ખાલી) {
        LV_LOG_WARN("Can't set the parent of a screen");
        પાછા;
    }

    જો(parent == obj->parent) {
        પાછા;
    }

    જો(!lv_obj_allocate_spec_attr(parent)) {
        પાછા;
    }

    /*Add the object to the new parents list first*/
    જો(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        પાછા;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, ખાલી);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, ખાલી);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

શૂન્ય lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    જો(!parent) {
        LV_LOG_WARN("parent is NULL");
        પાછા;
    }

    અચલ uint32_t parent_child_ગણતરી = lv_obj_get_child_ગણતરી(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    અચલ int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    જો(index < 0) {
        index += parent_child_ગણતરી;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    જો((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_ગણતરી)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        પાછા;
    }

    int32_t i = old_index;
    જો(index < old_index) {
        જ્યારે(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    નહીંતર {
        જ્યારે(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, ખાલી);
    lv_obj_invalidate(parent);
}

શૂન્ય lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, પાછા);
    LV_CHECK_OBJ(obj2, MY_CLASS, પાછા);

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

    જો(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    અચલ lv_obj_t * par = obj;
    અચલ lv_obj_t * act_par;

    કરો {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } જ્યારે(par != ખાલી);

    પાછા (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    અચલ lv_obj_t * scr;

    જો(obj->parent == ખાલી) scr = obj;  /*`obj` is a screen*/
    નહીંતર scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        માટે(i = 0; i < d->screen_cnt; i++) {
            જો(d->screens[i] == scr) પાછા d;
        }
    }

    LV_LOG_WARN("No screen found");
    પાછા ખાલી;
}

lv_obj_t * lv_obj_get_parent(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    પાછા obj->parent;
}

lv_obj_t * lv_obj_get_child(અચલ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    જો(obj->spec_attr == ખાલી) પાછા ખાલી;

    જો(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        જો(idx < 0) પાછા ખાલી;
    }

    uint32_t idu = (uint32_t) idx;

    જો(idu < obj->spec_attr->child_cnt) પાછા obj->spec_attr->children[idu];
    નહીંતર પાછા ખાલી;
}

lv_obj_t * lv_obj_get_child_by_type(અચલ lv_obj_t * obj, int32_t idx, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);
    LV_CHECK_ARG(class_p != ખાલી, પાછા ખાલી);

    જો(obj->spec_attr == ખાલી) પાછા ખાલી;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    જો(idx >= 0) {
        માટે(i = 0; i < cnt; i++) {
            જો(obj->spec_attr->children[i]->class_p == class_p) {
                જો(idx == 0) પાછા obj->spec_attr->children[i];
                નહીંતર idx--;
            }
        }
    }
    નહીંતર {
        idx++;   /*-1 means the first child*/
        માટે(i = cnt - 1; i >= 0; i--) {
            જો(obj->spec_attr->children[i]->class_p == class_p) {
                જો(idx == 0) પાછા obj->spec_attr->children[i];
                નહીંતર idx++;
            }
        }
    }
    પાછા ખાલી;
}

lv_obj_t * lv_obj_get_sibling(અચલ lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);
    LV_CHECK_ARG(obj->parent != ખાલી, પાછા ખાલી);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    જો(sibling_idx < 0) પાછા ખાલી;

    પાછા lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(અચલ lv_obj_t * obj, int32_t idx, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);
    LV_CHECK_ARG(obj->parent != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(class_p != ખાલી, પાછા ખાલી);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    જો(sibling_idx < 0) પાછા ખાલી;

    પાછા lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_ગણતરી(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા 0);
    જો(obj->spec_attr == ખાલી) પાછા 0;
    પાછા obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_ગણતરી_by_type(અચલ lv_obj_t * obj, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા 0);
    LV_CHECK_ARG(class_p != ખાલી, પાછા 0);

    જો(obj->spec_attr == ખાલી) પાછા 0;

    uint32_t i;
    uint32_t cnt = 0;
    માટે(i = 0; i < obj->spec_attr->child_cnt; i++) {
        જો(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    પાછા cnt;
}

#જો LV_USE_OBJ_NAME

શૂન્ય lv_obj_set_name(lv_obj_t * obj, અચલ અક્ષર * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);

    જો(!lv_obj_allocate_spec_attr(obj)) {
        પાછા;
    }

    જો(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((શૂન્ય *)obj->spec_attr->name);

    જો(name == ખાલી) {
        obj->spec_attr->name = ખાલી;
        obj->spec_attr->name_static = 1;
    }
    નહીંતર {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

શૂન્ય lv_obj_set_name_static(lv_obj_t * obj, અચલ અક્ષર * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG(name != ખાલી, પાછા);

    જો(!lv_obj_allocate_spec_attr(obj)) {
        પાછા;
    }
    જો(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((શૂન્ય *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

અચલ અક્ષર * lv_obj_get_name(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા ખાલી);

    જો(obj->spec_attr == ખાલી) પાછા ખાલી;
    નહીંતર પાછા obj->spec_attr->name;
}

શૂન્ય lv_obj_get_name_resolved(અચલ lv_obj_t * obj, અક્ષર buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા);
    LV_CHECK_ARG(buf != ખાલી && buf_size > 0, પાછા);

    અચલ અક્ષર * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    અક્ષર name_buf[LV_OBJ_NAME_MAX_LEN];
    જો(name == ખાલી) {
        lv_snprintf(name_buf, કદ(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    જો(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_ગણતરી(parent);
        uint32_t cnt = 0;
        uint32_t i;
        માટે(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            જો(child == obj) {
                અક્ષર અંક_buf[8];
                size_t અંક_len;
                અંક_len = lv_snprintf(અંક_buf, કદ(અંક_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                જો(buf_size > name_len + અંક_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], અંક_buf);
                }
                નહીંતર {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                તોડો;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            નહીંતર {
                અચલ અક્ષર * child_name = lv_obj_get_name(child);
                જો(child_name == ખાલી) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    જો(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                નહીંતર {
                    જો(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    નહીંતર {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(અચલ lv_obj_t * parent, અચલ અક્ષર * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, પાછા ખાલી);
    LV_CHECK_ARG(path != ખાલી, પાછા ખાલી);
    LV_CHECK_ARG(parent->spec_attr != ખાલી, પાછા ખાલી);

    જ્યારે(*path) {
        અચલ અક્ષર * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        જ્યારે(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        જો(!child) પાછા ખાલી; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        જો(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        જો(*path == '\0') પાછા child;

        parent = child;
    }

    પાછા ખાલી;

}

lv_obj_t * lv_obj_find_by_name(અચલ lv_obj_t * parent, અચલ અક્ષર * name)
{
    LV_CHECK_ARG(name != ખાલી, પાછા ખાલી);

    જો(parent == ખાલી) parent = lv_display_get_screen_active(lv_display_get_default());
    જો(parent == ખાલી) પાછા ખાલી;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    જો(child) પાછા child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_ગણતરી(parent);
    uint32_t i;
    માટે(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        જો(found != ખાલી) પાછા found;
    }

    પાછા ખાલી;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(અચલ lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    જો(parent == ખાલી) પાછા -1;

    uint32_t i = 0;
    માટે(i = 0; i < parent->spec_attr->child_cnt; i++) {
        જો(parent->spec_attr->children[i] == obj) પાછા (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    પાછા -1;
}

int32_t lv_obj_get_index_by_type(અચલ lv_obj_t * obj, અચલ lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, પાછા -1);
    LV_CHECK_ARG(class_p != ખાલી, પાછા -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    જો(parent == ખાલી) પાછા 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    માટે(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        જો(child->class_p == class_p) {
            જો(child == obj) પાછા idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    પાછા -1;
}

શૂન્ય lv_obj_વૃક્ષ_walk(lv_obj_t * start_obj, lv_obj_વૃક્ષ_walk_cb_t cb, શૂન્ય * user_data)
{
    LV_CHECK_ARG(cb != ખાલી, પાછા);

    walk_core(start_obj, cb, user_data);
}

શૂન્ય lv_obj_dump_વૃક્ષ(lv_obj_t * start_obj)
{
    જો(start_obj == ખાલી) {
        lv_display_t * disp = lv_display_get_next(ખાલી);
        જ્યારે(disp) {
            uint32_t i;
            માટે(i = 0; i < disp->screen_cnt; i++) {
                dump_વૃક્ષ_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    નહીંતર {
        dump_વૃક્ષ_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

સ્થિર શૂન્ય lv_obj_delete_async_cb(શૂન્ય * obj)
{
    lv_obj_delete(obj);
}

સ્થિર શૂન્ય obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    જો(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

સ્થિર શૂન્ય obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ખાલી);
    જો(obj->is_deleting)
        પાછા;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ખાલી);
    જો(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        પાછા;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    જો(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    જો(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    જ્યારે(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(ખાલી);
    જ્યારે(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        જો(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            જો(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            જો(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = ખાલી;
            }
            જો(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = ખાલી;
            }
        }

        જો(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    જ્યારે(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    જો(obj->parent == ખાલી) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        માટે(i = 0; i < disp->screen_cnt; i++) {
            જો(disp->screens[i] == obj) તોડો;
        }

        uint32_t id = i;
        માટે(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * કદ(lv_obj_t *));
        જો(new_screens != ખાલી) {
            disp->screens = new_screens;
        }
        નહીંતર {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    નહીંતર {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = ખાલી;

    /*Free the object itself*/
    lv_free(obj);
}

સ્થિર lv_obj_વૃક્ષ_walk_res_t walk_core(lv_obj_t * obj, lv_obj_વૃક્ષ_walk_cb_t cb, શૂન્ય * user_data)
{
    lv_obj_વૃક્ષ_walk_res_t res = LV_OBJ_વૃક્ષ_WALK_NEXT;

    જો(obj == ખાલી) {
        lv_display_t * disp = lv_display_get_next(ખાલી);
        જ્યારે(disp) {
            uint32_t i;
            માટે(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        પાછા LV_OBJ_વૃક્ષ_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    જો(res == LV_OBJ_વૃક્ષ_WALK_END) પાછા LV_OBJ_વૃક્ષ_WALK_END;

    જો(res != LV_OBJ_વૃક્ષ_WALK_SKIP_CHILDREN) {
        uint32_t i;
        માટે(i = 0; i < lv_obj_get_child_ગણતરી(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            જો(res == LV_OBJ_વૃક્ષ_WALK_END) પાછા LV_OBJ_વૃક્ષ_WALK_END;
        }
    }
    પાછા LV_OBJ_વૃક્ષ_WALK_NEXT;
}

સ્થિર શૂન્ય dump_વૃક્ષ_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != ખાલી);
#જો LV_USE_LOG
    અચલ અક્ષર * id;

#જો LV_USE_OBJ_ID
    અક્ષર buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, કદ(buf));
    જો(id == ખાલી) id = "obj0";
#નહીંતર
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (પૂર્ણાંક)(2 * depth), "", (શૂન્ય *)obj, id);
#endif /*LV_USE_LOG*/

    જો(obj->spec_attr && obj->spec_attr->child_cnt) {
        માટે(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_વૃક્ષ_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

સ્થિર lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != ખાલી);

    જો(obj->spec_attr == ખાલી) પાછા ખાલી;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    માટે(i = 0; i < cnt; i++) {
        જો(!obj->spec_attr->children[i]->is_deleting) {
            પાછા obj->spec_attr->children[i];
        }
    }

    પાછા ખાલી;
}

#જો LV_USE_OBJ_NAME

સ્થિર lv_obj_t * find_by_name_direct(અચલ lv_obj_t * parent, અચલ અક્ષર * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_ગણતરી(parent);
    માટે(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        અક્ષર child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, કદ(child_name_resolved));

        જો(len == UINT16_MAX) {
            જો(lv_strcmp(child_name_resolved, name) == 0) પાછા child;
        }
        નહીંતર {
            જો(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                પાછા child;
            }
        }
    }

    પાછા ખાલી;
}

#endif /*LV_USE_OBJ_NAME*/
