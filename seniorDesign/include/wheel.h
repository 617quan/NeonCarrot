/* wheel.h
 * Purpose: Wheel class
 */

#ifndef _WHEEL
#define _WHEEL

#include <stdint.h>
#include <Arduino.h>
#include "FastAccelStepper.h"
#include "pinout_defines.h"

class Wheel {
public:
    Wheel(uint8_t lift_pulse_pin,  uint8_t lift_enable_pin,  uint8_t lift_dir_pin,
          uint8_t turn_pulse_pin,  uint8_t turn_enable_pin,  uint8_t turn_dir_pin,
          uint8_t drive_pulse_pin, uint8_t drive_enable_pin, uint8_t drive_dir_pin
    );
    ~Wheel();
    
    void moveUp(uint32_t num_steps);
    void moveDown(uint32_t num_steps);
    void moveRight(uint32_t num_steps);
    void moveLeft(uint32_t num_steps);
    void moveForward(uint32_t num_steps);
    void moveBackwards(uint32_t num_steps);

    void stopMoving();

    void calibratePosition();

private:

    void initMotorPulsePins(uint8_t lift_pulse_pin, uint8_t turn_pulse_pin, uint8_t drive_pulse_pin);

    int32_t getLiftCurrentPosition();
    int32_t getTurnCurrentPosition();
    int32_t getDriveCurrentPosition();

    FastAccelStepper *lift_motor;
    FastAccelStepper *turn_motor;
    FastAccelStepper *drive_motor;
    static FastAccelStepperEngine engine;

    /* Three driver pins for each motor */
    uint8_t  lift_pulse_pin,  lift_enable_pin,  lift_dir_pin;
    uint8_t  turn_pulse_pin,  turn_enable_pin,  turn_dir_pin;
    uint8_t  drive_pulse_pin, drive_enable_pin, drive_dir_pin;
    
    /* Max speed and accel for each motor */
    uint32_t lift_max_speed, turn_max_speed, drive_max_speed;
    int32_t  lift_accel,     turn_accel,     drive_accel;
        

};

#endif