#include <Servo.h>

// ===== CẢM BIẾN GAS =====
#define GAS_PIN A0
#define GAS_THRESHOLD 200   // chỉnh theo thực tế

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("Gas + Servo san sang...");
}

void loop() {
  int gasValue = analogRead(GAS_PIN);

  Serial.print("Gia tri gas: ");
  Serial.println(gasValue);

  if (gasValue > GAS_THRESHOLD) {
    Serial.println("Gas cao -> Servo 180");
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  } else {
    Serial.println("Gas thap -> Servo 90");
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
  }

  delay(500);
}
