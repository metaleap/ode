#pragma once
#include "utils_libc_deps_basics.c"

typedef enum OdeGlyphStyleFlags {
    ode_glyphstyle_none = 0,
    ode_glyphstyle_bold = 2,
    ode_glyphstyle_italic = 4,
    ode_glyphstyle_underline = 8,
    ode_glyphstyle_strikethru = 16,
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
} OdeColored;
