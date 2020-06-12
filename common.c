#pragma once
#include <termios.h>

#include "utils_std_basics.c"
#include "utils_std_mem.c"

#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 256
#define ode_input_buf_size (1 * 1024 * 1024)

typedef enum OdeGlyphStyleFlags {
    ode_glyphstyle_none = 0,
    ode_glyphstyle_bold = 1,
    ode_glyphstyle_italic = 2,
    ode_glyphstyle_underline = 4,
    ode_glyphstyle_strikethru = 8,
    ode_glyphstyle_underline2 = 16,
    ode_glyphstyle_underline3 = 32,
    ode_glyphstyle_overline = 64,
} OdeGlyphStyleFlags;

typedef enum OdeOrientation {
    ode_orient_none,
    ode_orient_horiz,
    ode_orient_vert,
} OdeOrientation;

typedef struct OdeSize {
    U8 width;
    U8 height;
} OdeSize;

typedef struct OdePos {
    U8 x;
    U8 y;
} OdePos;

typedef struct OdeRect {
    OdePos pos;
    OdeSize size;
} OdeRect;

typedef struct OdeRgbaColor {
    Str ansi_esc;
    union {
        struct {
            U8 r;
            U8 g;
            U8 b;
            U8 a;
        };
        U32 rgba;
    };
} OdeRgbaColor;
typedef ·ListOf(OdeRgbaColor) OdeRgbaColors;

typedef struct OdeColored {
    OdeRgbaColor* bg;
    OdeRgbaColor* fg;
    OdeRgbaColor* ul3;
} OdeColored;

typedef struct OdeScreenCell {
    OdeColored color;
    OdeGlyphStyleFlags style;
    union {
        U32 u32;
        U8 bytes[4];
    } rune;
} OdeScreenCell;

struct OdeUiCtl;
struct OdeUiCtlPanel;

typedef struct termios Termios;
struct Ode {
    struct Init {
        Strs argv_paths;
        struct Term {
            Termios orig_attrs;
            Bool did_tcsetattr;
            int tty_fileno;
        } term;
    } init;
    struct Stats {
        UInt num_renders;
        UInt num_outputs;
        UInt last_output_payload;
    } stats;
    struct Input {
        Bool exit_requested;
        Bool screen_resized;
    } input;
    struct Output {
        struct Screen {
            OdeScreenCell real[ode_output_screen_max_width][ode_output_screen_max_height];
            OdeScreenCell prep[ode_output_screen_max_width][ode_output_screen_max_height];
            OdeSize size;
            Str term_esc_cursor_pos[ode_output_screen_max_width][ode_output_screen_max_height];
        } screen;
        OdeRgbaColors colors;
    } output;
    struct Ui {
        struct OdeUiCtlPanel* main;
        struct OdeUiCtlPanel* statusbar;
        struct OdeUiCtlPanel* sidebar_left;
        struct OdeUiCtlPanel* sidebar_right;
        struct OdeUiCtlPanel* sidebar_bottom;
        struct OdeUiCtlPanel* editors;
        struct OdeUiCtlPanel* view_explorer;
        struct OdeUiCtlPanel* view_extensions;
        struct OdeUiCtlPanel* view_search;
        struct OdeUiCtlPanel* view_outline;
        struct OdeUiCtlPanel* view_diags;
        struct OdeUiCtlPanel* view_logs;
        struct OdeUiCtlPanel* view_terminals;
    } ui;
} ode;

void odeDie(CStr const hint, Bool const got_errno);

OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    OdeRgbaColor spec = (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
    for (UInt i = 0; i < ode.output.colors.len; i += 1) {
        OdeRgbaColor* const color = &ode.output.colors.at[i];
        if (color->rgba == spec.rgba)
            return &ode.output.colors.at[i];
    }
    spec.ansi_esc =
        str7(strL(";2;", 3), uIntToStr(r, 1, 10), strL(";", 1), uIntToStr(g, 1, 10), strL(";", 1), uIntToStr(b, 1, 10), strL("m", 1));
    ·append(ode.output.colors, spec);
    return ·last(ode.output.colors);
}

OdePos pos(UInt x, UInt y) {
    return (OdePos) {.x = x, .y = y};
}

OdeSize size(UInt width, UInt height) {
    return (OdeSize) {.width = width, .height = height};
}

OdeRect rect(UInt x, UInt y, UInt width, UInt height) {
    return (OdeRect) {.pos = pos(x, y), .size = size(width, height)};
}

void odeScreenClearRectText(OdeRect const* const rect);
void odeRenderText(Str const text, OdeRect const* const screen_rect, Bool const clear_full_line);
OdeRect odeRender(struct OdeUiCtl* const ctl, OdeRect const screen_rect);
