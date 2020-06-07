#pragma once
#include "utils_libc_deps_basics.c"
#include "ui_ctl.c"

typedef struct OdeUiCtlPanel {
    OdeUiCtl base;
    OdeUiCtls ctls;
    UInt focus_idx;
    enum {
        ode_uictl_panel_style_none,
        ode_uictl_panel_style_tabs_h,
        ode_uictl_panel_style_tabs_v,
        ode_uictl_panel_style_headers_h,
        ode_uictl_panel_style_headers_v,
        ode_uictl_panel_style_layout_h,
        ode_uictl_panel_style_layout_v,
        ode_uictl_panel_style_split_h,
        ode_uictl_panel_style_split_v,
    } style;
} OdeUiCtlPanel;
