#include <Stepper.h>

// ===== HC-SR04 =====
#define TRIG_PIN 6
#define ECHO_PIN 5

// ===== ĐỘNG CƠ BƯỚC =====
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 12

const int STEPS_PER_REV = 2048; // 28BYJ-48

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

// đổi độ sang bước
int degreeToSteps(int degree) {
  return (long)degree * STEPS_PER_REV / 360;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myStepper.setSpeed(15);
  Serial.println("He thong san sang...");
}

void loop() {
  long duration;
  int distance;

  // Phát xung siêu âm
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Đo thời gian phản xạ
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  distance = duration * 0.034 / 2;

  Serial.print("Khoang cach: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 20) {
    Serial.println("Vat gan -> Quay nguoc chieu 90 do");
    myStepper.step(degreeToSteps(90));
    delay(500);
  } 
  else {
    Serial.println("Vat xa -> Quay cung chieu 90 do");
    myStepper.step(degreeToSteps(-90));
    delay(500);
  }

  delay(500);
}
