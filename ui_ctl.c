#pragma once
#include "utils_std_basics.c"
#include "common.c"

struct OdeUiCtl;
typedef void (*OdeUiCtlRenderFunc)(struct OdeUiCtl const* const, OdeRect const* const);

typedef enum OdeUiCtlDocking {
    ode_uictl_dock_none = 0,
    ode_uictl_dock_fill = 1,
    ode_uictl_dock_left = 2,
    ode_uictl_dock_right = 4,
    ode_uictl_dock_top = 8,
    ode_uictl_dock_bottom = 16,
} OdeUiCtlDocking;

typedef struct OdeUiCtl {
    OdeRect rect;
    Str text;
    OdeColored color;
    OdeGlyphStyleFlags style;
    OdeUiCtlDocking dock;
    struct {
        Bool dirty : 1;
        Bool disabled : 1;
        Bool visible : 1;
        Bool focused : 1;
        Bool has_ctls : 1;
    };
    struct {
        OdeUiCtlRenderFunc render;
    } on;
} OdeUiCtl;
typedef ·ListOfPtrs(OdeUiCtl) OdeUiCtls;


OdeUiCtl odeUiCtl(Str const text, OdeUiCtlDocking const dock, OdeRect rect) {
    OdeUiCtl ret_ctl = (OdeUiCtl) {.dirty = true, .visible = true, .text = text, .dock = dock, .rect = rect};
    return ret_ctl;
}
