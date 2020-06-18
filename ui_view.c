#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_sidebars.c"
#include "ui_view_explorer.c"
#include "ui_view_extensions.c"
#include "ui_view_issues.c"
#include "ui_view_notifications.c"
#include "ui_view_outline.c"
#include "ui_view_output.c"
#include "ui_view_search.c"
#include "ui_view_terminal.c"

OdeUiCtl* odeUiView(OdeUiViewKind const kind) {
    OdeUiCtl* ret_ctl = odeUiSidebarExistingViewOfKind(ode.ui.sidebar_bottom, kind);
    if (ret_ctl == NULL)
        ret_ctl = odeUiSidebarExistingViewOfKind(ode.ui.sidebar_left, kind);
    if (ret_ctl == NULL)
        ret_ctl = odeUiSidebarExistingViewOfKind(ode.ui.sidebar_right, kind);

    if (ret_ctl == NULL) {
#define ·newView(¹what_view__)                                                         \
    OdeUiView##¹what_view__* view_ptr = ·new(OdeUiView##¹what_view__, view_mem);       \
    *view_ptr = odeUiView##¹what_view__(view_mem);                                     \
    ret_ctl = (OdeUiCtl*)view_ptr;

        MemHeap* view_mem = odeMem(1 * 1024 * 1024);
        OdeUiSidebar* sidebar = ode.ui.sidebar_left;
        switch (kind) {
            case ode_uiview_extensions: {
                ·newView(Extensions);
            } break;
            case ode_uiview_explorer: {
                ·newView(Explorer);
            } break;
            case ode_uiview_issues: {
                sidebar = ode.ui.sidebar_bottom;
                ·newView(Issues)
            } break;
            case ode_uiview_notifications: {
                sidebar = ode.ui.sidebar_right;
                ·newView(Notifications);
            } break;
            case ode_uiview_outline: {
                sidebar = ode.ui.sidebar_right;
                ·newView(Outline)
            } break;
            case ode_uiview_output: {
                sidebar = ode.ui.sidebar_right;
                ·newView(Output)
            } break;
            case ode_uiview_search: {
                ·newView(Search)
            } break;
            case ode_uiview_terminal: {
                sidebar = ode.ui.sidebar_bottom;
                ·newView(Terminal)
            } break;
            default: odeDie(strZ(uIntToStr(NULL, kind, 1, 10)), false);
        }
        ·append(sidebar->ui_panel.base.ctls, ret_ctl);
        sidebar->ui_panel.ctl_idx = sidebar->ui_panel.base.ctls.len - 1;
        odeUiCtlSetDirty(&sidebar->ui_panel.base, true, true);
    }
    return ret_ctl;
}
