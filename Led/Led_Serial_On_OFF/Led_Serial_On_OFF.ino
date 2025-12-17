int ledpin = 13;
const byte bufferSize = 20;   
char inputBuffer[bufferSize]; 
byte index = 0;            

void setup() {
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();  

    if (c == '\n' || c == '\r') {
      inputBuffer[index] = '\0'; 

      // So sánh lệnh
      if (strcasecmp(inputBuffer, "on") == 0) {   
        digitalWrite(ledpin, HIGH);
        Serial.println("LED ON");
      } 
      else if (strcasecmp(inputBuffer, "off") == 0) {
        digitalWrite(ledpin, LOW);
        Serial.println("LED OFF");
      } 
      else {
        Serial.println("Lenh khong hop le! Hay nhap 'on' hoac 'off'.");
      }

      index = 0; 
    } 
    else {
      if (index < bufferSize - 1) {  
        inputBuffer[index++] = c;
      }
    }
  }
}
