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

void driveForward(Wheel wheel);

/* MAIN */
void loop() {
    Wheel wheel_three;
    setup();
    driveForward(wheel_three);
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
    // wheel.setPulseHigh(wheel.drive);
    wheel.setDirHigh(wheel.drive);
    // 240,000,000 / 4,000 = 60,000
    int frequency = 60;
    wheel.testPulseSignal(wheel.drive, frequency);

}

void lift() {

}

void turn(){
    
}
