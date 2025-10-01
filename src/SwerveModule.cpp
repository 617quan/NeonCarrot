//SwerveModule.cpp
//By Alex Wardwell for Bonnie Schnitta (SoundSense)

#include "SwerveModule.h"

SwerveModule::SwerveModule(StepperMotor* drive, StepperMotor* steer, StepperMotor* lift) {
    _drive = drive;
    _steer = steer;
    _lift = lift;
}

void SwerveModule::moveWheel(long steps) {
    _drive->moveTo(steps);
}

void SwerveModule::rotateWheel(float angle, int microstepping) {
    long steps = angleToSteps(angle, microstepping);
    _steer->moveTo(steps);
}

void SwerveModule::lift(bool up) { // up = true = raise, false = lower
    long steps;
    if (up) {
        steps = -10000;
    }
    else {
        steps = 10000;
    }
    _lift->moveTo(steps);
}

long SwerveModule::angleToSteps(float angle, int microstepping) {
    const int full_steps_per_rev = 200;
    int steps_per_rev = full_steps_per_rev * microstepping;
    return static_cast<long>((angle / 360.0f) * steps_per_rev);
}

void SwerveModule::setSpeedDrive(int speed) {
    _drive->setSpeed(speed);
}

void SwerveModule::setSpeedSteer(int speed) {
    _steer->setSpeed(speed);
}

void SwerveModule::setSpeedLift(int speed) {
    _lift->setSpeed(speed);
}

void SwerveModule::enable() {
    _drive->enable();
    _steer->enable();
    _lift->enable();
}

void SwerveModule::disable() {
    _drive->disable();
    _steer->disable();
    _lift->disable();
}

int SwerveModule::isMoving() {
    if(_drive->isMoving()) {
        return 1;
    }
    if(_steer->isMoving()) {
        return 2;
    }
    if(_lift->isMoving()) {
        return 3;
    }
    else {
        return 0;
    }
}
