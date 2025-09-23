#include <Arduino.h>

int sensorPin = A0;   // chân analog kết nối tới cảm biến LM35
int ALARM = 4;       // chân kết nối loa/buzzer
float sinVal;
int toneVal;

void setup() {
  Serial.begin(9600);       // Khởi động Serial ở mức baudrate 9600
  pinMode(ALARM, OUTPUT);   // cấu hình chân buzzer là OUTPUT
}

void loop() {
  // Đọc giá trị từ cảm biến LM35
  int reading = analogRead(sensorPin);

  // Tính hiệu điện thế (Volt)
  float voltage = reading * 5.0 / 1024.0;

  // LM35: 10mV = 1°C → 1V = 100°C
  float temp = voltage * 100.0;

  if (temp > 30) {
    
    for (int x = 0; x < 180; x++) {
      sinVal = sin(x * (PI / 180.0));         
     toneVal = 500 + int(sinVal * 200);    
      tone(ALARM, toneVal);                  
      delay(2);
    }
  } else {
    noTone(ALARM);   // tắt buzzer
  }

  Serial.print("Nhiet do: ");
  Serial.print(temp);
  Serial.println(" *C");

  delay(1000); 
}
