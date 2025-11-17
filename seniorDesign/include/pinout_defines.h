/* pinout_defines.h
 * Purpose: Has all of the pin number assignments for the motors.
 */

#ifndef _PINOUT_DEFINES
#define _PINOUT_DEFINES

#define DRIVE_PULSE 13
#define DRIVE_DIRECTION 12
#define DRIVE_ENABLE 14

#define LIFT_PULSE 27
#define LIFT_DIRECTION 26
#define LIFT_ENABLE 25

#define TURN_ENABLE 17

#define TURN1_PULSE 33
#define TURN1_DIRECTION 32

#define TURN2_PULSE 15
#define TURN2_DIRECTION 2

#define TURN3_PULSE 4
#define TURN3_DIRECTION 16

#define TURN4_PULSE 22
#define TURN4_DIRECTION 21

/* SPI Pins - just use VSPI for now */
#define SPI_CLK 18
#define VSPI_CIPO 19 // controller in peripheral out
#define VSPI_COPI 23 // controller out peripheral in
#define VSPI_CS 5 


#endif