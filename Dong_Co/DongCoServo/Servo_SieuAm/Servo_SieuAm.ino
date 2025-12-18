#include <Servo.h>

// ===== HC-SR04 =====
#define TRIG_PIN 6
#define ECHO_PIN 5
#define DIST_THRESHOLD 20   // cm

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("HC-SR04 + Servo san sang...");
}

void loop() {
  long duration;
  int distance;

  // Phat xung sieu am
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Doc tin hieu phan xa (timeout 30ms ~ 5m)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.println("Khong co tin hieu!");
    return;
  }

  distance = duration * 0.034 / 2;

  Serial.print("Khoang cach: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < DIST_THRESHOLD) {
    Serial.println("Vat gan -> Servo 180");
    myservo.write(180);
    delay(500);
    myservo.write(0);
    delay(500);
  } else {
    Serial.println("Vat xa -> Servo 90");
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
  }

  delay(500);
}
