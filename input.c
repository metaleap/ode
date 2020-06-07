#pragma once
#include "core.c"

Bool odeProcessInput() {
    static U8 input_buf[ode_input_buf_size];
    Int n_bytes_read = read(STDIN_FILENO, input_buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            return false;
        else
            odeDie("odeProcessInput: read", true);
    }

    for (UInt i = 0, max = (UInt)n_bytes_read; i < max; i += 1)
        if (input_buf[i] == (0x1f & 'q'))
            ode.input.exit_requested = true;
    return false;
}
