#pragma clang diagnostic ignored "-Wunused-parameter"
#define mem_bss_max (2 * 1024 * 1024)
#include "utils_std_basics.c"
#include "core.c"
#include "input.c"
#include "output.c"
#include "ui_main.c"

PtrAny odeCmdQuit(struct OdeCmd const* const cmd, PtrAny const args, UInt const args_len) {
    ode.input.exit_requested = true;
    return NULL;
}

int main() {
    odeInit();
    odeCmd(str("workbench.action.quit"), odeCmdQuit);
    odeUiInitMain();

    Bool redraw = true;
    while (!ode.input.exit_requested) {
        if (redraw)
            odeRenderOutput(&ode.ui.main->base, ode.output.screen.size);
        redraw = ode.input.screen_resized || odeProcessInput() || ode.input.screen_resized;
        ode.input.screen_resized = false;
    }
    // about final on-exit cleanups? the above `odeInit` call hooked core.c's `odeOnExit` into libc's `atexit`
    return 0;
}
