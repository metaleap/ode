#pragma once
#include "utils_libc_deps_basics.c"
#include "core.c"

typedef struct OdeTuiCtl {
    struct Size {
        UInt width;
        UInt height;
    } size;
    struct Pos {
        UInt x;
        UInt y;
    } pos;
    Str text;
    enum {
        ode_tui_ctl_dock_none = 0,
        ode_tui_ctl_dock_fill = 1,
        ode_tui_ctl_dock_left = 2,
        ode_tui_ctl_dock_right = 4,
        ode_tui_ctl_dock_top = 8,
        ode_tui_ctl_dock_bottom = 16,
    } dock;
    Bool focused;
} OdeTuiCtl;
typedef ·ListOf(OdeTuiCtl) OdeTuiCtls;

typedef struct OdeTuiCtlLabel {
    OdeTuiCtl base;
} OdeTuiCtlLabel;

typedef struct OdeTuiCtlBtn {
    Str command;
    Bool toggle;
} OdeTuiCtlBtn;
typedef ·ListOf(OdeTuiCtlBtn) OdeTuiCtlBtns;

typedef struct OdeTuiCtlMenu {
    OdeTuiCtl base;
    OdeTuiCtlBtns items;
} OdeTuiCtlMenu;

typedef struct OdeTuiCtlTextBox {
    OdeTuiCtl base;
    Str hint_text;
} OdeTuiCtlTextBox;

typedef struct OdeTuiCtlTreeView {
    OdeTuiCtl base;
} OdeTuiCtlTreeView;

typedef struct OdeTuiCtlPalette {
    OdeTuiCtl base;
} OdeTuiCtlPalette;

typedef struct OdeTuiCtlEditor {
    OdeTuiCtl base;
} OdeTuiCtlEditor;

typedef struct OdeTuiCtlPanel {
    OdeTuiCtl base;
    OdeTuiCtls ctls;
    UInt focus_idx;
    enum {
        ode_tui_ctl_panel_style_none,
        ode_tui_ctl_panel_style_tabs_h,
        ode_tui_ctl_panel_style_tabs_v,
        ode_tui_ctl_panel_style_headers_h,
        ode_tui_ctl_panel_style_headers_v,
        ode_tui_ctl_panel_style_layout_h,
        ode_tui_ctl_panel_style_layout_v,
        ode_tui_ctl_panel_style_split_h,
        ode_tui_ctl_panel_style_split_v,
    } style;
} OdeTuiCtlPanel;
