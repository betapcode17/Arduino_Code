#include <Servo.h>
#include <DHT.h>

// ===== DHT =====
#define DHTPIN 4
#define DHTTYPE DHT11     // nếu dùng DHT22 thì đổi thành DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== SERVO =====
#define SERVO_PIN 9
Servo myservo;


#define HUMI_THRESHOLD 50

void setup() {
  Serial.begin(9600);
  dht.begin();

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Serial.println("DHT + Servo san sang...");
}

void loop() {
  delay(1000); // DHT đọc chậm

  float tempC = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(tempC) || isnan(humidity)) {
    Serial.println("Loi doc DHT!");
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.print("Do am: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (humidity > HUMI_THRESHOLD) {
    Serial.println("Nong + Am cao -> Servo 180");
    myservo.write(180);
    delay(200);
    myservo.write(0);
  } else {
    Serial.println("Dieu kien binh thuong -> Servo 90");
    myservo.write(90);
    delay(200);
    myservo.write(0);
  }

  delay(200);
}
