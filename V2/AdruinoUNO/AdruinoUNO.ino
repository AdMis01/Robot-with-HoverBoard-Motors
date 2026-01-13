int speedL = 0;
int speedR = 0;

unsigned long lastCmdTime = 0;
const unsigned long TIMEOUT = 500;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    int comma = data.indexOf(',');
    if (comma > 0) {
      speedL = data.substring(0, comma).toInt();
      speedR = data.substring(comma + 1).toInt();

      speedL = constrain(speedL, 0, 255);
      speedR = constrain(speedR, 0, 255);

      analogWrite(9, speedL);
      analogWrite(10, speedR);

      lastCmdTime = millis();
    }
  }

  if (millis() - lastCmdTime > TIMEOUT) {
      analogWrite(9, 0);
      analogWrite(10, 0);
  }
}
