// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Servo.h>
// #include "DHT.h"

// // ================= WIFI =================
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // ================= PIN =================
// #define DHT_PIN    D5
// #define DHT_TYPE   DHT11
// #define SERVO_PIN  D1

// // ================= OBJECT =================
// DHT dht(DHT_PIN, DHT_TYPE);
// Servo myServo;
// AsyncWebServer server(80);

// // ================= BIẾN DHT =================
// float temperature = 0;
// float humidity = 0;

// // ================= SERVO =================
// int angleConditionMet = 90;      // khi nhiệt độ > ngưỡng
// int angleConditionNotMet = 0;    // khi nhiệt độ <= ngưỡng

// // ====== DAO ĐỘNG SERVO (LẤY TỪ CODE JOYSTICK) ======
// unsigned long prevMillis = 0;
// const unsigned long interval = 400; // ms
// bool servoState = false;

// // ================= READ DHT =================
// void readDHTSensor() {
//   temperature = dht.readTemperature();
//   humidity = dht.readHumidity();

//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("Failed to read from DHT!");
//     temperature = 0;
//     humidity = 0;
//   }
// }

// // ================= HTML =================
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>DHT Servo Control</title>
// </head>
// <body>
//   <h1>DHT11 Servo Control</h1>
//   <p>Temperature: <span id="temperature">%TEMP%</span> °C</p>
//   <p>Humidity: <span id="humidity">%HUM%</span> %</p>

//   <form id="servoForm">
//     <label>Angle when Temp > 27°C:</label>
//     <input type="number" name="angleMet" min="0" max="180"><br>
//     <label>Angle when Temp ≤ 27°C:</label>
//     <input type="number" name="angleNotMet" min="0" max="180"><br><br>
//     <input type="submit" value="Update">
//   </form>

//   <script>
//     document.getElementById("servoForm").addEventListener("submit", e => {
//       e.preventDefault();
//       fetch("/", { method: "POST", body: new FormData(e.target) });
//     });

//     setInterval(() => {
//       fetch("/status")
//         .then(r => r.json())
//         .then(d => {
//           document.getElementById("temperature").innerHTML = d.temperature;
//           document.getElementById("humidity").innerHTML = d.humidity;
//         });
//     }, 2000);
//   </script>
// </body>
// </html>
// )rawliteral";

// String processor(const String &var) {
//   if (var == "TEMP") return String(temperature);
//   if (var == "HUM") return String(humidity);
//   return String();
// }

// // ================= WIFI =================
// void initWiFi() {
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500); Serial.print(".");
//   }
//   Serial.println("\nIP: " + WiFi.localIP().toString());
// }

// // ================= SETUP =================
// void setup() {
//   Serial.begin(9600);
//   dht.begin();
//   initWiFi();

//   myServo.attach(SERVO_PIN, 500, 2500);
//   myServo.write(0);

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String json = "{\"temperature\":\"" + String(temperature) +
//                   "\",\"humidity\":\"" + String(humidity) + "\"}";
//     request->send(200, "application/json", json);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("angleMet", true))
//       angleConditionMet = constrain(
//         request->getParam("angleMet", true)->value().toInt(), 0, 180);

//     if (request->hasParam("angleNotMet", true))
//       angleConditionNotMet = constrain(
//         request->getParam("angleNotMet", true)->value().toInt(), 0, 180);

//     request->send(200, "text/plain", "OK");
//   });

//   server.begin();
// }

// // ================= LOOP =================
// void loop() {
//   static unsigned long lastRead = 0;
//   unsigned long now = millis();

//   // đọc DHT mỗi 2s
//   if (now - lastRead >= 2000) {
//     lastRead = now;
//     readDHTSensor();
//   }

//   int targetAngle;

//   // ====== CHỌN GÓC THEO NHIỆT ĐỘ ======
//   if (humidity > 80) {
//     targetAngle = angleConditionMet;
//   } else {
//     targetAngle = angleConditionNotMet;
//   }

//   // ====== DAO ĐỘNG SERVO (GIỐNG CODE JOYSTICK) ======
//   if (now - prevMillis >= interval) {
//     prevMillis = now;
//     servoState = !servoState;
//     myServo.write(servoState ? targetAngle : 0);
//   }
// }








#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
#include "DHT.h"

// ================= WIFI =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PIN =================
#define DHT_PIN    D5
#define DHT_TYPE   DHT11
#define SERVO_PIN  D1

// ================= OBJECT =================
DHT dht(DHT_PIN, DHT_TYPE);
Servo myServo;
AsyncWebServer server(80);

// ================= BIẾN DHT =================
float temperature = 0;
float humidity = 0;

// ================= SERVO =================
int servoAngle = 0;
bool isApplied = false;

// DAO ĐỘNG SERVO (GIỐNG CODE JOYSTICK)
unsigned long prevMillis = 0;
const unsigned long interval = 400;
bool servoState = false;

// ================= ĐỌC DHT =================
void readDHTSensor() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    temperature = 0;
    humidity = 0;
  }
}

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>DHT Display + Servo</title>
<script>
setInterval(()=>{
  fetch('/status')
    .then(r=>r.json())
    .then(d=>{
      document.getElementById('temp').innerHTML=d.temperature;
      document.getElementById('hum').innerHTML=d.humidity;
    });
},2000);

function applyServo(){
  let a=document.getElementById('angle').value;
  fetch(`/set?a=${a}`);
}

function stopServo(){
  fetch('/stop');
}
</script>
</head>
<body>

<h2>DHT11 Sensor</h2>
<p>Temperature: <b><span id="temp">--</span> °C</b></p>
<p>Humidity: <b><span id="hum">--</span> %</b></p>

<hr>

<h3>Servo Control</h3>
<p>Servo Angle (0-180)</p>
<input type="number" id="angle" min="0" max="180">

<br><br>
<button onclick="applyServo()">APPLY</button>
<button onclick="stopServo()">STOP</button>

</body>
</html>
)rawliteral";

// ================= WIFI =================
void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nIP: " + WiFi.localIP().toString());
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  dht.begin();
  initWiFi();

  myServo.attach(SERVO_PIN, 500, 2500);
  myServo.write(0);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send_P(200, "text/html", index_html);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send(200, "application/json",
      "{\"temperature\":" + String(temperature) +
      ",\"humidity\":" + String(humidity) + "}");
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *req){
    if (req->hasParam("a")) {
      servoAngle = constrain(req->getParam("a")->value().toInt(), 0, 180);
      isApplied = true;
      Serial.println("APPLY angle = " + String(servoAngle));
    }
    req->send(200, "text/plain", "OK");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *req){
    isApplied = false;
    servoState = false;
    myServo.write(0);
    req->send(200, "text/plain", "STOP");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  static unsigned long lastRead = 0;
  unsigned long now = millis();

  // DHT chỉ để hiển thị (2s)
  if (now - lastRead >= 2000) {
    lastRead = now;
    readDHTSensor();
  }

  // chưa APPLY → servo đứng yên
  if (!isApplied) {
    myServo.write(0);
    servoState = false;
    return;
  }

  // DAO ĐỘNG SERVO (0 ↔ góc nhập)
  if (now - prevMillis >= interval) {
    prevMillis = now;
    servoState = !servoState;
    myServo.write(servoState ? servoAngle : 0);
  }
}
