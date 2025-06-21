#pragma once
#include <HardwareSerial.h>
#include "comms.h"

class LillyGoSIM800L : public ICommunicator
{
public:
    LillyGoSIM800L(int rst = 5, int pwrkey = 4, int powerOn = 23,
                   int tx = 27, int rx = 26, int baudRate = 115200);

    void waitForResponse();
    String waitForLine();
    int powerOn();
    int getStatus();
    int connect();
    int disconnect();
    int HTTPGet(const char* url);

private:
    int rstPin;
    int pwrkeyPin;
    int powerOnPin;
    int txPin;
    int rxPin;
    int baudRate;
    HardwareSerial modem = HardwareSerial(1);
    int status = OFFLINE;
};