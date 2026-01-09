# Robot with HoverBoard Motors

## First configuration and testing

### Testing the hoverboard motors

Connect PIN POWER on the controller ZS-X11H to the pin 9/10 on the Arduino UNO. The GND must be common for both the controller and the microcontroller.

<img width="500" height="343" alt="Bez tytułu1" src="https://github.com/user-attachments/assets/8b790871-e6a6-454b-842a-c018a6ae23aa" />

```c++
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
### Testing the connection and controlling Arduino with Python

#### Arduino UNO code:

Arduino retrieves data that is sent serially from the computer via Python. W - moves forward, S - stops the motors.

```c++
int speedL = 0;
int speedR = 0;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'w':  // Forward
        speedL = 120;
        speedR = 120;
        break;

      case 's':  // Stop
        speedL = 0;
        speedR = 0;
        break;
    }

    analogWrite(9, speedL);
    analogWrite(10, speedR);
  }
}

```
#### Python code 

Python responds to two buttons being pressed (W and S) and sends them to Arduino.

Libraries for the project:

```bash
pip install pyserial opencv-python keyboard
```

```python
import serial
import time
import keyboard
import cv2

arduino = serial.Serial('COM3', 9600) 
time.sleep(2)

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if ret:
        cv2.imshow("Camera", frame)

    if keyboard.is_pressed('w'):
        arduino.write(b'w')
    elif keyboard.is_pressed('s'):
        arduino.write(b's')

    if cv2.waitKey(1) == 27:  # ESC
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
```

### Webcam operation

```
cap = cv2.VideoCapture(0)
ret, frame = cap.read()
if ret:
  cv2.imshow("Camera", frame)
```



## First version (V1)
The components:
- Esp32
- HoverBoard Batery
- HoverBoard Motors
- ZS-X11H

![v1](https://github.com/user-attachments/assets/5183eefd-307e-4d02-b0d3-3eda0b414396)
