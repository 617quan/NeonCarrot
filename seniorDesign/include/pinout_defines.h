/* pinout_defines.h
 * Purpose: Has all of the pin number assignments for the motors.
 */

#ifndef _PINOUT_DEFINES
#define _PINOUT_DEFINES

#define DRIVE_PULSE 13
#define DRIVE_DIRECTION 33
#define DRIVE_ENABLE 32

#define LIFT_PULSE 14
#define LIFT_DIRECTION 22
#define LIFT_ENABLE 4

#define TURN_DIRECTION 15
#define TURN_ENABLE 2


/* WHEEL 1 PIN DEFINES */

#define WHEEL_1_TURN_PULSE 12


/* WHEEL 2 PIN DEFINES */

#define WHEEL_2_TURN_PULSE 26


/* SPI Pins - just use VSPI for now */
#define SPI_CLK 18
#define VSPI_CIPO 19 // controller in peripheral out
#define VSPI_COPI 23 // controller out peripheral in
#define VSPI_CS 5 


#endif