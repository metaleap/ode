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
    static Bool is_very_first_render = true;
    renderControl(&ode.ui.main.base, (OdePos) {.x = 0, .y = 0}, ode.output.screen.size);

    static Bool dirty[ode_output_screen_max_width][ode_output_screen_max_height];
    Bool got_dirty_cells = is_very_first_render;
    for (UInt x = 0; x < ode.output.screen.size.width; x += 1)
        for (UInt y = 0; y < ode.output.screen.size.height; y += 1) {
            OdeScreenCell* prep = &ode.output.screen.prep.cells[x][y];
            OdeScreenCell* real = &ode.output.screen.real.cells[x][y];
            dirty[x][y] = is_very_first_render || (prep->rune.u32 != real->rune.u32) || (prep->style != real->style)
                          || (prep->color.bg != real->color.bg) || (prep->color.fg != real->color.fg);
            got_dirty_cells |= dirty[x][y];
        }

#define ode_output_screen_buf_size (64 * ode_output_screen_max_width * ode_output_screen_max_height)
    static U8 out_buf[ode_output_screen_buf_size];
    if (got_dirty_cells) {
        ·ListOf(U8) buf = {.len = 0, .cap = ode_output_screen_buf_size, .at = &out_buf[0]};
#define ·out(·the·str)                                                                                                                       \
    do {                                                                                                                                     \
        buf.len = strCopyTo((Str) {.at = buf.at, .len = buf.len}, (·the·str)).len;                                                           \
    } while (0)

        OdePos last = {.x = 0, .y = 0};
        static OdeScreenCell cur = {.color = {.bg = NULL, .fg = NULL, .ul3 = NULL}};
        for (UInt y = 0; y < ode.output.screen.size.height; y += 1)
            for (UInt x = 0; x < ode.output.screen.size.width; x += 1)
                if (dirty[x][y]) {
                    OdeScreenCell* const cell = &ode.output.screen.prep.cells[x][y];

                    // position cursor, unless we're next to previous
                    if ((x != (last.x + 1)) || (y != last.y))
                        ·out(ode.output.screen.term_esc_cursor_pos[x][y]);

                    // handle styles before colors, because here we might issue a reset-attrs
                    Bool styles_reset = false;
                    if (cell->style != cur.style || is_very_first_render) {
                        static CStr ansis[97] = {
                            [ode_glyphstyle_none] = "",
                            [ode_glyphstyle_bold] = term_esc "1m",
                            [ode_glyphstyle_italic] = term_esc "3m",
                            [ode_glyphstyle_underline] = term_esc "4m",
                            [ode_glyphstyle_strikethru] = term_esc "9m",
                            [ode_glyphstyle_underline2] = term_esc "21m",
                            [ode_glyphstyle_underline3] = term_esc "4:3m",
                            [ode_glyphstyle_overline] = term_esc "53m",
                        };
                        if (str(ansis[ode_glyphstyle_none]).len == 0) {
                            ansis[ode_glyphstyle_none] = term_esc "0m";
                            // #define ode_glyphstyle_count 7
                            // OdeGlyphStyleFlags flags[ode_glyphstyle_count];
                            // for (UInt i = 0, st0 = ode_glyphstyle_bold; st0 <= ode_glyphstyle_overline; st0 =
                            // st0 + st0) {
                            //     flags[i] = st0;
                            //     i += 1;
                            // }
                        }

                        cur.style = cell->style;
                        if (ansis[cur.style] == NULL)
                            odeDie(strZ(str2(str("TODO add glyphstyle to ansis: "), uIntToStr(cur.style, 1, 10))), false);
                        ·out(str(ansis[cur.style]));
                        styles_reset = (cur.style == 0);
                    }
                    for (UInt i = 0; i < 3; i += 1) { // colors: 0=bg, 1=fg, 2=ul3
                        OdeRgbaColor** col_cur = (i == 2) ? &cur.color.ul3 : (i == 1) ? &cur.color.fg : &cur.color.bg;
                        OdeRgbaColor* const col_cell = (i == 2) ? cell->color.ul3 : (i == 1) ? cell->color.fg : cell->color.bg;
                        CStr const ansi_reset = (i == 2) ? (term_esc "59m") : (i == 1) ? (term_esc "39m") : (term_esc "49m");
                        CStr const ansi_set = (i == 2) ? (term_esc "58") : (i == 1) ? (term_esc "38") : (term_esc "48");
                        if (styles_reset || col_cell != *col_cur) {
                            *col_cur = col_cell;
                            if (col_cell == NULL)
                                ·out(strL(ansi_reset, 2 + 3));
                            else {
                                ·out(strL(ansi_set, 2 + 2));
                                ·out(col_cell->ansi_esc);
                            }
                        }
                    }

                    Str rune_str = (cell->rune.u32 == 0) ? ·len0(U8) : (Str) {.at = cell->rune.bytes, .len = 4};
                    if (rune_str.len != 0)
                        for (UInt i = 0; i < 4; i += 1)
                            if (cell->rune.bytes[i] == 0) {
                                rune_str.len = i;
                                break;
                            }
                    ·out((rune_str.len == 0) ? strL(".", 1) : rune_str);
                    last = (OdePos) {.x = x, .y = y};
                }

        // odeDie(strZ(uIntToStr(buf.len, 1, 10)), false);
        // if ((buf.len > 0) && (write(STDOUT_FILENO, buf.at, buf.len) != (Int)buf.len))
        //     odeDie("odeRenderOutput: write", true);
    }
    is_very_first_render = false;
}
