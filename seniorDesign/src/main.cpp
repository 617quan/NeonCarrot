#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"

/* If the microstep is on 8, then we need 1600 steps per one rotation
(Hypothetically). TODO: It's not actually doing that.

Set acceration time to .5 seconds (0 to max speed in half a second)

Set max speed to 6400hz (This shouldn't matter I think it should do a whole
rotation no matter the speed).

Acceleration (steps/s/s) should just be the max speed divided by the
acceleration time.

*/

    Wheel *Wheel3 = nullptr;

void setup() {
    Serial.begin(115200);
    pinMode(WHEEL3_PULSE_LIFT, OUTPUT);
    pinMode(WHEEL3_ENABLE_LIFT, OUTPUT);
    pinMode(WHEEL3_DIRECTION_LIFT, OUTPUT);
    pinMode(WHEEL3_PULSE_DRIVE, OUTPUT);
    pinMode(WHEEL3_ENABLE_DRIVE, OUTPUT);
    pinMode(WHEEL3_DIRECTION_DRIVE, OUTPUT);
    pinMode(WHEEL3_PULSE_SWERVE, OUTPUT);
    pinMode(WHEEL3_ENABLE_SWERVE, OUTPUT);
    pinMode(WHEEL3_DIRECTION_SWERVE, OUTPUT);



    MotorSettings_t wheel3_lift_settings = {WHEEL3_PULSE_LIFT, WHEEL3_ENABLE_LIFT, WHEEL3_DIRECTION_LIFT, 10000, .75};
    MotorSettings_t wheel3_drive_settings = {WHEEL3_PULSE_DRIVE, WHEEL3_ENABLE_DRIVE, WHEEL3_DIRECTION_DRIVE, 10000, .75};
    MotorSettings_t wheel3_turn_settings = {WHEEL3_PULSE_SWERVE, WHEEL3_ENABLE_SWERVE, WHEEL3_DIRECTION_SWERVE, 10000, .75};

    
    Wheel3 = new Wheel(wheel3_lift_settings, wheel3_drive_settings, wheel3_turn_settings);

}


void loop() {   

    Wheel3->moveForward(8);
    
    
    

    delay(10000);
}
