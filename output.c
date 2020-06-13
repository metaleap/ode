#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_main.c"
#include "utils_std_basics.c"
#include "utils_std_mem.c"

void odeScreenClearRectText(OdeRect const* const rect) {
    for (UInt x = rect->pos.x, x_end = (rect->pos.x + rect->size.width); x < x_end; x += 1)
        for (UInt y = rect->pos.y, y_end = (rect->pos.y + rect->size.height); y < y_end; y += 1)
            ode.output.screen.prep[x][y].rune.u32 = 0;
}

void odeRenderText(Str const text, OdeRect const* const screen_rect, Bool const clear_full_line) {
    OdePos pos = screen_rect->pos;
    if (clear_full_line)
        for (UInt x = screen_rect->pos.x + text.len; x < (screen_rect->pos.x + screen_rect->size.width); x += 1)
            ode.output.screen.prep[x][pos.y].rune.u32 = 0;
    for (UInt i = 0; i < text.len; i += 1)
        if (((pos.x + 2) >= (screen_rect->pos.x + screen_rect->size.width)) || text.at[i] == '\n'
            || (i < (text.len - 1) && text.at[1 + i] == '\n')) {
            Str const ellipsis = str("…");
            ode.output.screen.prep[pos.x][pos.y]
                .rune.bytes[strCopyTo((Str) {.at = ode.output.screen.prep[pos.x][pos.y].rune.bytes, .len = 0}, ellipsis).len] = 0;
            break;
        } else {
            ode.output.screen.prep[pos.x][pos.y].rune.u32 = text.at[i];
            pos.x += 1;
        }
}

OdeRect odeRender(OdeUiCtl* const ctl, OdeRect const screen_rect) {
    OdeRect ret_rect = screen_rect;

    OdeRect dst_rect; // if if not dirty, still need ret_rect
    switch (ctl->dock) {
        case ode_uictl_dock_none: {
            dst_rect =
                rect(screen_rect.pos.x + ctl->rect.pos.x, screen_rect.pos.y + ctl->rect.pos.y, ctl->rect.size.width, ctl->rect.size.height);
        } break;
        case ode_uictl_dock_fill: {
            dst_rect = screen_rect;
        } break;
        case ode_uictl_dock_left: {
            UInt const width = uIntMin(ctl->rect.size.width, screen_rect.size.width);
            dst_rect = rect(screen_rect.pos.x, screen_rect.pos.y, width, screen_rect.size.height);
            ret_rect = rect(screen_rect.pos.x + width, screen_rect.pos.y, screen_rect.size.width - width, screen_rect.size.height);
        } break;
        case ode_uictl_dock_right: {
            UInt const width = uIntMin(ctl->rect.size.width, screen_rect.size.width);
            dst_rect = rect((screen_rect.pos.x + screen_rect.size.width) - width, screen_rect.pos.y, width, screen_rect.size.height);
            ret_rect = rect(screen_rect.pos.x, screen_rect.pos.y, screen_rect.size.width - width, screen_rect.size.height);
        } break;
        case ode_uictl_dock_top: {
            UInt const height = uIntMin(ctl->rect.size.height, screen_rect.size.height);
            dst_rect = rect(screen_rect.pos.x, screen_rect.pos.y, screen_rect.size.width, height);
            ret_rect = rect(screen_rect.pos.x, screen_rect.pos.y + height, screen_rect.size.width, screen_rect.size.height - height);
        } break;
        case ode_uictl_dock_bottom: {
            UInt const height = uIntMin(ctl->rect.size.height, screen_rect.size.height);
            dst_rect = rect(screen_rect.pos.x, (screen_rect.pos.y + screen_rect.size.height) - height, screen_rect.size.width, height);
            ret_rect = rect(screen_rect.pos.x, screen_rect.pos.y, screen_rect.size.width, screen_rect.size.height - height);
        } break;
        default: odeDie(strZ(str2(NULL, str("BUG in render caller: invalid ctl->dock "), uIntToStr(NULL, ctl->dock, 1, 10))), false);
    }

    if (ctl->dirty && (dst_rect.pos.x < (screen_rect.pos.x + screen_rect.size.width))
        && (dst_rect.pos.y < (screen_rect.pos.y + screen_rect.size.height))) {
        ctl->dirty = false;
        OdePos dst_pos_max = pos(dst_rect.pos.x + dst_rect.size.width, dst_rect.pos.y + dst_rect.size.height);
        if (ctl->ctls.len == 0)
            for (UInt x = dst_rect.pos.x; x < dst_pos_max.x; x += 1)
                for (UInt y = dst_rect.pos.y; y < dst_pos_max.y; y += 1) {
                    OdeScreenCell* cell = &ode.output.screen.prep[x][y];
                    cell->color = odeUiCtlEffectiveColors(ctl);
                    cell->style = ctl->style;
                }
        ·assert(ctl->on.render != NULL);
        ctl->on.render(ctl, &dst_rect);
    }
    return ret_rect;
}



void odeRenderOutput(OdeUiCtl* ode_ui_main, OdeSize const ode_output_screen_size) {
#define ode_output_screen_buf_size (64 * ode_output_screen_max_width * ode_output_screen_max_height)
    static U8 out_buf[ode_output_screen_buf_size];
    static Bool cells_dirty[ode_output_screen_max_width][ode_output_screen_max_height];
    static OdeScreenCell state = {.color = {.bg = NULL, .fg = NULL, .ul3 = NULL}};
    static OdeSize screen_size = (OdeSize) {0, 0};
    Bool const resized = (screen_size.width != ode_output_screen_size.width) || (screen_size.height != ode_output_screen_size.height);
    if (resized) {
        if (screen_size.height != 0 && screen_size.width != 0) // actual-resize instead of mere init?
            odeUiMainOnResized(&screen_size, &ode_output_screen_size);
        screen_size = ode_output_screen_size;
        odeUiCtlSetDirty(ode_ui_main, true, true);
        for (UInt x = 0; x < screen_size.width; x += 1)
            for (UInt y = 0; y < screen_size.height; y += 1) {
                ode.output.screen.prep[x][y].rune.u32 = 0;
                cells_dirty[x][y] = true;
            }
    }
    if (ode_ui_main->dirty)
        ode.stats.num_renders += 1;
    odeRender(ode_ui_main, rect(0, 0, screen_size.width, screen_size.height));

    Bool got_dirty_cells = resized;
    if (!got_dirty_cells)
        for (UInt x = 0; x < screen_size.width; x += 1)
            for (UInt y = 0; y < screen_size.height; y += 1) {
                OdeScreenCell* prep = &ode.output.screen.prep[x][y];
                OdeScreenCell* real = &ode.output.screen.real[x][y];
                cells_dirty[x][y] = (prep->rune.u32 != real->rune.u32) || (prep->style != real->style) || (prep->color.bg != real->color.bg)
                                    || (prep->color.fg != real->color.fg);
                got_dirty_cells |= cells_dirty[x][y];
            }

    if (got_dirty_cells) {
        ·ListOf(U8) buf = {.len = 0, .cap = ode_output_screen_buf_size, .at = &out_buf[0]};
#define ·out(·the·str)                                                                                                                       \
    do {                                                                                                                                     \
        buf.len = strCopyTo((Str) {.at = buf.at, .len = buf.len}, (·the·str)).len;                                                           \
    } while (0)

        ode.stats.num_outputs += 1;
        OdePos last = {.x = 0, .y = 0};
        for (UInt y = 0; y < screen_size.height; y += 1)
            for (UInt x = 0; x < screen_size.width; x += 1)
                if (cells_dirty[x][y]) {
                    OdeScreenCell* const cell = &ode.output.screen.prep[x][y];

                    // position cursor, unless we're next to previous
                    if ((x != (last.x + 1)) || (y != last.y))
                        ·out(ode.output.screen.term_esc_cursor_pos[x][y]);

                    // handle styles before colors, because here we might issue a reset-attrs
                    Bool styles_reset = false;
                    if (cell->style != state.style) {
                        static CStr ansis[97] = {
                            [ode_glyphstyle_none] = term_esc "0m",         [ode_glyphstyle_bold] = term_esc "1m",
                            [ode_glyphstyle_italic] = term_esc "3m",       [ode_glyphstyle_underline] = term_esc "4m",
                            [ode_glyphstyle_strikethru] = term_esc "9m",   [ode_glyphstyle_underline2] = term_esc "21m",
                            [ode_glyphstyle_underline3] = term_esc "4:3m", [ode_glyphstyle_overline] = term_esc "53m",
                        };
                        state.style = cell->style;
                        if (ansis[state.style] == NULL || ansis[state.style][0] == 0) {
                            Str ansi = newStr(NULL, 0, 24);
                            for (OdeGlyphStyleFlags st = ode_glyphstyle_bold; st <= ode_glyphstyle_overline; st += st)
                                if ((state.style & st) == st) {
                                    for (UInt i = ((ansi.len == 0) ? 0 : 2); ansis[st][i] != 'm'; i += 1)
                                        ·push(ansi, ansis[st][i]);
                                    ·push(ansi, ';');
                                }
                            ansi.at[ansi.len - 1] = 'm';
                            ansi.at[ansi.len] = 0;
                            ansis[state.style] = strZ(ansi);
                        }
                        ·out(str(ansis[state.style]));
                        styles_reset = (state.style == 0);
                    }
                    for (UInt i = 0; i < 3; i += 1) { // colors: 0=bg, 1=fg, 2=ul3
                        OdeRgbaColor** col_cur = (i == 2) ? &state.color.ul3 : (i == 1) ? &state.color.fg : &state.color.bg;
                        OdeRgbaColor* const col_cell = (i == 2) ? cell->color.ul3 : (i == 1) ? cell->color.fg : cell->color.bg;
                        if (styles_reset || col_cell != *col_cur) {
                            CStr const ansi_reset = (i == 2) ? (term_esc "59m") : (i == 1) ? (term_esc "39m") : (term_esc "49m");
                            CStr const ansi_set = (i == 2) ? (term_esc "58") : (i == 1) ? (term_esc "38") : (term_esc "48");
                            *col_cur = col_cell;
                            if (col_cell->rgba == 0)
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
                    ·out((rune_str.len == 0) ? strL(" ", 1) : rune_str);
                    last = pos(x, y);
                }

        if (buf.len > 0) {
            ode.stats.last_output_payload = buf.len;
            if (write(ode.init.term.tty_fileno, buf.at, buf.len) != (Int)buf.len)
                odeDie("odeRenderOutput: write", true);
        }
    }
}
