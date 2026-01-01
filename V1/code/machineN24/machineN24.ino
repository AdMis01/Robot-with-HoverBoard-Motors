#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int x;
  int y;
  int s;
  int p;
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
}

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  delay(2000);
}

void loop() {
  Serial.print("x: ");
  Serial.print(incomingData.x);
  Serial.print(" y: ");
  Serial.print(incomingData.y);
  Serial.print(" s: ");
  Serial.print(incomingData.s);
  Serial.print(" p: ");
  Serial.println(incomingData.p);
  int x = incomingData.y;
  int y = incomingData.x;
  int s = incomingData.s;
  int p = incomingData.p;
  int speedP = map(p,0,1023,0,255);
  if (s == 0) {
    digitalWrite(27, HIGH);
    digitalWrite(25, HIGH);
    analogWrite(5, 0);
    analogWrite(16, 0);
    return;
  }
  
  // BREAK OFF
  digitalWrite(27, LOW);
  digitalWrite(25, LOW);
  
  // DIRECTION FORWORD
  if (y > 3000) {
    digitalWrite(26, HIGH);
    digitalWrite(33, HIGH);
  }
  else if (y < 1500) { // DIRECTION BACKWORD
    digitalWrite(26, LOW);
    digitalWrite(33, LOW);
  }
  else { // STOP
    analogWrite(5, 0);
    analogWrite(16, 0);
    return;
  }
  
  int leftSpeed  = speedP;
  int rightSpeed = speedP;
  
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
