#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>
#include <WiFi.h>

class WebPage {
  public:
    WebPage(const char* ssid, const char* password, int ledPin = LED_BUILTIN);
    ~WebPage();

    void begin();            // Start AP and web server
    void handleClient();     // Serve incoming HTTP requests
    void updateLED();        // Call in loop() to flash LED based on state

  private:
    const char* _ssid;
    const char* _password;
    int _ledPin;
    WiFiServer _server;

    String _state;           // Current "position" state

    unsigned long _lastFlashTime;
    bool _ledOn;

    unsigned long getFlashInterval();  // Return interval based on state
    String buildWebPage();             // Generate HTML page
    void handleRequest(String request); // Parse GET requests
};

#endif
