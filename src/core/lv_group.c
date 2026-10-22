/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#अन्तर्भाव "lv_group_private.h"
#अन्तर्भाव "../core/lv_obj_private.h"
#अन्तर्भाव "../core/lv_global.h"
#अन्तर्भाव "../lvgl_public.h"

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
स्थिर bool focus_next_core(lv_group_t * group, शून्य * (*begin)(अचर lv_ll_t *),
                            शून्य * (*move)(अचर lv_ll_t *, अचर शून्य *));
स्थिर शून्य lv_group_refocus(lv_group_t * g);
स्थिर lv_indev_t * get_indev(अचर lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

शून्य lv_group_init(शून्य)
{
    lv_ll_init(group_ll_p, परिमाण(lv_group_t));
}

शून्य lv_group_deinit(शून्य)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(शून्य)
{
    lv_group_t * group = lv_ll_ins_head(group_ll_p);
    LV_ASSERT_MALLOC(group);
    यदि(group == रिक्त) फर्काऊ रिक्त;
    lv_ll_init(&group->obj_ll, परिमाण(lv_obj_t *));

    group->obj_focus      = रिक्त;
    group->frozen         = 0;
    group->focus_cb       = रिक्त;
    group->edge_cb        = रिक्त;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_PREV;
    group->wrap           = 1;
    group->user_data      = रिक्त;
#यदि LV_USE_EXT_DATA
    group->ext_data.free_cb = रिक्त;
    group->ext_data.data = रिक्त;
#endif

    फर्काऊ group;
}

शून्य lv_group_delete(lv_group_t * group)
{
    यदि(group == रिक्त) फर्काऊ;

    /*Defocus the currently focused object*/
    यदि(group->obj_focus != रिक्त) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        यदि((*obj)->spec_attr)(*obj)->spec_attr->group_p = रिक्त;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_next(रिक्त);
    जबसम्म(indev) {
        यदि(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, रिक्त);
        }
        indev = lv_indev_get_next(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    यदि(group == lv_group_get_default()) lv_group_set_default(रिक्त);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#यदि LV_USE_EXT_DATA
    यदि(group->ext_data.free_cb) {
        group->ext_data.free_cb(group->ext_data.data);
        group->ext_data.data = रिक्त;
    }
#endif
    lv_free(group);
}

शून्य lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(शून्य)
{
    फर्काऊ default_group;
}

शून्य lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);

    LV_LOG_TRACE("begin");

    यदि(!lv_obj_allocate_spec_attr(obj)) {
        फर्काऊ;
    }

    LV_ASSERT(obj->spec_attr != रिक्त);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** next = lv_ll_ins_tail(&group->obj_ll);
    LV_ASSERT_MALLOC(next);
    यदि(next == रिक्त) फर्काऊ;
    *next = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    यदि(lv_ll_get_head(&group->obj_ll) == next) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

शून्य lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != रिक्त, फर्काऊ);
    LV_CHECK_ARG(obj2 != रिक्त, फर्काऊ);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    यदि(g1 != g2) फर्काऊ;
    यदि(g1 == रिक्त) फर्काऊ;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        यदि((*obj_i) == obj1)(*obj_i) = obj2;
        अन्यथा यदि((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    यदि(focused == obj1) lv_group_focus_obj(obj2);
    अन्यथा यदि(focused == obj2) lv_group_focus_obj(obj1);
}

शून्य lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);

    lv_group_t * g = lv_obj_get_group(obj);
    यदि(g == रिक्त) फर्काऊ;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    यदि(g->obj_focus && *g->obj_focus == obj) {
        यदि(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        यदि(lv_ll_get_head(&g->obj_ll) != g->obj_focus || lv_ll_get_tail(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    यदि(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = रिक्त;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        यदि(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            यदि(obj->spec_attr) obj->spec_attr->group_p = रिक्त;
            रोक;
        }
    }
    LV_LOG_TRACE("finished");
}

शून्य lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    /*Defocus the currently focused object*/
    यदि(group->obj_focus != रिक्त) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = रिक्त;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        यदि((*obj)->spec_attr)(*obj)->spec_attr->group_p = रिक्त;
    }

    lv_ll_clear(&(group->obj_ll));
}

शून्य lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, फर्काऊ);

    lv_group_t * g = lv_obj_get_group(obj);

    यदि(g == रिक्त) फर्काऊ;
    यदि(g->frozen) फर्काऊ;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        यदि(*i == obj) {
            यदि(g->obj_focus != रिक्त) {
                यदि(*g->obj_focus == obj) रोक;  /*Refocusing same object*/
                lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                यदि(res != LV_RESULT_OK) फर्काऊ;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            यदि(g->focus_cb) g->focus_cb(g);
            lv_result_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            यदि(res != LV_RESULT_OK) फर्काऊ;
            lv_obj_invalidate(*g->obj_focus);
            रोक;
        }
    }
}

शून्य lv_group_focus_next(lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    bool focus_changed = focus_next_core(group, lv_ll_get_head, lv_ll_get_next);

    यदि(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

शून्य lv_group_focus_prev(lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    bool focus_changed = focus_next_core(group, lv_ll_get_tail, lv_ll_get_prev);

    यदि(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

शून्य lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    यदि(en == false) group->frozen = 0;
    अन्यथा group->frozen = 1;
}

lv_result_t lv_group_send_data(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ LV_RESULT_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    यदि(act == रिक्त) फर्काऊ LV_RESULT_OK;

    यदि(lv_obj_has_state(act, LV_STATE_DISABLED)) फर्काऊ LV_RESULT_OK;

    फर्काऊ lv_obj_send_event(act, LV_EVENT_साँचो, &c);
}

शून्य lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    group->focus_cb = focus_cb;
}

शून्य lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    group->edge_cb = edge_cb;
}

शून्य lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);
    uint8_t en_val = edit ? 1 : 0;

    यदि(en_val == group->editing) फर्काऊ; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    यदि(focused) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        यदि(res != LV_RESULT_OK) फर्काऊ;

        lv_obj_invalidate(focused);
    }
}

शून्य lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);
    group->refocus_policy = policy & 0x01;
}

शून्य lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(अचर lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ रिक्त);
    यदि(group->obj_focus == रिक्त) फर्काऊ रिक्त;

    फर्काऊ *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(अचर lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ रिक्त);
    फर्काऊ group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(अचर lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ रिक्त);
    फर्काऊ group->edge_cb;
}

bool lv_group_get_editing(अचर lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ false);
    फर्काऊ group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ false);
    फर्काऊ group->wrap;
}

uint32_t lv_group_get_obj_गन्ती(lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ 0);
    फर्काऊ lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_index(lv_group_t * group, uint32_t index)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ रिक्त);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        यदि(len == index) {
            फर्काऊ *obj;
        }
        len++;
    }
    फर्काऊ रिक्त;
}

uint32_t lv_group_get_गन्ती(शून्य)
{
    फर्काऊ lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_index(uint32_t index)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        यदि(len == index) {
            फर्काऊ group;
        }
        len++;
    }

    फर्काऊ रिक्त;
}

#यदि LV_USE_EXT_DATA
शून्य lv_group_set_external_data(lv_group_t * group, शून्य * data, शून्य (* free_cb)(शून्य * data))
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);

    group->ext_data.data = data;
    group->ext_data.free_cb = free_cb;
}
#endif

शून्य lv_group_set_user_data(lv_group_t * group, शून्य * user_data)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ);
    group->user_data = user_data;
}

शून्य * lv_group_get_user_data(अचर lv_group_t * group)
{
    LV_CHECK_ARG(group != रिक्त, फर्काऊ रिक्त);
    फर्काऊ group->user_data;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर शून्य lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != रिक्त);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t temp_wrap = g->wrap;
    g->wrap           = 1;

    यदि(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_NEXT)
        lv_group_focus_next(g);
    अन्यथा यदि(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_PREV)
        lv_group_focus_prev(g);
    /*Restore wrap property*/
    g->wrap = temp_wrap;
}

स्थिर bool focus_next_core(lv_group_t * group, शून्य * (*begin)(अचर lv_ll_t *),
                            शून्य * (*move)(अचर lv_ll_t *, अचर शून्य *))
{
    LV_ASSERT(group != रिक्त);
    LV_ASSERT(begin != रिक्त);
    LV_ASSERT(move != रिक्त);
    यदि(group->frozen) फर्काऊ false;

    bool focus_changed = false;

    lv_obj_t ** obj_next     = group->obj_focus;
    lv_obj_t ** obj_sentinel = रिक्त;
    bool can_move            = true;
    bool can_begin           = true;

    लागि(;;) {
        यदि(obj_next == रिक्त) {
            यदि(group->wrap || obj_sentinel == रिक्त) {
                यदि(!can_begin) फर्काऊ focus_changed;
                obj_next  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            अन्यथा {
                /*Currently focused object is the last/first in the group, keep it that way*/
                फर्काऊ focus_changed;
            }
        }

        यदि(obj_sentinel == रिक्त) {
            obj_sentinel = obj_next;
            यदि(obj_sentinel == रिक्त) फर्काऊ focus_changed; /*Group is empty*/
        }

        यदि(can_move) {
            obj_next = move(&group->obj_ll, obj_next);

            /*Give up if we walked the entire list and haven't found another visible object*/
            यदि(obj_next == obj_sentinel) फर्काऊ focus_changed;
        }

        can_move = true;

        यदि(obj_next == रिक्त) जारी;
        यदि(lv_obj_get_state(*obj_next) & LV_STATE_DISABLED) जारी;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_next;
        जबसम्म(parent) {
            यदि(lv_obj_is_hidden(parent)) रोक;
            parent = lv_obj_get_parent(parent);
        }

        यदि(parent && lv_obj_is_hidden(parent)) जारी;

        /*If we got her a good candidate is found*/
        रोक;
    }

    यदि(obj_next == group->obj_focus) फर्काऊ focus_changed; /*There's only one visible object and it's already focused*/

    यदि(group->obj_focus) {
        lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        यदि(res != LV_RESULT_OK) फर्काऊ focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_next;

    lv_result_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    यदि(res != LV_RESULT_OK) फर्काऊ focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    यदि(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    फर्काऊ focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
स्थिर lv_indev_t * get_indev(अचर lv_group_t * g)
{
    LV_ASSERT(g != रिक्त);
    lv_indev_t * indev_guess = रिक्त;
    lv_indev_t * indev = lv_indev_get_next(रिक्त);

    जबसम्म(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        यदि(indev_type == LV_INDEV_TYPE_POINTER) फर्काऊ indev;
        यदि(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_next(indev);
    }

    फर्काऊ indev_guess;
}
