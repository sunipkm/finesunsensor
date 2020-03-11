# %%
#!/usr/bin/env python3
import ctypes
import serial
import numpy as np
import matplotlib.pyplot as plt
import sys
import signal
from collections import deque
import matplotlib.animation as matanim

# %%
# load library
sensorlib = ctypes.CDLL('./libsunsensor.so')

# %%
# register the function
get_angle = sensorlib.get_angle
def getAngle(val):
    dest = np.zeros(2, dtype=np.float)
    err = get_angle(ctypes.c_void_p(dest.ctypes.data), ctypes.c_void_p(val.ctypes.data))
    return (dest, err)
# %%
# set up serial
if len(sys.argv) != 2:
    print("Invocation: sudo ./sunsensor.py /dev/<serial device>")
    sys.exit(0)

s = serial.Serial(sys.argv[1], 115200) # open serial with 115200 bps speed
# %%
# set up figure
fig, ax = plt.subplots(figsize = (10, 5))
angleX = deque(maxlen = 100)
angleY = deque(maxlen = 100)

for i in range(100):
    angleX.append(0.)
    angleY.append(0.)

lx, = ax.plot(angleX, color = 'r', label="Angle X", animated = True)
ly, = ax.plot(angleY, color = 'b', label="Angle Y", animated = True)

ax.legend()

ax.set_ylim(-60, 60)
ax.set_xlim(0,100)

lines = [lx, ly]

xaxis = np.array([i for i in range(100)])

def animate(framecount):
    global lines, angleX, angleY
    line = s.readline().decode('utf-8')
    words = line.rstrip('\n').split(',')
    vals = [np.uint16(w) for w in words]
    vals = np.array(vals, dtype = np.uint16)
    vals, err = getAngle(vals)
    angleX.append(vals[0])
    angleY.append(vals[1])
    lines[0].set_data(xaxis, angleX)
    lines[1].set_data(xaxis, angleY)
    return lines


# %%
anim = matanim.FuncAnimation(fig, animate, blit=True)
plt.show()

# %%
