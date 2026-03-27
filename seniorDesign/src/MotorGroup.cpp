/********** MotorGroup.cpp **********
 * 
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Implements the MotorGroup class which manages groups of stepper motors
 * (drive, lift, or turn). Using the FastAccelStepper library:
 * https://github.com/gin66/FastAccelStepper/tree/master
 * Handles initialization, motion commands, and time of movement calculation.
 * All movement commands are non blocking, as timers are utilized to allow
 * commands to be read in just in case of emergency
 * 
 ****************************************/
#include "MotorGroup.h"

/* Instantiates the engine object to be used for all stepper motors*/
FastAccelStepperEngine MotorGroup::engine;

/********** engineStartup **********
 *
 * Purpose:
 *      Initializes the shared FastAccelStepper engine used by all motors.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing.
 *
 * Expects:
 *      Must be called once in setup() before any MotorGroup objects are
 *      created or any motors are initialized.
 *
 * Notes:
 *      FastAccelStepper requires a single engine instance that manages the
 *      timers used for step generation. All motors connect through this
 *      engine.
 *
 ****************************************/
void MotorGroup::engineStartup() {
    engine.init();
}

/********** MotorGroup Constructor **********
 *
 * Purpose:
 *      Constructs a MotorGroup and initializes its associated stepper motors
 *      using the provided motor settings.
 *
 * Parameters:
 *      settings[4]  - Array containing configuration for each motor including
 *                     pulse pin, direction pin, enable pin, max speed, and
 *                     acceleration (Last two in steps per second).
 *
 *      _group_type   - Character identifying the type of motor group:
 *                          'd' = drive motors
 *                          'l' = lift motors
 *                          't' = turn motors
 *
 * Returns:
 *      A fully initialized MotorGroup object whose motors are ready to
 *      receive motion commands.
 *
 * Expects:
 *      - MotorGroup::engineStartup() has already been called.
 *      - The settings array contains valid pin assignments.
 *      - Drive groups only use the first two motors.
 *      - Lift and turn groups use all four motors.
 *
 * Notes:
 *      If a motor fails to initialize, the system prints an error message
 *      continuously and halts execution.
 *
 ****************************************/
MotorGroup::MotorGroup(MotorSettings_t settings[4], char group_type) {

    _group_type = group_type;

    // Store the settings in the class member
    _settings[0] = settings[0];
    _settings[1] = settings[1];
    wheel1_motor = initMotor(settings[0]);
    wheel2_motor = initMotor(settings[1]);
    
    /* If initializing drive motors, only initialize the first two */
    if (_group_type != 'd') {
        _settings[2] = settings[2];
        _settings[3] = settings[3];
        wheel3_motor = initMotor(settings[2]);
        wheel4_motor = initMotor(settings[3]);
    } else {
        _settings[2] = {};
        _settings[3] = {};
    }
    
}

/********** Destructor **********
 *
 * Purpose:
 *      Destroys the MotorGroup object.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing.
 *
 * Notes:
 *      The FastAccelStepper objects are managed by the shared engine, so no
 *      explicit cleanup is required here.
 *
 ****************************************/
MotorGroup::~MotorGroup() {
}


/********** initMotor **********
 *
 * Purpose:
 *      Initializes a FastAccelStepper motor using the provided configuration
 *      settings.
 *
 * Parameters:
 *      motor_settings - Structure containing:
 *              pulse_pin
 *              direction_pin
 *              enable_pin
 *              maximum speed
 *              acceleration
 *
 * Returns:
 *      Pointer to the initialized FastAccelStepper object.
 *
 * Expects:
 *      - The FastAccelStepper engine has already been initialized.
 *      - Pin numbers in motor_settings are valid.
 *      - The system has not exceeded the maximum number of supported motors (On
 *      ESP32, the limit is about 8).
 *
 * Notes:
 *      If the motor cannot be connected to the specified pulse pin, the
 *      function prints an error repeatedly and halts execution.
 *
 ****************************************/
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
    /* If a direction pin is specified, initialize it */
    if (motor_settings.dir_pin != 0) motor->setDirectionPin(motor_settings.dir_pin, true, 0);
    motor->setEnablePin(motor_settings.enable_pin, true);
    motor->setSpeedInHz(motor_settings.max_speed);
    motor->setAcceleration(motor_settings.accel);
    /* The enable will be auto set. If the motor is told to move, current will
    begin to flow into the motor. If there is no specified movement, no current
    will flow and the wheel will not lock up */
    motor->setAutoEnable(true);
    return motor;
}

/********** isDoneMoving **********
 *
 * Purpose:
 *      Determines whether the current movement command has finished.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      true  - The movement deadline has passed or no movement is active.
 *      false - The movement is still in progress.
 *
 * Expects:
 *      The movement deadline (_moveDeadlineMs) was set when a motion command
 *      was issued.
 *
 * Notes:
 *      This function does not query the motors directly. It only looks at the
 *      timer created when movement starts
 *
 ****************************************/
bool MotorGroup::isDoneMoving() {
    if (_moveDeadlineMs == 0) return true;
    if (millis() >= _moveDeadlineMs) {
        _moveDeadlineMs = 0;
        return true;
    }
    return false;
}

/********** moveForwards **********
 *
 * Purpose:
 *      Rotates the turning motor group to the forward position.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing. A non-blocking movement command is issued to all turn motors.
 *
 * Expects:
 *      The MotorGroup must represent a turn motor group ('t').
 *
 * Notes:
 *      Math: FULL_TURN_ROTATION (MICROSTEP * 200 * TURN_GEARBOX_RATIO)
 *      This calculates the number of steps per full wheel rotation. It
 *      multiples 200 (characteristic number of steps per full rotation with 1
 *      microstep), Microstep, and the gearbox ratio of the external gearbox
 *      ratio. 
 *      TURN_1_3_NUM_STEPS ((45.0f / 360.0f) * (float)FULL_TURN_ROTATION)
 *      TURN_2_4_NUM_STEPS ((135.0f / 360.0f) * (float)FULL_TURN_ROTATION)
 *      Motors 1 and 3 need to turn 45 degrees, and motors 2 and 4 need to
 *      rotate 135 degrees. So, just take the percent of steps needed for a full 
 *      rotation and execute that.
 *
 ****************************************/
void MotorGroup::moveForwards() {
    if (_group_type == 't') {
        uint32_t move_time = computeMoveTimeMs(TURN_1_3_NUM_STEPS, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL) + 100;
        wheel1_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(-TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(-TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(-TURN_2_4_NUM_STEPS, false);
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** moveForwards **********
 *
 * Purpose:
 *      Moves the lift mechanism upward by a specified distance.
 *
 * Parameters:
 *      distance - Distance to move in inches.
 *
 * Returns:
 *      Nothing. The lift motors begin a non-blocking movement.
 *
 * Expects:
 *      The MotorGroup must represent a lift motor group ('l').
 *
 * Notes:
 *      The distance (usually tenths or hundredths of an inch) is multiplied by
 *      100 to become an int, and then multiplied by
 *      STEPS_PER_LIFT_HUNDREDTH_INCH, which is an experimentally found value
 *      that finds out how many steps are needed to move the motor
 *
 ****************************************/
void MotorGroup::moveForwards(float distance) {
    if (_group_type == 'l') {
        int32_t steps_needed = (int)(distance * 100) * STEPS_PER_LIFT_HUNDREDTH_INCH;
        uint32_t move_time = computeMoveTimeMs(steps_needed, LIFT_MAX_SPEED, LIFT_ACCEL) + 100;
        wheel1_motor->move(-steps_needed, false);
        wheel2_motor->move(steps_needed, false);
        wheel3_motor->move(steps_needed, false);
        wheel4_motor->move(steps_needed, false);
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** moveForwards **********
 *
 * Purpose:
 *      Moves the robot forward by a specified distance using the drive motors.
 *
 * Parameters:
 *      distance   - Distance to move in inches.
 *      is_turning - If true, the wheels rotate in opposite directions to
 *                   perform a turn instead of straight motion.
 *
 * Returns:
 *      Nothing. The motors begin a non-blocking movement.
 *
 * Expects:
 *      The MotorGroup must represent a drive motor group ('d').
 *
 * Notes:
 *      The distance is divided by the wheel circumference to calculate what
 *      percent of a rotation needs to be executed. This is then multiplied by
 *      FULL_DRIVE_ROTATION, which is the number of steps needed for a full rotation
 *
 ****************************************/
void MotorGroup::moveForwards(float distance, bool is_turning) {
    if (_group_type == 'd') {
        int32_t steps_needed = (int32_t)((distance / (float)WHEEL_CIRCUMFERENCE) * (float)FULL_DRIVE_ROTATION);
        uint32_t move_time = computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100;
        if (is_turning) {
            wheel1_motor->move(steps_needed, false);
            wheel2_motor->move(-steps_needed, false);
        } else {
            wheel1_motor->move(steps_needed, false);
            wheel2_motor->move(steps_needed, false); 
        }
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** moveBackwards **********
 *
 * Purpose:
 *      Rotates the turning motor group back to the reverse position.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing. A non-blocking movement command is issued.
 *
 * Expects:
 *      The MotorGroup must represent a turn motor group ('t').
 *
 * Notes:
 *      Math: FULL_TURN_ROTATION (MICROSTEP * 200 * TURN_GEARBOX_RATIO)
 *      This calculates the number of steps per full wheel rotation. It
 *      multiples 200 (characteristic number of steps per full rotation with 1
 *      microstep), Microstep, and the gearbox ratio of the external gearbox
 *      ratio. 
 *      TURN_1_3_NUM_STEPS ((45.0f / 360.0f) * (float)FULL_TURN_ROTATION)
 *      TURN_2_4_NUM_STEPS ((135.0f / 360.0f) * (float)FULL_TURN_ROTATION)
 *      Motors 1 and 3 need to turn 45 degrees, and motors 2 and 4 need to
 *      rotate 135 degrees. So, just take the percent of steps needed for a full 
 *      rotation and execute that.
 *
 ****************************************/
void MotorGroup::moveBackwards() {
    if (_group_type == 't') {
        uint32_t move_time = computeMoveTimeMs(TURN_1_3_NUM_STEPS, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL) + 100;
        wheel1_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel2_motor->move(TURN_2_4_NUM_STEPS, false);
        wheel3_motor->move(TURN_1_3_NUM_STEPS, false);
        wheel4_motor->move(TURN_2_4_NUM_STEPS, false);
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** moveBackwards **********
 *
 * Purpose:
 *      Moves the lift mechanism downward by a specified distance.
 *
 * Parameters:
 *      distance - Distance to move in inches.
 *
 * Returns:
 *      Nothing. A non-blocking motion command is issued.
 *
 * Expects:
 *      The MotorGroup must represent a lift motor group ('l').
 *
 * Notes:
 *      The distance (usually tenths or hundredths of an inch) is multiplied by
 *      100 to become an int, and then multiplied by
 *      STEPS_PER_LIFT_HUNDREDTH_INCH, which is an experimentally found value
 *      that finds out how many steps are needed to move the motor
 *
 ****************************************/
void MotorGroup::moveBackwards(float distance) {
    if (_group_type == 'l') {
        int32_t steps_needed = (int)(distance * 100) * STEPS_PER_LIFT_HUNDREDTH_INCH;
        uint32_t move_time = computeMoveTimeMs(steps_needed, LIFT_MAX_SPEED, LIFT_ACCEL) + 100;
        wheel1_motor->move(steps_needed, false);
        wheel2_motor->move(steps_needed, false);
        wheel3_motor->move(steps_needed, false);
        wheel4_motor->move(steps_needed, false);
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** moveBackwards **********
 *
 * Purpose:
 *      Moves the robot backward by a specified distance using the drive
 *      motors.
 *
 * Parameters:
 *      distance   - Distance to move in inches.
 *      is_turning - If true, the wheels rotate in opposite directions to
 *                   perform a turning maneuver.
 *
 * Returns:
 *      Nothing. The motors begin a non-blocking movement.
 *
 * Expects:
 *      The MotorGroup must represent a drive motor group ('d').
 *
 * Notes:
 *      The distance is divided by the wheel circumference to calculate what
 *      percent of a rotation needs to be executed. This is then multiplied by
 *      FULL_DRIVE_ROTATION, which is the number of steps needed for a full rotation
 *
 ****************************************/
void MotorGroup::moveBackwards(float distance, bool is_turning) {
    if (_group_type == 'd') {
        int32_t steps_needed = (int32_t)((distance / (float)WHEEL_CIRCUMFERENCE) * (float)FULL_DRIVE_ROTATION);
        uint32_t move_time = computeMoveTimeMs(steps_needed, DRIVE_MAX_SPEED, DRIVE_ACCEL) + 100;
        if (is_turning) {
            wheel1_motor->move(-steps_needed, false);
            wheel2_motor->move(steps_needed, false); 
        } else {
            wheel1_motor->move(-steps_needed, false);
            wheel2_motor->move(-steps_needed, false); 
        }
        _moveDeadlineMs = millis() + move_time;
    }
}

/********** manualEnable **********
 *
 * Purpose:
 *      Manually enables the output drivers for all motors in the group.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing.
 *
 * Notes:
 *      Normally FastAccelStepper automatically enables drivers when motion
 *      begins. This function allows manual control when needed.
 *
 ****************************************/
void MotorGroup::manualEnable() {
    if (_group_type == 'd') {
        wheel1_motor->enableOutputs();
        wheel2_motor->enableOutputs();
    } else {
        wheel1_motor->enableOutputs();
        wheel2_motor->enableOutputs();
        wheel3_motor->enableOutputs();
        wheel4_motor->enableOutputs();
    }
}

/********** manualDisable **********
 *
 * Purpose:
 *      Manually disables the output drivers for all motors in the group.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing.
 *
 * Notes:
 *      Disabling the drivers removes holding torque from the motors.
 *
 ****************************************/
void MotorGroup::manualDisable() {
    if (_group_type == 'd') {
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
 * Purpose:
 *      Immediately stops all motors in the group.
 *
 * Parameters:
 *      None.
 *
 * Returns:
 *      Nothing.
 *
 * Notes:
 *      This cancels the currently executing movement command and halts
 *      all step generation for the motors.
 *
 ****************************************/
void MotorGroup::stopMoving() {
    wheel1_motor->stopMove();
    wheel2_motor->stopMove();
    wheel3_motor->stopMove();
    wheel4_motor->stopMove();
}

/********** computeMoveTimeMs **********
 *
 * Purpose:
 *      Calculates the amount of time required for a stepper motor movement
 *      based on the number of steps, maximum speed, and acceleration.
 *
 * Parameters:
 *      steps      - Number of steps the motor will travel.
 *      max_speed  - Maximum motor speed in steps per second.
 *      accel      - Acceleration in steps per second squared.
 *
 * Returns:
 *      Movement time in milliseconds.
 *
 * Notes:
 *      The calculation assumes a standard stepper motion profile with
 *      acceleration and deceleration phases.
 *
 *      Two possible motion profiles are considered:
 *
 *      Trapezoidal Profile:
 *          The motor accelerates to max speed, travels at constant speed,
 *          then decelerates.
 *
 *      Triangular Profile:
 *          If the movement is too short to reach max speed, the motor
 *          accelerates to a peak velocity and immediately decelerates.
 *
 *      The function calculates the expected duration of the appropriate
 *      profile and returns the estimated total motion time.
 *
 ****************************************/
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
