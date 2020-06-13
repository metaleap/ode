#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiSidebar {
    OdeUiCtlPanel base;
} OdeUiSidebar;

OdeUiSidebar odeUiSidebar(Str const text, UInt const size, OdeUiCtlDocking const dock, OdeOrientation const orient, UInt const ctls_cap) {
    ·assert(dock == ode_uictl_dock_left || dock == ode_uictl_dock_right || dock == ode_uictl_dock_bottom);
    return (OdeUiSidebar) {
        .base = odeUiCtlPanel(
            odeUiCtl(NULL, text, dock, rect(0, 0, (dock == ode_uictl_dock_bottom) ? 0 : size, (dock == ode_uictl_dock_bottom) ? size : 0)),
            orient, ode_uictl_panel_tabs, ctls_cap)};
}

void odeUiInitSidebarLeft() {
    OdeUiSidebar sidebar = odeUiSidebar(str("Left Sidebar"), 24, ode_uictl_dock_left, ode_orient_vert, 0);
    sidebar.base.base.color.bg = rgba(244, 211, 199, 0xff);
    ode.ui.sidebar_left = ·keep(OdeUiSidebar, NULL, &sidebar);
}

void odeUiInitSidebarRight() {
    OdeUiSidebar sidebar = odeUiSidebar(str("Right Sidebar"), 24, ode_uictl_dock_right, ode_orient_vert, 0);
    sidebar.base.base.color.bg = rgba(0x30, 0x20, 0x10, 0xff);
    ode.ui.sidebar_right = ·keep(OdeUiSidebar, NULL, &sidebar);
}

void odeUiInitSidebarBottom() {
    OdeUiSidebar sidebar = odeUiSidebar(str("Bottom Sidebar"), 9, ode_uictl_dock_bottom, ode_orient_horiz, 0);
    sidebar.base.base.color.bg = rgba(0x50, 0x40, 0x30, 0xff);
    sidebar.base.base.style = ode_glyphstyle_italic | ode_glyphstyle_bold;
    ode.ui.sidebar_bottom = ·keep(OdeUiSidebar, NULL, &sidebar);
}

void odeUiInitSidebars() {
    odeUiInitSidebarLeft();
    odeUiInitSidebarRight();
    odeUiInitSidebarBottom();
}
