#define mem_max (1 * 1024 * 1024)
#include "utils_libc_deps_basics.c"
#include "core.c"
#include "input.c"
#include "output.c"

int main() {
    odeInit();

    for (Bool repaint = true; !ode.input.exit_requested; repaint = odeProcessInput())
        if (repaint)
            odeRenderOutput();

    return 0;
}
