/********** espMother.cpp **********
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Purpose: Main control program for the ESP32 coordinating the system
 * state machine, web interface, and UART communication with motor
 * ESPs. Initializes serial links and services commands from the web 
 * server and connected ESP boards.
 **********************************/

#include "StateMachine.h"
#include "WebPage.h"
#include "defines.h"
    
WebPage webServer("Neon-Carrot-ESP32", "asdfghjkl;'");
StateMachine state_machine;

HardwareSerial ESP1(1);
HardwareSerial ESP2(2);

/********** setup **********
 *
 * Description:
 *      Initializes the ESP32 UART connections, and starts the web server used
 *      for issuing system commands.
 *
 * Parameters:
 *      Nothing.
 * 
 * Return:
 *      Nothing. UARTs and web server are initialized.
 *
 * Expects:
 *      Called once at boot before loop begins executing.
 *
 * Notes:
 *      UART channels are used to communicate motor commands and status
 *      updates between this controller and external boards.
 *
 ************************/
void setup() {
    Serial.begin(115200);
    ESP1.begin(115200, SERIAL_8N1, RXD1, TXD1);
    ESP2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    webServer.begin();
    
    pinMode(LED_BUILTIN, OUTPUT);    
}

/********** loop **********
 *
 * Description:
 *      Main runtime task loop. Services the web server, checks for new
 *      commands from the web server, and polls UART channels for incoming
 *      motor controller messages which are forwarded to the state
 *      machine for processing.
 *
 * Parameters:
 *      Nothing.
 * 
 * Return:
 *      Nothing. Runs continuously after setup completes.
 *
 * Expects:
 *      setup() must have completed successfully so that serial ports
 *      and the web server are properly initialized.
 *
 * Notes:
 *      UART input is polled each iteration to avoid blocking the
 *      web server or state machine execution.
 *
 ************************/

void loop() {  

    webServer.setBusyMessage(state_machine.isMoving());

    webServer.handleClient(state_machine.getCurrState());

    if (webServer.hasNewCommand()) {
        MOVE_COMMAND new_command = webServer.getCommand();
        state_machine.parseWebServerInput(new_command);
    }

    if (ESP1.available() > 0) {
        MOTOR_COMMAND incoming_command = (MOTOR_COMMAND)ESP1.read();
        state_machine.parseUARTInput(incoming_command);
    } else if (ESP2.available() > 0) {
        MOTOR_COMMAND incoming_command = (MOTOR_COMMAND)ESP2.read();
        state_machine.parseUARTInput(incoming_command);
    }
}
