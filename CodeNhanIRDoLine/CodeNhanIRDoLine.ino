#include <IRremote.hpp>
#include <Servo.h>

Servo myservo;                     // Tạo đối tượng servo
const int IR_RECEIVE_PIN = 2;      // Chân nhận tín hiệu IR

volatile bool signalReceived = false; // Biến lưu trạng thái có tín hiệu IR hay không

void setup() {
    Serial.begin(9600);                           // Khởi tạo Serial
    myservo.attach(9);                            // Gắn servo vào chân 9
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu nhận tín hiệu IR
    Serial.println("System ready. Waiting for IR signal...");
}

void loop() {
    if (signalReceived) {
        // Nếu có tín hiệu IR
        Serial.println("Có tín hiệu IR -> Servo di chuyển 0° -> 180°");
        myservo.write(0);
        delay(500);
        myservo.write(180);
        delay(500);
        signalReceived = false; // Reset lại cờ sau khi xử lý xong
    } else {
        // Nếu KHÔNG có tín hiệu IR
        Serial.println("Không có tín hiệu IR -> Servo di chuyển 0° -> 90°");
        myservo.write(0);
        delay(500);
        myservo.write(90);
        delay(500);
    }

    // Kiểm tra nếu nhận được dữ liệu IR
    if (IrReceiver.decode()) {
        Serial.println("Đã nhận tín hiệu IR!");
        handleReceivedIRData();   // Gọi hàm xử lý dữ liệu IR
        IrReceiver.resume();      // Tiếp tục chờ tín hiệu tiếp theo
    }
    delay(200);
}

// Hàm xử lý dữ liệu IR khi nhận được
void handleReceivedIRData() {
    Serial.print("Mã IR nhận được: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print("Giao thức: ");
    Serial.println(IrReceiver.decodedIRData.protocol);

    // Nếu mã IR nhận được là 0x101 thì kích hoạt servo
    if (IrReceiver.decodedIRData.decodedRawData == 0x101) {
        signalReceived = true;
        Serial.println("Tín hiệu hợp lệ -> Servo kích hoạt!");
    } else {
        signalReceived = false;
        Serial.println("Tín hiệu khác -> Servo ở trạng thái chờ.");
    }
}