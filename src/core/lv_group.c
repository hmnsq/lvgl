/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#شامل "lv_group_private.h"
#شامل "../core/lv_obj_private.h"
#شامل "../core/lv_global.h"
#شامل "../lvgl_public.h"

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
ایستا bool focus_next_core(lv_group_t * group, تهی * (*begin)(ثابت lv_ll_t *),
                            تهی * (*move)(ثابت lv_ll_t *, ثابت تهی *));
ایستا تهی lv_group_refocus(lv_group_t * g);
ایستا lv_indev_t * get_indev(ثابت lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

تهی lv_group_init(تهی)
{
    lv_ll_init(group_ll_p, اندازه(lv_group_t));
}

تهی lv_group_deinit(تهی)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(تهی)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    اگر(group == پوچ) بازگشت پوچ;
    lv_ll_init(&group->obj_ll, اندازه(lv_obj_t *));

    group->obj_focus      = پوچ;
    group->frozen         = 0;
    group->focus_cb       = پوچ;
    group->edge_cb        = پوچ;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = پوچ;
#اگر LV_USE_EXT_DATA
    group->ext_data.free_cb = پوچ;
    group->ext_data.data = پوچ;
#endif

    بازگشت group;
}

تهی lv_group_delete(lv_group_t * group)
{
    اگر(group == پوچ) بازگشت;

    /*Defocus the currently focused object*/
    اگر(group->obj_focus != پوچ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        اگر((*obj)->spec_attr)(*obj)->spec_attr->group_p = پوچ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(پوچ);
    وقتی(indev) {
        اگر(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, پوچ);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    اگر(group == lv_group_get_default()) lv_group_set_default(پوچ);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#اگر LV_USE_EXT_DATA
    اگر(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = پوچ;
    }
#endif
    lv_free(group);
}

تهی lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(تهی)
{
    بازگشت default_group;
}

تهی lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);
    LV_CHECK_ARG(obj != پوچ, بازگشت);

    LV_LOG_TRACE("begin");

    اگر(!lv_obj_allocate_spec_attr(obj)) {
        بازگشت;
    }

    LV_ASSERT(obj->spec_attr != پوچ);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    اگر(next == پوچ) بازگشت;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    اگر(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

تهی lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != پوچ, بازگشت);
    LV_CHECK_ARG(obj2 != پوچ, بازگشت);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    اگر(g1 != g2) بازگشت;
    اگر(g1 == پوچ) بازگشت;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        اگر((*obj_i) == obj1)(*obj_i) = obj2;
        وگرنه اگر((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    اگر(focused == obj1) lv_group_focus_obj(obj2);
    وگرنه اگر(focused == obj2) lv_group_focus_obj(obj1);
}

تهی lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);

    lv_group_t * g = lv_obj_get_group(obj);
    اگر(g == پوچ) بازگشت;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    اگر(g->obj_focus && *g->obj_focus == obj) {
        اگر(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        اگر(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    اگر(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = پوچ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        اگر(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            اگر(obj->spec_attr) obj->spec_attr->group_p = پوچ;
            بشکن;
        }
    }
    LV_LOG_TRACE("finished");
}

تهی lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    /*Defocus the currently focused object*/
    اگر(group->obj_focus != پوچ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = پوچ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        اگر((*obj)->spec_attr)(*obj)->spec_attr->group_p = پوچ;
    }

    lv_ll_clear(&(group->obj_ll));
}

تهی lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != پوچ, بازگشت);

    lv_group_t * g = lv_obj_get_group(obj);

    اگر(g == پوچ) بازگشت;
    اگر(g->frozen) بازگشت;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        اگر(*i == obj) {
            اگر(g->obj_focus != پوچ) {
                اگر(*g->obj_focus == obj) بشکن;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                اگر(res != LV_RESULT_OK) بازگشت;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            اگر(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            اگر(res != LV_RESULT_OK) بازگشت;
            lv_obj_invalidate(*g->obj_focus);
            بشکن;
        }
    }
}

تهی lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    اگر(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

تهی lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    اگر(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

تهی lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    اگر(en == false) group->frozen = 0;
    وگرنه group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != پوچ, بازگشت LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    اگر(act == پوچ) بازگشت LV_RESULT_OK;

    اگر(lv_obj_has_state(act, LV_STATE_DISABLED)) بازگشت LV_RESULT_OK;

    بازگشت lv_obj_send_event(act, LV_EVENT_کلید, &c);
}

تهی lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    group->focus_cb = focus_cb;
}

تهی lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    group->edge_cb = edge_cb;
}

تهی lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);
    uint8_t en_val = edit ? 1 : 0;

    اگر(en_val == group->editing) بازگشت; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    اگر(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        اگر(res != LV_RESULT_OK) بازگشت;

        lv_obj_invalidate(focused);
    }
}

تهی lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);
    group->refocus_policy = policy & 0x01;
}

تهی lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت پوچ);
    اگر(group->obj_focus == پوچ) بازگشت پوچ;

    بازگشت *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت پوچ);
    بازگشت group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت پوچ);
    بازگشت group->edge_cb;
}

bool lv_group_get_editing(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت false);
    بازگشت group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت false);
    بازگشت group->wrap;
}

uint32_t lv_group_get_obj_شمار(lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت 0);
    بازگشت lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != پوچ, بازگشت پوچ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        اگر(len == index) {
            بازگشت *obj;
        }
        len++;
    }
    بازگشت پوچ;
}

uint32_t lv_group_get_شمار(تهی)
{
    بازگشت lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        اگر(len == index) {
            بازگشت group;
        }
        len++;
    }

    بازگشت پوچ;
}

#اگر LV_USE_EXT_DATA
تهی lv_group_set_external_data(lv_group_t * group, تهی * data, تهی (* free_cb)(تهی * data))
{
    LV_CHECK_ARG(group != پوچ, بازگشت);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

تهی lv_group_set_user_data(lv_group_t * group, تهی * user_data)
{
    LV_CHECK_ARG(group != پوچ, بازگشت);
    group->user_data = user_data;
}

تهی * lv_group_get_user_data(ثابت lv_group_t * group)
{
    LV_CHECK_ARG(group != پوچ, بازگشت پوچ);
    بازگشت group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ایستا تهی lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != پوچ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    اگر(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    وگرنه اگر(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

ایستا bool focus_next_core(lv_group_t * group, تهی * (*begin)(ثابت lv_ll_t *),
                            تهی * (*move)(ثابت lv_ll_t *, ثابت تهی *))
{
    LV_ASSERT(group != پوچ);
    LV_ASSERT(begin != پوچ);
    LV_ASSERT(move != پوچ);
    اگر(group->frozen) بازگشت false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = پوچ;
    bool can_move            = true;
    bool can_begin           = true;

    برای(;;) {
        اگر(obj_next == پوچ) {
            اگر(group->wrap || obj_sentinel == پوچ) {
                اگر(!can_begin) بازگشت focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            وگرنه {
                /*Currently focused object is the last/first in the group, keep it that way*/
                بازگشت focus_changed;
            }
        }

        اگر(obj_sentinel == پوچ) {
            obj_sentinel = obj_next;
            اگر(obj_sentinel == پوچ) بازگشت focus_changed; /*Group is empty*/
        }

        اگر(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            اگر(obj_next == obj_sentinel) بازگشت focus_changed;
        }

        can_move = true;

        اگر(obj_next == پوچ) ادامه;
        اگر(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) ادامه;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        وقتی(parent) {
            اگر(lv_obj_is_hidden(parent)) بشکن;
            parent = lv_obj_get_parent(parent);
        }

        اگر(parent && lv_obj_is_hidden(parent)) ادامه;

        /*If we got her a good candidate is found*/
        بشکن;
    }

    اگر(obj_next == group->obj_focus) بازگشت focus_changed; /*There's only one visible object and it's already focused*/

    اگر(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        اگر(res != LV_RESULT_OK) بازگشت focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    اگر(res != LV_RESULT_OK) بازگشت focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    اگر(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    بازگشت focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ایستا lv_indev_t * get_indev(ثابت lv_group_t * g)
{
    LV_ASSERT(g != پوچ);
    lv_indev_t * indev_guess = پوچ;
    lv_indev_t * indev = lv_indev_get_next(پوچ);

    وقتی(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        اگر(indev_type == LV_INDEV_TYPE_POINTER) بازگشت indev;
        اگر(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    بازگشت indev_guess;
}
