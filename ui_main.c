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
} OdeUiMain;

static Bool onInputMain(OdeUiCtl* ctl_panel_main, MemHeap* mem_tmp, OdeInputs inputs) {
    // first: scan inputs for hotkeys bound to commands, invoke those commands and
    // remove the hotkeys from inputs before passing the latter on to child controls
    for (UInt i = 0; i < inputs.len; i += 1)
        if (inputs.at[i].kind == ode_input_hotkey) {
            OdeHotKey* const hotkey =
                odeHotKey(inputs.at[i].of.key, inputs.at[i].mod_key.ctl,
                          inputs.at[i].mod_key.alt, inputs.at[i].mod_key.shift);
            if (hotkey != NULL)
                ·forEach(OdeCmd, cmd, ode.input.all.commands, {
                    if (cmd->hotkey == hotkey) {
                        cmd->handler(cmd, NULL, 0);
                        for (UInt j = i + 1; j < inputs.len; j += 1)
                            inputs.at[j - 1] = inputs.at[i];
                        inputs.len -= 1;
                        i -= 1;
                        break;
                    }
                });
        }

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

    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.statusbar->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_left->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_right->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.sidebar_bottom->ui_panel.base);
    ·append(ode.ui.main->ui_panel.base.ctls, &ode.ui.editors->ui_panel.base);
}
