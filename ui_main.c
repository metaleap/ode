#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"
#include "ui_sidebars.c"
#include "ui_statusbar.c"
#include "ui_editors.c"

typedef struct OdeUiMain {
    OdeUiCtlPanel base;
} OdeUiMain;

static Bool onInputMain(OdeUiCtl* ctl_panel_main, Str const bytes) {
    Bool ret_dirty = false;
    if (bytes.len == 1)
        switch (bytes.at[0]) {
            case 'b': {
                ode.ui.sidebar_bottom->base.base.visible = !ode.ui.sidebar_bottom->base.base.visible;
                ret_dirty = true;
            } break;
            case 'l': {
                ode.ui.sidebar_left->base.base.visible = !ode.ui.sidebar_left->base.base.visible;
                ret_dirty = true;
            } break;
            case 'r': {
                ode.ui.sidebar_right->base.base.visible = !ode.ui.sidebar_right->base.base.visible;
                ret_dirty = true;
            } break;
            default: {
                // static UInt idxs[8] = {
                //     [0] = 'e', [1] = 'f', [2] = 'x', // sidebar_left
                //     [3] = 'o', [4] = 'l',            // sidebar_right
                //     [6] = 'd', [7] = 't',            // sidebar_bottom
                // };
                // OdeUiCtlPanel* sidebar = ode.ui.sidebar_left;
                // for (UInt i = 0; i < 8; i += 1) {
                //     if (i == 3)
                //         sidebar = ode.ui.sidebar_right;
                //     if (i == 6)
                //         sidebar = ode.ui.sidebar_bottom;
                //     if (idxs[i] != 0 && (idxs[i] & 0x1f) == bytes.at[0]) {
                //         UInt idx = i % 3;
                //         if (sidebar->ctl_idx != idx) {
                //             sidebar->ctl_idx = idx;
                //             ret_dirty = true;
                //             odeUiCtlSetDirty(&sidebar->base, true, true);
                //         }
                //         break;
                //     }
                // }
            } break;
        }
    if (ret_dirty)
        odeUiCtlSetDirty(ctl_panel_main, true, true);
    ret_dirty = odeUiCtlPanelOnInput(ctl_panel_main, bytes) // usually not needed, but this is the root panel
                | ret_dirty;
    return ret_dirty;
}

void odeUiMainOnResized(OdeSize const* const old, OdeSize const* const new) {
    odeUiCtlSetDirty(&ode.ui.main->base.base, true, true);
}

void odeUiInitMain() {
    OdeUiMain main = (OdeUiMain) {
        .base = odeUiCtlPanel(odeUiCtl(NULL, str("ode"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 5)};
    main.base.base.color.bg = rgba(0xa0, 0x60, 0x30, 0xff);
    main.base.base.color.fg = rgba(0xef, 0xe9, 0xe8, 0xff);
    main.base.base.on.input = onInputMain;
    ode.ui.main = ·keep(OdeUiMain, NULL, &main);

    odeUiInitStatusbar();
    odeUiInitSidebars();
    odeUiInitEditors();

    ·append(ode.ui.main->base.base.ctls, &ode.ui.statusbar->base.base);
    ·append(ode.ui.main->base.base.ctls, &ode.ui.sidebar_left->base.base);
    ·append(ode.ui.main->base.base.ctls, &ode.ui.sidebar_right->base.base);
    ·append(ode.ui.main->base.base.ctls, &ode.ui.sidebar_bottom->base.base);
    ·append(ode.ui.main->base.base.ctls, &ode.ui.editors->base.base);
}
