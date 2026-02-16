/********** state_machine.cpp **********
 * 
 * Program movement of the BEAST.
 * 
 */

#include "state_machine.h"

extern Frame *frame;

STATE_TYPE Curr_state = P1;
MOVE_COMMAND command = EMERGENCY_STOP;

/********** StateMachine **********
 * 
 * Initializes the state machine to the stop state.
 * 
 ************************/
StateMachine::StateMachine() {
    
}


/********** parseCommands **********
 * 
 * Handles commands from parent and calls appropriate wheel functions.
 * 
 * Inputs:
 *    uint8_t command - command from parent
 * 
 * Returns:
 *    None.
 * 
 * Expects:
 *   - Commands apply to both wheel 3 and wheel 4
 * 
 * Notes:
 *  - Commands defined in state_machine.h
 * 
 ************************/
 STATE_TYPE StateMachine::parseCommand(MOVE_COMMAND command) {
    
    switch (Curr_state) {
        case (P1): 
        if (command == MOVE_TO_P1) {
            return P1;
        } else if (command == MOVE_TO_P2) {
            // frame->rotateRight(90);
            return P2;
        } else if (command == MOVE_TO_P3) {
            frame->turnLeft(90);
            // frame->moveForward(TODO: FIND THIS OUT);
            frame->turnRight(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            // frame->rotateRight(135);
            frame->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }
        break;
        case (P2):
        if (command == MOVE_TO_P1) {
            // frame->rotateLeft(45);
            return P1;
        } else if (command == MOVE_TO_P2) {
            return P2;
        } else if (command == MOVE_TO_P3) {
            // frame->moveBackward(TODO: FIND THIS OUT);
            // frame->rotateLeft(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            // frame->rotateRight(45);
            frame->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }

        break;
        case (P3):

        if (command == MOVE_TO_P1) {
            frame->turnRight(90);
            // frame->moveForward(TODO: FIND THIS OUT);
            frame->turnLeft(90);
            return P1;
        } else if (command == MOVE_TO_P2) {
            // frame->rotateRight(90);
            // frame->moveForward(TODO: FIND THIS OUT);
            return P2;
        } else if (command == MOVE_TO_P3) {
            return P3;
        } else if (command == MOVE_TO_P4) {
            // frame->rotateRight(90);
            // frame->moveForward(TODO: FIND THIS OUT);
            // frame->rotateRight(45);
            frame->moveForward(23.622f);
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }

        break;
        case (P4):
        if (command == MOVE_TO_P1) {
            frame->moveBackwards(23.622f);
            // frame->rotateLeft(135);
            return P1;
        } else if (command == MOVE_TO_P2) {
            frame->moveBackwards(23.622f);
            // frame->rotateLeft(45);
            return P2;
        } else if (command == MOVE_TO_P3) {
            frame->moveBackwards(23.622f);
            // frame->rotateLeft(135);
            frame->turnLeft(90);
            // frame->moveForward(TODO: FIND THIS OUT);
            frame->turnRight(90);
            return P3;
        } else if (command == MOVE_TO_P4) {
            return P4;
        } else {
            Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
        }
        break;
    }
    return Curr_state;
}


void StateMachine::emergencyStop() {
    frame->stopMoving();
}