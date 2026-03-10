/********** MotorGroup.h **********
 * Purpose: Header file for the MotorGroup class. Defines important motor
 * information (wheel circumference, gearbox ratios, and microstep),
 * MotorSettings_t struct, and all of the MotorGroup methods. Relies heavily on the
 * FastAccelStepper github repository:
 * https://github.com/gin66/FastAccelStepper/tree/00462dea3e96813f6cbdc04bb2079be33c409ece
****************************************/

#ifndef _MotorGroup_H
#define _MotorGroup_H

#include <stdint.h>
#include <cmath>
#include "FastAccelStepper.h"
#include "defines.h"

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

    /* IN CASE ENABLES NEED TO BE MANUALLY SET */
    void manualEnable();
    void manualDisable();

    /* TODO: USE FOR EMERGENCY STOP */
    void stopMoving();

    bool isDoneMoving();

private:

    /* Call this in the constructor to initialize all motors to correct pins */
    FastAccelStepper* initMotor(MotorSettings_t lift_motor_settings);

    /* Calculates how long it will take for a movement command to execute */
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
    MotorSettings_t _settings[4];

    char _group_type; /* VERY IMPORTANT. 't' FOR TURN, 'd' FOR DRIVE, 'l' FOR LIFT */

    /* Timer to keep track of time left in a movement */
    unsigned long _moveDeadlineMs = 0;

};

#endif