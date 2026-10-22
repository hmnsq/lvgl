/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#અન્તર્ભાવ "lv_group_private.h"
#અન્તર્ભાવ "../core/lv_obj_private.h"
#અન્તર્ભાવ "../core/lv_global.h"
#અન્તર્ભાવ "../lvgl_public.h"

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
સ્થિર bool focus_next_core(lv_group_t * group, શૂન્ય * (*begin)(અચલ lv_ll_t *),
                            શૂન્ય * (*move)(અચલ lv_ll_t *, અચલ શૂન્ય *));
સ્થિર શૂન્ય lv_group_refocus(lv_group_t * g);
સ્થિર lv_indev_t * get_indev(અચલ lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

શૂન્ય lv_group_init(શૂન્ય)
{
    lv_ll_init(group_ll_p, કદ(lv_group_t));
}

શૂન્ય lv_group_deinit(શૂન્ય)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(શૂન્ય)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    જો(group == ખાલી) પાછા ખાલી;
    lv_ll_init(&group->obj_ll, કદ(lv_obj_t *));

    group->obj_focus      = ખાલી;
    group->frozen         = 0;
    group->focus_cb       = ખાલી;
    group->edge_cb        = ખાલી;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = ખાલી;
#જો LV_USE_EXT_DATA
    group->ext_data.free_cb = ખાલી;
    group->ext_data.data = ખાલી;
#endif

    પાછા group;
}

શૂન્ય lv_group_delete(lv_group_t * group)
{
    જો(group == ખાલી) પાછા;

    /*Defocus the currently focused object*/
    જો(group->obj_focus != ખાલી) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        જો((*obj)->spec_attr)(*obj)->spec_attr->group_p = ખાલી;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(ખાલી);
    જ્યારે(indev) {
        જો(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, ખાલી);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    જો(group == lv_group_get_default()) lv_group_set_default(ખાલી);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#જો LV_USE_EXT_DATA
    જો(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = ખાલી;
    }
#endif
    lv_free(group);
}

શૂન્ય lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(શૂન્ય)
{
    પાછા default_group;
}

શૂન્ય lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    LV_LOG_TRACE("begin");

    જો(!lv_obj_allocate_spec_attr(obj)) {
        પાછા;
    }

    LV_ASSERT(obj->spec_attr != ખાલી);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    જો(next == ખાલી) પાછા;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    જો(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

શૂન્ય lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ખાલી, પાછા);
    LV_CHECK_ARG(obj2 != ખાલી, પાછા);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    જો(g1 != g2) પાછા;
    જો(g1 == ખાલી) પાછા;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        જો((*obj_i) == obj1)(*obj_i) = obj2;
        નહીંતર જો((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    જો(focused == obj1) lv_group_focus_obj(obj2);
    નહીંતર જો(focused == obj2) lv_group_focus_obj(obj1);
}

શૂન્ય lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    lv_group_t * g = lv_obj_get_group(obj);
    જો(g == ખાલી) પાછા;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    જો(g->obj_focus && *g->obj_focus == obj) {
        જો(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        જો(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    જો(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ખાલી;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        જો(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            જો(obj->spec_attr) obj->spec_attr->group_p = ખાલી;
            તોડો;
        }
    }
    LV_LOG_TRACE("finished");
}

શૂન્ય lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    /*Defocus the currently focused object*/
    જો(group->obj_focus != ખાલી) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = ખાલી;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        જો((*obj)->spec_attr)(*obj)->spec_attr->group_p = ખાલી;
    }

    lv_ll_clear(&(group->obj_ll));
}

શૂન્ય lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ખાલી, પાછા);

    lv_group_t * g = lv_obj_get_group(obj);

    જો(g == ખાલી) પાછા;
    જો(g->frozen) પાછા;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        જો(*i == obj) {
            જો(g->obj_focus != ખાલી) {
                જો(*g->obj_focus == obj) તોડો;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                જો(res != LV_RESULT_OK) પાછા;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            જો(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            જો(res != LV_RESULT_OK) પાછા;
            lv_obj_invalidate(*g->obj_focus);
            તોડો;
        }
    }
}

શૂન્ય lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    જો(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

શૂન્ય lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    જો(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

શૂન્ય lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    જો(en == false) group->frozen = 0;
    નહીંતર group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != ખાલી, પાછા LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    જો(act == ખાલી) પાછા LV_RESULT_OK;

    જો(lv_obj_has_state(act, LV_STATE_DISABLED)) પાછા LV_RESULT_OK;

    પાછા lv_obj_send_event(act, LV_EVENT_ચાવી, &c);
}

શૂન્ય lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    group->focus_cb = focus_cb;
}

શૂન્ય lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    group->edge_cb = edge_cb;
}

શૂન્ય lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);
    uint8_t en_val = edit ? 1 : 0;

    જો(en_val == group->editing) પાછા; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    જો(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        જો(res != LV_RESULT_OK) પાછા;

        lv_obj_invalidate(focused);
    }
}

શૂન્ય lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);
    group->refocus_policy = policy & 0x01;
}

શૂન્ય lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(અચલ lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા ખાલી);
    જો(group->obj_focus == ખાલી) પાછા ખાલી;

    પાછા *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(અચલ lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા ખાલી);
    પાછા group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(અચલ lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા ખાલી);
    પાછા group->edge_cb;
}

bool lv_group_get_editing(અચલ lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા false);
    પાછા group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા false);
    પાછા group->wrap;
}

uint32_t lv_group_get_obj_ગણતરી(lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા 0);
    પાછા lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != ખાલી, પાછા ખાલી);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        જો(len == index) {
            પાછા *obj;
        }
        len++;
    }
    પાછા ખાલી;
}

uint32_t lv_group_get_ગણતરી(શૂન્ય)
{
    પાછા lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        જો(len == index) {
            પાછા group;
        }
        len++;
    }

    પાછા ખાલી;
}

#જો LV_USE_EXT_DATA
શૂન્ય lv_group_set_external_data(lv_group_t * group, શૂન્ય * data, શૂન્ય (* free_cb)(શૂન્ય * data))
{
    LV_CHECK_ARG(group != ખાલી, પાછા);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

શૂન્ય lv_group_set_user_data(lv_group_t * group, શૂન્ય * user_data)
{
    LV_CHECK_ARG(group != ખાલી, પાછા);
    group->user_data = user_data;
}

શૂન્ય * lv_group_get_user_data(અચલ lv_group_t * group)
{
    LV_CHECK_ARG(group != ખાલી, પાછા ખાલી);
    પાછા group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

સ્થિર શૂન્ય lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != ખાલી);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    જો(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    નહીંતર જો(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

સ્થિર bool focus_next_core(lv_group_t * group, શૂન્ય * (*begin)(અચલ lv_ll_t *),
                            શૂન્ય * (*move)(અચલ lv_ll_t *, અચલ શૂન્ય *))
{
    LV_ASSERT(group != ખાલી);
    LV_ASSERT(begin != ખાલી);
    LV_ASSERT(move != ખાલી);
    જો(group->frozen) પાછા false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = ખાલી;
    bool can_move            = true;
    bool can_begin           = true;

    માટે(;;) {
        જો(obj_next == ખાલી) {
            જો(group->wrap || obj_sentinel == ખાલી) {
                જો(!can_begin) પાછા focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            નહીંતર {
                /*Currently focused object is the last/first in the group, keep it that way*/
                પાછા focus_changed;
            }
        }

        જો(obj_sentinel == ખાલી) {
            obj_sentinel = obj_next;
            જો(obj_sentinel == ખાલી) પાછા focus_changed; /*Group is empty*/
        }

        જો(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            જો(obj_next == obj_sentinel) પાછા focus_changed;
        }

        can_move = true;

        જો(obj_next == ખાલી) ચાલુ;
        જો(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) ચાલુ;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        જ્યારે(parent) {
            જો(lv_obj_is_hidden(parent)) તોડો;
            parent = lv_obj_get_parent(parent);
        }

        જો(parent && lv_obj_is_hidden(parent)) ચાલુ;

        /*If we got her a good candidate is found*/
        તોડો;
    }

    જો(obj_next == group->obj_focus) પાછા focus_changed; /*There's only one visible object and it's already focused*/

    જો(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        જો(res != LV_RESULT_OK) પાછા focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    જો(res != LV_RESULT_OK) પાછા focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    જો(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    પાછા focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
સ્થિર lv_indev_t * get_indev(અચલ lv_group_t * g)
{
    LV_ASSERT(g != ખાલી);
    lv_indev_t * indev_guess = ખાલી;
    lv_indev_t * indev = lv_indev_get_next(ખાલી);

    જ્યારે(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        જો(indev_type == LV_INDEV_TYPE_POINTER) પાછા indev;
        જો(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    પાછા indev_guess;
}
