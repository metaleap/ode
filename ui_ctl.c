#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"

struct OdeUiCtl;
typedef struct OdeUiCtl OdeUiCtl;
typedef ·ListOfPtrs(OdeUiCtl) OdeUiCtls;

typedef void (*OdeUiCtlRenderFunc)(struct OdeUiCtl*, OdeRect*);
typedef Bool (*OdeUiCtlInputFunc)(struct OdeUiCtl*, Str const bytes);

typedef enum OdeUiCtlDocking {
    ode_uictl_dock_none = 0,
    ode_uictl_dock_fill = 1,
    ode_uictl_dock_left = 2,
    ode_uictl_dock_right = 4,
    ode_uictl_dock_top = 8,
    ode_uictl_dock_bottom = 16,
} OdeUiCtlDocking;

struct OdeUiCtl {
    OdeUiCtls ctls;
    struct OdeUiCtl* parent;
    Str text;
    OdeColored color;
    OdeGlyphStyleFlags style;
    OdeUiCtlDocking dock;
    OdeRect rect;
    struct {
        Bool dirty : 1;
        Bool disabled : 1;
        Bool visible : 1;
        Bool focused : 1;
    };
    struct {
        OdeUiCtlRenderFunc render;
        OdeUiCtlInputFunc input;
    } on;
};


OdeUiCtl odeUiCtl(Str const text, OdeUiCtlDocking const dock, OdeRect rect) {
    OdeUiCtl ret_ctl =
        (OdeUiCtl) {.dirty = true, .ctls = (OdeUiCtls)·listOfPtrs(OdeUiCtl, 0, 0), .visible = true, .text = text, .dock = dock, .rect = rect};
    return ret_ctl;
}

void odeUiCtlSetDirty(OdeUiCtl* ctl, Bool const dirty, Bool const propagate) {
    ctl->dirty = dirty;
    if (propagate) {
        for (UInt i = 0; i < ctl->ctls.len; i += 1)
            odeUiCtlSetDirty(ctl->ctls.at[i], dirty, true);
        for (ctl = ctl->parent; ctl != NULL; ctl = ctl->parent)
            odeUiCtlSetDirty(ctl, dirty, false);
    }
}

OdeColored odeUiCtlEffectiveColors(OdeUiCtl const* ctl) {
    OdeColored ret_col = ctl->color;
    while ((ret_col.bg == NULL) || (ret_col.fg == NULL) || (ret_col.ul3 == NULL)) {
        ctl = ctl->parent;
        OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a);
        ret_col.bg = (ret_col.bg != NULL) ? ret_col.bg : (ctl != NULL) ? ctl->color.bg : rgba(0, 0, 0, 0);
        ret_col.fg = (ret_col.fg != NULL) ? ret_col.fg : (ctl != NULL) ? ctl->color.fg : rgba(0, 0, 0, 0);
        ret_col.ul3 = (ret_col.ul3 != NULL) ? ret_col.ul3 : (ctl != NULL) ? ctl->color.ul3 : rgba(0, 0, 0, 0);
    }
    return ret_col;
}
