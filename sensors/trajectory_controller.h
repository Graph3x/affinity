#pragma once
#include "sensors.h"

// TODO -> constructor
#define CACHE_SIZE 3
#define DECISION_THRESHOLD 0.6

enum TrajectoryStatus
{
    STATIONARY,
    ASCENDING,
    DESCENDING,
    UNKNOWN,
    UNRELIABLE
};


class TrajectoryController
{
public:
    TrajectoryController(ISensor &bmp, ISensor &gps, ISensor &accel);
    int tick();

private:
    ISensor &barometricSensor;
    ISensor &gps;
    ISensor &accelerometer;
    int baroData[CACHE_SIZE] = {0};
    int gpsData[CACHE_SIZE] = {0};
    int accelData[CACHE_SIZE] = {0};
    int currentTick = 0;
};

const char* statusToString(int status);