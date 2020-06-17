#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"


PtrAny cmd·workbench·action·quit(OdeCmd const* const cmd, PtrAny const args_ptr, UInt const args_len) {
    ode.input.exit_requested = true;
    return NULL;
}


void odeInitCommands() {
    ·append(ode.input.all.commands, ((OdeCmd) {.id = str("workbench.action.quit"),
                                               .text = str("Exit"),
                                               .hotkey = odeHotKey('q', true, false, false),
                                               .menu_mnemonic = 'x',
                                               .handler = cmd·workbench·action·quit}));
}
