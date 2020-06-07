#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "utils_libc_deps_basics.c"
#include "utils_libc_deps_mem.c"

void renderControl(OdeUiCtl* const ctl, OdePos const screen_rect_pos, OdeSize const screen_rect_size) {
    if (!ctl->dirty)
        return;

    UInt const min_x = screen_rect_pos.x;
    UInt const min_y = screen_rect_pos.y;
    UInt const max_x = screen_rect_pos.x + screen_rect_size.width;
    UInt const max_y = screen_rect_pos.y + screen_rect_size.height;
    for (UInt x = min_x; x < max_x; x += 1)
        for (UInt y = min_y; y < max_y; y += 1) {
            OdeScreenCell* cell = &ode.output.screen.prep.cells[x][y];
            cell->color = ctl->color;
            cell->style = ctl->style;
        }
}

void odeRenderOutput() {
    renderControl(&ode.ui.main.base, (OdePos) {.x = 0, .y = 0}, ode.output.screen.size);

    static Bool dirty[ode_output_screen_max_width][ode_output_screen_max_height];
    Bool got_dirty_cells = false;
    for (UInt x = 0; x < ode.output.screen.size.width; x += 1)
        for (UInt y = 0; y < ode.output.screen.size.height; y += 1) {
            OdeScreenCell* prep = &ode.output.screen.prep.cells[x][y];
            OdeScreenCell* real = &ode.output.screen.real.cells[x][y];
            dirty[x][y] = (prep->rune.u32 != real->rune.u32) || (prep->style != real->style) || (prep->color.bg != real->color.bg)
                          || (prep->color.fg != real->color.fg);
            got_dirty_cells |= dirty[x][y];
        }

#define ode_output_screen_buf_size (64 * ode_output_screen_max_width * ode_output_screen_max_height)
    static U8 out_buf[ode_output_screen_buf_size];
    if (got_dirty_cells) {
        ·ListOf(U8) buf = {.len = 0, .cap = ode_output_screen_buf_size, .at = &out_buf[0]};
#define ·out(·the·str)                                                                                                                       \
    { buf.len = strCopyTo((Str) {.at = buf.at, .len = buf.len}, (·the·str)).len; }

        static OdeScreenCell cur = {.color = {.bg = NULL, .fg = NULL}};
        if (cur.color.bg == NULL && cur.color.fg == NULL)
            cur = ode.output.screen.real.cells[0][0];
        for (UInt x = 0; x < ode.output.screen.size.width; x += 1)
            for (UInt y = 0; y < ode.output.screen.size.height; y += 1)
                if (dirty[x][y]) {
                    OdeScreenCell* const cell = &ode.output.screen.prep.cells[x][y];

                    ·out(ode.output.screen.term_esc_cursor_pos[x][y]);
                    if (cell->color.bg != cur.color.bg) {
                        cur.color.bg = cell->color.bg;
                        ·out(strL(term_esc "48", 2 + 2));
                        ·out(cell->color.bg->ansi_esc);
                    }
                    if (cell->color.fg != cur.color.fg) {
                        cur.color.fg = cell->color.fg;
                        ·out(strL(term_esc "38", 2 + 2));
                        ·out(cell->color.fg->ansi_esc);
                    }
                    ·out(((Str) {.len = (cell->rune.u32 < 256) ? 1 : (cell->rune.u32 < 65536) ? 2 : (cell->rune.u32 < 16777216) ? 3 : 4,
                                 .at = (cell->rune.u32 == 0) ? strL(".", 1).at : cell->rune.bytes}));
                }
        if ((buf.len > 0) && (write(STDOUT_FILENO, buf.at, buf.len) != (Int)buf.len))
            odeDie("odeRenderOutput: write", true);
    }
}
