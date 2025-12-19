#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// ================= WIFI =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PIN =================
#define joystickX A0
#define servoPin D1

// ================= OBJECT =================
Servo myServo;
AsyncWebServer server(80);

// ================= BIẾN =================
int joystickValue = 0;
int angleConditionMet = 90;
int angleConditionNotMet = 0;
int joystickThreshold = 550;

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Joystick Servo</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>Joystick Servo Control</h1>
  <p>Joystick X Value: <span id="joystick">%JOYSTICK%</span></p>

  <form id="servoForm">
    <label>Angle (Joystick > Threshold): </label>
    <input type="number" name="angleMet" min="0" max="180" required><br>
    <label>Angle (Joystick <= Threshold): </label>
    <input type="number" name="angleNotMet" min="0" max="180" required><br><br>
    <input type="submit" value="Update">
  </form>

  <script>
    document.getElementById("servoForm").addEventListener("submit", function(e){
      e.preventDefault();
      fetch("/", { method: "POST", body: new FormData(this) });
    });

    setInterval(() => {
      fetch("/status")
        .then(r => r.json())
        .then(d => document.getElementById("joystick").innerHTML = d.joystick);
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, 1000, 2000);
}


// ================= READ JOYSTICK =================
int readJoystickX() {
  return analogRead(joystickX);
}

// ================= HTML PROCESS =================
String processor(const String &var) {
  if (var == "JOYSTICK") return String(joystickValue);
  return String();
}

// ================= WIFI =================
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n==============================");
  Serial.print("Web address: http://");
  Serial.println(WiFi.localIP());
  Serial.println("==============================");
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  initWiFi();

  pinMode(joystickX, INPUT);
  myServo.attach(servoPin);
  myServo.writeMicroseconds(angleToPulse(90)); // về giữa

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",
      "{\"joystick\":\"" + String(joystickValue) + "\"}");
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angleMet", true))
      angleConditionMet = request->getParam("angleMet", true)->value().toInt();

    if (request->hasParam("angleNotMet", true))
      angleConditionNotMet = request->getParam("angleNotMet", true)->value().toInt();

    Serial.print("Updated: ");
    Serial.print(angleConditionMet);
    Serial.print(" | ");
    Serial.println(angleConditionNotMet);

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  joystickValue = readJoystickX();
  Serial.print("Joystick: ");
  Serial.println(joystickValue);

  if (joystickValue > joystickThreshold) {
    myServo.writeMicroseconds(angleToPulse(angleConditionMet));
  } else {
    myServo.writeMicroseconds(angleToPulse(angleConditionNotMet));
  }

  delay(300);

  // Quay về 0 độ
  myServo.writeMicroseconds(angleToPulse(0));
  delay(300);
}
