/* wheel.cpp
 * Purpose:
 */

#include "wheel.h"
#include <Arduino.h>

/* Constructor */
Wheel::Wheel() {
}

/* Destructor */
Wheel::~Wheel() {
}

void Wheel::enable(Motor motor_type) {
        digitalWrite(motor_type.enable_pin, HIGH);
}

void Wheel::disable(Motor motor_type) {
        digitalWrite(motor_type.enable_pin, LOW);
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

void Wheel::testPulseSignal(Motor motor_type) {
        bool switch_clock = false;
        for (int i = 0; i < 200; i++) {
                digitalWrite(motor_type.pulse_pin, switch_clock);
                switch_clock = !switch_clock;
        }
}