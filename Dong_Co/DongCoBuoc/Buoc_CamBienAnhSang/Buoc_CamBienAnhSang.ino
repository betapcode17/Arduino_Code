#include <Stepper.h>

// ===== CẢM BIẾN ÁNH SÁNG =====
#define LIGHT_PIN 3

// ===== ĐỘNG CƠ BƯỚC =====
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 12

const int STEPS_PER_REV = 2048; // 28BYJ-48

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

// đổi độ sang số bước
int degreeToSteps(int degree) {
  return (long)degree * STEPS_PER_REV / 360;
}

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(15);
  Serial.println("He thong san sang...");
}

void loop() {
  int lightValue = analogRead(LIGHT_PIN);
  Serial.print("Gia tri anh sang: ");
  Serial.println(lightValue);

  if (lightValue > 700) {
    Serial.println("Sang manh -> Quay +90 do");
    myStepper.step(degreeToSteps(90));
    delay(500);
  } 
  else {
    Serial.println("Sang yeu -> Quay -90 do");
    myStepper.step(degreeToSteps(-90));
    delay(500);
  }

  delay(500);
}
