/********** state_machine.cpp **********
 * 
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info.
 * Implements the robot state machine controlling BEAST movement between
 * positions and coordinating commands sent to the motor controller ESPs.
 *
 **************************************/

#include "StateMachine.h"
#include "Arduino.h"

extern HardwareSerial ESP1;
extern HardwareSerial ESP2;

const MOTOR_COMMAND StateMachine::movement_memory[17][17] = {
    /* P1 to P1 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P2 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P3 */ {WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P1 to P4 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_45_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P1 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P2 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P3 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P2 to P4 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_135_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P1 */ {WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P2 */ {WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P3 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P3 to P4 */ {WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_45_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_FORWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_90_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT},
    /* P4 to P1 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_45_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P4 to P2 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_135_DEGREES, INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /* P4 to P3 */ {INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_RIGHT_90_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, INITIATE_TURN_MOTORS, WHEELS_DOWN, TURN_LEFT_45_DEGREES, INIT_LIFT, INIT_TURN, WHEELS_DOWN, MOVE_BACKWARDS_24_IN, INIT_LIFT, FINISH_MOVEMENT},
    /* P4 to P4 */ {FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT},
    /*   INIT   */ {INIT_LIFT, INIT_TURN, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT, FINISH_MOVEMENT}
};

/********** StateMachine **********
 *
 * Description:
 *      Constructor that initializes the state machine variables and
 *      sets the starting robot position.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      None.
 *
 * Expects:
 *      Called once during system initialization.
 *
 * Notes:
 *      Robot begins in Position 1 with no pending movement commands.
 *
 ************************/
StateMachine::StateMachine() {
    movement_index = 0;
    movement_commands = 0;
    curr_state = P1;
}

/********** isMoving **********
 *
 * Description:
 *      Indicates whether the robot is currently executing a movement
 *      sequence.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      bool - True if the robot is currently moving.
 *
 * Expects:
 *      State machine has been initialized.
 *
 * Notes:
 *
 ************************/
bool StateMachine::isMoving() {
    return curr_state == MOVING;
}

/********** parseWebServerInput **********
 *
 * Description:
 *      Processes a movement command received from the web interface
 *      and sends a message to the correct ESP to begin movement
 *
 * Parameters:
 *      MOVE_COMMAND command - requested destination or action.
 *
 * Return:
 *      bool - True if the command was accepted and executed.
 *
 * Expects:
 *      Robot must not currently be in the MOVING state.
 *
 * Notes:
 *      Uses the movement_memory lookup table to determine the sequence
 *      of motor commands needed to reach the destination.
 *
 ************************/
bool StateMachine::parseWebServerInput(MOVE_COMMAND command) {

    if (curr_state != MOVING) {
        movement_commands = (command == INITIALIZE) ? 16 : (curr_state * 4) + command; // Finds correct series of movements
        previous_state = curr_state;
        curr_state = MOVING;
        if (movement_memory[movement_commands][movement_index] != FINISH_MOVEMENT) {
            if (movement_memory[movement_commands][movement_index] % 2 == 1) {
                ESP1.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            } else {
                ESP2.write(movement_memory[movement_commands][movement_index]);
                movement_index++;
            }
        } else {
            curr_state = STATE_TYPE(movement_commands % 4);
            movement_index = 0;
            movement_commands = 0;
        }
    } else { // The bot is currently moving. Cannot do anything new while moving
        return false;
    }
    
    return true;
}

/********** parseUARTInput **********
 *
 * Description:
 *      Handles messages received from motor controller ESP boards
 *      indicating completion of a motion stage.
 *
 * Parameters:
 *      MOTOR_COMMAND command - message sent from child ESP board.
 *
 * Return:
 *      bool - True if the message was processed successfully.
 *
 * Expects:
 *      Only END_STAGE messages should be received from children.
 *
 * Notes:
 *      When a stage finishes, the next command in the movement sequence
 *      is issued until FINISH_MOVEMENT is reached.
 *
 ************************/
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
            curr_state = (movement_commands == 16) ? previous_state : (STATE_TYPE)(movement_commands % 4);
            movement_index = 0;
            movement_commands = 0;
        }
    } else {
        /* The message sent to the mother wasn't END_STAGE, which shouldn't be
        possible. Throw an error (return false) when this happens */
        return false;
    }
    return true;
}

/********** getCurrState **********
 *
 * Description:
 *      Returns the robot's current position or movement state.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      STATE_TYPE representing the robot's current state.
 *
 * Expects:
 *      State machine has been initialized.
 *
 * Notes:
 *      Used by the web interface and control logic to display status.
 *
 ************************/
STATE_TYPE StateMachine::getCurrState() {
    return curr_state;
}