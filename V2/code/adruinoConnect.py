import cv2
import serial
import time
import numpy as np

# ===== SERIAL =====
ser = serial.Serial('COM8', 115200, timeout=1)
time.sleep(2)

# ===== KAMERA =====
cap = cv2.VideoCapture(0)

def decide_from_image(frame):
    h, w, _ = frame.shape

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    edges = cv2.Canny(blur, 50, 150)

    left = np.sum(edges[:, :w//3])
    center = np.sum(edges[:, w//3:2*w//3])
    right = np.sum(edges[:, 2*w//3:])

    # ===== LOGIKA =====
    if center > 500000:
        if left < right:
            return 1000, 3500, 700, 1  # skręt w lewo
        else:
            return 3000, 3500, 700, 1  # skręt w prawo
    else:
        return 2048, 3500, 700, 1     # jedź prosto

while True:
    ret, frame = cap.read()
    if not ret:
        break

    x, y, p, s = decide_from_image(frame)

    cmd = f"X:{x} Y:{y} P:{p} S:{s}\n"
    ser.write(cmd.encode())

    cv2.imshow("Kamera", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
ser.close()
cv2.destroyAllWindows()
