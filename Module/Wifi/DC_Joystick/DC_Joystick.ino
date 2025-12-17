#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ===== WIFI =====
const char *ssid = "<3";
const char *password = "baophansayhi";

// ===== PIN =====
#define X_PIN A0
#define KEY_PIN D5

#define MOTOR_PIN1 D1
#define MOTOR_PIN2 D2
#define SPEED_PIN D4   // ⚠️ đổi từ D4 sang D6 (PWM ổn định)

// ===== MOTOR STATE =====
String direction = "Stopped";
int motorSpeed = 0;

// ===== SERVER =====
AsyncWebServer server(80);

// ===== READ INPUT =====
int readX() {
  return analogRead(X_PIN);
}

bool readKey() {
  return digitalRead(KEY_PIN) == LOW;
}

// ===== HTML =====
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DC Motor Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>DC Motor Control</h2>

  <p>Joystick X: <span id="x">0</span></p>
  <p>Direction: <span id="direction">Stopped</span></p>
  <p>Speed: <span id="speed">0</span></p>

  <form id="motorForm">
    <input type="number" name="speed" min="0" max="255" required>
    <input type="submit" value="Set Speed">
  </form>

  <script>
    document.getElementById("motorForm").onsubmit = e => {
      e.preventDefault();
      fetch("/", { method: "POST", body: new FormData(e.target) });
    };

    setInterval(() => {
      fetch("/status")
        .then(r => r.json())
        .then(d => {
          x.innerHTML = d.x;
          direction.innerHTML = d.direction;
          speed.innerHTML = d.speed;
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

// ===== WIFI =====
void initWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: " + WiFi.localIP().toString());
}

// ===== MOTOR CONTROL =====
void updateMotor() {
  if (motorSpeed == 0 || direction == "Stopped") {
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, LOW);
    analogWrite(SPEED_PIN, 0);
    return;
  }

  if (direction == "Clockwise") {
    digitalWrite(MOTOR_PIN1, HIGH);
    digitalWrite(MOTOR_PIN2, LOW);
  } else {
    digitalWrite(MOTOR_PIN1, LOW);
    digitalWrite(MOTOR_PIN2, HIGH);
  }

  analogWrite(SPEED_PIN, motorSpeed);
}

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  pinMode(KEY_PIN, INPUT_PULLUP);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send_P(200, "text/html", index_html);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *req) {
    String json =
      "{\"x\":" + String(readX()) +
      ",\"direction\":\"" + direction +
      "\",\"speed\":" + String(motorSpeed) + "}";
    req->send(200, "application/json", json);
  });

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *req) {
    if (req->hasParam("speed", true)) {
      motorSpeed = req->getParam("speed", true)->value().toInt();
      motorSpeed = constrain(motorSpeed, 0, 255);
      Serial.println("Speed set: " + String(motorSpeed));
    }
    req->send(200, "text/plain", "OK");
  });

  server.begin();
}

// ===== LOOP =====
void loop() {
  static unsigned long t = 0;
  if (millis() - t > 100) {
    t = millis();

    int x = readX();

    if (readKey()) {
      direction = "Stopped";
      motorSpeed = 0;
      Serial.println("Button STOP");
    } else if (x > 700) {
      direction = "Clockwise";
    } else if (x < 300) {
      direction = "Counterclockwise";
    }
  }

  updateMotor();
}
