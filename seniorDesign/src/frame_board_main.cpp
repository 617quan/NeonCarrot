/*********** parent_main.cpp **********
 * 
 *  Configures wheels 1 & 2
 *  - Need two different mains to flash to parent and child microcontroller
 *  - This file uses SPI in parent mode to send commands to child ESP32
 * 
 * Notes:
 *  - If the microstep is on 8, then we need 1600 steps per one rotation
 *  - (Hypothetically). TODO: It's not actually doing that.
 *  - Set acceration time to .5 seconds (0 to max speed in half a second)
 *  - Set max speed to 6400hz (This shouldn't matter I think it should do a whole
 *  - rotation no matter the speed).
 *  - Acceleration (steps/s/s) should just be the max speed divided by the
 *  - acceleration time.
 */

#include <Arduino.h>
#include "pinout_defines.h"
#include "frame.h"
#include <string>
#include "driver/spi_slave.h"
#include "state_machine.h"
#include "WebPage.h"

/* Use these functions and typedefs to avoid outdated language */
#define SPI_CHILD_INITIALIZE spi_slave_initialize
#define SPI_CHILD_TRANSMIT spi_slave_transmit
typedef spi_slave_transaction_t spi_child_transaction_t;
typedef spi_slave_interface_config_t spi_child_interface_config_t;

Frame *frame = nullptr;
WebPage webServer("ESP32-Access-Point", "123456789");
MOVE_COMMAND recieveMessageFromParent();
void initFrame();
void initSPI();
STATE_TYPE parseCommand(MOVE_COMMAND command);



void setup() {
    Serial.begin(115200);
    webServer.begin();
    Frame::engineStartup();
    //Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    pinMode(LED_BUILTIN, OUTPUT);
    initFrame();
    initSPI();    

    
}

/********** initSPI **********
 * 
 * Initializes the board flashed with this main as a child. Initializes the size
 * of the queue holding spi messages.
 *
 * Parameters:
 *      Nothing
 * 
 * Return:
 *     Nothing. Initializes the board to allow communication through SPI, with
 *     this board being the child board
 *
 * Expects:
 *      This board is being defined as the child board. There are at least 4
 *      pins available for this protocol to work. There is an available DMA
 *      channel for this protocol.
 *   
 ************************/
void initSPI() {

    /* SPI Child Init - SPI bus configuration */
    spi_bus_config_t buscfg = {
        .mosi_io_num = VSPI_COPI,
        .miso_io_num = VSPI_CIPO,
        .sclk_io_num = SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    /* SPI child interface configuration */
    spi_child_interface_config_t chldcfg = {
        .spics_io_num = VSPI_CS,
        .flags = 0,
        .queue_size = 1, // Only one transaction queued at a time
        .mode = 0,       // SPI mode 0 (CPOL=0, CPHA=0)
    };

    // Enable pull-ups, pins are connected to Vdd until parent sends low
    pinMode(VSPI_COPI, INPUT_PULLUP);
    pinMode(SPI_CLK, INPUT_PULLUP);
    pinMode(VSPI_CS, INPUT_PULLUP);
    // Controller in Peripheral out means child is sending data. CIPO IS HOW
    // CHILD SENDS DATA
    pinMode(VSPI_CIPO, OUTPUT);

    // Initialize SPI into child mode
    esp_err_t ret = SPI_CHILD_INITIALIZE(VSPI_HOST, &buscfg, &chldcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        while(1) {
            Serial.printf("SPI child init failed: %d\n", ret);
        }
    } 

}

/********** initFrame **********
 * 
 * Initializes all of the motors on the frame. Call in the setup function.
 * 
 * Inputs/Returns: 
 *      None. Initializes the frame
 * 
 ************************/
void initFrame() {

    pinMode(DRIVE_PULSE, OUTPUT);
    pinMode(DRIVE_DIRECTION, OUTPUT);
    pinMode(DRIVE_ENABLE, OUTPUT);

    pinMode(LIFT_PULSE, OUTPUT);
    pinMode(LIFT_DIRECTION, OUTPUT);
    pinMode(LIFT_ENABLE, OUTPUT);

    pinMode(TURN_ENABLE, OUTPUT);

    pinMode(TURN1_PULSE, OUTPUT);
    pinMode(TURN1_DIRECTION, OUTPUT);

    pinMode(TURN2_PULSE, OUTPUT);
    pinMode(TURN2_DIRECTION, OUTPUT);

    pinMode(TURN3_PULSE, OUTPUT);
    pinMode(TURN3_DIRECTION, OUTPUT);

    pinMode(TURN4_PULSE, OUTPUT);
    pinMode(TURN4_DIRECTION, OUTPUT);

    MotorSettings_t drive_motor_settings = {DRIVE_PULSE, DRIVE_DIRECTION, DRIVE_ENABLE, DRIVE_MAX_SPEED, DRIVE_ACCEL};
    MotorSettings_t lift_motor_settings = {LIFT_PULSE, LIFT_DIRECTION, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
    MotorSettings_t turn1_motor_settings = {TURN1_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_1_3_SPEED, TURN_1_3_ACCEL};
    MotorSettings_t turn2_motor_settings = {TURN2_PULSE, TURN2_DIRECTION, TURN_ENABLE, TURN_2_4_SPEED, TURN_2_4_ACCEL};
    MotorSettings_t turn3_motor_settings = {TURN3_PULSE, TURN3_DIRECTION, TURN_ENABLE, TURN_1_3_SPEED, TURN_1_3_ACCEL};
    MotorSettings_t turn4_motor_settings = {TURN4_PULSE, TURN4_DIRECTION, TURN_ENABLE, TURN_2_4_SPEED, TURN_2_4_ACCEL};

    MotorSettings_t turn_motor_settings_arr[4] = {turn1_motor_settings, 
                                                  turn2_motor_settings, 
                                                  turn3_motor_settings, 
                                                  turn4_motor_settings};
    
    frame = new Frame(drive_motor_settings, lift_motor_settings, turn_motor_settings_arr);
    if (frame == nullptr) {
        Serial.println("FATAL ERROR: Frame initialized incorrectly");
    }
}
STATE_TYPE Curr_state = P1;
MOVE_COMMAND command;
void loop() {  
    
    // THIS CODE SEGMENT WAS USED TO TEST WEBSERVER INTERFACING - QUAN
    // webServer.handleClient();
    // state = webServer.returnState();
    // frame->printPosition(state);

    /* Get command from parent */
    // MOVE_COMMAND command = recieveMessageFromParent();
    // Serial.printf("Command to be parsed: %u\n", command);
    // delay(1000); HOPE WE DON'T NEED THIS!
    // Curr_state = parseCommand(command);

    /* OPTION 2: WEBSERVER. THIS CODE USES THE WEBSERVER TO TELL THE FRAME WHAT
    TO DO */


    /* OPTION 3: MANUAL: THIS CODE JUST MANUALLY RUNS THE SYSTEM THROUGH CERTAIN
    TESTS */

    frame->turnLeft(45);
    delay(10000);
    frame->turnRight(45);
    delay(10000);
   
}

/********** recieveMessageFromParent **********
  * Description: 
  *     Recieves an uint8_t message from the parent ESP32.
  * 
  * Inputs: 
  *     None.
  * 
  * Returns:
  *     uint8_t - message from parent.
  * 
  * Notes:
  *     - Since there is no heap and we don't know how long of a message will
  *       come across we first send across size (in bytes) then make a buffer 
  *       to recieve the whole message
  *     - Use memset tp set everything in child struct to 0
  *     - Uses spi_child_transmit() blocking call to wait for parent to send
  * 
  ************************/
MOVE_COMMAND recieveMessageFromParent() {
    spi_child_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    /* Receive 1-byte uint8_t */
    uint8_t rec_buf[128] = {0}; // Must be 128
    t.length = 8; // 8 bits = 1 byte for uint8_t
    t.rx_buffer = rec_buf; 
    t.tx_buffer = nullptr;

    esp_err_t t_status = SPI_CHILD_TRANSMIT(VSPI_HOST, &t, portMAX_DELAY);
    if (t_status == ESP_OK) {
        Serial.print("Nothing wong\n");
    } else if (t_status == ESP_ERR_TIMEOUT) {
        Serial.print("Took too wong\n");
    } else if (t_status == ESP_ERR_INVALID_STATE) {
        Serial.print("Invalid state\n");
    } else if (t_status == ESP_ERR_INVALID_ARG) {
        Serial.print("Invalid argument\n");
    } else {
        Serial.print("Transaction failed\n");
    }

    MOVE_COMMAND command = (MOVE_COMMAND)rec_buf[0];
    Serial.printf("Receiving message from parent, command: %u\n", (uint8_t)command);
    
    return command;
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
 STATE_TYPE parseCommand(MOVE_COMMAND command) {

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
}