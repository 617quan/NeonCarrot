#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"
// #include <SPI.h>
// ESP Framework
#include "driver/spi_slave.h"

/* CHILD MAIN WHEELS 3 & 4*/

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

// Wheel *Wheel1 = nullptr;
// Wheel *Wheel2 = nullptr;
Wheel *Wheel3 = nullptr;
Wheel *Wheel4 = nullptr;

// SPI settings object (SPI_MODE0 means read on rising edge write on next
// falling edge, 20MHz is frequency for communication clock)
SPISettings mySettings(20000000, MSBFIRST, SPI_MODE0);

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Wheel::engineStartup();

    // pinMode(WHEEL1_PULSE_LIFT, OUTPUT);
    // pinMode(WHEEL1_ENABLE_LIFT, OUTPUT);
    // pinMode(WHEEL1_DIRECTION_LIFT, OUTPUT);
    // pinMode(WHEEL1_PULSE_DRIVE, OUTPUT);
    // pinMode(WHEEL1_ENABLE_DRIVE, OUTPUT);
    // pinMode(WHEEL1_DIRECTION_DRIVE, OUTPUT);
    // pinMode(WHEEL1_PULSE_SWERVE, OUTPUT);
    // pinMode(WHEEL1_ENABLE_SWERVE, OUTPUT);
    // pinMode(WHEEL1_DIRECTION_SWERVE, OUTPUT);

    // pinMode(WHEEL2_PULSE_LIFT, OUTPUT);
    // pinMode(WHEEL2_ENABLE_LIFT, OUTPUT);
    // pinMode(WHEEL2_DIRECTION_LIFT, OUTPUT);
    // pinMode(WHEEL2_PULSE_DRIVE, OUTPUT);
    // pinMode(WHEEL2_ENABLE_DRIVE, OUTPUT);
    // pinMode(WHEEL2_DIRECTION_DRIVE, OUTPUT);
    // pinMode(WHEEL2_PULSE_SWERVE, OUTPUT);
    // pinMode(WHEEL2_ENABLE_SWERVE, OUTPUT);
    // pinMode(WHEEL2_DIRECTION_SWERVE, OUTPUT);

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

    MotorSettings_t wheel3_lift_settings = {WHEEL3_PULSE_LIFT, WHEEL3_ENABLE_LIFT, WHEEL3_DIRECTION_LIFT, 10000, 4000};
    MotorSettings_t wheel3_drive_settings = {WHEEL3_PULSE_DRIVE, WHEEL3_ENABLE_DRIVE, WHEEL3_DIRECTION_DRIVE, 8000, 8000};
    MotorSettings_t wheel3_turn_settings = {WHEEL3_PULSE_SWERVE, WHEEL3_ENABLE_SWERVE, WHEEL3_DIRECTION_SWERVE, 1000, 1000};
    
    Wheel3 = new Wheel(wheel3_lift_settings, wheel3_drive_settings, wheel3_turn_settings);
    if (Wheel3 == nullptr) {
        Serial.println("Wheel 3 is fucked");
    }

    MotorSettings_t wheel4_lift_settings = {WHEEL4_PULSE_LIFT, WHEEL4_ENABLE_LIFT, WHEEL4_DIRECTION_LIFT, 10000, 4000};
    MotorSettings_t wheel4_drive_settings = {WHEEL4_PULSE_DRIVE, WHEEL4_ENABLE_DRIVE, WHEEL4_DIRECTION_DRIVE, 8000, 8000};
    MotorSettings_t wheel4_turn_settings = {WHEEL4_PULSE_SWERVE, WHEEL4_ENABLE_SWERVE, WHEEL4_DIRECTION_SWERVE, 10000, 4000};
    
    Wheel4 = new Wheel(wheel4_lift_settings, wheel4_drive_settings, wheel4_turn_settings);
    if (Wheel4 == nullptr) {
        Serial.println("Wheel 4 is fucked");
    }


    /* SPI Child Init */
    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = VSPI_CIPO,
        .miso_io_num = VSPI_COPI,
        .sclk_io_num = SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    // SPI child interface configuration
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

    // Initialize SPI into child mode
    esp_err_t ret = spi_slave_initialize(HSPI_HOST, &buscfg, &chldcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        Serial.print("SPI child init failed: %d\n", ret);
        while (1);
    }
    
}

void loop() {  
    // Wheel3->moveForward(48000); 
    // Wheel2->moveForward(48000);
    // Wheel1->moveForward(48000);
    // Wheel4->moveForward(48000);
    Wheel3->turnRight(48000);
    Wheel3->moveForward(48000);

    /* SPI */
    // to put a esp in child mode you can't use arduino library since that
    // initiliazes it as a parent, instead we use ESP libraries to configure it
    // into child mode

    // since there is no heap and we don't know how long of a message will come
    // across we first send across size (in bytes) then make a buffer to recieve
    // the whole message:

    // instance of transaction struct "t" 
    spi_slave_transaction_t t;
    // set everything in child struct to 0
    memset(&t, 0, sizeof(t));

    // first byte array of size 1 to get length of message
    uint8_t length_buf[1];
    t.length = 8; // 8 bits = 1 byte = length of transaction
    // assigns length_buf as the container for transferred bytes
    t.rx_buffer = length_buf;

    // waiting for parent to transmit (spi_slave_transmit() is a blocking call)
    esp_err_t t_status = spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
    if (t_status != ESP_OK) {
        Serial.print("Error with spi_slave_transmit transaction 1\n");
        return;
    }

    uint8_t msg_length = length_buf[0];
    Serial.print("Message incoming, size: %d\n");


    // intance of transaction struct "t2" 
    spi_slave_transaction_t t2;
    // set everything to 0 in the struct
    memset(&t2, 0, sizeof(t2));
    // data container 256 (max message length) bytes big all set to 0
    uint8_t rec_buf[256] = {0}; 
    t2.length = msg_length; // length of message is what we found above
    t2.rx_buffer = rec_buf;

    // waiting for parent esp to send data 
    esp_err_t t2_status = spi_slave_transmit(VSPI_HOST, &t2, portMAX_DELAY);
    if (t2_status == ESP_OK) {
        Serial.print("Recieved message: \n");
        for (int i = 0; i < msg_length; i++) {
            Serial.write(rec_buf[i]);
            Serial.println();
        }
    } else {
        Serial.print("Failed transaction 2\n");
    }
    

    delay(1000);
}
