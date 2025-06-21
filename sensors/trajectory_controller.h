#pragma once
#include "sensors.h"

enum TrajectoryStatus {
    STATIONARY,
    ASCENDING,
    DESCENDING,
    UNKNOWN,
    UNRELIABLE
};

class TrajectoryController{
    public:
        TrajectoryController(ISensor &bmp, ISensor &gps, ISensor &accel);
        int tick();
    private:
        ISensor &barometricSensor;
        ISensor &gps;
        ISensor &accelerometer;
};