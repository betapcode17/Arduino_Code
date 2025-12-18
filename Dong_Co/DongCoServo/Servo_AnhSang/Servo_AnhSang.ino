#include <Servo.h>

// ===== CẢM BIẾN ÁNH SÁNG =====
#define LIGHT_PIN A0
#define LIGHT_THRESHOLD 800   // chỉnh theo môi trường

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("LDR + Servo san sang...");
}

void loop() {
  int lightValue = analogRead(LIGHT_PIN);

  Serial.print("Gia tri anh sang: ");
  Serial.println(lightValue);

  if (lightValue > LIGHT_THRESHOLD) {
    Serial.println("Sang manh -> Servo 180");
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  } else {
    Serial.println("Sang yeu -> Servo 90");
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
  }

  delay(500);
}
