#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitViewSearch() {
    ode.ui.view_search =
        odeUiCtlPanel(odeUiCtl(str("Search"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.view_search->base.color.bg = rgba(88, 66, 44, 255);
}
