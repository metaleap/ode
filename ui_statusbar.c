#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiStatusbar {
    OdeUiCtlPanel ui_panel;
} OdeUiStatusbar;

static Bool onInputStatusbar(OdeUiCtl* ctl_panel_statusbar, Str const bytes) {
    Strs hexes = ·sliceOf(Str, NULL, bytes.len, bytes.len);
    for (UInt i = 0; i < bytes.len; i += 1)
        hexes.at[i] = uIntToStr(NULL, bytes.at[i], 2, 16);
    ctl_panel_statusbar->text =
        str8(NULL, str("nR="), uIntToStr(NULL, ode.stats.num_renders, 1, 10), str(" nO="), uIntToStr(NULL, ode.stats.num_outputs, 1, 10),
             str(" LP="), uIntToStr(NULL, ode.stats.last_output_payload, 1, 10), str("     x"), strConcat(NULL, hexes, 'x'));
    odeUiCtlSetDirty(ctl_panel_statusbar, true, true);
    return true;
}

void odeUiInitStatusbar() {
    OdeUiStatusbar statusbar =
        (OdeUiStatusbar) {.ui_panel = odeUiCtlPanel(odeUiCtl(NULL, str("Statusbar"), ode_uictl_dock_bottom, rect(0, 0, 0, 1)),
                                                    ode_orient_horiz, ode_uictl_panel_orient, 32)};
    statusbar.ui_panel.base.color.bg = rgba(0, 0, 0, 0);
    statusbar.ui_panel.base.color.fg = rgba(0, 0, 0, 0);
    statusbar.ui_panel.base.on.input = onInputStatusbar;
    ode.ui.statusbar = ·keep(OdeUiStatusbar, NULL, &statusbar);
}
