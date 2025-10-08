/* wheel.cpp
 * Purpose:
 */

#include "wheel.h"
#include <Arduino.h>

/* Constructor */
Wheel::Wheel() {
        
        /* assign pins */

        drive.enable_pin = WHEEL3_ENABLE_DRIVE;
        drive.direction_pin = WHEEL3_DIRECTION_DRIVE;
        drive.pulse_pin = WHEEL3_PULSE_DRIVE;
}

/* Destructor */
Wheel::~Wheel() {
}

void Wheel::enable(Motor motor_type) {
        // enable LOW means its on
        digitalWrite(motor_type.enable_pin, LOW);
}

void Wheel::disable(Motor motor_type) {
        // enable HIGH means its off
        digitalWrite(motor_type.enable_pin, HIGH);
}

void Wheel::setPulseHigh(Motor motor_type) {
        digitalWrite(motor_type.pulse_pin, HIGH);
}

void Wheel::setPulseLow(Motor motor_type) {
        digitalWrite(motor_type.pulse_pin, LOW);
}

void Wheel::setDirHigh(Motor motor_type) {
        digitalWrite(motor_type.direction_pin, HIGH);
}

void Wheel::setDirLow(Motor motor_type) {
        digitalWrite(motor_type.direction_pin, LOW);
}

void Wheel::testPulseSignal(Motor motor_type, int frequency) {
        // esp32 chip clock runs at 240MHZ
        // frequency is going to be in kHz so delay is in ms
        bool switch_clock = false;
        int time_delay = (1 / frequency) / 2;
        for ( ;; ) {
                digitalWrite(motor_type.pulse_pin, switch_clock);
                switch_clock = !switch_clock;
                delay(time_delay);
        }
}