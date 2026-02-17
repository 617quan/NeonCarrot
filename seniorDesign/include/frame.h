/* frame.h
 * Purpose: Header file for the frame class. Defines important motor
 * information (wheel circumference, gearbox ratios, and microstep),
 * MotorSettings_t struct, and all of the frame methods. Relies heavily on the
 * FastAccelStepper github repository:
 * https://github.com/gin66/FastAccelStepper/tree/00462dea3e96813f6cbdc04bb2079be33c409ece
 */

#ifndef _FRAME_H
#define _FRAME_H

#include <stdint.h>
#include "Arduino.h"
#include <string>
#include "FastAccelStepper.h"
#include "defines.h"
#include "driver/spi_slave.h"
#include "state_machine.h"
#include "WebPage.h"

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

    /* Call this at the beginning of setup to get the engine setup */
    static void engineStartup();
    
    /* Movement Functions */
    void moveUp(uint32_t num_steps);
    void moveDown(uint32_t num_steps);
    void turnRight(uint32_t degrees);
    void turnLeft(uint32_t degrees);
    void rotateRight(uint32_t degrees);
    void rotateLeft(uint32_t degrees);
    void moveForward(float num_inches);
    void moveBackwards(float num_inches);

    void stopMoving();
    bool isMoving();
    void calibratePosition();

    void printPosition(int positionNum); //FUNCTION IS TEMPORARY, USED TO TEST INTERFACING WEBSERVER INTO FRAME CLASS - QUAN

private:

    /* Call this in the constructor to initialize all motors to correct pins */
    FastAccelStepper* initMotor(MotorSettings_t lift_motor_settings);

    /* Getters for motor position. One function for each type of motor */
    int32_t getLiftCurrentPosition();
    std::array<int32_t, 4> getTurnCurrentPositions();
    int32_t getDriveCurrentPosition();

    /* Conversion Functions */
    float convertInchesToSteps(float num_inches);
    float convertDegreesToSteps(uint32_t num_degrees, int32_t wheels);
    
    /* Define the engine used to initialize motors - one engine is needed for all motors */
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
    
    /* Target positions for movement tracking */
    int32_t drive_target_pos = 0;
    int32_t lift_target_pos = 0;
    int32_t turn_target_pos[4] = {0, 0, 0, 0};
};

#endif