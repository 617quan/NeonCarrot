#include "WebPage.h"

WebPage::WebPage(const char* ssid, const char* password, int ledPin)
  : _ssid(ssid), _password(password), _ledPin(ledPin), _server(80), _state("Reset"),
    _lastFlashTime(0), _ledOn(false) {
}

WebPage::~WebPage() {
}

void WebPage::begin() {
  Serial.begin(115200);

  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, LOW);

  Serial.println("Setting up Access Point...");
  WiFi.softAP(_ssid, _password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  _server.begin();
}

void WebPage::handleClient() {
  WiFiClient client = _server.available();
  if (!client) return;

  Serial.println("New Client connected");
  String currentLine = "";
  String header = "";

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          handleRequest(header);  // Parse request and update state

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println(buildWebPage());
          client.println();
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  client.stop();
  Serial.println("Client disconnected.\n");
}

void WebPage::handleRequest(String request) {
  if (request.indexOf("GET /Position1") >= 0) _state = "Position1";
  else if (request.indexOf("GET /Position2") >= 0) _state = "Position2";
  else if (request.indexOf("GET /Position3") >= 0) _state = "Position3";
  else if (request.indexOf("GET /Position4") >= 0) _state = "Position4";
  else if (request.indexOf("GET /Reset") >= 0) _state = "Reset";
  else if (request.indexOf("GET /Redo") >= 0) _state = "Redo";
  Serial.println("State changed to: " + _state);
}

unsigned long WebPage::getFlashInterval() {
  if (_state == "Position1") return 200;
  if (_state == "Position2") return 400;
  if (_state == "Position3") return 600;
  if (_state == "Position4") return 800;
  if (_state == "Redo")      return 300;
  return 1000; // Reset
}

void WebPage::updateLED() {
  unsigned long interval = getFlashInterval();
  unsigned long currentMillis = millis();

  if (currentMillis - _lastFlashTime >= interval) {
    _ledOn = !_ledOn;
    digitalWrite(_ledPin, _ledOn ? HIGH : LOW);
    _lastFlashTime = currentMillis;
  }
}

String WebPage::buildWebPage() {
  String page = "<!DOCTYPE html><html>";
  page += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  page += "<link rel=\"icon\" href=\"data:,\">";
  page += "<style>";
  page += "html { font-family: Helvetica; display: inline-block; margin: 0 auto; text-align: center;}";
  page += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
  page += "text-decoration: none; font-size: 24px; margin: 2px; cursor: pointer; border-radius: 8px;}";
  page += ".active { background-color: #555555; }";
  page += "</style></head>";
  page += "<body><h1>ESP32 Position Control</h1>";
  page += "<p>Current State: " + _state + "</p>";

  String buttons[6] = {"Position1", "Position2", "Position3", "Position4", "Reset", "Redo"};
  for (int i = 0; i < 6; i++) {
    String cls = (buttons[i] == _state) ? "button active" : "button";
    page += "<p><a href=\"/" + buttons[i] + "\"><button class=\"" + cls + "\">" + buttons[i] + "</button></a></p>";
  }

  page += "</body></html>";
  return page;
}
