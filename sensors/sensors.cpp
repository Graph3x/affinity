#include "sensors.h"

int DummyGrowingSensor::readValue(){
    return value++;
}