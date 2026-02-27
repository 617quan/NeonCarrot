/* MotorGroup.h
 * Purpose: Header file for the MotorGroup class. Defines important motor
 * information (wheel circumference, gearbox ratios, and microstep),
 * MotorSettings_t struct, and all of the MotorGroup methods. Relies heavily on the
 * FastAccelStepper github repository:
 * https://github.com/gin66/FastAccelStepper/tree/00462dea3e96813f6cbdc04bb2079be33c409ece
 */

#ifndef _MotorGroup_H
#define _MotorGroup_H

#include <stdint.h>
#include <cmath>
#include "Arduino.h"
#include <string>
#include "FastAccelStepper.h"
#include "defines.h"
#include "driver/spi_slave.h"
#include "StateMachine.h"
#include "WebPage.h"

struct MotorSettings_t {
    uint8_t pulse_pin, dir_pin, enable_pin;
    uint32_t max_speed;
    int32_t accel;
};

class MotorGroup {
public:
    MotorGroup(MotorSettings_t settings[4], char group_type);
    ~MotorGroup();

    /* Call this at the beginning of setup to get the engine setup */
    static void engineStartup();
    
    /* Movement Functions */

    /* ONLY WORKS WITH TURN MOTORS. THEY ALWAYS TURN THE SAME AMOUNT, SO NO ARGS
    NEEDED */
    void moveForwards();
    void moveBackwards();

    /* ONLY WORKS WITH LIFT MOTORS. JUST GIVE A DISTANCE, AND THEY GO UP OR DOWN
    */
    void moveForwards(float distance);
    void moveBackwards(float distance);

    /* ONLY WORKS WITH DRIVE MOTORS. DISTANCE AND TURN NEEDED DUE TO BACK WHEELS
    MOUNTED IN A DIFFERENT ORIENTATION THAN FRONT. DIFFERENT DIRECTIONS NEEDED
    FOR BOTH SETS WHEN TURNING VS WHEN GOING STRAIGHT. */
    void moveForwards(float distance, bool is_turning);  // Forwards  = Up,   Clockwise
    void moveBackwards(float distance, bool is_turning); // Backwards = Down, Counter-Clockwise

    void manualEnable();
    void manualDisable();

    void stopMoving();
    bool isMoving();

    void printPosition(int positionNum); //FUNCTION IS TEMPORARY, USED TO TEST INTERFACING WEBSERVER INTO MotorGroup CLASS - QUAN

private:

    /* Call this in the constructor to initialize all motors to correct pins */
    FastAccelStepper* initMotor(MotorSettings_t lift_motor_settings);

    /* Getters for motor position. One function for each type of motor */
    std::array<int32_t, 4> getMotorPositions();

    /* Conversion Functions */
    int32_t convertInchesToSteps(float num_inches);

    uint32_t computeMoveTimeMs(int32_t steps, float max_speed, float accel);
    
    /* Define the engine used to initialize motors - one engine is needed for all motors */
    static FastAccelStepperEngine engine;

    /* Define stepper motor objects. Both turn and lift motors will have all of
    these initialized in the constructor. For the drive motors, only two of
    these will be initialized as front wheels share the same pulse pin */
    FastAccelStepper* wheel1_motor = nullptr;
    FastAccelStepper* wheel2_motor = nullptr;
    FastAccelStepper* wheel3_motor = nullptr;
    FastAccelStepper* wheel4_motor = nullptr;
    MotorSettings_t settings[4];

    char group_type; /* VERY IMPORTANT. 't' FOR TURN, 'd' FOR DRIVE, 'l' FOR LIFT */

    int position = 0; // used to track position based on webserver input
    
    /* Target positions for movement tracking */
    // int32_t drive_target_pos = 0;
    // int32_t lift_target_pos = 0;
    // int32_t turn_target_pos[4] = {0, 0, 0, 0};
};

#endif