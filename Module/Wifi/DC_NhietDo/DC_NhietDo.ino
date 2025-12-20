// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include "DHT.h" // Thư viện cho DHT11

// // Thông tin WiFi
// const char *ssid = "Tran Thi Kim Loan";
// const char *password = "01229333995";

// // Định nghĩa chân cho cảm biến nhiệt độ, độ ẩm, và động cơ DC
// #define DHT_PIN D5
// #define DHT_TYPE DHT11
// #define MOTOR_PIN1 D1
// #define MOTOR_PIN2 D2
// #define SPEED_PIN D4

// // Tạo đối tượng DHT
// DHT dht(DHT_PIN, DHT_TYPE);

// // Các thông số và trạng thái của động cơ DC
// String direction = "Stopped";  // Chiều quay ban đầu là dừng
// int motorSpeed = 0;  // Tốc độ ban đầu

// // Tạo web server
// AsyncWebServer server(80);

// // Hàm đọc dữ liệu từ cảm biến DHT
// float temperature = 0;
// float humidity = 0;

// void readDHTSensor() {
//   temperature = dht.readTemperature(); // Đọc nhiệt độ
//   humidity = dht.readHumidity();       // Đọc độ ẩm
//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("Failed to read from DHT sensor!");
//     temperature = 0;
//     humidity = 0;
//   } else {
//     Serial.print("Temperature: ");
//     Serial.println(temperature);
//     Serial.print("Humidity: ");
//     Serial.println(humidity);
//   }
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
//   <p>Temperature: <span id="temperature">%TEMP%</span> &deg;C</p>
//   <p>Humidity: <span id="humidity">%HUM%</span> %</p>
//   <p>Motor Direction: <span id="direction">%DIRECTION%</span></p>
//   <p>Motor Speed: <span id="speed">%SPEED%</span></p>
//   <form id="motorForm">
//     <label for="speed">Motor Speed (0-255): </label>
//     <input type="number" name="speed" min="0" max="255" required>
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
//           document.getElementById("temperature").innerHTML = data.temperature;
//           document.getElementById("humidity").innerHTML = data.humidity;
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
//   if (var == "TEMP") {
//     return String(temperature);
//   } else if (var == "HUM") {
//     return String(humidity);
//   } else if (var == "DIRECTION") {
//     return direction; // Hiển thị chiều quay của động cơ
//   } else if (var == "SPEED") {
//     return String(motorSpeed); // Hiển thị tốc độ động cơ
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
//   Serial.println("\nConnected to WiFi! IP Address: ");
//   Serial.println(WiFi.localIP());
// }

// void setup() {
//   // Khởi tạo Serial Monitor
//   Serial.begin(9600);

//   // Khởi tạo cảm biến DHT
//   dht.begin();

//   // Khởi tạo kết nối WiFi
//   initWiFi();

//   // Khởi tạo chân điều khiển động cơ DC
//   pinMode(MOTOR_PIN1, OUTPUT);
//   pinMode(MOTOR_PIN2, OUTPUT);
//   pinMode(SPEED_PIN, OUTPUT); // Chân PWM điều khiển tốc độ động cơ

//   // Web server routes
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String jsonResponse = "{\"temperature\":\"" + String(temperature) + "\",\"humidity\":\"" + String(humidity) +
//                           "\",\"direction\":\"" + direction + "\",\"speed\":\"" + String(motorSpeed) + "\"}";
//     request->send(200, "application/json", jsonResponse);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("speed", true)) {
//       motorSpeed = request->getParam("speed", true)->value().toInt();
//       Serial.print("Motor speed set to: ");
//       Serial.println(motorSpeed);

//       // Điều khiển động cơ DC theo nhiệt độ
//       if (temperature > 27) {
//         direction = "Counterclockwise";  // Quay ngược chiều khi nhiệt độ trên 27°C
//       } else {
//         direction = "Clockwise";         // Quay thuận chiều khi nhiệt độ dưới 27°C
//       }

//       // Điều khiển động cơ
//       if (motorSpeed > 0) {
//         if (direction == "Clockwise") {
//           digitalWrite(MOTOR_PIN1, HIGH);
//           digitalWrite(MOTOR_PIN2, LOW);
//         } else {
//           digitalWrite(MOTOR_PIN1, LOW);
//           digitalWrite(MOTOR_PIN2, HIGH);
//         }
//         analogWrite(SPEED_PIN, motorSpeed);  // Điều chỉnh tốc độ động cơ qua PWM
//       } else {
//         digitalWrite(MOTOR_PIN1, LOW);
//         digitalWrite(MOTOR_PIN2, LOW);  // Tắt động cơ khi tốc độ bằng 0
//         analogWrite(SPEED_PIN, 0);      // Tắt PWM khi không có tốc độ
//       }
//     }
//     request->send(200, "text/plain", "Settings updated");
//   });

//   server.begin();
// }

// void loop() {
//   static unsigned long lastDHTRead = 0;
//   const unsigned long DHTInterval = 2000; // Đọc cảm biến DHT mỗi 2 giây

//   // Đọc cảm biến DHT theo chu kỳ
//   if (millis() - lastDHTRead >= DHTInterval) {
//     lastDHTRead = millis();
//     readDHTSensor();

//     // Điều chỉnh chiều quay của động cơ dựa trên nhiệt độ
//     if (temperature > 28) {
//       direction = "Counterclockwise";  // Quay ngược chiều khi nhiệt độ trên 27°C
//     } else {
//       direction = "Clockwise";         // Quay thuận chiều khi nhiệt độ dưới 27°C
//     }

//     // In thông tin thay đổi nếu có
//     Serial.print("Temperature: ");
//     Serial.print(temperature);
//     Serial.println("°C");
//     Serial.print("Humidity: ");
//     Serial.print(humidity);
//     Serial.println("%");
//     Serial.print("Motor Direction: ");
//     Serial.println(direction);
//   }

//   // Điều khiển tốc độ động cơ DC
//   if (motorSpeed > 0) {
//     if (direction == "Clockwise") {
//       digitalWrite(MOTOR_PIN1, HIGH);
//       digitalWrite(MOTOR_PIN2, LOW);
//     } else {
//       digitalWrite(MOTOR_PIN1, LOW);
//       digitalWrite(MOTOR_PIN2, HIGH);
//     }
//     analogWrite(SPEED_PIN, motorSpeed);  // Điều chỉnh tốc độ động cơ qua PWM
//   } else{
//     digitalWrite(MOTOR_PIN1, LOW);
//     digitalWrite(MOTOR_PIN2, LOW);  // Tắt động cơ khi tốc độ bằng 0
//     analogWrite(SPEED_PIN, 0);      // Tắt PWM khi không có tốc độ
//   }
// }





#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"

// ===== WIFI =====
const char *ssid = "Tran Thi Kim Loan";
const char *password = "01229333995";

// ===== PIN =====
#define DHT_PIN D5
#define DHT_TYPE DHT11
#define MOTOR_PIN1 D1
#define MOTOR_PIN2 D2
#define SPEED_PIN D4

// ===== OBJECT DHT =====
DHT dht(DHT_PIN, DHT_TYPE);

// ===== MOTOR STATE =====
AsyncWebServer server(80);
String motorState = "Stopped";
int motorSpeed = 0;       // 0-255
int motorDirection = 0;   // 0=Stop,1=CW,2=CCW

// ===== SENSOR DATA =====
float temperature = 0;
float humidity = 0;

// ===== READ SENSOR =====
void readDHT() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    temperature = 0;
    humidity = 0;
  }
}

// ===== HTML =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DC Motor & DHT Sensor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>DHT Sensor & Motor Control</h2>
  <p>Temperature: <span id="temp">%TEMP%</span> °C</p>
  <p>Humidity: <span id="hum">%HUM%</span> %</p>
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
    document.getElementById("motorForm").onsubmit = e => {
      e.preventDefault();
      fetch("/", { method:"POST", body:new FormData(e.target) });
    };

    setInterval(() => {
      fetch("/status")
        .then(r=>r.json())
        .then(d=>{
          document.getElementById("temp").innerHTML = d.temperature;
          document.getElementById("hum").innerHTML = d.humidity;
          document.getElementById("direction").innerHTML = d.direction;
          document.getElementById("speed").innerHTML = d.speed;
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

// ===== HTML PROCESSOR =====
String processor(const String &var) {
  if(var=="TEMP") return String(temperature);
  if(var=="HUM") return String(humidity);
  if(var=="DIRECTION") return motorState;
  if(var=="SPEED") return String(motorSpeed);
  return String();
}

// ===== WIFI =====
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: "+WiFi.localIP().toString());
}

// ===== MOTOR CONTROL =====
void updateMotor() {
  switch(motorDirection){
    case 0: motorState="Stopped"; digitalWrite(MOTOR_PIN1,LOW); digitalWrite(MOTOR_PIN2,LOW); break;
    case 1: motorState="Clockwise"; digitalWrite(MOTOR_PIN1,HIGH); digitalWrite(MOTOR_PIN2,LOW); break;
    case 2: motorState="Counter Clockwise"; digitalWrite(MOTOR_PIN1,LOW); digitalWrite(MOTOR_PIN2,HIGH); break;
  }
  analogWrite(SPEED_PIN,motorSpeed);
}

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(MOTOR_PIN1,OUTPUT);
  pinMode(MOTOR_PIN2,OUTPUT);
  pinMode(SPEED_PIN,OUTPUT);

  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req){
    req->send_P(200,"text/html",index_html,processor);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req){
    String json="{\"temperature\":\""+String(temperature)+"\",\"humidity\":\""+String(humidity)+"\",\"direction\":\""+motorState+"\",\"speed\":\""+String(motorSpeed)+"\"}";
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
void loop() {
  static unsigned long lastRead=0;
  const unsigned long interval=2000;

  // Đọc DHT mỗi 2 giây
  if(millis()-lastRead>=interval){
    lastRead=millis();
    readDHT();
    Serial.print("Temp: "); Serial.print(temperature);
    Serial.print(" °C | Humidity: "); Serial.print(humidity);
    Serial.print(" % | Motor: "); Serial.print(motorState);
    Serial.print(" | Speed: "); Serial.println(motorSpeed);
  }

  // Cập nhật motor từ web
  updateMotor();
}


