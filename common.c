#pragma once
#pragma clang diagnostic ignored "-Wfixed-enum-extension"
#include <termios.h>

#include "utils_std_basics.c"
#include "utils_std_mem.c"

#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 256
#define term_esc "\x1b["

typedef enum OdeInputKind {
    ode_input_str,
    ode_input_mouse,
    ode_input_hotkey,
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

typedef enum OdeKeyMod {
    ode_key_mod_none = 0,
    ode_key_mod_ctl = 1,
    ode_key_mod_alt = 2,
    ode_key_mod_shift = 4,
} OdeKeyMod;

typedef enum OdeKeyKnown : U64 {
    ode_key_none = 0,
    ode_key_tab = 0x09,
    ode_key_f1 = 0x1b4f50,
    ode_key_f2 = 0x1b4f51,
    ode_key_f3 = 0x1b4f52,
    ode_key_f4 = 0x1b4f53,
    ode_key_f5 = 0x1b5b31357e,
    ode_key_f6 = 0x1b5b31377e,
    ode_key_f7 = 0x1b5b31387e,
    ode_key_f8 = 0x1b5b31397e,
    ode_key_f9 = 0x1b5b32307e,
    ode_key_f10 = 0x1b5b32317e,
    ode_key_f11 = 0x1b5b32337e,
    ode_key_f12 = 0x1b5b32347e,
    ode_key_enter = 0x0d,
    ode_key_esc = 0x1b,
    ode_key_back = 0x7f,
    ode_key_arr_l = 0x1b5b44,
    ode_key_arr_d = 0x1b5b42,
    ode_key_arr_u = 0x1b5b41,
    ode_key_arr_r = 0x1b5b43,
    ode_key_end = 0x1b5b46,
    ode_key_home = 0x1b5b48,
    ode_key_ins = 0x1b5b327e,
    ode_key_del = 0x1b5b337e,
    ode_key_pgup = 0x1b5b357e,
    ode_key_pgdn = 0x1b5b367e,
} OdeKeyKnown;

typedef struct OdeUiFlags {
    Bool dirty : 1;
    Bool disabled : 1;
    Bool hidden : 1;
    Bool selected : 1;
} OdeUiFlags;

typedef enum OdeUiViewKind {
    ode_ui_view_none,
    ode_ui_view_explorer,
    ode_ui_view_extensions,
    ode_ui_view_issues,
    ode_ui_view_notifications,
    ode_ui_view_outline,
    ode_ui_view_output,
    ode_ui_view_search,
    ode_ui_view_terminal,
} OdeUiViewKind;

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

typedef struct OdeHotKey {
    Str title;
    Str esc_seq;
    struct {
        OdeKeyKnown key;
        OdeKeyMod mod;
        Bool reserved : 1;
    };
} OdeHotKey;
typedef ·ListOf(OdeHotKey) OdeHotKeys;

typedef struct OdeMouseState {
    OdePos pos;
    struct {
        Bool left;
        Bool right;
        Bool mid;
    } btn_down;
    Bool dragging : 1;
} OdeMouseState;

struct OdeCmd;
typedef Any (*OdeCmdHandler)(struct OdeCmd const* const, Any const, UInt const);
typedef struct OdeCmd {
    Str id;
    Str text;
    U8 menu_mnemonic;
    OdeHotKey* hotkey;
    OdeHotKey* hotkey_chord;
    OdeCmdHandler handler;
} OdeCmd;
typedef ·ListOf(OdeCmd) OdeCmds;

typedef void (*OdeUiEventHandler)(Any, UInt);
typedef ·ListOf(OdeUiEventHandler) OdeUiEventHandlers;
typedef struct OdeUiEvent {
    OdeUiEventHandlers handlers;
} OdeUiEvent;

typedef struct OdeUiListProvider {
    Any self;
    struct {
        OdeUiEvent* changed;
    } on;
    ºBool (*hasSubItems)(Any item);
    ·ListOf(Any) (*subItems)(Any item);
    Strs (*columns)();
    Strs (*itemCmdIds)(Any item);
    Str (*itemText)(Any item, UInt col_idx);
    Bool (*itemEditable)(Any item, UInt col_idx);
    OdeUiFlags (*itemFlags)(Any item, UInt col_idx);
    OdeGlyphStyleFlags (*itemStyle)(Any item, UInt col_idx);
    OdeColored (*itemColors)(Any item, UInt col_idx);
} OdeUiListProvider;

typedef struct OdeInput {
    union OdeInputOf {
        Str string;
        struct OdeInputMouse {
            Bool scroll : 1;
            Bool down : 1;
            Bool btn_l : 1;
            Bool btn_m : 1;
            Bool btn_r : 1;
            Bool did_drop : 1;
            Bool did_move : 1;
            Bool did_click : 1;
        } mouse;
        OdeKeyKnown key;
    } of;
    OdeInputKind kind;
    OdeKeyMod mod_key;
} OdeInput;
typedef ·SliceOf(OdeInput) OdeInputs;




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
        OdeMouseState mouse;
        struct {
            OdeCmds commands;
            OdeHotKeys hotkeys;
        } all;
        UInt hotkeys_idx_esc;
        Bool exit_requested;
        Bool screen_resized;
    } input;
    struct Output {
        struct Screen {
            OdeScreenCell real[ode_output_screen_max_width]
                              [ode_output_screen_max_height];
            OdeScreenCell prep[ode_output_screen_max_width]
                              [ode_output_screen_max_height];
            OdeSize size;
            Str term_esc_cursor_pos[ode_output_screen_max_width]
                                   [ode_output_screen_max_height];
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




void odeDie(CStr const hint, Bool const got_errno);

OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    OdeRgbaColor spec = (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
    for (UInt i = 0; i < ode.output.colors.len; i += 1) {
        OdeRgbaColor* const color = &ode.output.colors.at[i];
        if (color->rgba == spec.rgba)
            return &ode.output.colors.at[i];
    }
    spec.ansi_esc = str7(NULL, strL(";2;", 3), uIntToStr(NULL, r, 1, 10), strL(";", 1),
                         uIntToStr(NULL, g, 1, 10), strL(";", 1),
                         uIntToStr(NULL, b, 1, 10), strL("m", 1));
    ·append(ode.output.colors, spec);
    return ·last(ode.output.colors);
}

OdePos pos(UInt const x, UInt const y) {
    return (OdePos) {.x = x, .y = y};
}

Bool posEql(OdePos const p1, OdePos const p2) {
    return (p1.x == p2.x) && (p1.y == p2.y);
}

OdeSize size(UInt const width, UInt const height) {
    return (OdeSize) {.width = width, .height = height};
}

OdeRect rect(UInt const x, UInt const y, UInt const width, UInt const height) {
    return (OdeRect) {.pos = pos(x, y), .size = size(width, height)};
}

MemHeap* odeMem(UInt const page_size) {
    MemHeap mem = (MemHeap) {.kind = mem_heap_pages_malloc, .cap = page_size};
    MemHeap* ret_mem = ·new(MemHeap, &mem);
    *ret_mem = mem;
    return ret_mem;
}

Str odeEnv(Str const name) {
    for (UInt i = 0; i < ode.init.env.names.len; i += 1)
        if (strEql(name, ode.init.env.names.at[i]))
            return ode.init.env.values.at[i];
    return ·len0(U8);
}

OdeCmd* odeCmd(Str const cmd_id, OdeCmdHandler const cmd_handler) {
    for (UInt i = 0; i < ode.input.all.commands.len; i += 1)
        if (strEql(cmd_id, ode.input.all.commands.at[i].id))
            return &ode.input.all.commands.at[i];
    ·assert(cmd_handler != NULL);
    ·append(ode.input.all.commands, ((OdeCmd) {.id = cmd_id, .handler = cmd_handler}));
    return ·last(ode.input.all.commands);
}

OdeKeyMod odeModKeyOf(Bool const ctl, Bool const alt, Bool const shift) {
    return (shift ? ode_key_mod_shift : ode_key_mod_none)
           | (ctl ? ode_key_mod_ctl : ode_key_mod_none)
           | (alt ? ode_key_mod_alt : ode_key_mod_none);
}

Bool odeModKeyHas(OdeKeyMod const mod, OdeKeyMod const must) {
    return ·fHas(mod, must);
}

OdeHotKey* odeHotKey(OdeKeyKnown const key, OdeKeyMod const mod) {
    ·forEach(OdeHotKey, hk, ode.input.all.hotkeys, {
        if (hk->key == key && hk->mod == mod)
            return hk;
    });
    return NULL;
}

UInt odeUiEventSub(OdeUiEvent* const event, OdeUiEventHandler const handler) {
    ·append(event->handlers, handler);
    return event->handlers.len - 1;
}

void odeUiEventUnSub(OdeUiEvent* const event, UInt const handler_idx) {
    for (UInt i = handler_idx + 1; i < event->handlers.len; i += 1)
        event->handlers.at[i - 1] = event->handlers.at[i];
    event->handlers.len -= 1;
}

void odeUiEventPub(OdeUiEvent* const event, Any const args, UInt const args_len) {
    for (UInt i = 0; i < event->handlers.len; i += 1)
        event->handlers.at[i](args, args_len);
}

void odeScreenClearRectText(OdeRect const* const rect);
void odeRenderText(Str const text, OdeRect const* const screen_rect,
                   Bool const clear_full_line);
struct OdeUiCtl;
typedef struct OdeUiCtl OdeUiCtl;
OdeRect odeRender(OdeUiCtl* const ctl, OdeRect const screen_rect);
