/********** WebPage.cpp **********
 * Created by Team Neon Carrot. Contact Carrot Griffin Faecher for Info
 * Purpose: Implements the WebPage class which creates and serves a web UI
 * for controlling robot movement and reporting system state over WiFi.
 * Handles HTTP requests, parses commands, and builds the dynamic page.
 ********************************/

#include "WebPage.h"

/********** WebPage **********
 *
 * Description:
 *      Constructor for the WebPage class. Initializes WiFi credentials,
 *      HTTP server instance, and internal command tracking variables.
 *
 * Parameters:
 *      ssid     - Name of the access point created by the ESP32.
 *      password - Password used to connect to the access point.
 *
 * Return:
 *      None. Initializes the WebPage object.
 *
 * Expects:
 *      Valid SSID and password strings provided by the caller.
 *
 * Notes:
 *      Server is initialized on port 80 but does not start until begin().
 *
 ************************/
WebPage::WebPage(const char* ssid, const char* password)
  : _ssid(ssid),
    _password(password),
    _server(80),
    _command(IDLE),
    _newCommand(false),
    _showBusyMessage(false) {}

/********** ~WebPage **********
 *
 * Description:
 *      Destructor for the WebPage class.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      Nothing.
 *
 * Expects:
 *      Nothing.
 *
 * Notes:
 *      Currently no dynamic memory is allocated so nothing is freed.
 *
 ************************/
WebPage::~WebPage() {}

/********** setBusyMessage **********
 *
 * Description:
 *      Updates the busy status displayed on the web interface when the
 *      robot is executing a movement command.
 *
 * Parameters:
 *      busy - Boolean indicating whether the robot is currently moving.
 *
 * Return:
 *      Nothing. Internal busy flag is updated.
 *
 * Expects:
 *      Called by the state machine before serving clients.
 *
 * Notes:
 *      Value is used when generating the status response.
 *
 ************************/
void WebPage::setBusyMessage(bool busy) {
  _showBusyMessage = busy;
}

/********** begin **********
 *
 * Description:
 *      Starts the WiFi access point and launches the HTTP server that
 *      serves the robot control web interface.
 *
 * Parameters:
 *      Nothing.
 *
 * Return:
 *      Nothing. Access point and server are initialized.
 *
 * Expects:
 *      Valid SSID and password were provided in the constructor.
 *
 * Notes:
 *
 ************************/
void WebPage::begin() {
  WiFi.softAP(_ssid, _password);
  _server.begin();
}

/********** handleClient **********
 *
 * Description:
 *      Handles incoming HTTP client connections. Parses requests,
 *      serves the web page, and responds to the status endpoint.
 *
 * Parameters:
 *      curr_state - Current position or movement state of the robot.
 *
 * Return:
 *      Nothing. Sends HTTP responses directly to the client.
 *
 * Expects:
 *      Called repeatedly in the main loop to service requests.
 *
 * Notes:
 *
 ************************/
void WebPage::handleClient(STATE_TYPE curr_state) {

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

          if (header.indexOf("GET /status") >= 0) {
            String stateString;
            switch (curr_state) {
                case P1:      stateString = "Position 1"; break;
                case P2:      stateString = "Position 2"; break;
                case P3:      stateString = "Position 3"; break;
                case P4:      stateString = "Position 4"; break;
                case MOVING:  stateString = "Moving";     break;
                default:      stateString = "Unknown";    break;
            }
            /* Logic to print the correct state on web page */
            String json = "{";
            json += "\"position\": \"" + stateString + "\",";
            json += "\"moving\": ";
            json += (_showBusyMessage ? "true" : "false");
            json += "}";

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.println(json);
            client.println();

            break;
          }

          /* Displays the web page to the client */
          handleRequest(header);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println(buildWebPage(curr_state));
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
}

/********** handleRequest **********
 *
 * Description:
 *      Parses the HTTP request string to determine if a movement
 *      command was issued from the web interface.
 *
 * Parameters:
 *      request - Raw HTTP request header string.
 *
 * Return:
 *      Nothing. Updates the stored command if a new one is detected.
 *
 * Expects:
 *      Valid HTTP request string from handleClient().
 *
 * Notes:
 *      Sets a flag so the main program knows a new command arrived.
 *
 ************************/
void WebPage::handleRequest(String request) {

  MOVE_COMMAND newCommand = IDLE;

  if (request.indexOf("GET /Position1") >= 0)
    newCommand = MOVE_TO_P1;

  else if (request.indexOf("GET /Position2") >= 0)
    newCommand = MOVE_TO_P2;

  else if (request.indexOf("GET /Position3") >= 0)
    newCommand = MOVE_TO_P3;

  else if (request.indexOf("GET /Position4") >= 0)
    newCommand = MOVE_TO_P4;
  
  else if (request.indexOf("GET /Initialize") >= 0)
    newCommand = INITIALIZE;

  /* Only trigger if the command actually changed */
  if (newCommand != _command) {
    _command = newCommand;
    _newCommand = true;
  }
}

/********** hasNewCommand **********
 *
 * Description:
 *      Indicates whether a new movement command has been received
 *      from the web interface.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      bool - True if a new command is waiting to be processed.
 *
 * Expects:
 *      Called by the main control loop to poll for new commands.
 *
 * Notes:
 *      Flag is cleared when getCommand() is called.
 *
 ************************/
bool WebPage::hasNewCommand() {
  return _newCommand;
}

/********** getCommand **********
 *
 * Description:
 *      Returns the most recently received movement command and
 *      clears the new command flag.
 *
 * Parameters:
 *      None.
 *
 * Return:
 *      MOVE_COMMAND representing the requested movement action.
 *
 * Expects:
 *      Called only when hasNewCommand() returns true.
 *
 * Notes:
 *      Allows the state machine to process commands exactly once.
 *
 ************************/
MOVE_COMMAND WebPage::getCommand() {
  _newCommand = false;
  return _command;
}

/********** buildWebPage **********
 *
 * Description:
 *      Dynamically builds the HTML page served to the client which
 *      displays the robot position, busy status, and control buttons.
 *
 * Parameters:
 *      curr_state - Current position or movement state of the robot.
 *
 * Return:
 *      String containing the full HTML page to send to the client.
 *
 * Expects:
 *      Valid state value provided by the system state machine.
 *
 * Notes:
 *      Includes JavaScript for polling status and animating the robot.
 *
 ************************/
String WebPage::buildWebPage(STATE_TYPE curr_state) {

  String stateString;
  switch (curr_state) {
    case P1:      stateString = "Position1"; break;
    case P2:      stateString = "Position2"; break;
    case P3:      stateString = "Position3"; break;
    case P4:      stateString = "Position4"; break;
    case MOVING:  stateString = "Moving"; break;
    default:      stateString = "Unknown"; break;
  }

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
}
.button:hover {
  box-shadow: 0 0 18px rgba(175,80,255,0.5); transform: translateY(-2px);
}
</style>
</head>

<body>

<h1>ESP32 Position Control</h1>
)rawliteral";

  // ---- POSITION DISPLAY ----
    page += "<p style=\"margin:10px 0;\">Position: <span id=\"position\">";
    page += stateString;
    page += "</span></p>";

  // ---- BUSY MESSAGE ----
  page += "<div id=\"busy\" style=\"display:none; color:#ff4d4d; "
          "font-weight:600;\">";
  page += "Bot currently moving. Please wait until movement is finished.";
  page += "</div>";

  page += R"rawliteral(

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
  <button class="button" id="Initialize">Initialize</button>
  <button class="button" id="Future">Bomboclat</button>
</div>

<script>

function updateStatus() {
  fetch('/status')
    .then(response => response.json())
    .then(data => {

      document.getElementById("position").innerText = data.position;

      const busyDiv = document.getElementById("busy");
      if (data.moving) {
        busyDiv.style.display = "block";
      } else {
        busyDiv.style.display = "none";
      }
    });
}

setInterval(updateStatus, 200);

// ---- Existing Robot Logic ----
const buttons=document.querySelectorAll('.button');
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
    case'Position4':x=roomSize.width-robotSize-10;
                     y=roomSize.height-robotSize-10;break;
  }
  robot.style.left=`${x}px`;
  robot.style.top=`${y}px`;
}

window.addEventListener('load',()=>{
  setRobotPosition(')rawliteral";

  page += stateString;

  page += R"rawliteral(');
});

buttons.forEach(button=>{
  button.addEventListener('click',()=>{
    let pos=button.id;
    if(pos==='Bomboclat'){
      // TODO: implement future button behavior
      return;
    }
    if(pos==='Initialize'){
      fetch(`/Initialize`);
      return;
    }
  });
});

</script>
</body></html>
)rawliteral";

  return page;
}
