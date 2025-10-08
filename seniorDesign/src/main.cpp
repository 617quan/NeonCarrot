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

/* MAIN */
void loop() {
    Wheel wheel;
    setup();
    driveForward(wheel);

}

void setup() {
    pinMode(WHEEL3_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL3_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL3_PULSE_DRIVE, OUTPUT);
    
    pinMode(WHEEL3_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL3_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL3_ENABLE_DRIVE, OUTPUT);

    pinMode(WHEEL3_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL3_DIRECTION_SWERVE, OUTPUT);
    pinMode(WHEEL3_DIRECTION_DRIVE, OUTPUT);
}

/* Move given wheel forwards */
void driveForward(Wheel wheel) {
    /* Set pins for drive motor */
    wheel.enable(wheel.drive);
    wheel.setPulseHigh(wheel.drive);
    wheel.setDirHigh(wheel.drive);

}

void lift() {

}

void turn(){
    
}
