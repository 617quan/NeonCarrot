README.md

Files
-----

1. frame.cpp & frame.h

TURNING:

Notes: Example: If we were to have a microstep setting of 1/8:
 *        Micro step at 8 = 1600 micro steps per step
 *        Pulses per revolution: Motor requires 1600 input pulses to complete 360° rotation
 *        Gear box ratio for drive is 1:30 -> 1600 * 30 = 48000 steps needed for one wheel
 *        Gear box ratio for turn is 1:100 -> 1600 * 100 = 160000 steps needed for one wheel
 
Turning functionality works as if the BEAST is rotating on an axel, meaning
 * two wheels need to rotate more from the original position than the other two
 * to complete a successful turn
 * - Wheels 1 and 3 need to turn 45 degrees
 * - Wheels 2 and 4 need to turn 135 degrees 


2. child_main.cpp

3. parent_main.cpp

4. state_machine.cpp


OLD SPI code to send char [] messages
------------------------------------

parent_main.cpp:

// void sendMessageToChild(const char message[], size_t size_of_msg, SPIClass *spi) {
//     /* Create buffer to hold message */
//     uint8_t buf[size_of_msg];
//     memcpy(buf, message, size_of_msg);

//     /* Transfer size of message */
//     spi->beginTransaction(mySettings);
//     // delay(1000);
//     digitalWrite(VSPI_CS, LOW);

//     // send size as a single byte (will truncate if >255)
//     uint8_t size_byte = (uint8_t)size_of_msg;
//     spi->transfer(size_byte);

//     digitalWrite(VSPI_CS, HIGH);
//     spi->endTransaction();

//     delay(100);

//     /* Transfer message itself */
//     spi->beginTransaction(mySettings);
//     digitalWrite(VSPI_CS, LOW);

//     // transfer the buffer (cast away const for API)
//     spi->transfer(buf, size_of_msg);

//     digitalWrite(VSPI_CS, HIGH);
//     spi->endTransaction();
// }


child_main.cpp:

// int recieveMessageFromParent() {
//     spi_slave_transaction_t t;
//     memset(&t, 0, sizeof(t));
    
//     /* First byte array of size 1 to get length of message */
//     uint8_t length_buf[128];
//     t.length = 8; // 8 bits = 1 byte = length of transaction
//     /* assigns length_buf as the container for transferred bytes */
//     t.rx_buffer = length_buf;
//     t.tx_buffer = nullptr;

//     esp_err_t t_status = spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
//     if (t_status == ESP_OK) {
//         Serial.print("Nothing wong\n");
//     } else if (t_status == ESP_ERR_TIMEOUT) {
//         Serial.print("Took too wong\n");
//     } else if (t_status == ESP_ERR_INVALID_STATE) {
//         Serial.print("Invalid state\n");
//     } else if (t_status == ESP_ERR_INVALID_ARG) {
//         Serial.print("Invalid argument\n");
//     } else {
//         Serial.print("Someting wong\n");
//     }

//     uint8_t msg_length = length_buf[0];
//     Serial.printf("Message incoming, size: %d\n", msg_length);

//     spi_slave_transaction_t t2;
//     memset(&t2, 0, sizeof(t2));
    
//     /* data container 256 (max message length) bytes big all set to 0 */
//     uint8_t rec_buf[256] = {0}; 
//     t2.length = msg_length * 8;
//     t2.rx_buffer = rec_buf;

//     // return string
//     char parent_msg_array [msg_length];
//     std::string parent_msg = "";
//     // waiting for parent esp to send data 
//     esp_err_t t2_status = spi_slave_transmit(VSPI_HOST, &t2, portMAX_DELAY);
//     if (t2_status == ESP_OK) {
//         Serial.print("Recieved message: \n");
//         for (int i = 0; i < msg_length; i++) {
//             Serial.write(rec_buf[i]);
//             parent_msg_array[i] = rec_buf[i];
//             parent_msg += parent_msg_array[i];
//             Serial.println();
//         }
//     } else {
//         Serial.print("Failed transaction 2\n");
//     }
//     return parent_msg;
// }