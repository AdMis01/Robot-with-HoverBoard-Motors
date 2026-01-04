int x = 2048;
int y = 2048;
int p = 0;
int s = 0;

void setup() {
  Serial.begin(115200);

  pinMode(5, OUTPUT);   // PWM lewy
  pinMode(16, OUTPUT);  // PWM prawy

  pinMode(26, OUTPUT);  // DIR lewy
  pinMode(33, OUTPUT);  // DIR prawy

  pinMode(27, OUTPUT);  // BRAKE lewy
  pinMode(25, OUTPUT);  // BRAKE prawy
}

void loop() {

  // ===== ODBIÓR DANYCH =====
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    sscanf(line.c_str(),
           "X:%d Y:%d P:%d S:%d",
           &x, &y, &p, &s);
  }

  int speedP = map(p, 0, 1023, 0, 255);

  // ===== STOP =====
  if (s == 0) {
    digitalWrite(27, HIGH);
    digitalWrite(25, HIGH);
    analogWrite(5, 0);
    analogWrite(16, 0);
    return;
  }

  // ===== ZWOLNIJ HAMULCE =====
  digitalWrite(27, LOW);
  digitalWrite(25, LOW);

  // ===== KIERUNEK =====
  if (y > 3000) {
    digitalWrite(26, HIGH);
    digitalWrite(33, HIGH);
  }
  else if (y < 1500) {
    digitalWrite(26, LOW);
    digitalWrite(33, LOW);
  }
  else {
    analogWrite(5, 0);
    analogWrite(16, 0);
    return;
  }

  int leftSpeed  = speedP;
  int rightSpeed = speedP;

  // ===== SKRĘT =====
  if (x > 3000) {
    leftSpeed = speedP / 2;
  }
  else if (x < 1500) {
    rightSpeed = speedP / 2;
  }

  analogWrite(5, leftSpeed);
  analogWrite(16, rightSpeed);

  delay(10);
}
