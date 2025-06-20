Source code used for our avionics - Work in progress.

The source code loosely follows NASA Power of 10 (https://en.wikipedia.org/wiki/The_Power_of_10:_Rules_for_Developing_Safety-Critical_Code)

The code uses arduino specific constructs and the following libraries:
    - TinyGPS++.h
    - HardwareSerial.h
    - Wire.h
    - Adafruit_Sensor.h
    - Adafruit_ADXL345_U.h
    - Adafruit_BMP280.h

I hope to make this as plaftorm independend as possible and reduce the dependencies in the future.
