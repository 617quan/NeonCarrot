/********** state_machine.cpp **********
 * 
 * Program movement of the BEAST.
 * 
 */

#include "StateMachine.h"

extern MotorGroup *motor_group;

/********** StateMachine **********
 * 
 * Initializes the state machine to the stop state.
 * 
 ************************/
StateMachine::StateMachine() {
    curr_state = P1;
    command = EMERGENCY_STOP;
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
    
    /* Check for emergency stop */
    if (command == EMERGENCY_STOP) {
        motor_group->stopMoving();
        /* Stay in current state */
        return curr_state;
    }
    
    /* Wait for current movement to complete before issuing next command */
    if (motor_group->isMoving()) {
        /* Stay in current state until movement completes */
        return curr_state;
    }

    switch (curr_state) {
        case (P1): 
        if (command == MOVE_TO_P1) {
            return P1;
        } else if (command == MOVE_TO_P2) {
            motor_group->rotateRight(90);
            return P2;
        } else if (command == MOVE_TO_P3) {
            motor_group->rotateLeft(90);
            motor_group->moveForward(23.622f);
            motor_group->rotateRight(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            motor_group->rotateRight(135);
            motor_group->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }
        break;
        case (P2):
        if (command == MOVE_TO_P1) {
            motor_group->rotateLeft(45);
            return P1;
        } else if (command == MOVE_TO_P2) {
            return P2;
        } else if (command == MOVE_TO_P3) {
            motor_group->moveBackwards(23.622f);
            motor_group->rotateLeft(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            motor_group->rotateRight(45);
            motor_group->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }
        break;
        case (P3):
        if (command == MOVE_TO_P1) {
            motor_group->rotateRight(90);
            motor_group->moveForward(23.622f);
            motor_group->rotateLeft(90);
            return P1;
        } else if (command == MOVE_TO_P2) {
            motor_group->rotateRight(90);
            motor_group->moveForward(23.622f);
            return P2;
        } else if (command == MOVE_TO_P3) {
            return P3;
        } else if (command == MOVE_TO_P4) {
            motor_group->rotateRight(90);
            motor_group->moveForward(23.622f);
            motor_group->rotateRight(45);
            motor_group->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }
        break;
        case (P4):
        if (command == MOVE_TO_P1) {
            motor_group->moveBackwards(23.622f);
            motor_group->rotateLeft(135);
            return P1;
        } else if (command == MOVE_TO_P2) {
            motor_group->moveBackwards(23.622f);
            motor_group->rotateLeft(45);
            return P2;
        } else if (command == MOVE_TO_P3) {
            motor_group->moveBackwards(23.622f);
            motor_group->rotateLeft(45);
            motor_group->moveBackwards(23.622f);
            motor_group->rotateLeft(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
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
