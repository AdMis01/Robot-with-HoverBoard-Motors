import serial
import time
import cv2
import numpy as np

arduino = serial.Serial('COM3', 9600)
time.sleep(2)

def open_camera():
    cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    return cap

cap = open_camera()

BASE_SPEED = 130
SLOW_SPEED = 70
TURN_DIFF = 60
OBSTACLE_THRESHOLD = 200000

print("AUTO MODE STARTED")

def autonomous_control(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (7, 7), 0)
    _, thresh = cv2.threshold(blur, 70, 255, cv2.THRESH_BINARY_INV)

    h, w = thresh.shape
    left = thresh[:, :w // 3]
    center = thresh[:, w // 3:2 * w // 3]
    right = thresh[:, 2 * w // 3:]

    left_sum = np.sum(left)
    center_sum = np.sum(center)
    right_sum = np.sum(right)

    speedL = BASE_SPEED
    speedR = BASE_SPEED
    obstacle = center_sum > OBSTACLE_THRESHOLD

    if obstacle:
        speedL = SLOW_SPEED
        speedR = SLOW_SPEED

        if left_sum < right_sum:
            speedL -= TURN_DIFF
            speedR += TURN_DIFF
        else:
            speedL += TURN_DIFF
            speedR -= TURN_DIFF

    speedL = max(0, min(255, speedL))
    speedR = max(0, min(255, speedR))

    return speedL, speedR, thresh, obstacle

try:
    while True:
        ret, frame = cap.read()

        if not ret:
            print("Camera error -> STOP")
            arduino.write(b"0,0\n")
            cap.release()
            time.sleep(1)
            cap = open_camera()
            continue

        speedL, speedR, thresh, obstacle = autonomous_control(frame)

        cmd = f"{speedL},{speedR}\n"
        arduino.write(cmd.encode())

        color = (0, 0, 255) if obstacle else (0, 255, 0)
        cv2.putText(frame, f"L:{speedL} R:{speedR}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)

        cv2.imshow("Camera", frame)
        cv2.imshow("Threshold", thresh)

        if cv2.waitKey(1) == 27:  # ESC
            break

finally:
    print("STOP MOTORS")
    arduino.write(b"0,0\n")
    time.sleep(0.2)

    cap.release()
    cv2.destroyAllWindows()
    arduino.close()
