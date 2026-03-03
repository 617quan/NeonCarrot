#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>
#include <WiFi.h>
#include "defines.h"

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