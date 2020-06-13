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
    OdeUiCtlPanel ui_panel;
} OdeUiMain;

static Bool onInputMain(OdeUiCtl* ctl_panel_main, Str const bytes) {
    Bool ret_dirty = false;
    if (bytes.len == 1)
        switch (bytes.at[0]) {
            case 'b' & 0x1f: {
                ode.ui.sidebar_bottom->ui_panel.base.visible = !ode.ui.sidebar_bottom->ui_panel.base.visible;
                ret_dirty = true;
            } break;
            case 'l' & 0x1f: {
                ode.ui.sidebar_left->ui_panel.base.visible = !ode.ui.sidebar_left->ui_panel.base.visible;
                ret_dirty = true;
            } break;
            case 'r' & 0x1f: {
                ode.ui.sidebar_right->ui_panel.base.visible = !ode.ui.sidebar_right->ui_panel.base.visible;
                ret_dirty = true;
            } break;
        }
    if (ret_dirty)
        odeUiCtlSetDirty(ctl_panel_main, true, true);
    ret_dirty = odeUiCtlPanelOnInput(ctl_panel_main, bytes) // usually not needed, but this is the root panel
                | ret_dirty;
    return ret_dirty;
}

void odeUiMainOnResized(OdeSize const* const old, OdeSize const* const new) {
    odeUiCtlSetDirty(&ode.ui.main->ui_panel.base, true, true);
}

void odeUiInitMain() {
    OdeUiMain main = (OdeUiMain) {.ui_panel = odeUiCtlPanel(odeUiCtl(NULL, str("ode"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
                                                            ode_orient_none, ode_uictl_panel_none, 5)};
    main.ui_panel.base.color.bg = rgba(0x18, 0x14, 0x18, 0xff);
    main.ui_panel.base.color.fg = rgba(0xaf, 0xa9, 0xa8, 0xff);
    main.ui_panel.base.on.input = onInputMain;
    ode.ui.main = ·keep(OdeUiMain, NULL, &main);

    odeUiInitStatusbar();
    odeUiInitSidebars();
    odeUiInitEditors();

    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.statusbar->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_left->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_right->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_bottom->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.editors->ui_panel.base);
}
