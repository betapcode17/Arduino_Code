#include <Servo.h>

// ===== CẢM BIẾN DÒ LINE =====
#define LINE_PIN 2   // chan DO

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;

void setup() {
  Serial.begin(9600);

  pinMode(LINE_PIN, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("Line sensor + Servo san sang...");
}

void loop() {
  int lineValue = digitalRead(LINE_PIN);

  Serial.print("Gia tri line: ");
  Serial.println(lineValue); // 0 = den, 1 = trang (thuong gap)

  if (lineValue == LOW) {
    Serial.println("Gap LINE DEN -> Servo 180");
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  } else {
    Serial.println("Line TRANG -> Servo 90");
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
  }

  delay(300);
}
