/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_obj_private.h"
#include "../lvgl_public.h"
#include "lv_obj_class_private.h"
#include "../indev/lv_indev_private.h"
#include "../display/lv_display_private.h"
#include "../misc/lv_anim_private.h"
#include "../core/lv_global.h"

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
static void lv_obj_delete_async_cb(void * obj);
static void obj_delete_core(lv_obj_t * obj);
static lv_obj_tree_walk_res_t walk_core(lv_obj_t * obj, lv_obj_tree_walk_cb_t cb, void * user_data);
static void dump_tree_core(lv_obj_t * obj, int32_t depth);
static lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#གལ་ཏེ LV_USE_OBJ_NAME
    static lv_obj_t * find_by_name_direct(const lv_obj_t * parent, const var_na * name, size_t len);
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

void lv_obj_delete(lv_obj_t * obj)
{
    གལ་ཏེ(obj == NULL) ཕྱིར;
    གལ་ཏེ(obj->is_deleting) ཕྱིར;

    LV_LOG_TRACE("begin (delete %p)", (void *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = NULL;
    bool act_screen_del = false;
    གལ་ཏེ(par == NULL) {
        disp = lv_obj_get_display(obj);
        གལ་ཏེ(!disp) ཕྱིར;   /*Shouldn't happen*/
        གལ་ཏེ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    གལ་ཏེ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, NULL);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, NULL);
    }

    /*Handle if the active screen was deleted*/
    གལ་ཏེ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = NULL;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (void *)obj);
}

void lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (void *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_count(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    སྐབས(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    གལ་ཏེ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    གལ་ཏེ(lv_obj_get_child_count(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, NULL);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, NULL);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (void *)obj);
}

void lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    གལ་ཏེ(obj == NULL) ཕྱིར;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, NULL);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

void lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != NULL, ཕྱིར);

    lv_obj_delete(a->var);
}

void lv_obj_delete_async(lv_obj_t * obj)
{
    གལ་ཏེ(obj == NULL) ཕྱིར;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);
    LV_CHECK_OBJ(parent, MY_CLASS, ཕྱིར);

    གལ་ཏེ(obj->parent == NULL) {
        LV_LOG_WARN("Can't set the parent of a screen");
        ཕྱིར;
    }

    གལ་ཏེ(parent == obj->parent) {
        ཕྱིར;
    }

    གལ་ཏེ(!lv_obj_allocate_spec_attr(parent)) {
        ཕྱིར;
    }

    /*Add the object to the new parents list first*/
    གལ་ཏེ(lv_obj_add_child(parent, obj) != LV_RESULT_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        ཕྱིར;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, NULL);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, NULL);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

void lv_obj_move_to_index(lv_obj_t * obj, int32_t index)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    གལ་ཏེ(!parent) {
        LV_LOG_WARN("parent is NULL");
        ཕྱིར;
    }

    const uint32_t parent_child_count = lv_obj_get_child_count(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    const int32_t old_index = lv_obj_get_index(obj);
    LV_ASSERT(0 <= old_index);

    གལ་ཏེ(index < 0) {
        index += parent_child_count;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    གལ་ཏེ((index < 0)
       /* Index is same or bigger than parent child count */
       || (index >= (int32_t) parent_child_count)
       /* If both previous and new index are the same */
       || (index == old_index)) {

        ཕྱིར;
    }

    int32_t i = old_index;
    གལ་ཏེ(index < old_index) {
        སྐབས(i > index)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    དེ་མིན {
        སྐབས(i < index) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[index] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, NULL);
    lv_obj_invalidate(parent);
}

void lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, ཕྱིར);
    LV_CHECK_OBJ(obj2, MY_CLASS, ཕྱིར);

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

    གལ་ཏེ(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);

    const lv_obj_t * par = obj;
    const lv_obj_t * act_par;

    do {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } སྐབས(par != NULL);

    ཕྱིར (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);

    const lv_obj_t * scr;

    གལ་ཏེ(obj->parent == NULL) scr = obj;  /*`obj` is a screen*/
    དེ་མིན scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_head = disp_ll_p;
    LV_LL_READ(disp_head, d) {
        uint32_t i;
        ཆེད་དུ(i = 0; i < d->screen_cnt; i++) {
            གལ་ཏེ(d->screens[i] == scr) ཕྱིར d;
        }
    }

    LV_LOG_WARN("No screen found");
    ཕྱིར NULL;
}

lv_obj_t * lv_obj_get_parent(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);

    ཕྱིར obj->parent;
}

lv_obj_t * lv_obj_get_child(const lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);

    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར NULL;

    གལ་ཏེ(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        གལ་ཏེ(idx < 0) ཕྱིར NULL;
    }

    uint32_t idu = (uint32_t) idx;

    གལ་ཏེ(idu < obj->spec_attr->child_cnt) ཕྱིར obj->spec_attr->children[idu];
    དེ་མིན ཕྱིར NULL;
}

lv_obj_t * lv_obj_get_child_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);
    LV_CHECK_ARG(class_p != NULL, ཕྱིར NULL);

    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར NULL;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    གལ་ཏེ(idx >= 0) {
        ཆེད་དུ(i = 0; i < cnt; i++) {
            གལ་ཏེ(obj->spec_attr->children[i]->class_p == class_p) {
                གལ་ཏེ(idx == 0) ཕྱིར obj->spec_attr->children[i];
                དེ་མིན idx--;
            }
        }
    }
    དེ་མིན {
        idx++;   /*-1 means the first child*/
        ཆེད་དུ(i = cnt - 1; i >= 0; i--) {
            གལ་ཏེ(obj->spec_attr->children[i]->class_p == class_p) {
                གལ་ཏེ(idx == 0) ཕྱིར obj->spec_attr->children[i];
                དེ་མིན idx++;
            }
        }
    }
    ཕྱིར NULL;
}

lv_obj_t * lv_obj_get_sibling(const lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);
    LV_CHECK_ARG(obj->parent != NULL, ཕྱིར NULL);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index(obj) + idx;
    གལ་ཏེ(sibling_idx < 0) ཕྱིར NULL;

    ཕྱིར lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(const lv_obj_t * obj, int32_t idx, const lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);
    LV_CHECK_ARG(obj->parent != NULL, ཕྱིར NULL);
    LV_CHECK_ARG(class_p != NULL, ཕྱིར NULL);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_index_by_type(obj, class_p) + idx;
    གལ་ཏེ(sibling_idx < 0) ཕྱིར NULL;

    ཕྱིར lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_count(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར 0);
    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར 0;
    ཕྱིར obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_count_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར 0);
    LV_CHECK_ARG(class_p != NULL, ཕྱིར 0);

    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར 0;

    uint32_t i;
    uint32_t cnt = 0;
    ཆེད་དུ(i = 0; i < obj->spec_attr->child_cnt; i++) {
        གལ་ཏེ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    ཕྱིར cnt;
}

#གལ་ཏེ LV_USE_OBJ_NAME

void lv_obj_set_name(lv_obj_t * obj, const var_na * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);

    གལ་ཏེ(!lv_obj_allocate_spec_attr(obj)) {
        ཕྱིར;
    }

    གལ་ཏེ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((void *)obj->spec_attr->name);

    གལ་ཏེ(name == NULL) {
        obj->spec_attr->name = NULL;
        obj->spec_attr->name_static = 1;
    }
    དེ་མིན {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

void lv_obj_set_name_static(lv_obj_t * obj, const var_na * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);
    LV_CHECK_ARG(name != NULL, ཕྱིར);

    གལ་ཏེ(!lv_obj_allocate_spec_attr(obj)) {
        ཕྱིར;
    }
    གལ་ཏེ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((void *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

const var_na * lv_obj_get_name(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར NULL);

    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར NULL;
    དེ་མིན ཕྱིར obj->spec_attr->name;
}

void lv_obj_get_name_resolved(const lv_obj_t * obj, var_na buf[], size_t buf_size)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར);
    LV_CHECK_ARG(buf != NULL && buf_size > 0, ཕྱིར);

    const var_na * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    var_na name_buf[LV_OBJ_NAME_MAX_LEN];
    གལ་ཏེ(name == NULL) {
        lv_snprintf(name_buf, parimaa_na(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    size_t name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    གལ་ཏེ(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_count(parent);
        uint32_t cnt = 0;
        uint32_t i;
        ཆེད་དུ(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            གལ་ཏེ(child == obj) {
                var_na num_buf[8];
                size_t num_len;
                num_len = lv_snprintf(num_buf, parimaa_na(num_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                གལ་ཏེ(buf_size > name_len + num_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], num_buf);
                }
                དེ་མིན {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_size);
                }
                bha_nga;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            དེ་མིན {
                const var_na * child_name = lv_obj_get_name(child);
                གལ་ཏེ(child_name == NULL) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    size_t class_name_len = lv_strlen(child->class_p->name);
                    གལ་ཏེ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                དེ་མིན {
                    གལ་ཏེ(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    དེ་མིན {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_size);
    }
}

lv_obj_t * lv_obj_get_child_by_name(const lv_obj_t * parent, const var_na * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, ཕྱིར NULL);
    LV_CHECK_ARG(path != NULL, ཕྱིར NULL);
    LV_CHECK_ARG(parent->spec_attr != NULL, ཕྱིར NULL);

    སྐབས(*path) {
        const var_na * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        སྐབས(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        གལ་ཏེ(!child) ཕྱིར NULL; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        གལ་ཏེ(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        གལ་ཏེ(*path == '\0') ཕྱིར child;

        parent = child;
    }

    ཕྱིར NULL;

}

lv_obj_t * lv_obj_find_by_name(const lv_obj_t * parent, const var_na * name)
{
    LV_CHECK_ARG(name != NULL, ཕྱིར NULL);

    གལ་ཏེ(parent == NULL) parent = lv_display_get_screen_active(lv_display_get_default());
    གལ་ཏེ(parent == NULL) ཕྱིར NULL;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    གལ་ཏེ(child) ཕྱིར child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_count(parent);
    uint32_t i;
    ཆེད་དུ(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        གལ་ཏེ(found != NULL) ཕྱིར found;
    }

    ཕྱིར NULL;
}

#endif /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_index(const lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    གལ་ཏེ(parent == NULL) ཕྱིར -1;

    uint32_t i = 0;
    ཆེད་དུ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        གལ་ཏེ(parent->spec_attr->children[i] == obj) ཕྱིར (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    ཕྱིར -1;
}

int32_t lv_obj_get_index_by_type(const lv_obj_t * obj, const lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, ཕྱིར -1);
    LV_CHECK_ARG(class_p != NULL, ཕྱིར -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    གལ་ཏེ(parent == NULL) ཕྱིར 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    ཆེད་དུ(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        གལ་ཏེ(child->class_p == class_p) {
            གལ་ཏེ(child == obj) ཕྱིར idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    ཕྱིར -1;
}

void lv_obj_tree_walk(lv_obj_t * start_obj, lv_obj_tree_walk_cb_t cb, void * user_data)
{
    LV_CHECK_ARG(cb != NULL, ཕྱིར);

    walk_core(start_obj, cb, user_data);
}

void lv_obj_dump_tree(lv_obj_t * start_obj)
{
    གལ་ཏེ(start_obj == NULL) {
        lv_display_t * disp = lv_display_get_next(NULL);
        སྐབས(disp) {
            uint32_t i;
            ཆེད་དུ(i = 0; i < disp->screen_cnt; i++) {
                dump_tree_core(disp->screens[i], 0);
            }
            disp = lv_display_get_next(disp);
        }
    }
    དེ་མིན {
        dump_tree_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_obj_delete_async_cb(void * obj)
{
    lv_obj_delete(obj);
}

static void obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    གལ་ཏེ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

static void obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != NULL);
    གལ་ཏེ(obj->is_deleting)
        ཕྱིར;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_result_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, NULL);
    གལ་ཏེ(res == LV_RESULT_INVALID) {
        obj->is_deleting = false;
        ཕྱིར;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    གལ་ཏེ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    གལ་ཏེ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    སྐབས(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_next(NULL);
    སྐབས(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        གལ་ཏེ(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            གལ་ཏེ(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            གལ་ཏེ(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = NULL;
            }
            གལ་ཏེ(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = NULL;
            }
        }

        གལ་ཏེ(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_next(indev);
    }

    /*Delete all pending async del-s*/
    lv_result_t async_cancel_res = LV_RESULT_OK;
    སྐབས(async_cancel_res == LV_RESULT_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    གལ་ཏེ(obj->parent == NULL) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        ཆེད་དུ(i = 0; i < disp->screen_cnt; i++) {
            གལ་ཏེ(disp->screens[i] == obj) bha_nga;
        }

        uint32_t id = i;
        ཆེད་དུ(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * parimaa_na(lv_obj_t *));
        གལ་ཏེ(new_screens != NULL) {
            disp->screens = new_screens;
        }
        དེ་མིན {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    དེ་མིན {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = NULL;

    /*Free the object itself*/
    lv_free(obj);
}

static lv_obj_tree_walk_res_t walk_core(lv_obj_t * obj, lv_obj_tree_walk_cb_t cb, void * user_data)
{
    lv_obj_tree_walk_res_t res = LV_OBJ_TREE_WALK_NEXT;

    གལ་ཏེ(obj == NULL) {
        lv_display_t * disp = lv_display_get_next(NULL);
        སྐབས(disp) {
            uint32_t i;
            ཆེད་དུ(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_data);
            }
            disp = lv_display_get_next(disp);
        }
        ཕྱིར LV_OBJ_TREE_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_data);

    གལ་ཏེ(res == LV_OBJ_TREE_WALK_END) ཕྱིར LV_OBJ_TREE_WALK_END;

    གལ་ཏེ(res != LV_OBJ_TREE_WALK_SKIP_CHILDREN) {
        uint32_t i;
        ཆེད་དུ(i = 0; i < lv_obj_get_child_count(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_data);
            གལ་ཏེ(res == LV_OBJ_TREE_WALK_END) ཕྱིར LV_OBJ_TREE_WALK_END;
        }
    }
    ཕྱིར LV_OBJ_TREE_WALK_NEXT;
}

static void dump_tree_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != NULL);
#གལ་ཏེ LV_USE_LOG
    const var_na * id;

#གལ་ཏེ LV_USE_OBJ_ID
    var_na buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, parimaa_na(buf));
    གལ་ཏེ(id == NULL) id = "obj0";
#དེ་མིན
    id = "obj0";
#endif

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (ཧྲིལ)(2 * depth), "", (void *)obj, id);
#endif /*LV_USE_LOG*/

    གལ་ཏེ(obj->spec_attr && obj->spec_attr->child_cnt) {
        ཆེད་དུ(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_tree_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

static lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != NULL);

    གལ་ཏེ(obj->spec_attr == NULL) ཕྱིར NULL;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    ཆེད་དུ(i = 0; i < cnt; i++) {
        གལ་ཏེ(!obj->spec_attr->children[i]->is_deleting) {
            ཕྱིར obj->spec_attr->children[i];
        }
    }

    ཕྱིར NULL;
}

#གལ་ཏེ LV_USE_OBJ_NAME

static lv_obj_t * find_by_name_direct(const lv_obj_t * parent, const var_na * name, size_t len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(parent);
    ཆེད་དུ(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        var_na child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, parimaa_na(child_name_resolved));

        གལ་ཏེ(len == UINT16_MAX) {
            གལ་ཏེ(lv_strcmp(child_name_resolved, name) == 0) ཕྱིར child;
        }
        དེ་མིན {
            གལ་ཏེ(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                ཕྱིར child;
            }
        }
    }

    ཕྱིར NULL;
}

#endif /*LV_USE_OBJ_NAME*/
