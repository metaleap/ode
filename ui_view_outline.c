#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitViewOutline() {
    ode.ui.view_outline =
        odeUiCtlPanel(odeUiCtl(str("Outline"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.view_outline->base.color.bg = rgba(33, 55, 77, 255);
}
