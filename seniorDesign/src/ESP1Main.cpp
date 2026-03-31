// /********** esp1_main.cpp **********
//  * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
//  * Purpose: Motor control for the ESP32 responsible for drive and lift motors. 
//  * Processes UART commands from the parent controller and executes movements 
//  * using MotorGroup objects. Uses FastAccelStepper Library:
//  * https://github.com/gin66/FastAccelStepper/tree/master
//  * Uses 1 Enable, 1 Direction, and 4 Pulse pins for the lift motors
//  * Uses 1 Enable, 2 Direction, and 2 Pulse pins for the drive motors
//  * Uses 4 pins for Limit Switches
//  **********************************/
// #include "MotorGroup.h"
// #include "defines.h"

// void initMotorGroup();
    
// MotorGroup *drive_motors = nullptr;
// MotorGroup *lift_motors = nullptr;

// HardwareSerial Mother(2);

// /********** setup **********
//  *
//  * Description:
//  *      Initializes the ESP communication with the parent controller. Starts the 
//  *      motor control engine and configures all pins required to drive the lift 
//  *      and drive motors.
//  *
//  * Parameters:
//  *      Nothing.
//  *
//  * Return:
//  *      Nothing. Hardware interfaces and motor engine are initialized.
//  *
//  * Expects:
//  *      Executed once during system boot before loop begins execution.
//  *
//  * Notes:
//  *      Calls initMotorGroup() to create and configure MotorGroup
//  *      instances used to control all stepper motors on this board.
//  *
//  ************************/
// void setup() {
//     Serial.begin(115200);
//     Mother.begin(115200, SERIAL_8N1, RXD2, TXD2);
//     MotorGroup::engineStartup();
//     initMotorGroup();

//     // Limit Switch Pins
//     // pinMode(LIMIT1, INPUT);
//     // pinMode(LIMIT2, INPUT);
//     // pinMode(LIMIT3, INPUT);
//     // pinMode(LIMIT4, INPUT);

//     // Initialize lift motors height
//     // liftMotorInit();
// }

// /********** initMotorGroup **********
//  *
//  * Description:
//  *      Configures all GPIO pins used by the drive and lift motors and
//  *      constructs MotorGroup objects using the required MotorSettings
//  *      structures. Each MotorGroup manages multiple stepper motors.
//  *
//  * Parameters:
//  *      Nothing.
//  *
//  * Return:
//  *      Nothing. Global MotorGroup pointers are initialized and ready
//  *      to issue motion commands.
//  *
//  * Expects:
//  *      MotorGroup::engineStartup() must be called before this function
//  *      so the underlying FastAccelStepper engine is available.
//  *
//  * Notes:
//  *      Two groups are created: one for the drive motors and one for the
//  *      lift motors controlling the wheel elevation mechanism.
//  *
//  ************************/
// void initMotorGroup() {

//     pinMode(DRIVE_ENABLE, OUTPUT);
//     pinMode(FRONT_DRIVE_DIRECTION, OUTPUT);
//     pinMode(FRONT_DRIVE_PULSE, OUTPUT);
//     pinMode(BACK_DRIVE_DIRECTION, OUTPUT);
//     pinMode(BACK_DRIVE_PULSE, OUTPUT);
//     pinMode(LIFT_ENABLE, OUTPUT);
//     pinMode(LIFT_DIRECTION, OUTPUT);
//     pinMode(LIFT1_PULSE, OUTPUT);
//     pinMode(LIFT2_PULSE, OUTPUT);
//     pinMode(LIFT3_PULSE, OUTPUT);
//     pinMode(LIFT4_PULSE, OUTPUT);

//     pinMode(BOARD1_LIMIT1, INPUT_PULLUP);
//     pinMode(BOARD1_LIMIT2, INPUT_PULLUP);
//     pinMode(BOARD1_LIMIT3, INPUT_PULLUP);
//     pinMode(BOARD1_LIMIT4, INPUT_PULLUP);

//     MotorSettings_t front_drive_settings = {FRONT_DRIVE_PULSE, FRONT_DRIVE_DIRECTION, DRIVE_ENABLE, DRIVE_MAX_SPEED, DRIVE_ACCEL};
//     MotorSettings_t back_drive_settings = {BACK_DRIVE_PULSE, BACK_DRIVE_DIRECTION, DRIVE_ENABLE, DRIVE_MAX_SPEED, DRIVE_ACCEL};
//     MotorSettings_t lift1_settings = {LIFT1_PULSE, LIFT_DIRECTION, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
//     MotorSettings_t lift2_settings = {LIFT2_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
//     MotorSettings_t lift3_settings = {LIFT3_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};
//     MotorSettings_t lift4_settings = {LIFT4_PULSE, 0, LIFT_ENABLE, LIFT_MAX_SPEED, LIFT_ACCEL};

//     MotorSettings_t drive_settings[4] = {front_drive_settings, 
//                                          back_drive_settings
//                                         };

//     MotorSettings_t lift_settings[4] = {lift1_settings, 
//                                         lift2_settings,
//                                         lift3_settings, 
//                                         lift4_settings
//                                         };

//     drive_motors = new MotorGroup(drive_settings, 'd');
//     if (drive_motors == nullptr) {
//         Serial.println("FATAL ERROR: drive motors initialized incorrectly");
//     }
//     lift_motors = new MotorGroup(lift_settings, 'l');
//     if (lift_motors == nullptr) {
//         Serial.println("FATAL ERROR: lift motors initialized incorrectly");
//     }
    
// }

// /********** liftMotorInit **********
//  *
//  * Description:
//  *      Configures all lift motors to starting position
//  *
//  * Parameters:
//  *      Nothing.
//  *
//  * Return:
//  *      Nothing. 
//  *
//  * Expects:
//  *      initMotorGroup() should be called before thise and it should only run
//  *      once during void setup.
//  * 
//  * Notes:
//  *
//  ************************/
// void liftMotorInit() {
//     // lifts motors move upwards all together
//     // lift_motors->moveForwards(8);
//     // int count;
//     // bool finished = false;
//     // while(!finished) {
//     //     //checks each limit switch until all have been pressed
//     //     count = 0;
//     //     if (digitalRead(LIMIT1) == Low) {
//     //         count++;
//     //         //stop lift motor 1
            
//     //     }
//     //     if (digitalRead(LIMIT2) == Low) {
//     //         count++
//     //         //stop lift motor 2
//     //     }
//     //     if (digitalRead(LIMIT3) == Low) {
//     //         count++
//     //         //stop lift motor 3
//     //     }
//     //     if (digitalRead(LIMIT4) == Low) {
//     //         count++;
//     //         // stop lift motor 4
//     //     }
//     //     if (count == 4) {
//     //         finished = true;
//     //     }
//     // }

//     // // TODO: Where do we want them to start at 
//     // // I assume wheel down 
//     // lift_motors->moveBackwards(0.4);
// }

// /********** loop **********
//  *
//  * Description:
//  *      Main execution loop which monitors the UART connection to the
//  *      parent controller for motor commands. Commands are translated
//  *      into motor actions and completion is reported when movement
//  *      finishes.
//  *
//  * Parameters:
//  *      Nothing.
//  *
//  * Return:
//  *      Nothing. Runs continuously after setup completes.
//  *
//  * Expects:
//  *      MotorGroup objects must already be initialized and the UART
//  *      connection to the parent controller must be active.
//  *
//  * Notes:
//  *
//  ************************/
// void loop() {

//     static bool waitingForMoveComplete = false;

//     if (Mother.available() > 0) {
//         MOTOR_COMMAND command = (MOTOR_COMMAND)Mother.read();

//         // TODO: check for emergency stop here before anything else

//         if (!waitingForMoveComplete) {
//             waitingForMoveComplete = true;

//             switch (command) {
//                 case WHEELS_UP:              lift_motors->moveForwards(.4);                             break;
//                 case WHEELS_DOWN:            lift_motors->moveBackwards(.4);                            break;
//                 case MOVE_FORWARDS_24_IN:    drive_motors->moveForwards(23.622, false);                 break;
//                 case MOVE_BACKWARDS_24_IN:   drive_motors->moveBackwards(23.622, false);                break;
//                 case TURN_RIGHT_90_DEGREES:  drive_motors->moveForwards(IN_FOR_90_DEGREE_TURN, true);   break;
//                 case TURN_LEFT_90_DEGREES:   drive_motors->moveBackwards(IN_FOR_90_DEGREE_TURN, true);  break;
//                 case TURN_RIGHT_45_DEGREES:  drive_motors->moveForwards(IN_FOR_45_DEGREE_TURN, true);   break;
//                 case TURN_LEFT_45_DEGREES:   drive_motors->moveBackwards(IN_FOR_45_DEGREE_TURN, true);  break;
//                 case TURN_RIGHT_135_DEGREES: drive_motors->moveForwards(IN_FOR_135_DEGREE_TURN, true);  break;
//                 case TURN_LEFT_135_DEGREES:  drive_motors->moveBackwards(IN_FOR_135_DEGREE_TURN, true); break;
//                 case INIT_LIFT:              lift_motors->initMotorPositions();                         break;
//                 default:                     waitingForMoveComplete = false;                            break;
//             }
//         }
//         // TODO: Add logic here for when board is requested to move when already moving
//     }

//     lift_motors->updateInit();

//     // Separately, check if the current move just finished
//     if (waitingForMoveComplete) {
//         if (lift_motors->isDoneMoving() && drive_motors->isDoneMoving()) {
//             waitingForMoveComplete = false;
//             Mother.write(END_STAGE);
//         }
//     }

// }
