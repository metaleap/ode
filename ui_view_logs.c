#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewLogOutput {
    OdeUiCtlPanel ui_panel;
} OdeUiViewLogOutput;

// void odeUiInitViewLogs() {
//     ode.ui.view_logs =
//         odeUiCtlPanel(odeUiCtl(NULL, str("Logs"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
//     ode.ui.view_logs->base.color.bg = rgba(55, 77, 99, 255);
// }
