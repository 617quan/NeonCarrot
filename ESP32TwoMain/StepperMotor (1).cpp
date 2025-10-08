//StepperMotor.cpp
//By Alex Wardwell for Bonnie Schnitta (SoundSense)


#include "StepperMotor.h"
// #include <Arduino.h> //only if using ArduinoIDE

// At the top of StepperMotor.cpp, after the includes:
FastAccelStepperEngine StepperMotor::_engine;

StepperMotor::StepperMotor(int stepPin, int dirPin, int enablePin) {
    _stepPin = stepPin;
    _dirPin = dirPin;
    _enablePin = enablePin;

    // Ensure the engine is only initialized once
    static bool engine_initialized = false;
    if (!engine_initialized) {
        _engine.init();
        engine_initialized = true;
    }

    _stepper = _engine.stepperConnectToPin(_stepPin);
    if (_stepper) {
        _stepper->setDirectionPin(_dirPin);
        if (_enablePin != -1) {
            _stepper->setEnablePin(_enablePin);
            _stepper->enableOutputs();
        }

        // Set defaults
        _stepper->setSpeedInHz(1000);          // default speed
        _stepper->setAcceleration(1000);       // default acceleration
    }
}

void StepperMotor::moveTo(long steps) {
    if (_stepper) {
        _stepper->moveTo(steps);
    }
}

void StepperMotor::setSpeed(float speed) {
    if (_stepper) {
        _stepper->setSpeedInHz((uint32_t)speed);
    }

}

void StepperMotor::enable() {
    if (_stepper && _enablePin != -1) {
        _stepper->enableOutputs();
    }
}

void StepperMotor::disable() {
    if (_stepper && _enablePin != -1) {
        _stepper->disableOutputs();
    }
}

bool StepperMotor::isMoving() {
    return _stepper && _stepper->isRunning();
}
