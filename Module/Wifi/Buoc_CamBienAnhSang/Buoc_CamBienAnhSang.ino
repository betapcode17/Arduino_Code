// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Stepper.h>

// // Thông tin WiFi
// const char *ssid = "Tran Thi Kim Loan";
// const char *password = "01229333995";

// // Định nghĩa chân cho động cơ bước và cảm biến ánh sáng
// #define lightSensor A0
// #define IN1 D1
// #define IN2 D2
// #define IN3 D3
// #define IN4 D4

// // Các biến cho động cơ bước
// const int stepperRevolution = 2048; // Số bước trong một vòng quay
// Stepper myStepper(stepperRevolution, IN1, IN3, IN2, IN4);

// // Thông số và trạng thái của động cơ
// int steps = 0;
// String direction = "Stopped";  // Chiều quay ban đầu là dừng

// // Tạo web server
// AsyncWebServer server(80);

// // Đọc giá trị từ cảm biến ánh sáng (LDR)
// int readLightSensor() {
//   return analogRead(lightSensor);  // Đọc giá trị từ cảm biến ánh sáng
// }

// // HTML giao diện web
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Stepper Motor Control</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
// </head>
// <body>
//   <h1>Stepper Motor Control</h1>
//   <p>Light Sensor Value: <span id="light">%LIGHT%</span></p>
//   <p>Motor Direction: <span id="direction">%DIRECTION%</span></p>
//   <form id="motorForm">
//     <label for="steps">Number of steps (0-360): </label>
//     <input type="number" name="steps" min="0" max="360" required>
//     <input type="submit" value="GO!">
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
//           document.getElementById("light").innerHTML = data.light;
//           document.getElementById("direction").innerHTML = data.direction;
//         })
//         .catch(error => console.error("Error:", error));
//     }, 2000);
//   </script>
// </body>
// </html>
// )rawliteral";

// // Hàm xử lý HTML
// String processor(const String &var) {
//   if (var == "LIGHT") {
//     return String(readLightSensor());  // Hiển thị giá trị cảm biến ánh sáng
//   } else if (var == "DIRECTION") {
//     return direction;  // Hiển thị chiều quay của động cơ
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

//   // Khởi tạo kết nối WiFi
//   initWiFi();

//   // Khởi tạo chân cảm biến ánh sáng
//   pinMode(lightSensor, INPUT);

//   // Cấu hình động cơ bước
//   myStepper.setSpeed(10);

//   // Web server routes
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send_P(200, "text/html", index_html, processor);
//   });

//   server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
//     String jsonResponse = "{\"light\":\"" + String(readLightSensor()) + "\",\"direction\":\"" + direction + "\"}";
//     request->send(200, "application/json", jsonResponse);
//   });

//   server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
//     if (request->hasParam("steps", true)) {
//       steps = request->getParam("steps", true)->value().toInt();
//       Serial.print("Number of steps set to: ");
//       Serial.println(steps);
//     }
//     request->send(200, "text/plain", "Settings updated");
//   });

//   server.begin();
// }

// void loop() {
//   static String prevDirection = "";   // Lưu chiều quay trước đó

//   // Đọc giá trị cảm biến ánh sáng
//   int lightValue = readLightSensor();
  
//   // Điều chỉnh chiều quay của động cơ dựa trên giá trị cảm biến ánh sáng
//   if (lightValue > 800) {
//     direction = "Counterclockwise";  // Quay ngược khi ánh sáng yếu
//   } else {
//     direction = "Clockwise";         // Quay thuận khi ánh sáng mạnh
//   }

//   // In thông tin thay đổi nếu có
//   if (direction != prevDirection) {
//     prevDirection = direction;
//     Serial.print("Light Sensor Value: ");
//     Serial.println(lightValue);
//     Serial.print("Motor Direction: ");
//     Serial.println(direction);
//     Serial.println();
//   }

//   // Xử lý chuyển động của động cơ
//   if (steps > 0) {
//     int totalSteps = map(steps, 0, 360, 0, stepperRevolution);
//     for (int i = 0; i < totalSteps; i++) {
//         myStepper.step(direction == "Clockwise" ? 1 : -1);
//         yield();  // Nhường CPU cho các tác vụ khác
//         ESP.wdtFeed();
//     }
//   }
  
//   delay(500);  // Thêm một chút độ trễ để tránh quá tải
// }













#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

// ================= WIFI =================
const char *ssid = "Tran Thi Kim Loan";
const char *password = "01229333995";

// ================= PHẦN CỨNG =================
#define lightSensor A0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

// ================= ĐỘNG CƠ BƯỚC =================
const int STEPS_PER_REV = 2048;
Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

int stepsToMove = 0;          // Số bước cần quay
String motorDirection = "Stopped";

// ================= WEB SERVER =================
AsyncWebServer server(80);

// ================= HÀM ĐỌC CẢM BIẾN =================
int readLightSensor() {
  return analogRead(lightSensor);
}

// ================= HTML =================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Stepper Control</title>
</head>
<body>
  <h2>Stepper Motor Control</h2>

  <p>Light Sensor: <span id="light">0</span></p>
  <p>Motor Direction: <span id="direction">Stopped</span></p>

  <form id="form">
    <input type="number" name="steps" placeholder="Steps (+/-)" required>
    <button type="submit">GO</button>
  </form>

  <script>
    document.getElementById("form").onsubmit = function(e) {
      e.preventDefault();
      fetch("/", {
        method: "POST",
        body: new FormData(this)
      });
    };

    setInterval(() => {
      fetch("/status")
        .then(r => r.json())
        .then(d => {
          document.getElementById("light").innerHTML = d.light;
          document.getElementById("direction").innerHTML = d.direction;
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  myStepper.setSpeed(10);

  // Trang chính
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Nhận lệnh động cơ
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("steps", true)) {
      stepsToMove = request->getParam("steps", true)->value().toInt();
      motorDirection = (stepsToMove > 0) ? "Clockwise" : "Counterclockwise";

      Serial.print("Steps: ");
      Serial.println(stepsToMove);
    }
    request->send(200, "text/plain", "OK");
  });

  // Trạng thái
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"light\":" + String(readLightSensor()) + ",";
    json += "\"direction\":\"" + motorDirection + "\"}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

// ================= LOOP =================
void loop() {
  // 👉 CẢM BIẾN HOẠT ĐỘNG ĐỘC LẬP
  int lightValue = readLightSensor();
  Serial.print("Light: ");
  Serial.println(lightValue);

  // 👉 ĐỘNG CƠ CHỈ QUAY KHI CÓ LỆNH
  if (stepsToMove != 0) {
    myStepper.step(stepsToMove);
    stepsToMove = 0;
    motorDirection = "Stopped";
  }

  delay(300);
}






