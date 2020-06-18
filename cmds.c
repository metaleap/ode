#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "ui_view.c"

static PtrAny workbench·action·quit(OdeCmd const* const cmd, PtrAny const args_ptr,
                                    UInt const args_len) {
    ode.input.exit_requested = true;
    return NULL;
}

static PtrAny workbench·view·explorer(OdeCmd const* const cmd, PtrAny const args_ptr,
                                      UInt const args_len) {
    return odeUiView(ode_uiview_explorer);
}

static PtrAny workbench·view·extensions(OdeCmd const* const cmd, PtrAny const args_ptr,
                                        UInt const args_len) {
    return odeUiView(ode_uiview_extensions);
}

static PtrAny workbench·view·issues(OdeCmd const* const cmd, PtrAny const args_ptr,
                                    UInt const args_len) {
    return odeUiView(ode_uiview_issues);
}

static PtrAny workbench·view·notifications(OdeCmd const* const cmd,
                                           PtrAny const args_ptr, UInt const args_len) {
    return odeUiView(ode_uiview_notifications);
}

static PtrAny workbench·view·outline(OdeCmd const* const cmd, PtrAny const args_ptr,
                                     UInt const args_len) {
    return odeUiView(ode_uiview_outline);
}

static PtrAny workbench·view·output(OdeCmd const* const cmd, PtrAny const args_ptr,
                                    UInt const args_len) {
    return odeUiView(ode_uiview_output);
}

static PtrAny workbench·view·search(OdeCmd const* const cmd, PtrAny const args_ptr,
                                    UInt const args_len) {
    return odeUiView(ode_uiview_search);
}

static PtrAny workbench·view·terminal(OdeCmd const* const cmd, PtrAny const args_ptr,
                                      UInt const args_len) {
    return odeUiView(ode_uiview_terminal);
}



void odeInitCommands() {
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.action.quit"),
                       .text = str("Quit"),
                       .hotkey = odeHotKey('q', ode_key_mod_ctl),
                       .menu_mnemonic = 'Q',
                       .handler = workbench·action·quit}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.view.explorer"),
                       .text = str("View: Explorer"),
                       .hotkey = odeHotKey('e', ode_key_mod_alt),
                       .menu_mnemonic = 'E',
                       .handler = workbench·view·explorer}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.view.extensions"),
                       .text = str("View: Extensions"),
                       .hotkey = odeHotKey('x', ode_key_mod_alt),
                       .menu_mnemonic = 'x',
                       .handler = workbench·view·extensions}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.action.problems.focus"),
                       .text = str("View: Problems"),
                       .hotkey = odeHotKey('p', ode_key_mod_alt),
                       .menu_mnemonic = 'P',
                       .handler = workbench·view·issues}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("notifications.showList"),
                       .text = str("View: Notifications"),
                       .hotkey = odeHotKey('n', ode_key_mod_alt),
                       .menu_mnemonic = 'N',
                       .handler = workbench·view·notifications}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("outline.focus"),
                       .text = str("View: Outline"),
                       .hotkey = odeHotKey('o', ode_key_mod_alt),
                       .menu_mnemonic = 'O',
                       .handler = workbench·view·outline}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.action.output.toggleOutput"),
                       .text = str("View: Log Outputs"),
                       .hotkey = odeHotKey('l', ode_key_mod_alt),
                       .menu_mnemonic = 'L',
                       .handler = workbench·view·output}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.view.search"),
                       .text = str("View: Search"),
                       .hotkey = odeHotKey('f', ode_key_mod_alt),
                       .menu_mnemonic = 'S',
                       .handler = workbench·view·search}));
    ·append(ode.input.all.commands,
            ((OdeCmd) {.id = str("workbench.action.terminal.toggleTerminal"),
                       .text = str("View: Terminal"),
                       .hotkey = odeHotKey('t', ode_key_mod_alt),
                       .menu_mnemonic = 'T',
                       .handler = workbench·view·terminal}));
}
