#pragma once
#include "utils_libc_deps_basics.c"
#include "ui_ctl.c"

typedef struct OdeUiCtlTextBox {
    OdeUiCtl base;
    Str hint_text;
} OdeUiCtlTextBox;
