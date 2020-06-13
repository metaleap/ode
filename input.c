#pragma once
#include "common.c"
#include "core.c"
#include "ui_main.c"
#include "utils_std_mem.c"

#define ode_input_buf_size (1 * 1024 * 1024)

Bool odeProcessInput() {
    static U8 input_buf[ode_input_buf_size];
    static OdeInput inputs_buf[ode_input_buf_size];
    // static MemHeap* bracketed_paste = NULL;

    Int n_bytes_read = read(ode.init.term.tty_fileno, input_buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            return false;
        else
            odeDie("odeProcessInput: read", true);
    }
    OdeInputs inputs = {.at = &inputs_buf[0], .len = 0};
    if (n_bytes_read > 0) {
        // Bool const more_pending = (n_bytes_read == ode_input_buf_size);

        for (UInt i = 0, max = (UInt)n_bytes_read; i < max && !ode.input.exit_requested; i += 1)
            if (input_buf[i] == (0x1f & 'q'))
                ode.input.exit_requested = true;
    }
    return (!ode.input.exit_requested) && (inputs.len > 0) && ode.ui.main->ui_panel.base.on.input(&ode.ui.main->ui_panel.base, inputs);
}
