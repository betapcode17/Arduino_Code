#include <Stepper.h>

// ===== CẢM BIẾN LINE =====
#define LINE_PIN 2

// ===== ĐỘNG CƠ BƯỚC =====
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 12

const int STEPS_PER_REV = 2048; // 28BYJ-48

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

// Hàm đổi độ sang bước
int degreeToSteps(int degree) {
  return (long)degree * STEPS_PER_REV / 360;
}

void setup() {
  Serial.begin(9600);

  pinMode(LINE_PIN, INPUT);

  myStepper.setSpeed(15); // tốc độ motor
  Serial.println("He thong san sang...");
}

void loop() {
  int val = digitalRead(LINE_PIN);

  if (val == LOW) { // Black
    Serial.println("Black -> Quay +90 do");
    myStepper.step(degreeToSteps(90));
    delay(500);
  } 
  else { // White
    Serial.println("White -> Quay -90 do");
    myStepper.step(degreeToSteps(-90));
    delay(500);
  }
}
