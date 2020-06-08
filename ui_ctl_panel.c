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

OdeRect odeRender(OdeUiCtl* const ctl, OdeRect const screen_rect);
void odeRenderText(Str const text, OdeRect const* const screen_rect);

static void onRender(OdeUiCtl* ctl_panel, OdeRect* screen_rect) {
    OdeUiCtlPanel* const this = (OdeUiCtlPanel*)ctl_panel;
    OdeRect rect = *screen_rect;
    for (UInt i = 0; i < this->ctls.len; i += 1)
        rect = odeRender(this->ctls.at[i], rect);
    odeRenderText(ctl_panel->text, &rect);
}

OdeUiCtlPanel odeUiCtlPanel(OdeUiCtl base, OdeOrientation orientation, OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    base.has_ctls = true;
    base.on.render = onRender;
    return (OdeUiCtlPanel) {
        .base = base, .ctl_idx = 0, .ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, ctls_capacity), .orient = orientation, .mode = mode};
}
