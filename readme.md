Source code used for our avionics - Work in progress.

The source code (very) loosely follows NASA Power of 10 (https://en.wikipedia.org/wiki/The_Power_of_10:_Rules_for_Developing_Safety-Critical_Code)

The code uses arduino specific constructs and the following libraries:
    - TinyGPS++.h
    - HardwareSerial.h
    - Adafruit_Sensor.h
    - Adafruit_ADXL345_U.h
    - Adafruit_BMP280.h

I hope to make this as plaftorm independend as possible and reduce the dependencies in the future.

The code in example.cpp was used in our first testflight for Affinity mk2 (now decomissioned) that was unsuccessful due to a multitude of smaller failiures.

We are currently slowly moving to a new avionics platform and a new vehicle, so I plan on rewriting some of the code here to a nicer and more platform independend state, once the hardware and specifics for our next flight computer are clear.