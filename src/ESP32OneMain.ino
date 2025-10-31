#include "SwerveModule.h"
#include "StepperMotor.h"
#include "WebPage.h"

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>

WebServer server(80);

//CHECK
const long STEPS_PER_METER = 10000;

int driveStep1 = 12;
int driveDir1 = 13;
int driveEnable1 = 4;

int steerStep1 = 14;
int steerDir1 = 18;
int steerEnable1 = 4;

int liftStep1 = 25;
int liftDir1 = 26;
int liftEnable1 = 4;

int driveStep2 = 19;
int driveDir2 = 21;
int driveEnable2 = 15;

int steerStep2 = 22;
int steerDir2 = 23;
int steerEnable2 = 15;

int liftStep2 = 27;
int liftDir2 = 32;
int liftEnable2 = 15;

StepperMotor drive1(driveStep1, driveDir1, driveEnable1);
StepperMotor steer1(steerStep1, steerDir1, steerEnable1);
StepperMotor lift1(liftStep1, liftDir1, liftEnable1);

SwerveModule mod1(&drive1, &steer1, &lift1);

StepperMotor drive2(driveStep2, driveDir2, driveEnable2);
StepperMotor steer2(steerStep2, steerDir2, steerEnable2);
StepperMotor lift2(liftStep2, liftDir2, liftEnable2);

SwerveModule mod2(&drive2, &steer2, &lift2);

int tx = 17; // purple
int rx = 16; // grey

HardwareSerial esp32Serial(1);

int curPos = 0;
bool isRaised = true;
volatile bool actionPending = false;
int pressCount = 0;

void handleRoot() {
    server.send(200, "text/html", MAIN_page);
}

void handleButton() {
    actionPending = true;
    server.send(200, "text/plain", "Button Pressed");
}

void handleStatus() {
    // Simple JSON format for webpage to read
    String json = "{";
    json += "\"position\":" + String(curPos) + ",";
    json += "\"raised\":" + String(isRaised ? "true" : "false");
    json += "}";
    server.send(200, "application/json", json);
}

void setup() {
    Serial.begin(115200);
    esp32Serial.begin(9600, SERIAL_8N1, rx, tx);

    WiFiManager wm;
//    wm.resetSettings();    // erases the stored SSID/password
//    ESP.restart();         // reboot into config-portal on next boot

    // This line blocks and shows the config portal if no saved WiFi
    bool res = wm.autoConnect("The_Beast");

    if (!res) {
        Serial.println("Failed to connect, restarting...");
        ESP.restart();
    }

    Serial.println("Connected! IP address:");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("thebeast")) {
        Serial.println("mDNS responder started");
        MDNS.addService("http", "tcp", 80);    // ← so clients know there’s a web server here
        // Now the page is available at http://thebeast.local
    }

    // Register web server routes
    server.on("/", handleRoot);
    server.on("/button", handleButton);
    server.on("/status", handleStatus);

    server.begin();
    Serial.println("Web server started");

    mod1.enable();
    mod2.enable();
}

void loop() {
  server.handleClient();

  if (!actionPending) {
    delay(10);
    return;
  }
  actionPending = false;

  // advance & wrap pressCount: 1..9, then 0 as “10th press”
  pressCount = (pressCount + 1) % 10;

  switch (pressCount) {
    case 1:
      // 1st press: Move to “position 0” (reset), stay raised
      curPos   = 0;
      isRaised = true;
      reset();
      break;

    case 2:
      // 2nd press: Move to position 1, stay raised
      curPos   = 1;
      isRaised = true;
      pos1();
      break;

    case 3:
      // 3rd press: lower only
      isRaised = false;
      liftAll(false);
      break;

    case 4:
      // 4th press: raise + move to pos2
      curPos   = 2;
      isRaised = true;
      liftAll(true);
      pos2();
      break;

    case 5:
      // 5th: lower only
      isRaised = false;
      liftAll(false);
      break;

    case 6:
      // 6th: raise + move to pos3
      curPos   = 3;
      isRaised = true;
      liftAll(true);
      pos3();
      break;

    case 7:
      // 7th: lower only
      isRaised = false;
      liftAll(false);
      break;

    case 8:
      // 8th: raise + move to pos4
      curPos   = 4;
      isRaised = true;
      liftAll(true);
      pos4();
      break;

    case 9:
      // 9th: lower only
      isRaised = false;
      liftAll(false);
      break;

    case 0:
      // 10th: raise + reset
      curPos   = 0;
      isRaised = true;
      reset();
      break;
  }
}


void reset() {
    esp32Serial.println("Reset");

    set0();

    Serial.println("Reset");
}

void pos1() {
    //from pos4

    esp32Serial.println("Pos1");

    move(0.6, "Fwd");
    turn(45, "Right");

    Serial.println("Position 1");
}

void pos2() {
    //90 degree rotation left

    esp32Serial.println("Pos2");

    turn(90, "Left");

    Serial.println("Position 2");
}

void pos3() {
    //200 mm btw pos 1 and 3
    //rotate 90 degrees right (back to pos 1)
    //move left 200 mm (0.2 m)

    esp32Serial.println("Pos3");

    turn(90, "Right");
    move(0.2, "Left");

    Serial.println("Position 3");
}

void pos4() {
    //move to pos 2
    //rotate 90 degrees right (back to pos 1)
    //rotate 45 degrees left
    //move 0.6 m down and right

    esp32Serial.println("Pos4");

    move(0.2, "Right");
    turn(45, "Left");
    move(0.6, "Back");

    Serial.println("Position 4");
}


void turn(int angle, String dir) {
    //for right turn
    float wheel1Angle = 43;
    float wheel2Angle = 319; //or -41

    float wheel1Dist = angleToDist(angle, 1);
    float wheel2Dist = angleToDist(angle, 2);

    if (dir == "Left") {
        wheel1Angle = 223;
        wheel2Angle = 139;

        wheel1Dist = -wheel1Dist;
        wheel2Dist = -wheel2Dist;
    }

    mod1.rotateWheel(wheel1Angle, 1);
    mod2.rotateWheel(wheel2Angle, 1);

    wait();

    mod1.moveWheel(wheel1Dist);
    mod2.moveWheel(-wheel2Dist);

    wait();

    Serial.println("Turned");
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
    mod1.rotateWheel(angle, 1);
    mod2.rotateWheel(angle, 1);

    wait();

    mod1.moveWheel(dist*STEPS_PER_METER);
    mod2.moveWheel(-dist*STEPS_PER_METER);

    wait();

    Serial.println("Moved");
}

float angleToDist(int angle, int wheel) {
    float radius;

    if (wheel == 1) {
        radius = 0.3556;
    }
    else if (wheel == 2) {
        radius = 0.4064;
    }

    float dist = radius * angle;
    return dist;
}

void wait() {
    while (mod1.isMoving() or mod2.isMoving()) {
        delay(100);
    }
}

void set0() {
    liftAll(true);

    wait();
    
    mod1.moveWheel(0);
    mod2.moveWheel(0);

    wait();

    mod1.rotateWheel(0, 1);
    mod2.rotateWheel(0, 1);

    wait();

    Serial.println("Set 0");
}

void liftAll(bool up) {
    esp32Serial.print("Lift,");
    esp32Serial.println(up ? "1" : "0");

    mod1.lift(up);
    mod2.lift(up);

    Serial.println("Lift all");
}
