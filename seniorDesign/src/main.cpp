/* main.cpp
 * Purpose: Pin framework for wheel bot motion. Defines pulse, enable, and
   direction on the MC.

 * FUNCTION NAMES = camelCase
 * GLOBAL VARIABLES = ALL CAPS
 * VARIABLES = snake_case
 */

#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"

int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;
void driveForward(Wheel wheel);

/* MAIN */
void loop() {

}

void setup() {
    pinMode(1, OUTPUT);

    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(1, ledChannel);
    ledcWrite(ledChannel, 128);

}

/* Move given wheel forwards */
void driveForward(Wheel wheel) {

}

void lift() {

}

void turn(){
    
}
