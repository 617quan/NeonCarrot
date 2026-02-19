/********** MotorGroup.cpp **********
 * Purpose: Defines all methods for the MotorGroup class. Instantiates all
 * FastAccelStepper objects to control motors. 
 */ 

#include "MotorGroup.h"

/* Instantiates the engine object to be used for all stepper motors*/
FastAccelStepperEngine MotorGroup::engine;

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
void MotorGroup::engineStartup() {
    engine.init();
}

/********** MotorGroup Constructor **********
 *
 * Initialize a MotorGroup from an array of four MotorSettings_t structs.
 * Each entry must include pulse, direction, and enable pins plus speed/accel.
 *
 * Parameters:
 *      MotorSettings_t settings[4] - configuration for each wheel's stepper
 *
 * Returns:
 *      Constructs a MotorGroup whose FastAccelStepper pointers are initialized
 *      and ready for motion commands.
 *
 * Expects:
 *      - Call MotorGroup::engineStartup() before creating any MotorGroup.
 *      - Each settings[i] must contain valid, non-conflicting pin numbers.
 *      - Exactly four motor settings are expected.
 *
 * Notes:
 *      If a motor cannot be connected, the constructor prints an error and
 *      halts execution.
 *
 ************************/
MotorGroup::MotorGroup(MotorSettings_t settings[4], char group_type) {

    this->group_type = group_type;

    // Store the settings in the class member
    this->settings[0] = settings[0];
    this->settings[1] = settings[1];
    wheel1_motor = initMotor(settings[0]);
    wheel2_motor = initMotor(settings[1]);
    
    if (group_type != 'd') {
        this->settings[2] = settings[2];
        this->settings[3] = settings[3];
        wheel3_motor = initMotor(settings[2]);
        wheel4_motor = initMotor(settings[3]);
    } else {
        this->settings[2] = {0, 0, 0, 0, 0};
        this->settings[3] = {0, 0, 0, 0, 0};
    }

    // Initialize all motors using the settings provided
    
}

/********** Deconstructor **********/
MotorGroup::~MotorGroup() {
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
FastAccelStepper* MotorGroup::initMotor(MotorSettings_t motor_settings) {
    FastAccelStepper *motor = engine.stepperConnectToPin(motor_settings.pulse_pin);
    if (motor == nullptr) {
        char strBuf[60];
        sprintf(strBuf, "Motor with pulse pin %u cannot be initialized correctly", motor_settings.pulse_pin);
        while (1) {
            Serial.println(strBuf);
        }
        return nullptr;
    }
    if (motor_settings.dir_pin != 0) motor->setDirectionPin(motor_settings.dir_pin, true, 0);
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
 *      whole MotorGroup up a real given distance (say one inch)
 *
 * Expects:
 *      Unsigned number. This function only moves the MotorGroup up, not down.
 *
 ************************/
// void MotorGroup::moveUp(uint32_t num_steps) {

//     /* TODO: Incorporate logic to be specific in how many inches the motor
//     lifts */

//     /* Set the target positions when movements start */
//     lift_target_pos = lift_motors->getCurrentPosition() - int32_t(num_steps);
//     lift_motors->move(-int32_t(num_steps), false);
// }

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
 *      whole MotorGroup down a real given distance (say one inch)
 *
 * Expects:
 *      Unsigned number. This function only moves the MotorGroup down, not up.
 * 
 ************************/
// void MotorGroup::moveDown(uint32_t num_steps) {
//     /* Set the target positions when movements start */
//     lift_target_pos = lift_motors->getCurrentPosition() + int32_t(num_steps);
//     lift_motors->move(int32_t(num_steps), false);
// }

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
 *      would probably hit the MotorGroup, which is not good for the integrity of the
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
// void MotorGroup::turnRight(uint32_t degrees) {
//     int32_t steps_needed = ((float)degrees / 360.0f) * (float)FULL_TURN_ROTATION;
    
//     /* Set the target positions when movements start */
//     turn_target_pos[0] = turn1_motor->getCurrentPosition() - int32_t(steps_needed);
//     turn_target_pos[1] = turn2_motor->getCurrentPosition() - int32_t(steps_needed);
//     turn_target_pos[2] = turn3_motor->getCurrentPosition() - int32_t(steps_needed);
//     turn_target_pos[3] = turn4_motor->getCurrentPosition() - int32_t(steps_needed);
    
//     drive_motors->enableOutputs();
//     turn1_motor->move(-int32_t(steps_needed), false);
//     turn2_motor->move(-int32_t(steps_needed), false);
//     turn3_motor->move(-int32_t(steps_needed), false);
//     turn4_motor->move(-int32_t(steps_needed), false);
//     drive_motors->disableOutputs();
// }

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
 *      would probably hit the MotorGroup, which is not good for the integrity of the
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
// void MotorGroup::turnLeft(uint32_t degrees) {
//     float steps_needed = ((float)degrees / 360.0f) * (float)FULL_TURN_ROTATION; // 40,000 steps
    
//      /* Set the target positions when movements start */
//     turn_target_pos[0] = turn1_motor->getCurrentPosition() + int32_t(steps_needed);
//     turn_target_pos[1] = turn2_motor->getCurrentPosition() + int32_t(steps_needed);
//     turn_target_pos[2] = turn3_motor->getCurrentPosition() + int32_t(steps_needed);
//     turn_target_pos[3] = turn4_motor->getCurrentPosition() + int32_t(steps_needed);
    
//     drive_motors->enableOutputs();
//     turn1_motor->move(int32_t(steps_needed), false);
//     turn2_motor->move(int32_t(steps_needed), false);
//     turn3_motor->move(int32_t(steps_needed), false);
//     turn4_motor->move(int32_t(steps_needed), false);
//     drive_motors->disableOutputs();
// }

// void MotorGroup::rotateRight(uint32_t degrees) {
//     int32_t steps_needed_1_3 = convertDegreesToSteps(90, 13);
//     int32_t steps_needed_2_4 = convertDegreesToSteps(135, 24);
    
//     /* Set the target positions when movements start */
//     turn_target_pos[0] = turn1_motor->getCurrentPosition() - int32_t(steps_needed_1_3);
//     turn_target_pos[1] = turn2_motor->getCurrentPosition() - int32_t(steps_needed_2_4);
//     turn_target_pos[2] = turn3_motor->getCurrentPosition() - int32_t(steps_needed_1_3);
//     turn_target_pos[3] = turn4_motor->getCurrentPosition() - int32_t(steps_needed_2_4);
    
//     drive_motors->enableOutputs();
//     turn1_motor->move(-int32_t(steps_needed_1_3), false);
//     turn2_motor->move(-int32_t(steps_needed_2_4), false);
//     turn3_motor->move(-int32_t(steps_needed_1_3), false);
//     turn4_motor->move(-int32_t(steps_needed_2_4), false);
//     drive_motors->disableOutputs();
// }

// void MotorGroup::rotateLeft(uint32_t degrees) {
//     int32_t steps_needed_1_3 = convertDegreesToSteps(45, 13);
//     int32_t steps_needed_2_4 = convertDegreesToSteps(135, 24);
    
//     /* Set the target positions when movements start */
//     turn_target_pos[0] = turn1_motor->getCurrentPosition() + int32_t(steps_needed_1_3);
//     turn_target_pos[1] = turn2_motor->getCurrentPosition() + int32_t(steps_needed_2_4);
//     turn_target_pos[2] = turn3_motor->getCurrentPosition() + int32_t(steps_needed_1_3);
//     turn_target_pos[3] = turn4_motor->getCurrentPosition() + int32_t(steps_needed_2_4);
    
//     drive_motors->enableOutputs();
//     turn1_motor->move(-int32_t(steps_needed_1_3), false);
//     turn2_motor->move(-int32_t(steps_needed_2_4), false);
//     turn3_motor->move(-int32_t(steps_needed_1_3), false);
//     turn4_motor->move(-int32_t(steps_needed_2_4), true);
//     drive_motors->disableOutputs();

//     moveForward(100);
//     drive_motors->enableOutputs();
//     turn1_motor->move(int32_t(steps_needed_1_3), false);
//     turn2_motor->move(int32_t(steps_needed_2_4), false);
//     turn3_motor->move(int32_t(steps_needed_1_3), false);
//     turn4_motor->move(int32_t(steps_needed_2_4), true);
//     drive_motors->disableOutputs();

//     moveForward(100);
// }

/********** moveForward **********
 *
 * Move all of the drive motors forward num_inches inches. All move function 
 * calls are non blocking.
 *
 * Parameters:
 *      uint32_t num_inches: number of inches we want to move the MotorGroup.
 * 
 * Return:
 *      Nothing. Moves the whole MotorGroup forward the given number of inches.
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
void MotorGroup::moveForwards(float distance) {
    if (group_type == 'd') {
        int32_t steps_needed = convertInchesToSteps(distance);
        wheel1_motor->move(steps_needed, false);
        wheel2_motor->move(-steps_needed, false); 
    } else if (group_type == 't') {
        wheel1_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(TURN_2_4_NUM_STEPS, false);
    } else if (group_type == 'l') {
        wheel1_motor->move(distance, false);
        wheel2_motor->move(distance, false);
        wheel3_motor->move(distance, false);
        wheel4_motor->move(distance, false);
    }
}

/********** moveBackward **********
 *
 * Move all of the drive motors backward num_inches inches. All move function 
 * calls are non blocking.
 *
 * Parameters:
 *      uint32_t num_inches: number of inches we want to move the MotorGroup.
 * 
 * Return:
 *      Nothing. Moves the whole MotorGroup bachward the given number of inches.
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
void MotorGroup::moveBackwards(float distance) {
    if (group_type == 'd') {
        int32_t steps_needed = convertInchesToSteps(distance);
        wheel1_motor->move(-steps_needed, false);
        wheel2_motor->move(steps_needed, false);
    } else if (group_type == 't') {
        wheel1_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(-TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(-TURN_2_4_NUM_STEPS, false);
    } else if (group_type == 'l') {
        wheel1_motor->move(-distance, false);
        wheel2_motor->move(-distance, false);
        wheel3_motor->move(-distance, false);
        wheel4_motor->move(-distance, false);
    }
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
void MotorGroup::stopMoving() {
    wheel1_motor->stopMove();
    wheel2_motor->stopMove();
    wheel3_motor->stopMove();
    wheel4_motor->stopMove();
}

/********** convertInchesToSteps **********
 * 
 * Converts the desired number of inches for moving forwards to microsteps for
 * the stepper driver to execute.
 *
 * Parameters:
 *      float num_inches: number of inches to move.
 * 
 * Return:
 *      The number of microsteps as a float.
 *  
 ************************/
int32_t MotorGroup::convertInchesToSteps(float num_inches) {
    return (int32_t)((num_inches / (float)WHEEL_CIRCUMFERENCE) * (float)FULL_DRIVE_ROTATION);
}

/********** isMoving **********
 * 
 * Determine if the bot is currently moving by checking if the motor has 
 * reached its target by comparing positions.
 *
 * Parameters:
 *     None.
 * 
 * Return:
 *     True if the bot is moving, false if the bot is not moving.
 *  
 ************************/
bool MotorGroup::isMoving() {
    // // Check if drive motors have reached target
    // if (drive_motors->getCurrentPosition() != drive_target_pos) {
    //     return true;
    // }
    // // Check if lift motors have reached target
    // if (lift_motors->getCurrentPosition() != lift_target_pos) {
    //     return true;
    // }
    // // Check if turn motors have reached target
    // if (turn1_motor->getCurrentPosition() != turn_target_pos[0]) {
    //     return true;
    // }
    // if (turn2_motor->getCurrentPosition() != turn_target_pos[1]) {
    //     return true;
    // }
    // if (turn3_motor->getCurrentPosition() != turn_target_pos[2]) {
    //     return true;
    // }
    // if (turn4_motor->getCurrentPosition() != turn_target_pos[3]) {
    //     return true;
    // }
    
    // return false;
    return false;
}

//FUNCTION IS TEMPORARY, USED TO TEST INTERFACING WEBSERVER INTO MotorGroup CLASS - QUAN
void MotorGroup::printPosition(int positionNum) {
    if (positionNum == 1) position = 1;
    else if (positionNum == 2) position = 2;
    else if (positionNum == 3) position = 3;
    else if (positionNum == 4) position = 4;
    else if (positionNum == 5) position = 5;
    else if (positionNum == 6) position = 6;
    Serial.print("Position Number: " );
    Serial.println(position);
};