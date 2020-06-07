#pragma once
#include "utils_libc_deps_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"
#include "ui_panel_diags.c"
#include "ui_panel_explorer.c"
#include "ui_panel_extensions.c"
#include "ui_panel_outline.c"
#include "ui_panel_logs.c"
#include "ui_panel_search.c"
#include "ui_panel_terminals.c"
#include "ui_sidebars.c"


void odeUiInitWorkbench() {
    ode.ui.main = odeUiCtlPanel(odeUiCtl(str("ode"), ode_uictl_dock_top | ode_uictl_dock_left, (OdePos) {5, 5}, (OdeSize) {90, 90}),
                                ode_orient_none, ode_uictl_panel_none, 4);
    // ode.ui.main.base.color.bg = rgba(0xef, 0xe9, 0xe8, 0xff);
    // ode.ui.main.base.color.fg = rgba(0xa0, 0x60, 0x30, 0xff);

    odeUiInitPanelDiags();
    odeUiInitPanelExplorer();
    odeUiInitPanelExtensions();
    odeUiInitPanelOutline();
    odeUiInitPanelLogs();
    odeUiInitPanelSearch();
    odeUiInitPanelTerminals();
    odeUiInitSidebars();

    ·append(ode.ui.main.ctls, &ode.ui.sidebar_left.base);
    ·append(ode.ui.main.ctls, &ode.ui.sidebar_right.base);
    ·append(ode.ui.main.ctls, &ode.ui.sidebar_bottom.base);
    ·append(ode.ui.main.ctls, &ode.ui.editors.base);
}
