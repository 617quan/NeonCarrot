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

void sendMessageToChild(const char message[], size_t size_of_msg, SPIClass *spi);
void initWheels();

void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    // Wheel::engineStartup();
    initWheels();

    // MotorSettings_t wheel1_lift_settings = {WHEEL1_PULSE_LIFT, WHEEL1_ENABLE_LIFT, WHEEL1_DIRECTION_LIFT, 10000, 4000};
    // MotorSettings_t wheel1_drive_settings = {WHEEL1_PULSE_DRIVE, WHEEL1_ENABLE_DRIVE, WHEEL1_DIRECTION_DRIVE, 8000, 8000};
    // MotorSettings_t wheel1_turn_settings = {WHEEL1_PULSE_SWERVE, WHEEL1_ENABLE_SWERVE, WHEEL1_DIRECTION_SWERVE, 10000, 4000};
    
    // Wheel1 = new Wheel(wheel1_lift_settings, wheel1_drive_settings, wheel1_turn_settings);
    // if (Wheel1 == nullptr) {
    //     Serial.println("Wheel 1 is fucked");
    // }

    // MotorSettings_t wheel2_lift_settings = {WHEEL2_PULSE_LIFT, WHEEL2_ENABLE_LIFT, WHEEL2_DIRECTION_LIFT, 10000, 4000};
    // MotorSettings_t wheel2_drive_settings = {WHEEL2_PULSE_DRIVE, WHEEL2_ENABLE_DRIVE, WHEEL2_DIRECTION_DRIVE, 8000, 8000};
    // MotorSettings_t wheel2_turn_settings = {WHEEL2_PULSE_SWERVE, WHEEL2_ENABLE_SWERVE, WHEEL2_DIRECTION_SWERVE, 1000, 1000};
    
    // Wheel2 = new Wheel(wheel2_lift_settings, wheel2_drive_settings, wheel2_turn_settings);
    // if (Wheel2 == nullptr) {
    //     Serial.println("Wheel 2 is fucked");
    // }

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

/********** initWheels **********
 * 
 * Intializes the pulse, enable, and direction pins of the lift, drive, and
 * swerve motors to output mode.
 * 
 * Inputs/Returns: 
 *      None.
 * 
 ************************/
void initWheels() {
    pinMode(WHEEL1_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL1_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL1_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL1_PULSE_DRIVE, OUTPUT);
    pinMode(WHEEL1_ENABLE_DRIVE, OUTPUT);
    pinMode(WHEEL1_DIRECTION_DRIVE, OUTPUT);
    pinMode(WHEEL1_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL1_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL1_DIRECTION_SWERVE, OUTPUT);

    pinMode(WHEEL2_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL2_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL2_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL2_PULSE_DRIVE, OUTPUT);
    pinMode(WHEEL2_ENABLE_DRIVE, OUTPUT);
    pinMode(WHEEL2_DIRECTION_DRIVE, OUTPUT);
    pinMode(WHEEL2_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL2_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL2_DIRECTION_SWERVE, OUTPUT);
}

void loop() {  

    /* SPI protocol: recognize commands sent over MISO and MOSI serial
     * communication (ex byte 0x01 indicates start of a packet, 0x0F -> 0x08 ->
     * 0xF0 means something like set wheel drive motor to value 0xF0)
     * a go byte (command)
     * SPI clock divider automatically set to 1/4 on chip system frequency
     * (don't use) */

    /* Send test message to child ESP32 */
    char message[] = "Hello World!";
    sendMessageToChild(message, sizeof(message), vspi);
    delay(1000);

    /* State Machine */
    char command[] = state_machine.getCommand();
    sendMessageToChild(command, sizeof(command), vspi);
}

/********** sendMessageToChild **********
 * Description: 
 *       Sends a character array message to the child peripheral/ESP32
 * Inputs: 
 *       char message[] - array of characters to send to child
 *       SPIClass spi   - SPI bus to use (vspi or hspi)
 * Returns: 
 *       None.
 * Notes:
 *     - Use begintransaction(mySettings), transfer(), and then endTransaction()
 *     - Set object's CS/SS pin to low to tell child peripheral/ESP32 to get
 *       ready, and then set it back to high when done
 *     - Any other library can't use SPI until endTransaction is called
 ************************/
void sendMessageToChild(const char message[], size_t size_of_msg, SPIClass *spi) {
    /* Create buffer to hold message */
    uint8_t buf[size_of_msg];
    memcpy(buf, message, size_of_msg);

    /* Transfer size of message */
    spi->beginTransaction(mySettings);
    // delay(1000);
    digitalWrite(VSPI_CS, LOW);

    // send size as a single byte (will truncate if >255)
    uint8_t size_byte = (uint8_t)size_of_msg;
    spi->transfer(size_byte);

    digitalWrite(VSPI_CS, HIGH);
    spi->endTransaction();

    delay(100);

    /* Transfer message itself */
    spi->beginTransaction(mySettings);
    digitalWrite(VSPI_CS, LOW);

    // transfer the buffer (cast away const for API)
    spi->transfer(buf, size_of_msg);

    digitalWrite(VSPI_CS, HIGH);
    spi->endTransaction();
}