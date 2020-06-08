#pragma once
#include "utils_std_basics.c"

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


OdePos pos(UInt x, UInt y) {
    return (OdePos) {.x = x, .y = y};
}

OdeSize size(UInt width, UInt height) {
    return (OdeSize) {.width = width, .height = height};
}

OdeRect rect(UInt x, UInt y, UInt width, UInt height) {
    return (OdeRect) {.pos = pos(x, y), .size = size(width, height)};
}
