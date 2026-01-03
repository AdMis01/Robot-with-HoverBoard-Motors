import cv2
import numpy as np

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)

    edges = cv2.Canny(blur, 50, 150)

    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    h, w = frame.shape[:2]

    left = center = right = 0

    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > 1000:
            x, y, cw, ch = cv2.boundingRect(cnt)
            cx = x + cw // 2

            if cx < w / 3:
                left += 1
            elif cx < 2 * w / 3:
                center += 1
            else:
                right += 1

            cv2.rectangle(frame, (x, y), (x + cw, y + ch), (0, 255, 0), 2)

    cv2.putText(frame, f"L:{left} C:{center} R:{right}",
                (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    cv2.imshow("FRAME", frame)
    cv2.imshow("EDGES", edges)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
