/* defines.h
 * Purpose: Has all of the pin number assignments for the motors. Holds constants
 * for the frame class and spi class.
 */

#ifndef _DEFINES
#define _DEFINES

/* ESP1 PINOUT */
#define DRIVE_ENABLE 13

#define FRONT_DRIVE_DIRECTION 12
#define FRONT_DRIVE_PULSE 14

#define BACK_DRIVE_DIRECTION 27
#define BACK_DRIVE_PULSE 26

#define LIFT_ENABLE 25
#define LIFT_DIRECTION 17

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
#define TURN3_DIRECTION 17

#define TURN4_PULSE 33
#define TURN4_DIRECTION 32



/* Pinout Defines OLD */
// #define DRIVE_PULSE 13
// #define DRIVE_DIRECTION 12
// #define DRIVE_ENABLE 14

// #define LIFT_PULSE 27
// #define LIFT_DIRECTION 26
// #define LIFT_ENABLE 25

// #define TURN_ENABLE 17

// #define TURN1_PULSE 33
// #define TURN1_DIRECTION 32

// #define TURN2_PULSE 15
// #define TURN2_DIRECTION 2

// #define TURN3_PULSE 4
// #define TURN3_DIRECTION 16

// #define TURN4_PULSE 22
// #define TURN4_DIRECTION 21

/* SPI Pin Defines - just use VSPI for now */
#define SPI_CLK 18
#define VSPI_CIPO 19 // controller in peripheral out
#define VSPI_COPI 23 // controller out peripheral in
#define VSPI_CS 5 

/* Frame Defines */
#define WHEEL_DIAMETER 5.9035
#define WHEEL_CIRCUMFERENCE (PI * WHEEL_DIAMETER)
#define DRIVE_GEARBOX_RATIO 62
#define TURN_GEARBOX_RATIO 100
#define MICROSTEP 8
#define STEPS_PER_DRIVE_REV (MICROSTEP * 200 * DRIVE_GEARBOX_RATIO)
#define STEPS_PER_TURN_REV (MICROSTEP * 200 * TURN_GEARBOX_RATIO)

#define FULL_DRIVE_ROTATION (STEPS_PER_DRIVE_REV)
#define FULL_TURN_ROTATION (STEPS_PER_TURN_REV)

#define TURN_1_3_NUM_STEPS ((45.0f / 360.0f) * (float)FULL_TURN_ROTATION) // 40,000
#define TURN_2_4_NUM_STEPS ((135.0f / 360.0f) * (float)FULL_TURN_ROTATION) // 120,000

#define TURN_2_4_MAX_SPEED 22500
#define TURN_1_3_MAX_SPEED (TURN_2_4_MAX_SPEED / 3)

#define TURN_2_4_ACCEL 22500
#define TURN_1_3_ACCEL (TURN_2_4_ACCEL / 3)

#define DRIVE_MAX_SPEED 20000
#define DRIVE_ACCEL 10000

#define LIFT_MAX_SPEED 30000 // was 50,000 Emmett changed it 
#define LIFT_ACCEL 1500

#endif