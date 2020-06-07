#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"
#include "ui_ctl_panel.c"

void odeUiInitSidebars() {
    ode.ui.sidebar_left = odeUiCtlPanel(odeUiCtl(str("Left Sidebar"), ode_uictl_dock_left, (OdePos) {0, 0}, (OdeSize) {16, 100}),
                                        ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.sidebar_right = odeUiCtlPanel(odeUiCtl(str("Right Sidebar"), ode_uictl_dock_right, (OdePos) {0, 0}, (OdeSize) {16, 100}),
                                         ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.sidebar_bottom = odeUiCtlPanel(odeUiCtl(str("Bottom Sidebar"), ode_uictl_dock_bottom, (OdePos) {0, 0}, (OdeSize) {100, 16}),
                                          ode_orient_none, ode_uictl_panel_none, 0);
    ode.ui.editors = odeUiCtlPanel(odeUiCtl(str("Editors Region"), ode_uictl_dock_fill, (OdePos) {0, 0}, (OdeSize) {100, 100}),
                                   ode_orient_none, ode_uictl_panel_none, 0);
}
