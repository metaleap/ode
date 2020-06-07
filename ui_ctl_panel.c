#pragma once
#include "utils_libc_deps_basics.c"
#include "utils_libc_deps_mem.c"
#include "common.c"
#include "ui_ctl.c"

typedef enum OdeUiCtlPanelMode {
    ode_uictl_panel_none,
    ode_uictl_panel_tabs,
    ode_uictl_panel_orient,
} OdeUiCtlPanelMode;

typedef struct OdeUiCtlPanel {
    OdeUiCtl base;
    OdeUiCtls ctls;
    UInt ctl_idx;
    OdeOrientation orient;
    OdeUiCtlPanelMode mode;
} OdeUiCtlPanel;

OdeUiCtlPanel odeUiCtlPanel(OdeUiCtl const base, OdeOrientation orientation, OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    return (OdeUiCtlPanel) {
        .base = base, .ctl_idx = 0, .ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, ctls_capacity), .orient = orientation, .mode = mode};
}
