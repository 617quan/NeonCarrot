/********** defines.h **********
 *
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Provides all global constants, enumerations, and hardware configuration
 * values used throughout the robot control software. This file centralizes
 * definitions for system states, movement commands, motor control messages,
 * pin assignments, and mechanical conversion constants.
 *
 * The header also contains parameters used to translate real world movement
 * (inches or degrees) into stepper motor microsteps based on gearbox ratios,
 * wheel geometry, and microstepping configuration. Motor speed and
 * acceleration limits are also defined here so they can be tuned in a single
 * location without modifying motor control code.
 *
 ****************************************/

#ifndef _DEFINES
#define _DEFINES

/* STATE MACHINE TYPES */

typedef enum {
    P1,
    P2,
    P3,
    P4,
    MOVING
} STATE_TYPE;

typedef enum {
    MOVE_TO_P1,
    MOVE_TO_P2,
    MOVE_TO_P3,
    MOVE_TO_P4,
    IDLE,
    INITIALIZE
} MOVE_COMMAND;

/* DECLARED SO THAT ODD MESSAGES GO TO ESP1, AND EVEN MESSAGES GO TO ESP2 */
typedef enum {
    END_STAGE = 0,

    /* ESP1 COMMANDS (odd) */
    WHEELS_UP            = 1,
    WHEELS_DOWN          = WHEELS_UP + 2,
    MOVE_FORWARDS_24_IN  = WHEELS_DOWN + 2,
    MOVE_BACKWARDS_24_IN = MOVE_FORWARDS_24_IN + 2,
    TURN_RIGHT_90_DEGREES= MOVE_BACKWARDS_24_IN + 2,
    TURN_LEFT_90_DEGREES = TURN_RIGHT_90_DEGREES + 2,
    TURN_RIGHT_45_DEGREES= TURN_LEFT_90_DEGREES + 2,
    TURN_LEFT_45_DEGREES = TURN_RIGHT_45_DEGREES + 2,
    TURN_RIGHT_135_DEGREES= TURN_LEFT_45_DEGREES + 2,
    TURN_LEFT_135_DEGREES = TURN_RIGHT_135_DEGREES + 2,
    INIT_LIFT = TURN_LEFT_135_DEGREES + 2,

    /* ESP2 COMMANDS (even) */
    INITIATE_TURN_MOTORS = 2,
    RETURN_TURN_MOTORS   = INITIATE_TURN_MOTORS + 2,
    INIT_TURN = RETURN_TURN_MOTORS + 2,

    FINISH_MOVEMENT = 100

} MOTOR_COMMAND;

/* ESP1 PINOUT */

#define DRIVE_ENABLE 13

#define FRONT_DRIVE_DIRECTION 12
#define FRONT_DRIVE_PULSE 14

#define BACK_DRIVE_DIRECTION 27
#define BACK_DRIVE_PULSE 26

#define LIFT_ENABLE 18 
#define LIFT_DIRECTION 23

#define LIFT1_PULSE 19 
#define LIFT2_PULSE 21 
#define LIFT3_PULSE 2 
#define LIFT4_PULSE 15 

/* LIMIT SWITCHES */

#define BOARD1_LIMIT1 22
#define BOARD1_LIMIT2 25
#define BOARD1_LIMIT3 32
#define BOARD1_LIMIT4 33

#define BOARD2_LIMIT1 19
#define BOARD2_LIMIT2 22
#define BOARD2_LIMIT3 15
#define BOARD2_LIMIT4 21

/* ESP2 PINOUT */

#define TURN_ENABLE 13

#define TURN1_PULSE 14 
#define TURN1_DIRECTION 12 

#define TURN2_PULSE 33 
#define TURN2_DIRECTION 25 

#define TURN3_PULSE 32 
#define TURN3_DIRECTION 23

#define TURN4_PULSE 26 
#define TURN4_DIRECTION 27 

/* UART Pin Defines. RXD2 and TXD2 are the specified pins on the board, while
RXD1 and TXD1 are both remapped for UART channel 1 */
#define TXD1 5
#define RXD1 18
#define TXD2 17
#define RXD2 16

/* DEFINES FOR CONVERTING REAL LIFE DISTANCES TO STEPS FOR THE MOTORS */
#define WHEEL_DIAMETER 5.9035
#define WHEEL_CIRCUMFERENCE (PI * WHEEL_DIAMETER)
#define DRIVE_GEARBOX_RATIO 30
#define TURN_GEARBOX_RATIO 100
#define MICROSTEP 8
#define FULL_DRIVE_ROTATION (MICROSTEP * 200 * DRIVE_GEARBOX_RATIO)
#define FULL_TURN_ROTATION (MICROSTEP * 200 * TURN_GEARBOX_RATIO)
#define STEPS_PER_LIFT_HUNDREDTH_INCH 28846 /* NOTE: EXPERIMENTALLY FOUND */

/* THESE ARE THE ONLY AMOUNTS THE TURN MOTORS WILL EVER NEED TO TURN */

#define TURN_1_3_NUM_STEPS ((45.0f / 360.0f) * (float)FULL_TURN_ROTATION) // 40,000
#define TURN_2_4_NUM_STEPS ((135.0f / 360.0f) * (float)FULL_TURN_ROTATION) // 120,000

#define IN_FOR_45_DEGREE_TURN 11.3
#define IN_FOR_90_DEGREE_TURN 22.6
#define IN_FOR_135_DEGREE_TURN 33.9

/* SPEED AND ACCELERATION DEFINES FOR ALL MOTOR TYPES. NOTE: INCREASING THESE
VALUES MAY CAUSE MOTORS TO STALL */

#define TURN_2_4_MAX_SPEED 22500
#define TURN_1_3_MAX_SPEED (TURN_2_4_MAX_SPEED / 3)

#define TURN_2_4_ACCEL 22500
#define TURN_1_3_ACCEL (TURN_2_4_ACCEL / 3)

#define TURN_INIT_SPEED (TURN_1_3_MAX_SPEED / 4)

#define DRIVE_MAX_SPEED 15000
#define DRIVE_ACCEL 10000

#define LIFT_MAX_SPEED 30000 
#define LIFT_ACCEL 1500

#endif