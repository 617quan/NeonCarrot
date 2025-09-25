//SwerveModule.h
//By Alex Wardwell for Bonnie Schnitta (SoundSense)

#ifndef SWERVE_MODULE_H
#define SWERVE_MODULE_H

#include "StepperMotor.h"

class SwerveModule {

public:
    SwerveModule(StepperMotor* drive, StepperMotor* steer, StepperMotor* lift);
    void moveWheel(long steps);
    void rotateWheel(float angle, int microstepping);
    void lift(bool up); // up = true = raise, false = lower
    void setSpeedDrive(int speed);
    void setSpeedSteer(int speed);
    void setSpeedLift(int speed);
    void enable();
    void disable();
    int isMoving();

private:
    StepperMotor* _drive;
    StepperMotor* _steer;
    StepperMotor* _lift;

    long angleToSteps(float angle, int microstepping);
};

#endif
