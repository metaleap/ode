#pragma once
#include "common.c"
#include "utils_libc_deps_basics.c"

typedef struct OdeUiCtl {
    OdeSize size;
    OdePos pos;
    Str text;
    OdeColored color;
    OdeGlyphStyleFlags style;
    enum {
        ode_uictl_dock_none = 0,
        ode_uictl_dock_fill = 1,
        ode_uictl_dock_left = 2,
        ode_uictl_dock_right = 4,
        ode_uictl_dock_top = 8,
        ode_uictl_dock_bottom = 16,
    } dock;
    Bool focused;
} OdeUiCtl;
typedef ·ListOf(OdeUiCtl) OdeUiCtls;
