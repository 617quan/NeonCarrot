#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"
#include <SPI.h>

/* If the microstep is on 8, then we need 1600 steps per one rotation
(Hypothetically). TODO: It's not actually doing that.

Set acceration time to .5 seconds (0 to max speed in half a second)

Set max speed to 6400hz (This shouldn't matter I think it should do a whole
rotation no matter the speed).

Acceleration (steps/s/s) should just be the max speed divided by the
acceleration time.
*/
#define RXD2 16
#define TXD2 17

Wheel *Wheel1 = nullptr;
Wheel *Wheel2 = nullptr;
Wheel *Wheel3 = nullptr;
Wheel *Wheel4 = nullptr;

/* Uninitialized pointers to SPI objects */
SPIClass *vspi = NULL;
//SPIClass *hspi = NULL;

// SPI settings object (SPI_MODE0 means read on rising edge write on next
// falling edge, 20MHz is frequency for communication clock)
SPISettings mySettings(20000000, MSBFIRST, SPI_MODE0);

void setup() {
    // Serial.begin(115200);
    // Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Wheel::engineStartup();

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

    //MotorSettings_t wheel3_lift_settings = {WHEEL3_PULSE_LIFT, WHEEL3_ENABLE_LIFT, WHEEL3_DIRECTION_LIFT, 10000, 4000};
    //MotorSettings_t wheel3_drive_settings = {WHEEL3_PULSE_DRIVE, WHEEL3_ENABLE_DRIVE, WHEEL3_DIRECTION_DRIVE, 8000, 8000};
    //MotorSettings_t wheel3_turn_settings = {WHEEL3_PULSE_SWERVE, WHEEL3_ENABLE_SWERVE, WHEEL3_DIRECTION_SWERVE, 1000, 1000};
    
    //Wheel3 = new Wheel(wheel3_lift_settings, wheel3_drive_settings, wheel3_turn_settings);
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


    /* SPI SHIT - just vspi for now */

    
    // need vspi object to determine which SPI bus you are using to transfer. 
    vspi = new SPIClass(VSPI);
    //hspi = new SPIClass(HSPI);

    // initializes all the pins to necessary modes
    vspi->begin(SPI_CLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); 

    
    
}

void loop() {  
    // Wheel3->moveForward(48000); 
    // Wheel2->moveForward(48000);
    // Wheel1->moveForward(48000);
    // Wheel4->moveForward(48000);
    Wheel3->turnRight(48000);
    Wheel3->moveForward(48000);

    /* SPI */
    // Basicaly we need:
    // two different mains to flash to parent and child microcontroller

    // a protocol structure to recognize commands sent over MISO and MOSI serial
    // communication (ex byte 0x01 indicates start of a packet, 0x0F -> 0x08 ->
    // 0xF0 means something like set wheel drive motor to value 0xF0)

    // a go byte (command)


    // SPI clock divider automatically set to 1/4 on chip system frequency
    // (don't use)

    // to send packets over vspi pins use begintransaction(mySettings), transfer(),
    // then endTransaction()
    vspi->beginTransaction(mySettings);

    // Set object's CS/SS pin to low to tell child peripheral/ESP32 to get ready
    digitalWrite(VSPI_SS, LOW);

    //only transfer one byte
    vspi->transfer(0x00);

    //transfer array of bytes
    uint8_t data_to_send{0x00, 0x01, 0x02};
    vspi->transfer(data_to_send, sizeof(data_to_send));
    // to recieve this array you need to know the size already or put on heap

    vspi->endTransaction();
    // hspi.begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

    //pinMode(hspi->pinSS(), OUTPUT);  //HSPI SS

    
    

    delay(1000);
}
