#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include <Arduino.h>

/* Define states */
#define STATE_STOP  0
#define STATE_AT_P1 1
#define STATE_AT_P2 2
#define STATE_AT_P3 3
#define STATE_AT_P4 4

/* Define commands */
#define CMD_STOP_MOVE  0
#define CMD_MOVE_TO_P1 1
#define CMD_MOVE_TO_P2 2
#define CMD_MOVE_TO_P3 3
#define CMD_MOVE_TO_P4 4

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
