/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাৱ "lv_group_private.h"
#অন্তর্ভাৱ "../core/lv_obj_private.h"
#অন্তর্ভাৱ "../core/lv_global.h"
#অন্তর্ভাৱ "../lvgl_public.h"

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
স্থির bool focus_next_core(lv_group_t * group, শূন্য * (*begin)(ধ্রুৱ lv_ll_t *),
                            শূন্য * (*move)(ধ্রুৱ lv_ll_t *, ধ্রুৱ শূন্য *));
স্থির শূন্য lv_group_refocus(lv_group_t * g);
স্থির lv_indev_t * get_indev(ধ্রুৱ lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

শূন্য lv_group_init(শূন্য)
{
    lv_ll_init(group_ll_p, পরিমাণ(lv_group_t));
}

শূন্য lv_group_deinit(শূন্য)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(শূন্য)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    যদি(group == রিক্ত) ঘূৰাই রিক্ত;
    lv_ll_init(&group->obj_ll, পরিমাণ(lv_obj_t *));

    group->obj_focus      = রিক্ত;
    group->frozen         = 0;
    group->focus_cb       = রিক্ত;
    group->edge_cb        = রিক্ত;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = রিক্ত;
#যদি LV_USE_EXT_DATA
    group->ext_data.free_cb = রিক্ত;
    group->ext_data.data = রিক্ত;
#endif

    ঘূৰাই group;
}

শূন্য lv_group_delete(lv_group_t * group)
{
    যদি(group == রিক্ত) ঘূৰাই;

    /*Defocus the currently focused object*/
    যদি(group->obj_focus != রিক্ত) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        যদি((*obj)->spec_attr)(*obj)->spec_attr->group_p = রিক্ত;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(রিক্ত);
    যেতিয়ালৈ(indev) {
        যদি(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, রিক্ত);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    যদি(group == lv_group_get_default()) lv_group_set_default(রিক্ত);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#যদি LV_USE_EXT_DATA
    যদি(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = রিক্ত;
    }
#endif
    lv_free(group);
}

শূন্য lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(শূন্য)
{
    ঘূৰাই default_group;
}

শূন্য lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই);

    LV_LOG_TRACE("begin");

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ঘূৰাই;
    }

    LV_ASSERT(obj->spec_attr != রিক্ত);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    যদি(next == রিক্ত) ঘূৰাই;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    যদি(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

শূন্য lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != রিক্ত, ঘূৰাই);
    LV_CHECK_ARG(obj2 != রিক্ত, ঘূৰাই);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    যদি(g1 != g2) ঘূৰাই;
    যদি(g1 == রিক্ত) ঘূৰাই;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        যদি((*obj_i) == obj1)(*obj_i) = obj2;
        অন্যথা যদি((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    যদি(focused == obj1) lv_group_focus_obj(obj2);
    অন্যথা যদি(focused == obj2) lv_group_focus_obj(obj1);
}

শূন্য lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই);

    lv_group_t * g = lv_obj_get_group(obj);
    যদি(g == রিক্ত) ঘূৰাই;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    যদি(g->obj_focus && *g->obj_focus == obj) {
        যদি(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        যদি(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    যদি(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = রিক্ত;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        যদি(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            যদি(obj->spec_attr) obj->spec_attr->group_p = রিক্ত;
            ভঙ্গ;
        }
    }
    LV_LOG_TRACE("finished");
}

শূন্য lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    /*Defocus the currently focused object*/
    যদি(group->obj_focus != রিক্ত) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = রিক্ত;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        যদি((*obj)->spec_attr)(*obj)->spec_attr->group_p = রিক্ত;
    }

    lv_ll_clear(&(group->obj_ll));
}

শূন্য lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != রিক্ত, ঘূৰাই);

    lv_group_t * g = lv_obj_get_group(obj);

    যদি(g == রিক্ত) ঘূৰাই;
    যদি(g->frozen) ঘূৰাই;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        যদি(*i == obj) {
            যদি(g->obj_focus != রিক্ত) {
                যদি(*g->obj_focus == obj) ভঙ্গ;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                যদি(res != LV_RESULT_OK) ঘূৰাই;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            যদি(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            যদি(res != LV_RESULT_OK) ঘূৰাই;
            lv_obj_invalidate(*g->obj_focus);
            ভঙ্গ;
        }
    }
}

শূন্য lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    যদি(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

শূন্য lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    যদি(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

শূন্য lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    যদি(en == false) group->frozen = 0;
    অন্যথা group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    যদি(act == রিক্ত) ঘূৰাই LV_RESULT_OK;

    যদি(lv_obj_has_state(act, LV_STATE_DISABLED)) ঘূৰাই LV_RESULT_OK;

    ঘূৰাই lv_obj_send_event(act, LV_EVENT_চাবি, &c);
}

শূন্য lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    group->focus_cb = focus_cb;
}

শূন্য lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    group->edge_cb = edge_cb;
}

শূন্য lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);
    uint8_t en_val = edit ? 1 : 0;

    যদি(en_val == group->editing) ঘূৰাই; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    যদি(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        যদি(res != LV_RESULT_OK) ঘূৰাই;

        lv_obj_invalidate(focused);
    }
}

শূন্য lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);
    group->refocus_policy = policy & 0x01;
}

শূন্য lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ধ্রুৱ lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই রিক্ত);
    যদি(group->obj_focus == রিক্ত) ঘূৰাই রিক্ত;

    ঘূৰাই *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ধ্রুৱ lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই রিক্ত);
    ঘূৰাই group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ধ্রুৱ lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই রিক্ত);
    ঘূৰাই group->edge_cb;
}

bool lv_group_get_editing(ধ্রুৱ lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই false);
    ঘূৰাই group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই false);
    ঘূৰাই group->wrap;
}

uint32_t lv_group_get_obj_গুনতি(lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই 0);
    ঘূৰাই lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই রিক্ত);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        যদি(len == index) {
            ঘূৰাই *obj;
        }
        len++;
    }
    ঘূৰাই রিক্ত;
}

uint32_t lv_group_get_গুনতি(শূন্য)
{
    ঘূৰাই lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        যদি(len == index) {
            ঘূৰাই group;
        }
        len++;
    }

    ঘূৰাই রিক্ত;
}

#যদি LV_USE_EXT_DATA
শূন্য lv_group_set_external_data(lv_group_t * group, শূন্য * data, শূন্য (* free_cb)(শূন্য * data))
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

শূন্য lv_group_set_user_data(lv_group_t * group, শূন্য * user_data)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই);
    group->user_data = user_data;
}

শূন্য * lv_group_get_user_data(ধ্রুৱ lv_group_t * group)
{
    LV_CHECK_ARG(group != রিক্ত, ঘূৰাই রিক্ত);
    ঘূৰাই group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির শূন্য lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != রিক্ত);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    যদি(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    অন্যথা যদি(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

স্থির bool focus_next_core(lv_group_t * group, শূন্য * (*begin)(ধ্রুৱ lv_ll_t *),
                            শূন্য * (*move)(ধ্রুৱ lv_ll_t *, ধ্রুৱ শূন্য *))
{
    LV_ASSERT(group != রিক্ত);
    LV_ASSERT(begin != রিক্ত);
    LV_ASSERT(move != রিক্ত);
    যদি(group->frozen) ঘূৰাই false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = রিক্ত;
    bool can_move            = true;
    bool can_begin           = true;

    বাবে(;;) {
        যদি(obj_next == রিক্ত) {
            যদি(group->wrap || obj_sentinel == রিক্ত) {
                যদি(!can_begin) ঘূৰাই focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            অন্যথা {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ঘূৰাই focus_changed;
            }
        }

        যদি(obj_sentinel == রিক্ত) {
            obj_sentinel = obj_next;
            যদি(obj_sentinel == রিক্ত) ঘূৰাই focus_changed; /*Group is empty*/
        }

        যদি(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            যদি(obj_next == obj_sentinel) ঘূৰাই focus_changed;
        }

        can_move = true;

        যদি(obj_next == রিক্ত) অনুৱর্ত;
        যদি(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) অনুৱর্ত;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        যেতিয়ালৈ(parent) {
            যদি(lv_obj_is_hidden(parent)) ভঙ্গ;
            parent = lv_obj_get_parent(parent);
        }

        যদি(parent && lv_obj_is_hidden(parent)) অনুৱর্ত;

        /*If we got her a good candidate is found*/
        ভঙ্গ;
    }

    যদি(obj_next == group->obj_focus) ঘূৰাই focus_changed; /*There's only one visible object and it's already focused*/

    যদি(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        যদি(res != LV_RESULT_OK) ঘূৰাই focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    যদি(res != LV_RESULT_OK) ঘূৰাই focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    যদি(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    ঘূৰাই focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
স্থির lv_indev_t * get_indev(ধ্রুৱ lv_group_t * g)
{
    LV_ASSERT(g != রিক্ত);
    lv_indev_t * indev_guess = রিক্ত;
    lv_indev_t * indev = lv_indev_get_next(রিক্ত);

    যেতিয়ালৈ(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        যদি(indev_type == LV_INDEV_TYPE_POINTER) ঘূৰাই indev;
        যদি(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    ঘূৰাই indev_guess;
}
