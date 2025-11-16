/* wheel.h
 * Purpose: Wheel class
 */

#ifndef _MOTOR
#define _MOTOR

#include <stdint.h>
#include <Arduino.h>
#include "FastAccelStepper.h"
#include "pinout_defines.h"

#define WHEEL_RADIUS 3
#define WHEEL_CIRCUMFERENCE (2 * PI * WHEEL_RADIUS)
#define TURN_GEARBOX_RATIO 100
#define DRIVE_GEARBOX_RATIO 30
#define MICROSTEP 16
#define STEPS_PER_REV MICROSTEP * 200

#define FULL_DRIVE_ROTATION (STEPS_PER_REV * DRIVE_GEARBOX_RATIO)
#define FULL_TURN_ROTATION (STEPS_PER_REV * TURN_GEARBOX_RATIO)

/* TODO: Find how many steps moves the bot up x number of inches */

struct MotorSettings_t {
    uint8_t pulse_pin, enable_pin, dir_pin;
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
    void moveForward(uint32_t num_inches);
    void moveBackwards(uint32_t num_inches);

    void stopMoving();

    void calibratePosition();

private:

    /* Call these in the constructor to initialize all the pins */
    FastAccelStepper* initMotor(MotorSettings_t lift_motor_settings);

    /* Get the current position of each motor */
    int32_t getLiftCurrentPosition();
    std::array<int32_t, 4> getTurnCurrentPositions();
    int32_t getDriveCurrentPosition();

    /* Define the three motors and the engine. NOTE: static only instantiates
    one engine */
    static FastAccelStepperEngine engine;

    FastAccelStepper *drive_motors, *lift_motors, *turn1_motor, *turn2_motor, *turn3_motor, *turn4_motor;

    /* Three structs to hold all of the pin numbers */
    MotorSettings_t drive_motor_settings, lift_motor_settings, turn1_motor_settings;
    MotorSettings_t turn2_motor_settings, turn3_motor_settings, turn4_motor_settings;

};

#endif