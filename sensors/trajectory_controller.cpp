#include "trajectory_controller.h"
#include <iostream>

TrajectoryController::TrajectoryController(ISensor &bmp, ISensor &gps, ISensor &accel)
    : barometricSensor{bmp}, gps{gps}, accelerometer{accel} {}

int TrajectoryController::tick()
{
    baroData[currentTick % CACHE_SIZE] = barometricSensor.readValue();
    gpsData[currentTick % CACHE_SIZE] = gps.readValue();
    accelData[currentTick % CACHE_SIZE] = accelerometer.readValue();

    int accelDir = accelData[((currentTick - 2) % CACHE_SIZE)];
    int baroUp = 0;
    int baroDown = 0;
    int gpsUp = 0;
    int gpsDown = 0;

    for (int i = 0; i < CACHE_SIZE - 1; i++)
    {
        int baroDiff = baroData[((currentTick - i) % CACHE_SIZE)] - baroData[((currentTick - i - 1) % CACHE_SIZE)];
        int gpsDiff = gpsData[((currentTick - i) % CACHE_SIZE)] - gpsData[((currentTick - i - 1) % CACHE_SIZE)];
        accelDir += accelData[((currentTick - i) % CACHE_SIZE)];

        if (baroDiff > 1)
        {
            baroUp++;
        }

        else if (baroDiff < -1)
        {
            baroDown++;
        }

        if (gpsDiff > 1)
        {
            gpsUp++;
        }

        else if (gpsDiff < -1)
        {
            gpsDown++;
        }
    }

    currentTick++;
    if (currentTick < CACHE_SIZE)
    {
        return UNKNOWN;
    }

    int gpsDirr = UNKNOWN;
    int baroDirr = UNKNOWN;
    accelDir = (accelDir > 0) ? ASCENDING : DESCENDING;

    int weightedDS = (CACHE_SIZE - 1) * DECISION_THRESHOLD;

    if (gpsUp - gpsDown > weightedDS)
    {
        gpsDirr = ASCENDING;
    }
    else
    {
        gpsDirr = (gpsDown - gpsUp > weightedDS) ? DESCENDING : STATIONARY;
    }

    if (baroUp - baroDown > weightedDS)
    {
        baroDirr = ASCENDING;
    }
    else
    {
        baroDirr = (baroDown - baroUp > weightedDS) ? DESCENDING : STATIONARY;
    }
    

    if ((baroDirr == gpsDirr) && (baroDirr == STATIONARY))
    {
        return STATIONARY;
    }

    if ((baroDirr == gpsDirr) && (baroDirr == accelDir))
    {
        return baroDirr;
    }

    return UNRELIABLE;
}

const char *statusToString(int status)
{
    switch (status)
    {
    case STATIONARY:
        return "stationary";
        break;

    case ASCENDING:
        return "ascending";
        break;

    case DESCENDING:
        return "descending";
        break;

    case UNKNOWN:
        return "unknown";
        break;

    case UNRELIABLE:
        return "unreliable";
        break;

    default:
        return "invalidStatus";
        break;
    }
};