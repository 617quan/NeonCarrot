//WebPage.h
//By Alex Wardwell for Bonnie Schnitta (SoundSense)

#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char* MAIN_page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Robot Control</title>
    <script>
        function sendCommand() {
            fetch("/button");
            updateStatus();
        }

        async function updateStatus() {
            const res = await fetch("/status");
            const data = await res.json();
            document.getElementById("position").innerText = "Position: " + data.position;
            document.getElementById("height").innerText = "Height: " + (data.raised ? "Raised" : "Lowered");
        }

        setInterval(updateStatus, 1000); // refresh status every second
    </script>
</head>
<body>
    <h1>Robot Web Controller</h1>
    <button onclick="sendCommand()">Advance State</button>
    <p id="position">Position: ...</p>
    <p id="height">Height: ...</p>
</body>
</html>
)rawliteral";

#endif
