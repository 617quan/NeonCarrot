/********** wheel.cpp **********
 * Purpose: Defines all functions for the Wheel class.
 * Notes: Microstep setting: 1/8
 *        Pulses per revolution: Motor requires 1600 input pulses to complete 360° rotation
 *        Micro step at 8 = 1600 micro steps per step
 *        Gear box ratio for drive is 1:30 -> 1600 * 30 = 48000 steps needed for one wheel
 *        Gear box ratio for turn is 1:100 -> 1600 * 100 = 160000 steps needed for one wheel
 */ 

#include "wheel.h"
#include "pinout_defines.h"

FastAccelStepperEngine Wheel::engine;

void Wheel::engineStartup() {
    engine.init();
}

/********** Wheel Constructor **********
 *
 * Takes in all information about all 3 motors (pin numbers, speeds, and
 * accelerations) and initializes them.
 *
 * Parameters:
 *      MotorSettings_t lift_motor_settings: pinout, speed, and accel for lift motor
 *      MotorSettings_t drive_motor_settings: pinout, speed, and accel for drive motor
 *      MotorSettings_t turn_motor_settings: pinout, speed, and accel for turn motor
 * 
 * Return:
 *      A Wheel object with 3 initialized motors.
 *
 * Expects:
 *      All pinouts are defined. The speeds and accells don't need to be.
 *      They'll just be set to 0.
 *
 * Notes:
 *      None.
 *      
 ************************/
Wheel::Wheel(MotorSettings_t lift_motor_settings, 
             MotorSettings_t drive_motor_settings, 
             MotorSettings_t turn_motor_settings) {

    /* Take the arguments from the constructor and set them equal to the private
    class members */
    this->lift_motor_settings = lift_motor_settings;
    this->drive_motor_settings = drive_motor_settings;
    this->turn_motor_settings = turn_motor_settings;
    
    /* Initialize pins for all motors */
    drive_motor = initMotor(drive_motor_settings);
    turn_motor  = initMotor(turn_motor_settings);
    lift_motor  = initMotor(lift_motor_settings);

    if (drive_motor != nullptr && turn_motor != nullptr && lift_motor != nullptr) {
        digitalWrite(19, 1);    
    }


}

Wheel::~Wheel() {
/* Nothing new being made, so nothing here */
}


/********** initMotor **********
 *
 * Takes in a motor pointer and a struct containing the pins, max speed, and
 * acceleration, and initializes the motor with that information
 *
 * Parameters:
 *      FastAccelStepper *motor: pointer to a motor
 *      MotorSettings_t motor_settings: struct containing pinout, speed, and
 *      accel info.
 * 
 * Return:
 *      Nothing. Motor is initialized
 *
 * Expects:
 *      Pointer to a private motor object. Speed and accel don't have to be
 *      initialized. Pins inside struct need to be initialized
 *
 * Notes:
 *      Nada
 *      
 ************************/
FastAccelStepper* Wheel::initMotor(MotorSettings_t motor_settings) {
    FastAccelStepper *motor = engine.stepperConnectToPin(motor_settings.pulse_pin);
    if (motor == nullptr) {
        // digitalWrite(19, 1);
        return nullptr;
    }
    motor->setDirectionPin(motor_settings.dir_pin, true, 0);
    motor->setEnablePin(motor_settings.enable_pin, false);
    if (motor_settings.max_speed != 0) motor->setSpeedInHz(motor_settings.max_speed);
    if (motor_settings.accel != 0) motor->setAcceleration(motor_settings.accel);
    return motor;
}

/********** moveUp **********
 *
 * Move the lift motor up num_steps number of steps.
 *
 * Parameters:
 *      uint32_t num_steps: Move the motor up this many steps. 
 * 
 * Return:
 *      Nothing. Moves the motor up TODO: Figure out actual distance for each
 *      motor.
 *
 * Expects:
 *      Unsigned number. 
 *
 ************************/
void Wheel::moveUp(uint32_t num_steps) {

    /* TODO: Incorporate logic to be specific in how many milimeters the motor
    lifts */

    lift_motor->move(num_steps);
}

/********** moveDown **********
 *
 * Move the lift motor down num_steps number of steps.
 *
 * Parameters:
 *      uint32_t num_steps: Move the motor down this many steps. 
 * 
 * Return:
 *      Nothing. Moves the motor down TODO: Figure out actual distance for each
 *      motor.
 *
 * Expects:
 *      Unsigned number. 
 *
 * Notes:
 *      Argument purely specifies number of steps to move. DO NOT SAY "Move
 *      -8000 steps". Use an unsigned number
 *      TODO: 
 *      
 ************************/
void Wheel::moveDown(uint32_t num_steps) {
    lift_motor->move((int32_t)-num_steps);
}

/********** turnRight **********
 *
 * Turn the motor right degrees number of degrees
 *
 * Parameters:
 *      uint32_t degrees: number of degrees to turn the motor to the right
 * 
 * Return:
 *      Nothing. Turns the motor the number of degrees specified in the argument
 *
 * Expects:
 *      Unsigned number in between 0 and 360. Can use larger number, but that
 *      would just turn the motor more than a full rotation
 *
 * Notes:
 *      Here's the math: 
 *      STEPS_PER_REV / 360 = STEPS_PER_DEGREE_TURNED (STEPS_PER_DEGREE_TURNED
 *      is not an actual declared variable. Just an intermediate step)
 *      STEPS_PER_DEGREE_TURNED * TURN_GEARBOX_RATIO = steps_needed
 *      
 ************************/
void Wheel::turnRight(uint32_t degrees) {
    // float steps_needed = (degrees / 360) * FULL_TURN_ROTATION;
    turn_motor->move(degrees, false);
}

/********** turnLeft **********
 *
 * Turn the motor left degrees number of degrees
 *
 * Parameters:
 *      uint32_t degrees: number of degrees to turn the motor to the left
 * 
 * Return:
 *      Nothing. Turns the motor the number of degrees specified in the argument
 *
 * Expects:
 *      Unsigned number in between 0 and 360. Can use larger number, but that
 *      would just turn the motor more than a full rotation
 *
 * Notes:
 *      Here's the math: 
 *      STEPS_PER_REV / 360 = STEPS_PER_DEGREE_TURNED (STEPS_PER_DEGREE_TURNED
 *      is not an actual declared variable. Just an intermediate step)
 *      STEPS_PER_DEGREE_TURNED * TURN_GEARBOX_RATIO = steps_needed
 * 
 *      After that, just make the number negative to turn it left
 *      
 ************************/
void Wheel::turnLeft(uint32_t degrees) {
    // float steps_needed = (degrees / 360) * FULL_TURN_ROTATION;
    turn_motor->move((int32_t)-degrees);
}

/* Moves the wheel forwards by given steps */
void Wheel::moveForward(uint32_t num_inches) {
    // float steps_needed = ((float)num_inches / (float)WHEEL_CIRCUMFERENCE) * (float)FULL_DRIVE_ROTATION;
    // drive_motor->move((uint32_t)steps_needed, true);
    drive_motor->move((int32_t)num_inches, false);
    // if(drive_motor->getStepPin() == 15){
    //     digitalWrite(19,testing_bool);
    //     testing_bool = !testing_bool;
    // }
    // digitalWrite(19,testing_bool);
    // testing_bool = !testing_bool;
}

/********** moveBackwards **********
 * 
 * Move the wheel backwards by given steps.
 * 
 * Inputs:
 *   uint32_t num_inches: Number of inches to move backwards.
 * 
 * Returns:
 *    None.
 * 
 ************************/
void Wheel::moveBackwards(uint32_t num_inches) {
    float steps_needed = (num_inches / WHEEL_CIRCUMFERENCE) * FULL_DRIVE_ROTATION;
    drive_motor->move((int32_t)-steps_needed);
}

/********** stopMoving **********
 * 
 * Stop the wheel from moving.
 * 
 * Inputs/Returns:
 *      None.
 * 
 ************************/
void Wheel::stopMoving() {
        drive_motor->stopMove();
}

/********** getCurrentPosition **********
 * 
 * Retrieve the current position of the lift stepper motor.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *    int32_t: Current position of the stepper in steps.
 * 
 * Notes: 
 * - The actual position may be off by the number of steps in the ongoing
 *   command. If precise real time position is needed, attaching a pulse 
 *   counter may be of help.
 *  
 ************************/
int32_t Wheel::getLiftCurrentPosition() {
    return lift_motor->getCurrentPosition();
}

/********** getTurnCurrentPosition **********
 * 
 * Retrieve the current position of the turn stepper motor.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *    int32_t: Current position of the stepper in steps.
 *  
 ************************/
int32_t Wheel::getTurnCurrentPosition() {
    return turn_motor->getCurrentPosition();
}

/********** getDriveCurrentPosition **********
 * 
 * Retrieve the current position of the drive stepper motor.
 * 
 * Inputs:
 *   None.
 * 
 * Returns:
 *    int32_t: Current position of the stepper in steps.
 *  
 ************************/
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