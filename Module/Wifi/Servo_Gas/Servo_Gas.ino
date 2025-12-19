#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// ================= WiFi =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define gasSensor A0
#define servoPin  D1

Servo myServo;

// ================= BIẾN =================
int gasValue = 0;
int angleConditionMet = 0;
int angleConditionNotMet = 0;

// ================= WEB SERVER =================
AsyncWebServer server(80);

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Servo Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266 Gas + Servo</h2>
  <p>Gas Sensor Value: <b><span id="gas">%GAS%</span></b></p>

  <form id="servoForm">
    <label>Angle (Gas Detected):</label><br>
    <input type="number" name="angleMet" min="0" max="180" required><br><br>

    <label>Angle (No Gas):</label><br>
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
        .then(d => document.getElementById("gas").innerHTML = d.gas);
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

// ================= HÀM =================
int readGasSensor() {
  return analogRead(gasSensor);
}

// 🔥 QUAN TRỌNG: CHUYỂN GÓC → XUNG
int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, 500, 2500);
}

String processor(const String &var) {
  if (var == "GAS") return String(gasValue);
  return String();
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: ");
  Serial.println(WiFi.localIP());
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  initWiFi();
Serial.println("================================");
Serial.println("ESP8266 Web Server Started");
Serial.print("Web Address: http://");
Serial.println(WiFi.localIP());
Serial.println("================================");
 


  pinMode(gasSensor, INPUT);

  // ⚠️ ATTACH SERVO CHUẨN ESP8266
  myServo.attach(servoPin, 500, 2500);
  myServo.writeMicroseconds(angleToPulse(0));

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",
      "{\"gas\":\"" + String(gasValue) + "\"}");
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angleMet", true))
      angleConditionMet = request->getParam("angleMet", true)->value().toInt();

    if (request->hasParam("angleNotMet", true))
      angleConditionNotMet = request->getParam("angleNotMet", true)->value().toInt();

    Serial.print("Gas: ");
    Serial.print(angleConditionMet);
    Serial.print(" | No Gas: ");
    Serial.println(angleConditionNotMet);

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  gasValue = readGasSensor();
  Serial.println(gasValue);

  // Quay tới góc theo trạng thái gas
  if (gasValue > 120) {
    myServo.writeMicroseconds(angleToPulse(angleConditionMet));
  } else {
    myServo.writeMicroseconds(angleToPulse(angleConditionNotMet));
  }

  delay(500);   // Giữ góc 300ms
  // Quay về 0 độ
  myServo.writeMicroseconds(angleToPulse(0));
  delay(500);   // Giữ 0 độ 300ms
}

