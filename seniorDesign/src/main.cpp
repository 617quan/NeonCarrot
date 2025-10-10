#include <Arduino.h>
#include "pinout_defines.h"
#include "motor.h"

FastAccelStepperEngine engine;
FastAccelStepper *stepper = nullptr;

void setup() {
    Serial.begin(115200);

    engine.init(0);

    stepper = engine.stepperConnectToPin(17);
    if (stepper == nullptr) {
        while (true) {
            Serial.println("SETUP DOESN'T WORK!");
            delay(1000);
        }
    }

    stepper->setDirectionPin(33);
    stepper->setEnablePin(32);
    stepper->setAutoEnable(true);
}

void loop() {
    delay(1000);
    stepper->setSpeedInHz(6400);
    stepper->setAcceleration(1000);
    stepper->move(1000);
    delay(2000);
}
