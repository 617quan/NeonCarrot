//StepperMotor.h
//By Alex Wardwell for Bonnie Schnitta (SoundSense)

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <FastAccelStepper.h>

class StepperMotor {

public:
  StepperMotor(int stepPin, int dirPin, int enablePin = -1);
  void moveTo(long steps);
  void setSpeed(float speed);
  void enable();
  void disable();
  bool isMoving();

private:
  int _stepPin, _dirPin, _enablePin;
  static FastAccelStepperEngine _engine;
  FastAccelStepper* _stepper = nullptr;

};

#endif
