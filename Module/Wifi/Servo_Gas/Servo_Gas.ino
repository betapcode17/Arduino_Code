// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Servo.h>

// // ================= WIFI =================
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // ================= PHẦN CỨNG =================
// #define GAS_PIN   A0
// #define SERVO_PIN D1

// Servo myServo;
// AsyncWebServer server(80);

// // ================= BIẾN =================
// int gasValue = 0;
// int angleGas = 90;
// int angleNoGas = 0;
// int gasThreshold = 120;

// // DAO ĐỘNG SERVO (GIỐNG CODE JOYSTICK)
// unsigned long prevMillis = 0;
// const unsigned long interval = 400;
// bool servoState = false;

// // ================= HTML =================
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head><meta charset="utf-8"><title>Gas Servo</title></head>
// <body>
// <h2>Gas Sensor</h2>
// <p>Gas Value: <b><span id="gas">---</span></b></p>

// <hr>

// <p>Angle when GAS detected</p>
// <input id="g1" type="number" min="0" max="180">

// <p>Angle when NO GAS</p>
// <input id="g2" type="number" min="0" max="180">

// <br><br>
// <button onclick="apply()">APPLY</button>

// <script>
// setInterval(()=>{
//   fetch('/status').then(r=>r.json())
//   .then(d=>document.getElementById('gas').innerHTML=d.gas);
// },1000);

// function apply(){
//   fetch(`/set?a=${g1.value}&b=${g2.value}`);
// }
// </script>
// </body>
// </html>
// )rawliteral";

// // ================= SETUP =================
// void setup() {
//   Serial.begin(115200);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) delay(500);

//   myServo.attach(SERVO_PIN, 500, 2500);
//   myServo.write(0);

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
//     r->send_P(200, "text/html", index_html);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *r){
//     r->send(200, "application/json",
//       "{\"gas\":" + String(gasValue) + "}");
//   });

//   server.on("/set", HTTP_GET, [](AsyncWebServerRequest *r){
//     angleGas = constrain(r->getParam("a")->value().toInt(),0,180);
//     angleNoGas = constrain(r->getParam("b")->value().toInt(),0,180);
//     r->send(200,"text/plain","OK");
//   });

//   server.begin();
// }

// // ================= LOOP =================
// void loop() {
//   gasValue = analogRead(GAS_PIN);
//   unsigned long now = millis();

//   int targetAngle = (gasValue > gasThreshold) ? angleGas : angleNoGas;

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
#define SERVO_PIN D1
#define GAS_PIN   A0

Servo myServo;
AsyncWebServer server(80);

// ================= BIẾN SERVO =================
int servoAngle = 0;
bool isApplied = false;

// DAO ĐỘNG SERVO (GIỐNG CODE JOYSTICK)
unsigned long prevMillis = 0;
const unsigned long interval = 400;
bool servoState = false;

// ================= BIẾN CẢM BIẾN =================
int gasValue = 0;

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Servo Manual + Gas Display</title>
<script>
setInterval(()=>{
  fetch('/status')
    .then(r=>r.json())
    .then(d=>{
      document.getElementById('gas').innerHTML = d.gas;
    });
},1000);

function apply(){
  let a=document.getElementById('a').value;
  fetch(`/set?a=${a}`);
}
function stopServo(){
  fetch('/stop');
}
</script>
</head>

<body>
<h2>Gas Sensor (Display Only)</h2>
<p>Gas value: <b><span id="gas">---</span></b></p>

<hr>

<h2>Manual Servo Control</h2>
<p>Servo Angle (0-180)</p>
<input id="a" type="number" min="0" max="180">

<br><br>
<button onclick="apply()">APPLY</button>
<button onclick="stopServo()">STOP</button>

</body>
</html>
)rawliteral";

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  myServo.attach(SERVO_PIN, 500, 2500);
  myServo.write(0);

  // ===== WEB =====
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
    r->send_P(200, "text/html", index_html);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *r){
    r->send(200, "application/json",
      "{\"gas\":" + String(gasValue) + "}");
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *r){
    if (r->hasParam("a")) {
      servoAngle = constrain(r->getParam("a")->value().toInt(), 0, 180);
      isApplied = true;
      Serial.println("APPLY angle = " + String(servoAngle));
    }
    r->send(200, "text/plain", "OK");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *r){
    isApplied = false;
    servoState = false;
    myServo.write(0);
    r->send(200, "text/plain", "STOP");
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // CẢM BIẾN CHỈ ĐỂ HIỂN THỊ
  gasValue = analogRead(GAS_PIN);

  unsigned long now = millis();

  // CHƯA APPLY → SERVO ĐỨNG
  if (!isApplied) {
    myServo.write(0);
    servoState = false;
    return;
  }

  // DAO ĐỘNG SERVO 0 ↔ GÓC NHẬP
  if (now - prevMillis >= interval) {
    prevMillis = now;
    servoState = !servoState;
    myServo.write(servoState ? servoAngle : 0);
  }
}
