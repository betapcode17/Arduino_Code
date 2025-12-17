// Chân kết nối
const int ALARM = 4;       // Chân digital số 4 điều khiển buzzer
const int LM35_PIN = A0;   // Chân analog A0 đọc tín hiệu LM35

// Biến
float tmpVal = 0;          // Biến lưu nhiệt độ
float sinVal;              // Biến trung gian tính sin
int toneVal;               // Biến lưu tần số âm thanh buzzer
unsigned long tepTimer;    // Biến đếm thời gian

void setup() {
    Serial.begin(9600);         // Khởi tạo Serial với tốc độ 9600 bps
    pinMode(ALARM, OUTPUT);     // Chân buzzer là OUTPUT
}

void loop() {
    // 1️⃣ Đọc giá trị analog từ LM35
    int val = analogRead(LM35_PIN);              // Giá trị 0 - 1023
    float voltage = val * (5.0 / 1023.0);        // Chuyển sang V
    tmpVal = voltage * 100;                      // Chuyển sang °C (10mV = 1°C)

    // 2️⃣ Kiểm tra nhiệt độ và điều khiển buzzer
    if (tmpVal > 27) {
        // Nếu nhiệt độ > 27°C, phát âm thanh dạng sóng sin
        for (int x = 0; x < 180; x++) {
            sinVal = sin(x * (3.1412 / 180));    // Chuyển độ sang radian
            toneVal = 2000 + (int(sinVal * 1000));
            tone(ALARM, toneVal);               // Phát âm thanh
            delay(2);                            // Trì hoãn để tạo sóng
        }
    } else {
        noTone(ALARM);                           // Nếu <=27°C, tắt buzzer
    }

    // 3️⃣ In nhiệt độ ra Serial mỗi 500ms
    if (millis() - tepTimer > 50) {
        tepTimer = millis();
        Serial.print("Temperature: ");
        Serial.print(tmpVal);
        Serial.println(" C");
    }
}
