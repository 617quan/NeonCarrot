/********** child_main.cpp **********
 *  
 *  Configure wheels 3 & 4
 *  - Uses SPI in child mode to recieve commands from parent ESP32
 * 
 *  Notes:
 *  - To put a esp in child mode you can't use arduino library since that
 *    initiliazes it as a parent, instead we use ESP libraries to configure it
 *    into child mode 
 */

#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"
#include <string>
#include "driver/spi_slave.h"
#include "state_machine.h"

#define RXD2 16
#define TXD2 17

Wheel *Wheel3 = nullptr;
Wheel *Wheel4 = nullptr;

uint8_t recieveMessageFromParent();
void initWheels34();
void parseCommand(uint8_t command);

void setup() {
    Serial.begin(115200);
    //Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Serial.println("=====SERIAL BEGIN=====");
    // Wheel::engineStartup();

    initWheels34();

    // MotorSettings_t wheel3_lift_settings = {WHEEL3_PULSE_LIFT, WHEEL3_ENABLE_LIFT, WHEEL3_DIRECTION_LIFT, 10000, 4000};
    // MotorSettings_t wheel3_drive_settings = {WHEEL3_PULSE_DRIVE, WHEEL3_ENABLE_DRIVE, WHEEL3_DIRECTION_DRIVE, 8000, 8000};
    // MotorSettings_t wheel3_turn_settings = {WHEEL3_PULSE_SWERVE, WHEEL3_ENABLE_SWERVE, WHEEL3_DIRECTION_SWERVE, 1000, 1000};
    
    // Wheel3 = new Wheel(wheel3_lift_settings, wheel3_drive_settings, wheel3_turn_settings);
    // if (Wheel3 == nullptr) {
    //     Serial.println("Wheel 3 is fucked");
    // }

    // MotorSettings_t wheel4_lift_settings = {WHEEL4_PULSE_LIFT, WHEEL4_ENABLE_LIFT, WHEEL4_DIRECTION_LIFT, 10000, 4000};
    // MotorSettings_t wheel4_drive_settings = {WHEEL4_PULSE_DRIVE, WHEEL4_ENABLE_DRIVE, WHEEL4_DIRECTION_DRIVE, 8000, 8000};
    // MotorSettings_t wheel4_turn_settings = {WHEEL4_PULSE_SWERVE, WHEEL4_ENABLE_SWERVE, WHEEL4_DIRECTION_SWERVE, 10000, 4000};
    
    // Wheel4 = new Wheel(wheel4_lift_settings, wheel4_drive_settings, wheel4_turn_settings);
    // if (Wheel4 == nullptr) {
    //     Serial.println("Wheel 4 is fucked");
    // }

    /* SPI Child Init - SPI bus configuration */
    spi_bus_config_t buscfg = {
        .mosi_io_num = VSPI_COPI,
        .miso_io_num = VSPI_CIPO,
        .sclk_io_num = SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    /* SPI child interface configuration */
    spi_slave_interface_config_t chldcfg = {
        .spics_io_num = VSPI_CS,
        .flags = 0,
        .queue_size = 1, // Only one transaction queued at a time
        .mode = 0,       // SPI mode 0 (CPOL=0, CPHA=0)
    };

    // Enable pull-ups, pins are connected to Vdd until parent sends low
    pinMode(VSPI_COPI, INPUT_PULLUP);
    pinMode(SPI_CLK, INPUT_PULLUP);
    pinMode(VSPI_CS, INPUT_PULLUP);
    // Controller in Peripheral out means child is sending data
    pinMode(VSPI_CIPO, OUTPUT);

    // Initialize SPI into child mode
    esp_err_t ret = spi_slave_initialize(VSPI_HOST, &buscfg, &chldcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        while(1) {
            Serial.printf("SPI child init failed: %d\n", ret);
        }
    } 

    // debug LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

/********** initWheels34 **********
 * 
 * Intializes the pulse, enable, and direction pins of the lift, drive, and
 * swerve motors to output mode.
 * 
 * Inputs/Returns: 
 *      None.
 * 
 ************************/
void initWheels34() {
    pinMode(WHEEL3_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL3_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL3_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL3_PULSE_DRIVE, OUTPUT);
    pinMode(WHEEL3_ENABLE_DRIVE, OUTPUT);
    pinMode(WHEEL3_DIRECTION_DRIVE, OUTPUT);
    pinMode(WHEEL3_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL3_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL3_DIRECTION_SWERVE, OUTPUT);

    pinMode(WHEEL4_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL4_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL4_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL4_PULSE_DRIVE, OUTPUT);
    pinMode(WHEEL4_ENABLE_DRIVE, OUTPUT);
    pinMode(WHEEL4_DIRECTION_DRIVE, OUTPUT);
    pinMode(WHEEL4_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL4_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL4_DIRECTION_SWERVE, OUTPUT);
}

void loop() {  

    /* Get command from parent */
    uint8_t command = recieveMessageFromParent();
    Serial.printf("Command to be parsed: %u\n", command);
    delay(1000);
    parseCommand(command);
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
  *     - Uses spi_slave_transmit() blocking call to wait for parent to send
  * 
  ************************/
uint8_t recieveMessageFromParent() {
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    
    /* Receive 1-byte uint8_t */
    uint8_t rec_buf[128] = {0};
    t.length = 8; // 8 bits = 1 byte for uint8_t
    t.rx_buffer = rec_buf;
    t.tx_buffer = nullptr;

    esp_err_t t_status = spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
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

    uint8_t command = rec_buf[0];
    Serial.printf("Receiving message from parent, command: %u\n", command);
    
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
 void parseCommand(uint8_t command) {
    if (command == CMD_MOVE_TO_P2) {
        Wheel3->moveForward(48000); 
        Wheel4->moveForward(48000);
    } else if (command == CMD_MOVE_TO_P3) {
        Wheel3->turnRight(48000);
        Wheel4->turnRight(48000);
        Wheel3->moveForward(48000); 
        Wheel4->moveForward(48000);
    } else if (command == CMD_MOVE_TO_P4) {
        Wheel3->turnRight(48000);
        Wheel4->turnRight(48000);
        Wheel3->moveForward(48000); 
        Wheel4->moveForward(48000);
    } else if (command == CMD_STOP_MOVE) {
        Wheel3->stopMoving();
        Wheel4->stopMoving();
    } else {
        Serial.println("Unknown command received");
    }
}