#pragma once
#include <iostream>
#include <HardwareSerial.h>

class ICommunicator
{
public:
    virtual int sendData(String data) = 0;
};

class DummyComms : public ICommunicator
{
public:
    int sendData(String data);
};

class LillyGoSIM800L : public ICommunicator
{
public:
    LillyGoSIM800L(int rst = 5, int pwrkey = 4, int powerOn = 23,
                   int tx = 27, int rx = 26, int baudRate = 115200);

    void waitForResponse();
    String waitForLine();
    int powerOn();
    int getSignalStrength();
    int getStatus();
    int connect();
    int disconnect();
    int sendData(String data);
    int asyncSendData(String data);
    HardwareSerial modem = HardwareSerial(1);

private:
    int rstPin;
    int pwrkeyPin;
    int powerOnPin;
    int txPin;
    int rxPin;
    int baudRate;
};