/********** StateMachine.h **********
 * 
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Defines the StateMachine class which manages the high level behavior of
 * the robot. The state machine interprets commands received from the web
 * interface and coordinates the sequence of MOTOR_COMMAND instructions that
 * must be sent to the motor control ESP boards.
 *
 * This class tracks the robot's current state, determines whether the system
 * is actively executing a movement sequence, and advances through a predefined
 * table of motor commands that implement each high level movement request.
 *
 ****************************************/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "defines.h"

class StateMachine {
public:

    StateMachine();

    /* Only takes inputs from Web Server */
    bool parseWebServerInput(MOVE_COMMAND command); 

    /* Only takes inputs from Motor ESPs */
    bool parseUARTInput(MOTOR_COMMAND command);

    STATE_TYPE getCurrState();

    MOVE_COMMAND getCurrCommand();

    bool isMoving();

private:
    STATE_TYPE curr_state;
    STATE_TYPE previous_state;

    static const MOTOR_COMMAND movement_memory[17][17];

    /* IT INDEXES LIKE THIS: movement_memory[movement_commands][movement_index] 
    SO, movement_index TELLS THE SYSTEM WHAT ACTION IS CURRENTLY TAKING PLACE,
    AND movement_commands TELLS THE SYSTEM WHAT SERIES OF COMMANDS IS CURRENTLY
    BEING EXECUTED. */
    int movement_index;
    int movement_commands;

};

#endif
