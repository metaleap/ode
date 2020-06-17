#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewTerminal {
    OdeUiCtlPanel ui_panel;
} OdeUiViewTerminal;

OdeUiViewTerminal odeUiViewTerminal(MemHeap* mem) {
    OdeUiViewTerminal ret_view = (OdeUiViewTerminal) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Terminals"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(33, 77, 55, 255);
    return ret_view;
}
