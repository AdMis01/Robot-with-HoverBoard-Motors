import numpy as np
import cv2 as cv

img = cv.imread('testing_camera/testImage.png')
img = cv.resize(img,(0,0), fx=0.75,fy=0.75)
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

corners = cv.goodFeaturesToTrack(gray, 100, 0.01, 10)
corners = np.int8(corners)

for corner in corners:
    x,y = corner.ravel()
    cv.circle(img,(x,y),5,(255,0,0),-1)

cv.imshow('dst', img)
cv.waitKey(0)
cv.destroyAllWindows()
 