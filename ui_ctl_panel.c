#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
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

OdeUiCtlPanel odeUiCtlPanel(OdeUiCtl base, OdeOrientation orientation, OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    base.has_ctls = true;
    return (OdeUiCtlPanel) {
        .base = base, .ctl_idx = 0, .ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, ctls_capacity), .orient = orientation, .mode = mode};
}
