/********** state_machine.cpp **********
 * 
 * Program movement of the BEAST.
 * 
 */

#include "StateMachine.h"
#include "Arduino.h"

extern HardwareSerial ESP1;
extern HardwareSerial ESP2;

/********** StateMachine **********
 * 
 * Initializes the state machine to the stop state.
 * 
 ************************/
StateMachine::StateMachine() {

    movement_memory[16][15] = {
    {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_135_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* THIS IS HELL */
    };



    curr_state = P1;
    command = IDLE;
}

/********** parseCommands **********
 * 
 * Handles commands from parent and calls appropriate wheel functions. 
 * 
 * Inputs:
 *    MOVE_COMMAND command - command from parent
 * 
 * Returns:
 *    None.
 * 
 * Expects:
 *   - Commands apply to both wheel 3 and wheel 4
 * 
 * Notes:
 *  - Commands defined in state_machine.h
 *  - If the emergency stop command is sent at any time then the emergencyStop() 
 *    function is called immediately
 * - While motors are moving, sMoving() returns true, so new commands aren't 
 *   issued
 * - When motors reach target position, isMoving() returns false, so the next 
 *   command can be issued
 * 
 ************************/
 STATE_TYPE StateMachine::parseCommands(MOVE_COMMAND command) {

    return P1;
    


    switch (curr_state) {

        case (P1): 
            if (command == MOVE_TO_P1) {
                return P1;
            } else if (command == MOVE_TO_P2) {
                ESP2.write(INITIATE_TURN_MOTORS);
                ESP1.write(LIFT_FRAME);
                ESP1.write(TURN_RIGHT_90_DEGREES);
                ESP1.write(LOWER_FRAME);
                ESP2.write(RETURN_TURN_MOTORS);
                return P2;
            } else if (command == MOVE_TO_P3) {
                return P3;
            } else if (command == MOVE_TO_P4) {
                return P4;
            } else {
            }
        break;
        case (P2):
            if (command == MOVE_TO_P1) {
                return P1;
            } else if (command == MOVE_TO_P2) {
                return P2;
            } else if (command == MOVE_TO_P3) {
                return P3;
            } else if (command == MOVE_TO_P4) {
                return P4;
            } else {
            }
        break;
        case (P3):
            if (command == MOVE_TO_P1) {
                return P1;
            } else if (command == MOVE_TO_P2) {
                return P2;
            } else if (command == MOVE_TO_P3) {
                return P3;
            } else if (command == MOVE_TO_P4) {
                return P4;
            } else {
            }
        break;
        case (P4):
            if (command == MOVE_TO_P1) {
                return P1;
            } else if (command == MOVE_TO_P2) {
                return P2;
            } else if (command == MOVE_TO_P3) {
                return P3;
            } else if (command == MOVE_TO_P4) {
                return P4;
            } else {
            }
        break;
    }
    return curr_state;
}


/********** getCurrState **********
 * 
 * 
 * 
 * Inputs: None.
 * 
 * Returns: None.
 * 
 ************************/
STATE_TYPE StateMachine::getCurrState() {
    return curr_state;
}

/********** getCurrCommand **********
 * 
 * Returns the current command in the state machine.
 * 
 * Inputs: None.
 * 
 * Returns: MOVE_COMMAND - the current command.
 * 
 ************************/
MOVE_COMMAND StateMachine::getCurrCommand() {
    return command;
}
