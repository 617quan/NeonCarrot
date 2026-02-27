/* defines.h
 * Purpose: Has all of the pin number assignments for the motors. Holds constants
 * for the frame class and spi class.
 */

#ifndef _DEFINES
#define _DEFINES

/* STATE MACHINE TYPES */

typedef enum {
    P1 = 49,    // ascii 1
    P2 = 50,    // ascii 2
    P3 = 51,    // ascii 3
    P4 = 52,    // ascii 4
    MOVING = 53 // ascii 5 - state to represent any time the bot is actively
                // moving so that we can check if emergency stop is pressed
} STATE_TYPE;

typedef enum {
    MOVE_TO_P1 = 63,    // ascii ?
    MOVE_TO_P2 = 64,    // ascii A
    MOVE_TO_P3 = 65,    // ascii B
    MOVE_TO_P4 = 66,    // ascii C
    IDLE = 67 // ascii D
} MOVE_COMMAND;

/* ESP1 PINOUT */
#define DRIVE_ENABLE 13

#define FRONT_DRIVE_DIRECTION 12
#define FRONT_DRIVE_PULSE 14

#define BACK_DRIVE_DIRECTION 27
#define BACK_DRIVE_PULSE 26

#define LIFT_ENABLE 25
#define LIFT_DIRECTION 23

#define LIFT1_PULSE 33
#define LIFT2_PULSE 32
#define LIFT3_PULSE 15
#define LIFT4_PULSE 2

/* ESP2 PINOUT */

#define TURN_ENABLE 13

#define TURN1_PULSE 12
#define TURN1_DIRECTION 14

#define TURN2_PULSE 27
#define TURN2_DIRECTION 26

#define TURN3_PULSE 25
#define TURN3_DIRECTION 23

#define TURN4_PULSE 33
#define TURN4_DIRECTION 32

/* SPI Pin Defines. Not used anymore */
#define SPI_CLK 18
#define VSPI_CIPO 19 // controller in peripheral out
#define VSPI_COPI 23 // controller out peripheral in
#define VSPI_CS 5 

/* UART Pin Defines. RX and TX 2 used on motor boards, both used on mother board
*/
#define TX1 10
#define RX1 9
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

/* SPEED AND ACCELERATION DEFINES FOR ALL MOTOR TYPES. NOTE: INCREASING THESE
VALUES MAY CAUSE MOTORS TO STALL */
#define TURN_2_4_MAX_SPEED 22500
#define TURN_1_3_MAX_SPEED (TURN_2_4_MAX_SPEED / 3)

#define TURN_2_4_ACCEL 22500
#define TURN_1_3_ACCEL (TURN_2_4_ACCEL / 3)

#define DRIVE_MAX_SPEED 15000
#define DRIVE_ACCEL 10000

#define LIFT_MAX_SPEED 30000 
#define LIFT_ACCEL 1500

#endif