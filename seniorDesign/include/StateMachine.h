#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "defines.h"

class StateMachine {
public:

    StateMachine();
    STATE_TYPE parseCommands(MOVE_COMMAND command);
    STATE_TYPE getCurrState();
    MOVE_COMMAND getCurrCommand();

private:
    STATE_TYPE curr_state;
    MOVE_COMMAND command;

    MOVE_COMMAND movement_memory[16][15];

};

#endif
