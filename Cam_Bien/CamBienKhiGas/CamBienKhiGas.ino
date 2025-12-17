#include <math.h>          // Thư viện toán học để dùng sin()

int gasPin = A0;           // MQ-2 nối vào chân analog A0
int buzzerPin = 4;         // Buzzer nối vào chân số 4

void setup() {
  pinMode(buzzerPin, OUTPUT);   // Đặt chân buzzerPin làm ngõ ra
  Serial.begin(9600);           // Bật Serial Monitor ở tốc độ 9600 để xem dữ liệu
}

void loop() {
  int gasValue = analogRead(gasPin); // Đọc giá trị analog từ cảm biến MQ-2 (0 đến 1023)

  Serial.println(gasValue);          // In giá trị cảm biến ra Serial Monitor để quan sát

  if (gasValue >600) {
    // Tạo hiệu ứng âm thanh với tần số thay đổi dạng sin
    for (int x = 0; x < 180; x++) {
      float s = sin(x * PI / 180);    
      int val = 2000 + int(1000 * s); // Tần số dao động từ 1000Hz -> 3000Hz
      tone(buzzerPin, val);
      delay(5);
    }
    // Nếu muốn buzzer dừng sau vòng for thì bật dòng này:
    // noTone(buzzerPin);
  } else {
    noTone(buzzerPin); // Tắt buzzer khi an toàn
  }

  delay(100); // delay 100ms
}
