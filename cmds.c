#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "ui_view.c"

static Any workbench·action·quit(OdeCmd const* const cmd, Any const args_ptr,
                                 UInt const args_len) {
    ode.input.exit_requested = true;
    return NULL;
}

static Any workbench·view·explorer(OdeCmd const* const cmd, Any const args_ptr,
                                   UInt const args_len) {
    return odeUiView(ode_ui_view_explorer, ode.ui.sidebar_left);
}

static Any workbench·view·extensions(OdeCmd const* const cmd, Any const args_ptr,
                                     UInt const args_len) {
    return odeUiView(ode_ui_view_extensions, ode.ui.sidebar_left);
}

static Any workbench·view·issues(OdeCmd const* const cmd, Any const args_ptr,
                                 UInt const args_len) {
    return odeUiView(ode_ui_view_issues, ode.ui.sidebar_right);
}

static Any workbench·view·notifications(OdeCmd const* const cmd, Any const args_ptr,
                                        UInt const args_len) {
    return odeUiView(ode_ui_view_notifications, ode.ui.sidebar_right);
}

static Any workbench·view·outline(OdeCmd const* const cmd, Any const args_ptr,
                                  UInt const args_len) {
    return odeUiView(ode_ui_view_outline, ode.ui.sidebar_right);
}

static Any workbench·view·output(OdeCmd const* const cmd, Any const args_ptr,
                                 UInt const args_len) {
    return odeUiView(ode_ui_view_output, ode.ui.sidebar_bottom);
}

static Any workbench·view·search(OdeCmd const* const cmd, Any const args_ptr,
                                 UInt const args_len) {
    return odeUiView(ode_ui_view_search, ode.ui.sidebar_bottom);
}

static Any workbench·view·terminal(OdeCmd const* const cmd, Any const args_ptr,
                                   UInt const args_len) {
    return odeUiView(ode_ui_view_terminal, ode.ui.sidebar_bottom);
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
