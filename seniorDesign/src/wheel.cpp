/* wheel.cpp
 * Purpose:
 * Notes: Microstep setting: 1/8
 * Pulses per revolution: Motor requires 1600 input pulses to complete 360° rotation
 * 
 * Micro step at 8 = 1600 micro steps per step
 * Gear box ratio for drive is 1:30 -> 1600 * 30 = 48000 steps needed for one
 * wheel
 * Gear box ratio for turn is 1:100 -> 1600 * 100 = 160000 steps needed for one
 * wheel 
 * 
 */ 

#include "wheel.h"

/* Constructor */
Wheel::Wheel(uint8_t lift_pulse_pin,  uint8_t lift_enable_pin,  uint8_t lift_dir_pin,
             uint8_t turn_pulse_pin,  uint8_t turn_enable_pin,  uint8_t turn_dir_pin,
             uint8_t drive_pulse_pin, uint8_t drive_enable_pin, uint8_t drive_dir_pin) {

    
    engine.init();


    initMotorPulsePins(lift_pulse_pin, turn_pulse_pin, drive_pulse_pin);

    /* Set the direction and enable pins for all three motors */
    if(lift_motor != nullptr && turn_motor != nullptr && drive_motor != nullptr) {
        lift_motor->setDirectionPin(lift_pulse_pin, true, 0);
        lift_motor->setEnablePin(lift_enable_pin);
        lift_motor->setAutoEnable(false);
        
        turn_motor->setDirectionPin(turn_pulse_pin, true, 0);
        turn_motor->setEnablePin(turn_enable_pin);
        turn_motor->setAutoEnable(false);

        drive_motor->setDirectionPin(drive_pulse_pin, true, 0);
        drive_motor->setEnablePin(drive_enable_pin);
        drive_motor->setAutoEnable(false);
    } else {
        while(1) {
            Serial.println("You're fucked!");
        }
    }
}

Wheel::~Wheel() {
/* Nothing new being made, so nothing here */
}

void Wheel::initMotorPulsePins(uint8_t lift_pulse_pin, uint8_t turn_pulse_pin, uint8_t drive_pulse_pin) {
    lift_motor = engine.stepperConnectToPin(lift_pulse_pin);
    turn_motor = engine.stepperConnectToPin(turn_pulse_pin);
    drive_motor = engine.stepperConnectToPin(turn_pulse_pin);
}

void Wheel::moveUp(uint32_t num_steps) {
    lift_motor->setSpeedInHz(num_steps);
    lift_motor->setDirectionPin(lift_dir_pin, true, 0);
}

void Wheel::moveDown(uint32_t num_steps) {

}

void Wheel::moveRight(uint32_t num_steps) {
    turn_motor->setSpeedInHz(num_steps);
    turn_motor->setDirectionPin(turn_dir_pin, true, 0);
}

void Wheel::moveLeft(uint32_t num_steps) {

}

/* Moves the wheel forwards by given steps */
void Wheel::moveForward(uint32_t num_steps) {
    drive_motor->setDirectionPin(drive_dir_pin, true, 0);
    drive_motor->setSpeedInHz(num_steps);
}

void Wheel::moveBackwards(uint32_t num_steps) {
    //drive_motor->setSpeedInHz(steps_s);
}

void Wheel::stopMoving() {
        drive_motor->stopMove();
}

/* Purpose: Retrieve the current position of the stepper
 * Notes: The actual position may be off by the number of steps in the ongoing
 * command. If precise real time position is needed, attaching a pulse counter
 * may be of help. */
int32_t Wheel::getLiftCurrentPosition() {
    return lift_motor->getCurrentPosition();
}

int32_t Wheel::getTurnCurrentPosition() {
    return turn_motor->getCurrentPosition();
}

int32_t Wheel::getDriveCurrentPosition() {
    return drive_motor->getCurrentPosition();
}

/* */
void Wheel::calibratePosition() {
    int32_t liftCurrPos = getLiftCurrentPosition();
    int32_t turnCurrPos = getLiftCurrentPosition();
    int32_t driveCurrPos = getLiftCurrentPosition();

    /* if (liftCurrPos isn't what we want it to be) {
        fix that shit
    } 
    */

}