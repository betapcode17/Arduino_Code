// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Stepper.h>

// // Replace with your network credentials
// // Thông tin WiFi
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // Define GPIO pins for Stepper Motor
// #define lineSensor D5
// #define IN1 D1
// #define IN2 D2
// #define IN3 D3
// #define IN4 D4

// const int stepperRevolution = 2048;
// Stepper myStepper(stepperRevolution, IN1, IN3, IN2, IN4);

// // Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// // Variables to save values from HTML form
// const char *PARAM_INPUT = "steps";

// String direction = "Stopped";  // Motor direction
// int steps = 0;                 // Number of steps

// // Read line sensor
// String readLineSensor() {
//   return (digitalRead(lineSensor) == HIGH) ? "black" : "white";
// }

// // HTML content
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Stepper Motor & Sensor</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h1>Stepper Motor Control</h1>
//   <h2>Sensor Reading</h2>
//   <p>Line Sensor Status: <span id="line">%LINE%</span></p>
//   <h2>Motor Status</h2>
//   <p>Motor Direction: <span id="direction">%DIRECTION%</span></p>
//   <form id="motorForm">
//     <label for="steps">Number of steps (0-360): </label>
//     <input type="number" name="steps" min="0" max="360" required>
//     <input type="submit" value="GO!">
//   </form>
//   <script>
//     document.getElementById('motorForm').addEventListener('submit', function (e) {
//       e.preventDefault();
//       const formData = new FormData(this);
//       fetch("/", {
//         method: "POST",
//         body: formData
//       }).then(response => response.text()).catch(error => console.error("Error:", error));
//     });

//     setInterval(function() {
//       fetch("/status")
//         .then(response => response.json())
//         .then(data => {
//           document.getElementById("line").innerHTML = data.line;
//           document.getElementById("direction").innerHTML = data.direction;
//         })
//         .catch(error => console.error("Error:", error));
//     }, 2000);
//   </script>
// </body>
// </html>
// )rawliteral";

// // Replace placeholder in HTML
// String processor(const String &var) {
//   if (var == "LINE") {
//     return readLineSensor();
//   } else if (var == "DIRECTION") {
//     return direction;
//   }
//   return String();
// }

// void initWiFi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print('.');
//     delay(1000);
//   }
//   Serial.println("\nConnected! IP Address: ");
//   Serial.println(WiFi.localIP());
// }

// void setup() {
//   // Initialize Serial Monitor
//   Serial.begin(9600);

//   // Initialize WiFi
//   initWiFi();

//   // Set up line sensor
//   pinMode(lineSensor, INPUT);

//   // Configure stepper motor
//   myStepper.setSpeed(10);

//   // Web server routes
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String jsonResponse = "{\"line\":\"" + readLineSensor() + "\",\"direction\":\"" + direction + "\"}";
//     request->send(200, "application/json", jsonResponse);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam(PARAM_INPUT, true)) {
//       steps = request->getParam(PARAM_INPUT, true)->value().toInt();
//       Serial.print("Number of steps set to: ");
//       Serial.println(steps);
//     }
//     request->send(200, "text/plain", "Settings updated");
//   });

//   // Start server
//   server.begin();
// }

// void loop() {
//   static int currentStep = 0;        // Số bước hiện tại
//   static String prevDirection = ""; // Hướng quay trước đó
//   static String prevSensorStatus = ""; // Trạng thái cảm biến trước đó

//   // Đọc trạng thái cảm biến
//   String sensorStatus = readLineSensor();

//   // Xác định hướng quay động cơ dựa trên cảm biến
//   if (sensorStatus == "white") {
//     direction = "Clockwise";
//   } else if (sensorStatus == "black") {
//     direction = "Counterclockwise";
//   }

//   // In thông tin nếu trạng thái cảm biến hoặc hướng quay thay đổi
//   if (sensorStatus != prevSensorStatus || direction != prevDirection) {
//     Serial.print("Sensor Status: ");
//     Serial.println(sensorStatus);
//     Serial.print("Motor Direction: ");
//     Serial.println(direction);
//     Serial.println();
//     if(direction != prevDirection) currentStep = 0;
//     prevSensorStatus = sensorStatus;
//     prevDirection = direction;
//   }

//   // Xử lý chuyển động của động cơ
//   if (steps > 0) {
//     // Chuyển đổi từ góc quay sang số bước
//     int totalSteps = map(steps, 0, 360, 0, stepperRevolution);

//     if (currentStep < totalSteps) {
//       myStepper.step(direction == "Clockwise" ? 1 : -1);
//       currentStep++;
//     } else {
//       // Đặt lại số bước và chờ trước khi lặp lại
//       currentStep = 0;  // Đặt lại để tiếp tục vòng quay
//       delay(500);
//     }
//   }
// }




















#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

// ================= WIFI =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define lineSensor D5
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

// ================= STEPPER =================
const int STEPS_PER_REV = 2048;
Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

// ================= WEB =================
AsyncWebServer server(80);
const char *PARAM_INPUT = "steps";

// ================= BIẾN =================
volatile int stepsToMove = 0;
String direction = "Stopped";

// ================= LINE SENSOR =================
String readLineSensor() {
  return digitalRead(lineSensor) == HIGH ? "black" : "white";
}

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Stepper & Line Sensor</title>
</head>
<body>
  <h2>Line Sensor</h2>
  <p>Status: <span id="line">---</span></p>

  <h2>Stepper Motor</h2>
  <p>Direction: <span id="direction">Stopped</span></p>

  <form id="form">
    <input type="number" name="steps" placeholder="Steps (+/-)" required>
    <button type="submit">GO</button>
  </form>

  <script>
    document.getElementById("form").onsubmit = e => {
      e.preventDefault();
      fetch("/", {
        method: "POST",
        body: new FormData(e.target)
      });
    };

    setInterval(() => {
      fetch("/status")
        .then(r => r.json())
        .then(d => {
          document.getElementById("line").innerHTML = d.line;
          document.getElementById("direction").innerHTML = d.direction;
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// ================= WIFI INIT =================
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  initWiFi();

  pinMode(lineSensor, INPUT);
  myStepper.setSpeed(10);

  // Trang chính
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Nhận lệnh quay
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT, true)) {
      stepsToMove = request->getParam(PARAM_INPUT, true)->value().toInt();
      direction = (stepsToMove > 0) ? "Clockwise" : "Counterclockwise";

      Serial.print("Stepper steps: ");
      Serial.println(stepsToMove);
    }
    request->send(200, "text/plain", "OK");
  });

  // Trạng thái
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"line\":\"" + readLineSensor() + "\",";
    json += "\"direction\":\"" + direction + "\"}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // 👉 LINE SENSOR ĐỘC LẬP
  static String prevLine = "";
  String line = readLineSensor();

  if (line != prevLine) {
    Serial.print("Line sensor: ");
    Serial.println(line);
    prevLine = line;
  }

  // 👉 STEPPER ĐỘC LẬP
  if (stepsToMove != 0) {
    myStepper.step(stepsToMove);
    stepsToMove = 0;
    direction = "Stopped";
  }

  delay(200);
}





