/* wheel.h
 * Purpose: Wheel class
 */

#ifndef _WHEEL
#define _WHEEL

#include <stdint.h>

/* Motor struct */
struct Motor {
        //Pin numbers
        uint8_t enable_pin;
        uint8_t direction_pin;
        uint8_t pulse_pin;
};

class Wheel {
public:
        Wheel();
        ~Wheel();
        
        void enable(Motor motor_type);
        void disable(Motor motor_type);

        void setPulseHigh(Motor motor_type);
        void setPulseLow(Motor motor_type);

        void setDirHigh(Motor motor_type);
        void setDirLow(Motor motor_type);

        void testPulseSignal(Motor motor_type);

        /* Motor Types */
        Motor drive;
        Motor lift;
        Motor swerve;

private:

        /* Pin numbers */
        uint8_t pulse_lift_pin;
        uint8_t pulse_swerve_pin;
        uint8_t pulse_drive_pin;

        uint8_t enable_lift_pin;
        uint8_t enable_swerve_pin;
        uint8_t enable_drive_pin;

        uint8_t direction_lift_pin;
        uint8_t direction_swerve_pin;
        uint8_t direction_drive_pin;

        /* Pin values */
        int enable_lift_value;
};

#endif