/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#অন্তর্ভাব "lv_group_private.h"
#অন্তর্ভাব "../core/lv_obj_private.h"
#অন্তর্ভাব "../core/lv_global.h"
#অন্তর্ভাব "../lvgl_public.h"

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
স্থির bool focus_পরবর্তী_core(lv_group_t * group, শূন্য * (*begin)(ধ্রুবক lv_ll_t *),
                            শূন্য * (*move)(ধ্রুবক lv_ll_t *, ধ্রুবক শূন্য *));
স্থির শূন্য lv_group_refocus(lv_group_t * g);
স্থির lv_indev_t * get_indev(ধ্রুবক lv_group_t * g);

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
    lv_ll_init(group_ll_p, আকার(lv_group_t));
}

শূন্য lv_group_deinit(শূন্য)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(শূন্য)
{
    lv_group_t * group = lv_ll_ins_শীর্ষ(group_ll_p);
    LV_ASSERT_MALLOC(group);
    যদি(group == ফাঁকা) ফেরত ফাঁকা;
    lv_ll_init(&group->obj_ll, আকার(lv_obj_t *));

    group->obj_focus      = ফাঁকা;
    group->frozen         = 0;
    group->focus_cb       = ফাঁকা;
    group->edge_cb        = ফাঁকা;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_পূর্ব;
    group->wrap           = 1;
    group->user_তথ্য      = ফাঁকা;
#যদি LV_USE_EXT_তথ্য
    group->ext_তথ্য.free_cb = ফাঁকা;
    group->ext_তথ্য.তথ্য = ফাঁকা;
#endif

    ফেরত group;
}

শূন্য lv_group_delete(lv_group_t * group)
{
    যদি(group == ফাঁকা) ফেরত;

    /*Defocus the currently focused object*/
    যদি(group->obj_focus != ফাঁকা) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        যদি((*obj)->spec_attr)(*obj)->spec_attr->group_p = ফাঁকা;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_পরবর্তী(ফাঁকা);
    যখন(indev) {
        যদি(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, ফাঁকা);
        }
        indev = lv_indev_get_পরবর্তী(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    যদি(group == lv_group_get_default()) lv_group_set_default(ফাঁকা);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#যদি LV_USE_EXT_তথ্য
    যদি(group->ext_তথ্য.free_cb) {
        group->ext_তথ্য.free_cb(group->ext_তথ্য.তথ্য);
        group->ext_তথ্য.তথ্য = ফাঁকা;
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
    ফেরত default_group;
}

শূন্য lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);

    LV_LOG_TRACE("begin");

    যদি(!lv_obj_allocate_spec_attr(obj)) {
        ফেরত;
    }

    LV_ASSERT(obj->spec_attr != ফাঁকা);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** পরবর্তী = lv_ll_ins_লেজ(&group->obj_ll);
    LV_ASSERT_MALLOC(পরবর্তী);
    যদি(পরবর্তী == ফাঁকা) ফেরত;
    *পরবর্তী = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    যদি(lv_ll_get_শীর্ষ(&group->obj_ll) == পরবর্তী) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

শূন্য lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != ফাঁকা, ফেরত);
    LV_CHECK_ARG(obj2 != ফাঁকা, ফেরত);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    যদি(g1 != g2) ফেরত;
    যদি(g1 == ফাঁকা) ফেরত;

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
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);

    lv_group_t * g = lv_obj_get_group(obj);
    যদি(g == ফাঁকা) ফেরত;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    যদি(g->obj_focus && *g->obj_focus == obj) {
        যদি(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        যদি(lv_ll_get_শীর্ষ(&g->obj_ll) != g->obj_focus || lv_ll_get_লেজ(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    যদি(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = ফাঁকা;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        যদি(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            যদি(obj->spec_attr) obj->spec_attr->group_p = ফাঁকা;
            বিরতি;
        }
    }
    LV_LOG_TRACE("finished");
}

শূন্য lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    /*Defocus the currently focused object*/
    যদি(group->obj_focus != ফাঁকা) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = ফাঁকা;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        যদি((*obj)->spec_attr)(*obj)->spec_attr->group_p = ফাঁকা;
    }

    lv_ll_clear(&(group->obj_ll));
}

শূন্য lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != ফাঁকা, ফেরত);

    lv_group_t * g = lv_obj_get_group(obj);

    যদি(g == ফাঁকা) ফেরত;
    যদি(g->frozen) ফেরত;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        যদি(*i == obj) {
            যদি(g->obj_focus != ফাঁকা) {
                যদি(*g->obj_focus == obj) বিরতি;  /*Refocusing same object*/
                lv_ফলাফল_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                যদি(res != LV_ফলাফল_OK) ফেরত;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            যদি(g->focus_cb) g->focus_cb(g);
            lv_ফলাফল_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            যদি(res != LV_ফলাফল_OK) ফেরত;
            lv_obj_invalidate(*g->obj_focus);
            বিরতি;
        }
    }
}

শূন্য lv_group_focus_পরবর্তী(lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    bool focus_changed = focus_পরবর্তী_core(group, lv_ll_get_শীর্ষ, lv_ll_get_পরবর্তী);

    যদি(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

শূন্য lv_group_focus_পূর্ব(lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    bool focus_changed = focus_পরবর্তী_core(group, lv_ll_get_লেজ, lv_ll_get_পূর্ব);

    যদি(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

শূন্য lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    যদি(en == false) group->frozen = 0;
    অন্যথা group->frozen = 1;
}

lv_ফলাফল_t lv_group_send_তথ্য(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত LV_ফলাফল_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    যদি(act == ফাঁকা) ফেরত LV_ফলাফল_OK;

    যদি(lv_obj_has_state(act, LV_STATE_DISABLED)) ফেরত LV_ফলাফল_OK;

    ফেরত lv_obj_send_event(act, LV_EVENT_চাবি, &c);
}

শূন্য lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    group->focus_cb = focus_cb;
}

শূন্য lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    group->edge_cb = edge_cb;
}

শূন্য lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);
    uint8_t en_val = edit ? 1 : 0;

    যদি(en_val == group->editing) ফেরত; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    যদি(focused) {
        lv_ফলাফল_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        যদি(res != LV_ফলাফল_OK) ফেরত;

        lv_obj_invalidate(focused);
    }
}

শূন্য lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);
    group->refocus_policy = policy & 0x01;
}

শূন্য lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(ধ্রুবক lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত ফাঁকা);
    যদি(group->obj_focus == ফাঁকা) ফেরত ফাঁকা;

    ফেরত *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(ধ্রুবক lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত ফাঁকা);
    ফেরত group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(ধ্রুবক lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত ফাঁকা);
    ফেরত group->edge_cb;
}

bool lv_group_get_editing(ধ্রুবক lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত false);
    ফেরত group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত false);
    ফেরত group->wrap;
}

uint32_t lv_group_get_obj_গুনতি(lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত 0);
    ফেরত lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_সূচক(lv_group_t * group, uint32_t সূচক)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত ফাঁকা);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        যদি(len == সূচক) {
            ফেরত *obj;
        }
        len++;
    }
    ফেরত ফাঁকা;
}

uint32_t lv_group_get_গুনতি(শূন্য)
{
    ফেরত lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_সূচক(uint32_t সূচক)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        যদি(len == সূচক) {
            ফেরত group;
        }
        len++;
    }

    ফেরত ফাঁকা;
}

#যদি LV_USE_EXT_তথ্য
শূন্য lv_group_set_external_তথ্য(lv_group_t * group, শূন্য * তথ্য, শূন্য (* free_cb)(শূন্য * তথ্য))
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);

    group->ext_তথ্য.তথ্য = তথ্য;
    group->ext_তথ্য.free_cb = free_cb;
}
#endif

শূন্য lv_group_set_user_তথ্য(lv_group_t * group, শূন্য * user_তথ্য)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত);
    group->user_তথ্য = user_তথ্য;
}

শূন্য * lv_group_get_user_তথ্য(ধ্রুবক lv_group_t * group)
{
    LV_CHECK_ARG(group != ফাঁকা, ফেরত ফাঁকা);
    ফেরত group->user_তথ্য;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

স্থির শূন্য lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != ফাঁকা);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t অস্থায়ী_wrap = g->wrap;
    g->wrap           = 1;

    যদি(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_পরবর্তী)
        lv_group_focus_পরবর্তী(g);
    অন্যথা যদি(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_পূর্ব)
        lv_group_focus_পূর্ব(g);
    /*Restore wrap property*/
    g->wrap = অস্থায়ী_wrap;
}

স্থির bool focus_পরবর্তী_core(lv_group_t * group, শূন্য * (*begin)(ধ্রুবক lv_ll_t *),
                            শূন্য * (*move)(ধ্রুবক lv_ll_t *, ধ্রুবক শূন্য *))
{
    LV_ASSERT(group != ফাঁকা);
    LV_ASSERT(begin != ফাঁকা);
    LV_ASSERT(move != ফাঁকা);
    যদি(group->frozen) ফেরত false;

    bool focus_changed = false;

    lv_obj_t ** obj_পরবর্তী     = group->obj_focus;
    lv_obj_t ** obj_sentinel = ফাঁকা;
    bool can_move            = true;
    bool can_begin           = true;

    জন্য(;;) {
        যদি(obj_পরবর্তী == ফাঁকা) {
            যদি(group->wrap || obj_sentinel == ফাঁকা) {
                যদি(!can_begin) ফেরত focus_changed;
                obj_পরবর্তী  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            অন্যথা {
                /*Currently focused object is the last/first in the group, keep it that way*/
                ফেরত focus_changed;
            }
        }

        যদি(obj_sentinel == ফাঁকা) {
            obj_sentinel = obj_পরবর্তী;
            যদি(obj_sentinel == ফাঁকা) ফেরত focus_changed; /*Group is empty*/
        }

        যদি(can_move) {
            obj_পরবর্তী = move(&group->obj_ll, obj_পরবর্তী);

            /*Give up if we walked the entire list and haven't found another visible object*/
            যদি(obj_পরবর্তী == obj_sentinel) ফেরত focus_changed;
        }

        can_move = true;

        যদি(obj_পরবর্তী == ফাঁকা) চালু;
        যদি(lv_obj_get_state(*obj_পরবর্তী) & LV_STATE_DISABLED) চালু;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_পরবর্তী;
        যখন(parent) {
            যদি(lv_obj_is_hidden(parent)) বিরতি;
            parent = lv_obj_get_parent(parent);
        }

        যদি(parent && lv_obj_is_hidden(parent)) চালু;

        /*If we got her a good candidate is found*/
        বিরতি;
    }

    যদি(obj_পরবর্তী == group->obj_focus) ফেরত focus_changed; /*There's only one visible object and it's already focused*/

    যদি(group->obj_focus) {
        lv_ফলাফল_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        যদি(res != LV_ফলাফল_OK) ফেরত focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_পরবর্তী;

    lv_ফলাফল_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    যদি(res != LV_ফলাফল_OK) ফেরত focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    যদি(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    ফেরত focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
স্থির lv_indev_t * get_indev(ধ্রুবক lv_group_t * g)
{
    LV_ASSERT(g != ফাঁকা);
    lv_indev_t * indev_guess = ফাঁকা;
    lv_indev_t * indev = lv_indev_get_পরবর্তী(ফাঁকা);

    যখন(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        যদি(indev_type == LV_INDEV_TYPE_POINTER) ফেরত indev;
        যদি(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_পরবর্তী(indev);
    }

    ফেরত indev_guess;
}
