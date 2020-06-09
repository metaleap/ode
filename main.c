#pragma clang diagnostic ignored "-Wunused-parameter"
#define mem_bss_max (2 * 1024 * 1024)
#include "utils_std_basics.c"
#include "core.c"
#include "input.c"
#include "output.c"
#include "ui_workbench.c"

int main() {
    odeInit();
    odeUiInitWorkbench();

    Bool redraw = true;
    while (!ode.input.exit_requested) {
        if (redraw)
            odeRenderOutput(&ode.ui.main->base, ode.output.screen.size);
        redraw = ode.input.screen_resized || odeProcessInput() || ode.input.screen_resized;
        ode.input.screen_resized = false;
    }

    return 0;
}
