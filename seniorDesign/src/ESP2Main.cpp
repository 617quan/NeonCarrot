/********** esp2_main.cpp **********
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Purpose: Motor control firmware for the ESP2 controller responsible
 * for the four turn motors. Receives commands from the parent board
 * and executes rotation movements through a MotorGroup interface.
 * Uses FastAccelStepper Library:
 * https://github.com/gin66/FastAccelStepper/tree/master
 * Uses 1 Enable, 4 Direction, and 4 Pulse pins for the turn motors
 * Uses 4 pins for Limit Switches
 **********************************/
#include "MotorGroup.h"
#include "defines.h"

void initMotorGroup();

MotorGroup *turn_motors = nullptr;

HardwareSerial Mother(2);

/********** setup **********
 *
 * Description:
 *      Initializes the serial communication with the parent controller, 
 *      initializes the motor control engine, and configures all turn motors.
 *
 * Parameters:
 *      Nothing.
 *
 * Return:
 *      Nothing. UART and motor control system are initialized and ready to 
 *      receive commands.
 *
 * Expects:
 *      Executed once during system boot before loop begins running.
 *
 * Notes:
 *      Calls initMotorGroup() to create the MotorGroup controlling
 *      the four turn motors on this board.
 *
 ************************/
void setup() {
    Serial.begin(115200);
    Mother.begin(115200, SERIAL_8N1, RXD2, TXD2);
    MotorGroup::engineStartup();
    initMotorGroup();
    pinMode(LED_BUILTIN, OUTPUT); 
}

/********** initMotorGroup **********
 *
 * Description:
 *      Initializes all GPIO pins associated with the four turn motors
 *      and constructs a MotorGroup object to manage their operation.
 *
 * Parameters:
 *      Nothing.
 *
 * Return:
 *      Nothing. The global MotorGroup pointer is initialized and ready
 *      to execute turn motor commands.
 *
 * Expects:
 *      MotorGroup::engineStartup() must have already been called so
 *      the FastAccelStepper engine is available for motor creation.
 *
 * Notes:
 *      Each motor has its own pulse and direction pins but shares
 *      a common enable line.
 *
 ************************/
void initMotorGroup() {

    pinMode(TURN_ENABLE, OUTPUT);

    pinMode(TURN1_DIRECTION, OUTPUT);
    pinMode(TURN1_PULSE, OUTPUT);

    pinMode(TURN2_DIRECTION, OUTPUT);
    pinMode(TURN2_PULSE, OUTPUT);

    pinMode(TURN3_DIRECTION, OUTPUT);
    pinMode(TURN3_PULSE, OUTPUT);

    pinMode(TURN4_DIRECTION, OUTPUT);
    pinMode(TURN4_PULSE, OUTPUT);

    pinMode(BOARD2_LIMIT1, INPUT_PULLUP);
    pinMode(BOARD2_LIMIT2, INPUT_PULLUP);
    pinMode(BOARD2_LIMIT3, INPUT_PULLUP);
    pinMode(BOARD2_LIMIT4, INPUT_PULLUP);

    MotorSettings_t turn1_settings = {TURN1_PULSE, TURN1_DIRECTION, TURN_ENABLE, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL};
    MotorSettings_t turn2_settings = {TURN2_PULSE, TURN2_DIRECTION, TURN_ENABLE, TURN_2_4_MAX_SPEED, TURN_2_4_ACCEL};
    MotorSettings_t turn3_settings = {TURN3_PULSE, TURN3_DIRECTION, TURN_ENABLE, TURN_1_3_MAX_SPEED, TURN_1_3_ACCEL};
    MotorSettings_t turn4_settings = {TURN4_PULSE, TURN4_DIRECTION, TURN_ENABLE, TURN_2_4_MAX_SPEED, TURN_2_4_ACCEL};

    MotorSettings_t turn_settings[4] = {turn1_settings, turn2_settings, turn3_settings, turn4_settings};

    turn_motors = new MotorGroup(turn_settings, 't');

    if (turn_motors == nullptr) {
        Serial.println("FATAL ERROR: turn motors not initialized");
    }

}

/********** loop **********
 *
 * Description:
 *      Continuously monitors the UART connection to the parent board
 *      for motor commands. When a valid command is received the turn
 *      motors execute the requested motion and report completion.
 *
 * Parameters:
 *      Nothing.
 *
 * Return:
 *      Nothing. Runs continuously after setup completes.
 *
 * Expects:
 *      MotorGroup must already be initialized and the parent UART
 *      connection must be active.
 *
 * Notes:
 *
 ************************/
void loop() {

    static bool waitingForMoveComplete = false;

    if (Mother.available() > 0) {
        MOTOR_COMMAND command = (MOTOR_COMMAND)Mother.read();

        // TODO: check for emergency stop here before anything else

        if (!waitingForMoveComplete) {
            waitingForMoveComplete = true;

            switch (command) {
                case INITIATE_TURN_MOTORS:  turn_motors->moveForwards();         break;
                case RETURN_TURN_MOTORS:    turn_motors->moveBackwards();        break;
                case INIT_TURN:             turn_motors->initMotorPositions();   break;
                default:                    waitingForMoveComplete = false;      break;
            }
        }
        // TODO: Add logic here for when board is requested to move when already moving
    }

    turn_motors->updateInit();

    // Separately, check if the current move just finished
    if (waitingForMoveComplete) {
        if (turn_motors->isDoneMoving()) {
            waitingForMoveComplete = false;
            Mother.write(END_STAGE);
        }
    }
}