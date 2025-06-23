#include "sensors.h"

DummyArraySensor::DummyArraySensor(int *data, int dataSize)
    : data{data}, dataSize{dataSize} {}

int DummyArraySensor::readValue()
{
    current++;
    return data[current % dataSize];
}