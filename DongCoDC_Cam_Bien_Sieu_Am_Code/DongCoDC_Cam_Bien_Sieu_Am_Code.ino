// Chân điều khiển cảm biến siêu âm (Pins for ultrasonic sensor control)
#define TRIG_PIN 2
#define ECHO_PIN 3

// Chân điều khiển động cơ (Pins for motor control)
#define ENA_PIN 9
#define IN1_PIN 7
#define IN2_PIN 6

// Tốc độ nhỏ nhất (vật gần) (Minimum speed - near object)
const int MIN_SPEED = 50; 
// Tốc độ lớn nhất (vật xa) (Maximum speed - far object)
const int MAX_SPEED = 255; 
// cm – khoảng cách nhỏ nhất (vật quá gần) (Minimum distance - object too close)
const int MIN_DISTANCE = 10; 
// cm – khoảng cách lớn nhất để map tốc độ (Maximum distance to map speed)
const int MAX_DISTANCE = 100; 

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  Serial.begin(9600);
}

// setMotorSpeed (Thiết lập tốc độ động cơ)
void setMotorSpeed(int speed) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, speed);
}

// Hàm đo khoảng cách (trả về đơn vị cm) (Function to measure distance, returns in cm)
long getDistance() {
  // Gửi xung 10µs để kích hoạt TRIG (Send a 10µs pulse to trigger TRIG)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Đo thời gian phản hồi của xung (Measure the pulse echo time)
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Tính khoảng cách (cm) (Calculate distance in cm)
  // Distance = (Duration * Speed of Sound) / 2
  // Speed of Sound in cm/µs is approx 0.034
  long distance = duration * 0.034 / 2;

  return distance;
}

// Note: The loop() function is missing from the images, 
// but the core functions and setup are present. 
// You would typically call setMotorSpeed() and getDistance() inside the loop().




void loop() {
  long distance = getDistance();

  // Giới hạn giá trị trong phạm vi đo (Limit the value within the measurement range)
  if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

  // Map khoảng cách sang tốc độ (Map distance to speed)
  int speed = map(distance, MIN_DISTANCE, MAX_DISTANCE, MIN_SPEED, MAX_SPEED);

  // Điều khiển động cơ (Control the motor)
  setMotorSpeed(speed);

  // In thông tin ra Serial (Print information to Serial)
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Motor speed: ");
  Serial.println(speed);

  delay(200);
}