#include "gps.h"

void GPS::init(int rx, int tx)
{
    gpsSerial.begin(9600, SERIAL_8N1, rx, tx); // 13, 14
    calibrateHeight();
}

void GPS::parseData()
{
    while (gpsSerial.available())
    {
        int nextChar = gpsSerial.read()
        Serial.print((char)nextChar);
        gpsParser.encode(nextChar);
    }
}

bool GPS::hasLock()
{
    return gpsParser.location.isValid();
}

double GPS::getLat()
{
    if (gpsParser.location.isValid())
    {
        return gpsParser.location.lat();
    }

    return 0;
}

double GPS::getLon()
{
    if (gpsParser.location.isValid())
    {
        return gpsParser.location.lng();
    }

    return 0;
}

void GPS::calibrateHeight()
{
    if (gpsParser.altitude.isValid())
    {
        groundHeight = gpsParser.altitude.meters();
    }
}

double GPS::getHeight()
{
    if (gpsParser.altitude.isValid())
    {
        return gpsParser.altitude.meters();
    }

    return 0;
}

double GPS::getAltitude()
{
    if (gpsParser.altitude.isValid())
    {
        return gpsParser.altitude.meters();
    }

    return 0;
}

uint32_t GPS::satCount()
{
    if (gpsParser.satellites.isValid())
    {
        return gpsParser.satellites.value();
    }
    return 0;
}