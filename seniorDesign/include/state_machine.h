#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include <Arduino.h>


typedef enum {
    P1 = 49, // ascii 1
    P2 = 50, // ascii 2
    P3 = 51, // ascii 3
    P4 = 52  // ascii 4
} STATE_TYPE;

typedef enum {
    MOVE_TO_P1 = 63, // ascii ?
    MOVE_TO_P2 = 64, // ascii A
    MOVE_TO_P3 = 65, // ascii B
    MOVE_TO_P4 = 66  // ascii C
} MOVE_COMMAND;

class StateMachine {
public:
    StateMachine();

    void sendCommandToStateMachine(uint8_t command);
    void updateState();
    void stop();

    uint8_t getState();
    uint8_t getCommand();
    
private:
    uint8_t currentState;
    uint8_t currentCommand;
};

#endif
