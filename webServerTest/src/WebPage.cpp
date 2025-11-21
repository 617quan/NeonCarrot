#include "WebPage.h"

WebPage::WebPage(const char* ssid, const char* password, int ledPin)
  : _ssid(ssid), _password(password), _ledPin(ledPin), _server(80), _state("Reset"),
    _lastFlashTime(0), _ledOn(false) {}

WebPage::~WebPage() {}

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

  String header = "";
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          handleRequest(header);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println(buildWebPage());
          client.println();
          break;
        } else currentLine = "";
      } else if (c != '\r') currentLine += c;
    }
  }
  client.stop();
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
  return 1000;
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
  String page = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>ESP32 Position Control</title>
<style>
body {
  font-family: 'Poppins', sans-serif;
  background: radial-gradient(circle at top, #0d021a, #1a002b, #000);
  display: flex; flex-direction: column; align-items: center; justify-content: center;
  height: 100vh; margin: 0; color: #fff; overflow: hidden;
}
h1 {
  font-size: 2.2em; color: #c58eff;
  text-shadow: 0 0 15px #a45bff; margin-bottom: 10px;
}
#state {
  color: #fff; font-weight: 600; font-size: 1.2em;
  background: rgba(255,255,255,0.05);
  padding: 10px 20px; border-radius: 10px;
  backdrop-filter: blur(10px); margin-bottom: 25px;
  box-shadow: 0 0 10px rgba(165,69,255,0.25);
}
.room {
  position: relative; width: 300px; height: 300px;
  background: rgba(255,255,255,0.03);
  border: 2px solid rgba(165,69,255,0.4);
  border-radius: 20px; box-shadow: 0 0 25px rgba(165,69,255,0.3);
  margin-bottom: 40px; backdrop-filter: blur(10px);
}
.corner {
  position: absolute; width: 15px; height: 15px; border-radius: 50%;
  background: rgba(165,69,255,0.4); box-shadow: 0 0 12px rgba(165,69,255,0.8);
}
.corner.pos1 { bottom: 10px; left: 10px; }
.corner.pos2 { top: 10px; left: 10px; }
.corner.pos3 { top: 10px; right: 10px; }
.corner.pos4 { bottom: 10px; right: 10px; }
.robot {
  position: absolute; width: 25px; height: 25px; border-radius: 50%;
  background: linear-gradient(135deg, #b066ff, #8a2be2);
  box-shadow: 0 0 25px rgba(175,80,255,0.8);
  transition: all 1.6s cubic-bezier(0.4,0,0.2,1);
}
.button-container {
  display: grid; grid-template-columns: repeat(auto-fit, minmax(150px,1fr));
  gap: 15px; width: 90%; max-width: 600px;
}
.button {
  border: 2px solid rgba(165,69,255,0.5);
  background: transparent; padding: 14px 26px; border-radius: 12px;
  font-size: 1.05em; color: #fff; cursor: pointer;
  transition: all 0.25s ease-in-out; text-transform: uppercase; letter-spacing: 1px;
  position: relative; box-shadow: 0 0 8px rgba(165,69,255,0.15);
}
.button:hover {
  box-shadow: 0 0 18px rgba(175,80,255,0.5); transform: translateY(-2px);
}
.button.active {
  border-color: #c58eff; box-shadow: 0 0 20px rgba(197,142,255,0.8);
}
</style>
</head>
<body>
<h1>ESP32 Position Control</h1>
<p>Current State: <span id="state">)rawliteral" + _state + R"rawliteral(</span></p>

<div class="room">
  <div class="corner pos1"></div>
  <div class="corner pos2"></div>
  <div class="corner pos3"></div>
  <div class="corner pos4"></div>
  <div class="robot" id="robot"></div>
</div>

<div class="button-container">
  <button class="button" id="Position1">Position 1</button>
  <button class="button" id="Position2">Position 2</button>
  <button class="button" id="Position3">Position 3</button>
  <button class="button" id="Position4">Position 4</button>
  <button class="button" id="Reset">Reset</button>
  <button class="button" id="Redo">Redo</button>
</div>

<script>
const buttons=document.querySelectorAll('.button');
const stateText=document.getElementById('state');
const robot=document.getElementById('robot');
let currentPos='Position1';
function setRobotPosition(pos){
  const room=document.querySelector('.room');
  const roomSize=room.getBoundingClientRect();
  const robotSize=25; let x=0,y=0;
  switch(pos){
    case'Position1':x=10;y=roomSize.height-robotSize-10;break;
    case'Position2':x=10;y=10;break;
    case'Position3':x=roomSize.width-robotSize-10;y=10;break;
    case'Position4':x=roomSize.width-robotSize-10;y=roomSize.height-robotSize-10;break;
  }
  robot.style.left=`${x}px`; robot.style.top=`${y}px`;
}
window.addEventListener('load',()=>setRobotPosition('Position1'));
buttons.forEach(button=>{
  button.addEventListener('click',()=>{
    buttons.forEach(btn=>btn.classList.remove('active'));
    let pos=button.id;
    if(pos==='Redo'){
      stateText.textContent=currentPos+" (Redo)";
      setRobotPosition(currentPos);
      fetch(`/Redo`);
      return;
    }
    if(pos==='Reset')pos='Position1';
    currentPos=pos;
    button.classList.add('active');
    stateText.textContent=pos;
    setRobotPosition(pos);
    fetch(`/${pos}`);
  });
});
</script>
</body></html>
)rawliteral";

  return page;
}
