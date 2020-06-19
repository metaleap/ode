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
    ºUInt tab_idx;
    OdeOrientation orient;
    OdeUiCtlPanelMode mode;
} OdeUiCtlPanel;

static OdeRect renderChild(OdeUiCtl* ctl_panel, OdeUiCtl* ctl, OdeRect dst_rect) {
    ctl->parent = ctl_panel;
    if (ctl->visible)
        dst_rect = odeRender(ctl, dst_rect);
    ctl->dirty = false;
    return dst_rect;
}

static void onRender(OdeUiCtl* ctl_panel, OdeRect* screen_rect) {
    OdeRect dst_rect = *screen_rect;
    OdeUiCtlPanel* panel = (OdeUiCtlPanel*)ctl_panel;

    if (panel->mode == ode_uictl_panel_none)
        for (UInt i = 0; i < ctl_panel->ctls.len; i += 1)
            dst_rect = renderChild(ctl_panel, ctl_panel->ctls.at[i], dst_rect);
    else if (panel->mode == ode_uictl_panel_tabs) {
        if (panel->tab_bar != NULL)
            dst_rect = renderChild(ctl_panel, panel->tab_bar, dst_rect);
        if (ctl_panel->ctls.len > 0)
            dst_rect =
                renderChild(ctl_panel, ctl_panel->ctls.at[panel->tab_idx.it], dst_rect);
    } else if (ctl_panel->ctls.len > 0)
        odeDie(strZ(str4(NULL, str("TODO: "), uIntToStr(NULL, panel->mode, 1, 10),
                         str(" "), panel->base.text)),
               false);

    // temp render logic: ctl.text if no other content
    if (ctl_panel->ctls.len == 0) {
        odeScreenClearRectText(&dst_rect);
        odeRenderText(ctl_panel->text, &dst_rect, false);
    }
}

Bool odeUiCtlPanelOnInput(OdeUiCtl* ctl_panel, MemHeap* mem_tmp,
                          OdeInputs const inputs) {
    Bool dirty = false;
    for (UInt i = 0; i < ctl_panel->ctls.len; i += 1) {
        if (ctl_panel->ctls.at[i]->on.input != NULL)
            dirty |=
                ctl_panel->ctls.at[i]->on.input(ctl_panel->ctls.at[i], mem_tmp, inputs);
        if (ctl_panel->ctls.at[i]->on.input != odeUiCtlPanelOnInput)
            dirty |= odeUiCtlPanelOnInput(ctl_panel->ctls.at[i], mem_tmp, inputs);
    }
    return dirty;
}

OdeUiCtlPanel odeUiCtlPanel(OdeUiCtl base, OdeOrientation orientation,
                            OdeUiCtlPanelMode mode, UInt const ctls_capacity) {
    base.on.render = onRender;
    base.on.input = odeUiCtlPanelOnInput;
    base.ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, base.mem, 0, ctls_capacity);
    return (OdeUiCtlPanel) {
        .base = base, .tab_idx = ·none(UInt), .orient = orientation, .mode = mode};
}

UInt odeUiCtlPanelAppend(OdeUiCtlPanel* const this, OdeUiCtl* const ctl) {
    for (UInt i = 0; i < this->base.ctls.len; i += 1)
        ·assert(this->base.ctls.at[i] != ctl);
    ·append(this->base.ctls, ctl);
    ctl->parent = &this->base;
    return this->base.ctls.len - 1;
}

Bool odeUiCtlPanelEnsureActiveTabIdx(OdeUiCtlPanel* const this, UInt const tab_idx) {
    Bool const did = (!this->tab_idx.got) || (tab_idx != this->tab_idx.it);
    if (did) {
        this->tab_idx = ·got(UInt, tab_idx);
        odeUiCtlSetDirty(this->base.ctls.at[tab_idx], true, true);
    }
    return did;
}
