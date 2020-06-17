#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewOutline {
    OdeUiCtlPanel ui_panel;
} OdeUiViewOutline;

OdeUiViewOutline odeUiViewOutline(MemHeap* mem) {
    OdeUiViewOutline ret_view = (OdeUiViewOutline) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Outline"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(33, 55, 77, 255);
    return ret_view;
}
