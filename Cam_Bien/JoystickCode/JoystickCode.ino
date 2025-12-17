int pinLed10 = 10; //duoi
int pinLed11 = 11; //tren
int pinLed12 = 12; // trai
int pinLed13 = 13; // phai


int JoyStick_X = 0;
int JoyStick_Y = 1;
int button = 5;


void setup() {
  pinMode(pinLed10, OUTPUT);
  pinMode(pinLed11, OUTPUT);
  pinMode(pinLed12, OUTPUT);
  pinMode(pinLed13, OUTPUT);


  pinMode(JoyStick_X, INPUT);
  pinMode(JoyStick_Y, INPUT);
  pinMode(button, INPUT_PULLUP);


  digitalWrite(pinLed10, HIGH);
    digitalWrite(pinLed11, HIGH);
    digitalWrite(pinLed12, HIGH);
    digitalWrite(pinLed13, HIGH);


  Serial.begin(9600);
}


void loop() {
  int x, y, z;


  x = analogRead(JoyStick_X);  
  y = analogRead(JoyStick_Y);
  z = digitalRead(button);


  Serial.print("X :");
  Serial.print(x, DEC);
  Serial.print(", ");
  Serial.print("Y :");
  Serial.print(y, DEC);
  Serial.print(", ");
  Serial.print("Button :");
  Serial.println(z, DEC);

   //duoi
  if (x < 200 ) {
    digitalWrite(pinLed10, LOW);
  }

   //tren
  if (x > 550) {
    digitalWrite(pinLed11, LOW);
  }

   //trai
  if (y < 500) {
    digitalWrite(pinLed12, LOW);
  }

   //phai
  if (y > 550) {
    digitalWrite(pinLed13, LOW);    
  }


  if (z == 0) {
    digitalWrite(pinLed10, HIGH);
    digitalWrite(pinLed11, HIGH);
    digitalWrite(pinLed12, HIGH);
    digitalWrite(pinLed13, HIGH);
  }
  delay(1000);
}
