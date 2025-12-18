// ===== JOYSTICK =====
int JoyStickX = A1;
int JoyStickY = A2;
int JoyStickZ = 4;

// ===== MOTOR A =====
int enableA = 3;
int input1 = 5;
int input2 = 6;

// ===== NGƯỠNG JOYSTICK =====
int center = 512;
int deadZone = 100;

void setup() {
  pinMode(enableA, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);

  pinMode(JoyStickZ, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  int x = analogRead(JoyStickX);
  int y = analogRead(JoyStickY);
  int z = digitalRead(JoyStickZ);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | Z: ");
  Serial.println(z);

  // ===== ĐIỀU KHIỂN ĐỘNG CƠ THEO TRỤC Y =====
  if (y > center + deadZone) {
    // Quay thuận
    analogWrite(enableA, 200);
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
  }
  else if (y < center - deadZone) {
    // Quay nghịch
    analogWrite(enableA, 200);
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
  }
  else {
    // Dừng động cơ
    analogWrite(enableA, 0);
    digitalWrite(input1, LOW);
    digitalWrite(input2, LOW);
  }

  delay(100);
}
