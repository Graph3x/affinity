#include "sensors.h"

DummyGrowingSensor::DummyGrowingSensor(int growthSpeed, int initValue)
    : growthSpeed{growthSpeed}, value{initValue} {}

int DummyGrowingSensor::readValue()
{
    value *= growthSpeed;
    return value;
}