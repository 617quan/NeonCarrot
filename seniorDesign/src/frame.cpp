/********** frame.cpp **********
 * Purpose: Defines all methods for the frame class. Instantiates all
 * FastAccelStepper objects to control motors. IMPORTANT: All of the drive
 * motors are physically wired to the same pins. IMPORTANT: All of the lift motors 
 * are wired to the same pins. IMPORTANT: All of the turn motors have individual 
 * pulse and direction pins but share an enable pin. Microstep, gearbox ratio,
 * and other importand variables are defined in the .h file.
 * Notes: Example: If we were to have a microstep setting of 1/8:
 *        Micro step at 8 = 1600 micro steps per step
 *        Pulses per revolution: Motor requires 1600 input pulses to complete 360° rotation
 *        Gear box ratio for drive is 1:30 -> 1600 * 30 = 48000 steps needed for one wheel
 *        Gear box ratio for turn is 1:100 -> 1600 * 100 = 160000 steps needed for one wheel
 */ 

#include "frame.h"
#include "pinout_defines.h"

/* Instantiates the engine object to be used for all stepper motors*/
FastAccelStepperEngine Frame::engine;

/********** engineStartup **********
 *
 * Initializes the engine to be used to initialize all FastAccelStepper objects.
 * Needs to be called before any initialization happens or else none of the
 * motors can be setup correctly.
 *
 * Parameters:
 *      Nothing.
 * 
 * Return:
 *      Nothing. Engine object is now initialized and ready to be used to
 *      connect motors to pulse pins.
 *
 * Expects:
 *      Nothing. Should be called in setup before any stepper motors are
 *      created.
 *
 * Notes:
 *      None.
 *      
 ************************/
void Frame::engineStartup() {
    engine.init();
}

/********** Frame Constructor **********
 *
 * Takes in information (pulse pin, direction pin, enable pin, max speed,
 * acceleration) for the singular drive motor, singular lift motor, and 4 turn
 * motors and initializes the motors.
 *
 * Parameters:
 *      MotorSettings_t lift_motor_settings: pinout, max speed, and acceleration
 *      for lift motor
 *      MotorSettings_t drive_motor_settings: pinout, max speed, and 
 *      acceleration for drivemotor
 *      MotorSettings_t turn_motor_settings[]: pinout, max speed, and 
 *      acceleration for each individual turn motor. All enables are the same.
 * 
 * Return:
 *      A Frame object with all motors initialized, and ready to be moved.
 *
 * Expects:
 *      All pinouts are defined and correctly added into MotorSettings_t
 *      structs. No direction pins are shared for motors of the same type.
 *      Expects 4 turn_motor_settings in the turn_motor_settings array.
 *
 * Notes:
 *      None.
 *      
 ************************/
Frame::Frame(MotorSettings_t drive_motor_settings, 
             MotorSettings_t lift_motor_settings, 
             MotorSettings_t turn_motor_settings[]) {

    /* Take the arguments from the constructor and set them equal to the private
    class members */
    this->lift_motor_settings = lift_motor_settings;
    this->drive_motor_settings = drive_motor_settings;
    this->turn1_motor_settings = turn_motor_settings[0];
    this->turn2_motor_settings = turn_motor_settings[1];
    this->turn3_motor_settings = turn_motor_settings[2];
    this->turn4_motor_settings = turn_motor_settings[3];

    
    
    /* Initialize pins, max speed, and acceleration for all motors */
    drive_motors = initMotor(drive_motor_settings);
    lift_motors = initMotor(lift_motor_settings);
    turn1_motor = initMotor(turn1_motor_settings);
    turn2_motor = initMotor(turn2_motor_settings);
    turn3_motor = initMotor(turn3_motor_settings);
    turn4_motor = initMotor(turn4_motor_settings);

}

Frame::~Frame() {
/* Nothing new being made, so nothing here */
}


/********** initMotor **********
 *
 * Takes in a struct containing the pins, max speed, and acceleration, and 
 * initializes the motor with that information
 *
 * Parameters:
 *      MotorSettings_t motor_settings: struct containing pinout, speed, and
 *      accel info.
 * 
 * Return:
 *     a pointer to a FastAccelStepper motor that has been fully initialized.
 *     Normally, this should just be set equal to whatever type of motor (drive,
 *     lift, turn1, turn2, turn3, turn4) that is trying to be initialized.
 *
 * Expects:
 *      All variables in MotorSettings_t motor_settings are initialized. Expects
 *      that there are not already too many motors instantiated and initialized
 *      for another motor to work.
 *
 * Notes:
 *      Nothing
 *      
 ************************/
FastAccelStepper* Frame::initMotor(MotorSettings_t motor_settings) {
    FastAccelStepper *motor = engine.stepperConnectToPin(motor_settings.pulse_pin);
    if (motor == nullptr) {
        char strBuf[60];
        sprintf(strBuf, "Motor with pulse pin %u cannot be initialized correctly", motor_settings.pulse_pin);
        while (1) {
            Serial.println(strBuf);
        }
        return nullptr;
    }
    motor->setDirectionPin(motor_settings.dir_pin, true, 0);
    motor->setEnablePin(motor_settings.enable_pin, true);
    motor->setSpeedInHz(motor_settings.max_speed);
    motor->setAcceleration(motor_settings.accel);
    motor->setAutoEnable(true);
    return motor;
}

/********** moveUp **********
 *
 * Moves all of the lift motors up num_steps amount of steps. All moves are
 * non blocking.
 *
 * Parameters:
 *      uint32_t num_steps: Move the motor up this many steps. 
 * 
 * Return:
 *      Nothing. Moves the motor up TODO: Figure out how many steps move the
 *      whole frame up a real given distance (say one inch)
 *
 * Expects:
 *      Unsigned number. This function only moves the frame up, not down.
 *
 ************************/
void Frame::moveUp(uint32_t num_steps) {

    /* TODO: Incorporate logic to be specific in how many inches the motor
    lifts */

    lift_motors->move(-int32_t(num_steps), false);
}

/********** moveDown **********
 *
 * Moves all of the lift motors down num_steps amount of steps. All moves are
 * non blocking.
 *
 * Parameters:
 *      uint32_t num_steps: Move the motor down this many steps. 
 * 
 * Return:
 *      Nothing. Moves the motor down TODO: Figure out how many steps move the
 *      whole frame down a real given distance (say one inch)
 *
 * Expects:
 *      Unsigned number. This function only moves the frame down, not up.
 * 
 ************************/
void Frame::moveDown(uint32_t num_steps) {
    lift_motors->move(int32_t(num_steps), true);
}

/********** turnRight **********
 *
 * Turn the motor right degrees number of degrees. All move function calls are
 * non blocking.
 *
 * Parameters:
 *      uint32_t degrees: number of degrees to turn the motor to the right
 * 
 * Return:
 *      Nothing. Turns all of the turn motors the specified number of degrees.
 *
 * Expects:
 *      Unsigned number in between 0 and 360. Can use larger number, but that
 *      would just turn the motor more than a full rotation. In our case, it
 *      would probably hit the frame, which is not good for the integrity of the
 *      wheels.
 *
 * Notes:
 *      Here's the math: 
 *      All math is floating point so degrees / 360 does not set steps_needed =
 *      0. Multiply the desired number of degrees by 360 to get what portion of
 *      a full rotation the motor needs to take. Then, multiply by the number of
 *      steps required for a full rotation (FULL_TURN_ROTATION) to get the exact
 *      number of steps.
 *      
 ************************/
void Frame::turnRight(uint32_t degrees) {
    // float steps_needed = ((float)degrees / 360) * (float)FULL_TURN_ROTATION;
    turn1_motor->move(-int32_t(degrees), false);
    turn2_motor->move(-int32_t(degrees), false);
    turn3_motor->move(-int32_t(degrees), false);
    turn4_motor->move(-int32_t(degrees), false);
}

/********** turnLeft **********
 *
 * Turn the motor left degrees number of degrees. All move function calls are
 * non blocking.
 *
 * Parameters:
 *      uint32_t degrees: number of degrees to turn the motor to the left
 * 
 * Return:
 *      Nothing. Turns all the motors the number of degrees specified in the 
 *      argument
 *
 * Expects:
 *      Unsigned number in between 0 and 360. Can use larger number, but that
 *      would just turn the motor more than a full rotation. In our case, it
 *      would probably hit the frame, which is not good for the integrity of the
 *      wheels.
 *
 * Notes:
 *      Here's the math: 
 *      All math is floating point so degrees / 360 does not set steps_needed =
 *      0. Multiply the desired number of degrees by 360 to get what portion of
 *      a full rotation the motor needs to take. Then, multiply by the number of
 *      steps required for a full rotation (FULL_TURN_ROTATION) to get the exact
 *      number of steps.
 *      
 ************************/
void Frame::turnLeft(uint32_t degrees) {
    // float steps_needed = (degrees / 360) * (float)FULL_TURN_ROTATION;
    turn1_motor->move((int32_t)degrees, false);
    turn2_motor->move((int32_t)degrees, false);
    turn3_motor->move((int32_t)degrees, false);
    turn4_motor->move((int32_t)degrees, false);
}

/********** moveForward **********
 *
 * Move all of the drive motors forward num_inches inches. All move function 
 * calls are non blocking.
 *
 * Parameters:
 *      uint32_t num_inches: number of inches we want to move the frame.
 * 
 * Return:
 *      Nothing. Moves the whole frame forward the given number of inches.
 *
 * Expects:
 *      No negative arguments.
 *
 * Notes:
 *      Here's the math: 
 *      divide num_inches by the circumference of the wheel to find what portion
 *      of a full rotation the motor needs to move. Then, multiply by the number
 *      of steps to figure out how many steps needed to be driven.
 *      
 ************************/
void Frame::moveForward(uint32_t num_inches) {
    float steps_needed = ((float)num_inches / (float)WHEEL_CIRCUMFERENCE) * (float)FULL_DRIVE_ROTATION;
    drive_motors->move(steps_needed, false);
}

/********** moveBackward **********
 *
 * Move all of the drive motors backward num_inches inches. All move function 
 * calls are non blocking.
 *
 * Parameters:
 *      uint32_t num_inches: number of inches we want to move the frame.
 * 
 * Return:
 *      Nothing. Moves the whole frame bachward the given number of inches.
 *
 * Expects:
 *      No negative arguments.
 *
 * Notes:
 *      Here's the math: 
 *      divide num_inches by the circumference of the wheel to find what portion
 *      of a full rotation the motor needs to move. Then, multiply by the number
 *      of steps to figure out how many steps needed to be driven.
 *      
 ************************/
void Frame::moveBackwards(uint32_t num_inches) {
    float steps_needed = (num_inches / WHEEL_CIRCUMFERENCE) * FULL_DRIVE_ROTATION;
    drive_motors->move(-int32_t(steps_needed), false);
}

/********** stopMoving **********
 *
 * Immediately stops all of the motors from running.
 *
 * Parameters:
 *      Nothing
 * 
 * Return:
 *      Nothing. Stops all of the drive motors from turning, stopping the bot.
 *
 * Expects:
 *      Nothing.
 *      
 ************************/
void Frame::stopMoving() {
        drive_motors->stopMove();
}

/********** getLiftCurrentPosition **********
 *
 * Gets the current position of all of the lift motors based on the software's
 * tracking capabilities
 *
 * Parameters:
 *      None
 * 
 * Return:
 *      An integer representing the position of the lift motors
 *
 * Expects:
 *      The position has been initialized and is being tracked by the library.
 *
 * Notes:
 *      Uses the libraries position tracking function
 *      
 ************************/
int32_t Frame::getLiftCurrentPosition() {
    return lift_motors->getCurrentPosition();
}

/********** getTurnCurrentPosition **********
 *
 * Gets the current position of all of the turn motors based on the software's
 * tracking capabilities
 *
 * Parameters:
 *      None
 * 
 * Return:
 *      An array of integers representing the angle of all of the turn motors
 *
 * Expects:
 *      The position has been initialized and is being tracked by the library.
 *
 * Notes:
 *      Uses the libraries position tracking function
 *      
 ************************/
std::array<int32_t, 4> Frame::getTurnCurrentPositions() {
    return {turn1_motor->getCurrentPosition(), 
    turn2_motor->getCurrentPosition(), 
    turn3_motor->getCurrentPosition(), 
    turn4_motor->getCurrentPosition()};
}

/********** getDriveCurrentPosition **********
 *
 * Gets the current position of all of the drive motors based on the software's
 * tracking capabilities
 *
 * Parameters:
 *      None
 * 
 * Return:
 *      An integer representing the position of the drive motors
 *
 * Expects:
 *      The position has been initialized and is being tracked by the library.
 *
 * Notes:
 *      Uses the libraries position tracking function
 *      
 ************************/
int32_t Frame::getDriveCurrentPosition() {
    return drive_motors->getCurrentPosition();
}

/********** calibrateMotors **********
 *
 * TODO: WE NEED TO WRITE THIS FUNCTION.
 *
 * Parameters:
 *      None
 * 
 * Return:
 *      An integer representing the position of the lift motors
 *
 * Expects:
 *      The position has been initialized and is being tracked by the library.
 *
 * Notes:
 *      Uses the libraries position tracking function
 *      
 ************************/
void Frame::calibratePosition() {
    int32_t liftCurrPos = getLiftCurrentPosition();
    int32_t turnCurrPos = getLiftCurrentPosition();
    int32_t driveCurrPos = getLiftCurrentPosition();

    /* if (liftCurrPos isn't what we want it to be) {
        fix that shit
    } 
    */

}