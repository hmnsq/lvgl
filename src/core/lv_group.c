/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ଅନ୍ତର୍ଭାଵ "lv_group_private.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_obj_private.h"
#ଅନ୍ତର୍ଭାଵ "../core/lv_global.h"
#ଅନ୍ତର୍ଭାଵ "../lvgl_public.h"

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
ସ୍ଥିର bool focus_next_core(lv_group_t * group, ଶୂନ୍ୟ * (*begin)(ଅଚଳ lv_ll_t *),
                            ଶୂନ୍ୟ * (*move)(ଅଚଳ lv_ll_t *, ଅଚଳ ଶୂନ୍ୟ *));
ସ୍ଥିର ଶୂନ୍ୟ lv_group_refocus(lv_group_t * g);
ସ୍ଥିର lv_indev_t * get_indev(ଅଚଳ lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ଶୂନ୍ୟ lv_group_init(ଶୂନ୍ୟ)
{
    lv_ll_init(group_ll_p, ଆକାର(lv_group_t));
}

ଶୂନ୍ୟ lv_group_deinit(ଶୂନ୍ୟ)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(ଶୂନ୍ୟ)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    ଯଦି(group == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;
    lv_ll_init(&group->obj_ll, ଆକାର(lv_obj_t *));

    group->obj_focus      = ଖାଲି;
    group->frozen         = 0;
    group->focus_cb       = ଖାଲି;
    group->edge_cb        = ଖାଲି;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = ଖାଲି;
#ଯଦି LV_USE_EXT_DATA
    group->ext_data.free_cb = ଖାଲି;
    group->ext_data.data = ଖାଲି;
#endif

    ଫେରନ୍ତୁ group;
}

ଶୂନ୍ୟ lv_group_delete(lv_group_t * group)
{
    ଯଦି(group == ଖାଲି) ଫେରନ୍ତୁ;

    /*Defocus the currently focused object*/
    ଯଦି(group->obj_focus != ଖାଲି) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ଯଦି((*obj)->spec_attr)(*obj)->spec_attr->group_p = ଖାଲି;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(ଖାଲି);
    ଯେତେବେଳେ(indev) {
        ଯଦି(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, ଖାଲି);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ଯଦି(group == lv_group_get_default()) lv_group_set_default(ଖାଲି);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#ଯଦି LV_USE_EXT_DATA
    ଯଦି(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = ଖାଲି;
    }
#endif
    lv_free(group);
}

ଶୂନ୍ୟ lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ default_group;
}

ଶୂନ୍ୟ lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    LV_LOG_TRACE("begin");

    ଯଦି(!lv_obj_allocate_spec_attr(obj)) {
        ଫେରନ୍ତୁ;
    }

    LV_ASSERT(obj->spec_attr != ଖାଲି);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    ଯଦି(next == ଖାଲି) ଫେରନ୍ତୁ;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ଯଦି(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

ଶୂନ୍ୟ lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ଖାଲି, ଫେରନ୍ତୁ);
    LV_CHECK_ARG(obj2 != ଖାଲି, ଫେରନ୍ତୁ);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    ଯଦି(g1 != g2) ଫେରନ୍ତୁ;
    ଯଦି(g1 == ଖାଲି) ଫେରନ୍ତୁ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ଯଦି((*obj_i) == obj1)(*obj_i) = obj2;
        ଅନ୍ୟଥା ଯଦି((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    ଯଦି(focused == obj1) lv_group_focus_obj(obj2);
    ଅନ୍ୟଥା ଯଦି(focused == obj2) lv_group_focus_obj(obj1);
}

ଶୂନ୍ୟ lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    lv_group_t * g = lv_obj_get_group(obj);
    ଯଦି(g == ଖାଲି) ଫେରନ୍ତୁ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ଯଦି(g->obj_focus && *g->obj_focus == obj) {
        ଯଦି(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ଯଦି(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ଯଦି(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ଖାଲି;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ଯଦି(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ଯଦି(obj->spec_attr) obj->spec_attr->group_p = ଖାଲି;
            ଭାଙ୍ଗ;
        }
    }
    LV_LOG_TRACE("finished");
}

ଶୂନ୍ୟ lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    /*Defocus the currently focused object*/
    ଯଦି(group->obj_focus != ଖାଲି) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = ଖାଲି;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ଯଦି((*obj)->spec_attr)(*obj)->spec_attr->group_p = ଖାଲି;
    }

    lv_ll_clear(&(group->obj_ll));
}

ଶୂନ୍ୟ lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ଖାଲି, ଫେରନ୍ତୁ);

    lv_group_t * g = lv_obj_get_group(obj);

    ଯଦି(g == ଖାଲି) ଫେରନ୍ତୁ;
    ଯଦି(g->frozen) ଫେରନ୍ତୁ;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ଯଦି(*i == obj) {
            ଯଦି(g->obj_focus != ଖାଲି) {
                ଯଦି(*g->obj_focus == obj) ଭାଙ୍ଗ;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ଯଦି(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ;
            lv_obj_invalidate(*g->obj_focus);
            ଭାଙ୍ଗ;
        }
    }
}

ଶୂନ୍ୟ lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    ଯଦି(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

ଶୂନ୍ୟ lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    ଯଦି(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

ଶୂନ୍ୟ lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    ଯଦି(en == false) group->frozen = 0;
    ଅନ୍ୟଥା group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    ଯଦି(act == ଖାଲି) ଫେରନ୍ତୁ LV_RESULT_OK;

    ଯଦି(lv_obj_has_state(act, LV_STATE_DISABLED)) ଫେରନ୍ତୁ LV_RESULT_OK;

    ଫେରନ୍ତୁ lv_obj_send_event(act, LV_EVENT_ଚାବି, &c);
}

ଶୂନ୍ୟ lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    group->focus_cb = focus_cb;
}

ଶୂନ୍ୟ lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    group->edge_cb = edge_cb;
}

ଶୂନ୍ୟ lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);
    uint8_t en_val = edit ? 1 : 0;

    ଯଦି(en_val == group->editing) ଫେରନ୍ତୁ; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    ଯଦି(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ;

        lv_obj_invalidate(focused);
    }
}

ଶୂନ୍ୟ lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);
    group->refocus_policy = policy & 0x01;
}

ଶୂନ୍ୟ lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ଅଚଳ lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଯଦି(group->obj_focus == ଖାଲି) ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ଅଚଳ lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଫେରନ୍ତୁ group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ଅଚଳ lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଫେରନ୍ତୁ group->edge_cb;
}

bool lv_group_get_editing(ଅଚଳ lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ false);
    ଫେରନ୍ତୁ group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ false);
    ଫେରନ୍ତୁ group->wrap;
}

uint32_t lv_group_get_obj_ଗଣନା(lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ 0);
    ଫେରନ୍ତୁ lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        ଯଦି(len == index) {
            ଫେରନ୍ତୁ *obj;
        }
        len++;
    }
    ଫେରନ୍ତୁ ଖାଲି;
}

uint32_t lv_group_get_ଗଣନା(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        ଯଦି(len == index) {
            ଫେରନ୍ତୁ group;
        }
        len++;
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

#ଯଦି LV_USE_EXT_DATA
ଶୂନ୍ୟ lv_group_set_external_data(lv_group_t * group, ଶୂନ୍ୟ * data, ଶୂନ୍ୟ (* free_cb)(ଶୂନ୍ୟ * data))
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

ଶୂନ୍ୟ lv_group_set_user_data(lv_group_t * group, ଶୂନ୍ୟ * user_data)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ);
    group->user_data = user_data;
}

ଶୂନ୍ୟ * lv_group_get_user_data(ଅଚଳ lv_group_t * group)
{
    LV_CHECK_ARG(group != ଖାଲି, ଫେରନ୍ତୁ ଖାଲି);
    ଫେରନ୍ତୁ group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ସ୍ଥିର ଶୂନ୍ୟ lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != ଖାଲି);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    ଯଦି(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    ଅନ୍ୟଥା ଯଦି(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

ସ୍ଥିର bool focus_next_core(lv_group_t * group, ଶୂନ୍ୟ * (*begin)(ଅଚଳ lv_ll_t *),
                            ଶୂନ୍ୟ * (*move)(ଅଚଳ lv_ll_t *, ଅଚଳ ଶୂନ୍ୟ *))
{
    LV_ASSERT(group != ଖାଲି);
    LV_ASSERT(begin != ଖାଲି);
    LV_ASSERT(move != ଖାଲି);
    ଯଦି(group->frozen) ଫେରନ୍ତୁ false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = ଖାଲି;
    bool can_move            = true;
    bool can_begin           = true;

    ପାଇଁ(;;) {
        ଯଦି(obj_next == ଖାଲି) {
            ଯଦି(group->wrap || obj_sentinel == ଖାଲି) {
                ଯଦି(!can_begin) ଫେରନ୍ତୁ focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ଅନ୍ୟଥା {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ଫେରନ୍ତୁ focus_changed;
            }
        }

        ଯଦି(obj_sentinel == ଖାଲି) {
            obj_sentinel = obj_next;
            ଯଦି(obj_sentinel == ଖାଲି) ଫେରନ୍ତୁ focus_changed; /*Group is empty*/
        }

        ଯଦି(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ଯଦି(obj_next == obj_sentinel) ଫେରନ୍ତୁ focus_changed;
        }

        can_move = true;

        ଯଦି(obj_next == ଖାଲି) ଜାରି;
        ଯଦି(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) ଜାରି;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        ଯେତେବେଳେ(parent) {
            ଯଦି(lv_obj_is_hidden(parent)) ଭାଙ୍ଗ;
            parent = lv_obj_get_parent(parent);
        }

        ଯଦି(parent && lv_obj_is_hidden(parent)) ଜାରି;

        /*If we got her a good candidate is found*/
        ଭାଙ୍ଗ;
    }

    ଯଦି(obj_next == group->obj_focus) ଫେରନ୍ତୁ focus_changed; /*There's only one visible object and it's already focused*/

    ଯଦି(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    ଯଦି(res != LV_RESULT_OK) ଫେରନ୍ତୁ focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    ଯଦି(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    ଫେରନ୍ତୁ focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ସ୍ଥିର lv_indev_t * get_indev(ଅଚଳ lv_group_t * g)
{
    LV_ASSERT(g != ଖାଲି);
    lv_indev_t * indev_guess = ଖାଲି;
    lv_indev_t * indev = lv_indev_get_next(ଖାଲି);

    ଯେତେବେଳେ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ଯଦି(indev_type == LV_INDEV_TYPE_POINTER) ଫେରନ୍ତୁ indev;
        ଯଦି(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    ଫେରନ୍ତୁ indev_guess;
}
