/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#அந்தர்பாவ "lv_group_private.h"
#அந்தர்பாவ "../core/lv_obj_private.h"
#அந்தர்பாவ "../core/lv_global.h"
#அந்தர்பாவ "../lvgl_public.h"

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
நிலையான bool focus_next_core(lv_group_t * group, வெறுமை * (*begin)(மாறிலி lv_ll_t *),
                            வெறுமை * (*move)(மாறிலி lv_ll_t *, மாறிலி வெறுமை *));
நிலையான வெறுமை lv_group_refocus(lv_group_t * g);
நிலையான lv_indev_t * get_indev(மாறிலி lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

வெறுமை lv_group_init(வெறுமை)
{
    lv_ll_init(group_ll_p, அளவு(lv_group_t));
}

வெறுமை lv_group_deinit(வெறுமை)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(வெறுமை)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    எனில்(group == வெற்று) திரும்பு வெற்று;
    lv_ll_init(&group->obj_ll, அளவு(lv_obj_t *));

    group->obj_focus      = வெற்று;
    group->frozen         = 0;
    group->focus_cb       = வெற்று;
    group->edge_cb        = வெற்று;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = வெற்று;
#எனில் LV_USE_EXT_DATA
    group->ext_data.free_cb = வெற்று;
    group->ext_data.data = வெற்று;
#endif

    திரும்பு group;
}

வெறுமை lv_group_delete(lv_group_t * group)
{
    எனில்(group == வெற்று) திரும்பு;

    /*Defocus the currently focused object*/
    எனில்(group->obj_focus != வெற்று) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        எனில்((*obj)->spec_attr)(*obj)->spec_attr->group_p = வெற்று;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(வெற்று);
    போது(indev) {
        எனில்(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, வெற்று);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    எனில்(group == lv_group_get_default()) lv_group_set_default(வெற்று);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#எனில் LV_USE_EXT_DATA
    எனில்(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = வெற்று;
    }
#endif
    lv_free(group);
}

வெறுமை lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(வெறுமை)
{
    திரும்பு default_group;
}

வெறுமை lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);

    LV_LOG_TRACE("begin");

    எனில்(!lv_obj_allocate_spec_attr(obj)) {
        திரும்பு;
    }

    LV_ASSERT(obj->spec_attr != வெற்று);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    எனில்(next == வெற்று) திரும்பு;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    எனில்(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

வெறுமை lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != வெற்று, திரும்பு);
    LV_CHECK_ARG(obj2 != வெற்று, திரும்பு);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    எனில்(g1 != g2) திரும்பு;
    எனில்(g1 == வெற்று) திரும்பு;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        எனில்((*obj_i) == obj1)(*obj_i) = obj2;
        இல்லையெனில் எனில்((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    எனில்(focused == obj1) lv_group_focus_obj(obj2);
    இல்லையெனில் எனில்(focused == obj2) lv_group_focus_obj(obj1);
}

வெறுமை lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);

    lv_group_t * g = lv_obj_get_group(obj);
    எனில்(g == வெற்று) திரும்பு;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    எனில்(g->obj_focus && *g->obj_focus == obj) {
        எனில்(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        எனில்(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    எனில்(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = வெற்று;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        எனில்(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            எனில்(obj->spec_attr) obj->spec_attr->group_p = வெற்று;
            நிறுத்து;
        }
    }
    LV_LOG_TRACE("finished");
}

வெறுமை lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    /*Defocus the currently focused object*/
    எனில்(group->obj_focus != வெற்று) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = வெற்று;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        எனில்((*obj)->spec_attr)(*obj)->spec_attr->group_p = வெற்று;
    }

    lv_ll_clear(&(group->obj_ll));
}

வெறுமை lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != வெற்று, திரும்பு);

    lv_group_t * g = lv_obj_get_group(obj);

    எனில்(g == வெற்று) திரும்பு;
    எனில்(g->frozen) திரும்பு;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        எனில்(*i == obj) {
            எனில்(g->obj_focus != வெற்று) {
                எனில்(*g->obj_focus == obj) நிறுத்து;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                எனில்(res != LV_RESULT_OK) திரும்பு;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            எனில்(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            எனில்(res != LV_RESULT_OK) திரும்பு;
            lv_obj_invalidate(*g->obj_focus);
            நிறுத்து;
        }
    }
}

வெறுமை lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    எனில்(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

வெறுமை lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    எனில்(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

வெறுமை lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    எனில்(en == false) group->frozen = 0;
    இல்லையெனில் group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    எனில்(act == வெற்று) திரும்பு LV_RESULT_OK;

    எனில்(lv_obj_has_state(act, LV_STATE_DISABLED)) திரும்பு LV_RESULT_OK;

    திரும்பு lv_obj_send_event(act, LV_EVENT_சாவி, &c);
}

வெறுமை lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    group->focus_cb = focus_cb;
}

வெறுமை lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    group->edge_cb = edge_cb;
}

வெறுமை lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);
    uint8_t en_val = edit ? 1 : 0;

    எனில்(en_val == group->editing) திரும்பு; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    எனில்(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        எனில்(res != LV_RESULT_OK) திரும்பு;

        lv_obj_invalidate(focused);
    }
}

வெறுமை lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);
    group->refocus_policy = policy & 0x01;
}

வெறுமை lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(மாறிலி lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு வெற்று);
    எனில்(group->obj_focus == வெற்று) திரும்பு வெற்று;

    திரும்பு *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(மாறிலி lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு வெற்று);
    திரும்பு group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(மாறிலி lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு வெற்று);
    திரும்பு group->edge_cb;
}

bool lv_group_get_editing(மாறிலி lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு false);
    திரும்பு group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு false);
    திரும்பு group->wrap;
}

uint32_t lv_group_get_obj_மொத்தம்(lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு 0);
    திரும்பு lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு வெற்று);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        எனில்(len == index) {
            திரும்பு *obj;
        }
        len++;
    }
    திரும்பு வெற்று;
}

uint32_t lv_group_get_மொத்தம்(வெறுமை)
{
    திரும்பு lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        எனில்(len == index) {
            திரும்பு group;
        }
        len++;
    }

    திரும்பு வெற்று;
}

#எனில் LV_USE_EXT_DATA
வெறுமை lv_group_set_external_data(lv_group_t * group, வெறுமை * data, வெறுமை (* free_cb)(வெறுமை * data))
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

வெறுமை lv_group_set_user_data(lv_group_t * group, வெறுமை * user_data)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு);
    group->user_data = user_data;
}

வெறுமை * lv_group_get_user_data(மாறிலி lv_group_t * group)
{
    LV_CHECK_ARG(group != வெற்று, திரும்பு வெற்று);
    திரும்பு group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

நிலையான வெறுமை lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != வெற்று);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    எனில்(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    இல்லையெனில் எனில்(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

நிலையான bool focus_next_core(lv_group_t * group, வெறுமை * (*begin)(மாறிலி lv_ll_t *),
                            வெறுமை * (*move)(மாறிலி lv_ll_t *, மாறிலி வெறுமை *))
{
    LV_ASSERT(group != வெற்று);
    LV_ASSERT(begin != வெற்று);
    LV_ASSERT(move != வெற்று);
    எனில்(group->frozen) திரும்பு false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = வெற்று;
    bool can_move            = true;
    bool can_begin           = true;

    க்கு(;;) {
        எனில்(obj_next == வெற்று) {
            எனில்(group->wrap || obj_sentinel == வெற்று) {
                எனில்(!can_begin) திரும்பு focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            இல்லையெனில் {
                /*Currently focused object is the last/first in the group, keep it that way*/
                திரும்பு focus_changed;
            }
        }

        எனில்(obj_sentinel == வெற்று) {
            obj_sentinel = obj_next;
            எனில்(obj_sentinel == வெற்று) திரும்பு focus_changed; /*Group is empty*/
        }

        எனில்(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            எனில்(obj_next == obj_sentinel) திரும்பு focus_changed;
        }

        can_move = true;

        எனில்(obj_next == வெற்று) தொடர்;
        எனில்(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) தொடர்;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        போது(parent) {
            எனில்(lv_obj_is_hidden(parent)) நிறுத்து;
            parent = lv_obj_get_parent(parent);
        }

        எனில்(parent && lv_obj_is_hidden(parent)) தொடர்;

        /*If we got her a good candidate is found*/
        நிறுத்து;
    }

    எனில்(obj_next == group->obj_focus) திரும்பு focus_changed; /*There's only one visible object and it's already focused*/

    எனில்(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        எனில்(res != LV_RESULT_OK) திரும்பு focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    எனில்(res != LV_RESULT_OK) திரும்பு focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    எனில்(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    திரும்பு focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
நிலையான lv_indev_t * get_indev(மாறிலி lv_group_t * g)
{
    LV_ASSERT(g != வெற்று);
    lv_indev_t * indev_guess = வெற்று;
    lv_indev_t * indev = lv_indev_get_next(வெற்று);

    போது(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        எனில்(indev_type == LV_INDEV_TYPE_POINTER) திரும்பு indev;
        எனில்(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    திரும்பு indev_guess;
}
