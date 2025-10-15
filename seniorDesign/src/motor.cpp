// /* motor.cpp
//  * Purpose: Make a Motor gang
//  */

// #include "motor.h"
// #include <Arduino.h>
// #include "FastAccelStepper.h"

// /* Constructor */
// Motor::Motor(uint8_t step_pin, uint8_t enable_pin, uint8_t swerve_pin) {

//     FastAccelStepperEngine engine;
//     engine.init();

//     stepper = engine.stepperConnectToPin(step_pin);

//     if(stepper != nullptr) {
//         /* SETS THE DIRECTION AND ENABLE PINS */
//         stepper->setDirectionPin(swerve_pin);
//         stepper->setEnablePin(enable_pin);
//         stepper->setAutoEnable(false);

//         stepper->setCurrentPosition(0);
//         stepper->setSpeedInHz(0);
//         stepper->setAcceleration(0);
//     } else {
//         while(1) {
//             Serial.println("Failed to connect stepper to pin!");
//         }
//     }
// }

// Motor::~Motor() {
// /* Nothing new being made, so nothing here */
// }

// void Motor::AccelForward(uint32_t steps_s_s) {

//     if (stepper->setAcceleration(steps_s_s) != (int8_t)steps_s_s) {
//         /* Something Broke */
//         return;
//     }
//     stepper->setLinearAcceleration(steps_s_s);
    
// }

// void Motor::spin_forward(uint32_t steps_s) {

//     stepper->setSpeedInHz(steps_s);

// }

/* THIS IS THE WORST DOCUMENTATION EVER:
https://github.com/gin66/FastAccelStepper/blob/master/src/FastAccelStepper.h

That is the .h file for the stepper motors. Honestly really bad.

*/

/* There are two stepper motor classes: FastAccelStepperEngine and
FastAccelStepper.

FastAccelStepperEngine: An object that (as far as I know) is almost completely
built of methods. You can call things (like stepperConnectToPin) to initalize
FastAccelStepper objects. If you want to know more about it, check out the .h
file.

Methods: 

    void init(uint8_t cpu_core) ONLY ON ESP32: Controls which core the motor runs
    on. If 0 or 1 is given, xTaskCreatePinnedToCore() is used to put it to 
    that specific core. If any other number is given, MF FREERTOSSSSSSS is
    used to schedule it on other avilable cores.

    void init(void): Just initialize

    FastAccelStepper *stepperConnectToPin(uint8_t step_pin): Returns a stepper motor
    object with the pulse pin mapped

    void setExternalCallForPin(bool (*func)(uint8_t pin, uint8_t value)): Don't even 
    worry about it. In case you have external circuitry that will set the pin.
    Need to give it a function pointer, but like I'm confused on how this would even work.

    void setDebugLed(uint8_t ledPin): Sets a debug LED for the motor. Pretty
    sure this blinks an led at 1hz when the motor is moving.

    inline void task_rate(uint8_t delay_ms): Dictates how many times per 
    second the background tasks running the stepper motor are executed. If 
    this number is high, we run the risk of having big delays between when we 
    actually call a stop function (for example) and when the motor actually stops.

    VARIABLE: _delay_ms.

FastAccelStepper: The actual motor object that can move, stop, and shake ass.

    uint8_t getStepPin(): Returns the step pin number

    void setDirectionPin(uint8_t dirPin, bool dirHighCountsUp = true, uint16_t dir_change_delay_us = 0): 
    Configures how the pin moves. Arguments:
        dirPin: The pin that controlls the direction
        dirHighCountsUp: true means that when dirPin is high, wheel moves forward
                        false means that when dirPin is low, wheel moves forward
        dir_change_delay_us: Adds a delay between when the dirPin is set and when the motor actually moves.
                            theres a min and max delay, but look at 298 on .h file for more info on that bs
    uint8_t getDirectionPin(): Returns direction pin
    bool directionPinHighCountsUp(): returns the dirHighCountsUp variable from above (I think)
    void setEnablePin(uint8_t enablePin, bool low_active_enables_stepper = true): Sets an Enable Pin and 
    configures the polarity. Arguments:
        enablePin: The enable pin you choose
        low_active_enables_stepper: true means that when enablePin is low, the motor functions
                                    false means that when enablePin is high, the motor functions
    inline uint8_t getEnablePinHighActive(): returns if you set the function before false
    inline uint8_t getEnablePinLowActive(): returns if you set the function before true
    bool enableOutputs(): Turns on the enable pin (no matter what mode you set it to before)
    bool disableOutputs(): Turns off the enable pin (no matter what mode you set it to before)
    void setAutoEnable(bool auto_enable): 
        IF TRUE: When the motor isn't moving, enable goes high
        (cutting off power to the motors). When we call a function to
        move the motor, the enable automatically switches low to allow
        for movement. When the motor stops moving, a timer starts. When
        the timer hits 1, enable goes high again to disable the motor.
        How long is the timer? No clue :(
        IF FALSE: We have to manually toggle the enable on and off when
        we want it to go on and off. 
    DelayResultCode setDelayToEnable(uint32_t delay_us): Set delay from enable to first step (in microseconds)
    void setDelayToDisable(uint16_t delay_ms): Set delay from last step to disable (in milliseconds)

    int8_t setSpeedInHz(uint32_t hz): Set speed in steps per second
    int8_t setSpeedInUs(uint32_t min_step_us): Set speed in microseconds per step
    int8_t setSpeedInMilliHz(uint32_t speed_mhz): Set speed in steps per 1000 seconds (speed_mhz = 1000 means 1 step per second)
    int8_t setSpeedInTicks(uint32_t min_step_ticks): ticks per tick (80Mhz I think?)
    YOU CAN GET ALL THESE TOO WITH FUNCTIONS getSpeedInUs() and so on
    uint32_t getMaxSpeedInUs() / getMaxSpeedInHz() / getMaxSpeedInMilliHz() / getMaxSpeedInTicks() tell you the max speed you can go
    int32_t getCurrentSpeedInUs(bool realtime = true): Gives you the actual speed of the motor in microseconds Argument:
            realtime: if true, gives you a more accurate reading. Can slow system down
                    if false, could be off by a couple ms.
    int32_t getCurrentSpeedInMilliHz(bool realtime = true): Same thing as above but in milliHz
    int8_t setAcceleration(int32_t step_s_s): Set acceleration in steps/s^2
    uint32_t getAcceleration(): gets the configured acceleration
    int32_t getCurrentAcceleration(): gets the actual acceleration
    
    SUPER IMPORTANT!!!
    void setLinearAcceleration(uint32_t linear_acceleration_steps): sets a linear acceleration to the set acceleration in steps/s^2
    (if linear_acceleration_steps = 100, then the motor speed will go from what it is to setAcceleration steps in 100 steps).

    MoveResultCode:
    MOVE_OK: All is OK:
    MOVE_ERR_NO_DIRECTION_PIN: Negative direction requested, but no direction pin
    MOVE_ERR_SPEED_IS_UNDEFINED: The maximum speed has not been set yet
    MOVE_ERR_ACCELERATION_IS_UNDEFINED: The acceleration to use has not been set yet

    MoveResultCode move(int32_t move, bool blocking = false): Tells the motor how far to move in steps.
            if blocking is false, this happens in the background. If true, nothing else can happen until this bs is done
    MoveResultCode moveTo(int32_t position, bool blocking = false): Moves to an absolute position
    I'm pretty sure (not 100%) that when the mcu turns on, it logs an initial position. Then, when you move
    anywhere, it tracks that movement. So, in relation to where your first move was called
    MoveResultCode runForward() / runBackward(): Run forward or run backward.
    MoveResultCode moveByAcceleration(int32_t acceleration, bool allow_reverse = true): Moves with acceleration specified.
    if allow_reverse is high, the motor can accelerate past 0.
    void stopMove(): Stop that shit
    void forwardStep(bool blocking = false) / backwardStep(bool blocking = false): Move one step
    void forceStopAndNewPosition(int32_t new_pos): Stop whatever you're doing and go to new_position
    void applySpeedAcceleration(): NO CLUE.
    void keepRunning(): Go forever
    int32_t getCurrentPosition(): Gets the current position. .h file says this if we're interested: 
    If precise real time position is needed, attaching a pulse counter may be of help.
    void setCurrentPosition(int32_t new_pos): Sets the current position as any given value.
    bool isRunning() / isStopping(): Self explanitory

*/