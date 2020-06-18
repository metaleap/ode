#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"

struct OdeUiCtl;
typedef struct OdeUiCtl OdeUiCtl;
typedef ·ListOfPtrs(OdeUiCtl) OdeUiCtls;

typedef void (*OdeUiCtlRenderFunc)(OdeUiCtl*, OdeRect*);
typedef void (*OdeUiCtlDisposeFunc)(OdeUiCtl*);
typedef Bool (*OdeUiCtlInputFunc)(OdeUiCtl*, MemHeap*, OdeInputs const inputs);

typedef enum OdeUiCtlDocking {
    ode_uictl_dock_none = 0,
    ode_uictl_dock_fill = 1,
    ode_uictl_dock_left = 2,
    ode_uictl_dock_right = 4,
    ode_uictl_dock_top = 8,
    ode_uictl_dock_bottom = 16,
} OdeUiCtlDocking;

struct OdeUiCtl {
    MemHeap* mem;
    OdeUiCtls ctls;
    OdeUiCtl* parent;
    OdeUiCtl* focus;
    Str text;
    OdeColored color;
    OdeRect rect;
    OdeGlyphStyleFlags style;
    OdeUiCtlDocking dock;
    OdeUiViewKind view_kind;
    struct {
        Bool dirty : 1;
        Bool disabled : 1;
        Bool visible : 1;
    };
    struct {
        OdeUiCtlRenderFunc render;
        OdeUiCtlInputFunc input;
        OdeUiCtlDisposeFunc dispose;
    } on;
};


OdeUiCtl odeUiCtl(MemHeap* mem_heap, Str const text, OdeUiCtlDocking const dock,
                  OdeRect rect) {
    OdeUiCtl ret_ctl = (OdeUiCtl) {.dirty = true,
                                   .visible = true,
                                   .text = text,
                                   .dock = dock,
                                   .rect = rect,
                                   .mem = mem_heap};
    return ret_ctl;
}

void odeUiCtlDispose(OdeUiCtl* ctl) {
    if (ctl->on.dispose != NULL)
        ctl->on.dispose(ctl);
    for (UInt i = 0; i < ctl->ctls.len; i += 1)
        odeUiCtlDispose(ctl->ctls.at[i]);
    ctl->ctls.len = 0;
    if (ctl->mem != NULL && (ctl->parent == NULL || ctl->mem != ctl->parent->mem))
        memHeapFree(ctl->mem);
}

void odeUiCtlSetDirty(OdeUiCtl* ctl, Bool const dirty, Bool const propagate_down) {
    ctl->dirty = dirty;
    if (propagate_down)
        for (UInt i = 0; i < ctl->ctls.len; i += 1)
            odeUiCtlSetDirty(ctl->ctls.at[i], dirty, true);
    for (ctl = ctl->parent; ctl != NULL; ctl = ctl->parent)
        odeUiCtlSetDirty(ctl, dirty, false);
}

OdeColored odeUiCtlEffectiveColors(OdeUiCtl const* ctl) {
    OdeColored ret_col = ctl->color;
    while ((ret_col.bg == NULL) || (ret_col.fg == NULL) || (ret_col.ul3 == NULL)) {
        ctl = ctl->parent;
        OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a);
        ret_col.bg = (ret_col.bg != NULL)
                         ? ret_col.bg
                         : (ctl != NULL) ? ctl->color.bg : rgba(0, 0, 0, 0);
        ret_col.fg = (ret_col.fg != NULL)
                         ? ret_col.fg
                         : (ctl != NULL) ? ctl->color.fg : rgba(0, 0, 0, 0);
        ret_col.ul3 = (ret_col.ul3 != NULL)
                          ? ret_col.ul3
                          : (ctl != NULL) ? ctl->color.ul3 : rgba(0, 0, 0, 0);
    }
    return ret_col;
}
