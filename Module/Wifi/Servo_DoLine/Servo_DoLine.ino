// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Servo.h>

// // ================= WIFI =================
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // ================= PHẦN CỨNG =================
// #define LINE_SENSOR_PIN D5
// #define SERVO_PIN       D1

// Servo myServo;
// AsyncWebServer server(80);

// // ================= BIẾN =================
// String lineSensorStateStr = "White";

// int angleLine = 90;     // Có line (đen)
// int angleNoLine = 0;    // Không line (trắng)

// // DAO ĐỘNG SERVO (GIỐNG CODE JOYSTICK)
// unsigned long prevMillis = 0;
// const unsigned long interval = 400;
// bool servoState = false;

// // ================= HTML =================
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
// <meta charset="utf-8">
// <title>Line Sensor Servo</title>
// <script>
// setInterval(()=>{
//   fetch('/status')
//     .then(r=>r.json())
//     .then(d=>{
//       document.getElementById('state').innerHTML=d.state;
//     });
// },1000);

// function apply(){
//   let a1=document.getElementById('a1').value;
//   let a2=document.getElementById('a2').value;
//   fetch(`/set?l=${a1}&n=${a2}`);
// }
// </script>
// </head>

// <body>
// <h2>Line Sensor</h2>
// <p>State: <b><span id="state">---</span></b></p>

// <hr>

// <h3>Servo Angles</h3>
// <p>Angle when BLACK line</p>
// <input id="a1" type="number" min="0" max="180">

// <p>Angle when WHITE surface</p>
// <input id="a2" type="number" min="0" max="180">

// <br><br>
// <button onclick="apply()">APPLY</button>
// </body>
// </html>
// )rawliteral";

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

//   pinMode(LINE_SENSOR_PIN, INPUT);
//   initWiFi();

//   myServo.attach(SERVO_PIN, 500, 2500);
//   myServo.write(0);

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
//     r->send_P(200, "text/html", index_html);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *r){
//     r->send(200, "application/json",
//       "{\"state\":\"" + lineSensorStateStr + "\"}");
//   });

//   server.on("/set", HTTP_GET, [](AsyncWebServerRequest *r){
//     if (r->hasParam("l"))
//       angleLine = constrain(r->getParam("l")->value().toInt(),0,180);
//     if (r->hasParam("n"))
//       angleNoLine = constrain(r->getParam("n")->value().toInt(),0,180);

//     Serial.println("Updated angles");
//     r->send(200,"text/plain","OK");
//   });

//   server.begin();
// }

// // ================= LOOP =================
// void loop() {
//   unsigned long now = millis();

//   int lineState = digitalRead(LINE_SENSOR_PIN);
//   int targetAngle;

//   if (lineState == HIGH) {
//     lineSensorStateStr = "Black";
//     targetAngle = angleLine;
//   } else {
//     lineSensorStateStr = "White";
//     targetAngle = angleNoLine;
//   }

//   // ===== DAO ĐỘNG SERVO 0 ↔ GÓC =====
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

// ================= WIFI =================
const char *ssid = "NGOC HOA";
const char *password = "home1234";

// ================= PHẦN CỨNG =================
#define LINE_SENSOR_PIN D5
#define SERVO_PIN       D1

Servo myServo;
AsyncWebServer server(80);

// ================= BIẾN =================
String lineSensorStateStr = "White";

int angleLine = 90;     // Có line (đen)
int angleNoLine = 0;    // Không line (trắng)

int targetAngle = 0;    // Góc servo hiện tại

// ===== DAO ĐỘNG SERVO =====
unsigned long prevMillis = 0;
const unsigned long interval = 400;
bool servoState = false;

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Line Sensor Servo</title>
<script>
setInterval(()=>{ fetch('/status').then(r=>r.json()).then(d=>{
  document.getElementById('state').innerHTML=d.state;
}); },1000);

function apply(){
  let a1=document.getElementById('a1').value;
  let a2=document.getElementById('a2').value;
  fetch(`/set?l=${a1}&n=${a2}`);
}
</script>
</head>
<body>
<h2>Line Sensor</h2>
<p>State: <b><span id="state">---</span></b></p>

<hr>
<h3>Servo Angles</h3>
<p>Angle when BLACK line</p>
<input id="a1" type="number" min="0" max="180">
<p>Angle when WHITE surface</p>
<input id="a2" type="number" min="0" max="180">
<br><br>
<button onclick="apply()">APPLY</button>
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

// ====================== MODULE CẢM BIẾN ======================
String readLineSensor() {
  int state = digitalRead(LINE_SENSOR_PIN);
  if (state == HIGH) {
    lineSensorStateStr = "Black";
  } else {
    lineSensorStateStr = "White";
  }
  return lineSensorStateStr;
}

int computeTargetAngle() {
  return (lineSensorStateStr == "Black") ? angleLine : angleNoLine;
}

// ====================== MODULE SERVO ======================
void updateServo() {
  unsigned long now = millis();
  if (now - prevMillis >= interval) {
    prevMillis = now;
    servoState = !servoState;
    myServo.write(servoState ? targetAngle : 0);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  pinMode(LINE_SENSOR_PIN, INPUT);

  initWiFi();

  myServo.attach(SERVO_PIN, 500, 2500);
  myServo.write(0);

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
    r->send_P(200, "text/html", index_html);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *r){
    r->send(200, "application/json",
      "{\"state\":\"" + lineSensorStateStr + "\"}");
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *r){
    if (r->hasParam("l"))
      angleLine = constrain(r->getParam("l")->value().toInt(), 0, 180);
    if (r->hasParam("n"))
      angleNoLine = constrain(r->getParam("n")->value().toInt(), 0, 180);
    Serial.println("Updated angles");
    r->send(200,"text/plain","OK");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // 1️⃣ Đọc trạng thái cảm biến line
  readLineSensor();

  // 2️⃣ Xác định góc servo mục tiêu
  targetAngle = computeTargetAngle();

  // 3️⃣ Dao động servo độc lập
  updateServo();

  // 4️⃣ Debug
  Serial.print("Line State: "); Serial.print(lineSensorStateStr);
  Serial.print(" | Target Angle: "); Serial.println(targetAngle);
}
