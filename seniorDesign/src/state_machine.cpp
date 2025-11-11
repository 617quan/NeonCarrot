/********** state_machine.cpp **********
 * 
 * Program movement of the BEAST.
 * 
 */

#include "state_machine.h"

/********** StateMachine **********
 * 
 * Initializes the state machine to the stop state.
 * 
 ************************/
StateMachine::StateMachine() {
    currentState = STATE_STOP;
}

/********** updateState **********
 * 
 * Updates the state machine to the next state when the current state
 * has been reached.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *   None.
 * 
 * Notes:
 *   - Current state progression just goes STOP -> P1 -> P2 -> P3 -> P4 -> STOP
 * 
 ************************/
void StateMachine::updateState() {
    int state = getState();
    int command = getCommand();

    if ((state == STATE_STOP) and (command == CMD_MOVE_TO_P1)) {
        currentCommand = CMD_MOVE_TO_P1;
        currentState   = STATE_AT_P1;

    } else if ((state == STATE_AT_P1) and (command == CMD_MOVE_TO_P2)) {
        currentCommand = CMD_MOVE_TO_P2;
        currentState   = STATE_AT_P2;

    } else if ((state == STATE_AT_P2) and (command == CMD_MOVE_TO_P3)) {
        currentCommand = CMD_MOVE_TO_P3;
        currentState   = STATE_AT_P3;

    } else if ((state == STATE_AT_P3) and (command == CMD_MOVE_TO_P4)) {
        currentCommand = CMD_MOVE_TO_P4;
        currentState   = STATE_AT_P4;

    } else if (command == CMD_STOP_MOVE) {
        stop();
    }
}

/********** getState **********
 * 
 * Returns the current state of the state machine.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *  int: Current state.
 * 
 ************************/
int StateMachine::getState() {
    return currentState;
}

/********** getCommand **********
 * 
 * Returns the command.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *  
 * 
 ************************/
int StateMachine::getCommand() {
    return currentCommand;
}

/********** stop **********
 * 
 * Stops all movement.
 * 
 ************************/
void StateMachine::stop() {
    currentCommand = CMD_STOP_MOVE;
    currentState   = STATE_STOP;
}