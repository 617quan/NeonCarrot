/********** WebPage.h **********
 * 
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Defines the WebPage class which hosts a simple HTTP interface on the ESP32
 * using WiFi Access Point mode. Allows a user to connect to the ESP32 and send
 * MOVE_COMMAND instructions through a browser.
 *
 * The class manages WiFi initialization, HTTP client handling, webpage
 * generation, and parsing incoming GET requests to determine which command
 * the user issued. Commands are stored internally until retrieved by the
 * main program.
 *
 ****************************************/

#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>
#include <WiFi.h>
#include "defines.h"
#include "SPIFFS.h"

class WebPage {
  public:
    WebPage(const char* ssid, const char* password);
    ~WebPage();

    void begin();               // Start AP and web server
    void handleClient(STATE_TYPE curr_state); // Serve incoming HTTP requests

    bool hasNewCommand();       // True if user issued new command
    MOVE_COMMAND getCommand();  // Returns command and clears flag
    void setBusyMessage(bool busy);

  private:
    const char* _ssid;
    const char* _password;
    WiFiServer _server;

    MOVE_COMMAND _command;   // Current command
    bool _newCommand;        // Event flag

    String buildWebPage(STATE_TYPE curr_state);           // Generate HTML page
    void handleRequest(String request); // Parse GET requests
    bool _showBusyMessage;
};

#endif