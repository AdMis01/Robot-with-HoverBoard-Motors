#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int x;
  int y;
  int s;
  int p;
} struct_message;

struct_message joystickData;

uint8_t receiverMAC[] = {0x8C, 0x4F, 0x00, 0xAD, 0x6F, 0x54};

void setup() {
  Serial.begin(115200);
  pinMode(18, INPUT_PULLUP);  
  pinMode(19, INPUT_PULLUP);  
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd inicjalizacji ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("reciver error");
    return;
  }
}

void loop() {
  joystickData.x = analogRead(34);
  joystickData.y = analogRead(35);

  int potValue = analogRead(32);
  joystickData.p = map(potValue,0,4095,0,1023);

  int sw1 = digitalRead(18);
  int sw2 = digitalRead(19);

  if (sw1 == LOW) {
    joystickData.s = 1;
  } else if (sw2 == LOW) {
    joystickData.s = 1;
  } else {
    joystickData.s = 0;
  }
  
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *) &joystickData, sizeof(joystickData));

  if (result == ESP_OK) {
    Serial.print("x: ");
    Serial.print(joystickData.x);
    Serial.print(" y: ");
    Serial.print(joystickData.y);
    Serial.print("s: ");
    Serial.print(joystickData.s);
    Serial.print(" p: ");
    Serial.println(joystickData.p);
  } else {
    Serial.println("sending error");
  }

  delay(100);
}
