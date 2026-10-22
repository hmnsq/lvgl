/**
 * @file lv_obj_tree.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#समावेश "lv_obj_private.h"
#समावेश "../lvgl_public.h"
#समावेश "lv_obj_class_private.h"
#समावेश "../indev/lv_indev_private.h"
#समावेश "../display/lv_display_private.h"
#समावेश "../misc/lv_anim_private.h"
#समावेश "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#घोषणा MY_CLASS (&lv_obj_class)
#घोषणा disp_ll_p &(LV_GLOBAL_DEFAULT()->disp_ll)

#घोषणा OBJ_DUMP_STRING_LEN 128
#घोषणा LV_OBJ_NAME_MAX_LEN 128

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
अटल व्योम lv_obj_delete_async_cb(व्योम * obj);
अटल व्योम obj_delete_core(lv_obj_t * obj);
अटल lv_obj_वृक्ष_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्ष_walk_cb_t cb, व्योम * user_तथ्य);
अटल व्योम dump_वृक्ष_core(lv_obj_t * obj, int32_t depth);
अटल lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj);
#अगर LV_USE_OBJ_NAME
    अटल lv_obj_t * find_by_name_direct(स्थिर lv_obj_t * parent, स्थिर अक्षर * name, माप_प्रकार len);
#पूर्ण_अगर /*LV_USE_OBJ_NAME*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

व्योम lv_obj_delete(lv_obj_t * obj)
{
    अगर(obj == शून्य) वापस;
    अगर(obj->is_deleting) वापस;

    LV_LOG_TRACE("begin (delete %p)", (व्योम *)obj);
    lv_obj_invalidate(obj);

    lv_obj_t * par = lv_obj_get_parent(obj);

    lv_display_t * disp = शून्य;
    bool act_screen_del = false;
    अगर(par == शून्य) {
        disp = lv_obj_get_display(obj);
        अगर(!disp) वापस;   /*Shouldn't happen*/
        अगर(disp->act_scr == obj) act_screen_del = true;
    }

    obj_delete_core(obj);

    /*Call the ancestor's event handler to the parent to notify it about the child delete*/
    अगर(par && !par->is_deleting) {
        lv_obj_scrollbar_invalidate(par);
        lv_obj_send_event(par, LV_EVENT_CHILD_CHANGED, शून्य);
        lv_obj_send_event(par, LV_EVENT_CHILD_DELETED, शून्य);
    }

    /*Handle if the active screen was deleted*/
    अगर(act_screen_del) {
        LV_LOG_WARN("the active screen was deleted");
        disp->act_scr = शून्य;
    }

    LV_ASSERT_MEM_INTEGRITY();
    LV_LOG_TRACE("finished (delete %p)", (व्योम *)obj);
}

व्योम lv_obj_clean(lv_obj_t * obj)
{
    LV_LOG_TRACE("begin (clean %p)", (व्योम *)obj);
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    lv_obj_invalidate(obj);

    uint32_t cnt = lv_obj_get_child_गिनती(obj);
    lv_obj_t * child = lv_obj_get_first_not_deleting_child(obj);
    जबतक(child) {
        obj_delete_core(child);
        child = lv_obj_get_first_not_deleting_child(obj);
    }
    /*Just to remove scroll animations if any*/
    lv_obj_scroll_to(obj, 0, 0, LV_ANIM_OFF);
    अगर(obj->spec_attr) {
        obj->spec_attr->scroll.x = 0;
        obj->spec_attr->scroll.y = 0;
    }

    अगर(lv_obj_get_child_गिनती(obj) < cnt) {
        lv_obj_send_event(obj, LV_EVENT_CHILD_CHANGED, शून्य);
        lv_obj_send_event(obj, LV_EVENT_CHILD_DELETED, शून्य);
    }

    LV_ASSERT_MEM_INTEGRITY();

    LV_LOG_TRACE("finished (clean %p)", (व्योम *)obj);
}

व्योम lv_obj_delete_delayed(lv_obj_t * obj, uint32_t delay_ms)
{
    अगर(obj == शून्य) वापस;

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, शून्य);
    lv_anim_set_duration(&a, 1);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);
}

व्योम lv_obj_delete_anim_completed_cb(lv_anim_t * a)
{
    LV_CHECK_ARG(a != शून्य, वापस);

    lv_obj_delete(a->var);
}

व्योम lv_obj_delete_async(lv_obj_t * obj)
{
    अगर(obj == शून्य) वापस;

    lv_async_call(lv_obj_delete_async_cb, obj);
}

व्योम lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_OBJ(parent, MY_CLASS, वापस);

    अगर(obj->parent == शून्य) {
        LV_LOG_WARN("Can't set the parent of a screen");
        वापस;
    }

    अगर(parent == obj->parent) {
        वापस;
    }

    अगर(!lv_obj_allocate_spec_attr(parent)) {
        वापस;
    }

    /*Add the object to the new parents list first*/
    अगर(lv_obj_add_child(parent, obj) != LV_परिणाम_OK) {
        LV_LOG_WARN("Failed to attach child to parent");
        वापस;
    }

    /*Remove the object from the old parent's child list
     * This should never fail, thus we do only do it after adding it to the new parent's list*/
    lv_obj_remove_child(obj->parent, obj);

    /*Notify the original parent because one of its children is lost*/
    lv_obj_scrollbar_invalidate(obj->parent);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(obj->parent, LV_EVENT_CHILD_DELETED, शून्य);
    obj->parent = parent;

    /*Notify the new parent about the child*/
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, शून्य);

    lv_obj_mark_layout_as_dirty(obj);

    lv_obj_invalidate(obj);
}

व्योम lv_obj_move_to_सूचकांक(lv_obj_t * obj, int32_t सूचकांक)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    /* Check parent validity */
    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(!parent) {
        LV_LOG_WARN("parent is NULL");
        वापस;
    }

    स्थिर uint32_t parent_child_गिनती = lv_obj_get_child_गिनती(parent);
    /* old_index only can be 0 or greater, this point cannot be reached if the parent is not null */
    स्थिर int32_t old_सूचकांक = lv_obj_get_सूचकांक(obj);
    LV_ASSERT(0 <= old_सूचकांक);

    अगर(सूचकांक < 0) {
        सूचकांक += parent_child_गिनती;
    }

    /* Index was negative and the absolute value is greater than parent child count */
    अगर((सूचकांक < 0)
       /* Index is same or bigger than parent child count */
       || (सूचकांक >= (int32_t) parent_child_गिनती)
       /* If both previous and new index are the same */
       || (सूचकांक == old_सूचकांक)) {

        वापस;
    }

    int32_t i = old_सूचकांक;
    अगर(सूचकांक < old_सूचकांक) {
        जबतक(i > सूचकांक)  {
            parent->spec_attr->children[i] = parent->spec_attr->children[i - 1];
            i--;
        }
    }
    अन्यथा {
        जबतक(i < सूचकांक) {
            parent->spec_attr->children[i] = parent->spec_attr->children[i + 1];
            i++;
        }
    }

    parent->spec_attr->children[सूचकांक] = obj;
    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, शून्य);
    lv_obj_invalidate(parent);
}

व्योम lv_obj_swap(lv_obj_t * obj1, lv_obj_t * obj2)
{
    LV_CHECK_OBJ(obj1, MY_CLASS, वापस);
    LV_CHECK_OBJ(obj2, MY_CLASS, वापस);

    lv_obj_t * parent = lv_obj_get_parent(obj1);
    lv_obj_t * parent2 = lv_obj_get_parent(obj2);

    uint_fast32_t index1 = lv_obj_get_सूचकांक(obj1);
    uint_fast32_t index2 = lv_obj_get_सूचकांक(obj2);

    lv_obj_send_event(parent2, LV_EVENT_CHILD_DELETED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_DELETED, obj1);

    parent->spec_attr->children[index1] = obj2;
    obj2->parent = parent;

    parent2->spec_attr->children[index2] = obj1;
    obj1->parent = parent2;

    lv_obj_send_event(parent, LV_EVENT_CHILD_CHANGED, obj2);
    lv_obj_send_event(parent, LV_EVENT_CHILD_CREATED, obj2);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CHANGED, obj1);
    lv_obj_send_event(parent2, LV_EVENT_CHILD_CREATED, obj1);

    lv_obj_invalidate(parent);

    अगर(parent != parent2) {
        lv_obj_invalidate(parent2);
    }
    lv_group_swap_obj(obj1, obj2);
}

lv_obj_t * lv_obj_get_screen(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

    स्थिर lv_obj_t * par = obj;
    स्थिर lv_obj_t * act_par;

    करो {
        act_par = par;
        par = lv_obj_get_parent(act_par);
    } जबतक(par != शून्य);

    वापस (lv_obj_t *)act_par;
}

lv_display_t * lv_obj_get_display(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

    स्थिर lv_obj_t * scr;

    अगर(obj->parent == शून्य) scr = obj;  /*`obj` is a screen*/
    अन्यथा scr = lv_obj_get_screen(obj);  /*get the screen of `obj`*/

    lv_display_t * d;
    lv_ll_t * disp_शीर्ष = disp_ll_p;
    LV_LL_READ(disp_शीर्ष, d) {
        uint32_t i;
        क्रम(i = 0; i < d->screen_cnt; i++) {
            अगर(d->screens[i] == scr) वापस d;
        }
    }

    LV_LOG_WARN("No screen found");
    वापस शून्य;
}

lv_obj_t * lv_obj_get_parent(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

    वापस obj->parent;
}

lv_obj_t * lv_obj_get_child(स्थिर lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

    अगर(obj->spec_attr == शून्य) वापस शून्य;

    अगर(idx < 0) {
        idx = obj->spec_attr->child_cnt + idx;
        अगर(idx < 0) वापस शून्य;
    }

    uint32_t idu = (uint32_t) idx;

    अगर(idu < obj->spec_attr->child_cnt) वापस obj->spec_attr->children[idu];
    अन्यथा वापस शून्य;
}

lv_obj_t * lv_obj_get_child_by_type(स्थिर lv_obj_t * obj, int32_t idx, स्थिर lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);
    LV_CHECK_ARG(class_p != शून्य, वापस शून्य);

    अगर(obj->spec_attr == शून्य) वापस शून्य;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    अगर(idx >= 0) {
        क्रम(i = 0; i < cnt; i++) {
            अगर(obj->spec_attr->children[i]->class_p == class_p) {
                अगर(idx == 0) वापस obj->spec_attr->children[i];
                अन्यथा idx--;
            }
        }
    }
    अन्यथा {
        idx++;   /*-1 means the first child*/
        क्रम(i = cnt - 1; i >= 0; i--) {
            अगर(obj->spec_attr->children[i]->class_p == class_p) {
                अगर(idx == 0) वापस obj->spec_attr->children[i];
                अन्यथा idx++;
            }
        }
    }
    वापस शून्य;
}

lv_obj_t * lv_obj_get_sibling(स्थिर lv_obj_t * obj, int32_t idx)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);
    LV_CHECK_ARG(obj->parent != शून्य, वापस शून्य);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_सूचकांक(obj) + idx;
    अगर(sibling_idx < 0) वापस शून्य;

    वापस lv_obj_get_child(parent, sibling_idx);
}

lv_obj_t * lv_obj_get_sibling_by_type(स्थिर lv_obj_t * obj, int32_t idx, स्थिर lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);
    LV_CHECK_ARG(obj->parent != शून्य, वापस शून्य);
    LV_CHECK_ARG(class_p != शून्य, वापस शून्य);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    int32_t sibling_idx = (int32_t)lv_obj_get_सूचकांक_by_type(obj, class_p) + idx;
    अगर(sibling_idx < 0) वापस शून्य;

    वापस lv_obj_get_child_by_type(parent, sibling_idx, class_p);
}

uint32_t lv_obj_get_child_गिनती(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);
    अगर(obj->spec_attr == शून्य) वापस 0;
    वापस obj->spec_attr->child_cnt;
}

uint32_t lv_obj_get_child_गिनती_by_type(स्थिर lv_obj_t * obj, स्थिर lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस 0);
    LV_CHECK_ARG(class_p != शून्य, वापस 0);

    अगर(obj->spec_attr == शून्य) वापस 0;

    uint32_t i;
    uint32_t cnt = 0;
    क्रम(i = 0; i < obj->spec_attr->child_cnt; i++) {
        अगर(obj->spec_attr->children[i]->class_p == class_p) cnt++;
    }
    वापस cnt;
}

#अगर LV_USE_OBJ_NAME

व्योम lv_obj_set_name(lv_obj_t * obj, स्थिर अक्षर * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }

    अगर(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((व्योम *)obj->spec_attr->name);

    अगर(name == शून्य) {
        obj->spec_attr->name = शून्य;
        obj->spec_attr->name_static = 1;
    }
    अन्यथा {
        obj->spec_attr->name = lv_strdup(name);
        obj->spec_attr->name_static = 0;
    }
}

व्योम lv_obj_set_name_static(lv_obj_t * obj, स्थिर अक्षर * name)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(name != शून्य, वापस);

    अगर(!lv_obj_allocate_spec_attr(obj)) {
        वापस;
    }
    अगर(!obj->spec_attr->name_static && obj->spec_attr->name) lv_free((व्योम *)obj->spec_attr->name);

    obj->spec_attr->name = name;
    obj->spec_attr->name_static = 1;
}

स्थिर अक्षर * lv_obj_get_name(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस शून्य);

    अगर(obj->spec_attr == शून्य) वापस शून्य;
    अन्यथा वापस obj->spec_attr->name;
}

व्योम lv_obj_get_name_resolved(स्थिर lv_obj_t * obj, अक्षर buf[], माप_प्रकार buf_आकार)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस);
    LV_CHECK_ARG(buf != शून्य && buf_आकार > 0, वापस);

    स्थिर अक्षर * name = lv_obj_get_name(obj);
    /*Use a default name which auto-indexing*/
    अक्षर name_buf[LV_OBJ_NAME_MAX_LEN];
    अगर(name == शून्य) {
        lv_snprintf(name_buf, माप(name_buf), "%s_#", obj->class_p->name);
        name = name_buf;
    }

    माप_प्रकार name_len = lv_strlen(name);
    lv_obj_t * parent = lv_obj_get_parent(obj);

    /*If the last character is # automatically index the children with the same name start*/
    अगर(parent && name_len > 0 && name[name_len - 1] == '#') {
        uint32_t child_cnt = lv_obj_get_child_गिनती(parent);
        uint32_t cnt = 0;
        uint32_t i;
        क्रम(i = 0; i < child_cnt; i++) {
            lv_obj_t * child = lv_obj_get_child(parent, i);
            /*All siblings older siblings are checked, craft the name of this widget*/
            अगर(child == obj) {
                अक्षर अंक_buf[8];
                माप_प्रकार अंक_len;
                अंक_len = lv_snprintf(अंक_buf, माप(अंक_buf), "%d", cnt);
                /*Is there enough space for the name and the index?*/
                अगर(buf_आकार > name_len + अंक_len) {
                    /*E.g. buf = "some_name_", so trim the # from the end*/
                    lv_strncpy(buf, name, name_len - 1);
                    lv_strcpy(&buf[name_len - 1], अंक_buf);
                }
                अन्यथा {
                    /*Use the name as it is as a fallback*/
                    lv_strlcpy(buf, name, buf_आकार);
                }
                अवरोध;
            }
            /*Check the older siblings. IF they start with the same name count them*/
            अन्यथा {
                स्थिर अक्षर * child_name = lv_obj_get_name(child);
                अगर(child_name == शून्य) {
                    /*If the name we are looking for start with the child's class name
                     *increment the index. E.g. <class_name>_#*/
                    माप_प्रकार class_name_len = lv_strlen(child->class_p->name);
                    अगर(name_len > 3 && class_name_len == name_len - 2 &&
                       lv_strncmp(child->class_p->name, name, class_name_len) == 0) {
                        cnt++;
                    }
                }
                /*The name is set, check if it's e.g. <some_name>#*/
                अन्यथा {
                    अगर(lv_strcmp(child->spec_attr->name, name) == 0) {
                        cnt++;
                    }
                }
            }
        }
    }
    अन्यथा {
        /*Just use the set name*/
        lv_strlcpy(buf, name, buf_आकार);
    }
}

lv_obj_t * lv_obj_get_child_by_name(स्थिर lv_obj_t * parent, स्थिर अक्षर * path)
{
    LV_CHECK_OBJ(parent, MY_CLASS, वापस शून्य);
    LV_CHECK_ARG(path != शून्य, वापस शून्य);
    LV_CHECK_ARG(parent->spec_attr != शून्य, वापस शून्य);

    जबतक(*path) {
        स्थिर अक्षर * segment = path;
        uint32_t len = 0;

        /* Calculate the length of the current segment */
        जबतक(path[len] && path[len] != '/')
            len++;

        /* Look for a child whose resolved name exactly matches the segment */
        lv_obj_t * child = find_by_name_direct(parent, segment, len);
        अगर(!child) वापस शून्य; /*Segment not found*/

        /* Advance to the next segment */
        path += len;
        अगर(*path == '/') path++; /* Skip the '/' */

        /* If there is no further segment, we've found the target child */
        अगर(*path == '\0') वापस child;

        parent = child;
    }

    वापस शून्य;

}

lv_obj_t * lv_obj_find_by_name(स्थिर lv_obj_t * parent, स्थिर अक्षर * name)
{
    LV_CHECK_ARG(name != शून्य, वापस शून्य);

    अगर(parent == शून्य) parent = lv_display_get_screen_active(lv_display_get_default());
    अगर(parent == शून्य) वापस शून्य;

    lv_obj_t * child = find_by_name_direct(parent, name, UINT16_MAX);
    अगर(child) वापस child;

    /*Search children recursively*/
    uint32_t child_cnt = lv_obj_get_child_गिनती(parent);
    uint32_t i;
    क्रम(i = 0; i < child_cnt; i++) {
        child = parent->spec_attr->children[i];
        lv_obj_t * found = lv_obj_find_by_name(child, name);
        अगर(found != शून्य) वापस found;
    }

    वापस शून्य;
}

#पूर्ण_अगर /*LV_USE_OBJ_NAME*/

int32_t lv_obj_get_सूचकांक(स्थिर lv_obj_t * obj)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent == शून्य) वापस -1;

    uint32_t i = 0;
    क्रम(i = 0; i < parent->spec_attr->child_cnt; i++) {
        अगर(parent->spec_attr->children[i] == obj) वापस (int32_t)i;
    }

    /*Shouldn't reach this point*/
    LV_ASSERT(0);
    वापस -1;
}

int32_t lv_obj_get_सूचकांक_by_type(स्थिर lv_obj_t * obj, स्थिर lv_obj_class_t * class_p)
{
    LV_CHECK_OBJ(obj, MY_CLASS, वापस -1);
    LV_CHECK_ARG(class_p != शून्य, वापस -1);

    lv_obj_t * parent = lv_obj_get_parent(obj);
    अगर(parent == शून्य) वापस 0xFFFFFFFF;

    uint32_t i = 0;
    uint32_t idx = 0;
    क्रम(i = 0; i < parent->spec_attr->child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];
        अगर(child->class_p == class_p) {
            अगर(child == obj) वापस idx;
            idx++;
        }
    }

    /*Can happen if there was no children with the given type*/
    वापस -1;
}

व्योम lv_obj_वृक्ष_walk(lv_obj_t * start_obj, lv_obj_वृक्ष_walk_cb_t cb, व्योम * user_तथ्य)
{
    LV_CHECK_ARG(cb != शून्य, वापस);

    walk_core(start_obj, cb, user_तथ्य);
}

व्योम lv_obj_dump_वृक्ष(lv_obj_t * start_obj)
{
    अगर(start_obj == शून्य) {
        lv_display_t * disp = lv_display_get_अग्र(शून्य);
        जबतक(disp) {
            uint32_t i;
            क्रम(i = 0; i < disp->screen_cnt; i++) {
                dump_वृक्ष_core(disp->screens[i], 0);
            }
            disp = lv_display_get_अग्र(disp);
        }
    }
    अन्यथा {
        dump_वृक्ष_core(start_obj, 0);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

अटल व्योम lv_obj_delete_async_cb(व्योम * obj)
{
    lv_obj_delete(obj);
}

अटल व्योम obj_indev_reset(lv_indev_t * indev, lv_obj_t * obj)
{
    /* If the input device is already in the release state,
     * there is no need to wait for the input device to be released
     */
    अगर(lv_indev_get_state(indev) != LV_INDEV_STATE_RELEASED) {
        /*Wait for release to avoid accidentally triggering other obj to be clicked*/
        lv_indev_wait_release(indev);
    }

    /*Reset the input device*/
    lv_indev_reset(indev, obj);
}

अटल व्योम obj_delete_core(lv_obj_t * obj)
{
    LV_ASSERT(obj != शून्य);
    अगर(obj->is_deleting)
        वापस;

    obj->is_deleting = true;

    /*Let the user free the resources used in `LV_EVENT_DELETE`*/
    lv_परिणाम_t res = lv_obj_send_event(obj, LV_EVENT_DELETE, शून्य);
    अगर(res == LV_परिणाम_INVALID) {
        obj->is_deleting = false;
        वापस;
    }

    /*Release the blur bookkeeping now: the style teardown below doesn't go
     *through lv_obj_update_blur_status()*/
    अगर(obj->has_blur) {
        obj->has_blur = 0;
        LV_ASSERT(LV_GLOBAL_DEFAULT()->blur_obj_cnt > 0);
        LV_GLOBAL_DEFAULT()->blur_obj_cnt--;
    }

    /*Clean registered event_cb*/
    अगर(obj->spec_attr) lv_event_remove_all(&(obj->spec_attr->event_list));

    /*Recursively delete the children*/
    lv_obj_t * child = lv_obj_get_child(obj, 0);
    जबतक(child) {
        obj_delete_core(child);
        child = lv_obj_get_child(obj, 0);
    }

    lv_group_t * group = lv_obj_get_group(obj);

    /*Reset all input devices if the object to delete is used*/
    lv_indev_t * indev = lv_indev_get_अग्र(शून्य);
    जबतक(indev) {
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        अगर(indev_type == LV_INDEV_TYPE_POINTER || indev_type == LV_INDEV_TYPE_BUTTON) {
            अगर(indev->pointer.act_obj == obj || indev->pointer.scroll_obj == obj) {
                obj_indev_reset(indev, obj);
            }
            अगर(indev->pointer.last_pressed == obj) {
                indev->pointer.last_pressed = शून्य;
            }
            अगर(indev->pointer.last_hovered == obj) {
                indev->pointer.last_hovered = शून्य;
            }
        }

        अगर(indev->group == group && obj == lv_indev_get_active_obj()) {
            obj_indev_reset(indev, obj);
        }
        indev = lv_indev_get_अग्र(indev);
    }

    /*Delete all pending async del-s*/
    lv_परिणाम_t async_cancel_res = LV_परिणाम_OK;
    जबतक(async_cancel_res == LV_परिणाम_OK) {
        async_cancel_res = lv_async_call_cancel(lv_obj_delete_async_cb, obj);
    }

    /*All children deleted. Now clean up the object specific data*/
    lv_obj_destruct(obj);

    /*Remove the screen for the screen list*/
    अगर(obj->parent == शून्य) {
        lv_display_t * disp = lv_obj_get_display(obj);
        uint32_t i;
        /*Find the screen in the list*/
        क्रम(i = 0; i < disp->screen_cnt; i++) {
            अगर(disp->screens[i] == obj) अवरोध;
        }

        uint32_t id = i;
        क्रम(i = id; i < disp->screen_cnt - 1; i++) {
            disp->screens[i] = disp->screens[i + 1];
        }
        disp->screen_cnt--;
        lv_obj_t ** new_screens = lv_realloc(disp->screens, disp->screen_cnt * माप(lv_obj_t *));
        अगर(new_screens != शून्य) {
            disp->screens = new_screens;
        }
        अन्यथा {
            LV_LOG_INFO("Failed to shrink the screen array");
        }
    }
    /*Remove the object from the child list of its parent*/
    अन्यथा {
        lv_obj_remove_child(obj->parent, obj);
    }

    obj->parent = शून्य;

    /*Free the object itself*/
    lv_free(obj);
}

अटल lv_obj_वृक्ष_walk_res_t walk_core(lv_obj_t * obj, lv_obj_वृक्ष_walk_cb_t cb, व्योम * user_तथ्य)
{
    lv_obj_वृक्ष_walk_res_t res = LV_OBJ_वृक्ष_WALK_अग्र;

    अगर(obj == शून्य) {
        lv_display_t * disp = lv_display_get_अग्र(शून्य);
        जबतक(disp) {
            uint32_t i;
            क्रम(i = 0; i < disp->screen_cnt; i++) {
                walk_core(disp->screens[i], cb, user_तथ्य);
            }
            disp = lv_display_get_अग्र(disp);
        }
        वापस LV_OBJ_वृक्ष_WALK_END;    /*The value doesn't matter as it wasn't called recursively*/
    }

    res = cb(obj, user_तथ्य);

    अगर(res == LV_OBJ_वृक्ष_WALK_END) वापस LV_OBJ_वृक्ष_WALK_END;

    अगर(res != LV_OBJ_वृक्ष_WALK_SKIP_CHILDREN) {
        uint32_t i;
        क्रम(i = 0; i < lv_obj_get_child_गिनती(obj); i++) {
            res = walk_core(lv_obj_get_child(obj, i), cb, user_तथ्य);
            अगर(res == LV_OBJ_वृक्ष_WALK_END) वापस LV_OBJ_वृक्ष_WALK_END;
        }
    }
    वापस LV_OBJ_वृक्ष_WALK_अग्र;
}

अटल व्योम dump_वृक्ष_core(lv_obj_t * obj, int32_t depth)
{
    LV_ASSERT(obj != शून्य);
#अगर LV_USE_LOG
    स्थिर अक्षर * id;

#अगर LV_USE_OBJ_ID
    अक्षर buf[OBJ_DUMP_STRING_LEN];
    id = lv_obj_stringify_id(obj, buf, माप(buf));
    अगर(id == शून्य) id = "obj0";
#अन्यथा
    id = "obj0";
#पूर्ण_अगर

    /*id of `obj0` is an invalid id for builtin id*/
    LV_LOG_USER("%*sobj:%p, id:%s;", (पूर्णांक)(2 * depth), "", (व्योम *)obj, id);
#पूर्ण_अगर /*LV_USE_LOG*/

    अगर(obj->spec_attr && obj->spec_attr->child_cnt) {
        क्रम(uint32_t i = 0; i < obj->spec_attr->child_cnt; i++) {
            dump_वृक्ष_core(lv_obj_get_child(obj, i), depth + 1);
        }
    }
}

अटल lv_obj_t * lv_obj_get_first_not_deleting_child(lv_obj_t * obj)
{
    LV_ASSERT(obj != शून्य);

    अगर(obj->spec_attr == शून्य) वापस शून्य;

    int32_t i;
    int32_t cnt = (int32_t)obj->spec_attr->child_cnt;
    क्रम(i = 0; i < cnt; i++) {
        अगर(!obj->spec_attr->children[i]->is_deleting) {
            वापस obj->spec_attr->children[i];
        }
    }

    वापस शून्य;
}

#अगर LV_USE_OBJ_NAME

अटल lv_obj_t * find_by_name_direct(स्थिर lv_obj_t * parent, स्थिर अक्षर * name, माप_प्रकार len)
{
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_गिनती(parent);
    क्रम(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = parent->spec_attr->children[i];

        अक्षर child_name_resolved[LV_OBJ_NAME_MAX_LEN];
        lv_obj_get_name_resolved(child, child_name_resolved, माप(child_name_resolved));

        अगर(len == UINT16_MAX) {
            अगर(lv_strcmp(child_name_resolved, name) == 0) वापस child;
        }
        अन्यथा {
            अगर(lv_strncmp(child_name_resolved, name, len) == 0 &&
               child_name_resolved[len] == '\0') {
                वापस child;
            }
        }
    }

    वापस शून्य;
}

#पूर्ण_अगर /*LV_USE_OBJ_NAME*/
