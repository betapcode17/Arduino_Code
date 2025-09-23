// C++ code
//
void setup()
{
  for(int i = 2; i <=6;i++){
  pinMode(i, OUTPUT);
  }
}

void loop()
{
  for(int i =2 ; i <= 6 ; i++ ){
  digitalWrite(i, HIGH);
  delay(500); // Wait for 1000 millisecond(s)
  digitalWrite(i, LOW);
  delay(500); // Wait for 1000 millisecond(s)
  }
}