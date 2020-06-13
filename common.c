#pragma once
#include <termios.h>

#include "utils_std_basics.c"
#include "utils_std_mem.c"

#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 256


typedef enum OdeInputKind {
    ode_input_bytes,
    ode_input_mouse,
    ode_input_key,
} OdeInputKind;

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

struct OdeCmd;
typedef PtrAny (*OdeCmdHandler)(struct OdeCmd const* const, PtrAny const, UInt const);
typedef struct OdeCmd {
    Str id;
    OdeCmdHandler handler;
} OdeCmd;
typedef ·ListOf(OdeCmd) OdeCmds;

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

struct OdeUiSidebar;
struct OdeUiMain;
struct OdeUiStatusbar;
struct OdeUiSidebar;
struct OdeUiSidebar;
struct OdeUiSidebar;
struct OdeUiEditors;
struct OdeUiViewExplorer;
struct OdeUiViewExtensions;
struct OdeUiViewSearch;
struct OdeUiViewOutline;
struct OdeUiViewDiags;
struct OdeUiViewLogOutput;
struct OdeUiViewTerminal;

typedef struct termios Termios;
struct Ode {
    struct Init {
        struct Env {
            Strs names;
            Strs values;
        } env;
        struct Fs {
            Str cur_dir_path;
            Str home_dir_path;
            Strs argv_dir_paths;
            Strs argv_file_paths;
        } fs;
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
        struct Mouse {
            OdePos pos;
            struct {
                Bool btn_l_down : 1;
                Bool btn_r_down : 1;
                Bool btn_m_down : 1;
                Bool dragging : 1;
            };
        } mouse;
        OdeCmds all_commands;
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
        struct OdeUiMain* main;
        struct OdeUiStatusbar* statusbar;
        struct OdeUiSidebar* sidebar_left;
        struct OdeUiSidebar* sidebar_right;
        struct OdeUiSidebar* sidebar_bottom;
        struct OdeUiEditors* editors;
    } ui;
} ode;

typedef struct OdeInput {
    union OdeInputOf {
        Str key_str;
        struct OdeInputMouse {
            Bool scroll_up : 1;
            Bool scroll_down : 1;
            Bool click : 1;
            Bool mouse_down : 1;
            Bool mouse_up : 1;
            Bool btn_l : 1;
            Bool btn_m : 1;
            Bool btn_r : 1;
        } mouse;
    } of;
    OdeInputKind kind;
    struct {
        Bool ctrl : 1;
        Bool alt : 1;
        Bool shift : 1;
    } mod_key;
} OdeInput;
typedef ·SliceOf(OdeInput) OdeInputs;



void odeDie(CStr const hint, Bool const got_errno);

OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    OdeRgbaColor spec = (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
    for (UInt i = 0; i < ode.output.colors.len; i += 1) {
        OdeRgbaColor* const color = &ode.output.colors.at[i];
        if (color->rgba == spec.rgba)
            return &ode.output.colors.at[i];
    }
    spec.ansi_esc = str7(NULL, strL(";2;", 3), uIntToStr(NULL, r, 1, 10), strL(";", 1), uIntToStr(NULL, g, 1, 10), strL(";", 1),
                         uIntToStr(NULL, b, 1, 10), strL("m", 1));
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

Str odeEnv(Str const name) {
    for (UInt i = 0; i < ode.init.env.names.len; i += 1)
        if (strEql(name, ode.init.env.names.at[i]))
            return ode.init.env.values.at[i];
    return ·len0(U8);
}

OdeCmd* odeCmd(Str const cmd_id, OdeCmdHandler const cmd_handler) {
    for (UInt i = 0; i < ode.input.all_commands.len; i += 1)
        if (strEql(cmd_id, ode.input.all_commands.at[i].id))
            return &ode.input.all_commands.at[i];
    ·assert(cmd_handler != NULL);
    ·append(ode.input.all_commands, ((OdeCmd) {.id = cmd_id, .handler = cmd_handler}));
    return ·last(ode.input.all_commands);
}

void odeScreenClearRectText(OdeRect const* const rect);
void odeRenderText(Str const text, OdeRect const* const screen_rect, Bool const clear_full_line);
struct OdeUiCtl;
OdeRect odeRender(struct OdeUiCtl* const ctl, OdeRect const screen_rect);
