#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);



const int MIN_SPEED = 100; // Tốc độ thấp nhất khi hoạt động (Minimum speed when active)
const int MAX_SPEED = 255; // Tốc độ cao nhất (Maximum speed)
const int HUM_THRESHOLD = 30; // Ngưỡng độ ẩm để bật động cơ (Humidity threshold to turn on the motor)

void setup() {
  Serial.begin(9600);
  dht.begin(); // Khởi động cảm biến (Initialize the sensor)

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
}

void setMotorSpeed(int speed) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, speed);
}

void stopMotor() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // / °C
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Lỗi đọc cảm biến DHT!"); // (Error reading DHT sensor!)
    delay(2000);
    return;
  }
  
  // In thông tin cảm biến (Print sensor information)
  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print(" °C | Do am: ");
  Serial.print(h);
  Serial.print(" %");

  if (h > HUM_THRESHOLD) {
    // Map độ ẩm (từ ngưỡng đến 90%) sang tốc độ (từ MIN_SPEED đến MAX_SPEED)
    int speed = map(h, HUM_THRESHOLD, 90, MIN_SPEED, MAX_SPEED);
    setMotorSpeed(speed);
    Serial.print(" | Dong co hoat dong - Toc do: "); // (Motor running - Speed:)
    Serial.println(speed);
  } else {
    stopMotor();
    Serial.println(" | Do am thap - Dong co dung"); // (Low humidity - Motor stopped)
  }

  delay(2000);
}