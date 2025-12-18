#include <Stepper.h>
#include <DHT.h>

// ===== DHT11 =====
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== ĐỘNG CƠ BƯỚC =====
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 12

const int STEPS_PER_REV = 2048; // 28BYJ-48

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

// đổi độ sang bước
int degreeToSteps(int degree) {
  return (long)degree * STEPS_PER_REV / 360;
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  myStepper.setSpeed(15);
  Serial.println("He thong DHT + Stepper san sang...");
}

void loop() {
  delay(1000); // DHT11 đọc chậm

  float tempC = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(tempC) || isnan(humidity)) {
    Serial.println("Loi doc DHT11!");
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.print("Do am: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (tempC > 28 && humidity > 80) {
    Serial.println("Nong + Am cao -> Quay +90 do");
    myStepper.step(degreeToSteps(90));
    delay(500);
  } 
  else {
    Serial.println("Dieu kien binh thuong -> Quay -90 do");
    myStepper.step(degreeToSteps(-90));
    delay(500);
  }

  delay(1000);
}
