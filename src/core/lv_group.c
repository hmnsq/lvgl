/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#അന്തര്ഭാവ "lv_group_private.h"
#അന്തര്ഭാവ "../core/lv_obj_private.h"
#അന്തര്ഭാവ "../core/lv_global.h"
#അന്തര്ഭാവ "../lvgl_public.h"

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
സ്ഥിതം bool focus_next_core(lv_group_t * group, ശൂന്യം * (*begin)(സ്ഥിരം lv_ll_t *),
                            ശൂന്യം * (*move)(സ്ഥിരം lv_ll_t *, സ്ഥിരം ശൂന്യം *));
സ്ഥിതം ശൂന്യം lv_group_refocus(lv_group_t * g);
സ്ഥിതം lv_indev_t * get_indev(സ്ഥിരം lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ശൂന്യം lv_group_init(ശൂന്യം)
{
    lv_ll_init(group_ll_p, വലുപ്പം(lv_group_t));
}

ശൂന്യം lv_group_deinit(ശൂന്യം)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(ശൂന്യം)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    എങ്കിൽ(group == ശൂന്യ) തിരികെ ശൂന്യ;
    lv_ll_init(&group->obj_ll, വലുപ്പം(lv_obj_t *));

    group->obj_focus      = ശൂന്യ;
    group->frozen         = 0;
    group->focus_cb       = ശൂന്യ;
    group->edge_cb        = ശൂന്യ;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = ശൂന്യ;
#എങ്കിൽ LV_USE_EXT_DATA
    group->ext_data.free_cb = ശൂന്യ;
    group->ext_data.data = ശൂന്യ;
#endif

    തിരികെ group;
}

ശൂന്യം lv_group_delete(lv_group_t * group)
{
    എങ്കിൽ(group == ശൂന്യ) തിരികെ;

    /*Defocus the currently focused object*/
    എങ്കിൽ(group->obj_focus != ശൂന്യ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        എങ്കിൽ((*obj)->spec_attr)(*obj)->spec_attr->group_p = ശൂന്യ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(ശൂന്യ);
    ആകുമ്പോൾ(indev) {
        എങ്കിൽ(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, ശൂന്യ);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    എങ്കിൽ(group == lv_group_get_default()) lv_group_set_default(ശൂന്യ);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#എങ്കിൽ LV_USE_EXT_DATA
    എങ്കിൽ(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = ശൂന്യ;
    }
#endif
    lv_free(group);
}

ശൂന്യം lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(ശൂന്യം)
{
    തിരികെ default_group;
}

ശൂന്യം lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);

    LV_LOG_TRACE("begin");

    എങ്കിൽ(!lv_obj_allocate_spec_attr(obj)) {
        തിരികെ;
    }

    LV_ASSERT(obj->spec_attr != ശൂന്യ);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    എങ്കിൽ(next == ശൂന്യ) തിരികെ;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    എങ്കിൽ(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

ശൂന്യം lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ശൂന്യ, തിരികെ);
    LV_CHECK_ARG(obj2 != ശൂന്യ, തിരികെ);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    എങ്കിൽ(g1 != g2) തിരികെ;
    എങ്കിൽ(g1 == ശൂന്യ) തിരികെ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        എങ്കിൽ((*obj_i) == obj1)(*obj_i) = obj2;
        അല്ലെങ്കിൽ എങ്കിൽ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    എങ്കിൽ(focused == obj1) lv_group_focus_obj(obj2);
    അല്ലെങ്കിൽ എങ്കിൽ(focused == obj2) lv_group_focus_obj(obj1);
}

ശൂന്യം lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);

    lv_group_t * g = lv_obj_get_group(obj);
    എങ്കിൽ(g == ശൂന്യ) തിരികെ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    എങ്കിൽ(g->obj_focus && *g->obj_focus == obj) {
        എങ്കിൽ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        എങ്കിൽ(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    എങ്കിൽ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ശൂന്യ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        എങ്കിൽ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            എങ്കിൽ(obj->spec_attr) obj->spec_attr->group_p = ശൂന്യ;
            നിർത്തുക;
        }
    }
    LV_LOG_TRACE("finished");
}

ശൂന്യം lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    /*Defocus the currently focused object*/
    എങ്കിൽ(group->obj_focus != ശൂന്യ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = ശൂന്യ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        എങ്കിൽ((*obj)->spec_attr)(*obj)->spec_attr->group_p = ശൂന്യ;
    }

    lv_ll_clear(&(group->obj_ll));
}

ശൂന്യം lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ശൂന്യ, തിരികെ);

    lv_group_t * g = lv_obj_get_group(obj);

    എങ്കിൽ(g == ശൂന്യ) തിരികെ;
    എങ്കിൽ(g->frozen) തിരികെ;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        എങ്കിൽ(*i == obj) {
            എങ്കിൽ(g->obj_focus != ശൂന്യ) {
                എങ്കിൽ(*g->obj_focus == obj) നിർത്തുക;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                എങ്കിൽ(res != LV_RESULT_OK) തിരികെ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            എങ്കിൽ(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            എങ്കിൽ(res != LV_RESULT_OK) തിരികെ;
            lv_obj_invalidate(*g->obj_focus);
            നിർത്തുക;
        }
    }
}

ശൂന്യം lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    എങ്കിൽ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

ശൂന്യം lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    എങ്കിൽ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

ശൂന്യം lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    എങ്കിൽ(en == false) group->frozen = 0;
    അല്ലെങ്കിൽ group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    എങ്കിൽ(act == ശൂന്യ) തിരികെ LV_RESULT_OK;

    എങ്കിൽ(lv_obj_has_state(act, LV_STATE_DISABLED)) തിരികെ LV_RESULT_OK;

    തിരികെ lv_obj_send_event(act, LV_EVENT_താക്കോൽ, &c);
}

ശൂന്യം lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    group->focus_cb = focus_cb;
}

ശൂന്യം lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    group->edge_cb = edge_cb;
}

ശൂന്യം lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);
    uint8_t en_val = edit ? 1 : 0;

    എങ്കിൽ(en_val == group->editing) തിരികെ; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    എങ്കിൽ(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        എങ്കിൽ(res != LV_RESULT_OK) തിരികെ;

        lv_obj_invalidate(focused);
    }
}

ശൂന്യം lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);
    group->refocus_policy = policy & 0x01;
}

ശൂന്യം lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(സ്ഥിരം lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ ശൂന്യ);
    എങ്കിൽ(group->obj_focus == ശൂന്യ) തിരികെ ശൂന്യ;

    തിരികെ *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(സ്ഥിരം lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ ശൂന്യ);
    തിരികെ group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(സ്ഥിരം lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ ശൂന്യ);
    തിരികെ group->edge_cb;
}

bool lv_group_get_editing(സ്ഥിരം lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ false);
    തിരികെ group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ false);
    തിരികെ group->wrap;
}

uint32_t lv_group_get_obj_എണ്ണം(lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ 0);
    തിരികെ lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ ശൂന്യ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        എങ്കിൽ(len == index) {
            തിരികെ *obj;
        }
        len++;
    }
    തിരികെ ശൂന്യ;
}

uint32_t lv_group_get_എണ്ണം(ശൂന്യം)
{
    തിരികെ lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        എങ്കിൽ(len == index) {
            തിരികെ group;
        }
        len++;
    }

    തിരികെ ശൂന്യ;
}

#എങ്കിൽ LV_USE_EXT_DATA
ശൂന്യം lv_group_set_external_data(lv_group_t * group, ശൂന്യം * data, ശൂന്യം (* free_cb)(ശൂന്യം * data))
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

ശൂന്യം lv_group_set_user_data(lv_group_t * group, ശൂന്യം * user_data)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ);
    group->user_data = user_data;
}

ശൂന്യം * lv_group_get_user_data(സ്ഥിരം lv_group_t * group)
{
    LV_CHECK_ARG(group != ശൂന്യ, തിരികെ ശൂന്യ);
    തിരികെ group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

സ്ഥിതം ശൂന്യം lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != ശൂന്യ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    എങ്കിൽ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    അല്ലെങ്കിൽ എങ്കിൽ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

സ്ഥിതം bool focus_next_core(lv_group_t * group, ശൂന്യം * (*begin)(സ്ഥിരം lv_ll_t *),
                            ശൂന്യം * (*move)(സ്ഥിരം lv_ll_t *, സ്ഥിരം ശൂന്യം *))
{
    LV_ASSERT(group != ശൂന്യ);
    LV_ASSERT(begin != ശൂന്യ);
    LV_ASSERT(move != ശൂന്യ);
    എങ്കിൽ(group->frozen) തിരികെ false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = ശൂന്യ;
    bool can_move            = true;
    bool can_begin           = true;

    വേണ്ടി(;;) {
        എങ്കിൽ(obj_next == ശൂന്യ) {
            എങ്കിൽ(group->wrap || obj_sentinel == ശൂന്യ) {
                എങ്കിൽ(!can_begin) തിരികെ focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            അല്ലെങ്കിൽ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                തിരികെ focus_changed;
            }
        }

        എങ്കിൽ(obj_sentinel == ശൂന്യ) {
            obj_sentinel = obj_next;
            എങ്കിൽ(obj_sentinel == ശൂന്യ) തിരികെ focus_changed; /*Group is empty*/
        }

        എങ്കിൽ(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            എങ്കിൽ(obj_next == obj_sentinel) തിരികെ focus_changed;
        }

        can_move = true;

        എങ്കിൽ(obj_next == ശൂന്യ) തുടരുക;
        എങ്കിൽ(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) തുടരുക;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        ആകുമ്പോൾ(parent) {
            എങ്കിൽ(lv_obj_is_hidden(parent)) നിർത്തുക;
            parent = lv_obj_get_parent(parent);
        }

        എങ്കിൽ(parent && lv_obj_is_hidden(parent)) തുടരുക;

        /*If we got her a good candidate is found*/
        നിർത്തുക;
    }

    എങ്കിൽ(obj_next == group->obj_focus) തിരികെ focus_changed; /*There's only one visible object and it's already focused*/

    എങ്കിൽ(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        എങ്കിൽ(res != LV_RESULT_OK) തിരികെ focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    എങ്കിൽ(res != LV_RESULT_OK) തിരികെ focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    എങ്കിൽ(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    തിരികെ focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
സ്ഥിതം lv_indev_t * get_indev(സ്ഥിരം lv_group_t * g)
{
    LV_ASSERT(g != ശൂന്യ);
    lv_indev_t * indev_guess = ശൂന്യ;
    lv_indev_t * indev = lv_indev_get_next(ശൂന്യ);

    ആകുമ്പോൾ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        എങ്കിൽ(indev_type == LV_INDEV_TYPE_POINTER) തിരികെ indev;
        എങ്കിൽ(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    തിരികെ indev_guess;
}
