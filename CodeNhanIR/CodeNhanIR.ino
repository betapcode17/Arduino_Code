#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 2;   // Pin để nhận tín hiệu IR
const int LED_Pin = 9;          // Pin để điều khiển đèn LED

volatile bool ledShouldBeOn = false; // Biến để lưu trạng thái của LED (dựa trên tín hiệu nhận được)

void setup() {
  Serial.begin(9600);                      // Khởi tạo kết nối Serial
  pinMode(LED_Pin, OUTPUT);                // Thiết lập LED_Pin là ngõ ra
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu bộ nhận IR
}

void loop() {
  // Điều khiển LED dựa trên trạng thái của ledShouldBeOn
  Serial.println("waiting");
  if (ledShouldBeOn) {
    digitalWrite(LED_Pin, HIGH); // Giữ LED bật nếu ledShouldBeOn là true
    Serial.println("LED ON");
  } else {
    digitalWrite(LED_Pin, LOW);  // Tắt LED nếu ledShouldBeOn là false
    Serial.println("LED OFF");
  }

  delay(10);

  // Resume receiving IR signals
  if (IrReceiver.decode()) {
    Serial.println("Receive Signal!");
    handleReceivedIRData();  // Gọi hàm xử lý dữ liệu khi nhận được tín hiệu IR
    delay(5);
    IrReceiver.resume();     // Tiếp tục nhận tín hiệu IR
  }

  delay(100);
}

// Hàm xử lý dữ liệu khi nhận được tín hiệu IR
void handleReceivedIRData() {
  Serial.print("Mã IR nhận được: 0x");
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  Serial.print("Giao thức: ");
  Serial.println(IrReceiver.decodedIRData.protocol);

  // Nếu giá trị nhận được là 0x101, bật LED
  if (IrReceiver.decodedIRData.decodedRawData == 0x101) {
    ledShouldBeOn = true;  // Thiết lập trạng thái của LED là bật
    Serial.println("LED ON");
  } else {
    ledShouldBeOn = false; // Thiết lập trạng thái của LED là tắt cho giá trị khác
    Serial.println("LED OFF");
  }
}