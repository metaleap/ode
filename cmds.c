#pragma once
#include "utils_std_basics.c"
#include "common.c"

OdeCmd* odeCmd(Str const cmd_id, OdeCmdHandler const cmd_handler) {
    for (UInt i = 0; i < ode.input.all_commands.len; i += 1)
        if (strEql(cmd_id, ode.input.all_commands.at[i].id))
            return &ode.input.all_commands.at[i];
    ·assert(cmd_handler != NULL);
    ·append(ode.input.all_commands, ((OdeCmd) {.id = cmd_id, .handler = cmd_handler}));
    return ·last(ode.input.all_commands);
}
