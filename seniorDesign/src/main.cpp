/* main.cpp
 * Purpose: Pin framework for wheel bot motion. Defines pulse, enable, and
   direction on the MC.

 * FUNCTION NAMES = camelCase
 * GLOBAL VARIABLES = ALL CAPS
 * VARIABLES = snake_case
 */

#include <Arduino.h>
#include "pinout_defines.h"
#include "wheel.h"
#include "driver/ledc.h"

const int pin = 1;
int freq = 2000;
const int ledChannel = 0;
const int resolution = 8;
const int duty = (1 << resolution) / 2;
void driveForward(Wheel wheel);

/* MAIN */
void loop() {
    for(int i = 1000; i > 0; i-=2) {
        analogWriteFrequency(pin, freq--);
        delay(10);
    }
    delay(500);
    for(int i = 0; i < 1000; i+=2) {
        analogWriteFrequency(pin, freq++);
        delay(10);
    }
    delay(500);

}

void setup() {
    Serial.begin(115200);
    pinMode(pin, OUTPUT);

    /* LOOK INTO WHAT THE APB CLOCK ACTUALLY IS. IT'S DIVIDED FROM THE MAIN
    80MHZ CLK, BUT TO WHAT? */
    // if (!ledcSetClockSource(LEDC_USE_REF_TICK)) printf("Failure Configuring LEDC Source Clock.\n");
    if (!ledcAttachChannel(pin, freq, resolution, 1)) printf("Failure Setting up pin %d channel.\n", pin);
    if (!ledcWrite(pin, duty)) printf("Failure Writing to Pin.\n");

}

/* Move given wheel forwards */
void driveForward(Wheel wheel) {

}

void lift() {

}

void turn(){
    
}
