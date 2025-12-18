#include <Stepper.h>

// ===== CẢM BIẾN GAS =====
#define GAS_PIN A0

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
  myStepper.setSpeed(15);
  Serial.println("He thong san sang...");
}

void loop() {
  int gasValue = analogRead(GAS_PIN);
  Serial.print("Gia tri gas: ");
  Serial.println(gasValue);

  if (gasValue > 200) {
    Serial.println("Gas cao -> Quay +90 do");
    myStepper.step(degreeToSteps(90));
    delay(500);
  } 
  else {
    Serial.println("Gas thap -> Quay -90 do");
    myStepper.step(degreeToSteps(-90));
    delay(500);
  }

  delay(500);
}
