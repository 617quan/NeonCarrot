/* motor.h
 * Purpose: Motor Class
 */

#ifndef _MOTOR
#define _MOTOR

#include <stdint.h>
#include "pinout_defines.h"
#include "FastAccelStepper.h"


class Motor {
public:
        Motor(uint8_t step_pin, uint8_t enable_pin, uint8_t swerve_pin);
        ~Motor();

private:

        int step_pin, enable_pin, swerve_pin;
        static FastAccelStepperEngine engine;
        FastAccelStepper *stepper;

};

#endif