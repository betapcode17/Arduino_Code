#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

/* ================= WIFI ================= */
const char *ssid = "Tran Thi Kim Loan";
const char *password = "01229333995";

/* ================= PHẦN CỨNG ================= */
#define gasSensor A0
#define motorPin1 D1
#define motorPin2 D2
#define motorSpeedPin D3

/* ================= BIẾN ================= */
AsyncWebServer server(80);
int motorSpeed = 0;        // 0-255
int motorDirection = 0;    // 0 = Stop, 1 = Clockwise, 2 = Counter Clockwise
String motorState = "Stopped";

/* ================= HTML ================= */
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DC Motor & Gas Sensor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP8266 Motor Control</h2>
  <h3>Gas Sensor:</h3>
  <p>Value: <span id="gas">%GAS%</span></p>

  <h3>Motor Status:</h3>
  <p>Direction: <span id="direction">%DIRECTION%</span></p>

  <form id="motorForm">
    <label>Speed (0-255):</label><br>
    <input type="number" name="speed" min="0" max="255"><br><br>

    <label>Direction:</label><br>
    <select name="direction">
      <option value="0">Stop</option>
      <option value="1">Clockwise</option>
      <option value="2">Counter Clockwise</option>
    </select><br><br>

    <input type="submit" value="Update">
  </form>

  <script>
    document.getElementById("motorForm").addEventListener("submit", function(e){
      e.preventDefault();
      fetch("/", { method:"POST", body:new FormData(this) });
    });

    setInterval(() => {
      fetch("/status")
        .then(res => res.json())
        .then(data => {
          document.getElementById("gas").innerHTML = data.gas;
          document.getElementById("direction").innerHTML = data.direction;
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

/* ================= HTML PROCESSOR ================= */
String processor(const String &var) {
  if (var == "GAS") return String(analogRead(gasSensor));
  if (var == "DIRECTION") return motorState;
  return String();
}

/* ================= WIFI ================= */
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(9600);
  initWiFi();

  pinMode(gasSensor, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"gas\":\"" + String(analogRead(gasSensor)) +
                  "\",\"direction\":\"" + motorState + "\"}";
    request->send(200, "application/json", json);
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("speed", true))
      motorSpeed = request->getParam("speed", true)->value().toInt();

    if (request->hasParam("direction", true))
      motorDirection = request->getParam("direction", true)->value().toInt();

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

/* ================= LOOP ================= */
void loop() {
  // --------- Đọc cảm biến khí gas (chỉ hiển thị) ---------
  int gasValue = analogRead(gasSensor);

  // --------- Điều khiển động cơ theo web ---------
  switch (motorDirection) {
    case 0:
      motorState = "Stopped";
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      break;
    case 1:
      motorState = "Clockwise";
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      break;
    case 2:
      motorState = "Counter Clockwise";
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      break;
  }

  analogWrite(motorSpeedPin, motorSpeed);

  // In debug
  Serial.print("Gas Sensor: "); Serial.print(gasValue);
  Serial.print(" | Motor: "); Serial.print(motorState);
  Serial.print(" | Speed: "); Serial.println(motorSpeed);

  delay(200);
}
