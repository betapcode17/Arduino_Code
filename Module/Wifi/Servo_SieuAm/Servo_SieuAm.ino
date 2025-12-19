#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// ================= WiFi =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define TRIG_PIN D5
#define ECHO_PIN D6
#define SERVO_PIN D1

Servo myServo;

// ================= BIẾN =================
long distanceCM = 0;
int angleNear = 90;     // Góc servo khi vật ở GẦN
int angleFar  = 0;      // Góc servo khi vật ở XA
int threshold = 20;     // Ngưỡng khoảng cách (cm)

// ================= WEB SERVER =================
AsyncWebServer server(80);

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Ultrasonic Servo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266 Ultrasonic + Servo</h2>

  <p>Distance: <b><span id="dist">%DIST%</span> cm</b></p>

  <form id="servoForm">
    <label>Angle (Object Near):</label><br>
    <input type="number" name="angleNear" min="0" max="180" required><br><br>

    <label>Angle (Object Far):</label><br>
    <input type="number" name="angleFar" min="0" max="180" required><br><br>

    <label>Threshold Distance (cm):</label><br>
    <input type="number" name="threshold" min="1" max="200" required><br><br>

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
        .then(d => document.getElementById("dist").innerHTML = d.distance);
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// ================= HÀM =================

// Đọc khoảng cách (cm)
long readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout ~5m
  if (duration == 0) return 999;                  // không đo được

  return duration * 0.034 / 2;
}

// 🔥 CHUYỂN GÓC → XUNG SERVO
int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, 500, 2500);
}

// Xử lý HTML
String processor(const String &var) {
  if (var == "DIST") return String(distanceCM);
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

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Attach servo chuẩn ESP8266
  myServo.attach(SERVO_PIN, 500, 2500);
  myServo.writeMicroseconds(angleToPulse(0));

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",
      "{\"distance\":\"" + String(distanceCM) + "\"}");
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angleNear", true))
      angleNear = request->getParam("angleNear", true)->value().toInt();

    if (request->hasParam("angleFar", true))
      angleFar = request->getParam("angleFar", true)->value().toInt();

    if (request->hasParam("threshold", true))
      threshold = request->getParam("threshold", true)->value().toInt();

    Serial.print("Near:");
    Serial.print(angleNear);
    Serial.print(" | Far:");
    Serial.print(angleFar);
    Serial.print(" | Thresh:");
    Serial.println(threshold);

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  distanceCM = readUltrasonic();
  Serial.print("Distance: ");
  Serial.print(distanceCM);
  Serial.println(" cm");

  // 🚗 Vật ở GẦN
  if (distanceCM <= threshold) {
    myServo.writeMicroseconds(angleToPulse(angleNear));
  }
  // 🛣️ Vật ở XA
  else {
    myServo.writeMicroseconds(angleToPulse(angleFar));
  }

  delay(500);  // Giữ góc

  // Quay về 0 độ
  myServo.writeMicroseconds(angleToPulse(0));
  delay(500);
}
