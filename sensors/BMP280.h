#pragma once
#include "sensors.h"
#include <Adafruit_BMP280.h>

#define BMP280_I2C_ADRESS (0x76)

class BMP280 : public IBarometer
{

public:
  float readPressure();
  float readTemperature();
  void calibrate();
  float readHeight();

private:
  void tryToStart();
  Adafruit_BMP280 bmp;
  bool started = false;
  float groundAltitude = 0.0;
};