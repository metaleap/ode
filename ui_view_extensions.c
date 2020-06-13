#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewExtensions {
    OdeUiCtlPanel base;
} OdeUiViewExtensions;

// void odeUiInitViewExtensions() {
//     ode.ui.view_extensions =
//         odeUiCtlPanel(odeUiCtl(NULL, str("Extensions"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
//     ode.ui.view_extensions->base.color.bg = rgba(99, 77, 55, 255);
// }
