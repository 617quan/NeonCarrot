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
#include "wheel.h"
#include "state_machine.h"
#include <SPI.h>

#define RXD2 16
#define TXD2 17

Wheel *Wheel1 = nullptr;
Wheel *Wheel2 = nullptr;
SPIClass *vspi = NULL;
StateMachine state_machine;

// SPI settings object (SPI_MODE0 means data captured on rising edge write on next
// falling edge, 20MHz is frequency for communication clock)
SPISettings mySettings(20000000, MSBFIRST, SPI_MODE0);

void sendMessageToChild(uint8_t message, SPIClass *spi);
void initWheels12();

void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Wheel::engineStartup();
    initWheels12();
    /* SPI Parent Init - just vspi for now */
    vspi = new SPIClass(VSPI);
        // hspi = new SPIClass(HSPI);

    // initializes all the VSPI by default pins to necessary modes EXCEPT CS pin
    vspi->begin(); 
    
    //initialize to output and high manually to prevent unwanted communication
    pinMode(VSPI_CS, OUTPUT);
    digitalWrite(VSPI_CS, HIGH);

    // Debug LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
}

/********** initWheels12 **********
 * 
 * Intializes the pulse, enable, and direction pins of the lift, drive, and
 * swerve motors to output mode.
 * 
 * Inputs/Returns: 
 *      None.
 * 
 ************************/
void initWheels12() {

    pinMode(DRIVE_PULSE, OUTPUT);
    pinMode(DRIVE_DIRECTION, OUTPUT);
    pinMode(DRIVE_ENABLE, OUTPUT);
    pinMode(TURN_DIRECTION, OUTPUT);
    pinMode(TURN_ENABLE, OUTPUT);
    pinMode(LIFT_DIRECTION, OUTPUT);
    pinMode(LIFT_ENABLE, OUTPUT);

    pinMode(WHEEL_1_TURN_PULSE, OUTPUT);
    pinMode(LIFT_PULSE, OUTPUT);

    pinMode(WHEEL_2_TURN_PULSE, OUTPUT);
    pinMode(LIFT_PULSE, OUTPUT);

    MotorSettings_t wheel1_lift_settings = {LIFT_PULSE, LIFT_ENABLE, LIFT_DIRECTION, 150000, 20000};
    MotorSettings_t wheel1_drive_settings = {DRIVE_PULSE, DRIVE_ENABLE, DRIVE_DIRECTION, 30000, 20000};
    MotorSettings_t wheel1_turn_settings = {WHEEL_1_TURN_PULSE, TURN_ENABLE, TURN_DIRECTION, 20000, 10000};
    
    Wheel1 = new Wheel(wheel1_lift_settings, wheel1_drive_settings, wheel1_turn_settings);
    if (Wheel1 == nullptr) {
        Serial.println("Wheel 1 is fucked");
    }

    MotorSettings_t wheel2_lift_settings = {LIFT_PULSE, LIFT_ENABLE, LIFT_DIRECTION, 150000, 20000};
    MotorSettings_t wheel2_drive_settings = {DRIVE_PULSE, DRIVE_ENABLE, DRIVE_DIRECTION, 30000, 20000};
    MotorSettings_t wheel2_turn_settings = {WHEEL_2_TURN_PULSE, TURN_ENABLE, TURN_DIRECTION, 20000, 10000};
    
    Wheel2 = new Wheel(wheel2_lift_settings, wheel2_drive_settings, wheel2_turn_settings);
    if (Wheel2 == nullptr) {
        Serial.println("Wheel 2 is fucked");
    }

}

void loop() {  

    /* SPI protocol: recognize commands sent over MISO and MOSI serial
     * communication (ex byte 0x01 indicates start of a packet, 0x0F -> 0x08 ->
     * 0xF0 means something like set wheel drive motor to value 0xF0)
     * a go byte (command)
     * SPI clock divider automatically set to 1/4 on chip system frequency
     * (don't use) */


    Wheel1->moveForward(60000);
    Wheel2->moveForward(60000);

    delay(15000);
    /* Test */
    // Serial.printf("Sending command to child: %u\n", CMD_MOVE_TO_P1);
    // sendMessageToChild(CMD_MOVE_TO_P1, vspi); 
    // delay(1000);

    // Serial.printf("Sending command to child: %u\n", CMD_MOVE_TO_P2);
    // sendMessageToChild(CMD_MOVE_TO_P2, vspi); 
    // delay(1000);

    // Serial.printf("Sending command to child: %u\n", CMD_MOVE_TO_P3);
    // sendMessageToChild(CMD_MOVE_TO_P3, vspi); 
    // delay(1000);

    /* State Machine */
    // Serial.printf("State Before: %u\n", state_machine.getState());
    // uint8_t command = state_machine.getCommand();
    
    // Serial.printf("Sending command: %u\n", command);
    // state_machine.sendCommandToStateMachine(command);
    // sendMessageToChild(command, vspi);

    // state_machine.updateState();
    // Serial.printf("State After: %u\n", state_machine.getState());

    // delay(1000);
}

/********** sendMessageToChild **********
 * Description: 
 *       Sends a message to the child peripheral/ESP32
 * 
 * Inputs: 
 *       uint8_t message  - integer message to send to child
 *       SPIClass spi - SPI bus to use (vspi or hspi)
 * 
 * Returns: 
 *       None.
 * 
 * Notes:
 *     - Use begintransaction(mySettings), transfer(), and then endTransaction()
 *     - Set object's CS/SS pin to low to tell child peripheral/ESP32 to get
 *       ready, and then set it back to high when done
 *     - Any other library can't use SPI until endTransaction is called
 ************************/
void sendMessageToChild(uint8_t message, SPIClass *spi) {
    /* Create buffer to hold message */
    uint8_t buf[sizeof(uint8_t)];
    memcpy(buf, &message, sizeof(uint8_t));

    /* Transfer message */
    spi->beginTransaction(mySettings);
    digitalWrite(VSPI_CS, LOW);

    /* Transfer the buffer (cast away const for API) */
    spi->transfer(buf, sizeof(uint8_t));

    digitalWrite(VSPI_CS, HIGH);
    spi->endTransaction();
}