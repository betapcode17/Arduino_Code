// ========================
// 🔹 KHAI BÁO CHO STEPPER MOTOR
// ========================
int motorPin1 = 2;
int motorPin2 = 3;
int motorPin3 = 4;
int motorPin4 = 5;
int delayTime = 3;          // 🔥 Thời gian giữa các bước (càng nhỏ -> quay nhanh)
int stepsPer360deg = 512;  // 360° = 2048 bước (động cơ 28BYJ-48)

// 🔹 CẢM BIẾN DÒ LINE
int sensor = 8;             // Chân D0 cảm biến dò line

// ========================
// ⚙️ SETUP
// ========================
void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

// ========================
// 🔁 LOOP CHÍNH
// ========================
void loop() {
  int lineState = digitalRead(sensor); // đọc tín hiệu cảm biến

  if (lineState == LOW) {
    // Nếu trắng có che
    Serial.println("line trắng- Quay nghịch");
    for (int i = 0; i < stepsPer360deg / 12; i++) { // quay nhanh 30°
      direction1();
    }
    delay(800); // 🕒 Dừng lại 0.8 giây để quan sát rõ
  } else {
    // Nếu đen không che
    Serial.println(" line đen - Quay thuận");
    for (int i = 0; i < stepsPer360deg / 4; i++) { // quay nhanh 90
      direction2();
    }
    delay(800); // 🕒 Dừng lại 0.8 giây để quan sát rõ
  }

  delay(100); // nghỉ ngắn rồi đọc lại cảm biến
}

// ========================
// 🔸 CÁC HÀM ĐIỀU KHIỂN STEPPER MOTOR
// ========================

void direction1() {  // Quay thuận
  A();
  AA();
  B();
  BB();
}

void direction2() {  // Quay ngược
  BB();
  B();
  AA();
  A();
}

void A() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
}

void AA() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
}

void B() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(delayTime);
}

void BB() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(delayTime);
}
