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


void odeUiInitWorkbench() {
    ode.ui.main = odeUiCtlPanel(odeUiCtl(str("ode"), ode_uictl_dock_fill, rect(0, 0, 0, 0)), ode_orient_none, ode_uictl_panel_none, 5);
    ode.ui.main.base.color.bg = rgba(0xa0, 0x60, 0x30, 0xff);
    ode.ui.main.base.color.fg = rgba(0xef, 0xe9, 0xe8, 0xff);

    odeUiInitStatusbar();
    odeUiInitViewDiags();
    odeUiInitViewExplorer();
    odeUiInitViewExtensions();
    odeUiInitViewOutline();
    odeUiInitViewLogs();
    odeUiInitViewSearch();
    odeUiInitViewTerminals();
    odeUiInitSidebars();

    // ·append(ode.ui.main.ctls, &ode.ui.statusbar.base);
    // ·append(ode.ui.main.ctls, &ode.ui.sidebar_left.base);
    // ·append(ode.ui.main.ctls, &ode.ui.sidebar_right.base);
    // ·append(ode.ui.main.ctls, &ode.ui.sidebar_bottom.base);
    // ·append(ode.ui.main.ctls, &ode.ui.editors.base);
}
