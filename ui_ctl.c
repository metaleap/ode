#pragma once
#include "utils_libc_deps_basics.c"
#include "common.c"

typedef void (*OdeUiCtlRenderFunc)(void);

typedef enum OdeUiCtlDocking {
    ode_uictl_dock_none = 0,
    ode_uictl_dock_fill = 1,
    ode_uictl_dock_left = 2,
    ode_uictl_dock_right = 4,
    ode_uictl_dock_top = 8,
    ode_uictl_dock_bottom = 16,
} OdeUiCtlDocking;

typedef struct OdeUiCtl {
    OdeSize size;
    OdePos pos;
    Str text;
    OdeColored color;
    OdeGlyphStyleFlags style;
    OdeUiCtlDocking dock;
    Bool dirty;
    Bool disabled;
    Bool visible;
    Bool focused;
    struct {
        OdeUiCtlRenderFunc render;
    } on;
} OdeUiCtl;
typedef ·ListOfPtrs(OdeUiCtl) OdeUiCtls;


OdeUiCtl odeUiCtl(Str const text, OdeUiCtlDocking const dock, OdePos const pos, OdeSize const size) {
    OdeUiCtl ret_ctl = (OdeUiCtl) {.on = {.render = NULL},
                                   .color = {.bg = NULL, .fg = NULL},
                                   .style = ode_glyphstyle_none,
                                   .dirty = true,
                                   .disabled = false,
                                   .visible = true,
                                   .focused = false,
                                   .text = text,
                                   .dock = dock,
                                   .pos = pos,
                                   .size = size};
    return ret_ctl;
}
