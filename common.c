#pragma once
#include "utils_libc_deps_basics.c"

typedef enum OdeGlyphStyleFlags {
    ode_glyphstyle_none = 0,
    ode_glyphstyle_bold = 2,
    ode_glyphstyle_italic = 4,
    ode_glyphstyle_underline = 8,
    ode_glyphstyle_strikethru = 16,
} OdeGlyphStyleFlags;

typedef struct OdeSize {
    UInt width;
    UInt height;
} OdeSize;

typedef struct OdePos {
    UInt x;
    UInt y;
} OdePos;

typedef struct OdeRgbaColor {
    U8 r;
    U8 g;
    U8 b;
    U8 a;
} OdeRgbaColor;

typedef struct OdeColored {
    OdeRgbaColor bg;
    OdeRgbaColor fg;
} OdeColored;




OdeRgbaColor rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    return (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
}

Bool rgbaEq(OdeRgbaColor const* const c1, OdeRgbaColor const* const c2) {
    return (c1->r == c2->r) && (c1->g == c2->g) && (c1->b == c2->b) && (c1->a == c2->a);
}
