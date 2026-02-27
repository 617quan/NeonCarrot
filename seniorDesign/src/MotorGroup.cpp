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
        this->settings[2] = {};
        this->settings[3] = {};
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

void MotorGroup::moveForwards() {
    if (group_type == 't') {
        wheel1_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(-TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(-TURN_2_4_NUM_STEPS, false);
        delay(computeMoveTimeMs(TURN_1_3_NUM_STEPS, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL) + 100);
    }
}

void MotorGroup::moveForwards(float distance) {
    if (group_type == 'l') {
        int32_t steps_needed = (int)(distance * 100) * STEPS_PER_LIFT_HUNDREDTH_INCH;
        wheel1_motor->move(-steps_needed, false);
        wheel2_motor->move(steps_needed, false);
        wheel3_motor->move(steps_needed, false);
        wheel4_motor->move(steps_needed, false);
        delay(computeMoveTimeMs(steps_needed, LIFT_MAX_SPEED, LIFT_ACCEL) + 100);
    }
}

/********** moveForward **********
 *
 * 
 *
 * Parameters:
 *      
 * 
 * Return:
 *      
 *
 * Expects:
 *      
 *
 * Notes:
 *      
 *      
 ************************/
void MotorGroup::moveForwards(float distance, bool is_turning) {
    if (group_type == 'd') {
        int32_t steps_needed = convertInchesToSteps(distance);
        if (is_turning) {
            wheel1_motor->move(steps_needed, false);
            wheel2_motor->move(-steps_needed, false);
            delay(computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100);
        } else {
            wheel1_motor->move(steps_needed, false);
            wheel2_motor->move(steps_needed, false); 
            delay(computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100);
        }
    }
}

void MotorGroup::moveBackwards() {
    if (group_type == 't') {
        wheel1_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(TURN_2_4_NUM_STEPS, false);
        delay(computeMoveTimeMs(TURN_1_3_NUM_STEPS, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL) + 100);
    }
}

void MotorGroup::moveBackwards(float distance) {
    if (group_type == 'l') {
        int32_t steps_needed = (int)(distance * 100) * STEPS_PER_LIFT_HUNDREDTH_INCH;
        wheel1_motor->move(steps_needed, false);
        wheel2_motor->move(steps_needed, false);
        wheel3_motor->move(steps_needed, false);
        wheel4_motor->move(steps_needed, false);
        delay(computeMoveTimeMs(steps_needed, LIFT_MAX_SPEED, LIFT_ACCEL) + 100);
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
void MotorGroup::moveBackwards(float distance, bool is_turning) {
    if (group_type == 'd') {
        int32_t steps_needed = convertInchesToSteps(distance);
        if (is_turning) {
            wheel1_motor->move(-steps_needed, false);
            wheel2_motor->move(steps_needed, false); 
            delay(computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100);
        } else {
            wheel1_motor->move(-steps_needed, false);
            wheel2_motor->move(-steps_needed, false); 
            delay(computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100);
        }
    }
}

void MotorGroup::manualEnable() {
    if (group_type == 'd') {
        wheel1_motor->enableOutputs();
        wheel2_motor->enableOutputs();
    } else {
        wheel1_motor->enableOutputs();
        wheel2_motor->enableOutputs();
        wheel3_motor->enableOutputs();
        wheel4_motor->enableOutputs();
    }
}

void MotorGroup::manualDisable() {
    if (group_type == 'd') {
        wheel1_motor->disableOutputs();
        wheel2_motor->disableOutputs();
    } else {
        wheel1_motor->disableOutputs();
        wheel2_motor->disableOutputs();
        wheel3_motor->disableOutputs();
        wheel4_motor->disableOutputs();
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


uint32_t MotorGroup::computeMoveTimeMs(int32_t steps,
                           float max_speed,
                           float accel)
{
    float N = fabsf((float)steps);
    float v = max_speed;
    float a = accel;

    if (N <= 0.0f || v <= 0.0f || a <= 0.0f)
        return 0;

    // Distance needed to reach max speed
    float d_min = (v * v) / a;

    float total_time_sec = 0.0f;

    if (N >= d_min) {
        // Trapezoidal profile
        float t_accel = v / a;
        float d_cruise = N - d_min;
        float t_cruise = d_cruise / v;

        total_time_sec = 2.0f * t_accel + t_cruise;
    } else {
        // Triangular profile
        float v_peak = sqrtf(N * a);
        float t_accel = v_peak / a;

        total_time_sec = 2.0f * t_accel;
    }

    return (uint32_t)(total_time_sec * 1000.0f);
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