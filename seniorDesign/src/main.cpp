#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"

FastAccelStepperEngine engine;
FastAccelStepper *stepper = nullptr;

/* If the microstep is on 8, then we need 1600 steps per one rotation
(Hypothetically). TODO: It's not actually doing that.

Set acceration time to .5 seconds (0 to max speed in half a second)

Set max speed to 6400hz (This shouldn't matter I think it should do a whole
rotation no matter the speed).

Acceleration (steps/s/s) should just be the max speed divided by the
acceleration time.

*/
const uint32_t steps_per_rev = 1600;      // 1/16 microstepping
const float accel_time_s = 0.5;           // accelerate in 0.5 s
const uint32_t max_speed_hz = 6400;       // steps per second
const uint32_t accel = max_speed_hz / accel_time_s;

void setup() {
    Serial.begin(115200);

    // Start your engine (just to make the stepper motor)
    engine.init(0);

    stepper = engine.stepperConnectToPin(17); // Stepper is the motor object
    if (stepper == nullptr) {
        while (true) {
            Serial.println("SETUP DOESN'T WORK!");
            delay(1000);
        }
    }

    // Set pins, and make the enable auto turn on and off
    stepper->setDirectionPin(33);
    stepper->setEnablePin(32);
    stepper->setAutoEnable(true);
    // stepper->setEnablePin(true, true);

    // Set the max speed the motor can go at, and its acceleration
    stepper->setSpeedInHz(max_speed_hz); 
    stepper->setAcceleration(accel);
}


void loop() {   
    /* Take 1600 steps (should be one full rotation on 8 microstep) */
    stepper->move(steps_per_rev, false);

    delay(2000);
}
