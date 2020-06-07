#pragma once
#include "core.c"

void odeRenderOutput() {
    Bool got_dirty_cells = false;

    for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
        for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
            OdeScreenCell* dst = &ode.output.screen.dst.cells[x][y];
            OdeScreenCell* src = &ode.output.screen.src.cells[x][y];
            dst->dirty = (src->rune != dst->rune) || (!rgbaEq(&src->color.bg, &dst->color.bg)) || (!rgbaEq(&src->color.fg, &dst->color.fg));
            got_dirty_cells |= dst->dirty;
        }

    printf("%d ", got_dirty_cells);
    fflush(stdout);
    if (got_dirty_cells) {
        Str buf = (Str) {.len = 0, .at = &ode.output.buf[0]};
        for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
            for (UInt y = 0; y < ode_output_screen_max_height; y += 1)
                if (ode.output.screen.dst.cells[x][y].dirty) {
                    // TODO: append positioning / formatting escapes and the cell's rune
                }
        if (buf.len > 0) {
            Int const n_written = write(STDOUT_FILENO, buf.at, buf.len);
            if (n_written < 0 || ((UInt)n_written) != buf.len)
                odeDie("odeRenderOutput: write");
        }
    }
}
