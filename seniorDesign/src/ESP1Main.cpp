/* esp1_main.cpp
 * Lift Motor: 1 EN, 1 DIR, 4 PUL
 * Drive Motor: 1 EN, 2 DIR, 2 PUL
 * 4 Pins for Limit Switches
*/

#include "MotorGroup.h"
#include "StateMachine.h"
#include "WebPage.h"
#include "defines.h"

#define SPI_CHILD_INITIALIZE spi_slave_initialize
#define SPI_CHILD_TRANSMIT spi_slave_transmit
typedef spi_slave_transaction_t spi_child_transaction_t;
typedef spi_slave_interface_config_t spi_child_interface_config_t;

void initMotorGroup();
void initSPI();
MOVE_COMMAND recieveMessageFromParent();
    

void setup() {
    Serial.begin(115200);
    // webServer.begin();
    // WebPage webServer("ESP32-Access-Point", "123456789");
    MotorGroup::engineStartup();
    pinMode(LED_BUILTIN, OUTPUT);
    initMotorGroup();
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
    pinMode(VSPI_COPI, INPUT_PULLUP);
    pinMode(SPI_CLK, INPUT_PULLUP);
    pinMode(VSPI_CS, INPUT_PULLUP);
    pinMode(VSPI_CIPO, OUTPUT);

    esp_err_t ret = SPI_CHILD_INITIALIZE(VSPI_HOST, &buscfg, &chldcfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        while(1) {
            Serial.printf("SPI child init failed: %d\n", ret);
        }
    }
}

/********** initMotorGroup **********
 * 
 * Initializes the lift and drive motors on ESP1. Called in the setup function.
 * 
 * Inputs/Returns: 
 *      None. Initializes the frame.
 * 
 ************************/
void initMotorGroup() {

    pinMode(DRIVE_ENABLE, OUTPUT);
    pinMode(FRONT_DRIVE_DIRECTION, OUTPUT);
    pinMode(FRONT_DRIVE_PULSE, OUTPUT);
    pinMode(BACK_DRIVE_DIRECTION, OUTPUT);
    pinMode(BACK_DRIVE_PULSE, OUTPUT);
    pinMode(LIFT_ENABLE, OUTPUT);
    pinMode(LIFT_DIRECTION, OUTPUT);
    pinMode(LIFT1_PULSE, OUTPUT);
    pinMode(LIFT2_PULSE, OUTPUT);
    pinMode(LIFT3_PULSE, OUTPUT);
    pinMode(LIFT4_PULSE, OUTPUT);

    MotorSettings_t front_drive_settings = {FRONT_DRIVE_PULSE, FRONT_DRIVE_DIRECTION, DRIVE_ENABLE, DRIVE_MAX_SPEED, DRIVE_ACCEL};
    MotorSettings_t back_drive_settings = {BACK_DRIVE_PULSE, BACK_DRIVE_DIRECTION, DRIVE_ENABLE, DRIVE_MAX_SPEED, DRIVE_ACCEL};
    MotorSettings_t lift1_settings = {LIFT1_PULSE, LIFT_DIRECTION, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
    MotorSettings_t lift2_settings = {LIFT2_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
    MotorSettings_t lift3_settings = {LIFT3_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
    MotorSettings_t lift4_settings = {LIFT4_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};

    MotorSettings_t drive_settings[4] = {front_drive_settings, 
                                         back_drive_settings,
                                         {0, 0, 0, 0, 0},
                                         {0, 0, 0, 0, 0}
                                        };

    MotorSettings_t lift_settings[4] = {lift1_settings, 
                                        lift2_settings,
                                        lift3_settings, 
                                        lift4_settings
                                        };

    MotorGroup *drive_motors = new MotorGroup(drive_settings, 'd');
    if (drive_motors == nullptr) {
        Serial.println("FATAL ERROR: drive motors initialized incorrectly");
    }
    MotorGroup *lift_motors = new MotorGroup(lift_settings, 'l');
    if (lift_motors == nullptr) {
        Serial.println("FATAL ERROR: lift motors initialized incorrectly");
    }
    
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

/********** loop **********
 * Description: 
 *      Testing.
 *
 * Inputs/Returns: 
 *      None.
 * 
 ************************/
void loop() {  
    
    delay(1000);
    delay(5000);

}
