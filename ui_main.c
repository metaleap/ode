#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"
#include "ui_view_diags.c"
#include "ui_view_explorer.c"
#include "ui_view_extensions.c"
#include "ui_view_outline.c"
#include "ui_view_logs.c"
#include "ui_view_search.c"
#include "ui_view_terminals.c"
#include "ui_sidebars.c"
#include "ui_statusbar.c"

static Bool onInputMain(OdeUiCtl* ctl_panel_main, Str const bytes) {
    Bool dirty = false;
    if (bytes.len == 1)
        switch (bytes.at[0]) {
            case 'b': {
                ode.ui.sidebar_bottom->base.visible = !ode.ui.sidebar_bottom->base.visible;
                dirty = true;
            } break;
            case 'l': {
                ode.ui.sidebar_left->base.visible = !ode.ui.sidebar_left->base.visible;
                dirty = true;
            } break;
            case 'r': {
                ode.ui.sidebar_right->base.visible = !ode.ui.sidebar_right->base.visible;
                dirty = true;
            } break;
            case 0x05: {
                if (ode.ui.sidebar_left->ctl_idx != 0) {
                    ode.ui.sidebar_left->ctl_idx = 0;
                    odeUiCtlSetDirty(&ode.ui.sidebar_left->base, true, true);
                }
            } break;
            case 0x13: {
                if (ode.ui.sidebar_left->ctl_idx != 1) {
                    ode.ui.sidebar_left->ctl_idx = 1;
                    odeUiCtlSetDirty(&ode.ui.sidebar_left->base, true, true);
                }
            } break;
        }
    if (dirty)
        odeUiCtlSetDirty(ctl_panel_main, true, true);
    dirty = odeUiCtlPanelOnInput(ctl_panel_main, bytes) // usually not needed, but this is the root panel
            | dirty;
    return dirty;
}

void odeUiMainOnResized(OdeSize const* const old, OdeSize const* const new) {
#define default_fallback 4
    UInt const frac_sidebar_left = (old->width > ode.ui.sidebar_left->base.rect.size.width)
                                       ? (old->width / ode.ui.sidebar_left->base.rect.size.width)
                                       : default_fallback;
    UInt const frac_sidebar_right = (old->width > ode.ui.sidebar_right->base.rect.size.width)
                                        ? (old->width / ode.ui.sidebar_right->base.rect.size.width)
                                        : default_fallback;
    UInt const frac_sidebar_bottom = (old->height > ode.ui.sidebar_bottom->base.rect.size.height)
                                         ? (old->height / ode.ui.sidebar_bottom->base.rect.size.height)
                                         : default_fallback;
    ode.ui.sidebar_left->base.rect.size.width = new->width / frac_sidebar_left;
    ode.ui.sidebar_right->base.rect.size.width = new->width / frac_sidebar_right;
    ode.ui.sidebar_bottom->base.rect.size.height = new->height / frac_sidebar_bottom;
}

void odeUiInitEditArea() {
    ode.ui.editors =
        odeUiCtlPanel(odeUiCtl(str("Editors Area"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 0);
}

void odeUiInitMain() {
    ode.ui.main = odeUiCtlPanel(odeUiCtl(str("ode"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 5);
    ode.ui.main->base.color.bg = rgba(0xa0, 0x60, 0x30, 0xff);
    ode.ui.main->base.color.fg = rgba(0xef, 0xe9, 0xe8, 0xff);
    ode.ui.main->base.on.input = onInputMain;

    odeUiInitStatusbar();
    odeUiInitViewExplorer();
    odeUiInitViewSearch();
    odeUiInitViewExtensions();
    odeUiInitViewOutline();
    odeUiInitViewLogs();
    odeUiInitViewDiags();
    odeUiInitViewTerminals();
    odeUiInitSidebars();
    odeUiInitEditArea();

    ·append(ode.ui.main->base.ctls, &ode.ui.statusbar->base);
    ·append(ode.ui.main->base.ctls, &ode.ui.sidebar_left->base);
    ·append(ode.ui.main->base.ctls, &ode.ui.sidebar_right->base);
    ·append(ode.ui.main->base.ctls, &ode.ui.sidebar_bottom->base);
    ·append(ode.ui.main->base.ctls, &ode.ui.editors->base);
}
