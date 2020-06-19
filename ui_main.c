#pragma once
#include "utils_std_mem.c"
#include "cmds.c"
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"
#include "ui_sidebars.c"
#include "ui_statusbar.c"
#include "ui_editors.c"

typedef struct OdeUiMain {
    OdeUiCtlPanel ui_panel;
    struct {
        OdeHotKey* chord_key_pending;
    } input;
} OdeUiMain;

static Bool onInputMain(OdeUiCtl* ctl_panel_main, MemHeap* mem_tmp, OdeInputs inputs) {
    // first: scan inputs for hotkeys bound to commands, invoke those commands and
    // remove the hotkeys from inputs before passing the latter on to child controls
    Bool const had_chord_pending = (ode.ui.main->input.chord_key_pending != NULL);
    Bool had_any_hotkeys = false;
    if (had_chord_pending) {
        for (UInt i = inputs.len; i > 0; i -= 1)
            inputs.at[i] = inputs.at[i - 1];
        inputs.len += 1;
        inputs.at[0] =
            (OdeInput) {.kind = ode_input_hotkey,
                        .of = {.key = ode.ui.main->input.chord_key_pending->key},
                        .mod_key = ode.ui.main->input.chord_key_pending->mod};
    }
    for (UInt i = 0; i < inputs.len; i += 1)
        if (inputs.at[i].kind == ode_input_hotkey) {
            had_any_hotkeys |= (i > 0 || !had_chord_pending);
            OdeHotKey* const hotkey =
                odeHotKey(inputs.at[i].of.key, inputs.at[i].mod_key);
            OdeHotKey* const hotkey_chord =
                (i == inputs.len - 1 || inputs.at[i + 1].kind != ode_input_hotkey)
                    ? NULL
                    : odeHotKey(inputs.at[i + 1].of.key, inputs.at[i + 1].mod_key);
            if (hotkey != NULL) {
                OdeCmd* cmd_found = NULL;
                Bool is_known_chord_key = false;
                ·forEach(OdeCmd, cmd, ode.input.all.commands, {
                    if (cmd->hotkey == hotkey) {
                        if (cmd->hotkey_chord != NULL)
                            is_known_chord_key = true;
                        if (cmd->hotkey_chord == hotkey_chord) {
                            cmd_found = cmd;
                            break;
                        }
                    }
                });
                if (cmd_found == NULL && !is_known_chord_key)
                    ·forEach(OdeCmd, cmd, ode.input.all.commands, {
                        if (cmd->hotkey == hotkey && cmd->hotkey_chord == NULL) {
                            cmd_found = cmd;
                            break;
                        }
                    });
                UInt n_del = (is_known_chord_key) ? 1 : 0;
                if (cmd_found != NULL) {
                    cmd_found->handler(cmd_found, NULL, 0);
                    n_del = (cmd_found->hotkey_chord == NULL) ? 1 : 2;
                } else if (is_known_chord_key)
                    ode.ui.main->input.chord_key_pending = hotkey;
                if (n_del > 0) {
                    for (UInt j = i + n_del; j < inputs.len; j += 1)
                        inputs.at[j - n_del] = inputs.at[i];
                    inputs.len -= n_del;
                    i -= 1;
                }
            }
        }
    if (had_chord_pending)
        ode.ui.main->input.chord_key_pending = NULL;
    return (inputs.len > 0 && // usually not needed, but this is the root panel:
            odeUiCtlPanelOnInput(ctl_panel_main, mem_tmp, inputs))
           | ctl_panel_main->dirty; // any input-processing commands or controls could
                                    // have caused this
}

void odeUiMainOnResized(OdeSize const* const old, OdeSize const* const new) {
    odeUiCtlSetDirty(&ode.ui.main->ui_panel.base, true, true);
}

void odeUiInitMain() {
    OdeUiMain main = (OdeUiMain) {
        .ui_panel = odeUiCtlPanel(
            odeUiCtl(NULL, str("ode"), ode_uictl_dock_fill, rect(0, 0, 0, 0)),
            ode_orient_none, ode_uictl_panel_none, 5)};
    main.ui_panel.base.color.bg = rgba(0x18, 0x14, 0x18, 0xff);
    main.ui_panel.base.color.fg = rgba(0xaf, 0xa9, 0xa8, 0xff);
    main.ui_panel.base.on.input = onInputMain;
    ode.ui.main = ·keep(OdeUiMain, NULL, &main);

    odeUiInitStatusbar();
    odeUiInitSidebars();
    odeUiInitEditors();

    odeUiCtlPanelAppend(&ode.ui.main->ui_panel, &ode.ui.statusbar->ui_panel.base);
    odeUiCtlPanelAppend(&ode.ui.main->ui_panel, &ode.ui.sidebar_left->ui_panel.base);
    odeUiCtlPanelAppend(&ode.ui.main->ui_panel, &ode.ui.sidebar_right->ui_panel.base);
    odeUiCtlPanelAppend(&ode.ui.main->ui_panel, &ode.ui.sidebar_bottom->ui_panel.base);
    odeUiCtlPanelAppend(&ode.ui.main->ui_panel, &ode.ui.editors->ui_panel.base);
}
