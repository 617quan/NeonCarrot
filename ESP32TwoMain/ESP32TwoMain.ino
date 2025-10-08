//ESP32TwoMain.cpp
//By Alex Wardwell for Bonnie Schnitta (SoundSense)
#include "Arduino.h"
#include "SwerveModule.h"
#include "StepperMotor.h"


//CHECK
const long STEPS_PER_METER = 10000;

//Set up UART
int rx = 16; // purple
int tx = 17; // grey

int baud = 9600;

HardwareSerial esp32Serial(1);

String action;

//Set up Swerve Modules
int driveStep3 = 12;
int driveDir3 = 13;
int driveEnable3 = 4;

int steerStep3 = 14;
int steerDir3 = 18;
int steerEnable3 = 4;

int liftStep3 = 25;
int liftDir3 = 26;
int liftEnable3 = 4;

int driveStep4 = 19;
int driveDir4 = 21;
int driveEnable4 = 15;

int steerStep4 = 22;
int steerDir4 = 23;
int steerEnable4 = 15;

int liftStep4 = 27;
int liftDir4 = 32;
int liftEnable4 = 15;

StepperMotor drive3(driveStep3, driveDir3, driveEnable3);
StepperMotor steer3(steerStep3, steerDir3, steerEnable3);
StepperMotor lift3(liftStep3, liftDir3, liftEnable3);

SwerveModule mod3(&drive3, &steer3, &lift3);

StepperMotor drive4(driveStep4, driveDir4, driveEnable4);
StepperMotor steer4(steerStep4, steerDir4, steerEnable4);
StepperMotor lift4(liftStep4, liftDir4, liftEnable4);

SwerveModule mod4(&drive4, &steer4, &lift4);


void setup() {
    Serial.begin(115200);

    //Set up UART
    esp32Serial.begin(baud, SERIAL_8N1, rx, tx);
    Serial.println("Serial 2 started at 9600 baud rate");

    //Enable serve modules
    mod3.enable();
    mod4.enable();

}

void loop() {
  setup();
  
  liftAll();
  
    
  // // Only run when there’s a fresh line available
  // if (esp32Serial.available()) {
  //   // 1) Read exactly one line (up to '\n')
  //   String packet = esp32Serial.readStringUntil('\n');
  //   // 2) Trim off any '\r' or stray spaces
  //   packet.trim();                   
  //   Serial.println("RX packet: '" + packet + "'");

  //   // 3) Now dispatch it immediately, based on the clean string:
  //   if      (packet == "Stop") {
  //     // no motion
  //     Serial.println("Stop");
  //   }
  //   else if (packet == "Pos1") {
  //     move(0.6, "Fwd");
  //     turn(45, "Right");
  //     Serial.println("Position 1");
  //   }
  //   else if (packet == "Pos2") {
  //     turn(90, "Left");
  //     Serial.println("Position 2");
  //   }
  //   else if (packet == "Pos3") {
  //     turn(90, "Right");
  //     move(0.2, "Left");
  //     Serial.println("Position 3");
  //   }
  //   else if (packet == "Pos4") {
  //     move(0.2, "Right");
  //     turn(45, "Left");
  //     move(0.6, "Back");
  //     Serial.println("Position 4");
  //   }
  //   else if (packet.startsWith("Lift,")) {
  //     // e.g. “Lift,1” or “Lift,0”
  //     bool up = (packet.substring(packet.indexOf(',')+1) == "1");
  //     mod3.lift(up);
  //     mod4.lift(up);
  //     Serial.println(up ? "Lift Up" : "Lift Down");
  //   }
  //   else if (packet == "Reset") {
  //     set0();
  //     Serial.println("Reset");
  //   }
  //   else if (packet == "Disable") {
  //     mod3.disable();
  //     mod4.disable();
  //     Serial.println("Disable");
  //   }
  //   else {
  //     Serial.println("Unknown command: '" + packet + "'");
  //   }
  // }

  // // no more “else” chains here—only acts when a new packet arrives
  // delay(10);
}



void turn(int angle, String dir) {
    //for right turn
    int wheel3Angle = 43;
    int wheel4Angle = 319; //or -41

    float wheel3Dist = angleToDist(angle);
    float wheel4Dist = angleToDist(angle);

    if (dir == "Left") {
        wheel3Angle = 223;
        wheel4Angle = 139;

        wheel3Dist = -wheel3Dist;
        wheel4Dist = -wheel4Dist;
    }

    mod3.rotateWheel(wheel3Angle, 1);
    mod4.rotateWheel(wheel4Angle, 1);

    wait();

    mod3.moveWheel(-wheel3Dist);
    mod4.moveWheel(wheel4Dist);

    wait();
}

void move(float dist, String dir) {
    int angle;
    if (dir == "Right") {
        angle = 180;
    }
    else if (dir == "Left") {
        dist = -dist;
        angle = 180;
    }
    else if (dir == "Fwd") {
        angle = 0;
    }
    else if (dir == "Back") {
        dist = -dist;
        angle = 0;
    }
    //set wheel orientations
    mod3.rotateWheel(angle, 1);
    mod4.rotateWheel(angle, 1);

    wait();

    mod3.moveWheel(-dist*STEPS_PER_METER);
    mod4.moveWheel(dist*STEPS_PER_METER);

    wait();
}

float angleToDist(int angle) {
    float radius = 0.381;

    float dist = radius * angle;
    return dist;
}

void wait() {
    while (mod3.isMoving() or mod4.isMoving()) {
        delay(100);
    }
}

void set0() {
    liftAll();

    wait();
    
    mod3.moveWheel(0);
    mod4.moveWheel(0);

    wait();

    mod3.rotateWheel(0, 1);
    mod4.rotateWheel(0, 1);

    wait();
}

void liftAll() {
    String strUp = esp32Serial.readStringUntil('\n');
    bool up;
    if (strUp == "1") {
        up = true;
    }
    else if (strUp == "0") {
        up = false;
    }
    else {
        up = true;
    }

    mod3.lift(up);
    mod4.lift(up);
}
