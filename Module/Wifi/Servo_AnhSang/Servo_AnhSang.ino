// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Servo.h>

// // ================= WiFi =================
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // ================= PHẦN CỨNG =================
// #define lightSensor A0
// #define servoPin    D1

// Servo myServo;

// // ================= BIẾN =================
// int lightValue = 0;
// int angleConditionMet = 90;      // Góc khi ánh sáng MẠNH
// int angleConditionNotMet = 0;    // Góc khi ánh sáng YẾU

// // DAO ĐỘNG SERVO (GIỐNG CODE LINE SENSOR)
// unsigned long prevMillis = 0;
// const unsigned long interval = 400;  // Thời gian dao động (ms)
// bool servoState = false;
// int targetAngle = 0;

// // ================= WEB SERVER =================
// AsyncWebServer server(80);

// // ================= HTML =================
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Light Sensor Servo</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h2>ESP8266 Light Sensor + Servo</h2>

//   <p>Light Sensor Value: <b><span id="light">%LIGHT%</span></b></p>
//   <p>Light Condition: <b><span id="condition">---</span></b></p>

//   <form id="servoForm">
//     <label>Angle (Light Strong):</label><br>
//     <input type="number" name="angleMet" min="0" max="180" required><br><br>

//     <label>Angle (Light Weak):</label><br>
//     <input type="number" name="angleNotMet" min="0" max="180" required><br><br>

//     <input type="submit" value="Update">
//   </form>

//   <script>
//     document.getElementById("servoForm").addEventListener("submit", function(e){
//       e.preventDefault();
//       fetch("/", { method:"POST", body:new FormData(this) });
//     });

//     setInterval(() => {
//       fetch("/status")
//         .then(r => r.json())
//         .then(d => {
//           document.getElementById("light").innerHTML = d.light;
//           document.getElementById("condition").innerHTML = d.condition;
//         });
//     }, 1000);
//   </script>
// </body>
// </html>
// )rawliteral";

// // ================= HÀM =================

// // Đọc cảm biến ánh sáng
// int readLightSensor() {
//   return analogRead(lightSensor);
// }

// // Xử lý HTML
// String processor(const String &var) {
//   if (var == "LIGHT") return String(lightValue);
//   return String();
// }

// // Kết nối WiFi
// void initWiFi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting WiFi");

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\n==============================");
//   Serial.println("ESP8266 Web Server Started");
//   Serial.print("Web Address: http://");
//   Serial.println(WiFi.localIP());
//   Serial.println("==============================");
// }

// // ================= SETUP =================
// void setup() {
//   Serial.begin(9600);

//   initWiFi();

//   pinMode(lightSensor, INPUT);

//   // Attach servo
//   myServo.attach(servoPin, 500, 2500);
//   myServo.write(0);  // Bắt đầu từ 0 độ

//   // ===== WEB =====
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String conditionStr = (lightValue < 800) ? "STRONG Light" : "WEAK Light";
//     request->send(200, "application/json",
//       "{\"light\":\"" + String(lightValue) + 
//       "\", \"condition\":\"" + conditionStr + "\"}");
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("angleMet", true))
//       angleConditionMet = request->getParam("angleMet", true)->value().toInt();

//     if (request->hasParam("angleNotMet", true))
//       angleConditionNotMet = request->getParam("angleNotMet", true)->value().toInt();

//     Serial.print("Light Strong Angle: ");
//     Serial.print(angleConditionMet);
//     Serial.print(" | Light Weak Angle: ");
//     Serial.println(angleConditionNotMet);

//     request->send(200, "text/plain", "OK");
//   });

//   server.begin();
// }

// // ================= LOOP =================
// void loop() {
//   unsigned long now = millis();
  
//   // Đọc cảm biến ánh sáng
//   lightValue = readLightSensor();
  
//   // Xác định góc mục tiêu dựa trên điều kiện ánh sáng
//   // 🌞 ÁNH SÁNG MẠNH → GIÁ TRỊ NHỎ (< 800)
//   if (lightValue < 800) {
//     targetAngle = angleConditionMet;
//   }
//   // 🌑 ÁNH SÁNG YẾU → GIÁ TRỊ LỚN (>= 800)
//   else {
//     targetAngle = angleConditionNotMet;
//   }
  
//   // ===== DAO ĐỘNG SERVO 0 ↔ GÓC MỤC TIÊU =====
//   // Giống với code cảm biến dò line
//   if (now - prevMillis >= interval) {
//     prevMillis = now;
//     servoState = !servoState;
    
//     // Dao động giữa 0 độ và góc mục tiêu
//     if (servoState) {
//       myServo.write(targetAngle);
//       Serial.print("Servo to target angle: ");
//       Serial.print(targetAngle);
//     } else {
//       myServo.write(0);
//       Serial.print("Servo to 0 degree");
//     }
    
//     // In thông tin debug
//     Serial.print(" | Light: ");
//     Serial.print(lightValue);
//     Serial.print(" | Condition: ");
//     Serial.println((lightValue < 800) ? "STRONG" : "WEAK");
//   }
// }





#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// ================= WiFi =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define lightSensor A0
#define servoPin    D1

Servo myServo;

// ================= BIẾN =================
int lightValue = 0;
int angleConditionMet = 90;      // Góc khi ánh sáng MẠNH
int angleConditionNotMet = 0;    // Góc khi ánh sáng YẾU

// ===== DAO ĐỘNG SERVO =====
unsigned long prevMillis = 0;
const unsigned long interval = 400;  // Thời gian dao động (ms)
bool servoState = false;
int targetAngle = 0;

// ================= WEB SERVER =================
AsyncWebServer server(80);

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Light Sensor Servo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266 Light Sensor + Servo</h2>

  <p>Light Sensor Value: <b><span id="light">%LIGHT%</span></b></p>
  <p>Light Condition: <b><span id="condition">---</span></b></p>

  <form id="servoForm">
    <label>Angle (Light Strong):</label><br>
    <input type="number" name="angleMet" min="0" max="180" required><br><br>

    <label>Angle (Light Weak):</label><br>
    <input type="number" name="angleNotMet" min="0" max="180" required><br><br>

    <input type="submit" value="Update">
  </form>

  <script>
    document.getElementById("servoForm").addEventListener("submit", function(e){
      e.preventDefault();
      fetch("/", { method:"POST", body:new FormData(this) });
    });

    setInterval(() => {
      fetch("/status")
        .then(r => r.json())
        .then(d => {
          document.getElementById("light").innerHTML = d.light;
          document.getElementById("condition").innerHTML = d.condition;
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// ================= HÀM =================
// Đọc cảm biến ánh sáng
int readLightSensor() {
  return analogRead(lightSensor);
}

// Cập nhật góc mục tiêu dựa trên giá trị cảm biến
int computeTargetAngle(int lightVal) {
  return (lightVal < 800) ? angleConditionMet : angleConditionNotMet;
}

// Dao động servo độc lập
void updateServo() {
  unsigned long now = millis();
  if (now - prevMillis >= interval) {
    prevMillis = now;
    servoState = !servoState;
    if (servoState) myServo.write(targetAngle);
    else myServo.write(0);
  }
}

// Xử lý HTML
String processor(const String &var) {
  if (var == "LIGHT") return String(lightValue);
  return String();
}

// Kết nối WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n==============================");
  Serial.println("ESP8266 Web Server Started");
  Serial.print("Web Address: http://");
  Serial.println(WiFi.localIP());
  Serial.println("==============================");
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  initWiFi();

  pinMode(lightSensor, INPUT);
  myServo.attach(servoPin, 500, 2500);
  myServo.write(0);

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String conditionStr = (lightValue < 800) ? "STRONG Light" : "WEAK Light";
    request->send(200, "application/json",
      "{\"light\":\"" + String(lightValue) + 
      "\", \"condition\":\"" + conditionStr + "\"}");
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angleMet", true))
      angleConditionMet = request->getParam("angleMet", true)->value().toInt();

    if (request->hasParam("angleNotMet", true))
      angleConditionNotMet = request->getParam("angleNotMet", true)->value().toInt();

    Serial.print("Light Strong Angle: ");
    Serial.print(angleConditionMet);
    Serial.print(" | Light Weak Angle: ");
    Serial.println(angleConditionNotMet);

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // 1️⃣ Đọc cảm biến
  lightValue = readLightSensor();

  // 2️⃣ Xác định góc mục tiêu
  targetAngle = computeTargetAngle(lightValue);

  // 3️⃣ Dao động servo
  updateServo();

  // 4️⃣ Debug
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print(" | Target Angle: ");
  Serial.println(targetAngle);
}
