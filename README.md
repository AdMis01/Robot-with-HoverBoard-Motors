# Robot with HoverBoard Motors

## First version (V1)
The components:
- Esp32
- HoverBoard Batery
- HoverBoard Motors
- ZS-X11H

![v1](https://github.com/user-attachments/assets/5183eefd-307e-4d02-b0d3-3eda0b414396)

### Testing the hoverboard motors

Connect PIN POWER on the controller ZS-X11H to the pin 9/10 on the Arduino UNO. The GND must be common for both the controller and the microcontroller.

<img width="500" height="343" alt="Bez tytułu1" src="https://github.com/user-attachments/assets/8b790871-e6a6-454b-842a-c018a6ae23aa" />

```
void setup() {
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  analogWrite(9,50);
  analogWrite(10,50);
  delay(2000);
  analogWrite(9,0);
  analogWrite(10,0);
  delay(2000);
}
```
