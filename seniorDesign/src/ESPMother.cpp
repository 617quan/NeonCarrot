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

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
    Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
    
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

    // webServer.handleClient();
    // static int lastState = -1;
    // int currentState = webServer.returnState();

    // if (currentState != lastState) {
    //     lastState = currentState;

    // }






}
