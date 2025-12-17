#include <DHT.h>

// ========================
// 🔹 KHAI BÁO CẢM BIẾN DHT11
// ========================
#define DHTPIN 8        // Chân kết nối DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ========================
// 🔹 KHAI BÁO CHO STEPPER MOTOR (28BYJ-48)
// ========================
int motorPin1 = 2;
int motorPin2 = 3;
int motorPin3 = 4;
int motorPin4 = 5;
int delayTime = 3;          // 🔥 Thời gian giữa các bước (càng nhỏ -> quay nhanh)
int stepsPer360deg = 512;   // 360° = 512 bước (hoặc 2048 tùy module)

// ========================
// 🔹 NGƯỠNG CẢM BIẾN
// ========================
const int HUM_THRESHOLD = 50;   // Ngưỡng độ ẩm (bật quay thuận)
const int TEMP_THRESHOLD = 30;  // Ngưỡng nhiệt độ (bật quay nghịch)

// ========================
// ⚙️ SETUP
// ========================
void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.println("=== HE THONG DIEU KHIEN BANG DHT11 ===");
}

// ========================
// 🔁 LOOP CHÍNH
// ========================
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // °C

  if (isnan(h) || isnan(t)) {
    Serial.println("⚠️ Loi doc cam bien DHT!");
    delay(2000);
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print(" °C | Do am: ");
  Serial.print(h);
  Serial.println(" %");

  // --- Điều khiển ---
  if (h > HUM_THRESHOLD) {
    Serial.println("💧 Do am cao -> Quay thuận");
    for (int i = 0; i < stepsPer360deg / 4; i++) { // quay 90°
      quayThuan();
    }
  } else if (t > TEMP_THRESHOLD) {
    Serial.println("🔥 Nhiet do cao -> Quay nghich");
    for (int i = 0; i < stepsPer360deg / 4; i++) { // quay 90°
      quayNghich();
    }
  } else {
    Serial.println("✅ Dieu kien on dinh - Dung dong co");
    stopMotor();
  }

  delay(2000);
}

// ========================
// 🔸 CÁC HÀM ĐIỀU KHIỂN STEPPER MOTOR
// ========================
void quayThuan() {
  A(); AA(); B(); BB();
}

void quayNghich() {
  BB(); B(); AA(); A();
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
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
