/* esp1_main.cpp
 * Lift Motor: 1 EN, 1 DIR, 4 PUL
 * Drive Motor: 1 EN, 2 DIR, 2 PUL
 * 4 Pins for Limit Switches
*/

#include "StateMachine.h"
#include "WebPage.h"
#include "defines.h"
    
WebPage webServer("ESP32-Access-Point", "123456789");
StateMachine state_machine;

HardwareSerial ESP1(1);
HardwareSerial ESP2(2);

void setup() {
    Serial.begin(115200);
    ESP1.begin(115200, SERIAL_8N1, RXD1, TXD1);
    ESP2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    webServer.begin();
    
    pinMode(LED_BUILTIN, OUTPUT);
    // initSPI();    
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

    webServer.handleClient(state_machine.getCurrState());
    if (webServer.hasNewCommand()) {
        MOVE_COMMAND new_command = webServer.getCommand();
        state_machine.parseWebServerInput(new_command);
    }
    if (ESP1.available() > 0) {
        uint8_t incoming_byte = ESP1.read();
        state_machine.parseUARTInput((MOTOR_COMMAND)incoming_byte);
    } else if (ESP2.available() > 0) {
        uint8_t incoming_byte = ESP2.read();
        state_machine.parseUARTInput((MOTOR_COMMAND)incoming_byte);
    }
}
