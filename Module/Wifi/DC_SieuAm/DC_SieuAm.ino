// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

// // Thông tin WiFi
// const char *ssid = "<3";
// const char *password = "baophansayhi";

// // Định nghĩa chân cho cảm biến siêu âm và động cơ DC
// const int trigPin = D5;  // Chân trigger
// const int echoPin = D6;  // Chân echo
// const int motorPin1 = D1; // Chân điều khiển chiều quay 1
// const int motorPin2 = D2; // Chân điều khiển chiều quay 2
// const int motorSpeedPin = D4; // Chân PWM điều khiển tốc độ động cơ

// // Tạo web server
// AsyncWebServer server(80);

// // Các biến để lưu giá trị
// int motorSpeed = 255;  // Tốc độ động cơ (0-255)
// String direction = "Stopped";  // Chiều quay ban đầu là dừng

// // Đọc giá trị từ cảm biến siêu âm
// long readDistance() {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   long duration = pulseIn(echoPin, HIGH);  // Đọc thời gian
//   long distance = duration * 0.034 / 2;   // Tính khoảng cách
//   return distance;
// }

// // HTML giao diện web
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>DC Motor Control</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h1>DC Motor Control</h1>
//   <p>Distance from sensor: <span id="distance">%DISTANCE%</span> cm</p>
//   <p>Motor Direction: <span id="direction">%DIRECTION%</span></p>
//   <form id="motorForm">
//     <label for="steps">Speed (0-255): </label>
//     <input type="number" name="steps" min="0" max="255" required>
//     <input type="submit" value="Set Speed">
//   </form>
//   <script>
//     document.getElementById('motorForm').addEventListener('submit', function (e) {
//       e.preventDefault();
//       const formData = new FormData(this);
//       fetch("/", {
//         method: "POST",
//         body: formData
//       }).then(response => response.text()).catch(error => console.error("Error:", error));
//     });

//     setInterval(function() {
//       fetch("/status")
//         .then(response => response.json())
//         .then(data => {
//           document.getElementById("distance").innerHTML = data.distance;
//           document.getElementById("direction").innerHTML = data.direction;
//           document.getElementById("speed").innerHTML = data.speed;
//         })
//         .catch(error => console.error("Error:", error));
//     }, 2000);
//   </script>
// </body>
// </html>
// )rawliteral";

// // Hàm xử lý HTML
// String processor(const String &var) {
//   if (var == "DISTANCE") {
//     return String(readDistance());  // Hiển thị giá trị khoảng cách
//   } else if (var == "DIRECTION") {
//     return direction;  // Hiển thị chiều quay của động cơ
//   } else if (var == "SPEED") {
//     return String(motorSpeed);  // Hiển thị tốc độ của động cơ
//   }
//   return String();
// }

// void initWiFi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print('.');
//     delay(1000);
//   }
//   Serial.println("\nConnected! IP Address: ");
//   Serial.println(WiFi.localIP());
// }

// void setup() {
//   // Khởi tạo Serial Monitor
//   Serial.begin(9600);

//   // Khởi tạo kết nối WiFi
//   initWiFi();

//   // Khởi tạo chân cho cảm biến siêu âm
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);

//   // Khởi tạo chân điều khiển động cơ DC
//   pinMode(motorPin1, OUTPUT);
//   pinMode(motorPin2, OUTPUT);
//   pinMode(motorSpeedPin, OUTPUT);

//   // Web server routes
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String jsonResponse = "{\"distance\":\"" + String(readDistance()) + "\",\"direction\":\"" + direction + "\"}";
//     request->send(200, "application/json", jsonResponse);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("steps", true)) {
//       motorSpeed = request->getParam("steps", true)->value().toInt();
//       motorSpeed = constrain(motorSpeed, 0, 255);  // Đảm bảo tốc độ nằm trong khoảng 0-255
//       Serial.print("Motor speed set to: ");
//       Serial.println(motorSpeed);

//       // Điều chỉnh chiều quay và tốc độ động cơ dựa trên khoảng cách
//       long distance = readDistance();
//       if (distance > 20) {
//         direction = "Clockwise";
//         digitalWrite(motorPin1, HIGH);
//         digitalWrite(motorPin2, LOW);
//       } else {
//         direction = "Counterclockwise";
//         digitalWrite(motorPin1, LOW);
//         digitalWrite(motorPin2, HIGH);
//       }

//       // Nếu tốc độ là 0, dừng động cơ
//       if (motorSpeed == 0) {
//         direction = "Stopped";
//         digitalWrite(motorPin1, LOW);
//         digitalWrite(motorPin2, LOW);  // Tắt cả hai chân điều khiển chiều quay
//         analogWrite(motorSpeedPin, 0); // Tắt tín hiệu PWM
//       } else {
//         analogWrite(motorSpeedPin, motorSpeed); // Điều chỉnh tốc độ động cơ
//       }
//     }
//     request->send(200, "text/plain", "Settings updated");
//   });

//   server.begin();
// }

// void loop() {
//  long distance = readDistance();
//   if (distance > 20) {
//     direction = "Clockwise";
//     digitalWrite(motorPin1, HIGH);
//     digitalWrite(motorPin2, LOW);
//   } else {
//     direction = "Counterclockwise";
//     digitalWrite(motorPin1, LOW);
//     digitalWrite(motorPin2, HIGH);
//   }

//   // Nếu tốc độ là 0, dừng động cơ
//   if (motorSpeed == 0) {
//     direction = "Stopped";
//     digitalWrite(motorPin1, LOW);
//     digitalWrite(motorPin2, LOW);  // Tắt cả hai chân điều khiển chiều quay
//     analogWrite(motorSpeedPin, 0); // Tắt tín hiệu PWM
//   } else {
//     analogWrite(motorSpeedPin, motorSpeed); // Điều chỉnh tốc độ động cơ
//   }
// }



















#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ===== WIFI =====
const char *ssid = "<3";
const char *password = "baophansayhi";

// ===== PIN =====
const int trigPin = D5;
const int echoPin = D6;
const int motorPin1 = D1;
const int motorPin2 = D2;
const int motorSpeedPin = D4;

// ===== SERVER =====
AsyncWebServer server(80);

// ===== MOTOR STATE =====
int motorSpeed = 0; // 0-255
int motorDirection = 0; // 0=Stop, 1=CW, 2=CCW
String motorState = "Stopped";

// ===== READ ULTRASONIC SENSOR =====
long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

// ===== HTML =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Ultrasonic Sensor & DC Motor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>Ultrasonic Sensor & Motor Control</h2>
  <p>Distance: <span id="distance">%DISTANCE%</span> cm</p>
  <p>Motor Direction: <span id="direction">%DIRECTION%</span></p>
  <p>Motor Speed: <span id="speed">%SPEED%</span></p>

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
    document.getElementById("motorForm").onsubmit = e=>{
      e.preventDefault();
      fetch("/", { method:"POST", body:new FormData(e.target) });
    };

    setInterval(()=>{
      fetch("/status")
      .then(r=>r.json())
      .then(d=>{
        document.getElementById("distance").innerHTML = d.distance;
        document.getElementById("direction").innerHTML = d.direction;
        document.getElementById("speed").innerHTML = d.speed;
      });
    },2000);
  </script>
</body>
</html>
)rawliteral";

// ===== PROCESS HTML =====
String processor(const String &var){
  if(var=="DISTANCE") return String(readDistance());
  if(var=="DIRECTION") return motorState;
  if(var=="SPEED") return String(motorSpeed);
  return String();
}

// ===== INIT WIFI =====
void initWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: "+WiFi.localIP().toString());
}

// ===== MOTOR CONTROL =====
void updateMotor(){
  switch(motorDirection){
    case 0: motorState="Stopped"; digitalWrite(motorPin1,LOW); digitalWrite(motorPin2,LOW); break;
    case 1: motorState="Clockwise"; digitalWrite(motorPin1,HIGH); digitalWrite(motorPin2,LOW); break;
    case 2: motorState="Counter Clockwise"; digitalWrite(motorPin1,LOW); digitalWrite(motorPin2,HIGH); break;
  }
  analogWrite(motorSpeedPin,motorSpeed);
}

// ===== SETUP =====
void setup(){
  Serial.begin(9600);

  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(motorPin1,OUTPUT);
  pinMode(motorPin2,OUTPUT);
  pinMode(motorSpeedPin,OUTPUT);

  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send_P(200,"text/html",index_html,processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req){
    String json="{\"distance\":\""+String(readDistance())+"\",\"direction\":\""+motorState+"\",\"speed\":\""+String(motorSpeed)+"\"}";
    req->send(200,"application/json",json);
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *req){
    if(req->hasParam("speed",true)) motorSpeed=req->getParam("speed",true)->value().toInt();
    if(req->hasParam("direction",true)) motorDirection=req->getParam("direction",true)->value().toInt();
    req->send(200,"text/plain","OK");
  });

  server.begin();
}

// ===== LOOP =====
void loop(){
  static unsigned long t=0;
  if(millis()-t>100){
    t=millis();
    updateMotor();
    // Chỉ in ra khoảng cách để kiểm tra sensor
    Serial.print("Distance: "); Serial.print(readDistance()); Serial.println(" cm");
    Serial.print("Motor: "); Serial.print(motorState); Serial.print(" | Speed: "); Serial.println(motorSpeed);
  }
}
