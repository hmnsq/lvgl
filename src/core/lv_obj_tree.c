/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#అన్తర్భావ "lv_obj_private.h"
#అన్తర్భావ "../lvgl_public.h"
#అన్తర్భావ "lv_obj_class_private.h"
#అన్తర్భావ "../indev/lv_indev_private.h"
#అన్తర్భావ "../display/lv_display_private.h"
#అన్తర్భావ "../misc/lv_anim_private.h"
#అన్తర్భావ "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#నిర్వచించు MY_CLASS (&lv_obj_class)
#నిర్వచించు disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#నిర్వచించు OBJ_DUMP_STRING_LEN 128
#నిర్వచించు LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
స్థిర శూన్య lv_obj_delete_async_cb(శూన్య * obj);
స్థిర శూన్య obj_delete_core(lv_obj_t * obj);
స్థిర lv_obj_వృక్షం_walk_res_t walk_core(lv_obj_t * obj, lv_obj_వృక్షం_walk_cb_t cb, శూన్య * user_దత్తాంశం);
స్థిర శూన్య dump_వృక్షం_core(lv_obj_t * obj, int32_t లోతు);
స్థిర lv_obj_t * lv_obj_get_మొదటి_not_deleting_బిడ్డ(lv_obj_t * obj);
#ఒకవేళ LV_USE_OBJ_NAME
    స్థిర lv_obj_t * కనుగొను_by_name_ప్రత్యక్ష(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి అక్షరం * name, కొలత_t len);
#అయితే_ముగింపు /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

శూన్య lv_obj_delete(lv_obj_t * obj)
{
    ఒకవేళ(obj == ఖాళీ) తిరిగి;
    ఒకవేళ(obj->is_deleting) తిరిగి;

    LV_LOG_TRACE("begin (delete %p)", (శూన్య *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_మాతృ(obj);

    lv_display_t * disp = ఖాళీ;
    bool act_screen_del = false;
    ఒకవేళ(par == ఖాళీ) {
        disp = lv_obj_get_display(obj);
        ఒకవేళ(!disp) తిరిగి;   /*Shouldn't happen*/
        ఒకవేళ(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    ఒకవేళ(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_బిడ్డ_మారింది, ఖాళీ);
        lv_obj_send_event(par, LV_EVENT_బిడ్డ_DELETED, ఖాళీ);
    }

    /*Handle if the active screen was deleted*/
    ఒకవేళ(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = ఖాళీ;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (శూన్య *)obj);
}

శూన్య lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (శూన్య *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_బిడ్డ_లెక్క(obj);
    lv_obj_t * బిడ్డ = lv_obj_get_మొదటి_not_deleting_బిడ్డ(obj);
    అయితే(బిడ్డ) {
        obj_delete_core(బిడ్డ);
        బిడ్డ = lv_obj_get_మొదటి_not_deleting_బిడ్డ(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_గమ్యం(obj, 0, 0, LV_ANIM_OFF);
    ఒకవేళ(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    ఒకవేళ(lv_obj_get_బిడ్డ_లెక్క(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_బిడ్డ_మారింది, ఖాళీ);
        lv_obj_send_event(obj, LV_EVENT_బిడ్డ_DELETED, ఖాళీ);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (శూన్య *)obj);
}

శూన్య lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    ఒకవేళ(obj == ఖాళీ) తిరిగి;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_సమితి_var(&a, obj);
    lv_anim_సమితి_exec_cb(&a, ఖాళీ);
    lv_anim_సమితి_duration(&a, 1);
    lv_anim_సమితి_delay(&a, delay_ms);
    lv_anim_సమితి_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_ప్రారంభం(&a);
}

శూన్య lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != ఖాళీ, తిరిగి);

    lv_obj_delete(a->var);
}

శూన్య lv_obj_delete_async(lv_obj_t * obj)
{
    ఒకవేళ(obj == ఖాళీ) తిరిగి;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

శూన్య lv_obj_సమితి_మాతృ(lv_obj_t * obj, lv_obj_t * మాతృ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_OBJ(మాతృ, MY_CLASS, తిరిగి);

    ఒకవేళ(obj->మాతృ == ఖాళీ) {
        LV_LOG_WARN("Can't set the parent of a screen");
        తిరిగి;
    }

    ఒకవేళ(మాతృ == obj->మాతృ) {
        తిరిగి;
    }

    ఒకవేళ(!lv_obj_allocate_spec_attr(మాతృ)) {
        తిరిగి;
    }

    /*Add the object to the new parents list first*/
    ఒకవేళ(lv_obj_కూడు_బిడ్డ(మాతృ, obj) != LV_ఫలితం_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        తిరిగి;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_బిడ్డ(obj->మాతృ, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->మాతృ);
    lv_obj_send_event(obj->మాతృ, LV_EVENT_బిడ్డ_మారింది, obj);
    lv_obj_send_event(obj->మాతృ, LV_EVENT_బిడ్డ_DELETED, ఖాళీ);
    obj->మాతృ = మాతృ;

    /*Notify the new parent about the child*/
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, obj);
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_CREATED, ఖాళీ);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

శూన్య lv_obj_move_గమ్యం_సూచీ(lv_obj_t * obj, int32_t సూచీ)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    /* Check parent validity */
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(!మాతృ) {
        LV_LOG_WARN("parent is NULL");
        తిరిగి;
    }

    స్థిరరాశి uint32_t మాతృ_బిడ్డ_లెక్క = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    స్థిరరాశి int32_t old_సూచీ = lv_obj_get_సూచీ(obj);
    LV_ASSERT(0 <= old_సూచీ);

    ఒకవేళ(సూచీ < 0) {
        సూచీ += మాతృ_బిడ్డ_లెక్క;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    ఒకవేళ((సూచీ < 0)
       /* Index is same or bigger than parent child count */
       || (సూచీ >= (int32_t) మాతృ_బిడ్డ_లెక్క)
       /* If both previous and new index are the same */
       || (సూచీ == old_సూచీ)) {

        తిరిగి;
    }

    int32_t i = old_సూచీ;
    ఒకవేళ(సూచీ < old_సూచీ) {
        అయితే(i > సూచీ)  {
            మాతృ->spec_attr->children[i] = మాతృ->spec_attr->children[i - 1];
            i--;
        }
    }
    లేకపోతే {
        అయితే(i < సూచీ) {
            మాతృ->spec_attr->children[i] = మాతృ->spec_attr->children[i + 1];
            i++;
        }
    }

    మాతృ->spec_attr->children[సూచీ] = obj;
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, ఖాళీ);
    lv_obj_invalidate(మాతృ);
}

శూన్య lv_obj_మార్పిడి(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, తిరిగి);
    LV_CHECK_OBJ(obj2, MY_CLASS, తిరిగి);

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj1);
    lv_obj_t * parent2 = lv_obj_get_మాతృ(obj2);

    uint_fast32_t index1 = lv_obj_get_సూచీ(obj1);
    uint_fast32_t index2 = lv_obj_get_సూచీ(obj2);

    lv_obj_send_event(parent2, LV_EVENT_బిడ్డ_DELETED, obj2);
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_DELETED, obj1);

    మాతృ->spec_attr->children[index1] = obj2;
    obj2->మాతృ = మాతృ;

    parent2->spec_attr->children[index2] = obj1;
    obj1->మాతృ = parent2;

    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_మారింది, obj2);
    lv_obj_send_event(మాతృ, LV_EVENT_బిడ్డ_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_బిడ్డ_మారింది, obj1);
    lv_obj_send_event(parent2, LV_EVENT_బిడ్డ_CREATED, obj1);

    lv_obj_invalidate(మాతృ);

    ఒకవేళ(మాతృ != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_సమూహం_మార్పిడి_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    స్థిరరాశి lv_obj_t * par = obj;
    స్థిరరాశి lv_obj_t * act_par;

    చేయి {
        act_par = par;
        par = lv_obj_get_మాతృ(act_par);
    } అయితే(par != ఖాళీ);

    తిరిగి (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    స్థిరరాశి lv_obj_t * scr;

    ఒకవేళ(obj->మాతృ == ఖాళీ) scr = obj;  /*`obj` is a screen*/
    లేకపోతే scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_తల = disp_ll_p;
    LV_LL_READ(disp_తల, d) {
        uint32_t i;
        కొరకు(i = 0; i < d->screen_cnt; i++) {
            ఒకవేళ(d->screens[i] == scr) తిరిగి d;
        }
    }

    LV_LOG_WARN("No screen found");
    తిరిగి ఖాళీ;
}

lv_obj_t * lv_obj_get_మాతృ(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    తిరిగి obj->మాతృ;
}

lv_obj_t * lv_obj_get_బిడ్డ(స్థిరరాశి lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి ఖాళీ;

    ఒకవేళ(idx < 0) {
        idx = obj->spec_attr->బిడ్డ_cnt + idx;
        ఒకవేళ(idx < 0) తిరిగి ఖాళీ;
    }

    uint32_t idu = (uint32_t) idx;

    ఒకవేళ(idu < obj->spec_attr->బిడ్డ_cnt) తిరిగి obj->spec_attr->children[idu];
    లేకపోతే తిరిగి ఖాళీ;
}

lv_obj_t * lv_obj_get_బిడ్డ_by_type(స్థిరరాశి lv_obj_t * obj, int32_t idx, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి ఖాళీ);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి ఖాళీ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->బిడ్డ_cnt;
    ఒకవేళ(idx >= 0) {
        కొరకు(i = 0; i < cnt; i++) {
            ఒకవేళ(obj->spec_attr->children[i]->class_p == class_p) {
                ఒకవేళ(idx == 0) తిరిగి obj->spec_attr->children[i];
                లేకపోతే idx--;
            }
        }
    }
    లేకపోతే {
        idx++;   /*-1 means the first child*/
        కొరకు(i = cnt - 1; i >= 0; i--) {
            ఒకవేళ(obj->spec_attr->children[i]->class_p == class_p) {
                ఒకవేళ(idx == 0) తిరిగి obj->spec_attr->children[i];
                లేకపోతే idx++;
            }
        }
    }
    తిరిగి ఖాళీ;
}

lv_obj_t * lv_obj_get_sibling(స్థిరరాశి lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);
    LV_CHECK_ARG(obj->మాతృ != ఖాళీ, తిరిగి ఖాళీ);

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_సూచీ(obj) + idx;
    ఒకవేళ(sibling_idx < 0) తిరిగి ఖాళీ;

    తిరిగి lv_obj_get_బిడ్డ(మాతృ, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(స్థిరరాశి lv_obj_t * obj, int32_t idx, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);
    LV_CHECK_ARG(obj->మాతృ != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి ఖాళీ);

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_సూచీ_by_type(obj, class_p) + idx;
    ఒకవేళ(sibling_idx < 0) తిరిగి ఖాళీ;

    తిరిగి lv_obj_get_బిడ్డ_by_type(మాతృ, sibling_idx, class_p);
}

uint32_t lv_obj_get_బిడ్డ_లెక్క(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);
    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;
    తిరిగి obj->spec_attr->బిడ్డ_cnt;
}

uint32_t lv_obj_get_బిడ్డ_లెక్క_by_type(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి 0);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి 0);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి 0;

    uint32_t i;
    uint32_t cnt = 0;
    కొరకు(i = 0; i < obj->spec_attr->బిడ్డ_cnt; i++) {
        ఒకవేళ(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    తిరిగి cnt;
}

#ఒకవేళ LV_USE_OBJ_NAME

శూన్య lv_obj_సమితి_name(lv_obj_t * obj, స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }

    ఒకవేళ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((శూన్య *)obj->spec_attr->name);

    ఒకవేళ(name == ఖాళీ) {
        obj->spec_attr->name = ఖాళీ;
        obj->spec_attr->name_static = 1;
    }
    లేకపోతే {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

శూన్య lv_obj_సమితి_name_static(lv_obj_t * obj, స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(name != ఖాళీ, తిరిగి);

    ఒకవేళ(!lv_obj_allocate_spec_attr(obj)) {
        తిరిగి;
    }
    ఒకవేళ(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((శూన్య *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

స్థిరరాశి అక్షరం * lv_obj_get_name(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి ఖాళీ);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి ఖాళీ;
    లేకపోతే తిరిగి obj->spec_attr->name;
}

శూన్య lv_obj_get_name_resolved(స్థిరరాశి lv_obj_t * obj, అక్షరం buf[], కొలత_t buf_కొలత)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి);
    LV_CHECK_ARG(buf != ఖాళీ && buf_కొలత > 0, తిరిగి);

    స్థిరరాశి అక్షరం * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    అక్షరం name_buf[LV_OBJ_NAME_MAX_LEN];
    ఒకవేళ(name == ఖాళీ) {
        lv_snprintf(name_buf, పరిమాణం(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    కొలత_t name_len = lv_strlen(name);
    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);

    /*If the last character is # automatically index the children with the same name start*/
    ఒకవేళ(మాతృ && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
        uint32_t cnt = 0;
        uint32_t i;
        కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
            lv_obj_t * బిడ్డ = lv_obj_get_బిడ్డ(మాతృ, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            ఒకవేళ(బిడ్డ == obj) {
                అక్షరం అంకె_buf[8];
                కొలత_t అంకె_len;
                అంకె_len = lv_snprintf(అంకె_buf, పరిమాణం(అంకె_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                ఒకవేళ(buf_కొలత > name_len + అంకె_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], అంకె_buf);
                }
                లేకపోతే {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_కొలత);
                }
                ఆపు;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            లేకపోతే {
                స్థిరరాశి అక్షరం * బిడ్డ_name = lv_obj_get_name(బిడ్డ);
                ఒకవేళ(బిడ్డ_name == ఖాళీ) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    కొలత_t class_name_len = lv_strlen(బిడ్డ->class_p->name);
                    ఒకవేళ(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(బిడ్డ->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                లేకపోతే {
                    ఒకవేళ(lv_strcmp(బిడ్డ->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    లేకపోతే {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_కొలత);
    }
}

lv_obj_t * lv_obj_get_బిడ్డ_by_name(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి అక్షరం * మార్గం)
{
    LV_CHECK_OBJ(మాతృ, MY_CLASS, తిరిగి ఖాళీ);
    LV_CHECK_ARG(మార్గం != ఖాళీ, తిరిగి ఖాళీ);
    LV_CHECK_ARG(మాతృ->spec_attr != ఖాళీ, తిరిగి ఖాళీ);

    అయితే(*మార్గం) {
        స్థిరరాశి అక్షరం * segment = మార్గం;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        అయితే(మార్గం[len] && మార్గం[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * బిడ్డ = కనుగొను_by_name_ప్రత్యక్ష(మాతృ, segment, len);
        ఒకవేళ(!బిడ్డ) తిరిగి ఖాళీ; /*Segment not found*/

        /* Advance to the next segment */
        మార్గం += len;
        ఒకవేళ(*మార్గం == '/') మార్గం++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        ఒకవేళ(*మార్గం == '\0') తిరిగి బిడ్డ;

        మాతృ = బిడ్డ;
    }

    తిరిగి ఖాళీ;

}

lv_obj_t * lv_obj_కనుగొను_by_name(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి అక్షరం * name)
{
    LV_CHECK_ARG(name != ఖాళీ, తిరిగి ఖాళీ);

    ఒకవేళ(మాతృ == ఖాళీ) మాతృ = lv_display_get_screen_చురుకైన(lv_display_get_default());
    ఒకవేళ(మాతృ == ఖాళీ) తిరిగి ఖాళీ;

    lv_obj_t * బిడ్డ = కనుగొను_by_name_ప్రత్యక్ష(మాతృ, name, UINT16_MAX);
    ఒకవేళ(బిడ్డ) తిరిగి బిడ్డ;

    /*Search children recursively*/
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
    uint32_t i;
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        బిడ్డ = మాతృ->spec_attr->children[i];
        lv_obj_t * దొరికింది = lv_obj_కనుగొను_by_name(బిడ్డ, name);
        ఒకవేళ(దొరికింది != ఖాళీ) తిరిగి దొరికింది;
    }

    తిరిగి ఖాళీ;
}

#అయితే_ముగింపు /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_సూచీ(స్థిరరాశి lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి -1);

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ == ఖాళీ) తిరిగి -1;

    uint32_t i = 0;
    కొరకు(i = 0; i < మాతృ->spec_attr->బిడ్డ_cnt; i++) {
        ఒకవేళ(మాతృ->spec_attr->children[i] == obj) తిరిగి (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    తిరిగి -1;
}

int32_t lv_obj_get_సూచీ_by_type(స్థిరరాశి lv_obj_t * obj, స్థిరరాశి lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, తిరిగి -1);
    LV_CHECK_ARG(class_p != ఖాళీ, తిరిగి -1);

    lv_obj_t * మాతృ = lv_obj_get_మాతృ(obj);
    ఒకవేళ(మాతృ == ఖాళీ) తిరిగి 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    కొరకు(i = 0; i < మాతృ->spec_attr->బిడ్డ_cnt; i++) {
        lv_obj_t * బిడ్డ = మాతృ->spec_attr->children[i];
        ఒకవేళ(బిడ్డ->class_p == class_p) {
            ఒకవేళ(బిడ్డ == obj) తిరిగి idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    తిరిగి -1;
}

శూన్య lv_obj_వృక్షం_walk(lv_obj_t * ప్రారంభం_obj, lv_obj_వృక్షం_walk_cb_t cb, శూన్య * user_దత్తాంశం)
{
    LV_CHECK_ARG(cb != ఖాళీ, తిరిగి);

    walk_core(ప్రారంభం_obj, cb, user_దత్తాంశం);
}

శూన్య lv_obj_dump_వృక్షం(lv_obj_t * ప్రారంభం_obj)
{
    ఒకవేళ(ప్రారంభం_obj == ఖాళీ) {
        lv_display_t * disp = lv_display_get_తదుపరి(ఖాళీ);
        అయితే(disp) {
            uint32_t i;
            కొరకు(i = 0; i < disp->screen_cnt; i++) {
                dump_వృక్షం_core(disp->screens[i], 0);
            }
            disp = lv_display_get_తదుపరి(disp);
        }
    }
    లేకపోతే {
        dump_వృక్షం_core(ప్రారంభం_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

స్థిర శూన్య lv_obj_delete_async_cb(శూన్య * obj)
{
    lv_obj_delete(obj);
}

స్థిర శూన్య obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    ఒకవేళ(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

స్థిర శూన్య obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != ఖాళీ);
    ఒకవేళ(obj->is_deleting)
        తిరిగి;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_ఫలితం_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, ఖాళీ);
    ఒకవేళ(res == LV_ఫలితం_INVALID) {
        obj->is_deleting = false;
        తిరిగి;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    ఒకవేళ(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    ఒకవేళ(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * బిడ్డ = lv_obj_get_బిడ్డ(obj, 0);
    అయితే(బిడ్డ) {
        obj_delete_core(బిడ్డ);
        బిడ్డ = lv_obj_get_బిడ్డ(obj, 0);
    }

    lv_సమూహం_t * సమూహం = lv_obj_get_సమూహం(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_తదుపరి(ఖాళీ);
    అయితే(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        ఒకవేళ(indev_type == LV_INDEV_TYPE_సూచిక || indev_type == LV_INDEV_TYPE_BUTTON) {
            ఒకవేళ(indev->సూచిక.act_obj == obj || indev->సూచిక.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            ఒకవేళ(indev->సూచిక.చివరి_pressed == obj) {
                indev->సూచిక.చివరి_pressed = ఖాళీ;
            }
            ఒకవేళ(indev->సూచిక.చివరి_hovered == obj) {
                indev->సూచిక.చివరి_hovered = ఖాళీ;
            }
        }

        ఒకవేళ(indev->సమూహం == సమూహం && obj == lv_indev_get_చురుకైన_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_తదుపరి(indev);
    }

    /*Delete all pending async del-s*/
    lv_ఫలితం_t async_cancel_res = LV_ఫలితం_OK;
    అయితే(async_cancel_res == LV_ఫలితం_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    ఒకవేళ(obj->మాతృ == ఖాళీ) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        కొరకు(i = 0; i < disp->screen_cnt; i++) {
            ఒకవేళ(disp->screens[i] == obj) ఆపు;
        }

        uint32_t id = i;
        కొరకు(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * పరిమాణం(lv_obj_t *));
        ఒకవేళ(new_screens != ఖాళీ) {
            disp->screens = new_screens;
        }
        లేకపోతే {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    లేకపోతే {
        lv_obj_remove_బిడ్డ(obj->మాతృ, obj);
    }

    obj->మాతృ = ఖాళీ;

    /*Free the object itself*/
    lv_free(obj);
}

స్థిర lv_obj_వృక్షం_walk_res_t walk_core(lv_obj_t * obj, lv_obj_వృక్షం_walk_cb_t cb, శూన్య * user_దత్తాంశం)
{
    lv_obj_వృక్షం_walk_res_t res = LV_OBJ_వృక్షం_WALK_తదుపరి;

    ఒకవేళ(obj == ఖాళీ) {
        lv_display_t * disp = lv_display_get_తదుపరి(ఖాళీ);
        అయితే(disp) {
            uint32_t i;
            కొరకు(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_దత్తాంశం);
            }
            disp = lv_display_get_తదుపరి(disp);
        }
        తిరిగి LV_OBJ_వృక్షం_WALK_చివర;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_దత్తాంశం);

    ఒకవేళ(res == LV_OBJ_వృక్షం_WALK_చివర) తిరిగి LV_OBJ_వృక్షం_WALK_చివర;

    ఒకవేళ(res != LV_OBJ_వృక్షం_WALK_SKIP_CHILDREN) {
        uint32_t i;
        కొరకు(i = 0; i < lv_obj_get_బిడ్డ_లెక్క(obj); i++) {
            res = walk_core(lv_obj_get_బిడ్డ(obj, i), cb, user_దత్తాంశం);
            ఒకవేళ(res == LV_OBJ_వృక్షం_WALK_చివర) తిరిగి LV_OBJ_వృక్షం_WALK_చివర;
        }
    }
    తిరిగి LV_OBJ_వృక్షం_WALK_తదుపరి;
}

స్థిర శూన్య dump_వృక్షం_core(lv_obj_t * obj, int32_t లోతు)
{
    LV_ASSERT(obj != ఖాళీ);
#ఒకవేళ LV_USE_LOG
    స్థిరరాశి అక్షరం * id;

#ఒకవేళ LV_USE_OBJ_ID
    అక్షరం buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, పరిమాణం(buf));
    ఒకవేళ(id == ఖాళీ) id = "obj0";
#లేకపోతే
    id = "obj0";
#అయితే_ముగింపు

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (పూర్ణాంకం)(2 * లోతు), "", (శూన్య *)obj, id);
#అయితే_ముగింపు /*LV_USE_LOG*/

    ఒకవేళ(obj->spec_attr && obj->spec_attr->బిడ్డ_cnt) {
        కొరకు(uint32_t i = 0; i < obj->spec_attr->బిడ్డ_cnt; i++) {
            dump_వృక్షం_core(lv_obj_get_బిడ్డ(obj, i), లోతు + 1);
        }
    }
}

స్థిర lv_obj_t * lv_obj_get_మొదటి_not_deleting_బిడ్డ(lv_obj_t * obj)
{
    LV_ASSERT(obj != ఖాళీ);

    ఒకవేళ(obj->spec_attr == ఖాళీ) తిరిగి ఖాళీ;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->బిడ్డ_cnt;
    కొరకు(i = 0; i < cnt; i++) {
        ఒకవేళ(!obj->spec_attr->children[i]->is_deleting) {
            తిరిగి obj->spec_attr->children[i];
        }
    }

    తిరిగి ఖాళీ;
}

#ఒకవేళ LV_USE_OBJ_NAME

స్థిర lv_obj_t * కనుగొను_by_name_ప్రత్యక్ష(స్థిరరాశి lv_obj_t * మాతృ, స్థిరరాశి అక్షరం * name, కొలత_t len)
{
    uint32_t i;
    uint32_t బిడ్డ_cnt = lv_obj_get_బిడ్డ_లెక్క(మాతృ);
    కొరకు(i = 0; i < బిడ్డ_cnt; i++) {
        lv_obj_t * బిడ్డ = మాతృ->spec_attr->children[i];

        అక్షరం బిడ్డ_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(బిడ్డ, బిడ్డ_name_resolved, పరిమాణం(బిడ్డ_name_resolved));

        ఒకవేళ(len == UINT16_MAX) {
            ఒకవేళ(lv_strcmp(బిడ్డ_name_resolved, name) == 0) తిరిగి బిడ్డ;
        }
        లేకపోతే {
            ఒకవేళ(lv_strncmp(బిడ్డ_name_resolved, name, len) == 0 &&
               బిడ్డ_name_resolved[len] == '\0') {
                తిరిగి బిడ్డ;
            }
        }
    }

    తిరిగి ఖాళీ;
}

#అయితే_ముగింపు /*LV_USE_OBJ_NAME*/
