// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Stepper.h>

// // Thông tin WiFi
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // Định nghĩa chân cho cảm biến siêu âm và động cơ bước
// const int trigPin = D5;  // Chân trigger
// const int echoPin = D6;  // Chân echo
// #define IN1 D1
// #define IN2 D2
// #define IN3 D3
// #define IN4 D4

// const int stepperRevolution = 2048; // Số bước trong một vòng quay
// Stepper myStepper(stepperRevolution, IN1, IN3, IN2, IN4);

// // Tạo web server
// AsyncWebServer server(80);

// // Các biến để lưu giá trị
// int steps = 0;  // Số bước động cơ quay
// String direction = "Stopped";  // Chiều quay ban đầu là dừng

// // Đọc giá trị từ cảm biến siêu âm
// long readDistance() {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   long duration = pulseIn(echoPin, HIGH);  // Đọc thời gian
//   long distance = duration * 0.034 / 2;   // Tính khoảng cách
//   return distance;
// }

// // HTML giao diện web
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Stepper Motor Control</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h1>Stepper Motor Control</h1>
//   <p>Distance from sensor: <span id="distance">%DISTANCE%</span> cm</p>
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
//           document.getElementById("distance").innerHTML = data.distance;
//           document.getElementById("direction").innerHTML = data.direction;
//         })
//         .catch(error => console.error("Error:", error));
//     }, 1000);
//   </script>
// </body>
// </html>
// )rawliteral";

// // Hàm xử lý HTML
// String processor(const String &var) {
//   if (var == "DISTANCE") {
//     return String(readDistance());  // Hiển thị giá trị khoảng cách
//   } else if (var == "DIRECTION") {
//     return direction;  // Hiển thị chiều quay của động cơ
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
//   // Khởi tạo Serial Monitor
//   Serial.begin(9600);

//   // Khởi tạo kết nối WiFi
//   initWiFi();

//   // Khởi tạo chân cho cảm biến siêu âm
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);

//   // Cấu hình động cơ bước
//   myStepper.setSpeed(10);

//   // Web server routes
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String jsonResponse = "{\"distance\":\"" + String(readDistance()) + "\",\"direction\":\"" + direction + "\"}";
//     request->send(200, "application/json", jsonResponse);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("steps", true)) {
//       steps = request->getParam("steps", true)->value().toInt();
//       Serial.print("Number of steps set to: ");
//       Serial.println(steps);
//     }
//     request->send(200, "text/plain", "Settings updated");
//   });

//   server.begin();
// }

// void loop() {
//   long distance = readDistance();  // Đọc khoảng cách từ cảm biến siêu âm

//   // Điều chỉnh chiều quay của động cơ dựa trên khoảng cách
//   if (distance > 20) {
//     direction = "Clockwise";  // Quay thuận nếu khoảng cách trên 20cm
//   } else {
//     direction = "Counterclockwise";  // Quay ngược nếu khoảng cách dưới 20cm
//   }

//   // Quay động cơ nếu có số bước
//   if (steps > 0) {
//     int totalSteps = map(steps, 0, 360, 0, stepperRevolution);
//     for (int i = 0; i < totalSteps; i++) {
//         myStepper.step(direction == "Clockwise" ? 1 : -1);
//         yield();  // Nhường CPU cho các tác vụ khác
//         ESP.wdtFeed();
//     }
//   }
  
//   delay(500);  // Thêm một chút độ trễ để tránh quá tải
// }
















#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

// ================= WIFI =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define TRIG_PIN D5
#define ECHO_PIN D6
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

// ================= SIÊU ÂM =================
long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Ultrasonic & Stepper</title>
</head>
<body>
  <h2>Ultrasonic Sensor</h2>
  <p>Distance: <span id="distance">---</span> cm</p>

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
          document.getElementById("distance").innerHTML = d.distance;
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

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myStepper.setSpeed(10);

  // Trang chính
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Nhận lệnh động cơ
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
    json += "\"distance\":" + String(readDistance()) + ",";
    json += "\"direction\":\"" + direction + "\"}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // 👉 SIÊU ÂM ĐỘC LẬP
  static long lastDistance = -1;
  long distance = readDistance();

  if (distance != lastDistance && distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    lastDistance = distance;
  }

  // 👉 STEPPER ĐỘC LẬP
  if (stepsToMove != 0) {
    myStepper.step(stepsToMove);
    stepsToMove = 0;
    direction = "Stopped";
  }

  delay(200);
}




