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
        .then(d => document.getElementById("light").innerHTML = d.light);
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

// ================= HÀM =================

// Đọc cảm biến ánh sáng
int readLightSensor() {
  return analogRead(lightSensor);
}

// 🔥 CHUYỂN GÓC → XUNG SERVO (CHUẨN ESP8266)
int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, 1000, 2000);
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

  // ⚠️ Attach servo chuẩn cho ESP8266
  myServo.attach(servoPin, 500, 2500);
  myServo.writeMicroseconds(angleToPulse(0));

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",
      "{\"light\":\"" + String(lightValue) + "\"}");
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
  lightValue = readLightSensor();
  Serial.print("Light Sensor Value: ");
  Serial.println(lightValue);

  // 🌞 ÁNH SÁNG MẠNH → GIÁ TRỊ NHỎ
  if (lightValue < 800) {
    myServo.writeMicroseconds(angleToPulse(angleConditionMet));
  }
  // 🌑 ÁNH SÁNG YẾU → GIÁ TRỊ LỚN
  else {
    myServo.writeMicroseconds(angleToPulse(angleConditionNotMet));
  }

  delay(500);  // Giữ góc

  // Quay về 0 độ
  myServo.writeMicroseconds(angleToPulse(0));
  delay(500);
}
