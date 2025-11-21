// #include <WiFi.h>
// #include "WebPage.h"

// // Replace with your network credentials for the ESP32 Access Point
// const char* ssid     = "ESP32-Access-Point";
// const char* password = "123456789";

// // Create a Wi-Fi server on port 80 (standard HTTP port)
// WiFiServer server(80);

// // String to store the incoming HTTP request
// String header;

// // Variables to keep track of the GPIO states
// String output26State = "off";
// String output27State = "off";

// // Assign pins for GPIO outputs (built-in LED and another pin)
// const int output26 = LED_BUILTIN;
// const int output27 = 2;

// void setup() {
//   Serial.begin(115200);       // Start serial communication at 115200 baud
//   // Initialize GPIO pins as outputs
//   pinMode(output26, OUTPUT);
//   pinMode(output27, OUTPUT);

//   // Set outputs LOW initially (LEDs off)
//   digitalWrite(output26, LOW);
//   digitalWrite(output27, LOW);

//   // Start the ESP32 in Access Point mode
//   Serial.print("Setting AP (Access Point)…");
//   WiFi.softAP(ssid, password);   // Start AP with SSID and password

//   // Get and print the ESP32 Access Point IP address
//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("AP IP address: ");
//   Serial.println(IP);
  
//   server.begin();  // Start the web server
// }

// void loop(){
//   // Check if a client has connected to the server
//   WiFiClient client = server.available();

//   if (client) {  // If a new client is connected
//     Serial.println("New Client.");  // Print to Serial monitor
//     String currentLine = "";        // Store incoming request line-by-line

//     // Loop while the client is still connected
//     while (client.connected()) {
//       if (client.available()) {     // If data is available from client
//         char c = client.read();     // Read a single character
//         Serial.write(c);            // Echo character to Serial monitor
//         header += c;                // Append character to full HTTP header

//         // Check if we've reached the end of a line
//         if (c == '\n') {            
//           // Blank line indicates the end of HTTP request headers
//           if (currentLine.length() == 0) {

//             // Send HTTP response headers
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println("Connection: close");
//             client.println();
            
//             // Parse HTTP GET request and control GPIO accordingly
//             if (header.indexOf("GET /26/on") >= 0) {
//               Serial.println("GPIO 26 on");
//               output26State = "on";
//               digitalWrite(output26, HIGH);
//             } else if (header.indexOf("GET /26/off") >= 0) {
//               Serial.println("GPIO 26 off");
//               output26State = "off";
//               digitalWrite(output26, LOW);
//             } else if (header.indexOf("GET /27/on") >= 0) {
//               Serial.println("GPIO 27 on");
//               output27State = "on";
//               digitalWrite(output27, HIGH);
//             } else if (header.indexOf("GET /27/off") >= 0) {
//               Serial.println("GPIO 27 off");
//               output27State = "off";
//               digitalWrite(output27, LOW);
//             }
            
//             // Send the HTML web page to the client
//             client.println("<!DOCTYPE html><html>");
//             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//             client.println("<link rel=\"icon\" href=\"data:,\">");

//             // CSS styling for buttons
//             client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
//             client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
//             client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
//             client.println(".button2 {background-color: #555555;}</style></head>");
            
//             // Body content
//             client.println("<body><h1>ESP32 Web Server</h1>");
            
//             // Display GPIO 26 status and buttons
//             client.println("<p>GPIO 26 - State " + output26State + "</p>");
//             if (output26State=="off") {
//               client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
//             } 
               
//             // Display GPIO 27 status and buttons
//             client.println("<p>GPIO 27 - State " + output27State + "</p>");
//             if (output27State=="off") {
//               client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
//             }
//             client.println("</body></html>");
            
//             client.println(); // End of HTTP response

//             break; // Exit while(client.connected())
//           } else { 
//             currentLine = ""; // Reset currentLine after each line
//           }
//         } else if (c != '\r') {  
//           // Append character to currentLine if it's not a carriage return
//           currentLine += c;
//         }
//       }
//     }

//     // Reset header variable for next client
//     header = "";
//     client.stop(); // Close connection
//     Serial.println("Client disconnected.");
//     Serial.println("");
//   }
// }
