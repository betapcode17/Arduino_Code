#include <Servo.h>

#define TRIG 11
#define ECHO 10

Servo myservo;

int pos = 0;               // Góc hiện tại
bool increasing = true;    // Hướng quay
unsigned long previousMillis = 0;
int speed = 10;            // tốc độ quay (ms mỗi bước)
int maxAngle = 90;         // góc tối đa mặc định
bool paused = false;       // trạng thái đang tạm dừng
unsigned long pauseStart = 0; // thời gian bắt đầu dừng

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  float dist = getDist();
  Serial.println(dist);

  // Nếu có vật gần (<= 20cm) thì quay 0↔180, nếu không thì 0↔90
  if (dist <= 20)
    maxAngle = 180;
  else
    maxAngle = 90;

  unsigned long currentMillis = millis();

  // Nếu đang tạm dừng thì kiểm tra đã hết 500ms chưa
  if (paused) {
    if (currentMillis - pauseStart >= 500) {
      paused = false;  // Hết thời gian dừng -> tiếp tục quay
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
        increasing = false;   // Đổi hướng
        paused = true;        // Tạm dừng 500ms
        pauseStart = millis();
      }
    } else {
      pos -= 5;
      if (pos <= 0) {
        pos = 0;
        increasing = true;    // Đổi hướng
        paused = true;        // Tạm dừng 500ms
        pauseStart = millis();
      }
    }

    myservo.write(pos);
  }
}

// Hàm đo khoảng cách bằng cảm biến siêu âm
float getDist() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2; // đổi thời gian sang cm
}
