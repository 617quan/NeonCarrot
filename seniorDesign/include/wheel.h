/* wheel.h
 * Purpose: Wheel class
 */

#ifndef _WHEEL
#define _WHEEL

#include <stdint.h>
#include <Arduino.h>
#include "FastAccelStepper.h"
#include "pinout_defines.h"

#define WHEEL_RADIUS 3
#define WHEEL_CIRCUMFERENCE (2 * PI * WHEEL_RADIUS)
#define TURN_GEARBOX_RATIO 100
#define DRIVE_GEARBOX_RATIO 30
#define MICROSTEP 8
#define STEPS_PER_REV MICROSTEP * 200

#define FULL_DRIVE_ROTATION (STEPS_PER_REV * DRIVE_GEARBOX_RATIO)
#define FULL_TURN_ROTATION (STEPS_PER_REV * TURN_GEARBOX_RATIO)

/* TODO: Find how many steps moves the bot up x number of inches */

struct MotorSettings_t {
    uint8_t pulse_pin, enable_pin, dir_pin;
    uint32_t max_speed = 0;
    int32_t accel = 0;
};

class Wheel {
public:
    Wheel(MotorSettings_t lift_motor_settings, 
          MotorSettings_t drive_motor_settings, 
          MotorSettings_t turn_motor_settings
         );

    ~Wheel();

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
    int32_t getTurnCurrentPosition();
    int32_t getDriveCurrentPosition();

    /* Define the three motors and the engine. NOTE: static only instantiates
    one engine */
    FastAccelStepper *lift_motor, *turn_motor, *drive_motor = nullptr;
    static FastAccelStepperEngine engine;

    /* Three structs to hold all of the pin numbers */
    MotorSettings_t lift_motor_settings, turn_motor_settings, drive_motor_settings;

};

#endif