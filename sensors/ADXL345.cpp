#include "ADXL345.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

struct AccelData ADXL345::getAccel(){

    if(!isSetup){
        accel = Adafruit_ADXL345_Unified();
        isSetup = accel.begin();
    }

    sensors_event_t event;
    accel.getEvent(&event);
    double xd=event.acceleration.x;
    double yd=event.acceleration.y;
    double zd=event.acceleration.z;
    struct AccelData data = {.x=xd, .y=yd, .z=zd};
    return data;
}

