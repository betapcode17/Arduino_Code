const int line_sensor = 7;   
const int led_pin = 4;      

void setup() {
  pinMode(line_sensor, INPUT);
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensor_value = digitalRead(line_sensor);

  if (sensor_value == LOW) {
   
    digitalWrite(led_pin, LOW);  
    Serial.println("Black -> LED ON");
  } else {
    
    digitalWrite(led_pin, HIGH);  
    Serial.println("White -> LED OFF");
  }

  delay(200);
}