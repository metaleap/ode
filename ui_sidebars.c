#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitSidebarLeft() {
    ode.ui.sidebar_left =
        odeUiCtlPanel(odeUiCtl(str("Left Sidebar"), ode_uictl_dock_left, rect(0, 0, 24, 0)), ode_orient_horiz, ode_uictl_panel_tabs, 3);
    ode.ui.sidebar_left->base.color.bg = rgba(244, 211, 199, 0xff);
    ·append(ode.ui.sidebar_left->base.ctls, &ode.ui.view_explorer->base);
    ·append(ode.ui.sidebar_left->base.ctls, &ode.ui.view_search->base);
    ·append(ode.ui.sidebar_left->base.ctls, &ode.ui.view_extensions->base);
}

void odeUiInitSidebarRight() {
    ode.ui.sidebar_right =
        odeUiCtlPanel(odeUiCtl(str("Right Sidebar"), ode_uictl_dock_right, rect(0, 0, 24, 0)), ode_orient_horiz, ode_uictl_panel_tabs, 2);
    ode.ui.sidebar_right->base.color.bg = rgba(0x30, 0x20, 0x10, 0xff);
    ·append(ode.ui.sidebar_right->base.ctls, &ode.ui.view_outline->base);
    ·append(ode.ui.sidebar_right->base.ctls, &ode.ui.view_logs->base);
}

void odeUiInitSidebarBottom() {
    ode.ui.sidebar_bottom =
        odeUiCtlPanel(odeUiCtl(str("Bottom Sidebar"), ode_uictl_dock_bottom, rect(0, 0, 0, 9)), ode_orient_horiz, ode_uictl_panel_tabs, 2);
    ode.ui.sidebar_bottom->base.color.bg = rgba(0x50, 0x40, 0x30, 0xff);
    ode.ui.sidebar_bottom->base.style = ode_glyphstyle_italic | ode_glyphstyle_bold;
    ·append(ode.ui.sidebar_bottom->base.ctls, &ode.ui.view_diags->base);
    ·append(ode.ui.sidebar_bottom->base.ctls, &ode.ui.view_terminals->base);
}

void odeUiInitSidebars() {
    odeUiInitSidebarLeft();
    odeUiInitSidebarRight();
    odeUiInitSidebarBottom();
}
