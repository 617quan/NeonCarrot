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
//     currentState = P1;
//     currentCommand = MOVE_TO_P1;

// }

// /********** recieveMessageFromParent **********
//   * Description: 
//   *     Recieves an uint8_t message from the parent ESP32.
//   * 
//   * Inputs: 
//   *     None.
//   * 
//   * Returns:
//   *     uint8_t - message from parent.
//   * 
//   * Notes:
//   *     - Since there is no heap and we don't know how long of a message will
//   *       come across we first send across size (in bytes) then make a buffer 
//   *       to recieve the whole message
//   *     - Use memset tp set everything in child struct to 0
//   *     - Uses spi_child_transmit() blocking call to wait for parent to send
//   * 
//   ************************/
// MOVE_COMMAND StateMachine::recieveMessageFromParent() {
//     spi_child_transaction_t t;
//     memset(&t, 0, sizeof(t));
    
//     /* Receive 1-byte uint8_t */
//     uint8_t rec_buf[128] = {0}; // Must be 128
//     t.length = 8; // 8 bits = 1 byte for uint8_t
//     t.rx_buffer = rec_buf; 
//     t.tx_buffer = nullptr;

//     esp_err_t t_status = SPI_CHILD_TRANSMIT(VSPI_HOST, &t, portMAX_DELAY);
//     if (t_status == ESP_OK) {
//         Serial.print("Nothing wong\n");
//     } else if (t_status == ESP_ERR_TIMEOUT) {
//         Serial.print("Took too wong\n");
//     } else if (t_status == ESP_ERR_INVALID_STATE) {
//         Serial.print("Invalid state\n");
//     } else if (t_status == ESP_ERR_INVALID_ARG) {
//         Serial.print("Invalid argument\n");
//     } else {
//         Serial.print("Transaction failed\n");
//     }

//     MOVE_COMMAND command = (MOVE_COMMAND)rec_buf[0];
//     Serial.printf("Receiving message from parent, command: %u\n", (uint8_t)command);
    
//     return command;
// }



// /********** parseCommands **********
//  * 
//  * Handles commands from parent and calls appropriate wheel functions.
//  * 
//  * Inputs:
//  *    uint8_t command - command from parent
//  * 
//  * Returns:
//  *    None.
//  * 
//  * Expects:
//  *   - Commands apply to both wheel 3 and wheel 4
//  * 
//  * Notes:
//  *  - Commands defined in state_machine.h
//  * 
//  ************************/
//  STATE_TYPE StateMachine::parseCommand(MOVE_COMMAND command) {

//     switch (Curr_state) {
//         case (P1): 
//         if (command == MOVE_TO_P1) {
//             return P1;
//         } else if (command == MOVE_TO_P2) {
//             // frame->rotateRight(90);
//             return P2;
//         } else if (command == MOVE_TO_P3) {
//             frame->turnLeft(90);
//             // frame->moveForward(TODO: FIND THIS OUT);
//             frame->turnRight(90);
//             return P3;
//         } else if (command == MOVE_TO_P4) {
//             // frame->rotateRight(135);
//             frame->moveForward(23.622f);
//             return P4;
//         } else {
//             Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
//         }
//         break;
//         case (P2):
//         if (command == MOVE_TO_P1) {
//             // frame->rotateLeft(45);
//             return P1;
//         } else if (command == MOVE_TO_P2) {
//             return P2;
//         } else if (command == MOVE_TO_P3) {
//             // frame->moveBackward(TODO: FIND THIS OUT);
//             // frame->rotateLeft(90);
//             return P3;
//         } else if (command == MOVE_TO_P4) {
//             // frame->rotateRight(45);
//             frame->moveForward(23.622f);
//             return P4;
//         } else {
//             Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
//         }

//         break;
//         case (P3):

//         if (command == MOVE_TO_P1) {
//             frame->turnRight(90);
//             // frame->moveForward(TODO: FIND THIS OUT);
//             frame->turnLeft(90);
//             return P1;
//         } else if (command == MOVE_TO_P2) {
//             // frame->rotateRight(90);
//             // frame->moveForward(TODO: FIND THIS OUT);
//             return P2;
//         } else if (command == MOVE_TO_P3) {
//             return P3;
//         } else if (command == MOVE_TO_P4) {
//             // frame->rotateRight(90);
//             // frame->moveForward(TODO: FIND THIS OUT);
//             // frame->rotateRight(45);
//             frame->moveForward(23.622f);
//             return P4;
//         } else {
//             Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
//         }

//         break;
//         case (P4):
//         if (command == MOVE_TO_P1) {
//             frame->moveBackwards(23.622f);
//             // frame->rotateLeft(135);
//             return P1;
//         } else if (command == MOVE_TO_P2) {
//             frame->moveBackwards(23.622f);
//             // frame->rotateLeft(45);
//             return P2;
//         } else if (command == MOVE_TO_P3) {
//             frame->moveBackwards(23.622f);
//             // frame->rotateLeft(135);
//             frame->turnLeft(90);
//             // frame->moveForward(TODO: FIND THIS OUT);
//             frame->turnRight(90);
//             return P3;
//         } else if (command == MOVE_TO_P4) {
//             return P4;
//         } else {
//             Serial.println("MOVEMENT_COMMAND_NOT_SPECIFIED");
//         }
//         break;
//     }
// }


// void StateMachine::emergencyStop() {
//     frame->stopMoving();
// }