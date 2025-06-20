#pragma once
#include "sensors.h"

class AltitudeAggregator{
    public:
        AltitudeAggregator(ISensor *bmp, ISensor *gps);
        int getAltitude();
    private:
        ISensor *baromethricSensor;
        ISensor *gps;
};