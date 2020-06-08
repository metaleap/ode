#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitStatusbar() {
    ode.ui.statusbar =
        odeUiCtlPanel(odeUiCtl(str("Statusbar"), ode_uictl_dock_bottom, rect(0, 0, 0, 1)), ode_orient_horiz, ode_uictl_panel_orient, 32);
    ode.ui.statusbar.base.color.bg = rgba(0xef, 0xe9, 0xe8, 0xff);
    ode.ui.statusbar.base.color.fg = rgba(0xa0, 0x60, 0x30, 0xff);
}
