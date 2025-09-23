const int trigPin = 9;     // Chân Trig
const int echoPin = 10;    // Chân Echo
const int buzzerPin = 4;   // Chân Buzzer

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // --- Đo khoảng cách ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Kiểm tra khoảng cách ---
  if (distance >= 10 && distance <= 90) {
    // Phát "tiếng hình sin" bằng cách quét tần số
    for (int x = 0; x < 180; x++) {
      float sinVal = sin(x * (3.1416 / 180.0));     // Giá trị sin -1..1
      int toneVal = 2000 + int(sinVal * 1000);      // Dao động 1000–3000 Hz
      tone(buzzerPin, toneVal);                     // Phát ra loa
      delay(2);                                     // Tốc độ quét
    }
  } else {
    noTone(buzzerPin); // Tắt loa khi không có vật trong khoảng 10–20 cm
  }

  delay(50); // Nghỉ 50ms trước khi đo lại
}