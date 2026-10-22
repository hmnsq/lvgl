/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ضمن "lv_group_private.h"
#ضمن "../core/lv_obj_private.h"
#ضمن "../core/lv_global.h"
#ضمن "../lvgl_public.h"

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
ساكن bool focus_next_core(lv_group_t * group, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *));
ساكن فراغ lv_group_refocus(lv_group_t * g);
ساكن lv_indev_t * get_indev(ثابت lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

فراغ lv_group_init(فراغ)
{
    lv_ll_init(group_ll_p, حجم(lv_group_t));
}

فراغ lv_group_deinit(فراغ)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(فراغ)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    𐤀𐤌(group == عدم) أرجع عدم;
    lv_ll_init(&group->obj_ll, حجم(lv_obj_t *));

    group->obj_focus      = عدم;
    group->frozen         = 0;
    group->focus_cb       = عدم;
    group->edge_cb        = عدم;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = عدم;
#𐤀𐤌 LV_USE_EXT_DATA
    group->ext_data.free_cb = عدم;
    group->ext_data.data = عدم;
#endif

    أرجع group;
}

فراغ lv_group_delete(lv_group_t * group)
{
    𐤀𐤌(group == عدم) أرجع;

    /*Defocus the currently focused object*/
    𐤀𐤌(group->obj_focus != عدم) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        𐤀𐤌((*obj)->spec_attr)(*obj)->spec_attr->group_p = عدم;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(عدم);
    طالما(indev) {
        𐤀𐤌(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, عدم);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    𐤀𐤌(group == lv_group_get_default()) lv_group_set_default(عدم);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#𐤀𐤌 LV_USE_EXT_DATA
    𐤀𐤌(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = عدم;
    }
#endif
    lv_free(group);
}

فراغ lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(فراغ)
{
    أرجع default_group;
}

فراغ lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != عدم, أرجع);
    LV_CHECK_ARG(obj != عدم, أرجع);

    LV_LOG_TRACE("begin");

    𐤀𐤌(!lv_obj_allocate_spec_attr(obj)) {
        أرجع;
    }

    LV_ASSERT(obj->spec_attr != عدم);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    𐤀𐤌(next == عدم) أرجع;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    𐤀𐤌(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

فراغ lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != عدم, أرجع);
    LV_CHECK_ARG(obj2 != عدم, أرجع);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    𐤀𐤌(g1 != g2) أرجع;
    𐤀𐤌(g1 == عدم) أرجع;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        𐤀𐤌((*obj_i) == obj1)(*obj_i) = obj2;
        وإلا 𐤀𐤌((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    𐤀𐤌(focused == obj1) lv_group_focus_obj(obj2);
    وإلا 𐤀𐤌(focused == obj2) lv_group_focus_obj(obj1);
}

فراغ lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_group_t * g = lv_obj_get_group(obj);
    𐤀𐤌(g == عدم) أرجع;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    𐤀𐤌(g->obj_focus && *g->obj_focus == obj) {
        𐤀𐤌(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        𐤀𐤌(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    𐤀𐤌(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = عدم;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        𐤀𐤌(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            𐤀𐤌(obj->spec_attr) obj->spec_attr->group_p = عدم;
            اكسر;
        }
    }
    LV_LOG_TRACE("finished");
}

فراغ lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    /*Defocus the currently focused object*/
    𐤀𐤌(group->obj_focus != عدم) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = عدم;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        𐤀𐤌((*obj)->spec_attr)(*obj)->spec_attr->group_p = عدم;
    }

    lv_ll_clear(&(group->obj_ll));
}

فراغ lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != عدم, أرجع);

    lv_group_t * g = lv_obj_get_group(obj);

    𐤀𐤌(g == عدم) أرجع;
    𐤀𐤌(g->frozen) أرجع;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        𐤀𐤌(*i == obj) {
            𐤀𐤌(g->obj_focus != عدم) {
                𐤀𐤌(*g->obj_focus == obj) اكسر;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                𐤀𐤌(res != LV_RESULT_OK) أرجع;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            𐤀𐤌(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            𐤀𐤌(res != LV_RESULT_OK) أرجع;
            lv_obj_invalidate(*g->obj_focus);
            اكسر;
        }
    }
}

فراغ lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    𐤀𐤌(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

فراغ lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    𐤀𐤌(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

فراغ lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    𐤀𐤌(en == false) group->frozen = 0;
    وإلا group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != عدم, أرجع LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    𐤀𐤌(act == عدم) أرجع LV_RESULT_OK;

    𐤀𐤌(lv_obj_has_state(act, LV_STATE_DISABLED)) أرجع LV_RESULT_OK;

    أرجع lv_obj_send_event(act, LV_EVENT_KEY, &c);
}

فراغ lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    group->focus_cb = focus_cb;
}

فراغ lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != عدم, أرجع);

    group->edge_cb = edge_cb;
}

فراغ lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != عدم, أرجع);
    uint8_t en_val = edit ? 1 : 0;

    𐤀𐤌(en_val == group->editing) أرجع; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    𐤀𐤌(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        𐤀𐤌(res != LV_RESULT_OK) أرجع;

        lv_obj_invalidate(focused);
    }
}

فراغ lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != عدم, أرجع);
    group->refocus_policy = policy & 0x01;
}

فراغ lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != عدم, أرجع);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع عدم);
    𐤀𐤌(group->obj_focus == عدم) أرجع عدم;

    أرجع *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع عدم);
    أرجع group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع عدم);
    أرجع group->edge_cb;
}

bool lv_group_get_editing(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع false);
    أرجع group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع false);
    أرجع group->wrap;
}

uint32_t lv_group_get_obj_count(lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع 0);
    أرجع lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != عدم, أرجع عدم);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        𐤀𐤌(len == index) {
            أرجع *obj;
        }
        len++;
    }
    أرجع عدم;
}

uint32_t lv_group_get_count(فراغ)
{
    أرجع lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        𐤀𐤌(len == index) {
            أرجع group;
        }
        len++;
    }

    أرجع عدم;
}

#𐤀𐤌 LV_USE_EXT_DATA
فراغ lv_group_set_external_data(lv_group_t * group, فراغ * data, فراغ (* free_cb)(فراغ * data))
{
    LV_CHECK_ARG(group != عدم, أرجع);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

فراغ lv_group_set_user_data(lv_group_t * group, فراغ * user_data)
{
    LV_CHECK_ARG(group != عدم, أرجع);
    group->user_data = user_data;
}

فراغ * lv_group_get_user_data(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != عدم, أرجع عدم);
    أرجع group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ساكن فراغ lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != عدم);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    𐤀𐤌(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    وإلا 𐤀𐤌(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

ساكن bool focus_next_core(lv_group_t * group, فراغ * (*begin)(ثابت lv_ll_t *),
                            فراغ * (*move)(ثابت lv_ll_t *, ثابت فراغ *))
{
    LV_ASSERT(group != عدم);
    LV_ASSERT(begin != عدم);
    LV_ASSERT(move != عدم);
    𐤀𐤌(group->frozen) أرجع false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = عدم;
    bool can_move            = true;
    bool can_begin           = true;

    لكل(;;) {
        𐤀𐤌(obj_next == عدم) {
            𐤀𐤌(group->wrap || obj_sentinel == عدم) {
                𐤀𐤌(!can_begin) أرجع focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وإلا {
                /*Currently focused object is the last/first in the group, keep it that way*/
                أرجع focus_changed;
            }
        }

        𐤀𐤌(obj_sentinel == عدم) {
            obj_sentinel = obj_next;
            𐤀𐤌(obj_sentinel == عدم) أرجع focus_changed; /*Group is empty*/
        }

        𐤀𐤌(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            𐤀𐤌(obj_next == obj_sentinel) أرجع focus_changed;
        }

        can_move = true;

        𐤀𐤌(obj_next == عدم) استمر;
        𐤀𐤌(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) استمر;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        طالما(parent) {
            𐤀𐤌(lv_obj_is_hidden(parent)) اكسر;
            parent = lv_obj_get_parent(parent);
        }

        𐤀𐤌(parent && lv_obj_is_hidden(parent)) استمر;

        /*If we got her a good candidate is found*/
        اكسر;
    }

    𐤀𐤌(obj_next == group->obj_focus) أرجع focus_changed; /*There's only one visible object and it's already focused*/

    𐤀𐤌(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        𐤀𐤌(res != LV_RESULT_OK) أرجع focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    𐤀𐤌(res != LV_RESULT_OK) أرجع focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    𐤀𐤌(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    أرجع focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ساكن lv_indev_t * get_indev(ثابت lv_group_t * g)
{
    LV_ASSERT(g != عدم);
    lv_indev_t * indev_guess = عدم;
    lv_indev_t * indev = lv_indev_get_next(عدم);

    طالما(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        𐤀𐤌(indev_type == LV_INDEV_TYPE_POINTER) أرجع indev;
        𐤀𐤌(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    أرجع indev_guess;
}
