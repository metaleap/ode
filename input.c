#pragma once
#include "core.c"

Bool odeProcessInput() {
    Int n_bytes_read = read(STDIN_FILENO, ode.input.buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            n_bytes_read = 0;
        else
            odeDie("odeProcessInput: read");
    }

    ode.input.n_bytes_read = n_bytes_read;

    for (UInt i = 0, max = n_bytes_read; i < max; i += 1)
        if (ode.input.buf[i] == (0x1f & 'q'))
            ode.input.exit_requested = true;

    return false;
}
