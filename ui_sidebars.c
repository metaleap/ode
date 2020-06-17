#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiSidebar {
    OdeUiCtlPanel ui_panel;
} OdeUiSidebar;

OdeUiSidebar odeUiSidebar(Str const text, UInt const size, OdeUiCtlDocking const dock,
                          OdeOrientation const orient, UInt const ctls_cap) {
    ·assert(dock == ode_uictl_dock_left || dock == ode_uictl_dock_right
            || dock == ode_uictl_dock_bottom);
    return (OdeUiSidebar) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(NULL, text, dock,
                     rect(0, 0, (dock == ode_uictl_dock_bottom) ? 0 : size,
                          (dock == ode_uictl_dock_bottom) ? size : 0)),
            orient, ode_uictl_panel_tabs, ctls_cap)};
}

static void initSidebarLeft(UInt const ctls_cap) {
    OdeUiSidebar sidebar = odeUiSidebar(str("Left Sidebar"), 24, ode_uictl_dock_left,
                                        ode_orient_vert, ctls_cap);
    sidebar.ui_panel.base.color.bg = rgba(0x38, 0x34, 0x38, 0xff);
    ode.ui.sidebar_left = ·keep(OdeUiSidebar, NULL, &sidebar);
}

static void initSidebarRight(UInt const ctls_cap) {
    OdeUiSidebar sidebar = odeUiSidebar(str("Right Sidebar"), 24, ode_uictl_dock_right,
                                        ode_orient_vert, ctls_cap);
    sidebar.ui_panel.base.color.bg = rgba(0x28, 0x24, 0x28, 0xff);
    ode.ui.sidebar_right = ·keep(OdeUiSidebar, NULL, &sidebar);
}

static void initSidebarBottom(UInt const ctls_cap) {
    OdeUiSidebar sidebar = odeUiSidebar(str("Bottom Sidebar"), 7, ode_uictl_dock_bottom,
                                        ode_orient_horiz, ctls_cap);
    sidebar.ui_panel.base.color.bg = rgba(0x48, 0x44, 0x48, 0xff);
    sidebar.ui_panel.base.style = ode_glyphstyle_italic | ode_glyphstyle_bold;
    ode.ui.sidebar_bottom = ·keep(OdeUiSidebar, NULL, &sidebar);
}

void odeUiInitSidebars() {
    initSidebarLeft(16);
    initSidebarRight(16);
    initSidebarBottom(16);
}
