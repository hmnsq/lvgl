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
#परिभाषय default_गणः LV_GLOBAL_DEFAULT()->गणः_default
#परिभाषय गणः_ll_p &(LV_GLOBAL_DEFAULT()->गणः_ll)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
स्थिर bool focus_अग्र_core(lv_गणः_t * गणः, शून्य * (*begin)(ध्रुव lv_ll_t *),
                            शून्य * (*move)(ध्रुव lv_ll_t *, ध्रुव शून्य *));
स्थिर शून्य lv_गणः_refocus(lv_गणः_t * g);
स्थिर lv_indev_t * get_indev(ध्रुव lv_गणः_t * g);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

शून्य lv_गणः_init(शून्य)
{
    lv_ll_init(गणः_ll_p, परिमाण(lv_गणः_t));
}

शून्य lv_गणः_deinit(शून्य)
{
    lv_ll_clear(गणः_ll_p);
}

lv_गणः_t * lv_गणः_create(शून्य)
{
    lv_गणः_t * गणः = lv_ll_ins_शीर्ष(गणः_ll_p);
    LV_ASSERT_MALLOC(गणः);
    यदि(गणः == रिक्त) प्रत्यागम रिक्त;
    lv_ll_init(&गणः->obj_ll, परिमाण(lv_obj_t *));

    गणः->obj_focus      = रिक्त;
    गणः->frozen         = 0;
    गणः->focus_cb       = रिक्त;
    गणः->edge_cb        = रिक्त;
    गणः->editing        = 0;
    गणः->refocus_policy = LV_गणः_REFOCUS_POLICY_पूर्व;
    गणः->wrap           = 1;
    गणः->user_दत्त      = रिक्त;
#यदि LV_USE_EXT_दत्त
    गणः->ext_दत्त.free_cb = रिक्त;
    गणः->ext_दत्त.दत्त = रिक्त;
#यदि_अन्तः

    प्रत्यागम गणः;
}

शून्य lv_गणः_delete(lv_गणः_t * गणः)
{
    यदि(गणः == रिक्त) प्रत्यागम;

    /*Defocus the currently focused object*/
    यदि(गणः->obj_focus != रिक्त) {
        lv_obj_send_event(*गणः->obj_focus, LV_EVENT_DEFOCUSED, get_indev(गणः));
        lv_obj_invalidate(*गणः->obj_focus);
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&गणः->obj_ll, obj) {
        यदि((*obj)->spec_attr)(*obj)->spec_attr->गणः_p = रिक्त;
    }

    /*Remove the group from any indev devices */
    lv_indev_t * indev = lv_indev_get_अग्र(रिक्त);
    यावत्(indev) {
        यदि(lv_indev_get_गणः(indev) == गणः) {
            lv_indev_समुच्चयः_गणः(indev, रिक्त);
        }
        indev = lv_indev_get_अग्र(indev);
    }

    /*If the group is the default group, set the default group as NULL*/
    यदि(गणः == lv_गणः_get_default()) lv_गणः_समुच्चयः_default(रिक्त);

    lv_ll_clear(&(गणः->obj_ll));
    lv_ll_remove(गणः_ll_p, गणः);
#यदि LV_USE_EXT_दत्त
    यदि(गणः->ext_दत्त.free_cb) {
        गणः->ext_दत्त.free_cb(गणः->ext_दत्त.दत्त);
        गणः->ext_दत्त.दत्त = रिक्त;
    }
#यदि_अन्तः
    lv_free(गणः);
}

शून्य lv_गणः_समुच्चयः_default(lv_गणः_t * गणः)
{
    default_गणः = गणः;
}

lv_गणः_t * lv_गणः_get_default(शून्य)
{
    प्रत्यागम default_गणः;
}

शून्य lv_गणः_सङ्कलय_obj(lv_गणः_t * गणः, lv_obj_t * obj)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    LV_LOG_TRACE("begin");

    यदि(!lv_obj_allocate_spec_attr(obj)) {
        प्रत्यागम;
    }

    LV_ASSERT(obj->spec_attr != रिक्त);

    /*Be sure the object is removed from its current group*/
    lv_गणः_remove_obj(obj);
    obj->spec_attr->गणः_p = गणः;

    lv_obj_t ** अग्र = lv_ll_ins_पुच्छ(&गणः->obj_ll);
    LV_ASSERT_MALLOC(अग्र);
    यदि(अग्र == रिक्त) प्रत्यागम;
    *अग्र = obj;

    /*If the head and the tail is equal then there is only one object in the linked list.
     *In this case automatically activate it*/
    यदि(lv_ll_get_शीर्ष(&गणः->obj_ll) == अग्र) {
        lv_गणः_refocus(गणः);
    }

    LV_LOG_TRACE("finished");
}

शून्य lv_गणः_swap_obj(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_ARG(obj1 != रिक्त, प्रत्यागम);
    LV_CHECK_ARG(obj2 != रिक्त, प्रत्यागम);

    lv_गणः_t * g1 = lv_obj_get_गणः(obj1);
    lv_गणः_t * g2 = lv_obj_get_गणः(obj2);

    यदि(g1 != g2) प्रत्यागम;
    यदि(g1 == रिक्त) प्रत्यागम;

    /*Do not add the object twice*/
    lv_obj_t ** obj_i;
    LV_LL_READ(&g1->obj_ll, obj_i) {
        यदि((*obj_i) == obj1)(*obj_i) = obj2;
        अन्यथा यदि((*obj_i) == obj2)(*obj_i) = obj1;
    }

    /* Swap the focus as well. */
    lv_obj_t * focused = lv_गणः_get_focused(g1);
    यदि(focused == obj1) lv_गणः_focus_obj(obj2);
    अन्यथा यदि(focused == obj2) lv_गणः_focus_obj(obj1);
}

शून्य lv_गणः_remove_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_गणः_t * g = lv_obj_get_गणः(obj);
    यदि(g == रिक्त) प्रत्यागम;

    LV_LOG_TRACE("begin");

    /* If we are removing the focused object */
    यदि(g->obj_focus && *g->obj_focus == obj) {
        यदि(g->frozen) g->frozen = 0;

        /*If there are more objects in the group, try to move focus to the next/prev object*/
        यदि(lv_ll_get_शीर्ष(&g->obj_ll) != g->obj_focus || lv_ll_get_पुच्छ(&g->obj_ll) != g->obj_focus) {
            lv_गणः_refocus(g);
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
            यदि(obj->spec_attr) obj->spec_attr->गणः_p = रिक्त;
            भङ्ग;
        }
    }
    LV_LOG_TRACE("finished");
}

शून्य lv_गणः_remove_all_objs(lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    /*Defocus the currently focused object*/
    यदि(गणः->obj_focus != रिक्त) {
        lv_obj_send_event(*गणः->obj_focus, LV_EVENT_DEFOCUSED, get_indev(गणः));
        lv_obj_invalidate(*गणः->obj_focus);
        गणः->obj_focus = रिक्त;
    }

    /*Remove the objects from the group*/
    lv_obj_t ** obj;
    LV_LL_READ(&गणः->obj_ll, obj) {
        यदि((*obj)->spec_attr)(*obj)->spec_attr->गणः_p = रिक्त;
    }

    lv_ll_clear(&(गणः->obj_ll));
}

शून्य lv_गणः_focus_obj(lv_obj_t * obj)
{
    LV_CHECK_ARG(obj != रिक्त, प्रत्यागम);

    lv_गणः_t * g = lv_obj_get_गणः(obj);

    यदि(g == रिक्त) प्रत्यागम;
    यदि(g->frozen) प्रत्यागम;

    /*On defocus edit mode must be leaved*/
    lv_गणः_समुच्चयः_editing(g, false);

    lv_obj_t ** i;
    LV_LL_READ(&g->obj_ll, i) {
        यदि(*i == obj) {
            यदि(g->obj_focus != रिक्त) {
                यदि(*g->obj_focus == obj) भङ्ग;  /*Refocusing same object*/
                lv_परिणाम_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_DEFOCUSED, get_indev(g));
                यदि(res != LV_परिणाम_OK) प्रत्यागम;
                lv_obj_invalidate(*g->obj_focus);
            }

            g->obj_focus = i;

            यदि(g->focus_cb) g->focus_cb(g);
            lv_परिणाम_t res = lv_obj_send_event(*g->obj_focus, LV_EVENT_FOCUSED, get_indev(g));
            यदि(res != LV_परिणाम_OK) प्रत्यागम;
            lv_obj_invalidate(*g->obj_focus);
            भङ्ग;
        }
    }
}

शून्य lv_गणः_focus_अग्र(lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    bool focus_परिवर्तितम् = focus_अग्र_core(गणः, lv_ll_get_शीर्ष, lv_ll_get_अग्र);

    यदि(!focus_परिवर्तितम् && गणः->edge_cb) {
        गणः->edge_cb(गणः, true);
    }
}

शून्य lv_गणः_focus_पूर्व(lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    bool focus_परिवर्तितम् = focus_अग्र_core(गणः, lv_ll_get_पुच्छ, lv_ll_get_पूर्व);

    यदि(!focus_परिवर्तितम् && गणः->edge_cb) {
        गणः->edge_cb(गणः, false);
    }
}

शून्य lv_गणः_focus_freeze(lv_गणः_t * गणः, bool en)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    यदि(en == false) गणः->frozen = 0;
    अन्यथा गणः->frozen = 1;
}

lv_परिणाम_t lv_गणः_send_दत्त(lv_गणः_t * गणः, uint32_t c)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम LV_परिणाम_INVALID);

    lv_obj_t * act = lv_गणः_get_focused(गणः);
    यदि(act == रिक्त) प्रत्यागम LV_परिणाम_OK;

    यदि(lv_obj_has_state(act, LV_STATE_DISABLED)) प्रत्यागम LV_परिणाम_OK;

    प्रत्यागम lv_obj_send_event(act, LV_EVENT_कुञ्चिका, &c);
}

शून्य lv_गणः_समुच्चयः_focus_cb(lv_गणः_t * गणः, lv_गणः_focus_cb_t focus_cb)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    गणः->focus_cb = focus_cb;
}

शून्य lv_गणः_समुच्चयः_edge_cb(lv_गणः_t * गणः, lv_गणः_edge_cb_t edge_cb)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    गणः->edge_cb = edge_cb;
}

शून्य lv_गणः_समुच्चयः_editing(lv_गणः_t * गणः, bool edit)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);
    uint8_t en_val = edit ? 1 : 0;

    यदि(en_val == गणः->editing) प्रत्यागम; /*Do not set the same mode again*/

    गणः->editing     = en_val;
    lv_obj_t * focused = lv_गणः_get_focused(गणः);

    यदि(focused) {
        lv_परिणाम_t res = lv_obj_send_event(*गणः->obj_focus, LV_EVENT_FOCUSED, get_indev(गणः));
        यदि(res != LV_परिणाम_OK) प्रत्यागम;

        lv_obj_invalidate(focused);
    }
}

शून्य lv_गणः_समुच्चयः_refocus_policy(lv_गणः_t * गणः, lv_गणः_refocus_policy_t policy)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);
    गणः->refocus_policy = policy & 0x01;
}

शून्य lv_गणः_समुच्चयः_wrap(lv_गणः_t * गणः, bool en)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);
    गणः->wrap = en ? 1 : 0;
}

lv_obj_t * lv_गणः_get_focused(ध्रुव lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम रिक्त);
    यदि(गणः->obj_focus == रिक्त) प्रत्यागम रिक्त;

    प्रत्यागम *गणः->obj_focus;
}

lv_गणः_focus_cb_t lv_गणः_get_focus_cb(ध्रुव lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम रिक्त);
    प्रत्यागम गणः->focus_cb;
}

lv_गणः_edge_cb_t lv_गणः_get_edge_cb(ध्रुव lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम रिक्त);
    प्रत्यागम गणः->edge_cb;
}

bool lv_गणः_get_editing(ध्रुव lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम false);
    प्रत्यागम गणः->editing;
}

bool lv_गणः_get_wrap(lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम false);
    प्रत्यागम गणः->wrap;
}

uint32_t lv_गणः_get_obj_गणनम्(lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम 0);
    प्रत्यागम lv_ll_get_len(&गणः->obj_ll);
}

lv_obj_t * lv_गणः_get_obj_by_सूचकांक(lv_गणः_t * गणः, uint32_t सूचकांक)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम रिक्त);
    uint32_t len = 0;
    lv_obj_t ** obj;

    LV_LL_READ(&गणः->obj_ll, obj) {
        यदि(len == सूचकांक) {
            प्रत्यागम *obj;
        }
        len++;
    }
    प्रत्यागम रिक्त;
}

uint32_t lv_गणः_get_गणनम्(शून्य)
{
    प्रत्यागम lv_ll_get_len(गणः_ll_p);
}

lv_गणः_t  * lv_गणः_by_सूचकांक(uint32_t सूचकांक)
{
    uint32_t len = 0;
    lv_गणः_t * गणः;

    LV_LL_READ_पृष्ठभागः(गणः_ll_p, गणः) {
        यदि(len == सूचकांक) {
            प्रत्यागम गणः;
        }
        len++;
    }

    प्रत्यागम रिक्त;
}

#यदि LV_USE_EXT_दत्त
शून्य lv_गणः_समुच्चयः_external_दत्त(lv_गणः_t * गणः, शून्य * दत्त, शून्य (* free_cb)(शून्य * दत्त))
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);

    गणः->ext_दत्त.दत्त = दत्त;
    गणः->ext_दत्त.free_cb = free_cb;
}
#यदि_अन्तः

शून्य lv_गणः_समुच्चयः_user_दत्त(lv_गणः_t * गणः, शून्य * user_दत्त)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम);
    गणः->user_दत्त = user_दत्त;
}

शून्य * lv_गणः_get_user_दत्त(ध्रुव lv_गणः_t * गणः)
{
    LV_CHECK_ARG(गणः != रिक्त, प्रत्यागम रिक्त);
    प्रत्यागम गणः->user_दत्त;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

स्थिर शून्य lv_गणः_refocus(lv_गणः_t * g)
{
    LV_ASSERT(g != रिक्त);
    /*Refocus must temporarily allow wrapping to work correctly*/
    uint8_t अस्थायी_wrap = g->wrap;
    g->wrap           = 1;

    यदि(g->refocus_policy == LV_गणः_REFOCUS_POLICY_अग्र)
        lv_गणः_focus_अग्र(g);
    अन्यथा यदि(g->refocus_policy == LV_गणः_REFOCUS_POLICY_पूर्व)
        lv_गणः_focus_पूर्व(g);
    /*Restore wrap property*/
    g->wrap = अस्थायी_wrap;
}

स्थिर bool focus_अग्र_core(lv_गणः_t * गणः, शून्य * (*begin)(ध्रुव lv_ll_t *),
                            शून्य * (*move)(ध्रुव lv_ll_t *, ध्रुव शून्य *))
{
    LV_ASSERT(गणः != रिक्त);
    LV_ASSERT(begin != रिक्त);
    LV_ASSERT(move != रिक्त);
    यदि(गणः->frozen) प्रत्यागम false;

    bool focus_परिवर्तितम् = false;

    lv_obj_t ** obj_अग्र     = गणः->obj_focus;
    lv_obj_t ** obj_sentinel = रिक्त;
    bool can_move            = true;
    bool can_begin           = true;

    प्रति(;;) {
        यदि(obj_अग्र == रिक्त) {
            यदि(गणः->wrap || obj_sentinel == रिक्त) {
                यदि(!can_begin) प्रत्यागम focus_परिवर्तितम्;
                obj_अग्र  = begin(&गणः->obj_ll);
                can_move  = false;
                can_begin = false;
            }
            अन्यथा {
                /*Currently focused object is the last/first in the group, keep it that way*/
                प्रत्यागम focus_परिवर्तितम्;
            }
        }

        यदि(obj_sentinel == रिक्त) {
            obj_sentinel = obj_अग्र;
            यदि(obj_sentinel == रिक्त) प्रत्यागम focus_परिवर्तितम्; /*Group is empty*/
        }

        यदि(can_move) {
            obj_अग्र = move(&गणः->obj_ll, obj_अग्र);

            /*Give up if we walked the entire list and haven't found another visible object*/
            यदि(obj_अग्र == obj_sentinel) प्रत्यागम focus_परिवर्तितम्;
        }

        can_move = true;

        यदि(obj_अग्र == रिक्त) अनुवर्त;
        यदि(lv_obj_get_state(*obj_अग्र) & LV_STATE_DISABLED) अनुवर्त;

        /*Hidden objects don't receive focus.
         *If any parent is hidden, the object is also hidden)*/
        lv_obj_t * parent = *obj_अग्र;
        यावत्(parent) {
            यदि(lv_obj_is_hidden(parent)) भङ्ग;
            parent = lv_obj_get_parent(parent);
        }

        यदि(parent && lv_obj_is_hidden(parent)) अनुवर्त;

        /*If we got her a good candidate is found*/
        भङ्ग;
    }

    यदि(obj_अग्र == गणः->obj_focus) प्रत्यागम focus_परिवर्तितम्; /*There's only one visible object and it's already focused*/

    यदि(गणः->obj_focus) {
        lv_परिणाम_t res = lv_obj_send_event(*गणः->obj_focus, LV_EVENT_DEFOCUSED, get_indev(गणः));
        यदि(res != LV_परिणाम_OK) प्रत्यागम focus_परिवर्तितम्;
        lv_obj_invalidate(*गणः->obj_focus);
    }

    गणः->obj_focus = obj_अग्र;

    lv_परिणाम_t res = lv_obj_send_event(*गणः->obj_focus, LV_EVENT_FOCUSED, get_indev(गणः));
    यदि(res != LV_परिणाम_OK) प्रत्यागम focus_परिवर्तितम्;

    lv_obj_invalidate(*गणः->obj_focus);

    यदि(गणः->focus_cb) गणः->focus_cb(गणः);
    focus_परिवर्तितम् = true;
    प्रत्यागम focus_परिवर्तितम्;
}

/**
 * Find an indev preferably with POINTER type (because it's the most generic) that uses the given group.
 * In other words, find an indev, that is related to the given group.
 * In the worst case simply return the latest indev
 * @param g     a group the find in the indevs
 * @return      the suggested indev
 */
स्थिर lv_indev_t * get_indev(ध्रुव lv_गणः_t * g)
{
    LV_ASSERT(g != रिक्त);
    lv_indev_t * indev_guess = रिक्त;
    lv_indev_t * indev = lv_indev_get_अग्र(रिक्त);

    यावत्(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        /*Prefer POINTER*/
        यदि(indev_type == LV_INDEV_TYPE_POINTER) प्रत्यागम indev;
        यदि(lv_indev_get_गणः(indev) == g) {
            indev_guess = indev;
        }
        indev = lv_indev_get_अग्र(indev);
    }

    प्रत्यागम indev_guess;
}
