#pragma once
#include "utils_std_basics.c"
#include "ui_ctl.c"

typedef struct OdeUiCtlTextBox {
    OdeUiCtl base;
    Str hint_text;
} OdeUiCtlTextBox;
