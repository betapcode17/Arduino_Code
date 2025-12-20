// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Servo.h>

// // ================= WiFi =================
// const char *ssid = "NGOC HOA";
// const char *password = "home1234";

// // ================= PHẦN CỨNG =================
// #define TRIG_PIN  D5
// #define ECHO_PIN  D6
// #define SERVO_PIN D1

// Servo myServo;
// AsyncWebServer server(80);

// // ================= BIẾN =================
// long distanceCM = 0;

// // servo
// int servoAngle = 0;
// bool isApplied = false;

// // DAO ĐỘNG SERVO (GIỐNG CODE TRƯỚC)
// unsigned long prevMillis = 0;
// const unsigned long interval = 400;
// bool servoState = false;

// // ================= HTML =================
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
// <meta charset="utf-8">
// <title>Ultrasonic + Servo</title>
// <script>
// setInterval(()=>{
//   fetch('/status')
//     .then(r=>r.json())
//     .then(d=>document.getElementById('dist').innerHTML=d.distance);
// },500);

// function applyServo(){
//   let a=document.getElementById('angle').value;
//   fetch(`/set?a=${a}`);
// }

// function stopServo(){
//   fetch('/stop');
// }
// </script>
// </head>
// <body>
// <h2>Distance:</h2>
// <h1><span id="dist">---</span> cm</h1>

// <hr>

// <p>Servo Angle (0-180)</p>
// <input type="number" id="angle" min="0" max="180">

// <br><br>
// <button onclick="applyServo()">APPLY</button>
// <button onclick="stopServo()">STOP</button>

// </body>
// </html>
// )rawliteral";

// // ================= HÀM =================
// long readUltrasonic() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   long duration = pulseIn(ECHO_PIN, HIGH, 30000);
//   if (duration == 0) return 999;
//   return duration * 0.034 / 2;
// }

// // ================= SETUP =================
// void setup() {
//   Serial.begin(115200);

//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);

//   myServo.attach(SERVO_PIN, 500, 2500);
//   myServo.write(0);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500); Serial.print(".");
//   }
//   Serial.println("\nIP: " + WiFi.localIP().toString());

//   // ===== WEB =====
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
//     req->send_P(200, "text/html", index_html);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req){
//     req->send(200, "application/json",
//       "{\"distance\":" + String(distanceCM) + "}");
//   });

//   server.on("/set", HTTP_GET, [](AsyncWebServerRequest *req){
//     if (req->hasParam("a")) {
//       servoAngle = constrain(req->getParam("a")->value().toInt(), 0, 180);
//       isApplied = true;
//       Serial.println("APPLY angle = " + String(servoAngle));
//     }
//     req->send(200, "text/plain", "OK");
//   });

//   server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *req){
//     isApplied = false;
//     servoState = false;
//     myServo.write(0);
//     req->send(200, "text/plain", "STOP");
//   });

//   server.begin();
// }

// // ================= LOOP =================
// void loop() {
//   unsigned long now = millis();

//   // CẢM BIẾN CHỈ HIỂN THỊ
//   distanceCM = readUltrasonic();

//   // chưa APPLY → dừng (GIỐNG CODE TRÊN)
//   if (!isApplied) {
//     myServo.write(0);
//     servoState = false;
//     return;
//   }

//   // DAO ĐỘNG SERVO (0 ↔ góc nhập)
//   if (now - prevMillis >= interval) {
//     prevMillis = now;
//     servoState = !servoState;
//     myServo.write(servoState ? servoAngle : 0);
//   }
// }




#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// ================= WIFI =================
const char* ssid = "NGOC HOA";
const char* password = "home1234";

// ================= PIN =================
#define TRIG_PIN   D5
#define ECHO_PIN   D6
#define SERVO_PIN  D1

// ================= OBJECT =================
Servo myServo;
ESP8266WebServer server(80);

// ================= BIẾN =================
long distanceCM = 0;

int servoAngle = 0;     // góc do người dùng nhập
bool isApplied = false;

// timer servo (GIỐNG CODE JOYSTICK)
unsigned long prevMillis = 0;
const unsigned long interval = 400;
bool servoState = false;

// ================= HÀM =================
long readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

// ================= WEB =================
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Ultrasonic + Servo</title>
<script>
setInterval(()=>{
  fetch('/sensor')
    .then(r=>r.json())
    .then(d=>document.getElementById('dist').innerHTML=d.dist);
},500);

function applyAngle(){
  let a=document.getElementById('angle').value;
  fetch(`/set?a=${a}`);
}

function stopServo(){
  fetch('/stop');
}
</script>
</head>
<body>
<h2>Distance:</h2>
<h1><span id="dist">---</span> cm</h1>

<hr>

<p>Servo Angle (0-180)</p>
<input type="number" id="angle" min="0" max="180">

<br><br>
<button onclick="applyAngle()">APPLY</button>
<button onclick="stopServo()">STOP</button>

</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

void handleSensor() {
  distanceCM = readUltrasonic();
  server.send(200, "application/json",
              "{\"dist\":" + String(distanceCM) + "}");
}

void handleSet() {
  servoAngle = constrain(server.arg("a").toInt(), 0, 180);
  isApplied = true;

  Serial.print("APPLY angle = ");
  Serial.println(servoAngle);

  server.send(200, "text/plain", "OK");
}

void handleStop() {
  isApplied = false;
  myServo.write(0);
  servoState = false;
  server.send(200, "text/plain", "STOP");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.attach(SERVO_PIN, 500, 2500);
  myServo.write(0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }

  Serial.println("\nIP:");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/sensor", handleSensor);
  server.on("/set", handleSet);
  server.on("/stop", handleStop);
  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();
  unsigned long now = millis();

  // chưa APPLY → dừng (GIỐNG CODE JOYSTICK)
  if (!isApplied) {
    myServo.write(0);
    servoState = false;
    return;
  }

  // DAO ĐỘNG SERVO (GIỐNG HỆT CODE JOYSTICK)
  if (now - prevMillis >= interval) {
    prevMillis = now;
    servoState = !servoState;
    myServo.write(servoState ? servoAngle : 0);
  }
}
