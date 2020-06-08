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
            odeRenderOutput(ode.output.screen.size);
        redraw = ode.output.screen.resized || odeProcessInput() || ode.output.screen.resized;
        ode.output.screen.resized = false;
    }

    return 0;
}
