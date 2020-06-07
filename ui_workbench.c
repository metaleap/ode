#pragma once
#include "common.c"
#include "core.c"
#include "ui_ctl.c"

void odeUiWorkbenchInit() {
    ode.ui.base.text = str("ode");
    ode.ui.base.dock = ode_uictl_dock_top | ode_uictl_dock_left;
    ode.ui.base.pos = (OdePos) {.x = 0, .y = 0};
    ode.ui.base.size = ode.output.screen.size;
    ode.ui.base.focused = true;
    ode.ui.focus_idx = 0;
}
