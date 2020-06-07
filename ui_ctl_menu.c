#pragma once
#include "utils_libc_deps_basics.c"
#include "ui_ctl.c"
#include "ui_ctl_btn.c"

typedef struct OdeUiCtlMenu {
    OdeUiCtl base;
    OdeUiCtlBtns items;
} OdeUiCtlMenu;
