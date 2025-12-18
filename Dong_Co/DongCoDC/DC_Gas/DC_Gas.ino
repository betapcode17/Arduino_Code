// ===== CAM BIEN GAS =====
#define GAS_PIN A0
#define GAS_THRESHOLD 200

// ===== DONG CO DC =====
#define enableA 3
#define input1 5
#define input2 6

void setup() {
  Serial.begin(9600);

  pinMode(enableA, OUTPUT);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);

  // Tat dong co luc khoi dong
  analogWrite(enableA, 0);
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);

  Serial.println("Gas + Dong co DC san sang...");
}

void loop() {
  int gasValue = analogRead(GAS_PIN);
  Serial.print("Gas: ");
  Serial.println(gasValue);

  if (gasValue > GAS_THRESHOLD) {
    Serial.println("Gas cao -> Dong co quay Nguoc");

    analogWrite(enableA, 150);     // toc do
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);

  } else {
    Serial.println("Gas thap -> Dong co quay Thuan");

    analogWrite(enableA, 150);
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
  }

  delay(300); // nho, tranh rung
}
