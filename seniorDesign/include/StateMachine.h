#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "defines.h"

class StateMachine {
public:

    StateMachine();
    bool parseWebServerInput(MOVE_COMMAND command);
    STATE_TYPE getCurrState();
    MOVE_COMMAND getCurrCommand();
    bool parseUARTInput(MOTOR_COMMAND command);

private:
    STATE_TYPE curr_state;
    bool is_operating;

    void sendMessage();

    static const MOTOR_COMMAND movement_memory[16][12];

    /* IT INDEXES LIKE THIS: movement_memory[movement_commands][movement_index] 
    SO, movement_index TELLS THE SYSTEM WHAT THE NEXT BOT MOVEMENT NEEDS TO BE,
    AND movement_commands TELLS THE SYSTEM WHAT SERIES OF COMMANDS IS CURRENTLY
    BEING EXECUTED. */
    int movement_index;
    int movement_commands;



};

#endif
