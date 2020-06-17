#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiViewExtensions {
    OdeUiCtlPanel ui_panel;
} OdeUiViewExtensions;

OdeUiViewExtensions odeUiViewExtensions(MemHeap* mem) {
    OdeUiViewExtensions ret_view = (OdeUiViewExtensions) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(mem, str("Extensions"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 0)};
    ret_view.ui_panel.base.color.bg = rgba(99, 77, 55, 255);
    return ret_view;
}
