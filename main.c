#define mem_bss_max (2 * 1024 * 1024)
#include "utils_std_basics.c"
#include "core.c"
#include "input.c"
#include "output.c"
#include "ui_workbench.c"

int main() {
    odeInit();
    odeUiInitWorkbench();

    for (Bool redraw = true; !ode.input.exit_requested; redraw = odeProcessInput())
        if (redraw)
            odeRenderOutput();

    return 0;
}
