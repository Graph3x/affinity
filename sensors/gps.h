#pragma once

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPS
{
public:
    void init(int rx, int tx);
    void parseData();
    bool hasLock();
    double getLat();
    double getLon();
    void calibrateHeight();
    double getHeight();
    double getAltitude();
    uint32_t satCount();

private:
    TinyGPSPlus gpsParser;
    HardwareSerial gpsSerial = HardwareSerial(2); //TODO settable
    double groundHeight;

};