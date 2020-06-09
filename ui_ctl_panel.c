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
    UInt ctl_idx;
    OdeOrientation orient;
    OdeUiCtlPanelMode mode;
} OdeUiCtlPanel;

OdeRect odeRender(OdeUiCtl* const ctl, OdeRect const screen_rect);
void odeRenderText(Str const text, OdeRect const* const screen_rect, Bool const clear_full_line);

void odeUiCtlPanelOnRender(OdeUiCtl* ctl_panel, OdeRect* screen_rect) {
    OdeRect rect = *screen_rect;

    for (UInt i = 0; i < ctl_panel->ctls.len; i += 1) {
        OdeUiCtl* ctl = ctl_panel->ctls.at[i];
        ctl->parent = ctl_panel;
        if (ctl->visible) {
            if (ctl->dirty)
                rect = odeRender(ctl_panel->ctls.at[i], rect);
        }
        ctl->dirty = false;
    }
    if (ctl_panel->ctls.len == 0)
        odeRenderText(ctl_panel->text, &rect, true);
}

Bool odeUiCtlPanelOnInput(OdeUiCtl* ctl_panel, Str const bytes) {
    Bool dirty = false;
    for (UInt i = 0; i < ctl_panel->ctls.len; i += 1) {
        if (ctl_panel->ctls.at[i]->on.input != NULL)
            dirty |= ctl_panel->ctls.at[i]->on.input(ctl_panel->ctls.at[i], bytes);
        if (ctl_panel->ctls.at[i]->on.input != odeUiCtlPanelOnInput)
            dirty |= odeUiCtlPanelOnInput(ctl_panel->ctls.at[i], bytes);
    }
    return dirty;
}

OdeUiCtlPanel odeUiCtlPanel(OdeUiCtl base, OdeOrientation orientation, OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    base.on.render = odeUiCtlPanelOnRender;
    base.on.input = odeUiCtlPanelOnInput;
    base.ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, ctls_capacity);
    return (OdeUiCtlPanel) {.base = base, .ctl_idx = 0, .orient = orientation, .mode = mode};
}
