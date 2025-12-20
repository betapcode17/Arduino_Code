#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// ================= WIFI =================
const char* ssid = "NGOC HOA";
const char* password = "home1234";

// ================= PIN ==================
#define JOYSTICK_PIN A0
#define SERVO_PIN    D1

// ================= OBJECT ===============
Servo myServo;
ESP8266WebServer server(80);

// ================= BIẾN =================
int joyVal = 0;
int angleLeft  = 0;
int angleRight = 0;
bool isApplied = false;

// ngưỡng joystick
const int LEFT_TH  = 300;
const int RIGHT_TH = 700;

// timer servo
unsigned long prevMillis = 0;
const unsigned long interval = 400; // ms
bool servoState = false;

// ================= WEB ==================
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Servo Joystick</title>
<script>
setInterval(()=>{
  fetch('/sensor')
    .then(r=>r.json())
    .then(d=>document.getElementById('joy').innerHTML=d.val);
},300);

function applyAngle(){
  let l=document.getElementById('left').value;
  let r=document.getElementById('right').value;
  fetch(/set?l=${l}&r=${r});
}

function stopServo(){
  fetch('/stop');
}
</script>
</head>
<body>
<h2>Joystick:</h2>
<h1 id="joy">---</h1>

<hr>

<p>Góc TRÁI (joystick &lt; 300)</p>
<input type="number" id="left" min="0" max="180">

<p>Góc PHẢI (joystick &gt; 700)</p>
<input type="number" id="right" min="0" max="180">

<br><br>
<button onclick="applyAngle()">APPLY</button>
<button onclick="stopServo()">STOP</button>

</body>
</html>
)=====";
  server.send(200, "text/html", html);
}

void handleSensor() {
  joyVal = analogRead(JOYSTICK_PIN);
  server.send(200, "application/json",
              "{\"val\":" + String(joyVal) + "}");
}

void handleSet() {
  angleLeft  = constrain(server.arg("l").toInt(), 0, 180);
  angleRight = constrain(server.arg("r").toInt(), 0, 180);
  isApplied = true;

  Serial.println("APPLY");
  Serial.println("LEFT  = " + String(angleLeft));
  Serial.println("RIGHT = " + String(angleRight));

  server.send(200, "text/plain", "OK");
}

void handleStop() {
  isApplied = false;
  myServo.write(0);
  server.send(200, "text/plain", "STOP");
}

// ================= SETUP ================
void setup() {
  Serial.begin(115200);

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
  joyVal = analogRead(JOYSTICK_PIN);
  unsigned long now = millis();

  // chưa APPLY → dừng
  if (!isApplied) {
    myServo.write(0);
    servoState = false;
    return;
  }

  // SANG PHẢI
  if (joyVal > RIGHT_TH) {
    if (now - prevMillis >= interval) {
      prevMillis = now;
      servoState = !servoState;
      myServo.write(servoState ? angleRight : 0);
      delay(500);
    }
  }
  // SANG TRÁI
  else if (joyVal < LEFT_TH) {
    if (now - prevMillis >= interval) {
      prevMillis = now;
      servoState = !servoState;
      myServo.write(servoState ? angleLeft : 0);
        delay(500);
    }
  }
  // THẢ JOYSTICK
  else {
    myServo.write(0);
    servoState = false;
  }
}