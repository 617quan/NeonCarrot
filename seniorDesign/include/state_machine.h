#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "frame.h"

typedef enum {
    P1 = 49,    // ascii 1
    P2 = 50,    // ascii 2
    P3 = 51,    // ascii 3
    P4 = 52,    // ascii 4
    MOVING = 53 // ascii 5 - state to represent any time the bot is actively
                // moving so that we can check if emergency stop is pressed
} STATE_TYPE;

typedef enum {
    MOVE_TO_P1 = 63,    // ascii ?
    MOVE_TO_P2 = 64,    // ascii A
    MOVE_TO_P3 = 65,    // ascii B
    MOVE_TO_P4 = 66,    // ascii C
    EMERGENCY_STOP = 67 // ascii D
} MOVE_COMMAND;

class StateMachine {
public:

    StateMachine();
    STATE_TYPE parseCommands(MOVE_COMMAND command);
    STATE_TYPE getCurrState();
    MOVE_COMMAND getCurrCommand();

private:
    void emergencyStop();

    STATE_TYPE curr_state;
    MOVE_COMMAND command;
};

#endif
