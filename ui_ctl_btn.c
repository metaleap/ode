#pragma once
#include "utils_std_basics.c"
#include "ui_ctl.c"

typedef struct OdeUiCtlBtn {
    OdeUiCtl base;
    Str command;
    Bool toggle;
} OdeUiCtlBtn;
typedef ·ListOf(OdeUiCtlBtn) OdeUiCtlBtns;
