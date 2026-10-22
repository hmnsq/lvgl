/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ಅನ್ತರ್ಭಾವ "lv_group_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_obj_private.h"
#ಅನ್ತರ್ಭಾವ "../core/lv_global.h"
#ಅನ್ತರ್ಭಾವ "../lvgl_public.h"

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
ಸ್ಥಿರ bool focus_next_core(lv_group_t * group, ಶೂನ್ಯ * (*begin)(ಧ್ರುವ lv_ll_t *),
                            ಶೂನ್ಯ * (*move)(ಧ್ರುವ lv_ll_t *, ಧ್ರುವ ಶೂನ್ಯ *));
ಸ್ಥಿರ ಶೂನ್ಯ lv_group_refocus(lv_group_t * g);
ಸ್ಥಿರ lv_indev_t * get_indev(ಧ್ರುವ lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

ಶೂನ್ಯ lv_group_init(ಶೂನ್ಯ)
{
    lv_ll_init(group_ll_p, ಪರಿಮಾಣ(lv_group_t));
}

ಶೂನ್ಯ lv_group_deinit(ಶೂನ್ಯ)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(ಶೂನ್ಯ)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    ಯದಿ(group == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    lv_ll_init(&group->obj_ll, ಪರಿಮಾಣ(lv_obj_t *));

    group->obj_focus      = ರಿಕ್ತ;
    group->frozen         = 0;
    group->focus_cb       = ರಿಕ್ತ;
    group->edge_cb        = ರಿಕ್ತ;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = ರಿಕ್ತ;
#ಯದಿ LV_USE_EXT_DATA
    group->ext_data.free_cb = ರಿಕ್ತ;
    group->ext_data.data = ರಿಕ್ತ;
#endif

    ಪ್ರತ್ಯಾಗಮ group;
}

ಶೂನ್ಯ lv_group_delete(lv_group_t * group)
{
    ಯದಿ(group == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    /*Defocus the currently focused object*/
    ಯದಿ(group->obj_focus != ರಿಕ್ತ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ಯದಿ((*obj)->spec_attr)(*obj)->spec_attr->group_p = ರಿಕ್ತ;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(ರಿಕ್ತ);
    ಯಾವತ್(indev) {
        ಯದಿ(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, ರಿಕ್ತ);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    ಯದಿ(group == lv_group_get_default()) lv_group_set_default(ರಿಕ್ತ);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#ಯದಿ LV_USE_EXT_DATA
    ಯದಿ(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = ರಿಕ್ತ;
    }
#endif
    lv_free(group);
}

ಶೂನ್ಯ lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ default_group;
}

ಶೂನ್ಯ lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    LV_LOG_TRACE("begin");

    ಯದಿ(!lv_obj_allocate_spec_attr(obj)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    LV_ASSERT(obj->spec_attr != ರಿಕ್ತ);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    ಯದಿ(next == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    ಯದಿ(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

ಶೂನ್ಯ lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    LV_CHECK_ARG(obj2 != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    ಯದಿ(g1 != g2) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ(g1 == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        ಯದಿ((*obj_i) == obj1)(*obj_i) = obj2;
        ಅನ್ಯಥಾ ಯದಿ((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    ಯದಿ(focused == obj1) lv_group_focus_obj(obj2);
    ಅನ್ಯಥಾ ಯದಿ(focused == obj2) lv_group_focus_obj(obj1);
}

ಶೂನ್ಯ lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_group_t * g = lv_obj_get_group(obj);
    ಯದಿ(g == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    ಯದಿ(g->obj_focus && *g->obj_focus == obj) {
        ಯದಿ(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        ಯದಿ(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    ಯದಿ(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ರಿಕ್ತ;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ಯದಿ(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            ಯದಿ(obj->spec_attr) obj->spec_attr->group_p = ರಿಕ್ತ;
            ಭಙ್ಗ;
        }
    }
    LV_LOG_TRACE("finished");
}

ಶೂನ್ಯ lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    /*Defocus the currently focused object*/
    ಯದಿ(group->obj_focus != ರಿಕ್ತ) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = ರಿಕ್ತ;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        ಯದಿ((*obj)->spec_attr)(*obj)->spec_attr->group_p = ರಿಕ್ತ;
    }

    lv_ll_clear(&(group->obj_ll));
}

ಶೂನ್ಯ lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    lv_group_t * g = lv_obj_get_group(obj);

    ಯದಿ(g == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ(g->frozen) ಪ್ರತ್ಯಾಗಮ;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        ಯದಿ(*i == obj) {
            ಯದಿ(g->obj_focus != ರಿಕ್ತ) {
                ಯದಿ(*g->obj_focus == obj) ಭಙ್ಗ;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            ಯದಿ(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ;
            lv_obj_invalidate(*g->obj_focus);
            ಭಙ್ಗ;
        }
    }
}

ಶೂನ್ಯ lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    ಯದಿ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

ಶೂನ್ಯ lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    ಯದಿ(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

ಶೂನ್ಯ lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    ಯದಿ(en == false) group->frozen = 0;
    ಅನ್ಯಥಾ group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    ಯದಿ(act == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;

    ಯದಿ(lv_obj_has_state(act, LV_STATE_DISABLED)) ಪ್ರತ್ಯಾಗಮ LV_RESULT_OK;

    ಪ್ರತ್ಯಾಗಮ lv_obj_send_event(act, LV_EVENT_ಕೀಲಿ, &c);
}

ಶೂನ್ಯ lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    group->focus_cb = focus_cb;
}

ಶೂನ್ಯ lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    group->edge_cb = edge_cb;
}

ಶೂನ್ಯ lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    uint8_t en_val = edit ? 1 : 0;

    ಯದಿ(en_val == group->editing) ಪ್ರತ್ಯಾಗಮ; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    ಯದಿ(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ;

        lv_obj_invalidate(focused);
    }
}

ಶೂನ್ಯ lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    group->refocus_policy = policy & 0x01;
}

ಶೂನ್ಯ lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ಧ್ರುವ lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಯದಿ(group->obj_focus == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ಧ್ರುವ lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ಧ್ರುವ lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ group->edge_cb;
}

bool lv_group_get_editing(ಧ್ರುವ lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    ಪ್ರತ್ಯಾಗಮ group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ false);
    ಪ್ರತ್ಯಾಗಮ group->wrap;
}

uint32_t lv_group_get_obj_ಎಣಿಕೆ(lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ 0);
    ಪ್ರತ್ಯಾಗಮ lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        ಯದಿ(len == index) {
            ಪ್ರತ್ಯಾಗಮ *obj;
        }
        len++;
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

uint32_t lv_group_get_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        ಯದಿ(len == index) {
            ಪ್ರತ್ಯಾಗಮ group;
        }
        len++;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#ಯದಿ LV_USE_EXT_DATA
ಶೂನ್ಯ lv_group_set_external_data(lv_group_t * group, ಶೂನ್ಯ * data, ಶೂನ್ಯ (* free_cb)(ಶೂನ್ಯ * data))
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

ಶೂನ್ಯ lv_group_set_user_data(lv_group_t * group, ಶೂನ್ಯ * user_data)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ);
    group->user_data = user_data;
}

ಶೂನ್ಯ * lv_group_get_user_data(ಧ್ರುವ lv_group_t * group)
{
    LV_CHECK_ARG(group != ರಿಕ್ತ, ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

ಸ್ಥಿರ ಶೂನ್ಯ lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != ರಿಕ್ತ);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    ಯದಿ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    ಅನ್ಯಥಾ ಯದಿ(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

ಸ್ಥಿರ bool focus_next_core(lv_group_t * group, ಶೂನ್ಯ * (*begin)(ಧ್ರುವ lv_ll_t *),
                            ಶೂನ್ಯ * (*move)(ಧ್ರುವ lv_ll_t *, ಧ್ರುವ ಶೂನ್ಯ *))
{
    LV_ASSERT(group != ರಿಕ್ತ);
    LV_ASSERT(begin != ರಿಕ್ತ);
    LV_ASSERT(move != ರಿಕ್ತ);
    ಯದಿ(group->frozen) ಪ್ರತ್ಯಾಗಮ false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = ರಿಕ್ತ;
    bool can_move            = true;
    bool can_begin           = true;

    ಪ್ರತಿ(;;) {
        ಯದಿ(obj_next == ರಿಕ್ತ) {
            ಯದಿ(group->wrap || obj_sentinel == ರಿಕ್ತ) {
                ಯದಿ(!can_begin) ಪ್ರತ್ಯಾಗಮ focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            ಅನ್ಯಥಾ {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ಪ್ರತ್ಯಾಗಮ focus_changed;
            }
        }

        ಯದಿ(obj_sentinel == ರಿಕ್ತ) {
            obj_sentinel = obj_next;
            ಯದಿ(obj_sentinel == ರಿಕ್ತ) ಪ್ರತ್ಯಾಗಮ focus_changed; /*Group is empty*/
        }

        ಯದಿ(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            ಯದಿ(obj_next == obj_sentinel) ಪ್ರತ್ಯಾಗಮ focus_changed;
        }

        can_move = true;

        ಯದಿ(obj_next == ರಿಕ್ತ) ಅನುವರ್ತ;
        ಯದಿ(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) ಅನುವರ್ತ;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        ಯಾವತ್(parent) {
            ಯದಿ(lv_obj_is_hidden(parent)) ಭಙ್ಗ;
            parent = lv_obj_get_parent(parent);
        }

        ಯದಿ(parent && lv_obj_is_hidden(parent)) ಅನುವರ್ತ;

        /*If we got her a good candidate is found*/
        ಭಙ್ಗ;
    }

    ಯದಿ(obj_next == group->obj_focus) ಪ್ರತ್ಯಾಗಮ focus_changed; /*There's only one visible object and it's already focused*/

    ಯದಿ(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    ಯದಿ(res != LV_RESULT_OK) ಪ್ರತ್ಯಾಗಮ focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    ಯದಿ(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    ಪ್ರತ್ಯಾಗಮ focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
ಸ್ಥಿರ lv_indev_t * get_indev(ಧ್ರುವ lv_group_t * g)
{
    LV_ASSERT(g != ರಿಕ್ತ);
    lv_indev_t * indev_guess = ರಿಕ್ತ;
    lv_indev_t * indev = lv_indev_get_next(ರಿಕ್ತ);

    ಯಾವತ್(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        ಯದಿ(indev_type == LV_INDEV_TYPE_POINTER) ಪ್ರತ್ಯಾಗಮ indev;
        ಯದಿ(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    ಪ್ರತ್ಯಾಗಮ indev_guess;
}
