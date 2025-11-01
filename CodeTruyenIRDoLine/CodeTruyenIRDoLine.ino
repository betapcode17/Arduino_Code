#include <IRremote.hpp>

const int IR_SEND_PIN = 3;               // Pin for sending IR signals
const int FEEDBACK_LED_PIN = 12;         // Optional: Pin for LED feedback

const int DOLINE_DO_PIN = 7;             // Chân D0 của cảm biến line

const unsigned long ON_CODE = 0x1;       // RC5 ON
const unsigned long OFF_CODE = 0x0;      // RC5 OFF

void setup() {
  Serial.begin(9600);
  pinMode(DOLINE_DO_PIN, INPUT);
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, FEEDBACK_LED_PIN);
}

void loop() {
  int lineState = digitalRead(DOLINE_DO_PIN); // Đọc trạng thái line
  Serial.print("Line DO value: ");
  Serial.println(lineState);

  if (lineState == LOW) {   // Line trắng → bật
    IrSender.sendRC5(ON_CODE, 8);
    Serial.println("Sent RC5 ON Code");
  } else {                   // Line đen → tắt
    IrSender.sendRC5(OFF_CODE, 8);
    Serial.println("Sent RC5 OFF Code");
  }

  delay(200); // Đọc liên tục nhưng không quá nhanh
}
