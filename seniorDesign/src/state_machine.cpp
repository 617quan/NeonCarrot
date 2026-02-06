// /********** state_machine.cpp **********
//  * 
//  * Program movement of the BEAST.
//  * 
//  */

// #include "state_machine.h"

// /********** StateMachine **********
//  * 
//  * Initializes the state machine to the stop state.
//  * 
//  ************************/
// StateMachine::StateMachine() {
//     currentState = STATE_STOP;
//     currentCommand = CMD_MOVE_TO_P1;
// }

// /********** sendCommandToStateMachine **********
//  * 
//  * Sends a command to the state machine.
//  * 
//  * Inputs:
//  *   uint8_t command - command to send.
//  * 
//  * Returns:
//  *   None.
//  * 
//  * Notes:
//  *   - Commands defined in state_machine.h
//  * 
//  ************************/
// void StateMachine::sendCommandToStateMachine(uint8_t command) {
//     currentCommand = command;
// }

// /********** updateState **********
//  * 
//  * Updates the state machine to the next state when the current state
//  * has been reached.
//  * 
//  * Inputs:
//  *   None.
//  * 
//  * Returns:
//  *   None.
//  * 
//  * Notes:
//  *   - Current state progression just goes STOP -> P1 -> P2 -> P3 -> P4 -> STOP
//  * 
//  ************************/
// void StateMachine::updateState() {
//     uint8_t state = getState();
//     uint8_t command = getCommand();

//     if ((state == STATE_STOP) and (command == CMD_MOVE_TO_P1)) {
//         Serial.printf("in stop state moving to P1\n");
//         currentCommand = CMD_MOVE_TO_P1;
//         currentState   = STATE_AT_P1;

//     } else if ((state == STATE_AT_P1) and (command == CMD_MOVE_TO_P2)) {
//         Serial.printf("in state P1 moving to P2\n");
//         currentCommand = CMD_MOVE_TO_P2;
//         currentState   = STATE_AT_P2;

//     } else if ((state == STATE_AT_P2) and (command == CMD_MOVE_TO_P3)) {
//         Serial.printf("in state P2 moving to P3\n");
//         currentCommand = CMD_MOVE_TO_P3;
//         currentState   = STATE_AT_P3;

//     } else if ((state == STATE_AT_P3) and (command == CMD_MOVE_TO_P4)) {
//         Serial.printf("in state P3 moving to P4\n");
//         currentCommand = CMD_MOVE_TO_P4;
//         currentState   = STATE_AT_P4;

//     } else if (command == CMD_STOP_MOVE) {
//         stop();
//     }
// }

// /********** getState **********
//  * 
//  * Returns the current state of the state machine.
//  * 
//  * Inputs:
//  *       None.
//  * 
//  * Returns:
//  *      uint8_t: Current state.
//  * 
//  ************************/
// uint8_t StateMachine::getState() {
//     return currentState;
// }

// /********** getCommand **********
//  * 
//  * Returns the command.
//  * 
//  * Inputs:
//  *   None.
//  * 
//  * Returns:
//  *      uint8_t: Current command.
//  * 
//  ************************/
// uint8_t StateMachine::getCommand() {
//     return currentCommand;
// }

// /********** stop **********
//  * 
//  * Stops all movement.
//  * 
//  ************************/
// void StateMachine::stop() {
//     currentCommand = CMD_STOP_MOVE;
//     currentState   = STATE_STOP;
// }