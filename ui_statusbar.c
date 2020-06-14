#pragma once
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiStatusbar {
    OdeUiCtlPanel ui_panel;
} OdeUiStatusbar;

static Bool onInputStatusbar(OdeUiCtl* ctl_panel_statusbar, MemHeap* mem_tmp, OdeInputs const inputs) {
    static U8 str_buf[1 * 1024 * 1024];

    Str text = (Str) {.at = &str_buf[0], .len = 0};
    for (UInt i = 0; i < inputs.len; i += 1) {
        ·push(text, ' ');
        OdeInput const input = inputs.at[i];
        switch (input.kind) {
            case ode_input_str: {
                for (UInt b = 0; b < input.of.string.len; b += 1) {
                    ·push(text, 'x');
                    text.len += uintToBuf(&text.at[text.len], input.of.string.at[b], 2, 16, 0).len;
                }
            } break;
            default: odeDie(strZ(uIntToStr(NULL, input.kind, 1, 10)), false);
        }
    }
    text.at[text.len] = 0;

    ctl_panel_statusbar->text =
        str8(NULL, str("nR="), uIntToStr(NULL, ode.stats.num_renders, 1, 10), str(" nO="), uIntToStr(NULL, ode.stats.num_outputs, 1, 10),
             str(" LP="), uIntToStr(NULL, ode.stats.last_output_payload, 1, 10), str("      "), text);
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
