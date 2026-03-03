/********** state_machine.cpp **********
 * 
 * Program movement of the BEAST.
 * 
 */

#include "StateMachine.h"
#include "Arduino.h"

extern HardwareSerial ESP1;
extern HardwareSerial ESP2;



const MOTOR_COMMAND StateMachine::movement_memory[16][12] = {
    /* P1 to P1 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P2 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P3 */ {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P4 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_45_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P1 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P2 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P3 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P4 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_135_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P1 */ {WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P2 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P3 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P4 */ {WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_45_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_FORWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT},
    /* P4 to P1 */ {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_45_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P4 to P2 */ {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_135_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P4 to P3 */ {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_45_DEGREES, WHEELS_UP, RETURN_TURN_MOTORS, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, WHEELS_UP, FINISH_MOVEMENT},
    /* P4 to P4 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT}
};

/********** StateMachine **********
 * 
 * Initializes the state machine to the stop state.
 * 
 ************************/
StateMachine::StateMachine() {
    movement_index = 0;
    movement_commands = 0;
    is_operating = false;
    curr_state = P1;
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
bool StateMachine::parseWebServerInput(MOVE_COMMAND command) {

    if (!is_operating) {
        movement_commands = (curr_state * 4) + command;
        curr_state = (STATE_TYPE)command;
        is_operating = true;
        // if (command == MOVE_TO_P1 && curr_state == P1) {
        //     ESP1.write(WHEELS_UP);
        // } else if (command == MOVE_TO_P2 && curr_state == P1) {
        //     ESP1.write(TURN_LEFT_90_DEGREES);
        // } else if (command == MOVE_TO_P3 && curr_state == P1) {
        //     ESP2.write(INITIATE_TURN_MOTORS);
        // } else if (command == MOVE_TO_P4 && curr_state == P1) {
        //     ESP2.write(RETURN_TURN_MOTORS);
        // } 
        if (movement_memory[movement_commands][movement_index] != FINISH_MOVEMENT) {
            if (movement_memory[movement_commands][movement_index] % 2 == 1) {
                ESP1.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            } else {
                ESP2.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            }
        } else {
            is_operating = false;
            movement_index = 0;
            movement_commands = 0;
        }
    } else {
        return false;
    }
    
    return true;
}

bool StateMachine::parseUARTInput(MOTOR_COMMAND command) {

    if (command == END_STAGE) {
        if (movement_memory[movement_commands][movement_index] != FINISH_MOVEMENT) {
            if (movement_memory[movement_commands][movement_index] % 2 == 1) {
                ESP1.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            } else {
                ESP2.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            }
        } else {

            is_operating = false;
            movement_index = 0;
            movement_commands = 0;
        }
    } else {
        return false;
    }

    return true;
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