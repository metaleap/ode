#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewLogOutput {
    OdeUiCtlPanel ui_panel;
} OdeUiViewLogOutput;

OdeUiViewLogOutput odeUiViewLogOutput(MemHeap* mem) {
    OdeUiViewLogOutput ret_view = (OdeUiViewLogOutput) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Logs"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(55, 77, 99, 255);
    return ret_view;
}
