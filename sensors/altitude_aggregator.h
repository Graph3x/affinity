#pragma once
#include <Adafruit_BMP280.h>
#include <TinyGPS++.h>


class AltitudeAggregator{
    public:
        AltitudeAggregator(Adafruit_BMP280 *bmp, TinyGPSPlus *gps);
        getAltitude();
    private:
        Adafruit_BMP280 *baromethrisSensor;
        TinyGPSPlus *gps;
}