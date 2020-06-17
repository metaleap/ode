#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewSearch {
    OdeUiCtlPanel ui_panel;
} OdeUiViewSearch;

OdeUiViewSearch odeUiViewSearch(MemHeap* mem) {
    OdeUiViewSearch ret_view = (OdeUiViewSearch) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Search"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(88, 66, 44, 255);
    return ret_view;
}
