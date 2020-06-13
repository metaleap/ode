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
    OdeUiCtl* tab_bar;
    UInt ctl_idx;
    OdeOrientation orient;
    OdeUiCtlPanelMode mode;
} OdeUiCtlPanel;

OdeRect odeUiCtlPanelRenderChild(OdeUiCtl* ctl_panel, OdeUiCtl* ctl, OdeRect dst_rect) {
    ctl->parent = ctl_panel;
    if (ctl->visible)
        dst_rect = odeRender(ctl, dst_rect);
    ctl->dirty = false;
    return dst_rect;
}

void odeUiCtlPanelOnRender(OdeUiCtl* ctl_panel, OdeRect* screen_rect) {
    OdeRect dst_rect = *screen_rect;
    OdeUiCtlPanel* panel = (OdeUiCtlPanel*)ctl_panel;

    if (panel->mode == ode_uictl_panel_none)
        for (UInt i = 0; i < ctl_panel->ctls.len; i += 1)
            dst_rect = odeUiCtlPanelRenderChild(ctl_panel, ctl_panel->ctls.at[i], dst_rect);
    else if (panel->mode == ode_uictl_panel_tabs) {
        if (panel->tab_bar != NULL)
            dst_rect = odeUiCtlPanelRenderChild(ctl_panel, panel->tab_bar, dst_rect);
        dst_rect = odeUiCtlPanelRenderChild(ctl_panel, ctl_panel->ctls.at[panel->ctl_idx], dst_rect);
    } else if (ctl_panel->ctls.len > 0)
        odeDie(strZ(str4(str("TODO: "), uIntToStr(panel->mode, 1, 10), str(" "), panel->base.text)), false);

    // temp render logic: ctl.text if no other content
    if (ctl_panel->ctls.len == 0) {
        odeScreenClearRectText(&dst_rect);
        odeRenderText(ctl_panel->text, &dst_rect, false);
    }
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

OdeUiCtlPanel* odeUiCtlPanel(OdeUiCtl base, OdeOrientation orientation, OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    OdeUiCtlPanel* ret_panel = ·new(OdeUiCtlPanel, NULL);
    base.on.render = odeUiCtlPanelOnRender;
    base.on.input = odeUiCtlPanelOnInput;
    base.ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, ctls_capacity);
    *ret_panel = (OdeUiCtlPanel) {.base = base, .tab_bar = NULL, .ctl_idx = 0, .orient = orientation, .mode = mode};
    return ret_panel;
}
