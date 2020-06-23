#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"
#include "ui_sidebars.c"
#include "ui_view_explorer.c"
#include "ui_view_extensions.c"
#include "ui_view_issues.c"
#include "ui_view_notifications.c"
#include "ui_view_outline.c"
#include "ui_view_output.c"
#include "ui_view_search.c"
#include "ui_view_terminal.c"

OdeUiCtl* odeUiView(OdeUiViewKind const kind, OdeUiSidebar* const preferred_sidebar) {
    OdeUiCtl* ret_ctl = NULL;
    OdeUiSidebar* sidebar = ode.ui.sidebar_bottom;
    ºUInt view_idx = odeUiSidebarIndexOfViewOfKind(sidebar, kind);
    if (view_idx.got)
        ret_ctl = sidebar->ui_panel.base.ctls.at[view_idx.it];
    else {
        sidebar = ode.ui.sidebar_right;
        view_idx = odeUiSidebarIndexOfViewOfKind(sidebar, kind);
        if (view_idx.got)
            ret_ctl = sidebar->ui_panel.base.ctls.at[view_idx.it];
        else {
            sidebar = ode.ui.sidebar_left;
            view_idx = odeUiSidebarIndexOfViewOfKind(sidebar, kind);
        }
    }
    if (!view_idx.got) {
#define ·newView(¹what_view__)                                                         \
    OdeUiView##¹what_view__* view_ptr = ·new(OdeUiView##¹what_view__, view_mem);       \
    *view_ptr = odeUiView##¹what_view__(view_mem);                                     \
    ret_ctl = (OdeUiCtl*)view_ptr;

        if (preferred_sidebar != NULL)
            sidebar = preferred_sidebar;

        MemHeap* view_mem = odeMem(1 * 1024 * 1024);
        switch (kind) {
            case ode_ui_view_extensions: {
                ·newView(Extensions);
            } break;
            case ode_ui_view_explorer: {
                ·newView(Explorer);
            } break;
            case ode_ui_view_issues: {
                ·newView(Issues)
            } break;
            case ode_ui_view_notifications: {
                ·newView(Notifications);
            } break;
            case ode_ui_view_outline: {
                ·newView(Outline)
            } break;
            case ode_ui_view_output: {
                ·newView(Output)
            } break;
            case ode_ui_view_search: {
                ·newView(Search)
            } break;
            case ode_ui_view_terminal: {
                ·newView(Terminal)
            } break;
            default: odeDie(strZ(uIntToStr(NULL, kind, 1, 10)), false);
        }

        static UInt counter = 0;
        counter += 1;
        ret_ctl->text = str2(NULL, ret_ctl->text, uIntToStr(NULL, counter, 1, 10));

        ret_ctl->view_kind = kind;
        view_idx = ·got(UInt, odeUiCtlPanelAppend(&sidebar->ui_panel, ret_ctl));
    }

    odeUiCtlPanelEnsureActiveTabIdx(&sidebar->ui_panel, view_idx.it);
    return ret_ctl;
}
