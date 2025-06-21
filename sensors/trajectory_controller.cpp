#include "trajectory_controller.h"

TrajectoryController::TrajectoryController(ISensor &bmp, ISensor &gps, ISensor &accel)
    : barometricSensor{bmp}, gps{gps}, accelerometer{accel} {}

int TrajectoryController::tick()
{
    barometricSensor.readValue();
    gps.readValue();
    accelerometer.readValue();

    //TODO

    return UNKNOWN;
}