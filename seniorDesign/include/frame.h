/* frame.h
 * Purpose: Header file for the frame class. Defines important motor
 * information (wheel circumference, gearbox ratios, and microstep),
 * MotorSettings_t struct, and all of the frame methods. Relies heavily on the
 * FastAccelStepper github repository:
 * https://github.com/gin66/FastAccelStepper/tree/00462dea3e96813f6cbdc04bb2079be33c409ece
 */

#ifndef _MOTOR
#define _MOTOR

#include <stdint.h>
#include <Arduino.h>
#include "FastAccelStepper.h"
#include "pinout_defines.h"

#define WHEEL_RADIUS 3
#define WHEEL_CIRCUMFERENCE (2 * PI * WHEEL_RADIUS)
#define DRIVE_GEARBOX_RATIO 30
#define MICROSTEP 1600
#define STEPS_PER_REV (MICROSTEP * 200)

#define FULL_DRIVE_ROTATION (STEPS_PER_REV * DRIVE_GEARBOX_RATIO)
#define FULL_TURN_ROTATION (STEPS_PER_REV)

#define TURN_1_3_SPEED 45000
#define Turn_2_4_SPEED (TURN_1_3_SPEED / 3)

#define TURN_1_3_ACCEL 22500
#define TURN_2_4_ACCEL (TURN_1_3_ACCEL / 3)

#define DRIVE_MAX_SPEED 20000
#define DRIVE_ACCEL 10000

#define LIFT_MAX_SPEED 50000
#define LIFT_ACCEL 2500

struct MotorSettings_t {
    uint8_t pulse_pin, dir_pin, enable_pin;
    uint32_t max_speed;
    int32_t accel;
};

class Frame {
public:
    Frame(MotorSettings_t drive_motor_settings, 
          MotorSettings_t lift_motor_settings, 
          MotorSettings_t turn_motor_settings[]
         );

    ~Frame();

    static void engineStartup(); /* Call this at the beginning of setup to get the engine setup */
    
    void moveUp(uint32_t num_steps);
    void moveDown(uint32_t num_steps);
    void turnRight(uint32_t degrees);
    void turnLeft(uint32_t degrees);
    void moveForward(float num_inches);
    void moveBackwards(uint32_t num_inches);

    void stopMoving();

    void calibratePosition();

    void printPosition(int positionNum); //FUNCTION IS TEMPORARY, USED TO TEST INTERFACING WEBSERVER INTO FRAME CLASS - QUAN

private:

    /* Call this in the constructor to initialize all motors to correct pins */
    FastAccelStepper* initMotor(MotorSettings_t lift_motor_settings);

    /* Getters for motor position. One function for each type of motor */
    int32_t getLiftCurrentPosition();
    std::array<int32_t, 4> getTurnCurrentPositions();
    int32_t getDriveCurrentPosition();

    /* Define the engine used to initialize motors. NOTE: one engine is
    needed for all motors */
    static FastAccelStepperEngine engine;

    /* Define stepper motor objects. These represent the physical motors on the
    frame. NOTE: All 4 drive motors share the same 3 pins. All 4 lift motors
    share the same 3 pins. Each turn motor has an individual pulse and direction
    pin, but they all share the same enable pin. */
    FastAccelStepper *drive_motors, *lift_motors, *turn1_motor, *turn2_motor, *turn3_motor, *turn4_motor;

    /* Structs to hold the information about each motor (pinouts, max speed,
    acceleration) */
    MotorSettings_t drive_motor_settings, lift_motor_settings, turn1_motor_settings;
    MotorSettings_t turn2_motor_settings, turn3_motor_settings, turn4_motor_settings;

    int position = 0; // used to track position based on webserver input
};

#endif