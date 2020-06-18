#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

typedef struct OdeUiStatusbar {
    OdeUiCtlPanel ui_panel;
} OdeUiStatusbar;

static Bool onInput(OdeUiCtl* ctl_panel_statusbar, MemHeap* mem_tmp,
                    OdeInputs const inputs) {
    static U8 str_buf[1 * 1024 * 1024];

    Str text = (Str) {.at = &str_buf[0], .len = 0};
    for (UInt i = 0; i < inputs.len; i += 1) {
        ·push(text, ' ');
        OdeInput const input = inputs.at[i];
        switch (input.kind) {
            case ode_input_str: {
                for (UInt b = 0; b < input.of.string.len; b += 1) {
                    ·push(text, 'x');
                    text.len +=
                        uintToBuf(&text.at[text.len], input.of.string.at[b], 2, 16, 0)
                            .len;
                }
            } break;
            case ode_input_mouse: {
                text = strPush(text, "Mouse@");
                text.len +=
                    uintToBuf(&text.at[text.len], ode.input.mouse.pos.x, 3, 10, 0).len;
                ·push(text, ',');
                text.len +=
                    uintToBuf(&text.at[text.len], ode.input.mouse.pos.y, 3, 10, 0).len;
                if (input.of.mouse.scroll)
                    text = strPush(text, input.of.mouse.down ? ",Event.ScrollDown"
                                                             : "Event.ScrollUp");
                if (input.of.mouse.did_move)
                    text = strPush(text, ",Event.Move");
                if (input.of.mouse.btn_l)
                    text = strPush(text, input.of.mouse.down
                                             ? ",Event.LBtnDown"
                                             : input.of.mouse.did_click
                                                   ? ",Event.LBtnClick"
                                                   : input.of.mouse.did_drop
                                                         ? ",Event.LBtnDrop"
                                                         : ",Event.LBtnDrag");
                if (input.of.mouse.btn_m)
                    text = strPush(text, input.of.mouse.down
                                             ? ",Event.MBtnDown"
                                             : input.of.mouse.did_click
                                                   ? ",Event.MBtnClick"
                                                   : input.of.mouse.did_drop
                                                         ? ",Event.MBtnDrop"
                                                         : ",Event.MBtnDrag");
                if (input.of.mouse.btn_r)
                    text = strPush(text, input.of.mouse.down
                                             ? ",Event.RBtnDown"
                                             : input.of.mouse.did_click
                                                   ? ",Event.RBtnClick"
                                                   : input.of.mouse.did_drop
                                                         ? ",Event.RBtnDrop"
                                                         : ",Event.RBtnDrag");
                if (ode.input.mouse.dragging)
                    text = strPush(text, ",State.Dragging");
                if (ode.input.mouse.btn_down.left)
                    text = strPush(text, ",State.LBtnDown");
                if (ode.input.mouse.btn_down.mid)
                    text = strPush(text, ",State.MBtnDown");
                if (ode.input.mouse.btn_down.right)
                    text = strPush(text, ",State.RBtnDown");
            } break;
            case ode_input_hotkey: {
                text = strPush(text, "HotKey:");
                OdeHotKey* const found = odeHotKey(input.of.key, input.mod_key);
                text = strPush(text,
                               (found == NULL) ? "Unknown" : ((CStr)found->title.at));
            } break;
            default: odeDie(strZ(uIntToStr(NULL, input.kind, 1, 10)), false);
        }
        text = strPush(text, "    ");
        if (odeModKeyHas(input.mod_key, ode_key_mod_ctl))
            text = strPush(text, "Ctrl");
        if (odeModKeyHas(input.mod_key, ode_key_mod_shift))
            text = strPush(text, "Shift");
        if (odeModKeyHas(input.mod_key, ode_key_mod_alt))
            text = strPush(text, "Alt");
    }
    text.at[text.len] = 0;

    ctl_panel_statusbar->text = str8(
        NULL, str("nR="), uIntToStr(NULL, ode.stats.num_renders, 1, 10), str(" nO="),
        uIntToStr(NULL, ode.stats.num_outputs, 1, 10), str(" LP="),
        uIntToStr(NULL, ode.stats.last_output_payload, 1, 10), str("      "), text);
    odeUiCtlSetDirty(ctl_panel_statusbar, true, true);
    return true;
}

void odeUiInitStatusbar() {
    OdeUiStatusbar statusbar = (OdeUiStatusbar) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(NULL, str("Statusbar"), ode_uictl_dock_bottom, rect(0, 0, 0, 1)),
            ode_orient_horiz, ode_uictl_panel_orient, 32)};
    statusbar.ui_panel.base.color.bg = rgba(0, 0, 0, 0);
    statusbar.ui_panel.base.color.fg = rgba(0, 0, 0, 0);
    statusbar.ui_panel.base.on.input = onInput;
    ode.ui.statusbar = ·keep(OdeUiStatusbar, NULL, &statusbar);
}
