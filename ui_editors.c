#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiEditors {
    OdeUiCtlPanel base;
} OdeUiEditors;

void odeUiInitEditors() {
    OdeUiEditors editors = (OdeUiEditors) {.base = odeUiCtlPanel(odeUiCtl(NULL, str("Editors Area"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
                                                                 ode_orient_none, ode_uictl_panel_none, 0)};
    ode.ui.editors = ·keep(OdeUiEditors, NULL, &editors);
}
