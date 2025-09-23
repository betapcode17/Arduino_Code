#include <DHT.h>
#include <Arduino.h>
#include <math.h>

#define DHT11_PIN 4        // Chân dữ liệu DHT11
#define DHTTYPE DHT11      // Loại cảm biến DHT11
#define BUZZER_PIN 8       // Chân buzzer

DHT dht(DHT11_PIN, DHTTYPE);

// Biến toàn cục
float sinVal;             
int toneVal;              
unsigned long tepTimer = 0;  

void setup() {
  Serial.begin(9600);
  dht.begin();

  Serial.println("DHT11 TEST PROGRAM");
  Serial.println("Type\tStatus\tHumidity (%)\tTemperature (C)");
}

void loop() {
  // đọc dữ liệu mỗi 500 ms
  delay(500);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("DHT11\tOK\t");
  Serial.print(h);
  Serial.print("\t");
  Serial.print(t);
  Serial.println("°C");

  // Nếu nhiệt độ > 27°C thì bật buzzer dạng sóng giả sin
  if (h > 70) {
    for (int x = 0; x < 180; x++) {
      sinVal = sin(x * (3.1412 / 180));    
      toneVal = 2000 + int(sinVal * 1000);
      tone(BUZZER_PIN, toneVal);              
      delay(2);                              
    }
  } else {
    noTone(BUZZER_PIN);            
  }

  // In lại dữ liệu sau mỗi 500ms
  if (millis() - tepTimer > 500) {
    tepTimer = millis();
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" C");
  }
}
