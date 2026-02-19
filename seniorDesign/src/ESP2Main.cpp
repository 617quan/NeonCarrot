// /* esp1_main.cpp
//  * Turn Motor: 1 EN, 4 DIR, 4 PUL
//  * 4 Pins for Limit Switches
// */

// #include "ESP2Main.h"

// void ESP2Main::setup() {
//     Serial.begin(115200);
//     webServer.begin();
//     Frame::engineStartup();
//     pinMode(LED_BUILTIN, OUTPUT);
//     initFrame();
//     initSPI();    
// }

// /********** initSPI **********
//  * 
//  * Initializes the board flashed with this main as a child. Initializes the size
//  * of the queue holding spi messages.
//  *
//  * Parameters:
//  *      Nothing
//  * 
//  * Return:
//  *     Nothing. Initializes the board to allow communication through SPI, with
//  *     this board being the child board
//  *
//  * Expects:
//  *      This board is being defined as the child board. There are at least 4
//  *      pins available for this protocol to work. There is an available DMA
//  *      channel for this protocol.
//  *   
//  ************************/
// void ESP2Main::initSPI() {

//     /* SPI Child Init - SPI bus configuration */
//     spi_bus_config_t buscfg = {
//         .mosi_io_num = VSPI_COPI,
//         .miso_io_num = VSPI_CIPO,
//         .sclk_io_num = SPI_CLK,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1
//     };

//     /* SPI child interface configuration */
//     spi_child_interface_config_t chldcfg = {
//         .spics_io_num = VSPI_CS,
//         .flags = 0,
//         .queue_size = 1, // Only one transaction queued at a time
//         .mode = 0,       // SPI mode 0 (CPOL=0, CPHA=0)
//     };
//     pinMode(VSPI_COPI, INPUT_PULLUP);
//     pinMode(SPI_CLK, INPUT_PULLUP);
//     pinMode(VSPI_CS, INPUT_PULLUP);
//     pinMode(VSPI_CIPO, OUTPUT);

//     esp_err_t ret = SPI_CHILD_INITIALIZE(VSPI_HOST, &buscfg, &chldcfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         while(1) {
//             Serial.printf("SPI child init failed: %d\n", ret);
//         }
//     }
// }

// /********** initMotorGroup **********
//  * 
//  * Initializes all of the motors on the frame. Call in the setup function.
//  * 
//  * Inputs/Returns: 
//  *      None. Initializes the frame
//  * 
//  ************************/
// void ESP2Main::initMotorGroup() {


//     pinMode(TURN_ENABLE, OUTPUT);

//     pinMode(TURN1_PULSE, OUTPUT);
//     pinMode(TURN1_DIRECTION, OUTPUT);

//     pinMode(TURN2_PULSE, OUTPUT);
//     pinMode(TURN2_DIRECTION, OUTPUT);

//     pinMode(TURN3_PULSE, OUTPUT);
//     pinMode(TURN3_DIRECTION, OUTPUT);

//     pinMode(TURN4_PULSE, OUTPUT);
//     pinMode(TURN4_DIRECTION, OUTPUT);

//     MotorSettings_t turn1_motor_settings = {TURN1_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_1_3_SPEED, TURN_1_3_ACCEL};
//     MotorSettings_t turn2_motor_settings = {TURN2_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_2_4_SPEED, TURN_2_4_ACCEL};
//     MotorSettings_t turn3_motor_settings = {TURN3_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_1_3_SPEED, TURN_1_3_ACCEL};
//     MotorSettings_t turn4_motor_settings = {TURN4_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_2_4_SPEED, TURN_2_4_ACCEL};

//     MotorSettings_t turn_motor_settings_arr[4] = {turn1_motor_settings, 
//                                                   turn2_motor_settings, 
//                                                   turn3_motor_settings, 
//                                                   turn4_motor_settings};
    
//     motor_group = new MotorGroup(drive_motor_settings, lift_motor_settings, turn_motor_settings_arr);
//     if (motor_group == nullptr) {
//         Serial.println("FATAL ERROR: Motor group initialized incorrectly");
//     }
// }

// /********** recieveMessageFromParent **********
//  * Description: 
//  *     Recieves an uint8_t message from the parent ESP32.
//  * 
//  * Inputs: 
//  *     None.
//  * 
//  * Returns:
//  *     uint8_t - message from parent.
//  * 
//  * Notes:
//  *     - Since there is no heap and we don't know how long of a message will
//  *       come across we first send across size (in bytes) then make a buffer 
//  *       to recieve the whole message
//  *     - Use memset tp set everything in child struct to 0
//  *     - Uses spi_child_transmit() blocking call to wait for parent to send
//  * 
//  ************************/
// MOVE_COMMAND ESP2Main::recieveMessageFromParent() {
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

// /********** loop **********
//  * Description: 
//  *      Testing.
//  *
//  * Inputs/Returns: 
//  *      None.
//  * 
//  ************************/
// void ESP2Main::loop() {  
    
//     delay(1000);
//     frame->rotateLeft(90);
//     delay(5000);

// }