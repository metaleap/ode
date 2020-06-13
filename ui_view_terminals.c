#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewTerminal {
    OdeUiCtlPanel ui_panel;
} OdeUiViewTerminal;

// void odeUiInitViewTerminals() {
//     ode.ui.view_terminals =
//         odeUiCtlPanel(odeUiCtl(NULL, str("Terminals"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
//     ode.ui.view_terminals->base.color.bg = rgba(33, 77, 55, 255);
// }
