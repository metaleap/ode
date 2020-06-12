#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitViewExplorer() {
    ode.ui.view_explorer =
        odeUiCtlPanel(odeUiCtl(str("Explorer"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.view_explorer->base.color.bg = rgba(77, 55, 33, 255);
}
