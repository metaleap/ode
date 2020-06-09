#pragma once
#include "core.c"
#include "ui_ctl_panel.c"

Bool odeProcessInput() {
    static U8 input_buf[ode_input_buf_size];
    Int n_bytes_read = read(STDIN_FILENO, input_buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            return false;
        else
            odeDie("odeProcessInput: read", true);
    }

    for (UInt i = 0, max = (UInt)n_bytes_read; i < max && !ode.input.exit_requested; i += 1)
        if (input_buf[i] == (0x1f & 'q'))
            ode.input.exit_requested = true;
    return (!ode.input.exit_requested) && (n_bytes_read > 0)
           && ode.ui.main->base.on.input(&ode.ui.main->base, (Str) {.at = input_buf, .len = (UInt)n_bytes_read});
}
