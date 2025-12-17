#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "<3";
const char* password = "baophansayhi";

ESP8266WebServer server(80);

const int ledPin = D1;

bool ledState = LOW;

// --- HTML, CSS, VÀ JAVASCRIPT CHO TRANG WEB ---
String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP8266 LED Control</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0; }
        .container { max-width: 400px; margin: 20px auto; padding: 20px; background-color: #fff; border-radius: 8px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }
        h1 { color: #333; }
        button { padding: 15px; font-size: 18px; margin: 10px; border-radius: 5px; border: none; cursor: pointer; width: 100px; }
        #btn-on { background-color: #28a745; color: white; }
        #btn-on:hover { background-color: #218838; }
        #btn-off { background-color: #dc3545; color: white; }
        #btn-off:hover { background-color: #c82333; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Bộ điều khiển đèn LED</h1>
        <button id="btn-on" onclick="toggleLED('on')">BẬT</button>
        <button id="btn-off" onclick="toggleLED('off')">TẮT</button>
    </div>

    <script>
        // Hàm bật/tắt LED
        function toggleLED(state) {
            fetch('/led?state=' + state)
                .then(response => response.text())
                .then(data => console.log('LED:', data));
        }
    </script>
</body>
</html>
)rawliteral";
// --------------------------------------------------

// HÀM XỬ LÝ CHO SETUP()
void setup() {
  Serial.begin(115200);
  
  // Cài đặt chân
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState); // Bắt đầu ở trạng thái TẮT

  // Kết nối Wi-Fi
  Serial.print("Dang ket noi den ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nDa ket noi Wi-Fi!");
  Serial.print("Dia chi IP: ");
  Serial.println(WiFi.localIP());

  // --- ĐỊNH NGHĨA CÁC ĐƯỜNG DẪN (ROUTES) CỦA WEB SERVER ---

  // 1. Đường dẫn trang chủ ("/")
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // 2. Đường dẫn điều khiển LED ("/led")
  // Nhận yêu cầu như: /led?state=on
  server.on("/led", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      if (state == "on") {
        digitalWrite(ledPin, HIGH);
        ledState = HIGH;
        server.send(200, "text/plain", "Da bat LED");
      } else {
        digitalWrite(ledPin, LOW);
        ledState = LOW;
        server.send(200, "text/plain", "Da tat LED");
      }
    } else {
      server.send(400, "text/plain", "Thieu tham so 'state'");
    }
  });

  // Bắt đầu máy chủ
  server.begin();
  Serial.println("Web Server da khoi dong!");
}

// HÀM XỬ LÝ CHO LOOP()
void loop() {
  server.handleClient(); // Luôn lắng nghe các yêu cầu từ client
}