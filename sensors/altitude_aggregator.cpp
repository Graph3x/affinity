#include "altitude_aggregator.h"

AltitudeAggregator::AltitudeAggregator(
    ISensor *bmp, ISensor *gps) : baromethricSensor { bmp }, gps {gps}
{}
int AltitudeAggregator::getAltitude()
{
    return (baromethricSensor->readValue() + gps->readValue()) / 2;
}