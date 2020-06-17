#pragma clang diagnostic ignored "-Wunused-parameter"
#define mem_bss_max (3 * 1024 * 1024)
#include "utils_std_basics.c"
#include "common.c"
#include "core.c"
#include "input.c"
#include "output.c"
#include "ui_main.c"

int main() {
    odeInit();
    odeUiInitMain();

    Bool redraw = true;
    while (!ode.input.exit_requested) {
        if (redraw)
            odeRenderOutput(&ode.ui.main->ui_panel.base, ode.output.screen.size);
        redraw =
            ode.input.screen_resized || odeProcessInput() || ode.input.screen_resized;
        ode.input.screen_resized = false;
    }
    // about final on-exit cleanups? the above `odeInit` call hooked core.c's
    // `odeOnExit` into libc's `atexit`
    return 0;
}
