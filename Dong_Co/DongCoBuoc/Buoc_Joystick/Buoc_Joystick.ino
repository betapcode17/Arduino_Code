#include <Stepper.h>

// ===== JOYSTICK =====
#define JOY_X A1
#define JOY_Y A2
#define JOY_Z 4   // nut nhan

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

// hàm kiểm tra joystick
bool joystick(int thresholdX = 550, int thresholdY = 550) {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);
  int z = digitalRead(JOY_Z);

  Serial.print("Joystick: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);

  if (x >= thresholdX || y >= thresholdY || z == LOW) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(JOY_Z, INPUT_PULLUP);

  myStepper.setSpeed(15);
  Serial.println("Joystick + Stepper san sang...");
}

void loop() {
  if (joystick()) {
    Serial.println("Joystick hoat dong -> Quay +90 do");
    myStepper.step(degreeToSteps(90));
    delay(300);
  } else {
    Serial.println("Joystick nghi -> Quay -90 do");
    myStepper.step(degreeToSteps(-90));
    delay(300);
  }

  delay(500);
}
