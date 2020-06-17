#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewIssues {
    OdeUiCtlPanel ui_panel;
} OdeUiViewIssues;

OdeUiViewIssues odeUiViewIssues(MemHeap* mem) {
    OdeUiViewIssues ret_view = (OdeUiViewIssues) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Issues"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(55, 99, 77, 255);
    return ret_view;
}
