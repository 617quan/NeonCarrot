#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>
#include <WiFi.h>

class WebPage {
  public:
    WebPage(const char* ssid, const char* password);
    ~WebPage();

    void begin();            // Start AP and web server
    void handleClient();     // Serve incoming HTTP requests
    int returnState();  // Return interval based on state

  private:
    const char* _ssid;
    const char* _password;
    WiFiServer _server;
    String _state;           // Current "position" state
    
    String buildWebPage();             // Generate HTML page
    void handleRequest(String request); // Parse GET requests
};

#endif
