#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include "DHT.h"

// ================= WIFI =================
const char *ssid = "D320";
const char *password = "20012005";

// ================= PIN =================
#define DHT_PIN    D5
#define DHT_TYPE   DHT11
#define SERVO_PIN  D1

// ================= OBJECT =================
DHT dht(DHT_PIN, DHT_TYPE);
Servo myServo;
AsyncWebServer server(80);

// ================= BIẾN =================
float temperature = 0;
float humidity = 0;

int angleConditionMet = 90;
int angleConditionNotMet = 0;

// ================= 🔥 ANGLE → SERVO PULSE =================
int angleToPulse(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, 1000, 2000);
}

// ================= READ DHT =================
void readDHTSensor() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT!");
    temperature = 0;
    humidity = 0;
  }
}

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DHT Servo Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>DHT11 Servo Control</h1>
  <p>Temperature: <span id="temperature">%TEMP%</span> &deg;C</p>
  <p>Humidity: <span id="humidity">%HUM%</span> %</p>

  <form id="servoForm">
    <label>Angle when Temp > 27°C:</label>
    <input type="number" name="angleMet" min="0" max="180" required><br>
    <label>Angle when Temp ≤ 27°C:</label>
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
        .then(d => {
          document.getElementById("temperature").innerHTML = d.temperature;
          document.getElementById("humidity").innerHTML = d.humidity;
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

// ================= HTML PROCESS =================
String processor(const String &var) {
  if (var == "TEMP") return String(temperature);
  if (var == "HUM") return String(humidity);
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

  dht.begin();
  initWiFi();

  // Servo (chuẩn ESP8266)
  myServo.attach(SERVO_PIN);
  myServo.writeMicroseconds(angleToPulse(angleConditionNotMet));

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json =
      "{\"temperature\":\"" + String(temperature) +
      "\",\"humidity\":\"" + String(humidity) + "\"}";
    request->send(200, "application/json", json);
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angleMet", true))
      angleConditionMet = constrain(
        request->getParam("angleMet", true)->value().toInt(), 0, 180);

    if (request->hasParam("angleNotMet", true))
      angleConditionNotMet = constrain(
        request->getParam("angleNotMet", true)->value().toInt(), 0, 180);

    Serial.print("Updated angles -> ");
    Serial.print(angleConditionMet);
    Serial.print(" | ");
    Serial.println(angleConditionNotMet);

    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  static unsigned long lastRead = 0;

  if (millis() - lastRead >= 2000) {
    lastRead = millis();
    readDHTSensor();

    if (temperature > 27) {
      myServo.writeMicroseconds(angleToPulse(angleConditionMet));
    } else {
      myServo.writeMicroseconds(angleToPulse(angleConditionNotMet));
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" °C | Hum: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  delay(500);

  // Quay về 0 độ
  myServo.writeMicroseconds(angleToPulse(0));
  delay(500);
}
