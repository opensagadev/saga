#include "gameapi/edtools/edui.h"

struct edui_expander_s : eduiitem_s {
    u32 field_48;
    eduiitem_s *first_child;
    eduiitem_s *last_child;
    u32 open : 1;
    u32 unknown_flags : 31;
};

extern "C" {
    static void eduicbItemExpanderClose(edui_expander_s *expander) {
        for (eduiitem_s *item = expander->first_child; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
            if (item == expander->last_child)
                break;
        }
        if (expander->first_child && expander->open) {
            expander->next = expander->last_child->next;
            if (expander->last_child->next)
                expander->last_child->next->previous = expander;
            expander->first_child->previous = NULL;
            expander->open = 0;
        }
    }

    void eduicbMenuCloseAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->last; item; item = item->previous) {
            if (item->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(item));
        }
    }

    void eduicbMenuOpenAllexpanders(eduimenu_s *menu) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == EDUI_ITEM_EXPANDER) {
                edui_expander_s *expander = static_cast<edui_expander_s *>(item);
                if (expander->first_child && !expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    expander->first_child->previous = expander;
                    expander->next = expander->first_child;
                    expander->open = 1;
                }
            }
        }
    }
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu) {
    if (menu) {
        eduicbMenuCloseAllexpanders(menu);
        while (menu->first) {
            eduiitem_s *next = menu->first->next;
            menu->first->destroy(menu, menu->first);
            menu->first = next;
        }
        menu->last = NULL;
        menu->selected = NULL;
        menu->field_0c = NULL;
        menu->field_10 = NULL;
    }
}
