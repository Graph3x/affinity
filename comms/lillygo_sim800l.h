#pragma once
#include <HardwareSerial.h>
#include "comms.h"

class LillyGoSIM800L : public ICommunicator
{
public:
    LillyGoSIM800L(int rst = 5, int pwrkey = 4, int powerOn = 23,
                   int tx = 27, int rx = 26, int baudRate = 115200);

    void CheckResponse(String response, String expected = "OK");
    String readLine();
    String waitForLine(long timeout = 5000);

    int powerOn();
    int getStatus();
    int connect();
    int disconnect();
    int HTTPGet(const char *url);
    int connectUDP(const char *ip, const char *port);
    int sendUDP(const uint8_t *data);
    int prepUDP(size_t length);
    int disconnectUDP();

    String ipAddr = "";

private:
    int rstPin;
    int pwrkeyPin;
    int powerOnPin;
    int txPin;
    int rxPin;
    int baudRate;
    HardwareSerial modem = HardwareSerial(1);
    int status = OFFLINE;
    size_t udpLength;
    bool commsDebug = true;
};