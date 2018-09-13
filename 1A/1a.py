#!/usr/bin/env python3

import sys
import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import cv2

if len(sys.argv) < 2:
    raise Exception("Usage: ./1a.py <path to image>")

img = cv2.imread(sys.argv[1], cv2.IMREAD_COLOR)

colors = ["b", "g", "r"]

fig = plt.figure(figsize=(8, 3))

for i in range(len(colors)):
    color = colors[i]
    ax = fig.add_subplot('13' + str(i + 1), projection='3d')

    xdata = []
    ydata = []
    zdata = []

    for y in range(0, img.shape[0]):
        for x in range(0, img.shape[1]):
            xdata.append(x)
            ydata.append(y)
            zdata.append(img[y, x, i])

    ax.bar3d(xdata, ydata, np.zeros(len(zdata)), 1, 1, zdata, color=color)
plt.show()


