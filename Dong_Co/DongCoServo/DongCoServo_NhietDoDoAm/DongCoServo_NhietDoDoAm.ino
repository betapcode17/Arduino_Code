#include <Servo.h>
#include <DHT.h>

#define DHTPIN 4       // Cảm biến DHT nối chân số 4
#define DHTTYPE DHT11  // hoặc DHT22 nếu bạn dùng loại đó

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;

int pos = 0;                 // Góc hiện tại
bool increasing = true;      // Hướng quay
unsigned long previousMillis = 0;
int speed = 10;              // tốc độ quay (ms mỗi bước)
int maxAngle = 90;           // góc tối đa mặc định
bool paused = false;         // trạng thái đang tạm dừng
unsigned long pauseStart = 0; // thời gian bắt đầu dừng

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  dht.begin();  // Khởi tạo cảm biến DHT
}

void loop() {
  float h = dht.readHumidity();     // Đọc độ ẩm (%)
  float t = dht.readTemperature();  // Đọc nhiệt độ (°C)

  // Kiểm tra lỗi đọc dữ liệu
  if (isnan(h) || isnan(t)) {
    Serial.println("Lỗi đọc cảm biến DHT!");
    return;
  }

  Serial.print("Nhiệt độ: ");
  Serial.print(t);
  Serial.print(" °C | Độ ẩm: ");
  Serial.print(h);
  Serial.println(" %");

  // Điều kiện thay đổi góc quay
  // Ví dụ: nếu nhiệt độ > 30°C → servo quay 0↔180°
  //        nếu < 30°C → servo quay 0↔90°
  if (t > 30)
    maxAngle = 180;
  else
    maxAngle = 90;

  unsigned long currentMillis = millis();

  // Nếu đang tạm dừng thì kiểm tra đã hết 500ms chưa
  if (paused) {
    if (currentMillis - pauseStart >= 500) {
      paused = false;
    } else {
      return; // Đang dừng thì không quay
    }
  }

  // Cập nhật vị trí servo mỗi "speed" ms
  if (currentMillis - previousMillis >= speed) {
    previousMillis = currentMillis;

    if (increasing) {
      pos += 5;
      if (pos >= maxAngle) {
        pos = maxAngle;
        increasing = false;
        paused = true;
        pauseStart = millis();
      }
    } else {
      pos -= 5;
      if (pos <= 0) {
        pos = 0;
        increasing = true;
        paused = true;
        pauseStart = millis();
      }
    }

    myservo.write(pos);
  }
}
