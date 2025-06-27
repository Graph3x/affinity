#pragma once
#include "sensors.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

class ADXL345 : public IAccelerometer
{
public:
    struct AccelData getAccel();

private:
    Adafruit_ADXL345_Unified accel;
    bool isSetup = false;
};