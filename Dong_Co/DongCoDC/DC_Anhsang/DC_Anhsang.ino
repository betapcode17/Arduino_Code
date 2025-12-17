int IN1 = 6;
int IN2 = 5;
int ENB = 9;
int LIGHT = A0;
int val = 0;
int speedMotor = 0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Đọc giá trị ánh sáng (0-1023)
  val = analogRead(LIGHT);
  Serial.print("Light value: ");
  Serial.println(val);

  // Nếu ánh sáng mạnh hơn 500 -> chạy tốc độ tối đa
  if (val > 100) {
    speedMotor = 255;
  } 
  // Ngược lại -> chạy chậm hơn
  else {
    speedMotor = 10;
  }

  // Cho động cơ quay thuận
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Xuất tín hiệu PWM điều khiển tốc độ
  analogWrite(ENB, 10);

  // In ra tốc độ để kiểm tra
  Serial.print("Motor speed (PWM): ");
  Serial.println(10);

  delay(200);
}
