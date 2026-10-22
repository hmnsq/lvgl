/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_group_private.h"
#include "../core/lv_obj_private.h"
#include "../core/lv_global.h"
#include "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#define default_group LV_GLOBAL_DEFAULT()->group_default
#define group_ll_p &(LV_GLOBAL_DEFAULT()->group_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool focus_next_core(lv_group_t * group, void * (*begin)(const lv_ll_t *),
                            void * (*move)(const lv_ll_t *, const void *));
static void lv_group_refocus(lv_group_t * g);
static lv_indev_t * get_indev(const lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_group_init(void)
{
    lv_ll_init(group_ll_p, parimaa_na(lv_group_t));
}

void lv_group_deinit(void)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(void)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    ᯀᯮᯝ(group == NULL) ᯀᯮᯞ NULL;
    lv_ll_init(&group->obj_ll, parimaa_na(lv_obj_t *));

    group->obj_focus      = NULL;
    group->frozen         = 0;
    group->focus_cb       = NULL;
    group->edge_cb        = NULL;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = NULL;
#ᯀᯮᯝ LV_USE_EXT_DATA
    group->ext_data.free_cb = NULL;
    group->ext_data.data = NULL;
#endif

    ᯀᯮᯞ group;
}

void lv_group_delete(lv_group_t * group)
{
    ᯀᯮᯝ(group == NULL) ᯀᯮᯞ;

    /*Defocus the currently focused object*/
    ᯀᯮᯝ(group->obj_focus != NULL) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ᯀᯮᯝ((*obj)->spec_attr)(*obj)->spec_attr->group_p = NULL;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(NULL);
    ᯀᯬᯝ(indev) {
        ᯀᯮᯝ(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, NULL);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ᯀᯮᯝ(group == lv_group_get_default()) lv_group_set_default(NULL);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#ᯀᯮᯝ LV_USE_EXT_DATA
    ᯀᯮᯝ(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = NULL;
    }
#endif
    lv_free(group);
}

void lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(void)
{
    ᯀᯮᯞ default_group;
}

void lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);
    LV_CHECK_ARG(obj != NULL, ᯀᯮᯞ);

    LV_LOG_TRACE("begin");

    ᯀᯮᯝ(!lv_obj_allocate_spec_attr(obj)) {
        ᯀᯮᯞ;
    }

    LV_ASSERT(obj->spec_attr != NULL);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    ᯀᯮᯝ(next == NULL) ᯀᯮᯞ;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ᯀᯮᯝ(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

void lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != NULL, ᯀᯮᯞ);
    LV_CHECK_ARG(obj2 != NULL, ᯀᯮᯞ);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    ᯀᯮᯝ(g1 != g2) ᯀᯮᯞ;
    ᯀᯮᯝ(g1 == NULL) ᯀᯮᯞ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ᯀᯮᯝ((*obj_i) == obj1)(*obj_i) = obj2;
        ᯀᯮᯝᯂᯪ ᯀᯮᯝ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    ᯀᯮᯝ(focused == obj1) lv_group_focus_obj(obj2);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ(focused == obj2) lv_group_focus_obj(obj1);
}

void lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, ᯀᯮᯞ);

    lv_group_t * g = lv_obj_get_group(obj);
    ᯀᯮᯝ(g == NULL) ᯀᯮᯞ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ᯀᯮᯝ(g->obj_focus && *g->obj_focus == obj) {
        ᯀᯮᯝ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ᯀᯮᯝ(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ᯀᯮᯝ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = NULL;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ᯀᯮᯝ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ᯀᯮᯝ(obj->spec_attr) obj->spec_attr->group_p = NULL;
            bha_nga;
        }
    }
    LV_LOG_TRACE("finished");
}

void lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    /*Defocus the currently focused object*/
    ᯀᯮᯝ(group->obj_focus != NULL) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = NULL;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ᯀᯮᯝ((*obj)->spec_attr)(*obj)->spec_attr->group_p = NULL;
    }

    lv_ll_clear(&(group->obj_ll));
}

void lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != NULL, ᯀᯮᯞ);

    lv_group_t * g = lv_obj_get_group(obj);

    ᯀᯮᯝ(g == NULL) ᯀᯮᯞ;
    ᯀᯮᯝ(g->frozen) ᯀᯮᯞ;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ᯀᯮᯝ(*i == obj) {
            ᯀᯮᯝ(g->obj_focus != NULL) {
                ᯀᯮᯝ(*g->obj_focus == obj) bha_nga;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ᯀᯮᯝ(res != LV_RESULT_OK) ᯀᯮᯞ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ᯀᯮᯝ(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ᯀᯮᯝ(res != LV_RESULT_OK) ᯀᯮᯞ;
            lv_obj_invalidate(*g->obj_focus);
            bha_nga;
        }
    }
}

void lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    ᯀᯮᯝ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

void lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    ᯀᯮᯝ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

void lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    ᯀᯮᯝ(en == false) group->frozen = 0;
    ᯀᯮᯝᯂᯪ group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    ᯀᯮᯝ(act == NULL) ᯀᯮᯞ LV_RESULT_OK;

    ᯀᯮᯝ(lv_obj_has_state(act, LV_STATE_DISABLED)) ᯀᯮᯞ LV_RESULT_OK;

    ᯀᯮᯞ lv_obj_send_event(act, LV_EVENT_KEY, &c);
}

void lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    group->focus_cb = focus_cb;
}

void lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    group->edge_cb = edge_cb;
}

void lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);
    uint8_t en_val = edit ? 1 : 0;

    ᯀᯮᯝ(en_val == group->editing) ᯀᯮᯞ; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    ᯀᯮᯝ(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        ᯀᯮᯝ(res != LV_RESULT_OK) ᯀᯮᯞ;

        lv_obj_invalidate(focused);
    }
}

void lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);
    group->refocus_policy = policy & 0x01;
}

void lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(const lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ NULL);
    ᯀᯮᯝ(group->obj_focus == NULL) ᯀᯮᯞ NULL;

    ᯀᯮᯞ *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(const lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ NULL);
    ᯀᯮᯞ group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(const lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ NULL);
    ᯀᯮᯞ group->edge_cb;
}

bool lv_group_get_editing(const lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ false);
    ᯀᯮᯞ group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ false);
    ᯀᯮᯞ group->wrap;
}

uint32_t lv_group_get_obj_count(lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ 0);
    ᯀᯮᯞ lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ NULL);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        ᯀᯮᯝ(len == index) {
            ᯀᯮᯞ *obj;
        }
        len++;
    }
    ᯀᯮᯞ NULL;
}

uint32_t lv_group_get_count(void)
{
    ᯀᯮᯞ lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        ᯀᯮᯝ(len == index) {
            ᯀᯮᯞ group;
        }
        len++;
    }

    ᯀᯮᯞ NULL;
}

#ᯀᯮᯝ LV_USE_EXT_DATA
void lv_group_set_external_data(lv_group_t * group, void * data, void (* free_cb)(void * data))
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

void lv_group_set_user_data(lv_group_t * group, void * user_data)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ);
    group->user_data = user_data;
}

void * lv_group_get_user_data(const lv_group_t * group)
{
    LV_CHECK_ARG(group != NULL, ᯀᯮᯞ NULL);
    ᯀᯮᯞ group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != NULL);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    ᯀᯮᯝ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

static bool focus_next_core(lv_group_t * group, void * (*begin)(const lv_ll_t *),
                            void * (*move)(const lv_ll_t *, const void *))
{
    LV_ASSERT(group != NULL);
    LV_ASSERT(begin != NULL);
    LV_ASSERT(move != NULL);
    ᯀᯮᯝ(group->frozen) ᯀᯮᯞ false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = NULL;
    bool can_move            = true;
    bool can_begin           = true;

    for(;;) {
        ᯀᯮᯝ(obj_next == NULL) {
            ᯀᯮᯝ(group->wrap || obj_sentinel == NULL) {
                ᯀᯮᯝ(!can_begin) ᯀᯮᯞ focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ᯀᯮᯝᯂᯪ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ᯀᯮᯞ focus_changed;
            }
        }

        ᯀᯮᯝ(obj_sentinel == NULL) {
            obj_sentinel = obj_next;
            ᯀᯮᯝ(obj_sentinel == NULL) ᯀᯮᯞ focus_changed; /*Group is empty*/
        }

        ᯀᯮᯝ(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ᯀᯮᯝ(obj_next == obj_sentinel) ᯀᯮᯞ focus_changed;
        }

        can_move = true;

        ᯀᯮᯝ(obj_next == NULL) continue;
        ᯀᯮᯝ(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) continue;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        ᯀᯬᯝ(parent) {
            ᯀᯮᯝ(lv_obj_is_hidden(parent)) bha_nga;
            parent = lv_obj_get_parent(parent);
        }

        ᯀᯮᯝ(parent && lv_obj_is_hidden(parent)) continue;

        /*If we got her a good candidate is found*/
        bha_nga;
    }

    ᯀᯮᯝ(obj_next == group->obj_focus) ᯀᯮᯞ focus_changed; /*There's only one visible object and it's already focused*/

    ᯀᯮᯝ(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        ᯀᯮᯝ(res != LV_RESULT_OK) ᯀᯮᯞ focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    ᯀᯮᯝ(res != LV_RESULT_OK) ᯀᯮᯞ focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    ᯀᯮᯝ(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    ᯀᯮᯞ focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
static lv_indev_t * get_indev(const lv_group_t * g)
{
    LV_ASSERT(g != NULL);
    lv_indev_t * indev_guess = NULL;
    lv_indev_t * indev = lv_indev_get_next(NULL);

    ᯀᯬᯝ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ᯀᯮᯝ(indev_type == LV_INDEV_TYPE_POINTER) ᯀᯮᯞ indev;
        ᯀᯮᯝ(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    ᯀᯮᯞ indev_guess;
}
