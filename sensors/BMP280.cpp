#include "BMP280.h"
#include <Adafruit_BMP280.h>

void BMP280::tryToStart(){
    if(!started){
        started = bmp.begin(BMP280_I2C_ADRESS);
    }
}

float BMP280::readPressure()
{
    tryToStart();
    return bmp.readPressure() / 100.0;
}

float BMP280::readTemperature()
{
    tryToStart();
    return bmp.readTemperature();
}

float BMP280::readHeight()
{
    tryToStart();
    return bmp.readAltitude() - groundAltitude;
}

void BMP280::calibrate()
{
    tryToStart();
    groundAltitude = bmp.readAltitude();
}
