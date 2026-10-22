/**
 * @file lv_group.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "lv_group_private.h"
#समावेश "../core/lv_obj_private.h"
#समावेश "../core/lv_global.h"
#समावेश "../lvgl_public.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा default_group LV_GLOBAL_DEFAULT()->group_default
#घोषणा group_ll_p &(LV_GLOBAL_DEFAULT()->group_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल bool focus_अग्र_core(lv_group_t * group, व्योम * (*begin)(स्थिर lv_ll_t *),
                            व्योम * (*move)(स्थिर lv_ll_t *, स्थिर व्योम *));
अटल व्योम lv_group_refocus(lv_group_t * g);
अटल lv_indev_t * get_indev(स्थिर lv_group_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

व्योम lv_group_init(व्योम)
{
    lv_ll_init(group_ll_p, माप(lv_group_t));
}

व्योम lv_group_deinit(व्योम)
{
    lv_ll_clear(group_ll_p);
}

lv_group_t * lv_group_create(व्योम)
{
    lv_group_t * group = lv_ll_ins_शीर्ष(group_ll_p);
    LV_ASSERT_MALLOC(group);
    अगर(group == शून्य) वापस शून्य;
    lv_ll_init(&group->obj_ll, माप(lv_obj_t *));

    group->obj_focus      = शून्य;
    group->frozen         = 0;
    group->focus_cb       = शून्य;
    group->edge_cb        = शून्य;
    group->editing        = 0;
    group->refocus_policy = LV_GROUP_REFOCUS_POLICY_पूर्व;
    group->wrap           = 1;
    group->user_तथ्य      = शून्य;
#अगर LV_USE_EXT_तथ्य
    group->ext_तथ्य.free_cb = शून्य;
    group->ext_तथ्य.तथ्य = शून्य;
#पूर्ण_अगर

    वापस group;
}

व्योम lv_group_delete(lv_group_t * group)
{
    अगर(group == शून्य) वापस;

    /*Defocus the currently focused object*/
    अगर(group->obj_focus != शून्य) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        अगर((*obj)->spec_attr)(*obj)->spec_attr->group_p = शून्य;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_अग्र(शून्य);
    जबतक(indev) {
        अगर(lv_indev_get_group(indev) == group) {
            lv_indev_set_group(indev, शून्य);
        }
        indev = lv_indev_get_अग्र(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    अगर(group == lv_group_get_default()) lv_group_set_default(शून्य);

    lv_ll_clear(&(group->obj_ll));
    lv_ll_remove(group_ll_p, group);
#अगर LV_USE_EXT_तथ्य
    अगर(group->ext_तथ्य.free_cb) {
        group->ext_तथ्य.free_cb(group->ext_तथ्य.तथ्य);
        group->ext_तथ्य.तथ्य = शून्य;
    }
#पूर्ण_अगर
    lv_free(group);
}

व्योम lv_group_set_default(lv_group_t * group)
{
    default_group = group;
}

lv_group_t * lv_group_get_default(व्योम)
{
    वापस default_group;
}

व्योम lv_group_add_obj(lv_group_t * group, lv_obj_t * obj)
{
    LV_CHECK_ARG(group != शून्य, वापस);
    LV_CHECK_ARG(obj != शून्य, वापस);

    LV_LOG_TRACE("begin");

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }

    LV_ASSERT(obj->spec_attr != शून्य);

    /*Be sure the object is removed from its current group*/
    lv_group_remove_obj(obj);
    obj->spec_attr->group_p = group;

    lv_obj_t ** अग्र = lv_ll_ins_पुच्छ(&group->obj_ll);
    LV_ASSERT_MALLOC(अग्र);
    अगर(अग्र == शून्य) वापस;
    *अग्र = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    अगर(lv_ll_get_शीर्ष(&group->obj_ll) == अग्र) {
        lv_group_refocus(group);
    }

    LV_LOG_TRACE("finished");
}

व्योम lv_group_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != शून्य, वापस);
    LV_CHECK_ARG(obj2 != शून्य, वापस);

    lv_group_t * g1 = lv_obj_get_group(obj1);
    lv_group_t * g2 = lv_obj_get_group(obj2);

    अगर(g1 != g2) वापस;
    अगर(g1 == शून्य) वापस;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        अगर((*obj_i) == obj1)(*obj_i) = obj2;
        अन्यथा अगर((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_group_get_focused(g1);
    अगर(focused == obj1) lv_group_focus_obj(obj2);
    अन्यथा अगर(focused == obj2) lv_group_focus_obj(obj1);
}

व्योम lv_group_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    lv_group_t * g = lv_obj_get_group(obj);
    अगर(g == शून्य) वापस;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    अगर(g->obj_focus && *g->obj_focus == obj) {
        अगर(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        अगर(lv_ll_get_शीर्ष(&g->obj_ll) != g->obj_focus || lv_ll_get_पुच्छ(&g->obj_ll) != g->obj_focus) {
            lv_group_refocus(g);
        }
    }

    /*If the focused object is still the one being removed (either it was the only object,
     *or refocus couldn't find a focusable candidate), defocus it and clear the focus.*/
    अगर(g->obj_focus && *g->obj_focus == obj) {
        lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
        g->obj_focus = शून्य;
    }

    /*Search the object and remove it from its group*/
    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        अगर(*i == obj) {
            lv_ll_remove(&g->obj_ll, i);
            lv_free(i);
            अगर(obj->spec_attr) obj->spec_attr->group_p = शून्य;
            अवरोध;
        }
    }
    LV_LOG_TRACE("finished");
}

व्योम lv_group_remove_all_objs(lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    /*Defocus the currently focused object*/
    अगर(group->obj_focus != शून्य) {
        lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        lv_obj_invalidate(*group->obj_focus);
        group->obj_focus = शून्य;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&group->obj_ll, obj) {
        अगर((*obj)->spec_attr)(*obj)->spec_attr->group_p = शून्य;
    }

    lv_ll_clear(&(group->obj_ll));
}

व्योम lv_group_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != शून्य, वापस);

    lv_group_t * g = lv_obj_get_group(obj);

    अगर(g == शून्य) वापस;
    अगर(g->frozen) वापस;

    /*On defocus edit mode must be leaved*/
    lv_group_set_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        अगर(*i == obj) {
            अगर(g->obj_focus != शून्य) {
                अगर(*g->obj_focus == obj) अवरोध;  /*Refocusing same object*/
                lv_परिणाम_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                अगर(res != LV_परिणाम_OK) वापस;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            अगर(g->focus_cb) g->focus_cb(g);
            lv_परिणाम_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            अगर(res != LV_परिणाम_OK) वापस;
            lv_obj_invalidate(*g->obj_focus);
            अवरोध;
        }
    }
}

व्योम lv_group_focus_अग्र(lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    bool focus_changed = focus_अग्र_core(group, lv_ll_get_शीर्ष, lv_ll_get_अग्र);

    अगर(!focus_changed && group->edge_cb) {
        group->edge_cb(group, true);
    }
}

व्योम lv_group_focus_पूर्व(lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    bool focus_changed = focus_अग्र_core(group, lv_ll_get_पुच्छ, lv_ll_get_पूर्व);

    अगर(!focus_changed && group->edge_cb) {
        group->edge_cb(group, false);
    }
}

व्योम lv_group_focus_freeze(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    अगर(en == false) group->frozen = 0;
    अन्यथा group->frozen = 1;
}

lv_परिणाम_t lv_group_send_तथ्य(lv_group_t * group, uint32_t c)
{
    LV_CHECK_ARG(group != शून्य, वापस LV_परिणाम_INVALID);

    lv_obj_t * act = lv_group_get_focused(group);
    अगर(act == शून्य) वापस LV_परिणाम_OK;

    अगर(lv_obj_has_state(act, LV_STATE_DISABLED)) वापस LV_परिणाम_OK;

    वापस lv_obj_send_event(act, LV_EVENT_कुंजी, &c);
}

व्योम lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    group->focus_cb = focus_cb;
}

व्योम lv_group_set_edge_cb(lv_group_t * group, lv_group_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(group != शून्य, वापस);

    group->edge_cb = edge_cb;
}

व्योम lv_group_set_editing(lv_group_t * group, bool edit)
{
    LV_CHECK_ARG(group != शून्य, वापस);
    uint8_t en_val = edit ? 1 : 0;

    अगर(en_val == group->editing) वापस; /*Do not set the same mode again*/

    group->editing     = en_val;
    lv_obj_t * focused = lv_group_get_focused(group);

    अगर(focused) {
        lv_परिणाम_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
        अगर(res != LV_परिणाम_OK) वापस;

        lv_obj_invalidate(focused);
    }
}

व्योम lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy)
{
    LV_CHECK_ARG(group != शून्य, वापस);
    group->refocus_policy = policy & 0x01;
}

व्योम lv_group_set_wrap(lv_group_t * group, bool en)
{
    LV_CHECK_ARG(group != शून्य, वापस);
    group->wrap = en ? 1 : 0;
}

lv_obj_t * lv_group_get_focused(स्थिर lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस शून्य);
    अगर(group->obj_focus == शून्य) वापस शून्य;

    वापस *group->obj_focus;
}

lv_group_focus_cb_t lv_group_get_focus_cb(स्थिर lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस शून्य);
    वापस group->focus_cb;
}

lv_group_edge_cb_t lv_group_get_edge_cb(स्थिर lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस शून्य);
    वापस group->edge_cb;
}

bool lv_group_get_editing(स्थिर lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस false);
    वापस group->editing;
}

bool lv_group_get_wrap(lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस false);
    वापस group->wrap;
}

uint32_t lv_group_get_obj_गिनती(lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस 0);
    वापस lv_ll_get_len(&group->obj_ll);
}

lv_obj_t * lv_group_get_obj_by_सूचकांक(lv_group_t * group, uint32_t सूचकांक)
{
    LV_CHECK_ARG(group != शून्य, वापस शून्य);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&group->obj_ll, obj) {
        अगर(len == सूचकांक) {
            वापस *obj;
        }
        len++;
    }
    वापस शून्य;
}

uint32_t lv_group_get_गिनती(व्योम)
{
    वापस lv_ll_get_len(group_ll_p);
}

lv_group_t  * lv_group_by_सूचकांक(uint32_t सूचकांक)
{
    uint32_t len = 0;
    lv_group_t * group;

    LV_LL_READ_BACK(group_ll_p, group) {
        अगर(len == सूचकांक) {
            वापस group;
        }
        len++;
    }

    वापस शून्य;
}

#अगर LV_USE_EXT_तथ्य
व्योम lv_group_set_external_तथ्य(lv_group_t * group, व्योम * तथ्य, व्योम (* free_cb)(व्योम * तथ्य))
{
    LV_CHECK_ARG(group != शून्य, वापस);

    group->ext_तथ्य.तथ्य = तथ्य;
    group->ext_तथ्य.free_cb = free_cb;
}
#पूर्ण_अगर

व्योम lv_group_set_user_तथ्य(lv_group_t * group, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(group != शून्य, वापस);
    group->user_तथ्य = user_तथ्य;
}

व्योम * lv_group_get_user_तथ्य(स्थिर lv_group_t * group)
{
    LV_CHECK_ARG(group != शून्य, वापस शून्य);
    वापस group->user_तथ्य;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल व्योम lv_group_refocus(lv_group_t * g)
{
    LV_ASSERT(g != शून्य);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t अस्थायी_wrap = g->wrap;
    g->wrap           = 1;

    अगर(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_अग्र)
        lv_group_focus_अग्र(g);
    अन्यथा अगर(g->refocus_policy == LV_GROUP_REFOCUS_POLICY_पूर्व)
        lv_group_focus_पूर्व(g);
    /*Restore wrap property*/
    g->wrap = अस्थायी_wrap;
}

अटल bool focus_अग्र_core(lv_group_t * group, व्योम * (*begin)(स्थिर lv_ll_t *),
                            व्योम * (*move)(स्थिर lv_ll_t *, स्थिर व्योम *))
{
    LV_ASSERT(group != शून्य);
    LV_ASSERT(begin != शून्य);
    LV_ASSERT(move != शून्य);
    अगर(group->frozen) वापस false;

    bool focus_changed = false;

    lv_obj_t ** obj_अग्र     = group->obj_focus;
    lv_obj_t ** obj_sentinel = शून्य;
    bool can_move            = true;
    bool can_begin           = true;

    क्रम(;;) {
        अगर(obj_अग्र == शून्य) {
            अगर(group->wrap || obj_sentinel == शून्य) {
                अगर(!can_begin) वापस focus_changed;
                obj_अग्र  = begin(&group->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            अन्यथा {
                /*Currently focused object is the last/first in the group, keep it that way*/
                वापस focus_changed;
            }
        }

        अगर(obj_sentinel == शून्य) {
            obj_sentinel = obj_अग्र;
            अगर(obj_sentinel == शून्य) वापस focus_changed; /*Group is empty*/
        }

        अगर(can_move) {
            obj_अग्र = move(&group->obj_ll, obj_अग्र);

            /*Give up if we walked the entire list and haven't found another visible object*/
            अगर(obj_अग्र == obj_sentinel) वापस focus_changed;
        }

        can_move = true;

        अगर(obj_अग्र == शून्य) जारी;
        अगर(lv_obj_get_state(*obj_अग्र) & LV_STATE_DISABLED) जारी;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_अग्र;
        जबतक(parent) {
            अगर(lv_obj_is_hidden(parent)) अवरोध;
            parent = lv_obj_get_parent(parent);
        }

        अगर(parent && lv_obj_is_hidden(parent)) जारी;

        /*If we got her a good candidate is found*/
        अवरोध;
    }

    अगर(obj_अग्र == group->obj_focus) वापस focus_changed; /*There's only one visible object and it's already focused*/

    अगर(group->obj_focus) {
        lv_परिणाम_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_DEFOCUSED, get_indev(group));
        अगर(res != LV_परिणाम_OK) वापस focus_changed;
        lv_obj_invalidate(*group->obj_focus);
    }

    group->obj_focus = obj_अग्र;

    lv_परिणाम_t res = lv_obj_send_event(*group->obj_focus, LV_EVENT_FOCUSED, get_indev(group));
    अगर(res != LV_परिणाम_OK) वापस focus_changed;

    lv_obj_invalidate(*group->obj_focus);

    अगर(group->focus_cb) group->focus_cb(group);
    focus_changed = true;
    वापस focus_changed;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
अटल lv_indev_t * get_indev(स्थिर lv_group_t * g)
{
    LV_ASSERT(g != शून्य);
    lv_indev_t * indev_guess = शून्य;
    lv_indev_t * indev = lv_indev_get_अग्र(शून्य);

    जबतक(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        अगर(indev_type == LV_INDEV_TYPE_POINTER) वापस indev;
        अगर(lv_indev_get_group(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_अग्र(indev);
    }

    वापस indev_guess;
}
