#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

static Bool onInput(OdeUiCtl* ctl_panel_statusbar, Str const bytes) {
    static UInt counter = 0;
    counter += 1;
    Strs hexes = ·sliceOf(Str, bytes.len, bytes.len);
    for (UInt i = 0; i < bytes.len; i += 1)
        hexes.at[i] = uIntToStr(bytes.at[i], 2, 16);
    ctl_panel_statusbar->text = str4(str("Statusbar @"), uIntToStr(counter, 1, 10), str(": x"), strConcat(hexes, 'x'));
    odeUiCtlSetDirty(ctl_panel_statusbar, true, true);
    return true;
}

void odeUiInitStatusbar() {
    ode.ui.statusbar =
        odeUiCtlPanel(odeUiCtl(str("Statusbar"), ode_uictl_dock_bottom, rect(0, 0, 0, 1)), ode_orient_horiz, ode_uictl_panel_orient, 32);
    ode.ui.statusbar.base.color.bg = rgba(0, 0, 0, 0);
    ode.ui.statusbar.base.color.fg = rgba(0, 0, 0, 0);
    ode.ui.statusbar.base.on.input = onInput;
}
