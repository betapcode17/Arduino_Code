#include <Servo.h>

// ===== JOYSTICK =====
#define JOY_X A1
#define JOY_Y A2
#define JOY_Z 4   // nut nhan

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;

// Hàm kiểm tra joystick
bool joystick(int thresholdX = 550, int thresholdY = 550) {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);
  int z = digitalRead(JOY_Z);

  Serial.print("JOYSTICK: ");
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
  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("Joystick + Servo san sang...");
}

void loop() {
  if (joystick()) {
    Serial.println("Joystick ON -> Servo 180");
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  } else {
    Serial.println("Joystick OFF -> Servo 90");
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
  }

  delay(300);
}
