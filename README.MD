# NANOSSOC A60 Calibration System Code (in development)
Current features:
1. `FineSunSensor.ino` can be compiled on an Arduino Due to read the sun sensor, then calculate the angles and plot using serial plotter.
1. `sunsensor_server/sunsensor_server.ino` can be compiled on an Arduino Uno to read the sensor at 10 Hz and transmit that data over serial to `sunsensor.py` which takes care of conversion and plotting the data using a C library that can be compiled using `make`.