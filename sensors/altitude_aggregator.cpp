#include "altitude_aggregator.h"

AltitudeAggregator::AltitudeAggregator(
    Adafruit_BMP280 *bmp, TinyGPSPlus *gps) : baromethrisSensor { bmp }, gps {gps}
{}
int AltitudeAggregator::getAltitude()
{
    return 69;
}