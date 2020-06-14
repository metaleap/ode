#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_main.c"

#define ode_input_buf_size (1 * 1024 * 1024)

Bool odeProcessInput() {
    static U8 buf[ode_input_buf_size];
    static OdeInput inputs_buf[ode_input_buf_size];
    static MemHeap bracketed_paste = (MemHeap) {.cap = (4 * 1024), .kind = mem_heap_pages_malloc};
    static U8 last7[7];

    MemHeap mem_tmp = (MemHeap) {.kind = mem_heap_pages_malloc, .cap = 4 * 1024};
    Int n_bytes_read = read(ode.init.term.tty_fileno, buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            return false;
        else
            odeDie("odeProcessInput: read", true);
    }
    OdeInputs inputs = {.at = &inputs_buf[0], .len = 0};
    if (n_bytes_read > 0) {
        for (UInt i = 0, max = (UInt)n_bytes_read; i < max; i += 1) {
            for (UInt j = 1; j < 7; j += 1)
                last7[j - 1] = last7[j];
            last7[6] = buf[i];

            if (bracketed_paste.ptr != NULL) {
                if (bracketed_paste.len == bracketed_paste.cap) {
                    memHeapAlloc(&bracketed_paste, 1);
                    bracketed_paste.len -= 1;
                }
                bracketed_paste.ptr[bracketed_paste.len] = buf[i];
                bracketed_paste.len += 1;
                if (strEq(&last7[1], strL("\x1b[201~", 6), 6)) {
                    ·assert(mem_tmp.ptr == NULL);
                    mem_tmp.cap = 4096 + memHeapSize(&bracketed_paste, true);
                    U8* const ptr = memHeapAlloc(&mem_tmp, memHeapSize(&bracketed_paste, false));
                    UInt const len = memHeapCopyTo(&bracketed_paste, ptr);
                    memHeapFree(&bracketed_paste);
                    bracketed_paste.ptr = NULL;
                    ·push(inputs, ((OdeInput) {
                                      .kind = ode_input_str,
                                      .of = {.string = (Str) {.len = len - 6, .at = ptr}},
                                  }));
                }
            } else if (((n_bytes_read - i) >= 6) && strEq("\x1b[200~", strL(&buf[i], 6), 6)) {
                memHeapAlloc(&bracketed_paste, 1);
                bracketed_paste.len -= 1;
                i += 5;
            } else {
                if (buf[i] == (0x1f & 'q'))
                    ode.input.exit_requested = true;
                ·push(inputs, ((OdeInput) {
                                  .kind = ode_input_str,
                                  .of = {.string = (Str) {.at = &buf[i], .len = 1}},
                              }));
            }
        }
    }

    Bool ret_dirty =
        (!ode.input.exit_requested) && (inputs.len > 0) && ode.ui.main->ui_panel.base.on.input(&ode.ui.main->ui_panel.base, &mem_tmp, inputs);
    memHeapFree(&mem_tmp);
    return ret_dirty;
}
