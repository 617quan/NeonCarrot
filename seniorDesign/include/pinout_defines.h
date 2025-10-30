/* pinout_defines.h
 * Purpose: Has all of the pin number assignments for the motors.
 */

#ifndef _PINOUT_DEFINES
#define _PINOUT_DEFINES

/* WHEEL 1 PIN DEFINES */

/* Controls the Pulse pins for All 3 Motors on Wheel 1 */
#define WHEEL1_PULSE_LIFT 33
#define WHEEL1_PULSE_SWERVE 25
#define WHEEL1_PULSE_DRIVE 26

/* Controls the Enable pins for All 3 Motors on Wheel 1 */
#define WHEEL1_ENABLE_LIFT 23
#define WHEEL1_ENABLE_SWERVE 23
#define WHEEL1_ENABLE_DRIVE 23

/* Controls the Direction pins for All 3 Motors on Wheel 1 */
#define WHEEL1_DIRECTION_LIFT 16
#define WHEEL1_DIRECTION_SWERVE 17
#define WHEEL1_DIRECTION_DRIVE 5

/* WHEEL 2 PIN DEFINES */

/* Controls the Pulse pins for All 3 Motors on Wheel 2 */
#define WHEEL2_PULSE_LIFT 27
#define WHEEL2_PULSE_SWERVE 14
#define WHEEL2_PULSE_DRIVE 12

/* Controls the Enable pins for All 3 Motors on Wheel 2 */
#define WHEEL2_ENABLE_LIFT 23
#define WHEEL2_ENABLE_SWERVE 23
#define WHEEL2_ENABLE_DRIVE 23

/* Controls the Direction pins for All 3 Motors on Wheel 2 */
#define WHEEL2_DIRECTION_LIFT 16
#define WHEEL2_DIRECTION_SWERVE 17
#define WHEEL2_DIRECTION_DRIVE 5

/* WHEEL 3 PIN DEFINES */

/* Controls the Pulse pins for All 3 Motors on Wheel 3 */
#define WHEEL3_PULSE_LIFT 4 /* According to Quan's Big Diagram */
#define WHEEL3_PULSE_SWERVE 2 /* According to Quan's Big Diagram */
#define WHEEL3_PULSE_DRIVE 15 /* According to Quan's Big Diagram */

/* Controls the Enable pins for All 3 Motors on Wheel 3 */
#define WHEEL3_ENABLE_LIFT 23 /* According to Quan's Big Diagram */
#define WHEEL3_ENABLE_SWERVE 23 /* According to Quan's Big Diagram */
#define WHEEL3_ENABLE_DRIVE 23 /* According to Quan's Big Diagram */

/* Controls the Direction pins for All 3 Motors on Wheel 3 */
#define WHEEL3_DIRECTION_LIFT 16 /* NOTE: GRIFFIN THINKS WE NEED THIS. UNIVERSAL FOR ALL LIFT DIRECTIONS */
#define WHEEL3_DIRECTION_SWERVE 17 /* NOTE: GRIFFIN THINKS WE NEED THIS. UNIVERSAL FOR ALL SWERVE DIRECTIONS */
#define WHEEL3_DIRECTION_DRIVE 5 /* According to Quan's Big Diagram */

/* WHEEL 4 PIN DEFINES */

/* Controls the Pulse pins for All 3 Motors on Wheel 4 */
#define WHEEL4_PULSE_LIFT 21
#define WHEEL4_PULSE_SWERVE 19
#define WHEEL4_PULSE_DRIVE 18

/* Controls the Enable pins for All 3 Motors on Wheel 4 */
#define WHEEL4_ENABLE_LIFT 23
#define WHEEL4_ENABLE_SWERVE 23
#define WHEEL4_ENABLE_DRIVE 23

/* Controls the Direction pins for All 3 Motors on Wheel 4 */
#define WHEEL4_DIRECTION_LIFT 16
#define WHEEL4_DIRECTION_SWERVE 17
#define WHEEL4_DIRECTION_DRIVE 5

/* SPI Pins - just use VSPI for now */
#define SPI_CLK 18
#define VSPI_CIPO 23
#define VSPI_COPI 19
#define VSPI_CS 5
// #define HSPI_MISO
// #define HSPI_MOSI 
// #define HSPI_SS

#endif