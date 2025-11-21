#include "WebPage.h"

WebPage web("ESP32-Access-Point", "123456789");

void setup() {
  web.begin();
}

void loop() {
  web.handleClient(); // Serve clients
  web.updateLED();    // Flash LED based on current state
}
