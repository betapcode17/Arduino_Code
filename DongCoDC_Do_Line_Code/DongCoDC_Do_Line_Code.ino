int in1 = 7;    // IN1 driver
int in2 = 6;    // IN2 driver
int e = 9;      // ENA (PWM)
int sensor = 8; // Chân D0 của cảm biến dò line (D0 pin of the line sensor)

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(sensor, INPUT); // Cảm biến là INPUT (Sensor is INPUT)
  Serial.begin(9600);
}

void loop() {
  int lineState = digitalRead(sensor);

  if (lineState == LOW) {
    // Khi cảm biến phát hiện vạch đen (LOW) (When the sensor detects a black line)
    Serial.println("Phat hien vach den - dung lai"); // (Detects black line - stop)
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(e, 0); // Dừng động cơ (Stop motor)
  } else {
    // Khi không phát hiện vạch đen (HIGH) (When not detecting a black line)
    Serial.println("Nen trang - quay thuan"); // (White surface - forward rotation)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(e, 200); // Chạy động cơ (Run motor)
  }

  delay(100);
}